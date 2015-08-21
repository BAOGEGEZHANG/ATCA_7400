/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_port.c
 * "Port" related code is implemented here.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"
#include "m2r_error.h"

#include "m2r_cb.h"
#include "m2r_common.h"
#include "m2r_gen.h"


/**************************************************************
 * Local function prototypes                                  *
 **************************************************************/
static
cs_status  m2r_port_init_sem(cs_uint16 port_id, m2r_port_cfg_t * pCfg) ;
static
void  m2r_port_destroy_sem(cs_uint16 port_id) ;
static
cs_status  m2r_port_init_cfg(cs_uint16 port_id, m2r_port_cfg_t * pCfg) ;

static
cs_status m2r_port_check_cfg_summ(cs_uint16 port_id,
				  m2r_port_summ_t * pSumm) ;
static
cs_status  m2r_port_check_cfg_params(cs_uint16 port_id,
				     m2r_port_cfg_t * pCfg) ;
static
cs_status  m2r_port_cfg_mode_ctl(cs_uint16 port_id,
				 m2r_port_cfg_t * pCfg) ;
static
void m2r_dump_cfg_summ(cs_uint16 port_id, m2r_port_summ_t * pSumm) ;



/****************************************************************/
/* $rtn_hdr_start  PORT INITIALIZATION                          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : BoTh                                            */
cs_status m2r_port_init(cs_uint16 port_id, m2r_port_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/* 		o Pointer to port configuration structure       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the specified port hardware and software    */
/* data structures with the given configuration.                */
/* The caller is responsible for allocating memory for the      */
/* configuration structure, and populating it.                  */
/*                                                              */
/* The [port_id] is an unique value in the system and           */
/* For Milan-2: there is only one port per ASIC. The port_id's  */
/* could be 0, 1, 2, or 3, if there are 4 Milan2 ASIC's.        */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/*                                                              */
/* This API should be called only after the corresponding device*/
/* has been initialized.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_dev_cb_t	* pdevcb ;
  m2r_port_cb_t * ppcb ;

  if (port_id > M2R_MAX_NUM_PORTS) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_ID_OUT_OF_RANGE, NULL) ;
    return (CS_ERROR) ;
  }

  if (pCfg == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_CFG_NULL, NULL ) ;
    return (CS_ERROR) ;
  }

  pdevcb = M2R_PORT_ID_TO_DEVCB_PTR(port_id) ;
  if ( (pdevcb == NULL) ||
       (pdevcb->state != M2R_DEV_STATE_INITIALIZED) ) {
    CS_HNDL_ERROR( M2R_PORT_ID_TO_DEV_ID(port_id),
		   EM2R_DEV_NOT_INITED, "port-id= %d\n", port_id ) ;
    return (CS_ERROR) ;
  }

  if ( m2r_port_init_data(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  //CS_PRINT("\tM2R Port-%d opened\n", port_id) ;

  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
  
  if ( m2r_port_init_cfg(port_id, pCfg) == CS_ERROR ) {
    CS_PRINT("\n\t **** Port-%d: Hardware Initialization FAILED! ****\n",
	     port_id) ; 
    return (CS_ERROR) ;
  }
  else {
    //CS_PRINT("\n\t *** Port-%d HW Initialization done! ****\n", port_id);
    //m2r_dump_cfg_summ(port_id, &pCfg->summ) ;
    //CS_PRINT("\n") ;
  }

  m2r_framer_set_gpio(port_id, 0xd0, 0xff);

  ppcb->state = M2R_PORT_STATE_HW_CFG_DONE ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  PORT INITIALIZATION                          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : BoTh                                            */
cs_status m2r_port_init_warm(cs_uint16 port_id, m2r_port_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/* 		o Pointer to port configuration structure       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the specified port hardware and software    */
/* data structures with the given configuration.                */
/* The caller is responsible for allocating memory for the      */
/* configuration structure, and populating it.                  */
/*                                                              */
/* The [port_id] is an unique value in the system and           */
/* For Milan-2: there is only one port per ASIC. The port_id's  */
/* could be 0, 1, 2, or 3, if there are 4 Milan2 ASIC's.        */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/*                                                              */
/* This API should be called only after the corresponding device*/
/* has been initialized.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_dev_cb_t	* pdevcb ;
  m2r_port_cb_t * ppcb ;

  if (port_id > M2R_MAX_NUM_PORTS) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_ID_OUT_OF_RANGE, NULL) ;
    return (CS_ERROR) ;
  }

  if (pCfg == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_CFG_NULL, NULL ) ;
    return (CS_ERROR) ;
  }

  pdevcb = M2R_PORT_ID_TO_DEVCB_PTR(port_id) ;
  if ( (pdevcb == NULL) ||
       (pdevcb->state != M2R_DEV_STATE_INITIALIZED) ) {
    CS_HNDL_ERROR( M2R_PORT_ID_TO_DEV_ID(port_id),
		   EM2R_DEV_NOT_INITED, "port-id= %d\n", port_id ) ;
    return (CS_ERROR) ;
  }

  if ( m2r_port_init_data(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  ppcb->state = M2R_PORT_STATE_HW_CFG_DONE ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  PORT CLOSE                                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP             : BoTh                                      */
cs_status m2r_port_close(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Closes the port and deletes the instance of this port.       */
/* All port related data-structures in memory will be freed up. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16     sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id) ;
  m2r_dev_cb_t  * pdevcb = M2R_PORT_ID_TO_DEVCB_PTR(port_id) ;
  m2r_port_cb_t * ppcb ;

  ppcb = pdevcb->ppcb[sl_num] ;
  if (ppcb == NULL) {
    CS_PRINT("port-%d: %s() - ERROR no port resources allocated\n",
	     port_id, __FUNCTION__) ;
    return(CS_ERROR) ;
  }

  /* If memory allocated for RingMAC accounting stats then free it */
  if (ppcb->p_rmac_acctg_stats) {
    CS_FREE(ppcb->p_rmac_acctg_stats) ;
  }

  /*
   * Clean up any memory allocated for port statistcs and reset the
   * stats pointers.
   */
  m2r_stats_clean_all(port_id) ;

#ifdef POSIX_PTHREAD
/* Feature #c -> */
  if ( ppcb->debug_thread.tid ) {
    m2r_stop_debug_thread( port_id ) ; /* cancel debug thread */
  }
/* Feature #c <- */
#endif

  /* Finally, destroy semaphores and free the port CB */
  m2r_port_destroy_sem(port_id) ;

  if ( pdevcb->ppcb[sl_num] ) {
    CS_FREE( pdevcb->ppcb[sl_num] ) ;
    pdevcb->ppcb[sl_num] = NULL ;
  }

  CS_PRINT("M2R Port-%d closed\n", port_id) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET PORT DEFAULT CONFIGURATION     	       	*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status m2r_port_get_default_cfg(cs_uint16 port_id,
				   m2r_port_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/* 		o Pointer to port configuration structure     	*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the driver's default port configuration based on the     */
/* configuration summary provided(inside the config data-       */
/* structure). Since the ASIC can be configured in many ways,   */
/* this API requires that the caller fills in ALL the different */
/* fields in the configuration summary section. This information*/
/* will be used by the driver in providing a default            */
/* configuration settings for the various blocks in the chip.   */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure allocated by the caller. The caller is        */
/* responsible for filling in the configuration summary         */
/* structure(ie. pCfg->summ).                                   */
/*                                                              */
/* The caller can then update the configuration parameters as   */
/* desired and use the same config datas-structure to initialize*/
/* the port.                                                    */
/*                                                              */
/* This API should be called only after the corresponding device*/
/* has been initialized.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if (pCfg == NULL) {
    CS_PRINT("Port-%d: %s() ERROR - pCfg is NULL!\n",
	     port_id, __FUNCTION__) ;
    return (CS_ERROR) ;
  }

  if ( m2r_port_check_cfg_summ(port_id, &pCfg->summ)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* clear the configuration structure except the summary section */
  CS_MEMSET( (void *) ((cs_uint64)pCfg + sizeof(m2r_port_summ_t)),
             0,
	     sizeof(m2r_port_cfg_t) - sizeof(m2r_port_summ_t) ) ;

  if ( m2r_spi42_get_default_cfg(port_id,
				 &pCfg->summ, &pCfg->spi42)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  pCfg->spi42_valid = TRUE ;


  if (pCfg->summ.l2_prot == M2R_PORT_PROT_ETHERNET) {
    if (m2r_eth_get_default_cfg(port_id, &pCfg->summ,
                                 &pCfg->ethernet) == CS_ERROR) {
      return (CS_ERROR) ;
    }
    pCfg->eth_valid = TRUE ;
  }
  else {
    if ( m2r_framer_get_default_cfg(port_id, &pCfg->summ,
                                    &pCfg->framer) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
    pCfg->framer_valid = TRUE ;
  }

  if ( m2r_lif_get_default_cfg(port_id, &pCfg->summ,
			       &pCfg->lif) == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  pCfg->lif_valid = TRUE ;

  if (pCfg->summ.mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    pCfg->rmac_valid = FALSE ;
  }

  else { /* SRP or RPR port mode */
    if ( m2r_rmac_get_default_cfg(port_id, &pCfg->summ,
				  &pCfg->rmac) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
    pCfg->rmac_valid = TRUE ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET PORT RUNNING CONFIGURATION     	       	*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status m2r_port_get_running_cfg(cs_uint16 port_id,
				   m2r_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*		o Pointer to Port Running Configuration      	*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the current run-time hardware configuration for the    	*/
/* specified port.                                              */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the running          */
/* configuration data-structure allocated by the caller.        */
/* The driver is responsible for filling in ALL the fields in   */
/* the datastructure by reading from HW.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32     err_code ;

  if ( pRunCfg == NULL ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, ": port_id= %d\n",
                  port_id) ;
    return(CS_ERROR) ;
  }

  CS_MEMSET( (void *)pRunCfg, 0, sizeof(m2r_port_runcfg_t) ) ;
  CS_PRINT("Port-%d Retrieving running config from HW....\n",
	   port_id) ;
  /*
   * Get chip's JTAG-Id and driver's dev_id
   */
  pRunCfg->dev_id = M2R_PORT_ID_TO_DEV_ID(port_id) ;
  pRunCfg->jtag_id = m2r_get_chip_jtag_id(pRunCfg->dev_id) ;
  /*
   * Get the config summary info (reading from HW!)
   */
  pRunCfg->summ.mac_mode     = M2R_GET_MAC_MODE_FROM_HW(port_id) ;
  pRunCfg->summ.line_rate    = M2R_GET_LINE_RATE_FROM_HW(port_id) ;
  pRunCfg->summ.l2_prot      = M2R_GET_L2_PROT_FROM_HW(port_id) ;
  pRunCfg->summ.lif_mode     = M2R_GET_LIF_MODE_FROM_HW(port_id) ;
  pRunCfg->summ.rmac_ms_mode = M2R_GET_RMAC_MS_MODE_FROM_HW(port_id) ;

  pRunCfg->spi42_valid = TRUE ;
  pRunCfg->lif_valid   = TRUE ;
  if ( pRunCfg->summ.l2_prot == M2R_PORT_PROT_ETHERNET ) {
    pRunCfg->eth_valid    = TRUE ;
    pRunCfg->framer_valid = FALSE ;
  }
  else {
    pRunCfg->eth_valid    = FALSE ;
    pRunCfg->framer_valid = TRUE ;
  }

  pRunCfg->rmac_valid =
    (pRunCfg->summ.mac_mode == M2R_PORT_IN_BYPASS_MODE) ? FALSE : TRUE ;

  /*
   * Get the running configuration from various blocks
   */
  if (pRunCfg->spi42_valid) {
    if ( m2r_spi42_get_running_cfg(port_id, &pRunCfg->summ,
				   &pRunCfg->spi42)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg->lif_valid) {
    if ( m2r_lif_get_running_cfg(port_id, &pRunCfg->summ,
                                 &pRunCfg->lif) == CS_ERROR) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg->framer_valid) {
    if ( m2r_framer_get_running_cfg(port_id, &pRunCfg->summ,
                                 &pRunCfg->framer) == CS_ERROR) {
      return (CS_ERROR) ;
    }
  }


  if (pRunCfg->eth_valid) {
    if ( m2r_eth_get_running_cfg(port_id, &pRunCfg->summ,
                                 &pRunCfg->eth) == CS_ERROR) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg->rmac_valid) {
    if (m2r_rmac_get_running_cfg(port_id, &pRunCfg->summ,
				 &pRunCfg->rmac)
	== CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP PORT RUNNING CONFIGURATION     	       	*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status m2r_port_dump_running_cfg(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves and prints the current run-time hardware          	*/
/* configuration for the specified port.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_port_runcfg_t * pRunCfg = NULL ;
  cs_uint32	    err_code ;
  cs_status	    status = CS_OK ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, ": port_id= %d\n",
                  port_id) ;
    return(CS_ERROR) ;
  }

  pRunCfg = (m2r_port_runcfg_t *)
            CS_MALLOC( sizeof(m2r_port_runcfg_t) ) ;

  if (pRunCfg == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED, NULL ) ;
    return (CS_ERROR) ;
  }

  if ( m2r_port_get_running_cfg(port_id, pRunCfg) == CS_ERROR ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  m2r_print_drvr_ver() ; /* first print driver rel info */
  CS_PRINT("\tWarm Start = %s\n", 
	  (m2r_is_warm_start_in_progress()) ? "In Progress!" : "Not active") ;
  CS_PRINT("\n") ;
  CS_PRINT("\t\t ***********************************\n") ;
  CS_PRINT("\t\t * PORT-%d  RUNNING CONFIGURATION  *\n", port_id) ;
  CS_PRINT("\t\t ***********************************\n") ;

  CS_PRINT("\t Chip JTAG-Id= 0x%08x, dev_id= 0x%x\n",
	   pRunCfg->jtag_id, pRunCfg->dev_id) ;
  m2r_dump_cfg_summ(port_id, &pRunCfg->summ) ;

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t SPI4.2 Block (%s)\n\n",
	   (pRunCfg->spi42_valid) ? "valid" : "INVALID") ;
  if (pRunCfg->spi42_valid) {
    if ( m2r_spi42_dump_running_cfg(port_id, &pRunCfg->spi42)
	 == CS_ERROR ) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t LIF Block (%s)\n\n",
	   (pRunCfg->lif_valid) ? "valid" : "INVALID") ;
  if (pRunCfg->lif_valid) {
    if ( m2r_lif_dump_running_cfg(port_id, &pRunCfg->lif) == CS_ERROR) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }


  if (pRunCfg->framer_valid) {
    CS_PRINT("=================================================\n") ;
    CS_PRINT("\t\t FRAMER Block (%s)\n\n",
	   (pRunCfg->framer_valid) ? "valid" : "INVALID") ;
    if ( m2r_framer_dump_running_cfg(port_id, &pRunCfg->framer) == CS_ERROR) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }

  if (pRunCfg->eth_valid) {
    CS_PRINT("=================================================\n") ;
    CS_PRINT("\t\t ETHERNET Block (%s)\n\n",
	   (pRunCfg->eth_valid) ? "valid" : "INVALID") ;
    if ( m2r_eth_dump_running_cfg(port_id, &pRunCfg->eth) == CS_ERROR) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }

  CS_PRINT("=================================================\n") ;
  CS_PRINT("\t\t RINGMAC Block (%s)\n\n",
	   (pRunCfg->rmac_valid) ? "valid" : "INVALID") ;
  if (pRunCfg->rmac_valid) {
    if ( m2r_rmac_dump_running_cfg(port_id, &pRunCfg->rmac)
	 == CS_ERROR ) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }

  CS_PRINT("\n\n") ;

  RTN_EXIT :
    if (pRunCfg) CS_FREE(pRunCfg) ;
  return(status) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET PORT MAC MODE (SRP/RPR/Bypass)           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status  m2r_set_port_mac_mode(cs_uint16 port_id,
				 m2r_port_mac_mode_t mode)
/* INPUTS     : o Port Id                                       */
/*		o port MAC mode                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets port to be in SRP, RPR or Framer-only(RingMAC bypass)   */
/* mode.                                                        */
/*                                                              */
/* The [mode] parameter is specified to be one of the following:*/
/* M2R_PORT_IN_SRP_MODE,                                        */
/* M2R_PORT_IN_RPR_MODE,    or                                  */
/* M2R_PORT_IN_BYPASS_MODE  (Framer-only mode)                  */
/*                                                              */
/* NOTE: For Palermo, only M2R_PORT_IN_BYPASS_MODE is valid!!  !*/
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	err_code ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, ": port_id= %d\n",
                  port_id) ;
    return(CS_ERROR) ;
  }

  if (mode == M2R_PORT_IN_BYPASS_MODE) {
    if ( m2r_mpif_ctl_bypass_mode(port_id, M2R_ENABLE) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  else {
    /*
     * If NOT in bypass mode(ie. SRP or RPR), then need to set the
     * RingMAC register appropriately. In bypass mode, it's setting
     * is not an issue as RingMAC would be powered down.
     */
    if ( m2r_mpif_ctl_bypass_mode(port_id, M2R_DISABLE) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
    if ( m2r_rmac_set_mac_mode_sel(port_id, mode) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET PORT MAC MODE (SRP/RPR/Bypass)           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
m2r_port_mac_mode_t  m2r_get_port_mac_mode(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : MAC mode(bypass, SRP or RPR)                    */
/* DESCRIPTION:                                                 */
/* Returns MAC mode(RingMAC bypass, SRP, or RPR) setting for    */
/* this port.                                                   */
/* The returned value is one of:                                */
/* M2R_PORT_IN_SRP_MODE,                                        */
/* M2R_PORT_IN_RPR_MODE,    or                                  */
/* M2R_PORT_IN_BYPASS_MODE  (Framer-only mode)                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	err_code ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, ": port_id= %d\n",
                  port_id) ;
    return(0) ;
  }

  if ( m2r_mpif_is_in_mac_bypass(port_id) ) {
    return (M2R_PORT_IN_BYPASS_MODE) ;
  }
  else {
    return ( m2r_rmac_get_mac_mode_sel(port_id) ) ;
  }
}


/****************************************************************/
/* $rtn_hdr_start  READ FROM REGISTER                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_uint16  m2r_reg_read(cs_uint16 port_id, cs_uint32 reg_offset)
/* INPUTS     : o Port Id                                       */
/*              o Register offset                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Register value                                  */
/* DESCRIPTION:                                                 */
/* Reads the value from the specified register.                 */
/* Note that the register offset is the actual register OFFSET  */
/* as given in the register spec, without any shifting!         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  volatile cs_reg 	* drvr_reg_addr ;
  cs_uint16             dev_id, dev_num, sl_num ;
  M2R_t                 * pDev ;
  cs_uint32		err_code = 0 ;

  if (port_id > M2R_MAX_NUM_PORTS) {
    CS_PRINT("ERROR %s(): Port-Id(%d) out of range!\n",
	     __FUNCTION__, port_id) ;
    return (-1) ;
  }

  dev_id = M2R_PORT_ID_TO_DEV_ID(port_id) ;
  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_PRINT("ERROR %s(): Device(dev_id:%d, port_id:%d) not registered!\n",
	     __FUNCTION__, dev_id, port_id) ;
    return(-1) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* If this is not a slice register, then sl_num better be 0 */
  if ( !(M2R_IS_SLICE_REG(reg_offset * sizeof(cs_reg))) ) {
    sl_num = 0 ;
  }

  /* Get the address into the driver's chip register map */
  drvr_reg_addr = (volatile cs_reg *)
    ( ((cs_uint64)pDev) +
      (sizeof(slice_t) * sl_num) +
      (sizeof(cs_reg) * reg_offset) ) ;

  return ( M2R_REG_READ(drvr_reg_addr) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  WRITE TO REGISTER                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status  m2r_reg_write(cs_uint16 port_id, cs_uint32 reg_offset,
			 cs_uint16 data)
/* INPUTS     : o Port Id                                       */
/*              o Register offset                               */
/*              o Write data                                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Writes the given value to the specified register.            */
/* Note that the register offset is the actual register OFFSET  */
/* as given in the register spec, without any shifting!         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  volatile cs_reg 	* drvr_reg_addr ;
  cs_uint16             dev_id, dev_num, sl_num ;
  M2R_t                 * pDev ;
  cs_uint32             err_code = 0 ;

  if (port_id > M2R_MAX_NUM_PORTS) {
    CS_PRINT("ERROR %s(): Port-Id(%d) out of range!\n",
	     __FUNCTION__, port_id) ;
    return (CS_ERROR) ;
  }

  dev_id = M2R_PORT_ID_TO_DEV_ID(port_id) ;
  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_PRINT("ERROR %s(): Device(dev_id:%d, port_id:%d) not registered!\n",
	     __FUNCTION__, dev_id, port_id) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* If this is not a slice register, then sl_num better be 0 */
  if ( !(M2R_IS_SLICE_REG(reg_offset * sizeof(cs_reg))) ) {
    sl_num = 0 ;
  }

  /* Get the address into the driver's chip register map */
  drvr_reg_addr = (volatile cs_reg *)
    ( ((cs_uint64)pDev) +
      (sizeof(slice_t) * sl_num) +
      (sizeof(cs_reg) * reg_offset) ) ;

  M2R_REG_WRITE(drvr_reg_addr, data) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP REGISTER(S)                             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
void  m2r_dump_reg(cs_uint16 port_id, cs_uint32 reg_offset,
		   cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/*              o Register offset                               */
/*              o Number of registers                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : ----                                            */
/* DESCRIPTION:                                                 */
/* Reads the HW registers for the specified port and prints     */
/* them.                                                        */
/* Note that the register offset is the actual register OFFSET  */
/* as given in the register spec, without any shifting!         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	ii ;
  cs_reg        data ;

  CS_PRINT("\tPort-%d register dump\n", port_id) ;

  for (ii = 0; ii < num; ii++) {
    data = m2r_reg_read(port_id, reg_offset + ii) ;
    if ( (ii % 8) == 0 ) {
      CS_PRINT("\n") ;
      CS_PRINT("%08x \t:", reg_offset + ii) ;
    }
    CS_PRINT(" %04x ", data) ;
  }

  CS_PRINT("\n") ;

  return ;
}


/***********************************************************
 *        INTERNAL UTILITIES AND FUNCTIONS                 *
 ***********************************************************/

/*****************************************
 *   Initialize port data structures     *
 *****************************************/
cs_status m2r_port_init_data(cs_uint16 port_id, m2r_port_cfg_t * pCfg)
{
  cs_uint16     sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id) ;
  m2r_dev_cb_t  * pdevcb = M2R_PORT_ID_TO_DEVCB_PTR(port_id) ;
  m2r_port_cb_t * ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  /*
   * If the port has been previously init'ed, then de-allocate all
   * port resources for the previous instance, before continuing
   * with the port initialization.
   */
  if (ppcb) {
    m2r_port_close(port_id) ;
  }

  ppcb = (m2r_port_cb_t *) CS_MALLOC( sizeof(m2r_port_cb_t) ) ;
  if (ppcb == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED, NULL ) ;
    return (CS_ERROR) ;
  }
  pdevcb->ppcb[sl_num] = ppcb ; /* save it in device cb */

  CS_MEMSET( (void *) ppcb, 0, sizeof(m2r_port_cb_t) ) ; /* clear pcb */
  ppcb->port_id = port_id ;
  ppcb->state   = M2R_PORT_STATE_POR ; /* power-on-reset state */

  /* save a copy of the configuration summary info */
  CS_MEMCPY( (cs_uint8 *)&ppcb->summ, (cs_uint8 *)&pCfg->summ,
             sizeof(m2r_port_summ_t) ) ;

  /* set some default values */
  ppcb->tx_ckref = M2R_LIF_XFP_LCL_TIME_REF_CLK ;
  ppcb->tx_ckref_lptime = M2R_LIF_XFP_LP_TIME_REF_CLK ;
  ppcb->tx_clk_mode = M2R_LIF_TX_CLK_LOCAL ;

  if ( m2r_port_init_sem(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* RingMAC (lookup, stat)accounting SRAM's for SA and DA */
  ppcb->p_rmac_acctg_stats = NULL ;
  ppcb->acctg_stats_malloced = pCfg->rmac.cam.acctg_stats_mem_malloced ;
  if ( ((pCfg->summ.mac_mode == M2R_PORT_IN_SRP_MODE) ||
	(pCfg->summ.mac_mode == M2R_PORT_IN_RPR_MODE))
       &&
       (pCfg->rmac.cam.acctg_stats_mem_malloced) ) {

    ppcb->p_rmac_acctg_stats = (m2r_rmac_acctg_stats_t *)
		      CS_MALLOC( sizeof(m2r_rmac_acctg_stats_t) ) ;
    if (ppcb->p_rmac_acctg_stats == NULL) {
      CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED,
		     ":p_rmac_acctg_stats\n") ;
      CS_FREE(ppcb) ;
      return (CS_ERROR) ;
    }
  }

  /* clear acctg stats structure */
  if ( ppcb->p_rmac_acctg_stats ) {
    CS_MEMSET( (void *) ppcb->p_rmac_acctg_stats, 0,
	       sizeof(m2r_rmac_acctg_stats_t) ) ;
  }

  ppcb->state   = M2R_PORT_STATE_DATA_INIT_DONE ;

  return (CS_OK) ;
}


/**************************************
 *   Initialize port semaphores       *
 **************************************/
static
cs_status  m2r_port_init_sem(cs_uint16 port_id, m2r_port_cfg_t * pCfg)
{
  m2r_port_cb_t * ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if (CS_SEM_INIT(&ppcb->sem.mpif) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for MPIF\n" ) ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.spi42) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for SPI42\n" ) ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.lif) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for LIF\n" ) ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.pprbs) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for PPRBS\n") ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.framer) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for Framer\n") ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.eth) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for Ethernet\n") ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.rmac) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for RingMAC\n") ;
    return (CS_ERROR) ;
  }

  if (CS_SEM_INIT(&ppcb->sem.xgxs) == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_INIT_FAILED,
		  "for XGXS\n") ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/**************************************
 *   Destroy port semaphores          *
 **************************************/
static
void  m2r_port_destroy_sem(cs_uint16 port_id)
{
  m2r_port_cb_t * ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  CS_SEM_DESTROY( &ppcb->sem.mpif) ;
  CS_SEM_DESTROY( &ppcb->sem.spi42 ) ;
  CS_SEM_DESTROY( &ppcb->sem.lif ) ;
  CS_SEM_DESTROY( &ppcb->sem.pprbs ) ;
  CS_SEM_DESTROY( &ppcb->sem.framer ) ;
  CS_SEM_DESTROY( &ppcb->sem.eth ) ;
  CS_SEM_DESTROY( &ppcb->sem.rmac ) ;
  CS_SEM_DESTROY( &ppcb->sem.xgxs ) ;
}


/*********************************************************************
 * Configure hardware with user provided configuration data structure*
 *********************************************************************/
static
cs_status m2r_port_init_cfg(cs_uint16 port_id, m2r_port_cfg_t * pCfg)
{
  cs_uint32	err_code ;
  cs_uint32	rst_sel ;

  if ( pCfg == NULL ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, ": port_id= %d\n",
                  port_id) ;
    return(CS_ERROR) ;
  }

  /*
   * Do sanity check on configuration parameters
   */
  if ( m2r_port_check_cfg_summ(port_id, &pCfg->summ)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_port_check_cfg_params(port_id, pCfg) == CS_ERROR ) {
     return (CS_ERROR) ;
  }

  /* don't reset LIF while we reset framer and other blocks */
  rst_sel = M2R_PORT_BLKS_ID & (~M2R_ID_LIF) ;

  /* Hard Reset port before configuring hdw */
  if ( m2r_port_hard_reset(port_id, rst_sel,
			   M2R_RESET_TOGGLE) == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  
  /* If no external FRCLK, don't hard reset LIF !! */
  /* Other subblocks in Framer need LIF to supply some sort */
  /* of clock to complete hard reset if no external FRCLK   */
  /* present                                                */
  if (pCfg->lif.frclk_is_external) {
    if ( m2r_port_hard_reset(port_id, M2R_ID_LIF,
			   M2R_RESET_TOGGLE) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /* Configure mode control bits */
  if (m2r_port_cfg_mode_ctl(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* try to take care of gpio output as early as possible */
  if (pCfg->framer_valid)
  {
    /* program gpio output data first to avoid glitches */
    if (m2r_framer_set_gpio(port_id, pCfg->framer.gpio_init_out, 0xff)
	== CS_ERROR ) {
      return (CS_ERROR) ;
    }
   
    if (m2r_framer_cfg_gpio(port_id, pCfg->framer.gpio_cfg, 0xff)
	== CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (m2r_lif_init_cfg(port_id, &pCfg->summ, &pCfg->lif)
      == CS_ERROR) {
    return (CS_ERROR) ;
  }

  CS_MDELAY(2) ;

  /* Keep the framer in logic reset while configuring the port */
  if (pCfg->framer_valid) {
    if ( m2r_framer_sreset_sequence(port_id, M2R_ENABLE)
         == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pCfg->summ.mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    goto RMAC_BYPASS ;
  }

  if (pCfg->rmac_valid) {
    if (m2r_rmac_init_cfg(port_id, &pCfg->summ, &pCfg->rmac)
	== CS_ERROR) {
      return (CS_ERROR) ;
    }
  }

  RMAC_BYPASS :
  if (m2r_spi42_init_cfg(port_id, &pCfg->summ, &pCfg->spi42)
      == CS_ERROR) {
    return (CS_ERROR) ;
  }

  if (pCfg->framer_valid) {
    if ( m2r_framer_init_cfg(port_id, &pCfg->summ, &pCfg->framer)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pCfg->eth_valid) {
    if (m2r_eth_init_cfg(port_id, &pCfg->summ, &pCfg->ethernet)
	== CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /* All done, take the framer out of reset */
  if (pCfg->framer_valid) {
    if ( m2r_framer_sreset_sequence(port_id, M2R_DISABLE)
         == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /* if doing warm start, don't clear stats */
  if ( m2r_is_warm_start_in_progress() ) {
    goto ALL_DONE ;
  }

#ifndef RELEASE_PLATFORM
  if (!IN_TEST_ENV() && !IN_SIM_ENV()) {
    if (m2r_clear_stats(port_id) == CS_ERROR) {
      return (CS_ERROR) ;
    }
  }
#else
  if (m2r_clear_stats(port_id) == CS_ERROR) {
    return (CS_ERROR) ;
  }
#endif

 ALL_DONE:
  return (CS_OK) ;
}


/******************************************************************
 * Check the validity of the configuration summary parameters     *
 * when getting the default configuration or initializing the     *
 * port.                                                          *
 ******************************************************************/
static
cs_status m2r_port_check_cfg_summ(cs_uint16 port_id,
				  m2r_port_summ_t * pSumm)
{
  cs_boolean eth_mode = FALSE ;

  eth_mode = (pSumm->l2_prot == M2R_PORT_PROT_ETHERNET) ? TRUE : FALSE ;

  if ((eth_mode == TRUE) &&
      (pSumm->line_rate != M2R_PORT_RATE_10G)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
	   "Line rate should be M2R_PORT_RATE_10G in Ethernet Mode\n") ;
    return (CS_ERROR) ;
  }

  if ((eth_mode == FALSE) &&
      (pSumm->line_rate == M2R_PORT_RATE_10G)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
	   "Line rate shouldn't be M2R_PORT_RATE_10G in SONET Mode\n") ;
    return (CS_ERROR) ;
  }

  if ( (pSumm->mac_mode == M2R_PORT_IN_SRP_MODE) &&
       (pSumm->rmac_ms_mode != M2R_MS_SRP) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
		  "Invalid rmac_ms_mode(0x%x) specified for SRP\n",
		  pSumm->rmac_ms_mode) ;
    return (CS_ERROR) ;
  }

  if ( (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) &&
       ((pSumm->rmac_ms_mode == M2R_MS_SRP) ||
	(pSumm->rmac_ms_mode == M2R_MS_UNKNOWN)) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
		  "Invalid rmac_ms_mode(0x%x) specified for RPR\n",
		  pSumm->rmac_ms_mode) ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/******************************************************************
 * Check the validity of the configuration parameters passed when *
 * init'ing the port.                                             *
 ******************************************************************/
static
cs_status m2r_port_check_cfg_params(cs_uint16 port_id,
				    m2r_port_cfg_t * pCfg)
{
  if ( pCfg->summ.mac_mode == M2R_PORT_IN_BYPASS_MODE ) {
    if (pCfg->rmac_valid) {
     CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
               ":RMAC s/b invalid\n") ;
     return (CS_ERROR) ;
    }
  }

  else { /* not in bypass mode */
    if ( !(pCfg->rmac_valid) ) {
      CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
                ":RMAC s/b valid\n") ;
      return (CS_ERROR) ;
    }
  }

  if ( !(pCfg->spi42_valid) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
              ":SPI42 s/b valid\n") ;
    return (CS_ERROR) ;
  }

  if (pCfg->framer_valid == pCfg->eth_valid) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
          ":Only Framer or Ethernet s/b valid at a time\n") ;
    return (CS_ERROR) ;
  }

  if ( !(pCfg->lif_valid) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_VALIDITY_ERR,
              ":LIF s/b valid\n") ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/*********************************************************************
 * Configure the mode control bits(in MPIF) as per given config summ *
 *********************************************************************/
static
cs_status m2r_port_cfg_mode_ctl(cs_uint16 port_id,
				m2r_port_cfg_t * pCfg)
{
  m2r_ctl_t	ctl ;

  /* Bypass mode */
  ctl = ( (pCfg->summ.mac_mode == M2R_PORT_IN_BYPASS_MODE) ?
	  M2R_ENABLE : M2R_DISABLE ) ;

  if ( m2r_mpif_ctl_bypass_mode(port_id, ctl) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Set XGE or Sonet Mode */
  ctl = ( (pCfg->summ.l2_prot == M2R_PORT_PROT_ETHERNET) ?
           M2R_ENABLE : M2R_DISABLE ) ;
  if ( m2r_mpif_ctl_eth_mode(port_id, ctl) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Set oc192 or oc48 Mode */
  ctl = ( ((pCfg->summ.line_rate == M2R_PORT_RATE_OC48C) ||
           (pCfg->summ.line_rate == M2R_PORT_RATE_OC48)) ?
          M2R_ENABLE : M2R_DISABLE ) ;
  if ( m2r_mpif_ctl_oc48_mode(port_id, ctl) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  CS_MDELAY(2) ;
  return (CS_OK) ;
}


/***************************************************
 * Get pointer to statistics structure in the PCB  *
 ***************************************************/
void * m2r_get_pcb_stats_ptr(cs_uint16 port_id,
			     m2r_module_id_t mod_id)
{
  m2r_port_cb_t		* ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
  m2r_port_stats_t	* ps ;

  ps = (m2r_port_stats_t *) &ppcb->stats ;

  switch (mod_id) {
    case M2R_ID_SPI42 : return ( (void *) &ps->spi42 ) ;
    case M2R_ID_PPRBS : return ( (void *) &ps->pprbs ) ;
    case M2R_ID_FRAMER: return ( (void *) &ps->framer ) ;
    case M2R_ID_RMAC  : return ( (void *) &ps->rmac ) ;
    case M2R_ID_XGXS  : return ( (void *) &ps->xgxs ) ;
    case M2R_ID_LIF   : return ( (void *) &ps->lif ) ;
    case M2R_ID_ALL_BLOCKS : return ( ( void *) ps ) ;
    default : return (NULL) ; /* error */
  }
}


/****************************************************
 * Print Configuration Summary                      *
 ****************************************************/
static
void m2r_dump_cfg_summ(cs_uint16 port_id, m2r_port_summ_t * pSumm)
{
  if (pSumm == NULL) {
    CS_PRINT("Port-%d: ERROR - pSumm is NULL!\n", port_id) ;
  }

  CS_PRINT(
  "==============================================================\n"
  ) ;
  if ( m2r_drvr_is_esc_code_enbld() ) {
    CS_PRINT("\033[4m") ; /* underline */
    CS_PRINT("\t\t Milan Port-%d Configuration  Summary\n\n", port_id) ;
    CS_PRINT("\033[m") ; /* back to normal printing */
  }
  else {
    CS_PRINT("\t\t Milan Port-%d Configuration Summary\n\n", port_id) ;
  }

  CS_PRINT("MAC Mode     = %s\n",
	   M2R_GET_MAC_MODE_NAME(pSumm->mac_mode)) ;
  CS_PRINT("Line Rate    = %s,   L2 Protocol   = %s\n",
	   M2R_GET_LINE_RATE_NAME(pSumm->line_rate),
	   M2R_GET_L2_PROT_NAME(pSumm->l2_prot)) ;
  CS_PRINT("Line Interface(LIF)   = %s\n",
	   M2R_GET_LIF_MODE_NAME(pSumm->lif_mode)) ;
  CS_PRINT("SRP/RPR Fairness Mode = %s\n",
	   M2R_GET_MS_MODE_NAME(pSumm->rmac_ms_mode)) ;
  CS_PRINT(
  "==============================================================\n"
  ) ;
}




