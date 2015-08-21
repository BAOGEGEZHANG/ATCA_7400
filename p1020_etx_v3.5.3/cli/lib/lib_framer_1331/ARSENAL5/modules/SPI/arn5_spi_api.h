/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_api.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported api header file for SPI module.
 */
#ifndef __ARN5_SPI_API_H__
#define __ARN5_SPI_API_H__

#include "cs_types.h"
#include "arn5_public.h"


/****************************************************************
 ******************    SPI DEVICE-LEVEL API's   *****************
 * These API's take dev_id and *not* port_id as argument.       *
 ****************************************************************/

extern
cs_status  arn5_spi_dev_sreset(cs_uint8 dev_id) ;
extern
arn5_spi_if_mode_t  arn5_spi_dev_get_if_mode(cs_uint8 dev_id) ;
extern
cs_uint8  arn5_spi_dev_get_logical2physical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_uint8 logical_chan_num) ;
extern
cs_uint8  arn5_spi_dev_get_physical2logical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_uint8 physical_chan_num) ;
extern
cs_status  arn5_spi_dev_set_chanmap(cs_uint8 dev_id, cs_dir_t dir,
                        cs_uint8 lch_num_pch0, cs_uint8 lch_num_pch1,
                        cs_uint8 lch_num_pch2, cs_uint8 lch_num_pch3,
                        cs_uint8 lch_num_pch4, cs_uint8 lch_num_pch5,
                        cs_uint8 lch_num_pch6, cs_uint8 lch_num_pch7) ;
extern
cs_status  arn5_spi_dev_set_pll_controls(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_uint8 freq_tune,
                        arn5_spi_pll_mult_t multiplier_sel) ;
extern
cs_status  arn5_spi_dev_set_rx_scheduler_cal_len(cs_uint8 dev_id,
                        cs_uint8 len) ;
extern
cs_status  arn5_spi_dev_set_tx_scheduler_entries(cs_uint8 dev_id,
                        cs_uint16 ent0_1_2_3,  cs_uint16 ent4_5_6_7,
                        cs_uint16 ent8_9_10_11,  cs_uint16 ent12_13_14_15) ;
extern
cs_status  arn5_spi_dev_set_burst_params(cs_uint8 dev_id,
                        cs_uint8 burst,
                        cs_uint16 maxburst1,
                        cs_uint16 maxburst2) ;
extern
cs_status  arn5_spi_dev_set_cal_params(cs_uint8 dev_id,
                        cs_dir_t dir,
                        arn5_spi_cal_params_t * pParams) ;
extern
cs_status  arn5_spi42_dev_set_controls(cs_uint8 dev_id,
                        arn5_spi42_ctrl_type_t ctl_sel,
                        cs_boolean set) ;
extern
cs_status  arn5_spi_dev_apish_set_cal_params(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint16 m, cs_uint8 len) ;
extern
cs_status  arn5_spi_dev_apish_set_cal_entries(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint8 starting_entry,
                        cs_uint8  ent0, cs_uint8  ent1,
                        cs_uint8  ent2, cs_uint8  ent3,
                        cs_uint8  ent4, cs_uint8  ent5,
                        cs_uint8  ent6, cs_uint8  ent7) ;
extern
cs_status  arn5_spi_dev_ctl_loopback(cs_uint8 dev_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_status  arn5_spi_dev_set_tx_fifo_params(cs_uint8 dev_id,
                        cs_uint8 chan_num,
                        cs_uint16 low_addr, cs_uint16 high_addr,
                        cs_uint16 low_wmark, cs_uint16 high_wmark) ;
extern
cs_status  arn5_spi_dev_set_rx_fifo_params(cs_uint8 dev_id,
                        cs_uint8 chan_num,
                        cs_uint8 low_wmark, cs_uint8 high_wmark,
                        cs_uint8 hysteresis_wmark) ;
extern
cs_status  arn5_spi42_dev_sync_sreset(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_reset_action_t act) ;
extern
cs_status  arn5_spi42_dev_set_rate(cs_uint8 dev_id,
                        cs_boolean quarter_rate) ;
extern
cs_status  arn5_spi42_dev_set_training_params(cs_uint8 dev_id,
                        cs_uint8 alpha,
                        cs_uint16 datamaxt) ;
extern
cs_status  arn5_spi42_dev_set_dip2_tholds(cs_uint8 dev_id,
                        cs_uint8 error_thold,
                        cs_uint8 match_thold) ;
extern
cs_status  arn5_spi42_dev_set_dip4_tholds(cs_uint8 dev_id,
                        cs_uint8 error_thold,
                        cs_uint16 match_thold) ;
extern
cs_status  arn5_spi42_dev_set_afifo_hysteresis(cs_uint8 dev_id,
                        cs_uint8 hysteresis) ;
extern
arn5_spi42_sync_t  arn5_spi42_dev_get_sync_state(cs_uint8 dev_id,
                        cs_dir_t dir) ;
extern
cs_status  arn5_spi3_dev_set_modes(cs_uint8 dev_id,
                        cs_boolean eight_bit_mode) ;
extern
cs_status  arn5_spi3_dev_ctl_stall_on_single_fifo_full(cs_uint8 dev_id,
                        cs_boolean enbl) ;
extern
cs_status  arn5_spi3_dev_set_controls(cs_uint8 dev_id,
                        cs_boolean ptpa_enable,
                        cs_boolean stpa_enable) ;
extern
cs_status  arn5_spi3_dev_set_pause_cycles(cs_uint8 dev_id,
                        cs_uint8 pause_cycles) ;
extern
cs_status  arn5_spi_dev_isr(cs_uint8 dev_id) ;


/****************************************************************
 *******************    SPI PORT-LEVEL API's    *****************
 ****************************************************************/
extern
cs_status arn5_spi_ctl_store_forward_mode(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_boolean gfp_pli_insert) ;

#endif /* __ARN5_SPI_H__ */

