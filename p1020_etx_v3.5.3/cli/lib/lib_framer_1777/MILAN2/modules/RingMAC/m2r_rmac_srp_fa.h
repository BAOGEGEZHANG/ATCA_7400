/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systgems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_srp_fa.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * Header file for SRP standard compliant fairness-related definitions.
 */
#ifndef __M2R_RMAC_SRP_FA_H__
#define __M2R_RMAC_SRP_FA_H__

#include "cs_types.h"
#include "m2r_public.h"

typedef struct {
  cs_uint8   lp_my_usage ; /* real value is power of 2 by itself */
  cs_uint8   lp_fwd_rate ; /* real value is power of 2 by itself */
  cs_uint8   lp_allow ; /* real value is power of 2 by itself */
  cs_uint32  decay_interval ; /* bytes in decay period */
  cs_uint32  max_usage ; /* bytes per ageCoef decay_interval */
  cs_uint8   agecoeff ; /* real value is power of 2 by itself */
} m2r_rmac_srp_fa_cfg_t ;

#define M2R_RMAC_SRP_MTU	9216

#endif /* __M2R_RMAC_SRP_FA_H__ */


