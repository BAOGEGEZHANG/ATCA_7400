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
**  FILE        : hyphy20g_sonet.c
**
**  $Date: 2013-01-17 19:35:54 +0800 (Thu, 17 Jan 2013) $
**
**  $Revision: 30563 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/


/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_sonet.h"

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
**  hyPhy20gSonetInit
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function resets one or all of the 16 2.5G SONET 
**                 processing resources within the SONET subsystem, and sets
**                 all timeslots within the slice to be configured manually
**                 as master timeslots.
**
**                 This function performs a top level reset of the SONET 
**                 processing resource, as well as a block level reset.
**
**                 One SONET slice will process an STS-48, STS-12 or STS-3 rate
**                 signal transferred from the SONET_LINE subsystem.
**
**                 For an STS-192 rate client signal, four adjacent SONET slices 
**                 (0-3, 4-7, 8-11, or 12-15) will be grouped together to 
**                 process the signal.
** 
**                 
**
**  INPUTS:       *fileHndl   - base address
**                slice       - SONET slice processing resource
**                            - 0 to 15 - reset 1 framer
**                            - 16 - reset all 16 framers
**                enable      - 0 - SONET Slice disabled - held in low power
**                            - 1 - SONET slice enabled
**                            - 2 - SONET slice enabled for POS OC-192
**                              loop timing application
**
**
**  OUTPUTS:       None.
**
**  NOTE:         This function must be called to reset the SONET subsystem when
**                either the line side SerDes is reconfigured or the SONET Line 
**                configuration is changed.  The required configuration sequence 
**                is: 
**                  1) SerDes configuration.
**                  2) SONET Line configuration. 
**                  3) SONET subsystem reset [i.e. call this function with 
**                     'enable' = 1], 4) SONET slice configuration.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 mask;
    UINT4 i, j;
    UINT4 txCid;
    UINT4 val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (revId != HYPHY20G_REV_ID_D && enable == 2)
        return HYPHY20G_ERR_INCOMPATIBLE_DEVICE;

    /* argument checking */
    if ((slice > 16)||(enable > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    
    if (slice == 16) {
        /* Configure top level reset for all 16 SONET slices */
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
        mask = 0xFFFF << HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF;
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xFFFF);
        if (result) return result;
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x0);
        if (result) return result;
        
        /* Apply Low Power settings */    
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        mask = 0xFFFF << HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF;
        if (enable) {
            /* remove low power bit for all 16 SONET slices */
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x0);            
            if (result) return result;
        } else {
            /* set low power bit for all 16 SONET slices */
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xFFFF);            
            if (result) return result;
        }        

        if (enable) {
            /* reset all 16 slices at block level */
            for(i = 0; i<slice; i++) {
                regAddr = HYPHY20G_SONET_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);   

				/* reset all 16 Rx slices at block level and verify that RTERB 
				blocks indirect access is working correctly */
				for (j = 0; j<10; j++) {
		            offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
                	result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
                	if (result) return result;

                	offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
                	result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
                	if (result) return result;
                	
                	result = sonetHyPhy20gRTERBIndWrite(fileHndl, i, 0, 0, 0, 0x5430 + i);
                	if (result) return result;
                	result = sonetHyPhy20gRTERBIndRead(fileHndl, i, 0, 0, 0, &txCid);
                	if (result) return result;
                	if (txCid == (0x5430 + i)) 
                	    j = 100;
                }
                if (j!= 101)
                	return HYPHY20G_ERR_DEV_ACCESS;
                		
				/* reset all 16 Tx slices at block level  */
			    offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
                if (result) return result;

                offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
                if (result) return result;
                	         
            }
            /* Configure all timeslots of all 16 slices as Master timeslots */
            for(i = 0; i<slice; i++) {
                regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;        

                regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;        

                regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;        

                regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;        

                regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;        

                regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result; 
                    
                /* Configure bits to enable fast LOS detection in SONET framer */
                regAddr = HYPHY20G_SONET_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gRead(fileHndl, regAddr, &val);
                if (result) return result;

                mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_LOS_DET_EN_MSK;
                result = hyPhy20gVarFieldWrite (&val, mask, 1);
                if (result) return result;

                mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_LOS_EN_MSK;
                result = hyPhy20gVarFieldWrite (&val, mask, 1);
                if (result) return result;

                mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_SARC_AISINS_EN_MSK;
                result = hyPhy20gVarFieldWrite (&val, mask, 1);
                if (result) return result;

                mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_LOS_EARLY_DECLARE_EN_MSK;
                result = hyPhy20gVarFieldWrite (&val, mask, 1);
                if (result) return result;

                result = sysHyPhy20gWrite(fileHndl, regAddr, val);
                if (result) return result;       
            }
            
            if (enable == 2) {
                for(i = 0; i<slice; i++) {
                    regAddr = HYPHY20G_SONET_RTOHT_REG_CFG_2;
                    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, i);
                    result = sysHyPhy20gBitWrite(fileHndl, regAddr, 3, 1);
                    if (result) return result;    
                }
            }
        }
    } else {
        /* Configure top level reset for 1 SONET slice */
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
        offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF + slice;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        if (enable) {
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
            if (result) return result;
        }
        
        /* Apply Low Power settings */    
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF + slice;
        if (enable) {
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
            if (result) return result;
        } else {
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
            if (result) return result;
        }

        if (enable) {
            /* reset 1 slice */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

			/* reset 1 Rx slice at block level and verify that RTERB 
			block indirect access is working correctly */
			for (j = 0; j<10; j++) {
		        offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
		        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
		        if (result) return result;
		 
		        offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_SRESET_OFF;
		        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
		        if (result) return result;
		        
		        result = sonetHyPhy20gRTERBIndWrite(fileHndl, slice, 0, 0, 0, 0x5430 + slice);
		        if (result) return result;
		        result = sonetHyPhy20gRTERBIndRead(fileHndl, slice, 0, 0, 0, &txCid);
		        if (result) return result;
		        if (txCid == (0x5430 + slice)) 
		        		j = 100;
		    }
		    if (j!= 101) 
		    	return HYPHY20G_ERR_DEV_ACCESS;
		       	
			/* reset 1 Tx slice at block level */
		    offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
		    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
		    if (result) return result;
		    
		    offset = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_TX_SRESET_OFF;
		    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
		    if (result) return result;
		            	
		    
            /* Configure all timeslots of 1 slice as Master timeslots */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
            if (result) return result;        

            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
            if (result) return result;        

            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
            if (result) return result;        

            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
            if (result) return result;        

            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
            if (result) return result;        

            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
            if (result) return result;   
                
            /* Configure bits to enable fast LOS detection in SONET framer */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_SRESET_AND_PROC_SLC_CFG2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gRead(fileHndl, regAddr, &val);
            if (result) return result;

            mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_LOS_DET_EN_MSK;
            result = hyPhy20gVarFieldWrite (&val, mask, 1);
            if (result) return result;

            mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_RX_LOS_EN_MSK;
            result = hyPhy20gVarFieldWrite (&val, mask, 1);
            if (result) return result;

            mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_SARC_AISINS_EN_MSK;
            result = hyPhy20gVarFieldWrite (&val, mask, 1);
            if (result) return result;

            mask = HYPHY20G_SONET_TOP_BIT_SDH_SRESET_AND_PROC_SLC_CFG2_LOS_EARLY_DECLARE_EN_MSK;
            result = hyPhy20gVarFieldWrite (&val, mask, 1);
            if (result) return result;

            result = sysHyPhy20gWrite(fileHndl, regAddr, val);
            if (result) return result;     

            if (enable == 2) {
                regAddr = HYPHY20G_SONET_RTOHT_REG_CFG_2;
                regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, 3, 1);
                if (result) return result;    
            }
        }
    }        


    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetInit */

/*******************************************************************************
**
**  hyPhy20gSonetPldFilterModeCfg
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:  This function configures the PCD_CONFIG and PLD_SELECT 
**                block fields for a 2.5 G SONET resource within the SONET 
**                subsystem.  These fields are used for payload detection and 
**                filtering when automatic payload configuration is enabled.
**
**                For a group of four SONET slices working to process an
**                STS-192 signal using automatic payload detection, this 
**                function should be called for all four slices.
**
**                Following this procedure, functions 
**                hyPhy20gSonetSts192FrmrCfg, hyPhy20gSonetSts48FrmrCfg
**                hyPhy20gSonetSts12FrmrCfg, and hyPhy20gSonetSts3FrmrCfg  
**                can be called to configure specific signals for automatic 
**                payload configuration.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - SONET slice instance
**                               0 to 15
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPldFilterModeCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 pcdCfg, UINT4 pldSel)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 offset;
    UINT4 mask;
    UINT4 pcdCfgData = 0;
    UINT4 pldSelData = 0;

    /* argument checking */
    if ((slice > 15)||(pcdCfg > 2)||(pldSel > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    /* Configure Tx PCD_CONFIG field */
    regAddr = HYPHY20G_SONET_TSHPI_REG_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    mask = HYPHY20G_SONET_TSHPI_BIT_CFG_PCD_CONFIG_MSK;
    if (pcdCfg) {
        result = hyPhy20gVarFieldWrite(&pcdCfgData, mask, pcdCfg+1);
        if (result) return result;                   
    } else {
        result = hyPhy20gVarFieldWrite(&pcdCfgData, mask, pcdCfg);
        if (result) return result;                   
    }
    result = sysHyPhy20gWrite(fileHndl, regAddr, pcdCfgData);
    if (result) return result;        

    /* Configure Rx PCD_CONFIG field */
    regAddr = HYPHY20G_SONET_RSHPI_REG_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    mask = HYPHY20G_SONET_RSHPI_BIT_CFG_PCD_CONFIG_MSK;
    if (pcdCfg) {
        result = hyPhy20gVarFieldWrite(&pcdCfgData, mask, pcdCfg+1);
        if (result) return result;                   
    } else {
        result = hyPhy20gVarFieldWrite(&pcdCfgData, mask, pcdCfg);
        if (result) return result;                   
    }
    result = sysHyPhy20gWrite(fileHndl, regAddr, pcdCfgData);
    if (result) return result;        

    /* Configure Tx/Rx PLD_FILTER_MODE fields */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_PCD_FLT_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    mask = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_PCD_FLT_CFG_TPLD_FILTER_MODE_MSK;
    result = hyPhy20gVarFieldWrite(&pldSelData, mask, pldSel);
    if (result) return result;                   

    mask = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_PCD_FLT_CFG_RPLD_FILTER_MODE_MSK;
    result = hyPhy20gVarFieldWrite(&pldSelData, mask, pldSel);
    if (result) return result;                   

    result = sysHyPhy20gWrite(fileHndl, regAddr, pldSelData);
    if (result) return result;        
    
    return HYPHY20G_SUCCESS;
    

} /* hyPhy20gSonetPldFilterModeCfg */

/*******************************************************************************
**
**  hyPhy20gSonetPcdPaisDetCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures PAIS detector block associated  
**                  with the RSHPI and TSHPI inside SONET subsystem.
**                  
**                  When PAIS Detector is enabled, the PCD output of the SHPI
**                  on the time slot interpreted as in AIS will be
**                  forced to MASTER, irrespective of its previous state.
**
**                  The PAIS Detector processes the payload configuration after 
**                  automatic payload detection but prior to automatic payload 
**                  configuration filtering. 
**
**                  Payload Detection Algorithm selection is performed with
**                  hyPhy20gSonetPldFilterModeCfg.
**                  
**                  This API should be called following 
**                  hyPhy20gSonetPldFilterModeCfg to ensure the payload
**                  configuration detection state machine and the justification
**                  counter have been set up properly.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - SONET slice instance
**                                0 to 15
**                  pAisDetEnbl - processing mode for PAIS Detector block
**                                0 - disable PAIS detector
**                                1 - enable PAIS detector
**                  aisFCnt     - number of consecutive frames in which H1 
**                                and H2 pointers indicate a AIS pattern (0xFF)
**                                after which the payload configuration on a 
**                                time slot is forced to a MASTER state
**                                Legal range: 0x1 - 0x7 
**                                Default value: 0x3
**                  normFCnt    - number of consecutive frames in which H1 and 
**                                H2 pointers indicate a non-AIS pattern 
**                                (pattern other than 0xFF) before the force on 
**                                the payload configuration of a time slot is 
**                                released from the MASTER state 
**                                Legal range: 0x3 - 0xF
**                                Default value: 0xB
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
INT4 hyPhy20gSonetPcdPaisDetCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 pAisDetEnbl, UINT4 aisFCnt, 
                                UINT4 normFCnt)
{
    /* Variable declaration */
    UINT4 rxAddr, txAddr;
    UINT4 addr, msk;
    UINT4 rshpiMsk, tshpiMsk;
    UINT4 rxNormCnt, rxAisCnt;
    UINT4 txNormCnt, txAisCnt;
    UINT4 regData; 
    INT4 result;
    
    /* Argument checking */
    if (slice > 15 || pAisDetEnbl > 1 || (aisFCnt > 7 || aisFCnt < 1) ||
        normFCnt > 0xF ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        
    }
    
    /* Check to confirm user supplied normFCnt is valid for the current 
       PCD configuration */
    /* Check PCD_CONFIG in RSHPI */
    addr = HYPHY20G_SONET_RSHPI_REG_CFG; 
    addr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(addr, slice); 
    msk = HYPHY20G_SONET_RSHPI_BIT_CFG_PCD_CONFIG_MSK; 
    
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &regData);
    if (result) return result;
    
    /* When Algorithm 1 is in use, NORM_FCNT must be at least 0xB 
       When Algorithm 2 is in use, NORM_FCNT must be at least 0x3 */    
    if ((regData == 0x0 && normFCnt < 0xB) || 
        ((regData == 0x2 || regData == 0x3) && normFCnt < 0x3))
        return HYPHY20G_ERR_INVALID_ARG;
    
    /* Check PCD_CONFIG in TSHPI */
    addr = HYPHY20G_SONET_TSHPI_REG_CFG; 
    addr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(addr, slice); 
    msk = HYPHY20G_SONET_TSHPI_BIT_CFG_PCD_CONFIG_MSK; 
    
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &regData);
    if (result) return result;
    
    /* When Algorithm 1 is in use, NORM_FCNT must be at least 0xB 
       When Algorithm 2 is in use, NORM_FCNT must be at least 0x3 */    
    if ((regData == 0x0 && normFCnt < 0xB) || 
        ((regData == 0x2 || regData == 0x3) && normFCnt < 0x3))
        return HYPHY20G_ERR_INVALID_ARG;
    
    /* Determine registers and masks */
    rxAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_PCD_FLT_CFG;
    rxAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(rxAddr, slice); 
    txAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_CTL;
    txAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(txAddr, slice); ;
    
    rshpiMsk = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_PCD_FLT_CFG_RPCD_PAIS_DET_EN_MSK;
    tshpiMsk = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_CTL_TPCD_PAIS_DET_EN_MSK;
    rxNormCnt = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_PCD_FLT_CFG_RPCD_TO_NORM_FCNT_MSK;
    rxAisCnt = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_PCD_FLT_CFG_RPCD_TO_AIS_FCNT_MSK;
    txNormCnt = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_CTL_TPCD_TO_NORM_FCNT_MSK;
    txAisCnt = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_CTL_TPCD_TO_AIS_FCNT_MSK;
    
    /* Configure PAIS detector block in RSHPI */
    result = sysHyPhy20gRead(fileHndl, rxAddr, &regData);
    if (result) return result;
    
    /* Set RPCD_PAIS_DET_EN */    
    result = hyPhy20gVarFieldWrite(&regData, rshpiMsk, pAisDetEnbl);
    if (result) return result; 
    
    /* Set RPCD_TO_AIS_FCNT */  
    result = hyPhy20gVarFieldWrite(&regData, rxAisCnt, aisFCnt);
    if (result) return result;
    
    /* Set RPCD_TO_NORM_FCNT */ 
    result = hyPhy20gVarFieldWrite(&regData, rxNormCnt, normFCnt);
    if (result) return result; 
    
    /* Write to the register */    
    result = sysHyPhy20gWrite(fileHndl, rxAddr, regData);
    if (result) return result;
       
    /* Configure PAIS detector block in TSHPI */
    result = sysHyPhy20gRead(fileHndl, txAddr, &regData);
    if (result) return result;
    
    /* Set TPCD_PAIS_DET_EN */ 
    result = hyPhy20gVarFieldWrite(&regData, tshpiMsk, pAisDetEnbl);
    if (result) return result;
        
    /* Set TPCD_TO_AIS_FCNT */
    result = hyPhy20gVarFieldWrite(&regData, txAisCnt, aisFCnt);
    if (result) return result;
    
    /* Set TPCD_TO_NORM_FCNT */
    result = hyPhy20gVarFieldWrite(&regData, txNormCnt, normFCnt);
    if (result) return result;
        
    /* Write to the register */ 
    result = sysHyPhy20gWrite(fileHndl, txAddr, regData);
    if (result) return result;
       
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPcdPaisDetCfg */

/*******************************************************************************
**
**  hyPhy20gSonetSts192FrmrCfg
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the payload for four 2.5G SONET 
**                 processing resources within the SONET subsystem to process
**                 an STS-192 rate signal.
**
**                 Prior to calling this function, SONET_LINE must have been 
**                 configured to specify the line side data source.  
**                 The line side data source (XFI/SFP, OTN subsystem, or 
**                 ODUKPP subsystem) must then be configured to allow a valid   
**                 receive timing signal to be provided to SONET subsystem.   
**
**                 Functions hyPhy20gSonetSts48FrmrCfg, 
**                 hyPhy20gSonetSts12FrmrCfg, and hyPhy20gSonetSts3FrmrCfg
**                 can then be called to further configure specific portions
**                 of the STS-192 payload.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - SONET slice instances to use
**                               0 - Sonet slices 0 to 3
**                               1 - Sonet slices 4 to 7
**                               2 - Sonet slices 8 to 11
**                               3 - Sonet slices 12 to 15
**                 mode        - 0 - 192 x STS-1, manual payload detection
**                             - 1 - 64 x STS-3c, manual payload detection
**                             - 2 - 16 x STS-12c, manual payload detection
**                             - 3 - 4 x STS-48c, manual payload detection
**                             - 4 - 1 x STS-192c, manual payload detection
**                             - 5 - 1 x STS-192, automatic payload detection
**                             - 6 - 8 x STS-24c, manual payload detection
**                             
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetSts192FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 mode)
{

    INT4 result;
    UINT4 regAddr;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sts3, sts1;
    UINT4 iData;
    UINT4 autoCfg = 0;
    UINT4 mask;
    UINT4 offset;
    UINT4 slice0RxCfg1 = 0;
    UINT4 slice0RxCfg2 = 0;
    UINT4 slice0RxCfg3 = 0;
    UINT4 slice123RxCfg1 = 0; 
    UINT4 slice123RxCfg2 = 0; 
    UINT4 slice123RxCfg3 = 0;
    UINT4 slice0TxCfg1 = 0; 
    UINT4 slice0TxCfg2 = 0; 
    UINT4 slice0TxCfg3 = 0;
    UINT4 slice123TxCfg1 = 0; 
    UINT4 slice123TxCfg2 = 0; 
    UINT4 slice123TxCfg3 = 0;
    UINT4 i;

  

    /* argument checking */
    if ((slice > 3)||(mode > 6)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    for (i = 4*slice; i< ((4*slice) + 4); i++) {
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
        offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF + i;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
        if (result) return result;
        if (topLvlRst) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    } 
    
    /* Check top level low power bit */
    for (i = 4*slice; i< ((4*slice) + 4); i++) {
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF + i;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
        if (result) return result;
        if (topLvlPwr) {
        	return HYPHY20G_ERR_INVALID_PRECONFIG;
        } 
    } 

    /* Create Tx Payload and Rx Payload manual settings for 4 slices */ 
    switch (mode) {
        case 0:
          slice0RxCfg1 = 0xFFFF;
          slice0RxCfg2 = 0xFFFF;
          slice0RxCfg3 = 0xFFFF;
          
          slice0TxCfg1 = 0xFFFF;
          slice0TxCfg2 = 0xFFFF;
          slice0TxCfg3 = 0xFFFF;

          slice123RxCfg1 = 0xFFFF;
          slice123RxCfg2 = 0xFFFF;
          slice123RxCfg3 = 0xFFFF;
      
          slice123TxCfg1 = 0xFFFF;
          slice123TxCfg2 = 0xFFFF;
          slice123TxCfg3 = 0xFFFF;

          autoCfg = 0;
          break;
        case 1:           
          slice0RxCfg1 = 0xFFFF;
          slice0RxCfg2 = 0x0;
          slice0RxCfg3 = 0x0;
          
          slice0TxCfg1 = 0xFFFF;
          slice0TxCfg2 = 0x0;
          slice0TxCfg3 = 0x0;

          slice123RxCfg1 = 0xFFFF;
          slice123RxCfg2 = 0x0;
          slice123RxCfg3 = 0x0;
      
          slice123TxCfg1 = 0xFFFF;
          slice123TxCfg2 = 0x0;
          slice123TxCfg3 = 0x0;

          autoCfg = 0;
          break;
        case 2:
          slice0RxCfg1 = 0x1111;
          slice0RxCfg2 = 0x0;
          slice0RxCfg3 = 0x0;
          
          slice0TxCfg1 = 0x1111;
          slice0TxCfg2 = 0x0;
          slice0TxCfg3 = 0x0;

          slice123RxCfg1 = 0x1111;
          slice123RxCfg2 = 0x0;
          slice123RxCfg3 = 0x0;
      
          slice123TxCfg1 = 0x1111;
          slice123TxCfg2 = 0x0;
          slice123TxCfg3 = 0x0;

          autoCfg = 0;
          break;
        case 3:
          slice0RxCfg1 = 0x1;
          slice0RxCfg2 = 0x0;
          slice0RxCfg3 = 0x0;
          
          slice0TxCfg1 = 0x1;
          slice0TxCfg2 = 0x0;
          slice0TxCfg3 = 0x0;

          slice123RxCfg1 = 0x1;
          slice123RxCfg2 = 0x0;
          slice123RxCfg3 = 0x0;
      
          slice123TxCfg1 = 0x1;
          slice123TxCfg2 = 0x0;
          slice123TxCfg3 = 0x0;

          autoCfg = 0;
          break;
        case 4:
          slice0RxCfg1 = 0x1;
          slice0RxCfg2 = 0x0;
          slice0RxCfg3 = 0x0;
          
          slice0TxCfg1 = 0x1;
          slice0TxCfg2 = 0x0;
          slice0TxCfg3 = 0x0;

          slice123RxCfg1 = 0x0;
          slice123RxCfg2 = 0x0;
          slice123RxCfg3 = 0x0;
      
          slice123TxCfg1 = 0x0;
          slice123TxCfg2 = 0x0;
          slice123TxCfg3 = 0x0;

          autoCfg = 0;
          break;
        case 5:
          slice0RxCfg1 = 0x1;
          slice0RxCfg2 = 0x0;
          slice0RxCfg3 = 0x0;
          
          slice0TxCfg1 = 0x1;
          slice0TxCfg2 = 0x0;
          slice0TxCfg3 = 0x0;

          slice123RxCfg1 = 0x0;
          slice123RxCfg2 = 0x0;
          slice123RxCfg3 = 0x0;
      
          slice123TxCfg1 = 0x0;
          slice123TxCfg2 = 0x0;
          slice123TxCfg3 = 0x0;

          autoCfg = 1;
          break;
        case 6:
          slice0RxCfg1 = 0x101;
          slice0RxCfg2 = 0x0;
          slice0RxCfg3 = 0x0;
          
          slice0TxCfg1 = 0x101;
          slice0TxCfg2 = 0x0;
          slice0TxCfg3 = 0x0;

          slice123RxCfg1 = 0x101;
          slice123RxCfg2 = 0x0;
          slice123RxCfg3 = 0x0;
      
          slice123TxCfg1 = 0x101;
          slice123TxCfg2 = 0x0;
          slice123TxCfg3 = 0x0;

          autoCfg = 0;
          break;
    }

    /* Configure Automatic Payload Detection Rx settings for 4 slices */ 
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (i=0; i<4; i++) {
        for (sts3=0; sts3<16; sts3++) {
            for (sts1=0; sts1<3; sts1++) {
                result = sonetHyPhy20gRSHPIIndRead(fileHndl, (4*slice) + i, 0x3, 
                                                        sts3, sts1, &iData);
                if (result) return result;
                if ((i==0)&&(sts3==0)&&(sts1==0)) {
                    /* Always set 1st timeslot to manual mode */
                    result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                    if (result) return result;                   
                } else {
                    result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                    if (result) return result;                   
                }
                result = sonetHyPhy20gRSHPIIndWrite(fileHndl, (4*slice) + i, 0x3, 
                                                        sts3, sts1, iData);
                if (result) return result;        
            }
        }
    }

    /* Configure Automatic Payload Detection Tx settings for 4 slices */ 
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (i=0; i<4; i++) {
        for (sts3=0; sts3<16; sts3++) {
            for (sts1=0; sts1<3; sts1++) {
                result = sonetHyPhy20gTSHPIIndRead(fileHndl, (4*slice) + i, 0x3, 
                                                        sts3, sts1, &iData);
                if (result) return result;
                if ((i==0)&&(sts3==0)&&(sts1==0)) {
                    /* Always set 1st timeslot to manual mode */
                    result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                    if (result) return result;                   
                } else {
                    result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                    if (result) return result;                   
                }
                result = sonetHyPhy20gTSHPIIndWrite(fileHndl, (4*slice) + i, 0x3, 
                                                        sts3, sts1, iData);
                if (result) return result;        
            }
        }
    }   
     
    /*Configure SONET Slice Rx Payload manual settings for 4 slices */
    for (i = 0; i<4; i++) {
        if (i==0) {
        /* First slice config */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0RxCfg1);
            if (result) return result;        
                
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0RxCfg2);
            if (result) return result;        
            
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0RxCfg3);
            if (result) return result;        
      
        } else {
        /* Remaining 3 slices config */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123RxCfg1);
            if (result) return result;        
                
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123RxCfg2);
            if (result) return result;        
            
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123RxCfg3);
            if (result) return result;        
        }
    }

    /* Configure SONET Slice Tx Payload manual settings for 4 slices */
    for (i = 0; i<4; i++) {
        if (i==0) {
        /* First slice config */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0TxCfg1);
            if (result) return result;        
                
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0TxCfg2);
            if (result) return result;        
            
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice0TxCfg3);
            if (result) return result;        
      
        } else {
        /* Remaining 3 slices config */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123TxCfg1);
            if (result) return result;        
                
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123TxCfg2);
            if (result) return result;        
            
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, (4*slice)+i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, slice123TxCfg3);
            if (result) return result; 
        }
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetSts192FrmrCfg */

/*******************************************************************************
**
**  hyPhy20gSonetSts48CalFrmrTxClk
**  ___________________________________________________________________________ 
**
**  DESCRIPTION: SW-based calibration procedure required in order to support mix
**               of SONET and SONET-over-OTN datapaths.
**             
**               The HyPHY is unable to support a mix of SONET and 
**               SONET-over-OTN datapaths within a group of 4 SONET framers 
**               unless a calibration procedure is used to phase-align the 
**               SerDes and SONET Tx clocks.
**               The calibration procedure makes it possible to support the 
**               following configuration:
**               One or more SONET/SDH framers configured for STS-48/STM-16 mode
**               and mapped directly to an SFP port while another SONET/SDH 
**               framer with the same group of four framers is mapped to an OTN
**               framer and then to an SFP port.
**
**  INPUTS:   *fileHndl   - base address
**            slice       - SONET slice instance to use
**                          0-15 - Sonet slices 0 to 15
**            spareSfp    - A spare Sfp port. valid range 0-15
**            otu1Sfp     - A sfp port which is for OTU1 traffic.  
**                          valid range 0-15.
**            maxNumAlign 
**                        - It stand for the amount of 'for' loop for the 
**                          calibration  at a 3ms interval. 1000 can be used as
**                          a default value. 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetSts48CalFrmrTxClk(struct file *fileHndl, 
              UINT4 slice, UINT4 spareSfp, UINT4 otu1Sfp, UINT4 maxNumAlign)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 regMask;
    UINT4 wciMode;
    UINT4 pisoSipoLoopback;
    UINT4 sfpLosEn;
    UINT4 tmpReg;
    UINT4 TmpRegHistory=0xaaaaaaaa,TmpRegHistory1=0;
    UINT4 i;

    UINT4 iGoodAlignment = 0;
    UINT4 iAlignmentWindowWidth = 0;
    UINT4 iAlignmentWindowCenter= 0;
    UINT4 iStep;
    UINT4 iFoundGoodAlignmentStart            = 0;
    UINT4 iFoundBadAlignmentStart             = 0;
    UINT4 iFoundGoodAlignmentSecondStart      = 0;
    UINT4 iGoodAlignmentStartIndex            = 0;
    UINT4 iBadAlignmentStartIndex             = 0;
    UINT4 iPrevAlignmentGood                  = 1;
    UINT4 iAlignmentResult                    = 0;


    /* argument checking */
    if ((slice > 15)||(spareSfp > 15)) {
        DBG_PRINT("%s, %s, %d, slice=%ld,sfp=%ld HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__,slice,spareSfp);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    if(otu1Sfp>15){
        DBG_PRINT("%s, %s, %d, alignWaitTimeMs=%ld ms HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__,otu1Sfp);
        return HYPHY20G_ERR_INVALID_ARG;
    }


    /* Record original value */
    regAddr = HYPHY20G_PGMRCLK_REG_WCIMODE_CTL_3;
    offset = HYPHY20G_PGMRCLK_BIT_WCIMODE_CTL_3_WCI_MODE_CTRL_SONET_SLICE_OFF(slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &wciMode);
    if (result) return result;

    regAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6,otu1Sfp);
    offset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_PISO_SIPO_LOOPBACK_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &pisoSipoLoopback);
    if (result) return result;

    regAddr = HYPHY20G_SONET_LINE_TOP_REG_SQUELCH_EN;
    offset = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN_OFF(otu1Sfp);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sfpLosEn);
    if (result) return result;

    
    /* Set the WCI_MODE_CTRL to read-to-clear mode */
    regAddr = HYPHY20G_PGMRCLK_REG_WCIMODE_CTL_3;
    offset = HYPHY20G_PGMRCLK_BIT_WCIMODE_CTL_3_WCI_MODE_CTRL_SONET_SLICE_OFF(slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* STEP 2: Internally loop back the SerDes Tx data to the Rx side using the 
    PISO to SIPO SerDes loopback. */

    regAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6,otu1Sfp);
    offset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_PISO_SIPO_LOOPBACK_OFF;    
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0x01);
    if (result) return result;

    regAddr = HYPHY20G_SONET_LINE_TOP_REG_SQUELCH_EN;
    offset = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN_OFF(otu1Sfp);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    for(i=0; i<maxNumAlign; i++){

        /* Change the clock phase alignments */

        regAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10,spareSfp);
        regMask = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_FRAC_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, regMask, 0x10);
        // result = sysHyPhy20gWrite(fileHndl, regAddr, 0x00001018);
        if (result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, regMask, 0x0);
        // result = sysHyPhy20gWrite(fileHndl, regAddr, 0x00000018);
        if (result) return result;
        sysHyPhy20gMsDelay(1);
        

        /* Ignore 1st value as changing ADSM_FRAC_DIV will cause OOF */
        regAddr = HYPHY20G_SONET_RRMP_REG_INT_STAT + 0x1000*slice;
        regMask = HYPHY20G_SONET_RRMP_BIT_INT_STAT_ALL_VALID_BIT_MSK;
        result =  hyPhy20gRegFieldRead(fileHndl, regAddr,regMask,&tmpReg); /* read-clear status bits first*/
        sysHyPhy20gMsDelay(1); // 5

        
        regAddr = HYPHY20G_SONET_RRMP_REG_INT_STAT + 0x1000*slice;
        regMask = HYPHY20G_SONET_RRMP_BIT_INT_STAT_ALL_VALID_BIT_MSK;
        result =  hyPhy20gRegFieldRead(fileHndl, regAddr,regMask,&tmpReg); /* read-clear status bits first*/
        if(tmpReg!=0){
            TmpRegHistory = ((TmpRegHistory<<1)|1);
        } else {
            TmpRegHistory = (TmpRegHistory<<1);
        }

        if((TmpRegHistory&0x1f) == 0){ // if it is good for successive 5 five times
            iGoodAlignment = 1;
        } else if((TmpRegHistory&0x1f)==0x1f){
            iGoodAlignment = 0;
        }
        if(iFoundGoodAlignmentStart == 0) {
            /* Now find the start of the sampling region. The start of the 
               sampling region is found when the data window transistions from
               having '1' consecutive non-zero values to an all zero value.*/
            if ((iGoodAlignment == 1) && (iPrevAlignmentGood == 0)) {
                iFoundGoodAlignmentStart = 1;
                iGoodAlignmentStartIndex = i;
            }
        }

        if((iFoundGoodAlignmentStart == 1) && (iFoundBadAlignmentStart == 0)) {
            /* Now find the end of the sampling region. The end of the sampling 
               region is found when the data window transistions from having
               an all zero value to an non-zero value. */

            if ((iGoodAlignment == 0) && (iPrevAlignmentGood == 1)) {
                iFoundBadAlignmentStart = 1;
                iBadAlignmentStartIndex = i;

                iAlignmentWindowWidth = iBadAlignmentStartIndex - iGoodAlignmentStartIndex;
                if (iAlignmentWindowWidth < 200) { // change 10 to 100
                /* Try searching again as a false lack of errors may have 
                   incorrectly triggered the alignment window logic. */
                    iFoundGoodAlignmentStart = 0;
                    iFoundBadAlignmentStart = 0;
                }
            }
        }

        if ((iFoundGoodAlignmentStart == 1) && (iFoundBadAlignmentStart == 1)) {

            /* Now find the start of the next sampling region.
               The start of the sampling region is found when the data window
               transistions from having a non-zero value to an all zero value.
            */
            if ((iGoodAlignment == 1) && (iPrevAlignmentGood == 0)) {

                iFoundGoodAlignmentSecondStart = 1;

                /* Now center the clock alignments by waiting 1/2 the good alignment width. */
                iAlignmentWindowCenter  = iAlignmentWindowWidth / 2;

                for(iStep = 0; iStep < iAlignmentWindowCenter; iStep++) {
                
                    /* Change the clock phase alignments */
                    regAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10,spareSfp);
                    regMask = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_FRAC_DIV_MSK;
                    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, regMask, 0x10);
                    // result = sysHyPhy20gWrite(fileHndl, regAddr, 0x00001018);
                    if (result) return result;
                    
                    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, regMask, 0x0);
                    // result = sysHyPhy20gWrite(fileHndl, regAddr, 0x00000018);
                    if (result) return result;
                    sysHyPhy20gMsDelay(1);

                    // ==============
                            /* Ignore 1st value as changing ADSM_FRAC_DIV will cause OOF */
                    regAddr = HYPHY20G_SONET_RRMP_REG_INT_STAT + 0x1000*slice;
                    regMask = HYPHY20G_SONET_RRMP_BIT_INT_STAT_ALL_VALID_BIT_MSK;
                    result =  hyPhy20gRegFieldRead(fileHndl, regAddr,regMask,&tmpReg); /* read-clear status bits first*/
                    sysHyPhy20gMsDelay(1); // 5
                    
                    regAddr = HYPHY20G_SONET_RRMP_REG_INT_STAT + 0x1000*slice;
                    regMask = HYPHY20G_SONET_RRMP_BIT_INT_STAT_ALL_VALID_BIT_MSK;
                    result =  hyPhy20gRegFieldRead(fileHndl, regAddr,regMask,&tmpReg); /* read-clear status bits first*/
                    if(tmpReg!=0){
                        TmpRegHistory1 = ((TmpRegHistory1<<1)|1);
                    } else {
                        TmpRegHistory1 = (TmpRegHistory1<<1);
                    }
                    // ==============
                }

                /* Now check that the clocks have been properly aligned
                   Ignore 1st value as changing ADSM_FRAC_DIV will cause OOF */
                   /*
                regAddr = HYPHY20G_SONET_RRMP_REG_INT_STAT + 0x1000*slice;
                regMask = HYPHY20G_SONET_RRMP_BIT_INT_STAT_ALL_VALID_BIT_MSK;
                result =  hyPhy20gRegFieldRead(fileHndl, regAddr,regMask,&tmpReg); 
                sysHyPhy20gMsDelay(10);

                result =  hyPhy20gRegFieldRead(fileHndl, regAddr,regMask,&tmpReg); 
                if(tmpReg == 0) {
                    iAlignmentResult = 1;
                }
                */

                if((TmpRegHistory1&0xffff) == 0) { // for successive 16 times
                    iAlignmentResult = 1;
                }
                break;
            }
        }

        iPrevAlignmentGood = iGoodAlignment;
    }


    /* Restore the LOS_EN bit*/
    regAddr = HYPHY20G_SONET_LINE_TOP_REG_SQUELCH_EN;
    offset = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN_OFF(otu1Sfp);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, sfpLosEn);
    if (result) return result;

    
    /* Restore the SerDes loopback */
    regAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6,otu1Sfp);
    offset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_PISO_SIPO_LOOPBACK_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, pisoSipoLoopback);
    if (result) return result;  

    /* Restore original WCI_MODE_CTRL value */
    regAddr = HYPHY20G_PGMRCLK_REG_WCIMODE_CTL_3;
    offset = HYPHY20G_PGMRCLK_BIT_WCIMODE_CTL_3_WCI_MODE_CTRL_SONET_SLICE_OFF(slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, wciMode);
    if (result) return result;

    if(iAlignmentResult == 1) {
        result = HYPHY20G_SUCCESS;
    } else {
        result = HYPHY20G_FAILURE;
    }
    return result;
}

/*******************************************************************************
**
**  hyPhy20gSonetSts48FrmrCfg
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the payload for one 2.5G SONET 
**                 processing resource within the SONET subsystem to process
**                 an STS-48 rate signal or component of an STS-192 rate signal.  
**
**                 Prior to calling this function, SONET_LINE must have been 
**                 configured to specify the line side data source.  
**                 The line side data source (XFI/SFP, OTN subsystem, or 
**                 ODUKPP subsystem) must then be configured to allow a valid   
**                 receive timing signal to be provided to SONET subsystem.    
**
**                 Functions hyPhy20gSonetSts24FrmrCfg, 
**                 hyPhy20gSonetSts12FrmrCfg and hyPhy20gSonetSts3FrmrCfg can  
**				   then be called to further configure specific portions of the 
**                 STS-48 payload.
**
**  INPUTS:        *fileHndl   - base address
**
**                 slice       - SONET slice instance to use
**                               0-15 - Sonet slices 0 to 15
**
**                 mode        - 0 - 48 x STS-1, manual payload detection
**                             - 1 - 16 x STS-3c, manual payload detection
**                             - 2 - 4 x STS-12c, manual payload detection
**                             - 3 - 1 x STS-48c, manual payload detection
**                             - 4 - 1 x STS-48, automatic payload detection - 
**                                  call hyPhy20gSonetPldFilterModeCfg before
**                                  using this option.
**                             - 5 - 2 x STS-24c, manual payload detection
**
**  NOTE:          Calling this function with mode = 0 brings the 2.5G SONET 
**                 slice to a default state.  (hyPhy20gSonetInit achieves the 
**                 same results).  This should be called first any 
**                 time a SONET slice is being re-configured to carry a 
**                 lower-speed signal than the present configuration.  Following
**                 that, hyPhy20gSonetSts24FrmrCfg, hyPhy20gSonetSts12FrmrCfg
**		           and/or hyPhy20gSonetSts3FrmrCfg can be used to configure the 
**				   actual link rate.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetSts48FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 mode)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sts3, sts1;
    UINT4 iData;
    UINT4 autoCfg = 0;
    UINT4 mask;
    UINT4 offset;
    UINT4 rate;
    UINT4 rxCfg1 = 0;
    UINT4 rxCfg2 = 0; 
    UINT4 rxCfg3 = 0;
    UINT4 txCfg1 = 0; 
    UINT4 txCfg2 = 0; 
    UINT4 txCfg3 = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15)||(mode > 5)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    /* Create Tx Payload and Rx manual Payload settings for this slice */
    switch (mode) {
      case 0:
          rxCfg1 = 0xFFFF;
          rxCfg2 = 0xFFFF;
          rxCfg3 = 0xFFFF;
      
          txCfg1 = 0xFFFF;
          txCfg2 = 0xFFFF;
          txCfg3 = 0xFFFF;
          autoCfg = 0;
          break;
      case 1:           
          rxCfg1 = 0xFFFF;
          rxCfg2 = 0x0;
          rxCfg3 = 0x0;
      
          txCfg1 = 0xFFFF;
          txCfg2 = 0x0;
          txCfg3 = 0x0;
          autoCfg = 0;
          break;
      case 2:
          rxCfg1 = 0x1111;
          rxCfg2 = 0x0;
          rxCfg3 = 0x0;
      
          txCfg1 = 0x1111;
          txCfg2 = 0x0;
          txCfg3 = 0x0;
          autoCfg = 0;
          break;
      case 3:
          rxCfg1 = 0x1;
          rxCfg2 = 0x0;
          rxCfg3 = 0x0;
      
          txCfg1 = 0x1;
          txCfg2 = 0x0;
          txCfg3 = 0x0;
          autoCfg = 0;
          break;
      case 4:
          rxCfg1 = 0x1;
          rxCfg2 = 0x0;
          rxCfg3 = 0x0;
      
          txCfg1 = 0x1;
          txCfg2 = 0x0;
          txCfg3 = 0x0;
          autoCfg = 1;
          break;
      case 5:
          rxCfg1 = 0x101;
          rxCfg2 = 0x0;
          rxCfg3 = 0x0;
      
          txCfg1 = 0x101;
          txCfg2 = 0x0;
          txCfg3 = 0x0;
          autoCfg = 0;
          break;
    }

    /* Configure Automatic Payload Detection Rx settings for the slice */ 
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=0; sts3<16; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==0)&&(sts1==0)) {
                /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }
    
    /* Configure Automatic Payload Detection Tx settings for the slice */ 
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=0; sts3<16; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==0)&&(sts1==0)) {
                /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
             result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }
    
    /* Configure SONET Slice Rx Manual Payload Settings */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg1);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg2);
    if (result) return result;        
    
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg3);
    if (result) return result;        


    /* Configure SONET Slice Tx Manual Payload Settings  */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg1);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg2);
    if (result) return result;        
    
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg3);
    if (result) return result;        

    /* If processing an OC-192 signal, configure First timeslot of next Rx 
       SONET slice for manual master mode */ 
    
    result = utilHyPhy20gSonetLnGetSignalRate(fileHndl, slice, &rate);
    if (result) return result;
    
    if (rate == 0) {  
      
        if ((slice+1)%4) {
            /* Set to Master Mode */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);
            if (result) return result;
            
            
            /* Set to Manual Mode */
    		mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice+1, 0x3, 
                                                       0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                        0x0, 0x0, iData);
            if (result) return result;        
        }
    
    
        /* Configure First timeslot of next Tx SONET slice for manual master mode */ 
        if ((slice+1)%4) {
            /* Set to Master Mode */
            regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);
            if (result) return result;
            
            
            /* Set to Manual Mode */
    		mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice+1, 0x3, 
                                                       0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                        0x0, 0x0, iData);
            if (result) return result;        
        }
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetSts48FrmrCfg */

/*******************************************************************************
**
**  hyPhy20gSonetSts24FrmrCfg
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the payload settings for a portion
**                 of one 2.5G SONET processing resource within the SONET 
**                 subsystem to process an STS-24 rate signal or component of 
**                 an STS-48 or STS-192 rate signal.
**
**                 Prior to calling this function, SONET_LINE must have been 
**                 configured to specify the line side data source.  
**                 The line side data source (XFI/SFP, OTN subsystem, or 
**                 ODUKPP subsystem) must then be configured to allow a valid   
**                 receive timing signal to be provided to SONET subsystem.    
**
**
**  INPUTS:        *fileHndl   - base address
**
**                 slice       - SONET slice instance to use
**                               0-15 - Sonet slices 0 to 15
**
**                 sts24Num    - STS24 number within this 2.5G SONET resource
**                             - valid values 0-1
**
**                 mode        - 0 - 24 x STS-1, manual payload detection
**                             - 1 - 8 x STS-3c, manual payload detection
**                             - 2 - 2 x STS-12c, manual payload detection
**                             - 3 - 1 x STS-24c, manual payload detection
**                             - 4 - 1 x STS-24, automatic payload detection - 
**                                  call hyPhy20gSonetPldFilterModeCfg before
**                                  using this option.
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetSts24FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 sts24Num, UINT4 mode)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rxCfg1, rxCfg2, rxCfg3;
    UINT4 txCfg1, txCfg2, txCfg3;
    UINT4 rxCfg1mask = 0; 
    UINT4 rxCfg2mask = 0; 
    UINT4 rxCfg3mask = 0;
    UINT4 txCfg1mask = 0; 
    UINT4 txCfg2mask = 0; 
    UINT4 txCfg3mask = 0;
    UINT4 sts3, sts1;
    UINT4 sts3Start, sts3End;
    UINT4 sts24Mask;
    UINT4 autoCfg = 0;
    UINT4 mask;
    UINT4 offset;
    UINT4 rate;
    UINT4 iData = 0;  
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);


    /* argument checking */
    if ((slice > 15)||(sts24Num > 1)||(mode > 4)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    sts24Mask = 0xFF  << 8*sts24Num;    
    /* Create Tx Payload and Rx manual Payload settings for this slice */
    switch (mode) {
      case 0:
          rxCfg1mask = 0xFF;
          rxCfg2mask = 0xFF;
          rxCfg3mask = 0xFF;
  
          txCfg1mask = 0xFF;
          txCfg2mask = 0xFF;
          txCfg3mask = 0xFF;
          
          autoCfg = 0;
          break;
      case 1:
          rxCfg1mask = 0xFF;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0xFF;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 0;
          break;
      case 2:
          rxCfg1mask = 0x11;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0x11;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 0;
          break;
      case 3:
          rxCfg1mask = 0x1;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0x1;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 0;
          break;
      case 4:
          rxCfg1mask = 0x1;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0x1;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 1;
          break;
    }
    
    sts3Start = 8*sts24Num;
    sts3End = sts3Start + 8;
    
    /* Configure Automatic Payload Detection Rx settings for the slice */ 
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=sts3Start; sts3<sts3End; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==sts3Start)&&(sts1==0)) {
                  /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }
    
    /* Configure Automatic Payload Detection Tx settings for the slice */ 
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=sts3Start; sts3<sts3End; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==sts3Start)&&(sts1==0)) {
                /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
             result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }   
    
    /*Configure SONET Slice Rx Payload */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg1);
    if (result) return result;                
    result = hyPhy20gVarFieldWrite(&rxCfg1, sts24Mask, rxCfg1mask);
    if (result) return result;                   
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg1);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg2);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&rxCfg2, sts24Mask, rxCfg2mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg2);
    if (result) return result;        
    
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg3);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&rxCfg3, sts24Mask, rxCfg3mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg3);
    if (result) return result;        


    /* Configure SONET Slice Tx Payload  */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg1);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg1, sts24Mask, txCfg1mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg1);
    if (result) return result;            
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg2);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg2, sts24Mask, txCfg2mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg2);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg3);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg3, sts24Mask, txCfg3mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg3);
    if (result) return result;        
    
    result = utilHyPhy20gSonetLnGetSignalRate(fileHndl, slice, &rate);
    if (result) return result;

    /* Configure First timeslot of next STS-24 Rx for manual master mode, but
       only touch next SONET slice if the signal is part of an OC-192 client */ 

    /* Set to Master Mode */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
    if (sts24Num == 1) {
        if ((rate == 0) && ((slice+1)%4)) {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);           
        }
    } else {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Start+8, 1);           
    }
            
    /* Set to Manual Mode */
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    if (sts24Num == 1) {
        if ((rate ==0) && ((slice+1)%4)) {
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice+1, 0x3, 
                                               0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                0x0, 0x0, iData);
            if (result) return result;   
        } 
    } else {
        result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, 0x3, 
                                               sts3Start+8, 0x0, &iData);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&iData, mask, 0);
        if (result) return result;                   
        result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice, 0x3, 
                                                sts3Start+8, 0x0, iData);
        if (result) return result;   
    }        


     /* Configure First timeslot of next STS-12 Tx for manual master mode, but
        only touch next SONET slice if the signal is part of an OC-192 client */ 

    /* Set to Master Mode */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
    if (sts24Num == 1) {
        if ((rate == 0) && ((slice+1)%4)) {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);           
        }
    } else {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Start+8, 1);           
    }
            
    /* Set to Manual Mode */
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    if (sts24Num == 1) {
        if ((rate ==0) && ((slice+1)%4)) {
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice+1, 0x3, 
                                               0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                0x0, 0x0, iData);
            if (result) return result;   
        } 
    } else {
        result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, 0x3, 
                                               sts3Start+8, 0x0, &iData);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&iData, mask, 0);
        if (result) return result;                   
        result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice, 0x3, 
                                                sts3Start+8, 0x0, iData);
        if (result) return result;   
    }        
   
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetSts24FrmrCfg */

/*******************************************************************************
**
**  hyPhy20gSonetSts12FrmrCfg
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the payload settings for a portion
**                 of one 2.5G SONET processing resource within the SONET 
**                 subsystem to process an STS-12 rate signal or component of 
**                 an STS-48 or STS-192 rate signal.
**
**                 Prior to calling this function, SONET_LINE must have been 
**                 configured to specify the line side data source.  
**                 The line side data source (XFI/SFP, OTN subsystem, or 
**                 ODUKPP subsystem) must then be configured to allow a valid   
**                 receive timing signal to be provided to SONET subsystem.  
**
**                 If the signal being carried by this slice is of rate STS-12,
**                 then either hyPhy20gSonetInit or hyPhy20gSonetSts48CFrmrCfg 
**                 (with mode = 0) should be called before calling this 
**                 function.
**
**
**  INPUTS:        *fileHndl   - base address
**
**                 slice       - SONET slice instance to use
**                               0-15 - Sonet slices 0 to 15
**
**                 sts12Num    - STS12 number within this 2.5G SONET resource
**                             - valid values 0-3
**
**                 mode        - 0 - 12 x STS-1, manual payload detection
**                             - 1 - 4 x STS-3c, manual payload detection
**                             - 2 - 1 x STS-12c, manual payload detection
**                             - 3 - 1 x STS-12, automatic payload detection - 
**                                  call hyPhy20gSonetPldFilterModeCfg before
**                                  using this option.
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetSts12FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 sts12Num, UINT4 mode)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rxCfg1, rxCfg2, rxCfg3;
    UINT4 txCfg1, txCfg2, txCfg3;
    UINT4 rxCfg1mask = 0; 
    UINT4 rxCfg2mask = 0; 
    UINT4 rxCfg3mask = 0;
    UINT4 txCfg1mask = 0; 
    UINT4 txCfg2mask = 0; 
    UINT4 txCfg3mask = 0;
    UINT4 sts3, sts1;
    UINT4 sts3Start, sts3End;
    UINT4 autoCfg = 0;
    UINT4 mask;
    UINT4 sts12Mask;
    UINT4 offset;
    UINT4 rate;
    UINT4 iData = 0;  
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);


    /* argument checking */
    if ((slice > 15)||(sts12Num > 3)||(mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    sts12Mask = 0xF << 4*sts12Num;    
    /* Create Tx Payload and Rx manual Payload settings for this slice */
    switch (mode) {
      case 0:
          rxCfg1mask = 0xF;
          rxCfg2mask = 0xF;
          rxCfg3mask = 0xF;
  
          txCfg1mask = 0xF;
          txCfg2mask = 0xF;
          txCfg3mask = 0xF;
          
          autoCfg = 0;
          break;
      case 1:
          rxCfg1mask = 0xF;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0xF;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 0;
          break;
      case 2:
          rxCfg1mask = 0x1;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0x1;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 0;
          break;
      case 3:
          rxCfg1mask = 0x1;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0x1;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 1;
          break;
    }
    
    sts3Start = 4*sts12Num;
    sts3End = sts3Start + 4;
    
    /* Configure Automatic Payload Detection Rx settings for the slice */ 
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=sts3Start; sts3<sts3End; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==sts3Start)&&(sts1==0)) {
                  /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }
    
    /* Configure Automatic Payload Detection Tx settings for the slice */ 
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=sts3Start; sts3<sts3End; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==sts3Start)&&(sts1==0)) {
                /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
             result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }   
    
    /*Configure SONET Slice Rx Payload */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg1);
    if (result) return result;                
    result = hyPhy20gVarFieldWrite(&rxCfg1, sts12Mask, rxCfg1mask);
    if (result) return result;                   
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg1);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg2);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&rxCfg2, sts12Mask, rxCfg2mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg2);
    if (result) return result;        
    
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg3);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&rxCfg3, sts12Mask, rxCfg3mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg3);
    if (result) return result;        


    /* Configure SONET Slice Tx Payload  */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg1);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg1, sts12Mask, txCfg1mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg1);
    if (result) return result;            
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg2);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg2, sts12Mask, txCfg2mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg2);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg3);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg3, sts12Mask, txCfg3mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg3);
    if (result) return result;        
    
    result = utilHyPhy20gSonetLnGetSignalRate(fileHndl, slice, &rate);
    if (result) return result;

    /* Configure First timeslot of next STS-12 Rx for manual master mode, but
       only touch next SONET slice if the signal is part of an OC-192 client */ 

    /* Set to Master Mode */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
    if (sts12Num == 3) {
        if ((rate == 0) && ((slice+1)%4)) {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);           
        }
    } else {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Start+4, 1);           
    }
            
    /* Set to Manual Mode */
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    if (sts12Num == 3) {
        if ((rate ==0) && ((slice+1)%4)) {
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice+1, 0x3, 
                                               0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                0x0, 0x0, iData);
            if (result) return result;   
        } 
    } else {
        result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, 0x3, 
                                               sts3Start+4, 0x0, &iData);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&iData, mask, 0);
        if (result) return result;                   
        result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice, 0x3, 
                                                sts3Start+4, 0x0, iData);
        if (result) return result;   
    }        


     /* Configure First timeslot of next STS-12 Tx for manual master mode, but
        only touch next SONET slice if the signal is part of an OC-192 client */ 

    /* Set to Master Mode */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
    if (sts12Num == 3) {
        if ((rate == 0) && ((slice+1)%4)) {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);           
        }
    } else {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Start+4, 1);           
    }
            
    /* Set to Manual Mode */
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    if (sts12Num == 3) {
        if ((rate ==0) && ((slice+1)%4)) {
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice+1, 0x3, 
                                               0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                0x0, 0x0, iData);
            if (result) return result;   
        } 
    } else {
        result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, 0x3, 
                                               sts3Start+4, 0x0, &iData);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&iData, mask, 0);
        if (result) return result;                   
        result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice, 0x3, 
                                                sts3Start+4, 0x0, iData);
        if (result) return result;   
    }        
   
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetSts12FrmrCfg */

/*******************************************************************************
**
**  hyPhy20gSonetSts3FrmrCfg
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the payload settings for a portion
**                 of one 2.5G SONET processing resource within the SONET 
**                 subsystem to process an STS-3 rate signal or component of 
**                 an STS-12, STS-48, or STS-192 rate signal.
**
**                 Prior to calling this function, SONET_LINE must have been 
**                 configured to specify the line side data source.  
**                 The line side data source (XFI/SFP, OTN subsystem, or 
**                 ODUKPP subsystem) must then be configured to allow a valid   
**                 receive timing signal to be provided to SONET subsystem.
**
**                 If the signal being carried by this slice is of rate STS-3,
**                 then either hyPhy20gSonetInit or hyPhy20gSonetSts48CFrmrCfg 
**                 (with mode = 0) should be called before calling this 
**                 function.
**
**
**  INPUTS:        *fileHndl   - base address
**
**                 slice       - SONET slice instance to use
**                               0-15 - Sonet slices 0 to 15
**                 sts12Num    - STS12 channel within this 2.5G SONET resource
**                             - valid values 0-3
**                 sts3Num     - STS3 channel within sts12Num
**                             - valid values 0-3
**                 mode        - 0 - 3 x STS-1, manual payload detection
**                             - 1 - 1 x STS-3c, manual payload detection
**                             - 2 - 1 x STS-3, automatic payload detection - 
**                                  call hyPhy20gSonetPldFilterModeCfg before
**                                  using this option.
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetSts3FrmrCfg(struct file *fileHndl, UINT4 slice, 
                              UINT4 sts12Num, UINT4 sts3Num, UINT4 mode)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rxCfg1, rxCfg2, rxCfg3;
    UINT4 txCfg1, txCfg2, txCfg3;
    UINT4 rxCfg1mask = 0;
    UINT4 rxCfg2mask = 0; 
    UINT4 rxCfg3mask = 0;
    UINT4 txCfg1mask = 0; 
    UINT4 txCfg2mask = 0; 
    UINT4 txCfg3mask = 0;
    UINT4 autoCfg = 0;
    UINT4 shift;
    UINT4 sts3, sts1;
    UINT4 sts3Start, sts3End;
    UINT4 mask;
    UINT4 sts3Mask;
    UINT4 offset;
    UINT4 rate;
    UINT4 iData = 0;  
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15)||(sts12Num > 3)||(sts3Num > 3)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONHY_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SONHY_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } 

    shift = 4*sts12Num + sts3Num;
    sts3Mask = 0x1 << shift;    
    /* Create Tx Payload and Rx manual Payload settings for this slice */
    switch (mode) {
      case 0:
          rxCfg1mask = 0x1;
          rxCfg2mask = 0x1;
          rxCfg3mask = 0x1;
  
          txCfg1mask = 0x1;
          txCfg2mask = 0x1;
          txCfg3mask = 0x1;
          
          autoCfg = 0;
          break;
      case 1:
          rxCfg1mask = 0x1;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0x1;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 0;
          break;
      case 2:
          rxCfg1mask = 0x1;
          rxCfg2mask = 0x0;
          rxCfg3mask = 0x0;
  
          txCfg1mask = 0x1;
          txCfg2mask = 0x0;
          txCfg3mask = 0x0;
          
          autoCfg = 1;
          break;
    }
    
    sts3Start = 4*sts12Num + sts3Num;
    sts3End = sts3Start + 1;
    
    /* Configure Automatic Payload Detection Rx settings for the slice */ 
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=sts3Start; sts3<sts3End; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==sts3Start)&&(sts1==0)) {
                  /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }
    
    /* Configure Automatic Payload Detection Tx settings for the slice */ 
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    for (sts3=sts3Start; sts3<sts3End; sts3++) {
        for (sts1=0; sts1<3; sts1++) {
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, 0x3, 
                                                    sts3, sts1, &iData);
            if (result) return result;
            if ((sts3==sts3Start)&&(sts1==0)) {
                /* Always set 1st timeslot to manual mode */
                result = hyPhy20gVarFieldWrite(&iData, mask, 0);
                if (result) return result;                   
            } else {
                result = hyPhy20gVarFieldWrite(&iData, mask, autoCfg);
                if (result) return result;                   
            }
             result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice, 0x3, 
                                                    sts3, sts1, iData);
            if (result) return result;        
        }
    }   

    /*Configure SONET Slice Rx Payload */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg1);
    if (result) return result;                
    result = hyPhy20gVarFieldWrite(&rxCfg1, sts3Mask, rxCfg1mask);
    if (result) return result;                   
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg1);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg2);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&rxCfg2, sts3Mask, rxCfg2mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg2);
    if (result) return result;        
    
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCfg3);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&rxCfg3, sts3Mask, rxCfg3mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, rxCfg3);
    if (result) return result;        


    /* Configure SONET Slice Tx Payload  */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg1);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg1, sts3Mask, txCfg1mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg1);
    if (result) return result;            
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg2);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg2, sts3Mask, txCfg2mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg2);
    if (result) return result;        
        
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &txCfg3);
    if (result) return result;        
    result = hyPhy20gVarFieldWrite(&txCfg3, sts3Mask, txCfg3mask);
    if (result) return result;                       
    result = sysHyPhy20gWrite(fileHndl, regAddr, txCfg3);
    if (result) return result;        
    
    result = utilHyPhy20gSonetLnGetSignalRate(fileHndl, slice, &rate);
    if (result) return result;

    /* Configure First timeslot of next STS-3 Rx for manual master mode, but
       only touch next SONET slice if the signal is part of an OC-192 client */ 

    /* Set to Master Mode */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_RX_PYLD_CFG_1;
    if (sts3Start == 15) {
        if ((rate == 0) && ((slice+1)%4)) {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);           
        }
    } else {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Start+1, 1);           
    }
            
    /* Set to Manual Mode */
    mask = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    if (sts3Start == 15) {
        if ((rate == 0) && ((slice+1)%4)) {
            result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice+1, 0x3, 
                                               0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                0x0, 0x0, iData);
            if (result) return result;   
        } 
    } else {
        result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, 0x3, 
                                               sts3Start+1, 0x0, &iData);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&iData, mask, 0);
        if (result) return result;                   
        result = sonetHyPhy20gRSHPIIndWrite(fileHndl, slice, 0x3, 
                                                sts3Start+1, 0x0, iData);
        if (result) return result;   
    }        


     /* Configure First timeslot of next STS-3 Tx for manual master mode, but
        only touch next SONET slice if the signal is part of an OC-192 client */ 

    /* Set to Master Mode */
    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_TX_PYLD_CFG_1;
    if (sts3Start == 15) {
        if ((rate == 0) && ((slice+1)%4)) {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0x0, 1);           
        }
    } else {
            regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Start+1, 1);           
    }
            
    /* Set to Manual Mode */
    mask = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_CFG_AUTOPLDCFG_EN_MSK;
    if (sts3Start == 15) {
        if ((rate ==0) && ((slice+1)%4)) {
            result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice+1, 0x3, 
                                               0x0, 0x0, &iData);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&iData, mask, 0);
            if (result) return result;                   
            result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice+1, 0x3, 
                                                0x0, 0x0, iData);
            if (result) return result;   
        } 
    } else {
        result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, 0x3, 
                                               sts3Start+1, 0x0, &iData);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&iData, mask, 0);
        if (result) return result;                   
        result = sonetHyPhy20gTSHPIIndWrite(fileHndl, slice, 0x3, 
                                                sts3Start+1, 0x0, iData);
        if (result) return result;   
    }        
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetSts3FrmrCfg */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
