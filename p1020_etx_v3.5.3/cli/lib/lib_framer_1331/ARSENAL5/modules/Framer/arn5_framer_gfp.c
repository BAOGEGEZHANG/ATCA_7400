/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_gfp.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#include "cs_types.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_framer.h"
#include "arn5_framer_priv.h"
#include "arn5_framer_gfp.h"

/****************************************************************/
/* $rtn_hdr_start   CONFIGURE THE GFP MODE                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_cfg_mode(cs_uint16 port_id,
                        arn5_framer_gfp_mode_t mode)
/* INPUTS     : o Port Id                                       */
/*            : o GFP Mode ( Transparent or Normal )            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure GFP mode. In transparent mode, packet coming from  */
/* the HOST side is expected to carry core-header, payload hdr  */
/* and HECs ( core-hec ) and THEC. In NORMAL MODE, the framer   */
/* GFP mapper, will take a raw packet and put the GFP header    */
/* core-header, payload hdr and HECs ( cHEC , tHEC and FCS ).   */
/*                                                              */
/* NOTE : By default GFP mapper is in store-and-forward mode.   */
/* It can be configured in cut-though mode using                */
/* arn5_framer_gfp_set_cut_through_mode() API. When configuring */
/* in cut-through mode, ARN5_FR_GFP_HDR_OP_CHDR and             */
/* ARN5_FR_GFP_HDR_OP_THDR insertion should be disabled by using*/
/* arn5_framer_gfp_ctl_hdr_opr() API.                           */
/*                                                              */
/* The [mode] parameter is specified as following:              */
/* ARN5_FR_GFP_TRANSPARENT(0) or ARN5_FR_GFP_NORMAL (1)         */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  arn5_framer_gfp_ctl_hdr_opr(port_id,
                             ARN5_FR_GFP_HDR_OP_HDR_OP_ALL,
                             CS_RX_AND_TX,
                           ((mode == ARN5_FR_GFP_NORMAL) ?
                             CS_ENABLE : CS_DISABLE) ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE CUT-THROUGH MODE             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_set_cut_through_mode(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable cut through mode.                             */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_TX_GFP_Control_0_t gfp_ctrl0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;
  gfp_ctrl0.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;

  if (ctl == CS_ENABLE) {
    if (gfp_ctrl0.bf.Gen_Pld_Header) {
      gfp_ctrl0.bf.Gen_Pld_Header = 0 ;
    }
  }
  gfp_ctrl0.bf.Enable_Cut_Through = (ctl == CS_ENABLE) ? 1 : 0 ;

  CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
               gfp_ctrl0.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET THE GFP PAYLAOD HEADER                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_set_tx_pld_hdr(cs_uint16 port_id,
                                         cs_uint16  pld_hdr)
/* INPUTS     : o Port Id                                       */
/*              o 2 byte paylaod header                         */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the GFP paylaod header. This payload header is appended */
/* onto the GFP TX frame when in NORMAL mode.                   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_TX_GFP_Payload_Hdr_t   payload_hdr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  /* Store the GFP payload header that will be put in the GFP frame */
  payload_hdr.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Payload_Hdr.wrd) ;
  payload_hdr.wrd = pld_hdr ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Payload_Hdr.wrd,
               payload_hdr.wrd) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PAYLOAD HDR PROCESSING ON RX */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_pld_processing(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE(1) or CS_DISABLE(0)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the processing of Paylaod header in the       */
/* received GFP Frame.                                          */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl1.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Dis_Payload_Hdr_Proc = (ctl == CS_ENABLE)  ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET SYNC THRESHOLD VALUE                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_set_sync_thr(cs_uint16 port_id, cs_uint8 val)
/* INPUTS     : o Port Id                                       */
/*              o Threshold value                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the Sync threshold (Delta) value. Delta is the number of */
/* good core HEC Frames before GFP Frame delineation state      */
/* machine transitions from HUNT State to SYNC State on RX.     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_0_t     rx_ctrl0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl0.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                           gfp_rx_GFP_Control_0.wrd) ;
  rx_ctrl0.bf.GFP_Delta = (val & 0x07) ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
               rx_ctrl0.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET OUT OF SYNC THRESHOLD VALUE             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_set_oos_thr(cs_uint16 port_id, cs_uint8 val)
/* INPUTS     : o Port Id                                       */
/*              o Threshold value                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the Out-of Sync threshold (Alpha) value. This is the     */
/* number of consecutive single bit errors frames before GFP    */
/* Frame delineation state machine transitions from SYNC State  */
/* to HUNT State on RX.                                         */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_0_t     rx_ctrl0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl0.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                           gfp_rx_GFP_Control_0.wrd) ;
  rx_ctrl0.bf.GFP_Alpha = (val & 0x07) ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
               rx_ctrl0.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE CORE HEADER (DE)SCRAMBLING   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_chdr_scrambling(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disable scrambling on TX Side and descrambling on RX */
/* side of the core header PLI field.                           */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
    ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

    rx_ctrl1.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                             GFP_Control_1.wrd) ;
    rx_ctrl1.bf.GFP_Dis_Header_XOR = (ctl == CS_DISABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
                 rx_ctrl1.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE PASSING OF CLIENT MGMT FRAME   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_pass_mgmt_frame(cs_uint16 port_id,
                                              cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable passing of client management frames to HOST.  */
/* When Enabled client management frames coming from the line   */
/* side are passed to the HOST else when disabled they are      */
/* dropped.                                                     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl1.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Pass_CM_Frame = (ctl == CS_ENABLE)  ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start CONTROL GFP FRAME HEADER OPERATIONS           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_hdr_opr(cs_uint16 port_id,
                        arn5_framer_gfp_hdr_oper_t field,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o GFP HEADRER FIELD TYPE                        */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable insertion and deletion of fields in GFP Frame.*/
/* This API controls the insertion/deletion of various fields   */
/* like core header, core HEC , paylaod header , payload HEC ,  */
/* and FCS in/from GFP Frame.                                   */
/* When enabled for TX direction, it insertes the field         */
/* specified the [field] parameter. For example if [field] is   */
/* ARN5_FR_GFP_HDR_OP_FCS, GFP mapper will insert FCS to the    */
/* frame coming from the HOST.                                  */
/* When ENABLED for RX direction, GFP mapper will strip the     */
/* header field specified in [field] parameter. For example     */
/* if the field specified were ARN5_FR_GFP_HDR_OP_FCS, GFP mapper*/
/* will strip the FCS from the received GFP Frame and no FCS    */
/* will be passed onto the HOST.                                */
/*                                                              */
/* The parameter [field] can be one of the following :          */
/* ARN5_FR_GFP_HDR_OP_FCS or  ARN5_FR_GFP_HDR_OP_CHDR  or       */
/* ARN5_FR_GFP_HDR_OP_CHEC or ARN5_FR_GFP_HDR_OP_THDR or        */
/* ARN5_FR_GFP_HDR_OP_THEC  or  ARN5_FR_GFP_HDR_OP_HDR_OP_ALL   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  cs_uint8              bfRX, bfTX ;
  ARN5_FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;
  ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  bfRX = (ctl == CS_ENABLE) ? 0 : 1 ;
  bfTX = (ctl == CS_ENABLE) ? 1 : 0 ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
    tx_ctrl0.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;

    if (field & ARN5_FR_GFP_HDR_OP_FCS) {
      tx_ctrl0.bf.Gen_CRC = bfTX ;
    }

    if (field & ARN5_FR_GFP_HDR_OP_THDR) {
      tx_ctrl0.bf.Gen_Pld_Header    = bfTX ;
    }

    if (field & ARN5_FR_GFP_HDR_OP_THEC) {
      tx_ctrl0.bf.Gen_THEC          = bfTX ;
    }

    CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }

  if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
    rx_ctrl1.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;

    if (field & ARN5_FR_GFP_HDR_OP_FCS) {
      rx_ctrl1.bf.GFP_Pass_FCS = bfRX ;
    }

    if ((field & ARN5_FR_GFP_HDR_OP_CHDR) ||
        (field & ARN5_FR_GFP_HDR_OP_CHEC)) {
      rx_ctrl1.bf.GFP_Pass_Core_Hdr = bfRX ;
    }

    if ((field & ARN5_FR_GFP_HDR_OP_THDR) ||
        (field & ARN5_FR_GFP_HDR_OP_THEC)) {
      rx_ctrl1.bf.GFP_Send_Payload_Hdr  = bfRX ;
    }

    CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
                 rx_ctrl1.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE UPI CHECKING                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_upi_chk(cs_uint16 port_id,
                        cs_uint8 upi, cs_boolean chk_upi)
/* INPUTS     : o Port Id                                       */
/*              o Expected UPI value                            */
/*              o TRUE ( Check UPI field ) else FALSE           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disable the checking of the UPI field in the RX      */
/* stream. parameter [upi] specifies the expected UPI if        */
/* chk_upi is TRUE. If [chk_upi] is FALSE, UPI checking is      */
/* disabled.                                                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_2_t     rx_ctrl2 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl2.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_2.wrd) ;

  if ( chk_upi ) {
    rx_ctrl2.bf.GFP_Exp_UPI = upi ;
  }
  rx_ctrl2.bf.GFP_Dis_Exp_UPI = (chk_upi) ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_2.wrd,
               rx_ctrl2.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PTI CHECKING                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_pti_chk(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( Check PTI field ) else FALSE           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disable the checking of the PTI field in the RX      */
/* frame.                                                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl1.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Dis_PTI = (ctl == CS_ENABLE)  ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   GET THE STATUS OF THE GFP STREAM            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_uint8 arn5_framer_gfp_get_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the current RX status for the GFP stream.                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  cs_uint8              rx_status ;
  ARN5_FR_GFP_RX_GFP_Status_t        status ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  status.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Status.wrd) ;
  rx_status = status.bf.Sync_Err_S;

  return (rx_status) ;
}


/****************************************************************/
/* $rtn_hdr_start   DROP LONG GFP PACKETS                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_drop_long(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable dropping of packets which are larger than     */
/* configured MAX packet size.                                  */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  cs_boolean            rx_dir , tx_dir ;
  ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;
  ARN5_FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( rx_dir ) {
    rx_ctrl1.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
    rx_ctrl1.bf.GFP_Drop_Long_pkts = (ctl == CS_ENABLE)  ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
                 rx_ctrl1.wrd) ;
  }

  if ( tx_dir ) {
    tx_ctrl0.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;
    tx_ctrl0.bf.Drop_Long_pkts  = (ctl == CS_ENABLE)  ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   DROP SHORT GFP PACKETS                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_drop_short(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable dropping of packets which are shorter than    */
/* configured MIN packet size.                                  */
/* respectively.                                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  cs_boolean            rx_dir , tx_dir ;
  ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;
  ARN5_FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( rx_dir ) {
    rx_ctrl1.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
    rx_ctrl1.bf.GFP_Drop_Short_pkts = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
                 rx_ctrl1.wrd) ;
  }

  if ( tx_dir ) {
    tx_ctrl0.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;
    tx_ctrl0.bf.Drop_Short_pkts = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE (DE) SCRAMBLING              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_scrambling(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE OR CS_DISABLE                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the (de) scrambling                           */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;
  ARN5_FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl1.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Dis_Descrambler = (ctl == CS_ENABLE)  ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  tx_ctrl0.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;
  tx_ctrl0.bf.Dis_Scrambler = (ctl == CS_ENABLE)  ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
               tx_ctrl0.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE 1 BIT ERROR CORRECTION       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_gfp_ctl_err_correction(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable correction of single bit errors.              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_GFP_RX_GFP_Control_0_t     rx_ctrl0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rx_ctrl0.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                           gfp_rx_GFP_Control_0.wrd) ;
  rx_ctrl0.bf.GFP_Dis_1bit_Err_Corr = (ctl == CS_DISABLE) ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
               rx_ctrl0.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   FRAMER GFP block reset                      */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_ctl_gfp_stream_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable GFP block soft reset.                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  cs_uint8              bit_rst ;
  cs_boolean            rx_dir , tx_dir ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == CS_DISABLE) ? 0 : 1 ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( tx_dir ) {
    ARN5_FR_GFP_TX_GFP_Control_0_t tx_rst ;
    tx_rst.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                             gfp_tx_GFP_Control_0.wrd) ;
    tx_rst.bf.Reset = bit_rst ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_rst.wrd) ;
  }

  if ( rx_dir ) {
    ARN5_FR_GFP_RX_GFP_Control_0_t    rx_rst ;
    rx_rst.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                             gfp_rx_GFP_Control_0.wrd) ;
    rx_rst.bf.GFP_Reset = bit_rst ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
                 rx_rst.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}

