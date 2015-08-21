/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_cmn.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This contains structures and defines and other internal APIs used by
 * different blocks inside the framer like ATM/GFP and PKT processor.
 */
#ifndef __M2R_FRAMER_CMN_H__
#define __M2R_FRAMER_CMN_H__

#include "cs_types.h"

#define M2R_FR_MAX_STREAMS              (4)

/***********************************************************************
 * SONET STREAM
 ***********************************************************************/
typedef enum {
  M2R_SONET_STREAM0   = 0,
  M2R_SONET_STREAM1,
  M2R_SONET_STREAM2,
  M2R_SONET_STREAM3,
  M2R_SONET_STREAM_ALL
} m2r_framer_stream_t ;

#endif /* __M2R_FRAMER_CMN_H__ */

