/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_stats.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for ARN5 Statistics.
 */
#ifndef __ARN5_STATS_H__
#define __ARN5_STATS_H__

#include "cs_types.h"
#include "arn5_public.h"

#include "arn5_lif_stats.h"
#include "arn5_framer_stats.h"
#include "arn5_spi_stats.h"
#include "arn5_eth_stats.h"
#include "arn5_pprbs_stats.h"


typedef   arn5_spi_dev_stats_t   arn5_host_if_dev_stats_t ;
typedef   arn5_spi_stats_t       arn5_host_if_port_stats_t ;
typedef   arn5_eth_stats_t       arn5_eth_port_stats_t ;
typedef   arn5_pprbs_stats_t     arn5_pprbs_port_stats_t;



/********************************************
 * DEVICE STATS STRUCTURE
 ********************************************/
typedef struct {
  arn5_host_if_dev_stats_t   host_if ;
} arn5_dev_stats_t ;


/********************************************
 * PORT STATS STRUCTURE
 ********************************************/
typedef struct {
  arn5_lif_stats_t           lif ;
  arn5_framer_stats_t        framer ;
  arn5_host_if_port_stats_t  host_if ;
  arn5_eth_port_stats_t      eth ;
  arn5_pprbs_port_stats_t    pprbs ;
} arn5_port_stats_t ;


/******************************************************
 *  APIs for Device Statistics
 ******************************************************/
extern
cs_status arn5_dev_print_stats(cs_uint8 dev_id) ;
extern
cs_status arn5_dev_print_blk_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id) ;
extern
cs_status arn5_dev_print_sect_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id) ;
extern
cs_status arn5_dev_print_unit_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id) ;
extern
cs_status arn5_dev_clear_stats(cs_uint8 dev_id) ;
extern
cs_status arn5_dev_clear_dir_stats(cs_uint8 dev_id, cs_dir_t dir) ;
extern
cs_status arn5_dev_clear_blk_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id) ;
extern
cs_status arn5_dev_clear_blk_dir_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_dir_t dir) ;
extern
cs_status arn5_dev_clear_sect_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id) ;
extern
cs_status arn5_dev_clear_sect_dir_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_dir_t dir) ;
extern
cs_status arn5_dev_clear_unit_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id) ;
extern
cs_status arn5_dev_clear_unit_dir_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        cs_dir_t dir) ;
extern
cs_status arn5_dev_get_stats(cs_uint8 dev_id, arn5_dev_stats_t * pStats) ;
extern
cs_status arn5_dev_get_blk_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        void * pStats, cs_uint16 len) ;
extern
cs_status arn5_dev_get_sect_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, void * pStats,
                        cs_uint16 len) ;
extern
cs_status arn5_dev_get_unit_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id,
                        arn5_stats_data_t * pUnit_Stats) ;
extern
cs_status arn5_dev_ctl_blk_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_ctl_t ctl) ;
extern
cs_status arn5_dev_ctl_sect_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_ctl_t ctl) ;
extern
cs_status arn5_dev_ctl_unit_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id, cs_ctl_t ctl) ;


/*******************************************************
 *  APIs for Port Statistics
 *******************************************************/
extern
cs_status arn5_port_print_stats(cs_uint16 port_id) ;
extern
cs_status arn5_port_print_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id) ;
extern
cs_status arn5_port_print_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id) ;
extern
cs_status arn5_port_print_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id) ;
extern
cs_status arn5_port_clear_stats(cs_uint16 port_id) ;
extern
cs_status arn5_port_clear_dir_stats(cs_uint16 port_id, cs_dir_t dir) ;
extern
cs_status arn5_port_clear_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id) ;
extern
cs_status arn5_port_clear_blk_dir_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_dir_t dir) ;
extern
cs_status arn5_port_clear_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id) ;
extern
cs_status arn5_port_clear_sect_dir_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_dir_t dir) ;
extern
cs_status arn5_port_clear_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id) ;
extern
cs_status arn5_port_clear_unit_dir_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        cs_dir_t dir) ;
extern
cs_status arn5_port_get_stats(cs_uint16 port_id,
                        arn5_port_stats_t * pStats) ;
extern
cs_status arn5_port_get_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        void * pStats, cs_uint16 len) ;
extern
cs_status arn5_port_get_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, void * pStats,
                        cs_uint16 len) ;
extern
cs_status arn5_port_get_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id,
                        arn5_stats_data_t * pUnit_Stats) ;
extern
cs_status arn5_port_ctl_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                                 cs_ctl_t ctl) ;
extern
cs_status arn5_port_ctl_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_ctl_t ctl) ;
extern
cs_status arn5_port_ctl_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id,
                        cs_ctl_t ctl) ;

#endif /* __ARN5_STATS_H__ */

