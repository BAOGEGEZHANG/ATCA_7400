/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for MILAN2 Statistics.
 */
#ifndef __M2R_STATS_H__
#define __M2R_STATS_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_spi42_stats.h"
#include "m2r_lif_stats.h"
#include "m2r_pprbs_stats.h"
#include "m2r_framer_stats.h"
#include "m2r_rmac_stats.h"
#include "m2r_eth_stats.h"
#include "m2r_rmac_xgxs_stats.h"

/************************************************************************
 * PORT STATS STRUCTURE
 ************************************************************************/
typedef struct {
  m2r_spi42_stats_t	spi42 ;
  m2r_lif_stats_t	lif ; 
  m2r_pprbs_stats_t	pprbs ;
  m2r_framer_stats_t	framer ;
  m2r_rmac_stats_t	rmac ;
  m2r_eth_stats_t       eth ;
  m2r_xgxs_stats_t	xgxs ;
} m2r_port_stats_t ;

/************************************************************************
 * Public APIs for Statistics
 ************************************************************************/
extern cs_status m2r_get_stats(cs_uint16 port_id, m2r_port_stats_t * pStats) ;

extern cs_status m2r_get_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                   void * pStats, cs_uint16 len) ;

extern cs_status m2r_get_sect_stats(cs_uint16 port_id,
                                    m2r_module_id_t mod_id,
                                    cs_uint16 sect_id,
                                    void * pStats, cs_uint16 len) ;

extern cs_status m2r_get_unit_stats(cs_uint16 port_id,
                                    m2r_module_id_t mod_id,
                                    cs_uint16 sect_id,
                                    cs_uint16 unit_id,
                                    m2r_stats_data_t * pUnit_Stats) ;

extern cs_status m2r_print_stats(cs_uint16 port_id) ;

extern cs_status m2r_print_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id) ;

extern cs_status m2r_print_sect_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                      cs_uint16 sect_id) ;

extern cs_status m2r_print_unit_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                 cs_uint16 sect_id, cs_uint16 unit_id) ;

/* Clear APIs */
extern cs_status m2r_clear_dir_stats(cs_uint16 port_id, m2r_dir_t dir) ;

extern cs_status m2r_clear_blk_dir_stats(cs_uint16 port_id,
                                         m2r_module_id_t mod_id,
                                         m2r_dir_t dir) ;

extern cs_status m2r_clear_sect_dir_stats(cs_uint16 port_id,
                                          m2r_module_id_t mod_id,
                                          cs_uint16 sect_id,
                                          m2r_dir_t dir) ;

extern cs_status m2r_clear_unit_dir_stats(cs_uint16 port_id,
                                          m2r_module_id_t mod_id,
                                          cs_uint16 sect_id,
                                          cs_uint16 unit_id,
                                          m2r_dir_t dir) ;

extern cs_status m2r_clear_stats(cs_uint16 port_id) ;

extern cs_status m2r_clear_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id) ;

extern cs_status m2r_clear_sect_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                      cs_uint16 sect_id) ;

extern cs_status m2r_clear_unit_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                 cs_uint16 sect_id, cs_uint16 unit_id) ;

extern cs_status m2r_ctl_stats(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern cs_status m2r_ctl_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                   m2r_ctl_t ctl) ;

extern cs_status m2r_ctl_sect_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                      cs_uint16 sect_id, m2r_ctl_t ctl) ;

extern cs_status m2r_ctl_unit_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                    cs_uint16 sect_id, cs_uint16 unit_id,
                                    m2r_ctl_t ctl) ;
extern
cs_status m2r_load_stats(cs_uint16 port_id, m2r_port_stats_t * pStats) ;

#endif /* __M2R_STATS_H__ */


