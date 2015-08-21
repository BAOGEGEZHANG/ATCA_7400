/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_pprbs.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_PPRBS_H__
#define __ARN5_PPRBS_H__

#include "cs_types.h"
#include "arn5_common.h"
#include "arn5_pprbs_irq.h"


/**********************************************************************
 * Defines for default values
 **********************************************************************/
#define ARN5_PPRBS_MIN_PKT_SIZE_DEF          (16)
#define ARN5_PPRBS_MAX_PKT_SIZE_DEF          (0x007f)

/**********************************************************************
 * PPRBS CHANNEL :
 *   ARN5_PPRBS_HOST_CHAN channel refers to generator and checker which
 *   send and receive packets from the HOST side i.e to/from SPI4.2
 *   ARN5_PPRBS_LINE_CHAN channel refers to generator and checker which
 *   send and receive packets from/to LINE side i.e. towards LIF.
 **********************************************************************/
typedef  enum {
  ARN5_PPRBS_HOST_CHAN   = 0,
  ARN5_PPRBS_LINE_CHAN,
  ARN5_PPRBS_CHAN_ALL,
} arn5_pprbs_chan_t ;

/**********************************************************************
 * PPRBS payload type ( ATM Cell, RPR/SRP packets or multicast packet)
 **********************************************************************/
typedef enum {
  ARN5_PPRBS_PLD_PKT = 0,
  ARN5_PPRBS_PLD_ATM,
  ARN5_PPRBS_PLD_MULTICAST
} arn5_pprbs_pld_t ;


typedef struct {
  cs_boolean        fixPld ;    /* FIXED PAYLOAD or PRBS PAYLOAD      */
  arn5_pprbs_pld_t   pldType ;   /* PAYLOAD TYPE                       */

  /* Traffic characteristics. */
  cs_uint16         burstSize ; /* Burst Size ( 0 for continous burst */
  cs_uint16         rate ;      /* Pkt generater rate.                */
  cs_uint16         minSize ;   /* Min pkt size to be generated.      */
  cs_uint16         maxSize ;   /* max packet size to be generated    */
  cs_ctl_t          genCtl ;    /* generator enable control           */
  cs_ctl_t          chkrCtl ;   /* checker enable control             */
} arn5_pprbs_chan_cfg_t;


typedef struct {
  arn5_pprbs_chan_cfg_t  chan[ARN5_PPRBS_CHAN_ALL] ;
  cs_uint16             irq_ctl_tbl[ARN5_PPRBS_MAX_GRP] ;
} arn5_pprbs_cfg_t;


typedef struct {
  cs_boolean        hostGen  ;
  cs_boolean        hostChkr ;
  cs_boolean        lineGen  ;
  cs_boolean        lineChkr ;
} arn5_pprbs_runcfg_t ;


/**********************************************************************
 * PPRBS APIs
 **********************************************************************/
extern
cs_status arn5_pprbs_init_cfg(cs_uint16 port_id,
                             arn5_pprbs_cfg_t * pCfg) ;

extern
cs_status arn5_pprbs_reset_stats(cs_uint16 port_id,
                arn5_pprbs_chan_t chan,
                cs_boolean pktGen,
                cs_boolean checker) ;
extern
cs_status arn5_pprbs_cfg_traffic(cs_uint16 port_id,
                arn5_pprbs_chan_t  chan,
                cs_boolean fixpld,
                cs_uint16 burstSize,
                cs_uint16 rate,
                cs_uint16 minSize,
                cs_uint16 maxSize) ;
extern
cs_status arn5_pprbs_ctl_payload(cs_uint16 port_id,
                arn5_pprbs_chan_t chan,
                arn5_pprbs_pld_t pld) ;
extern
cs_status arn5_pprbs_ctl_pktGen(cs_uint16 port_id,
                arn5_pprbs_chan_t chan,
                cs_ctl_t ctl) ;
extern
cs_status arn5_pprbs_ctl_ipg(cs_uint16 port_id,
                arn5_pprbs_chan_t  chan,
                cs_uint16 ipg) ;
extern
cs_status arn5_pprbs_inject_err(cs_uint16 port_id,
                arn5_pprbs_chan_t  chan,
                cs_boolean signleErr,
                cs_uint16 duration) ;
extern
cs_status arn5_pprbs_ctl_pktChkr(cs_uint16 port_id,
                arn5_pprbs_chan_t chan,
                cs_ctl_t ctl) ;
extern
cs_status arn5_pprbs_resync(cs_uint16 port_id,
                arn5_pprbs_chan_t chan) ;
extern
cs_status arn5_pprbs_chk_in_sync(cs_uint16 port_id,
                arn5_pprbs_chan_t chan,
                cs_boolean * pSyncStat) ;
extern
cs_status arn5_pprbs_get_ber(cs_uint16 port_id,
                arn5_pprbs_chan_t chan, cs_uint64 * pBer) ;
extern
cs_status arn5_pprbs_isr(cs_uint16 port_id) ;
extern
cs_status arn5_pprbs_dump_checker(cs_uint16 port_id,
                arn5_pprbs_chan_t chan) ;

#endif /* __ARN5_PPRBS_H__ */



