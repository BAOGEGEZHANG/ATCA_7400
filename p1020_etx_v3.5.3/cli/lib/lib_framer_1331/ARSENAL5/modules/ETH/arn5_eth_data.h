/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth_data.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_ETH_DATA_H__
#define __ARN5_ETH_DATA_H__

#include "arn5_registers.h"
#include "arn5_eth_irq.h"


typedef enum {
  ARN5_ETH_SUB_BLOCK_PCS ,
  ARN5_ETH_SUB_BLOCK_MAC ,
  ARN5_ETH_SUB_BLOCK_LINE ,
  ARN5_ETH_SUB_BLOCK_ALL ,
} arn5_eth_sub_block_t ;

typedef enum {
  ARN5_NO_PAUSE = ARN5_PS_NOPAUSE ,
  ARN5_SYMMETRIC_PAUSE = ARN5_PS_SYMPAUSE ,
  ARN5_ASYMMETRIC_PAUSE = ARN5_PS_ASYMPAUSE ,
  ARN5_SYM_N_ASYM_PAUSE = ARN5_PS_PAUSE ,
} arn5_eth_pause_ability_t ;

typedef enum {
  ARN5_FS_LINK_OK = ARN5_RmtFault_OK ,
  ARN5_FS_LINK_FAIL = ARN5_RmtFault_LINKFAIL ,
  ARN5_FS_OFFLINE = ARN5_RmtFault_OFFLINE ,
  ARN5_FS_AUTO_NEGOTIATION_ERR = ARN5_RmtFault_ANERROR ,
} arn5_eth_fault_state_t ;

typedef enum {
  ARN5_GPIO_8 ,
  ARN5_GPIO_9 ,
  ARN5_GPIO_10 ,
  ARN5_GPIO_11 ,
} arn5_eth_gpio_chan_t ;

typedef enum {
  ARN5_ETH_STATUS_LINK_STAT = ARN5_GPIO_ALARM_STAT_SEL_LINKSTAT,
  ARN5_ETH_STATUS_TX_ACTIVITY = ARN5_GPIO_ALARM_STAT_SEL_TXACTV,
  ARN5_ETH_STATUS_RX_ACTIVITY = ARN5_GPIO_ALARM_STAT_SEL_RXACTV,
} arn5_eth_status_sel_t ;

typedef enum {
  ARN5_PAUSE_SRC_SYS_INF_BACKPRESSURE = ARN5_TX_PAUSE_SRC_SEL_SI_BP,
  ARN5_PAUSE_SRC_PAUSE_CONTROL_INF = ARN5_TX_PAUSE_SRC_SEL_PAUSE_IF,
  ARN5_PAUSE_SRC_NO_PAUSE = ARN5_TX_PAUSE_SRC_SEL_NOPAUSE,
} arn5_tx_pause_src_t ;


typedef struct {
  cs_boolean                    next_page ;
  cs_boolean                    ack ;
  arn5_eth_fault_state_t        fault_state ;
  arn5_eth_pause_ability_t      pause_ability ;
  cs_boolean                    half_duplex ;
  cs_boolean                    full_duplex ;
} arn5_eth_ability_t ;

typedef struct {
  cs_boolean                    promiscuous ;
  cs_mac                        mac_addr ;
  cs_boolean                    tx_addr_insert ;
  cs_boolean                    len_check ;
  cs_boolean                    padding_removal ;
  cs_boolean                    rx_crc_fwd ;
  cs_boolean                    tx_crc_fwd ;
} arn5_eth_ethnet_mode_cfg_t ;

typedef struct {
  cs_boolean                    pause_ignore ;
  cs_boolean                    pause_frame_forward ;
  cs_boolean                    user_mac_frame_forward ;
} arn5_eth_mac_ctl_frame_cfg_t ;

typedef struct {
  cs_boolean                    enable ;
  cs_uint32                     link_timer ;  /* in microseconds (usec) */
} arn5_eth_auto_neg_cfg_t ;


typedef struct {
  cs_boolean                    rpr_mode ;
  arn5_eth_mac_ctl_frame_cfg_t  ctl_frame_cfg ;
  cs_uint16                     max_len ;
  arn5_eth_ethnet_mode_cfg_t    ethnet_mode_cfg ;       /* only used in the ethernet mode */
  cs_uint16                     pause_quanta ;
  arn5_eth_pause_ability_t      pause_ability ;
  arn5_eth_auto_neg_cfg_t       auto_neg_cfg ;
  cs_boolean                    rx_data_inv ;
  cs_boolean                    tx_data_inv ;
  cs_boolean                    rx2tx_lb ;
  cs_boolean                    tx2rx_lb ;
  arn5_eth_status_sel_t         gpio_sel ;
  cs_uint16                     irq_ctl_tbl[ARN5_ETH_MAX_GRP] ;
} arn5_eth_cfg_t;

typedef struct {
  cs_boolean                    rpr_mode ;
  arn5_eth_mac_ctl_frame_cfg_t  ctl_frame_cfg ;
  cs_uint16                     max_len ;
  arn5_eth_ethnet_mode_cfg_t    ethnet_mode_cfg ;       /* only used in the ethernet mode */
  cs_uint16                     pause_quanta ;
  arn5_eth_pause_ability_t      pause_ability ;
  arn5_eth_fault_state_t        fault_state ;
  arn5_eth_auto_neg_cfg_t       auto_neg_cfg ;
  cs_boolean                    rx_data_inv ;
  cs_boolean                    tx_data_inv ;
  cs_boolean                    rx_path_enbl ;
  cs_boolean                    tx_path_enbl ;
  cs_boolean                    rx_bswap ;
  cs_boolean                    tx_bswap ;
  cs_boolean                    rx2tx_lb ;
  cs_boolean                    tx2rx_lb ;
} arn5_eth_runcfg_t ;


#endif /* __ARN5_ETH_DATA_H__ */
