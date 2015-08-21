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
**  FILE        : hyphy20g_util_scbs.c
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION : Contains the HyPHY 20G utility functions.
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_util_scbs.h"

/******************************************************************************/
/* PUBLIC FUNCTIONS - SCBS Operations called from APIs                        */
/******************************************************************************/
/*******************************************************************************
**
**  utilHyPhy20gScbsInitSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes one of the schedulers contained in the
**                  HyPHY 20G device.  For single group schedulers (CPB, PIF,
**                  OPI) the length parameter specifies the number of scheduler
**                  rows the device will utilize for generating scheduling
**                  data.  For multi group instances (GFP) length parameter
**                  specifies the maximum number of rows the multiple groups may
**                  utilize.
**
**                  Single group instances will configure start_addr and
**                  end_addr to span all available to calendar rows.
**
**                  Multi group instances will configure start_addr and
**                  end_addr to point to the last row of scheduler.
**
**                  Higher level calling function may initialize other pSchd
**                  structure parameters.
**
**                  All calendar entries are initialized to the NULL address.
**
**                  This function allocates memory for maintaining a copy
**                  of online and offline scheduler pages and memory to
**                  track context for scheduler groups.
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the context associated with the
**                              scheduler being initialized
**                  schdId    - enumerated type identifying specific scheduler
**                              being initialized
**                  length    - number of 4 entry scheduler addresses
**                            - Legal ranges for CPB write schedulers:
**                                Line Interace - 1 to 32
**                                GFP A and B Interfaces - 1 to 32
**                                PIF Interface - No Write Scheduler
**                            - Legal range for CPB read schedulers:
**                                Line Interace - 1 to 16
**                                GFP A and B Interfaces - 1 to 16
**                                PIF Interface - 1 to 64
**                            - Legal range for CPB QDR Scheduler
**                                1 to 15
**                            - Legal range for PIF Tx Scheduler
**                                1 to 128
**                            - Legal range for GFP Tx Scheduler
**                                1 to 127
**                            - Legal range for OPI Scheduler
**                                1 to 2
**                  schdMode  - scheduling algorithm to be used by SCBS block
**                              Bonus mode may be enabled or disabled.
**                                HYPHY20G_SCBS_SCHD_BONUS_MODE_ENABLE = 0
**                                HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE = 1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsInitSchd(struct file *fileHndl, sHYPHY20G_SCHD_CTXT *pSchd,
                              eHYPHY20G_SCBS_SCHD_ID schdId, UINT4 length,
                              UINT4 schdMode)
{
    UINT4 *pCalData;
    UINT4 startAddr = 0, endAddr = length - 1;
    UINT4 size;
    UINT4 maxNumGrp;
    UINT4 reset, workPgSel, i, j, maxNumRow, maxChnl;
    UINT4 portOffset, intfGrpDir;
    INT4 result;

    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

DBG_PRINT("file %s, line %d, schdId=%d, length=%ld, schdMode=%d\n", __FILE__, __LINE__, (unsigned int)schdId, (long int)length, (unsigned int)schdMode);

    /* retrieve maximum number of rows, channels and groups
       supported by the scheduler */
    switch(schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            maxNumRow = HYPHY20G_CPB_LINE_WR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_WR_DIR;
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_LINE_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_LINE_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPA_WR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_WR_DIR;
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPA_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPA_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPB_WR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPB_WR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_WR_DIR;
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPB_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPB_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_PIF_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            maxNumRow = HYPHY20G_CPB_QDR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_QDR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            maxNumRow = HYPHY20G_PIF_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_PIF_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_PIF_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            maxNumRow = HYPHY20G_PIF_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_PIF_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_PIF_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            maxNumRow = HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_GFP_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_GFP_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            maxNumRow = HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_GFP_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_GFP_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            maxNumRow = HYPHY20G_OPI_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_OPI_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_OPI_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;
            break;
        case HYPHY20G_OPI_SCHD_1:
            maxNumRow = HYPHY20G_OPI_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_OPI_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_OPI_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;
            break;
        default:
            DBG_PRINT("file %s, line %d\n", __FILE__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (length < 1 || length > maxNumRow || schdMode > 1) {
        DBG_PRINT("file %s, line %d\n", __FILE__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (pSchd->pOffLineCalMem   != NULL ||
        pSchd->pOnLineCalMem    != NULL ||
        pSchd->pScbsGrpCtxt     != NULL) {
        DBG_PRINT("file %s, line %d\n", __FILE__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* allocate memory for use by scheduler and reference to pSchd pointers */
    result = utilHyPhy20gScbsCreateSchd(pSchd, length, maxNumGrp);
    if(result) return result;

    /* allocate local memory for initializing scheduler contents */
    size = sizeof(UINT4) * length * HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW;
    pCalData = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCalData == NULL) {
        utilHyPhy20gScbsDestroySchd(pSchd);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* configure parameters in pSchd to reflect configuration */
    /* Note: */
    pSchd->schdId = schdId;
    pSchd->numGrp = maxNumGrp;
    pSchd->schdMode = schdMode;
    pSchd->numRows = length;
    pSchd->maxChnl = maxChnl;
    // JCh - added the initialization of the following parameters
    pSchd->portOffset = portOffset;
    pSchd->intfGrpDir = intfGrpDir;


    /* put scheduler in reset and set scheduling mode */
    reset = 1;
    workPgSel = 0;
    result = utilHyPhy20gScbsWriteSchdCfg(fileHndl, pSchd, reset, schdMode,
                                          workPgSel);

    if (result) {
        sysHyPhy20gMemFree(pCalData);
        utilHyPhy20gScbsDestroySchd(pSchd);
        return result;
    }

    /*
     * Write Scheduler Context Memory
     */
    /* set startAddr and endAddr to span all possible rows for single group
       schedulers */
    if(maxNumGrp == 1){
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, 0,
                                               startAddr, endAddr);
        if (result) {
            sysHyPhy20gMemFree(pCalData);
            utilHyPhy20gScbsDestroySchd(pSchd);
            return result;
        }
    /* set startAddr and endAddr to point to the last row for multi group
       schedulers */
    } else {

        startAddr = maxNumRow;
        endAddr = maxNumRow;
        for (i = 0; i < maxNumGrp; i++) {
            result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, i,
                                                   startAddr, endAddr);
            if (result) {
                sysHyPhy20gMemFree(pCalData);
                utilHyPhy20gScbsDestroySchd(pSchd);
                return result;
            }
        }
    }


    /* write all used entries of online and offline scheduler to 0xFF */
    for (i = 0; i < length; i++) {
        for (j = 0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            *(pCalData + (HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW * i) + j) =
                HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
            DBG_PRINT("%s, %s, %d, numRows %u, row %u, column %u chnlData %u \n",__FILE__, __FUNCTION__, __LINE__,(unsigned int)length,(unsigned int)i,(unsigned int)j,(unsigned int)*(pCalData + (HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW*i) + j));
        }
    }


    result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
    if (result) {
        sysHyPhy20gMemFree(pCalData);
        utilHyPhy20gScbsDestroySchd(pSchd);
        return result;
    }
    pSchd->valid = HYPHY20G_VALID_SIGN;

    /* take scheduler out of reset */
    reset = 0;
    workPgSel = 0;
    result = utilHyPhy20gScbsWriteSchdCfg(fileHndl, pSchd, reset, schdMode,
                                          workPgSel);
    if (result) {
        sysHyPhy20gMemFree(pCalData);
        utilHyPhy20gScbsDestroySchd(pSchd);
        return result;
    }

    sysHyPhy20gMemFree(pCalData);
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsInitSchd */

/*******************************************************************************
**
**  utilHyPhy20gScbsCreateSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates dynamic memory used by pSchd.
**                  Allocates memory to track scheduler calendar data for
**                  online and offline pages and memory to track data for
**                  each scheduler group.
**
**
**
**  INPUTS:         pSchd     - pointer to the scheduler to be accessed
**                  length    - number of rows to be used by scheduler
**                  numGrp    - number of scheduler groups supported
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsCreateSchd(sHYPHY20G_SCHD_CTXT *pSchd, UINT4 length,
                                UINT4 numGrp)
{
    UINT4 size;

    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* allocate memory for required number of groups */
    size = sizeof(sHYPHY20G_SCHD_GRP_CTXT) * numGrp;
    pSchd->pScbsGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *) sysHyPhy20gMemAlloc(size);
    if (pSchd->pScbsGrpCtxt == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pSchd->sizeScbsGrpCtxt = size;
	
    /* allocate memory for online and offline calendar pages */
    /* allocate memory to track scheduler online and offline pages */
    size = sizeof(UINT4) * length * HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW;
    pSchd->pOnLineCalMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
	if (pSchd->pOnLineCalMem == NULL) {
        /* deallocate memory allocated up to this point in function */
        sysHyPhy20gMemFree(pSchd->pScbsGrpCtxt);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pSchd->sizeOnLineCalMem = size;
	
    pSchd->pOffLineCalMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pSchd->pOffLineCalMem == NULL) {
        /* deallocate memory allocated up to this point in function */
        sysHyPhy20gMemFree(pSchd->pScbsGrpCtxt);
        sysHyPhy20gMemFree(pSchd->pOnLineCalMem);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pSchd->sizeOffLineCalMem = size;
	
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsCreateSchd */

/*******************************************************************************
**
**  utilHyPhy20gScbsDestroySchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Free dynamic memory used by pSchd
**
**
**
**  INPUTS:         pSchd     - pointer to the scheduler to be accessed
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsDestroySchd(sHYPHY20G_SCHD_CTXT *pSchd)
{
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    if (pSchd->pScbsGrpCtxt != NULL) {
        sysHyPhy20gMemFree(pSchd->pScbsGrpCtxt);
    }

    pSchd->sizeScbsGrpCtxt = 0;

    if (pSchd->pOffLineCalMem != NULL) {
        sysHyPhy20gMemFree(pSchd->pOffLineCalMem);
    }

    pSchd->sizeOffLineCalMem = 0;

    if (pSchd->pOnLineCalMem != NULL) {
        sysHyPhy20gMemFree(pSchd->pOnLineCalMem);
    }

    pSchd->sizeOnLineCalMem = 0;
    
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsDestroySchd */

/*******************************************************************************
**
**  utilHyPhy20gScbsResetSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Free memory used to track scheduler context.  Puts the
**                  scheduler block in reset at local registers.
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd       - pointer to the scheduler to be accessed
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsResetSchd(struct file *fileHndl,
                               sHYPHY20G_SCHD_CTXT *pSchd)
{
    UINT4 reset, workPgSel;
    INT4 result;

    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* put scheduler in reset */
    reset = 1;
    workPgSel = 0;
    
    result = utilHyPhy20gScbsWriteSchdCfg(fileHndl, pSchd, reset,
                                          pSchd->schdMode, workPgSel);
                                          
    DBG_PRINT("%s, %s, %d, reset = %u, workPgSel = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)reset, (unsigned int)workPgSel);                                            
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
        return result;
    }          

    /* Write Scheduler Context Memory */
    result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, 0, 0, 0);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
        return result;
    }          

    /* free memory  */
    result = utilHyPhy20gScbsDestroySchd(pSchd);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
        return result;
    }          
    pSchd->numRows = 0;
    pSchd->valid = HYPHY20G_INVALID_VAL_32BIT;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsResetSchd */

/******** FUNCTIONS TO UPDATE SCHEDULER CALENDAR MEMORY WITH USER DATA ********/

/*******************************************************************************
**
**  utilHyPhy20gScbsUpdateCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Updates the online and offline scheduler pages to reflect
**                  the calendar data passed to the function.  The calendar
**                  data passed to this function will be activated for this
**                  scheduler and used by the device.
**
**  INPUTS:         fileHndl    - device handle
**                  pSchd       - pointer to the scheduler to be accessed
**                  pCalData    - pointer to a (UINT4) array that is calendar
**                                length by 4.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsUpdateCal(struct file *fileHndl,
                               sHYPHY20G_SCHD_CTXT *pSchd, UINT4 *pCalData)
{
    INT4 result;

    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    result = utilHyPhy20gScbsWriteCalOffLinePage(fileHndl, pSchd, pCalData);
    if (result) return result;

    result = utilHyPhy20gScbsSwitchActiveCalPage(fileHndl, pSchd);
    if (result) return result;

    result = utilHyPhy20gScbsWriteCalOffLinePage(fileHndl, pSchd, pCalData);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsUpdateCal */

/*******************************************************************************
**
**  utilHyPhy20gScbsWriteCalOffLinePage
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Writes the entire offline calendar page with values passed
**                  to function.
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**                  pCalData  - pointer to a (UINT4) array that is calendar
**                              length by 4.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsWriteCalOffLinePage(struct file *fileHndl,
                                         sHYPHY20G_SCHD_CTXT *pSchd,
                                         UINT4 *pCalData)
{
    UINT4 data[4];
    UINT4 i, j, maxChnl, chnlData;
    INT4 result;

    /* argument checking */
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    maxChnl = pSchd->maxChnl;
    for(i=0; i < pSchd->numRows; i++) {
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            chnlData = *(pCalData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
            if ((chnlData != HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) &&
                (chnlData >= maxChnl)) {
                DBG_PRINT("%s, %s, %d, %u, %u, %u, %u, %u, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__,(unsigned int)maxChnl,(unsigned int)pSchd->numRows,(unsigned int)i,(unsigned int)j,(unsigned int)chnlData);return HYPHY20G_ERR_INVALID_ARG;
            }
        }
    }


    /* apply scheduler configuration to offline page */
    for (i=0; i < pSchd->numRows; i++) {
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
             HYPHY20G_ASSERT((pSchd->pOffLineCalMem != NULL), HYPHY20G_ERR_INVALID_PRECONFIG,
                    "%s", "pSchd->pOffLineCalMem is NULL.");
                    
            data[j] = *(pCalData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
            *(pSchd->pOffLineCalMem + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j) =
                             *(pCalData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
        }

        result = utilHyPhy20gScbsQuadWriteSchdCal(fileHndl, pSchd, i, data[0],
                                                  data[1], data[2], data[3]);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsWriteCalOffLinePage */

/*******************************************************************************
**
**  utilHyPhy20gScbsSwitchActiveCalPage
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Switches the offline page to the online page for a
**                  specified scheduler.
**
**
**  INPUTS:         fileHndl    - device handle
**                  pSchd       - pointer to the scheduler to be accessed
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsSwitchActiveCalPage(struct file *fileHndl,
                                         sHYPHY20G_SCHD_CTXT *pSchd)
{

    UINT4 reset, schdMode, workPgSel;
    UINT4 entry1, entry2, entry3, entry4;
    UINT4 *ptr;
    INT4 result;

    /* argument checking */
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* retrieve configuration register settings */
    result = utilHyPhy20gScbsReadSchdCfg(fileHndl, pSchd, &reset, &schdMode,
                                         &workPgSel);
    if (result) return result;

    /* supplementary procedure to update QDR and GFP scheduler context */
    if (pSchd->schdId == HYPHY20G_CPB_QDR_SCHD  ||
        pSchd->schdId == HYPHY20G_GFP_TX_SCHD_0 ||
        pSchd->schdId == HYPHY20G_GFP_TX_SCHD_1) {

        /* After scheduling in the standby page, prior to a page switch,
           write the next (unused) word with all NULL channels (0xFFFFFFFF). */
        result = utilHyPhy20gScbsQuadWriteSchdCal(fileHndl, pSchd,
                                        pSchd->numRows,
                                        HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER,
                                        HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER,
                                        HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER,
                                        HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER);
        if (result) return result;

        // Read back the word.
        result = utilHyPhy20gScbsQuadReadSchdCal(fileHndl, pSchd,
                            pSchd->numRows, &entry1, &entry2, &entry3, &entry4);
        if (result) return result;
    }

    /* write configuration register settings with value of workPgSel inverted */
    workPgSel = (workPgSel == 1) ? 0:1;

    result = utilHyPhy20gScbsWriteSchdCfg(fileHndl, pSchd, reset, schdMode,
                                          workPgSel);
    if (result) return result;

    /* update context associated with online and offline pages */
    // GB->JXu - these pointers are our hooks to free memory used to
    // maintain the contents of schedulers.  Does this create potential
    // for a memory leak?
    ptr = pSchd->pOnLineCalMem;
    pSchd->pOnLineCalMem = pSchd->pOffLineCalMem;
    pSchd->pOffLineCalMem = ptr;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsSwitchActiveCalPage */

/*******************************************************************************
**
**  utilHyPhy20gScbsUpdateCalRows
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Updates the online and offline scheduler pages to reflect
**                  the rows of calendar data passed to the function.  The
**                  calendar row data passed to this function will be activated
**                  for this scheduler and used by the device.
**
**  INPUTS:         fileHndl    - device handle
**                  pSchd       - pointer to the scheduler to be accessed
**                  numRows     - number of rows to be written to the calendar
**                  srcOffset   - offset into pCalData where row data begins
**                  destOffset  - offset into offline calendar page where first
**                                row of data is to be written
**                  pCalData    - pointer to a (UINT4) array that is calendar
**                                length by 4.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsUpdateCalRows(struct file *fileHndl,
                                   sHYPHY20G_SCHD_CTXT *pSchd,
                                   UINT4 numRows, UINT4 srcOffset,
                                   UINT4 destOffset, UINT4 *pCalData)
{
    INT4 result;

    /* argument checking */
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");
    if((pSchd->numRows < (srcOffset + numRows))||
       (pSchd->numRows < (destOffset + numRows))){
        return HYPHY20G_ERR_INVALID_ARG;
    }
DBG_PRINT("SCBSupdatecalRows numRows = %u, srcOffset = %u, , destOffset = %u\n", (unsigned int)numRows, (unsigned int)srcOffset, (unsigned int)destOffset);


    result = utilHyPhy20gScbsWriteCalOffLinePageRows(fileHndl, pSchd, numRows,
                                                     srcOffset, destOffset,
                                                     pCalData);
    if (result) return result;

    result = utilHyPhy20gScbsSwitchActiveCalPage(fileHndl, pSchd);
    if (result) return result;

    result = utilHyPhy20gScbsWriteCalOffLinePageRows(fileHndl, pSchd, numRows,
                                                     srcOffset, destOffset,
                                                     pCalData);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsUpdateCalRows */

/*******************************************************************************
**
**  utilHyPhy20gScbsWriteCalOffLinePage
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Writes the specified rows of the offline calendar page with
**                  values passed to function.
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**                  numRows   - number of rows to be written to the calendar
**                  srcOffset - offset into pCalData where row data begins
**                  destOffset- offset into offline calendar page where first
**                              row of data is to be written
**                  pCalData  - pointer to a (UINT4) array that is calendar
**                              length by 4.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsWriteCalOffLinePageRows(struct file *fileHndl,
                                             sHYPHY20G_SCHD_CTXT *pSchd,
                                             UINT4 numRows,
                                             UINT4 srcOffset,
                                             UINT4 destOffset,
                                             UINT4 *pCalData)
{
    UINT4 data[4];
    UINT4 i, j, maxChnl, chnlData;
    INT4 result;

    /* argument checking */
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    if((pSchd->numRows < (srcOffset + numRows))||
       (pSchd->numRows < (destOffset + numRows))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    // debug code
    maxChnl = pSchd->maxChnl;
    for(i=0; i < pSchd->numRows; i++) {
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            chnlData = *(pCalData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
            //DBG_PRINT("%s, %s, %d, numRows %u, row %u, column %u chnlData %u \n",__FILE__, __FUNCTION__, __LINE__,pSchd->numRows,i,j,chnlData);
            if ((chnlData != HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) &&
                (chnlData >= maxChnl)) {
                DBG_PRINT("%s, %s, %d, %u, %u, %u, %u, %u, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__,(unsigned int)maxChnl,(unsigned int)pSchd->numRows,(unsigned int)i,(unsigned int)j,(unsigned int)chnlData);return HYPHY20G_ERR_INVALID_ARG;
            }
        }
    }
    // end debug code


    /* apply scheduler configuration to offline page */
    for (i=srcOffset; i < (srcOffset+numRows); i++) {
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            data[j] = *(pCalData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
            //GB -> JXu the copying of the below array has not been checked!!!
            *(pSchd->pOffLineCalMem + destOffset*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j) =
                             *(pCalData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
        }
        result = utilHyPhy20gScbsQuadWriteSchdCal(fileHndl, pSchd, destOffset,
                                                  data[0], data[1], data[2],
                                                  data[3]);
        if (result) return result;
        destOffset++;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsWriteCalOffLinePageRows */

/* FUNCTIONS TO ACCESS SCHEDULER CONFIGURATION, CONTEXT AND CALENDAR MEMORY */

/*******************************************************************************
**
**  utilHyPhy20gScbsWriteSchdCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Writes the configuration register for a scheduler
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**                  reset     - SOFT_RESET bit setting
**                              legal values 0 and 1
**                  schdMode  - SCHD_MODE bit setting
**                              legal values 0 and 1
**                  workPgSel - WORK_PAGE_SEL bit setting
**                              legal values 0 and 1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsWriteSchdCfg(struct file *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd, UINT4 reset,
                                  UINT4 schdMode, UINT4 workPgSel)
{
    UINT4 schdCfg;
    UINT4 val;
    INT4 result;


    /* argument checking */
    if ((reset > 1) ||
       (schdMode > 1) ||
       (workPgSel > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* retrieve scheduler row address for the specified scheduler */
    switch(pSchd->schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            schdCfg = HYPHY20G_CPB_LN_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            schdCfg = HYPHY20G_CPB_LN_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            schdCfg = HYPHY20G_CPB_GFPA_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            schdCfg = HYPHY20G_CPB_GFPA_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            schdCfg = HYPHY20G_CPB_GFPB_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            schdCfg = HYPHY20G_CPB_GFPB_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            schdCfg = HYPHY20G_CPB_PI_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            schdCfg = HYPHY20G_CPB_QDR_SCED_REG_CFG;
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            schdCfg = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            schdCfg = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 1);
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            schdCfg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            schdCfg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 1);
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            schdCfg = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_OPI_SCHD_1:
            schdCfg = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 1);
            break;

        default:
            DBG_PRINT("%s, %s, %d, returned HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Operationally perform dummy read to a non-calendar address to ensure
       future reads from non-calendar address are correct */
    /* rev B requirement */
    if ((pSchd->schdId != HYPHY20G_GFP_TX_SCHD_0)&&
        (pSchd->schdId != HYPHY20G_GFP_TX_SCHD_1)) {
        result = sysHyPhy20gRead(fileHndl, schdCfg, &val);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
            return result;
        }          
    }

    result = sysHyPhy20gRead(fileHndl, schdCfg, &val);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
        return result;
    }          

    /* normalize arguments to register field
       all schedulers identical architecture so will use
       a masks associated with QDR Scheduler */

    val = mHYPHY20G_SET_FIELD4(val, reset,
                              HYPHY20G_CPB_QDR_SCED_BIT_CFG_SOFT_RESET_MSK,
                              HYPHY20G_CPB_QDR_SCED_BIT_CFG_SOFT_RESET_OFF);
    val = mHYPHY20G_SET_FIELD4(val, schdMode,
                              HYPHY20G_CPB_QDR_SCED_BIT_CFG_SCHD_MODE_MSK,
                              HYPHY20G_CPB_QDR_SCED_BIT_CFG_SCHD_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, workPgSel,
                              HYPHY20G_CPB_QDR_SCED_BIT_CFG_WORK_PAGE_SEL_MSK,
                              HYPHY20G_CPB_QDR_SCED_BIT_CFG_WORK_PAGE_SEL_OFF);

    /* write scheduler configuration */
    result = sysHyPhy20gWrite(fileHndl, schdCfg, val);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
        return result;
    }          

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsWriteSchdCfg */

/*******************************************************************************
**
**  utilHyPhy20gScbsReadSchdCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reads the configuration register for a scheduler
**
**  INPUTS:         fileHndl  - device handle
**                 *pSchd     - pointer to the scheduler to be accessed
**                 *pReset     - pointer to SOFT_RESET bit setting
**                              legal values 0 and 1
**                 *pSchdMode  - pointer to SCHD_MODE bit setting
**                              legal values 0 and 1
**                 *pWorkPgSel - pointer to WORK_PAGE_SEL bit setting
**                              legal values 0 and 1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsReadSchdCfg(struct file *fileHndl,
                                 sHYPHY20G_SCHD_CTXT *pSchd, UINT4 *pReset,
                                 UINT4 *pSchdMode, UINT4 *pWorkPgSel)
{
    UINT4 schdCfg;
    UINT4 val;
    INT4 result;

    /* argument checking */
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

DBG_PRINT("%s, %s, %d, pSchd->schdId = %u\n",__FILE__, __FUNCTION__, __LINE__, pSchd->schdId);
    /* retrieve scheduler configuration register for the specified scheduler */
    switch(pSchd->schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            schdCfg = HYPHY20G_CPB_LN_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            schdCfg = HYPHY20G_CPB_LN_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            schdCfg = HYPHY20G_CPB_GFPA_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            schdCfg = HYPHY20G_CPB_GFPA_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            schdCfg = HYPHY20G_CPB_GFPB_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            schdCfg = HYPHY20G_CPB_GFPB_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            schdCfg = HYPHY20G_CPB_PI_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            schdCfg = HYPHY20G_CPB_QDR_SCED_REG_CFG;
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            schdCfg = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            schdCfg = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 1);
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            schdCfg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            schdCfg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 1);
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            schdCfg = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_OPI_SCHD_1:
            schdCfg = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 1);
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

DBG_PRINT("%s, %s, %d, schdCfg = 0x%xu\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)schdCfg);

    /* Operationally perform dummy read to a non-calendar address to ensure
       future reads from non-calendar address are correct */
    /* rev B requirement */
    if ((pSchd->schdId != HYPHY20G_GFP_TX_SCHD_0)&&
        (pSchd->schdId != HYPHY20G_GFP_TX_SCHD_1)) {
        result = sysHyPhy20gRead(fileHndl, schdCfg, &val);
        if (result) return result;
    }

    /* read scheduler configuration */
    result = sysHyPhy20gRead(fileHndl, schdCfg, &val);
    if (result) return result;

    /* normalize arguments to register field
       all schedulers identical architecture so will use
       a masks associated with LINE Write Scheduler */
    *pReset = ((val & HYPHY20G_CPB_QDR_SCED_BIT_CFG_SOFT_RESET_MSK) >>
           HYPHY20G_CPB_QDR_SCED_BIT_CFG_SOFT_RESET_OFF);
    *pSchdMode = ((val & HYPHY20G_CPB_QDR_SCED_BIT_CFG_SCHD_MODE_MSK) >>
           HYPHY20G_CPB_QDR_SCED_BIT_CFG_SCHD_MODE_OFF);
    *pWorkPgSel = ((val & HYPHY20G_CPB_QDR_SCED_BIT_CFG_WORK_PAGE_SEL_MSK) >>
           HYPHY20G_CPB_QDR_SCED_BIT_CFG_WORK_PAGE_SEL_OFF);

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsReadSchdCfg */

/*******************************************************************************
**
**  utilHyPhy20gScbsWriteSchdCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Writes the context register for a scheduler.  Function is
**                  dependent on pSchd->numRows being initialized.
**
**  INPUTS:         fileHndl   - device handle
**                  pSchd      - pointer to the scheduler to be accessed
**                  scbsGrpNum - SCBS group number to have context register
**                               updated. indexed from 0
**                  startAddr  - R/W Start Address
**                  endAddr    - R/W End Address
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsWriteSchdCtxt(struct file *fileHndl,
                                   sHYPHY20G_SCHD_CTXT *pSchd, UINT4 scbsGrpNum,
                                   UINT4 startAddr, UINT4 endAddr)
{
    UINT4 schdCtxt = 0;
    UINT4 val;
    sHYPHY20G_SCHD_GRP_CTXT *pScbsGrpCtxt;
    INT4 result;

    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");
    
    DBG_PRINT("%s, %s, %d, startAddr = %u, endAddr = %u, pSchd->numRows = %u, scbsGrpNum = %u, pSchd->numGrp = %u\n",__FILE__, __FUNCTION__, __LINE__, 
              (unsigned int)startAddr, (unsigned int)endAddr, (unsigned int)pSchd->numRows, (unsigned int)scbsGrpNum, (unsigned int)pSchd->numGrp);

    /* argument checking */
    if ((pSchd->schdId == HYPHY20G_GFP_TX_SCHD_0) ||
        (pSchd->schdId == HYPHY20G_GFP_TX_SCHD_1)) {
        if ((startAddr > endAddr)     ||
            (startAddr > pSchd->numRows)  ||
            (endAddr > pSchd->numRows) ||
            (scbsGrpNum >= pSchd->numGrp)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__); 
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
        if ((startAddr > endAddr)     ||
            (startAddr >= pSchd->numRows)  ||
            (endAddr >= pSchd->numRows) ||
            (scbsGrpNum >= pSchd->numGrp)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }


    /* argument checking and retrieve scheduler row address for the specified
       scheduler */
    switch(pSchd->schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            schdCtxt = HYPHY20G_CPB_LN_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_LINE_WR_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_LN_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_LINE_RD_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPA_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_GFPA_WR_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPA_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_GFPA_RD_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPB_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_GFPB_WR_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPB_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_GFPB_RD_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_PI_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_PIF_RD_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            schdCtxt = HYPHY20G_CPB_QDR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_CPB_QDR_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            schdCtxt = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CAL_PARAMS, 0);
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_PIF_TX_SCHD_0 = %u\n",__FILE__, __FUNCTION__, __LINE__, HYPHY20G_PIF_TX_SCHD_0);
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            schdCtxt = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CAL_PARAMS, 1);
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_PIF_TX_SCHD_1\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            schdCtxt = HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_GRP_N_CTXT_MEM_ACC_OFFSET(scbsGrpNum);
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_GFP_TX_SCHD_0\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            schdCtxt = HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_GRP_N_CTXT_MEM_ACC_OFFSET(scbsGrpNum);
            schdCtxt = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(schdCtxt, 1);
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_GFP_TX_SCHD_1\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            schdCtxt = HYPHY20G_OPI_SCBS_REG_CAL_PARAMS;
            schdCtxt = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(schdCtxt, 0);
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_OPI_SCHD_0\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_OPI_SCHD_1:
            schdCtxt = HYPHY20G_OPI_SCBS_REG_CAL_PARAMS;
            schdCtxt = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(schdCtxt, 1);
            DBG_PRINT("%s, %s, %d, pSchd->schdId: HYPHY20G_OPI_SCHD_1\n",__FILE__, __FUNCTION__, __LINE__);
            break;

        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    DBG_PRINT("%s, %s, %d, scbsGrpNum = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)scbsGrpNum);


    /* update the sHYPHY20G_SCHD_GRP_CTXT structure fields if they exist*/     
    pScbsGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + scbsGrpNum);                       
    DBG_PRINT("%s %d memory add: %p\n %p\n",__FUNCTION__, __LINE__, pSchd->pScbsGrpCtxt, pScbsGrpCtxt);     
    if (pScbsGrpCtxt != NULL) {
        DBG_PRINT("%s %d configure scheduler group context",__FUNCTION__, __LINE__);             
        pScbsGrpCtxt->startAddr = startAddr;                                                                
        pScbsGrpCtxt->endAddr = endAddr;                                                                    
        pScbsGrpCtxt->grpNumRows = endAddr - startAddr;                                                     
    }

    /* normalize arguments to register field
       all schedulers identical architecture so will use
       a masks associated with LINE Write Scheduler */
    val = startAddr << HYPHY20G_SCBS_SCHD_START_ADDR_OFF;
    val |= endAddr << HYPHY20G_SCBS_SCHD_END_ADDR_OFF;

DBG_PRINT("SCBSwriteSchdCtxt scbsGrpNum = %u, startAddr = %u, , endAddr = %u\n", (unsigned int)scbsGrpNum, (unsigned int)startAddr, (unsigned int)endAddr);


    /* write scheduler configuration */

    result = sysHyPhy20gWrite(fileHndl, schdCtxt, val);
    DBG_PRINT("%s, %s, %d, return result = %i\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsWriteSchdCtxt */

/*******************************************************************************
**
**  utilHyPhy20gScbsReadSchdCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reads the context register for a scheduler
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**                  scbsGrpNum - SCBS group number to have context register
**                               updated. indexed from 0
**                  pStartAddr- pointer to R/W Start Address
**                  pEndAddr  - pointer to R/W End Address
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsReadSchdCtxt(struct file *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd,
                                  UINT4 scbsGrpNum,
                                  UINT4 *pStartAddr,
                                  UINT4 *pEndAddr)
{
    UINT4 schdCtxt;
    UINT4 val;
    INT4 result;

    /* argument checking */
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    if (scbsGrpNum >= pSchd->numGrp) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve scheduler row address for the specified scheduler */
    switch(pSchd->schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            schdCtxt = HYPHY20G_CPB_LN_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_LN_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPA_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPA_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPB_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_GFPB_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            schdCtxt = HYPHY20G_CPB_PI_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            schdCtxt = HYPHY20G_CPB_QDR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            schdCtxt = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CAL_PARAMS, 0);
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            schdCtxt = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CAL_PARAMS, 1);
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            schdCtxt = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_GRP_N_CTXT_MEM_ACC_OFFSET(scbsGrpNum), 0);
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            schdCtxt = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_GRP_N_CTXT_MEM_ACC_OFFSET(scbsGrpNum), 1);
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            schdCtxt = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_OPI_SCHD_1:
            schdCtxt = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 1);
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* read scheduler configuration for specified group*/
    result = sysHyPhy20gRead(fileHndl, schdCtxt, &val);
    if (result) return result;

    *(pStartAddr) = (val & HYPHY20G_SCBS_SCHD_START_ADDR_MSK) >>
                         HYPHY20G_SCBS_SCHD_START_ADDR_OFF;
    *(pEndAddr) = (val & HYPHY20G_SCBS_SCHD_END_ADDR_MSK) >>
                       HYPHY20G_SCBS_SCHD_END_ADDR_OFF;
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsReadSchdCtxt */


/*******************************************************************************
**
**  utilHyPhy20gScbsWriteSchdCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Writes a single entry to a scheduler calendar.
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**                  row       - scheduler row number to be accessed
**                              range is startAddr to endAddr as defined in
**                              sHYPHY20G_SCHD_CTXT data structure
**                  entry     - entry number within row
**                              range is 0 to 3 for entry1 to entry4
**                              respectively
**                  value     - channel number to be written to a scheduler
**                              calendar entry
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsWriteSchdCal(struct file *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd, UINT4 row,
                                  UINT4 entry, UINT4 value)
{
    UINT4 data[4];
    INT4 result;

    /* argument checking */
    if ((entry > 3) || (value > HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER)) {
        return HYPHY20G_ERR_INVALID_ARG;
	}
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* retrieve row of scheduler calendar */
    result = utilHyPhy20gScbsQuadReadSchdCal(fileHndl, pSchd, row, &data[0],
                                             &data[1], &data[2], &data[3]);
    if (result) return result;

    /* modify entry to be written */
    data[entry] = value;

    /* write row data to schedular calendar */
    result = utilHyPhy20gScbsQuadWriteSchdCal(fileHndl, pSchd, row, data[0],
                                              data[1], data[2], data[3]);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsWriteSchdCal */

/*******************************************************************************
**
**  utilHyPhy20gScbsReadSchdCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reads a single entry from a scheduler row.
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**                  row       - scheduler row number to be accessed
**                              range is startAddr to endAddr as defined in
**                              sHYPHY20G_SCHD_CTXT data structure
**                  entry     - entry number within row
**                              range is 0 to 3 for entry1 to entry4
**                              respectively
**                  ptr       - pointer to channel number contained in specified
**                              scheduler entry
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsReadSchdCal(struct file *fileHndl,
                                 sHYPHY20G_SCHD_CTXT *pSchd, UINT4 row,
                                 UINT4 entry, UINT4 *ptr)
{
    UINT4 data[4];
    INT4 result;

    /* argument checking */
    if (entry > 3) {
		return HYPHY20G_ERR_INVALID_ARG;
	}
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* call utilHyPhy20gScbsQuadReadSchdCal to retrieve row of scheduler entries */
    result = utilHyPhy20gScbsQuadReadSchdCal(fileHndl, pSchd, row, &data[0],
                                             &data[1], &data[2], &data[3]);
    if (result) return result;

    /* return the appropriate entry */
    *ptr = data[entry];

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsReadSchdCal */



/*******************************************************************************
**
**  utilHyPhy20gScbsQuadWriteSchdCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Writes a row of a scheduler which contains four entries
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**                  row       - scheduler row number to be accessed
**                              range is startAddr to endAddr as defined in
**                              sHYPHY20G_SCHD_CTXT data structure
**                  entry1    - channel number contained in entry1
**                  entry2    - channel number contained in entry2
**                  entry3    - channel number contained in entry3
**                  entry4    - channel number contained in entry4
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsQuadWriteSchdCal(struct file *fileHndl,
                                      sHYPHY20G_SCHD_CTXT *pSchd, UINT4 row,
                                      UINT4 entry1, UINT4 entry2, UINT4 entry3,
                                      UINT4 entry4)
{
    UINT4 schdAddr, startAddr, endAddr;
    UINT4 val;
    INT4 result;
    UINT4 nonCalAddr;

    /* argument checking */
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL) && (pSchd->pScbsGrpCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    startAddr = pSchd->pScbsGrpCtxt->startAddr;
    endAddr = pSchd->pScbsGrpCtxt->endAddr;

     if (((row < startAddr) &&
         ((pSchd->schdId != HYPHY20G_GFP_TX_SCHD_0) &&
         (pSchd->schdId != HYPHY20G_GFP_TX_SCHD_1))) ||
         (entry1 > HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) ||
         (entry2 > HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) ||
         (entry3 > HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) ||
         (entry4 > HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER)) {
        DBG_PRINT("pSchd->schdId = %u, entry1 = %u, entry2 = %u, entry3 = %u, entry4 = %u\n",(unsigned int)pSchd->schdId, (unsigned int)entry1, (unsigned int)entry2, (unsigned int)entry3, (unsigned int)entry3);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    } else if (row > endAddr) {
        /* Okay to exceed last row for QDR and GFP schedulers
           rely on calling function to ensure these accesses do not
           exceed range of scheduler */
        if ((pSchd->schdId == HYPHY20G_CPB_QDR_SCHD)   ||
            (pSchd->schdId == HYPHY20G_GFP_TX_SCHD_0) ||
            (pSchd->schdId == HYPHY20G_GFP_TX_SCHD_1)) {
            /* Do nothing */
            DBG_PRINT("%s, %s, %d, Do nothing\n",__FILE__, __FUNCTION__, __LINE__);
        } else {
            DBG_PRINT("pSchd->schdId = %u, endAddr = %u, row = %u, pSchd->pScbsGrpCtxt->endAddr = %u, entry1 = %u, entry2 = %u, entry3 = %u, entry4 = %u\n",(unsigned int)pSchd->schdId, (unsigned int)endAddr, (unsigned int)row, (unsigned int)pSchd->pScbsGrpCtxt->endAddr, (unsigned int)entry1, (unsigned int)entry2, (unsigned int)entry3, (unsigned int)entry3);
            /* Line Rd/Wr Scheduler may exceed endAddr in cases where pSchd->numRows
               exceeds endAddr, we are okay in these cases, but row cannot exceed
               pSchd->numRows */
            if ((pSchd->schdId == HYPHY20G_CPB_LINE_WR_SCHD ||
                 pSchd->schdId == HYPHY20G_CPB_LINE_RD_SCHD) && 
                 row < pSchd->numRows){
                /* Do nothing */
                DBG_PRINT("%s, %s, %d, Do nothing\n",__FILE__, __FUNCTION__, __LINE__);                
            } else {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
            }
        }
    }

    /* retrieve scheduler row address for the specified scheduler */
    switch(pSchd->schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            schdAddr =
                  HYPHY20G_CPB_LN_WR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_LN_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            schdAddr =
                  HYPHY20G_CPB_LN_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_LN_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPA_WR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPA_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPA_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPA_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPB_WR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPB_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPB_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPB_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            schdAddr =
                  HYPHY20G_CPB_PI_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_PI_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            schdAddr =
                    HYPHY20G_CPB_QDR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_QDR_SCED_REG_CFG;
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            schdAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_STB_PG_OFFSET(row), 0);
            nonCalAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            schdAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_STB_PG_OFFSET(row), 1);
            nonCalAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 1);
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            schdAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 0);
            nonCalAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            schdAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 1);
            nonCalAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 1);
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            schdAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 0);
            nonCalAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_OPI_SCHD_1:
            schdAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 1);
            nonCalAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 1);
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* normalize entry values to CAL_DATA field */
    val = (entry1 << 0);
    val |= (entry2 << 8);
    val |= (entry3 << 16);
    val |= (entry4 << 24);

    /* write scheduler calendar row*/
    result = sysHyPhy20gWrite(fileHndl, schdAddr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsQuadWriteSchdCal */

/*******************************************************************************
**
**  utilHyPhy20gScbsQuadReadSchdCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reads a row of a scheduler which contains four entries
**
**  INPUTS:         fileHndl  - device handle
**                  pSchd     - pointer to the scheduler to be accessed
**
**                  row       - scheduler row number to be accessed
**                              range is startAddr to endAddr as defined in
**                              sHYPHY20G_SCHD_CTXT data structure
**                  pEntry1    - pointer to channel number contained in entry1
**                  pEntry2    - pointer to channel number contained in entry2
**                  pEntry3    - pointer to channel number contained in entry3
**                  pEntry4    - pointer to channel number contained in entry4
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsQuadReadSchdCal(struct file *fileHndl,
                                     sHYPHY20G_SCHD_CTXT *pSchd,
                                     UINT4 row, UINT4 *pEntry1, UINT4 *pEntry2,
                                     UINT4 *pEntry3, UINT4 *pEntry4)
{
    UINT4 schdAddr, startAddr, endAddr;
    UINT4 val;
    INT4 result;
    UINT4 nonCalAddr;

   /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL) && (pSchd->pScbsGrpCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    startAddr = pSchd->pScbsGrpCtxt->startAddr;
    endAddr = pSchd->pScbsGrpCtxt->endAddr;

    /* argument checking */
    if (row < startAddr) {
        if ((pSchd->schdId != HYPHY20G_GFP_TX_SCHD_0) &&
            (pSchd->schdId != HYPHY20G_GFP_TX_SCHD_1)) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (row > endAddr) {
        /* error is ok if configuring QDR scheduler */
        if ((pSchd->schdId == HYPHY20G_CPB_QDR_SCHD)   ||
            (pSchd->schdId == HYPHY20G_GFP_TX_SCHD_0) ||
            (pSchd->schdId == HYPHY20G_GFP_TX_SCHD_1)) {
            // Do nothing
            DBG_PRINT("%s, %s, %d, Do nothing\n",__FILE__, __FUNCTION__, __LINE__);
        } else {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* retrieve scheduler row address for the specified scheduler */
    switch(pSchd->schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            schdAddr =
                  HYPHY20G_CPB_LN_WR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_LN_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            schdAddr =
                  HYPHY20G_CPB_LN_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_LN_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPA_WR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPA_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPA_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPA_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPB_WR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPB_WR_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            schdAddr =
                HYPHY20G_CPB_GFPB_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_GFPB_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            schdAddr =
                  HYPHY20G_CPB_PI_RD_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_PI_RD_SCED_REG_CFG;
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            schdAddr =
                    HYPHY20G_CPB_QDR_SCED_REG_CLDR_STB_PG_ADR_N_ACC_OFFSET(row);
            nonCalAddr = HYPHY20G_CPB_QDR_SCED_REG_CFG;
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            schdAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_STB_PG_OFFSET(row), 0);
            nonCalAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            schdAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_STB_PG_OFFSET(row), 1);
            nonCalAddr = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 1);
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            schdAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 0);
            nonCalAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            schdAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 1);
            nonCalAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 1);
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            schdAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 0);
            nonCalAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 0);
            break;
        case HYPHY20G_OPI_SCHD_1:
            schdAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CLDR_STB_PG_MEM_ADR_N_ACC_OFFSET(row), 1);
            nonCalAddr = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 1);
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* read scheduler calendar row*/
    result = sysHyPhy20gRead(fileHndl, schdAddr, &val);
    if (result) return result;

    /* normalize entry values and associate with pointers */
    *pEntry1 = (val & 0xFF) >> 0;
    *pEntry2 = (val & 0xFF00) >> 8;
    *pEntry3 = (val & 0xFF0000) >> 16;
    *pEntry4 = (val & 0xFF000000) >> 24;

    /* Operationally perform dummy read to a non-calendar address to ensure
       future reads from non-calendar address are correct */
    /* rev B requirement */
    if ((pSchd->schdId != HYPHY20G_GFP_TX_SCHD_0)&&
        (pSchd->schdId != HYPHY20G_GFP_TX_SCHD_1)) {
        result = sysHyPhy20gRead(fileHndl, nonCalAddr, &val);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsQuadReadSchdCal */

/*******************************************************************************
**
**  utilHyPhy20gScbsGetSchdCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the schd.pScbsGrpCtxt, schd.pOnLineCalMem and 
**                  schd.pOffLineCalMem context size.
**
**
**
**  INPUTS:         pSchdCtxt    - point to the scheduler
**
**  OUTPUTS:        None
**
**  RETURNS:        The sum of schd.pScbsGrpCtxt, schd.pOnLineCalMem and 
**                  schd.pOffLineCalMem context size
**
*******************************************************************************/
UINT4 utilHyPhy20gScbsGetSchdCtxtSize(sHYPHY20G_SCHD_CTXT *pSchdCtxt)
{
    UINT4 size = 0;

    /*HYPHY20G_ASSERT((pSchdCtxt != NULL), 
        HYPHY20G_ERR_NULL_PTR_PASSED, "%s", "pSchd is NULL.");*/
    if ((pSchdCtxt == NULL)) {
        DBG_PRINT("ASSERT: - file %s, line %d, - ", __FILE__, __LINE__);
        return 0;
    }  

    /* include the ctxt header size for scbsGrpCtxt */
    size += utilHyPhy20gGetCtxtHdrSize();
    size += pSchdCtxt->sizeScbsGrpCtxt;

    /* include the ctxt header size for onLineCalMem */
    size += utilHyPhy20gGetCtxtHdrSize();
    size += pSchdCtxt->sizeOnLineCalMem;

    /* include the ctxt header size for offLineCalMem */
    size += utilHyPhy20gGetCtxtHdrSize();
    size += pSchdCtxt->sizeOffLineCalMem;

    return size;
} /* utilHyPhy20gScbsGetSchdCtxtSize */

/*******************************************************************************
**
**  utilHyPhy20gScbsExtractSchdCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the schd.pScbsGrpCtxt, schd.pOnLineCalMem and 
**                  schd.pOffLineCalMem context into pre-allocated buffer.
**
**
**
**  INPUTS:         pSchd     - pointer to the scheduler to be accessed
**                  pCtxtBuf - pre-allocated buffer for this schd context
**                  extraction
**
**  OUTPUTS:        pExtractedSize -the context size(in byte) has been extracted
**                  into the buffer
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsExtractSchdCtxt(sHYPHY20G_SCHD_CTXT *pSchd, 
                                    void *pCtxtBuf, UINT4 *pExtractedSize)
{
    UINT4 len = 0, offset = 0;
    UINT4 rtLen;

    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pSchd is NULL.");
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pExtractedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pExtractedSize is NULL.");


    /* extract schd[i].pScbsGrpCtxt with header */
	/* insert ctxt buffer header */
	len = sizeof(sHYPHY20G_CTXT_HDR) +  
        pSchd->sizeScbsGrpCtxt;
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                HYPHY_SCHD_SCBS_GRP_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    /* copy the payload into the buffer */
    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                    (void *)(pSchd->pScbsGrpCtxt), pSchd->sizeScbsGrpCtxt);
    offset += pSchd->sizeScbsGrpCtxt;

    /* extract schd[i].pOnLineCalMem with header */
    /* insert ctxt buffer header */    
	len = sizeof(sHYPHY20G_CTXT_HDR) +  
        pSchd->sizeOnLineCalMem;
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                HYPHY_SCHD_ONLINE_CAL_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    /* copy the payload into the buffer */
    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                    (void *)(pSchd->pOnLineCalMem), pSchd->sizeOnLineCalMem);
    offset += pSchd->sizeOnLineCalMem;

    /* extract schd[i].pOffLineCalMem with header */
    /* insert ctxt buffer header */    
	len = sizeof(sHYPHY20G_CTXT_HDR) +  
        pSchd->sizeOffLineCalMem;
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                HYPHY_SCHD_OFFLINE_CAL_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    /* copy the payload into the buffer */
    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                    (void *)(pSchd->pOffLineCalMem), pSchd->sizeOffLineCalMem);
    offset += pSchd->sizeOffLineCalMem;


    *pExtractedSize = offset;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsExtractSchdCtxt */

/*******************************************************************************
**
**  utilHyPhy20gScbsRestoreSchdCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the schd.pScbsGrpCtxt, schd.pOnLineCalMem and
**                  schd.pOffLineCalMem context into the driver context memory.
**
**
**  INPUTS:         pSchd     - pointer to the scheduler to be accessed
**                  pCtxtBuf - pre-stored schd context for restoration
**
**  OUTPUTS:        pParsedSize -the data size(in byte) has been parsed
**                  in the buffer for restoration
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsRestoreSchdCtxt(sHYPHY20G_SCHD_CTXT *pSchd, 
                                    void *pCtxtBuf, UINT4 *pParsedSize)
{
    INT4 result;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    UINT4 offset = 0;

    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pSchd is NULL.");
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the pScbsGrpCtxt header in the pre-stored context buffer */
    pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
    if (pCtxtBufHdr->id != HYPHY_SCHD_SCBS_GRP_CTXT_ID) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED 0x%p 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, pCtxtBufHdr, pCtxtBufHdr->id);
        *pParsedSize = 0;
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }
    HYPHY20G_ASSERT((pCtxtBufHdr->len >= sizeof(sHYPHY20G_CTXT_HDR)), 
                    HYPHY20G_ERR_DEV_CTXT_CORRUPTED,
                    "%s", "HYPHY20G_ERR_DEV_CTXT_CORRUPTED.");

    /* check if the payload is valid */
    if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
        /* if pScbsGrpCtxt is invalid, 
        pOnLineCalMem and pOffLineCalMem should be invalid too, 
        see utilHyPhy20gScbsInitSchd for the logic */
        offset += 3 * sizeof(sHYPHY20G_CTXT_HDR);
    } else {
        /* create Scbs context */
        result = utilHyPhy20gScbsCreateSchd(pSchd, 
                                  pSchd->numRows, pSchd->numGrp);
        if (result) {
            DBG_PRINT("%s, %s, %d, utilHyPhy20gScbsCreateSchd failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return HYPHY20G_ERR_MEM_ALLOC;
        }

        /* restore pSchdCtxt[schdId].pScbsGrpCtxt */
        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        sysHyPhy20gMemCpy((void *)(pSchd->pScbsGrpCtxt), 
                        ((UINT1 *)pCtxtBuf + offset), pSchd->sizeScbsGrpCtxt);
        offset += pSchd->sizeScbsGrpCtxt;

        /* restore pSchdCtxt[schdId].pOnLineCalMem */
        /* try to retrieve the pOnLineCalMem header in the pre-stored context buffer */
        pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
        if (pCtxtBufHdr->id != HYPHY_SCHD_ONLINE_CAL_CTXT_ID) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED 0x%p 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, pCtxtBufHdr, pCtxtBufHdr->id);
            utilHyPhy20gScbsDestroySchd(pSchd);
            *pParsedSize = 0;
            return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
        }

        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        sysHyPhy20gMemCpy((void *)(pSchd->pOnLineCalMem), 
                        ((UINT1 *)pCtxtBuf + offset), pSchd->sizeOnLineCalMem);
        offset += pSchd->sizeOnLineCalMem;

        /* restore pSchdCtxt[schdId].pOffLineCalMem */
        /* try to retrieve the pOffLineCalMem header in the pre-stored context buffer */
        pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
        if (pCtxtBufHdr->id != HYPHY_SCHD_OFFLINE_CAL_CTXT_ID) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED 0x%p 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, pCtxtBufHdr, pCtxtBufHdr->id);
            utilHyPhy20gScbsDestroySchd(pSchd);
            *pParsedSize = 0;
            return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
        }

        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        sysHyPhy20gMemCpy((void *)(pSchd->pOffLineCalMem), 
                        ((UINT1 *)pCtxtBuf + offset), pSchd->sizeOffLineCalMem);
        offset += pSchd->sizeOffLineCalMem;
    }

    *pParsedSize = offset;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gScbsRestoreSchdCtxt */

/*******************************************************************************
**
**  utilHyPhy20gScbsAuditSchdCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit sHYPHY20G_SCHD_CTXT used in CPB,GFP
**                  PIF and CBRI subCtxt. 
**
**                  PRIVATE function called by cpbHyPhy20gCpbAuditSchdCtxt,
**                  gfpHyPhy20gGfpAuditSchdCtxt,pifHyPhy20gPifAuditSchdCtxt,
**                  cbriHyPhy20gCbriAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gScbsAuditSchdCtxt(struct file *fileHndl, sHYPHY20G_SCHD_CTXT *pSchd)
{
    UINT4 data[HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW],*pOffLineCalMem,*pOnLineCalMem;
    UINT4 maxNumGrp;
    UINT4 i, j, maxNumRow, maxChnl, h;
    UINT4 portOffset, intfGrpDir, schdCfg, val, schdRst = 0;
    INT4 result=HYPHY20G_SUCCESS;
    UINT4 schdCtxt = 0;
    sHYPHY20G_SCHD_GRP_CTXT *pScbsGrpCtxt=NULL;
    UINT4 scbsGrpNum=0;

    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_FAILURE,
                    "%s", "pSchd is NULL in utilHyPhy20gScbsAuditSchdCtxt entry."); 
    if (pSchd->valid != HYPHY20G_VALID_SIGN) 
    {
        DBG_PRINT("%s, %s, %d, pSchd is still not initialized, so no need to audit further.\n",
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_SUCCESS;
    }

    /* confirm context pointer is valid before processing */
    if ((pSchd->pScbsGrpCtxt == NULL)||
        (pSchd->pOffLineCalMem == NULL)||(pSchd->pOnLineCalMem == NULL))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }


    switch(pSchd->schdId) {
        /* CPB SCBS */
        case HYPHY20G_CPB_LINE_WR_SCHD:
            maxNumRow = HYPHY20G_CPB_LINE_WR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_WR_DIR;
            
            schdCfg = HYPHY20G_CPB_LN_WR_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_LN_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_LINE_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_LINE_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_LINE_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;
            
            schdCfg = HYPHY20G_CPB_LN_RD_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_LN_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPA_WR_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPA_WR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_WR_DIR;
            
            schdCfg = HYPHY20G_CPB_GFPA_WR_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_GFPA_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPA_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPA_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPA_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;

            schdCfg = HYPHY20G_CPB_GFPA_RD_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_GFPA_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPB_WR_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPB_WR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPB_WR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_WR_DIR;

            schdCfg = HYPHY20G_CPB_GFPB_WR_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_GFPB_WR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_GFPB_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_GFPB_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_GFPB_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;

            schdCfg = HYPHY20G_CPB_GFPB_RD_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_GFPB_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_PIF_RD_SCHD:
            maxNumRow = HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_PIF_RD_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
            intfGrpDir = HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR;

            schdCfg = HYPHY20G_CPB_PI_RD_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_PI_RD_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        case HYPHY20G_CPB_QDR_SCHD:
            maxNumRow = HYPHY20G_CPB_QDR_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_CPB_QDR_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_CPB_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;

            schdCfg = HYPHY20G_CPB_QDR_SCED_REG_CFG;
            schdCtxt = HYPHY20G_CPB_QDR_SCED_REG_GRP_N_CTXT_MEM_ACC;
            break;
        /* PIF SCBS */
        case HYPHY20G_PIF_TX_SCHD_0:
            maxNumRow = HYPHY20G_PIF_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_PIF_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_PIF_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;

            schdCfg = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 0);
            schdCtxt = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CAL_PARAMS, 0);
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            maxNumRow = HYPHY20G_PIF_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_PIF_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_PIF_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;

            schdCfg = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CFG, 1);
            schdCtxt = HYPHY20G_PIF_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_PIF_SCBS_REG_CAL_PARAMS, 1);
            break;
        /* GFP SCBS */
        case HYPHY20G_GFP_TX_SCHD_0:
            maxNumRow = HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_GFP_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_GFP_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;

            schdCfg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 0);
            schdCtxt = HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_GRP_N_CTXT_MEM_ACC_OFFSET(scbsGrpNum);
            break;
        case HYPHY20G_GFP_TX_SCHD_1:
            maxNumRow = HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_GFP_TX_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_GFP_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;

            schdCfg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_CFG, 1);
            schdCtxt = HYPHY20G_GFP_TRA_TXGFPT_SCBS_REG_GRP_N_CTXT_MEM_ACC_OFFSET(scbsGrpNum);
            schdCtxt = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(schdCtxt, 1);
            break;
        /* OPI SCBS */
        case HYPHY20G_OPI_SCHD_0:
            maxNumRow = HYPHY20G_OPI_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_OPI_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_OPI_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;

            schdCfg = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 0);
            schdCtxt = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CAL_PARAMS, 0);
            break;
        case HYPHY20G_OPI_SCHD_1:
            maxNumRow = HYPHY20G_OPI_SCHD_MAX_NUM_ROW;
            maxChnl = HYPHY20G_OPI_SCHD_MAX_CHNL;
            maxNumGrp = HYPHY20G_OPI_SCHD_MAX_GRP;
            portOffset = HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET;
            intfGrpDir = HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR;

            schdCfg = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CFG, 1);
            schdCtxt = HYPHY20G_OPI_SCBS_TSB_SLICE_REG_OFFSET(HYPHY20G_OPI_SCBS_REG_CAL_PARAMS, 1);
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
    }

    /* check the valid according to the SOFT_RST state for CPB and PIF module
    ** Only these two modules are able to reset the SCHD via utilHyPhy20gScbsResetSchd.
    */
    if (pSchd->schdId<= HYPHY20G_PIF_TX_SCHD_1 )
    {
        if (pSchd->schdId<=HYPHY20G_CPB_QDR_SCHD)
        {
            result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_CPB_CPB_REG_CFG, HYPHY20G_CPB_CPB_BIT_CFG_SOFT_RST_OFF, &schdRst);

        }
        else if (pSchd->schdId==HYPHY20G_PIF_TX_SCHD_0)
        {
            result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST, HYPHY20G_PIF_PIF_BIT_SW_RST_SCH0_RST_OFF, &schdRst);
            
        }
        else if (pSchd->schdId==HYPHY20G_PIF_TX_SCHD_1)
        {
            result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST, HYPHY20G_PIF_PIF_BIT_SW_RST_SCH1_RST_OFF, &schdRst);
        }

        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        if (schdRst == 1)
        {
            if ((pSchd->valid != HYPHY20G_INVALID_VAL_32BIT))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            else
                return HYPHY20G_SUCCESS;
        }
    }    

    if (pSchd->valid != HYPHY20G_VALID_SIGN)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }
    if ( (pSchd->numRows < 1) || (pSchd->numRows > maxNumRow) 
        || (pSchd->schdMode > HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE) ) 
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* configure parameters in pSchd to reflect configuration */
    /* Note: */
    if ((pSchd->numGrp != maxNumGrp)||(pSchd->maxChnl != maxChnl)||
        (pSchd->portOffset != portOffset)||(pSchd->intfGrpDir != intfGrpDir)||
        (pSchd->pOffLineCalMem==NULL)||(pSchd->pOnLineCalMem == NULL))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }
    /* test schdMode */
    /* retrieve the SCHD_MODE state of SCBS */
    result = hyPhy20gRegFieldRead(fileHndl, schdCfg, HYPHY20G_CPB_LN_WR_SCED_BIT_CFG_SCHD_MODE_MSK, &val);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "hyPhy20gRegFieldRead fail."); 

    if (pSchd->schdMode != val)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    for (scbsGrpNum=0; scbsGrpNum<pSchd->numGrp; scbsGrpNum++)
    {
        pScbsGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + scbsGrpNum);
        
        if ((pSchd->schdId == HYPHY20G_GFP_TX_SCHD_0) ||
            (pSchd->schdId == HYPHY20G_GFP_TX_SCHD_1)) 
        {
            if ((pScbsGrpCtxt->startAddr > pScbsGrpCtxt->endAddr)||
                (pScbsGrpCtxt->startAddr > pSchd->numRows)||
                (pScbsGrpCtxt->endAddr > pSchd->numRows)||
                (scbsGrpNum >= pSchd->numGrp)) 
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__); 
                return HYPHY20G_FAILURE;
            }
        } 
        else 
        {
            if ((pScbsGrpCtxt->startAddr > pScbsGrpCtxt->endAddr)     ||
                (pScbsGrpCtxt->startAddr >= pSchd->numRows)  ||
                (pScbsGrpCtxt->endAddr >= pSchd->numRows) ||
                (scbsGrpNum >= pSchd->numGrp)) 
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
        }

        if (pScbsGrpCtxt->grpNumRows != (pScbsGrpCtxt->endAddr - pScbsGrpCtxt->startAddr)) 
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }

        /* retrieve END_ADDRESS for the scheduler */
        result = hyPhy20gRegFieldRead(fileHndl, schdCtxt, HYPHY20G_CPB_LN_WR_SCED_BIT_GRP_N_CTXT_MEM_ACC_END_ADDRESS_MSK, &val);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "hyPhy20gRegFieldRead fail."); 

        if (pSchd->pScbsGrpCtxt->endAddr != val) 
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }

        /* retrieve START_ADDRESS for the scheduler */
        result = hyPhy20gRegFieldRead(fileHndl, schdCtxt, HYPHY20G_CPB_LN_WR_SCED_BIT_GRP_N_CTXT_MEM_ACC_START_ADDRESS_MSK, &val);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "hyPhy20gRegFieldRead fail."); 

        if (pSchd->pScbsGrpCtxt->startAddr != val) 
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }

    }

    /* based on code structure it is assumed that online and offline SCBS pages are identical
       SCBS page switch will not cause an error.  To test the on line page we switch the 
       scbs to make the current online page the offline page */
    /* compare on line and off line calendar memories to confirm they are identical, the rule 
       come from utilHyPhy20gScbsUpdateCal function */
    pOffLineCalMem = pSchd->pOffLineCalMem;
    pOnLineCalMem = pSchd->pOnLineCalMem;
    
    for ( i = 0; i < pSchd->numRows; i++){       
        for ( j = 0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++){
            if(*pOffLineCalMem != *pOnLineCalMem)
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            pOffLineCalMem++;
            pOnLineCalMem++;                
        }
    } 

    /* check *pOffLineCalMem against offline page data */
    for ( h = 0; h < 2; h++) 
    {
        pOffLineCalMem = pSchd->pOffLineCalMem;
        for ( i = 0; i < pSchd->numRows; i++)
        {       
            result = utilHyPhy20gScbsQuadReadSchdCal(fileHndl, pSchd, i, &data[0], &data[1], &data[2], &data[3]);
            if (result != HYPHY20G_SUCCESS) 
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return result;
            }
            for ( j = 0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++)
            {
                if(*pOffLineCalMem != data[j]) 
                {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
                pOffLineCalMem++;
            }
        }
        result = utilHyPhy20gScbsSwitchActiveCalPage(fileHndl, pSchd);
        if (result != HYPHY20G_SUCCESS) 
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return result;
        }
    }
            
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gScbsAuditSchdCtxt */

/* end of file */
