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
**  FILE        : hyphy20g_hw.c
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION : This file contains all the hardware interface functions.
**                The user should modify the contents of this file and the
**                associated header file, hyphy20g_hw.h, according to the
**                hardware characteristics of their system.
**
**  NOTES       : The user should feel free to modify the functions
**                listed here OR convert them to macros (for speed and
**                conciseness). If converting to macros, the macros should
**                be relocated to hyphy20g_hw.h
**
*******************************************************************************/

/** include files **/
#include <linux/pci.h>
#include <linux/delay.h>

#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hw.h"
#include "../qnf_drv.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/
/* local  Macro define */
#define HYPHY20G_DRIVER_SUPPORT_DEV_NUM        4
/* To address multiple devices from a single SW driver modifications are required.
** The an array of sHYPHY20G_CTXT hyPhy20gCtxt[n] can be declared for multiple 
** device context. Functions sysHyPhy20gGetDeviceCtxt need to be updated to 
** target context relative to fileHndl argument. 
*/
PRIVATE sHYPHY20G_CTXT hyPhy20gCtxt[HYPHY20G_DRIVER_SUPPORT_DEV_NUM] = { 
    /* device #0 context */
    {
        .valid = 0,
        .version = 0,
        .realDeviceId = 0,
        .realRevId = 0,
        
	 /* initialize global variables with default value */
    	.gData.hyphy20gCheckRev = 1,
        .gData.forceRevId = 0,
        .gData.hyphy20gQdrStressMode = 0,
        .gData.hyphy20gQdrStressDist = 0,
        .gData.intQdrBlkSize = 0,
        .gData.hostMsgFwOp = 1,
        .gData.hyphy20gTraceWrite = 0,
        .gData.hyphy20gCbrbPacketSizeMode = 0,
	
        .errDevice = 0,
        .mimicDeviceID = 0,
        .mimicRevId = 0,
        .stateDevice = 0,
        .baseAddr = NULL,
        .pCpbCtxt = NULL,
        .pLsciCtxt = {NULL},
        .pHsciCtxt = {NULL},
        .pPifCtxt = NULL,
        .pGfpACtxt = NULL,
        .pGfpBCtxt = NULL,
        .pCbriCtxt = {NULL},
        .pTseCtxt = NULL,
        .pVcatACtxt = NULL,
        .pVcatBCtxt = NULL,
        .pSfpSerdesCtxt = {NULL},
        .pXfiSerdesCtxt = {NULL},
        .pBackplaneSerdesCtxt = {NULL},
        .pPifSerdesCtxt = {NULL},
        .pHostMsgCtxt = NULL,
        .pOtnOhRamCtxt = NULL,
        .pOdukppOhRamCtxt = NULL,
#ifndef PMC_HW_CODE
        .pRtosTimerCtxt = NULL,
#endif
#ifdef HYPHY20G_API_REENTRANT
        .sem = 0,
#endif
    },
    /* device #1 context */
    {
        .valid = 0,
        .version = 0,
        .realDeviceId = 0,
        .realRevId = 0,

	/* initialize global variables with default value */
    	.gData.hyphy20gCheckRev = 1,
        .gData.forceRevId = 0,
        .gData.hyphy20gQdrStressMode = 0,
        .gData.hyphy20gQdrStressDist = 0,
        .gData.intQdrBlkSize = 0,
        .gData.hostMsgFwOp = 1,
        .gData.hyphy20gTraceWrite = 0,
        .gData.hyphy20gCbrbPacketSizeMode = 0,
	
        .errDevice = 0,
        .mimicDeviceID = 0,
        .mimicRevId = 0,
        .stateDevice = 0,
        .baseAddr = NULL,
        .pCpbCtxt = NULL,
        .pLsciCtxt = {NULL},
        .pHsciCtxt = {NULL},
        .pPifCtxt = NULL,
        .pGfpACtxt = NULL,
        .pGfpBCtxt = NULL,
        .pCbriCtxt = {NULL},
        .pTseCtxt = NULL,
        .pVcatACtxt = NULL,
        .pVcatBCtxt = NULL,
        .pSfpSerdesCtxt = {NULL},
        .pXfiSerdesCtxt = {NULL},
        .pBackplaneSerdesCtxt = {NULL},
        .pPifSerdesCtxt = {NULL},
        .pHostMsgCtxt = NULL,
        .pOtnOhRamCtxt = NULL,
        .pOdukppOhRamCtxt = NULL,
#ifndef PMC_HW_CODE
        .pRtosTimerCtxt = NULL,
#endif
#ifdef HYPHY20G_API_REENTRANT
        .sem = 0,
#endif
    },
    /* device #2 context */
    {
        .valid = 0,
        .version = 0,
        .realDeviceId = 0,
        .realRevId = 0,

	/* initialize global variables with default value */
    	.gData.hyphy20gCheckRev = 1,
        .gData.forceRevId = 0,
        .gData.hyphy20gQdrStressMode = 0,
        .gData.hyphy20gQdrStressDist = 0,
        .gData.intQdrBlkSize = 0,
        .gData.hostMsgFwOp = 1,
        .gData.hyphy20gTraceWrite = 0,
        .gData.hyphy20gCbrbPacketSizeMode = 0,
	
        .errDevice = 0,
        .mimicDeviceID = 0,
        .mimicRevId = 0,
        .stateDevice = 0,
        .baseAddr = NULL,
        .pCpbCtxt = NULL,
        .pLsciCtxt = {NULL},
        .pHsciCtxt = {NULL},
        .pPifCtxt = NULL,
        .pGfpACtxt = NULL,
        .pGfpBCtxt = NULL,
        .pCbriCtxt = {NULL},
        .pTseCtxt = NULL,
        .pVcatACtxt = NULL,
        .pVcatBCtxt = NULL,
        .pSfpSerdesCtxt = {NULL},
        .pXfiSerdesCtxt = {NULL},
        .pBackplaneSerdesCtxt = {NULL},
        .pPifSerdesCtxt = {NULL},
        .pHostMsgCtxt = NULL,
        .pOtnOhRamCtxt = NULL,
        .pOdukppOhRamCtxt = NULL,
#ifndef PMC_HW_CODE
        .pRtosTimerCtxt = NULL,
#endif
#ifdef HYPHY20G_API_REENTRANT
        .sem = 0,
#endif
    },
    /* device #3 context */
    {
        .valid = 0,
        .version = 0,
        .realDeviceId = 0,
        .realRevId = 0,

	/* initialize global variables with default value */
    	.gData.hyphy20gCheckRev = 1,
        .gData.forceRevId = 0,
        .gData.hyphy20gQdrStressMode = 0,
        .gData.hyphy20gQdrStressDist = 0,
        .gData.intQdrBlkSize = 0,
        .gData.hostMsgFwOp = 1,
        .gData.hyphy20gTraceWrite = 0,
        .gData.hyphy20gCbrbPacketSizeMode = 0,
	
        .errDevice = 0,
        .mimicDeviceID = 0,
        .mimicRevId = 0,
        .stateDevice = 0,
        .baseAddr = NULL,
        .pCpbCtxt = NULL,
        .pLsciCtxt = {NULL},
        .pHsciCtxt = {NULL},
        .pPifCtxt = NULL,
        .pGfpACtxt = NULL,
        .pGfpBCtxt = NULL,
        .pCbriCtxt = {NULL},
        .pTseCtxt = NULL,
        .pVcatACtxt = NULL,
        .pVcatBCtxt = NULL,
        .pSfpSerdesCtxt = {NULL},
        .pXfiSerdesCtxt = {NULL},
        .pBackplaneSerdesCtxt = {NULL},
        .pPifSerdesCtxt = {NULL},
        .pHostMsgCtxt = NULL,
        .pOtnOhRamCtxt = NULL,
        .pOdukppOhRamCtxt = NULL,
#ifndef PMC_HW_CODE
        .pRtosTimerCtxt = NULL,
#endif
#ifdef HYPHY20G_API_REENTRANT
        .sem = 0,
#endif
    },
};

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
inline INT4 pcie_read(struct file *fileHndl, UINT4 mode, UINT4 offset, UINT4 *pvalue);
inline INT4 pcie_write(struct file *fileHndl, UINT4 mode, UINT4 offset, UINT4 value);
inline INT4 pcie_burst_read(struct file * filp, UINT4 startAddr, UINT4 length, UINT4 * dataList);
inline INT4 pcie_burst_write(struct file * filp, UINT4 startAddr, UINT4 length, UINT4 * dataList);
inline INT4 gen_read(struct file *filp, UINT4 offset, UINT4 *pvalue);
inline INT4 gen_write(struct file *filp, UINT4 offset, UINT4 value);


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gCheckRev
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts mimic device and revision data.  It
**                 returns a status to indicate SW driver compatibility with
**                 the revision.
**
**                 This is a diagnostic function.  Actual devices will have
**                 device context as part of PCIe interface bring up.
**
**
**  INPUTS:         *fileHndl  - base address
**                  *pDevId    - pointer to mimic device ID
**                  *pRevId    - pointer to mimic revision ID
**                  *pStatus   - Status of SW Driver Compatibility with
**                               device and revision ID.
**                               0 - Incompatible
**                               1 - Compatible
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCheckRev(struct file *fileHndl, UINT4 *pDevId, UINT4 *pRevId,
                      UINT4 *pStatus)
{
    UINT4 devId, revId;
#ifdef HWACCESSMODE_USB
    UINT4 val;
#endif
    INT4 result;

#ifdef HWACCESSMODE_USB
    /* revision ID extraction for emulation hardware - NOT APPLICABLE TO
       actual HyPHY 20G devices */
    result = sysHyPhy20gRead (fileHndl, 0x4F30000, &val);
    if (result) return result;

    *pDevId = val;

    result = sysHyPhy20gRead (fileHndl, 0x4F30004, &val);
    if (result) return result;

    *pRevId = val;

    /* unconditionally pass compatible status to emulation control layer.
       Control layer responsability to process emulation device and rev
       data */
    *pStatus = 1;

    return HYPHY20G_SUCCESS;
#endif

    /* get device revision */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
    if (result) return result;
    *pRevId = revId;

    /* get device part type */
    result = sysHyPhy20gGetDevicePartType(fileHndl, &devId);
    if (result) return result;
    *pDevId = devId;

    /* evaluation of compatibility between SW driver and device */
    /* if SW driver support B, C or D rev returns pStatus 1 */
    if(((*pRevId == HYPHY20G_REV_ID_B) ||
        (*pRevId == HYPHY20G_REV_ID_C) ||
        (*pRevId == HYPHY20G_REV_ID_D))){
        *pStatus = 1;
    } else {
        *pStatus = 0;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCheckRev */

/*******************************************************************************
**
**  hyPhy20gCheckRealRev
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts real device and revision data.  It
**                 returns a status to indicate SW driver compatibility with
**                 the real revision.
**
**                 This is a diagnostic function.  Actual devices will have
**                 device context as part of PCIe interface bring up.
**
**
**  INPUTS:         *fileHndl  - base address
**                  *pDevId    - pointer to real device ID
**                  *pRevId    - pointer to real revision ID
**                  *pStatus   - Status of SW Driver Compatibility with
**                               device and revision ID.
**                               0 - Incompatible
**                               1 - Compatible
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCheckRealRev(struct file *fileHndl, UINT4 *pDevId, UINT4 *pRevId,
                      UINT4 *pStatus)
{
#ifdef HWACCESSMODE_USB
    UINT4 val;
    INT4 result;
#endif
    sHYPHY20G_CTXT  *pDevCtxt;

#ifdef HWACCESSMODE_USB
    /* revision ID extraction for emulation hardware - NOT APPLICABLE TO
       actual HyPHY 20G devices */
    result = sysHyPhy20gRead (fileHndl, 0x4F30000, &val);
    if (result) return result;

    *pDevId = val;

    result = sysHyPhy20gRead (fileHndl, 0x4F30004, &val);
    if (result) return result;

    *pRevId = val;

    /* unconditionally pass compatible status to emulation control layer.
       Control layer responsability to process emulation device and rev
       data */
    *pStatus = 1;

    return HYPHY20G_SUCCESS;
#endif


    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        return HYPHY20G_ERR_INVALID_DEV;
    }

    /* get device revision */
    *pRevId = (UINT4)(pDevCtxt->realRevId);
	
    /* get device part type */
    *pDevId = (UINT4)(pDevCtxt->realDeviceId);

    /* evaluation of compatibility between SW driver and device */
    /* if SW driver support B C or D returns pStatus 1 */
    if(((*pDevId == HYPHY20G_HYPHY20G_DEVICE)   ||
        (*pDevId == HYPHY20G_HYPHY10G_DEVICE)  ||
        (*pDevId == HYPHY20G_META20G_DEVICE))  &&
        ((*pRevId == HYPHY20G_REV_ID_B) ||
        (*pRevId == HYPHY20G_REV_ID_D)  ||
        (*pRevId == HYPHY20G_REV_ID_C))){
        *pStatus = 1;
    } else {
        *pStatus = 0;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCheckRealRev */

#ifndef PMC_HW_CODE
/*******************************************************************************
**
**  hwHyPhy20gTimerInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function initializes the timers and variables in the
**                 RTOS_TIMER software context.
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - 0 - disable RTOS_TIMER SW context
**                               1 - enable RTOS_TIMER SW context
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hwHyPhy20gTimerInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    
    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (enable == 0) {
        /* free memory used by RTOS_TIMER SW context */
        result = sysHyPhy20gTimerDestroyCtxt(fileHndl);
        if (result) return result;
    } else {
        /* allocate and initialize memory used by RTOS_TIMER SW context */
        result = sysHyPhy20gTimerCreateCtxt(fileHndl);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* hwHyPhy20gTimerInit */

/*******************************************************************************
**
**  hwHyPhy20gStartTimer
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function starts the specified timer in the RTOS_TIMER
**                 software context.  Call hwHyPhy20gTimerInit must be called
**                 BEFORE this function.
**
**  INPUTS:         *fileHndl  - base address
**                  timerId    - timer ID
**                               Legal range 0 to 4
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hwHyPhy20gStartTimer(struct file *fileHndl, UINT4 timerId)
{
    TIMEVAL *pStartTime;
    sHYPHY20G_CTXT *pDevCtxt;
    
    /* argument checking */
    if (timerId > 4) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    HYPHY20G_ASSERT((pDevCtxt->pRtosTimerCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt->pRtosTimerCtxt is NULL.");
    
    switch (timerId) {
        case 0:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_0);
            break;
        case 1:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_1);
            break;
        case 2:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_2);
            break;
        case 3:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_3);
            break;
        case 4:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_4);
            break;
        default:
            pStartTime = NULL;
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set start time */
    sysHyPhy20gGetTimeOfDay(pStartTime);
    
    return HYPHY20G_SUCCESS;
} /* hwHyPhy20gStartTimer */

/*******************************************************************************
**
**  hwHyPhy20gStopTimer
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function stops the specified timer in the RTOS_TIMER SW
**                 context.  hwHyPhy20gStartTimer must be called BEFORE this
**                 function.
**
**  INPUTS:         *fileHndl  - base address
**                  timerId    - timer ID
**                               Legal range 0 to 4
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hwHyPhy20gStopTimer(struct file *fileHndl, UINT4 timerId)
{
    TIMEVAL *pEndTime;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (timerId > 4) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    
    HYPHY20G_ASSERT((pDevCtxt->pRtosTimerCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt->pRtosTimerCtxt is NULL.");
    
    switch (timerId) {
        case 0:
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_0);
            break;
        case 1:
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_1);
            break;
        case 2:
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_2);
            break;
        case 3:
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_3);
            break;
        case 4:
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_4);
            break;
        default:
            pEndTime = NULL;
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set end time */
    sysHyPhy20gGetTimeOfDay(pEndTime);

    return HYPHY20G_SUCCESS;
} /* hwHyPhy20gStopTimer */

/*******************************************************************************
**
**  hwHyPhy20gGetTimer
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function returns the start and stop time for a 
**                 specific timer in the RTOS_TIMER software context.
**
**  INPUTS:         *fileHndl  - base address
**                  timerId    - timer ID
**                               Legal range 0 to 4
**                  *pStartSec - start time (seconds)
**                  *pStartUs  - start time (microseconds)
**                  *pEndSec   - end time (seconds)
**                  *pEndUs    - end time (microseconds)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hwHyPhy20gGetTimer(struct file *fileHndl, UINT4 timerId, UINT4 *pStartSec,
                      UINT4 *pStartUs, UINT4 *pEndSec, UINT4 *pEndUs)
{
    TIMEVAL *pStartTime;
    TIMEVAL *pEndTime;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (timerId > 4) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    HYPHY20G_ASSERT((pDevCtxt->pRtosTimerCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt->pRtosTimerCtxt is NULL.");

    switch (timerId) {
        case 0:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_0);
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_0);
            break;
        case 1:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_1);
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_1);
            break;
        case 2:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_2);
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_2);
            break;
        case 3:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_3);
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_3);
            break;
        case 4:
            pStartTime = &(pDevCtxt->pRtosTimerCtxt->startTime_4);
            pEndTime = &(pDevCtxt->pRtosTimerCtxt->endTime_4);
            break;
        default:
            pStartTime = NULL;
            pEndTime = NULL;
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    *pStartSec = (UINT4)pStartTime->tv_sec;
    *pStartUs = (UINT4)pStartTime->tv_usec;
    
    *pEndSec   = (UINT4)pEndTime->tv_sec;
    *pEndUs   = (UINT4)pEndTime->tv_usec;

    return HYPHY20G_SUCCESS;
} /* hwHyPhy20gGetTimer */

/*******************************************************************************
**
**  hwHyPhy20gGetVar
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function returns the value of the specified variable
**                 from the RTOS_TIMER SW context.
**
**  INPUTS:         *fileHndl   - base address
**                  varId       - variable ID
**                                Legal range 0 to 4
**                  *pVal       - point to storage
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hwHyPhy20gGetVar(struct file *fileHndl, UINT4 varId, UINT4 *pVal)
{
    sHYPHY20G_RTOS_TIMER_CTXT *pRtosTimerCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (varId > 4) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pRtosTimerCtxt = pDevCtxt->pRtosTimerCtxt;
    HYPHY20G_ASSERT((pRtosTimerCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    switch (varId) {
        case 0:
            *pVal = pRtosTimerCtxt->var0;
            break;
        case 1:
            *pVal = pRtosTimerCtxt->var1;
            break;
        case 2:
            *pVal = pRtosTimerCtxt->var2;
            break;
        case 3:
            *pVal = pRtosTimerCtxt->var3;
            break;
        case 4:
            *pVal = pRtosTimerCtxt->var4;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hwHyPhy20gGetVar */

/*******************************************************************************
**
**  hwHyPhy20gSetVar
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sets the value of the specified variable
**                 in the RTOS_TIMER software context.
**
**  INPUTS:         *fileHndl   - base address
**                  varId       - variable ID
**                                Legal range 0 to 4
**                  value       - value
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hwHyPhy20gSetVar(struct file *fileHndl, UINT4 varId, UINT4 value)
{
    sHYPHY20G_RTOS_TIMER_CTXT *pRtosTimerCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (varId > 4) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pRtosTimerCtxt = pDevCtxt->pRtosTimerCtxt;
    HYPHY20G_ASSERT((pRtosTimerCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    switch (varId) {
        case 0:
            pRtosTimerCtxt->var0 = value;
            break;
        case 1:
            pRtosTimerCtxt->var1 = value;
            break;
        case 2:
            pRtosTimerCtxt->var2 = value;
            break;
        case 3:
            pRtosTimerCtxt->var3 = value;
            break;
        case 4:
            pRtosTimerCtxt->var4 = value;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hwHyPhy20gSetVar */
#endif /* PMC_HW_CODE */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  sysHyPhy20gPcieCfgRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a direct register read of a 32 bit
**                 wide register in PCIe configuration space.
**
**  INPUTS:        *fileHndl   - base address
**                 offset      - offset to the register location to be read
**                               as it appears in the hardware data sheet
**                 *pvalue      - pointer to storage for register value
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gPcieCfgRead (struct file *fileHndl, UINT4 offset, UINT4 *pvalue)
{
    /* content to be updated by user for system specific I/F */
    /* PCIe configuration access */
    UINT4 mode = 1;
    INT4 result;

    result = pcie_read(fileHndl, mode, offset, pvalue);
    if (result) return result;

    /* end of content to be updated by user for system specific I/F */

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gPcieCfgRead */


/*******************************************************************************
**
**  sysHyPhy20gPcieCfgWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a direct register write to a 32 bit
**                 wide register in the PCIe configuration space.
**
**  INPUTS:        *fileHndl   - base address
**                 offset      - offset to the register location to be read
**                               as it appears in the hardware data sheet
**                 value       - register value to be written
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gPcieCfgWrite (struct file *fileHndl, UINT4 offset, UINT4 value)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    /* content to be updated by user for system specific I/F */
    /* PCIe configuration access */
    UINT4 mode = 1;
    INT4 result;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    result = pcie_write(fileHndl, mode, offset, value);
    if (result) return result;

    if(pDevCtxt->gData.hyphy20gTraceWrite) {
        TRACEF("hyphy20gWrite hyphy20g 0x%0lx 0x%0lx\n", offset, value);
    }

    /* end of content to be updated by user for system specific I/F */

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gPcieCfgWrite */


/*******************************************************************************
**
**  sysHyPhy20gPcieBurstRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a PCIe burst register read of 32 bit
**                 wide registers.
**
**  INPUTS:        *fileHndl   - base address
**                 startAddr   - device address where burst read will start
**                 length      - burst length in 32-bit words (1-32)
**                 *dataList   - pointer to storage for return register values
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gPcieBurstRead (struct file *fileHndl, UINT4 startAddr,
                               UINT4 length, UINT4 *dataList)
{
    /* content to be updated by user for system specific I/F */
    INT4 result;

    result = pcie_burst_read(fileHndl, startAddr, length, dataList);
    if (result) return result;
    /* end of content to be updated by user for system specific I/F */

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gPcieBurstRead */


/*******************************************************************************
**
**  sysHyPhy20gPcieBurstWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a PCIe burst register write of 32 bit
**                 wide registers.
**
**  INPUTS:        *fileHndl   - base address
**                 startAddr   - device address where burst read will start
**                 length      - burst length in 32-bit words (1-32)
**                 *dataList   - pointer to storage for write register values
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gPcieBurstWrite (struct file *fileHndl, UINT4 startAddr,
                                UINT4 length, UINT4 *dataList)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    /* content to be updated by user for system specific I/F */
    UINT4 i, offset, value;
    INT4 result;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    result = pcie_burst_write(fileHndl, startAddr, length, dataList);
    if (result) return result;

    if(pDevCtxt->gData.hyphy20gTraceWrite) {
        for (i = 0; i < length; i++) {
            offset = startAddr + (i*0x4);
            value = *dataList;
            TRACEF("hyphy20gWrite hyphy20g 0x%0lx 0x%0lx\n", offset, value);
            dataList++;
        }
    }
    /* end of content to be updated by user for system specific I/F */

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gPcieBurstWrite */


/*******************************************************************************
**
**  sysHyPhy20gFwDownload
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads a binary file, writes its contents to
**                 SPRAM and auto-launch MIPS.
**
**  INPUTS:        *fileHndl  - base address
**                 *fread     - path to binary file
**                              (for the MIPS-SBC, this path would be:
**                                 /home/apps/firmware/<filename>)
**                  jumpAddr   - vector address, for MIPS to start execution at
**                               (Please refer to FW release notes, PMC-2091033,
**                                associated with this release for jumpAddr
**                                value)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_FW_INVALID_BOOT_STATE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gFwDownload(struct file *fileHndl, STRING *fread, UINT4 jumpAddr)
{
    struct file *f = NULL;
    UINT4 bEof;
    UINT4 length_read;
    UINT4 length;
    UINT4 burstAddr;
    UINT4 burstLength;
    UINT4 fullBufferValue;
    UINT4 val;
    UINT4 size;
    UINT8 filePosOffset;
    UINT4 *pDataList;
    UINT4 *pTmpDataList;
    UINT4 *pTmp;
    STRING buffer[5]; /* buffer is 5 + 1 bytes long */
    INT4 result;
    UINT4 regData;


    /* Check that MIPS is ready for FW download (check BOOT_INFO register for
       POLL_BOOT_ENABLE state) */
    result = sysHyPhy20gRead(fileHndl, HYPHY20G_TOP_REG_BT_INFO, &val);
    if (result) {
        return HYPHY20G_ERR_DEV_ACCESS;
    }
    val &= HYPHY20G_TOP_BIT_BT_INFO_BOOT_INFO_MSK;
    if (val != 0x1003) {
        return HYPHY20G_ERR_FW_INVALID_BOOT_STATE;
    }
   
    /* Open the file for reading */
    f = sysHyPhy20gFileOpen(fread);
    if (f == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* allocate memory for up to a maximum of 32 words of burst write data */
    size = sizeof(UINT4)*32;
    pDataList = (UINT4 *)sysHyPhy20gMemAlloc(size);
    if (pDataList == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    
    
    bEof = 0;
    filePosOffset = 0;
    length = 0;
    burstLength = 0;
    burstAddr = 0;
    pTmpDataList = pDataList;

    /* read contents from file and write to SPRAM */
    while (bEof == 0) {
        /* read 4-bytes from file */
        result = sysHyPhy20gFileRead(f, buffer, &filePosOffset, &length_read);
        if (result) {
            sysHyPhy20gFileClose(f);
            sysHyPhy20gMemFree(pDataList);
            return result;
        }
        buffer[length_read] = '\0';

        /* check for end of file or process read data */
        if (length_read == 0) {
            bEof = 1;
        } else {
            /* increment file pointer position */
            length = length + length_read;
            filePosOffset = length;

            /* format read data into group of 32-bit data based on endianness */
            pTmp = (UINT4 *) buffer;
            if (sysHyPhy20gEndianness() == BIG_ENDIAN) {
                fullBufferValue = *pTmp;
            } else {
                fullBufferValue = 0;
                fullBufferValue += (*pTmp & 0xff) << 24;
                fullBufferValue += (*pTmp & 0xff00) << 8;
                fullBufferValue += (*pTmp & 0xff0000) >> 8;
                fullBufferValue += (*pTmp & 0xff000000) >> 24;
            }

            /* store into memory */
            *pTmpDataList = fullBufferValue;
            pTmpDataList++;
            burstLength++;
        }

        /* write data to SPRAM */
        if (burstLength >= 32 || (bEof == 1 && burstLength != 0)) {
            pTmpDataList = pDataList;
            result = sysHyPhy20gPcieBurstWrite(fileHndl, burstAddr,
                                               burstLength, pTmpDataList);
            if (result) {
                sysHyPhy20gFileClose(f);
                sysHyPhy20gMemFree(pDataList);
                return result;
            }

            burstAddr = burstAddr + (burstLength * 4);
            pTmpDataList = pDataList;
            burstLength = 0;
        }
    } /* end of while */
    
    /* free memory */
    sysHyPhy20gMemFree(pDataList);
    /* close the file */
    sysHyPhy20gFileClose(f);

    /* Set HyPHY BOOT_JMP_ADDR_REG to the supplied value */
    result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_TOP_REG_BT_ADR,
                                   HYPHY20G_TOP_BIT_BT_ADR_BOOT_JMP_ADDR_MSK,
                                   jumpAddr);
    if (result) {
        return HYPHY20G_ERR_DEV_ACCESS;
    }
    
    /* set HyPHY BOOT_ENABLE to start code execution */
    result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_TOP_REG_BT_CTL,
                                   HYPHY20G_TOP_BIT_BT_CTL_BOOT_ENABLE_MSK, 1);
    if (result) {
        return HYPHY20G_ERR_DEV_ACCESS;
    }
    
    /* Poll for boot success - NORMAL_BOOT_SUCCESS state from BOOT_INFO
       register */
    sysHyPhy20gUsDelay(500);
    sysHyPhy20gRead(fileHndl, HYPHY20G_TOP_REG_BT_INFO, &regData);
    DBG_PRINT("%s, %s, %d, boot info: 0x%08x\n", __FILE__, __FUNCTION__, __LINE__, regData);
    printk("Firmware download sucess!\n"); 
#if 1
    if (regData != 0x00005000){
        printk("But, regData is not 0x5000\n");
	    return HYPHY20G_ERR_DEV_ACCESS;
    }
#endif
    result = sysHyPhy20gPollBit(fileHndl, HYPHY20G_TOP_REG_BT_INFO,
                                HYPHY20G_TOP_BIT_BT_INFO_BOOT_INFO_MSK, 0x5000,
                                500);
    if (result) 
    {
	return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gFwDownload */

/*******************************************************************************
**
**  sysHyPhy20gFwReload
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reloads the firmware without the need to
**                 reset the device first by performing a soft reset on the
**                 MIPS core.
**
**                 Prior to using this function to reload new FW, ensure
**                 previous version of FW is operational using
**                 sysHyPhy20gFwDownload.
**
**  INPUTS:        *fileHndl  - base address
**                 *fread     - path to binary file
**                              (for the MIPS-SBC, this path would be:
**                                 /home/apps/firmware/<filename>)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gFwReload(struct file *fileHndl, STRING *fread)
{
    struct file *f = NULL;
    UINT4 bEof;
    UINT4 length_read;
    UINT4 length;
    UINT4 burstAddr;
    UINT4 burstLength;
    UINT4 fullBufferValue;
    UINT4 val;
    UINT4 size;
    UINT8 filePosOffset;
    UINT4 *pDataList;
    UINT4 *pTmpDataList;
    UINT4 *pTmp;
    STRING buffer[5]; /* buffer is 5 + 1 bytes long */
    INT4 result;

    /* Check that FW is operational before reloading FW */
    result = hyPhy20gRegFieldRead(fileHndl, HYPHY20G_TOP_REG_BT_INFO,
                                  HYPHY20G_TOP_BIT_BT_INFO_BOOT_INFO_MSK, &val);
    if (result) return result;
    if (val != 0x5000) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    result = hyPhy20gRegFieldRead(fileHndl, HYPHY20G_TOP_REG_APP_USR_1,
                                  HYPHY20G_TOP_BIT_APP_USR_1_APP_USER1_MSK,
                                  &val);
    if (result) return result;
    if ((val & 0x1) != 0x1) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Place MIPS core in reset */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_CTL,
                                 HYPHY20G_TOP_BIT_CTL_MIPS_RST_OFF, 1);
    if (result) return result;

    /* Indicate SW is resetting FW */
    result = hyPhy20gRegFieldRead(fileHndl, HYPHY20G_TOP_REG_APP_USR_0,
                                  HYPHY20G_TOP_BIT_APP_USR_0_APP_USER0_MSK,
                                  &val);
    if (result) return result;

    val |= 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_TOP_REG_APP_USR_0,
                                   HYPHY20G_TOP_BIT_APP_USR_0_APP_USER0_MSK,
                                   val);
    if (result) return result;

    /* Disable watchdog timer */
    result = sysHyPhy20gRead(fileHndl, HYPHY20G_WDOG_REG_WTCHDG_CTL, &val);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val, HYPHY20G_WDOG_BIT_WTCHDG_CTL_RESEN_MSK,
                                   0);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val, HYPHY20G_WDOG_BIT_WTCHDG_CTL_INTEN_MSK,
                                   0);
    if (result) return result;
    result = sysHyPhy20gWrite(fileHndl, HYPHY20G_WDOG_REG_WTCHDG_CTL, val);
    if (result) return result;

    /* Re-write BOOT_INFO register for FW load check later */
    result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_TOP_REG_BT_INFO,
                                   HYPHY20G_TOP_BIT_BT_INFO_BOOT_INFO_MSK,
                                   0x1003);
    if (result) return result;

    /* Open the file for reading */
    f = sysHyPhy20gFileOpen(fread);
    if (f == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* allocate memory for up to a maximum of 32 words of burst write data */
    size = sizeof(UINT4)*32;
    pDataList = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pDataList == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    bEof = 0;
    filePosOffset = 0;
    length = 0;
    burstLength = 0;
    burstAddr = 0;
    pTmpDataList = pDataList;

    /* read contents from file and write to SPRAM */
    while (bEof == 0) {
        /* read 4-bytes from file */
        result = sysHyPhy20gFileRead(f, buffer, &filePosOffset, &length_read);
        if (result) {
            sysHyPhy20gMemFree(pDataList);
            sysHyPhy20gFileClose(f);
            return result;
        }
        buffer[length_read] = '\0';

        /* check for end of file or process read data */
        if (length_read == 0) {
            bEof = 1;
        } else {
            /* increment file pointer position */
            length = length + length_read;
            filePosOffset = length;

            /* format read data into group of 32-bit data based on endianness */
            pTmp = (UINT4 *) buffer;
            if (sysHyPhy20gEndianness() == BIG_ENDIAN) {
                fullBufferValue = *pTmp;
            } else {
                fullBufferValue = 0;
                fullBufferValue += (*pTmp & 0xff) << 24;
                fullBufferValue += (*pTmp & 0xff00) << 8;
                fullBufferValue += (*pTmp & 0xff0000) >> 8;
                fullBufferValue += (*pTmp & 0xff000000) >> 24;
            }

            /* store into memory */
            *pTmpDataList = fullBufferValue;
            pTmpDataList++;
            burstLength++;
        }

        /* write data to SPRAM */
        if (burstLength >= 32 || (bEof == 1 && burstLength != 0)) {
            pTmpDataList = pDataList;
            result = sysHyPhy20gPcieBurstWrite(fileHndl, burstAddr,
                                               burstLength, pTmpDataList);
            if (result) {
                sysHyPhy20gFileClose(f);
                sysHyPhy20gMemFree(pDataList);
                return result;
            }

            burstAddr = burstAddr + (burstLength * 4);
            pTmpDataList = pDataList;
            burstLength = 0;
        }
    } /* end of while */

    /* free memory */
    sysHyPhy20gMemFree(pDataList);
    /* close the file */
    sysHyPhy20gFileClose(f);

    /* Bring MIPS out of reset */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_CTL,
                                 HYPHY20G_TOP_BIT_CTL_MIPS_RST_OFF, 0);
    if (result) return result;

    /* Poll for boot success - NORMAL_BOOT_SUCCESS state from BOOT_INFO
       register */
    result = sysHyPhy20gPollBit(fileHndl, HYPHY20G_TOP_REG_BT_INFO,
                                HYPHY20G_TOP_BIT_BT_INFO_BOOT_INFO_MSK, 0x5000,
                                500);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sysHyPhy20gFwReload */

/*******************************************************************************
**
**  sysHyPhy20gFileOpen
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function opens a file. Content must be updated by
**                 user for their specific system requirements.
**
**  INPUTS:        *path   - path to file
**                           (for the MIPS-SBC, this path would be:
**                              /home/apps/firmware/<filename>)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
struct file* sysHyPhy20gFileOpen(const STRING *path)
{
    /* content to be updated by user for system specific functions */
    struct file* filp = NULL;
    mm_segment_t oldfs;

    if (path == NULL) {
        return NULL;
    }
    
    oldfs = get_fs();

    set_fs(get_ds());
    filp = filp_open((char *) path, O_RDONLY, 00);
    set_fs(oldfs);
    if (IS_ERR(filp)) {
        return NULL;
    }
    return filp;

} /* sysHyPhy20gFileOpen */

/*******************************************************************************
**
**  sysHyPhy20gFileClose
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function closes a file. Content must be updated by
**                 user for their specific system requirements.
**
**  INPUTS:        *file  - file pointer
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gFileClose(struct file *file)
{
    /* content to be updated by user for system specific functions */
    filp_close(file, NULL);
    return HYPHY20G_SUCCESS;

} /* sysHyPhy20gFileClose */


/*******************************************************************************
**
**  sysHyPhy20gFileRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads 4-bytes from the file at the specified
**                 pointer location. Content must be updated by
**                 user for their specific system requirements.
**
**  INPUTS:        *file  - file pointer
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
*******************************************************************************/
INT4 sysHyPhy20gFileRead(struct file *f, STRING *buffer, UINT8 *offset,
                         UINT4 *rdLen)
{
    /* content to be updated by user for system specific functions */
    mm_segment_t oldfs;

    if (f == NULL || 
        buffer == NULL || 
        offset == NULL || 
        rdLen == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }
    
    oldfs = get_fs();
    set_fs(get_ds());
    *rdLen = f->f_op->read(f, buffer, 4, offset);
    set_fs(oldfs);

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gFileRead */

/*******************************************************************************
**
**  sysHyPhy20gFileReadBytes
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads n-bytes from the file at the specified
**                 pointer location. Content must be updated by
**                 user for their specific system requirements.
**
**  INPUTS:        *file  - file pointer
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
*******************************************************************************/
INT4 sysHyPhy20gFileReadBytes(struct file *f, char *buffer, UINT4 bytes,
                              UINT8 *offset, UINT4 *rdLen)
{
    /* content to be updated by user for system specific functions */
    mm_segment_t oldfs;

    if (f == NULL || 
        buffer == NULL || 
        offset == NULL || 
        rdLen == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    oldfs = get_fs();
    set_fs(get_ds());
    *rdLen = f->f_op->read(f, buffer, bytes, offset);
    set_fs(oldfs);

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gFileReadBytes */

/*******************************************************************************
**
**  sysHyPhy20gPollBit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function simply polls masked register data until
**                 it is matches the pollFlg, times out, or driver failure.
**
**  INPUTS:        *fileHndl   - base address
**                 offset      - offset to the register location as it
**                               appears in the hardware data sheet
**                 mask        - mask to apply to byte read
**                 pollFlg     - desired poll result, for example if mask
**                               0x0f00 and we want masked field to be 
**                               0x6 then pollFlg must be specified as
**                               0x6 << 8 or 0x0600.  If we want to poll
**                               for a result of 0, simply set to 0.
**                 delay       - number of 1 microsecond polls performed before
**                               declaring failure.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 sysHyPhy20gPollBit(struct file *fileHndl, UINT4 offset, UINT4  mask,
                        UINT4  pollFlg, UINT4 delay)
{
    /* variable declaration */
    UINT4 i;
    UINT4 busyBit = 0;
    UINT4 drvBusyBit;
    /* typecast register offset for gendriver type */
    UINT4 drvOffset = (UINT4) offset;
    INT4  result;

    /* Poll masked field until same as pollFlg value, or quit after delay cycles. */
    for (i = 0; i < (delay+1); i++) {
        /* Read register containing busy bit */
        if (gen_read(fileHndl, drvOffset, &drvBusyBit))
           return HYPHY20G_ERR_DEV_ACCESS;

        /* typecast gendriver returned value to 16 bit number. */
        busyBit = drvBusyBit;

        /* Mask out busy bit only */
        busyBit &= mask;

        if (busyBit == pollFlg)
            break;
        if (i < delay) {
            /* wait 1 us between reads except for last possible read */
            result = sysHyPhy20gUsDelay(1);
            if (result) return result;
        }
    }

    /* Verify 'busy bit' field is the same as the pollFlg,
    if not then return error. */
    if (busyBit == pollFlg) {
        return HYPHY20G_SUCCESS;
    } else {
        return HYPHY20G_ERR_POLL_TIMEOUT;
    }

} /* sysHyPhy20gPollBit */


/*******************************************************************************
**
**  sysHyPhy20gRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a direct register read of a 32 bit
**                 wide register.
**
**  INPUTS:        *fileHndl   - base address
**                 offset      - offset to the register location to be read
**                               as it appears in the hardware data sheet
**                 *value      - pointer to storage for register value
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gRead (struct file *fileHndl, UINT4 offset, UINT4 *pvalue)
{
    INT4 result;
    result = gen_read(fileHndl, offset, pvalue);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gRead */


/*******************************************************************************
**
**  sysHyPhy20gWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a direct register write to a 32 bit
**                 wide register.
**
**  INPUTS:        *fileHndl   - base address
**                 offset      - offset to the register location to be read
**                               as it appears in the hardware data sheet
**                 value       - register value to be written
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gWrite (struct file *fileHndl, UINT4 offset, UINT4 value)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    INT4 result;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    result = gen_write(fileHndl, offset, value);
    if (result) return result;

    if(pDevCtxt->gData.hyphy20gTraceWrite) {
        TRACEF("hyphy20gWrite hyphy20g 0x%0lx 0x%0lx\n", offset, value);
    }

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gWrite */


/*******************************************************************************
**
**  sysHyPhy20gBitWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function writes a single bit to a register.
**
**  INPUTS:        *fileHndl - base address
**                 offset    - offset to the register location as it
**                             appears in the hardware data sheet
**                 bit       - bit position to be written
**                 value     - register value
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gBitWrite (struct file *fileHndl, UINT4 offset, UINT4 bit,
                          UINT4 value)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    /* variable declaration */
    UINT4 bitmask = 0;
    UINT4 drvValue;
    UINT4 drvOffset;
    UINT4 tempValue;
    INT4 result;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    value &= 0x00000001;

    if ((value != 0) && (value !=1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (bit <= 31) {
        bitmask = 0x00000001 << bit;
    } else {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /*  typecast register address for generic driver */
    drvOffset = (UINT4) offset;

    result = gen_read(fileHndl, drvOffset, &drvValue);
    if (result) return result;

    /* typecast returned value from generic driver */
    tempValue = (UINT4) drvValue;
    if (value == 1) {
        tempValue = tempValue | bitmask;
    } else {
        tempValue = tempValue & (~bitmask);
    }

    drvValue = (UINT4) tempValue;
    result = gen_write(fileHndl, drvOffset, drvValue);
    if (result) return result;

    if(pDevCtxt->gData.hyphy20gTraceWrite) {
        TRACEF("hyphy20gWrite hyphy20g 0x%0lx 0x%0lx\n", drvOffset, drvValue);
    }

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gBitWrite */


/*******************************************************************************
**
**  sysHyPhy20gBitRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads a single bit from a register.
**
**  INPUTS:        *fileHndl - base address
**                 offset    - offset to the register location as it
**                             appears in the hardware data sheet
**                 bit       - offset for the bit to be read 0 to 31
**                 *value    - pointer to storage for the bit value (0 or 1)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gBitRead (struct file *fileHndl, UINT4 offset, UINT4 bit,
                         UINT4 *value)
{
    /* variable declaration */
    UINT4 bitmask = 0;
    UINT4 drvValue;
    UINT4 drvOffset;
    UINT4 tempValue;
    INT4 result;

    /*  typecast register address for generic driver */
    drvOffset = (UINT4) offset;

    result = gen_read(fileHndl, drvOffset, &drvValue);
    if (result) return result;

    /* typecast returned value from generic driver */
    tempValue = (UINT4) drvValue;

    if (bit <= 31) {
        bitmask = 0x00000001 << bit;
    } else {
        return HYPHY20G_ERR_DEV_ACCESS;
    }
    bitmask = (tempValue & bitmask) >> bit;
    *value = bitmask;

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gBitRead */


/*******************************************************************************
**
**  hyPhy20gRegFieldWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function writes a value to a specified field for a
**                 register
**
**  INPUTS:        *fileHndl  - base address
**                 reg        - register number to have a field modified
**                 fldMsk     - mask of the bits in the register to be modifed
**                              The bits of the mask must be continuous
**                 fldValue   - value to be written to the field identified by
**                              fldMsk.  fldValue is expected to be provided
**                              with a zero offset.
**
**  CAUTIONS:      This function can only be called against registers that
**                 have the ability to be read/modify/written.  This function
**                 will not work for indirect data registers as these registers
**                 are architected with seperate read and write ports.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gRegFieldWrite (struct file *fileHndl, UINT4 reg, UINT4 fldMsk,
                            UINT4 fldValue)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    /* variable declaration */
    UINT4 bitMask=0;
    UINT4 mskOffset = 0;
    UINT4 regValue = 0;
    INT4 result;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (!fldMsk) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    bitMask = fldMsk;

    while (!(bitMask & 0x1)) {
        bitMask = bitMask >> 1;
        mskOffset++;
    }

    fldValue = fldValue << mskOffset;

    result = sysHyPhy20gRead(fileHndl, reg, &regValue);
    if (result) return result;

    regValue &= ~fldMsk;
    regValue |= (fldValue & fldMsk);

    result = sysHyPhy20gWrite(fileHndl, reg, regValue);
    if (result) return result;

    if(pDevCtxt->gData.hyphy20gTraceWrite) {
        TRACEF("hyphy20gWrite hyphy20g 0x%0lx 0x%0lx\n", reg, regValue);
    }

    return HYPHY20G_SUCCESS;
}  /* hyPhy20gRegFieldWrite */

/*******************************************************************************
**
**  hyPhy20gRegFieldRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads a value from a specified field of a
**                 register.
**
**  INPUTS:        *fileHndl  - base address
**                 reg        - register address to be read from
**                 fldMsk     - mask of the bits in the register to be read
**                              The bits of the mask must be continuous
**                 *fldValue  - value read from field identified by
**                              fldMsk.  fldValue is returned
**                              with a zero offset.
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gRegFieldRead (struct file *fileHndl, UINT4 reg, UINT4 fldMsk,
                           UINT4 *fldValue)
{
    /* variable declaration */
    UINT4 bitMask = 0;
    UINT4 mskOffset = 0;
    UINT4 val;
    UINT4 tmp;
    INT4 result;

    //DBG_PRINT("variable = 0x%lx\n", variable);
    if (!fldMsk) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* access device register */
    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if (result) return result;

    bitMask = fldMsk;

    while (!(bitMask & 0x1)) {
        bitMask = bitMask >> 1;
        mskOffset++;
    }
    //DBG_PRINT("mskOffSet = %i\n", mskOffset);
    tmp = val;
    tmp = (tmp & fldMsk) >> mskOffset;
    //DBG_PRINT("tmp = %i\n", tmp);
    *fldValue = tmp;
    //DBG_PRINT("fldValue = %i\n", *fldValue);

    return HYPHY20G_SUCCESS;
}  /* hyPhy20gRegFieldRead */


/*******************************************************************************
**
**  hyPhy20gVarFieldWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function writes a value to a specified field for a
**                 variable
**
**  INPUTS:        *variable  - pointer to UINT4 type variable to have field
**                              modified
**                 fldMsk     - mask of the bits in the register to be modifed
**                               The bits of the mask must be continuous
**                 fldValue   - value to be written to the field identified by
**                              fldMsk.  fldValue is expected to be provided
**                              with a zero offset.
**
**  NOTES:         This function is intended to modify fields of indirect data
**                 registers that have been read from the device.  The usage
**                 is as follows:
**                 1- perform indirect read access and store indirect data
**                    register value to a variable
**                 2- modify fields of variable as required using this function
**                 3- perform indirect write access to update the indirect
**                    memory field
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVarFieldWrite (UINT4 *variable, UINT4 fldMsk, UINT4 fldValue)
{
    /* variable declaration */
    UINT4 bitMask = 0;
    UINT4 mskOffset = 0;
    UINT4 tmp;

    if (!fldMsk) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    bitMask = fldMsk;

    while (!(bitMask & 0x1)) {
        bitMask = bitMask >> 1;
        mskOffset++;
    }

    fldValue = fldValue << mskOffset;

    tmp = *variable;
    tmp &= ~fldMsk;
    tmp |= (fldValue & fldMsk);
    *variable = tmp;

    return HYPHY20G_SUCCESS;
}  /* hyPhy20gVarFieldWrite */

/*******************************************************************************
**
**  hyPhy20gVarFieldRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads a value from a specified field of a
**                 variable
**
**  INPUTS:        variable  -  UINT4 type variable to read from
**                 fldMsk     - mask of the bits in the register to be read
**                               The bits of the mask must be continuous
**                 *fldValue  - value read from field identified by
**                              fldMsk.  fldValue is returned
**                              with a zero offset.
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVarFieldRead (UINT4 variable, UINT4 fldMsk, UINT4 *fldValue)
{
    /* variable declaration */
    UINT4 bitMask = 0;
    UINT4 mskOffset = 0;
    UINT4 tmp;

    if (!fldMsk) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    bitMask = fldMsk;

    while (!(bitMask & 0x1)) {
        bitMask = bitMask >> 1;
        mskOffset++;
    }
    tmp = variable;
    tmp = (tmp & fldMsk) >> mskOffset;
    *fldValue = tmp;

    return HYPHY20G_SUCCESS;
}  /* hyPhy20gVarFieldRead */

/*******************************************************************************
**
**  sysHyPhy20gMutexAquire
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function aquires a HCPU generic MUTEX.  It will poll
**                 MUTEX bit for 100 msecond before declaring a timeout.
**
**                 When a MUTEX is aquired, it is the calling function's
**                 responsability to release the MUTEX with
**                 sysHyPhy20gMutexRelease.
**
**  INPUTS:        *fileHndl   - base address
**                 mutex       - MUTEX number to aquire
**                               0 - 15
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gMutexAquire (struct file *fileHndl, UINT4 mutex)
{
    /* variable declaration */
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;

    /* argument checking */
    if(mutex > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_TOP_REG_MUTEX_OFFSET(mutex);
    mask = HYPHY20G_TOP_BIT_MUTEX_MUTEX_MSK;

    /* Aquire Mutex */
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_100MSEC);
    /* Mutex Aquisition Failed */
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;

    /* Mutex Aquisition Successful */
    return HYPHY20G_SUCCESS;

} /* sysHyPhy20gMutexAquire */

/*******************************************************************************
**
**  sysHyPhy20gMutexRelease
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function unconditionally releases a HCPU generic MUTEX.
**
**                 It is the calling function's responsability to ensure that
**                 the MUTEX was aquired with sysHyPhy20gMutexAquire.  Otherwise
**                 HyPHY 20G FW operation may become corrupted.
**
**  INPUTS:        *fileHndl   - base address
**                 mutex       - MUTEX number to aquire
**                               0 - 15
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gMutexRelease (struct file *fileHndl, UINT4 mutex)
{
    /* variable declaration */
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;

    /* argument checking */
    if(mutex > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_TOP_REG_MUTEX_OFFSET(mutex);
    offset = HYPHY20G_TOP_BIT_MUTEX_MUTEX_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gMutexRelease */

/*******************************************************************************
**
**  sysHyPhy20gMsDelay
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function implements a milli-second delay.
**
**  INPUTS:        msDelay   - number of milli-second delay
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gMsDelay (UINT4 msDelay)
{
    /* content to be updated by user for system specific I/F */
    mdelay(msDelay);
    /* end of content to be updated by user for system specific I/F */

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gMsDelay */

/*******************************************************************************
**
**  sysHyPhy20gUsDelay
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function implements a micro-second delay timer.
**
**  INPUTS:        usDelay   - number of micro-second delay
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gUsDelay (UINT4 usDelay)
{
    /* content to be updated by user for system specific I/F */
    udelay(usDelay);
    /* end of content to be updated by user for system specific I/F */

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gUsDelay */

/*******************************************************************************
**
**  hyPhy20gSetTraceWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function enables or disables the trace of register write
**                 accesses.
**
**  INPUTS:        *fileHndl   - base address
**                 enbl        - 0 disable the trace of register write accesses
**                               1 enable the trace of register write accesses
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSetTraceWrite (struct file *fileHndl, UINT2 enbl)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if(enbl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(enbl) {
        pDevCtxt->gData.hyphy20gTraceWrite = 1;
    } else {
        pDevCtxt->gData.hyphy20gTraceWrite = 0;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSetTraceWrite */

/*******************************************************************************
**
**  sysHyPhy20gGetDevicePartType
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function returns the device part type number.
**                 hyPhy20gCreateCtxt must be called prior to calling this
**                 function.
**
**  INPUTS:        *fileHndl  - base address
**                 *pPartType - pointer to storage for part type number
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_DEV
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gGetDevicePartType (struct file *fileHndl, UINT4 *pPartType)
{
    sHYPHY20G_CTXT  *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        return HYPHY20G_ERR_INVALID_DEV;
    }

    *pPartType = (UINT4)(pDevCtxt->mimicDeviceID);

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gGetDevicePartType */

/*******************************************************************************
**
**  sysHyPhy20gGetDeviceRevId
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function returns the device revision ID.
**                 hyPhy20gCreateCtxt must be called prior to calling this
**                 function.
**
**  INPUTS:        *fileHndl - base address
**                 *pRevId   - pointer to storage for revision ID
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_DEV
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gGetDeviceRevId (struct file *fileHndl, UINT4 *pRevId)
{
    sHYPHY20G_CTXT  *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        return HYPHY20G_ERR_INVALID_DEV;
    }

    *pRevId = (UINT4)(pDevCtxt->mimicRevId);

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gGetDeviceRevId */

/*******************************************************************************
**
**  sysHyPhy20gGetDeviceCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         fileHndl  - base address
**
**  OUTPUTS:        None      - None
**
**  RETURNS:      The pointer to context for special device
**                  
**
*******************************************************************************/
sHYPHY20G_CTXT* sysHyPhy20gGetDeviceCtxt(struct file *fileHndl)
{
    UINT4        devIndex = 0;
    qnf_softc_t *sc;
    sc = (qnf_softc_t *)fileHndl->private_data;
    
    devIndex = sc->unit;
    DBG_PRINT("%s, %s, %d, devIndex is: %d\n", __FILE__, __FUNCTION__, __LINE__, devIndex);
    			
    return (sHYPHY20G_CTXT *)&(hyPhy20gCtxt[devIndex]);
} /* sysHyPhy20gGetDeviceCtxt */

/*******************************************************************************
**
**  sysHyPhy20gCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         None      - None
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        NULL
**                  
**
*******************************************************************************/
sHYPHY20G_CTXT* sysHyPhy20gCreateCtxt(struct file *fileHndl)
{    
    return NULL;
} /* sysHyPhy20gCreateCtxt */

/*******************************************************************************
**
**  sysHyPhy20gDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         None      - None
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        NULL
**                  
**
*******************************************************************************/
sHYPHY20G_CTXT* sysHyPhy20gDestroyCtxt(struct file *fileHndl)
{
    return NULL;
} /* sysHyPhy20gDestroyCtxt */

#ifndef PMC_HW_CODE
/*******************************************************************************
**
**  sysHyPhy20gTimerCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates memory for RTOS_TIMER software context
**
**  INPUTS:         None      - None
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gTimerCreateCtxt(struct file *fileHndl)
{
    /* variable declaration */
    sHYPHY20G_RTOS_TIMER_CTXT *pRtosTimerCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 size;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pRtosTimerCtxt is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pRtosTimerCtxt != NULL) {
        sysHyPhy20gTimerDestroyCtxt(fileHndl);
    }

    /* allocate memory for and associate with pDevCtxt */
    size = sizeof(sHYPHY20G_RTOS_TIMER_CTXT);
    pDevCtxt->pRtosTimerCtxt = (sHYPHY20G_RTOS_TIMER_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pRtosTimerCtxt == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    DBG_PRINT("Timer Context memory allocated: pRtosTimerCtxt addr=0x%x, size=%ld\n",
              (unsigned int)pDevCtxt->pRtosTimerCtxt, size);
    pRtosTimerCtxt = pDevCtxt->pRtosTimerCtxt;

    /* initialize context */
    pRtosTimerCtxt->valid = HYPHY20G_VALID_VAL_32BIT;
    pRtosTimerCtxt->var0 = 0;
    pRtosTimerCtxt->var1 = 0;
    pRtosTimerCtxt->var2 = 0;
    pRtosTimerCtxt->var3 = 0;
    pRtosTimerCtxt->var4 = 0;

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gTimerCreateCtxt */

/*******************************************************************************
**
**  sysHyPhy20gTimerDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Frees memory allocated for RTOS_TIMER software context.
**
**  INPUTS:         None      - None
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gTimerDestroyCtxt(struct file *fileHndl)
{
    /* variable declaration */
    sHYPHY20G_RTOS_TIMER_CTXT *pRtosTimerCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve host message context */
    pRtosTimerCtxt = pDevCtxt->pRtosTimerCtxt;
    if(pRtosTimerCtxt == NULL){
        /* Nothing to destroy */
    	return HYPHY20G_SUCCESS;
    }

    /* destroy host message context */
    sysHyPhy20gMemFree(pDevCtxt->pRtosTimerCtxt);

    return HYPHY20G_SUCCESS;
} /* sysHyPhy20gTimerDestroyCtxt */

/*******************************************************************************
**
**  sysHyPhy20gTimerGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of Timer context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of Timer context
**
*******************************************************************************/
UINT4 sysHyPhy20gTimerGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 size = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* include ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();

    /* calculate context size */
    if (pDevCtxt->pRtosTimerCtxt != NULL) {
        size += sizeof(sHYPHY20G_RTOS_TIMER_CTXT);
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx TIMEVAL size %d\n", 
                __FILE__, __FUNCTION__, __LINE__, size, sizeof(TIMEVAL));

    return size;
}/* sysHyPhy20gTimerGetCtxtSize */

/*******************************************************************************
**
**  sysHyPhy20gTimerExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the Timer context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for Timer context extraction
**
**  OUTPUTS:        pExtractedSize -the context size(in byte) has been extracted
**                  into the buffer
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gTimerExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_RTOS_TIMER_CTXT *pRtosTimerCtxt;
    UINT4 rtLen;
    UINT4 offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pExtractedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pExtractedSize is NULL.");

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pRtosTimerCtxt = pDevCtxt->pRtosTimerCtxt;

    DBG_PRINT("%s, %s, %d, header offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    len = (pRtosTimerCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
            (sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_RTOS_TIMER_CTXT));

    /* insert ctxt buffer header */
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                            HYPHY_RTOS_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);
    
    if (pRtosTimerCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, payload offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, offset);        
        /* copy the context into the buffer */
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                (void *)pRtosTimerCtxt, sizeof(sHYPHY20G_RTOS_TIMER_CTXT));
        offset += sizeof(sHYPHY20G_RTOS_TIMER_CTXT);
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;

    return HYPHY20G_SUCCESS;
}/* sysHyPhy20gTimerExtractCtxt */

/*******************************************************************************
**
**  sysHyPhy20gTimerRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the Timer context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored Timer context for restoration
**
**  OUTPUTS:        pParsedSize -the data size(in byte) has been parsed in the
**                  buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gTimerRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_RTOS_TIMER_CTXT *pRtosTimerCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the RTOS header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_RTOS_CTXT_ID, HYPHY_NO_INDEX);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_RTOS_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
		/* maybe the pre-stored data without any RTOS Timer header/data */	
        return HYPHY20G_SUCCESS;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    DBG_PRINT("%s, %s, %d, 1st offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, len);

    /* check if the payload is valid */
    if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
        offset = sizeof(sHYPHY20G_CTXT_HDR);
    } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
        /* create Timer context */
        result = sysHyPhy20gTimerCreateCtxt(fileHndl);
        if (result) {
            DBG_PRINT("%s, %s, %d, sysHyPhy20gTimerCreateCtxt failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return result;
        }
        
        /* retrieve device context */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pDevCtxt is NULL.");

        pRtosTimerCtxt = pDevCtxt->pRtosTimerCtxt;

        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        /* payload len */
        len = pCtxtBufHdr->len - 
                sizeof(sHYPHY20G_CTXT_HDR);
        
        sysHyPhy20gMemCpy((void *)(pRtosTimerCtxt), 
                        ((UINT1 *)pCtxtBuf + offset), len);
        offset += len;
    } else {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        *pParsedSize = 0;
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);
    
    *pParsedSize = offset;

    return HYPHY20G_SUCCESS;
}/* sysHyPhy20gTimerRestoreCtxt */

/********************************************************************************
**
**  rtosTimerHyPhy20gRtosTimerAuditCtxt  
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function audit rtosTimer subCTXT.
**                  
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 rtosTimerHyPhy20gRtosTimerAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_RTOS_TIMER_CTXT *pRtosTimerCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    /* the below check rule mainly come from sysHyPhy20gTimerCreateCtxt */
    pRtosTimerCtxt=pDevCtxt->pRtosTimerCtxt;
    if (pRtosTimerCtxt!=NULL)
    {
        if (pRtosTimerCtxt->valid!=HYPHY20G_VALID_VAL_32BIT)
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
    }
    
    return HYPHY20G_SUCCESS;
    
}/*rtosTimerHyPhy20gRtosTimerAuditCtxt*/
#endif /* PMC_HW_CODE */




inline INT4 pcie_read(struct file *fileHndl, UINT4 mode, UINT4 offset, UINT4 *pvalue)
{
    uint32_t    offset_tmp;
    qnf_softc_t *sc;
    sc = (qnf_softc_t *)fileHndl->private_data;
    
    if (mode == 1) {
        /* pci config access */
        offset_tmp = ((offset & 0xff) << 24) | ((offset & 0xff00) << 8) | ((offset & 0xff0000) >> 8) | ((offset & 0xff000000) >> 24);
        *(unsigned int *)(sc->mmio + 0x100044) = offset_tmp;
	*(unsigned int *)(sc->mmio + 0x100040) = 0x05000000;
	
	sysHyPhy20gUsDelay(200);
        
	*pvalue = *(unsigned int *)(sc->mmio + 0x10004C);
    } else if (mode == 0) {
        /* pci register access */
        offset_tmp = ((offset & 0xff) << 24) | ((offset & 0xff00) << 8) | ((offset & 0xff0000) >> 8) | ((offset & 0xff000000) >> 24);
	*(unsigned int *)(sc->mmio + 0x100044) = offset_tmp;
	*(unsigned int *)(sc->mmio + 0x100040) = 0x01000000;
	
	sysHyPhy20gUsDelay(200);
	
	*pvalue = *(unsigned int *)(sc->mmio + 0x10004C);
    } else {
        printk(KERN_WARNING "[pcie_read]: unkown mode!\n");
        return HYPHY20G_FAILURE;
    }
    
    return HYPHY20G_SUCCESS;
}

inline INT4 pcie_write(struct file *fileHndl, UINT4 mode, UINT4 offset, UINT4 value)
{
    uint32_t    offset_tmp;
    qnf_softc_t *sc;
    sc = (qnf_softc_t *)fileHndl->private_data;
    
    if (mode == 1) {
        /* pci config access */
        offset_tmp = ((offset & 0xff) << 24) | ((offset & 0xff00) << 8) | ((offset & 0xff0000) >> 8) | ((offset & 0xff000000) >> 24);
	*(unsigned int *)(sc->mmio + 0x100044) = offset_tmp;
	*(unsigned int *)(sc->mmio + 0x100048) = value;
	*(unsigned int *)(sc->mmio + 0x100040) = 0x06000000;
	
	sysHyPhy20gUsDelay(200);
    } else if (mode == 0) {
        /* pci register access */
        offset_tmp = ((offset & 0xff) << 24) | ((offset & 0xff00) << 8) | ((offset & 0xff0000) >> 8) | ((offset & 0xff000000) >> 24);
	*(unsigned int *)(sc->mmio + 0x100044) = offset_tmp;
	*(unsigned int *)(sc->mmio + 0x100048) = value;
	*(unsigned int *)(sc->mmio + 0x100040) = 0x02000000;
	
	sysHyPhy20gUsDelay(200);
    } else {
        printk(KERN_WARNING "[pcie_write]: unkown mode!\n");
        return HYPHY20G_FAILURE;
    }
    
    return HYPHY20G_SUCCESS;
}

inline INT4 pcie_burst_read(struct file * fileHndl, UINT4 startAddr, UINT4 length, UINT4 * dataList)
{
    UINT4 value, offset, index;
    qnf_softc_t *sc;
    sc = (qnf_softc_t *)fileHndl->private_data;

    for (index = 0; index < length; index++) {
        offset = startAddr + index*0x4;
	pcie_read(fileHndl, 0, offset, &value);
        //*dataList = ((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24);
	*dataList = value;
	dataList++;
    }

    return HYPHY20G_SUCCESS;
}

inline INT4 pcie_burst_write(struct file * fileHndl, UINT4 startAddr, UINT4 length, UINT4 * dataList)
{
    UINT4 value, offset, index;
    qnf_softc_t *sc;
    sc = (qnf_softc_t *)fileHndl->private_data;
    
    for (index = 0; index < length; index++) {
        value = *dataList;
        //*(UINT4 *)(sc->mmio + startAddr + index*0x4) = ((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24);
	offset = startAddr + index*0x4;
	pcie_write(fileHndl, 0, offset, value);
	dataList++;
    }
    
    return HYPHY20G_SUCCESS;
}

inline INT4 gen_read(struct file *fileHndl, UINT4 offset, UINT4 *pvalue)
{
    pcie_read(fileHndl, 0, offset, pvalue);
    
    return HYPHY20G_SUCCESS;
}

inline INT4 gen_write(struct file *fileHndl, UINT4 offset, UINT4 value)
{
    pcie_write(fileHndl, 0, offset, value);
    
    return HYPHY20G_SUCCESS;
}


/* end of file */
