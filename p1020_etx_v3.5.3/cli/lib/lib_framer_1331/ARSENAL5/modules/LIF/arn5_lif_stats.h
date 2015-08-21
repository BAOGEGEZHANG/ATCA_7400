/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_lif_stats.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for ARN5 LIF Statistics.
 */
#ifndef __ARN5_LIF_STATS_H__
#define __ARN5_LIF_STATS_H__

/************************************************************************
 * LIF Statistics structure
 ************************************************************************/
typedef struct {
  cs_uint32     unused ;
  /* No LIF counters are there - stats struct added for completeness */
} arn5_lif_stats_t ;

#endif /*  __ARN5_LIF_STATS_H__ */

