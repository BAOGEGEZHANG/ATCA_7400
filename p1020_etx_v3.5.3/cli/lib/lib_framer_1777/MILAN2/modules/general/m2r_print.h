/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_print.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PRIVATE header file - to be included only by the driver
 * code!
 * This contains macro's for printing driver debug and error 
 * messages.
 */

#ifndef __M2R_PRINT_H__
#define __M2R_PRINT_H__

#ifndef CS_DONT_USE_STDLIB
#	include <stdio.h>
#	include <stdarg.h> 
#endif /* CS_DONT_USE_STDLIB */

#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"

/*
 * For now, this header file is included in m2r_common.h.        *
 * Ideally, each driver source file should independently include *
 * this.
 */

#ifndef CS_DONT_USE_STDLIB 
extern  FILE * m2r_drvr_get_out_fp() ;
#endif 

extern  int cs_trans_cb_client_dump( char * );

#define PROMPT1(str)    "     " str
#define PROMPT2(prompt,format)  "     %-40s" format, prompt

/****************************************************************
 * Print/Debug utility                                          *
 * This can be customized as needed by the user.                *
 ****************************************************************/
#ifdef CS_DONT_USE_VAR_ARGS
#	define __CS_DONT_PRINT__
#endif

#ifdef CS_DONT_USE_STDLIB
#	ifndef __CS_DONT_PRINT__
#	  	define __CS_DONT_PRINT__
#	endif
#endif

#ifndef __CS_DONT_PRINT__
CS_INLINE void  CS_PRINT(cs_int8 * fmt, ...)
{
  va_list       VarArg;
  char          log_str[256];
  FILE          * fp ;

  if (CS_IS_INTR_CONTEXT()) {
     return ;
  }

  va_start(VarArg, fmt);
  vsprintf(log_str, fmt, VarArg) ;

#ifndef RELEASE_PLATFORM
  cs_trans_cb_client_dump(log_str);
#endif			

  fprintf(stdout, log_str) ;
  fp = m2r_drvr_get_out_fp() ;
  if (fp != stdout) {
    fprintf(fp, log_str);
    fflush(fp) ;
  }
  va_end(VarArg) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_PRINT(fmt) ;
  }  
#endif
}

#else
/* Add customer defined CS_PRINT macro here                   */
#define CS_PRINT              printk                                    
#endif /* __CS_DONT_PRINT__ */

#ifdef __CS_DONT_PRINT__
#	undef __CS_DONT_PRINT__
#endif

/****************************************************************
 * Error handling and logging utility                           *
 * This can be customized as needed by the user.                *
 ****************************************************************/
extern void m2r_error_handler(cs_uint32 id, cs_uint32 errCode,
                              cs_int8 * fmt, ...);

#define CS_HNDL_ERROR                                            \
        CS_PRINT("M2R ERROR @ %s:%d ", __FILE__, __LINE__) ;  \
        m2r_error_handler


#endif /* __M2R_PRINT_H__ */










