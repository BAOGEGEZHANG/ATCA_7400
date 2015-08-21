/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for Framer Statistics.
 */
#ifndef __M2R_FRAMER_STATS_H__
#define __M2R_FRAMER_STATS_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_framer.h"

/*****************************************************************
 * Framer Packet Mode Statistics structures and enums
 *****************************************************************/
/* Enums for unit stats of Stream Section in Packet Mode */
typedef enum {
  M2R_FR_PKT_STATS_GOOD_PKT,
  M2R_FR_PKT_STATS_BYTES,
  M2R_FR_PKT_STATS_GOOD_BYTES,
  M2R_FR_PKT_STATS_SHORT_PKT,
  M2R_FR_PKT_STATS_LONG_PKT,
  M2R_FR_PKT_STATS_ABORT_PKT,
  M2R_FR_PKT_STATS_FCS_ERR_PKT,
  M2R_FR_PKT_STATS_DISCARDED_PKT,
  M2R_FR_PKT_STATS_DROP_PKT,
  M2R_FR_PKT_STATS_INVALID_PKT,

  M2R_FR_PKT_STATS_MAX_CTR
} m2r_framer_pkt_stats_unit_t ;

/* Stream section stats for framer in Packet Mode */
typedef struct {
  m2r_stats_data_t   good_pkts ;
  m2r_stats_data_t   bytes_stats ;
  m2r_stats_data_t   good_bytes ;
  m2r_stats_data_t   short_pkts ;
  m2r_stats_data_t   long_pkts ;
  m2r_stats_data_t   abort_pkts ;
  m2r_stats_data_t   fcs_error ;
  m2r_stats_data_t   drop_abrt ;
  m2r_stats_data_t   drop ;
  m2r_stats_data_t   invalid_pkts ;
} m2r_framer_pkt_stats_t ;


/* Enums for GFP Stream */
typedef enum {
  M2R_FR_GFP_STATS_GOOD_PKT,
  M2R_FR_GFP_STATS_GOOD_BYTES,
  M2R_FR_GFP_STATS_ERR_PKT,
  M2R_FR_GFP_STATS_MULTI_ERR_PKT,
  M2R_FR_GFP_STATS_SINGLE_ERR_PKT,
  M2R_FR_GFP_STATS_DISCARDED_PKT,
  M2R_FR_GFP_STATS_SHORT_PKT,
  M2R_FR_GFP_STATS_LONG_PKT,
  M2R_FR_GFP_STATS_OVRSIZE_PKT,
  M2R_FR_GFP_STATS_DROP_PKT,
  M2R_FR_GFP_STATS_ERR_CORRECTED,
  M2R_FR_GFP_STATS_IDLE_PKT,

  M2R_FR_GFP_STATS_MAX_CTR
} m2r_framer_gfp_stats_unit_t ;

typedef struct {
  m2r_stats_data_t   good_pkts ;
  m2r_stats_data_t   good_bytes ;
  m2r_stats_data_t   error_pkts ;
  m2r_stats_data_t   multi_hec_err ;
  m2r_stats_data_t   single_hec_err ;
  m2r_stats_data_t   discarded ;
  m2r_stats_data_t   short_pkts ;
  m2r_stats_data_t   long_pkts ;
  m2r_stats_data_t   oversize_pkts ;
  m2r_stats_data_t   drop ;
  m2r_stats_data_t   err_corrected ;
  m2r_stats_data_t   idle_pkts ;
} m2r_framer_gfp_stats_t ;


/* Enums and ATM stats struct */
typedef enum {
  M2R_FR_ATM_STATS_CELL,
  M2R_FR_ATM_STATS_IDLE_CELL,
  M2R_FR_ATM_STATS_ERR_CORRECTED,
  M2R_FR_ATM_STATS_ERR,
  M2R_FR_ATM_STATS_DROP_OVERFLOW,
  M2R_FR_ATM_STATS_OCD,
  M2R_FR_ATM_STATS_DROP_ENBL,

  M2R_FR_ATM_STATS_MAX_CTR
} m2r_framer_atm_stats_unit_t ;

typedef struct {
  m2r_stats_data_t   good_cells ;
  m2r_stats_data_t   idle_cells ;
  m2r_stats_data_t   err_cells ;
  m2r_stats_data_t   err_drop ;
  m2r_stats_data_t   drop_overflow ;
  m2r_stats_data_t   ocd ;
  m2r_stats_data_t   drop_enbl ;
} m2r_framer_atm_stats_t ;


typedef union {
  m2r_framer_pkt_stats_t  pkt ;
  m2r_framer_gfp_stats_t  gfp ;
  m2r_framer_atm_stats_t  atm ;
} m2r_framer_stream_stats ;


/* Enums and stats section struct for SONET Section */
typedef enum {
  M2R_FR_SONET_SECT_STATS_B1_ERR,

  M2R_FR_SONET_SECT_STATS_MAX_CTR
} m2r_framer_sonet_sect_stats_unit_t ;

typedef struct {
  m2r_stats_data_t   b1_err ;
} m2r_framer_sonet_sect_stats_t ;

/* Enums and stats section struct for SONET Line overhead */
typedef enum {
  M2R_FR_SONET_LINE_STATS_B2_ERR,
  M2R_FR_SONET_LINE_STATS_REI,

  M2R_FR_SONET_LINE_STATS_MAX_CTR
} m2r_framer_sonet_line_stats_unit_t ;

typedef struct {
  m2r_stats_data_t   b2_err ;
  m2r_stats_data_t   rei_cnt ;
} m2r_framer_sonet_line_stats_t ;

typedef enum {
  M2R_FR_SONET_PATH_STATS_B3_ERR,
  M2R_FR_SONET_PATH_STATS_REI_CNT,
  M2R_FR_SONET_PATH_STATS_NEG_PTR,
  M2R_FR_SONET_PATH_STATS_POS_PTR,
  M2R_FR_SONET_PATH_STATS_NEW_PTR,

  M2R_FR_SONET_PATH_STATS_MAX_CTR
} m2r_framer_sonet_path_stats_unit_t ;

typedef struct {
  m2r_stats_data_t   b3_err ;
  m2r_stats_data_t   rei_cnt ;
  m2r_stats_data_t   pntr_neg ;
  m2r_stats_data_t   pntr_pos ;
  m2r_stats_data_t   new_ptr ;
} m2r_framer_sonet_path_stats_t ;

/*****************************************************************
 * Framer Block Statistics structure
 *****************************************************************/
/* Section ID enums for Framer Block Stats */
typedef enum {
  M2R_FR_STATS_SECT_OVRHD,
  M2R_FR_STATS_LINE_OVRHD,

  M2R_FR_STATS_PATH0_OVRHD,
  M2R_FR_STATS_PATH1_OVRHD,
  M2R_FR_STATS_PATH2_OVRHD,
  M2R_FR_STATS_PATH3_OVRHD,

  M2R_FR_STATS_STR0_PKT_MODE,
  M2R_FR_STATS_STR1_PKT_MODE,
  M2R_FR_STATS_STR2_PKT_MODE,
  M2R_FR_STATS_STR3_PKT_MODE,

  M2R_FR_STATS_STR0_ATM_MODE,
  M2R_FR_STATS_STR1_ATM_MODE,
  M2R_FR_STATS_STR2_ATM_MODE,
  M2R_FR_STATS_STR3_ATM_MODE,

  M2R_FR_STATS_STR0_GFP_MODE,

  M2R_FR_STATS_MAX_SECTION_ID
} m2r_framer_stats_sect_t ;

/* Stats structure for Framer block */
typedef struct {
  m2r_framer_sonet_sect_stats_t section_ovrhd ;
  m2r_framer_sonet_line_stats_t line_ovrhd ;
  m2r_framer_sonet_path_stats_t path_0 ;
  m2r_framer_sonet_path_stats_t path_1 ;
  m2r_framer_sonet_path_stats_t path_2 ;
  m2r_framer_sonet_path_stats_t path_3 ;

  m2r_framer_stream_stats       strm0 ;
  m2r_framer_stream_stats       strm1 ;
  m2r_framer_stream_stats       strm2 ;
  m2r_framer_stream_stats       strm3 ;
} m2r_framer_stats_t ;

extern
cs_status  m2r_framer_init_stats(cs_uint16 port_id,
                                 m2r_framer_stream_t channel,
                                 m2r_framer_svc_type_t svc) ;
extern
cs_status m2r_framer_init_strm_stats(cs_uint16 port_id,
                                     m2r_framer_stream_t channel,
                                     m2r_framer_svc_type_t svc) ;

#endif /* __M2R_FRAMER_STATS_H__ */
