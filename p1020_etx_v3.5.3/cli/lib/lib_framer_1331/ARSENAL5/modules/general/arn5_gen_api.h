/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_gen.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_GEN_API_H__
#define __ARN5_GEN_API_H__

#include "cs_types.h"
#include "arn5_public.h"
#include "arn5_spi_data.h"
#include "arn5_gen_data.h"


/*******************************************************
 *                  Driver Externs                     *
 *******************************************************/
extern
cs_status  arn5_drvr_load(void) ;
extern
cs_status  arn5_drvr_unload(void) ;
extern
cs_status  arn5_start_output_to_file(char * fname) ;
extern
cs_status  arn5_stop_driver_output_to_file(void) ;
extern
void  arn5_stats_hex_format_ctl(cs_ctl_t hex_ctl) ;
extern
void  arn5_stats_mask_zero_cntr_ctl(cs_ctl_t mask_zero_ctl) ;
extern
void  arn5_esc_code_print_ctl(cs_ctl_t ctl) ;
extern
void  arn5_print_drvr_ver() ;
extern
void  arn5_print_drvr_summary() ;


/*******************************************************
 *                 Device Externs                      *
 *******************************************************/
extern
cs_status  arn5_dev_register(cs_uint8 dev_id,
                        cs_uint32 base_addr) ;
extern
cs_status  arn5_dev_unregister(cs_uint8 dev_id) ;
extern
cs_status  arn5_dev_init(cs_uint8 dev_id, arn5_dev_cfg_t * pDevCfg) ;
extern
cs_status  arn5_dev_get_default_cfg(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg) ;
extern
cs_status  arn5_dev_get_running_cfg(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg) ;
extern
cs_status  arn5_dev_dump_running_cfg(cs_uint8 dev_id) ;
extern
cs_status  arn5_dev_dump_vital_status(cs_uint8 dev_id) ;
extern
void  arn5_dump_dev_chan_map(cs_uint8 dev_id) ;


/*******************************************************
 *                 Port Externs                        *
 *******************************************************/
extern
cs_uint16 arn5_port_open(cs_uint8 dev_id, arn5_port_cfg_t * pCfg,
                        cs_uint8 slice_num) ;
extern
cs_status arn5_port_close(cs_uint16 port_id) ;
extern
cs_status arn5_port_init(cs_uint16 port_id, arn5_port_cfg_t * pCfg) ;
extern
cs_status arn5_port_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;
extern
cs_status arn5_port_dump_running_cfg(cs_uint16 port_id) ;
extern
cs_status arn5_port_dump_vital_status(cs_uint16 port_id) ;

#endif  /* __ARN5_GEN_H__ */

