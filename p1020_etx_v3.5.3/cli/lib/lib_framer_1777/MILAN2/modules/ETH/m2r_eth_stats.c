/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_eth_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_eth_stats.h"

/******************************************************************************
 * XETH STATS TABLES
 ******************************************************************************/
const static m2r_stats_info_t  eth_stats_tbl[M2R_ETH_STATS_MAX_CTR] = {
  CS_DI([M2R_ETH_STATS_GOOD_PKT] =) { "Good Packets",
                  M2R_ETH_REG_OFFSET(tx_GoodPkt),
                  M2R_ETH_REG_OFFSET(rx_GoodPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },
  CS_DI([M2R_ETH_STATS_BAD_PKT] =) { "Bad Packets",
                  M2R_ETH_REG_OFFSET(tx_ErrPkt),
                  M2R_ETH_REG_OFFSET(rx_ErrPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },
  CS_DI([M2R_ETH_STATS_DROP_PKT] =) { "Drop Packets",
                  0, M2R_ETH_REG_OFFSET(DropPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                }
/* Feature 1 */
} ;

const static cs_int8 * eth_stats_sect_name = "ETH Stats Section 0" ;

/****************************************************************/
/* $rtn_hdr_start   INIT XETH STATS                             */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : Both                                            */
cs_status  m2r_eth_init_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Init the XETH Statistics.                                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_port_stats_t   * p_port_stats ;
  m2r_eth_stats_t    * p_eth_stats ;

  if (m2r_stats_register_blk_cb(port_id, M2R_ID_ETH,
                                TRUE, NULL, NULL, NULL) == NULL) {
    return (CS_ERROR) ;
  }

  p_port_stats = M2R_GET_STATS_PTR(port_id) ;
  if (p_port_stats == NULL) {
    return (CS_ERROR) ;
  }
  p_eth_stats = &p_port_stats->eth ;

  if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_ETH,
				    (cs_int8*) eth_stats_sect_name,
				    0, 0, TRUE,
				    M2R_ETH_STATS_MAX_CTR,
				    (m2r_stats_data_t *) p_eth_stats,
				    eth_stats_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }


  return (CS_OK) ;
}




