/***********************************************************************/
/* This file contains unpublished documentation and software           */ 
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif_xgxs
 *
 * Milan driver libraries pertaining to LIF XGXS block.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_registers.h"
#include "m2r_public.h"
#include "m2r_lif_xgxs.h"

/************************************************************************/
/*    Macros and defines                                                */
/************************************************************************/
#define M2R_LIF_GET_DEV_BASEADDR(PORT_ID, BASE_ADDR, ERR_CODE)      \
{                                                                   \
  cs_uint32     err_code ;                                          \
  if ( !(M2R_IS_PORT_VALID(PORT_ID, err_code)) ) {                  \
     CS_HNDL_ERROR(port_id, err_code, NULL) ;                       \
     return (ERR_CODE) ;                                            \
  }                                                                 \
  if(M2R_IS_SONET_MODE((PORT_ID)) == TRUE) {                        \
    CS_HNDL_ERROR((PORT_ID), EM2R_INVALID_IN_SONET_MODE,            \
                  "XGXS is powered DOWN in SONET Mode\n") ;         \
     return (ERR_CODE) ;                                            \
  }                                                                 \
  (BASE_ADDR) = M2R_PORT_ID_TO_DEV_BASE(port_id);                   \
}

/************************************************************************/
/* $rtn_hdr_start  GET XGXS CLOCK STATUS                                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_xgxs_get_clk_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Return the Clock status. If either RX or TX Clock is NOT OK, CS_ERROR*/
/* is returned. If Both the Clocks are OK, CS_OK is returned.           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  return m2r_xgxs_get_clk_status(port_id, M2R_ID_LIF, &pDev->LIF.XGXS) ;
}

/************************************************************************/
/* $rtn_hdr_start   GET  XGXS STATUS                                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_uint16 m2r_lif_xgxs_get_status(cs_uint16 port_id, m2r_xgxs_status_t sel)
/* INPUTS     : o Port Id                                               */
/*              o Status Type selection                                 */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Return the status of the XGXS block.                                 */
/* The [sel] parameter is specified as one of the following:            */
/* M2R_XGXS_LANE_LOCK_DET or M2R_XGXS_LANE_DATA_DET or M2R_XGXS_LANE_CLOCK*/
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  return m2r_xgxs_get_status(port_id, M2R_ID_LIF, &pDev->LIF.XGXS, sel);
}


/************************************************************************/
/* $rtn_hdr_start     CHECK XGXS ALIGNMENT STATUS                       */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_boolean m2r_lif_xgxs_is_align_detected(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : TRUE(aligned) or FALSE(not-aligned)                     */
/* DESCRIPTION:                                                         */
/* Return the alignment status of XGXS(Rx).                             */
/* A (word) alignment detction implies that each of the 4 lanes are     */
/* good(ie. lock, clock, data, and sync are detected) and so can be     */
/* used as a status indication for the XGXS interface.                  */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  return ( m2r_xgxs_is_align_detected(port_id, M2R_ID_LIF, &pDev->XGXS) ) ; 
} 


/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE POINTER ADJUST IN XAUI I/F            */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_ctl_pointer_adj(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable pointer adjust in XAUI interface for signal detection.*/
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  return m2r_xgxs_ctl_pointer_adj(port_id, M2R_ID_LIF, &pDev->LIF.XGXS, ctl) ;
}

/************************************************************************/
/* $rtn_hdr_start  RE-INIT ELASTIC STORE                                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_xgxs_reset_estore(cs_uint16 port_id )
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Re-initialise the elastic FIFO ( also called e-store). This will     */
/* re-initialise the read pointers for all the Lanes of XAUI.           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  return m2r_xgxs_reset_estore(port_id, M2R_ID_LIF, &pDev->LIF.XGXS) ;
}

/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PRBS                                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_xgxs_ctl_prbs(cs_uint16 port_id, m2r_ctl_t ctl,
                                m2r_xgxs_prbs_t pattern)
/* INPUTS     : o Port Id                                               */
/*              o control (M2R_ENABLE or M2R_DISABLE)                   */
/*              o PRBS PATTERN                                          */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable XGXS PRBS.                                            */
/* The [pattern] is one of the following :                              */
/* M2R_XGXS_REG_PATTERN or M2R_XGXS_HFPAT or M2R_XGXS_LFPAT or          */
/* M2R_XGXS_MFPAT or M2R_XGXS_CRPAT_0 or M2R_XGXS_CRPAT_1 or            */
/* M2R_XGXS_CJPAT                                                       */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  return m2r_xgxs_ctl_prbs(port_id, M2R_ID_LIF, &pDev->LIF.XGXS,
                           M2R_XGXS_XAUI_LANE_ALL,pattern, ctl) ;
}

/************************************************************************/
/* $rtn_hdr_start  INJECT ERRORS IN PRBS STREAM                         */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_xgxs_inj_error(cs_uint16 port_id, m2r_xgxs_xaui_lane_t lane)
/* INPUTS     : o Port Id                                               */
/*              o XAUI lane ID                                          */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Injects error in the PRBS stream on the lane specified.              */
/* The parameter [lane] can be one of the following:                    */
/* M2R_XGXS_XAUI_LANE0 or M2R_XGXS_XAUI_LANE1 or M2R_XGXS_XAUI_LANE2 or */
/* M2R_XGXS_XAUI_LANE3 or M2R_XGXS_XAUI_LANE_ALL                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  return m2r_xgxs_inj_error(port_id, M2R_ID_LIF, &pDev->LIF.XGXS, lane) ;
}

/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE DATA DETECT FOR LOCK DETECT STATE     */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_ctl_data_detect(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable Data detect status for Lock detect.                   */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  return m2r_xgxs_ctl_data_det_for_lock_det(port_id, M2R_ID_LIF,
                                            &pDev->LIF.XGXS, ctl) ;
}

/************************************************************************/
/* $rtn_hdr_start  CONFIGURE THE REGISTER BASED PRBS PATTERN            */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_xgxs_cfg_prbs_reg_data(cs_uint16 port_id,
                                         m2r_xgxs_xaui_lane_t lane,
                                         cs_boolean ctrl_data, cs_uint8 data)
/* INPUTS     : o Port Id                                               */
/*              o XAUI Lane ID                                          */
/*              o TRUE ( CONTROL DATA ) or FALSE ( NORMAL DATA )        */
/*              o Data Byte                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Configure the register based pattern to be sent on individual lanes. */
/* The parameter [lane] can be one of the following:                    */
/* M2R_XGXS_XAUI_LANE0 or M2R_XGXS_XAUI_LANE1 or M2R_XGXS_XAUI_LANE2 or */
/* M2R_XGXS_XAUI_LANE3 or M2R_XGXS_XAUI_LANE_ALL                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  return m2r_xgxs_cfg_prbs_reg_data(port_id, M2R_ID_LIF, &pDev->LIF.XGXS,
                                    lane, ctrl_data, data) ;
}

