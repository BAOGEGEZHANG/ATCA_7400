/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for MILAN2 LIF Statistics.
 */
#ifndef __M2R_LIF_STATS_H__
#define __M2R_LIF_STATS_H__

/************************************************************************
 * LIF Statistics structure
 ************************************************************************/
/* Enums for unit stats in PCS6 */
typedef enum {
  M2R_LIF_PCS6_STATS_GOOD_PKT,
  M2R_LIF_PCS6_STATS_ERR_PKT,
  M2R_LIF_PCS6_STATS_DECODE_FAIL,
  M2R_LIF_PCS6_STATS_ERR_PATTERN,
  M2R_LIF_PCS6_STATS_BER_CNT,

  M2R_LIF_PCS6_STATS_MAX_CTR
} m2r_lif_pcs6_stats_unit_t ;

typedef struct {
  m2r_stats_data_t   good_pkt ;
  m2r_stats_data_t   err_pkt ;
  m2r_stats_data_t   decode_fail ;
  m2r_stats_data_t   err_pat ;
  m2r_stats_data_t   ber_cnt ;
} m2r_lif_pcs6_stats_t ;

/* Enum for unit stats in LIF XGXS */
typedef enum {
  M2R_LIF_XGXS_STATS_GOOD_PKT,
  M2R_LIF_XGXS_STATS_ERR_PKT,
  M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_0,
  M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_1,
  M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_2,
  M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_3,
  M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_0,
  M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_1,
  M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_2,
  M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_3,

  M2R_LIF_XGXS_STATS_MAX_CTR
} m2r_lif_xgxs_stats_unit_t ;

typedef struct {
  m2r_stats_data_t   good_pkt ;
  m2r_stats_data_t   err_pkt ;
  m2r_stats_data_t   inv_code_0 ;
  m2r_stats_data_t   inv_code_1 ;
  m2r_stats_data_t   inv_code_2 ;
  m2r_stats_data_t   inv_code_3 ;
  m2r_stats_data_t   pattern_err0 ;
  m2r_stats_data_t   pattern_err1 ;
  m2r_stats_data_t   pattern_err2 ;
  m2r_stats_data_t   pattern_err3 ;
} m2r_lif_xgxs_stats_t ;

typedef enum {
  M2R_LIF_STATS_SECT_PCS6,
  M2R_LIF_STATS_SECT_XGXS,

  M2R_LIF_STATS_MAX_SECTION_ID
} m2r_lif_stats_sect_t ;

typedef struct {
  union {
    m2r_lif_pcs6_stats_t pcs6 ;
    m2r_lif_xgxs_stats_t xgxs ;
  } mode ;
} m2r_lif_stats_t ;

extern cs_status  m2r_lif_init_stats(cs_uint16 port_id,
                                     m2r_lif_sel_t lif_mode,
                                     cs_boolean sonet_mode) ;

#endif /* __M2R_LIF_STATS_H__ */
