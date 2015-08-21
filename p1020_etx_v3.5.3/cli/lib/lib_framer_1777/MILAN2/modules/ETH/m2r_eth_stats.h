/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_eth_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for MILAN2 XETH Statistics.
 */
#ifndef __M2R_ETH_STATS_H__
#define __M2R_ETH_STATS_H__

#include "cs_types.h"

/************************************************************************
 * XETH Statistics unit enums
 ************************************************************************/
typedef enum {
  M2R_ETH_STATS_GOOD_PKT,
  M2R_ETH_STATS_BAD_PKT,
  M2R_ETH_STATS_DROP_PKT,
  M2R_ETH_STATS_CRC_ERR,
/* Feature 1 */

  M2R_ETH_STATS_MAX_CTR
} m2r_eth_stats_unit_t ;

/************************************************************************
 * XETH Statistics structure
 ************************************************************************/
typedef struct {
  m2r_stats_data_t   good_pkt ;
  m2r_stats_data_t   bad_pkt ;
  m2r_stats_data_t   drop_pkt ;
/* Feature 1 */
} m2r_eth_stats_t ;

extern cs_status  m2r_eth_init_stats(cs_uint16 port_id) ;

#endif /*  __M2R_ETH_STATS_H__ */
