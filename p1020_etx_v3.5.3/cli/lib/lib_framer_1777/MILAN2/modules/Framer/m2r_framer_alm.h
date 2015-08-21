/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_alm.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_FRAMER_ALM_H__
#define __M2R_FRAMER_ALM_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_framer_cmn.h"

/***************************************************************
 * Sonet Alarm and defects
 ***************************************************************/
typedef enum {
  M2R_FR_PATH_DEF_PTU        = 0x0001,
  M2R_FR_PATH_DEF_PTM        = 0x0002,
  M2R_FR_PATH_DEF_PSLU       = 0x0004,
  M2R_FR_PATH_DEF_PSLM       = 0x0008,
  M2R_FR_PATH_DEF_UNEQP      = 0x0010,
  M2R_FR_PATH_DEF_LOP        = 0x0020,
  M2R_FR_PATH_DEF_AIS        = 0x0040,
  M2R_FR_PATH_DEF_RDI        = 0x0080,

  M2R_FR_PATH_DEF_ALL        = 0x00ff
} m2r_framer_path_defect_t ;

typedef enum {
  M2R_FR_SECT_LINE_DEF_STM   = 0x0001,
  M2R_FR_SECT_LINE_DEF_STU   = 0x0002,
  M2R_FR_SECT_LINE_DEF_BERSF = 0x0004,
  M2R_FR_SECT_LINE_DEF_SD    = 0x0008,
  M2R_FR_SECT_LINE_DEF_AIS   = 0x0010,
  M2R_FR_SECT_LINE_DEF_RDI   = 0x0020,
  M2R_FR_SECT_LINE_DEF_LOF   = 0x0040,
  M2R_FR_SECT_LINE_DEF_SEF   = 0x0080,
  M2R_FR_SECT_LINE_DEF_LOS   = 0x0100,
  M2R_FR_SECT_LINE_DEF_SF    = 0x0200,

  M2R_FR_SECT_LINE_DEF_ALL   = 0x03ff
} m2r_framer_sect_line_defect_t ;

typedef enum {
  M2R_FR_ALM_AIS_P,
  M2R_FR_ALM_RDI_P,
  M2R_FR_ALM_REI_P,
  M2R_FR_ALM_AIS_L,
  M2R_FR_ALM_RDI_L,
  M2R_FR_ALM_REI_L,

  M2R_FR_MAX_ALM_TYPE
} m2r_framer_alm_t ;


#define MAX_FR_ALARM_TYPE       (M2R_FR_MAX_ALM_TYPE)


typedef enum {
  M2R_FR_RXALMPORT_LOS      = 0x0,
  M2R_FR_RXALMPORT_LOF      = 0x1,
  M2R_FR_RXALMPORT_AIS_L    = 0x2,
  M2R_FR_RXALMPORT_SF       = 0x3,
  M2R_FR_RXALMPORT_AIS_P0   = 0x8,
  M2R_FR_RXALMPORT_AIS_P1   = 0x9,
  M2R_FR_RXALMPORT_AIS_P2   = 0xa,
  M2R_FR_RXALMPORT_AIS_P3   = 0xb,
  M2R_FR_RXALMPORT_LOP_P0   = 0xc,
  M2R_FR_RXALMPORT_LOP_P1   = 0xd,
  M2R_FR_RXALMPORT_LOP_P2   = 0xe,
  M2R_FR_RXALMPORT_LOP_P3   = 0xf
} m2r_framer_rxalmport_src_t ;



extern
cs_status m2r_framer_alm_set_alm_src(cs_uint16 port_id,
                                    m2r_framer_alm_t alarm,
                                    m2r_framer_alm_src_t src) ;

extern
cs_status m2r_framer_alm_insert_line_alarm(cs_uint16 port_id,
                                           m2r_framer_alm_t alm_type,
                                           cs_uint8 b2_err_cnt,
                                           m2r_ctl_t ctl) ;

extern
cs_status m2r_framer_alm_insert_path_alarm(cs_uint16 port_id,
                                           m2r_framer_stream_t channel,
                                           m2r_framer_alm_t alm_type,
                                           cs_uint8 b3_err_cnt,
                                           m2r_ctl_t ctl) ;

extern
cs_status m2r_framer_alm_ctl_enh_rdi_p_mode(cs_uint16 port_id,
                                      m2r_framer_stream_t channel,
                                      m2r_dir_t dir, m2r_ctl_t ctl) ;

extern
cs_status m2r_framer_alm_ctl_auto_ins_mode(cs_uint16 port_id,
                                           m2r_ctl_t ctl) ;

extern
cs_status m2r_framer_insert_rdi_p_on_defect(cs_uint16 port_id,
                                m2r_framer_path_defect_t defect,
                                m2r_framer_stream_t channel,
                                m2r_dir_t dir, m2r_ctl_t ctl) ;

extern
cs_status m2r_framer_drop_pld_on_path_defect(cs_uint16 port_id,
                                m2r_framer_path_defect_t defect,
                                m2r_framer_stream_t channel,
                                m2r_ctl_t ctl) ;

extern
cs_uint16 m2r_framer_alm_get_path_status(cs_uint16 port_id,
                                         m2r_framer_stream_t channel) ;

extern
cs_status m2r_framer_insert_rdi_l_on_defect(cs_uint16 port_id,
                             m2r_framer_sect_line_defect_t defect,
                             m2r_ctl_t ctl) ;


extern
cs_status m2r_framer_insert_ais_l_on_defect(cs_uint16 port_id,
                             m2r_framer_sect_line_defect_t defect,
                             m2r_ctl_t ctl) ;

extern
cs_status m2r_framer_drop_pld_on_line_defect(cs_uint16 port_id,
                             m2r_framer_sect_line_defect_t defect,
                             m2r_ctl_t ctl) ;

extern
cs_uint16 m2r_framer_alm_get_sect_line_status(cs_uint16 port_id) ;

extern
cs_status m2r_framer_alm_sel_rxalmport_src(cs_uint16 port_id,
                        m2r_framer_rxalmport_src_t sel) ;

#endif /* __M2R_FRAMER_ALM_H__ */
