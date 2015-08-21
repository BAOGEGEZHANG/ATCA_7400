/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_spi42.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_SPI42_H__
#define __M2R_SPI42_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_spi42_irq.h"

/* Tx and Rx sync states */
typedef enum {
  M2R_SPI42_INIT_STATE          = 0,
  M2R_SPI42_OUT_OF_SYNC,
  M2R_SPI42_IN_SYNC
} m2r_spi42_sync_t ;


/* SPI4.2 Tx scheduler modes */
typedef enum {
  M2R_SPI42_1CH_CUT_THRU       = 0,
  M2R_SPI42_1CH_ST_FWD,
  M2R_SPI42_4CH_RR_CUT_THRU,
  M2R_SPI42_4CH_PRIO_ST_FWD
} m2r_spi42_sched_mode_t ;

#define M2R_SPI42_REG_OFFSET(REG)           \
        (REG_OFFSET(M2R_t, SPI4P2) + REG_OFFSET(SPI4P2_t, REG))

typedef struct {
  /* stat_lvds specifies if status channels are LVDS or LV-TTL */
  cs_boolean	stat_lvds ;
  /* dynalign specifies dynamic or static alignment for Tx path */
  cs_boolean	dynalign ;
  /* quater_rate specifies 1/4 clocking rate for OC48 mode */
  cs_boolean	quarter_rate ;
} m2r_spi42_gen_char_t ;

typedef struct {
  /* burst_size is no. of 16-byte blocks - basic unit for MAXBURST1/2 */
  cs_uint8	burst_size ;
  /* maxburst1 is data burst credit when RSTAT = STARVING */
  cs_uint8	maxburst1 ;
  /* maxburst2 is data burst credit when RSTAT = HUNGRY */
  cs_uint8	maxburst2 ;
} m2r_spi42_burst_params_t ;

typedef struct {
  /* alpha specifies no. of training repetitions between DATA_MAX_T  */
  cs_uint8	alpha ;
  /* datamaxt specifies maximum interval between training sequences */
  cs_uint16	datamaxt ;
  /* DIP-2 thresholds for Rx out-of-sync and in-sync, sent with RSTAT */
} m2r_spi42_training_t ;

typedef struct {
  /* DIP-2 thresholds for Rx out-of-sync and in-sync, sent with RSTAT */
  cs_uint8	error_thold ;
  cs_uint8	match_thold ;
} m2r_spi42_dip2_t ;

typedef struct {
  /* DIP-4 thresholds for Tx out-of-sync and in-sync, sent with TDAT */
  cs_uint8	error_thold ;
  cs_uint16	match_thold ;
} m2r_spi42_dip4_t ;

typedef struct {
  /* mode specifies Tx mode - 1 ch(cut-thru or st&fwd),              */
  /*                        - 4 ch(RR cut-thru or priority st&fwd)   */
  m2r_spi42_sched_mode_t mode ;
  /* tx_threshlo- min bytes in FIFO before draining(for cut-thry only*/
  cs_uint16	         threshlo ;
  /* txcal_burst specifies max 64-bit data to FIFO before ch switching*/
  cs_uint16	         cal_burst ;
} m2r_spi42_tx_sched_t ;

typedef struct {
  /* low specifies FIFO ALMOST EMPTY condition(valid for Tx only) */
  cs_uint16	low ;
  /* high specifies FIFO ALMOST FULL condition                    */
  cs_uint16	high ;
} m2r_spi42_fifo_wmarks_t ;

typedef struct {
  cs_uint16	m ;
  cs_uint8	len ;
  cs_uint8	ent0 ;
  cs_uint8	ent1 ;
  cs_uint8	ent2 ;
  cs_uint8	ent3 ;
} m2r_spi42_cal_params_t ;


/****************************************
 *         SPI42 Configuration          *
 ****************************************/
typedef struct {
  /* General(Electrical) Characterstics */
  m2r_spi42_gen_char_t	    gen_char ;
  /* Scale factor for RDCLK wrt to RDREFCLK */
  cs_uint8                  rdclk_mult ;  /* 2, 4 or 8 x RDREFCLK */
  /* atm_mode if SPI42 is to process cells or packets */
  cs_boolean	            atm_mode[4] ;
  /* Burst parameters */
  m2r_spi42_burst_params_t  burst_params ;
  /* Training parameters */
  m2r_spi42_training_t	    training ;
  /* DIP2 thresholds */
  m2r_spi42_dip2_t	    dip2 ;
  /* DIP4 thresholds */
  m2r_spi42_dip4_t	    dip4 ;
  /* Tx scheduler */
  m2r_spi42_tx_sched_t	    tx_sched ;
  /* Tx FIFO watermarks(low and high) */
  m2r_spi42_fifo_wmarks_t   tx_fifo_wmarks ;
  /* Rx FIFO watermarks(only high) */
  m2r_spi42_fifo_wmarks_t   rx_fifo_wmarks ;
  /* Tx calendar parameters */
  m2r_spi42_cal_params_t    tx_cal ;
  /* Rx calendara parameters */
  m2r_spi42_cal_params_t    rx_cal ;
  /* Loopbacks */
  /* Note: BERT loopback is used for lab debug only.      */
  /*       Do NOT use this for looping back packets !!!   */
  cs_boolean	tx_lpbk ; /* BERT loopback */
  cs_boolean	rx_lpbk ; /* LB loopback   */

  cs_uint16     irq_ctl_tbl[M2R_SPI42_MAX_GRP] ; /* Default IRQ */
} m2r_spi42_cfg_t ;


/*********************************************
 *     SPI4.2 Running Configuration          *
 *********************************************/
typedef struct {
  m2r_spi42_sync_t          tx_sync_state ;
  m2r_spi42_sync_t          rx_sync_state ;
  cs_uint8                  rdclk_mult ;
  m2r_spi42_burst_params_t  burst_params ;
  m2r_spi42_training_t	    training ;
  m2r_spi42_tx_sched_t	    tx_sched ;
  m2r_spi42_fifo_wmarks_t   tx_fifo_wmarks ;
  m2r_spi42_fifo_wmarks_t   rx_fifo_wmarks ;
  m2r_spi42_cal_params_t    tx_cal ;
  m2r_spi42_cal_params_t    rx_cal ;
  /* Loopbacks */
  cs_boolean    tx_lpbk ; /* BERT loopback */
  cs_boolean    rx_lpbk ; /* LB loopback   */
} m2r_spi42_runcfg_t ;


extern
cs_status m2r_spi42_init_cfg(cs_uint16 port_id,
			     m2r_port_summ_t * pSumm,
			     m2r_spi42_cfg_t * pCfg) ;
extern
cs_status m2r_spi42_get_default_cfg(cs_uint16 port_id,
				    m2r_port_summ_t * pSumm,
				    m2r_spi42_cfg_t * pCfg) ;
extern
cs_status m2r_spi42_get_running_cfg(cs_uint16 port_id,
				    m2r_port_summ_t * pSumm,
				    m2r_spi42_runcfg_t * pRunCfg) ;
extern
cs_status m2r_spi42_dump_running_cfg(cs_uint16 port_id,
                                     m2r_spi42_runcfg_t * pRunCfg) ;
extern
cs_status  m2r_spi42_sreset(cs_uint16 port_id) ;
extern
cs_status  m2r_spi42_sync_sreset(cs_uint16 port_id, m2r_dir_t dir,
				 m2r_reset_action_t act) ;
extern
cs_status  m2r_spi42_set_gen_char(cs_uint16 port_id, cs_boolean stat_lvds,
				  cs_boolean dynalign,
				  cs_boolean quarter_rate) ;
extern
cs_status m2r_spi42_set_rdclk_mult(cs_uint16 port_id,
				   cs_uint8 mult) ;
extern
cs_status  m2r_spi42_atm_mode(cs_uint16 port_id, cs_uint8 chan_id,
			      cs_boolean atm_mode) ;
extern
cs_status  m2r_spi42_set_burst_params(cs_uint16 port_id,
				      cs_uint8 burst_size,
				      cs_uint8 maxburst1,
				      cs_uint8 maxburst2) ;
extern
cs_status  m2r_spi42_set_training_params(cs_uint16 port_id,
					 cs_uint8 alpha,
					 cs_uint16 datamaxt) ;
extern
cs_status  m2r_spi42_set_dip2_tholds(cs_uint16 port_id,
				     cs_uint8 error_thold,
				     cs_uint8 match_thold) ;
extern
cs_status  m2r_spi42_set_dip4_tholds(cs_uint16 port_id,
				     cs_uint8 error_thold,
				     cs_uint16 match_thold) ;
extern
m2r_spi42_sync_t  m2r_spi42_get_sync_state(cs_uint16 port_id,
					   m2r_dir_t dir) ;
extern
cs_status  m2r_spi42_set_tx_sched_mode(cs_uint16 port_id,
				       m2r_spi42_sched_mode_t mode,
				       cs_uint16 threshlo,
				       cs_uint16 txcal_burst) ;
extern
cs_status m2r_spi42_set_tx_fifo_wmarks(cs_uint16 port_id,
				       cs_uint16 low_wmark,
				       cs_uint16 high_wmark) ;
extern
cs_status m2r_spi42_set_rx_fifo_wmarks(cs_uint16 port_id,
				       cs_uint16 high_wmark) ;
extern
cs_status  m2r_spi42_set_cal_params(cs_uint16 port_id,
		          m2r_dir_t dir, cs_uint16 m, cs_uint8  len,
				     cs_uint8  ent0, cs_uint8  ent1,
				     cs_uint8  ent2, cs_uint8  ent3) ;

extern
cs_status  m2r_spi42_set_loopback(cs_uint16 port_id,
				  m2r_dir_t dir, cs_boolean enbl) ;

extern
cs_status m2r_spi42_isr(cs_uint16 port_id) ;

#endif /* __M2R_SPI42_H__ */
