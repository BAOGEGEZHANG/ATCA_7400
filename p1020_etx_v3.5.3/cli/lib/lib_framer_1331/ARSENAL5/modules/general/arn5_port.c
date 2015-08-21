/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_port.c
 * "Port" related code is implemented here.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_cb.h"
#include "arn5_print.h"
#include "arn5_common.h"

#include "arn5_gen_data.h"

#include "arn5_gen_api.h"
#include "arn5_mpif_api.h"
#include "arn5_eth_api.h"
#include "arn5_framer.h"

#include "arn5_lif_priv.h"
#include "arn5_framer_priv.h"
#include "arn5_eth_priv.h"
#include "arn5_spi_priv.h"
#include "arn5_pprbs_priv.h"
#include "arn5_spi_api.h"
#include "arn5_mpif_priv.h"


#define ARN5_SPI42Q_MAX_RATE_UNIT       (17)
/* based on 133MHz */
#define ARN5_SPI3_MAX_RATE_UNIT         (28)


/**************************************************************
 * Local function prototypes                                  *
 **************************************************************/
static
cs_status  arn5_port_verify_summ(cs_uint8 dev_id, cs_uint16 port_id,
                        arn5_port_summ_t * pSumm) ;
static
cs_status  arn5_port_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pCfg) ;
static
void  arn5_port_commit_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pCfg) ;
cs_status  arn5_port_init_data(cs_uint16 port_id, arn5_port_cfg_t * pCfg) ;
static
void  arn5_port_free_mem(cs_uint16 port_id) ;
static
cs_status  arn5_port_init_hw(cs_uint16 port_id, arn5_port_cfg_t * pCfg) ;
static
cs_status  arn5_port_verify_cfg(cs_uint16 port_id, arn5_port_cfg_t * pCfg) ;
static
void  arn5_port_dump_cfg_summ(cs_uint16 port_id, arn5_port_summ_t * pSumm) ;
static
void  arn5_port_destroy_sem(cs_uint16 port_id) ;



/****************************************************************/
/* $rtn_hdr_start  PORT OPEN                                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_uint16  arn5_port_open(cs_uint8 dev_id, arn5_port_cfg_t * pCfg,
                        cs_uint8 slice_num)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port configuration structure       */
/*              o Slice Number[0..7] to be used for the port    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Port Id (or port handle)                        */
/* DESCRIPTION:                                                 */
/* This API, on successful completion, will create a valid      */
/* port handle(aka port_id) and also provide the caller with    */
/* the driver recommended default configuration for the port.   */
/*                                                              */
/* The caller of this API is expected to allocate memory for    */
/* pCfg and also fill in the summary section in it.             */
/*                                                              */
/* The port_handle(or port_id) created by the driver should be  */
/* used in further invocations of the port-level API's.         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32     err_code ;
  cs_uint16     port_id = ARN5_INVALID_PORT_ID ;
  arn5_port_cb_t        * ppcb ;
  arn5_dev_cb_t         * pdcb ;
  cs_status     status = CS_OK ;
  cs_uint16     rateUnits, desiredRate ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_NULL_PTR, ": pCfg") ;
    return (CS_ERROR) ;
  }

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  /* also check if the device has been initialized! */
  if (arn5_dev_get_state(dev_id) != ARN5_DEV_STATE_INITIALIZED) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_NOT_INITED, NULL) ;
    return (CS_ERROR) ;
  }

  pdcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  switch (pCfg->summ.line_rate) {
    case ARN5_PORT_RATE_OC12C :
      if ((g_ARN5_ver[slice_num] & 0x4) == 0) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as OC12C.\n", slice_num) ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      if (pdcb->oc12_num >= g_ARN5_ver[ARN5_MAX_NUM_PORTS + 1]) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nToo many OC12C ports are opened.\n") ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      rateUnits = 4 ;
      break ;

    case ARN5_PORT_RATE_OC3C :
      if ((g_ARN5_ver[slice_num] & 0x2) == 0) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as OC3.\n", slice_num) ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      if (pdcb->oc3_num >= g_ARN5_ver[ARN5_MAX_NUM_PORTS + 2]) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nToo many OC3C ports are opened.\n") ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      rateUnits = 1 ;
      break ;

    case ARN5_PORT_RATE_GIGE :
      if ((g_ARN5_ver[slice_num] & 0x1) == 0) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as GigE.\n", slice_num) ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      if (pdcb->gige_num >= g_ARN5_ver[ARN5_MAX_NUM_PORTS + 3]) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nToo many GigE ports are opened.\n") ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      rateUnits = 7 ;
      break ;

    default :
      if ((g_ARN5_ver[slice_num] & 0x8) == 0) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as OC48.\n", slice_num) ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      if (pdcb->oc48_num >= g_ARN5_ver[ARN5_MAX_NUM_PORTS]) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nToo many OC48C ports are opened.\n") ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
      }
      rateUnits = 16 ;
      break ;
  }

  desiredRate = pdcb->total_rate_units + rateUnits ;
  if (desiredRate >
        g_ARN5_ver[ARN5_MAX_NUM_PORTS + 4]) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
      "\n** Warning!! Total rate exceeds the maximum.\n") ;
    status = CS_OK ;
  }

  switch (arn5_spi_dev_get_if_mode(dev_id)) {
    case ARN5_SPI3_MODE :
      if (desiredRate > ARN5_SPI3_MAX_RATE_UNIT) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\n** Warning!! Total rate exceeds the maximum.\n") ;
        status = CS_OK ;
      }
      break ;

    case ARN5_SPI42_QUARTER_RATE_MODE :
      if (desiredRate > ARN5_SPI42Q_MAX_RATE_UNIT) {
        CS_HNDL_ERROR(dev_id, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\n** Warning!! Total rate exceeds the maximum.\n") ;
        status = CS_OK ;
      }
      break ;

    default :
      /* no extra check */
      break ;
  }

  CS_MEMSET( (void *) ((cs_uint32)pCfg + sizeof(arn5_port_summ_t)),
             0,
             sizeof(arn5_port_cfg_t) - sizeof(arn5_port_summ_t) ) ;

  if ( arn5_dev_create_port_id(dev_id, &pCfg->summ, &port_id,
        &ppcb, slice_num)
       != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  /* clear the Port CB */
  CS_MEMSET( (void *) ppcb, 0, sizeof(arn5_port_cb_t) ) ;

  /* copy some essential info into the port cb */
  ppcb->port_id = port_id ;
  ppcb->state   = ARN5_PORT_STATE_OPENED ;
  ppcb->summ    = pCfg->summ ;

  if ( arn5_port_verify_summ(dev_id, port_id, &pCfg->summ)
       != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  /* Provide the user with the driver recommended default config */
  if ( arn5_port_get_default_cfg(port_id, pCfg) != CS_OK ) {
    CS_PRINT("dev-%d %s() ERROR: Cannot get default cfg!\n",
             dev_id, __FUNCTION__) ;
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  arn5_port_commit_default_cfg(port_id, pCfg) ;
  //CS_PRINT("\tARN5 Port(port-id = port handle = 0x%04x) successfully opened\n",
  //         port_id) ;

  switch (pCfg->summ.line_rate) {
    case ARN5_PORT_RATE_OC12C :
      (pdcb->oc12_num)++ ;
      break ;

    case ARN5_PORT_RATE_OC3C :
      (pdcb->oc3_num)++ ;
      break ;

    case ARN5_PORT_RATE_GIGE :
      (pdcb->gige_num)++ ;
      break ;

    default :
      (pdcb->oc48_num)++ ;
      break ;
  }
  pdcb->total_rate_units += rateUnits ;

  RTN_EXIT :
    if ( (status != CS_OK) && (port_id != ARN5_INVALID_PORT_ID) ) {
      arn5_dev_delete_port_id(port_id) ;
      port_id = ARN5_INVALID_PORT_ID ;
    }

  return (port_id) ;
}


/****************************************************************/
/* $rtn_hdr_start  PORT CLOSE                                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_port_close(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Port Id (or port handle)                        */
/* DESCRIPTION:                                                 */
/* This API deletes the port instance specified. It frees up    */
/* all the port data-structures that were dynamically           */
/* allocated.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32             err_code ;
  arn5_port_cb_t        * ppcb ;
  arn5_dev_cb_t         * pdcb ;
  cs_uint8              slice_num, dev_id ;


  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  pdcb = ARN5_PORT_ID_TO_DEVCB_PTR(port_id) ;
  ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  switch (ppcb->summ.line_rate) {
    case ARN5_PORT_RATE_OC12C :
      if (pdcb->oc12_num) (pdcb->oc12_num)-- ;
      pdcb->total_rate_units -= 4 ;
      break ;

    case ARN5_PORT_RATE_OC3C :
      if (pdcb->oc3_num) (pdcb->oc3_num)-- ;
      pdcb->total_rate_units -= 1 ;
      break ;

    case ARN5_PORT_RATE_GIGE :
      if (pdcb->gige_num) (pdcb->gige_num)-- ;
      pdcb->total_rate_units -= 7 ;
      break ;

    default :
      if (pdcb->oc48_num) (pdcb->oc48_num)-- ;
      pdcb->total_rate_units -= 16 ;
      break ;
  }

  /* shut down the slice upon port close */
  /* drop the corresponding api chan as well */
  slice_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  dev_id = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  arn5_spi_drop_chan_data_ctl(dev_id, slice_num, TRUE) ;
  arn5_slice_ctl_pwr_down(dev_id, slice_num, CS_ENABLE) ;

  /* Deallocate the semaphores */
  arn5_port_destroy_sem(port_id) ;

  /* Free any dynamically allocated memory */
  arn5_port_free_mem(port_id) ;

  /* Finally, delete the port_id and de-allocate the Port PCB */
  arn5_dev_delete_port_id(port_id) ;

  CS_PRINT("\tPort-0x%04x closed\n", port_id) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  PORT INITIALIZATION                          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_port_init(cs_uint16 port_id, arn5_port_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port configuration structure       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the specified port hardware and software    */
/* data structures with the given configuration.                */
/* The caller is responsible for allocating memory for the      */
/* configuration structure, and populating it.                  */
/*                                                              */
/* The [port_id] is an unique value in the system and           */
/* this is the driver created port-handle when the port was     */
/* opened.                                                      */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint8      dev_id ;
  cs_uint32     err_code ;

  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  dev_id    = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  /*
   * Do sanity check on configuration parameters
   */
  if ( arn5_port_verify_summ(dev_id, port_id, &pCfg->summ)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_port_verify_cfg(port_id, pCfg) != CS_OK ) {
     return (CS_ERROR) ;
  }

  if ( arn5_port_init_data(port_id, pCfg) != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_port_init_hw(port_id, pCfg) != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET PORT RUNNING CONFIGURATION               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_port_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to Port Running Configuration         */
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
  cs_uint8            dev_id, port_num ;
  arn5_port_cb_t      * ppcb ;
  cs_uint32           err_code ;

  if ( pRunCfg == NULL ) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  dev_id    = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  port_num  = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;
  ppcb      = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  CS_MEMSET( (void *)pRunCfg, 0, sizeof(arn5_port_runcfg_t) ) ;
  CS_PRINT("ARN5 port-0x%x: Retrieving port running cfg...\n",
           port_id) ;

  /*
   * Get driver's dev_id
   */
   pRunCfg->dev_id = dev_id ;
  /*
   * Get port summary
   */
  pRunCfg->summ.line_rate = ARN5_GET_LINE_RATE_FROM_HW(port_id) ;
  pRunCfg->summ.l2_prot = ARN5_GET_L2_PROT_FROM_HW(port_id) ;
  pRunCfg->eth_valid = (pRunCfg->summ.line_rate == ARN5_PORT_RATE_GIGE) ;
  pRunCfg->framer_valid = !pRunCfg->eth_valid ;
  pRunCfg->lif_valid = TRUE ;
  pRunCfg->pprbs_valid = TRUE ;

  /*
   * Get port configuration
   */
  /* spi will get eth info if needed */
  if (arn5_spi_get_running_cfg(port_id, pRunCfg) != CS_OK) {
    return (CS_ERROR) ;
  }

  if (pRunCfg->framer_valid) {
    if (arn5_framer_get_running_cfg(port_id, pRunCfg) != CS_OK) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg->eth_valid) {
    if (arn5_eth_get_running_cfg(port_id, pRunCfg) != CS_OK) {
      return (CS_ERROR) ;
    }
  }

  if (arn5_lif_get_running_cfg(port_id, pRunCfg) != CS_OK) {
    return (CS_ERROR) ;
  }

  if (pRunCfg->pprbs_valid) {
    if (arn5_pprbs_get_running_cfg(port_id, pRunCfg) != CS_OK) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP PORT RUNNING CONFIGURATION              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_port_dump_running_cfg(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves and prints the current run-time hardware           */
/* configuration for the specified port.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_port_runcfg_t    * pRunCfg = NULL ;
  cs_uint32             err_code ;
  cs_status             status = CS_OK ;

  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pRunCfg = (arn5_port_runcfg_t *) CS_MALLOC( sizeof(arn5_port_runcfg_t) ) ;

  if (pRunCfg == NULL) {
    CS_PRINT("ARN5 port-0x%x ERROR: pRunCfg is NULL\n", port_id) ;
    return (CS_ERROR) ;
  }

  if ( arn5_port_get_running_cfg(port_id, pRunCfg) != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  arn5_print_drvr_ver() ;  /* first print driver rel info */
  CS_PRINT("\n") ;
  CS_PRINT("\t\t **************************************\n") ;
  CS_PRINT("\t\t * PORT-0x%04x  RUNNING CONFIGURATION *\n", port_id) ;
  CS_PRINT("\t\t **************************************\n") ;

  CS_PRINT("dev_id= 0x%x\n", pRunCfg->dev_id) ;

  arn5_port_dump_cfg_summ(port_id, &pRunCfg->summ) ;

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t SPI Block\n\n") ;
  if ( arn5_spi_dump_running_cfg(port_id, pRunCfg)
       != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t Framer Block (%s)\n\n",
          (pRunCfg->framer_valid) ? "valid" : "invalid") ;
  if (pRunCfg->framer_valid) {
    if ( arn5_framer_dump_running_cfg(port_id, pRunCfg)
         != CS_OK ) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t ETH Block (%s)\n\n",
          (pRunCfg->eth_valid) ? "valid" : "invalid") ;
  if (pRunCfg->eth_valid) {
    if ( arn5_eth_dump_running_cfg(port_id, pRunCfg)
         != CS_OK ) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t LIF Block\n\n") ;
  if ( arn5_lif_dump_running_cfg(port_id, pRunCfg)
       != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  /* pprbs is valid only when any of pprbs gen or checker is enabled */
  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t PPRBS Block (%s)\n\n",
           (pRunCfg->pprbs_valid) ? "valid" : "invalid") ;
  if (pRunCfg->pprbs_valid) {
    if ( arn5_pprbs_dump_running_cfg(port_id, pRunCfg)
         != CS_OK ) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }

RTN_EXIT :
  if (pRunCfg) CS_FREE(pRunCfg) ;
  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP PORT VITAL STATUS                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_port_dump_vital_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves and prints the current status at a glance          */
/* This is mainly for debug purpose as most of status are       */
/* available via individual API's call.                         */
/*                                                              */
/* This API will dump vital status for the device as well.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_port_runcfg_t    * pRunCfg = NULL ;
  cs_uint32             err_code ;
  cs_uint8              devId, portNum ;
  cs_status             status = CS_OK ;

  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pRunCfg = (arn5_port_runcfg_t *) CS_MALLOC( sizeof(arn5_port_runcfg_t) ) ;

  if (pRunCfg == NULL) {
    return (CS_ERROR) ;
  }

  if ( arn5_port_get_running_cfg(port_id, pRunCfg) != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  portNum = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;

  arn5_dev_dump_vital_status(devId) ;

  CS_PRINT("\n") ;
  CS_PRINT("\t *** PORT-0x%x  Vital Status  ***\n", port_id) ;
  CS_PRINT("=================================================\n") ;

  if (pRunCfg->summ.line_rate == ARN5_PORT_RATE_GIGE) {
    CS_PRINT("ETH Link Status               = %s\n",
      (arn5_eth_get_link_status(port_id) ? "Link Up" : "Link Down")) ;
    if (pRunCfg->eth.auto_neg_cfg.enable) {
      CS_PRINT("ETH Auto_Negotiation Status   = %sCompleted\n",
        (arn5_eth_get_auto_negotiation_status(port_id) ? "" : "Not ")) ;
    }
  }
  else {
    CS_PRINT("Framer In Frame Status        = %s\n",
      (arn5_framer_get_sync_status(port_id) ?
         "In Frame" : "Out of Frame")) ;
    CS_PRINT("Framer Rx Data Path Status    = %sReady\n",
      (arn5_framer_get_rx_data_path_status(port_id) ?
         "" : "Not ")) ;
  }

  if (pRunCfg->pprbs.hostChkr || pRunCfg->pprbs.lineChkr) {
    /* print pprbs header only if we need to */
    CS_PRINT("-- PPRBS --\n") ;
  }
  if (pRunCfg->pprbs.hostChkr) {
    arn5_pprbs_dump_checker(port_id, ARN5_PPRBS_HOST_CHAN) ;
  }
  if (pRunCfg->pprbs.lineChkr) {
    arn5_pprbs_dump_checker(port_id, ARN5_PPRBS_LINE_CHAN) ;
  }

RTN_EXIT :
  if (pRunCfg) CS_FREE(pRunCfg) ;
  return(status) ;
}


/*****************************************************************
 *  INTERNAL FUNCTIONS AND UTILITIES                             *
 *****************************************************************/

/*****************************************
 *         Verify port summary           *
 * Check that all the parameters in the  *
 * port configuration summary section    *
 * are valid.                            *
 *****************************************/
static
cs_status arn5_port_verify_summ(cs_uint8 dev_id, cs_uint16 port_id,
                               arn5_port_summ_t * pSumm)
{
  if (pSumm->l2_prot == ARN5_PORT_PROT_ETHERNET) {
    if (pSumm->line_rate != ARN5_PORT_RATE_GIGE) {
      CS_PRINT("Port-0x%x %s() ERROR: l2_prot(%d) is invalid!\n",
             port_id, __FUNCTION__, pSumm->l2_prot) ;
      return (CS_ERROR) ;
    }
  }
  else {
    if (pSumm->line_rate == ARN5_PORT_RATE_GIGE) {
      CS_PRINT("Port-0x%x %s() ERROR: l2_prot(%d) is invalid!\n",
             port_id, __FUNCTION__, pSumm->l2_prot) ;
      return (CS_ERROR) ;
    }
  }
  return (CS_OK) ;
}


/*****************************************
 * Get port default configuration
 * Retrieve the driver's default port
 * configuration based on the configuration
 * summary.
 *****************************************/
static
cs_status arn5_port_get_default_cfg(cs_uint16 port_id,
                               arn5_port_cfg_t * pCfg)
{
  arn5_dev_cb_t     * pdevcb ;
  arn5_port_summ_t  * pSumm ;

  pdevcb = ARN5_PORT_ID_TO_DEVCB_PTR(port_id) ;
  pSumm = &pCfg->summ ;

  /* SPI default configuration  */
  if (arn5_spi_get_default_cfg(port_id, pCfg) != CS_OK) {
    return (CS_ERROR) ;
  }

  if (pSumm->line_rate == ARN5_PORT_RATE_GIGE) {
    if ( arn5_eth_get_default_cfg(port_id, pCfg) != CS_OK) {
      return (CS_ERROR) ;
    }
    pCfg->eth_valid = TRUE ;
  }
  else {
    if ( arn5_framer_get_default_cfg(port_id, pCfg) != CS_OK ) {
      return (CS_ERROR) ;
    }
    pCfg->framer_valid = TRUE ;
  }

  if ( arn5_lif_get_default_cfg(port_id, pCfg) != CS_OK ) {
    return (CS_ERROR) ;
  }
  pCfg->lif_valid = TRUE ;

  return (CS_OK) ;
}

/*****************************************
 * Commit port default configuration
 * Commit any needed resources of a default
 * port configuration
 *****************************************/
static
void arn5_port_commit_default_cfg(cs_uint16 port_id,
                                 arn5_port_cfg_t * pCfg)
{
  /*
   * Call individual block functions to commit the default
   * configurations.
   */
  arn5_spi_commit_default_cfg(port_id, pCfg) ;
}


/************************************************
 * Initialize Port Data-structures              *
 * This is called from port initialization code *
 * after the port has been opened. The port CB  *
 * has already been allocated when the port was *
 * opened and this function just initializes it.*
 ************************************************/
cs_status arn5_port_init_data(cs_uint16 port_id, arn5_port_cfg_t * pCfg)
{
  arn5_port_cb_t        * ppcb ;

  ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  /* save the cfg summary */
  CS_MEMCPY( (cs_uint8 *)&ppcb->summ, (cs_uint8 *)&pCfg->summ,
             sizeof(arn5_port_summ_t) ) ;

  /* Initialize the port semaphores */
  if (CS_SEM_INIT(&ppcb->sem.pprbs) != CS_OK) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_SEM_INIT_FAILED,
                  "for PPRBS\n") ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.framer) != CS_OK) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_SEM_INIT_FAILED,
                  "for Framer\n") ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.lif) != CS_OK) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_SEM_INIT_FAILED,
                  "for LIF\n") ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.eth) != CS_OK) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_SEM_INIT_FAILED,
                  "for ETH\n") ;
    return (CS_ERROR) ;
  }

  ppcb->state = ARN5_PORT_STATE_DATA_INIT_DONE ;
  return (CS_OK) ;
}


static
void arn5_port_free_mem(cs_uint16 port_id)
{
  arn5_stats_clean_all(port_id, ARN5_PORT) ;
}


/***********************************************
 *  Initialize Port Hardware                   *
 ***********************************************/
static
cs_status arn5_port_init_hw(cs_uint16 port_id, arn5_port_cfg_t * pCfg)
{
  cs_uint8              dev_id, port_num ;
  arn5_port_summ_t      * pSumm ;
  arn5_port_cb_t        * ppcb ;
  cs_status             ret = CS_ERROR ;

  dev_id   = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  port_num = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;
  ppcb     = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;
  pSumm    = &pCfg->summ ;

  if ( arn5_slice_ctl_pwr_down(dev_id, port_num, CS_DISABLE)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_slice_hard_reset(dev_id, port_num, ARN5_PORT_BLKS_ID,
                            CS_RESET_TOGGLE)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* Configure mode control bits */
  if (arn5_mpif_set_line_rate(port_id, pCfg->summ.line_rate) != CS_OK ) {
    return (CS_ERROR) ;
  }

  if (arn5_lif_init_cfg(port_id, pCfg) != CS_OK) {
    return (CS_ERROR) ;
  }
  CS_MDELAY(2) ;

  /* Keep the framer in logic reset while configuring the port */
  if (pCfg->framer_valid) {
    if ( arn5_framer_sreset_sequence(port_id, CS_ENABLE)
         != CS_OK ) {
      return (CS_ERROR) ;
    }
  }

  /* SPI port initialization */
  if ( arn5_spi_init_cfg(port_id, pCfg) != CS_OK) {
    goto RTN_EXIT_INIT ;
  }

  if (pCfg->framer_valid) {
    if ( arn5_framer_init_cfg(port_id, pCfg) != CS_OK) {
    goto RTN_EXIT_INIT ;
    }
  }

  if (pCfg->eth_valid) {
    if ( arn5_eth_init_cfg(port_id, pCfg) != CS_OK ) {
    goto RTN_EXIT_INIT ;
    }
  }

  /*
   * As part of port initialization, the driver does NOT enable
   * the top-level BLOCK interrupts. When the application is ready
   * to handle interrupts, then it can do so by calling
   * arn5_port_enable_blk_irq(port_id, ARN5_ID_ALL_BLOCKS).
   */
  /* Enable all the port(channel) IRQ in the globalIntEnable reg. */
  if ( arn5_port_enable_irq(port_id) != CS_OK ) {
    goto RTN_EXIT_INIT ;
  }

  /* everything is ok */
  ret = CS_OK ;

RTN_EXIT_INIT :
  /* All done, take the framer out of reset */
  if (pCfg->framer_valid) {
    if ( arn5_framer_sreset_sequence(port_id, CS_DISABLE)
         != CS_OK ) {
      return (CS_ERROR) ;
    }
  }

  if (ret != CS_OK) return CS_ERROR ;

  ppcb->state = ARN5_PORT_STATE_HW_INIT_DONE ;
  return (CS_OK) ;
}


/*************************************
 * Verify port configuration         *
 *************************************/
static
cs_status arn5_port_verify_cfg(cs_uint16 port_id, arn5_port_cfg_t * pCfg)
{
  arn5_dev_cb_t  * pdevcb ;

  pdevcb   =  ARN5_PORT_ID_TO_DEVCB_PTR(port_id) ;

  if (pCfg->summ.line_rate == ARN5_PORT_RATE_GIGE) {
    if (pCfg->framer_valid) {
      CS_PRINT("Port-0x%x %s() ERROR: framer_valid s/b FALSE in GIGE mode!\n",
             port_id, __FUNCTION__) ;
      return (CS_ERROR) ;
    }
  }
  else {
    if (pCfg->eth_valid) {
      CS_PRINT("Port-0x%x %s() ERROR: eth_valid s/b FALSE in SONET/SDH mode!\n",
             port_id, __FUNCTION__) ;
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/**************************************
 *   Destroy port semaphores          *
 **************************************/
static
void  arn5_port_destroy_sem(cs_uint16 port_id)
{
  arn5_port_cb_t * ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  CS_SEM_DESTROY( &ppcb->sem.framer) ;
  CS_SEM_DESTROY( &ppcb->sem.lif ) ;
  CS_SEM_DESTROY( &ppcb->sem.eth ) ;
  CS_SEM_DESTROY( &ppcb->sem.pprbs ) ;
}


/*****************************************
 * Print Port Configuration Summary      *
 *****************************************/
static
void arn5_port_dump_cfg_summ(cs_uint16 port_id, arn5_port_summ_t * pSumm)
{
  if (pSumm == NULL) {
    CS_PRINT("Port-0x%x %s() ERROR: pSumm is NULL\n", port_id,
             __FUNCTION__) ;
    return ;
  }

  CS_PRINT(
  "==============================================================\n"
  ) ;
  if ( arn5_drvr_is_esc_code_enbld() ) {
    CS_PRINT("\033[4m") ; /* underline */
    CS_PRINT("\t\t Arsenal5 Port-0x%x Configuration Summary\n\n",
             port_id) ;
    CS_PRINT("\033[m") ; /* back to normal printing */
  }
  else {
    CS_PRINT("\t\t Arsenal5 Port-0x%x Configuration Summary\n\n",
             port_id) ;
  }

  CS_PRINT("Line Rate    = %s,   L2 Protocol   = %s\n",
           ARN5_GET_LINE_RATE_NAME(pSumm->line_rate),
           ARN5_GET_L2_PROT_NAME(pSumm->l2_prot)) ;
  CS_PRINT(
  "==============================================================\n"
  ) ;
}


