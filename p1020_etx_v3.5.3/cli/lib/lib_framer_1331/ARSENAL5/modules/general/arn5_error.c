/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_error.c
 *
 * Error table for various errors called out from the driver.
 *
 */
#ifndef CS_DONT_USE_VAR_ARGS
#       include <stdarg.h>
#endif

#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_print.h"
#include "arn5_error.h"

const cs_uint8  * earn5_err_tbl[ EARN5_MAX_ERROR_CODES ] = {
  /*
   * Driver related errors
   */
  [ EARN5_OFST(EARN5_DRVR_ALRDY_LOADED) ] = "Driver is already loaded",
  [ EARN5_OFST(EARN5_DRVR_NOT_LOADED) ]   = "Driver not loaded yet",
  [ EARN5_OFST(EARN5_DRVR_INCORRECT_ENDIANESS) ] =
  "Incorrect endian option defined",
  [ EARN5_OFST(EARN5_ALRDY_OUT_FILE_OPENED) ] =
  "Driver output file already opened",
  [ EARN5_OFST(EARN5_OUT_FILE_OPEN_ERROR) ] =
  "Error opening driver output file",
  [ EARN5_OFST(EARN5_NO_OUT_FILE_OPENED) ] =
  "No driver output file opened",

  /*
   * Device(dev_id) related errors
   */
  [ EARN5_OFST(EARN5_DEV_ID_OUT_OF_RANGE) ] = "dev_id out of range",
  [ EARN5_OFST(EARN5_DEV_NOT_REGD) ]      = "Device not registered yet",
  [ EARN5_OFST(EARN5_DEV_NOT_INITED) ] 	  = "Device not initialized yet",
  [ EARN5_OFST(EARN5_CHIP_JTAG_ID_MISMATCH) ] = "Chip JTAG Id Mismatch",
  [ EARN5_OFST(EARN5_DEV_INVALID_HRESET_SEL) ] =
  "Invalid dev hard reset selection",
  [ EARN5_OFST(EARN5_BIST_NOT_DONE) ]     = "BIST not completed yet",
  [ EARN5_OFST(EARN5_MBIST_BAD_STATUS) ]  = "Error from MBIST",
  [ EARN5_OFST(EARN5_SBIST_BAD_STATUS) ]  = "Error from SBIST",
  [ EARN5_OFST(EARN5_DEV_INVALID_MODE) ]  = "Invalid device Mode",
  [ EARN5_OFST(EARN5_DEV_SEM_INIT_FAILED) ] = "Semaphore init failed",
  [ EARN5_OFST(EARN5_DEV_SEM_TAKE_FAILED) ] = "Semaphore take failed",
  [ EARN5_OFST(EARN5_DEV_CFG_NULL) ]      = "pCfg is NULL",
  [ EARN5_OFST(EARN5_DEV_SUMM_NULL) ]     = "pSumm is NULL",
  [ EARN5_OFST(EARN5_DEV_INVALID_USER_ARG) ] = "Invalid user arg specified",
  [ EARN5_OFST(EARN5_DEV_MALLOC_FAILED) ] = "Cannot allocate memory",
  [ EARN5_OFST(EARN5_DEV_NULL_PTR) ] = "NULL pointer",
  [ EARN5_OFST(EARN5_DEV_TBL_NULL) ] = "Table Pointer is NULL",
  [ EARN5_OFST(EARN5_DEV_TBL_START_INVALID) ] =
  "Table/Memory starting entry num is invalid",
  [ EARN5_OFST(EARN5_DEV_TBL_NUM_INVALID) ] =
  "Table/Memory number of entries is invalid",
  [ EARN5_OFST(EARN5_DEV_RANGE_ERR) ] = "Argument out of Range",
  [ EARN5_OFST(EARN5_DEV_INVALID_SIZE) ] = "Invalid user data size",
  [ EARN5_OFST(EARN5_DEV_CFG_VALIDITY_ERR) ] = "Failed to validate Dev CFG",
  [ EARN5_OFST(EARN5_DEV_INIT_FAILED) ] = "Failed on Dev Init",
  [ EARN5_OFST(EARN5_DEV_CAPACITY_EXCEEDED) ] = "Chip capacity exceeded",
  [ EARN5_OFST(EARN5_DEV_SPI_CHAN_MAP_FAILED) ] = "SPI channel mapping failed",
  /*
   * Port(port_id) related errors
   */
  [ EARN5_OFST(EARN5_PORT_ID_OUT_OF_RANGE) ] = "port_id out of range",
  [ EARN5_OFST(EARN5_PORT_NOT_OPEN) ] = "Port not opened yet",
  [ EARN5_OFST(EARN5_PORT_MALLOC_FAILED) ] = "Cannot allocate memory",
  [ EARN5_OFST(EARN5_PORT_SEM_INIT_FAILED) ] = "Failed to Init Semaphore",
  [ EARN5_OFST(EARN5_PORT_CFG_NULL) ]     = "pCfg is NULL",
  [ EARN5_OFST(EARN5_PORT_SUMM_NULL) ]    = "pSumm is NULL",
  [ EARN5_OFST(EARN5_PORT_DEBUG_MODE_INVALID) ]   = "Invalid Debug Mode",
  [ EARN5_OFST(EARN5_PORT_CFG_VALIDITY_ERR) ]     = "Failed to validate CFG",
  [ EARN5_OFST(EARN5_PORT_LOCK_NULL) ] = "CS_SEM pointer NULL",
  [ EARN5_OFST(EARN5_PORT_INVALID_OP) ] = "Invalid Option",
  [ EARN5_OFST(EARN5_PORT_INVALID_DIR) ] = "Invalid Tx/Rx option",
  [ EARN5_OFST(EARN5_PORT_NULL_PTR) ] = "NULL pointer",
  [ EARN5_OFST(EARN5_PORT_TBL_NULL) ] = "Table Pointer is NULL",
  [ EARN5_OFST(EARN5_PORT_TBL_START_INVALID) ] =
  "Table/Memory starting entry num is invalid",
  [ EARN5_OFST(EARN5_PORT_TBL_NUM_INVALID) ] =
  "Table/Memory number of entries is invalid",
  [ EARN5_OFST(EARN5_PORT_INVALID_MOD_ID) ] = "Invalid Module ID",
  [ EARN5_OFST(EARN5_PORT_MIN_GT_MAX) ] = "Min value greater than Max value",
  [ EARN5_OFST(EARN5_PORT_INVALID_REF_CLK) ] = "Invalid Refernce Clock value",
  [ EARN5_OFST(EARN5_PORT_RANGE_ERR) ] = "Argument out of Range",
  [ EARN5_OFST(EARN5_INVALID_HRESET_SEL) ] =
  "Invalid block selection for Hard Reset",
  [ EARN5_OFST(EARN5_RESET_TIMEOUT) ] = "Timed out waiting for Reset to clear",
  [ EARN5_OFST(EARN5_PORT_INVALID_USER_ARG) ] = "Invalid user arg",
  [ EARN5_OFST(EARN5_PORT_INVALID_SIZE) ] = "Invalid user data size",

  /* LIF related errors */
  [ EARN5_OFST(EARN5_LIF_INVALID_IF) ] = "Invalid I/F Selection",
  [ EARN5_OFST(EARN5_LIF_INVALID_IN_ETH_MODE) ] = "LIF in Ethernet PHY mode",
  [ EARN5_OFST(EARN5_LIF_INVALID_IN_SONET_MODE) ] = "LIF in SONET PHY mode",
  [ EARN5_OFST(EARN5_LIF_INVALID_LPTIME_MODE) ] = "Loop timing only available for XFP in Ethernet Mode",
  [ EARN5_OFST(EARN5_LIF_INVALID_DIGITAL_LB) ] = "Digital Loopback not supported",
  [ EARN5_OFST(EARN5_LIF_INVALID_ANALOG_LB) ] = "Analog Loopback not supported",
  [ EARN5_OFST(EARN5_LIF_INVALID_REF_CLK) ] = "Invalid Refernce Clock value",
  [ EARN5_OFST(EARN5_LIF_INVALID_ERROR_OP) ] = "Invalid Error Injection Option",

  /* FRAMER RELATED ERRORS */
  [ EARN5_OFST(EARN5_INVALID_IN_FRAMER_MODE) ] = "Invalid Operation in Framer Mode",
  [ EARN5_OFST(EARN5_INVALID_IN_ETH_MODE) ] = "Invalid Operation in GigE Mode",
  [ EARN5_OFST(EARN5_INVALID_OVRHD) ] = "Invalid sonet overhead option",
  [ EARN5_OFST(EARN5_INVALID_FR_DOMAIN) ] = "Invalid Framer domain",
  [ EARN5_OFST(EARN5_INVALID_SPE_BW) ] = "Invalid Framer domain",
  [ EARN5_OFST(EARN5_INVALID_RX_BURST_LEN) ] = "Invalid RX burst length",
  [ EARN5_OFST(EARN5_INVALID_TOH_BYTE) ] = "Invalid TOH Byte",
  [ EARN5_OFST(EARN5_INVALID_OVRHD_SRC) ] = "Invalid TOH source",
  [ EARN5_OFST(EARN5_OVRHD_SRC_NOT_VALID) ] = "TOH source not valid",
  [ EARN5_OFST(EARN5_FR_NULL_TRACE_MSG) ] = "Trace Msg Pointer is NULL",

  /* ETH related errors */
  [ EARN5_OFST(EARN5_ETH_RESTART_TIMEOUT) ] = "Timed out waiting for restart to clear",

  /* PPRBS related errors */
  [ EARN5_OFST(EARN5_PPRBS_INVALID_CHANNEL) ] = "Invalid PPRBS channel",
  [ EARN5_OFST(EARN5_PPRBS_INVALID_RATE) ] = "Invalid PPRBS Pkt Gen rate",
  [ EARN5_OFST(EARN5_PPRBS_RESYNC_FAILURE) ] = "Failed to Resync Checker",
} ;

/*
 * arn5_error_handler :
 *  This function is the error handler and prints the error information.
 */

void arn5_error_handler(cs_uint32 id, cs_uint32 errCode, 
			cs_int8 * fmt, ...)
{
  cs_boolean  found = TRUE ;
#ifndef CS_DONT_USE_VAR_ARGS
  va_list     VarArg ;
  char        log_str[256];
#endif

#ifndef CS_DONT_USE_STDLIB
  CS_SET_ERRNO(errCode);
#endif

  if((errCode < (EARN5_DRVR_CODE_BASE+1)) ||
     (errCode >= EARN5_MAX_ERR_CODE)) {
  }
  else if (errCode < EARN5_DEV_CODE_BASE) {
    CS_PRINT("Driver (%s)",
             earn5_err_tbl[EARN5_OFST(errCode)]) ;
  }
  else if (errCode < EARN5_PORT_CODE_BASE) {
    CS_PRINT("Dev-%d (%s)",
             id, earn5_err_tbl[EARN5_OFST(errCode)]) ;
  }
  else if (errCode < EARN5_MAX_ERR_CODE) {
    CS_PRINT("Port-0x%04x (%s)",
             id, earn5_err_tbl[EARN5_OFST(errCode)]) ;
  }
  else {
    found = FALSE ;
  }

  /* print additional error text, if any */
  if ( found && fmt ) {
#ifndef CS_DONT_USE_VAR_ARGS
      va_start(VarArg, fmt) ;
      vsprintf(log_str, fmt, VarArg) ;
      va_end(VarArg) ;
      CS_PRINT(log_str);
#else
      CS_PRINT("\n") ;
#endif
  }
  else {
    CS_PRINT("\n");
  }

  return;
}
