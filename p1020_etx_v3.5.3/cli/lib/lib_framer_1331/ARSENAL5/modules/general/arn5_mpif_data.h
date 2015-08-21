/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_mpif_data.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_MPIF_DATA_H__
#define __ARN5_MPIF_DATA_H__

#include "cs_types.h"
#include "arn5_mpif_irq.h"


typedef struct {
  /* Default MPIF IRQ, mostly related to GPIO */
  cs_uint16 irq_ctl_tbl[ARN5_MPIF_MAX_GRP] ;
} arn5_mpif_cfg_t ;

#endif /* __ARN5_MPIF_DATA_H__ */
