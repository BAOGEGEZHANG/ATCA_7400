/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth_api.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported api header file for ETH module.
 */
#ifndef __ARN5_ETH_API_H__
#define __ARN5_ETH_API_H__

#include "cs_types.h"
#include "arn5_public.h"



/****************************************************************
 *******************    ETH PORT-LEVEL API's    *****************
 ****************************************************************/
extern
cs_status  arn5_eth_soft_reset(cs_uint16 port_id,
                        arn5_eth_sub_block_t sub_block_id) ;
extern
cs_status  arn5_eth_ctl_loopback(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean enbl) ;
extern
cs_status  arn5_eth_ctl_data_path(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t enbl) ;
extern
cs_status  arn5_eth_set_data_line_params(cs_uint16 port_id,
                        cs_boolean rx_data_inv, cs_boolean tx_data_inv) ;
extern
cs_status  arn5_eth_mac_ctl_frame_cfg(cs_uint16 port_id,
                        cs_boolean pause_ignore, cs_boolean pause_frame_forward,
                        cs_boolean user_mac_frame_forward) ;
extern
cs_status  arn5_eth_set_max_frame_len(cs_uint16 port_id,
                        cs_uint16 max_len) ;
extern
cs_status  arn5_eth_set_mac_address(cs_uint16 port_id,
                        cs_mac mac_addr) ;

extern
cs_status  arn5_eth_set_mac_address_bytes(cs_uint16 port_id,
                        cs_uint16 mac5_4, cs_uint16 mac3_2, cs_uint16 mac1_0) ;

extern
cs_status  arn5_eth_set_rpr_mode(cs_uint16 port_id,
                        cs_boolean rpr) ;
extern
cs_status  arn5_eth_ethnet_mode_cfg(cs_uint16 port_id,
                        cs_boolean promiscuous, cs_boolean tx_addr_insert,
                        cs_boolean len_check, cs_boolean padding_removal,
                        cs_boolean rx_crc_fwd, cs_boolean tx_crc_fwd) ;
extern
cs_status  arn5_eth_set_ability(cs_uint16 port_id,
                        arn5_eth_pause_ability_t pause_ability,
                        arn5_eth_fault_state_t fault_state) ;
extern
cs_status  arn5_eth_get_partner_ability(cs_uint16 port_id,
                        arn5_eth_ability_t * pAbility) ;
extern
cs_boolean  arn5_eth_get_link_status(cs_uint16 port_id) ;
extern
cs_boolean  arn5_eth_get_auto_negotiation_status(cs_uint16 port_id) ;
extern
cs_status  arn5_eth_auto_negotiation_cfg(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_uint32 link_timer) ;
extern
cs_status  arn5_eth_restart_auto_negotiation(cs_uint16 port_id) ;

extern
cs_status  arn5_eth_set_pause_quanta(cs_uint16 port_id,
                        cs_uint16 pause_quanta) ;
extern
cs_status  arn5_eth_generate_pause_frame(cs_uint16 port_id,
                        cs_boolean xon) ;
extern
cs_status  arn5_eth_sel_status_to_gpio(cs_uint16 port_id,
                        arn5_eth_status_sel_t sel) ;
extern
cs_status  arn5_eth_set_conv_tx_min_pkt_size(cs_uint16 port_id,
                        cs_uint8 min_size) ;
extern
cs_status  arn5_eth_sel_tx_pause_src(cs_uint16 port_id,
                        arn5_tx_pause_src_t src) ;
extern
cs_status  arn5_eth_ctl_conv_rx_drop_on_backpressure(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_eth_set_conv_tx_fifo_params(cs_uint16 port_id,
                        cs_uint8 pkt_skid, cs_uint8 threshold) ;

#endif /* __ARN5_ETH_H__ */


