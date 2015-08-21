/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_public.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains basic API argument data types, defines, etc..
 */
#ifndef __M2R_PUBLIC_H__
#define __M2R_PUBLIC_H__

#include "cs_types.h"

#define CS_MAX_NUM_DEVICES      (2)


typedef enum {
  M2R_DISABLE	= CS_DISABLE,
  M2R_ENABLE	= CS_ENABLE
} m2r_ctl_t ;

typedef enum {
  M2R_RESET_DEASSERT	= CS_RESET_DEASSERT,
  M2R_RESET_ASSERT	= CS_RESET_ASSERT,
  M2R_RESET_TOGGLE	= CS_RESET_TOGGLE
} m2r_reset_action_t ;

typedef enum {
  M2R_TX	= CS_TX,
  M2R_RX        = CS_RX,
  M2R_TX_AND_RX = CS_TX_AND_RX,
  M2R_RX_AND_TX = M2R_TX_AND_RX
} m2r_dir_t ;

typedef enum {
  M2R_READ	= CS_OP_READ,
  M2R_WRITE     = CS_OP_WRITE
} m2r_rw_t ;

typedef enum {
  M2R_PORT_IF_SFI4_1    = 0,    /* LIF Mode */
  M2R_PORT_IF_SFI4_2,
  M2R_PORT_IF_XFP
} m2r_port_lif_mode_t ;

typedef enum {
  M2R_PORT_RATE_OC192C = 0,
  M2R_PORT_RATE_OC192,
  M2R_PORT_RATE_OC48C,
  M2R_PORT_RATE_OC48,
  M2R_PORT_RATE_10G
} m2r_port_line_rate_t ;

#define M2R_CORE_CLOCK	200 /* MHz */

/* On Palermo, M2R_PORT_IN_BYPASS_MODE is the ONLY valid option! */
typedef enum {
  M2R_PORT_IN_SRP_MODE   = 0,
  M2R_PORT_IN_RPR_MODE,
  M2R_PORT_IN_BYPASS_MODE       /* framer only           */
} m2r_port_mac_mode_t ;

typedef enum {
  M2R_PORT_PROT_POS = 0,
  M2R_PORT_PROT_PPP,
  M2R_PORT_PROT_ATM,
  M2R_PORT_PROT_GFP,
  M2R_PORT_PROT_CLEAR_CH,     /* clear channel */
  M2R_PORT_PROT_ETHERNET
} m2r_port_prot_t ;

typedef enum {
  M2R_ID_MPIF   = 0x0001,
  M2R_ID_SPI42  = 0x0002,
  M2R_ID_PPRBS  = 0x0004,
  M2R_ID_FRAMER = 0x0008,
  M2R_ID_RMAC   = 0x0010,
  M2R_ID_XGXS   = 0x0020,
  M2R_ID_LIF    = 0x0040,
  M2R_ID_ETH    = 0x0080,
  M2R_ID_ALL_BLOCKS = 0x00FF
} m2r_module_id_t ;
#define M2R_PORT_BLKS_ID      (M2R_ID_ALL_BLOCKS)


/*
 * Currently Cortina driver provides five types of M-CODE
 */
typedef enum {
  M2R_MS_SRP 	= 0,
  M2R_MS_RPR_AGGRESSIVE_DUAL_TB,
  M2R_MS_RPR_AGGRESSIVE_SINGLE_TB,
  M2R_MS_RPR_CONSERVATIVE_DUAL_TB,
  M2R_MS_RPR_CONSERVATIVE_SINGLE_TB,
  M2R_MS_UNKNOWN  /* use this for bypass(framer only) mode */
} m2r_rmac_ms_mode_t ;

typedef struct {
  m2r_port_mac_mode_t   mac_mode ;
  m2r_port_line_rate_t  line_rate ;
  m2r_port_prot_t       l2_prot ;
  m2r_port_lif_mode_t   lif_mode ;
  m2r_rmac_ms_mode_t    rmac_ms_mode ;
} m2r_port_summ_t ;

/*******************************************************************
 * STATISTICS DATA ( THIS IS ATMOIC ENTITY OF STATS INFRASTRUCTURE )
 *******************************************************************/
typedef struct {
  cs_uint64     tx ;    /* TX COUNTER VALUE */
  cs_uint64     rx ;    /* RX COUNTER VALUE */
} m2r_stats_data_t ;



#endif  /* __M2R_PUBLIC_H__ */









