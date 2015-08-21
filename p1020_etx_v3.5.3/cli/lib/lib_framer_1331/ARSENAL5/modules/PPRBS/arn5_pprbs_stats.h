/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_pprbs_stats.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for ARN5 PPRBS Statistics.
 */
#ifndef __ARN5_PPRBS_STATS_H__
#define __ARN5_PPRBS_STATS_H__

/**********************************************************************
 * PPRBS Statistics
**********************************************************************/
typedef enum {
  ARN5_PPRBS_CHAN_STATS_PKT,
  ARN5_PPRBS_CHAN_STATS_BYTES,
  ARN5_PPRBS_CHAN_STATS_ERR,
  /* all */
  ARN5_PPRBS_CHAN_STATS_MAX_CTR
} arn5_pprbs_chan_stats_unit_t ;


typedef struct {
  arn5_stats_data_t   pkts ;
  arn5_stats_data_t   bytes ;
  arn5_stats_data_t   bit_err ;
} arn5_pprbs_chan_stats_t ;


typedef enum {
  ARN5_PPRBS_STATS_LINE_SECT,
  ARN5_PPRBS_STATS_HOST_SECT,
} arn5_pprbs_stats_sect_t ;


typedef struct {
  arn5_pprbs_chan_stats_t   line ;
  arn5_pprbs_chan_stats_t   host ;
} arn5_pprbs_stats_t ;


extern
cs_status arn5_pprbs_init_stats(cs_uint16 port_id) ;

#endif /* __ARN5_PPRBS_STATS_H__ */
