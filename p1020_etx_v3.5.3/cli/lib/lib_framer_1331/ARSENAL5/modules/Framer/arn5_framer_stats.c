/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_stats.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_stats_priv.h"
#include "arn5_framer_stats.h"
#include "arn5_framer_priv.h"

/******************************************************************************
 * FRAMER STATS TABLES
 ******************************************************************************/


/******************************************************************************
 * FRAMER STREAM SECTION TABLE IN PKT MODE
 ******************************************************************************/
const static arn5_stats_info_t  framer_pkt_stats_tbl[ARN5_FR_PKT_STATS_MAX_CTR] = {
  [ARN5_FR_PKT_STATS_GOOD_PKT] = { "Good Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt1Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt1Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_PKT_STATS_BYTES] = { "Bytes",
                  ARN5_FR_REG_OFST(streamTx_Cnt7Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt8Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_PKT_STATS_GOOD_BYTES] = { "Good Bytes",
                  ARN5_FR_REG_OFST(streamTx_Cnt6Q1, 0, 0),
                  ARN5_FR_REG_OFST(Cnt9Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_PKT_STATS_SHORT_PKT] = { "Short Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt3Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt2Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_PKT_STATS_LONG_PKT] = { "Long Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt4Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt6Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_PKT_STATS_ABORT_PKT] = { "Abort Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt2Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt4Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_PKT_STATS_FCS_ERR_PKT] = { "FCS Error Packets",
                  0,
                  ARN5_FR_REG_OFST(streamRx_Cnt3Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID)
                },

  [ARN5_FR_PKT_STATS_DISCARDED_PKT] = {
                  "Discarded Packets (Overflow/Underflow)",
                  ARN5_FR_REG_OFST(streamTx_Cnt5Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt5Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_PKT_STATS_DROP_PKT] = { "Drop Packets (Drop Enabled)",
                  0,
                  ARN5_FR_REG_OFST(streamRx_Cnt7Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID )
                },

  [ARN5_FR_PKT_STATS_INVALID_PKT] = { "Invalid Packets",
                  0,
                  ARN5_FR_REG_OFST(Cnt10Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID )
                },

  [ARN5_FR_PKT_STATS_X86_ADAPTATION_BYTES] = { "X86 Adaptation Bytes",
                  0,
                  ARN5_FR_REG_OFST(Cnt11Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID )
                },

} ;

/******************************************************************************
 * FRAMER STREAM SECTION TABLE IN GFP MODE
 ******************************************************************************/
const static arn5_stats_info_t  framer_gfp_stats_tbl[ARN5_FR_GFP_STATS_MAX_CTR] = {
  [ARN5_FR_GFP_STATS_GOOD_PKT] = { "Good Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt1Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt1Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_GOOD_BYTES] = { "Good Bytes",
                  ARN5_FR_REG_OFST(streamTx_Cnt6Q1, 0, 0),
                  ARN5_FR_REG_OFST(Cnt9Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_ERR_PKT] = { "FCS Error Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt2Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt3Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_MULTI_ERR_PKT] = { "Multiple Bit Error Packets",
                  0 ,
                  ARN5_FR_REG_OFST(streamRx_Cnt4Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID)
                },

  [ARN5_FR_GFP_STATS_SINGLE_ERR_PKT] = { "Single Bit HEC Error",
                  0 ,
                  ARN5_FR_REG_OFST(streamRx_Cnt8Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID)
                },

  [ARN5_FR_GFP_STATS_DISCARDED_PKT] = {
                  "Discarded Packets (Overflow/Underflow)",
                  ARN5_FR_REG_OFST(streamTx_Cnt5Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt5Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_SHORT_PKT] = { "Short Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt3Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt2Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_LONG_PKT] = { "Long Packets",
                  ARN5_FR_REG_OFST(streamTx_Cnt4Q1, 0, 0),
                  ARN5_FR_REG_OFST(streamRx_Cnt6Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_DROP_PKT] = { "Drop Packets",
                  0 ,
                  ARN5_FR_REG_OFST(streamRx_Cnt7Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID)
                },

  [ARN5_FR_GFP_STATS_ERR_CORRECTED] = { "Error corrected (1 Bit)",
                  ARN5_FR_REG_OFST(streamTx_Cnt7Q1, 0, 0),
                  0,
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_IDLE_PKT] = { "IDLE Packets",
                  ARN5_FR_REG_OFST(gfp_tx_Cnt8Q1, 0, 0),
                  ARN5_FR_REG_OFST(Cnt10Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                },

  [ARN5_FR_GFP_STATS_THEC_ERR_PKT] = { "tHEC Error Packets",
                  0 ,
                  ARN5_FR_REG_OFST(Cnt11Q1, 0, 0),
                  3, /* 48 bit counter */
                  256,
                  (ARN5_STATS_FLAG_RX_VALID)
                },

} ;


/******************************************************************************
 * FRAMER STREAM SECTION TABLE IN ATM MODE
 ******************************************************************************/
const static arn5_stats_info_t
                    framer_atm_stats_tbl[ARN5_FR_ATM_STATS_MAX_CTR] = {
  [ARN5_FR_ATM_STATS_CELL] = { "Good Cells",
                   ARN5_FR_REG_OFST(streamTx_Cnt1Q1, 0, 0),
                   ARN5_FR_REG_OFST(streamRx_Cnt1Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                 },

  [ARN5_FR_ATM_STATS_IDLE_CELL] = { "IDLE Cells",
                   ARN5_FR_REG_OFST(streamTx_Cnt2Q1, 0, 0),
                   ARN5_FR_REG_OFST(streamRx_Cnt2Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (ARN5_STATS_FLAG_RX_VALID | ARN5_STATS_FLAG_TX_VALID)
                 },

  [ARN5_FR_ATM_STATS_ERR_CORRECTED] = { "Header CheckSum Error Corrected",
                   0,
                   ARN5_FR_REG_OFST(streamRx_Cnt3Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (ARN5_STATS_FLAG_RX_VALID )
                 },

  [ARN5_FR_ATM_STATS_ERR] = { "Header CheckSum Error Uncorrected",
                   0,
                   ARN5_FR_REG_OFST(streamRx_Cnt4Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (ARN5_STATS_FLAG_RX_VALID )
                 },

  [ARN5_FR_ATM_STATS_DROP_OVERFLOW] = { "Cells Dropped (FIFO Overflow)",
                   0,
                   ARN5_FR_REG_OFST(streamRx_Cnt5Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (ARN5_STATS_FLAG_RX_VALID )
                 },

  [ARN5_FR_ATM_STATS_OCD] = { "OCD conditions",
                   0,
                   ARN5_FR_REG_OFST(streamRx_Cnt6Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (ARN5_STATS_FLAG_RX_VALID )
                 },

  [ARN5_FR_ATM_STATS_DROP_ENBL] = { "Cells dropped (Drop Enabled)",
                   0,
                   ARN5_FR_REG_OFST(streamRx_Cnt7Q1, 0, 0),
                   3, /* 48 bit counter */
                   256,
                   (ARN5_STATS_FLAG_RX_VALID )
                 }
} ;

/******************************************************************************
 * FRAMER SONET SECTION OVERHEAD STATS TABLE
 ******************************************************************************/
const static arn5_stats_info_t
        framer_sonet_sect_stats[ARN5_FR_SONET_SECT_STATS_MAX_CTR] = {

  [ARN5_FR_SONET_SECT_STATS_B1_ERR] = { "B1 Error",
                 0, ARN5_FR_REG_OFST(B1BIPErrorCount, 0, 0),
                 1, 0, (ARN5_STATS_FLAG_RX_VALID)
               }

} ;

/******************************************************************************
 * FRAMER SONET LINE OVERHEAD STATS TABLE
 ******************************************************************************/
const static arn5_stats_info_t
        framer_sonet_line_stats[ARN5_FR_SONET_LINE_STATS_MAX_CTR] = {
  [ARN5_FR_SONET_LINE_STATS_B2_ERR] = { "B2 Error",
                0,
                ARN5_FR_REG_OFST(B2BIPErrorCountHigh, 0, 0),
                2, 0, (ARN5_STATS_FLAG_RX_VALID)
              },

  [ARN5_FR_SONET_LINE_STATS_REI] = { "REI",
                0,
                ARN5_FR_REG_OFST(LineREICountHigh, 0, 0),
                2, 0, (ARN5_STATS_FLAG_RX_VALID)
              }

} ;

/******************************************************************************
 * FRAMER SONET PATH OVERHEAD STATS TABLE
 ******************************************************************************/
const static arn5_stats_info_t
        framer_sonet_path_stats[ARN5_FR_SONET_PATH_STATS_MAX_CTR] = {
  [ARN5_FR_SONET_PATH_STATS_B3_ERR] = { "B3 Error",
                0,
                ARN5_FR_REG_OFST(PathBIPCount, 0, 0),
                1, 0, (ARN5_STATS_FLAG_RX_VALID)
              },

  [ARN5_FR_SONET_PATH_STATS_REI_CNT] = { "REI Count",
                0,
                ARN5_FR_REG_OFST(PathREICount, 0, 0),
                1, 0, (ARN5_STATS_FLAG_RX_VALID)
              },

  [ARN5_FR_SONET_PATH_STATS_NEG_PTR] = { "Negative Pointer justifications",
                0,
                ARN5_FR_REG_OFST(PathNegativeCount, 0, 0),
                1, 0, (ARN5_STATS_FLAG_RX_VALID)
              },

  [ARN5_FR_SONET_PATH_STATS_POS_PTR] = { "Positive Pointer justifications",
                0,
                ARN5_FR_REG_OFST(PathPositiveCount, 0, 0),
                1, 0, (ARN5_STATS_FLAG_RX_VALID)
              },

  [ARN5_FR_SONET_PATH_STATS_NEW_PTR] = { "New Pointers",
                0,
                ARN5_FR_REG_OFST(PathNewPtrCount, 0, 0),
                1, 0, (ARN5_STATS_FLAG_RX_VALID)
              },


} ;

/******************************************************************************
 * FRAMER SECTION STATS TABLE
 ******************************************************************************/
const static arn5_stats_sect_tbl_entry_t
            framer_stats_sect_tbl[ARN5_FR_STATS_MAX_SECTION_ID] = {
    [ARN5_FR_STATS_SECT_OVRHD] = {
                    "Framer Section Overhead Stats",
                    ARN5_OFST(arn5_framer_stats_t, section_ovrhd),
                    0, framer_sonet_sect_stats,
                    ARN5_FR_SONET_SECT_STATS_MAX_CTR },

    [ARN5_FR_STATS_LINE_OVRHD] = {
                    "Framer Line Overhead Stats",
                    ARN5_OFST(arn5_framer_stats_t, line_ovrhd),
                    0, framer_sonet_line_stats,
                    ARN5_FR_SONET_LINE_STATS_MAX_CTR},

    [ARN5_FR_STATS_PATH_OVRHD] = {
                    "Framer Path Overhead Stats",
                    ARN5_OFST(arn5_framer_stats_t, path),
                    0, framer_sonet_path_stats,
                    ARN5_FR_SONET_PATH_STATS_MAX_CTR},

    [ARN5_FR_STATS_PKT_MODE] = {
                    "Framer (Packet Mode) Stats",
                    ARN5_OFST(arn5_framer_stats_t, strm.pkt),
                    0, framer_pkt_stats_tbl,
                    ARN5_FR_PKT_STATS_MAX_CTR},

    [ARN5_FR_STATS_ATM_MODE] = {
                    "Framer (ATM Mode) Stats",
                    ARN5_OFST(arn5_framer_stats_t, strm.atm),
                    0, framer_atm_stats_tbl,
                    ARN5_FR_ATM_STATS_MAX_CTR},

    [ARN5_FR_STATS_GFP_MODE] = {
                    "Framer (GFP Mode) Stats",
                    ARN5_OFST(arn5_framer_stats_t, strm.gfp),
                    0, framer_gfp_stats_tbl,
                    ARN5_FR_GFP_STATS_MAX_CTR},
} ;

static
cs_status arn5_framer_register_sect_stats(cs_uint16 port_id,
                                         cs_uint16 sect_id)
{
  arn5_stats_data_t     * p_data ;
  arn5_port_stats_t     * p_port_stats ;
  arn5_framer_stats_t   * p_framer_stats ;
  cs_uint8              sl_num ;
  const arn5_stats_sect_tbl_entry_t * tbl = NULL ;

  p_port_stats = ARN5_PORT_GET_STATS_PTR(port_id) ;
  if (p_port_stats == NULL) {
    return (CS_ERROR) ;
  }

  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;

  p_framer_stats = &p_port_stats->framer ;

  tbl = &framer_stats_sect_tbl[sect_id] ;

  if ( tbl->sect_offset >= sizeof(arn5_framer_stats_t) ) {
    CS_PRINT("%s() SW ERROR- offset(0x%x) s/b < table-size(0x%x)\n",
             __FUNCTION__,
             tbl->sect_offset, sizeof(arn5_framer_stats_t) ) ;
    return (CS_ERROR) ;
  }

  p_data = (arn5_stats_data_t *)
    (((cs_uint32)p_framer_stats) + tbl->sect_offset) ;

  if ( arn5_stats_add_sect_tbl_entry(port_id, ARN5_PORT, ARN5_ID_FRAMER,
                                    tbl->sect_name, sect_id,
                                    sl_num, sizeof(ARN5_slice_t),
                                    TRUE, tbl->num_unit,
                                    p_data, tbl->info_tbl)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   INIT FRAMER STATS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_init_stats(cs_uint16 port_id,
                        arn5_framer_svc_type_t svc)
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
  arn5_stats_blk_cb_t * framer_stats_blk_cb ;

  /* Register Framer Stats */
  framer_stats_blk_cb = arn5_stats_register_blk_cb(port_id, ARN5_PORT,
                                                  ARN5_ID_FRAMER,
                                                  TRUE,
                                                  NULL,
                                                  NULL,
                                                  NULL) ;
  if (framer_stats_blk_cb == NULL) {
      return (CS_ERROR) ;
  }

  /* Register and enable sonet section overhead stats */
  if ( arn5_framer_register_sect_stats(port_id, ARN5_FR_STATS_SECT_OVRHD)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  if ( arn5_framer_register_sect_stats(port_id, ARN5_FR_STATS_LINE_OVRHD)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* Register  SONET Path Overhead stats sections */
  if ( arn5_framer_register_sect_stats(port_id, ARN5_FR_STATS_PATH_OVRHD)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* Register Stream ( ATM/GFP/PKT Section stats */
  if ( arn5_framer_init_strm_stats(port_id, svc)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


cs_status arn5_framer_init_strm_stats(cs_uint16 port_id,
                                     arn5_framer_svc_type_t svc)
{
  cs_uint16            sect_id ;

  switch (svc) {
    case ARN5_FR_SVC_POS_PPP :
    case ARN5_FR_SVC_POS_HDLC :
    case ARN5_FR_SVC_LAPS :
      sect_id = ARN5_FR_STATS_PKT_MODE ;
      break ;
    case ARN5_FR_SVC_ATM :
      sect_id = ARN5_FR_STATS_ATM_MODE ;
      break ;
    case ARN5_FR_SVC_GFP :
      sect_id = ARN5_FR_STATS_GFP_MODE ;
      break ;
    case ARN5_FR_SVC_PRBS :
    case ARN5_FR_SVC_CLR_CH :
      return (CS_OK) ;
    default :
      return (CS_ERROR) ;
  }

  if ( arn5_framer_register_sect_stats(port_id, sect_id) != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* X68 adaptation bytes only valid in LAPS mode */
  if ((svc == ARN5_FR_SVC_POS_PPP) || (svc == ARN5_FR_SVC_POS_HDLC))
  {
    if ( arn5_port_ctl_unit_stats(port_id, ARN5_ID_FRAMER,
           ARN5_FR_STATS_PKT_MODE, ARN5_FR_PKT_STATS_X86_ADAPTATION_BYTES,
           CS_DISABLE) != CS_OK ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}

