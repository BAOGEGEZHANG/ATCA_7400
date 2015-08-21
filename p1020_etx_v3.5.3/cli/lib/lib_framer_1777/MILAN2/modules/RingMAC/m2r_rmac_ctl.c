/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly pgrohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_ctl.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_rmac.h"
#include "m2r_rmac_priv.h"


/****************************************************************
 *               Local function prototypes                      *
 ****************************************************************/
static
void m2r_print_rcvd_ips_ctl_pkt(cs_uint16 port_id, cs_boolean ips_pkt,
				cs_uint32 pkt_num, cs_uint8 * buf,
				cs_uint16 * pbytes_read) ;



/****************************************************************
 **********     IPS/CTL PACKET CONTROLS    **********************
 ****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  SET IPS/CTL PACKET RECEIVE BUFFER ENABLE     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_enable_ips_ctl_pkt_rxbuf(cs_uint16 port_id,
					     cs_boolean ips_buf,
					     cs_boolean ctl_buf)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(enable IPS Rcv buffer) or FALSE(disable) */
/*              o TRUE(enable CTL Rcv buffer) or FALSE(disable) */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This is the global enable control for local buffering of     */
/* (IPS and other control) packets. When these packets are      */
/* locally buffered, they do NOT go to the Host interface but   */
/* go to the CPU instead(via MPIF).                             */
/*                                                              */
/* NOTE:                                                        */
/* When the local buffering is ENABLED, the control packets     */
/* STILL have the option of being locally buffered(to CPU) or   */
/* sent to HOST(similar to data packets). This option is        */
/* decided by the setting of the default destination bit as well*/
/* as the programmed entries in the Control CAM.                */
/*                                                              */
/* It is recommended the local buffer is ENABLED to receive     */
/* IPS and other control packets at initialization time, and    */
/* routing decision(CPU vs Host) be made by programming the     */
/* default destination and the Control CAM entries.             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_TBD_IPS_CTL_CR_t   cr ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd) ;
  cr.bf.IPS_RCV_ENABLE = (ips_buf) ? 1 : 0 ;
  cr.bf.IPS_RCV_ERR_ENABLE = (ips_buf) ? 1 : 0 ;
  cr.bf.CTL_RCV_ENABLE = (ctl_buf) ? 1 : 0 ;
  cr.bf.CTL_RCV_ERR_ENABLE = (ctl_buf) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd, cr.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET IPS/CTL PACKET DEFAULT DESTINATION       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_set_ips_ctl_pkt_rx_default_dstn(cs_uint16 port_id,
						   cs_boolean ips_buf,
						   cs_boolean ctl_buf)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(default to IPS Rcv buffer) or FALSE(Host)*/
/*              o TRUE(default to CTL Rcv buffer) or FALSE(Host)*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selects what the default destination of control(IPS and other*/
/* control)packets should be - to local buffer(for CPU) or      */
/* to the Host.                                                 */
/*                                                              */
/* Note:                                                        */
/* If Control CAM is programmed and if there is a 'hit', then   */
/* that will override the default destination by INVERTING      */
/* the default destination(ie. go to Host instead of local      */
/* buffer and vice-versa).                                      */
/* That is,                                                     */
/* to_buffer = buf_enable & (default_dest_to_buf ^ cam_hit)     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_TBD_IPS_CTL_CR_t   cr ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd) ;
  cr.bf.IPS_RCV_DEFAULT_DEST = (ips_buf) ? 1 : 0 ;
  cr.bf.CTL_RCV_DEFAULT_DEST = (ctl_buf) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd, cr.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ADD IPS/CTL CAM ENTRY                        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_add_ips_ctl_cam_entry(cs_uint16 port_id,
					  cs_uint8 num,
					  cs_uint8 dec_mode,
					  cs_uint16 ctl_mask,
					  cs_uint16 ctl_type)
/* INPUTS     : o Port Id                                       */
/*              o Entry number[0...3]                           */
/*              o Bit mask for Mode in SRP or [WE,PT] in RPR    */
/*              o Control Mask(16 bits)                         */
/*              o Control Type(16 bits - type and version)      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure a Control CAM entry.                               */
/* A maximum of 4 entries can be programmed in the CAM.         */
/* If there is CAM 'hit', then that will override the default   */
/* destination by INVERTING the default destination(ie.         */
/* go to Host instead of local buffer and vice-versa).          */
/* That is,                                                     */
/* to_buffer = buf_enable & (default_dest_to_buf ^ cam_hit)     */
/*                                                              */
/* The incoming Control packet will looked up in this CAM, and  */
/* will be considered a match if its Control Type matches       */
/* 'under' the mask and the dec_mode has 1 in the bit           */
/* corresponding to the packet's MODE value. It is a 'hit' if   */
/* ((ctl_type & ctl_mask) ==                                    */
/*      (RCVD_TYPE & ctl_mask)) && dec_mode[RCVD_MODE]          */
/*                                                              */
/* The [dec_mode] parameter is specified as the bit-map of the  */
/* MODE value which is a 3-bit field.                           */
/* For RPR, The MODE is [Wrap Eligible(WE), Packet-Type(PE)].   */
/* For SRP, The MODE is Mode field in the header.               */
/*                                                              */
/* The [ctl_mask] parameter is used for masking the Control Type*/
/* field(16-bits for type and version fields) in RPR/SRP control*/
/* packet header.                                               */
/*                                                              */
/* The [ctl_type] parameter corresponds to the Control Type     */
/* field(16-bits) in RPR/SRP control packet header.             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_TBD_DEC_MODE0_1_t  mode01 ;
  DR_TBD_DEC_MODE2_3_t  mode23 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if (num >= 4) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": num(%d) s/b [0...3]\n", num) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  switch (num) {
    default:
    case 0 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_TYPE0.wrd,
		   ctl_type) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_MASK0.wrd,
		   ctl_mask) ;

      mode01.wrd =
	M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DEC_MODE0_1.wrd) ;
      mode01.bf.DEC_MODE0 = dec_mode ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DEC_MODE0_1.wrd,
		   mode01.wrd) ;
      break ;

    case 1 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_TYPE1.wrd,
		   ctl_type) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_MASK1.wrd,
		   ctl_mask) ;

      mode01.wrd =
	M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DEC_MODE0_1.wrd) ;
      mode01.bf.DEC_MODE1 = dec_mode ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DEC_MODE0_1.wrd,
		   mode01.wrd) ;
      break ;

    case 2 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_TYPE2.wrd,
		   ctl_type) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_MASK2.wrd,
		   ctl_mask) ;

      mode23.wrd =
	M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DEC_MODE2_3.wrd) ;
      mode23.bf.DEC_MODE2 = dec_mode ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DEC_MODE2_3.wrd,
		   mode23.wrd) ;
      break ;

    case 3 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_TYPE3.wrd,
		   ctl_type) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_MASK3.wrd,
		   ctl_mask) ;
      mode23.wrd =
	M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DEC_MODE2_3.wrd) ;
      mode23.bf.DEC_MODE3 = dec_mode ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DEC_MODE2_3.wrd,
		   mode23.wrd) ;
      break ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************
 **********     SEND IPS/CTL PACKET FROM CPU    *****************
 ****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  CHECK IPS/CTL SEND BUFFER BUSY STATUS        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint16  m2r_rmac_is_ips_ctl_sendbuf_busy(cs_uint16 port_id,
					    cs_boolean ips_buf)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(for IPS buffer) or FALSE(for CTL buffer) */
/* OUTPUTS    : ----                                            */
/* RETURNS    : 0(not-busy), number of bytes left(busy),        */
/*              or (-1) on error.                               */
/* DESCRIPTION:                                                 */
/* Checks the busy status of the specified send buffer.         */
/* If the return value is zero then the send buffer is NOT busy.*/
/*                                                              */
/* The [ips_buf] parameter specifies if it is IPS Send buffer   */
/* (if TRUE), or CTL Send buffer(if FALSE).                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_reg	sz ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, (-1)) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, (-1)) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (ips_buf) {
    sz = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_SND_SIZE.wrd) ;
  }
  else {
    sz = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_SND_SIZE.wrd) ;
  }

  return (sz) ;
}

/****************************************************************/
/* $rtn_hdr_start  SEND A CTL PACKET VIA MPIF                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_send_ctl_pkt(cs_uint16 port_id, cs_uint8 * ppkt,
				 cs_uint16 pkt_len)
/* INPUTS     : o Port Id                                       */
/*              o pointer to user provided packet               */
/*              o packet length                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Send one CTL packet(non-IPS) out from CPU via MPIF.          */
/*                                                              */
/* The [ppkt] parameter is a pointer to caller provided CTL     */
/* packet. The bytes should be in the networking order, ie. if  */
/* pkt length is n bytes, then ppkt[0] is TTL field in SRP/RPR  */
/* header, and ppkt[n-1] is last byte of FCS.                   */
/*                                                              */
/* The [pkt_len] is the length of the packet in bytes.          */
/* The buffer size is ~8K bytes and is written one word(8 bytes)*/
/* at a time.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( m2r_rmac_write_pkt_to_ctl_send_buf(port_id, ppkt, pkt_len)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return ( m2r_rmac_send_same_ctl_pkt(port_id, pkt_len) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  WRITE A CTL PACKET TO CTL SEND BUFFER        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_write_pkt_to_ctl_send_buf(cs_uint16 port_id,
					      cs_uint8 * ppkt,
					      cs_uint16 pkt_len)
/* INPUTS     : o Port Id                                       */
/*              o pointer to user provided packet               */
/*              o packet length                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Writes one CTL packet(non-IPS) into the Control Buffer.      */
/* This API will only write the packet to the buffer. The packet*/
/* will be sent out only when a subsequent call to API,         */
/* m2r_rmac_send_same_ctl_pkt() is made.                        */
/*                                                              */
/* The [ppkt] parameter is a pointer to caller provided CTL     */
/* packet. The bytes should be in the networking order, ie. if  */
/* pkt length is n bytes, then ppkt[0] is TTL field in SRP/RPR  */
/* header, and ppkt[n-1] is last byte of FCS.                   */
/*                                                              */
/* The [pkt_len] is the length of the packet in bytes.          */
/* The buffer size is ~8K bytes and is written one word(8 bytes)*/
/* at a time.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_uint16		num_words, wc, busy_sz ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  if (ppkt == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_NULL_PTR, ": ppkt\n") ;
    return (CS_ERROR) ;
  }

  if (pkt_len >= M2R_RMAC_CTL_SEND_BUF_SZ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": pkt_len(0x%x) s/b <= 0x%x\n",
		  pkt_len, M2R_RMAC_CTL_SEND_BUF_SZ) ;
    return (CS_ERROR) ;
  }

  /* Check if any previously sent packet is still not done */
  busy_sz = m2r_rmac_is_ips_ctl_sendbuf_busy(port_id, FALSE) ;
  if (busy_sz) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_CTL_SEND_BUF_BUSY,
		  ": has 0x%x bytes to be processed\n",
		  busy_sz) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_SND_HEAD.wrd, 0) ;
  num_words = (pkt_len + 7) / 8 ;     /* 8 bytes per word */

  /* HEAD gets auto-incremented */
  for (wc = 0; wc < num_words; wc++) {
    cs_reg	data ;
    cs_uint16   idx, bytes_left ;

    data = 0 ;
    idx = (wc * 8) ;
    bytes_left = (pkt_len - (wc * 8)) ;

    data  = (bytes_left >= 1) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 2) ? (ppkt[idx++] << 0) : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_SND_DATA3.wrd,
		 data) ;
    data = 0 ;
    if (bytes_left <= 2) goto write_done ;

    data  = (bytes_left >= 3) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 4) ? (ppkt[idx++] << 0) : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_SND_DATA2.wrd,
		 data) ;
    data = 0 ;
    if (bytes_left <= 4) goto write_done ;

    data  = (bytes_left >= 5) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 6) ? (ppkt[idx++] << 0) : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_SND_DATA1.wrd,
		 data) ;
    data = 0 ;
    if (bytes_left <= 6) goto write_done ;

    data  = (bytes_left >= 7) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 8) ? (ppkt[idx++] << 0) : 0 ;

    write_done:
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_SND_DATA0.wrd,
		 data) ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SEND SAME CTL PACKET VIA MPIF                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_send_same_ctl_pkt(cs_uint16 port_id,
				      cs_uint16 pkt_len)
/* INPUTS     : o Port Id                                       */
/*              o packet length                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sends one CTL packet that was already written into           */
/* CTL Send Buffer.                                             */
/*                                                              */
/* This API can be used where the same packet may need to be    */
/* sent repeatedly. After writing the packet once into the      */
/* send buffer using m2r_rmac_send_ctl_pkt() API, the user      */
/* can call this API to send it again and again without         */
/* re-writing the same packet into the buffer everytime.        */
/*                                                              */
/* The caller is responsible for ensuring that previously,      */
/* m2r_rmac_send_ctl_pkt() was called and that the same length  */
/* of the packet is provided for both API's.                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_uint16		busy_sz ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* Check if any previously sent packet is still not done */
  busy_sz = m2r_rmac_is_ips_ctl_sendbuf_busy(port_id, FALSE) ;
  if (busy_sz) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_CTL_SEND_BUF_BUSY,
		  ": has 0x%x bytes to be processed\n",
		  busy_sz) ;
    return (CS_ERROR) ;
  }

  if ( !(CS_IN_TEST_ENV()) ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_SND_SIZE.wrd,
		 pkt_len) ;
  }
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SEND AN IPS PACKET VIA MPIF                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_send_ips_pkt(cs_uint16 port_id, cs_uint8 * ppkt,
				 cs_uint16 pkt_len)
/* INPUTS     : o Port Id                                       */
/*              o pointer to user provided packet               */
/*              o packet length                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Send one IPS packet out from CPU via MPIF.                   */
/*                                                              */
/* The [ppkt] parameter is a pointer to caller provided IPS     */
/* packet. The bytes should be in the networking order, ie. if  */
/* pkt length is n bytes, then ppkt[0] is TTL field in SRP/RPR  */
/* header, and ppkt[n-1] is last byte of FCS.                   */
/*                                                              */
/* The [pkt_len] is the length of the packet in bytes.          */
/* The buffer size is ~512 bytes and is written one word        */
/* (8 bytes) at a time.                                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( m2r_rmac_write_pkt_to_ips_send_buf(port_id, ppkt, pkt_len)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return( m2r_rmac_send_same_ips_pkt(port_id, pkt_len) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  WRITE AN IPS PACKET TO IPS SEND BUFFER       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_write_pkt_to_ips_send_buf(cs_uint16 port_id,
					      cs_uint8 * ppkt,
					      cs_uint16 pkt_len)
/* INPUTS     : o Port Id                                       */
/*              o pointer to user provided packet               */
/*              o packet length                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Writes one IPS packet into the IPS Send Buffer.              */
/* This API will only write the packet to the buffer. The packet*/
/* will be sent out only when a subsequent call to API,         */
/* m2r_rmac_send_same_ips_pkt() is made.                        */
/*                                                              */
/* The [ppkt] parameter is a pointer to caller provided IPS     */
/* packet. The bytes should be in the networking order, ie. if  */
/* pkt length is n bytes, then ppkt[0] is TTL field in SRP/RPR  */
/* header, and ppkt[n-1] is last byte of FCS.                   */
/*                                                              */
/* The [pkt_len] is the length of the packet in bytes.          */
/* The buffer size is ~512 bytes and is written one word        */
/* (8 bytes) at a time.                                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_uint16		num_words, wc, busy_sz ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  if (ppkt == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_NULL_PTR, ": ppkt\n") ;
    return (CS_ERROR) ;
  }

  if (pkt_len >= M2R_RMAC_CTL_SEND_BUF_SZ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": pkt_len(0x%x) s/b <= 0x%x\n",
		  pkt_len, M2R_RMAC_IPS_SEND_BUF_SZ) ;
    return (CS_ERROR) ;
  }

  /* Check if any previously sent packet is still not done */
  busy_sz = m2r_rmac_is_ips_ctl_sendbuf_busy(port_id, TRUE) ;
  if (busy_sz) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_IPS_SEND_BUF_BUSY,
		  ": has 0x%x bytes to be processed\n",
		  busy_sz) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_SND_HEAD.wrd, 0) ;
  num_words = (pkt_len + 7) / 8 ;     /* 8 bytes per word */

  /* HEAD gets auto-incremented */
  for (wc = 0; wc < num_words; wc++) {
    cs_reg	data ;
    cs_uint16   idx, bytes_left ;

    data = 0 ;
    idx = (wc * 8) ;
    bytes_left = (pkt_len - (wc * 8)) ;

    data  = (bytes_left >= 1) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 2) ? (ppkt[idx++] << 0) : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_SND_DATA3.wrd,
		 data) ;
    data = 0 ;
    if (bytes_left <= 2) goto write_done ;

    data  = (bytes_left >= 3) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 4) ? (ppkt[idx++] << 0) : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_SND_DATA2.wrd,
		 data) ;
    data = 0 ;
    if (bytes_left <= 4) goto write_done ;

    data  = (bytes_left >= 5) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 6) ? (ppkt[idx++] << 0) : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_SND_DATA1.wrd,
		 data) ;
    data = 0 ;
    if (bytes_left <= 6) goto write_done ;

    data  = (bytes_left >= 7) ? (ppkt[idx++] << 8) : 0 ;
    data |= (bytes_left >= 8) ? (ppkt[idx++] << 0) : 0 ;

    write_done:
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_SND_DATA0.wrd,
		 data) ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SEND SAME IPS PACKET VIA MPIF                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_send_same_ips_pkt(cs_uint16 port_id,
				      cs_uint16 pkt_len)
/* INPUTS     : o Port Id                                       */
/*              o packet length                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sends one IPS packet that was already written into           */
/* IPS Send Buffer.                                             */
/*                                                              */
/* This API is provided where the same packet may need to be    */
/* sent repeatedly. After writing the packet once into the      */
/* send buffer using m2r_rmac_send_ips_pkt() API, the user      */
/* can call this API to send it again and again without         */
/* re-writing the same packet into the buffer everytime.        */
/*                                                              */
/* The caller is responsible for ensuring that previously,      */
/* m2r_rmac_send_ips_pkt() was called and that the same length  */
/* of the packet is provided for both API's.                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_uint16		busy_sz ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* Check if any previously sent packet is still not done */
  busy_sz = m2r_rmac_is_ips_ctl_sendbuf_busy(port_id, TRUE) ;
  if (busy_sz) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_IPS_SEND_BUF_BUSY,
		  ": has 0x%x bytes to be processed\n",
		  busy_sz) ;
    return (CS_ERROR) ;
  }

  if ( !(CS_IN_TEST_ENV()) ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_SND_SIZE.wrd,
		 pkt_len) ;
  }
  return (CS_OK) ;
}


/****************************************************************
 **********     READ IPS/CTL PACKETS FROM BUFFER    *************
 ****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  READ CTL PACKETS FROM RECEIVE BUFFER         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_read_ctl_pkts_from_rxbuf(cs_uint16 port_id,
			cs_uint8 * buf, cs_uint16 * pbuf_len,
			cs_uint16  * pnum_pkts, cs_uint16 pkt_limit)
/* INPUTS     : o Port Id                                       */
/*              o caller provided buffer                        */
/*              o pointer to buffer length(also updated by drvr)*/
/*              o pointer for driver to indciate no. of packets */
/*              o Limit on how many packets to be read out      */
/* OUTPUTS    : o Packets received(in networking order)         */
/*              o Total no. of bytes in buffer                  */
/*              o Number of packets read out                    */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reads one or more CTL packets that were received in the      */
/* Receive CTL buffer. The packets are put in the buffer in the */
/* same order(network order) as they are in the Rcv buffer.     */
/* Each packet has a 8-byte packet length header word which     */
/* has the actual length of the pkt(in bits 12:0, and pkt Error */
/* indication in bit 15).                                       */
/*                                                              */
/* The [buf] is pointer to caller allocated memory.             */
/*                                                              */
/* The [pbuf_len] parameter serves two purposes:                */
/*  o For caller to let driver know the size of buf             */
/*  o For driver to update and let the caller know how much     */
/*    of it contains valid data(pkts + pkt length headers).     */
/* This length should be a multiple of 8 bytes.                 */
/*                                                              */
/* The [pnum_pkts] parameter is used by the driver to indicate  */
/* to the caller, the number of valid packets read out.         */
/*                                                              */
/* ----- IMPORTANT NOTE REGARDING BUFFER SIZE(pbuf_len)!! ----- */
/* This length should be a multiple of 8 bytes and MUST be the  */
/* same size as the CTL Rcv buffer(8K bytes)! This is required  */
/* as SW cannot just read the packet hdr to find the new packet */
/* length but not read the packet itself due to insufficient    */
/* length- otherwise, the HEAD pointer gets auto-incremented to */
/* SOP of the next packet and the packet hdr entry could be     */
/* overwritten with new packet data.                            */
/*                                                              */
/* ----- IMPORTANT NOTE REGARDING PACKET LIMIT(pkt_limit)!! --- */
/* The [pkt_limit] parameter specifies the max number of pkts   */
/* and must be 0xFFFF. This will ensure that all packets in the */
/* buffer at the time of calling this API will be read out.     */
/* Otherwise, packets could be left behind without being read   */
/* out and the interrupt de-asserted. The interrupt is triggered*/
/* only on the arrival of a packet and can be cleared by SW     */
/* irrespective of whether there are packet(s) in the buffer or */
/* or not waiting to be read. This parameter is still there for */
/* downward compatibility of the driver for the existing .      */
/* customers.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_reg		data = 0 ;
  cs_uint16		head, tail, buf_len ;
  cs_uint16		i, num_words, pkt_bytes_to_process = 0 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  if (buf == NULL || pbuf_len == NULL || pnum_pkts == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_NULL_PTR,
		  ": buf(%p), pbuf_len(%p), pnum_pkts(%p)\n",
		  buf, pbuf_len, pnum_pkts) ;
    return (CS_ERROR) ;
  }

  if (*pbuf_len < M2R_RMAC_CTL_RX_BUF_SZ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
                  ": User CTL Rx buf_len(0x%x) s/b 0x%x bytes\n",
                  *pbuf_len, M2R_RMAC_CTL_RX_BUF_SZ) ;
    return (CS_ERROR) ;
  }

  if (pkt_limit != 0xFFFF) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
                  ": pkt_limit(0x%x) s/b 0xffff bytes\n",
                  pkt_limit) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  buf_len   = *pbuf_len ;
  CS_MEMSET( (void *)buf, 0, buf_len ) ;
  *pbuf_len = 0 ;
  *pnum_pkts = 0 ;

  head = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_RCV_HEAD.wrd) ;
  tail = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_RCV_TAIL.wrd) ;

  if (head == tail) {   /* no packet in the buffer */
    goto rtn_exit ;
  }

  num_words = (head > tail) ?
    ( (M2R_RMAC_CTL_RX_BUF_TOTAL_WORDS - head) + tail ) :
    (tail - head) ;

  /*
   * Steps for reading data out of the buffer
   * 1. Read DATA0 once & discard it(dummy read for auto-increment)
   * 2. Read DATA registers iteratively until all valid bytes
   *    of data currently in the Receive buffer are read.
   * 3. Keep track of the Head pointer, and before exiting update
   *    HEAD register to EOP of the last packet.
   */
  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_RCV_DATA0.wrd) ;
  head = (head + 1) % M2R_RMAC_CTL_RX_BUF_TOTAL_WORDS ;

  for (i = 0; i < num_words; i++) {
    cs_uint16	pkt_len ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_RCV_DATA3.wrd) ;
    buf[i*8 + 0] = (data >> 8) & 0xFF ;
    buf[i*8 + 1] = (data >> 0) & 0xFF ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_RCV_DATA2.wrd) ;
    buf[i*8 + 2] = (data >> 8) & 0xFF ;
    buf[i*8 + 3] = (data >> 0) & 0xFF ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_RCV_DATA1.wrd) ;
    buf[i*8 + 4] = (data >> 8) & 0xFF ;
    buf[i*8 + 5] = (data >> 0) & 0xFF ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CTL_RCV_DATA0.wrd) ;
    buf[i*8 + 6] = (data >> 8) & 0xFF ;
    buf[i*8 + 7] = (data >> 0) & 0xFF ;

    if (pkt_bytes_to_process <= 0) { /* new header */
      pkt_len = (data & M2R_RMAC_IPS_CTL_PKT_LEN_MASK) ;
      /* Packets are padded at the end - to align on 8-byte boundary */
      pkt_bytes_to_process = (pkt_len + 7) & 0xfff8 ;

      /* check if there is room in user buffer for another pkt */
      if ( pkt_bytes_to_process > (buf_len - *pbuf_len) ) {
	CS_PRINT("port-%d: User CTL pkt buffer FULL(num pkts = %d)\n",
		 port_id, *pnum_pkts) ;
	break ;
      }

      *pbuf_len += (8 + pkt_bytes_to_process) ;
      (*pnum_pkts)++ ;
    }

    else { /* packet bytes */
      pkt_bytes_to_process -= 8 ;
    }

    head = (head +1) % M2R_RMAC_CTL_RX_BUF_TOTAL_WORDS ;
  } /* for */

  /*
   * Make sure that hw HEAD pointer points to the EOP of the last
   * packet processed. This is needed for auto-mechanism to work
   * next time around.
   */
  head = ( (head == 0) ?
	   (M2R_RMAC_CTL_RX_BUF_TOTAL_WORDS - 1) : (head - 1) ) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_RCV_HEAD.wrd, head) ;

  rtn_exit :
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  READ IPS PACKETS FROM RECEIVE BUFFER         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_read_ips_pkts_from_rxbuf(cs_uint16 port_id,
			cs_uint8 * buf, cs_uint16 * pbuf_len,
			cs_uint16  * pnum_pkts, cs_uint16 pkt_limit)
/* INPUTS     : o Port Id                                       */
/*              o caller provided buffer                        */
/*              o pointer to buffer length(also updated by drvr)*/
/*              o pointer for driver to indciate no. of packets */
/*              o Limit on how many packets to be read out      */
/* OUTPUTS    : o Packets received(in networking order)         */
/*              o Total no. of bytes in buffer                  */
/*              o Number of packets read out                    */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reads one or more IPS packets that were received in the      */
/* Receive IPS buffer. The packets are put in the buffer in the */
/* same order(network order) as they are in the Rcv buffer.     */
/* Each packet has a 8-byte packet length header word which     */
/* has the actual length of the pkt(in bits 12:0, and pkt Error */
/* indication in bit 15).                                       */
/* Note: This API should be called only when IPS packets are    */
/* received in buffer(to CPU) and when TTL-based mode option is */
/* off. A different API is provided for TTL-based mode.         */
/*                                                              */
/* The [buf] is pointer to caller allocated memory.             */
/*                                                              */
/* The [pbuf_len] parameter serves two purposes:                */
/*  o For caller to let driver know the size of buf             */
/*  o For driver to update and let the caller know how much     */
/*    of it contains valid data(pkts + pkt length headers).     */
/* This length should be a multiple of 8 bytes.                 */
/*                                                              */
/* The [pnum_pkts] parameter is used by the driver to indicate  */
/* to the caller, the number of valid packets read out.         */
/*                                                              */
/* ----- IMPORTANT NOTE REGARDING BUFFER SIZE(pbuf_len)!! ----- */
/* This length should be a multiple of 8 bytes and MUST be the  */
/* same size as the IPS Rcv buffer(2048 bytes)! This is required*/
/* as SW cannot just read the packet hdr to find the new packet */
/* length but not read the packet itself due to insufficient    */
/* length- otherwise, the HEAD pointer gets auto-incremented to */
/* SOP of the next packet and the packet hdr entry could be     */
/* overwritten with new packet data.                            */
/*                                                              */
/* ----- IMPORTANT NOTE REGARDING PACKET LIMIT(pkt_limit)!! --- */
/* The [pkt_limit] parameter specifies the max number of pkts   */
/* and must be 0xFFFF. This will ensure that all packets in the */
/* buffer at the time of calling this API will be read out.     */
/* Otherwise, packets could be left behind without being read   */
/* out and the interrupt de-asserted. The interrupt is triggered*/
/* only on the arrival of a packet and can be cleared by SW     */
/* irrespective of whether there are packet(s) in the buffer or */
/* or not waiting to be read. This parameter is still there for */
/* downward compatibility of the driver for the existing .      */
/* customers.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_reg		data = 0 ;
  cs_uint16		head, tail, buf_len ;
  cs_uint16		i, num_words, pkt_bytes_to_process = 0 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  if (buf == NULL || pbuf_len == NULL || pnum_pkts == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_NULL_PTR,
		  ": buf(%p), pbuf_len(%p), pnum_pkts(%p)\n",
		  buf, pbuf_len, pnum_pkts) ;
    return (CS_ERROR) ;
  }

  if (*pbuf_len < M2R_RMAC_IPS_RX_BUF_SZ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": User IPS Rx buf_len(0x%x) s/b 0x%x bytes\n",
		  *pbuf_len, M2R_RMAC_IPS_RX_BUF_SZ) ;
    return (CS_ERROR) ;
  }

  if (pkt_limit != 0xFFFF) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": pkt_limit(0x%x) s/b 0xffff bytes\n",
		  pkt_limit) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  buf_len   = *pbuf_len ;
  CS_MEMSET( (void *)buf, 0, buf_len ) ;
  *pbuf_len = 0 ;
  *pnum_pkts = 0 ;

  head = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_HEAD.wrd) ;
  tail = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_TAIL.wrd) ;

  if (head == tail) {   /* no packet in the buffer */
    goto rtn_exit ;
  }

  num_words = (head > tail) ?
    ( (M2R_RMAC_IPS_RX_BUF_TOTAL_WORDS - head) + tail ) :
    (tail - head) ;

  /*
   * Steps for reading data out of the buffer
   * 1. Read DATA0 once & discard it(dummy read for auto-increment)
   * 2. Read DATA registers iteratively until all valid bytes
   *    of data currently in the Receive buffer are read.
   * 3. Keep track of the Head pointer, and before exiting update
   *    HEAD register to EOP of the last packet.
   */
  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA0.wrd) ;
  head = (head + 1) % M2R_RMAC_IPS_RX_BUF_TOTAL_WORDS ;

  for (i = 0; i < num_words; i++) {
    cs_uint16	pkt_len ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA3.wrd) ;
    buf[i*8 + 0] = (data >> 8) & 0xFF ;
    buf[i*8 + 1] = (data >> 0) & 0xFF ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA2.wrd) ;
    buf[i*8 + 2] = (data >> 8) & 0xFF ;
    buf[i*8 + 3] = (data >> 0) & 0xFF ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA1.wrd) ;
    buf[i*8 + 4] = (data >> 8) & 0xFF ;
    buf[i*8 + 5] = (data >> 0) & 0xFF ;

    data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA0.wrd) ;
    buf[i*8 + 6] = (data >> 8) & 0xFF ;
    buf[i*8 + 7] = (data >> 0) & 0xFF ;

    if (pkt_bytes_to_process <= 0) { /* new header */
      pkt_len = (data & M2R_RMAC_IPS_CTL_PKT_LEN_MASK) ;
      /* Packets are padded at the end - to align on 8-byte boundary */
      pkt_bytes_to_process = (pkt_len + 7) & 0xfff8 ;

      /* check if there is room in user buffer for another pkt */
      if ( pkt_bytes_to_process > (buf_len - *pbuf_len) ) {
	CS_PRINT("port-%d: User IPS pkt buffer FULL(num pkts = %d)\n",
		 port_id, *pnum_pkts) ;
	break ;
      }

      *pbuf_len += (8 + pkt_bytes_to_process) ;
      (*pnum_pkts)++ ;
    }

    else { /* packet bytes */
      pkt_bytes_to_process -= 8 ;
    }

    head = (head +1) % M2R_RMAC_IPS_RX_BUF_TOTAL_WORDS ;
  } /* for */

  /*
   * Make sure that hw HEAD pointer points to the EOP of the last
   * packet processed. This is needed for auto-mechanism to work
   * next time around.
   */
  head = ( (head == 0) ?
	   (M2R_RMAC_IPS_RX_BUF_TOTAL_WORDS - 1) : (head - 1) ) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_HEAD.wrd, head) ;

  rtn_exit :
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP IPS/CTL PACKETS FROM RECEIVE BUFFER     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_dump_ips_ctl_pkts_from_rxbuf(cs_uint16 port_id,
						 cs_boolean ips_pkts,
						 cs_boolean ctl_pkts)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(for IPS pkts) or FALSE(no action)        */
/*              o TRUE(for CTL pkts) or FALSE(no action)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves all the IPS and/or CTL packets from the respective */
/* receive buffer and prints them.                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint8	* buf ;
  cs_uint16	buf_len, bytes_read = 0 ;
  cs_uint16     pkt_num, num_pkts = 0 ;
  cs_boolean    ctl_user_buf_print = FALSE ;
  cs_boolean    ips_user_buf_print = FALSE ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  if (!ips_pkts && !ctl_pkts) {
    CS_PRINT("port-%d: No packets dumped - no selection made!\n",
	     port_id) ;
    return (CS_ERROR) ;
  }

  if (ips_pkts) {
    buf_len = M2R_RMAC_IPS_RX_BUF_SZ ;
    buf = (cs_uint8 *) CS_MALLOC(buf_len) ;
    if (buf == NULL) return (CS_ERROR) ;
    /* Use pkt_limit = 0xffff always */
    if (m2r_rmac_read_ips_pkts_from_rxbuf(port_id, buf, &buf_len,
					  &num_pkts, 0xffff)
	== CS_ERROR) {
      CS_PRINT("port-%d ERROR encountered: IPS num_pkts=%d\n",
	       port_id, num_pkts) ;
      CS_FREE(buf) ;
      return (CS_ERROR) ;
    }

    CS_PRINT("=============================================\n") ;
    CS_PRINT("\t TOTAL NUMBER OF IPS RECEIVED PKTS IN BUFFER = 0x%x\n",
	     num_pkts) ;

    /********* debug code *******/
    if (ips_user_buf_print) {
      cs_uint32  ii ;

      CS_PRINT("DEBUG: The IPS user buffer is: \n") ;
      CS_PRINT("*********** User Buffer start ***************\n") ;
      for (ii = 0; ii < buf_len; ii++) {
	if (ii % 8 == 0) CS_PRINT("\n0x%04x:\t", ii) ;
	CS_PRINT(" %02x", buf[ii]) ;
      }
      CS_PRINT("\n********* User Buffer end  **************\n") ;
    }
    /******** debug code *******/

    for (pkt_num = 0; pkt_num < num_pkts; pkt_num++) {
      m2r_print_rcvd_ips_ctl_pkt(port_id, TRUE, pkt_num, &buf[bytes_read],
				 &bytes_read) ;
    }

    CS_PRINT("=============================================\n") ;

    CS_FREE(buf) ;
  }


  if (ctl_pkts) {
    buf_len = M2R_RMAC_CTL_RX_BUF_SZ ;
    buf = (cs_uint8 *) CS_MALLOC(buf_len) ;
    bytes_read = 0 ;
    if (buf == NULL) return (CS_ERROR) ;
    if (m2r_rmac_read_ctl_pkts_from_rxbuf(port_id, buf, &buf_len,
					  &num_pkts, 0xffff)
	== CS_ERROR) {
      CS_PRINT("port-%d ERROR encountered: CTL num_pkts=%d\n",
	       port_id, num_pkts) ;
      CS_FREE(buf) ;
      return (CS_ERROR) ;
    }

    CS_PRINT("=============================================\n") ;
    CS_PRINT("\t TOTAL NUMBER OF CTL RECEIVED PKTS IN BUFFER = 0x%x\n",
	     num_pkts) ;

    /********* debug code *******/
    if (ctl_user_buf_print) {
      cs_uint32  ii ;

      CS_PRINT("DEBUG: The CTL user buffer is: \n") ;
      CS_PRINT("*********** User Buffer start ***************\n") ;
      for (ii = 0; ii < buf_len; ii++) {
	if (ii % 8 == 0) CS_PRINT("\n0x%04x:\t", ii) ;
	CS_PRINT(" %02x", buf[ii]) ;
      }
      CS_PRINT("\n********* User Buffer end  **************\n") ;
    }
    /******** debug code *******/

    for (pkt_num = 0; pkt_num < num_pkts; pkt_num++) {
      m2r_print_rcvd_ips_ctl_pkt(port_id, FALSE, pkt_num, &buf[bytes_read],
				 &bytes_read) ;
    }

    CS_PRINT("=============================================\n") ;

    CS_FREE(buf) ;
  }

  return (CS_OK) ;
}


/****************************************************************
 *******     IPS RECEIVE IN TTL-BASED MODE (for RPR)     ********
 ****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  INITIALIZE IPS RCV BUFFER FOR TTL MODE       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : MILAN2                                          */
cs_status  m2r_rmac_init_ips_buf_in_ttl_mode(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initialize IPS Receive Buffer for the parity bits to operate */
/* properly when in IPS TTL based mode.                         */
/* The buffer is 256 entries X 85 bits wide. Each entry is made */
/* of 80-bits of IPS data(SA and 4-byte payload), and 5 bits of */
/* parity(1 parity bit for each 16-bits of data).               */
/* For the data and parity bits to operate properly they need   */
/* to be explicitly set before they are used.                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  return (m2r_rmac_clear_ips_buf_entries(port_id, 0, 256)) ;
}


/****************************************************************/
/* $rtn_hdr_start  CLEAR IPS RCV BUFFER ENTRY(ies) IN TTL MODE  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : MILAN2                                          */
cs_status  m2r_rmac_clear_ips_buf_entries(cs_uint16 port_id,
					  cs_uint16 start,
					  cs_uint16 num_entries)
/* INPUTS     : o Port Id                                       */
/*              o Starting entry num [0...255]                  */
/*              o Number of entries [1..256]                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Clear specific IPS buffer entry(or entries).                 */
/* This API is only needed when in IPS in TTL mode is enabled.  */
/*                                                              */
/* This API is provided if the upper layer software needs to    */
/* clear specific entry(ies) in the IPS receive buffer in order */
/* to re-build the topology data-base or for other reasons.     */
/*                                                              */
/* NOTE: Since the software clearing of this entry is           */
/* asynchronous to the event of detection a new source(TP frame */
/* received), the calling software should take care in handling */
/* the following situations to avoid any unpredictable results: */
/*                                                              */
/* 1) A potential race conditon exists! Although clearing of    */
/* the a single buffer entry by software or hardware updating is*/
/* atomic, the race conditon can still happen. For eg., the hw  */
/* indicates a new source(ie. TP with a different protection    */
/* message is received)interrupt but sw doesn't handle it       */
/* immediately but in the process of clearing the entries clears*/
/* the new source message also which will leave the interrupt   */
/* still pending but the new message is all zeroes. So, one     */
/* possible way for software to deal with this is for it to     */
/* ignore the all-zeroes message and handle it when the next TP */
/* frame is received from the same station.                     */
/*                                                              */
/* 2) Any processing of the buffer(clearing or retrieving new   */
/* message) should be viewed as operation on a critical section */
/* and the caller is responsible in ensuring that context switch*/
/* at safe places(ie. not in the middle of processing). The     */
/* reason for this is both, clearing and retrieving new message */
/* work on the same set of buffer registers(head/tail pointers, */
/* and data) and they need to be done in an atomic fashion.     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_uint32		ii ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;
  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
                            256,__FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  for (ii = start; ii < (start+num_entries); ii++) {
    /*
     * Ensure HEAD = TAIL as otherwise, the HEAD pointer gets
     * pre-incremented  and entry (ii+1) will be cleared instead.
     */
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_HEAD.wrd,
		 ii & 0xff) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_TAIL.wrd,
		 ii & 0xff) ;

    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA4.wrd, 0) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA3.wrd, 0) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA2.wrd, 0) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA1.wrd, 0) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA0.wrd, 0) ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE IPS TTL BASED MODE                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_enable_ips_ttl_mode(cs_uint16 port_id,
					cs_boolean ttl_mode)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(enable IPS TTL mode) or FALSE(disable)   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable control of IPS TTL based mode for RPR.        */
/* In this mode, only the SA(6-bytes) and IPS message(4 bytes)  */
/* are extracted from the IPS packet and saved in the buffer.   */
/* This mode is active only when the IPS packet is to be        */
/* received in the IPS buffer.                                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_TBD_IPS_CTL_CR_t   cr ;
  m2r_port_mac_mode_t	mac_mode ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;

  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return (CS_ERROR) ;
  }

  if ( (ttl_mode) &&
       (mac_mode == M2R_PORT_IN_SRP_MODE) ) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_SRP, NULL) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd) ;
  cr.bf.TTL_MODE = (ttl_mode) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd,
	       cr.wrd) ;

  if (ttl_mode) { /* initialize IPS Receive buffer */
    if (m2r_rmac_init_ips_buf_in_ttl_mode(port_id)
	== CS_ERROR) {
      M2R_PORT_UNLOCK (port_id, M2R_ID_RMAC) ;
      return (CS_ERROR) ;
    }
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    SET IPS TTL MASKS(in TTL Mode)             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : MILAN2                                          */
cs_status  m2r_rmac_set_ips_ttl_masks(cs_uint16 port_id,
				      cs_boolean ttl_0_mask,
				      cs_boolean ttl_max_mask,
				      cs_uint8 ttl_max_stations)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(to mask TTL-0) or FALSE                  */
/*              o TRUE(to mask max stations) or FALSE           */
/*              o Max stations[0..255]                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets up the TTL masks which are used when reporting          */
/* IPS inactivity(ie. NO_IPS interrupt on expiration of the     */
/* IPS timer.                                                   */
/*                                                              */
/* The [ttl_0_mask] parameter when specified to TRUE, will mask */
/* OFF the checking for TTL=0.                                  */
/* The [ttl_max_mask] parameter when specified to TRUE, will    */
/* mask OFF the checking for TTL > ttl_max_stations.            */
/*                                                              */
/* The [ttl_max_stations] parameter is specified as per the     */
/* number of stations on the ring.                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_TBD_STATIONS_t	stations ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id,
			   (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  stations.wrd = 0 ;
  stations.bf.MASK_ZERO = (ttl_0_mask) ? 1 : 0 ;
  stations.bf.MASK_MAX  = (ttl_max_mask) ? 1 : 0 ;
  stations.bf.MAX       = ttl_max_stations ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.STATIONS.wrd,
	       stations.wrd) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    SET IPS TIMER(in TTL Mode)                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_set_ips_timer(cs_uint16 port_id,
				  cs_uint32 tmr_cnt)
/* INPUTS     : o Port Id                                       */
/*              o Protection Timer counter(32-bit value)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the IPS Protection Timer value, which is used for       */
/* detecting if one or more IPS packets have NOT been received  */
/* from any of the valid sources before the timer times out.    */
/* The timer counts by 1 for every 10 core clock cycles.        */
/* For Milan2: Core clock is 200 MHz                            */
/* When it times out, TBD_NOIPS interrupt bit will be set.      */
/* Note: A value of 0 will DISABLE the timer.                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  m2r_port_mac_mode_t	mac_mode ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;

  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return (CS_ERROR) ;
  }

  if ( (tmr_cnt) &&
       (mac_mode == M2R_PORT_IN_SRP_MODE) ) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_SRP, NULL) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PROT_TIMER0.wrd,
	       tmr_cnt & 0xffff) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PROT_TIMER1.wrd,
	       tmr_cnt >> 16) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  CHECK FOR IPS NEW MESSAGE(in TTL Mode)       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Milan2                                          */
cs_boolean  m2r_rmac_is_ips_new_msg_rcvd(cs_uint16 port_id,
					 cs_uint8 * pnew)
/* INPUTS     : o Port Id                                       */
/*              o New IPS Source address(pointer)               */
/* OUTPUTS    : Node source address                             */
/* RETURNS    : TRUE(new message) or FALSE(no messages)         */
/* DESCRIPTION:                                                 */
/* Checks if there are one or more new IPS messages received in */
/* IPS-TTL mode.                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_TBD_IPS_CTL_CR_t   cr ;
  cs_boolean		ret ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, FALSE) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id,
			   (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     FALSE) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd) ;
  *pnew = cr.bf.IPS_NEW_SRC ;

  ret = (cr.bf.IPS_NEW) ? TRUE : FALSE ;
  return (ret) ;
}

/****************************************************************/
/* $rtn_hdr_start      GET IPS NEW SOURCE(in TTL Mode)          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint16  m2r_rmac_get_ips_new_src(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : 0...255(if new msg) or -1(if no new msg)        */
/* DESCRIPTION:                                                 */
/* Returns the source address of the first IPS message          */
/* received(or -1 if there is none). This is valid only in      */
/* in IPS-TTL mode.                                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint8  new_src ;

  if ( m2r_rmac_is_ips_new_msg_rcvd(port_id, &new_src) ) {
    return( (cs_uint16) new_src ) ;
  }
  else {
    return ( (cs_uint16) -1 ) ;
  }
}


/****************************************************************/
/* $rtn_hdr_start     READ IPS NEW MESSAGE(in TTL MODE)         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_read_ips_new_msg(cs_uint16 port_id, cs_uint8 new_src,
				     m2r_ips_new_msg_t * pmsg)
/* INPUTS     : o Port Id                                       */
/*              o New IPS source[0...255]                       */
/*              o Pointer to IPS message struct                 */
/* OUTPUTS    : MAC SA(6 Bytes) and payload(4 Bytes)            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reads the new IPS message from the IPS buffer.               */
/* This is valid only in IPS-TTL mode.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_uint8		head, tail ;
  cs_uint32		data = 0 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id,
			   (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  head = new_src - 1 ; /* one less for auto-increment */
  tail = new_src + 1 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_HEAD.wrd, head) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IPS_RCV_TAIL.wrd, tail) ;

  /* Do dummy read for auto-increment */
  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA0.wrd) ;

  /* Get IPS Source MAC address */
  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA4.wrd) ;
  pmsg->mac_sa.byte5 = (data >> 8) & 0xff ;
  pmsg->mac_sa.byte4 = data & 0xff ;

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA3.wrd) ;
  pmsg->mac_sa.byte3 = (data >> 8) & 0xff ;
  pmsg->mac_sa.byte2 = data & 0xff ;

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA2.wrd) ;
  pmsg->mac_sa.byte1 = (data >> 8) & 0xff ;
  pmsg->mac_sa.byte0 = data & 0xff ;

  /* Get IPS Payload(4 bytes) */
  pmsg->payload = 0 ;
  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA1.wrd) ;
  pmsg->payload |= (data << 16) ;

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_RCV_DATA0.wrd) ;
  pmsg->payload |= data ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    DUMP IPS NEW MESSAGE(in TTL MODE)          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_dump_ips_new_msg(cs_uint16 port_id,
				     cs_uint8 new_src)
/* INPUTS     : o Port Id                                       */
/*              o New IPS source[0...255]                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the new IPS message and prints it.                 */
/* This is valid only in IPS-TTL mode.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_ips_new_msg_t 	new_msg  ;

  if ( m2r_rmac_read_ips_new_msg(port_id, new_src, &new_msg)
       == CS_ERROR) {
    return (CS_ERROR) ;
  }

  CS_PRINT(
  "Port-%d Prot msg(0x%08x) rcvd from src(TTL=%d):%02x.%02x.%02x.%02x.%02x.%02x\n",
       port_id, new_msg.payload, new_src,
       new_msg.mac_sa.byte5,
       new_msg.mac_sa.byte4,
       new_msg.mac_sa.byte3,
       new_msg.mac_sa.byte2,
       new_msg.mac_sa.byte1,
       new_msg.mac_sa.byte0) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start      RAW DUMP OF IPS/CTL BUFFER               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : MILAN2                                          */
cs_status  m2r_rmac_debug_dump_buf(cs_uint16 port_id,
				   cs_uint32 pkt_sel,
				   cs_uint32 buf_sel,
				   cs_uint32 start, cs_uint32 num_entries)
/* INPUTS     : o Port Id                                       */
/*              o Control(0) or IPS(1) packet selection         */
/*              o RECEIVE buffer(0) or SEND buffer(1)           */
/*              o Starting entry number(entry size = 8 bytes)   */
/*              o Number of entries                             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This does the raw dumping of the selected buffer.            */
/* The max number of entries(8-byte words) are,                 */
/* CTL SEND BUFFER - 1024                                       */
/* CTL RCV  BUFFER - 1024                                       */
/* IPS SEND BUFFER -   64                                       */
/* IPS RCV  BUFFER -  256                                       */
/* This API is for debug purpose ONLY!                          */
/*                                                              */
/* NOTE -                                                       */
/* This API assumes that when it is called that no packets are  */
/* being received or sent out of this buffer. Otherwise, the    */
/* contents of the dump will be unpredictable as the TAIL ptr   */
/* could get updated by hardware while software thinks that is  */
/* in its control.                                              */
/*                                                              */
/* Also, when in IPS in TTL mode and when dumping IPS RCV buffer*/
/* there is a possiblity to miss a new src event as this API    */
/* reads DR_TBD_IPS_CTL_CR register which pops one new src off  */
/* hardware's new src pending list.                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t         * pDev  ;
  cs_uint16     dev_num, sl_num, dummy_entry ;
  cs_uint32     i, data, save_head, save_tail, sz ;
  volatile      cs_uint16    * head_reg, * tail_reg ;
  volatile      cs_uint16    * data0_reg, * data1_reg,
                             * data2_reg, * data3_reg,
                             * data4_reg ;
  char * str = NULL ;
  DR_TBD_IPS_CTL_CR_t   cr ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (buf_sel == M2R_RMAC_RX_BUF) { /* RECEIVE buffer */
    if (pkt_sel == M2R_RMAC_CTL_PKT) {
      sz        = M2R_RMAC_CTL_RX_BUF_TOTAL_WORDS ;
      head_reg  = &pDev->slice[sl_num].RingMAC.CTL_RCV_HEAD.wrd ;
      tail_reg  = &pDev->slice[sl_num].RingMAC.CTL_RCV_TAIL.wrd ;
      data0_reg = &pDev->slice[sl_num].RingMAC.CTL_RCV_DATA0.wrd ;
      data1_reg = &pDev->slice[sl_num].RingMAC.CTL_RCV_DATA1.wrd ;
      data2_reg = &pDev->slice[sl_num].RingMAC.CTL_RCV_DATA2.wrd ;
      data3_reg = &pDev->slice[sl_num].RingMAC.CTL_RCV_DATA3.wrd ;
      data4_reg = NULL ; /* only 8 bytes of data per entry */
      str       = "CTL RCV BUFFER" ;
    }
    else { /* M2R_RMAC_IPS_PKT */
      sz        = M2R_RMAC_IPS_RX_BUF_TOTAL_WORDS ;
      head_reg  = &pDev->slice[sl_num].RingMAC.IPS_RCV_HEAD.wrd ;
      tail_reg  = &pDev->slice[sl_num].RingMAC.IPS_RCV_TAIL.wrd ;
      data0_reg = &pDev->slice[sl_num].RingMAC.IPS_RCV_DATA0.wrd ;
      data1_reg = &pDev->slice[sl_num].RingMAC.IPS_RCV_DATA1.wrd ;
      data2_reg = &pDev->slice[sl_num].RingMAC.IPS_RCV_DATA2.wrd ;
      data3_reg = &pDev->slice[sl_num].RingMAC.IPS_RCV_DATA3.wrd ;
      data4_reg = &pDev->slice[sl_num].RingMAC.IPS_RCV_DATA4.wrd ;
      str       = "IPS RCV BUFFER" ;
    }
  }

  else {        /* SEND buffer */
    if (pkt_sel == M2R_RMAC_CTL_PKT) {
      sz        = M2R_RMAC_CTL_SEND_BUF_TOTAL_WORDS ;
      head_reg  = &pDev->slice[sl_num].RingMAC.CTL_SND_HEAD.wrd ;
      tail_reg  = NULL ;
      data0_reg = &pDev->slice[sl_num].RingMAC.CTL_SND_DATA0.wrd ;
      data1_reg = &pDev->slice[sl_num].RingMAC.CTL_SND_DATA1.wrd ;
      data2_reg = &pDev->slice[sl_num].RingMAC.CTL_SND_DATA2.wrd ;
      data3_reg = &pDev->slice[sl_num].RingMAC.CTL_SND_DATA3.wrd ;
      data4_reg = NULL ; /* only 8 bytes of data per entry */
      str       = "CTL SEND BUFFER" ;
    }
    else { /* M2R_RMAC_IPS_PKT */
      sz        = M2R_RMAC_IPS_SEND_BUF_TOTAL_WORDS ;
      head_reg  = &pDev->slice[sl_num].RingMAC.IPS_SND_HEAD.wrd ;
      tail_reg  = NULL ;
      data0_reg = &pDev->slice[sl_num].RingMAC.IPS_SND_DATA0.wrd ;
      data1_reg = &pDev->slice[sl_num].RingMAC.IPS_SND_DATA1.wrd ;
      data2_reg = &pDev->slice[sl_num].RingMAC.IPS_SND_DATA2.wrd ;
      data3_reg = &pDev->slice[sl_num].RingMAC.IPS_SND_DATA3.wrd ;
      data4_reg = NULL ; /* only 8 bytes of data per entry */
      str       = "IPS SEND BUFFER" ;
    }
  }

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
                            sz,__FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  save_head = M2R_REG_READ(head_reg) ;
  if (tail_reg) save_tail = M2R_REG_READ(tail_reg) ;

  /* If IPS Rcv buffer in TTL mode is enabled, then need data4 reg also */
  cr.wrd = 0 ;
  if (data4_reg) {
    cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd) ;
  }

  /* Do dummy read - read and discard to get auto-increment to work */
  dummy_entry = (start == 0) ? (sz - 1) : ((start % sz) - 1) ;
  M2R_REG_WRITE(head_reg, dummy_entry) ;
  if (tail_reg) {
    /* Have Tail set to one entry prior to Head */
    M2R_REG_WRITE(tail_reg,
		 (dummy_entry == 0) ? (sz - 1) : (dummy_entry-1)) ;
  }
  data = M2R_REG_READ(data0_reg) ; /* dummy read */
  if (tail_reg) {
    /* Now Head has auto-incremented, have Tail point to dummy entry*/
    M2R_REG_WRITE(tail_reg, dummy_entry) ;
  }


  CS_PRINT("\n Raw dump of %s\n", str) ;
  for (i = start; i < start+num_entries; i++) {
    cs_uint32   entry_num ;

    entry_num = i % sz ;

    CS_PRINT("entry(0x%04x):  ", entry_num) ;
    if (data4_reg && cr.bf.TTL_MODE) { /* IPS/TTL mode: 10-byte entry */
      data = M2R_REG_READ(data4_reg) ;
      CS_PRINT("%02x %02x ", data >> 8, data & 0xff) ;
    }
    data = M2R_REG_READ(data3_reg) ;
    CS_PRINT("%02x %02x ", data >> 8, data & 0xff) ;
    data = M2R_REG_READ(data2_reg) ;
    CS_PRINT("%02x %02x ", data >> 8, data & 0xff) ;
    data = M2R_REG_READ(data1_reg) ;
    CS_PRINT("%02x %02x ", data >> 8, data & 0xff) ;
    data = M2R_REG_READ(data0_reg) ;
    CS_PRINT("%02x %02x ", data >> 8, data & 0xff) ;
    CS_PRINT("\n") ;
    if ( (i % 0x10) == 0x0f ) CS_PRINT("\n") ;
  }

  M2R_REG_WRITE(head_reg, save_head) ;
  if (tail_reg) M2R_REG_WRITE(tail_reg, save_tail) ;

  return (CS_OK) ;
}


/***********************************************************
 *     INTERNAL UTILITIES AND FUNCTIONS                    *
 ***********************************************************/

/***********************************************************
 *      Utility to print the Received IPS/CTL packet.      *
 ***********************************************************/
static
void m2r_print_rcvd_ips_ctl_pkt(cs_uint16 port_id, cs_boolean ips_pkt,
				cs_uint32 pkt_num, cs_uint8 * buf,
				cs_uint16 * pbytes_read)
{
  cs_uint16     i, hdr, pkt_len, max_len ;

  /* buf[0...3] is reserved field in the header */
  hdr = ( (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7] ) ;
  pkt_len = hdr & M2R_RMAC_IPS_CTL_PKT_LEN_MASK ;

  CS_PRINT("\tport-%d: Received %s packet 0x%x (len= 0x%x %s)\n",
	   port_id, (ips_pkt) ? "IPS" : "CTL", pkt_num, pkt_len,
	   (hdr & M2R_RMAC_IPS_CTL_PKT_ERR_MASK) ? "Error bit set" :
	   "") ;
  max_len = ((ips_pkt) ? M2R_RMAC_IPS_RX_BUF_SZ
	     : M2R_RMAC_CTL_RX_BUF_SZ) ;
  if ( (pkt_len == 0) || (pkt_len > max_len) ) {
    CS_PRINT(
     "ERROR: %s packet in buffer has INVALID length(0x%x bytes)!!!\n",
     (ips_pkt) ? "Rcvd IPS" : "Rcvd CTL", pkt_len) ;
    return ;
  }

  for (i = 0; i < (pkt_len + 8); i++) {
    if (i % 8 == 0) CS_PRINT("\n0x%04x:\t", i) ;
    CS_PRINT(" %02x", buf[i]) ;
    if (i ==  7) CS_PRINT("  <--- pkt length hdr") ;
    if (i == 15) CS_PRINT("  <--- pkt start") ;
  }

  CS_PRINT("\n\n") ;

  *pbytes_read += ( 8 + pkt_len ) ; /* header + payload */
  if (*pbytes_read % 8) { /* need to be on 8-byte boundary */
    *pbytes_read += (8 - (*pbytes_read % 8)) ;
  }
}


/********************************************************************
 * Debug utility to receiving and sending Control packets via
 * the CPU buffer.
 * 1) Check if packet(s) are waiting in the CTL receive buffers.
 * 2) If yes, retrieve them into a local buffer.
 * 3) Send them back out by writing into the CTL Send buffer.
 *
 * NOTE: This is for debug/testing purpose only.
 *********************************************************************/
cs_status  m2r_rmac_debug_send_rcvd_cpu_ctl_pkts(cs_uint16 port_id)
{
  static cs_uint8   ctl_buf[ M2R_RMAC_CTL_RX_BUF_SZ ] ;
  cs_uint16	    ctl_buf_len = M2R_RMAC_CTL_RX_BUF_SZ ;
  cs_uint16         ctl_num_pkts = 0 ;
  cs_uint32	    hdr, word_num = 0,  bytes_processed = 0 ;

  /* CTL packets */
  if ( m2r_rmac_read_ctl_pkts_from_rxbuf(port_id,
					 ctl_buf, &ctl_buf_len,
					 &ctl_num_pkts, 0xffff)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( ctl_num_pkts == 0 ) {
    /* no packets received */
    return (CS_OK) ;
  }

  while (bytes_processed < ctl_buf_len) {
    cs_uint32    pkt_len, num_pkt_words ;
    cs_uint8	 * pPkt ;

    /* process packet length header */
    hdr = ( (ctl_buf[ (word_num * 8) + 6 ] << 8 |
	     (ctl_buf[ (word_num * 8) + 7 ] & 0xff)) ) ;
    pkt_len = (hdr & M2R_RMAC_IPS_CTL_PKT_LEN_MASK) ;
    num_pkt_words = (pkt_len + 7) / 8 ;
    pPkt = &ctl_buf[ (word_num + 1) * 8 ] ;

    /* process packet */
    if ( m2r_rmac_send_ctl_pkt(port_id, pPkt, pkt_len) == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    word_num += (1 + num_pkt_words)  ;
    bytes_processed += ( (num_pkt_words + 1) * 8 ) ;
  }

  return (CS_OK) ;
}


/********************************************************************
 * Debug utility to receiving and sending IPS packets via
 * the CPU buffer.
 * 1) Check if packet(s) are waiting in the IPS receive buffers.
 * 2) If yes, retrieve them into a local buffer.
 * 3) Send them back out by writing into the IPS Send buffer.
 *
 * NOTE: This is for debug/testing purpose only.
 *********************************************************************/
cs_status  m2r_rmac_debug_send_rcvd_cpu_ips_pkts(cs_uint16 port_id)
{
  static cs_uint8   ips_buf[ M2R_RMAC_IPS_RX_BUF_SZ ] ;
  cs_uint16	    ips_buf_len = M2R_RMAC_IPS_RX_BUF_SZ ;
  cs_uint16         ips_num_pkts = 0 ;
  cs_uint32	    hdr, word_num = 0,  bytes_processed = 0 ;

  /* IPS packets */
  if ( m2r_rmac_read_ips_pkts_from_rxbuf(port_id,
					 ips_buf, &ips_buf_len,
					 &ips_num_pkts, 0xffff)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( ips_num_pkts == 0 ) {
    /* no packets received */
    return (CS_OK) ;
  }

  while (bytes_processed < ips_buf_len) {
    cs_uint32    pkt_len, num_pkt_words ;
    cs_uint8	 * pPkt ;

    /* process packet length header */
    hdr = ( (ips_buf[ (word_num * 8) + 6 ] << 8 |
	     (ips_buf[ (word_num * 8) + 7 ] & 0xff)) ) ;
    pkt_len = (hdr & M2R_RMAC_IPS_CTL_PKT_LEN_MASK) ;
    num_pkt_words = (pkt_len + 7) / 8 ;
    pPkt = &ips_buf[ (word_num + 1) * 8 ] ;

    /* process packet */
    if ( m2r_rmac_send_ips_pkt(port_id, pPkt, pkt_len) == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    word_num += (1 + num_pkt_words) ;
    bytes_processed += ( (num_pkt_words + 1) * 8 ) ;
  }

  return (CS_OK) ;
}


/********************************************************************
 * Debug utility to look for a new IPS message and dump it.
 ********************************************************************/
void m2r_rmac_check_and_print_new_ips_msg(cs_uint16 port_id)
{
  cs_uint16  new_src ;

  new_src = m2r_rmac_get_ips_new_src(port_id) ;

  if ( new_src == (cs_uint16)-1 ) {
    return ;
  }

  m2r_rmac_dump_ips_new_msg(port_id, new_src) ;
}









