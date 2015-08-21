/***********************************************************************/
/* This file contains unpublished documentation and software           */ 
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * cs_rtos.h
 *
 * Header file with RTOS specific defines and macros that can be 
 * customized as required by the target environment.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __CS_RTOS_H__
#define __CS_RTOS_H__


extern unsigned int framer_read(int qnffd, unsigned int offset, unsigned short *value);
extern unsigned int framer_write(int qnffd, unsigned int offset, unsigned short value );

/******** NOTE ********** NOTE ********** NOTE ********* NOTE *********
 * NOTE: This header file MUST BE CUSTOMIZED before using the driver!!*
 *                                                                    *
 * This contains all the RTOS(like system calls) and environment      *
 * related macro's and stub utilities which should be modified or     *
 * filled in as suited to the customer environment. It is important   *
 * that this customization or porting of the driver is done BEFORE    *
 * making any attempt to compile or use the driver.                   *
 ******** NOTE ********** NOTE ********** NOTE ********* NOTE *********/

#ifndef CS_DONT_USE_STDLIB
#	include <stdlib.h>	/* for malloc() and free() */
#	include <unistd.h>     /* for usleep              */
#	include <errno.h>
#	include <string.h>     /* for memcpy()            */
#endif /* CS_DONT_USE_STDLIB */

#include "cs_types.h"


#ifndef RELEASE_PLATFORM  /* for Cortina Internal use only! */
#ifndef POSIX_PTHREAD
#	define POSIX_PTHREAD
#endif 

#include "cs_lab_rtos.h" 

#endif 			  /* for Cortina Internal use only! */


/*
 * The following macro determines if we are running in interrupt 
 * context. Customer can change this macro to determine if we are 
 * in interrupt context by reading the CPU register or through the 
 * OS provided APIs.
 */
#ifndef CS_IS_INTR_CONTEXT
#	define CS_IS_INTR_CONTEXT()            0  
#endif /* CS_IS_INTR_CONTEXT */


/*
 * CS_IRQ_USER_HANDLER macro maps the user provided handler to be called.
 * Customer can redefine this macro to call their ISR handler.
 */
#ifndef CS_IRQ_USER_HANDLER
#	define CS_IRQ_USER_HANDLER(DPID, IRQ_INFO)		\
	{ /* add code to call user handler */ }
#endif /* CS_IRQ_USER_HANDLER */


/**********************************************************
 *         Register Access Methods                        *
 **********************************************************/
#ifndef __CS_REG_ACCESS_DEFINED__
#define __CS_REG_ACCESS_DEFINED__
/********   Read  Register  *********/
CS_INLINE  cs_uint16 CS_REG_READ(volatile cs_uint16 * pReg)
{
  /*
   * Add platform dependent code for reading from register
   */
  int fd;
  unsigned int offset;
  cs_uint16 uData;

  //printf("pReg is: 0x%x\n", pReg);

  fd = ((unsigned int)pReg)/0x100000;
  offset = ((unsigned int)pReg & 0xfffff);

  //printf("fd is: %d, offset is: 0x%x\n", fd, offset);

  framer_read(fd, offset, &uData);
  usleep(500); 
 
  return uData;
  
  
/*  return(0) ;*/
#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_REG_READ(pReg) ;
  } 
#endif /* CS_DONT_USE_INLINE */
}

/********   Write  Register  *********/
CS_INLINE  void  CS_REG_WRITE(volatile cs_uint16 * pReg, cs_uint16 data)
{
  /*
   * Add platform dependent code to writing to register
   */
  int fd;
  unsigned int offset;
  
  fd = ((unsigned int)pReg)/0x100000;
  offset = ((unsigned int)pReg & 0xfffff);
  
  framer_write(fd, offset, data);
  usleep(500);  
  
#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_REG_WRITE(pReg, data) ;
  }	 
#endif /* CS_DONT_USE_INLINE */
}

#endif /* __CS_REG_ACCESS_DEFINED__ */


/**********************************************************
 *         Timer delay utilities                          *
 **********************************************************/
#ifndef __CS_DELAY_UTILS_DEFINED__
#define __CS_DELAY_UTILS_DEFINED__
#       define CS_UDELAY(usecs) ( usleep(usecs) )
#       define CS_MDELAY(msecs) ( usleep(msecs * 1000) )
#endif /* __CS_DELAY_UTILS_DEFINED__ */


/**********************************************************
 *         Memory Handling                                *
 **********************************************************/
#ifndef __CS_MEM_UTILS_DEFINED__
#define __CS_MEM_UTILS_DEFINED__
#ifndef CS_DONT_USE_STDLIB
#	define  CS_MALLOC(x)		    malloc(x)
#	define  CS_FREE(x)		    free(x)
#	define  CS_MEMCPY(DST, SRC, LEN)    memcpy((DST), (SRC), (LEN))
#	define  CS_MEMSET(DST, VALUE, LEN)  memset((DST), (VALUE), (LEN))
#else
/* user defined memory handling utilities should be defined here  */
#	define  CS_MALLOC(x)		    malloc(x)		   
#	define  CS_FREE(x)		    free(x)
#	define  CS_MEMCPY(DST, SRC, LEN)    memcpy((DST), (SRC), (LEN))
#    define  CS_MEMSET(DST, VALUE, LEN)     memset((DST), (VALUE), (LEN))
#endif /* CS_DONT_USE_STDLIB */
#endif /* __CS_MEM_UTILS_DEFINED__ */


/**********************************************************
 *         Error Reporting                                *
 **********************************************************/
#ifndef CS_SET_ERRNO
#ifndef CS_DONT_USE_STDLIB
#	define  CS_SET_ERRNO(x)	errno = x 
#endif /* CS_DONT_USE_STDLIB */
#endif /* CS_SET_ERRNO */


/**********************************************************
 *                  Semaphores                            *
 *                                                        *
 * Change CS_SEM typedef appropriately and add code to    * 
 * CS_SEM_* inline functions to implement RTOS-dependent  *
 * semaphore handling mechanism.                          *
 *                                                        *
 * NOTE: Use mutually exclusive semaphores(and not plain  *
 * binary ones) which allow the same task/process which is*
 * currently the owner of a semaphore take it again(ie.   *
 * recursively)! This is required as some driver API's    *
 * may be a nested call to other API's and could take the *
 * same semaphore more than once.                         *
 **********************************************************/
#ifndef __CS_SEM_UTILS_DEFINED__
#define __CS_SEM_UTILS_DEFINED__

typedef void *	CS_SEM ;

CS_INLINE  cs_status  CS_SEM_INIT(CS_SEM * sem_id) 
{
  /* Add code to create semaphore */
  return(CS_OK) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_SEM_INIT(sem_id) ;
  } 
#endif
}


CS_INLINE  cs_status  CS_SEM_TAKE(CS_SEM * sem_id) 
{
  if (CS_IS_INTR_CONTEXT()) {
    return (CS_ERROR) ;
  }
  /* Add code to take semaphore */
  return(CS_OK) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_SEM_TAKE(sem_id) ;
  } 
#endif
}


CS_INLINE  void  CS_SEM_GIVE(CS_SEM * sem_id)
{
  if (CS_IS_INTR_CONTEXT()) {
    return ;
  }
  /* Add code to post/give semaphore */
  return ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_SEM_GIVE(sem_id) ;
  } 
#endif
}


CS_INLINE  cs_status  CS_SEM_DESTROY(CS_SEM * sem_id)
{
  /* Delete semaphore */
  return(CS_OK) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_SEM_DESTROY(sem_id) ;
  } 
#endif
} 

#endif /* __CS_SEM_UTILS_DEFINED__ */



/**********************************************************
 *         Interrupt locking                              *
 **********************************************************/
#ifndef __CS_IRQ_LOCKING_DEFINED__
#define __CS_IRQ_LOCKING_DEFINED__

CS_INLINE  cs_uint32  CS_INTERRUPT_LOCK()
{
  /* Add code to lock out maskable interrupts */
  return (0) ; /* return key */

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_INTERRUPT_LOCK() ;
  } 
#endif
}


CS_INLINE  void CS_INTERRUPT_UNLOCK(cs_uint32 key)
{
  /* Add code to unlock maskable interrupts */

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_INTERRUPT_UNLOCK(key) ;
  } 
#endif
}

#endif /* __CS_IRQ_LOCKING_DEFINED__ */



#endif /* __CS_RTOS_H__ */










