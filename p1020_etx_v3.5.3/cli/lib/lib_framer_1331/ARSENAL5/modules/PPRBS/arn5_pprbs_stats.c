/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_pprbs_stats.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_stats_priv.h"
#include "arn5_pprbs_stats.h"

/******************************************************************************
 * PPRBS STATS TABLES
 ******************************************************************************/

/******************************************************************************
 * PPRBS CHANNEL STATS TABLE ( HOST )
 ******************************************************************************/
const arn5_stats_info_t  arn5_pprbs_chan0_stats_tbl[ARN5_PPRBS_CHAN_STATS_MAX_CTR] = {
  [ARN5_PPRBS_CHAN_STATS_PKT] = {
    "Total Pkts",
    ARN5_PPRBS_REG_OFST(GenPacketMSB0),
    ARN5_PPRBS_REG_OFST(ChkPacketMSB0),
    2, 0,
    (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
  },

  [ARN5_PPRBS_CHAN_STATS_BYTES] = {
    "Total Bytes",
    ARN5_PPRBS_REG_OFST(GenByteMSB0),
    ARN5_PPRBS_REG_OFST(ChkByteMSB0),
    2, 0,
    (ARN5_STATS_FLAG_RX_VALID |
     ARN5_STATS_FLAG_TX_VALID)
  },

  [ARN5_PPRBS_CHAN_STATS_ERR] = {
    "Total Errors",
    0, ARN5_PPRBS_REG_OFST(ChkErr0),
    1, 0, (ARN5_STATS_FLAG_RX_VALID)
  },
} ;


/******************************************************************************
 * PPRBS CHANNEL STATS TABLE ( LINE )
 ******************************************************************************/
const arn5_stats_info_t  arn5_pprbs_chan1_stats_tbl[ARN5_PPRBS_CHAN_STATS_MAX_CTR] = {
  [ARN5_PPRBS_CHAN_STATS_PKT] = {
    "Total Pkts",
    ARN5_PPRBS_REG_OFST(GenPacketMSB1),
    ARN5_PPRBS_REG_OFST(ChkPacketMSB1),
    2, 0,
    (ARN5_STATS_FLAG_RX_VALID |
     ARN5_STATS_FLAG_TX_VALID)
  },

  [ARN5_PPRBS_CHAN_STATS_BYTES] = {
    "Total Bytes",
    ARN5_PPRBS_REG_OFST(GenByteMSB1),
    ARN5_PPRBS_REG_OFST(ChkByteMSB1),
    2, 0,
    (ARN5_STATS_FLAG_RX_VALID |
     ARN5_STATS_FLAG_TX_VALID)
  },

  [ARN5_PPRBS_CHAN_STATS_ERR] = {
    "Total Errors",
    0, ARN5_PPRBS_REG_OFST(ChkErr1),
    1, 0, (ARN5_STATS_FLAG_RX_VALID)
  }
} ;


/****************************************************************/
/* $rtn_hdr_start   INIT PPRBS STATS                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_pprbs_init_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Init the PPRBS Statistics. This API will register with the   */
/* Framer block and its stats control tables with the stats.    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  arn5_stats_blk_cb_t * pprbs_stats_blk_cb ;
  arn5_port_cb_t      * ppcb = NULL ;
  cs_uint8             portNum ;

  /* Register Framer Stats */
  pprbs_stats_blk_cb = arn5_stats_register_blk_cb(port_id, ARN5_PORT,
                             ARN5_ID_PPRBS, TRUE, NULL, NULL, NULL) ;

  if (pprbs_stats_blk_cb == NULL) {
    return (CS_ERROR) ;
  }

  ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;
  portNum = ARN5_PORT_ID_TO_PORT_NUM(port_id);

  if ( arn5_stats_add_sect_tbl_entry(port_id, ARN5_PORT, ARN5_ID_PPRBS,
                                    "PPRBS Line Channel Stats",
                                    ARN5_PPRBS_STATS_LINE_SECT,
                                    portNum, sizeof(ARN5_slice_t), TRUE,
                                    ARN5_PPRBS_CHAN_STATS_MAX_CTR,
                                    (arn5_stats_data_t *)
                                    (&ppcb->stats.pprbs.line),
                                    arn5_pprbs_chan1_stats_tbl)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_stats_add_sect_tbl_entry(port_id, ARN5_PORT, ARN5_ID_PPRBS,
                                    "PPRBS Host Channel Stats",
                                    ARN5_PPRBS_STATS_HOST_SECT,
                                    portNum, sizeof(ARN5_slice_t), TRUE,
                                    ARN5_PPRBS_CHAN_STATS_MAX_CTR,
                                    (arn5_stats_data_t *)
                                    (&ppcb->stats.pprbs.host),
                                    arn5_pprbs_chan0_stats_tbl)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


