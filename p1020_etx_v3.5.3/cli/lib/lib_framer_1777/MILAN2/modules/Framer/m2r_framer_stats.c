/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_framer_stats.h"

/******************************************************************************
 * FRAMER STATS TABLES
 ******************************************************************************/


/******************************************************************************
 * FRAMER STREAM SECTION TABLE IN PKT MODE
 ******************************************************************************/
const static m2r_stats_info_t  framer_pkt_stats_tbl[M2R_FR_PKT_STATS_MAX_CTR] = {
  CS_DI([M2R_FR_PKT_STATS_GOOD_PKT] =) { "Good Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt1Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt1Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_BYTES] =) { "Bytes",
                  M2R_FR_REG_OFFSET(streamTx_Cnt7Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt8Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_GOOD_BYTES] =) { "Good Bytes",
                  M2R_FR_REG_OFFSET(streamTx_Cnt6Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt9Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_SHORT_PKT] =) { "Short Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt3Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt2Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_LONG_PKT] =) { "Long Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt4Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt6Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_ABORT_PKT] =) { "Abort Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt2Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt4Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_FCS_ERR_PKT] =) { "FCS Error Packets",
                  0,
                  M2R_FR_REG_OFFSET(streamRx_Cnt3Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_DISCARDED_PKT] =) {
                  "Discarded Packets (Overflow/Underflow)",
                  M2R_FR_REG_OFFSET(streamTx_Cnt5Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt5Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_PKT_STATS_DROP_PKT] =) { "Drop Packets (Drop Enabled)",
                  0,
                  M2R_FR_REG_OFFSET(streamRx_Cnt7Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID )
                },

  CS_DI([M2R_FR_PKT_STATS_INVALID_PKT] =) { "Invalid Packets",
                  0,
                  M2R_FR_REG_OFFSET(Cnt10Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID )
                }
} ;

/******************************************************************************
 * FRAMER STREAM SECTION TABLE IN GFP MODE
 ******************************************************************************/
const static m2r_stats_info_t  framer_gfp_stats_tbl[M2R_FR_GFP_STATS_MAX_CTR] = {
  CS_DI([M2R_FR_GFP_STATS_GOOD_PKT] =) { "Good Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt1Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt1Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_GOOD_BYTES] =) { "Good Bytes",
                  M2R_FR_REG_OFFSET(streamTx_Cnt6Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt9Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_ERR_PKT] =) { "FCS Error Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt2Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt3Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_MULTI_ERR_PKT] =) { "Multiple Bit Error Packets",
                  0 ,
                  M2R_FR_REG_OFFSET(streamRx_Cnt4Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_SINGLE_ERR_PKT] =) { "Single Bit HEC Error",
                  0 ,
                  M2R_FR_REG_OFFSET(streamRx_Cnt8Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_DISCARDED_PKT] =) {
                  "Discarded Packets (Overflow/Underflow)",
                  M2R_FR_REG_OFFSET(streamTx_Cnt5Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt5Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_SHORT_PKT] =) { "Short Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt3Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt2Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_LONG_PKT] =) { "Long Packets",
                  M2R_FR_REG_OFFSET(streamTx_Cnt4Q1, 0, 0),
                  M2R_FR_REG_OFFSET(streamRx_Cnt6Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_OVRSIZE_PKT] =) { "Oversize Packets (10000 bytes)",
                  M2R_FR_REG_OFFSET(gfp_tx_Cnt9Q1, 0, 0),
                  0,
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_DROP_PKT] =) { "Drop Packets",
                  0 ,
                  M2R_FR_REG_OFFSET(streamRx_Cnt7Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_ERR_CORRECTED] =) { "Error corrected (1 Bit)",
                  M2R_FR_REG_OFFSET(streamTx_Cnt7Q1, 0, 0),
                  0,
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_TX_VALID)
                },

  CS_DI([M2R_FR_GFP_STATS_IDLE_PKT] =) { "IDLE Packets",
                  M2R_FR_REG_OFFSET(gfp_tx_Cnt8Q1, 0, 0),
                  M2R_FR_REG_OFFSET(Cnt10Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                }
} ;


/******************************************************************************
 * FRAMER STREAM SECTION TABLE IN ATM MODE
 ******************************************************************************/
const static m2r_stats_info_t
                    framer_atm_stats_tbl[M2R_FR_ATM_STATS_MAX_CTR] = {
  CS_DI([M2R_FR_ATM_STATS_CELL] =) { "Good Cells",
                   M2R_FR_REG_OFFSET(streamTx_Cnt1Q1, 0, 0),
                   M2R_FR_REG_OFFSET(streamRx_Cnt1Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                 },

  CS_DI([M2R_FR_ATM_STATS_IDLE_CELL] =) { "IDLE Cells",
                   M2R_FR_REG_OFFSET(streamTx_Cnt2Q1, 0, 0),
                   M2R_FR_REG_OFFSET(streamRx_Cnt2Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (M2R_STATS_FLAG_RX_VALID | M2R_STATS_FLAG_TX_VALID)
                 },

  CS_DI([M2R_FR_ATM_STATS_ERR_CORRECTED] =) { "Error Cells Corrected",
                   0,
                   M2R_FR_REG_OFFSET(streamRx_Cnt3Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (M2R_STATS_FLAG_RX_VALID )
                 },

  CS_DI([M2R_FR_ATM_STATS_ERR] =) { "Error Cells",
                   0,
                   M2R_FR_REG_OFFSET(streamRx_Cnt4Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (M2R_STATS_FLAG_RX_VALID )
                 },

  CS_DI([M2R_FR_ATM_STATS_DROP_OVERFLOW] =) { "Cells Dropped (FIFO Overflow)",
                   0,
                   M2R_FR_REG_OFFSET(streamRx_Cnt5Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (M2R_STATS_FLAG_RX_VALID )
                 },

  CS_DI([M2R_FR_ATM_STATS_OCD] =) { "OCD conditions",
                   0,
                   M2R_FR_REG_OFFSET(streamRx_Cnt6Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (M2R_STATS_FLAG_RX_VALID )
                 },

  CS_DI([M2R_FR_ATM_STATS_DROP_ENBL] =) { "Cells dropped (Drop Enabled)",
                   0,
                   M2R_FR_REG_OFFSET(streamRx_Cnt7Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (M2R_STATS_FLAG_RX_VALID )
                 }
} ;

/******************************************************************************
 * FRAMER SONET SECTION OVERHEAD STATS TABLE
 ******************************************************************************/
const static m2r_stats_info_t
        framer_sonet_sect_stats[M2R_FR_SONET_SECT_STATS_MAX_CTR] = {

  CS_DI([M2R_FR_SONET_SECT_STATS_B1_ERR] =) { "B1 Error",
                 0, M2R_FR_REG_OFFSET(B1BIPErrorCount, 0, 0),
                 1, 0, (M2R_STATS_FLAG_RX_VALID)
               }
} ;

/******************************************************************************
 * FRAMER SONET LINE OVERHEAD STATS TABLE
 ******************************************************************************/
const static m2r_stats_info_t
        framer_sonet_line_stats[M2R_FR_SONET_LINE_STATS_MAX_CTR] = {
  CS_DI([M2R_FR_SONET_LINE_STATS_B2_ERR] =) { "B2 Error",
                0,
                M2R_FR_REG_OFFSET(B2BIPErrorCountHigh, 0, 0),
                2, 0, (M2R_STATS_FLAG_RX_VALID)
              },

  CS_DI([M2R_FR_SONET_LINE_STATS_REI] =) { "REI",
                0,
                M2R_FR_REG_OFFSET(LineREICountHigh, 0, 0),
                2, 0, (M2R_STATS_FLAG_RX_VALID)
              }
} ;

/******************************************************************************
 * FRAMER SONET PATH OVERHEAD STATS TABLE
 ******************************************************************************/
const static m2r_stats_info_t
        framer_sonet_path_stats[M2R_FR_SONET_PATH_STATS_MAX_CTR] = {
  CS_DI([M2R_FR_SONET_PATH_STATS_B3_ERR] =) { "B3 Error",
                0,
                M2R_FR_REG_OFFSET(PathBIPCount, 0, 0),
                1, 0, (M2R_STATS_FLAG_RX_VALID)
              },

  CS_DI([M2R_FR_SONET_PATH_STATS_REI_CNT] =) { "REI Count",
                0,
                M2R_FR_REG_OFFSET(PathREICount, 0, 0),
                1, 0, (M2R_STATS_FLAG_RX_VALID)
              },

  CS_DI([M2R_FR_SONET_PATH_STATS_NEG_PTR] =) { "Negative Pointer justifications",
                0,
                M2R_FR_REG_OFFSET(PathNegativeCount, 0, 0),
                1, 0, (M2R_STATS_FLAG_RX_VALID)
              },

  CS_DI([M2R_FR_SONET_PATH_STATS_POS_PTR] =) { "Positive Pointer justifications",
                0,
                M2R_FR_REG_OFFSET(PathPositiveCount, 0, 0),
                1, 0, (M2R_STATS_FLAG_RX_VALID)
              },

  CS_DI([M2R_FR_SONET_PATH_STATS_NEW_PTR] =) { "New Pointers",
                0,
                M2R_FR_REG_OFFSET(PathNewPtrCount, 0, 0),
                1, 0, (M2R_STATS_FLAG_RX_VALID)
              }
} ;

/******************************************************************************
 * FRAMER SECTION STATS TABLE
 ******************************************************************************/
const static m2r_stats_sect_tbl_entry_t
            framer_stats_sect_tbl[M2R_FR_STATS_MAX_SECTION_ID] = {
    CS_DI([M2R_FR_STATS_SECT_OVRHD] =) {
                    "Framer SONET Section Overhead Stats",
                    M2R_OFST(m2r_framer_stats_t, section_ovrhd),
                    0, framer_sonet_sect_stats,
                    M2R_FR_SONET_SECT_STATS_MAX_CTR },

    CS_DI([M2R_FR_STATS_LINE_OVRHD] =) {
                    "Framer SONET Line Overhead Stats",
                    M2R_OFST(m2r_framer_stats_t, line_ovrhd),
                    0, framer_sonet_line_stats,
                    M2R_FR_SONET_LINE_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_PATH0_OVRHD] =) {
                    "Framer SONET Stream-0 Path Overhead Stats",
                    M2R_OFST(m2r_framer_stats_t, path_0),
                    0, framer_sonet_path_stats,
                    M2R_FR_SONET_PATH_STATS_MAX_CTR},
    CS_DI([M2R_FR_STATS_PATH1_OVRHD] =) {
                    "Framer SONET Stream-1 Path Overhead Stats",
                    M2R_OFST(m2r_framer_stats_t, path_1),
                    1, framer_sonet_path_stats,
                    M2R_FR_SONET_PATH_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_PATH2_OVRHD] =) {
                    "Framer SONET Stream-2 Path Overhead Stats",
                    M2R_OFST(m2r_framer_stats_t, path_2),
                    2, framer_sonet_path_stats,
                    M2R_FR_SONET_PATH_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_PATH3_OVRHD] =) {
                    "Framer SONET Stream-3 Path Overhead Stats",
                    M2R_OFST(m2r_framer_stats_t, path_3),
                    3, framer_sonet_path_stats,
                    M2R_FR_SONET_PATH_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR0_PKT_MODE] =) {
                    "Framer Stream-0 (Packet Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm0.pkt),
                    0, framer_pkt_stats_tbl,
                    M2R_FR_PKT_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR1_PKT_MODE] =) {
                    "Framer Stream-1 (Packet Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm1.pkt),
                    1, framer_pkt_stats_tbl,
                    M2R_FR_PKT_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR2_PKT_MODE] =) {
                    "Framer Stream-2 (Packet Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm2.pkt),
                    2, framer_pkt_stats_tbl,
                    M2R_FR_PKT_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR3_PKT_MODE] =) {
                    "Framer Stream-3 (Packet Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm3.pkt),
                    3, framer_pkt_stats_tbl,
                    M2R_FR_PKT_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR0_ATM_MODE] =) {
                    "Framer Stream-0 (ATM Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm0.atm),
                    0, framer_atm_stats_tbl,
                    M2R_FR_ATM_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR1_ATM_MODE] =) {
                    "Framer Stream-1 (ATM Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm1.atm),
                    1, framer_atm_stats_tbl,
                    M2R_FR_ATM_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR2_ATM_MODE] =) {
                    "Framer Stream-2 (ATM Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm2.atm),
                    2, framer_atm_stats_tbl,
                    M2R_FR_ATM_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR3_ATM_MODE] =) {
                    "Framer Stream-3 (ATM Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm3.atm),
                    3, framer_atm_stats_tbl,
                    M2R_FR_ATM_STATS_MAX_CTR},

    CS_DI([M2R_FR_STATS_STR0_GFP_MODE] =) {
                    "Framer Stream-0 (GFP Mode) Stats",
                    M2R_OFST(m2r_framer_stats_t, strm0.gfp),
                    0, framer_gfp_stats_tbl,
                    M2R_FR_GFP_STATS_MAX_CTR}
} ;

static
cs_status m2r_framer_register_sect_stats(cs_uint16 port_id,
                                         cs_uint16 sect_id)
{
  m2r_stats_data_t   * p_data ;
  m2r_port_stats_t   * p_port_stats ;
  m2r_framer_stats_t * p_framer_stats ;
  const m2r_stats_sect_tbl_entry_t * tbl = NULL ;

  p_port_stats = M2R_GET_STATS_PTR(port_id) ;
  if (p_port_stats == NULL) {
    return (CS_ERROR) ;
  }
  p_framer_stats = &p_port_stats->framer ;

  tbl = &framer_stats_sect_tbl[sect_id] ;

  if ( tbl->sect_offset >= sizeof(m2r_framer_stats_t) ) {
    CS_PRINT("%s() SW ERROR- offset(0x%x) s/b < table-size(0x%x)\n",
	     __FUNCTION__,
	     tbl->sect_offset, sizeof(m2r_framer_stats_t) ) ;
    return (CS_ERROR) ;
  }

  p_data = (m2r_stats_data_t *)
    (((cs_uint64)p_framer_stats) + tbl->sect_offset) ;

  if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_FRAMER,
				    tbl->sect_name, sect_id,
				    tbl->stride_id, TRUE,
				    tbl->num_unit,
				    p_data, tbl->info_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   INIT FRAMER STATS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_init_stats(cs_uint16 port_id,
                                 m2r_framer_stream_t channel,
                                 m2r_framer_svc_type_t svc)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( channelized Mode ) else FALSE          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Init the Framer Statistics. This API will register with the  */
/* Framer block and its stats control tables with the stats.    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_stats_blk_cb_t * framer_stats_blk_cb ;
  cs_uint32            str_start = 0, str_end = 0, str_iter ;

  /* Register Framer Stats */
  framer_stats_blk_cb = m2r_stats_register_blk_cb(port_id,
                                                  M2R_ID_FRAMER,
                                                  TRUE,
                                                  NULL,
                                                  NULL,
                                                  NULL) ;
  if (framer_stats_blk_cb == NULL) {
      return (CS_ERROR) ;
  }

  /* Register and enable sonet section overhead stats */
  if ( m2r_framer_register_sect_stats(port_id, M2R_FR_STATS_SECT_OVRHD)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_framer_register_sect_stats(port_id, M2R_FR_STATS_LINE_OVRHD)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if (channel != M2R_SONET_STREAM_ALL) {
    str_start = channel ;
    str_end   = channel + 1 ;
  }
  else {
    str_start = M2R_SONET_STREAM0 ;
    str_end   = M2R_SONET_STREAM_ALL ;
  }

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {

    /* Register  SONET Path Overhead stats sections */
    if ( m2r_framer_register_sect_stats(port_id,
			(M2R_FR_STATS_PATH0_OVRHD + str_iter))
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    /* Register Stream ( ATM/GFP/PKT Section stats */
    if ( m2r_framer_init_strm_stats(port_id, str_iter, svc)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


cs_status m2r_framer_init_strm_stats(cs_uint16 port_id,
                                     m2r_framer_stream_t channel,
                                     m2r_framer_svc_type_t svc)
{
  cs_uint16            sect_id ;

  switch (svc) {
    case M2R_FR_SVC_POS_PPP :
    case M2R_FR_SVC_POS_HDLC :
      sect_id = (M2R_FR_STATS_STR0_PKT_MODE + channel) ;
      break ;
    case M2R_FR_SVC_ATM :
      sect_id = (M2R_FR_STATS_STR0_ATM_MODE + channel) ;
      break ;
    case M2R_FR_SVC_GFP :
      sect_id = (M2R_FR_STATS_STR0_GFP_MODE + channel) ;
      break ;
    case M2R_FR_SVC_PRBS :
    case M2R_FR_SVC_CLR_CH :
      return (CS_OK) ;  
    default :
      return (CS_ERROR) ;
  }

  if ( m2r_framer_register_sect_stats(port_id, sect_id) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

