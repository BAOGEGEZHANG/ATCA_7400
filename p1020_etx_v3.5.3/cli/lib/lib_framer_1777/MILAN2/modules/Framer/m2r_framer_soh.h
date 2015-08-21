/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_soh.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_FRAMER_SOH_H__
#define __M2R_FRAMER_SOH_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_framer_cmn.h"

/***************************************************************
 * Sonet Overhead bytes
 ***************************************************************/
typedef enum {
  M2R_FR_OVRHD_C2 = 0,
  M2R_FR_OVRHD_G1,
  M2R_FR_OVRHD_K1,
  M2R_FR_OVRHD_K2,
  M2R_FR_OVRHD_K3,
  M2R_FR_OVRHD_K4,
  M2R_FR_OVRHD_S1,
  M2R_FR_OVRHD_Z2,
  M2R_FR_OVRHD_Z3,
  M2R_FR_OVRHD_Z4,
  M2R_FR_OVRHD_Z5,
  M2R_FR_OVRHD_F1,
  M2R_FR_OVRHD_E1,
  M2R_FR_OVRHD_E2,
  M2R_FR_OVRHD_SDCC,    /* Section DCC channels */
  M2R_FR_OVRHD_LDCC,    /* Line DCC channels.   */
  M2R_FR_OVRHD_J0,
  M2R_FR_OVRHD_J1
} m2r_framer_ovrhd_byte_t ;

#define M2R_FR_MAX_OVRHD_BYTE_TYPE      (M2R_FR_OVRHD_J1 + 1)

/***************************************************************
 * Sonet APS Port Overhead bytes
 ***************************************************************/
typedef enum {
  M2R_FR_APS_PORT_insAISL,
  M2R_FR_APS_PORT_insRDIL,
  M2R_FR_APS_PORT_dSTU,
  M2R_FR_APS_PORT_dSTM,
  M2R_FR_APS_PORT_dRDIL,
  M2R_FR_APS_PORT_dSD,
  M2R_FR_APS_PORT_dAISL,
  M2R_FR_APS_PORT_dSF,
  M2R_FR_APS_PORT_dLOF,
  M2R_FR_APS_PORT_dSEF,
  M2R_FR_APS_PORT_dLOS,
  M2R_FR_APS_PORT_REIL,
  M2R_FR_APS_PORT_B2ErCnt,

  /* Path alarm, aps data for APS port */
  M2R_FR_APS_PORT_insRDIP,
  M2R_FR_APS_PORT_dLOPConcat,
  M2R_FR_APS_PORT_dPSLUI,
  M2R_FR_APS_PORT_dPSLM,
  M2R_FR_APS_PORT_dPTU,
  M2R_FR_APS_PORT_dPTM,
  M2R_FR_APS_PORT_dUNEQP,
  M2R_FR_APS_PORT_dRDIP,
  M2R_FR_APS_PORT_dPAIS,
  M2R_FR_APS_PORT_dLOP,

  M2R_FR_APS_PORT_REIP,
  M2R_FR_APS_PORT_B3ErCnt
} m2r_framer_aps_port_sect_line_data_t ;

extern  cs_status m2r_framer_cfg_dcc_port(cs_uint16 port_id,
                                          m2r_framer_dcc_port_mode_t  sel,
                                          m2r_dir_t  direction,
                                          m2r_ctl_t ctl) ;

extern  cs_status m2r_framer_ctl_aps_port(cs_uint16 port_id,
                                         m2r_aps_port_sel_t sel,
                                         m2r_dir_t  direction,
                                         m2r_ctl_t ctl) ;

extern  cs_status m2r_framer_sel_aps_port_src(cs_uint16 port_id,
                                      cs_boolean cpu_reg,
                                      m2r_dir_t  direction) ;

extern  cs_status m2r_framer_insert_aps_rx_port(cs_uint16 port_id,
                       m2r_framer_aps_port_sect_line_data_t aps_data,
                       cs_uint8 val, m2r_framer_stream_t channel) ;

extern  cs_status m2r_framer_set_overheadByte(cs_uint16 port_id,
                                      m2r_framer_ovrhd_byte_t ovrhd,
                              cs_uint8 val, m2r_framer_stream_t channel) ;

extern  cs_uint8  m2r_framer_get_overheadByte(cs_uint16 port_id,
                                    m2r_framer_ovrhd_byte_t ovrhd,
                                    m2r_framer_stream_t channel) ;

extern  cs_status m2r_framer_set_rx_path_label(cs_uint16 port_id,
                      cs_uint8 c2_byte, m2r_framer_stream_t channel) ;

extern  cs_status m2r_framer_set_tx_ovrhd_src(cs_uint16 port_id,
                                    m2r_framer_ovrhd_byte_t ovrhd,
                                    m2r_framer_tx_ovrhd_src_t src,
                                    m2r_framer_stream_t channel) ;

extern  cs_status m2r_framer_fill_trace_buf(cs_uint16 port_id,
                                    cs_boolean sectionTrace,
                                    cs_uint8 * traceMsg,
                                    m2r_framer_stream_t channel,
                                    m2r_dir_t dir) ;

extern  cs_status m2r_framer_get_trace_msg(cs_uint16 port_id,
                                    cs_boolean sectionTrace,
                                    cs_uint8 * traceMsg,
                                    m2r_framer_stream_t channel) ;

extern  cs_status m2r_framer_set_rx_trace_mode(cs_uint16 port_id,
                                       cs_boolean sectionTrace,
                                       cs_boolean chkMode,
                                       cs_boolean shortTr,
                                       m2r_framer_stream_t channel) ;

extern  cs_status m2r_framer_sel_tx_trace_msg_src(cs_uint16 port_id,
                                       cs_boolean sectionTrace,
                                       m2r_framer_tx_trace_src_t src) ;

extern  cs_status m2r_framer_set_Z0_mode(cs_uint16 port_id, cs_boolean fixed,
                                    cs_uint8 Z0_byte) ;

extern
cs_status m2r_framer_set_signal_thresholds(cs_uint16 port_id,
                               m2r_framer_signal_ber_t sd_declare,
                               m2r_framer_signal_ber_t sd_clear,
                               m2r_framer_signal_ber_t sf_declare,
                               m2r_framer_signal_ber_t sf_clear) ;

extern
cs_status m2r_framer_set_path_signal_thresholds(cs_uint16 port_id,
                               m2r_framer_signal_ber_t sf_declare,
                               m2r_framer_signal_ber_t sf_clear) ;

extern
cs_status m2r_framer_encode_trace_msg_util(cs_uint8 * raw_trace_msg,
                        cs_uint8 * encoded_trace_msg,
                        cs_uint8 len);

extern
cs_status m2r_framer_decode_trace_msg_util(cs_uint8 * raw_trace_msg,
                        cs_uint8 * decoded_trace_msg,
                        cs_uint8 len);

extern
cs_boolean m2r_framer_verify_trace_msg_util(cs_uint8 * trace_msg,
                        cs_uint8 len);


#endif /* __M2R_FRAMER_SOH_H__ */
