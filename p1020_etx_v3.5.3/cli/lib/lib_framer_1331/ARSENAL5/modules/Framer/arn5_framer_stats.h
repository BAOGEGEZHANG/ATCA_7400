/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_stats.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for Framer Statistics.
 */
#ifndef __ARN5_FRAMER_STATS_H__
#define __ARN5_FRAMER_STATS_H__

#include "cs_types.h"
#include "arn5_public.h"
#include "arn5_framer.h"

/*****************************************************************
 * Framer Packet Mode Statistics structures and enums
 *****************************************************************/
/* Enums for unit stats of Stream Section in Packet Mode */
typedef enum {
  ARN5_FR_PKT_STATS_GOOD_PKT,
  ARN5_FR_PKT_STATS_BYTES,
  ARN5_FR_PKT_STATS_GOOD_BYTES,
  ARN5_FR_PKT_STATS_SHORT_PKT,
  ARN5_FR_PKT_STATS_LONG_PKT,
  ARN5_FR_PKT_STATS_ABORT_PKT,
  ARN5_FR_PKT_STATS_FCS_ERR_PKT,
  ARN5_FR_PKT_STATS_DISCARDED_PKT,
  ARN5_FR_PKT_STATS_DROP_PKT,
  ARN5_FR_PKT_STATS_INVALID_PKT,
  ARN5_FR_PKT_STATS_X86_ADAPTATION_BYTES,

  ARN5_FR_PKT_STATS_MAX_CTR
} arn5_framer_pkt_stats_unit_t ;

/* Stream section stats for framer in Packet Mode */
typedef struct {
  arn5_stats_data_t   good_pkts ;
  arn5_stats_data_t   bytes_stats ;
  arn5_stats_data_t   good_bytes ;
  arn5_stats_data_t   short_pkts ;
  arn5_stats_data_t   long_pkts ;
  arn5_stats_data_t   abort_pkts ;
  arn5_stats_data_t   fcs_error ;
  arn5_stats_data_t   drop_abrt ;
  arn5_stats_data_t   drop ;
  arn5_stats_data_t   invalid_pkts ;
  arn5_stats_data_t   x86_adaptation_bytes ;
} arn5_framer_pkt_stats_t ;


/* Enums for GFP Stream */
typedef enum {
  ARN5_FR_GFP_STATS_GOOD_PKT,
  ARN5_FR_GFP_STATS_GOOD_BYTES,
  ARN5_FR_GFP_STATS_ERR_PKT,
  ARN5_FR_GFP_STATS_MULTI_ERR_PKT,
  ARN5_FR_GFP_STATS_SINGLE_ERR_PKT,
  ARN5_FR_GFP_STATS_DISCARDED_PKT,
  ARN5_FR_GFP_STATS_SHORT_PKT,
  ARN5_FR_GFP_STATS_LONG_PKT,
  ARN5_FR_GFP_STATS_OVRSIZE_PKT,
  ARN5_FR_GFP_STATS_DROP_PKT,
  ARN5_FR_GFP_STATS_ERR_CORRECTED,
  ARN5_FR_GFP_STATS_IDLE_PKT,
  ARN5_FR_GFP_STATS_THEC_ERR_PKT,

  ARN5_FR_GFP_STATS_MAX_CTR
} arn5_framer_gfp_stats_unit_t ;

typedef struct {
  arn5_stats_data_t   good_pkts ;
  arn5_stats_data_t   good_bytes ;
  arn5_stats_data_t   error_pkts ;
  arn5_stats_data_t   multi_hec_err ;
  arn5_stats_data_t   single_hec_err ;
  arn5_stats_data_t   discarded ;
  arn5_stats_data_t   short_pkts ;
  arn5_stats_data_t   long_pkts ;
  arn5_stats_data_t   oversize_pkts ;
  arn5_stats_data_t   drop ;
  arn5_stats_data_t   err_corrected ;
  arn5_stats_data_t   idle_pkts ;
  arn5_stats_data_t   tHEC_err_pkts ;
} arn5_framer_gfp_stats_t ;


/* Enums and ATM stats struct */
typedef enum {
  ARN5_FR_ATM_STATS_CELL,
  ARN5_FR_ATM_STATS_IDLE_CELL,
  ARN5_FR_ATM_STATS_ERR_CORRECTED,
  ARN5_FR_ATM_STATS_ERR,
  ARN5_FR_ATM_STATS_DROP_OVERFLOW,
  ARN5_FR_ATM_STATS_OCD,
  ARN5_FR_ATM_STATS_DROP_ENBL,

  ARN5_FR_ATM_STATS_MAX_CTR
} arn5_framer_atm_stats_unit_t ;

typedef struct {
  arn5_stats_data_t   good_cells ;
  arn5_stats_data_t   idle_cells ;
  arn5_stats_data_t   err_cells ;
  arn5_stats_data_t   err_drop ;
  arn5_stats_data_t   drop_overflow ;
  arn5_stats_data_t   ocd ;
  arn5_stats_data_t   drop_enbl ;
} arn5_framer_atm_stats_t ;


typedef union {
  arn5_framer_pkt_stats_t  pkt ;
  arn5_framer_gfp_stats_t  gfp ;
  arn5_framer_atm_stats_t  atm ;
} arn5_framer_stream_stats ;


/* Enums and stats section struct for SONET Section */
typedef enum {
  ARN5_FR_SONET_SECT_STATS_B1_ERR,

  ARN5_FR_SONET_SECT_STATS_MAX_CTR
} arn5_framer_sonet_sect_stats_unit_t ;

typedef struct {
  arn5_stats_data_t   b1_err ;
} arn5_framer_sonet_sect_stats_t ;

/* Enums and stats section struct for SONET Line overhead */
typedef enum {
  ARN5_FR_SONET_LINE_STATS_B2_ERR,
  ARN5_FR_SONET_LINE_STATS_REI,

  ARN5_FR_SONET_LINE_STATS_MAX_CTR
} arn5_framer_sonet_line_stats_unit_t ;

typedef struct {
  arn5_stats_data_t   b2_err ;
  arn5_stats_data_t   rei_cnt ;
} arn5_framer_sonet_line_stats_t ;

typedef enum {
  ARN5_FR_SONET_PATH_STATS_B3_ERR,
  ARN5_FR_SONET_PATH_STATS_REI_CNT,
  ARN5_FR_SONET_PATH_STATS_NEG_PTR,
  ARN5_FR_SONET_PATH_STATS_POS_PTR,
  ARN5_FR_SONET_PATH_STATS_NEW_PTR,

  ARN5_FR_SONET_PATH_STATS_MAX_CTR
} arn5_framer_sonet_path_stats_unit_t ;

typedef struct {
  arn5_stats_data_t   b3_err ;
  arn5_stats_data_t   rei_cnt ;
  arn5_stats_data_t   pntr_neg ;
  arn5_stats_data_t   pntr_pos ;
  arn5_stats_data_t   new_ptr ;
} arn5_framer_sonet_path_stats_t ;

/*****************************************************************
 * Framer Block Statistics structure
 *****************************************************************/
/* Section ID enums for Framer Block Stats */
typedef enum {
  ARN5_FR_STATS_SECT_OVRHD,
  ARN5_FR_STATS_LINE_OVRHD,
  ARN5_FR_STATS_PATH_OVRHD,
  ARN5_FR_STATS_PKT_MODE,
  ARN5_FR_STATS_ATM_MODE,
  ARN5_FR_STATS_GFP_MODE,
  ARN5_FR_STATS_MAX_SECTION_ID
} arn5_framer_stats_sect_t ;

/* Stats structure for Framer block */
typedef struct {
  arn5_framer_sonet_sect_stats_t section_ovrhd ;
  arn5_framer_sonet_line_stats_t line_ovrhd ;
  arn5_framer_sonet_path_stats_t path ;
  arn5_framer_stream_stats       strm ;
} arn5_framer_stats_t ;


#endif /* __ARN5_FRAMER_STATS_H__ */
