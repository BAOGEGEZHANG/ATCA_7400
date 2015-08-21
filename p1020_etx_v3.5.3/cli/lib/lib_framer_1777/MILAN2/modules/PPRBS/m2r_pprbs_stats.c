/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_pprbs_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_pprbs_stats.h"

/******************************************************************************
 * PPRBS STATS TABLES
 ******************************************************************************/

/******************************************************************************
 * PPRBS CHANNEL STATS TABLE ( HOST )
 ******************************************************************************/
const m2r_stats_info_t  m2r_pprbs_chan0_stats_tbl[M2R_PPRBS_CHAN_STATS_MAX_CTR] = {
  CS_DI([M2R_PPRBS_CHAN_STATS_PKT] =) { "Total Pkts",
                              M2R_PPRBS_REG_OFST(GenPacketMSB0),
                              M2R_PPRBS_REG_OFST(ChkPacketMSB0),
                              2, 0,
                              (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                            },

  CS_DI([M2R_PPRBS_CHAN_STATS_BYTES] =) { "Total Bytes",
                              M2R_PPRBS_REG_OFST(GenByteMSB0),
                              M2R_PPRBS_REG_OFST(ChkByteMSB0),
                              2, 0,
                              (M2R_STATS_FLAG_RX_VALID |
			       M2R_STATS_FLAG_TX_VALID)
                            },

  CS_DI([M2R_PPRBS_CHAN_STATS_ERR] =) { "Total Errors",
                              0, M2R_PPRBS_REG_OFST(ChkErr0),
                              1, 0, (M2R_STATS_FLAG_RX_VALID)
                            }
} ;

/******************************************************************************
 * PPRBS CHANNEL STATS TABLE ( LINE )
 ******************************************************************************/
const m2r_stats_info_t  m2r_pprbs_chan1_stats_tbl[M2R_PPRBS_CHAN_STATS_MAX_CTR] = {
  CS_DI([M2R_PPRBS_CHAN_STATS_PKT] =) { "Total Pkts",
                              M2R_PPRBS_REG_OFST(GenPacketMSB1),
                              M2R_PPRBS_REG_OFST(ChkPacketMSB1),
                              2, 0,
                              (M2R_STATS_FLAG_RX_VALID |
			       M2R_STATS_FLAG_TX_VALID)
                            },

  CS_DI([M2R_PPRBS_CHAN_STATS_BYTES] =) { "Total Bytes",
                              M2R_PPRBS_REG_OFST(GenByteMSB1),
                              M2R_PPRBS_REG_OFST(ChkByteMSB1),
                              2, 0,
                              (M2R_STATS_FLAG_RX_VALID |
			       M2R_STATS_FLAG_TX_VALID)
                            },

  CS_DI([M2R_PPRBS_CHAN_STATS_ERR] =) { "Total Errors",
                              0, M2R_PPRBS_REG_OFST(ChkErr1),
                              1, 0, (M2R_STATS_FLAG_RX_VALID)
                            }
} ;

/****************************************************************/
/* $rtn_hdr_start   INIT PPRBS STATS                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_pprbs_init_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Init the PPRBS Statistics. This API will register with the   */
/* Framer block and its stats control tables with the stats.    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_stats_blk_cb_t * pprbs_stats_blk_cb ;
  m2r_port_cb_t      * ppcb = NULL ;
  cs_uint32           error;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ": port_id= %d\n", port_id);
    return (CS_ERROR) ;
  }

  /* Register Framer Stats */
  pprbs_stats_blk_cb = m2r_stats_register_blk_cb(port_id,
                             M2R_ID_PPRBS, TRUE, NULL, NULL, NULL) ;

  if (pprbs_stats_blk_cb == NULL) {
    return (CS_ERROR) ;
  }

  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_PPRBS,
				    "PPRBS Line Channel Stats",
				    M2R_PPRBS_STATS_LINE_SECT,
				    0, TRUE,
				    M2R_PPRBS_CHAN_STATS_MAX_CTR,
				    (m2r_stats_data_t *)
				    (&ppcb->stats.pprbs.line),
				    m2r_pprbs_chan1_stats_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_PPRBS,
				    "PPRBS Host Channel Stats",
				    M2R_PPRBS_STATS_HOST_SECT,
				    0, TRUE,
				    M2R_PPRBS_CHAN_STATS_MAX_CTR,
				    (m2r_stats_data_t *)
				    (&ppcb->stats.pprbs.host),
				    m2r_pprbs_chan0_stats_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}



