/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_atm.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_FRAMER_ATM_H__
#define __M2R_FRAMER_ATM_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_framer_cmn.h"

typedef enum {
  M2R_FR_ATM_OOS_THR    = 0,    /* out of sync thresold         */
  M2R_FR_ATM_IS_THR,            /* In sync threshold            */
  M2R_FR_ATM_ERR_CORR_THR       /* Error correction threshold   */
} m2r_framer_atm_thr_t ;

/* ATM Cell processor related APIs. */
extern
cs_status  m2r_framer_atm_drop_cells(cs_uint16 port_id,
                                     m2r_framer_stream_t channel,
                                     m2r_ctl_t ctl, m2r_dir_t dir) ;

extern
cs_status  m2r_framer_atm_drop_idle(cs_uint16 port_id,
                                    m2r_framer_stream_t channel,
                                    m2r_ctl_t ctl) ;

extern
cs_status  m2r_framer_atm_ctl_hec(cs_uint16 port_id,
                                  m2r_framer_stream_t channel,
                                  m2r_ctl_t ctl ) ;


extern
cs_status  m2r_framer_atm_ctl_scrambling(cs_uint16 port_id,
                                         m2r_framer_stream_t channel,
                                         m2r_ctl_t ctl) ;

extern
cs_status  m2r_framer_atm_ctl_err_correction(cs_uint16 port_id,
                        m2r_framer_stream_t channel, m2r_ctl_t ctl) ;

extern
cs_status  m2r_framer_atm_set_thr_val(cs_uint16 port_id,
                                      m2r_framer_stream_t channel,
                                      m2r_framer_atm_thr_t thr,
                                      cs_uint8  value) ;

#endif /* __M2R_FRAMER_ATM_H__ */

