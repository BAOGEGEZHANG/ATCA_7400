/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_pkt.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_FRAMER_PKT_H__
#define __M2R_FRAMER_PKT_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_framer_cmn.h"

extern
cs_status  m2r_framer_pkt_ctl_crc32(cs_uint16 port_id,
                    m2r_framer_stream_t channel, cs_boolean crc32) ;

extern
cs_status  m2r_framer_pkt_ctl_fcs(cs_uint16 port_id,
                  m2r_framer_stream_t channel, cs_boolean fcs_enbl) ;

extern
cs_status  m2r_framer_pkt_pass_fcs(cs_uint16 port_id,
                       m2r_framer_stream_t channel, m2r_ctl_t ctl) ;

extern
cs_status  m2r_framer_pkt_set_fill_threshold(cs_uint16 port_id,
                   m2r_framer_stream_t channel, cs_uint16 thrshold) ;

extern
cs_status  m2r_framer_pkt_ctl_drop(cs_uint16 port_id,
                                   m2r_framer_stream_t channel,
                                   m2r_dir_t dir, cs_boolean ctl) ;

extern
cs_status  m2r_framer_pkt_drop_long(cs_uint16 port_id,
                                m2r_framer_stream_t channel,
                                m2r_dir_t dir, cs_boolean ctl) ;

extern
cs_status  m2r_framer_pkt_drop_short(cs_uint16 port_id,
                            m2r_framer_stream_t channel,
                            m2r_dir_t dir, cs_boolean ctl) ;

extern
cs_status  m2r_framer_pkt_ctl_scrambling(cs_uint16 port_id,
                        m2r_framer_stream_t channel, m2r_ctl_t ctl) ;

#endif /* __M2R_FRAMER_PKT_H__ */

