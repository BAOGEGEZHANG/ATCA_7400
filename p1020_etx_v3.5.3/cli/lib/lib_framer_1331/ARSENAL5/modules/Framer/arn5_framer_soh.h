/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_soh.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_FRAMER_SOH_H__
#define __ARN5_FRAMER_SOH_H__

#include "cs_types.h"
#include "arn5_public.h"

/***************************************************************
 * Sonet Overhead bytes
 ***************************************************************/
typedef enum {
  ARN5_FR_OVRHD_C2 = 0,
  ARN5_FR_OVRHD_G1,
  ARN5_FR_OVRHD_K1,
  ARN5_FR_OVRHD_K2,
  ARN5_FR_OVRHD_K3,
  ARN5_FR_OVRHD_K4,
  ARN5_FR_OVRHD_S1,
  ARN5_FR_OVRHD_Z2,
  ARN5_FR_OVRHD_Z3,
  ARN5_FR_OVRHD_Z4,
  ARN5_FR_OVRHD_Z5,
  ARN5_FR_OVRHD_F1,
  ARN5_FR_OVRHD_E1,
  ARN5_FR_OVRHD_E2,
  ARN5_FR_OVRHD_SDCC,    /* Section DCC channels */
  ARN5_FR_OVRHD_LDCC,    /* Line DCC channels.   */
  ARN5_FR_OVRHD_J0,
  ARN5_FR_OVRHD_J1,
} arn5_framer_ovrhd_byte_t ;

#define ARN5_FR_MAX_OVRHD_BYTE_TYPE      (ARN5_FR_OVRHD_J1 + 1)


extern  cs_status arn5_framer_cfg_dcc_port(cs_uint16 port_id,
                        arn5_framer_dcc_port_mode_t  sel,
                        cs_dir_t  direction,
                        cs_ctl_t ctl) ;
extern
cs_status arn5_framer_set_overheadByte(cs_uint16 port_id,
                        arn5_framer_ovrhd_byte_t ovrhd,
                        cs_uint8 val) ;
extern
cs_uint8  arn5_framer_get_overheadByte(cs_uint16 port_id,
                        arn5_framer_ovrhd_byte_t ovrhd) ;
extern
cs_status arn5_framer_set_rx_path_label(cs_uint16 port_id,
                        cs_uint8 c2_byte) ;
extern
cs_status arn5_framer_set_tx_ovrhd_src(cs_uint16 port_id,
                        arn5_framer_ovrhd_byte_t ovrhd,
                        arn5_framer_tx_ovrhd_src_t src) ;
extern
cs_status arn5_framer_fill_trace_buf(cs_uint16 port_id,
                        cs_boolean sectionTrace,
                        cs_uint8 * traceMsg,
                        cs_dir_t dir) ;
extern
cs_status arn5_framer_get_trace_msg(cs_uint16 port_id,
                        cs_boolean sectionTrace,
                        cs_uint8 * traceMsg) ;
extern
cs_status arn5_framer_set_rx_trace_mode(cs_uint16 port_id,
                        cs_boolean sectionTrace,
                        cs_boolean chkMode,
                        cs_boolean shortTr) ;
extern
cs_status arn5_framer_sel_tx_trace_msg_src(cs_uint16 port_id,
                        cs_boolean sectionTrace,
                        arn5_framer_tx_trace_src_t src) ;
extern
cs_status arn5_framer_set_Z0_mode(cs_uint16 port_id, cs_boolean fixed,
                        cs_uint8 Z0_byte) ;
extern
cs_status arn5_framer_set_signal_thresholds(cs_uint16 port_id,
                        arn5_framer_signal_ber_t sd_declare,
                        arn5_framer_signal_ber_t sd_clear,
                        arn5_framer_signal_ber_t sf_declare,
                        arn5_framer_signal_ber_t sf_clear) ;
extern
cs_status arn5_framer_set_path_signal_thresholds(cs_uint16 port_id,
                        arn5_framer_signal_ber_t sf_declare,
                        arn5_framer_signal_ber_t sf_clear) ;


#endif /* __ARN5_FRAMER_SOH_H__ */
