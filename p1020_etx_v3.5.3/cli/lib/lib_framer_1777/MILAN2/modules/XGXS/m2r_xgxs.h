/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_xgxs
 *
 * XGXS: Public structures and exported API's for applications.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2_XGXS_H__
#define __M2_XGXS_H__

#include "cs_types.h"

#include "m2r_registers.h"

/************************************************************************
 * XAUI LANE
 ************************************************************************/
typedef enum {
 M2R_XGXS_XAUI_LANE0     = 0,
 M2R_XGXS_XAUI_LANE1,
 M2R_XGXS_XAUI_LANE2,
 M2R_XGXS_XAUI_LANE3,
 M2R_XGXS_XAUI_LANE_ALL
} m2r_xgxs_xaui_lane_t ;

/************************************************************************
 * XAUI REFERENCE CLOCK FOR XAUI INTERFACE
 ************************************************************************/
typedef enum {
  M2R_XGXS_XAUI_REF_CLK_156, /*  156.25 MHz */
  M2R_XGXS_XAUI_REF_CLK_312, /*  312.5  MHz */
  M2R_XGXS_XAUI_REF_CLK_625  /*  625    MHz */
} m2r_xgxs_xaui_ref_clk ;

/************************************************************************
 * XGXS PRBS PATTERNS
 ************************************************************************/
typedef enum {
  M2R_XGXS_HFPAT,
  M2R_XGXS_LFPAT,
  M2R_XGXS_MFPAT,
  M2R_XGXS_CRPAT_0,     /* CRPAT with PRBS pattern (x**7 + x**3 + 1) */
  M2R_XGXS_CRPAT_1,     /* CRPAT with PRBS pattern (x**7 + x**6 + 1) */
  M2R_XGXS_CJPAT,
  M2R_XGXS_REG_PATTERN
} m2r_xgxs_prbs_t ;

/************************************************************************
 * XGXS STATUS
 ************************************************************************/
typedef enum {
  M2R_XGXS_LANE_LOCK_DET,
  M2R_XGXS_LANE_DATA_DET,
  M2R_XGXS_LANE_CLOCK,
  M2R_XGXS_LANE_SYNC_DET
} m2r_xgxs_status_t ;

/************************************************************************
 * XGXS API
 ************************************************************************/
extern  cs_status  m2r_xgxs_set_ref_clock(cs_uint16 port_id, cs_uint16 modId,
                               XGXS_t * xgxs, m2r_xgxs_xaui_ref_clk refClk) ;

extern  cs_status  m2r_xgxs_ctl_clocks(cs_uint16 port_id, cs_uint16 modId,
                               XGXS_t * xgxs, m2r_dir_t dir, m2r_ctl_t ctl) ;

extern  cs_status  m2r_xgxs_ctl_pointer_adj(cs_uint16 port_id, cs_uint16 modId,
                                    XGXS_t * xgxs, m2r_ctl_t ctl) ;

extern  cs_status  m2r_xgxs_ctl_rx_tx_loopback(cs_uint16 port_id,
                        cs_uint16 modId, XGXS_t * xgxs, cs_uint8 ctl) ;

extern  cs_status  m2r_xgxs_ctl_tx_rx_loopback(cs_uint16 port_id,
                        cs_uint16 modId, XGXS_t * xgxs, cs_uint8 ctl) ;

extern  cs_status m2r_xgxs_ctl_xaui_loopback(cs_uint16 port_id, cs_uint16 modId,
                                     XGXS_t *xgxs, m2r_ctl_t ctl) ;

extern  cs_status  m2r_xgxs_get_clk_status(cs_uint16 port_id,
                                      cs_uint16 modId, XGXS_t * xgxs) ;

extern  cs_uint16 m2r_xgxs_get_status(cs_uint16 port_id,
                cs_uint16 modId, XGXS_t * xgxs, m2r_xgxs_status_t sel) ;

extern
cs_boolean m2r_xgxs_is_align_detected(cs_uint16 port_id, cs_uint16 modId,
				      XGXS_t * xgxs) ;

extern  cs_status m2r_xgxs_reset_estore(cs_uint16 port_id,
                                      cs_uint16 modId, XGXS_t * xgxs) ;

extern  cs_status m2r_xgxs_ctl_prbs(cs_uint16 port_id, cs_uint16 modId,
                            XGXS_t * xgxs, m2r_xgxs_xaui_lane_t lane,
                            m2r_xgxs_prbs_t pattern, m2r_ctl_t ctl) ;

extern  cs_status m2r_xgxs_inj_error(cs_uint16 port_id, cs_uint16 modId,
                                  XGXS_t *xgxs, m2r_xgxs_xaui_lane_t lane) ;

extern  cs_uint16 m2r_xgxs_get_code_err(cs_uint16 port_id, cs_uint16 modId,
                             XGXS_t *xgxs, m2r_xgxs_xaui_lane_t lane) ;

extern  cs_status m2r_xgxs_ctl_ddsa(cs_uint16 port_id, cs_uint16 modId,
                            XGXS_t *xgxs, m2r_ctl_t ctl) ;

extern  cs_status m2r_xgxs_ctl_data_det_for_lock_det(cs_uint16 port_id,
                            cs_uint16 modId, XGXS_t *xgxs, m2r_ctl_t ctl) ;

extern  cs_status m2r_xgxs_ctl_power(cs_uint16 port_id, cs_uint16 modId,
                             XGXS_t *xgxs, m2r_ctl_t ctl) ;

extern  cs_status m2r_xgxs_cfg_prbs_reg_data(cs_uint16 port_id,
                                         cs_uint16 modId,
                                         XGXS_t *xgxs,
                                         m2r_xgxs_xaui_lane_t lane,
                                         cs_boolean control_data,
                                         cs_uint8 reg_pattern) ;

#endif /* __M2_XGXS_H__ */


