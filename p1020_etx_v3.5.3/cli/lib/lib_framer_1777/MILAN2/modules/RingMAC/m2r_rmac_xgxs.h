/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_xgxs
 *
 * RMAC: Public structures and exported API's for applications.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2_RMAC_XGXS_H__
#define __M2_RMAC_XGXS_H__

#include "cs_types.h"
#include "m2r_xgxs.h"

/***********************************************************************
 * RMAC XGXS APIs
 ***********************************************************************/
extern
cs_status  m2r_rmac_xgxs_set_ref_clock(cs_uint16 port_id,
                                       m2r_xgxs_xaui_ref_clk refClk) ;

extern
cs_status  m2r_rmac_xgxs_get_clk_status(cs_uint16 port_id) ;

extern
cs_status  m2r_rmac_xgxs_ctl_clocks(cs_uint16 port_id, m2r_dir_t dir,
                                    m2r_ctl_t ctl) ;

extern
cs_status  m2r_rmac_xgxs_ctl_pointer_adj(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_status  m2r_rmac_xgxs_ctl_rx_tx_loopback(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_status  m2r_rmac_xgxs_ctl_tx_rx_loopback(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_status  m2r_rmac_xgxs_ctl_xaui_loopback(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_uint16 m2r_rmac_xgxs_get_status(cs_uint16 port_id, m2r_xgxs_status_t sel) ;

extern
cs_boolean m2r_rmac_xgxs_is_align_detected(cs_uint16 port_id) ;

extern
cs_status m2r_rmac_xgxs_reset_estore(cs_uint16 port_id) ;

extern
cs_status m2r_rmac_xgxs_ctl_prbs(cs_uint16 port_id, m2r_ctl_t ctl,
                                m2r_xgxs_prbs_t pattern) ;

extern
cs_status m2r_rmac_xgxs_inj_error(cs_uint16 port_id,
                                  m2r_xgxs_xaui_lane_t lane) ;

extern
cs_uint16 m2r_rmac_xgxs_get_code_errors(cs_uint16 port_id,
                                  m2r_xgxs_xaui_lane_t lane) ;

extern
cs_status m2r_rmac_xgxs_ctl_data_detect(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_status m2r_rmac_xgxs_ctl_power(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_status m2r_rmac_xgxs_cfg_prbs_reg_data(cs_uint16 port_id,
                                         m2r_xgxs_xaui_lane_t lane,
                                         cs_boolean ctrl_data, cs_uint8 data) ;

#endif /* __M2_RMAC_XGXS_H__ */

