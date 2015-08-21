/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_pkt.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_FRAMER_PKT_H__
#define __ARN5_FRAMER_PKT_H__

#include "cs_types.h"
#include "arn5_public.h"

extern
cs_status  arn5_framer_pkt_ctl_crc32(cs_uint16 port_id,
                        cs_boolean crc32) ;
extern
cs_status  arn5_framer_pkt_ctl_fcs(cs_uint16 port_id,
                        cs_boolean fcs_enbl) ;
extern
cs_status  arn5_framer_pkt_pass_fcs(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_pkt_set_fill_threshold(cs_uint16 port_id,
                        cs_uint16 thrshold) ;
extern
cs_status  arn5_framer_pkt_ctl_drop(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean ctl) ;
extern
cs_status  arn5_framer_pkt_drop_long(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean ctl) ;
extern
cs_status  arn5_framer_pkt_drop_short(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean ctl) ;
extern
cs_status  arn5_framer_pkt_ctl_scrambling(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_pkt_set_frame_addr_n_ctl(cs_uint16 port_id,
                        cs_dir_t dir,
                        cs_uint8 addr, cs_uint8 ctl) ;
extern
cs_status  arn5_framer_pkt_ctl_frame_addr_n_ctl_check(cs_uint16 port_id,
                        cs_boolean check_addr, cs_boolean check_ctl) ;
extern
cs_status  arn5_framer_pkt_set_frame_sapi(cs_uint16 port_id,
                        cs_uint16 sapi) ;
extern
cs_status  arn5_framer_pkt_ctl_frame_addr_n_ctl_tx_insert(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_ctl_t sapi_ctl) ;
extern
cs_status  arn5_framer_pkt_ctl_frame_addr_n_ctl_rx_drop(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_ctl_t sapi_ctl) ;

#endif /* __ARN5_FRAMER_PKT_H__ */

