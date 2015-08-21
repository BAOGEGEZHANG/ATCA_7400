/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_cb.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PRIVATE header file - for driver use only!
 * Contains the control block and other internal data structures
 * and associated defines.
 *
 */
#ifndef __ARN5_CB_H__
#define __ARN5_CB_H__

#ifndef CS_DONT_USE_STDLIB
#       include <stdio.h>
#endif /* CS_DONT_USE_STDLIB */
#include "cs_types.h"
#include "cs_rtos.h"

#ifdef POSIX_PTHREAD
#include <pthread.h>
#endif

#include "arn5_public.h"
#include "arn5_stats_priv.h"
#include "arn5_irq_priv.h"


#define ARN5_INVALID_PORT_NUM      (0xFF) /* internal use only */
#define ARN5_MAX_NUM_PORTS         (8)    /* 8 slices */


/*************************************************************
 ************     PORT CONTROL BLOCK (port_cb)  **************
 *************************************************************/

/* Port states */
#define  ARN5_PORT_STATE_INVALID         (0) /* Pwron reset default */
#define  ARN5_PORT_STATE_OPENED          (1)
#define  ARN5_PORT_STATE_DATA_INIT_DONE  (2) /* data structs init'ed */
#define  ARN5_PORT_STATE_HW_INIT_DONE    (3) /* hw cfg done */

/*
 * semaphores
 */
typedef struct {
  CS_SEM        framer ;
  CS_SEM        lif ;
  CS_SEM        eth ;
  CS_SEM        pprbs ;
} arn5_port_sem_t ;


typedef struct {
  cs_uint16             port_id ;
  cs_uint32             state ;
  arn5_port_sem_t       sem ;   /* semaphores     */
  arn5_port_summ_t      summ ;  /* config summary */
  arn5_port_stats_cb_t  stats_cb ;
  arn5_port_stats_t     stats ;
  cs_boolean            acctg_stats_malloced ;
  arn5_port_irq_cb_t    irq ;
} arn5_port_cb_t ;


/*************************************************************
 *************    DEVICE CONTROL BLOCK (dev_cb)   ************
 *************************************************************/

#define  ARN5_DEV_STATE_INVALID          0
#define  ARN5_DEV_STATE_REGISTERED       1
#define  ARN5_DEV_STATE_INITIALIZED      2

/*
 * semaphores
 */
typedef struct {
  CS_SEM        mpif ;
  CS_SEM        host_if ;
} arn5_dev_sem_t ;


#ifdef POSIX_PTHREAD
typedef struct {
  pthread_t     tid ;
  cs_boolean    poll ;
  cs_uint32     msec_poll_intvl ;
  cs_uint32     blk_id ;
} arn5_dev_dbt_irq_poll_t ;

typedef struct {
  arn5_dev_dbt_irq_poll_t  irq_poll ;
} arn5_dev_debug_thread_t ;
#endif /* POSIX_PTHREAD */


/*
 * Device Control Block (dev_cb)
 * One control block per chip
 */
typedef struct {
  cs_uint8              dev_id ;
  cs_uint32             state ;
  arn5_dev_sem_t        sem ;
  arn5_dev_summ_t       summ ;  /* config summary */
  cs_uint32             base_addr ;
  cs_uint32             debug_flag ;
  cs_uint32             slice_enbl ; /* [7:0] <-> slice 7 .. 0 */
  cs_uint8              oc48_num ;
  cs_uint8              oc12_num ;
  cs_uint8              oc3_num ;
  cs_uint8              gige_num ;
  cs_uint32             total_rate_units ;
  arn5_port_cb_t        * ppcb[ ARN5_MAX_NUM_PORTS ] ;
  /* phy_chan_map use phy as index to keep track of log chan */
  cs_uint8              rx_phy_chan_map[ ARN5_MAX_NUM_CHANNELS ] ;
  cs_uint8              tx_phy_chan_map[ ARN5_MAX_NUM_CHANNELS ] ;
  /* log_chan_map use log as index to keep track of phy chan */
  cs_uint8              rx_log_chan_map[ ARN5_MAX_NUM_CHANNELS ] ;
  cs_uint8              tx_log_chan_map[ ARN5_MAX_NUM_CHANNELS ] ;
  arn5_dev_stats_cb_t   stats_cb ;
  arn5_dev_stats_t      stats ;
  arn5_dev_irq_cb_t     irq;

#ifdef POSIX_PTHREAD
  arn5_dev_debug_thread_t dbt ;
#endif

} arn5_dev_cb_t ;


/*************************************************************
 ***********    DRIVER CONTROL BLOCK (drvr_cb)    ************
 *************************************************************/
typedef struct {
  cs_boolean    drvr_loaded ;
  cs_uint32     max_num_devices ;
  arn5_dev_cb_t  * p_dev_cb_tbl[ ARN5_MAX_NUM_DEVICES ] ;

#ifndef CS_DONT_USE_STDLIB
  FILE          * drvr_out_fp ;
#endif

  cs_uint8      drvr_out_filename[256] ;
  cs_uint16     drvr_flag ;
} arn5_drvr_cb_t ;

/*
 * Driver flags
 */
#define ARN5_DRVR_DEFAULT_FLAG      0x0000
#define ARN5_DRVR_STATS_IN_HEX      0x0001  /* display stats in hex */
#define ARN5_DRVR_STATS_MASK_ZERO   0x0002  /* not displaying zero counters */
#define ARN5_DRVR_ESC_CODE_PRINT    0x0004  /* using VT-100 escape codes */

extern arn5_drvr_cb_t  * g_pARN5 ;

#endif /* __ARN5_CB_H__ */

