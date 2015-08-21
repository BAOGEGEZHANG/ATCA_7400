/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_dev.c 
 * "Device" related code is implemented here.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"
#include "m2r_error.h"

#include "m2r_common.h"
#include "m2r_cb.h"

/****************************************************************/
/* $rtn_hdr_start  DEVICE ADDRESS REGISTRATION                  */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status m2r_dev_register(cs_uint16 dev_id, cs_uint64 base_addr)
/* INPUTS     : o Device Id                                     */
/*              o Base address of the ASIC                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Registers the given address as the base address for the      */
/* specified device. This address will be used in computing     */
/* the actual address of a register within the chip, ie.        */
/* (base_addr + offset), and used in the M2R_REG_READ() and      */
/* M2R_REG_WRITE() macros for register access.                   */
/*                                                              */
/* If this base address is not the physical address(as seen     */
/* by the CPU), then the register access macro's can be         */
/* customized to map it accordingly.                            */
/*                                                              */
/* This API should be called only after the driver has been     */
/* loaded.                                                      */
/* $rtn_hdr_end							*/
/****************************************************************/
{
  cs_uint32	i ;
  m2r_dev_cb_t 	* pdevcb ; 

  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( dev_id, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if (dev_id >= CS_MAX_NUM_DEVICES) {
    CS_HNDL_ERROR( dev_id, EM2R_DEV_ID_OUT_OF_RANGE, NULL ) ;
    return(CS_ERROR) ;
  }

  pdevcb = M2R_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if (pdevcb->state != M2R_DEV_STATE_INVALID) {
    CS_PRINT("dev-%d %s() ERROR: Device already registered(0x%08lx)\n",
	     dev_id, __FUNCTION__, pdevcb->base_addr) ;
    CS_PRINT("\tUn-register and re-register the device if needed\n") ;
    return(CS_ERROR) ;
  }

  pdevcb->state	        = M2R_DEV_STATE_REGISTERED ;
  pdevcb->dev_id       	= dev_id ;
  pdevcb->base_addr     = base_addr ;
  pdevcb->max_num_ports	= M2R_NUM_PORTS_PER_ASIC ;

  for (i = 0; i < M2R_NUM_PORTS_PER_ASIC; i++) {
    pdevcb->ppcb[i] = NULL ;
  }

#ifndef RELEASE_PLATFORM
  cs_lab_dev_register(CS_CHIP_MILAN2, dev_id, base_addr) ;
#endif

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start DEVICE INITIALIZATION                         */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status  m2r_dev_init(cs_uint16 dev_id) 
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initializes the device state to ready after verifying the    */
/* JTAG Id of the chip and hard-resetting any blocks that are   */
/* shared by all the ports.                                     */
/* It does NOT do any port related initialization.              */
/*                                                              */
/* This API should be called only after the device corresponding*/
/* to this port has been registered.                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_dev_cb_t  * pdevcb ;
  cs_uint32     jtag_id ;
  cs_uint32     err_code ;
  
  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  jtag_id = m2r_get_chip_jtag_id(dev_id) ;
  if ( !(M2R_IS_JTAG_ID_CORRECT(jtag_id)) ) {
    CS_HNDL_ERROR(dev_id, EM2R_CHIP_JTAG_ID_MISMATCH, 
                  ":chip's JTAG Id is 0x%x\n", jtag_id) ;
    return (CS_ERROR) ;
  }
  
  pdevcb = M2R_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  pdevcb->state = M2R_DEV_STATE_INITIALIZED ;

#ifndef RELEASE_PLATFORM
  if (!IN_TEST_ENV() && !IN_SIM_ENV()) {
    if ( m2r_wait_for_bist_done(dev_id, FALSE, TRUE, 50) == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    if ( m2r_check_bist_status(dev_id, FALSE, TRUE) == CS_ERROR ) {
      CS_PRINT("\n*************************************************\n") ;
      CS_PRINT("SELF-TEST AND REPAIR(STAR) BIST ERROR !!!!!!\n") ;
      CS_PRINT("*************************************************\n\n") ;
    }
  }
#else
  if ( m2r_wait_for_bist_done(dev_id, FALSE, TRUE, 50) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_check_bist_status(dev_id, FALSE, TRUE) == CS_ERROR ) {
    CS_PRINT("\n*************************************************\n") ;
    CS_PRINT("SELF-TEST AND REPAIR(STAR) BIST ERROR !!!!!!\n") ;
    CS_PRINT("*************************************************\n\n") ;
  }
#endif

// CS_PRINT("\tDevice(%d) initialized: base addr = 0x%lx\n",
//	dev_id, pdevcb->base_addr) ;

#ifndef RELEASE_PLATFORM
  cs_lab_dev_init(CS_CHIP_MILAN2, dev_id) ;
#endif

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start DEVICE INITIALIZATION                         */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status  m2r_dev_init_warm(cs_uint16 dev_id) 
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initializes the device state to ready after verifying the    */
/* JTAG Id of the chip and hard-resetting any blocks that are   */
/* shared by all the ports.                                     */
/* It does NOT do any port related initialization.              */
/*                                                              */
/* This API should be called only after the device corresponding*/
/* to this port has been registered.                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_dev_cb_t  * pdevcb ;
  cs_uint32     jtag_id ;
  cs_uint32     err_code ;
  
  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  jtag_id = m2r_get_chip_jtag_id(dev_id) ;
  if ( !(M2R_IS_JTAG_ID_CORRECT(jtag_id)) ) {
    CS_HNDL_ERROR(dev_id, EM2R_CHIP_JTAG_ID_MISMATCH, 
                  ":chip's JTAG Id is 0x%x\n", jtag_id) ;
    return (CS_ERROR) ;
  }
  
  pdevcb = M2R_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  pdevcb->state = M2R_DEV_STATE_INITIALIZED ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start UNREGISTER DEVICE                             */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status  m2r_dev_unregister(cs_uint16 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Unregisters the device, by setting its state to init state.  */ 
/* All port and device API's related to this device should NOT  */
/* be called after this.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	i ;
  m2r_dev_cb_t 	* pdevcb ;
  cs_uint16     port_id ;

  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( dev_id, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if (dev_id >= CS_MAX_NUM_DEVICES) {
    CS_HNDL_ERROR( dev_id, EM2R_DEV_ID_OUT_OF_RANGE, NULL ) ;
    return(CS_ERROR) ;
  }

  pdevcb =  M2R_DEV_ID_TO_DEVCB_PTR(dev_id) ;  

  /* need to check if this device is registered */
  if ( pdevcb->state == M2R_DEV_STATE_INVALID) {
  	return (CS_OK);
  }

  for (i = 0; i < pdevcb->max_num_ports; i++) {
    if (pdevcb->ppcb[i]) {
      port_id = m2r_get_port_id_from_dev_id(dev_id, i) ;
      if ( m2r_port_close(port_id) == CS_ERROR ) {
	CS_PRINT("M2R %s ERROR: dev%d- Failed to close port-%d\n",
		 __FUNCTION__, dev_id, port_id) ;
	return (CS_ERROR) ;
      }
    }
  }

  pdevcb->state = M2R_DEV_STATE_INVALID ;

#ifndef RELEASE_PLATFORM
  cs_lab_dev_unregister(CS_CHIP_MILAN2, dev_id) ;
#endif

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET PORT ID FROM DEVICE AND SLICE ID's       */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_uint16  m2r_get_port_id_from_dev_id(cs_uint16 dev_id, 
				       cs_uint16 slice_id)
/* INPUTS     : o Device Id                                     */
/*              o Slice(ie. channel) Id                         */ 
/* OUTPUTS    : ----                                            */
/* RETURNS    : Port Id or -1(on ERRROR)                        */ 
/* DESCRIPTION:                                                 */
/* Returns the port_id(as used in the port API's) for the given */
/* device and slice number.                                     */  
/*                                                              */
/* The [dev_id] parameter specifies the ASIC id on the board    */ 
/* and is 0...3 (with current CS_MAX_NUM_DEVICES setting at 4). */ 
/*                                                              */
/* The [slice_id] parameter specifies which slice within a      */ 
/* device.                                                      */ 
/* For Milan2: This value can be only 0 (one slice per device). */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(M2R_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( dev_id, EM2R_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if ( (dev_id >= CS_MAX_NUM_DEVICES) ||
       (slice_id  >= M2R_NUM_PORTS_PER_ASIC) ) {
    CS_HNDL_ERROR(dev_id, EM2R_DEV_INVALID_USER_ARG,
		  "dev_id(%d) s/b 0...%d and slice_id(%d) s/b 0...%d\n",
		  dev_id, slice_id, 
		  CS_MAX_NUM_DEVICES,
		  M2R_NUM_PORTS_PER_ASIC - 1) ;
    return (-1) ;
  }

  return ( (dev_id * M2R_NUM_PORTS_PER_ASIC) + slice_id) ;
}

















