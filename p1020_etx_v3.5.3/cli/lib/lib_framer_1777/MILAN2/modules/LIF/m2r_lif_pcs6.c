/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif_pcs6
 *
 * Milan driver libraries pertaining to LIF PCS64/66 block.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_registers.h"
#include "m2r_public.h"
#include "m2r_lif_pcs6.h"

/************************************************************************/
/*    Macros and defines                                                */
/************************************************************************/
#define M2R_LIF_PCS_GET_DEV_BASEADDR(PORT_ID, BASE_ADDR, ERR_CODE)  \
{                                                                   \
  cs_uint32     err_code ;                                          \
  if ( !(M2R_IS_PORT_VALID(PORT_ID, err_code)) ) {                  \
     CS_HNDL_ERROR(port_id, err_code, NULL) ;                       \
     return (ERR_CODE) ;                                            \
  }                                                                 \
  if(M2R_IS_SONET_MODE((PORT_ID)) == TRUE) {                        \
    CS_HNDL_ERROR((PORT_ID), EM2R_INVALID_IN_SONET_MODE,            \
                  "PCS6 is powered DOWN in SONET Mode\n") ;         \
     return (ERR_CODE) ;                                            \
  }                                                                 \
  (BASE_ADDR) = M2R_PORT_ID_TO_DEV_BASE(port_id);                   \
}


/************************************************************************/
/* $rtn_hdr_start   GET PCS SYNC STATUS                                 */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_boolean m2r_lif_pcs6_is_sync(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : TRUE or FALSE                                           */
/* DESCRIPTION:                                                         */
/* Get the sync status of PCS.                                          */
/* If the returned value is TRUE, then it is in-sync.                   */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  cs_boolean           inSync = FALSE ;
  PCS6_RX_RxStat_t     syncStatus ;
  PCS6_RX_RxCntrl_t    rx_ctrl ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  rx_ctrl.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.RxCntrl.wrd) ;
  syncStatus.wrd = M2R_REG_READ(&pDev->LIF.PCS6.RxStat.wrd) ;

  if (rx_ctrl.bf.pcs6_rx_test  == pcs6_rx_test_Normal) {
    inSync = ( syncStatus.bf.SyncDetS ) ? TRUE : FALSE ;
  }
  else {
    inSync = ( syncStatus.bf.PatSyncS ) ? TRUE : FALSE ;
  }
  return (inSync) ;
}

/************************************************************************/
/* $rtn_hdr_start   SET THE IN-SYNC THRESHOLD VALUE                     */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_uint16 m2r_lif_pcs6_set_in_sync_threshold(cs_uint16 port_id,
                                             cs_uint8 thrshold)
/* INPUTS     : o Port Id                                               */
/*              o Sync Threshold value (33 - 95)                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Sets the "in-sync" threshold value. This is the number of valid      */
/* SYNC Headers in a window of 125 microsecond that leads to sync state.*/
/* Default value is high BER threshold is 16.                           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_RX_SynOff_t     invalid_off ;
  cs_uint8             val = 0 ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  if ((thrshold < M2R_LIF_PCS_IN_SYNC_MIN_THR) ||
      (thrshold > M2R_LIF_PCS_IN_SYNC_MAX_THR)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR, "valid range (33-95)\n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  if ( thrshold < M2R_LIF_PCS_IN_SYNC_DEF_THR ) {
    val = (0x0040 | (M2R_LIF_PCS_IN_SYNC_DEF_THR - thrshold)) ;
  }
  else {
    val = (thrshold - M2R_LIF_PCS_IN_SYNC_DEF_THR) ;
  }

  invalid_off.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.SynOff.wrd) ;
  invalid_off.bf.sync_valid_off = (cs_uint8) val ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.SynOff.wrd, invalid_off.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   SET THE HIGH BER THRESHOLD VALUE                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_uint16 m2r_lif_pcs6_set_ber_threshold(cs_uint16 port_id,
                                         cs_uint8 thrshold)
/* INPUTS     : o Port Id                                               */
/*              o High BER threshold value ( 9 to 23 )                  */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Set the BER sync header error rate high threshold value. BER monitor */
/* counts the number of sync header errors in 125 microsecond window.   */
/* Default value is high BER threshold is 16.                           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_RX_SynOff_t     syncOff ;
  cs_uint8             val = 0 ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  if ((thrshold < M2R_LIF_PCS_BER_HIGH_MIN_THR) ||
      (thrshold > M2R_LIF_PCS_BER_HIGH_MAX_THR)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR, "valid range (9-23)\n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  if ( thrshold < M2R_LIF_PCS_BER_HIGH_DEF_THR ) {
    val = (0x0008 | (M2R_LIF_PCS_BER_HIGH_DEF_THR - thrshold)) ;
  }
  else {
    val = (thrshold - M2R_LIF_PCS_BER_HIGH_DEF_THR) ;
  }

  syncOff.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.SynOff.wrd) ;
  syncOff.bf.sync_ber_off = (cs_uint8) val ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.SynOff.wrd, syncOff.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   SET THE OUT OF SYNC THRESHOLD VALUE                 */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_pcs6_set_oos_threshold(cs_uint16 port_id,
                                         cs_uint8 thrshold)
/* INPUTS     : o Port Id                                               */
/*              o OOS Threshold value (9 - 23)                          */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Sets the "out of sync" threshold value. This is the number of invalid*/
/* SYNC Headers in a window of 125 microsecond that leads to sync loss. */
/* Default value is high BER threshold is 16.                           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_RX_SynOff_t     invalid_off ;
  cs_uint8             val = 0 ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  if ((thrshold < M2R_LIF_PCS_OOS_MIN_THR) ||
      (thrshold > M2R_LIF_PCS_OOS_MAX_THR)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR, "valid range (9-23)\n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  if ( thrshold < M2R_LIF_PCS_OOS_DEF_THR ) {
    val = (0x0008 | (M2R_LIF_PCS_OOS_DEF_THR - thrshold)) ;
  }
  else {
    val = (thrshold - M2R_LIF_PCS_OOS_DEF_THR) ;
  }

  invalid_off.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.SynOff.wrd) ;
  invalid_off.bf.sync_invalid_off = (cs_uint8) val ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.SynOff.wrd, invalid_off.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   GET THE TRAFFIC BER AT PCS                          */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_uint16 m2r_lif_pcs6_get_ber(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(default) or M2R_ENABLE                    */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : BER value                                               */
/* DESCRIPTION:                                                         */
/* Return the current BER ( Bit error rate ).                           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_RX_ErrBer_t     rxBer ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  rxBer.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.ErrBer.wrd) ;
  return (rxBer.wrd) ;
}

/************************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PCS (DE)SCRAMBLER                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_pcs6_ctl_scrambling(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(default) or M2R_ENABLE                    */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the PCS64/66 scrambling and descrambling.             */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_TX_TxCntrl_t    tx_ctrl ;
  PCS6_RX_RxCntrl_t    rx_ctrl ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  tx_ctrl.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.TxCntrl.wrd) ;
  rx_ctrl.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.RxCntrl.wrd) ;

  tx_ctrl.bf.scram_tx_dis = (ctl == M2R_DISABLE) ? 1 : 0 ;
  rx_ctrl.bf.descram_rx_dis = (ctl == M2R_DISABLE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->LIF.PCS6.TxCntrl.wrd, tx_ctrl.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.RxCntrl.wrd, rx_ctrl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF) ;
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  CONFIGURE SEQUENCE ORDERED SET DATA                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_pcs6_cfg_sos(cs_uint16 port_id, cs_uint8 byte1,
                               cs_uint8 byte2, cs_uint8 byte3)
/* INPUTS     : o Port Id                                               */
/*              o byte-1 of SOS                                         */
/*              o byte-2 of SOS                                         */
/*              o byte-3 of SOS                                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Configures the SOS to be sent . SOS is sent in case a fault is       */
/* detected.                                                            */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_TX_SOS_A_t      sosA ;
  PCS6_TX_SOS_B_t      sosB ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  sosA.bf.sequ_txd_1 = byte1 ;
  sosA.bf.sequ_txd_2 = byte2 ;
  sosB.bf.sequ_txd_3 = byte3 ;

  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_SOS_A.wrd, sosA.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_SOS_B.wrd, sosB.wrd) ;

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE SOS DATA TRANSMISSION                 */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_pcs6_ctl_sos(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)                       */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable transmission of SOS.                                  */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_TX_TxCntrl_t    tx_ctrl ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  tx_ctrl.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.TxCntrl.wrd) ;
  tx_ctrl.bf.sequ_tx_ena = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.TxCntrl.wrd, tx_ctrl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF) ;
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE PCS TEST PATTERN GENERATOR AND CHECKER */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_pcs6_ctl_test_pattern(cs_uint16 port_id,
                                        m2r_lif_pcs_prbs_t pattern,
                                        m2r_dir_t dir,
                                        m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Test pattern type                                     */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX                     */
/*              o M2R_DISABLE(0) or M2R_ENABLE (1)                      */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the PCS64/66 PRBS.                                    */
/* If pseudo-random test pattern is to be generated/checked, user       */
/* should use m2r_lif_set_prbs_rnd_seed() API to set the seed and data  */
/* values.                                                              */
/*                                                                      */
/* The [pattern] parameter is one of the following:                     */
/* M2R_LIF_PCS_DAT_LOW or M2R_LIF_PCS_SQ1 or M2R_LIF_PCS_SQ2  or        */
/* M2R_LIF_PCS_SQ4 or M2R_LIF_PCS_SQ8 or M2R_LIF_PCS_PULSE0 or          */
/* M2R_LIF_PCS_PULSE1 or M2R_LIF_PCS_DAT_HIGH or M2R_LIF_PCS_PRBS_31    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_TX_TxCntrl_t    tx_ctrl ;
  PCS6_RX_RxCntrl_t    rx_ctrl ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
    tx_ctrl.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.TxCntrl.wrd) ;

    if (ctl == M2R_DISABLE) {
      tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Normal ;
    }
    else {
      switch (pattern) {
        case M2R_LIF_PCS_PRBS_31 :
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_PRBS31 ;
        break ;
        case M2R_LIF_PCS_PRBS_RANDOM :
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Pseudo ;
        break ;
        case M2R_LIF_PCS_SQ1:
          tx_ctrl.bf.square_tx_pat = square_tx_pat_Square1 ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        case M2R_LIF_PCS_SQ2:
          tx_ctrl.bf.square_tx_pat = square_tx_pat_Square2 ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        case M2R_LIF_PCS_SQ4:
          tx_ctrl.bf.square_tx_pat = square_tx_pat_Square4 ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        case M2R_LIF_PCS_SQ8:
          tx_ctrl.bf.square_tx_pat = square_tx_pat_Square8 ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        case M2R_LIF_PCS_PULSE0:
          tx_ctrl.bf.square_tx_pat = square_tx_pat_Pulse0 ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        case M2R_LIF_PCS_PULSE1:
          tx_ctrl.bf.square_tx_pat = square_tx_pat_Pulse1 ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        case M2R_LIF_PCS_DAT_LOW :
          tx_ctrl.bf.square_tx_pat = square_tx_pat_Low ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        case M2R_LIF_PCS_DAT_HIGH :
          tx_ctrl.bf.square_tx_pat = square_tx_pat_High ;
          tx_ctrl.bf.pcs6_tx_test  = pcs6_tx_test_Square ;
        break ;
        default :
        break ;
      }
    }
    M2R_REG_WRITE(&pDev->LIF.PCS6.TxCntrl.wrd, tx_ctrl.wrd) ;
  }

  if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
    rx_ctrl.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.RxCntrl.wrd) ;

    if (ctl == M2R_DISABLE) {
      rx_ctrl.bf.pcs6_rx_test  = pcs6_rx_test_Normal ;
    }
    else {
      switch (pattern) {
        case M2R_LIF_PCS_PRBS_31 :
          rx_ctrl.bf.pcs6_rx_test  = pcs6_rx_test_PRBS31 ;
        break ;
        case M2R_LIF_PCS_PRBS_RANDOM :
          rx_ctrl.bf.pcs6_rx_test  = pcs6_rx_test_Pseudo ;
        break ;
        default :
        break ;
      }
    }
    M2R_REG_WRITE(&pDev->LIF.PCS6.RxCntrl.wrd, rx_ctrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF) ;
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   SET PSEUDO PATTERN SEEDS                            */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_set_prbs_rnd_seed(cs_uint16 port_id,
                                    m2r_dir_t dir,
                                    m2r_lif_prbs_rnd_seed_t type,
                                    cs_uint64 seed_A,
                                    cs_uint64 seed_B,
                                    cs_uint64 data)
/* INPUTS     : o Port Id                                               */
/*              o M2R_TX or M2R_RX or M2R_RX_AND_TX                     */
/*              o Random PRBS seed or random data type                  */
/*              o Random seed-A                                         */
/*              o Random seed-B                                         */
/*              o Data                                                  */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Sets the Random pseudo pattern's seed or data value.                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
    /* Set SEED A value */
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_0.wrd, seed_A) ;
    seed_A = (seed_A >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_1.wrd, seed_A) ;
    seed_A = (seed_A >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_2.wrd, seed_A) ;
    seed_A = (seed_A >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_3.wrd, seed_A & 0x03FF) ;

    /* Set SEED B value */
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_0.wrd, seed_B) ;
    seed_B = (seed_B >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_1.wrd, seed_B) ;
    seed_B = (seed_B >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_2.wrd, seed_B) ;
    seed_B = (seed_B >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_3.wrd, seed_B & 0x03FF) ;

    /* Set PRBS Data value */
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_0.wrd, data) ;
    data = (data >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_1.wrd, data) ;
    data = (data >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_2.wrd, data) ;
    data = (data >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_3.wrd, data & 0x03FF) ;
  }

  if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
    /* Set SEED A value */
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_0.wrd, seed_A) ;
    seed_A = (seed_A >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_1.wrd, seed_A) ;
    seed_A = (seed_A >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_2.wrd, seed_A) ;
    seed_A = (seed_A >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_3.wrd, seed_A & 0x03FF) ;

    /* Set SEED B value */
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_0.wrd, seed_B) ;
    seed_B = (seed_B >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_1.wrd, seed_B) ;
    seed_B = (seed_B >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_2.wrd, seed_B) ;
    seed_B = (seed_B >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_3.wrd, seed_B & 0x03FF) ;

    /* Set PRBS Data value */
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_0.wrd, data) ;
    data = (data >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_1.wrd, data) ;
    data = (data >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_2.wrd, data) ;
    data = (data >> 16) ;
    M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_3.wrd, data & 0x03FF) ;
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   INJECT ERRORS IN PCS PRBS PATTERN                   */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_pcs6_inj_error(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Injects error in the PRBS stream.                                    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_TX_TxCntrl_t    tx_ctrl ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  tx_ctrl.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.TxCntrl.wrd) ;
  tx_ctrl.bf.test_tx_err = 0 ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.TxCntrl.wrd, tx_ctrl.wrd) ;
  tx_ctrl.bf.test_tx_err = 1 ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.TxCntrl.wrd, tx_ctrl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF) ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start   GET PCS ERRORED TEST PATTERN COUNT                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_uint16 m2r_lif_pcs6_get_prbs_errors(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : Error count since last read                             */
/* DESCRIPTION:                                                         */
/* Get the errored test patterns encountered in PCS since the last call */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_RX_ErrPat_t     rxErr ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  rxErr.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.ErrPat.wrd) ;
  return (rxErr.wrd) ;
}

/************************************************************************/
/* $rtn_hdr_start  READ THE SEQUENCE ORDERED SET DATA RECEIVED          */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_uint32 m2r_lif_pcs6_get_sos(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : SOS data received                                       */
/* DESCRIPTION:                                                         */
/* Reads the SOS Data received. The data is returned in uint32.         */
/* Data returned from this API should be interpreted as :               */
/* SOS BYTE3 [23:16] , SOS BYTE2 [15:8] AND SOS BYTE1 [7:0]             */
/* NOTE : user to make sure that valid data is received(indicated by    */
/* the SOS interrupt), before reading the SOS data.                     */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  PCS6_RX_SOS_A_t      sosA ;
  PCS6_RX_SOS_B_t      sosB ;
  cs_uint32            sosData = 0 ;

  M2R_LIF_PCS_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  sosA.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.rx_SOS_A.wrd) ;
  sosB.wrd =  M2R_REG_READ(&pDev->LIF.PCS6.rx_SOS_B.wrd) ;

  sosData = ((sosB.bf.sequ_rxd_3 << 24) | (sosA.bf.sequ_rxd_2 << 16) |
             (sosA.bf.sequ_rxd_1)) ;
  return (sosData) ;
}


