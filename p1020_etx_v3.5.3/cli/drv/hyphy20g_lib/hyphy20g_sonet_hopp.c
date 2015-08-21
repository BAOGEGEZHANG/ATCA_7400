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
**  FILE        : hyphy20g_sonet_hopp.c
**
**  $Date: 2011-11-17 17:04:59 +0800 (Thu, 17 Nov 2011) $
**
**  $Revision: 17155 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/


/** include files **/
#include "hyphy20g_api_wrapper.h"
#include    "hyphy20g_sonet_hopp.h"

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
**  hyPhy20gHoppDeviceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the HOPP subsystem as a whole to be
**                  in low power - power savings mode or out of low power mode
**                  and ready for individual slice configuration.
**
**                  This function impacts all 4 HOPP slices.  It is the
**                  caller's responsibility to ensure that this function is not
**                  called with enable = 0 while a slice is intended to be
**                  processing user data.
**
**                  This function must be called prior to calling
**                  hyPhy20gHoppSliceInit to enable a HOPP slice for a desired
**                  mode.
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - Operation Mode of HOPP Subsystem
**                               0 - Subsystem Disabled - low power modes
**                               1 - Subsystem enabled and reset to default
**                                   values
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppDeviceInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure top level lowpwr bit */
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_1,
                                 HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF,
                                 0);
        if(result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_1,
                                 HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF,
                                 1);
        if(result) return result;
    }

    /* reset at top level */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_1,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF,
                                 1);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_1,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF,
                                 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatHoppInit */

/*******************************************************************************
**
**  hyPhy20gHoppSliceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a SW Reset of the HOPP block for one
**                 or more slices.  This function should be called before
**                 configuring the HOPP block for any SONET link.
**
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice instance
**                               0 - Slice 0
**                               1 - Slice 1
**                               2 - Slice 2
**                               3 - Slice 3
**                               4 - All 4 slices
**                 enable      - 0 - HOPP Subsystem disabled - low power mode
**                               1 - HOPP Subsystem enabled
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppSliceInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{

    INT4 result;
    UINT4 offset;
    UINT4 regAddr;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 i,j;
    UINT4 txCid;


    /* argument checking */
    if ((slice > 4)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

	if (slice < 4) {
	    /* Reset 1 slice */
	    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
      regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

     	if (enable) {
		      result = sysHyPhy20gBitWrite(fileHndl, regAddr,
		                HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF,
		                                1);
			    if (result) return result;

		      result = sysHyPhy20gBitWrite(fileHndl, regAddr,
		      					HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF,
		                            		0);
		      if (result) return result;

					for (j = 0; j<10; j++) {
				  		result = sysHyPhy20gBitWrite(fileHndl, regAddr,
				           		HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF,
				                                1);
				      if (result) return result;

				      result = sysHyPhy20gBitWrite(fileHndl, regAddr,
				      		    HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF,
				                                0);
				      if (result) return result;

              result = hoppHyPhy20gTTERBIndWrite(fileHndl, slice, 0, 0, 0, 0x5430 + slice);
              if (result) return result;
              result = hoppHyPhy20gTTERBIndRead(fileHndl, slice, 0, 0, 0, &txCid);
              if (result) return result;
              if (txCid == (0x5430 + slice))
                				j = 100;
        	}
          if (j!= 101)
          		return HYPHY20G_ERR_DEV_ACCESS;
		  } else {
		      result = sysHyPhy20gBitWrite(fileHndl, regAddr,
		                HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF,
		                                1);
			    if (result) return result;
		  		result = sysHyPhy20gBitWrite(fileHndl, regAddr,
			           		HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF,
				                                1);
		      if (result) return result;
			}

    } else {
	    /* Reset all 4 slices */
        for (i = 0; i<4; i++) {
            regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            if (enable) {
            		result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                		HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF,
                                    		1);
            		if (result) return result;
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
               			HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF,
                                        0);
                if (result) return result;

								for (j = 0; j<10; j++) {
				  					result = sysHyPhy20gBitWrite(fileHndl, regAddr,
				           		HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF,
				                                1);
				      			if (result) return result;

				      			result = sysHyPhy20gBitWrite(fileHndl, regAddr,
				      		    HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF,
				                                0);
				      			if (result) return result;

              			result = hoppHyPhy20gTTERBIndWrite(fileHndl, i, 0, 0, 0, 0x5430 + i);
              			if (result) return result;
              			result = hoppHyPhy20gTTERBIndRead(fileHndl, i, 0, 0, 0, &txCid);
              			if (result) return result;
              			if (txCid == (0x5430 + i))
                				j = 100;
        				}
          			if (j!= 101)
          				return HYPHY20G_ERR_DEV_ACCESS;
            } else {
		      			result = sysHyPhy20gBitWrite(fileHndl, regAddr,
		                HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF,
		                                1);
			    			if (result) return result;
		  					result = sysHyPhy20gBitWrite(fileHndl, regAddr,
			           		HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF,
				                                1);
		      			if (result) return result;

        		}
    		}
    }
   return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSliceInit

/*******************************************************************************
**
**  hyPhy20gHoppSts192LinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the 4 slices within the HOPP
**                 to be grouped together for processing of an STS-192c signal.
**
**  INPUTS:        *fileHndl   - base address
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppSts192LinkCfg(struct file *fileHndl)
{

	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceRxRst, sliceTxRst;
	UINT4 slice0RxCfg1, slice0RxCfg2, slice0RxCfg3;
	UINT4 slice123RxCfg1, slice123RxCfg2, slice123RxCfg3;
	UINT4 slice0TxCfg1, slice0TxCfg2, slice0TxCfg3;
	UINT4 slice123TxCfg1, slice123TxCfg2, slice123TxCfg3;
	UINT4 i;

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    for (i = 0; i<HYPHY20G_HOPP_MAX_SLICES; i++) {
        regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
        regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);

        offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
        if (result) return result;
        if (sliceRxRst) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
        if (result) return result;
        if (sliceTxRst) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }



    /* Create Tx Payload and Rx Payload settings for 4 slices */
 	slice0RxCfg1 = 0x1;
	slice0RxCfg2 = 0x0;
	slice0RxCfg3 = 0x0;

	slice123RxCfg1 = 0x0;
	slice123RxCfg2 = 0x0;
	slice123RxCfg3 = 0x0;

	slice0TxCfg1 = 0x1;
	slice0TxCfg2 = 0x0;
	slice0TxCfg3 = 0x0;

	slice123TxCfg1 = 0x0;
	slice123TxCfg2 = 0x0;
	slice123TxCfg3 = 0x0;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 1);
    if (result) return result;


    /*Configure HOPP Slice Rx Payload for all 4 slices */
    for (i = 0; i<HYPHY20G_HOPP_MAX_SLICES; i++) {
        if (i==0) {
        /* First slice config */
            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0RxCfg1);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0RxCfg2);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0RxCfg3);
            if (result) return result;

        } else {
        /* Remaining 3 slices config */
            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123RxCfg1);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123RxCfg2);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123RxCfg3);
            if (result) return result;
        }
    }

    /* Configure HOPP Slice Tx Payload for all 4 slices */
    for (i = 0; i<HYPHY20G_HOPP_MAX_SLICES; i++) {
        if (i==0) {
        /* First slice config */
            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0TxCfg1);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0TxCfg2);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0TxCfg3);
            if (result) return result;

        } else {
        /* Remaining 3 slices config */
            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123TxCfg1);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123TxCfg2);
            if (result) return result;

            regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
            regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123TxCfg3);
            if (result) return result;
        }
    }
	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts192LinkCfg

/*******************************************************************************
**
**  hyPhy20gHoppSts48LinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures one ofthe 4 OC-48 rate slices within
**                 the HOPP subsystem to process an STS-48c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppSts48LinkCfg(struct file *fileHndl, UINT4 slice)
{

	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceRxRst, sliceTxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }


    /* Create Tx Payload and Rx Payload settings */
 	RxCfg1 = 0x1;
	RxCfg2 = 0x0;
	RxCfg3 = 0x0;

	TxCfg1 = 0x1;
	TxCfg2 = 0x0;
	TxCfg3 = 0x0;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 slice */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;

    /* Configure HOPP Slice Tx Payload for 1 slice */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts48LinkCfg

/*******************************************************************************
**
**  hyPhy20gHoppSts24LinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/2 of one of the 4 OC-48 rate
**                 slices within the HOPP subsystem to process an STS-24c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts24Num    - selects one of the 2 possible STS-24c channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-1)
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
INT4 hyPhy20gHoppSts24LinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts24Num)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceRxRst, sliceTxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xFF << 8*sts24Num;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 STS-24c link */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;


    /* Configure HOPP Slice Tx Payload for 1 STS-24c link  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts24LinkCfg

/*******************************************************************************
**
**  hyPhy20gHoppSts12LinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/4 of one of the 4 OC-48 rate
**                 slices within the HOPP subsystem to process an STS-12c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts12Num    - selects one of the 4 possible STS-12c channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-3)
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
INT4 hyPhy20gHoppSts12LinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts12Num)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceRxRst, sliceTxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xF << 4*sts12Num;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 STS-12c link */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;


    /* Configure HOPP Slice Tx Payload for 1 STS-12c link  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts12LinkCfg

/*******************************************************************************
**
**  hyPhy20gHoppSts3LinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/16 of one of the 4 OC-48 rate
**                 slices within the HOPP subsystem to process an STS-3 or
**                 STS-3c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts12Num    - selects one of the 4 possible STS-12 channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-3)
**                 sts3Num     - selects one of the 4 possible STS-3 channels
**															 within the STS-12 specified by sts12Num
**                             - valid values (0-3)
**                 concat      - selects concatenation mode for the STS-3 link
**                             - 1 - STS-3c link
**                             - 0 - 3xSTS1 link
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
INT4 hyPhy20gHoppSts3LinkCfg(struct file *fileHndl, UINT4 slice, UINT4 sts12Num,
                             UINT4 sts3Num, UINT4 concat)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceRxRst, sliceTxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 sts3Off;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)||(sts3Num > 3)||(concat > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }


    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    sts3Off = 4*sts12Num + sts3Num;
    mask = 0x1 << sts3Off;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 STS-3 link */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;


    /* Configure HOPP Slice Tx Payload for 1 STS-3 link  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts3LinkCfg

/*******************************************************************************
**
**  hyPhy20gHoppSts24LinkUnprov
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function unprovisions an STS-24c signal of one of the
**                  4 OC-48 rate slices within the HOPP subsystem.
**
**                  This function configures all 24 STS-1 timeslots of the
**                  specified STS-24c signal as master.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - HOPP slice to unprovision
**                              - valid values (0-3)
**                  sts24Num    - selects one of the 2 possible STS-24c channels
**                                within the STS-48 rate output for the slice
**                              - valid values (0-1)
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
INT4 hyPhy20gHoppSts24LinkUnprov(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts24Num)
{
	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceRxRst, sliceTxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xFF << 8*sts24Num;

    /*Configure HOPP Slice Rx Payload for 24 x STS-1 links */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    /* Configure all 8 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    /* Configure all 8 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    /* Configure all 8 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;


    /* Configure HOPP Slice Tx Payload for 24 x STS-1 links  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    /* Configure all 8 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    /* Configure all 8 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    /* Configure all 8 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppSts24LinkUnprov */

/*******************************************************************************
**
**  hyPhy20gHoppSts12LinkUnprov
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function unprovisions an STS-12c signal of one of the
**                  4 OC-48 rate slices within the HOPP subsystem.
**
**                  This function configures all 12 STS-1 timeslots of the
**                  specified STS-12c signal as master.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - HOPP slice to configure
**                              - valid values (0-3)
**                  sts12Num    - selects one of the 4 possible STS-12c channels
**                                within the STS-48 rate output for the slice
**                              - valid values (0-3)
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
INT4 hyPhy20gHoppSts12LinkUnprov(struct file *fileHndl, UINT4 slice,
							     UINT4 sts12Num)
{

	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceRxRst, sliceTxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xF << 4*sts12Num;

    /* Configure HOPP Slice Rx Payload for 12 x STS-1 links */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    /* Configure all 4 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    /* Configure all 4 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    /* Configure all 4 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;


    /* Configure HOPP Slice Tx Payload for 12 x STS-1 links  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    /* Configure all 4 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    /* Configure all 4 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    /* Configure all 4 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppSts12LinkUnprov */

/*******************************************************************************
**
**  hyPhy20gHoppSts48TxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures one ofthe 4 OC-48 rate Tx slices within
**                 the HOPP subsystem to process an STS-48c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppSts48TxLinkCfg(struct file *fileHndl, UINT4 slice)
{

	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceTxRst;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }


    /* Create Tx Payload and Rx Payload settings */
	TxCfg1 = 0x1;
	TxCfg2 = 0x0;
	TxCfg3 = 0x0;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /* Configure HOPP Slice Tx Payload for 1 slice */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts48TxLinkCfg

/*******************************************************************************
**
**  hyPhy20gHoppSts48RxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures one ofthe 4 OC-48 rate Rx slices within
**                 the HOPP subsystem to process an STS-48c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppSts48RxLinkCfg(struct file *fileHndl, UINT4 slice)
{

	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceRxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }


    /* Create Tx Payload and Rx Payload settings */
 	RxCfg1 = 0x1;
	RxCfg2 = 0x0;
	RxCfg3 = 0x0;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 slice */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts48RxLinkCfg



/*******************************************************************************
**
**  hyPhy20gHoppSts24TxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/2 of one of the 4 OC-48 rate
**                 Tx slices within the HOPP subsystem to process an STS-24c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts24Num    - selects one of the 2 possible STS-24c channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-1)
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
INT4 hyPhy20gHoppSts24TxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts24Num)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceTxRst;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xFF << 8*sts24Num;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /* Configure HOPP Slice Tx Payload for 1 STS-24c link  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts24TxLinkCfg


/*******************************************************************************
**
**  hyPhy20gHoppSts24RxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/2 of one of the 4 OC-48 rate
**                 Rx slices within the HOPP subsystem to process an STS-24c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts24Num    - selects one of the 2 possible STS-24c channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-1)
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
INT4 hyPhy20gHoppSts24RxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts24Num)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceRxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    
    mask = 0xFF << 8*sts24Num;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 STS-24c link */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts24RxLinkCfg



/*******************************************************************************
**
**  hyPhy20gHoppSts12TxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/4 of one of the 4 OC-48 rate
**                 Tx slices within the HOPP subsystem to process an STS-12c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts12Num    - selects one of the 4 possible STS-12c channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-3)
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
INT4 hyPhy20gHoppSts12TxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts12Num)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceTxRst;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xF << 4*sts12Num;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /* Configure HOPP Slice Tx Payload for 1 STS-12c link  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts12TxLinkCfg


/*******************************************************************************
**
**  hyPhy20gHoppSts12RxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/4 of one of the 4 OC-48 rate
**                 Rx slices within the HOPP subsystem to process an STS-12c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts12Num    - selects one of the 4 possible STS-12c channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-3)
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
INT4 hyPhy20gHoppSts12RxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts12Num)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceRxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xF << 4*sts12Num;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 STS-12c link */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts12RxLinkCfg


/*******************************************************************************
**
**  hyPhy20gHoppSts3TxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/16 of one of the 4 OC-48 rate
**                 Tx slices within the HOPP subsystem to process an STS-3 or
**                 STS-3c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts12Num    - selects one of the 4 possible STS-12 channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-3)
**                 sts3Num     - selects one of the 4 possible STS-3 channels
**															 within the STS-12 specified by sts12Num
**                             - valid values (0-3)
**                 concat      - selects concatenation mode for the STS-3 link
**                             - 1 - STS-3c link
**                             - 0 - 3xSTS1 link
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
INT4 hyPhy20gHoppSts3TxLinkCfg(struct file *fileHndl, UINT4 slice, UINT4 sts12Num,
                             UINT4 sts3Num, UINT4 concat)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceTxRst;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 sts3Off;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)||(sts3Num > 3)||(concat > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }


    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    sts3Off = 4*sts12Num + sts3Num;
    mask = 0x1 << sts3Off;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /* Configure HOPP Slice Tx Payload for 1 STS-3 link  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts3TxLinkCfg


/*******************************************************************************
**
**  hyPhy20gHoppSts3RxLinkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures 1/16 of one of the 4 OC-48 rate
**                 Rx slices within the HOPP subsystem to process an STS-3 or
**                 STS-3c signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - HOPP slice to configure
**                             - valid values (0-3)
**                 sts12Num    - selects one of the 4 possible STS-12 channels
**															 within the STS-48 rate output for the slice
**                             - valid values (0-3)
**                 sts3Num     - selects one of the 4 possible STS-3 channels
**															 within the STS-12 specified by sts12Num
**                             - valid values (0-3)
**                 concat      - selects concatenation mode for the STS-3 link
**                             - 1 - STS-3c link
**                             - 0 - 3xSTS1 link
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
INT4 hyPhy20gHoppSts3RxLinkCfg(struct file *fileHndl, UINT4 slice, UINT4 sts12Num,
                             UINT4 sts3Num, UINT4 concat)
{

	INT4 result;
	UINT4 regAddr;
  UINT4 offset;
  UINT4 topLvlRst, topLvlPwr;
  UINT4 sliceRxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 sts3Off;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)||(sts3Num > 3)||(concat > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }


    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    sts3Off = 4*sts12Num + sts3Num;
    mask = 0x1 << sts3Off;

    /* Configure QUAD48_SONET bit */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_GPIO;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                        HYPHY20G_HOPP_TOP_BIT_SLC_GPIO_QUAD48_SONET_OFF, 0);
    if (result) return result;

    /*Configure HOPP Slice Rx Payload for 1 STS-3 link */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} //hyPhy20gHoppSts3RxLinkCfg


/*******************************************************************************
**
**  hyPhy20gHoppSts24TxLinkUnprov
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function unprovisions an STS-24c signal of one of the
**                  4 OC-48 rate Tx slices within the HOPP subsystem.
**
**                  This function configures all 24 STS-1 timeslots of the
**                  specified STS-24c signal as master.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - HOPP slice to unprovision
**                              - valid values (0-3)
**                  sts24Num    - selects one of the 2 possible STS-24c channels
**                                within the STS-48 rate output for the slice
**                              - valid values (0-1)
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
INT4 hyPhy20gHoppSts24TxLinkUnprov(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts24Num)
{
	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceTxRst;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xFF << 8*sts24Num;

    /* Configure HOPP Slice Tx Payload for 24 x STS-1 links  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    /* Configure all 8 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    /* Configure all 8 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    /* Configure all 8 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppSts24TxLinkUnprov */


/*******************************************************************************
**
**  hyPhy20gHoppSts24RxLinkUnprov
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function unprovisions an STS-24c signal of one of the
**                  4 OC-48 rate Rx slices within the HOPP subsystem.
**
**                  This function configures all 24 STS-1 timeslots of the
**                  specified STS-24c signal as master.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - HOPP slice to unprovision
**                              - valid values (0-3)
**                  sts24Num    - selects one of the 2 possible STS-24c channels
**                                within the STS-48 rate output for the slice
**                              - valid values (0-1)
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
INT4 hyPhy20gHoppSts24RxLinkUnprov(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts24Num)
{
	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceRxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xFF << 8*sts24Num;

    /*Configure HOPP Slice Rx Payload for 24 x STS-1 links */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    /* Configure all 8 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    /* Configure all 8 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    /* Configure all 8 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0xFF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppSts24RxLinkUnprov */

/*******************************************************************************
**
**  hyPhy20gHoppSts12TxLinkUnprov
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function unprovisions an STS-12c signal of one of the
**                  4 OC-48 rate Tx slices within the HOPP subsystem.
**
**                  This function configures all 12 STS-1 timeslots of the
**                  specified STS-12c signal as master.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - HOPP slice to configure
**                              - valid values (0-3)
**                  sts12Num    - selects one of the 4 possible STS-12c channels
**                                within the STS-48 rate output for the slice
**                              - valid values (0-3)
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
INT4 hyPhy20gHoppSts12TxLinkUnprov(struct file *fileHndl, UINT4 slice,
							     UINT4 sts12Num)
{

	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceTxRst;
	UINT4 TxCfg1, TxCfg2, TxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceTxRst);
    if (result) return result;
    if (sliceTxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xF << 4*sts12Num;

    /* Configure HOPP Slice Tx Payload for 12 x STS-1 links  */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg1);
    if (result) return result;

    /* Configure all 4 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg1, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg2);
    if (result) return result;

    /* Configure all 4 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg2, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &TxCfg3);
    if (result) return result;

    /* Configure all 4 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&TxCfg3, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, TxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppSts12TxLinkUnprov */

/*******************************************************************************
**
**  hyPhy20gHoppSts12RxLinkUnprov
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function unprovisions an STS-12c signal of one of the
**                  4 OC-48 rate Rx slices within the HOPP subsystem.
**
**                  This function configures all 12 STS-1 timeslots of the
**                  specified STS-12c signal as master.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - HOPP slice to configure
**                              - valid values (0-3)
**                  sts12Num    - selects one of the 4 possible STS-12c channels
**                                within the STS-48 rate output for the slice
**                              - valid values (0-3)
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
INT4 hyPhy20gHoppSts12RxLinkUnprov(struct file *fileHndl, UINT4 slice,
							     UINT4 sts12Num)
{

	INT4 result;
	UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst, topLvlPwr;
    UINT4 sliceRxRst;
	UINT4 RxCfg1, RxCfg2, RxCfg3;
	UINT4 mask;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHO_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHO_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_HOPP_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_HOPP_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sliceRxRst);
    if (result) return result;
    if (sliceRxRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = 0xF << 4*sts12Num;

    /* Configure HOPP Slice Rx Payload for 12 x STS-1 links */
    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg1);
    if (result) return result;

    /* Configure all 4 STS3_1 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg1, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg1);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg2);
    if (result) return result;

    /* Configure all 4 STS3_2 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg2, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg2);
    if (result) return result;

    regAddr = HYPHY20G_HOPP_TOP_REG_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &RxCfg3);
    if (result) return result;

    /* Configure all 4 STS3_3 timeslots as master */
    result = hyPhy20gVarFieldWrite(&RxCfg3, mask, 0xF);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, RxCfg3);
    if (result) return result;

	return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppSts12RxLinkUnprov */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
