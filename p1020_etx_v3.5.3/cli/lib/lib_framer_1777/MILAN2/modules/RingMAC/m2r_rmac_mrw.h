/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_mrw.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_RMAC_MRW_H__
#define __M2R_RMAC_MRW_H__

#include "cs_types.h"
#include "m2r_public.h"

#define M2R_RMAC_MAX_REWRITE_SRAM_IDX   (0x3ff)
#define M2R_RMAC_REWRITE_SRAM_SIZE      (0x0400)

/*
 * Protocol values
 */
#define M2R_RMAC_PROTO_SRP_CONTROL      (0x2007)
#define M2R_RMAC_PROTO_IPV4_HDLC        (0x0800)
#define M2R_RMAC_PROTO_IPV4_PPP         (0x0021)
#define M2R_RMAC_PROTO_TAG_HDLC         (0x8847)
#define M2R_RMAC_PROTO_TAG_PPP          (0x0281)
#define M2R_RMAC_PROTO_TAG_HDLC_MCAST   (0x8848)
#define M2R_RMAC_PROTO_TAG_PPP_MCAST    (0x0283)
#define M2R_RMAC_PROTO_IPV6_HDLC        (0x0057)
#define M2R_RMAC_PROTO_IPV6_PPP         (0x86DD)


typedef enum {
  M2R_RMAC_RPR_FIELD_WE     = 0x01,
  M2R_RMAC_RPR_FIELD_STRICT = 0x02,
  M2R_RMAC_RPR_FIELD_WS     = 0x04,
  M2R_RMAC_RPR_FIELD_PRIO   = 0x08,
  M2R_RMAC_RPR_FIELD_DA     = 0x10,
  M2R_RMAC_RPR_FIELD_SA     = 0x20,
  M2R_RMAC_RPR_FIELD_ALL    = 0x3f
} m2r_rmac_rpr_hdr_field_t ;

typedef enum {
  M2R_RMAC_NO_FLOOD_TTL         = 0x01,
  M2R_RMAC_NO_FLOOD_TTL_BASE    = 0x02,
  M2R_RMAC_UNI_FLOOD_TTL        = 0x04,
  M2R_RMAC_UNI_FLOOD_TTL_BASE   = 0x08,
  M2R_RMAC_BI_FLOOD_TTL         = 0x10,
  M2R_RMAC_BI_FLOOD_TTL_BASE    = 0x20,
  M2R_RMAC_TTL_TYPE_ALL         = 0x3f
} m2r_rmac_rpr_ttl_t ;

/***********************************************************************
 * RPR TTL FIELD RE-WRITE MODES
 ***********************************************************************/
typedef enum {
  M2R_RMAC_RPR_TTL_NO_CHANGE    = 0,
  M2R_RMAC_RPR_TTL_DA_SRAM,
  M2R_RMAC_RPR_TTL_REG,
  M2R_RMAC_RPR_TTL_BASE
} m2r_rmac_rpr_hdr_ttl_mode_t ;

typedef struct {
  cs_mac        da_addr ;
  cs_uint8      prio ;
  cs_uint8      ttl ;
} m2r_rmac_rewrite_entry_t ;

typedef enum {
  M2R_RMAC_DROP_TSN_MISMATCH = 0,
  M2R_RMAC_UPDATE_TSN,
  M2R_RMAC_STRIP_IDLE
} m2r_rmac_idle_pkt_ctl_t ;

/*******************************************************************
 * APIs
 *******************************************************************/
extern
cs_status m2r_rmac_ctl_rewrite(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_status m2r_rmac_rewrite_add_entry_da(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 mac5_4,
                                       cs_uint16 mac3_2,
                                       cs_uint16 mac1_0,
                                       cs_uint8 ttl) ;

extern
cs_status m2r_rmac_rewrite_add_entry_priority(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint8 priority) ;

extern
cs_status m2r_rmac_rewrite_add_entry(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 mac5_4,
                                       cs_uint16 mac3_2,
                                       cs_uint16 mac1_0,
                                       cs_uint8 priority,
                                       cs_uint8 ttl) ;

extern
cs_status m2r_rmac_rewrite_add_entries(cs_uint16 port_id,
                                       cs_uint16 start_idx,
                                       cs_uint16 num,
                                       m2r_rmac_rewrite_entry_t * tbl) ;

extern
cs_uint16 m2r_rmac_rewrite_get_entry(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 num,
                                       m2r_rmac_rewrite_entry_t * tbl) ;

extern
cs_uint16 m2r_rmac_rewrite_dump_entry(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 num) ;

extern
cs_status m2r_rmac_rewrite_reset_entries(cs_uint16 port_id,
                                        cs_uint16 sram_index,
                                        cs_uint16 num) ;

extern
cs_status m2r_rmac_rewrite_set_protocol(cs_uint16 port_id,
                                        cs_uint8  index,
                                        cs_uint16 protocol) ;

extern
cs_status m2r_rmac_rewrite_set_rpr_hdr_field(cs_uint16 port_id,
                                        cs_uint16 index,
                                        m2r_rmac_rpr_hdr_field_t field,
                                        cs_uint8  field_val) ;

extern
cs_status m2r_rmac_ctl_rpr_hdr_field_rewrite(cs_uint16 port_id,
                    m2r_rmac_rpr_hdr_field_t field, m2r_ctl_t ctl) ;

extern
cs_status m2r_rmac_rewrite_set_ipda(cs_uint16 port_id,
                              cs_uint16 index, cs_uint8  ipda_val) ;

extern
cs_status m2r_rmac_rewrite_set_cos_offset(cs_uint16 port_id,
                             cs_uint16 index, cs_uint8  offset_val) ;

extern
cs_status m2r_rmac_rewrite_force_strict(cs_uint16 port_id,
                                        m2r_ctl_t ctl) ;

extern
cs_status m2r_rmac_rewrite_set_sa_mac(cs_uint16 port_id,
                                      cs_uint16 sa_mac_4_5,
                                      cs_uint16 sa_mac_2_3,
                                      cs_uint16 sa_mac_0_1,
                                      cs_boolean rewrite) ;

extern
cs_status m2r_rmac_rewrite_set_ttl(cs_uint16 port_id,
                             cs_uint32 ttl_type, cs_uint8 ttl_val) ;

extern
cs_status m2r_rmac_rewrite_ctl_ttl_mode(cs_uint16 port_id,
                                 cs_uint32 ttl_type,
                                 m2r_rmac_rpr_hdr_ttl_mode_t mode) ;

extern
cs_status m2r_rmac_rewrite_set_pkt_type(cs_uint16 port_id,
                                        cs_uint8  pktType) ;

extern
cs_status m2r_rmac_rewrite_set_tsn(cs_uint16 port_id, cs_uint8  tsn) ;

extern
cs_uint8 m2r_rmac_rewrite_get_tsn(cs_uint16 port_id) ;

extern
cs_status m2r_rmac_rewrite_ctl_tsn_chk(cs_uint16 port_id,
                         m2r_rmac_idle_pkt_ctl_t op, m2r_ctl_t ctl) ;

#endif /* __M2R_RMAC_MRW_H__ */

