/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systgems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_rpr_fa.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * Header file for RPR standard compliant fairness-related definitions.
 */
#ifndef __M2R_RMAC_RPR_FA_H__
#define __M2R_RMAC_RPR_FA_H__

#include "cs_types.h"
#include "m2r_public.h"

typedef struct {
  /* Input variable won't put into this structure (?)
  cs_uint8   hopsToDsWrap ;
  cs_uint8   numStations ;
   */

  /* System variables won't put into this structure (?)
  cs_boolean roundUpActiveWeights ; // always TRUE for Cortina 
  cs_uint32  agingInterval ; // determined by link rate
   */

  cs_boolean myDualQueueStation ;
  cs_boolean conservativeRateAdjustment ;
  cs_boolean activeWeightsDetection ;
  cs_uint8   activeWeightsCoef ;
  cs_uint32  advertisementRatio ; /* in units of 0.00001 */
  cs_uint8   ageCoef ; /* real value is power of 2 by itself */
  cs_uint8   localWeight ; 
  cs_uint8   lpCoef ; /* real value is power of 2 by itself */
  cs_uint32  reservedRate ; /* in uints of Mbps */
  cs_uint32  maxAllowedRate ; /* bytes per ageCoef agingInterval */
  cs_uint8   rampUpCoef ; /* real value is power of 2 by itself */
  cs_uint8   rampDnCoef ; /* real value is power of 2 by itself */
  cs_uint8   starveCoef ; /* used to get starveFactor,          */
                          /* Here is equation:                  */
                          /* [starveFactor] = 1/2**[starveCoef],*/
                          /* where [starveCoef] is from [0, 7]  */
  cs_uint8   keepaliveTimeoutValue ; /* in units of msec */
  cs_uint8   reportCoef ;
} m2r_rmac_rpr_fa_cfg_t ;

#define M2R_RMAC_RPR_MTU	9216

#endif /* __M2R_RMAC_RPR_FA_H__ */


