/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_drvr.c
 * "Driver" related code is implemented here.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef CS_DONT_USE_STDLIB
#	include <stdio.h>
#	include <string.h>
#endif /* CS_DONT_USE_STDLIB */

#include "cs_types.h"
#include "cs_rtos.h"
#include "m2r_error.h"

#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_cb.h"

extern  cs_status m2r_dev_unregister(cs_uint16 dev_id) ;

static
m2r_drvr_cb_t       m2r_drvr_cb ;
m2r_drvr_cb_t	    * pM2R = &m2r_drvr_cb ;  /* global */
static cs_boolean   esc_code_print  = FALSE ;
static char         * cs_drvr_ver = "Released on Mon May  1 00:51:10 2006" ;


/****************************************************************/
/* $rtn_hdr_start  LOAD DEVICE DRIVER				*/
/* CATEGORY   : Driver				    		*/
/* ACCESS     : public				       		*/
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
cs_status m2r_load_driver(void)
/* INPUTS     : ----					 	*/
/* OUTPUTS    : ----					 	*/
/* RETURNS    : CS_OK or CS_ERROR			 	*/
/* DESCRIPTION:						 	*/
/* This loads the driver, initializes the driver data-structures*/
/* and allocates resource for the device control block table 	*/
/* and initializes it.                                      	*/
/*              					 	*/
/* This API should be called BEFORE invoking any of the other	*/
/* API's.         					 	*/
/* $rtn_hdr_end						 	*/
/****************************************************************/
{
  cs_uint32	    i, j ;

  if ( CS_VERIFY_ENDIANESS() != CS_OK ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_INCORRECT_ENDIANESS, NULL ) ;
    return (CS_ERROR) ;
  }
  if ( M2R_DRVR_IS_LOADED() ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_ALRDY_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *) pM2R, 0, sizeof(m2r_drvr_cb_t) ) ;
  pM2R->drvr_loaded      = FALSE ;
  pM2R->max_num_devices  = CS_MAX_NUM_DEVICES ;
  /*
  pM2R->drvr_flag	 = M2R_DRVR_DEFAULT_FLAG ;
  */
  pM2R->drvr_flag        = M2R_DRVR_STATS_IN_HEX;

#ifndef CS_DONT_USE_STDLIB
  pM2R->drvr_out_fp      = stdout ;
  strcpy(pM2R->drvr_out_filename, " ") ;
#endif

  for (i = 0; i < CS_MAX_NUM_DEVICES; i++) {
    m2r_dev_cb_t	* pdevcb = &pM2R->dev_tbl[i] ;

    pdevcb->state       = M2R_DEV_STATE_INVALID ;
    pdevcb->base_addr  	= 0 ;
    pdevcb->debug_flag	= (cs_uint32) -1 ;
    for (j = 0; j < M2R_NUM_PORTS_PER_ASIC; j++) {
      pdevcb->ppcb[j] = NULL ;
    }
  }

  pM2R->warm_start = FALSE ;
  pM2R->drvr_loaded = TRUE ;
  //CS_PRINT("\tM2R Driver loaded\n") ;
  m2r_print_drvr_ver() ;

#ifndef RELEASE_PLATFORM
  cs_lab_load_driver(CS_CHIP_MILAN2) ;
#endif

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  UNLOAD DEVICE DRIVER				*/
/* CATEGORY   : Driver				    		*/
/* ACCESS     : public				       		*/
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
cs_status m2r_unload_driver(void)
/* INPUTS     : ----					 	*/
/* OUTPUTS    : ----					 	*/
/* RETURNS    : CS_OK or CS_ERROR			 	*/
/* DESCRIPTION:	i						*/
/* This unloads the driver and releases any related system      */
/* resources held by the driver.		 		*/
/*               					 	*/
/* The API's in the driver should not called after this, until 	*/
/* after m2r_load_driver() is loaded again.		 	*/
/* $rtn_hdr_end						 	*/
/****************************************************************/
{
  cs_uint32	i ;

  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  for (i = 0; i < pM2R->max_num_devices; i++) {
    if (m2r_dev_unregister(i) == CS_ERROR) {
      return (CS_ERROR) ;
    }
  }

  CS_PRINT("\tM2R Driver unloaded\n") ;
  CS_MEMSET( (void *) pM2R, 0, sizeof(m2r_drvr_cb_t) ) ;
  pM2R->drvr_loaded = FALSE ;

#ifndef RELEASE_PLATFORM
  cs_lab_unload_driver(CS_CHIP_MILAN2) ;
#endif

  return(CS_OK) ;
}

#ifndef CS_DONT_USE_STDLIB
/****************************************************************/
/* $rtn_hdr_start   START DRIVER OUTPUT TO FILE			*/
/* CATEGORY   : Driver						*/
/* ACCESS     : Public						*/
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
cs_status  m2r_start_output_to_file(char *fname)
/* INPUTS     : Filename for driver output                      */
/* OUTPUTS    : Driver print messages to the specified file     */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:	                                                */
/* All program output going to stdout will also be written to   */
/* specified file.                                              */
/*                                                              */
/* The output to the specified file can be stopped by calling   */
/* m2r_stop_driver_output_to_file().                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if (pM2R->drvr_out_fp != stdout) {
    CS_HNDL_ERROR(0, EM2R_ALRDY_OUT_FILE_OPENED,":%s\n",
		  pM2R->drvr_out_filename) ;
    return (CS_ERROR) ;
  }

  if (fname == NULL) {
    CS_HNDL_ERROR(0, EM2R_OUT_FILE_OPEN_ERROR, ": fname = ???\n",
		  fname) ;
    return (CS_ERROR) ;
  }

  pM2R->drvr_out_fp = fopen(fname, "w") ;
  if (pM2R->drvr_out_fp == NULL) {
    pM2R->drvr_out_fp = stdout ;
    CS_HNDL_ERROR(0, EM2R_OUT_FILE_OPEN_ERROR, "%s\n",
		  fname) ;
    return (CS_ERROR) ;
  }

  strcpy(pM2R->drvr_out_filename, fname) ;

  CS_PRINT("Start driver output to %s ....\n",
	   pM2R->drvr_out_filename) ;
  return (CS_OK) ;
}
#endif /* CS_DONT_USE_STDLIB */


#ifndef CS_DONT_USE_STDLIB
/****************************************************************/
/* $rtn_hdr_start   STOP DRIVER OUTPUT TO FILE                  */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
cs_status  m2r_stop_driver_output_to_file(void)
/* INPUTS     :                                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:	                                                */
/* Stop the writing to the file of the program output and reset */
/* it to stdout.                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if (pM2R->drvr_out_fp == stdout) {
    CS_HNDL_ERROR(0, EM2R_NO_OUT_FILE_OPENED, NULL) ;
    return (CS_ERROR) ;
  }

  CS_PRINT(".... Driver output to %s stopped!\n",
	   pM2R->drvr_out_filename) ;

  fclose(pM2R->drvr_out_fp) ;
  pM2R->drvr_out_fp = stdout ;
  strcpy(pM2R->drvr_out_filename, " ") ;

  return(CS_OK) ;
}
#endif /* #ifndef CS_DONT_USE_STDLIB */


/****************************************************************/
/* $rtn_hdr_start   STATISTICS HEX FORMAT DISPLAY CONTROL      	*/
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
void  m2r_stats_hex_format_ctl(m2r_ctl_t hex_ctl)
/* INPUTS     : o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Enables/disables debug printing of statistics counters in    */
/* hexa-decimal format for display.                             */
/* By default, DECIMAL format is used.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( M2R_DRVR_IS_LOADED() ) {
    if (hex_ctl == M2R_DISABLE) {
      pM2R->drvr_flag &= ((~M2R_DRVR_STATS_IN_HEX) & 0xFFFF);
    }
    else {
      pM2R->drvr_flag |= M2R_DRVR_STATS_IN_HEX ;
    }
  }
}


/****************************************************************/
/* $rtn_hdr_start   STATISTICS ZERO COUNTER DISPLAY CONTROL   	*/
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
void  m2r_stats_mask_zero_cntr_ctl(m2r_ctl_t mask_zero_ctl)
/* INPUTS     : o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Enables/disables if debug printing the statistics counters   */
/* with zero values.                                            */
/* By default, all ZERO counters are printed out.               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( M2R_DRVR_IS_LOADED() ) {
    if (mask_zero_ctl == M2R_DISABLE) {
      pM2R->drvr_flag &= ((~M2R_DRVR_STATS_MASK_ZERO) & 0xFFFF);
    }
    else {
      pM2R->drvr_flag |= M2R_DRVR_STATS_MASK_ZERO ;
    }
  }
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE VT100 ESCAPE CODE PRINTING  	*/
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
void  m2r_esc_code_print_ctl(cs_ctl_t ctl)
/* INPUTS     : o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Enables/disables the driver print formatting using VT100     */
/* terminal emulator escape codes.                              */
/* By default, the escape code printing is enabled.             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( M2R_DRVR_IS_LOADED() ) {
    esc_code_print = (ctl == CS_ENABLE) ? TRUE : FALSE ;
  }
}


/****************************************************************/
/* $rtn_hdr_start   PRINT DRIVER VERSION INFORMATION            */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : both                                            */
void  m2r_print_drvr_ver()
/* INPUTS     : ---                                             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Prints the driver release version information.               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  //CS_PRINT("\tM2R Driver - %s.\n", cs_drvr_ver) ;
}


/****************************************************************/
/* $rtn_hdr_start   PRINT DRIVER SUMMARY INFORMATION            */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Milan2                                          */
void  m2r_print_drvr_summary()
/* INPUTS     : ---                                             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Prints the driver summary information. Indicates which       */
/* device(s) and port(s) are valid, active, etc..               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16     num_devices = 0 ;
  m2r_dev_cb_t * dev_cb_tbl ;
  cs_uint16     ii ;

  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return ;
  }

  dev_cb_tbl = m2r_get_dev_tbl() ;

  CS_PRINT("\n\t M2R  DRIVER  SUMMARY\n") ;
  for (ii = 0; ii < CS_MAX_NUM_DEVICES; ii++) {
    m2r_dev_cb_t  * pdevcb = &dev_cb_tbl[ii] ;

    if ( (pdevcb) && (pdevcb->state != M2R_DEV_STATE_INVALID) ) {
      num_devices++ ;
      CS_PRINT(
	 "**********************************************************\n"
	 ) ;
      CS_PRINT("Device-Id %d (@ 0x%08lx) is valid and its state is %s\n",
	       pdevcb->dev_id, pdevcb->base_addr,
	       (pdevcb->state == M2R_DEV_STATE_REGISTERED) ? "REGISTERED"
	       : "INITIALIZED") ;

      if (pdevcb->state == M2R_DEV_STATE_INITIALIZED) {
	const char * state_tbl[] = {
	  CS_DI([M2R_PORT_STATE_POR] 	         =) "POWER-ON RESET",
	  CS_DI([M2R_PORT_STATE_DATA_INIT_DONE]  =) "DATA INIT DONE  ",
	  CS_DI([M2R_PORT_STATE_HW_CFG_DONE]     =) "HW INIT DONE" } ;
	m2r_port_cb_t * ppcb = pdevcb->ppcb[0] ;
	CS_PRINT(
	     "--------------------------------------------------------\n"
	     ) ;

	CS_PRINT("\tPort-Id is 0x%04x and its state is %s\n",
		 M2R_DEV_SLICE_NUM_TO_PORT_ID(pdevcb->dev_id, 0),
		 state_tbl[((ppcb) ? (ppcb->state) : M2R_PORT_STATE_POR) % 4 ]
		 ) ;
      }
    }
  }

  CS_PRINT("**********************************************************\n") ;
  CS_PRINT("Number of valid devices in M2R driver = %d\n", num_devices) ;
  CS_PRINT("\nUse m2r_port_dump_running_cfg() for more info\n") ;
}


/****************************************************************/
/* $rtn_hdr_start   START WARM START OPERATION                  */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Milan2                                          */
cs_status  m2r_start_warm_start(void)
/* INPUTS     :                                                 */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Start warm start operation.                                  */
/* While warm start is active, no destructive hardware access   */
/* is done and traffic is not impacted. The main purpose is for */
/* driver software to rebuild its datastructures.               */
/* After activating warm start, the application software        */
/* should call the initialization sequence normally as it would */
/* for cold start.                                              */
/* It is SW's responsibility to save the driver statistics and  */
/* restore it after warm start. Also, it should mask off the    */
/* the interrupt chip at the CPU.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR);
  }
  
  pM2R->warm_start = TRUE ;
  CS_PRINT("\n\t***********************************************\n") ;
  CS_PRINT("\t\t Started warm-start operation\n") ;
  CS_PRINT("\t***********************************************\n\n") ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   STOP WARM START OPERATION                   */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Milan2                                          */
cs_status  m2r_stop_warm_start(void)
/* INPUTS     :                                                 */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* STOP warm start operation.                                   */
/* While warm start is active, no destructive hardware access   */
/* is done and traffic is not impacted. The main purpose is for */
/* driver software to rebuild its datastructures.               */
/* After activating warm start, the application software should */
/* call the initialization sequence normally as it would for    */
/* cold start.                                                  */
/* It is SW's responsibility to save the driver statistics and  */
/* restore it after warm start. Also, it should mask off the    */
/* the interrupt chip at the CPU.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }
  
  pM2R->warm_start = FALSE ;
  CS_PRINT("\n\t***********************************************\n") ;
  CS_PRINT("\t\t Warm-start operation stopped\n") ;
  CS_PRINT("\t***********************************************\n\n") ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   IS WARM START IN PROGRESS                   */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Milan2                                          */
cs_boolean m2r_is_warm_start_in_progress(void)
/* INPUTS     :                                                 */
/* OUTPUTS    : ---                                             */
/* RETURNS    : TRUE or FALSE                                   */
/* DESCRIPTION:                                                 */
/* Check if Warm Start is active currently.                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
 if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (FALSE);
  }

 return(pM2R->warm_start) ;
}

/*********************************************
 * GET DRIVER CONTROL BLOCK                  *
 *********************************************/
m2r_dev_cb_t * m2r_get_dev_tbl(void)
{
  return (pM2R->dev_tbl) ;
}

/*********************************************
 * CHECK IF THE DRIVER IS LOADED             *
 *********************************************/
cs_boolean m2r_drvr_is_loaded(void)
{
  if ( (pM2R) && (pM2R->drvr_loaded) ) {
    return (TRUE) ;
  }
  return (FALSE) ;
}

/*********************************************
 * GET DRIVER'S OUTPUT FILE POINTER          *
 *********************************************/
#ifndef CS_DONT_USE_STDLIB
FILE * m2r_drvr_get_out_fp(void)
{
   if ( M2R_DRVR_IS_LOADED() ) {
     return(pM2R->drvr_out_fp) ;
   }
   return (stdout) ;
}
#endif /* CS_DONT_USE_STDLIB */


/************************************************
 * CHECK IF STATISTICS ARE TO BE PRINTED IN HEX *
 ************************************************/
cs_boolean m2r_drvr_is_stats_in_hex_fmt(void)
{
   if ( M2R_DRVR_IS_LOADED() ) {
     return( (pM2R->drvr_flag & M2R_DRVR_STATS_IN_HEX) ? TRUE
	     : FALSE) ;
   }
   else {
     return (FALSE) ;
   }
}

/**************************************************
 * CHECK IF ZERO STATISTICS ARE TO BE PRINTED OUT *
 **************************************************/
cs_boolean m2r_drvr_is_mask_zero_cntr(void)
{
   if ( M2R_DRVR_IS_LOADED() ) {
     return( (pM2R->drvr_flag & M2R_DRVR_STATS_MASK_ZERO) ? TRUE
	     : FALSE) ;
   }
   else {
    return (FALSE) ;
   }
}

/**************************************************
 * CHECK IF VT100 ESCAPE CODE PRINTING IS ENABLED *
 **************************************************/
cs_boolean m2r_drvr_is_esc_code_enbld(void)
{
  return(esc_code_print) ;
}










