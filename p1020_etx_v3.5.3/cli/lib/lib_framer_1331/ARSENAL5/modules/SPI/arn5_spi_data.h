/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_data.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported data header file for SPI module.
 */
#ifndef __ARN5_SPI_DATA_H__
#define __ARN5_SPI_DATA_H__

#include "cs_types.h"
#include "arn5_registers.h"
#include "arn5_public.h"
#include "arn5_spi_irq.h"

#define ARN5_SPI_MAX_CAL_ENTRY        96
#define ARN5_SPI_MAX_TX_SCHD_ENTRY    16


/***********************************************
 * Device-level Configuration                  *
 ***********************************************/

/* Tx and Rx sync states */
typedef enum {
  ARN5_SPI42_INIT_STATE  = 0,
  ARN5_SPI42_OUT_OF_SYNC,
  ARN5_SPI42_IN_SYNC,
} arn5_spi42_sync_t ;

typedef enum {
  ARN5_SPI42_MODE  = 0,
  ARN5_SPI3_MODE,
  ARN5_SPI42_QUARTER_RATE_MODE,
} arn5_spi_if_mode_t ;

typedef enum {
  /* in register SPI4P2_TXIF_spi42ctl */
  ARN5_SPI42_TX_TRN_SEQ_STRICT_MODE,
  ARN5_SPI42_DISABLE_SOP_DIST_ERR_CLOSE_ALL_PKT,
  ARN5_SPI42_DISABLE_BURST_LEN_ERR_CLOSE_ALL_PKT,
  ARN5_SPI42_DISABLE_RSVD_WORD_CLOSE_ALL_PKT,
  ARN5_SPI42_IGNORE_SOP_DISTANCE_DELETION,
  ARN5_SPI42_DISABLE_SEQ_ERR_CLOSE_ALL_PKT,
  ARN5_SPI42_DISABLE_DIP4_CLOSE_ALL_PKT,

  /* in register SPI4P2_RXCORE_chclose */
  ARN5_SPI42_ENABLE_CLOSING_ACTIVE_CHAN_UPON_ENTERING_IN_SYNC,
  ARN5_SPI42_ENABLE_PROTOCOL_FIXING,
} arn5_spi42_ctrl_type_t ;

typedef enum {
  ARN5_SPI_PLL_MULT_2 = ARN5_multsel_DIV2,
  ARN5_SPI_PLL_MULT_4 = ARN5_multsel_DIV4,
  ARN5_SPI_PLL_MULT_8 = ARN5_multsel_DIV8,
  ARN5_SPI_PLL_MULT_DONTCARE = ARN5_SPI_PLL_MULT_2,
} arn5_spi_pll_mult_t ;



typedef struct {
  /* burst_size is no. of 16-byte blocks - basic unit for MAXBURST1/2 */
  cs_uint8      burst ;
  /* maxburst1 is data burst credit when RSTAT = STARVING */
  cs_uint8      maxburst1 ;
  /* maxburst2 is data burst credit when RSTAT = HUNGRY */
  cs_uint8      maxburst2 ;
} arn5_spi_burst_params_t ;

typedef struct {
  /* alpha specifies no. of training repetitions between DATA_MAX_T  */
  cs_uint8      alpha ;
  /* datamaxt specifies maximum interval between training sequences */
  cs_uint16     datamaxt ;
  /* DIP-2 thresholds for Rx out-of-sync and in-sync, sent with RSTAT */
} arn5_spi42_training_t ;

typedef struct {
  /* DIP-2 thresholds for Rx out-of-sync and in-sync, sent with RSTAT */
  cs_uint8      error_thold ;
  cs_uint8      match_thold ;
} arn5_spi42_dip2_t ;

typedef struct {
  /* DIP-4 thresholds for Tx out-of-sync and in-sync, sent with TDAT */
  cs_uint8      error_thold ;
  cs_uint16     match_thold ;
} arn5_spi42_dip4_t ;

typedef struct {
  /* starting address of the FIFO */
  cs_uint16     low_addr ;
  /* ending address of the FIFO */
  cs_uint16     high_addr ;
  /* low_wmark specifies FIFO ALMOST EMPTY condition  */
  cs_uint16     low_wmark ;
  /* high_wmark specifies FIFO ALMOST FULL condition  */
  cs_uint16     high_wmark ;
} arn5_spi_tx_fifo_params_t ;

typedef struct {
  /* low_wmark specifies FIFO ALMOST EMPTY condition  */
  cs_uint16     low_wmark ;
  /* high_wmark specifies FIFO ALMOST FULL condition  */
  cs_uint16     high_wmark ;
  /* hysteresis_wmark specifies FIFO read enable condition  */
  cs_uint16     hysteresis_wmark ;
} arn5_spi_rx_fifo_params_t ;

typedef struct {
  cs_uint16     m ;
  cs_uint8      len ;
  cs_uint8      ent[ARN5_SPI_MAX_CAL_ENTRY] ;
} arn5_spi_cal_params_t ;

typedef struct {
  cs_uint8                      freq_tune ;
  arn5_spi_pll_mult_t           multiplier_sel ;
} arn5_spi_pll_params_t ;


/* SPI device-level related configuration */
typedef struct {
  /* logical chan assignment */
  cs_boolean                    use_rx_map_for_rx_tx ;
  cs_uint8                      rx_lch_map[ARN5_MAX_NUM_CHANNELS] ;
  cs_uint8                      tx_lch_map[ARN5_MAX_NUM_CHANNELS] ;

  /* PLL parameters */
  arn5_spi_pll_params_t         tx_pll_params;
  arn5_spi_pll_params_t         rx_pll_params;
  /* Burst parameters */
  arn5_spi_burst_params_t       burst_params ;
  /* Tx calendar parameters */
  arn5_spi_cal_params_t         tx_cal ;
  /* Rx calendar parameters */
  arn5_spi_cal_params_t         rx_cal ;
  /* Calendar length seen by scheduler */
  cs_uint8                      rx_scheduler_cal_len ;
  /* Tx Scheduler */
  cs_uint16                     tx_scheduler_0_1_2_3 ;
  cs_uint16                     tx_scheduler_4_5_6_7 ;
  cs_uint16                     tx_scheduler_8_9_10_11 ;
  cs_uint16                     tx_scheduler_12_13_14_15 ;
  /* Tx fifo parameters */
  arn5_spi_tx_fifo_params_t     tx_fifo_params[ARN5_MAX_NUM_CHANNELS] ;
  /* Rx fifo parameters */
  arn5_spi_rx_fifo_params_t     rx_fifo_params[ARN5_MAX_NUM_CHANNELS] ;
  /* Loopbacks */
  cs_boolean                    tx_lpbk ; /* tx-to-rx loopback */
  cs_boolean                    rx_lpbk ; /* rx-tp-tx loopback */
} arn5_spi_dev_common_cfg_t ;

typedef struct {
  /* spi42 rate selector */
  cs_boolean                    quarter_rate ;
  /* Training parameters */
  arn5_spi42_training_t         training ;
  /* DIP2 thresholds */
  arn5_spi42_dip2_t             dip2 ;
  /* DIP4 thresholds */
  arn5_spi42_dip4_t             dip4 ;
  /* afifo hysteresis    */
  cs_uint8                      afifo_hysteresis ;
} arn5_spi42_dev_cfg_t ;

typedef struct {
  cs_boolean                    eight_bit_mode ;
  cs_boolean                    stall_on_single_fifo_full ;
  cs_uint8                      npause ;
  cs_boolean                    ptpa_enable ;
  cs_boolean                    stpa_enable ;
} arn5_spi3_dev_cfg_t ;

typedef struct {
  arn5_spi_dev_common_cfg_t     spi ;
  arn5_spi42_dev_cfg_t          spi42 ;
  arn5_spi3_dev_cfg_t           spi3 ;
  cs_uint16                     irq_ctl_tbl[ARN5_SPI_MAX_GRP] ; /* Default IRQ */
} arn5_spi_dev_cfg_t;


/***********************************************
 * Device-level Running Configuration          *
 ***********************************************/
typedef struct {
  arn5_spi_dev_common_cfg_t     spi ;
  arn5_spi42_dev_cfg_t          spi42 ;
  arn5_spi3_dev_cfg_t           spi3 ;
  /* data drop ctl indication, bit 7:0 -> ch 7..0 */
  cs_uint8                      rx_drop_bits ;
  cs_uint8                      tx_drop_bits ;
  cs_boolean                    lvds ;
} arn5_spi_dev_runcfg_t ;


/***********************************************
 * Port-level  configuration                   *
 ***********************************************/
typedef struct {
  cs_ctl_t                      store_fwd ;
  cs_boolean                    gfp_pli_insert ;
} arn5_spi_port_cfg_t ;


/***********************************************
 * Port-level Running Configuration            *
 ***********************************************/
typedef struct {
  cs_ctl_t                      store_fwd ;
  cs_boolean                    gfp_pli_insert ;
} arn5_spi_port_runcfg_t ;


#endif /* __ARN5_SPI_DATA_H__ */


