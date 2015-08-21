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
**  MODULE      : HyPHY 20G SW Reference Design Device Driver
**
**  FILE        : hyphy20g_rtos.c
**
**  $Date: 2011-04-18 17:52:15 +0800 (Mon, 18 Apr 2011) $
**
**  $Revision: 11722 $
**
**  DESCRIPTION : Contains the RTOS interface functions
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include    "hyphy20g_rtos.h"

#ifdef HYPHY20G_API_REENTRANT
#include    "hyphy20g.h"
#endif

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/** public functions **/

/*******************************************************************************
**
**  sysHyPhy20gMemAlloc
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates specified number of bytes of memory.
**
**  INPUTS:         numBytes    - number of bytes to be allocated
**
**  OUTPUTS:        None
**
**  RETURNS:        (pointer to) first byte of allocated memory or NULL on
**                  failure
**
*******************************************************************************/
void *sysHyPhy20gMemAlloc(UINT4 size)
{
	void *x;

	x = kmalloc((size_t)size, GFP_KERNEL);
	if (x == NULL) {
        DBG_PRINT("sysHyPhy20gMemAlloc: cannot allocate %d bytes\n", (size_t)size);
	} else {
	    memset(x, 0, (size_t)size);
    }
	return x;
}


/*******************************************************************************
**
**  sysHyPhy20gEndianness
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Determine endianness at run time.
**                  Code from http://www.ibm.com/developerworks/aix/library/
**                  au-endianc/index.html?ca=drs
**
**  INPUTS:         
**
**  OUTPUTS:        
**
**  RETURNS:        LITTLE_ENDIAN
**                  BIG_ENDIAN
**
*******************************************************************************/
INT4 sysHyPhy20gEndianness(void) {
    int i = 1;
    char *p = (char *)&i;

    if (p[0] == 1)
        return LITTLE_ENDIAN;
    else
        return BIG_ENDIAN;
}

#ifdef HYPHY20G_API_REENTRANT

/*******************************************************************************
**
**  sysHyPhy20gAtomicCreate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Create semaphore for atomic API operation.
**                  As a example of how to port this function to other OS , you 
**                  may use semMCreate to create and initialize a 
**                  mutual-exclusion semaphore in vxWorks.
**
**  INPUTS:         pSem        - semaphore pointer
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 sysHyPhy20gAtomicCreate(UINT4 *pSem)
{
    struct semaphore *sem;

    sem = (struct semaphore *)sysHyPhy20gMemAlloc(sizeof(struct semaphore));
    if (NULL == sem) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    sema_init(sem, 1);


    *pSem = (UINT4)sem;

    return HYPHY20G_SUCCESS;
}

/*******************************************************************************
**
**  sysHyPhy20gAtomicDelete
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Delete semaphore for atomic API operation.
**                  As a example of how to port this function to other OS , you 
**                  may use semDelete to delete a semaphore in vxWorks.
**
**  INPUTS:         sem       - semaphore to delete
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gAtomicDelete(UINT4 sem)
{
    sysHyPhy20gMemFree(((void *)sem));
    return HYPHY20G_SUCCESS;
}

#endif

#ifndef PMC_HW_CODE

/*******************************************************************************
**
**  sysHyPhy20gGetTimeOfDay
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Get current day time.
**
**  INPUTS:         TIMEVAL      - time value
**
**  OUTPUTS:        None
**
**  RETURNS:        None
**
*******************************************************************************/
void sysHyPhy20gGetTimeOfDay(TIMEVAL *pTime)
{
    do_gettimeofday(pTime);
}

#endif

/** private functions **/

/* end of file */
