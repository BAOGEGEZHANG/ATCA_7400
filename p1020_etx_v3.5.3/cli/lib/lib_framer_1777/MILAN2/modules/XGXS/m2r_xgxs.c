/***********************************************************************/
/* This file contains unpublished documentation and software           */ 
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_xgxs
 *
 * Milan driver libraries pertaining to XGXS block
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_registers.h"
#include "m2r_public.h"
#include "m2r_xgxs.h"

/************************************************************************/
/*                 SET REFERNCE CLOCK FOR XAUI INTERFACE                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_xgxs_set_ref_clock(cs_uint16 port_id, cs_uint16 modId,
                               XGXS_t * xgxs, m2r_xgxs_xaui_ref_clk refClk)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC XGXS )                        */
/*              o Pointer to XGXS block                                 */
/*              o refernce clock selection                              */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Set the refernce clock for XAUI interface.                           */
/************************************************************************/
{
  XGXS_MP_XAUI0_t      xgxs_xaui ;
  cs_uint8             bfRefClk ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( CS_ERROR ) ;
  }

  switch(refClk) {
    case M2R_XGXS_XAUI_REF_CLK_156 :
      bfRefClk = 0x00 ;
      break ;
    case M2R_XGXS_XAUI_REF_CLK_312 :
      bfRefClk = 0x01 ;
      break ;
    case M2R_XGXS_XAUI_REF_CLK_625 :
      bfRefClk = 0x02 ;
      break ;
    default :
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_REF_CLK, NULL ) ;
      return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_xaui.wrd = CS_REG_2READ(&xgxs->XAUI0.wrd) ;
  xgxs_xaui.bf.xaui_tx_ckref_d = bfRefClk ;
  M2R_REG_WRITE(&xgxs->XAUI0.wrd, xgxs_xaui.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                ENABLE/DISABLE CLOCKS TO XGXS BLOCK                   */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_xgxs_ctl_clocks(cs_uint16 port_id, cs_uint16 modId,
                               XGXS_t * xgxs, m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC XGXS )                        */
/*              o Pointer to XGXS block                                 */
/*              o control (M2R_ENABLE or M2R_DISABLE)                   */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable clocks to XGXS block.                                 */
/*                                                                      */
/************************************************************************/
{
  XGXS_MP_MnCntrl0_t   xgxs_ctrl ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( CS_ERROR ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_ctrl.wrd = CS_REG_2READ(&xgxs->MnCntrl0.wrd) ;

  if((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
    xgxs_ctrl.bf.rx_clock_ena = (ctl == M2R_ENABLE) ? 1 : 0 ;
  }

  if((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
    xgxs_ctrl.bf.tx_clock_ena = (ctl == M2R_ENABLE) ? 1 : 0 ;
  }
  M2R_REG_WRITE(&xgxs->MnCntrl0.wrd, xgxs_ctrl.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                ENABLE/DISABLE POINTER ADJUST                         */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_xgxs_ctl_pointer_adj(cs_uint16 port_id, cs_uint16 modId,
                                    XGXS_t * xgxs, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC XGXS )                        */
/*              o Pointer to XGXS block                                 */
/*              o control (M2R_ENABLE or M2R_DISABLE)                   */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable pointer adjust for signal detect.                      */
/************************************************************************/
{
  XGXS_MP_MnCntrl0_t   xgxs_ctrl ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( CS_ERROR ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_ctrl.wrd = CS_REG_2READ(&xgxs->MnCntrl0.wrd) ;
  xgxs_ctrl.bf.xgxs_pa_enable = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&xgxs->MnCntrl0.wrd, xgxs_ctrl.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 XGXS ENABLE/DISABLE RX TO TX LOOPBACK                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_xgxs_ctl_rx_tx_loopback(cs_uint16 port_id, cs_uint16 modId,
                                       XGXS_t * xgxs, cs_uint8 ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC XGXS )                        */
/*              o Pointer to XGXS block                                 */
/*              o control (M2R_ENABLE or M2R_DISABLE)                   */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable RX to TX Loopback at XGXS block.                      */
/************************************************************************/
{
  XGXS_MP_MnCntrl0_t   xgxs_ctrl ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( CS_ERROR ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_ctrl.wrd = CS_REG_2READ(&xgxs->MnCntrl0.wrd) ;
  xgxs_ctrl.bf.xgxs_rx_tx_lb = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&xgxs->MnCntrl0.wrd, xgxs_ctrl.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 XGXS ENABLE/DISABLE TX TO RX LOOPBACK                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_xgxs_ctl_tx_rx_loopback(cs_uint16 port_id, cs_uint16 modId,
                                       XGXS_t * xgxs, cs_uint8 ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o control (M2R_ENABLE or M2R_DISABLE)                   */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable TX to RX Loopback at XGXS block.                      */
/************************************************************************/
{
  XGXS_MP_MnCntrl0_t   xgxs_ctrl ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( CS_ERROR ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_ctrl.wrd = CS_REG_2READ(&xgxs->MnCntrl0.wrd) ;
  xgxs_ctrl.bf.xgxs_tx_rx_lb = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&xgxs->MnCntrl0.wrd, xgxs_ctrl.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 ENABLE/DISABLE XAUI LOOPBACK                         */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_ctl_xaui_loopback(cs_uint16 port_id, cs_uint16 modId,
                                     XGXS_t *xgxs, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable XAUI Loopback.                                        */
/************************************************************************/
{
  XGXS_MP_XAUI0_t      xgxs_xaui ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_xaui.wrd = CS_REG_2READ(&xgxs->XAUI0.wrd) ;
  xgxs_xaui.bf.xaui_rx_loopbk = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&xgxs->XAUI0.wrd, xgxs_xaui.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 CHECK XGXS CLOCK STATUS                              */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_xgxs_get_clk_status(cs_uint16 port_id, cs_uint16 modId,
                                                           XGXS_t * xgxs)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Return the Clock status.                                             */
/*                                                                       */
/************************************************************************/
{
  XGXS_MP_MnSta1_t     clk_status ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( CS_ERROR ) ;
  }

  clk_status.wrd = CS_REG_2READ(&xgxs->MnSta1.wrd) ;
  if ( clk_status.bf.tx_no_clock || clk_status.bf.rx_no_clock ) {
    return (CS_ERROR) ;
  }
  return (CS_OK) ;
}

/************************************************************************/
/*                 CHECK XGXS STATUS                                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_uint16 m2r_xgxs_get_status(cs_uint16 port_id, cs_uint16 modId,
                              XGXS_t * xgxs, m2r_xgxs_status_t sel)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o Status type selection                                 */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Return the status of the XGXS block.                                 */
/*                                                                      */
/************************************************************************/
{
  XGXS_MP_MnSta0_t  status0 ;
  XGXS_RX_RxSta_t   rxstatus ; 

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  if (sel == M2R_XGXS_LANE_SYNC_DET) {
    rxstatus.wrd = M2R_REG_READ(&xgxs->RxSta.wrd) ;
    return( (rxstatus.bf.SyncDet0 << 0) |
	    (rxstatus.bf.SyncDet1 << 1) |
	    (rxstatus.bf.SyncDet2 << 2) |
	    (rxstatus.bf.SyncDet3 << 3) ) ;
  }
  else {
    status0.wrd = CS_REG_2READ(&xgxs->MnSta0.wrd) ;
    switch (sel) {
      case M2R_XGXS_LANE_LOCK_DET:
	return status0.bf.lock_det ;
      case M2R_XGXS_LANE_DATA_DET:
	return status0.bf.data_det ;
      case M2R_XGXS_LANE_CLOCK:
	return status0.bf.no_clock ;
      default:
	return status0.wrd ;
    }
  }

  return (CS_OK) ;
}


/************************************************************************/
/*                 CHECK XGXS ALIGNMENT STATUS                          */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_boolean m2r_xgxs_is_align_detected(cs_uint16 port_id, cs_uint16 modId,
				      XGXS_t * xgxs)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : TRUE(aligned) or FALSE(not-aligned)                     */
/* DESCRIPTION:                                                         */
/* Return the alignment status of XGXS(Rx).                             */
/* A (word) alignment detction implies that each of the 4 lanes are     */
/* good(ie. lock, clock, data, and sync are detected) and so can be     */
/* used as a status indication for the XGXS interface.                  */
/************************************************************************/
{
  XGXS_RX_RxSta_t   rxstatus ; 

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  rxstatus.wrd = M2R_REG_READ(&xgxs->RxSta.wrd) ;

  if (rxstatus.bf.xgxs_align_det) return(TRUE) ;
  return(FALSE) ;

}


/************************************************************************/
/*                 RE-INIT ELASTIC STORE                                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_reset_estore(cs_uint16 port_id, cs_uint16 modId,
                                    XGXS_t * xgxs)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Reset the elastic store.                                             */
/*                                                                       */
/************************************************************************/
{
  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  M2R_REG_WRITE(&xgxs->ESINIT.wrd, 0) ;
  M2R_REG_WRITE(&xgxs->ESINIT.wrd, 0x00ff) ;
  return (CS_OK) ;
}

/************************************************************************/
/*                 ENABLE/DISABLE PRBS                                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_ctl_prbs(cs_uint16 port_id, cs_uint16 modId,
                            XGXS_t * xgxs, m2r_xgxs_xaui_lane_t lane,
                            m2r_xgxs_prbs_t pattern, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o XAUI lane                                             */
/*              o PRBS PATTERN                                          */
/*              o control (M2R_ENABLE or M2R_DISABLE)                   */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable XGXS PRBS. If Register based pattern is selected, user*/
/* should configure the pattern to be sent for the lane using           */
/* m2r_lif_xgxs_cfg_prbs_reg_data() API.                                */
/************************************************************************/
{
  XGXS_TX_TxCntrl0_t   tx_ctrl ;
  XGXS_RX_RxCntrl0_t   rx_ctrl ;
  cs_uint16            lane_start, lane_end, ii ;

  if (!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  if (lane > M2R_XGXS_XAUI_LANE_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_INVALID_XAUI_LANE_ID,
                  "Lane ID : %d\n", lane) ;
    return (CS_ERROR) ;
  }

  if ( lane == M2R_XGXS_XAUI_LANE_ALL ) {
    lane_start = M2R_XGXS_XAUI_LANE0 ;
    lane_end   = M2R_XGXS_XAUI_LANE3 ;
  }
  else {
    lane_start = lane_end = lane ;
  }

  M2R_PORT_LOCK(port_id, modId);

  tx_ctrl.wrd = M2R_REG_READ(&xgxs->TxCntrl0.wrd) ;
  rx_ctrl.wrd = M2R_REG_READ(&xgxs->RxCntrl0.wrd) ;

  if ( ctl == M2R_ENABLE ) {
    switch ( pattern ) {
      case M2R_XGXS_HFPAT:
        tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Frequency ;
        rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Frequency ;
        for(ii = lane_start; ii <= lane_end; ii++ ) {
          tx_ctrl.bf.xgxs_fpat_sel |= (xgxs_fpat_sel_High << (ii*2));
          rx_ctrl.bf.xgxs_fpat_sel |= (0x01 << (ii*2)) ;
        }
        break ;

      case M2R_XGXS_LFPAT:
        tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Frequency ;
        rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Frequency ;
        for(ii = lane_start; ii <= lane_end; ii++ ) {
          tx_ctrl.bf.xgxs_fpat_sel = (xgxs_fpat_sel_Low << (ii*2));
          rx_ctrl.bf.xgxs_fpat_sel = (0x02 << (ii*2)) ;
        }
        break ;

      case M2R_XGXS_MFPAT:
        tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Frequency ;
        rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Frequency ;
        for(ii = lane_start; ii <= lane_end; ii++ ) {
          tx_ctrl.bf.xgxs_fpat_sel = (xgxs_fpat_sel_Mixed << (ii*2));
          rx_ctrl.bf.xgxs_fpat_sel = (0x03 << (ii*2)) ;
        }
        break ;

      case M2R_XGXS_CRPAT_0:
        tx_ctrl.bf.xgxs_tx_random_6 = 0 ;
        tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Random ;
        rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Random ;
        break ;

      case M2R_XGXS_CRPAT_1:
        tx_ctrl.bf.xgxs_tx_random_6 = 1 ;
        tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Random ;
        rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Random ;
        break ;

      case M2R_XGXS_CJPAT:
        tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Jitter ;
        rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Jitter ;
        break ;

      case M2R_XGXS_REG_PATTERN:
        tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Frequency ;
        rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Frequency ;
        for(ii = lane_start; ii <= lane_end; ii++ ) {
          tx_ctrl.bf.xgxs_fpat_sel = (xgxs_fpat_sel_Register << (ii*2)) ;
          rx_ctrl.bf.xgxs_fpat_sel = (xgxs_rx_mode_Data      << (ii*2));
        }
        break ;
    }
  }
  else {
    tx_ctrl.bf.xgxs_tx_mode  = xgxs_tx_mode_Data ;
    rx_ctrl.bf.xgxs_rx_mode  = xgxs_rx_mode_Data ;
  }

  M2R_REG_WRITE(&xgxs->TxCntrl0.wrd, tx_ctrl.wrd) ;
  M2R_REG_WRITE(&xgxs->RxCntrl0.wrd, tx_ctrl.wrd) ;

  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 INJECT PRBS ERROR                                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_inj_error(cs_uint16 port_id, cs_uint16 modId,
                             XGXS_t *xgxs, m2r_xgxs_xaui_lane_t lane)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o XAUI Lane ID                                          */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Injects error in the PRBS stream.                                    */
/************************************************************************/
{
  XGXS_TX_TxCntrl0_t   tx_ctrl ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( CS_ERROR ) ;
  }

  if (lane > M2R_XGXS_XAUI_LANE_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_INVALID_XAUI_LANE_ID,
                  "Lane ID : %d\n", lane) ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  tx_ctrl.wrd = M2R_REG_READ(&xgxs->TxCntrl0.wrd) ;
  tx_ctrl.bf.xgxs_tx_err_inj = 0 ;
  M2R_REG_WRITE(&xgxs->TxCntrl0.wrd, tx_ctrl.wrd) ;
  if ( lane == M2R_XGXS_XAUI_LANE_ALL ) {
    tx_ctrl.bf.xgxs_tx_err_inj = 0x0f ;
  }
  else {
    tx_ctrl.bf.xgxs_tx_err_inj = (1 << lane) ;
  }
  M2R_REG_WRITE(&xgxs->TxCntrl0.wrd, tx_ctrl.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 GET INVALID CODES RECEIVED COUNTER                   */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_uint16 m2r_xgxs_get_code_err(cs_uint16 port_id, cs_uint16 modId,
                             XGXS_t *xgxs, m2r_xgxs_xaui_lane_t lane)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o Lane ID                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Return the invalid codes stats.                                      */
/************************************************************************/
{
  cs_uint16 ret;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  if (lane > M2R_XGXS_XAUI_LANE_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_INVALID_XAUI_LANE_ID,
                  "Lane ID : %d\n", lane) ;
    return (0) ;
  }

  switch(lane) {
    case M2R_XGXS_XAUI_LANE0 : {
      ret = M2R_REG_READ(&xgxs->ICC0.wrd) ;
      return (ret);
     }
     break ;

    case M2R_XGXS_XAUI_LANE1 : {
      ret = M2R_REG_READ(&xgxs->ICC1.wrd) ;
      return (ret);
     }
     break ;

    case M2R_XGXS_XAUI_LANE2 : {
      ret = M2R_REG_READ(&xgxs->ICC2.wrd) ;
      return (ret);
     }
     break ;

    case M2R_XGXS_XAUI_LANE3 : {
      ret = M2R_REG_READ(&xgxs->ICC3.wrd) ;
      return (ret);
    }

    case M2R_XGXS_XAUI_LANE_ALL :
     CS_PRINT("m2r_xgxs_get_code_err : This API returns Code Error count on 1 lane\n") ;
     break ;
  }
  return (0) ;
}

/************************************************************************/
/*                 ENABLE DATA DETECT SYNC ALGORITHM                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_ctl_ddsa(cs_uint16 port_id, cs_uint16 modId,
                            XGXS_t *xgxs, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable data detect sync algorithm in RX.                     */
/************************************************************************/
{
  XGXS_RX_RxCntrl0_t   rx_ctrl ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  rx_ctrl.wrd = M2R_REG_READ(&xgxs->RxCntrl0.wrd) ;
  rx_ctrl.bf.xgxs_ddsa_enable = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&xgxs->RxCntrl0.wrd, rx_ctrl.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 ENABLE/DISABLE DATA DETECT FOR LOCK DETECT           */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_ctl_data_det_for_lock_det(cs_uint16 port_id,
                                             cs_uint16 modId, XGXS_t *xgxs,
                                             m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable Data detect status for Lock detect.                   */
/************************************************************************/
{
  XGXS_MP_XAUI0_t      xgxs_xaui ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_xaui.wrd = CS_REG_2READ(&xgxs->XAUI0.wrd) ;
  xgxs_xaui.bf.xaui_data_lock = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&xgxs->XAUI0.wrd, xgxs_xaui.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/*                 ENABLE/DISABLE POWER TO XGXS BLOCK                   */
/* CATEGORY   : API                                                     */
/* ACCESS     : Private                                                 */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_ctl_power(cs_uint16 port_id, cs_uint16 modId,
                             XGXS_t *xgxs, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable power to XAUI and XGXS block.                         */
/************************************************************************/
{
  XGXS_MP_XAUI0_t      xgxs_xaui ;

  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  M2R_PORT_LOCK(port_id, modId);
  xgxs_xaui.wrd = CS_REG_2READ(&xgxs->XAUI0.wrd) ;
  xgxs_xaui.bf.xaui_rx_pd = (ctl == M2R_ENABLE) ? 0 : 1 ;
  xgxs_xaui.bf.xaui_tx_pd = (ctl == M2R_ENABLE) ? 0 : 1 ;
  M2R_REG_WRITE(&xgxs->XAUI0.wrd, xgxs_xaui.wrd) ;
  M2R_PORT_UNLOCK(port_id, modId);
  return (CS_OK) ;
}

/************************************************************************/
/* *               CONFIGURE THE REGISTER BASED PRBS PATTERN            */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : XGXS                                                    */
/* CHIP       : MILAN2                                                  */
cs_status m2r_xgxs_cfg_prbs_reg_data(cs_uint16 port_id,
                                         cs_uint16 modId,
                                         XGXS_t *xgxs,
                                         m2r_xgxs_xaui_lane_t lane,
                                         cs_boolean control_data,
                                         cs_uint8 reg_pattern)
/* INPUTS     : o Port Id                                               */
/*              o Module ID ( LIF or RMAC )                             */
/*              o Pointer to XGXS block                                 */
/*              o XAUI lane selection                                   */
/*              o TRUE ( CONTROL DATA ) or FALSE ( NORMAL DATA )        */
/*              o Register pattern                                      */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Configure the register based pattern to be sent on individual lanes. */
/* If control_data is TRUE, control code is transmitted else regular    */
/* 8 bit data is sent over the Lane.                                    */
/************************************************************************/
{
  if(!xgxs) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_BLK_POINTER_NULL, NULL) ;
    return ( 0 ) ;
  }

  if (lane > M2R_XGXS_XAUI_LANE_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_XGXS_INVALID_XAUI_LANE_ID,
                  "Lane ID : %d\n", lane) ;
    return (CS_ERROR) ;
  }

  switch (lane) {
   case M2R_XGXS_XAUI_LANE0 : {
      XGXS_TX_TxCntrl1_t   xgxs_tx_ctrl1 ;

      xgxs_tx_ctrl1.wrd = M2R_REG_READ(&xgxs->TxCntrl1.wrd) ;
      xgxs_tx_ctrl1.bf.xgxs_fpat_0 = reg_pattern ;
      if(control_data) {
        xgxs_tx_ctrl1.bf.xgxs_fpat_0 |= 0x0100 ;
      }
      M2R_REG_WRITE(&xgxs->TxCntrl1.wrd, xgxs_tx_ctrl1.wrd) ;
    }
    break ;

   case M2R_XGXS_XAUI_LANE1 : {
      XGXS_TX_TxCntrl2_t   xgxs_tx_ctrl2 ;

      xgxs_tx_ctrl2.wrd = M2R_REG_READ(&xgxs->TxCntrl2.wrd) ;
      xgxs_tx_ctrl2.bf.xgxs_fpat_1 = reg_pattern ;
      if(control_data) {
        xgxs_tx_ctrl2.bf.xgxs_fpat_1 |= 0x0100 ;
      }
      M2R_REG_WRITE(&xgxs->TxCntrl2.wrd, xgxs_tx_ctrl2.wrd) ;
    }
    break ;

   case M2R_XGXS_XAUI_LANE2 : {
      XGXS_TX_TxCntrl3_t   xgxs_tx_ctrl3 ;

      xgxs_tx_ctrl3.wrd = M2R_REG_READ(&xgxs->TxCntrl3.wrd) ;
      xgxs_tx_ctrl3.bf.xgxs_fpat_2 = reg_pattern ;
      if(control_data) {
        xgxs_tx_ctrl3.bf.xgxs_fpat_2 |= 0x0100 ;
      }
      M2R_REG_WRITE(&xgxs->TxCntrl3.wrd, xgxs_tx_ctrl3.wrd) ;
    }
    break ;

   case M2R_XGXS_XAUI_LANE3 : {
      XGXS_TX_TxCntrl4_t   xgxs_tx_ctrl4 ;

      xgxs_tx_ctrl4.wrd = M2R_REG_READ(&xgxs->TxCntrl4.wrd) ;
      xgxs_tx_ctrl4.bf.xgxs_fpat_3 = reg_pattern ;
      if(control_data) {
        xgxs_tx_ctrl4.bf.xgxs_fpat_3 |= 0x0100 ;
      }
      M2R_REG_WRITE(&xgxs->TxCntrl4.wrd, xgxs_tx_ctrl4.wrd) ;
    }
    break ;

    case M2R_XGXS_XAUI_LANE_ALL :
      CS_PRINT("m2r_xgxs_cfg_prbs_reg_data : lane parameter can't be M2R_XGXS_XAUI_LANE_ALL \n") ;
    break ;
  }
  return (CS_OK) ;
}

