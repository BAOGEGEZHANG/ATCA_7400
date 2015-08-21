/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_alm.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_FRAMER_ALM_H__
#define __ARN5_FRAMER_ALM_H__

#include "cs_types.h"
#include "arn5_public.h"

/***************************************************************
 * Sonet Alarm and defects
 ***************************************************************/
typedef enum {
  ARN5_FR_PATH_DEF_PTU        = 0x0001,
  ARN5_FR_PATH_DEF_PTM        = 0x0002,
  ARN5_FR_PATH_DEF_PSLU       = 0x0004,
  ARN5_FR_PATH_DEF_PSLM       = 0x0008,
  ARN5_FR_PATH_DEF_UNEQP      = 0x0010,
  ARN5_FR_PATH_DEF_LOP        = 0x0020,
  ARN5_FR_PATH_DEF_AIS        = 0x0040,
  ARN5_FR_PATH_DEF_RDI        = 0x0080,

  ARN5_FR_PATH_DEF_ALL        = 0x00ff
} arn5_framer_path_defect_t ;

typedef enum {
  ARN5_FR_SECT_LINE_DEF_STM   = 0x0001,
  ARN5_FR_SECT_LINE_DEF_STU   = 0x0002,
  ARN5_FR_SECT_LINE_DEF_BERSF = 0x0004,
  ARN5_FR_SECT_LINE_DEF_SD    = 0x0008,
  ARN5_FR_SECT_LINE_DEF_AIS   = 0x0010,
  ARN5_FR_SECT_LINE_DEF_RDI   = 0x0020,
  ARN5_FR_SECT_LINE_DEF_LOF   = 0x0040,
  ARN5_FR_SECT_LINE_DEF_SEF   = 0x0080,
  ARN5_FR_SECT_LINE_DEF_LOS   = 0x0100,
  ARN5_FR_SECT_LINE_DEF_SF    = 0x0200,

  ARN5_FR_SECT_LINE_DEF_ALL   = 0x03ff
} arn5_framer_sect_line_defect_t ;

typedef enum {
  ARN5_FR_ALM_AIS_P,
  ARN5_FR_ALM_RDI_P,
  ARN5_FR_ALM_REI_P,
  ARN5_FR_ALM_AIS_L,
  ARN5_FR_ALM_RDI_L,
  ARN5_FR_ALM_REI_L,

  ARN5_FR_MAX_ALM_TYPE
} arn5_framer_alm_t ;

#define MAX_FR_ALARM_TYPE       (ARN5_FR_MAX_ALM_TYPE)


typedef enum {
  ARN5_FR_RXALMPORT_LOS      = 0x0,
  ARN5_FR_RXALMPORT_LOF      = 0x1,
  ARN5_FR_RXALMPORT_AIS_L    = 0x2,
  ARN5_FR_RXALMPORT_SF       = 0x3,
  ARN5_FR_RXALMPORT_AIS_P    = 0x8,
  ARN5_FR_RXALMPORT_LOP_P    = 0xc,
} arn5_framer_rxalmport_src_t ;


extern
cs_status arn5_framer_alm_set_alm_src(cs_uint16 port_id,
                        arn5_framer_alm_t alarm,
                        arn5_framer_alm_src_t src) ;
extern
cs_status arn5_framer_alm_insert_line_alarm(cs_uint16 port_id,
                        arn5_framer_alm_t alm_type,
                        cs_uint8 b2_err_cnt,
                        cs_ctl_t ctl) ;
extern
cs_status arn5_framer_alm_insert_path_alarm(cs_uint16 port_id,
                        arn5_framer_alm_t alm_type,
                        cs_uint8 b3_err_cnt,
                        cs_ctl_t ctl) ;
extern
cs_status arn5_framer_alm_ctl_enh_rdi_p_mode(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_status arn5_framer_alm_ctl_auto_ins_mode(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status arn5_framer_insert_rdi_p_on_defect(cs_uint16 port_id,
                        arn5_framer_path_defect_t defect,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_status arn5_framer_drop_pld_on_path_defect(cs_uint16 port_id,
                        arn5_framer_path_defect_t defect,
                        cs_ctl_t ctl) ;
extern
cs_uint16 arn5_framer_alm_get_path_status(cs_uint16 port_id) ;
extern
cs_status arn5_framer_insert_rdi_l_on_defect(cs_uint16 port_id,
                        arn5_framer_sect_line_defect_t defect,
                        cs_ctl_t ctl) ;
extern
cs_status arn5_framer_insert_ais_l_on_defect(cs_uint16 port_id,
                        arn5_framer_sect_line_defect_t defect,
                        cs_ctl_t ctl) ;
extern
cs_status arn5_framer_drop_pld_on_line_defect(cs_uint16 port_id,
                        arn5_framer_sect_line_defect_t defect,
                        cs_ctl_t ctl) ;
extern
cs_uint16 arn5_framer_alm_get_sect_line_status(cs_uint16 port_id) ;
extern
cs_status arn5_framer_alm_sel_rxalmport_src(cs_uint16 port_id,
                        arn5_framer_rxalmport_src_t sel) ;

#endif /* __ARN5_FRAMER_ALM_H__ */
