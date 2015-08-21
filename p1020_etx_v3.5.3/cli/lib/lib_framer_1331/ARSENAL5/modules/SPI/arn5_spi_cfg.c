/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_cfg.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * Arsenal5configurtion and init related to SPI4.2/3;
 * arn5_spi_* apply to both SPI4.2 and SPI3.
 * arn5_spi42_* apply to SPI4.2 only.
 * arn5_spi3_* apply to SPI3 only.
 */

#include "cs_types.h"
#include "cs_rtos.h"
#include "cs_utils.h"

#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_gen_data.h"

#include "arn5_spi_data.h"
#include "arn5_spi_api.h"
#include "arn5_spi_priv.h"
#include "arn5_spi_stats.h"


typedef enum {
  ARN5_CAL_ROUND_ROBIN = 0,
} arn5_cal_algorithm_t ;


/* Forward declarations */
static cs_status  arn5_spi_dev_init_cfg_common(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg) ;

static cs_status  arn5_spi_dev_get_default_cfg_common(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg) ;

static cs_status  arn5_spi_dev_get_running_cfg_common(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg) ;

static cs_status  arn5_spi_dev_dump_running_cfg_common(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg) ;

static cs_status  arn5_spi_cal_programming(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg,
                        arn5_cal_algorithm_t select) ;

cs_status  arn5_spi_init_data(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;

/****************************************************************/
/* _hdr_start  SPI INITIALIZATION                               */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_spi_dev_init_cfg(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to configuration structure            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the device level of the SPI block           */
/* with the given configuration.                                */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/* _hdr_end                                                     */
/****************************************************************/
{
  /*
   * Assert and keep SPI host and line interfaces in sync reset while
   * it gets initialized.
   */
  if ( arn5_spi42_dev_sync_sreset(dev_id, CS_TX_AND_RX,
                                 CS_RESET_ASSERT) != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* drop every chan first */
  {
    arn5_spi_drop_chan_data_ctl(dev_id, 0xff, TRUE);
    arn5_spi_drop_chan_data_ctl(dev_id, 0xff, TRUE);
  }

  if ( CS_IN_TEST_ENV() ) {
    /* fake TBI or SPI3 status */
    ARN5_t  * pDev ;
    pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

    CS_REG_WRITE(&pDev->SPI4P2.mif_status.wrd,
      ((pDevCfg->summ.host_if == ARN5_HOST_IF_SPI3) ? 0x0004 : 0)) ;
  }

  /* host */
  if (arn5_spi_dev_init_cfg_common(dev_id, pDevCfg)
       != CS_OK) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INIT_FAILED,
                  ": Host SPI\n") ;
    return(CS_ERROR) ;
  }

  /* init device_level statistics */
  if (arn5_spi_init_dev_stats(dev_id) != CS_OK) {
    return (CS_ERROR);
  }

  /*
   * De-assert the sync resets
   */
  if ( arn5_spi42_dev_sync_sreset(dev_id, CS_TX_AND_RX,
                                   CS_RESET_DEASSERT) != CS_OK ) {
      return (CS_ERROR) ;
  }

  /* Init default IRQ */
  if ( arn5_spi_dev_init_irq_cfg(dev_id, &pDevCfg->spi) != CS_OK ) {
    return (CS_ERROR) ;
  }

  //CS_PRINT("\n\t *** SPI Block Init done! ***\n");

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  GET DEFAULT SPI CONFIGURATION                   */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_spi_dev_get_default_cfg(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to configuration structure            */
/* OUTPUTS    : o SPI default config                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the driver's default SPI configuration for both host     */
/* and line based on specified summary config info.             */
/*                                                              */
/* The [pDevCfg] parameter is a pointer to the configuration    */
/* data-structure. The API will fill in this structure with     */
/* default settings based on the config summary info.           */
/*                                                              */
/* The API expects the data structure (pCfg) already            */
/* allocated, and the config summary filled.                    */
/* $_hdr_end                                                    */
/****************************************************************/
{
  /* host */
  arn5_spi_dev_get_default_cfg_common(dev_id, pDevCfg);


  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  GET SPI RUNNING CONFIGURATION                   */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_get_running_cfg(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to running config structure           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time SPI hardware configuration      */
/* of the specified port.                                       */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to all                  */
/* configuration data-structure.                                */
/* $_hdr_end                                                    */
/****************************************************************/
{
  /* host */
  if (arn5_spi_dev_get_running_cfg_common(dev_id, pRunCfg)
       != CS_OK) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  DUMP SPI RUNNING CONFIGURATION                  */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_spi_dev_dump_running_cfg(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to config struct of the whole device  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Prints the current run-time hardware configuration for the   */
/* this block.                                                  */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the running          */
/* configuration data-structure allocated by the caller with    */
/* the configuration data from HW already retrieved and filled  */
/* in, prior to calling this API.                               */
/* $_hdr_end                                                    */
/****************************************************************/
{
  arn5_spi_dev_dump_running_cfg_common(dev_id, pRunCfg) ;

  return (CS_OK) ;
}



/****************************************************************
 *******************   SPI PORT-LEVEL API's    ******************
 ****************************************************************/

/****************************************************************/
/* _hdr_start  SPI PORT INITIALIZATION                          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_spi_init_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to device configuration struct        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the port level of the SPI block             */
/* with the given summary.                                      */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/* _hdr_end                                                     */
/****************************************************************/
{
  cs_uint8              sliceNum ;
  cs_uint16             devId ;
  arn5_spi_port_cfg_t    * pCfg = &pPortCfg->spi ;

  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  sliceNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;

  if (arn5_spi_init_data(port_id, pPortCfg)
      != CS_OK) {
    return CS_ERROR ;
  }

  /* allow traffic flow */
  arn5_spi_drop_chan_data_ctl(devId, sliceNum, FALSE) ;

  if (arn5_spi_ctl_store_forward_mode(port_id,
        pCfg->store_fwd, pCfg->gfp_pli_insert)
      != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* init port_level statistics */
  if (arn5_spi_init_port_stats(port_id)
      != CS_OK) {
    return (CS_ERROR);
  }

  //CS_PRINT("\n\t *** SPI Block Init done! ***\n");

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  GET DEFAULT SPI PORT CONFIGURATION              */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_spi_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to configuration structure            */
/* OUTPUTS    : o SPI port related default config               */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the port's default SPI configuration for both host       */
/* and line based on specified summary config info.             */
/*                                                              */
/* The [pPortCfg] parameter is a pointer to the configuration   */
/* data-structure. The API will fill in this structure with     */
/* default settings based on the config summary info.           */
/*                                                              */
/* The API expects the config summary filled.                   */
/* $_hdr_end                                                    */
/****************************************************************/
{
  arn5_spi_port_cfg_t    * pCfg = &pPortCfg->spi ;

  pCfg->store_fwd = CS_DISABLE ;
  pCfg->gfp_pli_insert = FALSE ;

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  COMMIT DEFAULT SPI PORT CONFIGURATION           */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
void arn5_spi_commit_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to configuration structure            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : None                                            */
/* DESCRIPTION:                                                 */
/* Commit the port's resource usage. From this point on,        */
/* resource needs to be freed specifically if no longer needed  */
/* $_hdr_end                                                    */
/****************************************************************/
{
}


/****************************************************************/
/* $_hdr_start  GET SPI RUNNING CONFIGURATION                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to SPI running config structure       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time SPI hardware configuration      */
/* of the specified port.                                       */
/* The [pRunCfg] parameter is a pointer to the port's complete  */
/* configuration data-structure.                                */
/*                                                              */
/* This API will fill in the all the fields in the block        */
/* run-time configuration datastructure.                        */
/* $_hdr_end                                                    */
/****************************************************************/
{
  ARN5_t                         * pDev ;
  arn5_spi_port_runcfg_t         * pCfg ;
  ARN5_SPI4P2A_TXCFG_phy_ch0_t   chCfg;
  volatile cs_reg               * pReg ;
  cs_uint8                      sliceNum ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sliceNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  pCfg = &pRunCfg->spi ;

  pReg = (cs_reg *)&pDev->SPI4P2a.phy_ch0 + sliceNum ;
  chCfg.wrd = CS_REG_READ(pReg) ;
  pCfg->store_fwd = (chCfg.bf.store_fwd == 1) ?
        CS_ENABLE :  CS_DISABLE ;
  pCfg->gfp_pli_insert = (chCfg.bf.gfp_pli == 1) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  DUMP SPI RUNNING CONFIGURATION                  */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_spi_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to SPI config structure               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Prints the current run-time hardware configuration for the   */
/* this block.                                                  */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the running          */
/* configuration data-structure allocated by the caller with    */
/* the configuration data from HW already retrieved and filled  */
/* in, prior to calling this API.                               */
/* $_hdr_end                                                    */
/****************************************************************/
{
  arn5_spi_port_runcfg_t * pCfg ;

  pCfg = &pRunCfg->spi ;

  CS_PRINT("\n\t*** Misc Controls ***\n") ;
  CS_PRINT("Store Forward           = %s\n",
             (pCfg->store_fwd == CS_ENABLE) ? "On" : "Off");
  CS_PRINT("GFP PLI Insert          = %s\n",
             pCfg->gfp_pli_insert ? "On" : "Off");

  return (CS_OK) ;
}



/****************************************************************
 *****************     Internal functions       *****************
 ****************************************************************/

/*-----------------------------------------------------------------*/
/* this common part of device init between host and line interface */
/*-----------------------------------------------------------------*/
static cs_status arn5_spi_dev_init_cfg_common(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg)
{
  arn5_dev_summ_t       * pSumm;
  arn5_spi_dev_cfg_t    * pCfg;
  ARN5_t                * pDev ;
  cs_uint8              i ;

  pSumm = &pDevCfg->summ ;
  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  pCfg = &pDevCfg->spi ;

  /* match desired interface mode with hardware selection */
  /* don't need to do so since summ validation is done by now */

  {
    if (pCfg->spi.use_rx_map_for_rx_tx) {
      for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
        pCfg->spi.tx_lch_map[i] = pCfg->spi.rx_lch_map[i] ;
      }
    }

    if (arn5_spi_dev_set_chanmap(dev_id, CS_RX,
          pCfg->spi.rx_lch_map[0], pCfg->spi.rx_lch_map[1],
          pCfg->spi.rx_lch_map[2], pCfg->spi.rx_lch_map[3],
          pCfg->spi.rx_lch_map[4], pCfg->spi.rx_lch_map[5],
          pCfg->spi.rx_lch_map[6], pCfg->spi.rx_lch_map[7])
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi_dev_set_chanmap(dev_id, CS_TX,
          pCfg->spi.tx_lch_map[0], pCfg->spi.tx_lch_map[1],
          pCfg->spi.tx_lch_map[2], pCfg->spi.tx_lch_map[3],
          pCfg->spi.tx_lch_map[4], pCfg->spi.tx_lch_map[5],
          pCfg->spi.tx_lch_map[6], pCfg->spi.tx_lch_map[7])
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi_dev_set_pll_controls(dev_id, CS_RX,
          pCfg->spi.rx_pll_params.freq_tune,
          pCfg->spi.rx_pll_params.multiplier_sel)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi_dev_set_pll_controls(dev_id, CS_TX,
          pCfg->spi.tx_pll_params.freq_tune,
          pCfg->spi.tx_pll_params.multiplier_sel)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi_dev_set_burst_params(dev_id,
          pCfg->spi.burst_params.burst,
          pCfg->spi.burst_params.maxburst1,
          pCfg->spi.burst_params.maxburst2)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
      if (arn5_spi_dev_set_rx_fifo_params(dev_id, i,
            pCfg->spi.rx_fifo_params[i].low_wmark,
            pCfg->spi.rx_fifo_params[i].high_wmark,
            pCfg->spi.rx_fifo_params[i].hysteresis_wmark)
            != CS_OK ) {
        return (CS_ERROR) ;
      }
    }

    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
      if (arn5_spi_dev_set_tx_fifo_params(dev_id, i,
            pCfg->spi.tx_fifo_params[i].low_addr,
            pCfg->spi.tx_fifo_params[i].high_addr,
            pCfg->spi.tx_fifo_params[i].low_wmark,
            pCfg->spi.tx_fifo_params[i].high_wmark)
            != CS_OK ) {
        return (CS_ERROR) ;
      }
    }

    /* don't do loopback here */
    /* setup the whole thing in normal first and then apply */
    /* needed loopbacks                                     */
  }

  if (arn5_spi_dev_is_in_spi3_mode(dev_id)) {
    /* spi3 specific */
    if (arn5_spi3_dev_set_modes(dev_id,
          pCfg->spi3.eight_bit_mode)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi3_dev_ctl_stall_on_single_fifo_full(dev_id,
          pCfg->spi3.stall_on_single_fifo_full)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi3_dev_set_controls(dev_id,
          pCfg->spi3.ptpa_enable,
          pCfg->spi3.stpa_enable)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi3_dev_set_pause_cycles(dev_id,
          pCfg->spi3.npause)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

#if 0
/* FIXME, check later */
    /* since it is a phy device, disable rx ptpa and stpa */
    if (arn5_spi3_dev_set_controls(dev_id, CS_RX , FALSE, FALSE)
        != CS_OK ) {
      return (CS_ERROR) ;
    }
#endif
  }

  if (arn5_spi_dev_is_in_spi42_mode(dev_id)) {
    /* spi42 specific */
    if (arn5_spi42_dev_set_rate(dev_id,
          pCfg->spi42.quarter_rate)
          != CS_OK ) {
      return(CS_ERROR) ;
    }

    if (arn5_spi42_dev_set_training_params(dev_id,
          pCfg->spi42.training.alpha,
          pCfg->spi42.training.datamaxt)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi42_dev_set_dip2_tholds(dev_id,
          pCfg->spi42.dip2.error_thold,
          pCfg->spi42.dip2.match_thold)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi42_dev_set_dip4_tholds(dev_id,
          pCfg->spi42.dip4.error_thold,
          pCfg->spi42.dip4.match_thold)
          != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi42_dev_set_afifo_hysteresis(dev_id,
          pCfg->spi42.afifo_hysteresis)
          != CS_OK ) {
      return (CS_ERROR) ;
    }
  }

  /* apply loopback */
  if (arn5_spi_dev_ctl_loopback(dev_id,
                               CS_TX, pCfg->spi.tx_lpbk)
        != CS_OK ) {
    return (CS_ERROR) ;
  }

  if (arn5_spi_dev_ctl_loopback(dev_id,
                               CS_RX, pCfg->spi.rx_lpbk)
          != CS_OK ) {
    return (CS_ERROR) ;
  }

  /*
   * Done with configuring the registers. SRESET(logic reset) the
   * block.
   *
   * Soft reset does not affect any configuration registers except may be
   * the calendar programming. So, do the calendar configuration after doing
   * soft reset.
   */
  if ( arn5_spi_dev_sreset(dev_id) != CS_OK ) {
    return (CS_ERROR) ;
  }

  {
    if (arn5_spi_dev_set_cal_params(dev_id,
                CS_TX, &pCfg->spi.tx_cal)
        != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi_dev_set_cal_params(dev_id,
                CS_RX, &pCfg->spi.rx_cal)
        != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi_dev_set_rx_scheduler_cal_len(dev_id,
                pCfg->spi.rx_scheduler_cal_len)
        != CS_OK ) {
      return (CS_ERROR) ;
    }

    if (arn5_spi_dev_set_tx_scheduler_entries(dev_id,
          pCfg->spi.tx_scheduler_0_1_2_3, pCfg->spi.tx_scheduler_4_5_6_7,
          pCfg->spi.tx_scheduler_8_9_10_11, pCfg->spi.tx_scheduler_12_13_14_15)
        != CS_OK ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/*-----------------------------------------------------------------*/
/* the common part of device get default config between host and   */
/* line interface                                                  */
/*-----------------------------------------------------------------*/
static cs_status arn5_spi_dev_get_default_cfg_common(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg)
{
  arn5_spi_dev_cfg_t    * pCfg = &pDevCfg->spi ;
  arn5_dev_summ_t       * pSumm = &pDevCfg->summ ;
  cs_uint8              i ;

  /* logical chan assignment */
  for (i = 0 ; i < ARN5_MAX_NUM_CHANNELS ; i++) {
    pCfg->spi.rx_lch_map[i] = i ;
    pCfg->spi.tx_lch_map[i] = i ;
  }
  pCfg->spi.use_rx_map_for_rx_tx = TRUE ;

  /* pll params */
  pCfg->spi.rx_pll_params.freq_tune = 0x04 ;
  pCfg->spi.tx_pll_params.freq_tune = 0x04 ;

  pCfg->spi.rx_pll_params.multiplier_sel =
      (pSumm->host_if == ARN5_HOST_IF_SPI3) ?
              3 : ARN5_SPI_PLL_MULT_8 ;

/*  pCfg->spi.tx_pll_params.multiplier_sel =
      pCfg->spi.rx_pll_params.multiplier_sel ;*/
  pCfg->spi.tx_pll_params.multiplier_sel = ARN5_SPI_PLL_MULT_2; 	

  /* burst params */
  pCfg->spi.burst_params.burst     = 0x3 ;  /* (3+1) * 16 = 64 bytes */
  pCfg->spi.burst_params.maxburst1 = 0xc ;  /* 12 * 16 = 192 bytes */
  pCfg->spi.burst_params.maxburst2 = 0x8 ;  /* 8 * 16 = 128 bytes */

  /* calender params */
  arn5_spi_cal_programming(dev_id, pDevCfg, ARN5_CAL_ROUND_ROBIN) ;

  /* tx fifo params */
  pCfg->spi.rx_fifo_params[0].low_wmark  = 0x0040; /* () * 128 bits */
  pCfg->spi.rx_fifo_params[0].high_wmark = 0x0060; /* () * 128 bits */
  pCfg->spi.rx_fifo_params[0].hysteresis_wmark = 0x0010; /* () * 128 bits */

  pCfg->spi.rx_fifo_params[1].low_wmark  = 0x0040;
  pCfg->spi.rx_fifo_params[1].high_wmark = 0x0060;
  pCfg->spi.rx_fifo_params[1].hysteresis_wmark = 0x0010;

  pCfg->spi.rx_fifo_params[2].low_wmark  = 0x0040;
  pCfg->spi.rx_fifo_params[2].high_wmark = 0x0060;
  pCfg->spi.rx_fifo_params[2].hysteresis_wmark = 0x0010;

  pCfg->spi.rx_fifo_params[3].low_wmark  = 0x0040;
  pCfg->spi.rx_fifo_params[3].high_wmark = 0x0060;
  pCfg->spi.rx_fifo_params[3].hysteresis_wmark = 0x0010;

  pCfg->spi.rx_fifo_params[4].low_wmark  = 0x0040;
  pCfg->spi.rx_fifo_params[4].high_wmark = 0x0060;
  pCfg->spi.rx_fifo_params[4].hysteresis_wmark = 0x0010;

  pCfg->spi.rx_fifo_params[5].low_wmark  = 0x0040;
  pCfg->spi.rx_fifo_params[5].high_wmark = 0x0060;
  pCfg->spi.rx_fifo_params[5].hysteresis_wmark = 0x0010;

  pCfg->spi.rx_fifo_params[6].low_wmark  = 0x0040;
  pCfg->spi.rx_fifo_params[6].high_wmark = 0x0060;
  pCfg->spi.rx_fifo_params[6].hysteresis_wmark = 0x0010;

  pCfg->spi.rx_fifo_params[7].low_wmark  = 0x0040;
  pCfg->spi.rx_fifo_params[7].high_wmark = 0x0060;
  pCfg->spi.rx_fifo_params[7].hysteresis_wmark = 0x0010;

  /* tx fifo params */
  pCfg->spi.tx_fifo_params[0].low_addr = 0 ;
  pCfg->spi.tx_fifo_params[0].high_addr = 0x37f ;
  pCfg->spi.tx_fifo_params[0].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[0].high_wmark = 0x1c0 ;

  pCfg->spi.tx_fifo_params[1].low_addr = 0x380 ;
  pCfg->spi.tx_fifo_params[1].high_addr = 0x6ff ;
  pCfg->spi.tx_fifo_params[1].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[1].high_wmark = 0x1c0 ;

  pCfg->spi.tx_fifo_params[2].low_addr = 0x700 ;
  pCfg->spi.tx_fifo_params[2].high_addr = 0xa7f ;
  pCfg->spi.tx_fifo_params[2].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[2].high_wmark = 0x1c0 ;

  pCfg->spi.tx_fifo_params[3].low_addr = 0xa80 ;
  pCfg->spi.tx_fifo_params[3].high_addr = 0xdff ;
  pCfg->spi.tx_fifo_params[3].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[3].high_wmark = 0x1c0 ;

  pCfg->spi.tx_fifo_params[4].low_addr = 0xe00 ;
  pCfg->spi.tx_fifo_params[4].high_addr = 0x117f ;
  pCfg->spi.tx_fifo_params[4].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[4].high_wmark = 0x1c0 ;

  pCfg->spi.tx_fifo_params[5].low_addr = 0x1180 ;
  pCfg->spi.tx_fifo_params[5].high_addr = 0x14ff ;
  pCfg->spi.tx_fifo_params[5].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[5].high_wmark = 0x1c0 ;

  pCfg->spi.tx_fifo_params[6].low_addr = 0x1500 ;
  pCfg->spi.tx_fifo_params[6].high_addr = 0x187f ;
  pCfg->spi.tx_fifo_params[6].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[6].high_wmark = 0x1c0 ;

  pCfg->spi.tx_fifo_params[7].low_addr = 0x1880 ;
  pCfg->spi.tx_fifo_params[7].high_addr = 0x1bff ;
  pCfg->spi.tx_fifo_params[7].low_wmark = 0xe0 ;
  pCfg->spi.tx_fifo_params[7].high_wmark = 0x1c0 ;

  /* Loopbacks */
  pCfg->spi.tx_lpbk = FALSE ;
  pCfg->spi.rx_lpbk = FALSE ;

  /* spi42 rate selector */
  pCfg->spi42.quarter_rate = FALSE ;

  /* Training */
  pCfg->spi42.training.alpha    = 3 ;
  pCfg->spi42.training.datamaxt = (1000 * pCfg->spi42.training.alpha);

  /* DIP-2 and DIP-4 thresholds */
  pCfg->spi42.dip2.error_thold = 1 ;
  pCfg->spi42.dip2.match_thold = 4 ;
  pCfg->spi42.dip4.error_thold = 1 ;
  pCfg->spi42.dip4.match_thold = 0x0c80 ;

  /* async_fifo hysteresis */
  pCfg->spi42.afifo_hysteresis = 9 ;

  /* spi3 specific */
  pCfg->spi3.eight_bit_mode = FALSE ;
  pCfg->spi3.stall_on_single_fifo_full = FALSE ;
  pCfg->spi3.npause      = 0 ;
  pCfg->spi3.ptpa_enable = TRUE ;
  pCfg->spi3.stpa_enable = TRUE ;

  arn5_spi_dev_get_default_irq_cfg(dev_id, pSumm, pCfg) ;

  return (CS_OK) ;
}


/*-----------------------------------------------------------------*/
/* this common part of get device running config between host and  */
/* line interface                                                  */
/*-----------------------------------------------------------------*/
static cs_status arn5_spi_dev_get_running_cfg_common(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg)
{
  ARN5_t                * pDev ;
  ARN5_SPI4P2_t         * pSpi ;
  arn5_dev_cb_t         * pDevCb ;
  arn5_spi_dev_runcfg_t * pCfg = &pRunCfg->spi ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;
  pDevCb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  /* logical chan map, reflects rx direction only */
  {
    cs_uint8            log, i ;
    volatile cs_reg     * pReg ;

    pReg = &pSpi->lch_num_pch0.wrd ;
    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++, pReg++) {
      log = CS_REG_READ(pReg) ;
      if (log != pDevCb->rx_phy_chan_map[i]) {
        CS_PRINT("rx Phy(%d)->Log chan map mismatch, log target = %d, act = %d\n",
               i, pDevCb->rx_phy_chan_map[i], log);
      }
      pCfg->spi.rx_lch_map[i] = pDevCb->rx_phy_chan_map[i] ;
    }
  }

  /* logical chan map, reflects tx direction only */
  {
    cs_uint8            phy, i ;
    volatile cs_reg     * pReg ;

    pReg = &pSpi->phy_ch_num_lch0.wrd ;
    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++, pReg++) {
      phy = CS_REG_READ(pReg) ;
      if (phy != pDevCb->tx_log_chan_map[i]) {
        CS_PRINT("tx Log(%d)->Phy chan map mismatch, phy target = %d, act = %d\n",
               i, pDevCb->tx_log_chan_map[i], phy);
      }
      pCfg->spi.tx_lch_map[i] = pDevCb->tx_phy_chan_map[i] ;
    }
  }

  /* drop bits */
  {
    cs_uint8    i, val;
    ARN5_SPI4P2A_TXCFG_phy_ch0_t  txCfg ;

    pRunCfg->spi.rx_drop_bits =
      (cs_uint8 )CS_REG_READ((cs_reg *)&pDev->SPI4P2a.dropch) ;

    pRunCfg->spi.tx_drop_bits = 0 ;
    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
      txCfg.wrd =
        CS_REG_READ((cs_reg *)&pDev->SPI4P2a.phy_ch0 + i) ;
      val = txCfg.bf.drop ;
      val <<= i ;
      pRunCfg->spi.tx_drop_bits |= val ;
    }

  }

  /* LDVS stat */
  {
    ARN5_SPI4P2_MIF_status_t mifStatus;

    mifStatus.wrd = CS_REG_READ(&pSpi->mif_status.wrd) ;
    pRunCfg->spi.lvds = (mifStatus.bf.lvdsstat == 1) ;
  }

  /* PLL parameters */
  {
    ARN5_SPI4P2_MIF_txpllcontrol1_t  txPllCtl;
    ARN5_SPI4P2_MIF_rxpllcontrol1_t  rxPllCtl;

    txPllCtl.wrd = CS_REG_READ(&pSpi->txpllcontrol1.wrd) ;
    rxPllCtl.wrd = CS_REG_READ(&pSpi->rxpllcontrol1.wrd) ;

    pCfg->spi.rx_pll_params.freq_tune = rxPllCtl.bf.freqtune ;
    pCfg->spi.tx_pll_params.freq_tune = txPllCtl.bf.freqtune ;
    pCfg->spi.rx_pll_params.multiplier_sel = rxPllCtl.bf.multsel ;
    pCfg->spi.tx_pll_params.multiplier_sel = txPllCtl.bf.multsel ;
  }

  /* Burst parameters */
  {
    arn5_spi_burst_params_t  * p = &pCfg->spi.burst_params ;

    p->burst = CS_REG_READ(&pSpi->burst.wrd) ;
    p->maxburst1  = CS_REG_READ(&pSpi->maxburst1.wrd) ;
    p->maxburst2  = CS_REG_READ(&pSpi->maxburst2.wrd) ;
  }

  /* Tx calendar */
  {
    arn5_spi_cal_params_t   * p = &pCfg->spi.tx_cal ;
    cs_reg                 * pReg ;
    cs_uint16                i ;

    pReg = (cs_reg *)&pSpi->txcal_cal_m.wrd ;
    p->m   = CS_REG_READ(pReg) + 1 ;

    pReg = (cs_reg *)&pSpi->txcal_cal_len.wrd ;
    p->len = CS_REG_READ(pReg) + 1 ;

    pReg = (cs_reg *)&pSpi->txcal_ent_0.wrd ;

    for (i = 0; i < p->len; i++, pReg++) {
      p->ent[i] = CS_REG_READ(pReg) ;
    }

    for (; i < ARN5_SPI_MAX_CAL_ENTRY; i++) {
      p->ent[i] = 0xff ;
    }
  }

  /* Rx calendar */
  {
    arn5_spi_cal_params_t   * p = &pCfg->spi.rx_cal ;
    cs_reg                 * pReg;
    cs_uint16                i;
    cs_uint16           actLen  = pCfg->spi.rx_scheduler_cal_len ;

    pReg = (cs_reg *)&pSpi->rxcal_cal_m.wrd ;
    p->m   = CS_REG_READ(pReg) + 1 ;

    pReg = (cs_reg *)&pSpi->rxcal_cal_len.wrd ;
    p->len = CS_REG_READ(pReg) + 1 ;

    pReg = (cs_reg *)&pSpi->rxcal_ent_0.wrd ;

    actLen = p->len ;

    for (i = 0; i < actLen; i++, pReg++) {
      p->ent[i] = CS_REG_READ(pReg) ;
    }

    for (; i < ARN5_SPI_MAX_CAL_ENTRY; i++) {
      p->ent[i] = 0xff ;
    }
  }

  /* Rx Scheduler Cal Len */
  {
    pCfg->spi.rx_scheduler_cal_len =
        CS_REG_READ(&pSpi->sched_cal_len.wrd) + 1 ;
  }

  /* Tx Scheduler Entries */
  {
    pCfg->spi.tx_scheduler_0_1_2_3 =
        CS_REG_READ(&pDev->SPI4P2a.tx_sched_0.wrd) ;

    pCfg->spi.tx_scheduler_4_5_6_7 =
        CS_REG_READ(&pDev->SPI4P2a.tx_sched_1.wrd) ;

    pCfg->spi.tx_scheduler_8_9_10_11 =
        CS_REG_READ(&pDev->SPI4P2a.tx_sched_2.wrd) ;

    pCfg->spi.tx_scheduler_12_13_14_15 =
        CS_REG_READ(&pDev->SPI4P2a.tx_sched_3.wrd) ;
  }

  /* Rx FIFO params */
  {
    cs_uint32  i ;
    arn5_spi_dev_common_cfg_t   * p = &pCfg->spi ;

    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; ++i)
    {
      p->rx_fifo_params[i].low_wmark =
        CS_REG_READ(&pDev->SPI4P2.almost_empty_0.wrd + (i * 3)) ;
      p->rx_fifo_params[i].high_wmark =
        CS_REG_READ(&pDev->SPI4P2.almost_full_0.wrd + (i * 3)) ;
      p->rx_fifo_params[i].hysteresis_wmark =
        CS_REG_READ(&pDev->SPI4P2.hysteresis_0.wrd + (i * 3)) ;
    }
  }

  /* Tx FIFO params */
  {
    cs_uint32  i ;
    arn5_spi_dev_common_cfg_t   * p = &pCfg->spi ;

    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; ++i)
    {
      p->tx_fifo_params[i].low_addr =
        CS_REG_READ(&pDev->SPI4P2a.mem_lo_0.wrd + i) ;
      p->tx_fifo_params[i].high_addr =
        CS_REG_READ(&pDev->SPI4P2a.mem_hi_0.wrd + i) ;
      p->tx_fifo_params[i].low_wmark =
        CS_REG_READ(&pDev->SPI4P2a.almost_empty.wrd + (i * 16)) ;
      p->tx_fifo_params[i].high_wmark =
        CS_REG_READ(&pDev->SPI4P2a.almost_full.wrd + (i * 16)) ;
    }
  }

  /* Loopbacks */
  {
    if (arn5_spi_dev_is_in_spi42_mode(dev_id)) {
      ARN5_SPI4P2_MIF_loopbacks_t lpbk;

      lpbk.wrd = CS_REG_READ(&pSpi->loopbacks.wrd);

      /* host */
      pCfg->spi.tx_lpbk =
          (lpbk.bf.bertlb) ? TRUE : FALSE ;
      pCfg->spi.rx_lpbk =
          (lpbk.bf.lb) ? TRUE : FALSE ;
    }
    else {
      ARN5_SPI4P2_TXIF_spi3ctl_t txif_spi3ctl;
      txif_spi3ctl.wrd = CS_REG_READ(&pSpi->txif_spi3ctl.wrd) ;

      if (txif_spi3ctl.bf.tx2rx == 1) {
        pCfg->spi.tx_lpbk = 1 ;
      }
      else {
        pCfg->spi.tx_lpbk = 0 ;
      }

      if (txif_spi3ctl.bf.rx2tx == 1) {
        pCfg->spi.rx_lpbk = 1 ;
      }
      else {
        pCfg->spi.rx_lpbk = 0 ;
      }
    }
  }

  /* SPI42 quarter rate */
  {
    ARN5_SPI4P2_MIF_mode_t   mode ;
    mode.wrd = CS_REG_READ(&pSpi->mode.wrd);
    if (mode.bf.quarterrate_override == 1) {
      pCfg->spi42.quarter_rate = mode.bf.quarterrate_value ;
    }
    else {
      ARN5_SPI4P2_MIF_status_t status ;
      status.wrd = CS_REG_READ(&pSpi->mif_status.wrd) ;
      pCfg->spi42.quarter_rate = status.bf.quarterrate ;
    }
  }

  /* Training parameters */
  {
    arn5_spi42_training_t  * p = &pCfg->spi42.training ;

    p->alpha    = CS_REG_READ(&pSpi->alpha.wrd) ;
    p->datamaxt = CS_REG_READ(&pSpi->datamaxt.wrd) ;
  }

  /* DIP-2 thresholds */
  {
    arn5_spi42_dip2_t  * p = &pCfg->spi42.dip2 ;

    p->error_thold = CS_REG_READ(&pSpi->numdip2err.wrd) ;
    p->match_thold = CS_REG_READ(&pSpi->numdip2match.wrd) ;
  }

  /* DIP-4 thresholds */
  {
    arn5_spi42_dip4_t  * p = &pCfg->spi42.dip4 ;

    p->error_thold = CS_REG_READ(&pSpi->numdip4err.wrd) ;
    p->match_thold = CS_REG_READ(&pSpi->numdip4match.wrd) ;

  }

  /* async_fifo hysteresis */
  {
    pCfg->spi42.afifo_hysteresis = CS_REG_READ(
      &pSpi->spi42_hysteresis.wrd) ;
  }

  /* 8 bit mode */
  {
    ARN5_SPI4P2_TXIF_spi3ctl_t tCtl ;
    tCtl.wrd = CS_REG_READ(&pSpi->txif_spi3ctl.wrd) ;
    pCfg->spi3.eight_bit_mode =
      (tCtl.bf.Eightbmode == 1) ;
  }

  /* stall on single fifo full */
  {
    ARN5_SPI4P2_TXCORE_spi3_single_fifo_full_t fifoCtl ;
    fifoCtl.wrd = CS_REG_READ(&pSpi->spi3_single_fifo_full.wrd) ;
    pCfg->spi3.stall_on_single_fifo_full =
      (fifoCtl.bf.stall_mode == 1) ;
  }

  /* npause */
  {
    ARN5_SPI4P2_RXIF_spi3ctl_t rCtl  ;
    rCtl.wrd = CS_REG_READ(&pSpi->rxif_spi3ctl.wrd) ;
    pCfg->spi3.npause = rCtl.bf.npause ;
  }

  /* ptpa & stpa enable */
  {
    /* ARN5 acts as Phy device on SPI3 */
    ARN5_SPI4P2_TXIF_spi3ctl_t  tCtl ;

    tCtl.wrd = CS_REG_READ(&pSpi->txif_spi3ctl.wrd) ;
    pCfg->spi3.ptpa_enable = (tCtl.bf.ptpa_enable == 1) ;
    pCfg->spi3.stpa_enable = (tCtl.bf.stpa_enable == 1) ;
  }
  return (CS_OK) ;
}


/*-----------------------------------------------------------------*/
/* general routine to dump device running config for host          */
/* interface                                                       */
/*-----------------------------------------------------------------*/
static cs_status arn5_spi_dev_dump_running_cfg_common(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg)
{
  cs_uint8              i ;
  arn5_spi_dev_runcfg_t * pCfg = &pRunCfg->spi ;

  CS_PRINT("\n\t *** Logical Channel Map ***\n") ;
  CS_PRINT("Tx = ") ;
  for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
    CS_PRINT(" %2d", pCfg->spi.tx_lch_map[i]) ;
  }
  CS_PRINT("\n") ;
  CS_PRINT("Rx = ") ;
  for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
    CS_PRINT(" %2d", pCfg->spi.rx_lch_map[i]) ;
  }
  CS_PRINT("\n\n") ;
  CS_PRINT("tx drop bits           = 0x%02x\n", pRunCfg->spi.tx_drop_bits) ;
  CS_PRINT("rx drop bits           = 0x%02x\n", pRunCfg->spi.rx_drop_bits) ;
  CS_PRINT("LDVS                   = %s\n",
     pRunCfg->spi.lvds ? "TRUE" : "FALSE") ;

  /* PLL parameters */
  {
    CS_PRINT("\n\t *** PLL Parameters ***\n") ;
    CS_PRINT("Tx Freq Tune           = 0x%x \n",
             pCfg->spi.tx_pll_params.freq_tune) ;
    CS_PRINT("Tx Multiplier          = ") ;
    if (pCfg->spi.tx_pll_params.multiplier_sel == ARN5_SPI_PLL_MULT_2) {
      CS_PRINT("x 2\n") ;
    }
    else if (pCfg->spi.tx_pll_params.multiplier_sel == ARN5_SPI_PLL_MULT_4) {
      CS_PRINT("x 4\n") ;
    }
    else if (pCfg->spi.tx_pll_params.multiplier_sel == ARN5_SPI_PLL_MULT_8) {
      CS_PRINT("x 8\n") ;
    }
    else {
      CS_PRINT("SPI3 only (3)\n") ;
    }

    CS_PRINT("Rx Freq Tune           = 0x%x \n",
             pCfg->spi.rx_pll_params.freq_tune) ;
    CS_PRINT("Rx Multiplier          = ") ;
    if (pCfg->spi.rx_pll_params.multiplier_sel == ARN5_SPI_PLL_MULT_2) {
      CS_PRINT("x 2\n") ;
    }
    else if (pCfg->spi.rx_pll_params.multiplier_sel == ARN5_SPI_PLL_MULT_4) {
      CS_PRINT("x 4\n") ;
    }
    else if (pCfg->spi.rx_pll_params.multiplier_sel == ARN5_SPI_PLL_MULT_8) {
      CS_PRINT("x 8\n") ;
    }
    else {
      CS_PRINT("SPI3 only (3)\n") ;
    }
  }

  /* Burst parameters */
  {
    arn5_spi_burst_params_t  * p = &pCfg->spi.burst_params ;

    CS_PRINT("\n\t *** Burst Parameters ***\n") ;
    CS_PRINT("Burst Size             = 0x%x (or 0x%x bytes)\n",
             p->burst, (p->burst + 1) * 16) ;
    CS_PRINT("Maxburst1              = 0x%x (or 0x%x bytes)\n",
             p->maxburst1, p->maxburst1 * 16) ;
    CS_PRINT("Maxburst2              = 0x%x (or 0x%x bytes)\n",
             p->maxburst2, p->maxburst2 * 16) ;
  }

  /* Tx calendar */
  {
    arn5_spi_cal_params_t  * p ;

    p = &pCfg->spi.tx_cal ;
    CS_PRINT("\n\t *** Tx Calendar ***\n") ;
    CS_PRINT("m (num of repetitions) = %d\n", p->m) ;
    CS_PRINT("Calendar Length        = %d\n", p->len) ;
    CS_PRINT("Calendar Entries       =\n") ;
    for (i = 0; i < ARN5_SPI_MAX_CAL_ENTRY; i++) {
      if (p->ent[i] == 0xff) break ;
      CS_PRINT(" %2d", p->ent[i]) ;
      if ((i % 20) == 19) {
        CS_PRINT("\n") ;
      }
    }
    CS_PRINT("\n") ;
  }

  /* Rx calendar */
  {
    arn5_spi_cal_params_t  * p ;
    p = &pCfg->spi.rx_cal ;

    CS_PRINT("\n\t *** Rx Calendar ***\n") ;
    CS_PRINT("m (num of repetitions) = %d\n", p->m) ;
    CS_PRINT("Calendar length        = %d\n", p->len) ;
    CS_PRINT("Scheduler Cal Length   = %d\n",
             pCfg->spi.rx_scheduler_cal_len) ;

    CS_PRINT("Calendar entries       =\n") ;
    for (i = 0; i < ARN5_SPI_MAX_CAL_ENTRY; i++) {
      if (p->ent[i] == 0xff) break ;
      CS_PRINT(" %2d", p->ent[i]) ;
      if ((i % 20) == 19) {
        CS_PRINT("\n") ;
      }
    }
    CS_PRINT("\n") ;
  }

  /* Tx Scheduler */
  {
    arn5_spi_dev_common_cfg_t* p ;
    p = &pCfg->spi ;

    CS_PRINT("\n\t *** Tx Scheduler ***\n") ;
    CS_PRINT("Entries       = ") ;
    CS_PRINT("%04x%04x%04x%04x\n", p->tx_scheduler_0_1_2_3,
        p->tx_scheduler_4_5_6_7, p->tx_scheduler_8_9_10_11,
        p->tx_scheduler_12_13_14_15) ;
  }

  /* tx fifo params */
  {
    cs_uint8    i ;
    arn5_spi_tx_fifo_params_t    * p ;

    CS_PRINT("\n\t *** TX FIFO Params ***\n") ;
    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
      p = &pCfg->spi.tx_fifo_params[i] ;
      CS_PRINT("Ch_%1d Low Addr          = 0x%x\n", i, p->low_addr) ;
      CS_PRINT("Ch_%1d High Addr         = 0x%x\n", i, p->high_addr) ;
      CS_PRINT("Ch_%1d Low Wmark         = 0x%x\n", i, p->low_wmark) ;
      CS_PRINT("Ch_%1d High Wmark        = 0x%x\n", i, p->high_wmark) ;
    }
  }

  /* rx fifo params */
  {
    cs_uint8    i ;
    arn5_spi_rx_fifo_params_t    * p ;

    CS_PRINT("\n\t *** RX FIFO Params ***\n") ;
    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
      p = &pCfg->spi.rx_fifo_params[i] ;
      CS_PRINT("Ch_%1d Low Wmark         = 0x%x\n", i, p->low_wmark) ;
      CS_PRINT("Ch_%1d High Wmark        = 0x%x\n",i, p->high_wmark) ;
      CS_PRINT("Ch_%1d Hysteresis Wmark  = 0x%x\n", i, p->hysteresis_wmark) ;
    }
  }

  /* Loopbacks */
  CS_PRINT("\n\t *** Loopbacks ***\n") ;
  CS_PRINT("Tx-to-Rx Loopback      = %s\n",
           (pCfg->spi.tx_lpbk) ? "ON" : "OFF") ;
  CS_PRINT("Rx-to-Tx Loopback      = %s\n",
           (pCfg->spi.rx_lpbk) ? "ON" : "OFF") ;

  /* SPI4.2 specific */
  if (pRunCfg->summ.host_if == ARN5_HOST_IF_SPI42) {
    CS_PRINT("\n\t *** SPI4.2 Only Params ***\n") ;

    /* quarter rate */
    {
      CS_PRINT("Quarter Rate           = %s\n",
               (pCfg->spi42.quarter_rate) ? "ON" : "OFF") ;
    }

    /* training parameters */
    {
      arn5_spi42_training_t * p = &pCfg->spi42.training ;
      CS_PRINT("Alpha                  = 0x%x (num repetitions)\n",
               p->alpha) ;
      CS_PRINT("DataMaxt               = 0x%x (interval)\n",
               p->datamaxt) ;
    }

    /* DIP-2 thresholds */
    {
      arn5_spi42_dip2_t  * p = &pCfg->spi42.dip2 ;
      CS_PRINT("DIP-2 Error Thresholds = 0x%x\n",
               p->error_thold) ;
      CS_PRINT("DIP-2 Match Thresholds = 0x%x\n",
               p->match_thold) ;
    }

    /* DIP-4 thresholds */
    {
      arn5_spi42_dip4_t  * p = &pCfg->spi42.dip4 ;
      CS_PRINT("DIP-4 Error Thresholds = 0x%x\n",
               p->error_thold) ;
      CS_PRINT("DIP-4 Match Thresholds = 0x%x\n",
               p->match_thold) ;
    }

    {
      CS_PRINT("Async_fifo Hysteresis  = %d\n",
               pCfg->spi42.afifo_hysteresis) ;
    }
  }
  else {
    CS_PRINT("\n\t *** SPI3 Only Params ***\n") ;
    {
      CS_PRINT("Eight bit Mode         = %s\n",
        (pCfg->spi3.eight_bit_mode) ? "ON" : "OFF") ;
      CS_PRINT("Stall On Single FIFO Full = %s\n",
        (pCfg->spi3.stall_on_single_fifo_full) ? "ON" : "OFF") ;
      CS_PRINT("npause                 = %d\n",
        pCfg->spi3.npause) ;
      CS_PRINT("Ptpa control           = %s\n",
        (pCfg->spi3.ptpa_enable) ? "ON" : "OFF") ;
      CS_PRINT("Stpa control           = %s\n",
        (pCfg->spi3.stpa_enable) ? "ON" : "OFF") ;
    }
  }
  return (CS_OK) ;
}


/*-----------------------------------------------------------------*/
/* Control traffic of a given phy chan, drop(1) or pass(0)         */
/* control will be done in both directions                         */
/*-----------------------------------------------------------------*/
cs_status arn5_spi_drop_chan_data_ctl(cs_uint8 dev_id,
                        cs_uint8 phy_chan,
                        cs_boolean ctl)
{
  ARN5_t                * pDev ;
  volatile cs_reg       * pReg ;
  cs_uint16             rxDrop ;
  cs_uint16             mask ;
  cs_uint8              myChan, startChan, endChan, myLogChan ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  if (phy_chan == 0xff) {
    startChan = 0 ;
    endChan = ARN5_MAX_NUM_CHANNELS - 1 ;
  }
  else {
    startChan = phy_chan;
    endChan = phy_chan ;
  }

  ARN5_SPI_DEV_LOCK(dev_id) ;
  for (myChan = startChan; myChan <= endChan; myChan++) {
    ARN5_SPI4P2A_TXCFG_phy_ch0_t  txCfg ;

    /* due to the design in RTL, we need to use logical chan */
    /* to control the drop bit in tx direction               */
    myLogChan = arn5_physical2logical_chan_num(dev_id, CS_TX, myChan) ;
    pReg = (cs_reg *)&pDev->SPI4P2a.phy_ch0 + myLogChan ;
    txCfg.wrd = CS_REG_READ(pReg) ;
    txCfg.bf.drop = ctl ? 1 : 0 ;
    CS_REG_WRITE(pReg, txCfg.wrd) ;

    pReg = (cs_reg *)&pDev->SPI4P2a.dropch.wrd ;
    mask = 1 << myChan ;
    rxDrop = CS_REG_READ(pReg) ;
    rxDrop = (rxDrop & (~mask)) | (ctl ? mask : 0) ;
    CS_REG_WRITE(pReg, rxDrop) ;
  }
  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK);
}


/*-----------------------------------------------------------------*/
/* Calender programing helper utility (with choices of algorithms) */
/* assume tx_cal and rx_cal are the same for now                   */
/* (Scheduler programming are included here                        */
/*-----------------------------------------------------------------*/
static cs_status arn5_spi_cal_programming(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg,
                        arn5_cal_algorithm_t select)
{
  cs_uint8              i ;
  cs_status             retVal = CS_OK ;
  arn5_spi_dev_cfg_t    * pCfg = &pDevCfg->spi ;

  /* initialize all cal entries as "not used" */
  for (i = 0 ; i < ARN5_SPI_MAX_CAL_ENTRY; i++) {
    pCfg->spi.tx_cal.ent[i] = 0xff ;
  }
  for (i = 0 ; i < ARN5_SPI_MAX_CAL_ENTRY; i++) {
    pCfg->spi.rx_cal.ent[i] = 0xff ;
  }

  switch (select) {
    case ARN5_CAL_ROUND_ROBIN :
    default :
    {
      /* tx cal */
      pCfg->spi.tx_cal.m = 1 ;
      pCfg->spi.tx_cal.len = 4; /*ARN5_MAX_NUM_CHANNELS;*/

      /*for (i = 0 ; i < ARN5_MAX_NUM_CHANNELS ; i++) {*/
	for (i = 0; i < 4; i++){
        pCfg->spi.tx_cal.ent[i] = i ;
      }

      /* rx_cal == tx_cal */
      pCfg->spi.rx_cal.m = pCfg->spi.tx_cal.m ;
      pCfg->spi.rx_cal.len = pCfg->spi.tx_cal.len ;

      for (i = 0; i < ARN5_SPI_MAX_CAL_ENTRY; i++) {
        pCfg->spi.rx_cal.ent[i] = pCfg->spi.tx_cal.ent[i] ;
      }

      /* rx_scheduler */
      pCfg->spi.rx_scheduler_cal_len = pCfg->spi.rx_cal.len ;

      /* tx_scheduler */
      pCfg->spi.tx_scheduler_0_1_2_3      = 0x0123 ;
      pCfg->spi.tx_scheduler_4_5_6_7      = 0x0123 ; /*0x4567 ;*/
      pCfg->spi.tx_scheduler_8_9_10_11    = 0x0123 ; /*0xffff ;*/
      pCfg->spi.tx_scheduler_12_13_14_15  = 0x0123 ; /*0xffff ;*/

      break ;
    }
  }

  return (retVal) ;
}


/*-----------------------------------------------------------------*/
/* Drop chan reconcile helper.  This routine is mainly called after*/
/* post-dev_init chan reassignment.                                */
/*-----------------------------------------------------------------*/
cs_status arn5_spi_dev_drop_chan_ctl_reconcile(cs_uint8 dev_id)
{
  cs_uint8      i ;

  for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
    cs_uint16           portId ;
    arn5_port_cb_t      * pPcb ;

    portId = ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, i) ;
    pPcb = ARN5_PORT_ID_TO_PCB_PTR(portId) ;

    if (pPcb != NULL) {
      if (pPcb->state == ARN5_PORT_STATE_HW_INIT_DONE) {
        arn5_spi_drop_chan_data_ctl(dev_id, i, FALSE) ;
      }
      else {
        arn5_spi_drop_chan_data_ctl(dev_id, i, TRUE) ;
      }
    }
    else {
      /* port doesn't exist yet. */
      arn5_spi_drop_chan_data_ctl(dev_id, i, TRUE) ;
    }
  }

  return (CS_OK) ;
}


/*-----------------------------------------------------------------*/
/* Port init helper.  This routine is mainly to update chan        */
/* assignment state machine; called by arn5_spi_init_cfg()          */
/*-----------------------------------------------------------------*/
cs_status arn5_spi_init_data(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
{
  return (CS_OK) ;
}
