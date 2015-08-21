/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_xgxs_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for MILAN2 RMAC XGXS Statistics.
 */
#ifndef __M2R_RMAC_XGXS_STATS_H__
#define __M2R_RMAC_XGXS_STATS_H__

typedef enum {
/*
  M2R_XGXS_STATS_GOOD_PKT,
*/
  M2R_XGXS_STATS_ERR_PKT,
  M2R_XGXS_STATS_INVALID_CODE_LANE_0,
  M2R_XGXS_STATS_INVALID_CODE_LANE_1,
  M2R_XGXS_STATS_INVALID_CODE_LANE_2,
  M2R_XGXS_STATS_INVALID_CODE_LANE_3,
  M2R_XGXS_STATS_PATTERN_ERR_LANE_0,
  M2R_XGXS_STATS_PATTERN_ERR_LANE_1,
  M2R_XGXS_STATS_PATTERN_ERR_LANE_2,
  M2R_XGXS_STATS_PATTERN_ERR_LANE_3,

  M2R_XGXS_STATS_MAX_CTR
} m2r_xgxs_stats_unit_t ;

/************************************************************************
 * RMAC XGXS Stats
 ************************************************************************/
typedef struct {
/*
  m2r_stats_data_t   good_pkt ;
*/
  m2r_stats_data_t   err_pkt ;
  m2r_stats_data_t   inv_code_0 ;
  m2r_stats_data_t   inv_code_1 ;
  m2r_stats_data_t   inv_code_2 ;
  m2r_stats_data_t   inv_code_3 ;
  m2r_stats_data_t   pattern_err0 ;
  m2r_stats_data_t   pattern_err1 ;
  m2r_stats_data_t   pattern_err2 ;
  m2r_stats_data_t   pattern_err3 ;
} m2r_xgxs_stats_t ;

extern cs_status  m2r_rmac_xgxs_init_stats(cs_uint16 port_id) ;

#endif /*  __M2R_RMAC_XGXS_STATS_H__ */
