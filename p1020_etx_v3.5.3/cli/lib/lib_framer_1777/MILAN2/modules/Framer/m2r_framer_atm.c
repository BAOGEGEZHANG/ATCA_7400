/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_atm.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_framer_priv.h"
#include "m2r_framer.h"
#include "m2r_framer_atm.h"

/****************************************************************/
/* $rtn_hdr_start   DROP ATM CELLS                              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_atm_drop_cells(cs_uint16 port_id,
                                     m2r_framer_stream_t channel,
                                     m2r_ctl_t  ctl, m2r_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the ATM Cell processor to drop all received cells.*/
/* When M2R_ENABLE is set for RX direction, no cells are passed */
/* to the datacom interface. And likewise when DROP is enabled  */
/* for the TX direction, no cells are accepted from the datacon */
/* interface.                                                   */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_boolean               rx_dir , tx_dir ;
  cs_uint32                str_start, str_end, str_iter;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;
  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
      if ( rx_dir ) {
          FR_STREAMRX_CellControl_t  rx_cellCtrl ;
          regaddr =
              (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd +
              (str_iter * M2R_FR_STREAMRX__STRIDE) ;

          rx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
          rx_cellCtrl.bf.DropCells = (ctl == M2R_ENABLE) ? 1 : 0 ; 
          M2R_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;
      }

    if ( tx_dir ) {
          FR_STREAMTX_CellControl_t tx_cellCtrl ;
          regaddr =
              (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_CellControl.wrd +
              (str_iter * M2R_FR_STREAMTX__STRIDE) ;

          tx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
          tx_cellCtrl.bf.Drop = (ctl == M2R_ENABLE) ? 1 : 0 ; 
          M2R_REG_WRITE(regaddr, tx_cellCtrl.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   FILTER IDLE CELLS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_atm_drop_idle(cs_uint16 port_id,
                        m2r_framer_stream_t channel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable / Disable the filtering of IDLE/unassigned cells .    */
/* When Enabled no idle/unassigned cells are passed onto system */
/* interface.                                                   */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    FR_STREAMRX_CellControl_t  rx_cellCtrl ;

    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd +
        (str_iter * M2R_FR_STREAMRX__STRIDE) ;

    rx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
    rx_cellCtrl.bf.DropIdle = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE ATM CELL HEC CHECK           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_atm_ctl_hec(cs_uint16 port_id,
                        m2r_framer_stream_t channel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disables the HEC processing by the ATM Cell processor*/
/* When the HEC check is disabled, cells coming from the datacom*/
/* interface no HEC is inserted. HEC thats coming is passed     */
/* along. On RX side, when the check is disabled no HEC checking*/
/* is done and the cells with HEC errors is also passed to      */
/* datacom interface.                                           */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                  sl_num ;
  M2R_t                    * pDev ;
  cs_uint32                  str_start, str_end, str_iter;
  volatile cs_reg          * regaddr ;
  FR_STREAMRX_CellControl_t  rx_cellCtrl ;
  FR_STREAMTX_CellControl_t  tx_cellCtrl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd +
        (str_iter * M2R_FR_STREAMRX__STRIDE) ;

    rx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
    rx_cellCtrl.bf.PassHEC = (ctl == M2R_DISABLE) ? 1 : 0 ; 
    M2R_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;

    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_CellControl.wrd +
        (str_iter * M2R_FR_STREAMTX__STRIDE) ;

    tx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
    tx_cellCtrl.bf.HECDis = (ctl == M2R_DISABLE) ? 1 : 0 ; 
    M2R_REG_WRITE(regaddr, tx_cellCtrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE (DE)SCRAMBLING OF CELL       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_atm_ctl_scrambling(cs_uint16 port_id,
                       m2r_framer_stream_t channel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disables the scrambling of the ATM cell payload.     */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                  sl_num ;
  M2R_t                    * pDev ;
  cs_uint32                  str_start, str_end, str_iter;
  volatile cs_reg          * regaddr ;
  FR_STREAMTX_CellControl_t  tx_cellCtrl ;
  FR_STREAMRX_CellControl_t  rx_cellCtrl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    regaddr =
      (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd +
      (str_iter * M2R_FR_STREAMRX__STRIDE) ;

    rx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
    rx_cellCtrl.bf.DisDScr = (ctl == M2R_ENABLE) ? 0 : 1 ;
    M2R_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;


    regaddr =
      (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_CellControl.wrd +
      (str_iter * M2R_FR_STREAMTX__STRIDE) ;

    tx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
    tx_cellCtrl.bf.DisScr = (ctl == M2R_ENABLE) ? 0 : 1 ;
    M2R_REG_WRITE(regaddr, tx_cellCtrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE HEC ERROR CORRECTION         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_atm_ctl_err_correction(cs_uint16 port_id,
                        m2r_framer_stream_t channel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disables the single bit HEC error correction.        */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                  sl_num ;
  M2R_t                    * pDev ;
  cs_uint32                  str_start, str_end, str_iter;
  volatile cs_reg          * regaddr ;
  FR_STREAMRX_CellControl_t  rx_cellCtrl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd +
        (str_iter * M2R_FR_STREAMRX__STRIDE) ;

    rx_cellCtrl.wrd = M2R_REG_READ(regaddr) ;
    rx_cellCtrl.bf.CorrHec = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   SET VARIOUS THRESHOLD VALUES FOR CELL FSM   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_atm_set_thr_val(cs_uint16 port_id,
                                      m2r_framer_stream_t channel,
                                      m2r_framer_atm_thr_t thr,
                                      cs_uint8  value)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o Threshold type                                */
/*              o Threshold value                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the variousthresolds for ATM Cell FSM.                  */
/*                                                              */
/* The [thr] parameter is specified as following:               */
/* M2R_FR_ATM_OOS_THR or M2R_FR_ATM_IS_THR or                   */
/* M2R_FR_ATM_ERR_CORR_THR                                      */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                  sl_num ;
  M2R_t                    * pDev ;
  cs_uint32                  str_start, str_end, str_iter;
  volatile cs_reg          * regaddr ;
  FR_STREAMRX_Cellfsm_t      cellFsm ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  if ((thr != M2R_FR_ATM_ERR_CORR_THR) && (value <= 1)) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_ID_OUT_OF_RANGE, NULL) ;
    return (CS_ERROR) ;
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.Cellfsm.wrd +
        (str_iter * M2R_FR_STREAMRX__STRIDE) ;

    cellFsm.wrd = M2R_REG_READ(regaddr) ;

    switch (thr) {
      case M2R_FR_ATM_OOS_THR :
        cellFsm.bf.ALPHA = value ;
      break ;

      case M2R_FR_ATM_IS_THR :
        cellFsm.bf.DELTA = value ;
      break ;

      case M2R_FR_ATM_ERR_CORR_THR :
        cellFsm.bf.GAMMA = value ;
      break ;
    }
    M2R_REG_WRITE(regaddr, cellFsm.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}




