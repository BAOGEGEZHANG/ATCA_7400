/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 * API's for SPI4.2/3;
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



/* Forward declarations */
static cs_status arn5_spi42_dev_ctl_loopback(cs_uint8 dev_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;

static cs_status arn5_spi3_dev_ctl_loopback(cs_uint8 dev_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;



/****************************************************************/
/* $rtn_hdr_start  SPI SRESET                                   */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_sreset(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Soft reset the entire SPI block. All the configuration       */
/* registers are unaffected.                                    */
/*                                                              */
/* The reset bit is SELF CLEARING. This API will poll for the   */
/* the reset bit to clear before returning.                     */
/* A timeout of 20 ms is provided to prevent any indefinite     */
/* blocking by this API.                                        */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_SPI4P2_CORE_control_t ctl ;
  cs_uint32             usec_delay = 100, usec_timeout = 20000 ;
  cs_uint32             usec_elapsed = 0;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  ctl.wrd = CS_REG_READ(&pSpi->control.wrd) ;
  ctl.bf.reset = ( CS_IN_TEST_ENV() ) ? 0 : 1 ;

  CS_REG_WRITE(&pSpi->control.wrd, ctl.wrd) ;

  ctl.wrd = CS_REG_READ(&pSpi->control.wrd) ;

  while (ctl.bf.reset) { /* poll for reset bit to clear */
    if (usec_elapsed >= usec_timeout) break ;
    CS_PRINT("Device-%d: SPI - waiting for SRESET to clear...\n",
             dev_id) ;
    ARN5_UDELAY(dev_id, usec_delay) ;
    usec_elapsed += usec_delay ;
    ctl.wrd = CS_REG_READ(&pSpi->control.wrd) ;
  }

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  if (ctl.bf.reset) {
    CS_HNDL_ERROR(dev_id, EARN5_RESET_TIMEOUT, ": SPI sreset\n") ;
    return (CS_ERROR) ;
  }
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET SPI INTERFACE MODE                       */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
arn5_spi_if_mode_t  arn5_spi_dev_get_if_mode(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : SPI interface mode                              */
/* DESCRIPTION:                                                 */
/* Get configured interface mode                                */
/* The return mode is either of the following.                  */
/* ARN5_SPI42_MODE              (0)                             */
/* ARN5_SPI3_MODE               (1)                             */
/* ARN5_SPI42_QUARTER_RATE_MODE (2)                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t             * pSpi ;
  ARN5_SPI4P2_MIF_mode_t    mifMode ;
  ARN5_SPI4P2_MIF_status_t  status ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (arn5_spi_dev_is_in_spi3_mode(dev_id)) {
    return (ARN5_SPI3_MODE) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;
  mifMode.wrd = CS_REG_READ(&pSpi->mode.wrd);
  if (mifMode.bf.quarterrate_override == 1) {
    return (mifMode.bf.quarterrate_value ?
               ARN5_SPI42_QUARTER_RATE_MODE :
               ARN5_SPI42_MODE) ;
  }

  status.wrd = CS_REG_READ(&pSpi->mif_status.wrd) ;
  return (status.bf.quarterrate ?
             ARN5_SPI42_QUARTER_RATE_MODE :
             ARN5_SPI42_MODE) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET SPI PLL CONTROL                          */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_pll_controls(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_uint8 freq_tune,
                        arn5_spi_pll_mult_t multiplier_sel)
/* INPUTS     : o Device Id                                     */
/*              o CS_TX, CS_RX, or CS_TX_AND_RX.                */
/*              o VCO Tune parameter                            */
/*              o Multiplier Selection                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the control parameters for PLL's                         */
/*                                                              */
/* [freq_tune] :                                                */
/*   SPI4.2  -  0x04 only                                       */
/*   SPI3    -  0x04 (for 77-104 MHz) or 0x0e (for 104-133MHz)  */
/*                                                              */
/* [multiplier_sel] :  for SPI4.2 only                          */
/*   ARN5_SPI_PLL_MULT_2 (default) or ARN5_SPI_PLL_MULT_4       */
/*   ARN5_SPI_PLL_MULT_8           or ARN5_SPI_PLL_MULT_DONTCARE*/
/*                                                              */
/*   In case of SPI3 mode, multiplier_sel will be ignored.      */
/*   Actually, a predetermined value for SPI3 mode will be set  */
/*   to the register automatically.                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (arn5_spi_dev_is_in_spi3_mode(dev_id)) {
    /* overwrite the multipler_sel to SPI3 specific */
    multiplier_sel = 3 ;
  }
  else {
    if (multiplier_sel > ARN5_SPI_PLL_MULT_8) {
      CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        ":multiplier_sel(%d) is not valid in SPI4.2 mode\n",
        multiplier_sel) ;
      return (CS_ERROR) ;
    }
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  if ((dir == CS_TX_AND_RX) || (dir == CS_TX)) {
    ARN5_SPI4P2_MIF_txpllcontrol1_t  txPllCtl;

    txPllCtl.wrd = CS_REG_READ(&pSpi->txpllcontrol1.wrd) ;
    txPllCtl.bf.freqtune = freq_tune ;
    txPllCtl.bf.multsel  = multiplier_sel ;
    CS_REG_WRITE(&pSpi->txpllcontrol1.wrd, txPllCtl.wrd) ;
  }

  if ((dir == CS_TX_AND_RX) || (dir == CS_RX)) {
    ARN5_SPI4P2_MIF_rxpllcontrol1_t  rxPllCtl;

    rxPllCtl.wrd = CS_REG_READ(&pSpi->rxpllcontrol1.wrd) ;
    rxPllCtl.bf.freqtune = freq_tune ;
    rxPllCtl.bf.multsel  = multiplier_sel ;
    CS_REG_WRITE(&pSpi->rxpllcontrol1.wrd, rxPllCtl.wrd) ;
  }

  ARN5_SPI_DEV_UNLOCK(dev_id) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start GET PHYSICAL CHAN NUM FROM LOGICAL CHAN NUM   */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_uint8  arn5_spi_dev_get_logical2physical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_uint8 logical_chan_num)
/* INPUTS     : o Device Id                                     */
/*              o CS_RX or CS_TX                                */
/*              o Logical Channel Number                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Physical Channel Number or 0xff if any error    */
/* DESCRIPTION:                                                 */
/* Get the corresponding physical channel number from a given   */
/* logical channel number in the specified direction.           */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, 0xff) ;

  if ((dir != CS_RX) && (dir != CS_TX)) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : dir s/b CS_RX or CS_TX\n") ;
    return (0xff) ;
  }

  if (logical_chan_num >= ARN5_MAX_NUM_CHANNELS) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      " : logical_chan_num(%d) s/b 0...7\n", logical_chan_num) ;
    return (0xff) ;
  }

  return arn5_logical2physical_chan_num(dev_id, dir,
           logical_chan_num) ;
}


/****************************************************************/
/* $rtn_hdr_start GET LOGICAL CHAN NUM FROM PHYSICAL CHAN NUM   */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_uint8  arn5_spi_dev_get_physical2logical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_uint8 physical_chan_num)
/* INPUTS     : o Device Id                                     */
/*              o CS_RX or CS_TX                                */
/*              o Physical Channel Number                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Logical Channel Number or 0xff if any error     */
/* DESCRIPTION:                                                 */
/* Get the corresponding logical channel number from a given    */
/* physical channel number in the specified direction.          */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, 0xff) ;

  if ((dir != CS_RX) && (dir != CS_TX)) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : dir s/b CS_RX or CS_TX\n") ;
    return (0xff) ;
  }

  if (physical_chan_num >= ARN5_MAX_NUM_CHANNELS) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      " : physical_chan_num(%d) s/b 0...7\n", physical_chan_num) ;
    return (0xff) ;
  }

  return arn5_physical2logical_chan_num(dev_id, dir,
           physical_chan_num) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE SPI PHYICAL-LOGICAL CHANNEL MAP      */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_chanmap(cs_uint8 dev_id, cs_dir_t dir,
                        cs_uint8 lch_num_pch0, cs_uint8 lch_num_pch1,
                        cs_uint8 lch_num_pch2, cs_uint8 lch_num_pch3,
                        cs_uint8 lch_num_pch4, cs_uint8 lch_num_pch5,
                        cs_uint8 lch_num_pch6, cs_uint8 lch_num_pch7)
/* INPUTS     : o Device Id                                     */
/*              o CS_TX, CS_RX, or CS_TX_AND_RX.                */
/*              o Logical channel number for physical ch 0      */
/*              o Logical channel number for physical ch 1      */
/*              o Logical channel number for physical ch 2      */
/*              o Logical channel number for physical ch 3      */
/*              o Logical channel number for physical ch 4      */
/*              o Logical channel number for physical ch 5      */
/*              o Logical channel number for physical ch 6      */
/*              o Logical channel number for physical ch 7      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the SPI physical to logical channel mapping.             */
/*                                                              */
/* [lch_num_pch0(...7)] must be uniquely assigned to (0...7)    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t * pSpi ;
  arn5_dev_cb_t * pdevcb ;
  cs_uint8      i ;
  cs_uint8      logMap[8] = { 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff } ;
  cs_status     status = CS_ERROR ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;
  if (lch_num_pch0 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch0 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch0] == 0xff) {
    logMap[lch_num_pch0] = 0 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch0 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (lch_num_pch1 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch1 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch1] == 0xff) {
    logMap[lch_num_pch1] = 1 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch1 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (lch_num_pch2 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch2 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch2] == 0xff) {
    logMap[lch_num_pch2] = 2 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch2 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

    if (lch_num_pch3 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch3 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch3] == 0xff) {
    logMap[lch_num_pch3] = 3 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch3 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

    if (lch_num_pch4 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch4 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch4] == 0xff) {
    logMap[lch_num_pch4] = 4 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch4 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (lch_num_pch5 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch5 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch5] == 0xff) {
    logMap[lch_num_pch5] = 5 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch5 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (lch_num_pch6 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch6 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch6] == 0xff) {
    logMap[lch_num_pch6] = 6 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch6 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (lch_num_pch7 >= 8) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch7 s/b 0...7\n") ;
    goto SET_CHMAP_EXIT ;
  }

  if (logMap[lch_num_pch7] == 0xff) {
    logMap[lch_num_pch7] = 7 ;
  }
  else {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_SPI_CHAN_MAP_FAILED ,
      ": lch_num_pch7 is not unique\n") ;
    goto SET_CHMAP_EXIT ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;
  pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  if ((dir == CS_TX_AND_RX) || (dir == CS_TX)) {
    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
      pdevcb->tx_log_chan_map[i] = logMap[i] ;
    }
    pdevcb->tx_phy_chan_map[0] = lch_num_pch0 ;
    pdevcb->tx_phy_chan_map[1] = lch_num_pch1 ;
    pdevcb->tx_phy_chan_map[2] = lch_num_pch2 ;
    pdevcb->tx_phy_chan_map[3] = lch_num_pch3 ;
    pdevcb->tx_phy_chan_map[4] = lch_num_pch4 ;
    pdevcb->tx_phy_chan_map[5] = lch_num_pch5 ;
    pdevcb->tx_phy_chan_map[6] = lch_num_pch6 ;
    pdevcb->tx_phy_chan_map[7] = lch_num_pch7 ;

    CS_REG_WRITE(&pSpi->phy_ch_num_lch0.wrd, logMap[0]) ;
    CS_REG_WRITE(&pSpi->phy_ch_num_lch1.wrd, logMap[1]) ;
    CS_REG_WRITE(&pSpi->phy_ch_num_lch2.wrd, logMap[2]) ;
    CS_REG_WRITE(&pSpi->phy_ch_num_lch3.wrd, logMap[3]) ;
    CS_REG_WRITE(&pSpi->phy_ch_num_lch4.wrd, logMap[4]) ;
    CS_REG_WRITE(&pSpi->phy_ch_num_lch5.wrd, logMap[5]) ;
    CS_REG_WRITE(&pSpi->phy_ch_num_lch6.wrd, logMap[6]) ;
    CS_REG_WRITE(&pSpi->phy_ch_num_lch7.wrd, logMap[7]) ;

  }

  if ((dir == CS_TX_AND_RX) || (dir == CS_RX)) {
    for (i = 0; i < ARN5_MAX_NUM_CHANNELS; i++) {
      pdevcb->rx_log_chan_map[i] = logMap[i] ;
    }
    pdevcb->rx_phy_chan_map[0] = lch_num_pch0 ;
    pdevcb->rx_phy_chan_map[1] = lch_num_pch1 ;
    pdevcb->rx_phy_chan_map[2] = lch_num_pch2 ;
    pdevcb->rx_phy_chan_map[3] = lch_num_pch3 ;
    pdevcb->rx_phy_chan_map[4] = lch_num_pch4 ;
    pdevcb->rx_phy_chan_map[5] = lch_num_pch5 ;
    pdevcb->rx_phy_chan_map[6] = lch_num_pch6 ;
    pdevcb->rx_phy_chan_map[7] = lch_num_pch7 ;

    CS_REG_WRITE(&pSpi->lch_num_pch0.wrd, lch_num_pch0) ;
    CS_REG_WRITE(&pSpi->lch_num_pch1.wrd, lch_num_pch1) ;
    CS_REG_WRITE(&pSpi->lch_num_pch2.wrd, lch_num_pch2) ;
    CS_REG_WRITE(&pSpi->lch_num_pch3.wrd, lch_num_pch3) ;
    CS_REG_WRITE(&pSpi->lch_num_pch4.wrd, lch_num_pch4) ;
    CS_REG_WRITE(&pSpi->lch_num_pch5.wrd, lch_num_pch5) ;
    CS_REG_WRITE(&pSpi->lch_num_pch6.wrd, lch_num_pch6) ;
    CS_REG_WRITE(&pSpi->lch_num_pch7.wrd, lch_num_pch7) ;
  }

  ARN5_SPI_DEV_UNLOCK(dev_id) ;
  status = arn5_spi_dev_drop_chan_ctl_reconcile(dev_id) ;

SET_CHMAP_EXIT :
  return (status);
}


/****************************************************************/
/* $rtn_hdr_start  SET BURST PARAMETERS                         */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_burst_params(cs_uint8 dev_id,
                        cs_uint8 burst,
                        cs_uint16 maxburst1,
                        cs_uint16 maxburst2)
/* INPUTS     : o Device Id                                     */
/*              o burst     parameter(0...15)                   */
/*              o maxburst1 parameter(0...1023)                 */
/*              o maxburst2 parameter(0...1023)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets burst, maxburst1 and maxburst2 parameters.              */
/*                                                              */
/* [burst] is the number of 16-byte data ((N+1) * 16 bytes)     */
/* bursted between payload control words.                       */
/* e.g.: When burst = 3, burst size byte count is 64 bytes.     */
/*                                                              */
/* [maxburst1] is the data burst credit in the increment of 16  */
/* bytes (N * 16) granted to the RX channel when the incoming   */
/* RX status indicates Starving.                                */
/* e.g.: When maxburst1 = 0xc, the data burst credit for a      */
/* starving RX channel is 12 * 16 bytes.                        */
/*                                                              */
/* [maxburst2] is the data burst credit in the increment of 16  */
/* bytes (N * 16) that may be granted to the RX channel when    */
/* the incoming RX status indicates Hungry                      */
/* e.g.: When maxburst2 = 0x8, the data burst credit that may   */
/* be granted to a hungry RX channel is 8 * 16 bytes.           */
/*                                                              */
/* Note that burst < maxburst2 <= maxburst1.                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (burst > 15) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        ":burst(%d) s/b 0...15\n", burst) ;
    return (CS_ERROR) ;
  }

  if ( (maxburst1 > 1023) || (maxburst2 > 1023) ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        ":max_burst1(%d), max_burst2(%d) s/b 0...1023\n",
        maxburst1, maxburst2) ;
    return (CS_ERROR) ;
  }

  if (maxburst2 > maxburst1) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : maxburst1(%d) s/b >= maxburst2(%d)\n",
                  maxburst1, maxburst2) ;
    return (CS_ERROR) ;
  }

  if (maxburst2 <= burst ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : maxburst2(%d) s/b > burst(%d)\n",
                  maxburst2, burst) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  CS_REG_WRITE(&pSpi->burst.wrd, burst) ;
  CS_REG_WRITE(&pSpi->maxburst1.wrd, maxburst1) ;
  CS_REG_WRITE(&pSpi->maxburst2.wrd, maxburst2) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SPI TX/RX SYNC SRESET                        */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi42_dev_sync_sreset(cs_uint8 dev_id,
                        cs_dir_t dir,
                        cs_reset_action_t act)
/* INPUTS     : o Device Id                                     */
/*              o CS_TX, CS_RX, or CS_TX_AND_RX.                */
/*              o Reset Action                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reset control of the specified sync state machine.           */
/* This is a soft reset and the configuration registers are     */
/* unaffected.                                                  */
/*                                                              */
/* The [act] parameter is specified to be:                      */
/* CS_RESET_DEASSERT, CS_RESET_ASSERT, or                       */
/* CS_RESET_TOGGLE.                                             */
/* The ARN5_RESET_TOGGLE option will assert reset, hold the     */
/* reset for a while and then de-assert the reset bit(s).       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_SPI4P2_CORE_control_t ctl, bits ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  bits.wrd = 0 ;
  if ((dir == CS_TX_AND_RX) || (dir == CS_TX)) {
    bits.bf.txsynreset = 1 ;
  }

  if ((dir == CS_TX_AND_RX) || (dir == CS_RX)) {
    bits.bf.rxsynreset = 1 ;
  }

  ARN5_SPI_DEV_LOCK(dev_id) ;
  ctl.wrd = CS_REG_READ(&pSpi->control.wrd) ;
  switch (act) {
    case CS_RESET_DEASSERT :
      ctl.wrd &= ~bits.wrd ;
      CS_REG_WRITE(&pSpi->control.wrd, ctl.wrd) ;
      break ;

    case CS_RESET_ASSERT :
      ctl.wrd |= bits.wrd ;
      CS_REG_WRITE(&pSpi->control.wrd, ctl.wrd) ;
      break ;

    case CS_RESET_TOGGLE :
      /* set, wait for a few msecs, and clear hard reset bit(s) */
      ctl.wrd |= bits.wrd ;
      CS_REG_WRITE(&pSpi->control.wrd, ctl.wrd) ;
      ARN5_MDELAY( dev_id, ARN5_RESET_MSEC_DELAY ) ;
      ctl.wrd &= ~bits.wrd ;
      CS_REG_WRITE(&pSpi->control.wrd, ctl.wrd) ;
      break ;
  }

  ARN5_MDELAY( dev_id, ARN5_RESET_MSEC_DELAY ) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET SPI4.2 DATA RATE                         */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi42_dev_set_rate(cs_uint8 dev_id,
                        cs_boolean quarter_rate)
/* INPUTS     : o Device Id                                     */
/*              o TRUE(if quarter rate mode) or FALSE           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Specifies whether the Quarter Rate (OC48) mode clocking is   */
/* to be enabled or not.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_MIF_mode_t   mode;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  mode.wrd = CS_REG_READ(&pSpi->mode.wrd);

  mode.bf.quarterrate_override = 1;
  mode.bf.quarterrate_value = (quarter_rate) ? 1 : 0;

 /* added by zhuchao. spi4.2 rsclk invert	*/
  mode.bf.rsclk_invert = 1;
/* mode.bf.tsclk_invert = 1; */

  CS_REG_WRITE(&pSpi->mode.wrd, mode.wrd) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET SPI4.2 TRAINING PARAMETERS               */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi42_dev_set_training_params(cs_uint8 dev_id,
                        cs_uint8 alpha,
                        cs_uint16 datamaxt)
/* INPUTS     : o Device Id                                     */
/*              o alpha (0...15)                                */
/*              o datamaxt(0...65535)                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the data training parameters for SPI4.2 interface.      */
/*                                                              */
/* [alpha] -                                                    */
/* This is alpha parameter and it specifies the number of       */
/* repetitions that must be scheduled every datamaxt cycles.    */
/*                                                              */
/* datamaxt -                                                   */
/* This is datamaxt parameter and it specifies the max          */
/* interval between training sequences.                         */
/* The training interval specified is counted at 1/2 the SPI Rx */
/* frequency. It can be DISABLED by specifying a value of 0.    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (alpha > 15) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : alpha(%d) s/b 0...15\n", alpha) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  CS_REG_WRITE(&pSpi->alpha.wrd, alpha) ;
  CS_REG_WRITE(&pSpi->datamaxt.wrd, datamaxt) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    SET SPI4.2 DIP-2 THRESHOLDS                */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi42_dev_set_dip2_tholds(cs_uint8 dev_id,
                        cs_uint8 error_thold,
                        cs_uint8 match_thold)
/* INPUTS     : o Device Id                                     */
/*              o Error Threshold(0...15)                       */
/*              o Match Threshold(0...15)                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* DIP-2 is sent with RSTAT and is checked by chip.             */
/* These thresholds decide the transition of Rx state machine to*/
/* out-of-sync or in-sync states respectively.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (error_thold > 15) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : error_thold(%d) s/b 0...15\n", error_thold) ;
    return (CS_ERROR) ;
  }

  if (match_thold > 15) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : match_thold(%d) s/b 0...15\n", match_thold) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  CS_REG_WRITE(&pSpi->numdip2err.wrd, error_thold) ;
  CS_REG_WRITE(&pSpi->numdip2match.wrd, match_thold) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    SET SPI4.2 DIP-4 THRESHOLDS                */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi42_dev_set_dip4_tholds(cs_uint8 dev_id,
                        cs_uint8 error_thold,
                        cs_uint16 match_thold)
/* INPUTS     : o Device Id                                     */
/*              o Error Threshold(0...15)                       */
/*              o Match Threshold(0...65535)                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* DIP-4 is sent with TDAT and is checked by chip.              */
/* These thresholds decide the transition of Tx state machine to*/
/* out-of-sync or in-sync states respectively.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (error_thold > 15) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : error_thold(%d) s/b 0...15\n", error_thold) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  CS_REG_WRITE(&pSpi->numdip4err.wrd, error_thold) ;
  CS_REG_WRITE(&pSpi->numdip4match.wrd, match_thold) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   GET SPI4.2 RX or TX SYNC STATE              */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
arn5_spi42_sync_t  arn5_spi42_dev_get_sync_state(cs_uint8 dev_id,
                        cs_dir_t dir)
/* INPUTS     : o Device Id                                     */
/*              o CS_TX or CS_RX                                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Tx or Rx Sync Status                            */
/* DESCRIPTION:                                                 */
/* Indicates the current sync state of Tx or Rx state machine   */
/* The return value is:                                         */
/* ARN5_SPI42_INIT_STATE      : No framing acquired yet         */
/* ARN5_SPI42_OUT_OF_SYNC     : Waiting for DIP2 codeword match */
/* ARN5_SPI42_IN_SYNC         : In-sync and ready               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_CORE_status_t status ;
  cs_uint8              bits ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if ( (dir != CS_TX) && (dir != CS_RX) ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : dir(%d) s/b CS_TX(%d) or CS_RX(%d)",
                  dir, CS_TX, CS_RX) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  status.wrd = CS_REG_READ(&pSpi->core_status.wrd) ;

  bits = (dir == CS_TX) ?
                status.bf.spi4p2txstate :
                status.bf.spi4p2rxstate  ;

  switch (bits) {
    default :
    case ARN5_spi4p2rxstate_INIT        :
      return (ARN5_SPI42_INIT_STATE) ;
    case ARN5_spi4p2rxstate_OUT_OF_SYNC :
      return (ARN5_SPI42_OUT_OF_SYNC) ;
    case ARN5_spi4p2rxstate_SYNC        :
      return (ARN5_SPI42_IN_SYNC) ;
  }
}


/****************************************************************/
/* $rtn_hdr_start   SET SPI4.2 MISC CONTROLS                    */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi42_dev_set_controls(cs_uint8 dev_id,
                        arn5_spi42_ctrl_type_t ctl_sel,
                        cs_boolean set)
/* INPUTS     : o Device Id                                     */
/*              o Control Type                                  */
/*              o TRUE (set) or FALSE (clear)                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set various control for spi42 interface                      */
/*                                                              */
/* [ctl_sel] :                                                  */
/*   ARN5_SPI42_TX_TRN_SEQ_STRICT_MODE ,                        */
/*   ARN5_SPI42_DISABLE_SOP_DIST_ERR_CLOSE_ALL_PKT ,            */
/*   ARN5_SPI42_DISABLE_BURST_LEN_ERR_CLOSE_ALL_PKT ,           */
/*   ARN5_SPI42_DISABLE_RSVD_WORD_CLOSE_ALL_PKT ,               */
/*   ARN5_SPI42_IGNORE_SOP_DISTANCE_DELETION ,                  */
/*   ARN5_SPI42_DISABLE_SEQ_ERR_CLOSE_ALL_PKT ,                 */
/*   ARN5_SPI42_DISABLE_DIP4_CLOSE_ALL_PKT ,                    */
/*                                                              */
/*   ARN5_SPI42_ENABLE_CLOSING_ACTIVE_CHAN_UPON_ENTERING_IN_SYNC*/
/*   ARN5_SPI42_ENABLE_PROTOCOL_FIXING ,                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_TXIF_spi42ctl_t  spi42Ctl ;
  ARN5_SPI4P2_RXCORE_chclose_t chClose ;
  cs_boolean    isTxReg = TRUE ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  spi42Ctl.wrd = CS_REG_READ(&pSpi->spi42ctl.wrd) ;
  chClose.wrd = CS_REG_READ(&pSpi->chclose.wrd) ;

  switch (ctl_sel) {
    case ARN5_SPI42_TX_TRN_SEQ_STRICT_MODE :
      spi42Ctl.bf.tx_trn_seq_strict_mode = set ? 1 : 0 ;
      break ;

    case ARN5_SPI42_DISABLE_SOP_DIST_ERR_CLOSE_ALL_PKT :
      spi42Ctl.bf.disable_sop_dist_err_close_all_pkt = set ? 1 : 0 ;
      break ;

    case ARN5_SPI42_DISABLE_BURST_LEN_ERR_CLOSE_ALL_PKT :
      spi42Ctl.bf.disable_burst_len_err_close_all_pkt = set ? 1 : 0 ;
      break ;

    case ARN5_SPI42_DISABLE_RSVD_WORD_CLOSE_ALL_PKT :
      spi42Ctl.bf.disable_rsvd_word_close_all_pkt = set ? 1 : 0 ;
      break ;

    case ARN5_SPI42_IGNORE_SOP_DISTANCE_DELETION :
      spi42Ctl.bf.ignore_sop_distance_deletion = set ? 1 : 0 ;
      break ;

    case ARN5_SPI42_DISABLE_SEQ_ERR_CLOSE_ALL_PKT :
      spi42Ctl.bf.disable_seq_err_close_all_pkt = set ? 1 : 0 ;
      break ;

    case ARN5_SPI42_DISABLE_DIP4_CLOSE_ALL_PKT :
      spi42Ctl.bf.disable_dip4_close_all_pkt = set ? 1 : 0 ;
      break ;

    case ARN5_SPI42_ENABLE_CLOSING_ACTIVE_CHAN_UPON_ENTERING_IN_SYNC :
      chClose.bf.enable = set ? 1 : 0 ;
      isTxReg = FALSE ;
      break ;

    case ARN5_SPI42_ENABLE_PROTOCOL_FIXING :
      chClose.bf.pcol = set ? 1 : 0 ;
      isTxReg = FALSE ;
      break ;

    default :
      CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " Invalid spi42 control type selection.\n") ;
      ARN5_SPI_DEV_UNLOCK(dev_id) ;
      return (CS_ERROR) ;
      break ;
  }

  if (isTxReg) {
    CS_REG_WRITE(&pSpi->spi42ctl.wrd, spi42Ctl.wrd) ;
  }
  else {
    CS_REG_WRITE(&pSpi->chclose.wrd, chClose.wrd) ;
  }

  ARN5_SPI_DEV_UNLOCK(dev_id) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET SPI4.2 ASYNC FIFO HYSTERESIS            */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi42_dev_set_afifo_hysteresis(cs_uint8 dev_id,
                        cs_uint8 hysteresis)
/* INPUTS     : o Device Id                                     */
/*              o Hysteresis Value (0...31)                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set additional hysteresis used to enable reading from the    */
/* spi42 async fifo for spi42 interface.                        */
/*                                                              */
/* Note : Set the hysteresis to non-zero value only when the rx */
/* clock is significantly faster than the core clock frequency  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (hysteresis >= 32) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : hysteresis(%d) s/b 0...31\n", hysteresis) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;
  CS_REG_WRITE(&pSpi->spi42_hysteresis.wrd, hysteresis) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET CALENDAR PARAMETERS                     */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_cal_params(cs_uint8 dev_id,
                        cs_dir_t dir,
                        arn5_spi_cal_params_t * pParams)
/* INPUTS     : o Device Id                                     */
/*              o CS_TX, CS_RX or CS_TX_AND_RX                  */
/*              o Pointer to Calendar Parameters' Data Structure*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the various calendar parameters for the specified path. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  cs_uint8                i ;
  cs_reg                * pReg ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  if ((dir == CS_TX_AND_RX) || (dir == CS_TX)) {
    CS_REG_WRITE(&pSpi->txcal_cal_m.wrd, pParams->m-1) ;
    CS_REG_WRITE(&pSpi->txcal_cal_len.wrd, pParams->len-1) ;

    pReg = (cs_reg *)&pSpi->txcal_ent_0.wrd ;
    for (i = 0; i < ARN5_SPI_MAX_CAL_ENTRY; i++, pReg++) {
      if (pParams->ent[i] == 0xff) break ;
      CS_REG_WRITE(pReg, pParams->ent[i]) ;
    }
  }

  if ((dir == CS_TX_AND_RX) || (dir == CS_RX)) {
    CS_REG_WRITE(&pSpi->rxcal_cal_m.wrd, pParams->m-1) ;
    CS_REG_WRITE(&pSpi->rxcal_cal_len.wrd, pParams->len-1) ;

    pReg = (cs_reg *)&pSpi->rxcal_ent_0.wrd ;
    for (i = 0; i < ARN5_SPI_MAX_CAL_ENTRY; i++, pReg++) {
      if (pParams->ent[i] == 0xff) break ;
      CS_REG_WRITE(pReg, pParams->ent[i]) ;
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET CALENDER LENGTH SEEN BY RX SCHEDULER    */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_rx_scheduler_cal_len(cs_uint8 dev_id,
                        cs_uint8 len)
/* INPUTS     : o Device Id                                     */
/*              o Calender length seen by rx scheduler (1...96) */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the length of the calendar sequence as seen by the       */
/* scheduler.                                                   */
/*                                                              */
/* The [len] is normally set the same as calender length.       */
/* If it is greater than calender length, the extra calender    */
/* entries are used by the rx schedule to give more bandwidth to*/
/* the channels associated with the extra entries.              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_RXCAL_cal_len_t  rxCalLen;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if ((len == 0) || ( len > 96)) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : len(%d) s/b 1...96\n", len) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  rxCalLen.wrd = CS_REG_READ(&pSpi->rxcal_cal_len.wrd) ;
  if ((rxCalLen.bf.value + 1) > len) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : scheduler cal len(%d) s/b >= cal len\n") ;
    return (CS_ERROR) ;
  }

  CS_REG_WRITE(&pSpi->sched_cal_len.wrd, len-1) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET TX SCHEDULER ENTRIES                    */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_tx_scheduler_entries(cs_uint8 dev_id,
                        cs_uint16 ent0_1_2_3,  cs_uint16 ent4_5_6_7,
                        cs_uint16 ent8_9_10_11,  cs_uint16 ent12_13_14_15)
/* INPUTS     : o Device Id                                     */
/*              o Entry 0_1_2_3                                 */
/*              o Entry 4_5_6_7                                 */
/*              o Entry 8_9_10_11                               */
/*              o Entry 12_13_14_15                             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the 16 entries in the TX scheduler.                      */
/*                                                              */
/* [ent0_1_2_3], [ent4_5_6_7], [ent8_9_10_11], [ent12_13_14_15] */
/* Each has 4 nibbles which set corresponding 4 entries (out of */
/* 16).  If the value of a nibble is greater than 7 (recommend  */
/* to use 0xf), the entry will be skipped by scheduler.         */
/* e.g. Assume ent0_1_2_3 = 0x235f, then entry 0, 1, 2, 3 will  */
/*      be set to 2, 3, 5, 0xf respectively.                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                 * pDev ;
  ARN5_SPI4P2A_TXCFG_tx_sched_0_t  schEnt ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  schEnt.wrd = ent0_1_2_3 ;
  if (schEnt.bf.ent_0 > 7) schEnt.bf.ent_0 = 0xf ;
  if (schEnt.bf.ent_1 > 7) schEnt.bf.ent_1 = 0xf ;
  if (schEnt.bf.ent_2 > 7) schEnt.bf.ent_2 = 0xf ;
  if (schEnt.bf.ent_3 > 7) schEnt.bf.ent_3 = 0xf ;
  CS_REG_WRITE(&pDev->SPI4P2a.tx_sched_0.wrd, schEnt.wrd) ;

  schEnt.wrd = ent4_5_6_7 ;
  if (schEnt.bf.ent_0 > 7) schEnt.bf.ent_0 = 0xf ;
  if (schEnt.bf.ent_1 > 7) schEnt.bf.ent_1 = 0xf ;
  if (schEnt.bf.ent_2 > 7) schEnt.bf.ent_2 = 0xf ;
  if (schEnt.bf.ent_3 > 7) schEnt.bf.ent_3 = 0xf ;
  CS_REG_WRITE(&pDev->SPI4P2a.tx_sched_1.wrd, schEnt.wrd) ;

  schEnt.wrd = ent8_9_10_11 ;
  if (schEnt.bf.ent_0 > 7) schEnt.bf.ent_0 = 0xf ;
  if (schEnt.bf.ent_1 > 7) schEnt.bf.ent_1 = 0xf ;
  if (schEnt.bf.ent_2 > 7) schEnt.bf.ent_2 = 0xf ;
  if (schEnt.bf.ent_3 > 7) schEnt.bf.ent_3 = 0xf ;
  CS_REG_WRITE(&pDev->SPI4P2a.tx_sched_2.wrd, schEnt.wrd) ;

  schEnt.wrd = ent12_13_14_15 ;
  if (schEnt.bf.ent_0 > 7) schEnt.bf.ent_0 = 0xf ;
  if (schEnt.bf.ent_1 > 7) schEnt.bf.ent_1 = 0xf ;
  if (schEnt.bf.ent_2 > 7) schEnt.bf.ent_2 = 0xf ;
  if (schEnt.bf.ent_3 > 7) schEnt.bf.ent_3 = 0xf ;
  CS_REG_WRITE(&pDev->SPI4P2a.tx_sched_3.wrd, schEnt.wrd) ;

  /* toggle the change bit */
  CS_REG_WRITE(&pDev->SPI4P2a.tx_sched_changed.wrd, 1) ;
  CS_UDELAY(1) ;
  CS_REG_WRITE(&pDev->SPI4P2a.tx_sched_changed.wrd, 0) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET CALENDAR PARAMETERS                     */
/* CATEGORY   : Device                                          */
/* ACCESS     : Private                                         */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_apish_set_cal_params(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint16 m, cs_uint8 len)
/* INPUTS     : o Device Id                                     */
/*              o CS_TX, CS_RX or CS_TX_AND_RX                  */
/*              o Calendar Seq repetition number (1...60000)    */
/*              o Calendar Length (1...96)                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets calendar repetition and length for the specified path.  */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if ((m == 0) || ( m > 60000)) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : m(%d) s/b 1...60000\n", m) ;
    return (CS_ERROR) ;
  }

  if ((len == 0) || ( len > 96)) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  " : len(%d) s/b 1...96\n", len) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  if ((dir == CS_TX_AND_RX) || (dir == CS_TX)) {
    CS_REG_WRITE(&pSpi->txcal_cal_m.wrd, m-1) ;
    CS_REG_WRITE(&pSpi->txcal_cal_len.wrd, len-1) ;
  }

  if ((dir == CS_TX_AND_RX) || (dir == CS_RX)) {
    CS_REG_WRITE(&pSpi->rxcal_cal_m.wrd, m-1) ;
    CS_REG_WRITE(&pSpi->rxcal_cal_len.wrd, len-1) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET CALENDAR ENTRIESS                       */
/* CATEGORY   : Device                                          */
/* ACCESS     : Private                                         */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_apish_set_cal_entries(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint8 starting_entry,
                        cs_uint8  ent0, cs_uint8  ent1,
                        cs_uint8  ent2, cs_uint8  ent3,
                        cs_uint8  ent4, cs_uint8  ent5,
                        cs_uint8  ent6, cs_uint8  ent7)
/* INPUTS     : o Device Id                                     */
/*              o CS_TX, CS_RX or CS_TX_AND_RX                  */
/*              o The starting number of the 8 Entries (0...88) */
/*              o Entry 0                                       */
/*              o Entry 1                                       */
/*              o Entry 2                                       */
/*              o Entry 3                                       */
/*              o Entry 4                                       */
/*              o Entry 5                                       */
/*              o Entry 6                                       */
/*              o Entry 7                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets 8 of 96 calendar entries for the specified path.        */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t         * pSpi ;
  cs_reg                * pReg ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  if ((dir == CS_TX_AND_RX) || (dir == CS_TX)) {
    pReg = (cs_reg *)&pSpi->txcal_ent_0.wrd + starting_entry ;

    CS_REG_WRITE(pReg, ent0) ;
    CS_REG_WRITE(++pReg, ent1) ;
    CS_REG_WRITE(++pReg, ent2) ;
    CS_REG_WRITE(++pReg, ent3) ;
    CS_REG_WRITE(++pReg, ent4) ;
    CS_REG_WRITE(++pReg, ent5) ;
    CS_REG_WRITE(++pReg, ent6) ;
    CS_REG_WRITE(++pReg, ent7) ;
  }

  if ((dir == CS_TX_AND_RX) || (dir == CS_RX)) {
    pReg = (cs_reg *)&pSpi->rxcal_ent_0.wrd + starting_entry ;

    CS_REG_WRITE(pReg, ent0) ;
    CS_REG_WRITE(++pReg, ent1) ;
    CS_REG_WRITE(++pReg, ent2) ;
    CS_REG_WRITE(++pReg, ent3) ;
    CS_REG_WRITE(++pReg, ent4) ;
    CS_REG_WRITE(++pReg, ent5) ;
    CS_REG_WRITE(++pReg, ent6) ;
    CS_REG_WRITE(++pReg, ent7) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SPI LOOPBACK                                */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_ctl_loopback(cs_uint8 dev_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Device Id                                     */
/*              o Loopback selection                            */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control(disable/enable) the specified loopback in SPI        */
/* interface.                                                   */
/*                                                              */
/* The [dir] parameter specifies which loopback to control and  */
/* should be one of the following:                              */
/* CS_TX : When enabled, TX outputs are looped back to RX       */
/* CS_RX : When enabled, RX outputs are looped back to TX       */
/*                                                              */
/* Please note that the CS_TX loopback is not supported in      */
/* SPI4.2 mode.  Also in SPI3 mode, user needs to enable        */
/* stall_on_single_fifo_full                                    */
/* (see API arn5_spi3_dev_ctl_stall_on_single_fifo_full()) for  */
/* backpressure to work while in CS_RX loopback.                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (arn5_spi_dev_is_in_spi3_mode(dev_id)) {
    return arn5_spi3_dev_ctl_loopback(dev_id, dir, ctl);
  }
  else {
    return arn5_spi42_dev_ctl_loopback(dev_id, dir, ctl);
  }
}


/****************************************************************/
/* $rtn_hdr_start   SPI3 MODE SETUP                             */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi3_dev_set_modes(cs_uint8 dev_id,
                        cs_boolean eight_bit_mode)
/* INPUTS     : o Device Id                                     */
/*              o TRUE(enable) or FALSE(disable)                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Setup SPI3 eight bit modes.                                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_TXIF_spi3ctl_t tCtl ;
  ARN5_SPI4P2_RXIF_spi3ctl_t rCtl ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  tCtl.wrd = CS_REG_READ(&pSpi->txif_spi3ctl.wrd) ;
  tCtl.bf.Eightbmode = eight_bit_mode ? 1 : 0 ;
  CS_REG_WRITE(&pSpi->txif_spi3ctl.wrd, tCtl.wrd) ;

  rCtl.wrd = CS_REG_READ(&pSpi->rxif_spi3ctl.wrd) ;
  rCtl.bf.Eightbmode = eight_bit_mode ? 1 : 0 ;
  CS_REG_WRITE(&pSpi->rxif_spi3ctl.wrd, rCtl.wrd) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SPI3 CONTROL STALL_ON_SINGLE_FIFO CONTROL   */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi3_dev_ctl_stall_on_single_fifo_full(cs_uint8 dev_id,
                        cs_boolean enbl)
/* INPUTS     : o Device Id                                     */
/*              o TRUE(enable) or FALSE(disable)                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls stall_on_single_fifo_full.                          */
/*                                                              */
/* [stall_on_single_fifo_full] controls whether to assert back  */
/* pressure on SPI3 interface even if one fifo is full.         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_TXCORE_spi3_single_fifo_full_t fifoCtl ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  fifoCtl.wrd = CS_REG_READ(&pSpi->spi3_single_fifo_full.wrd) ;
  fifoCtl.bf.stall_mode = enbl ? 1 : 0 ;
  CS_REG_WRITE(&pSpi->spi3_single_fifo_full.wrd, fifoCtl.wrd) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET SPI3 CONTROLS                           */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi3_dev_set_controls(cs_uint8 dev_id,
                        cs_boolean ptpa_enable,
                        cs_boolean stpa_enable)
/* INPUTS     : o Device Id                                     */
/*              o TRUE(enable) or FALSE(disable)                */
/*              o TRUE(enable) or FALSE(disable)                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set various SPI3 controls.                                   */
/*                                                              */
/* ptpa_enable controls the polled status interface             */
/* stpa_enable controls the selected status interface.          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_TXIF_spi3ctl_t tCtl ;
  ARN5_SPI4P2_RXIF_spi3ctl_t rCtl ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  tCtl.wrd = CS_REG_READ(&pSpi->txif_spi3ctl.wrd) ;
  tCtl.bf.ptpa_enable = ptpa_enable ? 1 : 0 ;
  tCtl.bf.stpa_enable = stpa_enable ? 1 : 0 ;
  CS_REG_WRITE(&pSpi->txif_spi3ctl.wrd, tCtl.wrd) ;

  /* always tie the ptpa_enable and stpa_enable of RXIF to 0 */
  rCtl.wrd = CS_REG_READ(&pSpi->rxif_spi3ctl.wrd) ;
  if (rCtl.bf.ptpa_enable || rCtl.bf.stpa_enable) {
    rCtl.bf.ptpa_enable =  0 ;
    rCtl.bf.stpa_enable =  0 ;
    CS_REG_WRITE(&pSpi->rxif_spi3ctl.wrd, rCtl.wrd) ;
  }

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SPI3 PAUSE CYCLES                           */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi3_dev_set_pause_cycles(cs_uint8 dev_id,
                        cs_uint8 pause_cycles)
/* INPUTS     : o Device Id                                     */
/*              o Number of pause cycles (0...3)                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Setup the number of cycles to pause (deassert valid) before  */
/* sending a new in-band address in SPI3 mode                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t         * pSpi ;
  ARN5_SPI4P2_RXIF_spi3ctl_t rCtl ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (pause_cycles > 3 ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      ":pause_cycles(%d) must be less then 4\n", pause_cycles) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  rCtl.wrd = CS_REG_READ(&pSpi->rxif_spi3ctl.wrd) ;
  rCtl.bf.npause = pause_cycles ;
  CS_REG_WRITE(&pSpi->rxif_spi3ctl.wrd, rCtl.wrd) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET TX FIFO PARAMETERS                      */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_tx_fifo_params(cs_uint8 dev_id,
                        cs_uint8 chan_num,
                        cs_uint16 low_addr, cs_uint16 high_addr,
                        cs_uint16 low_wmark, cs_uint16 high_wmark)
/* INPUTS     : o Device Id                                     */
/*              o Channel Number (0...7)                        */
/*              o Low(start) address(16-bits)                   */
/*              o High(end) address(16-bits)                    */
/*              o Low(almost empty) watermark(16-bits)          */
/*              o High(almost full) watermark(16-bits)          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the FIFO parameters for the specified channel.          */
/*                                                              */
/* The [low_addr] parameter specifies the Tx FIFO's starting    */
/* address in memory. This operates on 128-bit boundary.        */
/*                                                              */
/* The [high_addr] parameter specifies the Tx FIFO's ending     */
/* address in memory. This operates on 128-bit boundary.        */
/* If the size is n bytes, then this should be configured to    */
/* (low_addr + (n * 8 / 128) - 1). Min value of n is 16 bytes.  */
/*                                                              */
/* The [low_wmark] parameter specifies ALMOST EMPTY condition.  */
/* The TSTAT signal(to Host) indicates STARVING when the        */
/* Tx FIFO is below this watermark. If the FIFO depth exceeds   */
/* this watermark then HUNGRY state is indicated.               */
/* This operates on 128-bit boundary. This is an offset(and not */
/* an address) into the specified channel's FIFO.               */
/*                                                              */
/* The [high_wmark] parameter specifies ALMOST FULL condition.  */
/* The TSTAT signal(to Host) indicates SATISFIED when the       */
/* Tx FIFO depth reaches this threshold.                        */
/* This operates on 128-bit boundary. This is an offset(and not */
/* an address) into the specified channel's FIFO.               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                * pDev ;
  volatile cs_reg       * reg_low_addr,  * reg_high_addr ;
  volatile cs_reg       * reg_low_wmark, * reg_high_wmark ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (chan_num > ARN5_MAX_NUM_CHANNELS) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  ": chan_num(%d) s/b < 8\n", chan_num) ;
    return (CS_ERROR) ;
  }

  if (low_addr > high_addr) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  ": low_addr(0x%x) s/b <= high_addr(0x%x)\n",
                  low_addr, high_addr) ;
    return (CS_ERROR) ;
  }

  if (low_wmark > high_wmark) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  ": low_wmark(0x%x) s/b <= high_wmark(0x%x)\n",
                  low_wmark, high_wmark) ;
    return (CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  reg_low_addr   = &pDev->SPI4P2a.mem_lo_0.wrd + chan_num ;
  reg_high_addr  = &pDev->SPI4P2a.mem_hi_0.wrd + chan_num ;
  reg_low_wmark  = &pDev->SPI4P2a.almost_empty.wrd + (chan_num * 16) ;
  reg_high_wmark = &pDev->SPI4P2a.almost_full.wrd + (chan_num * 16) ;

  CS_REG_WRITE(reg_low_addr,   low_addr) ;
  CS_REG_WRITE(reg_high_addr,  high_addr) ;
  CS_REG_WRITE(reg_low_wmark,  low_wmark) ;
  CS_REG_WRITE(reg_high_wmark, high_wmark) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET RX FIFO PARAMETERS                      */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_dev_set_rx_fifo_params(cs_uint8 dev_id,
                        cs_uint8 chan_num,
                        cs_uint8 low_wmark, cs_uint8 high_wmark,
                        cs_uint8 hysteresis_wmark)
/* INPUTS     : o Device Id                                     */
/*              o Channel Number (0...7)                        */
/*              o Low(almost empty) watermark(8-bits)           */
/*              o High(almost full) watermark(8-bits)           */
/*              o Hysteresis watermark(8-bits)                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the RX FIFO parameters for a port.                      */
/*                                                              */
/* The [low_wmark] parameter specifies ALMOST EMPTY condition.  */
/*                                                              */
/* The [high_wmark] parameter specifies ALMOST FULL condition.  */
/*                                                              */
/* The [hysteresis_wmark] specifies read enable condition.      */
/* when number of entries in Rx FIFO reaches this level, it     */
/* will enable reading from the RX FIFO.                        */
/*                                                              */
/* All watermarks operate on 128-bit boundary. They are just    */
/* offsets(and not an address) into a channel's FIFO.           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_SPI4P2_t         * pSpi ;
  cs_reg                * pReg ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (low_wmark > high_wmark) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
                  ": low_wmark(0x%x) s/b <= high_wmark(0x%x)\n",
                  low_wmark, high_wmark) ;
    return (CS_ERROR) ;
  }

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  pReg = (cs_reg *) &pSpi->almost_empty_0.wrd + chan_num * 3 ;
  CS_REG_WRITE(pReg, low_wmark) ;
  pReg = (cs_reg *) &pSpi->almost_full_0.wrd + chan_num * 3 ;
  CS_REG_WRITE(pReg, high_wmark) ;
  pReg = (cs_reg *) &pSpi->hysteresis_0.wrd + chan_num * 3 ;
  CS_REG_WRITE(pReg, hysteresis_wmark) ;

  return (CS_OK) ;
}


/****************************************************************
 *******************   SPI PORT-LEVEL API's    ******************
 ****************************************************************/
/****************************************************************/
/* $rtn_hdr_start   CONTROL STORE AND FORWARD MODE              */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_ctl_store_forward_mode(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_boolean gfp_pli_insert)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/*              o TRUE (insert GFP PLI) or FALSE (no insert)    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable store and forward mode for a given channel.   */
/*                                                              */
/* [gfp_pli_insert] can only be TRUE when stort and forward mode*/
/* is enabled.                                                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                * pDev ;
  volatile cs_reg       * pReg ;
  cs_uint8              devId, sliceNum ;
  ARN5_SPI4P2A_TXCFG_phy_ch0_t  chCfg;

  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sliceNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;

  ARN5_SPI_DEV_LOCK(devId) ;

  pReg = (cs_reg *)&pDev->SPI4P2a.phy_ch0 + sliceNum ;
  chCfg.wrd = CS_REG_READ(pReg) ;
  chCfg.bf.store_fwd = (ctl == CS_ENABLE) ? 1 : 0 ;
  chCfg.bf.gfp_pli = gfp_pli_insert ? 1 : 0 ;
  CS_REG_WRITE(pReg, chCfg.wrd) ;

  ARN5_SPI_DEV_UNLOCK(devId) ;

  return (CS_OK) ;
}


/****************************************************************
 *****************     Internal functions       *****************
 ****************************************************************/


/*----------------------------------------------------*/
/* util to convert device (logical) channel number to */
/* physical channel number                            */
/*----------------------------------------------------*/
cs_uint8  arn5_logical2physical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint8 logical_chan_num)
{
  arn5_dev_cb_t         * pDevCb ;
  cs_uint8              phy ;

  /* internal function, dev_id is presumablly valid */
  pDevCb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if (logical_chan_num >= ARN5_MAX_NUM_CHANNELS) {
    return 0xff ;
  }

  phy = (dir == CS_RX) ?
    pDevCb->rx_log_chan_map[logical_chan_num] :
    pDevCb->tx_log_chan_map[logical_chan_num] ;

  return (phy) ;
}

/*----------------------------------------------------*/
/* util to convert physical channel number to         */
/* device (logical) channel number                    */
/* technically speaking, caller needs to check for    */
/* not found condition, i.e. return number == 255     */
/*----------------------------------------------------*/
cs_uint8  arn5_physical2logical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint8 physical_chan_num)
{
  arn5_dev_cb_t         * pDevCb ;
  cs_uint8              log ;

  /* internal function, dev_id is presumablly valid */
  pDevCb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if (physical_chan_num >= ARN5_MAX_NUM_CHANNELS) {
    return 0xff ;
  }

  log = (dir == CS_RX) ?
    pDevCb->rx_phy_chan_map[physical_chan_num] :
    pDevCb->tx_phy_chan_map[physical_chan_num] ;

  return (log) ;
}


/*----------------------------------------------------*/
/* SPI4.2 loopback                                    */
/*----------------------------------------------------*/
static cs_status  arn5_spi42_dev_ctl_loopback(cs_uint8 dev_id,
                        cs_dir_t dir, cs_ctl_t ctl)
{
  ARN5_t                 * pDev ;
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_MIF_loopbacks_t lpbk ;

  if (ctl == CS_ENABLE) {
    if (dir == CS_TX) {
      CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        ": The loopback selection is not supported.\n") ;
      return (CS_ERROR) ;
    }
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  lpbk.wrd = CS_REG_READ(&pSpi->loopbacks.wrd) ;

  if (dir == CS_TX) {
    lpbk.bf.bertlb = (ctl == CS_ENABLE) ? 1 : 0 ;
    if (ctl == CS_ENABLE) {
      lpbk.bf.lb = 0 ;
    }
  }
  else {
    lpbk.bf.lb = (ctl == CS_ENABLE) ? 1 : 0 ;
    if (ctl == CS_ENABLE) {
      lpbk.bf.bertlb = 0 ;
    }
  }
  CS_REG_WRITE(&pSpi->loopbacks.wrd, lpbk.wrd) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/*----------------------------------------------------*/
/* SPI3 loopback                                      */
/*----------------------------------------------------*/
static cs_status  arn5_spi3_dev_ctl_loopback(cs_uint8 dev_id,
                        cs_dir_t dir, cs_ctl_t ctl)
{
  ARN5_SPI4P2_t         * pSpi ;
  ARN5_SPI4P2_TXIF_spi3ctl_t txif_spi3ctl;
  ARN5_SPI4P2_RXIF_spi3ctl_t rxif_spi3ctl;
  ARN5_SPI4P2_MIF_loopbacks_t lpbk ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  ARN5_SPI_DEV_LOCK(dev_id) ;

  txif_spi3ctl.wrd = CS_REG_READ(&pSpi->txif_spi3ctl.wrd) ;
  rxif_spi3ctl.wrd = CS_REG_READ(&pSpi->rxif_spi3ctl.wrd) ;
  lpbk.wrd = CS_REG_READ(&pSpi->loopbacks.wrd) ;

  if (ctl == CS_ENABLE) {
    if (dir == CS_TX) {
      /*     ----------|    |----------                */
      /*               |    |                          */
      /*             -<|----|--<--                     */
      /*   My Chip   | |    |        Other Chip        */
      /*             | |    |                          */
      /*             ->|----|-->--                     */
      /*               |    |                          */
      /*     ----------|    |----------                */
      /*                                               */
      txif_spi3ctl.bf.phylink_flip = 0 ;
      txif_spi3ctl.bf.rx2tx = 0 ;
      txif_spi3ctl.bf.tx2rx = 1 ;
      rxif_spi3ctl.bf.phylink_flip = 0 ;
      rxif_spi3ctl.bf.rx2tx = 0 ;
      rxif_spi3ctl.bf.tx2rx = 1 ;
      lpbk.bf.lb = 0 ;   /* for clock mux control */
    }
    else {
      /*     ----------|    |----------                */
      /*               |    |                          */
      /*       ---<--- |    |                          */
      /*   My Chip   | |    |        Other Chip        */
      /*             | |    |                          */
      /*       --->--- |    |                          */
      /*               |    |                          */
      /*     ----------|    |----------                */
      /*                                               */
      /* use either RENB or PTPA/STPA as flow control  */
      txif_spi3ctl.bf.phylink_flip = 1 ;
      txif_spi3ctl.bf.rx2tx = 1 ;
      txif_spi3ctl.bf.tx2rx = 0;
      rxif_spi3ctl.bf.phylink_flip = 0 ;
      rxif_spi3ctl.bf.rx2tx = 1 ;
      rxif_spi3ctl.bf.tx2rx = 0 ;
      lpbk.bf.lb = 1 ;   /* for clock mux control */
    }
  }
  else {
    txif_spi3ctl.bf.phylink_flip = 0 ;
    txif_spi3ctl.bf.rx2tx = 0 ;
    txif_spi3ctl.bf.tx2rx = 0 ;
    rxif_spi3ctl.bf.phylink_flip = 0 ;
    rxif_spi3ctl.bf.rx2tx = 0 ;
    rxif_spi3ctl.bf.tx2rx = 0 ;
    lpbk.bf.lb = 0 ;
  }

  CS_REG_WRITE(&pSpi->txif_spi3ctl.wrd, txif_spi3ctl.wrd) ;
  CS_REG_WRITE(&pSpi->rxif_spi3ctl.wrd, rxif_spi3ctl.wrd) ;
  CS_REG_WRITE(&pSpi->loopbacks.wrd, lpbk.wrd) ;

  ARN5_SPI_DEV_UNLOCK(dev_id) ;

  return (CS_OK) ;
}


/*----------------------------------------------------*/
/* Check whether the interface is SPI3 mode or not    */
/*----------------------------------------------------*/
cs_boolean  arn5_spi_dev_is_in_spi3_mode(cs_uint8 dev_id)
{
  return (!arn5_spi_dev_is_in_spi42_mode(dev_id));
}

/*----------------------------------------------------*/
/* Check whether the interface is SPI42 mode or not   */
/*----------------------------------------------------*/
cs_boolean  arn5_spi_dev_is_in_spi42_mode(cs_uint8 dev_id)
{
  ARN5_SPI4P2_t          * pSpi ;
  ARN5_SPI4P2_MIF_status_t       mif_status ;

  pSpi = ARN5_SPI_GET_PCOMM(dev_id) ;

  mif_status.wrd = CS_REG_READ(&pSpi->mif_status.wrd);
  return (mif_status.bf.spi3mode == 0);
}

