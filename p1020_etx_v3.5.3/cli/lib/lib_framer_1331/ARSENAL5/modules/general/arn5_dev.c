/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_dev.c
 * "Device" related code is implemented here.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"
#include "cs_utils.h"

#include "arn5_error.h"
#include "arn5_cb.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_gen_data.h"
#include "arn5_spi_api.h"

#include "arn5_gen_api.h"
#include "arn5_mpif_api.h"
#include "arn5_mpif_priv.h"
#include "arn5_spi_priv.h"
#include "arn5_pprbs_priv.h"

/*
 * Forward declarations
 */
cs_status arn5_dev_init_data(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg) ;
static
cs_status arn5_dev_init_hw(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg) ;
static
cs_status arn5_dev_verify_summ(cs_uint8 dev_id, arn5_dev_summ_t * pSumm) ;
static
cs_status arn5_dev_verify_cfg(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg) ;
static
void arn5_dev_dump_cfg_summ(cs_uint8 dev_id, arn5_dev_summ_t * pSumm) ;



/****************************************************************/
/* $rtn_hdr_start  DEVICE ADDRESS REGISTRATION                  */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_dev_register(cs_uint8 dev_id, cs_uint32 base_addr)
/* INPUTS     : o Device Id                                     */
/*              o Base address of the ASIC                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Registers the given address as the base address for the      */
/* specified device. This address will be used in computing     */
/* the actual address of a register within the chip, ie.        */
/* (base_addr + offset), and used in the CS_REG_READ() and      */
/* CS_REG_WRITE() macros for register access.                   */
/*                                                              */
/* If this base address is not the physical address(as seen     */
/* by the CPU), then the register access macro's can be         */
/* customized to map it accordingly.                            */
/*                                                              */
/* This API should be called only after the driver has been     */
/* loaded.                                                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_dev_cb_t  ** p_pdevcb = NULL ;
  arn5_dev_cb_t  * pdevcb = NULL ;

  if ( !(ARN5_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR( dev_id, EARN5_DRVR_NOT_LOADED, NULL ) ;
    return (CS_ERROR) ;
  }

  if (dev_id >= ARN5_MAX_NUM_DEVICES) {
    CS_HNDL_ERROR( dev_id, EARN5_DEV_ID_OUT_OF_RANGE, NULL ) ;
    return(CS_ERROR) ;
  }

  p_pdevcb = &( (ARN5_GET_DEVCB_TBL())[dev_id] ) ;
  pdevcb = *p_pdevcb ;
  if (pdevcb) {
    /* device was already registered - unregister first */
    if ( arn5_dev_unregister(dev_id) != CS_OK ) {
      return (CS_ERROR) ;
    }
  }

  pdevcb = (arn5_dev_cb_t *)CS_MALLOC(sizeof(arn5_dev_cb_t)) ;
  if (pdevcb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_MALLOC_FAILED, NULL) ;
    return (CS_ERROR) ;
  }

  *p_pdevcb = pdevcb ; /* save in driver cb */
  CS_MEMSET( (void *)pdevcb, 0, sizeof(arn5_dev_cb_t) ) ;
  pdevcb->state = ARN5_DEV_STATE_INVALID ;

  if (CS_SEM_INIT(&pdevcb->sem.mpif) != CS_OK) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SEM_INIT_FAILED,
                  "for MPIF\n" ) ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&pdevcb->sem.host_if) != CS_OK) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SEM_INIT_FAILED,
                  "for HOST_IF\n" ) ;
    return (CS_ERROR) ;
  }
  pdevcb->dev_id        = dev_id ;
  pdevcb->base_addr     = base_addr ;
  pdevcb->debug_flag    = (cs_uint32) -1 ;


#ifndef RELEASE_PLATFORM
  cs_lab_dev_register(CS_CHIP_ARSENAL5, (cs_uint16)dev_id, base_addr) ;
#endif

  /* Finally, update the state of the device */
  pdevcb->state         = ARN5_DEV_STATE_REGISTERED ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start UNREGISTER DEVICE                             */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_dev_unregister(cs_uint8 dev_id)
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
  cs_uint32     ii ;
  arn5_dev_cb_t  * pdevcb ;
  arn5_dev_cb_t  ** p_pdevcb ;
  cs_uint32     err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, ":dev_id(%d)\n",
                  dev_id) ;
    return (CS_ERROR) ;
  }

  p_pdevcb = &( (ARN5_GET_DEVCB_TBL())[dev_id] ) ;
  pdevcb =  *p_pdevcb ;

  /* need to check if this device is registered */
  if ( pdevcb == NULL)return (CS_OK) ;

  for (ii = 0; ii < ARN5_MAX_NUM_PORTS; ii++) {
    if (pdevcb->ppcb[ii]) {
      arn5_port_close( ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, ii) ) ;
    }
  }

#ifdef POSIX_PTHREAD
  if (pdevcb->dbt.irq_poll.tid) {
    arn5_stop_irq_polling(dev_id) ; /* cancel irq polling thread */
  }
#endif /* POSIX_PTHREAD */

  CS_SEM_DESTROY(&pdevcb->sem.mpif);
  CS_SEM_DESTROY(&pdevcb->sem.host_if);

  arn5_stats_clean_all((cs_uint16)dev_id, ARN5_DEVICE) ;

  CS_FREE(pdevcb) ;
  *p_pdevcb = NULL ;

  CS_PRINT("\tARN5 device(%d) unregistered!\n", dev_id) ;

#ifndef RELEASE_PLATFORM
  cs_lab_dev_unregister(CS_CHIP_ARSENAL5, (cs_uint16)dev_id) ;
#endif

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start GET DEVICE DEFAULT CONFIGURATION              */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_dev_get_default_cfg(cs_uint8 dev_id,
                                   arn5_dev_cfg_t * pDevCfg)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the driver's default DEVICE configuration based on the   */
/* configuration summary provided(inside the config data-       */
/* structure).                                                  */
/* This information will be used by the driver in providing a   */
/* default chip configuration.                                  */
/*                                                              */
/* Note that, this API does not access or configure the chip and*/
/* only recommends to the caller a recommended configuration.   */
/* The caller can choose to use it as is, or with necessary     */
/* modifications when initializing the device.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( !(ARN5_DRVR_IS_LOADED()) ) {
    CS_HNDL_ERROR(dev_id, EARN5_DRVR_NOT_LOADED, NULL) ;
    return (CS_ERROR) ;
  }

  if (dev_id >= ARN5_MAX_NUM_DEVICES) {
    CS_HNDL_ERROR( dev_id, EARN5_DEV_ID_OUT_OF_RANGE, NULL ) ;
    return(CS_ERROR) ;
  }

  if (pDevCfg == NULL) {
    CS_PRINT("ARN5 dev-%d: %s() ERROR - pDevCfg is NULL!\n",
             dev_id, __FUNCTION__) ;
    return (CS_ERROR) ;
  }

  /*
   * Ensure it is a valid summary before composing the default
   * configuration.
   */
  if ( arn5_dev_verify_summ(dev_id, &pDevCfg->summ) != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* General Purpose I/O - GPIO[7:0] inputs and GPIO[15:8] outputs */
  pDevCfg->gpio.alm_status_map = 0 ;
 /* pDevCfg->gpio.io_map = 0x00ff ;*/
  pDevCfg->gpio.io_map = 0x00f0;
  pDevCfg->gpio.output_values = 0x0000 ;

  /* MPIF default IRQ's */
  if ( arn5_mpif_dev_get_default_irq_cfg(dev_id, &pDevCfg->summ, &pDevCfg->mpif)
         != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_spi_dev_get_default_cfg(dev_id, pDevCfg) != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start DEVICE INITIALIZATION                         */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_dev_init(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initializes the device state to ready after verifying the    */
/* JTAG Id of the chip and hard-resetting and configuring any   */
/* HW blocks that are common and shared by all the ports.       */
/*                                                              */
/* It does NOT do any port related initialization.              */
/*                                                              */
/* This API should be called only after the device corresponding*/
/* to this port has been registered.                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_dev_cb_t  * pdevcb ;
  cs_uint32     err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  pdevcb =  ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  if (pdevcb == NULL) {
    CS_PRINT("ARN5 dev-%d %s() ERROR: pdevcb is NULL\n",
             dev_id, __FUNCTION__) ;
    return(CS_ERROR) ;
  }

  if (pdevcb->state == ARN5_DEV_STATE_INVALID) {
    CS_PRINT("ARN5 dev-%d %s() ERROR: Device not registered\n",
             dev_id, __FUNCTION__) ;
    return(CS_ERROR) ;
  }

  if (pdevcb->state == ARN5_DEV_STATE_INITIALIZED) {
    cs_uint32  base_addr = pdevcb->base_addr ;

    /* re-init'ing the device - do unreg/reg device again */
    arn5_dev_unregister(dev_id) ;
    arn5_dev_register(dev_id, base_addr) ;

    /* Need to re-get the pointer because previous structure */
    /* has been rebuilt.                                     */
    pdevcb =  ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  }

  //CS_PRINT(" before arn5_dev_init_data.\n");

  if ( arn5_dev_init_data(dev_id, pDevCfg) != CS_OK ) {
    return (CS_ERROR) ;
  }
 
  //CS_PRINT(" before arn5_dev_init_hw..\n");  
 
  if ( arn5_dev_init_hw(dev_id, pDevCfg) != CS_OK ) {
    return (CS_ERROR) ;
  }
  
  //CS_PRINT("\tARN5 dev-%d initialized: base addr = 0x%x\n",
  //         dev_id, pdevcb->base_addr) ;

#ifndef RELEASE_PLATFORM
  cs_lab_dev_init(CS_CHIP_ARSENAL5, (cs_uint16)dev_id) ;
#endif

  pdevcb->state = ARN5_DEV_STATE_INITIALIZED ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET DEVICE RUNNING CONFIGURATION             */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_dev_get_running_cfg(cs_uint8 dev_id,
                                  arn5_dev_runcfg_t * pRunCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to Device Running Configuration       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the current run-time hardware configuration for the      */
/* specified device(chip).                                      */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the running          */
/* configuration data-structure allocated by the caller.        */
/* The driver is responsible for filling in ALL the fields in   */
/* the datastructure by reading from HW.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t          * pDev ;
  arn5_dev_cb_t   * pdevcb ;
  arn5_dev_summ_t * pSumm ;
  cs_uint32      err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if ( pRunCfg == NULL ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  pdevcb =  ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  CS_MEMSET( (void *)pRunCfg, 0, sizeof(arn5_dev_runcfg_t) ) ;
  CS_PRINT("ARN5 dev-%d: Retrieving device running configuration..\n",
           dev_id) ;

  /*
   * Get chip's JTAG-Id
   */
  pRunCfg->jtag_id = arn5_get_chip_jtag_id(dev_id) ;

  /*
   * Get device summary
   */
  pSumm = &pRunCfg->summ ;
  pSumm->host_if = ( arn5_spi_dev_is_in_spi42_mode(dev_id) ?
                     ARN5_HOST_IF_SPI42 : ARN5_HOST_IF_SPI3 ) ;

  /* set block valid flags */

  /* GPIO pin configuration */
  pRunCfg->gpio.alm_status_map = CS_REG_READ(&pDev->MicroIF.GPIOAlarmControl.wrd) ;
  pRunCfg->gpio.io_map =
    ((CS_REG_READ(&pDev->MicroIF.GPIODirection1.wrd) & 0x00ff) << 8) |
    ((CS_REG_READ(&pDev->MicroIF.GPIODirection0.wrd) & 0x00ff)) ;

  pRunCfg->gpio.output_values =
    ((CS_REG_READ(&pDev->MicroIF.GPIOOutput1.wrd) & 0x00ff) << 8) |
    ((CS_REG_READ(&pDev->MicroIF.GPIOOutput0.wrd) & 0x00ff)) ;

  /* Block configurations */
  if (arn5_spi_dev_get_running_cfg(dev_id, pRunCfg) != CS_OK) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP DEVICE RUNNING CONFIGURATION            */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_dev_dump_running_cfg(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves and prints the current run-time hardware           */
/* configuration for the specified device.                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_dev_runcfg_t     * pRunCfg = NULL ;
  cs_uint32             err_code ;
  cs_status             status = CS_OK ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pRunCfg = (arn5_dev_runcfg_t *) CS_MALLOC( sizeof(arn5_dev_runcfg_t) ) ;

  if (pRunCfg == NULL) {
    CS_PRINT("ARN5 dev-%d ERROR: pRunCfg is NULL\n", dev_id) ;
    return (CS_ERROR) ;
  }

  if ( arn5_dev_get_running_cfg(dev_id, pRunCfg) != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  arn5_print_drvr_ver() ;  /* first print driver rel info */
  CS_PRINT("\n") ;
  CS_PRINT("\t\t *************************************\n") ;
  CS_PRINT("\t\t * DEVICE-%02d  RUNNING CONFIGURATION  *\n", dev_id) ;
  CS_PRINT("\t\t *************************************\n") ;

  CS_PRINT("Chip JTAG-Id= 0x%08x\n", pRunCfg->jtag_id) ;
  arn5_dev_dump_cfg_summ(dev_id, &pRunCfg->summ) ;

  CS_PRINT("\t GPIO alarm status pins = 0x%02x, output pins = 0x%04x,\n",
           pRunCfg->gpio.alm_status_map, pRunCfg->gpio.io_map) ;
  CS_PRINT("\t\t\t\t output values = 0x%04x\n",
           pRunCfg->gpio.output_values) ;

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t SPI Block\n\n") ;
  if ( arn5_spi_dev_dump_running_cfg(dev_id, pRunCfg)
       != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  RTN_EXIT :
    if (pRunCfg) CS_FREE(pRunCfg) ;
  return(status) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP DEVICE VITAL STATUS                     */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_dev_dump_vital_status(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves and prints the current status at a glance          */
/* This is mainly for debug purpose as most of status are       */
/* available via individual API's call.                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_dev_cb_t         * pdevcb ;
  cs_uint8              * str = NULL ;
  cs_uint32             err_code ;
  cs_status             status = CS_OK ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pdevcb =  ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  if (pdevcb->state != ARN5_DEV_STATE_INITIALIZED) { /* hwcfg not done */
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  ": device hw cfg not done yet!\n") ;
    return (CS_ERROR) ;
  }

  CS_PRINT("\n") ;
  CS_PRINT("\t *** DEVICE-%d  Vital Status  ***\n", dev_id) ;
  CS_PRINT("=================================================\n") ;

  if (arn5_spi_dev_is_in_spi42_mode(dev_id)) {
    arn5_spi42_sync_t    spi42Status ;

    spi42Status = arn5_spi42_dev_get_sync_state(dev_id, CS_TX) ;
    if (spi42Status == ARN5_SPI42_IN_SYNC) {
      str = "In Sync" ;
    }
    else if (spi42Status == ARN5_SPI42_INIT_STATE) {
      str = "Init State" ;
    }
    else {
      str = "Out Of Sync" ;
    }
    CS_PRINT("Host SPI4.2 TX Status  = %s\n", str) ;

    spi42Status = arn5_spi42_dev_get_sync_state(dev_id, CS_RX) ;
    if (spi42Status == ARN5_SPI42_IN_SYNC) {
      str = "In Sync" ;
    }
    else if (spi42Status == ARN5_SPI42_INIT_STATE) {
      str = "Init State" ;
    }
    else {
      str = "Out Of Sync" ;
    }
    CS_PRINT("Host SPI4.2 RX status  = %s\n", str) ;
  }

  return(status) ;
}


/***********************************************************
 *        INTERNAL UTILITIES AND FUNCTIONS                 *
 ***********************************************************/

/*****************************************
 *   Initialize device data structures   *
 *****************************************/
cs_status arn5_dev_init_data(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg)
{
  cs_uint32      ii ;
  arn5_dev_cb_t   * pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  /* save the cfg summary */
  CS_MEMCPY( (cs_uint8 *)&pdevcb->summ, (cs_uint8 *)&pDevCfg->summ,
             sizeof(arn5_dev_summ_t) ) ;

  for (ii = 0; ii < ARN5_MAX_NUM_PORTS; ii++) {
    pdevcb->ppcb[ii] = NULL ;
  }

  for (ii = 0; ii < ARN5_MAX_NUM_CHANNELS; ii++) {
    pdevcb->rx_phy_chan_map[ii] = ii ;
    pdevcb->tx_phy_chan_map[ii] = ii ;
    pdevcb->rx_log_chan_map[ii] = ii ;
    pdevcb->tx_log_chan_map[ii] = ii ;
  }

  return (CS_OK) ;
}


/*****************************************
 *   Initialize device hardware          *
 * All common sections of the chip that  *
 * are shared by all the ports will be   *
 * initialized.                          *
 *****************************************/
static
cs_status arn5_dev_init_hw(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg)
{
  cs_uint32     jtag_id ;

  jtag_id = arn5_get_chip_jtag_id(dev_id) ;
  if ( !(ARN5_IS_JTAG_ID_CORRECT(jtag_id)) ) {
    CS_HNDL_ERROR(dev_id, EARN5_CHIP_JTAG_ID_MISMATCH,
                  ":chip's JTAG Id is 0x%x\n", jtag_id) ;
    return (CS_ERROR) ;
  }

//  CS_PRINT("JTAG ID: 0x%x\n", jtag_id);

// bypass bist temperory
#if 1
  if ( CS_IN_CUSTOMER_ENV() || CS_IN_LAB_ENV() ) {
    if ( arn5_wait_for_bist_done(dev_id, FALSE, TRUE, 50)
         != CS_OK ) {
      return (CS_ERROR) ;
    }

    if ( arn5_check_bist_status(dev_id, FALSE, TRUE) != CS_OK ) {
      CS_PRINT("\n*************************************************\n") ;
      CS_PRINT("SELF-TEST AND REPAIR(STAR) BIST ERROR !!!!!!\n") ;
      CS_PRINT("*************************************************\n\n") ;
    }
  }
#endif

  if ( arn5_dev_verify_summ(dev_id, &pDevCfg->summ) != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_dev_verify_cfg(dev_id, pDevCfg) != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* sync slice enable state with hardware, assume slice 0 must be */
  /* powered on                                                    */
  if ( arn5_slice_ctl_pwr_down(dev_id, 0, CS_DISABLE)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* from now on, the slice enable is cached for driver to reference */
  /* Hard reset the chip */
  if ( arn5_dev_hard_reset(dev_id, ARN5_ID_ALL_BLOCKS, CS_RESET_TOGGLE)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* Configure GPIO */
  if ( arn5_mpif_cfg_gpio_alm_status(dev_id, pDevCfg->gpio.alm_status_map,
        0xff) != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_mpif_cfg_gpio_io(dev_id,
        (pDevCfg->gpio.io_map | (cs_uint16)pDevCfg->gpio.alm_status_map),
        0xffff) != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_mpif_write_gpio_output(dev_id,
        pDevCfg->gpio.output_values,
        0xffff) != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_mpif_dev_init_irq_cfg(dev_id, &pDevCfg->mpif)
        != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_spi_dev_init_cfg(dev_id, pDevCfg)
        != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/*****************************************
 *     Verify device summary             *
 * Check that all the parameters in the  *
 * device configuration summary section  *
 * are valid.                            *
 *****************************************/
static
cs_status arn5_dev_verify_summ(cs_uint8 dev_id, arn5_dev_summ_t * pSumm)
{
  if ( !(CS_IN_TEST_ENV()) ) {
    if ( (pSumm->host_if == ARN5_HOST_IF_SPI42) &&
         (!arn5_spi_dev_is_in_spi42_mode(dev_id))) {
      CS_PRINT(
        "dev-%d ERROR: Actual Host IF is not in SPI4.2 mode.\n",
        dev_id) ;
      return (CS_ERROR) ;
    }

    if ( (pSumm->host_if == ARN5_HOST_IF_SPI3) &&
         (!arn5_spi_dev_is_in_spi3_mode(dev_id))) {
      CS_PRINT(
        "dev-%d ERROR: Actual Host IF is not in SPI3 mode.\n",
        dev_id) ;
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/*****************************************
 *     Verify device configuration       *
 * Do sanity checking of the             *
 * configuration parameters.             *
 *****************************************/
static
cs_status arn5_dev_verify_cfg(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg)
{

  return (CS_OK) ;
}


/*****************************************
 * Print Device Configuration Summary    *
 *****************************************/
static
void arn5_dev_dump_cfg_summ(cs_uint8 dev_id, arn5_dev_summ_t * pSumm)
{
  if (pSumm == NULL) {
    CS_PRINT("dev-%d %s() ERROR: pSumm is NULL!\n",
             dev_id, __FUNCTION__) ;
    return ;
  }

  CS_PRINT(
  "==============================================================\n"
  ) ;
  if ( arn5_drvr_is_esc_code_enbld() ) {
    CS_PRINT("\033[4m") ; /* underline */
    CS_PRINT("\t\t Arsenal5 Device-%d Configuration  Summary\n\n",
             dev_id) ;
    CS_PRINT("\033[m") ; /* back to normal printing */
  }
  else {
    CS_PRINT("\t\t Arsenal5 Device-%d Configuration Summary\n\n",
             dev_id) ;
  }

  CS_PRINT("Host Interface         = %s\n",
           ARN5_GET_HOST_IF_NAME(pSumm->host_if)) ;
  CS_PRINT(
  "==============================================================\n"
  ) ;
}


/*********************************************************************
 * Create Port-Id(or Port Handle)                                    *
 * Port-Id is created by OR'ing (dev_id << 8) and port-num.          *
 * On successful assignment of the Port-Id, memory for the Port      *
 * Control Block(PCB) will be dynamically allocated and returned     *
 * to the user.                                                      *
 *********************************************************************/
cs_status  arn5_dev_create_port_id(cs_uint8 dev_id,
                                  arn5_port_summ_t * pSumm,
                                  cs_uint16 * p_port_id,
                                  arn5_port_cb_t ** p_ppcb,
                                  cs_uint8 slice_num)
{
  arn5_dev_cb_t  * pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if (pdevcb == NULL || p_port_id == NULL || p_ppcb == NULL) {
    return (CS_ERROR) ;
  }

  if (slice_num > ARN5_MAX_NUM_PORTS) {
    return (CS_ERROR) ;
  }

  *p_port_id = ARN5_INVALID_PORT_ID ;
  *p_ppcb    = NULL ;

  if (pdevcb->ppcb[slice_num]) { /* check if it is already in-use */
    CS_PRINT("Dev-%d ERROR: slice-%d already in use!\n",
             dev_id, slice_num) ;
    return (CS_ERROR) ;
  }

  *p_ppcb = (arn5_port_cb_t *) CS_MALLOC( sizeof(arn5_port_cb_t) ) ;
  if (*p_ppcb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_MALLOC_FAILED, NULL) ;
    return (CS_ERROR) ;
  }
  pdevcb->ppcb[slice_num] = *p_ppcb ;
  *p_port_id = ( (dev_id << 8) | slice_num ) ;
  return (CS_OK) ;
}


/*******************************************************************
 * Delete Port-Id                                                  *
 * The port instance is deleted and the device control block is    *
 * updated.                                                        *
 *******************************************************************/
void arn5_dev_delete_port_id(cs_uint16 port_id)
{
  cs_uint8      dev_id = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  arn5_dev_cb_t  * pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  cs_uint8      port_num = ARN5_PORT_ID_TO_PORT_NUM(port_id)  ;

  if ( pdevcb->ppcb[port_num] ) {
    CS_FREE( pdevcb->ppcb[port_num] ) ;
    pdevcb->ppcb[port_num] = NULL ;
  }
}


/******************************************************************
 * Get device state                                               *
 ******************************************************************/
cs_uint32  arn5_dev_get_state(cs_uint8 dev_id)
{
  arn5_dev_cb_t  * pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  return (pdevcb->state) ;
}



