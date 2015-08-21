/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_pkt.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#include "cs_types.h"
#include "arn5_registers.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_framer.h"
#include "arn5_framer_priv.h"
#include "arn5_framer_pkt.h"

/****************************************************************/
/* $rtn_hdr_start   CRC16 or CRC32 SELECTION                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_ctl_crc32(cs_uint16 port_id,
                        cs_boolean crc32)
/* INPUTS     : o Port Id                                       */
/*              o TRUE if CRC32, FALSE if CRC16(default)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selects whether CRC32 or CRC16 is appended/expected in       */
/* frame. By default, CRC16 is used.                            */
/* This API applies to both TX and RX both                      */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_uint8              bit_crc32 ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMTX_PktControl_t         tx_pktctl ;
  ARN5_FR_STREAMRX_PktControl_t         rx_pktctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  bit_crc32 = (crc32) ? 1 : 0 ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  /* RX Pkt/cell control */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
  rx_pktctl.wrd = CS_REG_READ(regaddr) ;
  rx_pktctl.bf.crc32 = bit_crc32 ;
  CS_REG_WRITE(regaddr, rx_pktctl.wrd) ;

  /* TX Pkt/cell control */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd ;
  tx_pktctl.wrd = CS_REG_READ(regaddr) ;
  tx_pktctl.bf.crc32 = bit_crc32 ;
  CS_REG_WRITE(regaddr, tx_pktctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONTROL FRAMER FCS CHECKING/INSERTION       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_ctl_fcs(cs_uint16 port_id,
                        cs_boolean fcs_enbl)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(enable FCS), or FALSE to disable FCS     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable Framer FCS checking and insertion.            */
/* If fcs_enbl is TRUE, CRC16 or CRC32 is inserted depending    */
/* on the CRC option selected using arn5_framer_pkt_ctl_crc32() */
/* API. If fcs_enbl is disabled, no FCS is appened to packet.   */
/* This API applies to both TX and RX both                      */
/* NOTE : For RPR/SRP mode, FCS should be disabled, since the   */
/* RPR/SRP carry their own CRC.                                 */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_uint8              bfFcs = (fcs_enbl) ? 0 : 1 ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMTX_PktControl_t         tx_pktctl ;
  ARN5_FR_STREAMRX_PktControl_t         rx_pktctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  /* RX Pkt control */
  regaddr =
      (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
  rx_pktctl.wrd = CS_REG_READ(regaddr) ;
  rx_pktctl.bf.nullFCS = bfFcs;
  CS_REG_WRITE(regaddr, rx_pktctl.wrd) ;

  /* TX pkt control */
  regaddr =
      (cs_uint16 *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd ;
  tx_pktctl.wrd = CS_REG_READ(regaddr) ;
  tx_pktctl.bf.NullFCS = bfFcs;
  CS_REG_WRITE(regaddr, tx_pktctl.wrd);

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PASSING OF FCS TO HOST ON RX */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_pass_fcs(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable passing of FCS to HOST.                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_STREAMRX_PktControl_t         rx_pktctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  /* RX Pkt control */
  regaddr =
      (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
  rx_pktctl.wrd = CS_REG_READ(regaddr) ;
  rx_pktctl.bf.passFCS = (ctl == CS_ENABLE) ? 1 : 0 ;

  /* added by wjd , not tested */
  rx_pktctl.bf.DropAddrCtrl = 0;

  CS_REG_WRITE(regaddr, rx_pktctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start SET FIFO THRESHOLD VALUE FOR PKT PROCESSING   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_set_fill_threshold(cs_uint16 port_id,
                        cs_uint16 thrshold)
/* INPUTS     : o Port Id                                       */
/*              o Fill threshold value in bytes                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the fifo fill threshold value. specifies the      */
/* number of bytes to be available in the Datacom bus input FIFO*/
/* before the packet processing engine starts mapping a given   */
/* packet in the sonet payload envelopes.                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  CS_REG_WRITE(&pDev->slice[sl_num].Framer.Threshold.wrd, thrshold) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   DROP PACKETS                                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_ctl_drop(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o TRUE( DROP) or FALSE                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable dropping of packets.                                  */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_boolean            rx_dir , tx_dir ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( rx_dir ) {
    ARN5_FR_STREAMRX_PktControl_t       rx_pktctl ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
    rx_pktctl.wrd = CS_REG_READ(regaddr) ;
    rx_pktctl.bf.dropBit =  (ctl) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, rx_pktctl.wrd) ;
  }

  if ( tx_dir ) {
    ARN5_FR_STREAMTX_PktControl_t       tx_pktctl ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd ;
    tx_pktctl.wrd = CS_REG_READ(regaddr) ;
    tx_pktctl.bf.DropFlag =  (ctl) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, tx_pktctl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   DROP LONG PACKETS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_drop_long(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o TRUE( DROP) or FALSE                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable dropping of packets which are longer than the         */
/* configured maximum packet length. To configure the max/min   */
/* packet lenght, user should call                              */
/* arn5_framer_pkt_cfg_max_min_size() API.                      */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_boolean            rx_dir , tx_dir ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( rx_dir ) {
    ARN5_FR_STREAMRX_PktControl_t       rx_pktctl ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
    rx_pktctl.wrd = CS_REG_READ(regaddr) ;
    rx_pktctl.bf.dropLong =  (ctl) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, rx_pktctl.wrd) ;
  }

  if ( tx_dir ) {
    ARN5_FR_STREAMTX_PktControl_t       tx_pktctl ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd ;
    tx_pktctl.wrd = CS_REG_READ(regaddr) ;
    tx_pktctl.bf.DropLong =  (ctl) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, tx_pktctl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE THE FILTERING OF SHORT PKTS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_drop_short(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o TRUE or FALSE                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables the filtering of packets which are shorter than 8    */
/* bytes or less on TX side and filtering of packets which are  */
/* are less than the min size specified on the RX side.         */
/* NOTE : Pkts coming from the system interface are dropped if  */
/* they are less than 8 bytes.                                  */
/*                                                              */
/* The [ctl] parameter is specified as following:               */
/* TRUE - Enable Dropping of short packets                      */
/* FALSE - Disable dropping of short packets                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_boolean            rx_dir , tx_dir ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( rx_dir ) {
    ARN5_FR_STREAMRX_PktControl_t       rx_pktctl ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
    rx_pktctl.wrd = CS_REG_READ(regaddr) ;
    rx_pktctl.bf.dropShort =  (ctl) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, rx_pktctl.wrd) ;
  }

  if ( tx_dir ) {
    ARN5_FR_DATACOMTX_StrConfig_t       tx_pktctl;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd ;
    tx_pktctl.wrd = CS_REG_READ(regaddr) ;
    tx_pktctl.bf.dropShort =  (ctl) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, tx_pktctl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONTROL PACKET DATA (DE)SCRAMBLING          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_ctl_scrambling(cs_uint16 port_id,
                       cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable (de)scrambling for both Tx and Rx sonet data. */
/* By default, (de)scrambling is ON.                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  /* RX */
  {
    /* if x.86 processing is enabled, the descramble control is */
    /* done at a different reg. Make sure the right one is set  */
    /* and the other scrambler does not interfere.              */
    ARN5_FR_STREAMRX_PktControl_t       rxPktctl ;
    ARN5_FR_STREAMRX_PktX86Ctrl_t       x86Pktctl ;

    if (!arn5_framer_pkt_is_x86_enabled(port_id)) {
      regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.PktX86Ctrl.wrd ;
      x86Pktctl.wrd = CS_REG_READ(regaddr) ;
      x86Pktctl.bf.X86DisDscr = 1 ;
      CS_REG_WRITE(regaddr, x86Pktctl.wrd) ;

      regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
      rxPktctl.wrd = CS_REG_READ(regaddr) ;
      rxPktctl.bf.disDscr = (ctl == CS_ENABLE) ? 0 : 1 ;
      CS_REG_WRITE(regaddr, rxPktctl.wrd) ;
    }
    else {
      regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
      rxPktctl.wrd = CS_REG_READ(regaddr) ;
      rxPktctl.bf.disDscr = 1 ;
      CS_REG_WRITE(regaddr, rxPktctl.wrd) ;

      regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.PktX86Ctrl.wrd ;
      x86Pktctl.wrd = CS_REG_READ(regaddr) ;
      x86Pktctl.bf.X86DisDscr = (ctl == CS_ENABLE) ? 0 : 1 ;
      CS_REG_WRITE(regaddr, x86Pktctl.wrd) ;
    }
  }

  /* TX */
  {
    ARN5_FR_STREAMTX_PktControl_t       tx_pktctl ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd ;
    tx_pktctl.wrd = CS_REG_READ(regaddr) ;
    tx_pktctl.bf.DisScr = (ctl == CS_ENABLE) ? 0 : 1 ;
    CS_REG_WRITE(regaddr, tx_pktctl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET PACKET FRAME ADDR AND CTRL FIELDS       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_set_frame_addr_n_ctl(cs_uint16 port_id,
                        cs_dir_t dir,
                        cs_uint8 addr, cs_uint8 ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o Value of the Address field                    */
/*              o Value of the Control field                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the expected and/or inserted values for fields Address   */
/* and Control of a POS frame.                                  */
/* By default, Address == 0xff, Control == 0x03                 */
/*                                                              */
/* In addition, the actual usage of expected values are further */
/* controlled by API arn5_framer_pkt_ctl_frame_addr_n_ctl()     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_boolean            rxDir, txDir ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_FR_PARSE_DIR_CTL(rxDir, txDir, dir) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if (rxDir) {
    ARN5_FR_STREAMRX_PktExAddrCtrl_t    rxAddrCtl;

    rxAddrCtl.bf.ExAddr = addr ;
    rxAddrCtl.bf.ExCtrl = ctl ;

    CS_REG_WRITE(&pDev->slice[sl_num].Framer.PktExAddrCtrl.wrd, rxAddrCtl.wrd) ;
  }

  if (txDir) {
    ARN5_FR_STREAMTX_PktAddrCtrl_t      txAddrCtl ;
    txAddrCtl.bf.Addr = addr ;
    txAddrCtl.bf.Ctrl = ctl ;

    CS_REG_WRITE(&pDev->slice[sl_num].Framer.PktAddrCtrl.wrd, txAddrCtl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CHECK PACKET FRAME ADDR AND CTRL FIELDS     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_ctl_frame_addr_n_ctl_check(cs_uint16 port_id,
                        cs_boolean check_addr, cs_boolean check_ctl)
/* INPUTS     : o Port Id                                       */
/*              o TRUE (Check Address Field) or FALSE (No Check)*/
/*              o TRUE (Check Control Field) or FALSE (No Check)*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control the checking for RX Address and control fields.      */
/* Frames with mis-matched fields will be dropped automatically.*/
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_STREAMRX_PktControl_t  pktCtl;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  CS_REG_READ(&pDev->slice[sl_num].Framer.streamRx_PktControl.wrd) ;
  pktCtl.bf.ChkAddr = check_addr ;
  pktCtl.bf.ChkCtrl = check_ctl ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.streamRx_PktControl.wrd, pktCtl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET PACKET FRAME SAPI(PROTOCOL) FIELD       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_set_frame_sapi(cs_uint16 port_id,
                        cs_uint16 sapi)
/* INPUTS     : o Port Id                                       */
/*              o Value of the SAPI field                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the inserted values for field SAPI(or PROTOCOL) of       */
/* a POS frame.                                                 */
/* By default, SAPI == 0x0021                                   */
/*                                                              */
/* In addition, the actual usage of inserted value are further  */
/* controlled by API arn5_framer_pkt_ctl_frame_addr_n_ctl()     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.PktSAPI.wrd, sapi) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONTROL PACKET ADDR/CTRL TX INSERT           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_ctl_frame_addr_n_ctl_tx_insert(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_ctl_t sapi_ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control the insertion of the Address/Control fields and      */
/* optional sapi field in the TX direction.                     */
/*                                                              */
/* The inserted address and control are set by API              */
/* arn5_framer_pkt_set_frame_addr_n_ctl()                       */
/* The inserted sapi are set by API                             */
/* arn5_framer_pkt_set_frame_sapi()                             */
/*                                                              */
/* When the insertion (in TX) is disabled, the RX side should   */
/* be set to drop these fields as well.  See also API           */
/* arn5_framer_pkt_ctl_frame_addr_n_ctl_rx_drop()               */
/*                                                              */
/* Please note that sapi insertion can only be enabled when     */
/* address/control insertion is enabled.                        */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_STREAMTX_PktControl_t  txPktCtl;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if ((ctl == CS_DISABLE) && (sapi_ctl == CS_ENABLE)) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
      ":sapi_ctl can only set to enable when ctl is also set to enable\n") ;
    return (CS_ERROR) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  txPktCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.streamTx_PktControl.wrd) ;
  txPktCtl.bf.InsAddrCtrl = (ctl == CS_ENABLE) ? 1 : 0 ;
  txPktCtl.bf.InsSAPI = (sapi_ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.streamTx_PktControl.wrd, txPktCtl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONTROL PACKET ADDR/CTRL RX DROP             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_ctl_frame_addr_n_ctl_rx_drop(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_ctl_t sapi_ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control the removal of the Address/Control fields and        */
/* optional sapi field in the RX direction.                     */
/*                                                              */
/* When the drop (in RX) is disabled, the TX side should        */
/* be set to insert these fields as well.  See also API         */
/* arn5_framer_pkt_ctl_frame_addr_n_ctl_tx_insert()             */
/*                                                              */
/* Please note that sapi drop can only be enabled when          */
/* address/control drop is enabled.                             */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_STREAMRX_PktControl_t  rxPktCtl;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if ((ctl == CS_DISABLE) && (sapi_ctl == CS_ENABLE)) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
      ":sapi_ctl can only set to enable when ctl is also set to enable\n") ;
    return (CS_ERROR) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rxPktCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.streamRx_PktControl.wrd) ;
  rxPktCtl.bf.DropAddrCtrl = (ctl == CS_ENABLE) ? 1 : 0 ;
  rxPktCtl.bf.DropSAPI = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.streamRx_PktControl.wrd, rxPktCtl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start   CHECK X.86 PROCESSING MODE                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_boolean  arn5_framer_pkt_is_x86_enabled(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : TRUE or FALSE                                   */
/* DESCRIPTION:                                                 */
/* Check whether x.86 frsame processiong is enabled or not.     */
/* $_hdr_end.                                                   */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_STREAMRX_PktX86Ctrl_t  x86Ctl;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_OK) ;

  x86Ctl.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.PktX86Ctrl.wrd) ;
  return (x86Ctl.bf.X86En == 1) ;
}

