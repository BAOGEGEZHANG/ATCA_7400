/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_gen.h
 * 
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_GEN_H__
#define __M2R_GEN_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_mpif.h"
#include "m2r_spi42.h"
#include "m2r_lif.h"
#include "m2r_pprbs.h"
#include "m2r_framer.h"
#include "m2r_rmac.h"
#include "m2r_eth.h"
#include "m2r_rmac_xgxs.h"

/******************************************************* 
 *                        Driver       		       *
 *******************************************************/
extern  cs_status  m2r_load_driver(void) ;
extern  cs_status  m2r_unload_driver(void) ;
extern  cs_status  m2r_start_output_to_file(char * fname) ;
extern  cs_status  m2r_stop_driver_output_to_file(void) ;
extern  void  m2r_stats_hex_format_ctl(m2r_ctl_t hex_ctl) ;
extern  void  m2r_print_drvr_ver() ;
extern  void  m2r_print_drvr_summary() ;
extern  cs_status  m2r_stop_warm_start() ;
extern  cs_status m2r_stop_warm_start() ;
extern  cs_boolean m2r_is_warm_start_in_progress() ;
/******************************************************* 
 *                        Device       		       *
 *******************************************************/
extern  cs_status  m2r_dev_register(cs_uint16 dev_id, 
				    cs_uint64 base_addr) ;
extern  cs_status  m2r_dev_unregister(cs_uint16 dev_id) ;
extern  cs_status  m2r_dev_init(cs_uint16 dev_id) ;
extern  cs_status  m2r_dev_init_warm(cs_uint16 dev_id) ;
extern
cs_uint16  m2r_get_port_id_from_dev_id(cs_uint16 dev_id,
                                       cs_uint16 slice_id) ;

/******************************************************* 
 *                        Port			       *
 *******************************************************/
/*************************************
 * Port Initialization Configuration *
 *************************************/ 
typedef struct {
  m2r_port_summ_t	summ ;        /* config summary info */

  cs_boolean		spi42_valid ;
  m2r_spi42_cfg_t	spi42 ;

  cs_boolean		lif_valid ;
  m2r_lif_cfg_t		lif ;


  cs_boolean		framer_valid ;
  m2r_framer_cfg_t	framer ;

  cs_boolean            eth_valid ;
  m2r_eth_cfg_t         ethernet ;

  cs_boolean		rmac_valid ;
  m2r_rmac_cfg_t	rmac ;
} m2r_port_cfg_t ;


/*************************************
 *   Port Running Configuration      *
 *************************************/ 
typedef struct {
  cs_uint32		jtag_id ;
  cs_uint16             dev_id ; 
  m2r_port_summ_t	summ ;        /* config summary info */

  cs_boolean		spi42_valid ;
  m2r_spi42_runcfg_t    spi42 ; 

  cs_boolean		lif_valid ;
  m2r_lif_runcfg_t     	lif ;


  cs_boolean		framer_valid ;
  m2r_framer_runcfg_t	framer ;

  cs_boolean            eth_valid ;
  m2r_eth_runcfg_t      eth ;

  cs_boolean		rmac_valid ;
  m2r_rmac_runcfg_t	rmac ; 
} m2r_port_runcfg_t ;


extern  
cs_uint16  m2r_reg_read(cs_uint16 port_id, cs_uint32 reg_offset) ;
extern  
cs_status  m2r_reg_write(cs_uint16 port_id, cs_uint32 reg_offset,
			 cs_uint16 data) ;  
extern  
cs_status m2r_port_init(cs_uint16 port_id, 
			m2r_port_cfg_t * pCfg) ;
extern  
cs_status m2r_port_init_warm(cs_uint16 port_id, 
			m2r_port_cfg_t * pCfg) ;
extern
cs_status m2r_port_close(cs_uint16 port_id) ;
extern  
cs_status m2r_port_get_default_cfg(cs_uint16 port_id, 
				   m2r_port_cfg_t * pCfg) ;
extern  
cs_status m2r_port_get_running_cfg(cs_uint16 port_id, 
				   m2r_port_runcfg_t * pRunCfg) ;
extern
cs_status m2r_port_dump_running_cfg(cs_uint16 port_id) ;
extern  
cs_status  m2r_set_port_mac_mode(cs_uint16 port_id,
				 m2r_port_mac_mode_t mac_mode) ;
extern  
m2r_port_mac_mode_t  m2r_get_port_mac_mode(cs_uint16 port_id) ;





#endif  /* __M2R_GEN_H__ */

