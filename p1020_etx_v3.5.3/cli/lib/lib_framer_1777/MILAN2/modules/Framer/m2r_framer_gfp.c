/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_gfp.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_framer.h"
#include "m2r_framer_priv.h"
#include "m2r_framer_gfp.h"

/****************************************************************/
/* $rtn_hdr_start   CONFIGURE THE GFP MODE                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_cfg_mode(cs_uint16 port_id,
                                   m2r_framer_gfp_mode_t mode)
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
/* m2r_framer_gfp_set_cut_through_mode() API. When configuring  */
/* in cut-through mode, M2R_FR_GFP_HDR_OP_CHDR and              */
/* M2R_FR_GFP_HDR_OP_THDR insertion should be disabled by using */
/* m2r_framer_gfp_ctl_hdr_opr() API.                            */
/*                                                              */
/* The [mode] parameter is specified as following:              */
/* M2R_FR_GFP_TRANSPARENT(0) or M2R_FR_GFP_NORMAL (1)           */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_framer_gfp_ctl_hdr_opr(port_id,
                             M2R_FR_GFP_HDR_OP_HDR_OP_ALL,
                             M2R_RX_AND_TX,
                           ((mode == M2R_FR_GFP_NORMAL) ?
                             M2R_ENABLE : M2R_DISABLE) ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE CUT-THROUGH MODE             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_set_cut_through_mode(cs_uint16 port_id,
                                               m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable cut through mode.                             */
/* When cut-through mode is enabled, GFP mapper's               */
/* M2R_FR_GFP_HDR_OP_CHDR and  M2R_FR_GFP_HDR_OP_THDR are       */
/* DISABLED on TX i.e. Core-header and type header are not      */
/* inserted by the mapper and it is expected HOST will present  */
/* the frame to the GFP mapper with core-header and type header */
/* in there. Core HEC and Type HEC can still be inserted by the */
/* mapper.                                                      */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                 sl_num ;
  M2R_t                   * pDev ;
  FR_GFP_TX_GFP_Control_0_t gfp_ctrl0 ;
  FR_GFP_TX_GFP_Control_0_t tx_ctrl0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if (ctl == M2R_ENABLE) {
    tx_ctrl0.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;

    if (tx_ctrl0.bf.Gen_Core_Header) {
      tx_ctrl0.bf.Gen_Core_Header = 0 ;
    }

    if (tx_ctrl0.bf.Gen_Pld_Header) {
      tx_ctrl0.bf.Gen_Pld_Header = 0 ;
    }

    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }

  gfp_ctrl0.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
                              gfp_tx_GFP_Control_0.wrd) ;

  gfp_ctrl0.bf.Enable_Cut_Through = (ctl == M2R_ENABLE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
               gfp_ctrl0.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   SET THE GFP PAYLAOD HEADER                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_set_tx_pld_hdr(cs_uint16 port_id,
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
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_TX_GFP_Payload_Hdr_t   payload_hdr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  /* Store the GFP payload header that will be put in the GFP frame */
  payload_hdr.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Payload_Hdr.wrd) ;
  payload_hdr.wrd = pld_hdr ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Payload_Hdr.wrd,
               payload_hdr.wrd) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PAYLOAD HDR PROCESSING ON RX */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_pld_processing(cs_uint16 port_id,
                                             m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE(1) or M2R_DISABLE(0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the processing of Paylaod header in the       */
/* received GFP Frame.                                          */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl1.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Dis_Payload_Hdr_Proc = (ctl == M2R_ENABLE)  ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   SET SYNC THRESHOLD VALUE                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_set_sync_thr(cs_uint16 port_id, cs_uint8 val)
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
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_0_t     rx_ctrl0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl0.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
                           gfp_rx_GFP_Control_0.wrd) ;
  rx_ctrl0.bf.GFP_Delta = (val & 0x07) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
               rx_ctrl0.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   SET OUT OF SYNC THRESHOLD VALUE             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_set_oos_thr(cs_uint16 port_id, cs_uint8 val)
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
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_0_t     rx_ctrl0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl0.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
                           gfp_rx_GFP_Control_0.wrd) ;
  rx_ctrl0.bf.GFP_Alpha = (val & 0x07) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
               rx_ctrl0.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE CORE HEADER (DE)SCRAMBLING   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_chdr_scrambling(cs_uint16 port_id,
                                              m2r_dir_t dir,
                                              m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disable scrambling on TX Side and descrambling on RX */
/* side of the core header PLI field. (De)Scrambling performed  */
/* is logical XOR with 0xB6AB31E0.                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
    FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

    rx_ctrl1.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
                             gfp_rx_GFP_Control_0.wrd) ;
    rx_ctrl1.bf.GFP_Dis_Header_XOR = (ctl == M2R_DISABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
                 rx_ctrl1.wrd) ;
  }

  if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
    FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

    tx_ctrl0.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;
    tx_ctrl0.bf.Dis_Idle_XOR = (ctl == M2R_DISABLE)  ? 1 : 0 ;
    tx_ctrl0.bf.Dis_Header_XOR = (ctl == M2R_DISABLE)  ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE PASSING OF CLIENT MGMT FRAME   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_pass_mgmt_frame(cs_uint16 port_id,
                                              m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
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
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl1.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Pass_CM_Frame = (ctl == M2R_ENABLE)  ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start CONTROL GFP FRAME HEADER OPERATIONS           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_hdr_opr(cs_uint16 port_id,
                                   m2r_framer_gfp_hdr_oper_t field,
                                   m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o GFP HEADRER FIELD TYPE                        */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable insertion and deletion of fields in GFP Frame.*/
/* This API controls the insertion/deletion of various fields   */
/* like core header, core HEC , paylaod header , payload HEC ,  */
/* and FCS in/from GFP Frame.                                   */
/* When enabled for TX direction, it insertes the field         */
/* specified the [field] parameter. For example if [field] is   */
/* M2R_FR_GFP_HDR_OP_FCS, GFP mapper will insert FCS to the     */
/* frame coming from the HOST.                                  */
/* When ENABLED for RX direction, GFP mapper will strip the     */
/* header field specified in [field] parameter. For example     */
/* if the field specified were M2R_FR_GFP_HDR_OP_FCS, GFP mapper*/
/* will strip the FCS from the received GFP Frame and no FCS    */
/* will be passed onto the HOST.                                */
/*                                                              */
/* The parameter [field] can be one of the following :          */
/* M2R_FR_GFP_HDR_OP_FCS or  M2R_FR_GFP_HDR_OP_CHDR  or         */
/* M2R_FR_GFP_HDR_OP_CHEC or M2R_FR_GFP_HDR_OP_THDR or          */
/* M2R_FR_GFP_HDR_OP_THEC  or  M2R_FR_GFP_HDR_OP_HDR_OP_ALL     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  cs_uint8                      bfRX, bfTX ;
  FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;
  FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  bfRX = (ctl == M2R_ENABLE) ? 0 : 1 ;
  bfTX = (ctl == M2R_ENABLE) ? 1 : 0 ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
    tx_ctrl0.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;

    if (field & M2R_FR_GFP_HDR_OP_FCS) {
      tx_ctrl0.bf.Gen_CRC = bfTX ;
    }

    if (field & M2R_FR_GFP_HDR_OP_CHDR) {
      tx_ctrl0.bf.Gen_Core_Header = bfTX ;
    }

    if (field & M2R_FR_GFP_HDR_OP_CHEC) {
      tx_ctrl0.bf.Gen_Core_HEC      = bfTX ;
    }

    if (field & M2R_FR_GFP_HDR_OP_THDR) {
      tx_ctrl0.bf.Gen_Pld_Header    = bfTX ;
    }

    if (field & M2R_FR_GFP_HDR_OP_THEC) {
      tx_ctrl0.bf.Gen_THEC          = bfTX ;
    }

    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }

  if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
    rx_ctrl1.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;

    if (field & M2R_FR_GFP_HDR_OP_FCS) {
      rx_ctrl1.bf.GFP_Pass_FCS = bfRX ;
    }

    if ((field & M2R_FR_GFP_HDR_OP_CHDR) ||
        (field & M2R_FR_GFP_HDR_OP_CHEC)) {
      rx_ctrl1.bf.GFP_Pass_Core_Hdr = bfRX ;
    }

    if ((field & M2R_FR_GFP_HDR_OP_THDR) ||
        (field & M2R_FR_GFP_HDR_OP_THEC)) {
      rx_ctrl1.bf.GFP_Send_Payload_Hdr  = bfRX ;
    }

    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
                 rx_ctrl1.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE UPI CHECKING                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_upi_chk(cs_uint16 port_id,
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
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_2_t     rx_ctrl2 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl2.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_2.wrd) ;

  if ( chk_upi ) {
    rx_ctrl2.bf.GFP_Exp_UPI = upi ;
  }
  rx_ctrl2.bf.GFP_Dis_Exp_UPI = (chk_upi) ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_2.wrd,
               rx_ctrl2.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PTI CHECKING                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_pti_chk(cs_uint16 port_id,
                                      m2r_ctl_t ctl)
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
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl1.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Dis_PTI = (ctl == M2R_ENABLE)  ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   GET THE STATUS OF THE GFP STREAM            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_uint8 m2r_framer_gfp_get_status(cs_uint16 port_id,
                                   m2r_framer_gfp_rx_status_t sel)
/* INPUTS     : o Port Id                                       */
/*              o GFP Status type                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the current RX status for the GFP stream.                */
/* The [sel] parameter is specified as following:               */
/* M2R_GFP_SYNC_STATUS or M2R_GFP_LOSS_CLIENT or                */
/* M2R_GFP_LOSS_SYNC                                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  cs_uint8                      rx_status ;
  FR_GFP_RX_GFP_Status_t        status ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  status.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Status.wrd) ;

  switch ( sel ) {
      case M2R_GFP_LOSS_CLIENT:
          rx_status = status.bf.Loss_Client_S ;
          break;
      case M2R_GFP_LOSS_SYNC:
          rx_status = status.bf.Loss_Sync_S ;
          break;
      case M2R_GFP_SYNC_STATUS:
      default:
          rx_status = status.bf.Sync_Err_S;
          break;
  }

  return (rx_status) ;
}

/****************************************************************/
/* $rtn_hdr_start   DROP LONG GFP PACKETS                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_drop_long(cs_uint16 port_id,
                                    m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable dropping of packets which are larger than     */
/* configured MAX packet size.                                  */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  cs_boolean                    rx_dir , tx_dir ;
  FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;
  FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if ( rx_dir ) {
    rx_ctrl1.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
    rx_ctrl1.bf.GFP_Drop_Long_pkts = (ctl == M2R_ENABLE)  ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
                 rx_ctrl1.wrd) ;
  }

  if ( tx_dir ) {
    tx_ctrl0.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;
    tx_ctrl0.bf.Drop_Long_pkts  = (ctl == M2R_ENABLE)  ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }
  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   DROP SHORT GFP PACKETS                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_drop_short(cs_uint16 port_id,
                                     m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable dropping of packets which are shorter than    */
/* configured MIN packet size.                                  */
/* respectively.                                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  cs_boolean                    rx_dir , tx_dir ;
  FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;
  FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if ( rx_dir ) {
    rx_ctrl1.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
    rx_ctrl1.bf.GFP_Drop_Short_pkts = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
                 rx_ctrl1.wrd) ;
  }

  if ( tx_dir ) {
    tx_ctrl0.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;
    tx_ctrl0.bf.Drop_Short_pkts = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_ctrl0.wrd) ;
  }
  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE (DE) SCRAMBLING              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_scrambling(cs_uint16 port_id,
                                         m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE OR M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the (de) scrambling                           */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_1_t     rx_ctrl1 ;
  FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl1.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd) ;
  rx_ctrl1.bf.GFP_Dis_Descrambler = (ctl == M2R_ENABLE)  ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GFP_Control_1.wrd,
               rx_ctrl1.wrd) ;

  tx_ctrl0.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;
  tx_ctrl0.bf.Dis_Scrambler = (ctl == M2R_ENABLE)  ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
               tx_ctrl0.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE 1 BIT ERROR CORRECTION       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_gfp_ctl_err_correction(cs_uint16 port_id,
                                       m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable correction of single bit errors.              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_GFP_RX_GFP_Control_0_t     rx_ctrl0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rx_ctrl0.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
                           gfp_rx_GFP_Control_0.wrd) ;
  rx_ctrl0.bf.GFP_Dis_1bit_Err_Corr = (ctl == M2R_DISABLE) ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
               rx_ctrl0.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   FRAMER GFP block reset                      */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_ctl_gfp_stream_sreset(cs_uint16 port_id,
                                      m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable GFP block soft reset.                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint8                 bit_rst ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == M2R_DISABLE) ? 0 : 1 ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if ( tx_dir ) {
    FR_GFP_TX_GFP_Control_0_t tx_rst ;
    tx_rst.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
                             gfp_tx_GFP_Control_0.wrd) ;
    tx_rst.bf.Reset = bit_rst ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd,
                 tx_rst.wrd) ;
  }

  if ( rx_dir ) {
    FR_GFP_RX_GFP_Control_0_t    rx_rst ;
    rx_rst.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
                             gfp_rx_GFP_Control_0.wrd) ;
    rx_rst.bf.GFP_Reset = bit_rst ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.gfp_rx_GFP_Control_0.wrd,
                 rx_rst.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

