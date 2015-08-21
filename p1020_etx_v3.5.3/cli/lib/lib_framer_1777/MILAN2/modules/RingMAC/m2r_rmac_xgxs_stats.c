/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_xgxs_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_rmac_xgxs_stats.h"

/******************************************************************************
 * RMAC XGXS STATS TABLES
 ******************************************************************************/
const static m2r_stats_info_t  xgxs_stats_tbl[M2R_XGXS_STATS_MAX_CTR] = {
/*
  CS_DI([M2R_XGXS_STATS_GOOD_PKT] =) { "Total Good Pkts",
                  M2R_XGXS_REG_OFFSET(tx_GoodPkt),
                  M2R_XGXS_REG_OFFSET(rx_GoodPkt),
                  1, / * 16 bit counter * /
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },
*/
  CS_DI([M2R_XGXS_STATS_ERR_PKT] =) { "Total Error Pkts",
                  M2R_XGXS_REG_OFFSET(tx_ErrPkt),
                  M2R_XGXS_REG_OFFSET(rx_ErrPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_INVALID_CODE_LANE_0] =) { "Lane 0 Invalid Code Count",
                  0, M2R_XGXS_REG_OFFSET(ICC0),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_INVALID_CODE_LANE_1] =) { "Lane 1 Invalid Code Count",
                  0, M2R_XGXS_REG_OFFSET(ICC1),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_INVALID_CODE_LANE_2] =) { "Lane 2 Invalid Code Count",
                  0, M2R_XGXS_REG_OFFSET(ICC2),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_INVALID_CODE_LANE_3] =) { "Lane 3 Invalid Code Count",
                  0, M2R_XGXS_REG_OFFSET(ICC3),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_PATTERN_ERR_LANE_0] =) { "Lane 0 Pattern Error Count",
                  0, M2R_XGXS_REG_OFFSET(PEC0),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_PATTERN_ERR_LANE_1] =) { "Lane 1 Pattern Error Count",
                  0, M2R_XGXS_REG_OFFSET(PEC1),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_PATTERN_ERR_LANE_2] =) { "Lane 2 Pattern Error Count",
                  0, M2R_XGXS_REG_OFFSET(PEC2),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_XGXS_STATS_PATTERN_ERR_LANE_3] =) { "Lane 3 Pattern Error Count",
                  0, M2R_XGXS_REG_OFFSET(PEC3),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                }
} ;

const static cs_int8 * xgxs_sect_name = "RMAC MATE XGXS Stats" ;

/****************************************************************/
/* $rtn_hdr_start   INIT XGXS STATS                             */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_xgxs_init_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Init the XGXS Statistics. This API will register with the    */
/* Framer block and its stats control tables with the stats.    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_port_stats_t   * p_port_stats ;
  m2r_xgxs_stats_t    * p_xgxs_stats ;

  if (m2r_stats_register_blk_cb(port_id, M2R_ID_XGXS,
                                TRUE, NULL, NULL, NULL) == NULL) {
      goto RTN_EXIT;
  }

  p_port_stats = M2R_GET_STATS_PTR(port_id) ;
  if (p_port_stats == NULL) {
    return (CS_ERROR) ;
  }
  p_xgxs_stats = &p_port_stats->xgxs ;

  m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_XGXS,
                (cs_int8*) xgxs_sect_name,
                0, 0, TRUE,
                M2R_XGXS_STATS_MAX_CTR,
                (m2r_stats_data_t *) p_xgxs_stats,
                xgxs_stats_tbl) ;

RTN_EXIT:
  return (CS_OK) ;
}


