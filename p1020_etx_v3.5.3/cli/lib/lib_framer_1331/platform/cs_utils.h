/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * cs_utils.h
 *
 * Include file containing some basic and common data types and defines
 * used by driver.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
*/
#ifndef __CS_UTILS_H__
#define __CS_UTILS_H__

#include "cs_types.h"
#ifndef RELEASE_PLATFORM
   #include "cs_lab_rtos.h"
#endif

/**************************************************
 * Cortina specific defines - DO NOT MODIFY       *
 **************************************************/
#ifndef RELEASE_PLATFORM
/*
 * These macros are used to temporarily turn off the register name
 * dump in the lab environment.
 */
#define CS_TURN_OFF_REG_NAME_DUMP()      \
        (CS_LAB_TURN_OFF_REG_NAME_DUMP())
#define CS_TURN_ON_REG_NAME_DUMP()       \
        (CS_LAB_TURN_ON_REG_NAME_DUMP())
#define CS_RESTORE_REG_NAME_DUMP()       \
        (CS_LAB_RESTORE_REG_NAME_DUMP())
#define CS_IS_REG_NAME_DUMP_ON()         \
        (CS_LAB_IS_REG_NAME_DUMP_ON())
#else
#define CS_TURN_OFF_REG_NAME_DUMP()
#define CS_TURN_ON_REG_NAME_DUMP() 
#define CS_RESTORE_REG_NAME_DUMP()
#define CS_IS_REG_NAME_DUMP_ON()         FALSE
#endif


/**************************************************
 * Platform/environment defines                   *
 **************************************************/
/*
 * RTL Simulation environment *
 */
CS_INLINE cs_boolean CS_IN_SIM_ENV()
{
#ifndef RELEASE_PLATFORM
  if ( IN_SIM_ENV() ) return (TRUE) ;
#endif
  return (FALSE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_IN_SIM_ENV() ;
  }
#endif
}


/*
 * Test(sw) environment
 */
CS_INLINE cs_boolean CS_IN_TEST_ENV()
{
#ifndef RELEASE_PLATFORM
  if ( IN_TEST_ENV() ) return (TRUE) ;
#endif
  return (FALSE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_IN_TEST_ENV() ;
  }
#endif
}


/*
 * Lab debug(hw) environment
 */
CS_INLINE cs_boolean CS_IN_LAB_ENV()
{
#ifndef RELEASE_PLATFORM
  if ( IN_HW_ENV() ) return (TRUE) ;
#endif
  return (FALSE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_IN_LAB_ENV() ;
  }
#endif
}


/*
 * CUSTOMER environment
 */
CS_INLINE cs_boolean CS_IN_CUSTOMER_ENV()
{
#ifndef RELEASE_PLATFORM
  return (FALSE) ;
#endif
  return (TRUE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_IN_CUSTOMER_ENV() ;
  }
#endif
}


#ifdef CS_BIG_ENDIAN
#ifdef CS_LITTLE_ENDIAN
#error "CS_BIG_ENDIAN and CS_LITTLE_ENDIAN are both defined."
#endif
#else
#ifndef CS_LITTLE_ENDIAN
#error "Either CS_BIG_ENDIAN or CS_LITTLE_ENDIAN must be defined."
#endif
#endif

CS_INLINE cs_status CS_VERIFY_ENDIANESS()
{
  cs_uint16 val = 0x2211 ;
  cs_uint8  low_byte = ((cs_uint8 *)&val)[0] ;

  if (low_byte == 0x22) {
#ifdef CS_BIG_ENDIAN
    return (CS_OK) ;
#else /* CS_LITTLE_ENDIAN */
    return (CS_ERROR) ;
#endif
  }
  else if (low_byte == 0x11) {
#ifdef CS_BIG_ENDIAN
    return (CS_ERROR) ;
#else /* CS_LITTLE_ENDIAN */
    return (CS_OK) ;
#endif
  }
  else {
    return (CS_ERROR) ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_VERIFY_ENDIANESS() ;
  }
#endif
}

#endif /* __CS_UTILS_H__ */

