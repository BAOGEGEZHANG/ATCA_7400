/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_lif_stats.h"

/******************************************************************************
 * LIF STATS TABLES
 ******************************************************************************/
const static m2r_stats_info_t  lif_pcs6_stats_tbl[M2R_LIF_PCS6_STATS_MAX_CTR] = {
  CS_DI([M2R_LIF_PCS6_STATS_GOOD_PKT] =) { "Total Good Pkts",
                  M2R_LIF_PCS_REG_OFFSET(tx_GoodPkt),
                  M2R_LIF_PCS_REG_OFFSET(rx_GoodPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_PCS6_STATS_ERR_PKT] =) { "Total Error Pkts",
                  M2R_LIF_PCS_REG_OFFSET(tx_ErrPkt),
                  M2R_LIF_PCS_REG_OFFSET(rx_ErrPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_PCS6_STATS_DECODE_FAIL] =) { "Total Decode Failures",
                  0, M2R_LIF_PCS_REG_OFFSET(ErrDec),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_RX_READ_CLR)
                },

  CS_DI([M2R_LIF_PCS6_STATS_ERR_PATTERN] =) { "Total Error Test Pattern",
                  0, M2R_LIF_PCS_REG_OFFSET(ErrPat),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_RX_READ_CLR)
                },

  CS_DI([M2R_LIF_PCS6_STATS_BER_CNT] =) { "Total BER Count",
                  0, M2R_LIF_PCS_REG_OFFSET(ErrBer),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_RX_READ_CLR)
                }
} ;

const static m2r_stats_info_t  lif_xgxs_stats_tbl[M2R_LIF_XGXS_STATS_MAX_CTR] = {
  CS_DI([M2R_LIF_XGXS_STATS_GOOD_PKT] =) { "Total Good Pkts",
                  M2R_LIF_XGXS_REG_OFFSET(tx_GoodPkt),
                  M2R_LIF_XGXS_REG_OFFSET(rx_GoodPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_ERR_PKT] =) { "Total Error Pkts",
                  M2R_LIF_XGXS_REG_OFFSET(tx_ErrPkt),
                  M2R_LIF_XGXS_REG_OFFSET(rx_ErrPkt),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_TX_VALID |
                   M2R_STATS_FLAG_TX_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_0] =) { "Lane 0 Invalid Code Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(ICC0),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_1] =) { "Lane 1 Invalid Code Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(ICC1),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_2] =) { "Lane 2 Invalid Code Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(ICC2),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_INVALID_CODE_LANE_3] =) { "Lane 3 Invalid Code Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(ICC3),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_0] =) { "Lane 0 Pattern Error Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(PEC0),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_1] =) { "Lane 1 Pattern Error Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(PEC1),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_2] =) { "Lane 2 Pattern Error Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(PEC2),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                },

  CS_DI([M2R_LIF_XGXS_STATS_PATTERN_ERR_LANE_3] =) { "Lane 3 Pattern Error Count",
                  0, M2R_LIF_XGXS_REG_OFFSET(PEC3),
                  1, /* 16 bit counter */
                  0,
                  (M2R_STATS_FLAG_RX_VALID |
                   M2R_STATS_FLAG_RX_READ_CLR )
                }
} ;


/****************************************************************/
/* $rtn_hdr_start   INIT LIF STATS                              */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_lif_init_stats(cs_uint16 port_id,
                              m2r_lif_sel_t lif_mode,
                              cs_boolean sonet_mode)
/* INPUTS     : o Port Id                                       */
/*              o LIF Mode                                      */
/*              o TRUE ( channelized Mode ) else FALSE          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Init the LIF Statistics.                                     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_port_stats_t   * p_port_stats ;
  m2r_lif_stats_t    * p_lif_stats ;

  if (sonet_mode == TRUE) {
    return (CS_OK) ; /* No Stats in SONET Mode */
  }

  if (m2r_stats_register_blk_cb(port_id, M2R_ID_LIF,
                                TRUE, NULL, NULL, NULL) == NULL) {
    return (CS_ERROR) ;
  }

  p_port_stats = M2R_GET_STATS_PTR(port_id) ;
  if (p_port_stats == NULL) {
    return (CS_ERROR) ;
  }
  p_lif_stats = &p_port_stats->lif ;

  if (lif_mode == M2R_LIF_INTERFACE_SFI42) {
    /* Register XGXS Section stats */
    if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_LIF,
		"LIF XGXS Stats",
                M2R_LIF_STATS_SECT_XGXS, 0, TRUE,
                M2R_LIF_XGXS_STATS_MAX_CTR,
                (m2r_stats_data_t *) &p_lif_stats->mode.xgxs,
                lif_xgxs_stats_tbl)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }
  else {
    /* Register PCS Section stats */
    if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_LIF,
		"LIF PCS64/66 Stats",
                M2R_LIF_STATS_SECT_PCS6, 0, TRUE,
                M2R_LIF_PCS6_STATS_MAX_CTR,
                (m2r_stats_data_t *) &p_lif_stats->mode.pcs6,
                lif_pcs6_stats_tbl)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


