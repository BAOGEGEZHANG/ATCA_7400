/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_error.c
 *
 * Error table for various errors called out from the driver.
 *
 */
#ifndef CS_DONT_USE_VAR_ARGS
#	include <stdarg.h>
#endif

#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_print.h"
#include "m2r_error.h"

const char * em2r_err_tbl[EM2R_OFST(EM2R_MAX_ERR_CODE)] = {
  /*
   * Driver related errors
   */
  CS_DI([ EM2R_OFST(EM2R_DRVR_ALRDY_LOADED) ]	=) "Driver is already loaded",
  CS_DI([ EM2R_OFST(EM2R_DRVR_NOT_LOADED) ] 	=) "Driver not loaded yet",
  CS_DI([ EM2R_OFST(EM2R_DRVR_INCORRECT_ENDIANESS) ] =)
  "Incorrect endian option defined",
  CS_DI([ EM2R_OFST(EM2R_ALRDY_OUT_FILE_OPENED) ] =)
  "Driver output file already opened",
  CS_DI([ EM2R_OFST(EM2R_OUT_FILE_OPEN_ERROR) ] =)
  "Error opening driver output file",
  CS_DI([ EM2R_OFST(EM2R_NO_OUT_FILE_OPENED) ] =)
  "No driver output file opened",

  /*
   * Device(dev_id) related errors
   */
  CS_DI([ EM2R_OFST(EM2R_DEV_ID_OUT_OF_RANGE) ] =)   "dev_id out of range",
  CS_DI([ EM2R_OFST(EM2R_DEV_NOT_REGD) ] 	=)   "Device not registered yet",
  CS_DI([ EM2R_OFST(EM2R_DEV_NOT_INITED) ] 	=)   "Device not initialized yet",
  CS_DI([ EM2R_OFST(EM2R_CHIP_JTAG_ID_MISMATCH) ] =) "Chip JTAG Id Mismatch",
  CS_DI([ EM2R_OFST(EM2R_DEV_INVALID_HRESET_SEL) ] =)
  "Invalid dev hard reset selection",
  CS_DI([ EM2R_OFST(EM2R_BIST_NOT_DONE) ] 	=)  "BIST not completed yet",
  CS_DI([ EM2R_OFST(EM2R_MBIST_BAD_STATUS) ] 	=)  "Error from MBIST",
  CS_DI([ EM2R_OFST(EM2R_SBIST_BAD_STATUS) ] 	=)  "Error from SBIST",
  CS_DI([ EM2R_OFST(EM2R_DEV_INVALID_MODE) ] 	=)  "Invalid device Mode",
  CS_DI([ EM2R_OFST(EM2R_DEV_SEM_INIT_FAILED) ] =)  "Semaphore init failed",
  CS_DI([ EM2R_OFST(EM2R_DEV_SEM_TAKE_FAILED) ] =)  "Semaphore take failed",
  CS_DI([ EM2R_OFST(EM2R_DEV_INVALID_USER_ARG) ] =) "Invalid user arg specified",

  /*
   * Port(port_id) related errors
   */
  CS_DI([ EM2R_OFST(EM2R_PORT_ID_OUT_OF_RANGE) ] =) "port_id out of range",
  CS_DI([ EM2R_OFST(EM2R_PORT_UNINITIALIZED) ] =)   "Port not initialized",
  CS_DI([ EM2R_OFST(EM2R_PORT_MALLOC_FAILED) ] =)   "Cannot allocate memory",
  CS_DI([ EM2R_OFST(EM2R_PORT_SEM_INIT_FAILED) ] =) "Failed to Init Semaphore",
  CS_DI([ EM2R_OFST(EM2R_PORT_SEM_TAKE_FAILED) ] =) "Failed to take Semaphore",
  CS_DI([ EM2R_OFST(EM2R_PORT_CFG_NULL) ] 	=)  "pCfg is NULL",
  CS_DI([ EM2R_OFST(EM2R_PORT_SUMM_NULL) ] 	=)  "pSumm is NULL",
  CS_DI([ EM2R_OFST(EM2R_PORT_DEBUG_MODE_INVALID) ] =) "Invalid Debug Mode",
  CS_DI([ EM2R_OFST(EM2R_PORT_CFG_VALIDITY_ERR) ] =)   "Failed to validate CFG",
  CS_DI([ EM2R_OFST(EM2R_PORT_LOCK_NULL) ] =)   "CS_SEM pointer NULL",
  CS_DI([ EM2R_OFST(EM2R_PORT_INVALID_OP) ] =)  "Invalid Option",
  CS_DI([ EM2R_OFST(EM2R_PORT_INVALID_DIR) ] =) "Invalid Tx/Rx option",
  CS_DI([ EM2R_OFST(EM2R_PORT_NULL_PTR) ] =)    "NULL pointer",
  CS_DI([ EM2R_OFST(EM2R_PORT_TBL_NULL) ] =)    "Table Pointer is NULL",
  CS_DI([ EM2R_OFST(EM2R_PORT_TBL_START_INVALID) ] =)
  "Table/Memory starting entry num is invalid",
  CS_DI([ EM2R_OFST(EM2R_PORT_TBL_NUM_INVALID) ] =)
  "Table/Memory number of entries is invalid",
  CS_DI([ EM2R_OFST(EM2R_PORT_INVALID_MOD_ID) ] =)  "Invalid Module ID",
  CS_DI([ EM2R_OFST(EM2R_PORT_MIN_GT_MAX) ] =)      "Min value greater than Max value",
  CS_DI([ EM2R_OFST(EM2R_PORT_INVALID_REF_CLK) ] =) "Invalid Refernce Clock value",
  CS_DI([ EM2R_OFST(EM2R_PORT_RANGE_ERR) ] =)       "Argument out of Range",
  CS_DI([ EM2R_OFST(EM2R_INVALID_HRESET_SEL) ] =)
  "Invalid block selection for Hard Reset",
  CS_DI([ EM2R_OFST(EM2R_RESET_TIMEOUT) ] =)         "Timed out waiting for Reset to clear",
  CS_DI([ EM2R_OFST(EM2R_PORT_INVALID_USER_ARG) ] =) "Invalid user arg",
  CS_DI([ EM2R_OFST(EM2R_PORT_INVALID_SIZE) ] =)     "Invalid user data size",


  /* FRAMER RELATED ERRORS */
  CS_DI([ EM2R_OFST(EM2R_INVALID_IN_RMAC_MODE) ] =)
  "Invalid operation in RMAC mode",
  CS_DI([ EM2R_OFST(EM2R_INVALID_FRAMER_MODE) ] =)   "Invalid Framer Mode specified",
  CS_DI([ EM2R_OFST(EM2R_INVALID_IN_ETH_MODE) ] =)   "Invalid Operation in Ethernet Mode",
  CS_DI([ EM2R_OFST(EM2R_INVALID_IN_SONET_MODE) ] =) "Invalid Operation in SONET Mode",
  CS_DI([ EM2R_OFST(EM2R_INVALID_STREAM) ] =)        "Invalid Channel ID specified",
  CS_DI([ EM2R_OFST(EM2R_INVALID_OVRHD) ] =)         "Invalid sonet overhead option",
  CS_DI([ EM2R_OFST(EM2R_INVALID_FR_DOMAIN) ] =)     "Invalid Framer domain",
  CS_DI([ EM2R_OFST(EM2R_INVALID_SPE_BW) ] =)        "Invalid Framer domain",
  CS_DI([ EM2R_OFST(EM2R_INVALID_RX_BURST_LEN) ] =)  "Invalid RX burst length",
  CS_DI([ EM2R_OFST(EM2R_INVALID_TOH_BYTE) ] =)      "Invalid TOH Byte",
  CS_DI([ EM2R_OFST(EM2R_INVALID_OVRHD_SRC) ] =)     "Invalid TOH source",
  CS_DI([ EM2R_OFST(EM2R_OVRHD_SRC_NOT_VALID) ] =)   "TOH source not valid",
  CS_DI([ EM2R_OFST(EM2R_FR_NULL_TRACE_MSG) ] =)     "Trace Msg Pointer is NULL",

  /* RMAC RELATED ERRORS */
  CS_DI([ EM2R_OFST(EM2R_RMAC_INVALID_IN_BYPASS)] =)
  "Invalid operation in Bypass mode",
  CS_DI([ EM2R_OFST(EM2R_RMAC_INVALID_IN_SRP) ] =)  "Invalid operation in SRP",
  CS_DI([ EM2R_OFST(EM2R_RMAC_INVALID_IN_RPR) ] =)  "Invalid operation in RPR",
  CS_DI([ EM2R_OFST(EM2R_RMAC_INVALID_MEM_ID) ] =)  "Memory selection invalid",
  CS_DI([ EM2R_OFST(EM2R_CAM_ENTRY_INVALID_SA) ] =) "Invalid CAM index for SA",
  CS_DI([ EM2R_OFST(EM2R_RMAC_INVALID_SRESET_SEL) ] =)
  "Invalid RingMAC sub-block selection for soft reset",
  CS_DI([ EM2R_OFST(EM2R_RMAC_CTL_SEND_BUF_BUSY) ] =)
  "CTL Send Buffer is busy",
  CS_DI([ EM2R_OFST(EM2R_RMAC_IPS_SEND_BUF_BUSY) ] =)
  "IPS Send Buffer is busy",
  CS_DI([ EM2R_OFST(EM2R_RMAC_INVALID_IN_ONETB) ] =)
  "Invalid operation in Single TB mode",
  CS_DI([ EM2R_OFST(EM2R_RMAC_INVALID_IN_DUAL_TB) ] =)
  "Invalid operation in DUAL TB mode",
  CS_DI([ EM2R_OFST(EM2R_MS_FILE_OPEN_ERROR) ] =)
  "Error opening micro-sequencer file",
  CS_DI([ EM2R_OFST(EM2R_MS_INVALID_FILE_ENTRIES) ] =)
  "File contains invalid entries",
  CS_DI([ EM2R_OFST(EM2R_MS_INVALID_FA_PKT_LENGTH) ] =)
  "Invalid fairness packet length",
  CS_DI([ EM2R_OFST(EM2R_MS_UNMATCHED_CFG_MODE) ] =)
  "Specified RingMAC Micro-sequencer mode and specified MAC mode are inconsistent",
  CS_DI([ EM2R_OFST(EM2R_MS_UNKNOWN_CFG_MODE) ] =)
  "Specified RingMAC Micro-sequencer operating mode are unknown",
  CS_DI([ EM2R_OFST(EM2R_MS_UNCLEAR_SEMA) ] =)
  "RingMAC Micro-sequencer has not checked last sent semaphore",
  CS_DI([ EM2R_OFST(EM2R_MS_SEMA_REQ_NO_RESPONSE) ] =)
  "Micro-sequencer does not response requested semaphore",
  CS_DI([ EM2R_OFST(EM2R_RMAC_UNDEFINED_SPL_IRQ_OP) ] =)
  "Specified RingMAC special IRQ operation is undefined",
  CS_DI([ EM2R_OFST(EM2R_RMAC_WATCHDOG_LOCKED) ] =)
  "RingMAC watchdog timer has been locked",


  /* LIF related errors */
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_IF) ] =)            "Invalid I/F Selection",
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_IN_ETH_MODE) ] =)   "LIF in Ethernet PHY mode",
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_IN_SONET_MODE) ] =) "LIF in SONET PHY mode",
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_LPTIME_MODE) ] =)   "Loop timing only available for XFP,SFP",
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_DIGITAL_LB) ] =)    "Digital Loopback not supported",
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_ANALOG_LB) ] =)     "Analog Loopback not supported",
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_REF_CLK) ] =)       "Invalid Refernce Clock value",
  CS_DI([ EM2R_OFST(EM2R_LIF_INVALID_ERROR_OP) ] =)      "Invalid Error Injection Option",

  /* PPRBS related ERRORS     */
  CS_DI([ EM2R_OFST(EM2R_PPRBS_INVALID_CHANNEL) ] =) "Invalid PPRBS channel",
  CS_DI([ EM2R_OFST(EM2R_PPRBS_INVALID_RATE) ] =)    "Invalid PPRBS Pkt Gen rate",
  CS_DI([ EM2R_OFST(EM2R_PPRBS_RESYNC_FAILURE) ] =)  "Failed to Resync Checker",

  /* ETHERNET related errors */
  CS_DI([ EM2R_OFST(EM2R_ETH_INVALID_MSG_SIZE) ] =)  "Invalid Message Size",
  CS_DI([ EM2R_OFST(EM2R_ETH_MSG_TX_BUSY) ] =)    "Msg transmitter busy",
  CS_DI([ EM2R_OFST(EM2R_XETH_NULL_BUFFER) ] =)   "Pointer to msg buffer is NULL",
  CS_DI([ EM2R_OFST(EM2R_XETH_PROTOCOL_ERR) ] =)  "SOP/EOP Protocol Error",
  CS_DI([ EM2R_OFST(EM2R_XETH_CLK_ERROR) ] =)     "TX/RX clocks failed",
  CS_DI([ EM2R_OFST(EM2R_XETH_INVALID_ALARM) ] =) "Invalid Alarm",

  /* XGXS block related errors */
  CS_DI([ EM2R_OFST(EM2R_XGXS_BLK_POINTER_NULL) ] =)     "Pointer to XGXS block is NULL !",
  CS_DI([ EM2R_OFST(EM2R_XGXS_INVALID_XAUI_LANE_ID) ] =) "Invalid XAUI lane ID"
} ;

/*
 * m2r_error_handler :
 *  This function is the error handler and prints the error information.
 */

void m2r_error_handler(cs_uint32 id, cs_uint32 errCode,
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

  if((errCode < (EM2R_DRVR_CODE_BASE+1)) ||
     (errCode >= EM2R_MAX_ERR_CODE)) {
  }
  else if(errCode < EM2R_DEV_CODE_BASE) {
    CS_PRINT("Driver (%s) ",
	     em2r_err_tbl[EM2R_OFST(errCode)]) ;
  }
  else if(errCode < EM2R_PORT_CODE_BASE) {
    CS_PRINT("Dev-%d (%s) ",
	     id, em2r_err_tbl[EM2R_OFST(errCode)]) ;
  }
  else if(errCode < EM2R_MAX_ERR_CODE) {
    CS_PRINT("Port-%d (%s) ",
	     id, em2r_err_tbl[EM2R_OFST(errCode)]) ;
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

