/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth_stats.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_stats_priv.h"
#include "arn5_eth_stats.h"


#define ARN5_ETH_STATS_TX_ONLY   ( ARN5_STATS_FLAG_TX_VALID   | \
                                   ARN5_STATS_FLAG_NO_CLR_TX  | \
                                   ARN5_STATS_FLAG_PROTECT_TX)

#define ARN5_ETH_STATS_RX_ONLY   ( ARN5_STATS_FLAG_RX_VALID   | \
                                   ARN5_STATS_FLAG_NO_CLR_RX  | \
                                   ARN5_STATS_FLAG_PROTECT_RX)

#define ARN5_ETH_STATS_TX_N_RX   ( ARN5_ETH_STATS_TX_ONLY     | \
                                   ARN5_ETH_STATS_RX_ONLY )





/*****************************************************************
 *             ETH PORT STATISTICS                               *
 *****************************************************************/

/******* INDIVIDUAL SECTION STATS ***************/
static arn5_stats_info_t
arn5_eth_stats_mac_tbl [ARN5_ETH_STATS_MAC_SECT_ALL] = {
  [ ARN5_ETH_STATS_MAC_FRAME_OK ] = {
    "aFramesOK",                                                /* name        */
    ARN5_ETH_EI_REG_OFST(ecore.aFramesTransmittedOK1),          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.aFramesReceivedOK1),             /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_TX_N_RX)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_FCS_ERR ] = {
    "aFrameCheckSequenceErrors",                                /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.aFrameCheckSequenceErrors1),     /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_ALIGNMENT_ERR ] = {
    "aAlignmentErrors",                                         /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.aAlignmentErrors1),              /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_OCTET_OK ] = {
    "aOctetsOK",                                                /* name        */
    ARN5_ETH_EI_REG_OFST(ecore.aOctetsTransmittedOK1),          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.aOctetsReceivedOK1),             /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_TX_N_RX)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_PAUSE_FRAME ] = {
    "aPauseMacCtrlFrames",                                      /* name        */
    ARN5_ETH_EI_REG_OFST(ecore.aTxPauseMacCtrlFrames1),         /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.aRxPauseMacCtrlFrames1),         /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_TX_N_RX)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_IF_ERR ] = {
    "ifErrors",                                                 /* name        */
    ARN5_ETH_EI_REG_OFST(ecore.ifOutErrors1),                   /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.ifInErrors1),                    /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_TX_N_RX)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_UCAST_PKT ] = {
    "ifUcastPkts",                                              /* name        */
    ARN5_ETH_EI_REG_OFST(ecore.ifOutUcastPkts1),                /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.ifInUcastPkts1),                 /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_TX_N_RX)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_MCAST_PKT ] = {
    "ifMulticastPkts",                                          /* name        */
    ARN5_ETH_EI_REG_OFST(ecore.ifOutMulticastPkts1),            /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.ifInMulticastPkts1),             /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_TX_N_RX)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_BCAST_PKT ] = {
    "ifBroadcastPkts",                                          /* name        */
    ARN5_ETH_EI_REG_OFST(ecore.ifOutBroadcastPkts1),            /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.ifInBroadcastPkts1),             /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_TX_N_RX)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_DROP_EVENT ] = {
    "etherStatsDropEvents",                                     /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsDropEvents1),          /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_OCTET ] = {
    "etherStatsOctets",                                         /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsOctets1),              /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_PKT ] = {
    "etherStatsPkts",                                           /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts1),                /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_UNDER_SIZE_PKT ] = {
    "etherStatsUndersizePkts",                                  /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsUndersizePkts1),       /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_OVER_SIZE_PKT ] = {
    "etherStatsOversizePkts",                                   /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsOversizePkts1),        /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_64_OCTET ] = {
    "etherStatsPkts64Octets",                                   /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts64Octets1),        /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_65_127_OCTET ] = {
    "etherStatsPkts65to127Octets",                              /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts65to127Octets1),   /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_128_255_OCTET ] = {
    "etherStatsPkts128to255Octets",                             /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts128to255Octets1),  /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_256_511_OCTET ] = {
    "etherStatsPkts256to511Octets",                             /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts256to511Octets1),  /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_512_1023_OCTET ] = {
    "etherStatsPkts512to1023Octets",                            /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts512to1023Octets1), /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_1024_1518_OCTET ] = {
    "etherStatsPkts1024to1518Octets",                           /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts1024to1518Octets1),/* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },

  [ ARN5_ETH_STATS_MAC_1519_MAX_OCTET ] = {
    "etherStatsPkts1519toMaxOctets",                            /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.etherStatsPkts1519toMaxOctets1), /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },
} ;

static arn5_stats_info_t
arn5_eth_stats_pcs_tbl [ARN5_ETH_STATS_PCS_SECT_ALL] = {
  [ ARN5_ETH_STATS_PCS_8B10B_DEC_ERR ] = {
    "8B10B Decoder Errors",                                     /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_EI_REG_OFST(ecore.RX_8B10B_DEC_ERR1),              /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_ETH_STATS_RX_ONLY)                                    /* flag        */
  },
} ;

static arn5_stats_info_t
arn5_eth_stats_conv_tbl [ARN5_ETH_STATS_CONV_SECT_ALL] = {
  [ ARN5_ETH_STATS_CONV_RX_DROP ] = {
    "Conv RX drop due to backpressure",                         /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_REG_OFST(CONV_RX_DROPCOUNT),                       /* Rx counter  */
    1,                                                          /* 16-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_STATS_FLAG_RX_VALID)                                  /* flag        */
  },

  [ ARN5_ETH_STATS_CONV_RX_ABORT ] = {
    "Conv RX abort due to backpressure",                        /* name        */
    0,                                                          /* Tx counter  */
    ARN5_ETH_REG_OFST(CONV_RX_ABORTCOUNT),                      /* Rx counter  */
    1,                                                          /* 16-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_STATS_FLAG_RX_VALID)                                  /* flag        */
  },

  [ ARN5_ETH_STATS_CONV_TX_DROP ] = {
    "Conv TX drop due to min size filtering",                   /* name        */
    ARN5_ETH_REG_OFST(CONV_TX_DROPCOUNT1),                      /* Tx counter  */
    0,                                                          /* Rx counter  */
    2,                                                          /* 32-bit wrds */
    0,                                                          /* stride, not used */
    (ARN5_STATS_FLAG_TX_VALID)                                  /* flag        */
  },
} ;


/****************************************************************/
/* $_hdr_start   INIT PORT RELATED ETH STATS                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_init_port_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initialize port related ETH statistics.                      */
/* $_hdr_end.                                                   */
/****************************************************************/
{
  arn5_stats_blk_cb_t   * pBlkCb ;
  arn5_dev_cb_t         * pDevCb ;
  cs_uint8              sl_num ;
  arn5_stats_data_t     * pData ;
  arn5_port_stats_t     * pPortStats ;

  pBlkCb = arn5_stats_register_blk_cb(port_id, ARN5_PORT,
                 ARN5_ID_ETH,
                 TRUE,    /* ctl_enabled */
                 NULL,    /* pGet_func */
                 NULL,    /* pPrint func */
                 NULL) ;  /* pClear_func */
  if (pBlkCb == NULL) {
    CS_PRINT("port-%d: ETH block NOT registered!\n", port_id) ;
    return (CS_ERROR) ;
  }

  /* register for all sections related to this port */
  pDevCb = ARN5_PORT_ID_TO_DEVCB_PTR(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id);
  pPortStats = ARN5_PORT_GET_STATS_PTR(port_id) ;

  pData = pPortStats->eth.mac ;
  if (arn5_stats_add_sect_tbl_entry(port_id, ARN5_PORT, ARN5_ID_ETH,
                                    "ETH Stats MAC Section",
                                    ARN5_ETH_STATS_SECT_MAC,
                                    sl_num, sizeof(ARN5_slice_t), TRUE,
                                    ARN5_ETH_STATS_MAC_SECT_ALL,
                                    pData,
                                    arn5_eth_stats_mac_tbl)
       != CS_OK ) {
    CS_PRINT("Port-%d ERROR: Cannot add mac section stats!\n", port_id) ;
    return (CS_ERROR) ;
  }

  pData = pPortStats->eth.pcs ;
  if (arn5_stats_add_sect_tbl_entry(port_id, ARN5_PORT, ARN5_ID_ETH,
                                    "ETH Stats PCS Section",
                                    ARN5_ETH_STATS_SECT_PCS,
                                    sl_num, sizeof(ARN5_slice_t), TRUE,
                                    ARN5_ETH_STATS_PCS_SECT_ALL,
                                    pData,
                                    arn5_eth_stats_pcs_tbl)
       != CS_OK ) {
    CS_PRINT("Port-%d ERROR: Cannot add pcs section stats!\n", port_id) ;
    return (CS_ERROR) ;
  }

  pData = pPortStats->eth.conv ;
  if (arn5_stats_add_sect_tbl_entry(port_id, ARN5_PORT, ARN5_ID_ETH,
                                    "ETH Stats CONV Section",
                                    ARN5_ETH_STATS_SECT_CONV,
                                    sl_num, sizeof(ARN5_slice_t), TRUE,
                                    ARN5_ETH_STATS_CONV_SECT_ALL,
                                    pData,
                                    arn5_eth_stats_conv_tbl)
       != CS_OK ) {
    CS_PRINT("Port-%d ERROR: Cannot add conv section stats!\n", port_id) ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

