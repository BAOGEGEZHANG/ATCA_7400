/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_debug.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file and contains some top-level debug
 * utilities.
 */

#ifndef __M2R_DEBUG_H__
#define __M2R_DEBUG_H__

#include  "cs_types.h"
#include  "m2r_public.h"
#include  "m2r_gen.h"


typedef enum {
  DATA_PATTERN_INCR_BYTE	= 0,
  DATA_PATTERN_DECR_BYTE,
  DATA_PATTERN_REPEATING
  /* ,
  DATA_PATTERN_RANDOM, */
} m2r_data_pattern_t ;


CS_INLINE void M2R_ADD_TO_MAC_ADDR(cs_uint16 * pbytes_54,
				   cs_uint16 * pbytes_32,
				   cs_uint16 * pbytes_10,
				   cs_uint32 incr)
{
  cs_uint32 low_wrd = 0, saved_wrd = 0 ;

  low_wrd = ( (((cs_uint32) *pbytes_32) << 16) |
	      ((cs_uint32) *pbytes_10) ) ;
  saved_wrd = low_wrd ;
  low_wrd += incr ;
  if (low_wrd < saved_wrd) (*pbytes_54)++ ;
  *pbytes_32 = (cs_uint16)(low_wrd >> 16) ;
  *pbytes_10 = (cs_uint16)(low_wrd & 0xffff) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_ADD_TO_MAC_ADDR(pbytes_54, pbytes_32, pbytes_10, incr) ;
  }
#endif
}


extern
cs_status  m2r_port_fast_stats_init(cs_uint16 port_id) ;

extern
cs_status  m2r_port_debug_init(cs_uint16 port_id,
                               m2r_port_mac_mode_t mac_mode,
                               m2r_port_line_rate_t line_rate,
                               m2r_port_prot_t l2_prot,
                               m2r_port_lif_mode_t lifCfg,
                               m2r_rmac_ms_mode_t rmac_ms_mode) ;

#ifdef POSIX_PTHREAD
/* Feature #c -> */
extern
cs_status m2r_start_debug_thread(cs_uint16 port_id,
				 cs_uint32 msec_poll_intvl,
				 cs_boolean send_rcvd_ctl_pkts,
				 cs_boolean send_rcvd_ips_pkts,
				 cs_boolean get_ips_msg_in_ttl_mode,
				 cs_boolean blk_print_stats,
				 cs_uint32  blk_irq_poll) ;
extern
cs_status m2r_stop_debug_thread(cs_uint16 port_id) ;
/* Feature #c <- */
#endif

extern
cs_status m2r_dump_port_cb(cs_uint16 port_id) ;
extern
cs_uint16  m2r_get_crc16(cs_uint8 * buf, cs_uint32 len) ;
extern
cs_uint32 m2r_get_crc32(cs_uint8 * buf, cs_uint32 len) ;
extern
cs_uint16  m2r_get_srp_hdr(cs_uint8 ttl, cs_uint8 ri, cs_uint8 mode,
                           cs_uint8 prio) ;
extern
cs_uint16  m2r_get_rpr_hdr(cs_uint8 ttl, cs_uint8 ri, cs_uint8 fe,
                           cs_uint8 pt, cs_uint8 sc,
                           cs_uint8 we) ;
extern
cs_uint16  m2r_get_rpr_ext_hdr(cs_uint8 ttl, cs_uint8 ext, cs_uint8 ft,
                               cs_uint8 ws, cs_uint8 strict) ;
extern
cs_status  m2r_debug_send_test_pkt_via_ctl_buffer(cs_uint16 port_id,
						  cs_uint16 hdr,
						  cs_uint16 da_54,
						  cs_uint16 da_32,
						  cs_uint16 da_10,
						  cs_uint16 sa_54,
						  cs_uint16 sa_32,
						  cs_uint16 sa_10,
						  cs_uint16 ext_hdr,
						  cs_uint16 proto,
						  cs_uint8  ctl_type,
						  cs_uint8  ctl_ver,
						  cs_uint16 len,
						  m2r_data_pattern_t pat,
                                                  cs_uint32 rep_data,
						  cs_uint16 num_pkts) ;
extern
cs_status  m2r_debug_send_extended_rpr_pkts(cs_uint16 port_id,
					    cs_uint16 rpr_hdr,
					    cs_uint16 rpr_da_54,
					    cs_uint16 rpr_da_32,
					    cs_uint16 rpr_da_10,
					    cs_uint16 rpr_sa_54,
					    cs_uint16 rpr_sa_32,
					    cs_uint16 rpr_sa_10,
					    cs_uint16 ext_hdr,
					    cs_uint16 eth_da_54,
					    cs_uint16 eth_da_32,
					    cs_uint16 eth_da_10,
					    cs_uint16 eth_sa_54,
					    cs_uint16 eth_sa_32,
					    cs_uint16 eth_sa_10,
					    cs_uint16 eth_da_stride,
					    cs_uint16 eth_sa_stride,
					    cs_uint16 ether_type,
					    cs_boolean vlan_pkt,
					    cs_uint8  user_prio,
					    cs_uint16 vlan_id,
					    m2r_data_pattern_t pat,
					    cs_uint32 rep_data,
					    cs_uint16 len,
					    cs_uint16 num_pkts) ;
extern
void m2r_pkt_decode_dump(m2r_port_mac_mode_t mac_mode, cs_uint8 * buf,
			 cs_uint16 pkt_len) ;

extern
void m2r_ctl_clear_rmac_mems(cs_ctl_t ctl) ;

#endif /* __M2R_DEBUG_H__ */
