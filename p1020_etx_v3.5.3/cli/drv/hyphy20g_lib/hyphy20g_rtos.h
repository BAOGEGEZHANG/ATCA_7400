/******************************************************************************/
/**  COPYRIGHT (C) 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
/**--------------------------------------------------------------------------**/
/** This software embodies materials and concepts which are proprietary and  **/
/** confidential to PMC-Sierra, Inc.                                         **/
/** PMC-Sierra distributes this software to its customers pursuant to the    **/
/** terms and conditions of the Device Driver Software License Agreement     **/
/** contained in the text file software.lic that is distributed along with   **/
/** the device driver software. This software can only be utilized if all    **/
/** terms and conditions of the Device Driver Software License Agreement are **/
/** accepted. If there are any questions, concerns, or if the Device Driver  **/
/** Software License Agreement text file, software.lic, is missing please    **/
/** contact PMC-Sierra for assistance.                                       **/
/**--------------------------------------------------------------------------**/
/**                                                                          **/
/******************************************************************************/
/*******************************************************************************
**  MODULE      : HyPHY 20G SW Reference Design Device Driver
**
**  FILE        : hyphy20g_rtos.h
**
**  $Date: 2011-04-18 17:52:15 +0800 (Mon, 18 Apr 2011) $
**
**  $Revision: 11722 $
**
**  DESCRIPTION : Contains RTOS specific definitions
**
**  NOTES       : User should modify this file's contents based on their
**                RTOS characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_RTOS_H
#define _HYPHY20G_RTOS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PMC_HW_CODE
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/interrupt.h>
//#include <linux/tqueue.h>
#include <linux/poll.h>
#include <linux/sem.h>
#else
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#endif

#include "hyphy20g_typs.h"

/* Constants */

//#define DEBUG_ENABLE

/* Macro Definitions */
#ifdef DEBUG_ENABLE
/* Restriction: this macro should not be used inside a void function */
/* Customer must edit HYPHY20G_ASSERT, DBG_PRINT and TRACEF macro definitions
   for their respective environments/systems */
#define HYPHY20G_ASSERT(expr, rtnValue, format, args...)                \
do {                                                                    \
    if (!(expr)) {                                                      \
        printk("ASSERT: - file %s, line %d, - ", __FILE__, __LINE__);   \
        printk(format, ##args);                                         \
        return rtnValue;                                                \
    }                                                                   \
} while (0)
#define DBG_PRINT(format, args...)   printk(format, ## args)

#else /* DEBUG_ENABLE */
#define HYPHY20G_ASSERT(expr, rtnValue, format, args...)                \
do {                                                                    \
    if (!(expr)) {                                                      \
        printk("ASSERT: - file %s, line %d, - ", __FILE__, __LINE__);   \
        printk(format, ##args);                                         \
        return rtnValue;                                                \
    }                                                                   \
} while (0)
#define DBG_PRINT(format, args...)
#endif /* DEBUG_ENABLE */

#define TRACEF(format, args...)      printk(format, ## args)

/*
** Memory Management
*/
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
void *sysHyPhy20gMemAlloc(UINT4 size);

/*******************************************************************************
**
**  sysHyPhy20gMemFree
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Frees memory allocated using sysHyPhy20gMemAlloc.  The
**                  argument of sysHyPhy20gMemFree must be a pointer returned by 
**                  sysHyPhy20gMemAlloc. Do not free memory not originally 
**                  allocated by sysHyPhy20gMemAlloc. No action will be taken 
**                  when sysHyPhy20gMemFree is called with a NULL pointer.
**
**                  Customer must modify for their specific systems and 
**                  maintain behavior described here.
**                  
**  INPUTS:         pfirstByte  - (pointer to) first byte of the memory region
**                                being de-allocated
**
**  OUTPUTS:        None
**
**  RETURNS:        None
**
*******************************************************************************/
#define sysHyPhy20gMemFree(pfirstByte)      \
do {                                        \
    kfree((void*)(pfirstByte));             \
    pfirstByte = NULL;                      \
} while (0)

/*******************************************************************************
**
**  sysHyPhy20gMemSet
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes a block of memory with a given value
**
**                  Customer must modify for their specific systems.
**
**  INPUTS:         pmem        - (pointer to) first byte of the memory block
**                                to initialize
**                  val         - value to use
**                  sz          - size of the block to initialize
**
**  OUTPUTS:        pmem        - updated memory block
**
**  RETURNS:        (pointer to) first byte of memory block
**
*******************************************************************************/
#define sysHyPhy20gMemSet(pmem, val, sz)             \
            memset((void *)(pmem), (val), (sz))


/*******************************************************************************
**
**  sysHyPhy20gMemCpy
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Copy a block of memory.
**
**                  Customer must modify for their specific systems.
**
**  INPUTS:         pdst        - (pointer to) first byte of destination block
**                  psrc        - (pointer to) first byte of source block
**                  sz          - block size
**
**  OUTPUTS:        pdst        - updated memory block
**
**  RETURNS:        (pointer to) first byte of destination block
**
*******************************************************************************/
#define sysHyPhy20gMemCpy(pdst, psrc, sz)            \
            memcpy((void *)(pdst), (void *)(psrc), (sz))


/*******************************************************************************
**
**  sysHyPhy20gEndianness
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Determine endianness at run time.
**
**  INPUTS:         
**
**  OUTPUTS:        
**
**  RETURNS:        LITTLE_ENDIAN
**                  BIG_ENDIAN
**
*******************************************************************************/
INT4 sysHyPhy20gEndianness(void);

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN    1

#ifdef HYPHY20G_API_REENTRANT

/*******************************************************************************
**
**  sysHyPhy20gAtomicCreate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Create semaphore for atomic API operation
**
**  INPUTS:         pSem        - semaphore pointer
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 sysHyPhy20gAtomicCreate(UINT4 *pSem);

/*******************************************************************************
**
**  sysHyPhy20gAtomicDelete
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Delete semaphore for atomic API operation
**
**  INPUTS:         sem       - semaphore to delete
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sysHyPhy20gAtomicDelete(UINT4 sem);

/*******************************************************************************
**
**  sysHyPhy20gAtomicStart
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Down semaphore for atomic API operation.
**                  To port this function to other OS, you need replace 
**                  down_interruptible properly. As a example, you may use 
**                  semTake to take a semaphore in vxWorks.
**
**  INPUTS:         sem    - semaphore
**
**  OUTPUTS:        None
**
**  RETURNS:        None
**
*******************************************************************************/
#define sysHyPhy20gAtomicStart(sem) \
    do{down_interruptible((struct semaphore *)sem);}while(0)

/*******************************************************************************
**
**  sysHyPhy20gAtomicEnd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Up semaphore for atomic API operation
**                  To port this function to other OS, you need replace 
**                  up properly. As a example, you may use semGive to give a 
**                  semaphore in vxWorks.
**
**  INPUTS:         sem    - semaphore
**
**  OUTPUTS:        None
**
**  RETURNS:        None
**
*******************************************************************************/
#define sysHyPhy20gAtomicEnd(sem) \
    do{up((struct semaphore *)sem);}while(0)

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
void sysHyPhy20gGetTimeOfDay(TIMEVAL *pTime);

#endif


#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_RTOS_H */
/* end of file */
