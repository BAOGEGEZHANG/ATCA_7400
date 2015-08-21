/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_cb.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PRIVATE header file - for driver use only!
 * Contains the control block and other internal data structures
 * and associated defines.
 *
 */
#ifndef __M2R_CB_H__
#define __M2R_CB_H__

#ifndef CS_DONT_USE_STDLIB
#	include <stdio.h>
#endif /* CS_DONT_USE_STDLIB */
#include "cs_types.h"
#include "cs_rtos.h"
#include "m2r_error.h"

#ifdef POSIX_PTHREAD
/* Feature #c -> */
#include <pthread.h>
/* Feature #c <- */
#endif

#include "m2r_public.h"
#include "m2r_gen.h"
#include "m2r_rmac_cam.h"
#include "m2r_stats_priv.h"
#include "m2r_irq_priv.h"
#include "m2r_lif.h"


#define M2R_NUM_PORTS_PER_ASIC   ( 1 )

#define M2R_MAX_NUM_PORTS (CS_MAX_NUM_DEVICES * M2R_NUM_PORTS_PER_ASIC)


/* Port states */
#define  M2R_PORT_STATE_POR		(0) /* Pwron reset default */
#define  M2R_PORT_STATE_DATA_INIT_DONE  (1) /* data structs alloc'ed */
#define  M2R_PORT_STATE_HW_CFG_DONE  	(2) /* HW cfg done   */
#define  M2R_PORT_STATE_ENABLED		(3)
#define  M2R_PORT_STATE_DISABLED	(4)

/*
 * semaphores
 */
typedef struct {
  CS_SEM       	mpif ;
  CS_SEM       	spi42 ;
  CS_SEM	pprbs ;
  CS_SEM	framer ;
  CS_SEM	rmac ;
  CS_SEM	xgxs ;
  CS_SEM	lif ;
  CS_SEM        eth ;
} m2r_port_sem_t ;


#ifdef POSIX_PTHREAD
/* Feature #c -> */
/*
 * Debug parameters for thread processing
 */
typedef struct {
  pthread_t     tid ;
  cs_boolean    poll ;
  cs_uint32     msec_poll_intvl ;
  cs_boolean    send_rcvd_ctl_pkts ;
  cs_boolean	send_rcvd_ips_pkts ;
  cs_boolean    get_ips_msg_in_ttl_mode ;
  cs_boolean    blk_print_stats ;
  cs_boolean	blk_irq_poll ;
} m2r_port_debug_thread_t ;
/* Feature #c <- */
#endif


/*************************************************************
 ************     PORT CONTROL BLOCK (port_cb)  **************
 *************************************************************/
typedef struct {
  cs_uint16		port_id ;
  cs_uint32     	state ;
  m2r_port_sem_t	sem ;   /* semaphores     */
  m2r_port_summ_t	summ ;  /* config summary */
  m2r_port_stats_cb_t   stats_cb ;
  m2r_port_stats_t      stats ;
  /* Save the following two LIF config params here since we need */
  /* to reference them at a later time                           */
  /* tx_ckref : clock fed to TX_CKREF_P/N                        */
  /* tx_ckref_lptime : clock fed to TX_CKREF_LPTIME_P/N          */
  /* tx_ckref_custom : custom flag to indicate above two are     */
  /*                   not the default values                    */
  /* tx_clk_mode : keep track of the last clk_mode that is not   */
  /*               internal loop timing for restore from line    */
  /*               loopback                                      */
  m2r_lif_ref_clk_t     tx_ckref ;
  m2r_lif_ref_clk_t     tx_ckref_lptime ;
  cs_boolean            tx_ckref_custom ;
  m2r_lif_tx_clk_mode_t tx_clk_mode ;
  cs_boolean		acctg_stats_malloced ;
  m2r_rmac_acctg_stats_t * p_rmac_acctg_stats ;
  m2r_port_irq_cb_t     irq ;

#ifdef POSIX_PTHREAD
/* Feature #c -> */
  m2r_port_debug_thread_t debug_thread ;
/* Feature #c <- */
#endif
} m2r_port_cb_t ;


/*************************************************************
 *************    DEVICE CONTROL BLOCK (dev_cb)   ************
 *************************************************************/
#define  M2R_DEV_STATE_INVALID          0
#define  M2R_DEV_STATE_REGISTERED	1
#define  M2R_DEV_STATE_INITIALIZED      2

typedef struct {
  cs_boolean    init_done ;
  cs_uint16     master_port_id ;
} m2r_dev_shared_ctl_t ;

/*
 * Device Control Block (dev_cb)
 * One control block per chip
 */
typedef struct {
  cs_uint16		dev_id ;
  cs_uint32		state ;
  cs_uint64		base_addr ;
  cs_uint32		debug_flag ;
  cs_uint32		max_num_ports ;
  m2r_port_cb_t		* ppcb[ M2R_NUM_PORTS_PER_ASIC ] ;
} m2r_dev_cb_t ;

/*************************************************************
 ***********    DRIVER CONTROL BLOCK (drvr_cb)    ************
 *************************************************************/
typedef struct {
  cs_boolean	drvr_loaded ;
  cs_uint32     max_num_devices ;
  m2r_dev_cb_t	dev_tbl[ CS_MAX_NUM_DEVICES ] ;
#ifndef CS_DONT_USE_STDLIB
  FILE          * drvr_out_fp ;
#endif
  char          drvr_out_filename[256] ;
  cs_uint16     drvr_flag ;
  cs_boolean    warm_start ;
} m2r_drvr_cb_t ;

/*
 * Driver flags
 */
#define M2R_DRVR_DEFAULT_FLAG      0x0000
#define M2R_DRVR_STATS_IN_HEX      0x0001  /* display stats in hex */
#define	M2R_DRVR_STATS_MASK_ZERO   0x0002  /* not displaying zero counters */

extern m2r_drvr_cb_t  * pM2R ;

#endif /* __M2R_CB_H__ */






