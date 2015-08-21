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
#include "m2r_framer_cmn.h"
#include "m2r_framer_pkt.h"

/****************************************************************/
/* $rtn_hdr_start   CRC16 or CRC32 SELECTION                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_ctl_crc32(cs_uint16 port_id,
                     m2r_framer_stream_t channel, cs_boolean crc32)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o TRUE if CRC32, FALSE if CRC16(default)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selects whether CRC32 or CRC16 is appended/expected in       */
/* frame. By default, CRC16 is used.                            */
/* This API applies to both TX and RX both                      */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  cs_uint8                 bit_crc32 ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  bit_crc32 = (crc32) ? 1 : 0 ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    FR_STREAMTX_PktControl_t            tx_pktctl ;
    FR_STREAMRX_PktControl_t            rx_pktctl ;

    /* RX Pkt/cell control */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMRX__STRIDE) ;
    rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    rx_pktctl.bf.crc32 = bit_crc32 ;
    M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;

    /* TX Pkt/cell control */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMTX__STRIDE) ;
    tx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    tx_pktctl.bf.crc32 = bit_crc32 ;
    M2R_REG_WRITE(regaddr, tx_pktctl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   CONTROL FRAMER FCS CHECKING/INSERTION       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_ctl_fcs(cs_uint16 port_id,
                   m2r_framer_stream_t channel, cs_boolean fcs_enbl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o TRUE(enable FCS), or FALSE to disable FCS     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable Framer FCS checking and insertion.            */
/* If fcs_enbl is TRUE, CRC16 or CRC32 is inserted depending    */
/* on the CRC option selected using m2r_framer_pkt_ctl_crc32()  */
/* API. If fcs_enbl is disabled, no FCS is appened to packet.   */
/* This API applies to both TX and RX both                      */
/* NOTE : For RPR/SRP mode, FCS should be disabled, since the   */
/* RPR/SRP carry thier CRC                                      */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  cs_uint8                 bfFcs = (fcs_enbl) ? 0 : 1 ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    FR_STREAMTX_PktControl_t            tx_pktctl ;
    FR_STREAMRX_PktControl_t            rx_pktctl ;

    /* RX Pkt control */
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMRX__STRIDE) ;
    rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    rx_pktctl.bf.nullFCS = bfFcs;
    M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;

    /* TX pkt control */
    regaddr =
        (cs_uint16 *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd +
        (str_iter * M2R_FR_STREAMTX__STRIDE) ;
    tx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    tx_pktctl.bf.NullFCS = bfFcs;
    M2R_REG_WRITE(regaddr, tx_pktctl.wrd);
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PASSING OF FCS TO HOST ON RX */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_pass_fcs(cs_uint16 port_id,
                       m2r_framer_stream_t channel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable passing of FCS to HOST.                       */
/*                                                              */
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
    FR_STREAMRX_PktControl_t            rx_pktctl ;

    /* RX Pkt control */
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMRX__STRIDE) ;
    rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    rx_pktctl.bf.passFCS = (ctl == M2R_ENABLE) ? 1 : 0 ;
   
    /* newly added wjd, if we are in ppp mode default do not pass ff03, we must
       command it to pass; if it is in hdlc mode, the following will not acctually take en
    */
	
    rx_pktctl.bf.modeDropFF03 = 0;
	
    M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start SET FIFO THRESHOLD VALUE FOR PKT PROCESSING   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_set_fill_threshold(cs_uint16 port_id,
                  m2r_framer_stream_t channel, cs_uint16 thrshold)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o Fill threshold value in bytes                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the fifo fill threshold value. specifies the      */
/* number of bytes to be available in the Datacom bus input FIFO*/
/* before the packet processing engine starts mapping a given   */
/* packet in the sonet payload envelopes.                       */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    M2R_REG_WRITE((&pDev->slice[sl_num].Framer.Threshold.wrd +
                 (str_iter * M2R_FR_STREAMTX__STRIDE)), thrshold) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   DROP PACKETS                                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_ctl_drop(cs_uint16 port_id,
                                   m2r_framer_stream_t channel,
                                   m2r_dir_t dir, cs_boolean ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o TRUE( DROP) or FALSE                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable dropping of packets.                                  */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  cs_boolean               rx_dir , tx_dir ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    if ( rx_dir ) {
      FR_STREAMRX_PktControl_t            rx_pktctl ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                  (str_iter * M2R_FR_STREAMRX__STRIDE) ;
      rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
      rx_pktctl.bf.dropBit =  (ctl) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;
    }

    if ( tx_dir ) {
      FR_STREAMTX_PktControl_t            tx_pktctl ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd +
                  (str_iter * M2R_FR_STREAMTX__STRIDE) ;
      tx_pktctl.wrd = M2R_REG_READ(regaddr) ;
      tx_pktctl.bf.DropFlag =  (ctl) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, tx_pktctl.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   DROP LONG PACKETS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_drop_long(cs_uint16 port_id,
                                    m2r_framer_stream_t channel,
                                    m2r_dir_t dir, cs_boolean ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o TRUE( DROP) or FALSE                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable dropping of packets which are longer than the         */
/* configured maximum packet length. To configure the max/min   */
/* packet lenght, user should call                              */
/* m2r_framer_pkt_cfg_max_min_size() API.                       */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  cs_boolean               rx_dir , tx_dir ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    if ( rx_dir ) {
      FR_STREAMRX_PktControl_t            rx_pktctl ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                  (str_iter * M2R_FR_STREAMRX__STRIDE) ;
      rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
      rx_pktctl.bf.dropLong =  (ctl) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;
    }

    if ( tx_dir ) {
      FR_STREAMTX_PktControl_t            tx_pktctl ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd +
                  (str_iter * M2R_FR_STREAMTX__STRIDE) ;
      tx_pktctl.wrd = M2R_REG_READ(regaddr) ;
      tx_pktctl.bf.DropLong =  (ctl) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, tx_pktctl.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE THE FILTERING OF SHORT PKTS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_drop_short(cs_uint16 port_id,
        m2r_framer_stream_t channel, m2r_dir_t dir, cs_boolean ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
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
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/*                                                              */
/* The [ctl] parameter is specified as following:               */
/* TRUE - Enable Dropping of short packets                      */
/* FALSE - Disable dropping of short packets                    */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  cs_boolean               rx_dir , tx_dir ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    if ( rx_dir ) {
      FR_STREAMRX_PktControl_t            rx_pktctl ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                  (str_iter * M2R_FR_STREAMRX__STRIDE) ;
      rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
      rx_pktctl.bf.dropShort =  (ctl) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;
    }

    if ( tx_dir ) {
      FR_DATACOMTX_StrConfig_t            tx_pktctl;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd +
                  (str_iter * M2R_FR_DATACOMTX_StrConfig_STRIDE) ;
      tx_pktctl.wrd = M2R_REG_READ(regaddr) ;
      tx_pktctl.bf.dropShort =  (ctl) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, tx_pktctl.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONTROL PACKET DATA (DE)SCRAMBLING          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_ctl_scrambling(cs_uint16 port_id,
                       m2r_framer_stream_t channel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable (de)scrambling for both Tx and Rx sonet data. */
/* By default, (de)scrambling is ON.                            */
/*                                                              */
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
    FR_STREAMRX_PktControl_t            rx_pktctl ;
    FR_STREAMTX_PktControl_t            tx_pktctl ;

    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMRX__STRIDE) ;
    rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    rx_pktctl.bf.disDscr =  (ctl == M2R_ENABLE) ? 0 : 1 ;
    M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;

    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMTX__STRIDE) ;
    tx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    tx_pktctl.bf.DisScr = (ctl == M2R_ENABLE) ? 0 : 1 ;
    M2R_REG_WRITE(regaddr, tx_pktctl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

