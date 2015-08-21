/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_atm.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#include "cs_types.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_framer_priv.h"
#include "arn5_framer.h"
#include "arn5_framer_atm.h"

/****************************************************************/
/* $rtn_hdr_start   DROP ATM CELLS                              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_atm_drop_cells(cs_uint16 port_id,
                        cs_ctl_t  ctl, cs_dir_t dir)
/* INPUTS     : o Port Id                                       */

/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the ATM Cell processor to drop all received cells.*/
/* When CS_ENABLE is set for RX direction, no cells are passed  */
/* to the datacom interface. And likewise when DROP is enabled  */
/* for the TX direction, no cells are accepted from the datacon */
/* interface.                                                   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  ARN5_t                  * pDev ;
  cs_boolean               rx_dir , tx_dir ;
  volatile cs_reg        * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( rx_dir ) {
      ARN5_FR_STREAMRX_CellControl_t  rx_cellCtrl ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd ;

      rx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
      rx_cellCtrl.bf.DropCells = (ctl == CS_ENABLE) ? 1 : 0 ;
      CS_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;
  }

  if ( tx_dir ) {
        ARN5_FR_STREAMTX_CellControl_t tx_cellCtrl ;
        regaddr =
            (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_CellControl.wrd ;

        tx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
        tx_cellCtrl.bf.Drop = (ctl == CS_ENABLE) ? 1 : 0 ;
        CS_REG_WRITE(regaddr, tx_cellCtrl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   FILTER IDLE CELLS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_atm_drop_idle(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable / Disable the filtering of IDLE/unassigned cells .    */
/* When Enabled no idle/unassigned cells are passed onto system */
/* interface.                                                   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMRX_CellControl_t  rx_cellCtrl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr =
      (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd ;

  rx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
  rx_cellCtrl.bf.DropIdle = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE ATM CELL HEC CHECK           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_atm_ctl_hec(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disables the HEC processing by the ATM Cell processor*/
/* When the HEC check is disabled, cells coming from the datacom*/
/* interface no HEC is inserted. HEC thats coming is passed     */
/* along. On RX side, when the check is disabled no HEC checking*/
/* is done and the cells with HEC errors is also passed to      */
/* datacom interface.                                           */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMRX_CellControl_t  rx_cellCtrl ;
  ARN5_FR_STREAMTX_CellControl_t  tx_cellCtrl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd ;

  rx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
  rx_cellCtrl.bf.PassHEC = (ctl == CS_DISABLE) ? 1 : 0 ;
  CS_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_CellControl.wrd ;

  tx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
  tx_cellCtrl.bf.HECDis = (ctl == CS_DISABLE) ? 1 : 0 ;
  CS_REG_WRITE(regaddr, tx_cellCtrl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE (DE)SCRAMBLING OF CELL       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_atm_ctl_scrambling(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disables the scrambling of the ATM cell payload.     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMTX_CellControl_t  tx_cellCtrl ;
  ARN5_FR_STREAMRX_CellControl_t  rx_cellCtrl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr =
    (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd ;

  rx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
  rx_cellCtrl.bf.DisDScr = (ctl == CS_ENABLE) ? 0 : 1 ;
  CS_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;


  regaddr =
    (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_CellControl.wrd ;

  tx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
  tx_cellCtrl.bf.DisScr = (ctl == CS_ENABLE) ? 0 : 1 ;
  CS_REG_WRITE(regaddr, tx_cellCtrl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE HEC ERROR CORRECTION         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_atm_ctl_err_correction(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disables the single bit HEC error correction.        */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMRX_CellControl_t  rx_cellCtrl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_CellControl.wrd ;

  rx_cellCtrl.wrd = CS_REG_READ(regaddr) ;
  rx_cellCtrl.bf.CorrHec = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(regaddr, rx_cellCtrl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET VARIOUS THRESHOLD VALUES FOR CELL FSM   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_atm_set_thr_val(cs_uint16 port_id,
                                      arn5_framer_atm_thr_t thr,
                                      cs_uint8  value)
/* INPUTS     : o Port Id                                       */
/*              o Threshold type                                */
/*              o Threshold value                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the variousthresolds for ATM Cell FSM.                  */
/*                                                              */
/* The [thr] parameter is specified as following:               */
/* ARN5_FR_ATM_OOS_THR or ARN5_FR_ATM_IS_THR or                 */
/* ARN5_FR_ATM_ERR_CORR_THR                                     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMRX_Cellfsm_t      cellFsm ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if ((thr != ARN5_FR_ATM_ERR_CORR_THR) && (value <= 1)) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_ID_OUT_OF_RANGE, NULL) ;
    return (CS_ERROR) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.Cellfsm.wrd ;

  cellFsm.wrd = CS_REG_READ(regaddr) ;

  switch (thr) {
    case ARN5_FR_ATM_OOS_THR :
      cellFsm.bf.ALPHA = value ;
    break ;

    case ARN5_FR_ATM_IS_THR :
      cellFsm.bf.DELTA = value ;
    break ;

    case ARN5_FR_ATM_ERR_CORR_THR :
      cellFsm.bf.GAMMA = value ;
    break ;
  }
  CS_REG_WRITE(regaddr, cellFsm.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


