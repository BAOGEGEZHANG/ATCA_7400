/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_pprbs_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for PPRBS Statistics.
 */
#ifndef __M2R_PPRBS_STATS_H__
#define __M2R_PPRBS_STATS_H__

/**********************************************************************
 * PPRBS Statistics
**********************************************************************/
typedef enum {
  M2R_PPRBS_CHAN_STATS_PKT,
  M2R_PPRBS_CHAN_STATS_BYTES,
  M2R_PPRBS_CHAN_STATS_ERR,

  M2R_PPRBS_CHAN_STATS_MAX_CTR
} m2r_pprbs_chan_stats_unit_t ;


typedef struct {
  m2r_stats_data_t   pkts ;
  m2r_stats_data_t   bytes ;
  m2r_stats_data_t   bit_err ;
} m2r_pprbs_chan_stats_t ;

typedef enum {
  M2R_PPRBS_STATS_LINE_SECT,
  M2R_PPRBS_STATS_HOST_SECT
} m2r_pprbs_stats_sect_t ;

typedef struct {
  m2r_pprbs_chan_stats_t line ;
  m2r_pprbs_chan_stats_t host ;
} m2r_pprbs_stats_t ;

extern cs_status m2r_pprbs_init_stats(cs_uint16 port_id) ;

#endif /* __M2R_PPRBS_STATS_H__ */
