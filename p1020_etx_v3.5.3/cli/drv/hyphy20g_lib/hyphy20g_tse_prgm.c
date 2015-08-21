/******************************************************************************/
/**  COPYRIGHT (C) 2009 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        : hyphy20g_tse_prgm.c
**
**  $Date: 2011-10-18 11:06:14 +0800 (Tue, 18 Oct 2011) $
**
**  $Revision: 16021 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_tse_prgm.h"

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
**  hyPhy20gTsePrgmInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function resets the PRGM and SHPI blocks within the TSE
**                 subsystem to a known state.
**
**                 This function must be called before calling the following
**                 TSE:PRGM payload configuration functions:
**                 - hyPhy20gTsePrgmTxSts48PayloadCfg
**                 - hyPhy20gTsePrgmTxSts12PayloadCfg
**                 - hyPhy20gTsePrgmTxSts3PayloadCfg
**                 - hyPhy20gTsePrgmRxSts48PayloadCfg
**                 - hyPhy20gTsePrgmRxSts12PayloadCfg
**                 - hyPhy20gTsePrgmRxSts3PayloadCfg
**
**
**  INPUTS:       *fileHndl     - base address
**                enable        - 1 - PRGM & SHPI are taken out of reset
**                              - 0 - PRGM & SHPI are held in reset
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    UINT4 mask;
    UINT4 regAddr;

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_TSE_TOP_REG_SW_RST;

    /* configure PRGM_RST */
    mask = HYPHY20G_TSE_TOP_BIT_SW_RST_PRGM_RST_MSK;
    if (enable) {
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
    		if (result) return result;

    		result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x0);
    		if (result) return result;

    } else {
    		result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
    		if (result) return result;
    }

    /* configure SHPI_RST */
    mask = HYPHY20G_TSE_TOP_BIT_SW_RST_SHPI_RST_MSK;
    if (enable) {
    		result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 1);
    		if (result) return result;

    		result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0);
    		if (result) return result;
    } else {
    		result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 1);
    		if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmInit */

/*******************************************************************************
**
**  hyPhy20gTsePrgmTxSts48PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the payload for STS-48 data rate
**                for PRGM generator within the TSE subsystem.
**
**                After calling this function, hyPhy20gTsePrgmTxSts12PayloadCfg
**                hyPhy20gTsePrgmTxSts3PayloadCfg can be called to further
**                configure the payload for each STS-12c and STS-3c.
**
**  INPUTS:       *fileHndl  - base address
**                pldType    - 0 - STS-48c
**                             1 - 4 x STS-12c
**                             2 - 16 x STS-3c
**                             3 - 48 x STS-1
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmTxSts48PayloadCfg(struct file *fileHndl, UINT4 pldType)
{
    /* Variable declaration */
    UINT4 regAddr;
    UINT4 prgmGenCfg;
    UINT4 msk;
    UINT4 i = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if (pldType > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (pldType) {
        /* 1 x STS-48c rate */
        case 0:
            /* Configure PRGMs #1 for master mode */
            regAddr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
   	        result = sysHyPhy20gRead(fileHndl, regAddr, &prgmGenCfg);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12CSL_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_MSSLEN_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0x3);
            if (result) return result;

            result = sysHyPhy20gWrite(fileHndl, regAddr, prgmGenCfg);
            if (result) return result;

            /* Configure PRGMs #2-4 for slave mode */
            for (i = 1; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

   		        result = sysHyPhy20gRead(fileHndl, regAddr, &prgmGenCfg);
    	        if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	        if (result) return result;

		        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12CSL_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	        if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_MSSLEN_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0x3);
    	        if (result) return result;

    	        result = sysHyPhy20gWrite(fileHndl, regAddr, prgmGenCfg);
    	        if (result) return result;
    	    }

    		break;

    	/* 4 x STS-12c rate */
        case 1:
             for (i = 0; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

   	            result = sysHyPhy20gRead(fileHndl, regAddr, &prgmGenCfg);
                if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	        if (result) return result;

		        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12CSL_MSK;
                result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0);
                if (result) return result;

                msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_MSSLEN_MSK;
                result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0x0);
    	        if (result) return result;

    	        result = sysHyPhy20gWrite(fileHndl, regAddr, prgmGenCfg);
    	        if (result) return result;
            }

            break;

        /* 16 x STS-3c rate */
        case 2:
            for (i = 0; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

   	            result = sysHyPhy20gRead(fileHndl, regAddr, &prgmGenCfg);
                if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0);
    	        if (result) return result;

		        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12CSL_MSK;
                result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0);
                if (result) return result;

                msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_MSSLEN_MSK;
                result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0x0);
    	        if (result) return result;

    	        result = sysHyPhy20gWrite(fileHndl, regAddr, prgmGenCfg);
    	        if (result) return result;
            }

            break;

        /* 48 x STS-1 rate */
        case 3:
            for (i = 0; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

 	            result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
 	            if (result)  return result;
            }

            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmTxSts48cPayloadCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmTxSts12PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures one of the four PRGM generator blocks
**                payload for STS-12 signal for 1 x STS-12c, 4 x STS-3c, or
**                12 x STS-1s within the TSE subsystem.
**
**                After calling this function, hyPhy20gTsePrgmTxSts3PayloadCfg
**                can be called to further configure the payload for each
**                STS-3c.
**
**  INPUTS:       *fileHndl - base address
**                prgmNum   - STS-12 rate PRGM to query
**                            0 - 3
**                pldType   - 0 - 1 x STS-12c
**                          - 1 - 4 x STS-3c
**                          - 2 - 12 x STS-1
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_MASTER
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmTxSts12PayloadCfg(struct file *fileHndl, UINT4 prgmNum,
                                      UINT4 pldType)
{
    /* Variable declaration */
    UINT4 regAddr;
    UINT4 prgmGenCfg;
    UINT4 masterMode;
    UINT4 msk;
    UINT4 offset;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if (pldType > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine registers 0x4932508 + 0x40*A(A=0:3) Generator Payload
       Configuration */
    regAddr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
    regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, prgmNum);

    /* Check the master mode for selected payload type:
       if configuring for 4xSTS-3c or 12xSTS-1, the PRGM block must not be
       a part of concatenated STS-12c */
    if (pldType == 1 || pldType == 2) {
        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
        offset = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_OFF;

        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &masterMode);
        if (result) return result;

        if (masterMode == 0x1)
            return HYPHY20G_ERR_INVALID_MASTER;
    }

    switch (pldType) {

        /* For STS-12c rate */
        case 0:
            /* Configure PRGM for master mode */
            result = sysHyPhy20gRead(fileHndl, regAddr, &prgmGenCfg);
            if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	    if (result) return result;

		    msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12CSL_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_MSSLEN_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0x0);
    	    if (result) return result;

    	    result = sysHyPhy20gWrite(fileHndl, regAddr, prgmGenCfg);
    	    if (result) return result;

            break;

        /* For 4 x STS-3c rate */
        case 1:
            result = sysHyPhy20gRead(fileHndl, regAddr, &prgmGenCfg);
            if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0);
    	    if (result) return result;

		    msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12CSL_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_MSSLEN_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 0x0);
    	    if (result) return result;

    	    result = sysHyPhy20gWrite(fileHndl, regAddr, prgmGenCfg);
    	    if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C3_MSK;
            result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	    if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C2_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	    if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C1_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	    if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C0_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmGenCfg, msk, 1);
    	    if (result) return result;

            break;

        /* For 12 x STS-1s rate */
        case 2:
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
            if (result) return result;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmTxSts12PayloadCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmTxSts3PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the TSE:PRGM Generator STS-3 payload
**                 type for one of the 16 STS-3 signals among the four PRGM
**                 blocks.
**
**                 This function needs to be called four times to configure all
**                 four STS-3 time slots in a PRGM block.
**
**  INPUTS:       *fileHndl - base address
**                prgmNum   - STS-12 rate PRGM to query
**                            0 - 3
**                sts3Num   - STS-3 rate payload inside a PRGM block
**                            0 - 3
**                pldType   - 0 - 1 x STS-3c
**                            1 - 3 x STS-1
**                            Legal combinations of mixed payload can be:
**                            3 x STS-3c with 3 x STS-1s
**                            2 x STS-3c with 6 x STS-1s
**                            1 x STS-3c with 9 x STS-1s
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_MASTER
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmTxSts3PayloadCfg(struct file *fileHndl, UINT4 prgmNum,
                                     UINT4 sts3Num, UINT4 pldType)
{
    /* Variable declaration */
    UINT4 regAddr;
    UINT4 masterMode;
    UINT4 msk = 0;
    UINT4 offset;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if ((prgmNum > 3) || (sts3Num > 3) || (pldType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register for payload configuration */
    regAddr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
    regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, prgmNum);

    /* Check the master mode for selected payload type:
       if configuring for 3xSTS-1, the PRGM must not be STS-12c master mode */
    if (pldType == 1) {
        msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
        offset = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_OFF;

        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &masterMode);
        if (result) return result;

        if (masterMode == 0x1)
            return HYPHY20G_ERR_INVALID_MASTER;
    }

    /* Determing which STS-3 timeslot to configure */
    switch (sts3Num) {
        case 0:
            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C0_MSK;
            break;
        case 1:
            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C1_MSK;
            break;
        case 2:
            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C2_MSK;
            break;
        case 3:
            msk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS3C3_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure STS-3 timeslot */
    switch (pldType) {
        /* For 1 x STS-3c rate */
        case 0:
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    	    if (result) return result;
            break;

       /* For 3 x STS-1s */
       case 1:
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0);
    	    if (result) return result;
            break;
       default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmTxSts3PayloadCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxSts48PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the payload for STS-48 data rate
**                for PRGM monitor within the TSE subsystem.
**
**                After calling this function, hyPhy20gTsePrgmRxSts12PayloadCfg
**                hyPhy20gTsePrgmRxSts3PayloadCfg can be called to further
**                configure the payload for each STS-12c and STS-3c.
**
**  INPUTS:       *fileHndl  - base address
**                pldType    - 0 - STS-48c
**                             1 - 4 x STS-12c
**                             2 - 16 x STS-3c
**                             3 - 48 x STS-1
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmRxSts48PayloadCfg(struct file *fileHndl, UINT4 pldType)
{
    /* Variable declaration */
    UINT4 regAddr;
    UINT4 prgmMonCfg;
    UINT4 msk;
    UINT4 i = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if (pldType > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (pldType) {
        /* 1 x STS-48c rate */
        case 0:
            /* Configure PRGM #1 for master mode */
            regAddr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
   	        result = sysHyPhy20gRead(fileHndl, regAddr, &prgmMonCfg);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12CSL_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_MSSLEN_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0x3);
            if (result) return result;

            result = sysHyPhy20gWrite(fileHndl, regAddr, prgmMonCfg);
            if (result) return result;

            /* Configure PRGMs #2-4 for slave mode */
            for (i = 1; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

   		        result = sysHyPhy20gRead(fileHndl, regAddr, &prgmMonCfg);
    	        if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	        if (result) return result;

		        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12CSL_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	        if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_MSSLEN_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0x3);
    	        if (result) return result;

    	        result = sysHyPhy20gWrite(fileHndl, regAddr, prgmMonCfg);
    	        if (result) return result;
            }

        break;

    	/* 4 x STS-12c rate */
        case 1:
             for (i = 0; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

   	            result = sysHyPhy20gRead(fileHndl, regAddr, &prgmMonCfg);
                if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	        if (result) return result;

		        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12CSL_MSK;
                result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0);
                if (result) return result;

                msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_MSSLEN_MSK;
                result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0x0);
    	        if (result) return result;

    	        result = sysHyPhy20gWrite(fileHndl, regAddr, prgmMonCfg);
    	        if (result) return result;
            }

        break;

        /* 16 x STS-3c rate or 48 x STS-1 */
        case 2:
            for (i = 0; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

   	            result = sysHyPhy20gRead(fileHndl, regAddr, &prgmMonCfg);
                if (result) return result;

    	        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
    	        result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0);
    	        if (result) return result;

		        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12CSL_MSK;
                result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0);
                if (result) return result;

                msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_MSSLEN_MSK;
                result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0x0);
    	        if (result) return result;

    	        result = sysHyPhy20gWrite(fileHndl, regAddr, prgmMonCfg);
    	        if (result) return result;
            }

        break;

        /* 48 x STS-1 rate */
        case 3:
            for (i = 0; i < 4; i++) {
                regAddr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
 	            regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, i);

 	            result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
 	            if (result)  return result;
            }

        break;

        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;

    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxSts48cPayloadCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxSts12PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures one of the four PRGM monitor blocks
**                payload for STS-12 signal for 1 x STS-12c, 4 x STS-3c, or
**                12 x STS-1s within the TSE subsystem.
**
**                After calling this function, hyPhy20gTsePrgmRxSts3PayloadCfg
**                can be called to further configure the payload for each
**                STS-3c.
**
**  INPUTS:       *fileHndl - base address
**                prgmNum   - STS-12 rate PRGM to query
**                            0 - 3
**                pldType   - 0 - 1 x STS-12c
**                          - 1 - 4 x STS-3c
**                          - 2 - 12 x STS-1
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_MASTER
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmRxSts12PayloadCfg(struct file *fileHndl, UINT4 prgmNum,
                                      UINT4 pldType)
{
    /* Variable declaration */
    UINT4 regAddr;
    UINT4 prgmMonCfg;
    UINT4 masterMode;
    UINT4 msk;
    UINT4 offset;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if ((prgmNum > 3) || (pldType > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register 0x493250C + 0x40*A(A=0:3) Monitor Payload
       Configuration */
    regAddr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
    regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, prgmNum);

    /* Check the master mode for selected payload type:
       if configuring for 4xSTS-3c or 12xSTS-1, the PRGM block must not be
       a part of concatenated STS-12c */
    if (pldType == 1 || pldType == 2) {
        msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
        offset = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_OFF;

        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &masterMode);
        if (result) return result;

        if (masterMode == 0x1)
            return HYPHY20G_ERR_INVALID_MASTER;
    }

    switch (pldType) {

        /* For STS-12c rate */
        case 0:
            /* Configure PRGM for master mode */
            result = sysHyPhy20gRead(fileHndl, regAddr, &prgmMonCfg);
            if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	    if (result) return result;

		    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12CSL_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_MSSLEN_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0x0);
    	    if (result) return result;

    	    result = sysHyPhy20gWrite(fileHndl, regAddr, prgmMonCfg);
    	    if (result) return result;

            break;

        /* For 4 x STS-3c */
        case 1:
   	        result = sysHyPhy20gRead(fileHndl, regAddr, &prgmMonCfg);
            if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0);
    	    if (result) return result;

		    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12CSL_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0);
            if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_MSSLEN_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 0x0);
    	    if (result) return result;

            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C3_MSK;
            result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	    if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C2_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	    if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C1_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	    if (result) return result;

    	    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C0_MSK;
    	    result = hyPhy20gVarFieldWrite(&prgmMonCfg, msk, 1);
    	    if (result) return result;

    	    result = sysHyPhy20gWrite(fileHndl, regAddr, prgmMonCfg);
    	    if (result) return result;

            break;

        /* 12 x STS-1s */
        case 2:
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
            if (result) return result;

            break;

        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;

    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxSts12PayloadCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxSts3PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures each STS-3 payload for PRGM monitor
**                 in a PRGM block within the TSE subsystem.
**
**                 This function needs to be called four times to configure all
**                 four STS-3 time slots in a PRGM block.
**
**  INPUTS:       *fileHndl - base address
**                prgmNum   - STS-12 rate PRGM to query
**                            0 - 3
**                sts3Num   - STS-3 rate payload inside a PRGM block
**                            0 - 3
**                pldType   - 0 - 1 x STS-3c
**                            1 - 3 x STS-1s
**                            Legal combinations of mixed payload can be:
**                                3 x STS-3c with 3 x STS-1s
**                                2 x STS-3c with 6 x STS-1s
**                                1 x STS-3c with 9 x STS-1s
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_MASTER
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmRxSts3PayloadCfg(struct file *fileHndl, UINT4 prgmNum,
                                     UINT4 sts3Num, UINT4 pldType)
{
    /* Variable declaration */
    UINT4 regAddr;
    UINT4 masterMode;
    UINT4 msk;
    UINT4 offset;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if ((prgmNum > 3) || (sts3Num > 3) || (pldType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register for monitoring payload configuration */
    regAddr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
    regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, prgmNum);

    /* Check the master mode for the selected payload type:
       Ensure each program block is not configured for STS-12c master mode */
    msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
    offset = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_OFF;

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &masterMode);
    if (result) return result;

    if (masterMode == 0x1)
        return HYPHY20G_ERR_INVALID_MASTER;

    /* Determine which STS-3 timeslot to configure */
    switch (sts3Num) {
        case 0:
            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C0_MSK;
            break;
        case 1:
            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C1_MSK;
            break;
        case 2:
            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C2_MSK;
            break;
        case 3:
            msk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS3C3_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure the STS-3 timeslot */
    switch (pldType) {
        /* For 1 x STS-3c rate */
        case 0:
 	        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
            if (result) return result;
            break;

       /* For 3 x STS-1s */
       case 1:
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0);
            if (result) return result;
            break;

       default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxSts3PayloadCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmTxPatternCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the Tx PRGM block to generate
**                 a PRBS or sequential (incrementing) pattern.
**
**                 Prior to calling this function,
**                 hyPhy20gTsePrgmTxSts48PayloadCfg,
**                 hyPhy20gTsePrgmTxSts12PayloadCfg, and
**                 hyPhy20gTsePrgmTxSts3PayloadCfg must be called to configure
**                 the payload type.
**
**                 When configuring the pattern type for STS-48c rate, calling
**                 this function with prgmNum = 0 and path =1 will configure the
**                 pattern type for all 4 PRGM blocks.
**
**
**  INPUTS:       *fileHndl     - base address
**                prgmNum       - STS-12 rate PRGM number
**                                0 to 3
**                path          - STS-1 path in a STS-12 PRGM
**                                1 to 12 for STS-1 path #1 to STS-1 path #12
**                rate          - Rate of PRGM signal to be generated
**                                0 - STS-48c
**                                1 - STS-12c
**                                2 - STS-3c
**                                3 - STS-1
**                patternType   - 0 - PRGM Generator is disabled
**                                1 - X^23+X^18+1 PRBS pattern
**                                2 - X^23+X^18+1 PRBS pattern is inverted
**                                3 - Sequential pattern
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
INT4 hyPhy20gTsePrgmTxPatternCfg(struct file *fileHndl, UINT4 prgmNum,
                                 UINT4 path, UINT4 rate, UINT4 patternType)
{
    /* Variable declaration */
    UINT4 iAddr;
    UINT4 genAccumAddr;
    UINT4 accumMsk;
    UINT4 aModeMsk;
    UINT4 enblMsk;
    UINT4 invMsk;
    UINT4 seqMsk;
    UINT4 prgmRegData;
    UINT4 i, j;
    UINT4 concatMsk, msMsk, mssLenMsk;
    UINT4 concatData, msData, mssLenData;
    UINT4 addr, cfgData;
    INT4 result;


    /* Argument checking */
    if ((prgmNum > 3) || (rate > 3) || (path > 12) || (patternType > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* For STS-48c rate */
    if ((rate == 0) && ((prgmNum != 0) || (path != 1)))
        return HYPHY20G_ERR_INVALID_ARG;

    /* For STS-12c rate */
    if ((rate == 1) && (path != 1))
        return HYPHY20G_ERR_INVALID_ARG;

    /* For STs-3c rate */
    if ((rate == 2) && (path > 4))
        return HYPHY20G_ERR_INVALID_ARG;

    /* Check payload configuration status */
    addr = HYPHY20G_TSE_PRGM_N_REG_GNRTR_PYLD_CFG;
    addr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(addr, prgmNum);

    concatMsk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12C_MSK;
    msMsk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_STS12CSL_MSK;
    mssLenMsk = HYPHY20G_TSE_PRGM_N_BIT_GNRTR_PYLD_CFG_GEN_MSSLEN_MSK;

    if (rate == 0) { /* STS-48c rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, concatMsk, &concatData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, msMsk, &msData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, mssLenMsk, &mssLenData);
        if (result) return result;

        if (concatData != 1|| msData != 0 || mssLenData != 0x3)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
     } else if (rate == 1) { /* STS-12c rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, concatMsk, &concatData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, msMsk, &msData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, mssLenMsk, &mssLenData);
        if (result) return result;

        if (concatData != 1|| msData != 0 || mssLenData != 0x0)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    } else if (rate == 2) { /* STS-3c rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, concatMsk, &concatData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, msMsk, &msData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, mssLenMsk, &mssLenData);
        if (result) return result;

        if (concatData != 0|| msData != 0 || mssLenData != 0x0)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    } else { /* STS-1 rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        if (cfgData != 0x0)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Determine registers and masks */
    iAddr = 0x8; /* Generator STS-1 Path Configuration */
    genAccumAddr = 0x9; /* Generator PRBS_22:7 Accumulator */

    /* Unconditionally enable GEN_AMODE for proper operation
       and initialize generator accumulator to a non zero value */
    aModeMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_GNRTR_STS_1_PATH_CFG_GEN_AMODE_MSK;
    accumMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_GNRTR_PRBS_22_7_ACCM_GEN_PRBS_MSK;

    /* Rates other than STS-48c, enable for only one PRGM block */

        for (i = 1; i < 13; i++) {
            result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                            &prgmRegData);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&prgmRegData, aModeMsk, 1);
            if (result) return result;

            result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr, i,
                                             prgmRegData);
            if (result) return result;

            /* Set the PRBS to a non-zero value in the first STS-1 path */
            result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, genAccumAddr, i,
                                            &prgmRegData);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&prgmRegData, accumMsk, 0x0001);
            if (result) return result;

            result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, genAccumAddr, i,
                                             prgmRegData);
            if (result) return result;
        }


    /* Determine masks for Pattern type */
    enblMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_GNRTR_STS_1_PATH_CFG_LINKENA0_MSK;
    seqMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_GNRTR_STS_1_PATH_CFG_GEN_SEQ_PRBSB_MSK;
    invMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_GNRTR_STS_1_PATH_CFG_GEN_INV_PRBS_MSK;

    if (rate == 0) {
        /* For STS-48c */
        switch (patternType) {
            /* PRGM Generator is disabled */
            case 0:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {
                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }

            break;

            /* X^23+X^18+1 PRBS pattern */
            case 1:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {

                        result = tseHyPhy20gPRGMIndRead(fileHndl, i,
                                                        genAccumAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, accumMsk,
                                                       0x0001);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i,
                                                         genAccumAddr, j,
                                                         prgmRegData);

                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, aModeMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }
            break;

            /* PRBS pattern is inverted */
            case 2:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {
                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, aModeMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }

                break;
            /* Sequential pattern */
            case 3:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {
                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, aModeMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }

            break;

            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;

        }
     }
     /* For STS-12c rate */
     else if (rate == 1) {
        /* Configure PRBS pattern type to be generated */
        switch (patternType) {
            /* PRGM Generator is disabled */
            case 0:
                for (i = 1; i < 13; i++) {
                result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                &prgmRegData);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 0);
                if (result) return result;

                result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr, i,
                                                 prgmRegData);
                if (result) return result;
            }

            break;
            /* X^23+X^18+1 PRBS pattern */
            case 1:
                for (i = 1; i < 13; i++) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }
                break;
            /* PRBS pattern is inverted */
            case 2:
                for (i = 1; i < 13; i++) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

                break;
            /* Sequential pattern */
            case 3:
                for (i = 1; i < 13; i++) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

                break;

            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    /* For STS-3c rate */
    else if (rate == 2) {
        /* Configure PRBS pattern type to be generated */
        switch (patternType) {
            /* PRGM Generator is disabled */
            case 0:
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

            break;
            /* X^23+X^18+1 PRBS pattern */
            case 1:
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }
            break;
            /* X^23+X^18+1 PRBS pattern inverted */
            case 2:
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

            break;
            /* Sequential pattern */
            case 3:
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

            break;

            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    /* For STS-1 rate */
    else {
        /* Configure PRBS pattern type to be generated */
        result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, path,
                                        &prgmRegData);
        if (result) return result;
        switch (patternType) {
            /* PRGM Generator is disabled */
            case 0:
                result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 0);
                if (result) return result;

            break;
            /* X^23+X^18+1 PRBS pattern */
            case 1:
                result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                if (result) return result;

            break;
            /* X^23+X^18+1 PRBS pattern inverted */
            case 2:
               result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
               if (result) return result;

               result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
               if (result) return result;

               result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
               if (result) return result;

            break;
            /* Sequential pattern */
            case 3:
                result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                if (result) return result;

            break;

            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
        result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr, path,
                                         prgmRegData);

        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmTxPatternCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxPatternCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the Rx PRGM block to monitor
**                 a PRBS or sequential (incrementing) pattern.
**
**                 Prior to calling this function,
**                 hyPhy20gTsePrgmRxSts48PayloadCfg,
**                 hyPhy20gTsePrgmRxSts12PayloadCfg, and
**                 hyPhy20gTsePrgmRxSts3PayloadCfg must be called to configure
**                 the payload type.
**
**                 When configuring the pattern type for STS-48c rate, calling
**                 this function with prgmNum = 0 and path = 1 will configure
**                 the pattern type for all 4 PRGM blocks.
**
**
**  INPUTS:       *fileHndl     - base address
**                prgmNum       - STS-12 rate PRGM number
**                                0 to 3
**                path          - STS-1 path in a STS-12 PRGM
**                                1 to 12 for STS-1 path #1 to STS-1 path #12
**                rate          - Rate of PRGM signal to be monitored
**                                0 - STS-48c
**                                1 - STS-12c
**                                2 - STS-3c
**                                3 - STS-1
**                patternType   - 0 - PRGM Monitor is disabled
**                                1 - X^23+X^18+1 PRBS pattern
**                                2 - X^23+X^18+1 PRBS pattern is inverted
**                                3 - Sequential pattern
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
INT4 hyPhy20gTsePrgmRxPatternCfg(struct file *fileHndl, UINT4 prgmNum,
                                 UINT4 path, UINT4 rate, UINT4 patternType)
{
    /* variable declaration */
    UINT4 iAddr;
    UINT4 addr;
    UINT4 cfgData;
    UINT4 monAccumAddr;
    UINT4 enblMsk;
    UINT4 seqMsk;
    UINT4 invMsk;
    UINT4 prgmRegData;
    UINT4 i, j;
    UINT4 concatMsk, msMsk, mssLenMsk;
    UINT4 concatData, msData, mssLenData;
    INT4 result;

    /* argument checking */
    if ((prgmNum > 3) || (rate > 3) || (path > 12) || (patternType > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    DBG_PRINT("Testing new driver");
    /* For STS-48c rate */
    if ((rate == 0) && ((prgmNum != 0) || (path != 1)))
        return HYPHY20G_ERR_INVALID_ARG;

    /* For STS-12c rate */
    if ((rate == 1) && (path != 1))
        return HYPHY20G_ERR_INVALID_ARG;

    /* For STs-3c rate */
    if ((rate == 2) && (path > 4))
        return HYPHY20G_ERR_INVALID_ARG;

    /* Check payload configuration status */
    addr = HYPHY20G_TSE_PRGM_N_REG_MON_PYLD_CFG;
    addr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(addr, prgmNum);

    concatMsk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12C_MSK;
    msMsk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_STS12CSL_MSK;
    mssLenMsk = HYPHY20G_TSE_PRGM_N_BIT_MON_PYLD_CFG_MON_MSSLEN_MSK;

    if (rate == 0) { /* STS-48c rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, concatMsk, &concatData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, msMsk, &msData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, mssLenMsk, &mssLenData);
        if (result) return result;

        if (concatData != 1|| msData != 0 || mssLenData != 0x3)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
     } else if (rate == 1) { /* STS-12c rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, concatMsk, &concatData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, msMsk, &msData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, mssLenMsk, &mssLenData);
        if (result) return result;

        if (concatData != 1|| msData != 0 || mssLenData != 0x0)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    } else if (rate == 2) { /* STS-3c rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, concatMsk, &concatData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, msMsk, &msData);
        if (result) return result;

        result = hyPhy20gVarFieldRead(cfgData, mssLenMsk, &mssLenData);
        if (result) return result;

        if (concatData != 0|| msData != 0 || mssLenData != 0x0)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    } else { /* STS-1 rate */
        result =  sysHyPhy20gRead (fileHndl, addr, &cfgData);
        if (result) return result;

        if (cfgData != 0x0)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Determine registers and masks */
    iAddr = 0x0; /* Monitor STS-1 Path Configuration */
    monAccumAddr = 0x1; /* Monitor PRBS_22:7 Accumulator */

    /* Determine masks for pattern types */
    enblMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_MON_STS_1_PATH_CFG_MON_ENA_MSK;
    seqMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_MON_STS_1_PATH_CFG_MON_SEQ_PRBSB_MSK;
    invMsk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_MON_STS_1_PATH_CFG_MON_INV_PRBS_MSK;

    if (rate == 0) {
        /* For STS-48c, configure all 12 STS-1 paths in 4 PRGM blocks */
        switch (patternType) {
            /* PRGM Generator is disabled */
            case 0:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {
                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }

            break;

            /* X^23+X^18+1 PRBS pattern */
            case 1:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {
                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }
            break;
            /* PRBS pattern is inverted */
            case 2:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {
                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }

                break;
            /* Sequential pattern */
            case 3:
                for (i = 0; i < 4; i++) {
                    for (j = 1; j < 13; j++) {
                        result = tseHyPhy20gPRGMIndRead(fileHndl, i, iAddr, j,
                                                        &prgmRegData);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk,
                                                       1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                        if (result) return result;

                        result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                        if (result) return result;

                        result = tseHyPhy20gPRGMIndWrite(fileHndl, i, iAddr, j,
                                                         prgmRegData);
                        if (result) return result;
                    }
                }

            break;
            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
     }
     /* For STS-12c rate, configure all 12 STS-1 path in the specified PRGM
        block */
     else if (rate == 1) {
        /* Configure PRBS pattern type to be generated */
        switch (patternType) {
            /* PRGM Generator is disabled */
            case 0:
                for (i = 1; i < 13; i++) {
                result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                &prgmRegData);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 0);
                if (result) return result;

                result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr, i,
                                                 prgmRegData);
                if (result) return result;
            }

            break;
            /* X^23+X^18+1 PRBS pattern */
            case 1:
                for (i = 1; i < 13; i++) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }
                break;
            /* PRBS pattern is inverted */
            case 2:
                for (i = 1; i < 13; i++) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

                break;
            /* Sequential pattern */
            case 3:
                for (i = 1; i < 13; i++) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

                break;

            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    /* For STS-3c rate, configure 1 STS-3c signal in the specified STS-3c
       location */
    else if (rate == 2) {
        /* Configure PRBS pattern type to be generated */
        switch (patternType) {
            /* PRGM Generator is disabled */
            case 0:
                /* valid master timeslots are #1, #2, #3, or #4 for each STS-3c
                   in a PRGM block */
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

            break;
            /* X^23+X^18+1 PRBS pattern */
            case 1:
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }
            break;
            /* X^23+X^18+1 PRBS pattern is inverted */
            case 2:
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

            break;
            /* Sequential pattern */
            case 3:
                for (i = path; i < path+9; i=i+4) {
                    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, i,
                                                    &prgmRegData);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr,
                                                     i, prgmRegData);
                    if (result) return result;
                }

            break;

            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    /* For STS-1 rate, configure one specified STS-1 path */
    else {
        /* Configure PRBS pattern type to be generated */
        result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, path,
                                        &prgmRegData);
            if (result) return result;
            switch (patternType) {
                /* PRGM Generator is disabled */
                case 0:
                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 0);
                    if (result) return result;

                break;
                /* X^23+X^18+1 PRBS pattern */
                case 1:
                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                break;
                /* X^23+X^18+1 PRBS pattern is inverted */
                case 2:
                   result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                   if (result) return result;

                   result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 1);
                   if (result) return result;

                   result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 0);
                   if (result) return result;

                break;
                /* Sequential pattern */
                case 3:
                    result = hyPhy20gVarFieldWrite(&prgmRegData, enblMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, seqMsk, 1);
                    if (result) return result;

                    result = hyPhy20gVarFieldWrite(&prgmRegData, invMsk, 0);
                    if (result) return result;

                break;

                default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
        result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr, path,
                                         prgmRegData);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxPatternCfg */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxErrCnt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function checks the error count within the PRGM monitor
**                 for a selected STS-1 path within the TSE subsystem. For
**                 concatenated signals, the master STS-1 time slot should be
**                 addressed.
**
**  INPUTS:       *fileHndl     - base address
**                prgmNum       - STS-12 rate PRGM to query
**                                0 - 3
**                path          - STS-1 path to issue resync command
**                                1 - 12 for STS-1 path #1 to STS-1 path #12
**                accessMode    - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                *errCnt       - Pointer to 16-bit error count.
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmRxErrCnt(struct file *fileHndl, UINT4 prgmNum,
                             UINT4 path, UINT4 accessMode, UINT4 *errCnt)
{
    /* variable declaration */
    UINT4 iAddr;
    UINT4 regAddr;
    UINT4 msk;
    UINT4 prgmRegData;
    UINT4 errorCount;
    INT4 result;

    /* argument checking */
    if ((prgmNum > 3) || (path > 12) || (accessMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Trigger access locally at 0x4932530 + 0x40*A(A=0:3) Performance Counters
       Transfer Trigger */
    regAddr = HYPHY20G_TSE_PRGM_N_REG_PERF_CNT_XFER_TRIG;
    regAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(regAddr, prgmNum);
    msk = HYPHY20G_TSE_PRGM_N_BIT_PERF_CNT_XFER_TRIG_TIP_MSK;

    if (accessMode == 1) {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    }

    iAddr = 0x4; /* Monitor Error Count */
    msk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_MON_ERR_CNT_ERR_CNT_MSK;

    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, path,
                                    &prgmRegData);
    if (result) return result;

    result = hyPhy20gVarFieldRead(prgmRegData, msk, &errorCount);
    if (result) return result;

    *errCnt = errorCount;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxErrCnt */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxResync
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function will issue a 'resync' command to a PRGM STS-1
**                 path monitor within the TSE subsystem.  For concatenated
**                 payloads, the resync command should be issued to the master
**                 STS-1 slot.
**
**
**  INPUTS:       *fileHndl     - base address
**                prgmNum       - STS-12 rate PRGM to issue resync command
**                                0 - 3
**                path          - STS-1 path to issue resync command
**                                1 - 12 for STS-1 path #1 to STS-1 path #12
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmRxResync(struct file *fileHndl, UINT4 prgmNum, UINT4 path)
{
    /* variable declaration */
    UINT4 iAddr;
    UINT4 msk;
    UINT4 pathCfgRegData;
    INT4 result;

    /* argument checking */
    if ((prgmNum > 3) || (path > 12)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddr = 0x0; /* Monitor STS-1 Path Configuration */
    msk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_MON_STS_1_PATH_CFG_RESYNC_MSK;

    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, path,
                                    &pathCfgRegData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&pathCfgRegData, msk, 1);
    if (result) return result;

    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr, path,
                                     pathCfgRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxResync */

/*******************************************************************************
**
**  hyPhy20gTsePrgmTxErrIns
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function will inject a single error on a PRGM STS-1
**                 path generator within the TSE subsystem.  The error is
**                 injected by inverting the MSB of the next byte to be sent.
**
**  INPUTS:       *fileHndl     - base address
**                prgmNum       - STS-12 rate PRGM to issue error insert command
**                              - 0 - 3
**                path          - STS-1 path to issue error insert command
**                                1 - 12 for STS-1 path #1 to STS-1 path #12
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmTxErrIns(struct file *fileHndl, UINT4 prgmNum, UINT4 path)
{
    /* variable declaration */
    UINT4 iAddr;
    UINT4 msk;
    UINT4 pathCfgRegData;
    INT4 result;

    /* argument checking */
    if ((prgmNum > 3) || (path > 12)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddr = 0x8; /* Generator STS-1 Path Configuration */
    msk = HYPHY20G_TSE_PRGM_N_CFG_AND_STAT_IBIT_GNRTR_STS_1_PATH_CFG_FORCE_ERR_MSK;

    result = tseHyPhy20gPRGMIndRead(fileHndl, prgmNum, iAddr, path,
                                    &pathCfgRegData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&pathCfgRegData, msk, 1);
    if (result) return result;

    result = tseHyPhy20gPRGMIndWrite(fileHndl, prgmNum, iAddr, path,
                                     pathCfgRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmTxErrIns */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxPortSel
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the selected TSE input port to be
**                 routed to the PRGM monitor.  The PRGM monitor consists of
**                 4 PRGM blocks that can process upto an STS-48c signal.  It
**                 may also process up to 48 x STS-1 signals with independently
**                 configured pattern types.
**
**  INPUTS:       *fileHndl     - base address
**                 prgmSrc        0  to  3 - SONET_SLICE 0 to 3
**                                4  to  7 - SONET_SLICE 4 to 7
**                                8  to 11 - SONET_SLICE 8 to 11
**                                12 to 15 - SONET_SLICE 12 to 15
**                                16 to 19 - HYPHY_HOPP slice 0 to 3
**                                20 to 23 - DMI Slice 0 port 0 to 3
**                                24 to 27 - DMI Slice 1 port 0 to 3
**                                28 to 31 - DMI Slice 2 port 0 to 3
**                                32 to 35 - DMI Slice 3 port 0 to 3
**                                36 to 39 - DMI Slice 4 port 0 to 3
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmRxPortSel(struct file *fileHndl, UINT4 prgmSrc)
{
    INT4 result;
    UINT4 regAddr, mask;

   /* argument checking */
    if (prgmSrc > 39) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_TSE_TOP_REG_MUX_PRT_SEL;
	mask = HYPHY20G_TSE_TOP_BIT_MUX_PRT_SEL_TST_IN_SEL_MSK;	
	result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, prgmSrc);	
	if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxPortSel */

/*******************************************************************************
**
**  hyPhy20gTsePrgmRxStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function checks the status of the the PRGM monitor
**                 within the TSE subsystem.   It returns the status for the
**                 12 STS-1 signals that may be processed by a PRGM slice.  For
**                 concatenated signals, the status for the master timeslot is
**                 valid.
**
**  INPUTS:       *fileHndl     - base address
**                prgmNum       - STS-12 rate PRGM to query
**                              - 0 - 3
**                intClear      - 1 - all MON_SYNC_I interrupt bits will be 
**                                    cleared by reading the interrupt register
**                              - 0 - all MON_SYNC_I interrupt bits will be left 
**                                    untouched.
**                *statusInts   - Pointer to storage for STS-1 status interrupts
**                              - bits 12:1 - MON_SYNC_I[12:1]
**                *statusVals   - Pointer to storage for STS-1 status values
**                              - bits 12:1 - MON_SYNC_V[12:1]
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrgmRxStatus(struct file *fileHndl, UINT4 prgmNum, 
                             UINT4 intClear, UINT4 *statusInts, 
                             UINT4 *statusVals)
{
    /* variable declaration */
    UINT4 intStatRegAddr, addr, off;
    UINT4 statRegAddr;
    UINT4 prgmIntStatRegData;
    UINT4 prgmStatRegData;
    INT4 result;

    /* argument checking */
    if (prgmNum > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    addr = HYPHY20G_PGMRCLK_REG_WCIMODE_CTL_3;
    off = HYPHY20G_PGMRCLK_BIT_WCIMODE_CTL_3_WCI_MODE_CTRL_OFF + 2;
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, 0);
        if (result) return result;
    }       

    intStatRegAddr = HYPHY20G_TSE_PRGM_N_REG_MON_SYNC_INT_STAT;
    intStatRegAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(intStatRegAddr,
                                                              prgmNum);
    statRegAddr = HYPHY20G_TSE_PRGM_N_REG_MON_SYNC_STAT;
    statRegAddr = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(statRegAddr,
                                                           prgmNum);

    result = sysHyPhy20gRead(fileHndl, intStatRegAddr, &prgmIntStatRegData);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, statRegAddr, &prgmStatRegData);
    if (result) return result;

    *statusInts = prgmIntStatRegData;
    *statusVals = prgmStatRegData;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrgmRxStatus */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
