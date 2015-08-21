/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_atm.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_FRAMER_ATM_H__
#define __ARN5_FRAMER_ATM_H__

#include "cs_types.h"
#include "arn5_public.h"

typedef enum {
  ARN5_FR_ATM_OOS_THR    = 0,    /* out of sync thresold         */
  ARN5_FR_ATM_IS_THR,            /* In sync threshold            */
  ARN5_FR_ATM_ERR_CORR_THR,      /* Error correction threshold   */
} arn5_framer_atm_thr_t ;

/* ATM Cell processor related APIs. */
extern
cs_status  arn5_framer_atm_drop_cells(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_dir_t dir) ;

extern
cs_status  arn5_framer_atm_drop_idle(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_atm_ctl_hec(cs_uint16 port_id,
                        cs_ctl_t ctl ) ;
extern
cs_status  arn5_framer_atm_ctl_scrambling(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_atm_ctl_err_correction(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_atm_set_thr_val(cs_uint16 port_id,
                        arn5_framer_atm_thr_t thr,
                        cs_uint8  value) ;

#endif /* __ARN5_FRAMER_ATM_H__ */

