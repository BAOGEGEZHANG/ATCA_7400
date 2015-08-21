/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth_stats.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for ETH Statistics.
 */
#ifndef __ARN5_ETH_STATS_H__
#define __ARN5_ETH_STATS_H__


/*****************************************************************
 *             ETH PORT STATISTICS                               *
 *****************************************************************/

/*********************************************************************
 *                PORT UNIT STATS ENUM's                             *
 *********************************************************************/
typedef enum {                      /*   Rx                           Tx                    */
  ARN5_ETH_STATS_MAC_FRAME_OK  = 0,  /* aFramesReceivedOK           , aFramesTransmittedOK   */
  ARN5_ETH_STATS_MAC_FCS_ERR,        /* aFrameCheckSequenceErrors   , ----                   */
  ARN5_ETH_STATS_MAC_ALIGNMENT_ERR,  /* aAlignmentErrors            , ----                   */
  ARN5_ETH_STATS_MAC_OCTET_OK,       /* aOctetsReceivedOK           , aOctetsTransmittedOK   */
  ARN5_ETH_STATS_MAC_PAUSE_FRAME,    /* aRxPauseMacCtrlFrames       , aTxPauseMacCtrlFrames  */
  ARN5_ETH_STATS_MAC_IF_ERR,         /* ifInErrors                  , ifOutErrors            */
  ARN5_ETH_STATS_MAC_UCAST_PKT,      /* ifInUcastPkts               , ifOutUcastPkts         */
  ARN5_ETH_STATS_MAC_MCAST_PKT,      /* ifInMulticastPkts           , ifOutMulticastPkts     */
  ARN5_ETH_STATS_MAC_BCAST_PKT,      /* ifInBroadcastPkts           , ifOutBroadcastPkts     */
  ARN5_ETH_STATS_MAC_DROP_EVENT,     /* etherStatsDropEvents        , ----                   */
  ARN5_ETH_STATS_MAC_OCTET,          /* etherStatsOctets            , ----                   */
  ARN5_ETH_STATS_MAC_PKT,            /* etherStatsPkts              , ----                   */
  ARN5_ETH_STATS_MAC_UNDER_SIZE_PKT, /* etherStatsUndersizePkts     , ----                   */
  ARN5_ETH_STATS_MAC_OVER_SIZE_PKT,  /* etherStatsOversizePkts      , ----                   */
  ARN5_ETH_STATS_MAC_64_OCTET,       /* etherStatsPkts64Octets      , ----                   */
  ARN5_ETH_STATS_MAC_65_127_OCTET,   /* etherStatsPkts65to127Octets , ----                   */
  ARN5_ETH_STATS_MAC_128_255_OCTET,  /* etherStatsPkts128to255Octets, ----                   */
  ARN5_ETH_STATS_MAC_256_511_OCTET,  /* etherStatsPkts256to511Octets, ----                   */
  ARN5_ETH_STATS_MAC_512_1023_OCTET, /* etherStatsPkts512to1023Octets , ----                 */
  ARN5_ETH_STATS_MAC_1024_1518_OCTET,/* etherStatsPkts1024to1518Octets, ----                 */
  ARN5_ETH_STATS_MAC_1519_MAX_OCTET, /* etherStatsPkts1519toMaxOctets , ----                 */
    /* all */
  ARN5_ETH_STATS_MAC_SECT_ALL,
} arn5_eth_stats_mac_enum_t ;


typedef enum {                         /*   Rx                  Tx    */
  ARN5_ETH_STATS_PCS_8B10B_DEC_ERR = 0, /* RX_8B10B_DEC_ERR   , ----   */
    /* all */
  ARN5_ETH_STATS_PCS_SECT_ALL,
} arn5_eth_stats_pcs_enum_t ;


typedef enum {                         /*   Rx                  Tx                  */
  ARN5_ETH_STATS_CONV_RX_DROP = 0,      /* CONV_RX_DROPCOUNT   , ----                */
  ARN5_ETH_STATS_CONV_RX_ABORT,         /* CONV_RX_ABORTCOUNT  , ----                */
  ARN5_ETH_STATS_CONV_TX_DROP,          /* ----                , CONV_TX_ABORTCOUNT  */
    /* all */
  ARN5_ETH_STATS_CONV_SECT_ALL,
} arn5_eth_stats_conv_enum_t ;




/*********************************************************************
 *               SECTION-ID ENUM's FOR ETH BLOCK                     *
 *********************************************************************/
typedef enum {
  ARN5_ETH_STATS_SECT_MAC = 0,
  ARN5_ETH_STATS_SECT_PCS,
  ARN5_ETH_STATS_SECT_CONV,
   /*** ALL ****/
  ARN5_ETH_STATS_SECT_ALL,
} arn5_eth_stats_sect_enum_t ;


/********************************************************************
 *              ETH PORT STATISTICS DATA-STRUCTURE                  *
 ********************************************************************/
typedef struct {
  arn5_stats_data_t      mac[ ARN5_ETH_STATS_MAC_SECT_ALL ] ;
  arn5_stats_data_t      pcs[ ARN5_ETH_STATS_PCS_SECT_ALL ] ;
  arn5_stats_data_t      conv[ ARN5_ETH_STATS_CONV_SECT_ALL ] ;
} arn5_eth_stats_t ;


#endif /* __ARN5_ETH_STATS_H__ */

