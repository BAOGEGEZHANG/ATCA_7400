/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_drvr.c
 * "Driver" related code is implemented here.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef CS_DONT_USE_STDLIB
#       include <stdio.h>
#       include <string.h>
#endif /* CS_DONT_USE_STDLIB */

#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_cb.h"
#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_gen_data.h"

#include "arn5_gen_api.h"


arn5_drvr_cb_t      * g_pARN5  = NULL ; /* global */
static cs_boolean   esc_code_print    = FALSE ;
static cs_uint8     * cs_drvr_ver = "Released on Wed Nov 23 22:30:04 2005" ;


/****************************************************************/
/* $rtn_hdr_start  LOAD DEVICE DRIVER                           */
/* CATEGORY   : Driver                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_drvr_load(void)
/* INPUTS     : ----                                            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This loads the driver, initializes the driver data-structures*/
/* and allocates resource for the device control block table    */
/* and initializes it.                                          */
/*                                                              */
/* This API should be called BEFORE invoking any of the other   */
/* API's.                                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( CS_VERIFY_ENDIANESS() != CS_OK ) {
    CS_HNDL_ERROR( 0, EARN5_DRVR_INCORRECT_ENDIANESS, NULL ) ;
    return (CS_ERROR) ;
  }
  if ( ARN5_DRVR_IS_LOADED() ) {
    CS_HNDL_ERROR( 0, EARN5_DRVR_ALRDY_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  g_pARN5 = (arn5_drvr_cb_t *) CS_MALLOC( sizeof(arn5_drvr_cb_t) ) ;
  if (g_pARN5 == NULL) {
    CS_PRINT("ERROR: Fail to allocate memory when loading driver\n") ;
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *) g_pARN5, 0, sizeof(arn5_drvr_cb_t) ) ;
  g_pARN5->drvr_loaded      = FALSE ;
  g_pARN5->max_num_devices  = ARN5_MAX_NUM_DEVICES ;
  g_pARN5->drvr_flag        = ARN5_DRVR_DEFAULT_FLAG ;

#ifndef CS_DONT_USE_STDLIB
  g_pARN5->drvr_out_fp      = stdout ;
  strcpy(g_pARN5->drvr_out_filename, " ") ;
#endif

  g_pARN5->drvr_loaded = TRUE ;
  //CS_PRINT("\t**** ARN5(Arsenal5) Driver loaded ****\n") ;
  //arn5_print_drvr_ver()  ;


#ifndef RELEASE_PLATFORM
  cs_lab_load_driver(CS_CHIP_ARSENAL5) ;
#endif

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  UNLOAD DEVICE DRIVER                         */
/* CATEGORY   : Driver                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_drvr_unload(void)
/* INPUTS     : ----                                            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION: i                                               */
/* This unloads the driver and releases any related system      */
/* resources held by the driver.                                */
/*                                                              */
/* The API's in the driver should not called after this, until  */
/* after arn5_load_driver() is loaded again.                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32     ii ;
  arn5_dev_cb_t  ** dev_cb_tbl ;
  arn5_dev_cb_t  * pdevcb ;

  if ( !(ARN5_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EARN5_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  dev_cb_tbl =  ARN5_GET_DEVCB_TBL() ;
  for (ii = 0; ii < g_pARN5->max_num_devices; ii++) {
    pdevcb = dev_cb_tbl[ii] ;

    if (pdevcb) {
      if ( arn5_dev_unregister(pdevcb->dev_id) != CS_OK ) {
        return (CS_ERROR) ;
      }
    }
  }

  //CS_PRINT("\t**** ARN5(Arsenal5) Driver unloaded ****\n") ;
  CS_FREE(g_pARN5) ;
  g_pARN5 = NULL ;

#ifndef RELEASE_PLATFORM
  cs_lab_unload_driver(CS_CHIP_ARSENAL5) ;
#endif

  return(CS_OK) ;
}

#ifndef CS_DONT_USE_STDLIB
/****************************************************************/
/* $rtn_hdr_start   START DRIVER OUTPUT TO FILE                 */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_start_output_to_file(char *fname)
/* INPUTS     : Filename for driver output                      */
/* OUTPUTS    : Driver print messages to the specified file     */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* All program output going to stdout will also be written to   */
/* specified file.                                              */
/*                                                              */
/* The output to the specified file can be stopped by calling   */
/* arn5_stop_driver_output_to_file().                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(ARN5_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EARN5_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if (g_pARN5->drvr_out_fp != stdout) {
    CS_HNDL_ERROR(0, EARN5_ALRDY_OUT_FILE_OPENED,":%s\n",
                  g_pARN5->drvr_out_filename) ;
    return (CS_ERROR) ;
  }

  if (fname == NULL) {
    CS_HNDL_ERROR(0, EARN5_OUT_FILE_OPEN_ERROR, ": fname = ???\n",
                  fname) ;
    return (CS_ERROR) ;
  }

  g_pARN5->drvr_out_fp = fopen(fname, "w") ;
  if (g_pARN5->drvr_out_fp == NULL) {
    g_pARN5->drvr_out_fp = stdout ;
    CS_HNDL_ERROR(0, EARN5_OUT_FILE_OPEN_ERROR, "%s\n",
                  fname) ;
    return (CS_ERROR) ;
  }

  strcpy(g_pARN5->drvr_out_filename, fname) ;

  //CS_PRINT("Start driver output to %s ....\n",
  //         g_pARN5->drvr_out_filename) ;
  return (CS_OK) ;
}
#endif /* CS_DONT_USE_STDLIB */


#ifndef CS_DONT_USE_STDLIB
/****************************************************************/
/* $rtn_hdr_start   STOP DRIVER OUTPUT TO FILE                  */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_stop_driver_output_to_file(void)
/* INPUTS     :                                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Stop the writing to the file of the program output and reset */
/* it to stdout.                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(ARN5_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EARN5_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if (g_pARN5->drvr_out_fp == stdout) {
    CS_HNDL_ERROR(0, EARN5_NO_OUT_FILE_OPENED, NULL) ;
    return (CS_ERROR) ;
  }

  //CS_PRINT(".... Driver output to %s stopped!\n",
  //         g_pARN5->drvr_out_filename) ;

  fclose(g_pARN5->drvr_out_fp) ;
  g_pARN5->drvr_out_fp = stdout ;
  strcpy(g_pARN5->drvr_out_filename, " ") ;

  return(CS_OK) ;
}
#endif /* #ifndef CS_DONT_USE_STDLIB */


/****************************************************************/
/* $rtn_hdr_start   STATISTICS HEX FORMAT DISPLAY CONTROL       */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void  arn5_stats_hex_format_ctl(cs_ctl_t hex_ctl)
/* INPUTS     : o CS_DISABLE(0) or CS_ENABLEC(1)                */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Enables/disables debug printing of statistics counters in    */
/* hexa-decimal format for display.                             */
/* By default, DECIMAL format is used.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( ARN5_DRVR_IS_LOADED() ) {
    if (hex_ctl == CS_DISABLE) {
      g_pARN5->drvr_flag &= ((~ARN5_DRVR_STATS_IN_HEX) & 0xFFFF);
    }
    else {
      g_pARN5->drvr_flag |= ARN5_DRVR_STATS_IN_HEX ;
    }
  }
}


/****************************************************************/
/* $rtn_hdr_start   STATISTICS ZERO COUNTER DISPLAY CONTROL     */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void  arn5_stats_mask_zero_cntr_ctl(cs_ctl_t mask_zero_ctl)
/* INPUTS     : o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Enables/disables if debug printing the statistics counters   */
/* with zero values.                                            */
/* By default, all ZERO counters are printed out.               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( ARN5_DRVR_IS_LOADED() ) {
    if (mask_zero_ctl == CS_DISABLE) {
      g_pARN5->drvr_flag &= ((~ARN5_DRVR_STATS_MASK_ZERO) & 0xFFFF);
    }
    else {
      g_pARN5->drvr_flag |= ARN5_DRVR_STATS_MASK_ZERO ;
    }
  }
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE VT100 ESCAPE CODE PRINTING   */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void  arn5_esc_code_print_ctl(cs_ctl_t ctl)
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
  if ( ARN5_DRVR_IS_LOADED() ) {
    esc_code_print = (ctl == CS_ENABLE) ? TRUE : FALSE ;
  }
}


/****************************************************************/
/* $rtn_hdr_start   PRINT DRIVER VERSION INFORMATION            */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void  arn5_print_drvr_ver()
/* INPUTS     : ---                                             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Prints the driver release version information.               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  //CS_PRINT("\tARN5 Driver - %s.\n", cs_drvr_ver) ;
}


/****************************************************************/
/* $rtn_hdr_start   PRINT DRIVER SUMMARY INFORMATION            */
/* CATEGORY   : Driver                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void  arn5_print_drvr_summary()
/* INPUTS     : ---                                             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : none                                            */
/* DESCRIPTION:                                                 */
/* Prints the driver summary information. Indicates which       */
/* device(s) and port(s) are valid, active, etc..               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16     num_devices = 0, num_ports = 0 ;
  arn5_dev_cb_t ** dev_cb_tbl ;
  cs_uint16     ii, jj ;


  if ( !(ARN5_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( 0, EARN5_DRVR_NOT_LOADED, NULL ) ;
    return ;          ;
  }

  dev_cb_tbl =  ARN5_GET_DEVCB_TBL() ;
  //CS_PRINT("\n\t ARN5  DRIVER  SUMMARY\n") ;
  for (ii = 0; ii < ARN5_MAX_NUM_DEVICES; ii++) {
    arn5_dev_cb_t  * pdevcb = dev_cb_tbl[ii] ;

    if (pdevcb) {
      num_devices++ ;
      CS_PRINT("**********************************************************\n") ;
      CS_PRINT("Device-Id %d (@ 0x%08x) is valid and its state is %s\n",
               pdevcb->dev_id, pdevcb->base_addr,
               (pdevcb->state == ARN5_DEV_STATE_REGISTERED) ? "REGISTERED"
               : "INITIALIZED") ;

      if (pdevcb->state == ARN5_DEV_STATE_INITIALIZED) {
        for (jj = 0; jj < ARN5_MAX_NUM_PORTS; jj++) {
          arn5_port_cb_t * ppcb = pdevcb->ppcb[jj] ;

          if (ppcb) {
            num_ports++ ;
            CS_PRINT(
              "--------------------------------------------------------\n"
              ) ;
            CS_PRINT("\tPort-Id 0x%04x is valid and its state is %s\n",
                     ppcb->port_id,
                     (ppcb->state == ARN5_PORT_STATE_OPENED) ?
                     "OPENED" : "INITIALIZED") ;
            CS_PRINT("\tLine Rate    = %s,   L2 Protocol   = %s\n",
                     ARN5_GET_LINE_RATE_NAME(ppcb->summ.line_rate),
                     ARN5_GET_L2_PROT_NAME(ppcb->summ.l2_prot)) ;
          }
        }
        CS_PRINT(
                 "--------------------------------------------------------\n"
                 ) ;
        CS_PRINT("Number of valid ports in dev-%d = %d\n\n", ii, num_ports) ;
      }
    }
  }

  CS_PRINT("**********************************************************\n") ;
  CS_PRINT("Number of valid devices in ARN5 driver = %d\n", num_devices) ;
  CS_PRINT("\nFor more information, use:\n") ;
  CS_PRINT("\tarn5_dev_dump_running_cfg(), \n") ;
  CS_PRINT("\tarn5_port_dump_running_cfg() \n\n") ;
}

