/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h" 
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_rmac.h"
#include "m2r_rmac_irq.h"
#include "m2r_rmac_priv.h"

#include "m2r_rmac_xgxs_stats.h"


/****************************************************************/
/* $rtn_hdr_start  RINGMAC BLOCK SOFT RESET                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_soft_reset(cs_uint16 port_id, cs_uint32 sel,
                               m2r_reset_action_t act)
/* INPUTS     : o Port Id                                       */
/*              o Block Id                                      */
/*              o Reset control                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Soft reset control of the RingMac Block.                     */
/*                                                              */
/* The [sel] parameter indicates the block selection and is     */
/* one of the following:                                        */
/* M2R_RMAC_SRESET_MATE,              M2R_RMAC_SRESET_MS        */
/* M2R_RMAC_SRESET_ALL                                          */
/*                                                              */
/* The [act] parameter specifies what kind of reset to be done  */
/* and is one of the following:                                 */
/* M2R_RESET_DEASSERT, M2R_RESET_ASSERT, or                     */
/* M2R_RESET_TOGGLE.                                            */
/* The M2R_RESET_TOGGLE option will assert reset, hold the      */
/* reset for a while and then de-assert the reset bit(s).       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_SRESET_t  sreset ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if ( (sel != M2R_RMAC_SRESET_MATE)
      && (sel != M2R_RMAC_SRESET_MS)
      && (sel != M2R_RMAC_SRESET_ALL) ) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_SRESET_SEL, ": sel= 0x%x\n",
                  sel) ;
    return(CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  sreset.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SRESET.wrd) ;

  /* always use sticky way to perform the soft reset */
  sreset.bf.STICKY_RESET = 1;

  switch (act) {
    case M2R_RESET_DEASSERT :
      /* clear the soft reset bit(s) */
      sreset.wrd &= ~sel ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SRESET.wrd, sreset.wrd) ;
      break ;

    case M2R_RESET_ASSERT :
      /* set the soft reset bit(s) */
      sreset.wrd |= sel ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SRESET.wrd, sreset.wrd) ;
      break ;

    case M2R_RESET_TOGGLE :
      /* set, wait for a few msecs, and clear hard reset bit(s) */
      sreset.wrd |= sel ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SRESET.wrd, sreset.wrd) ;

#ifndef RELEASE_PLATFORM
      if (!IN_SIM_ENV()) {
        CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
      }
#else
      CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
#endif

      sreset.wrd &= ~sel ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SRESET.wrd, sreset.wrd) ;

#ifndef RELEASE_PLATFORM
      if (!IN_SIM_ENV()) {
        CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
      }
#else
      CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
#endif

      break ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONTROL OF RINGMAC LOCK                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_lock(cs_uint16 port_id, cs_boolean lock)         
/* INPUTS     : o Port Id                                       */
/*              o TRUE(to lock) or FALSE(to unlock)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls lock/unlock of RingMAC's protected registers.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (lock) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.LOCK.wrd, LOCK_LOCK) ;
    CS_PRINT("Port-%d: RingMAC protected registers are LOCKED!\n",
	     port_id) ;
  }
  else {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.LOCK.wrd, LOCK_UNLOCK) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start     SET RING-ID                               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_ring_id(cs_uint16 port_id, cs_uint8 ring_id,
				cs_boolean check_ring_id) 
/* INPUTS     : o Port Id                                       */
/*              o Ring-Id                                       */
/*              o TRUE(check RI before DA lookup) or FALSE      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the ring-id for the specified port. The ring-id         */
/* indicates which ringlet the packets are received from.       */
/*                                                              */
/* The [check_ring_id] parameter specifies whether to detect    */
/* a RING_ERR interrupt or not when packet is received on the   */
/* wrong ringlet or not(when the node is not in wrap mode).     */
/*                                                              */
/* Note on [check_ring_id] parameter:                           */
/* The ONLY effect of this check here is for notificiation and  */
/* any decision on the packet flow is NOT made at this point!   */  
/* Further ring-id checking happens after this initial check    */
/* here - refer to the Data Packet Decision Flowchart in the    */
/* spec for more info.                                          */
/* This checking happens BEFORE doing any address lookup's.     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{ 
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR2_t	cr2 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (ring_id != 0 && ring_id != 1 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		  " :ring_id(%d) s/b 0 or 1\n", ring_id) ; 
    return(CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  cr2.bf.RING_ID     = ring_id ;
  cr2.bf.CHK_RING_ID = (check_ring_id) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start     SET RING-MODE                             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_ring_mode(cs_uint16 port_id, 
				  m2r_ring_mode_t  ring_mode)
/* INPUTS     : o Port Id                                       */
/*              o Ring(MAC) mode                                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets RingMAC in one of the following modes:                  */
/* M2R_RING_NORMAL,                M2R_RING_PASSTHROUGH,        */
/* M2R_RING_WRAP,                  M2R_RING_TRANSPARENT,        */
/* M2R_RING_SNIFFER_PASSIVE,    or M2R_RING_SNIFFER_ACTIVE      */ 
/*                                                              */
/* Note: After driver initialization, the ring mode is set to   */
/* M2R_RING_PASSTHROUGH mode. This API should be called to      */
/* put the mode into M2R_RING_NORMAL, when the station(this port*/
/* and the mate) is ready for normal operations on the ring.    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR0_t	cr0 ;
  cs_uint16		prev_ring_mode ;
  m2r_port_mac_mode_t	mac_mode ;
  
  mac_mode = M2R_GET_MAC_MODE(port_id) ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
  prev_ring_mode = cr0.bf.MODE ; 
  switch (ring_mode) {
    case M2R_RING_NORMAL :
      if ( (mac_mode == M2R_PORT_IN_RPR_MODE) && 
	   (prev_ring_mode == MODE_WRAP) ) { 	
	m2r_rmac_ctl_ws_for_pkts_from_host(port_id, TRUE, 0) ;
      }
      cr0.bf.MODE = MODE_NORMAL ;
      m2r_rmac_ctl_we_in_wrap(port_id, TRUE, FALSE) ;
      break ;

    default : 
    case M2R_RING_PASSTHROUGH : 
      if ( (mac_mode == M2R_PORT_IN_RPR_MODE) &&
	   (prev_ring_mode == MODE_WRAP) ) { 	
	m2r_rmac_ctl_ws_for_pkts_from_host(port_id, TRUE, 0) ;
      }      
      cr0.bf.MODE = MODE_PASSTHROUGH ; break ;

    case M2R_RING_WRAP :
      /* need to set WS bit = 1 for wrap mode */
      m2r_rmac_ctl_ws_for_pkts_from_host(port_id, TRUE, 1) ;
      cr0.bf.MODE = MODE_WRAP ; 
      m2r_rmac_ctl_we_in_wrap(port_id, TRUE, FALSE) ;
      break ;

    case M2R_RING_TRANSPARENT : 
      cr0.bf.MODE = MODE_TRANSPARENT ; 
      /* 
       * In Transparent mode, host CRC handling could be asymmetric
       * if we strip CRC on Rx since it does not generate CRC on 
       * Tx(irrespective of the corresponding control bit set to do
       * that). So, disable Rx CRC stripping(as well as Tx CRC 
       * insertion).
       */
      m2r_rmac_host_crc_strip_or_insert(port_id, FALSE, FALSE) ;
      CS_PRINT("port-%d NOTE: Host CRC handling is DISABLED!\n",
	       port_id) ;
      break ;
    case M2R_RING_SNIFFER_PASSIVE : 
      cr0.bf.MODE = MODE_SNIFFER_PASSIVE ; break ;
    case M2R_RING_SNIFFER_ACTIVE : 
      cr0.bf.MODE = MODE_SNIFFER_ACTIVE ; break ;
  }
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start     GET CURRENT RING-MODE                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
m2r_ring_mode_t  m2r_rmac_get_ring_mode(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Current, actual ring mode of operation          */
/* DESCRIPTION:                                                 */
/* Gets current RingMAC mode of operation, and is one of the    */
/* following:                                                   */
/* M2R_RING_NORMAL,                M2R_RING_PASSTHROUGH,        */
/* M2R_RING_WRAP,                  M2R_RING_TRANSPARENT,        */
/* M2R_RING_SNIFFER_PASSIVE,    or M2R_RING_SNIFFER_ACTIVE      */ 
/*                                                              */
/* Note: This retrieves the CURRENT Ring mode of operation.     */
/* It may not necessarily be what was configured, as the mode   */
/* change could happen due to watch-dog timing out, mate's      */
/* mode of operation changing, etc..                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_MODE_OP_t mode_op ;
  m2r_ring_mode_t	md = M2R_RING_PASSTHROUGH ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, md) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, md) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  mode_op.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MODE_OP.wrd) ;
  switch(mode_op.wrd) {
    case 0x01 : md = M2R_RING_NORMAL ;          break ;
    default: 
    case 0x02 : md = M2R_RING_PASSTHROUGH ;     break ;
    case 0x04 : md = M2R_RING_WRAP ;            break ;
    case 0x08 : md = M2R_RING_TRANSPARENT ;     break ; 
    case 0x10 : md = M2R_RING_SNIFFER_PASSIVE ; break ; 
    case 0x20 : md = M2R_RING_SNIFFER_ACTIVE ;  break ;
  }

  return (md) ;
}

/****************************************************************/
/* $rtn_hdr_start  CONTROL AUTOMATIC HARDWARE MODE CHANGE       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_hw_ctl_ring_mode(cs_uint16 port_id, 
				  m2r_ctl_t hw_wrap_off,
				  m2r_ctl_t hw_wrap_on,
				  m2r_ctl_t hw_pass_off,
				  m2r_ctl_t hw_pass_on)
/* INPUTS     : o Port Id                                       */
/*              o Enable/Disable auto getting out of WRAP mode  */
/*              o Enable/Disable auto entering WRAP mode        */
/*              o Enable/Disable auto getting out of PASSTHRU   */
/*              o Enable/Disable auto entering PASSTHRU mode    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the ability to go into and out of ring modes of         */
/* M2R_RING_WRAP and M2R_RING_PASSTHROUGH by hardware when      */
/* certain conditions are detected.                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR1_t	cr1 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;

  cr1.bf.HW_WRAP_OFF = (hw_wrap_off == M2R_ENABLE) ? 1 : 0 ;
  cr1.bf.HW_WRAP_ON = (hw_wrap_on == M2R_ENABLE) ? 1 : 0 ;
  cr1.bf.HW_PASS_OFF = (hw_pass_off == M2R_ENABLE) ? 1 : 0 ;
  cr1.bf.HW_PASS_ON = (hw_pass_on == M2R_ENABLE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start     CONTROL OF TTL DECREMENTING               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_ttl_action(cs_uint16 port_id, 
				   m2r_ttl_action_t ttl_action)
/* INPUTS     : o Port Id                                       */
/*              o TTL decrement action                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the type of TTL decrementing to one of the following:   */ 
/* M2R_DECR_TTL_ALWAYS      - Always decrement the TTL          */
/* M2R_DECR_TTL_CORRECT_RI  - decrement TTL only on correct     */
/*                            Ring-Id                           */ 
/* M2R_DECR_TTL_CORRECT_RI_OR_NODE_WRAP - Decrement on correct  */
/*                            RI or if the node is wrapped.     */ 
/* M2R_TTL_UNCHANGED        - Never decrement TTL               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR2_t	cr2 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  switch (ttl_action) {
   default : 
   case M2R_DECR_TTL_ALWAYS : 
     cr2.bf.TTL = TTL_TTL_DEC_ALWAYS ; break ;
   case M2R_DECR_TTL_RINGID :
     cr2.bf.TTL = TTL_TTL_DEC_RINGID ; break ;
   case M2R_DECR_TTL_RINGID_OR_NODE_WRAP :
     cr2.bf.TTL = TTL_TTL_DEC_RINGID_OR_WRAP ; break ;
   case M2R_TTL_UNCHANGED :
     cr2.bf.TTL = TTL_TTL_DEC_NEVER ; break ;
  }
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    ENABLE/DISABLE OF GLOBAL SAA/REJECT MODE   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_saa_mode(cs_uint16 port_id, 
				 cs_boolean saa_enbl)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(for SAA enable) or FALSE(for SAA disable)*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/disable control of the global SAA mode bit which is   */ 
/* used to do SA filtering.                                     */
/* The setting of this bit combined with the setting of the     */
/* SR bit in the corresponding lookup CAM entry decides whether */
/* the Host should accept a packet from a specific SA.          */
/*                                                              */
/* When SAA mode = 0, then the Host will accept all packets     */
/* EXCEPT for those SA's which have SR=1(Reject) in the CAM     */
/* entry.                                                       */
/* When SAA mode = 1, then the Host will NOT accept packets     */
/* EXCEPT for those SA's which have SR=1(Accept) in the CAM     */
/* entry.                                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR2_t	cr2 ;
  m2r_port_mac_mode_t	mac_mode ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  
  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  cr2.bf.SAA_MODE = (saa_enbl) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    SET SRP PRIORITY LEVEL CONTROL             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_srp_prio_level(cs_uint16 port_id, 
				       cs_uint8 prio_level)
/* INPUTS     : o Port Id                                       */
/*              o SRP Priority level[0...7]                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This sets up SRP priority level control(ie. the priority     */
/* slicing point). SRP packets whose PRI value in the header    */
/* is equal to or less than this value will be considered as    */
/* LOW priority. This determines which Transit Buffer they get  */
/* stored into(when dual Transit Buffers are used).             */
/* For RPR packets, this value is not used(Class A0/A1 go to    */
/* High TB and the rest go to Low TB).                          */
/*                                                              */
/* The [prio_level] parameter specifies the priority slicing    */
/* point.                                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR2_t	cr2 ;
  m2r_port_mac_mode_t	mac_mode ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return (CS_ERROR) ;
  }

  if (prio_level > 7) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "prio_level(%d) s/b [0...7]\n", prio_level) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  
  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  cr2.bf.PRI = prio_level ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}
  

/****************************************************************/
/* $rtn_hdr_start   STRICT MODE(FOR 802.* COMPLIANCE) MODE      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_strict_mode(cs_uint16 port_id, 
				cs_boolean strict_mode)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(in STRICT mode) or FALSE                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/disable control for STRICT mode checking as per       */
/* 802.* compliance for RPR packets.                            */
/*                                                              */
/* In the STRICT mode, extra checking with wrap-state and       */
/* ttl_base are made on those packets which are sensitive to    */
/* packet re-ordering and duplication. These packets will have  */
/* the S-bit set in the packet header.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR0_t	cr0 ;
  m2r_port_mac_mode_t	mac_mode ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return (CS_ERROR) ;
  }

  if ( (strict_mode) &&
       (mac_mode == M2R_PORT_IN_SRP_MODE) ) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_SRP, NULL) ;
    return (CS_ERROR) ;
  } 
 
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  
  cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
  cr0.bf.STRICT_MODE = (strict_mode) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start       SET HEC MSB CALCULATION MODE            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_hecmsb_mode(cs_uint16 port_id, 
				cs_boolean hecmsb_mode)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(perform HEC in MSB mode) or FALSE        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Perform HEC calculation in MSB/LSB mode                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR0_t	cr0 ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
       (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP), CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  
  cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
  cr0.bf.HECMSB = (hecmsb_mode) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    TB FLUSH CONTROL                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_tb_flush_ctl(cs_uint16 port_id,
				  cs_boolean ptq,
				  m2r_flush_type_t flush_type,
				  m2r_ctl_t ctl, 
				  cs_uint8 deassert_bp_host_chnl_map)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(for PTQ) or FALSE(for STQ)               */
/*              o Flush(kill packets) type                      */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/*              o Channel bitmap with backpressure deasserted   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/disable control of the specified flush type and       */
/* Transit Buffer. When enabled, based on the flush type        */
/* selected, packets matching the flush type will be deleted    */
/* from the specified Transit Buffer(Primary or Secondary).     */
/*                                                              */
/* The [ptq] parameter specifies whether it is PTQ(if TRUE) or  */
/* STQ(if FALSE). For single TB configurations, only PTQ is     */
/* valid.                                                       */
/*                                                              */
/* The [flush_type] parameter specifies the type of flushing.   */
/* It is one of the following:                                  */
/* M2R_FLUSH_WRONG_RI,  M2R_FLUSH_STRICT,  or   M2R_FLUSH_ALL.  */ 
/*                                                              */
/* The [ctl] parameter specifies whether to ENABLE or DISABLE   */
/* the selected flush type.                                     */
/*                                                              */
/* Bit 0 through 3 of [deassert_bp_host_chnl_map] specify if    */
/* deasserting backpressue on the host CONTROL channel, the high*/
/* priority host DATA channel, the medium priority host DATA    */
/* channel and the low priority host DATA channel, respectively */
/* when enabling PTQ/STQ flushing.  Deasserting backpressure    */
/* will help quickly drain the packets inside specified Host    */
/* channels.                                                    */
/*                                                              */
/* Note:                                                        */
/* When enabling the following two types of PTQ/STQ flushing,   */
/* TXH packets will be also flushed(deleted) :                  */
/* M2R_FLUSH_STRICT,   and M2R_FLUSH_ALL                          */
/*                                                              */
/* This feautre is valid only in RPR mode.                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR0_t     cr0 ;
  DR_MICRO_IF_CR6_t	cr6 ;
  DR_RPR_CRLMT2_t       crlmt2 ;
  cs_uint8		bit ;
  cs_boolean            onetb ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
	 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
  onetb = (cr0.bf.ONETB) ? TRUE : FALSE ;
  if (onetb && !ptq) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
                  ":in single TB mode, only PTQ is valid\n") ;
    return (CS_ERROR) ;
  }
    
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  bit = (ctl == M2R_ENABLE) ? 1 : 0 ;
  cr6.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR6.wrd) ;
  crlmt2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT2.wrd) ;
  
  if (ptq) { /* Primary TB */
    switch (flush_type) {
      default: 
      case M2R_FLUSH_WRONG_RI :
	cr6.bf.KILL_WRI_PTQ = bit ;
	if (onetb) cr6.bf.KILL_WRI_STQ = bit ;
	break ;
      case M2R_FLUSH_STRICT :
	cr6.bf.KILL_S_PTQ   = bit ;
	if (onetb) cr6.bf.KILL_S_STQ = bit ;
	break ;
      case M2R_FLUSH_ALL :
	cr6.bf.KILL_ALL_PTQ = bit ;
	if (onetb) cr6.bf.KILL_ALL_STQ = bit ;
	break ;
    }
    crlmt2.bf.PTQ_DEASSERT = deassert_bp_host_chnl_map & 0xF ;
  }
  else { /* Secondary TB */
    switch (flush_type) {
      default: 
      case M2R_FLUSH_WRONG_RI :
	cr6.bf.KILL_WRI_STQ = bit ; break ;
      case M2R_FLUSH_STRICT :
	cr6.bf.KILL_S_STQ   = bit ; break ;
      case M2R_FLUSH_ALL :
	cr6.bf.KILL_ALL_STQ = bit ; break ;
    }
    crlmt2.bf.STQ_DEASSERT = deassert_bp_host_chnl_map & 0xF ;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT2.wrd, crlmt2.wrd) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR6.wrd, cr6.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    HOST FLUSH CONTROL                         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_set_host_flush_ctl(cs_uint16 port_id,
                                  m2r_ctl_t ctl,
                                  cs_uint8 deassert_bp_host_chnl_map)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/*              o Channel bitmap with backpressure deasserted   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/disable flushing(killing) the packets from RingMAC TXH*/
/* sub-block.                                                   */
/*                                                              */
/* The [ctl] parameter specifies whether to ENABLE or DISABLE   */
/* the selected flush type.                                     */
/*                                                              */
/* Bit 0 through 3 of [deassert_bp_host_chnl_map] specify if    */
/* deasserting backpressue on the host CONTROL channel, the high*/
/* priority host DATA channel, the medium priority host DATA    */
/* channel and the low priority host DATA channel, respectively,*/
/* when enabling TXH flushing.  When disabling TXH flushing,    */
/* this parameter has no effect.                                */
/* Deasserting backpressure will help quickly drain the packets */
/* inside specified Host channels.                              */
/*                                                              */
/* This feautre is valid only in RPR mode.                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_RPR_CRLMT_t        crlmt ;
  DR_MAC_HEAD_MAC_MODE_t macmode ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id,
         (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
  macmode.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd) ;

  if (ctl == M2R_ENABLE) {
    if (deassert_bp_host_chnl_map) {
      crlmt.bf.HOST_DEASSERT = 1;
      macmode.bf.SEL_FLUSH_CH = deassert_bp_host_chnl_map & 0xF ;
    } else {
      crlmt.bf.HOST_DEASSERT = 0;
      macmode.bf.SEL_FLUSH_CH = 0xF;
    }
  } else {
    crlmt.bf.HOST_DEASSERT = 0;
    macmode.bf.SEL_FLUSH_CH = 0;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd, macmode.wrd) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start       LOCK RINGMAC WATCHDOG TIMER             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_lock_wdog(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Locks the watchdog timer circuitry.                          */
/*                                                              */
/* This sets the value "0xA" into WDLOCK[3:0] field in WDOG_CR  */
/* register and once this is set any further disable of the     */
/* watchdog is ignored by the hardware.                         */
/*                                                              */
/* Note : The only way to unlock watchdog timer is to reset     */
/* the RingMAC block.                                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		          dev_num, sl_num ;
  M2R_t        		    * pDev ;
  DR_MICRO_IF_WDOG_CR_t   wdog_cr ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			                 CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  wdog_cr.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.WDOG_CR.wrd);
  wdog_cr.bf.WDLOCK = 0xA ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.WDOG_CR.wrd, wdog_cr.wrd);
    
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    SET RINGMAC WATCHDOG TIMER TIMEOUT VALUE   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_wdog_timeout(cs_uint16 port_id, cs_uint32 tmr_val)
/* INPUTS     : o Port Id                                       */
/*              o Timer value in micro sec                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the timeout value for the RINGMAC's watchdog timer.     */
/* For MILAN2, the watchdog timer supports 1~21474836 micro     */
/* seconds;                                                     */
/* Note: In order for this value to be taken by watchdog cir-   */
/* cuitry, the watchdog timer should be enabled first and after */
/* calling this API, call API 'm2r_rmac_wdog_kickstart' to re-  */
/* -start the timer.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		          dev_num, sl_num ;
  M2R_t        		    * pDev ;
  cs_uint32               num_clk;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			                 CS_ERROR) ;

  num_clk = (tmr_val) ?  M2R_RMAC_WDOG_MICRO_SEC_TO_TMR_CNT(tmr_val) :
                            M2R_RMAC_WATCHDOG_1MS_TMR_CNT;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.WDVAL1.wrd, 
               ((num_clk & 0xffff0000)>>16));
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.WDVAL0.wrd, 
               (num_clk & 0x0000ffff));
    
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    RINGMAC WATCHDOG TIMER CONTROL             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_wdog(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/disables RingMAC watchdog timer.                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		          dev_num, sl_num ;
  M2R_t        		    * pDev ;
  DR_MICRO_IF_WDOG_CR_t   wdog_cr ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			                 CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  wdog_cr.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.WDOG_CR.wrd);
  
  if (wdog_cr.bf.WDLOCK == 0xA) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_WATCHDOG_LOCKED, NULL) ;
    M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
    return(CS_ERROR) ;
  }

  wdog_cr.bf.WDENB = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.WDOG_CR.wrd, wdog_cr.wrd);
    
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    RINGMAC WATCHDOG TIMER KICK START          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_wdog_kickstart(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Restarts the RingMAC watchdog timer again.                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		          dev_num, sl_num ;
  M2R_t        		    * pDev ;
  DR_MICRO_IF_WDOG_CR_t   wdog_cr ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			                 CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  wdog_cr.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.WDOG_CR.wrd);
  
  wdog_cr.bf.WDKICK = 5 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.WDOG_CR.wrd, wdog_cr.wrd);
    
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SELECTION AND CONTROL OF LOOPBACKS          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_loopback(cs_uint16 port_id, 
			     m2r_rmac_lpbk_sel_t sel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Loopback selection                            */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/disable control of various loopbacks in RingMAC.      */
/*                                                              */
/* The [sel] parameter specifies the loopback selection and is  */
/* one of the following:                                        */
/* M2R_TXH_TO_RXH   - Host side loopback                        */	
/* M2R_TXF_TO_RXF   - Line side loopback                        */
/* M2R_RXF_TO_MATE  - RXF to Mate - all line traffic to mate    */
/* M2R_RXF_TO_HOST  - RXF to Host - all line traffic to host    */
/* M2R_TOM_TO_FRM   - All traffic going to mate looped back     */ 
/* M2R_FRM_TO_TOM   - All traffic from mate looped back         */
/*                                                              */
/* The [ctl] parameter specifies enable or disable control of   */
/* the selected loopback.                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_DIAG_CR_t	cr ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  
  cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DIAG_CR.wrd) ;
  
  switch (sel) {
    case M2R_TXH_TO_RXH :
      cr.bf.TXH_TO_RXH = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
    case M2R_TXF_TO_RXF :
      cr.bf.TXF_TO_RXF = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
    case M2R_RXF_TO_MATE :
      cr.bf.RXF_TO_TOM = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
    case M2R_RXF_TO_HOST :
      cr.bf.RXF_TO_RXH = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
    case M2R_TOM_TO_FRM :
      /* 
       * On Milan2 when enabling TOM-to-FRM loopback, additionally the
       * the controls for WE drop, WS drop, and Flood bits setting,
       * need to be disabled.
       */
      {
	DR_MATE_CR3_t  cr3 ;

	cr3.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR3_.wrd) ;
	if (ctl == M2R_ENABLE) {	
	  cr3.bf.WE_DROP_OFF = 1 ;
	  cr3.bf.WS_DROP_OFF = 1 ;
	  cr3.bf.FT_ENABLE   = 0 ;
	}
	else { /* M2R_DISABLE */
	  /* put values back to chip default */
	  cr3.bf.WE_DROP_OFF = 0 ;
	  cr3.bf.WS_DROP_OFF = 0 ; 
	}

	M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR3_.wrd, cr3.wrd) ;
      }
      cr.bf.TOM_TO_FRM = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
    case M2R_FRM_TO_TOM :
      cr.bf.FRM_TO_TOM = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;      
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DIAG_CR.wrd, cr.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/********************          TB           ********************/
/****************************************************************/
/* $rtn_hdr_start    TRANSIT BUFFER PARAMETERS SETTING          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_tb_params(cs_uint16 port_id, cs_boolean one_tb,
				  cs_uint16 stq_base, cs_uint16 stq_sz,
				  cs_uint16 ptq_base, cs_uint16 ptq_sz)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(Single TB mode) or FALSE(dual TB mode)   */
/*              o Secondary TB(STQ) Base address                */
/*              o Secondary TB(STQ) size(256-byte words)        */
/*              o Primary TB(PTQ) Base address                  */
/*              o Primary TB(PTQ) size(256-byte words)          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the control of Transit Buffer mode and specifying the   */
/* the location and size of the primary and secondary transit   */
/* buffers.                                                     */
/* In single TB mode, the setting of the STQ parameters should  */
/* be zeroes(as only PTQ parameters are used). The base and size*/
/* parameters operate on 256-byte words.                        */   
/*                                                              */
/* For Milan-2: TB is internal and fixed size of 1MB (or 4K     */
/*              256-byte words).                                */
/* Note - The ptq_sz and stq_sz parameters are not the actual   */
/* sizes, but are the last valid offsets(ie. size - 1).         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR0_t	cr0 ;
  cs_uint32		max_words ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;
  if ( (one_tb) && 
       (M2R_GET_MAC_MODE(port_id) == M2R_PORT_IN_SRP_MODE) ) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_SRP, 
		  ": Single TB mode invalid in SRP\n") ;
    return(CS_ERROR) ;
  }
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  max_words = M2R_NUM_OF_256B_WORDS_IN_TB(0x00100000) ; /* 1 MB */

  if (one_tb) {
    if ( (stq_base) || (stq_sz) ) {
      CS_PRINT(
      "port-%d: WARNG- STQ params are ignored(STQ base= 0x%x, sz= 0x%x)\n",
      stq_base, stq_sz
      ) ; 
    }

    if ( (ptq_base + ptq_sz) >= max_words ) { /* check bounds */
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		    " : PTQ(base= 0x%x, sz= 0x%x, max_words= 0x%x)\n", 
		    ptq_base, ptq_sz, max_words) ;
      return (CS_ERROR) ;
    }

    M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

    cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
    cr0.bf.ONETB = 1;

    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PTQ_BASE.wrd, ptq_base) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PTQ_MAX.wrd, ptq_sz) ;

    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd) ;

  }
 
  else { /* dual transit buffer */
    if (
	( (ptq_base >= stq_base) && (ptq_base <= (stq_base + stq_sz)) ) 
	||
	( (stq_base >= ptq_base) && (stq_base <= (ptq_base + ptq_sz)) )
	) { /* check overlap */
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
	       ": STQ/PTQ overlap - STQ(0x%x, 0x%x), PTQ(0x%x, 0x%x)\n",
		    stq_base, stq_sz, ptq_base, ptq_sz) ;
      return (CS_ERROR) ;
    }

    if ( (stq_base + stq_sz) >= max_words ) { /* check bounds */
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		    " : STQ(base= 0x%x, sz= 0x%x, max_words= 0x%x)\n", 
		    stq_base, stq_sz, max_words) ;
      return (CS_ERROR) ;
    }
 
    if ( ptq_base + ptq_sz >= max_words ) { /* check bounds */
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		    " : PTQ(base= 0x%x, sz= 0x%x, max_words= 0x%x)\n", 
		    ptq_base, ptq_sz, max_words) ;
      return (CS_ERROR) ;
    }

    M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

    cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;  
    cr0.bf.ONETB = 0;

    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.STQ_BASE.wrd, stq_base) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.STQ_MAX.wrd, stq_sz) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PTQ_BASE.wrd, ptq_base) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PTQ_MAX.wrd, ptq_sz) ;

    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd) ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET DELAY QUEUE TIMER FOR SINGLE TB MODE    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_onetb_queue_delay_timer(cs_uint16 port_id, 
                  m2r_service_class_t sc, cs_uint32 timer_val)
/* INPUTS     : o Port Id                                       */
/*              o Packet service class                          */
/*              o Timer value in micro second                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the transmission delay timer for specified service class*/
/* packet in transit buffer of single TB mode.                  */
/*                                                              */
/* [sc] has the following settings:                             */
/* M2R_SERVICE_CLASS_A,           M2R_SERVICE_CLASS_B,          */
/* M2R_SERVICE_CALSS_C                                          */
/*                                                              */
/* Note: This API is only valid for RPR mode with single transit*/
/* buffer(TB) configuration. When a packet of given service     */
/* class is awaiting transmission in TB, the delay queue timer  */
/* will start.  If the packet is not transmitted by the time the*/
/* timer expires, a timeout signal is asserted.                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0 ;
  cs_uint32             cnts ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                             (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP), 
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr0.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

  if (!cr0.bf.ONETB) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_DUAL_TB, 
	    ": Delay Queue Timers are only valid in Single TB mode \n") ;
    return (CS_ERROR) ;
  }

  cnts = M2R_RMAC_DQT_MICRO_SEC_TO_TMR_CNT(timer_val) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  switch ( sc ) {
    case M2R_SERVICE_CLASS_A :
        M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DQT_A1.wrd, 
                     ((cnts & 0xffff0000)>>16));
        M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DQT_A0.wrd, 
                     (cnts & 0x0000ffff));
    	break ;
    case M2R_SERVICE_CLASS_B :
        M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DQT_B1.wrd, 
                     ((cnts & 0xffff0000)>>16));
        M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DQT_B0.wrd, 
                     (cnts & 0x0000ffff));
    	break ;
    case M2R_SERVICE_CLASS_C :
        M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DQT_C1.wrd, 
                     ((cnts & 0xffff0000)>>16));
        M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.DQT_C0.wrd, 
                     (cnts & 0x0000ffff));
    	break ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start     LOW PRIORITY TB THRESHOLDS SETTING        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_stq_thresh(cs_uint16 port_id, 
				   cs_uint16 stq_hthresh,
				   cs_uint16 stq_lthresh, 
				   cs_uint16 stq_mthresh)
/* INPUTS     : o Port Id                                       */
/*              o Threshold TB_HTHRESH of Secondary TB(STQ)     */
/*              o Threshold TB_LTHRESH of Secondary TB(STQ)     */
/*              o Threshold TB_MTHRESH of Secondary TB(STQ)     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the threshold settings of Secondary TB(STQ).  These     */
/* values are used by the Forwarding block and Micro-sequencer  */
/* blocks and all in unit of 256 bytes.                         */
/*                                                              */
/* [stq_hthresh] reflects StqFullThreshold in RPR Standard. In  */
/* SRP draft, it reflects StqHighThreshold.  In fact, in CORTINA*/
/* SRP/RPR hardware implementation, real "StqHighThreshold" in  */
/* SRP or real "StqFullThreshold" in RPR is adjusted by STQ high*/
/* threshold boundary settings based on [stq_hthresh].  The     */
/* equation is:                                                 */
/*   "StqHighThreshold"/"StqFullThreshold"                      */
/*       = [stq_hthresh] * 256 - [n_4KBs] * 4K ;                */
/* where [n_4KBs] is the STQ high threshold boundary set by API */
/* "m2r_rmac_set_stq_hthresh_boundry".  When STQ occupancy goes */
/* "StqHighThreshold"/"StqFullThreshold", no packet can be added*/
/* from the HOST.                                               */
/*                                                              */
/* [stq_lthresh] is applied differently for SRP and RPR mode.   */
/* This reflects StqLowThreshold in SRP draft.  In SRP, when STQ*/
/* occupancy goes beyond this value, no packet can be added from*/
/* the HOST; in RPR mode, [stq_lthresh] works as a switch to    */
/* decide whether to activate the Unreserved (Downstream) Rate  */
/* Limiter on STQ in dual TB implementation.  Only when STQ     */
/* depth is less than this threshold and the application enables*/
/* the Unreserved Rate Limiter to back-pressure STQ by calling  */
/* API "m2r_rmac_set_unrsrvd_rate_limiter", the Unreserved Rate */
/* Limiter can have effect on STQ.                              */
/*                                                              */
/* [stq_mthresh] is only meaningful for RPR mode and ignored by */
/* SRP mode.  When STQ occupancy goes beyond [stq_mthresh], no  */
/* fairness eligible packets can be added from th HOST.  This   */
/* reflects the StqHighThreshold in RPR standard.               */
/*                                                              */
/* Please refer to API "m2r_ms_set_rpr_stq_thresh" for setting  */
/* up all RPR-required STQ thresholds.                          */
/* Please refer to API "m2r_ms_set_srp_stq_thresh" for setting  */
/* up all SRP-required STQ thresholds.                          */
/*                                                              */
/* Note : When this API is called, it assumes caller has called */
/* API "m2r_rmac_set_tb_params" already.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0;
  cs_uint16             stq_sz;
  m2r_port_mac_mode_t  mac_mode ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }

  if ( (mac_mode == M2R_PORT_IN_SRP_MODE) &&
       (stq_hthresh < stq_lthresh) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
     ": s/b stq_lthresh(0x%x) < stq_hthresh(0x%x) in SRP mode\n",
                  stq_lthresh, stq_hthresh) ;
    return (CS_ERROR) ;
  } else if ( (mac_mode == M2R_PORT_IN_RPR_MODE) &&
              (stq_hthresh < stq_mthresh) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
     ": s/b stq_mthresh(0x%x) < stq_hthresh(0x%x) in RPR mode\n",
                  stq_mthresh, stq_hthresh) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr0.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

  if (cr0.bf.ONETB) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_ONETB, 
		  ": STQ threshold can not be set in Single TB mode \n") ;
    return (CS_ERROR) ;
  }

  stq_sz = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.STQ_MAX.wrd) ;

  if (stq_hthresh>stq_sz) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
	       ": stq_hthresh(0x%x) exceeds stq_sz(0x%x)\n",
		    stq_hthresh, stq_sz) ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TB_HTHRESH.wrd, stq_hthresh) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TB_LTHRESH.wrd, stq_lthresh) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TB_MTHRESH.wrd, stq_mthresh) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start     STQ HIGH THRESHOLD BOUNDRY SETTING        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_stq_hthresh_boundry(cs_uint16 port_id, 
					    cs_uint8 n_4KBs)
/* INPUTS     : o Port Id                                       */
/*              o Gap to STQ High Threshold (in unit of 4KB)    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets how close the low priority TB can come to the high      */
/* threshold.                                                   */
/*                                                              */
/* When STQ occupancy goes beyond [stq_hthresh] * 256 - [n_4KBs]*/
/* * 4K where [stq_hthresh] is the high threshold of STQ, no    */
/* packet can be added from the HOST.                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0;
  DR_MICRO_IF_CR1_t     cr1;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                             M2R_INVALID_IN_BYPASS, 
                             CS_ERROR) ;

  if (n_4KBs > 7) {
    CS_HNDL_ERROR(port_id, EM2R_DEV_INVALID_USER_ARG, 
      ": STQ high threshold boundry can not be larger than 28KB \n") ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr0.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

  if (cr0.bf.ONETB) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_ONETB, 
      ": STQ high threshold boundry can not be set in STQ mode \n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr1.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
  
  cr1.bf.LP_TH_BNDRY = n_4KBs ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}


/****************************************************************/
/* $rtn_hdr_start      STQ HIGH THRESHOLD LIMITATION CONTROL    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_stq_hthresh(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls if the high threshold of STQ limits the STQ.        */
/*                                                              */
/* When this API is called to disable the limitation of the high*/
/* threshold, it could potentially cause overflow of the STQ.   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0;
  DR_MICRO_IF_CR2_t     cr2;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                             M2R_INVALID_IN_BYPASS, 
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr0.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

  if (cr0.bf.ONETB) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_ONETB, 
	 ": STQ high threshold control can not be set in STQ mode \n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr2.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  
  cr2.bf.TB_T_OFF = (ctl==M2R_ENABLE) ? 0 : 1 ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}


/****************************************************************/
/* $rtn_hdr_start     HIGH PRIORITY TB IPS THRESHOLD SETTING    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_ptq_thresh(cs_uint16 port_id, cs_uint16 ptq_thresh)
/* INPUTS     : o Port Id                                       */
/*              o IPS threshold of Primary TB(PTQ)              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the IPS threshold settings of Primary TB(STQ).  This    */
/* values is used to determine IPS packet priority compared by  */
/* high priority packets in PTQ and it is in unit of 256 bytes. */
/*                                                              */
/* When PTQ occupancy goes below [ptq_thresh], IPS packets from */
/* the Microprocessor interface will be served before High      */
/* Priority packets from the Transit Buffer.                    */
/*                                                              */
/* Note : When this API is called, it assumes caller has called */
/* API "m2r_rmac_set_tb_params" already.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  cs_uint16             ptq_sz;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                             M2R_INVALID_IN_BYPASS, 
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  ptq_sz = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.PTQ_MAX.wrd) ;

  if (ptq_thresh>ptq_sz) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
	       ": ptq_thresh(0x%x) exceeds ptq_sz(0x%x)\n",
		    ptq_thresh, ptq_sz) ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TBH_THRESH.wrd, ptq_thresh) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}


/********************          SRPF          ********************/

/****************************************************************/
/* $rtn_hdr_start    SET SRP AND FAIRNESS PACKET TTL            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_srp_ttl(cs_uint16 port_id, cs_boolean overwrite,
				  cs_uint8 ttl)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(overwrite HOST pkt hdr) or FALSE(untouch)*/
/*              o Specified ttl value                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control the overwriting of both SRP/RPR data packet header   */
/* TTL field when MAC Re-write function is OFF.   And also, this*/ 
/* value is also used by Micro-sequencer to insert into fairness*/
/* packet as provisioned station number in the ring.            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_SRP_HDR_t srp_hdr;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                             M2R_INVALID_IN_BYPASS, 
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  srp_hdr.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SRP_HDR.wrd) ;

  srp_hdr.bf.WR_TTL = (overwrite) ? 1 : 0 ;
  srp_hdr.bf.SRP_TTL = ttl ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SRP_HDR.wrd, srp_hdr.wrd);

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MAX ALLOWABLE USAGE BY THIS STATION    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_srpf_max_allow(cs_uint16 port_id, cs_uint16 usage)
/* INPUTS     : o Port Id                                       */
/*              o Max allowable usage value for FE HOST packets */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the max allowable usage value to control the transmit of*/
/* fairness eligible HOST packets.                              */
/*                                                              */
/* Note: The max allowable usage determines the maximum bytes   */
/* allowed to be added by this station per ageCoef agingInterval*/
/* where both ageCoef and agingInterval are fairness provision  */
/* parameters.  In order to effectively use this API, the app-  */
/* lication should configure Micro-sequencer correctly before   */
/* calling this API with the corresponding value.  The max      */
/* allowable usage is in terms of 16 bytes.                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                             M2R_INVALID_IN_BYPASS, 
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAX_ALLOW.wrd, usage);

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}


/*******************     RINGMAC - MATE     *********************/
/****************************************************************/
/* $rtn_hdr_start   SET FLOOD TYPE AND TTL FOR HOST TX PKTS     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_flood_ctl(cs_uint16 port_id, 
				  cs_boolean ft_enbl,
				  m2r_rmac_flood_type_t ft_type,
				  cs_uint8 ft_ttl)
/* INPUTS     : o Port Id                                       */
/*              o Enable Flood Type setting                     */
/*              o Flood Type                                    */  
/*              o Flood TTL                                     */  
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable control of setting Flood Type and Flood TTL   */  
/* fields for Host sourced packets. This is for those stations  */  
/* with bridged LAN, where the destination address(DA) of the   */  
/* of the RPR station on the ring is not known.                 */  
/* When enabled, for those packets which "miss" on the STAT CAM */  
/* for DA lookup(ie. unknown DA), the Flood Type and Flood TTL  */  
/* are set.                                                     */  
/*                                                              */  
/* The [ft_enbl] parameter specifies whether to enable or       */  
/* disable the over-writing of Flood Type and Flood TTL fields. */  
/*                                                              */  
/* The [ft_type] parameter specifies either,                    */  
/* M2R_RMAC_FLOOD_TYPE_UNI_DIR or M2R_RMAC_FLOOD_TYPE_BI_DIR    */  
/*                                                              */  
/* Note that if the MAC Rewrite block were to be configured to  */  
/* set the TTL field, this block could overwrite that setting.  */  
/* Also, when [ft_enbl] is disabled(FALSE), then the Flood Type */  
/* and Flood TTL are passed untouched in the packet.            */  
/*                                                              */  
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MATE_CR3_t		cr3 ; 
    
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  
  cr3.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR3_.wrd) ;
  cr3.bf.FT_ENABLE = (ft_enbl) ? 1 : 0 ;

  switch (ft_type) {
    default : 
    case M2R_RMAC_FLOOD_TYPE_NONE    : cr3.bf.FT = 0 ; break ; 
    case M2R_RMAC_FLOOD_TYPE_UNI_DIR : cr3.bf.FT = 1 ; break ; 
    case M2R_RMAC_FLOOD_TYPE_BI_DIR  : cr3.bf.FT = 2 ; break ;
    case M2R_RMAC_FLOOD_TYPE_RSRVD   : cr3.bf.FT = 3 ; break ;
  }
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR3_.wrd, cr3.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.FT_TTL.wrd, ft_ttl) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   FROM MATE(FRM) FIFO UNDERFLOW CHECKING      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_from_mate_underflow_check(cs_uint16 port_id, 
						  cs_boolean enbl_chk)
/* INPUTS     : o Port Id                                       */
/*              o Enable/Disable of FRM Underflow checking      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable control of From Mate(FRM) FIFO underflow      */  
/* checking.                                                    */  
/*                                                              */  
/* Note that, if the XAUI reference clock is running above the  */  
/* nominal value of 156.25MHz, then this check should be        */  
/* DISABLED. Otherwise, there will be idle cycles when there is */  
/* no valid data which could an bogus underflow condition to be */  
/* detected which will result in the packet getting dropped at  */  
/* the mate interface.                                          */  
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MATE_CR3_t		cr3 ; 
    
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  
  cr3.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR3_.wrd) ;
  cr3.bf.FRM_UNDERFLOW_EN = enbl_chk ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR3_.wrd, cr3.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/********************   IDLE PACKET CONTROL  ********************/

/****************************************************************/
/* $rtn_hdr_start  SET TX SLOW/FAST IDLE TIMERS                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_tx_idle_timers(cs_uint16 port_id, 
				       cs_uint32 slow_tmr_cnt,
				       cs_uint32 fast_tmr_cnt)
/* INPUTS     : o Port Id                                       */
/*              o SLOW Timer counter                            */
/*              o FAST Timer counter                            */  
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls the settings of the Slow and Fast Idle sync timers, */
/* which determine the rate of Idle packet generation.          */
/* The timer value is decremented by 1 for every 10 clock       */   
/* cycles.                                                      */ 
/* For Milan2: The max rate is 20 MHz (clock speed = 200 MHz).  */  
/* Higher the rate, more IDLE sync packets and less actual data */ 
/* is sent.                                                     */
/* The slow timer(ie. less IDLE sync pkts) is used when         */  
/* HTB_DEPTH > IDLE_THRESH (ie. more pkts pending in HTB).      */
/*                                                              */  
/* Note:                                                        */
/* 1. A value of 0 for the timers, will DISABLE the corresponding*/
/*    timer and the idle packet generation.                     */
/* 2. When generating IDLE frames in RPR application, RPR       */
/*    standard IEEE 802.17 addresses that Fast Idle timer should*/
/*    be programmed such that IDLE frames are inserted into the */
/*    transmit datapath at a fixed rate equivalent to 0.05% of  */
/*    the linerate, and Slow Idle timer should be programmed such*/
/*    that IDLE frames are inserted into the transmit datapath  */
/*    at a fixed rate equivalent to 0.025% of the linerate.  So */
/*    we can get the following formula:                         */
/*                                                              */
/*    Fast_tmr = (1 / (0.0005 * LINE_RATE / (16 * 8)))          */
/*               / (10 * (1/(CORE_CLK)))                        */
/*                                                              */
/*    Slow_tmr = (1 / (0.00025 * LINE_RATE / (16 * 8)))         */
/*               / (10 * (1/(CORE_CLK)))                        */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;
  
  if (slow_tmr_cnt < fast_tmr_cnt) { /* slow rate > fast rate */
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": slow_tmr_cnt(%d) s/b >= fast_tmr_cnt(%d)\n",
		  slow_tmr_cnt, fast_tmr_cnt) ;
    return (CS_ERROR) ;
  }
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TX_SYNC_TSLOW0.wrd,
	       slow_tmr_cnt & 0xFFFF) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TX_SYNC_TSLOW1.wrd,
	       slow_tmr_cnt >> 16) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TX_SYNC_TFAST0.wrd,
	       fast_tmr_cnt & 0xFFFF) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TX_SYNC_TFAST1.wrd,
	       fast_tmr_cnt >> 16) ;
  
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET TX IDLE THRESHOLD                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_tx_idle_thold(cs_uint16 port_id, 
				      cs_uint16 thold)
/* INPUTS     : o Port Id                                       */
/*              o Idle Threshold value [0...65535]              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the Tx Idle Threshold for High Priority Transit Buffer. */
/* The threshold operates on 256 byte blocks.                   */
/* When the depth of High Priority Transit Buffer exceeds this  */
/* threshold value, it uses the SLOW Idle sync timer(in order   */
/* to transmit less idle packets).                              */
/*                                                              */  
/* The [thold] parameter specifies how many 256-bytes blocks    */
/* make up the threshold.                                       */
/*                                                              */
/* Note:                                                        */
/* When generating IDLE frames in RPR application, RPR standard */
/* IEEE 802.17 addresses that this threshold should be in the   */
/* range of [sizePTQ-mtuSize, mtuSize], and the deault is       */
/* 'sizePTQ-mtuSize'.                                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IDLE_THRESH.wrd,
	       thold) ;
 
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   WRITE TX IDLE PACKET TO IDLE BUFFER         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_write_tx_idle_pkt(cs_uint16 port_id, 
				      cs_uint8 * ppkt,
				      cs_uint8 pktlen)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to Idle packet(in user buffer)        */
/*              o Idle packet length                            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Writes the user provided Idle packet into Idle buffer.       */
/* This Idle packet will be generated by the Forwarding block   */ 
/* and sent out periodically(as specified by the Idle sync      */
/* timers.                                                      */
/*                                                              */  
/* The [ppkt] parameter is a pointer to idle packet and the     */ 
/* bytes are expected to be in network order.                   */ 
/*                                                              */  
/* The [pktlen] parameter is the size of the Idle packet.       */ 
/* Received Idle sync packets are consumed by RXF block.        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  volatile cs_reg	* p ;
  cs_uint8		i ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  if (pktlen < 16 || pktlen > 32) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": pktlen(%d) s/b [16...32]\n", pktlen) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.ILEN.wrd, 
	       (pktlen - 16)) ;      

  p = &pDev->slice[sl_num].RingMAC.IDLE0.wrd ;
  
  /*
   * The bytes in the user buffer are in network order.
   */
  for (i = 0; i < pktlen; i += 2, p++) {
    if ( (pktlen - i) == 1 ) { 
       M2R_REG_WRITE(p, ppkt[i] << 8) ;
    }
    else {
       M2R_REG_WRITE(p, (ppkt[i] << 8 | ppkt[i+1]) ) ;

    }
  }
    
  return(CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   WRITE CANNED DEBUG IDLE PACKET              */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_debug_write_tx_idle_pkt(cs_uint16 port_id) 
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Writes the canned 16-byte Idle packet into Idle buffer.      */
/* This Idle packet will be generated by the Forwarding block   */ 
/* and sent out periodically(as specified by the Idle sync      */
/* timers.                                                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_uint16             ii, hdr, val ;
  cs_uint32     	crc32 ;
  DR_MICRO_IF_CR2_t     cr2 ;
  cs_uint8		buf[16] ; 

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;

  /* Compose the packet */
  hdr = m2r_get_rpr_hdr(1, cr2.bf.RING_ID, 0, 0, 3, 0) ;
  buf[0] = hdr >> 8 ;
  buf[1] = hdr & 0xff ;

  /* Compose SA */
  val = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_SA2.wrd) ;
  buf[2] = val >> 8 ;
  buf[3] = val & 0xff ;

  val = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_SA1.wrd) ;
  buf[4] = val >> 8 ;
  buf[5] = val & 0xff ;

  val = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_SA0.wrd) ;
  buf[6] = val >> 8 ;
  buf[7] = val & 0xff ;

  /* idle payload should be set to all ZEROs */
  for (ii = 8; ii < 12; ii++) {
    buf[ii] = 0 ;
  }

  /* Insert computed CRC-32 */
  crc32 = m2r_get_crc32( &buf[2], 10 ) ;
  buf[12] = (crc32 >> 24) & 0xff ;
  buf[13] = (crc32 >> 16) & 0xff ;
  buf[14] = (crc32 >>  8) & 0xff ;
  buf[15] = crc32 & 0xff ;

  CS_PRINT("\n\tIdle Packet(length = 16 bytes) written to buffer\n") ; 

  if ( m2r_rmac_write_tx_idle_pkt(port_id, buf, 16)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/*************************** HOST *******************************/
/****************************************************************/
/* $rtn_hdr_start     SET RINGMAC HOST CHANNEL NUMBER           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_num_host_channel(cs_uint16 port_id, 
                cs_boolean single_channel, cs_uint8 single_channel_id,
                cs_uint8 num_pri_channels )
/* INPUTS     : o Port Id                                       */
/*              o TRUE(single receiving channel) or FALSE       */
/*              o SPI-selected Host channel id                  */
/*              o Number of host priority channels              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the number of host receiving channels.                  */ 
/*                                                              */
/* [single_channel] indicates if the application wants to use   */
/* only one single host channel identified by [single_channel_  */
/* id], which should be consistent with the SPI RX channel      */
/* calendar setting.                                            */
/*                                                              */
/* [single_channel_id] can have value from 0 to 3.              */
/*                                                              */
/* [num_pri_channels] specifies how many host channels are used */
/* for priority-mapped receiving. Since there are up to 4 rece- */
/* -iving host channels, when this parameter is set as 1, all   */
/* packets are mapped to channel 0; when it is 2, control and   */
/* high priority packets go to channel 0 and others go to       */
/* channel 1; when it is 3, control and high priority packets go*/
/* to channel 0, medium priority packets go to channel 1 and low*/
/* priority packets go to channel 2; when it is 4, the control  */
/* packets go to channel 0, high priority packets go to channel */
/* 1, medium priority packets go to channel 2 and low priority  */
/* packets go to channel 3.                                     */
/*                                                              */
/* Note: When the application chooses single channel mode, [num_*/
/* pri_channels] is ignored, but in normal RPR/SRP mode, this   */
/* mode is not recommended.                                     */ 
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		    dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_HOST_CR4_t     cr4 ;
  DR_HOST_CR4A_t    cr4a ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			     CS_ERROR) ;

  if (single_channel_id > M2R_RMAC_HOST_MAX_NUM_OF_CHANNEL - 1) {
    CS_HNDL_ERROR(port_id, EM2R_DEV_INVALID_USER_ARG, 
		  ": Host channel id can only be 0 to 3 \n") ;
    return (CS_ERROR) ;
  }

  if ((num_pri_channels == 0) || 
      (num_pri_channels > M2R_RMAC_HOST_MAX_NUM_OF_CHANNEL)) {
    CS_HNDL_ERROR(port_id, EM2R_DEV_INVALID_USER_ARG, 
		  ": Host channel number can only be 1 to 4 \n") ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr4a.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4A.wrd) ;

  if (single_channel) {
  	cr4a.bf.HOSTPRIEN = 0 ;
  	cr4.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4.wrd) ;
  	cr4.bf.RXH_CHANNEL = single_channel_id ;
  	M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR4.wrd, cr4.wrd) ;
  } else {
    cr4a.bf.HOSTPRIEN = 1 ;
    cr4a.bf.HOSTNUMCHAN = num_pri_channels - 1 ;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR4A.wrd, cr4a.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start     RPR OAM CONTROL PACKET RECEIVING CONTROL  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_rpr_oam_on_ch0(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/Disables mapping rpr OAM packet onto channel 0.      */ 
/*                                                              */ 
/* Note: Normally OAM packet will be mapped to the data channels*/
/*       corresponding to its associative service class.        */ 
/*                                                              */
/*       For Milan-2, do not call this API with M2R_DISABLE     */
/*       option. The driver initialization enables it and it is */
/*       recommended that the user does not change it!          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_HOST_CR4A_t        cr4a ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
	         CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr4a.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4A.wrd) ;

  cr4a.bf.CTL_OAM_CH0_ON = (ctl==M2R_ENABLE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR4A.wrd, cr4a.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start     SET SRP HOST CHANNEL PRIORITY THRESHOLD   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_srp_host_prio_thresh(cs_uint16 port_id, 
					     cs_uint8 med_pri,
                                             cs_uint8 hi_pri)
/* INPUTS     : o Port Id                                       */
/*              o Medium Packet Priority Threshold              */
/*              o High Packet Priority Threshold                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the host channel priority thresholds for SRP mode.      */ 
/*                                                              */ 
/* Note: This API is only valid in SRP mode.                    */ 
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_HOST_CR4A_t        cr4a ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_RPR),
	         CS_ERROR) ;

  if (med_pri >= hi_pri) {
    CS_HNDL_ERROR(port_id, EM2R_DEV_INVALID_USER_ARG, 
	    ": Medium priority should be less than High priority \n") ;
    return (CS_ERROR) ;
  }

  if (hi_pri > 7) {
    CS_HNDL_ERROR(port_id, EM2R_DEV_INVALID_USER_ARG, 
		  ": Priority value should be less than 8 \n") ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr4a.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4A.wrd) ;

  cr4a.bf.HOSTPRIMED = med_pri ;
  cr4a.bf.HOSTPRI = hi_pri ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR4A.wrd, cr4a.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ACCEPT WRAPPED PACKETS WITH WRONG RING-ID   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_accept_wpkts_on_wrong_ri(cs_uint16 port_id,     
	       cs_boolean accept_ucast, cs_boolean accept_mcast)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(accept unicast packets) or FALSE         */
/*              o TRUE(accept multicast packets) or FALSE       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls whether the Host accepts WRAPPED packets that arrive*/
/* on the wrong ringlet.                                        */ 
/*                                                              */ 
/* The [accept_ucast] parameter specifies whether the Host      */ 
/* should accept wrapped Uni-cast packets.                      */ 
/*                                                              */ 
/* The [accept_mcast] parameter specifies whether the Host      */ 
/* should accept wrapped Mulit-cast packets.                    */ 
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR2_t	cr2 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
	 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  cr2.bf.ACCEPT_WRAP_UNICAST   = (accept_ucast) ? 1 : 0 ;
  cr2.bf.ACCEPT_WRAP_MULTICAST = (accept_mcast) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   HOST ACCEPT FLOODED DATA PACKET CONTROL     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_accept_flooded_data_pkts(cs_uint16 port_id,     
					     cs_boolean accept)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(accept flooded packets) or FALSE         */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls whether the Host accepts FLOODED data packets       */
/* irrespective of DA match.                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_MICRO_IF_CR0_t	cr0 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
	 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

  cr0.bf.ACCEPT_FLOOD = (accept) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   HOST PKTS STRIP/INSERT CRC CONTROL          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_host_crc_strip_or_insert(cs_uint16 port_id,     
	       cs_boolean rx_strip, cs_boolean tx_insert)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(strips CRC to Host(Rx)) or FALSE         */
/*              o TRUE(inserts CRC from Host(Tx)) or FALSE      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls the handling of the 4-byte CRC on packets for       */
/* Host packets(ingress and egress).                            */
/*                                                              */
/* The [rx_strip] parameter specifies if the 4-byte CRC at the  */
/* end of the packet should be stripped or not before sending   */
/* it to the Host.                                              */
/*                                                              */
/* The [tx_strip] parameter specifies if the 4-byte CRC should  */
/* be computed and appended to the packets from the Host.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_HOST_CR4_t		cr4 ;
  DR_MICRO_IF_CR2_t	cr2 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  /* CRC strip control for packets to Host */
  cr4.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4.wrd) ;
  cr4.bf.RXH_STRIP_CRC = (rx_strip) ? 1 : 0 ; 
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR4.wrd, cr4.wrd) ;

  /* CRC insert control for packets from Host */
  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  cr2.bf.TX_CRC_OFF = (tx_insert) ? 0 : 1 ;  /* disable bit */
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONTROL OF SRP/RPR HEADER STRIPPING         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_hdr_strip(cs_uint16 port_id, 
				  m2r_rmac_hdr_strip_sel_t sel,
				  cs_boolean ppp_enbl)
/* INPUTS     : o Port Id                                       */ 
/*              o Header strip selection                        */
/*              o TRUE(PPP encap) or FALSE(HDLC encap)          */
/* RETURNS    : OK or CS_ERROR                                  */
/* DESCRIPTION:                                                 */
/* Controls the SRP/RPR header stripping for SRP/RPR packets    */
/* received from the line, and before sending it to the Host.   */
/*                                                              */
/* The [sel] parameter specifies one of the following:          */
/* M2R_RMAC_HDR_STRIP_DISABLED,   M2R_RMAC_HDR_STRIP_AND_ENCAP, */
/* or M2R_RMAC_HDR_STRIP_NO_ENCAP                               */
/*                                                              */
/* The [ppp_enbl] parameter is used only if [sel] parameter is  */
/* M2R_MAC_HDR_STRIP_AND_ENCAP. Otherwise it is ignored. If     */
/* [ppp_enbl] is TRUE, then after stripping the header, a value */
/* of 0xFF03 is inserted(implied PPP encapsulation).            */
/* If [ppp-enbl] is FALSE, then after stripping the header, a   */
/* value of 0x0F00 is inserted(implied HDLC encapsulation).     */
/*                                                              */
/* NOTE: If [sel] parameter is M2R_MAC_HDR_STRIP_NO_ENCAP, then */
/* all the fields in the RPR header is stripped except for the  */
/* station's DA and SA fields.                                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  DR_HOST_CR4_t		cr4 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  cr4.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4.wrd) ;
  switch (sel) {
    default :
    case M2R_RMAC_HDR_STRIP_DISABLED:
      cr4.bf.RXH_STRIP_SRP = RXH_STRIP_SRP_SRP_NOSTRIP ;  break ;

    case M2R_RMAC_HDR_STRIP_AND_ENCAP :
      cr4.bf.RXH_STRIP_SRP = RXH_STRIP_SRP_SRP_ENCAP_STRIP ;
      cr4.bf.RXH_PPP = ppp_enbl ? 1 : 0 ; break ;
   
    case M2R_RMAC_HDR_STRIP_NO_ENCAP :
      cr4.bf.RXH_STRIP_SRP =  RXH_STRIP_SRP_SRP_FULLSTRIP ; break ;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR4.wrd, cr4.wrd) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start SRP/RPR HEADER STRIP - ENABLING NEW PKT TYPE  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_hdr_strip_ctl_new_pkt_type(cs_uint16 port_id,
					       cs_boolean enbl,
					       cs_uint8 mode,
					       cs_uint16 proto)
/* INPUTS     : o Port Id                                       */ 
/*              o Enable/Disable new packet type                */
/*              o Mode bits in the header                       */
/*              o Protocol type in the header                   */
/* RETURNS    : OK or CS_ERROR                                  */
/* DESCRIPTION:                                                 */
/* Specify a new packet type to be included in the default list */
/* for SRP/RPR testing. The new packet type is determined by    */
/* matching the mode and protocol type bits in the header of    */
/* the SRP/RPR packet received.                                 */
/*                                                              */
/* The [enbl] parameter when set to TRUE enables the inclusion  */
/* of the new packet type, specified by the mode and protocol   */
/* type field. When set to FALSE, the mode and protocol fields  */
/* are cleared.                                                 */
/*                                                              */
/* The [mode] parameter specified corresponds to the mode field */
/* in the SRP packet header or [WE,PT] for RPR packet header.   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16			dev_num, sl_num ;
  M2R_t        			* pDev ;
  DR_HOST_STRIP_SRP_PRG0_t      prg0 ; 
  DR_HOST_STRIP_SRP_PRG1_t      prg1 ; 

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  prg0.wrd = prg1.wrd = 0 ;
  
  if ( !(enbl) ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.STRIP_SRP_PRG0.wrd, 
		 prg0.wrd) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.STRIP_SRP_PRG1.wrd, 
		  prg1.wrd) ;
  } 
  else {  
    prg0.wrd = proto ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.STRIP_SRP_PRG0.wrd, 
		 prg0.wrd) ;
  
    prg1.bf.STRIP_PRG_MODE = mode ;
    prg1.bf.STRIP_PRG_EN   = 1 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.STRIP_SRP_PRG1.wrd, 
		 prg1.wrd) ;
  }
  
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   RING-ID CONTROL FOR PACKETS FROM HOST       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_host_ctl_ri(cs_uint16 port_id, 
				m2r_rmac_host_ctl_ri_t ctl) 
/* INPUTS     : o Port Id                                       */
/*              o Control of Ring-Id                            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control the value of the Ring-Id on Host-sourced packets.    */
/*                                                              */
/* The [ctl] parameter is specified to be one of the following, */
/* M2R_RMAC_INSERT_N_WFLIP,           M2R_RMAC_RI_WFLIP,        */
/* M2R_RMAC_RI_INSERT,            or, M2R_RMAC_RI_UNCHANGED.    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16			dev_num, sl_num ; 
  M2R_t         		* pDev ;
  DR_MAC_HEAD_MAC_MODE_t	md ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  md.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd) ;
  
  switch (ctl) {
    default :
    case M2R_RMAC_RI_INSERT_N_WFLIP : md.bf.WR_RID = 0 ; break ;
    case M2R_RMAC_RI_WFLIP          : md.bf.WR_RID = 1 ; break ;
    case M2R_RMAC_RI_INSERT         : md.bf.WR_RID = 2 ; break ;
    case M2R_RMAC_RI_UNCHANGED      : md.bf.WR_RID = 3 ; break ;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd, md.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/********************** Rate Limiter ****************************/

/****************************************************************/
/* $rtn_hdr_start  SET HOST TX RATE LIMIT PARAMETERS            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_host_rate_limit_params(cs_uint16 port_id, 
		      m2r_prio_traffic_t prio, cs_uint16 tmr_val,
		      cs_uint16 delta, cs_uint16 max_accum) 
/* INPUTS     : o Port Id                                       */
/*              o Traffic priority type                         */
/*              o Timer value                                   */ 
/*              o Delta                                         */ 
/*              o Maximum accumulated value                     */ 
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the rate limiting parameters for specified priority for */
/* Host sourced traffic.                                        */
/*                                                              */
/* The [prio] parameter specifies the traffic priority and is   */
/* one of the following:                                        */
/* M2R_TRAFFIC_PRIO_LOW,                M2R_TRAFFIC_PRIO_MED,   */
/* M2R_TRAFFIC_PRIO_HI,                 M2R_TRAFFIC_PRIO_A0,    */
/* M2R_TRAFFIC_PRIO_CTL,                M2R_TRAFFIC_PRIO_HIRES  */
/* For SRP, only M2R_TRAFFIC_PRIO_LOW and M2R_TRAFFIC_PRIO_HI   */
/* are valid.                                                   */
/*                                                              */
/* The [tmr_val], [delta], and [max_accum] are parameters that  */
/* needs to be specified to get the desired rate limit.         */
/* Refer to the HW spec for more info.                          */
/*                                                              */
/* The rate-limiters can also be controlled such that they      */
/* they can always block or always pass, and the API,           */
/* m2r_rmac_ctl_host_rate_limiter() for that purpose.           */
/* When these controls are in effect, they over-ride the        */
/* rate-limit parameters.                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;
  m2r_port_mac_mode_t	mac_mode ;
  DR_RPR_CRLMT_t	crlmt ;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;                        
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  } 
  if (mac_mode == M2R_PORT_IN_SRP_MODE) {
    if ( (prio != M2R_TRAFFIC_PRIO_LOW) && 
	 (prio != M2R_TRAFFIC_PRIO_HI) ) {
      CS_HNDL_ERROR( port_id, EM2R_RMAC_INVALID_IN_SRP,
		     ": invalid priority(%d)\n", prio) ;
      return(CS_ERROR) ;
    }
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  switch (prio) {
    default : 
    case M2R_TRAFFIC_PRIO_LOW :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.LO_TIME.wrd, 
		   tmr_val) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.LO_DELTA.wrd,
		   delta) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.LO_MAX.wrd,
		   max_accum) ;
      break ;

    case M2R_TRAFFIC_PRIO_MED :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MED_TIME.wrd, 
		   tmr_val) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MED_DELTA.wrd,
		   delta) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MED_MAX.wrd,
		   max_accum) ;
      break ;

    case M2R_TRAFFIC_PRIO_HI : 
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HI_TIME.wrd, 
		   tmr_val) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HI_DELTA.wrd,
		   delta) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HI_MAX.wrd,
		   max_accum) ;
      /* Disable A0 RL if SRP mode */
      if ( mac_mode == M2R_PORT_IN_SRP_MODE ) {
        crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
        crlmt.bf.HIA0_OFF = 0 ;
        crlmt.bf.HIA0_DIS = 1 ;
        M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;      
      }
      break ;

    case M2R_TRAFFIC_PRIO_A0 : 
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HIA0_TIME.wrd,
		   tmr_val) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HIA0_DELTA.wrd,
		   delta) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HIA0_MAX.wrd,
		   max_accum) ;
      break ;

    case M2R_TRAFFIC_PRIO_CTL : 
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_TIME.wrd,
		   tmr_val) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_DELTA.wrd,
		   delta) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CTL_MAX.wrd,
		   max_accum) ;
      break ;

    case M2R_TRAFFIC_PRIO_HIRES : 
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HIRES_TIME.wrd,
		   tmr_val) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HIRES_DELTA.wrd,
		   delta) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.HIRES_MAX.wrd,
		   max_accum) ;
      break ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  /* Turn on rate limiter at the end */
  return (m2r_rmac_ctl_host_rate_limiter(port_id, prio, FALSE, FALSE)) ;
}			 
                             
/****************************************************************/
/* $rtn_hdr_start SET HOST TX RATE LIMIT BASED ON KBPS          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_set_prio_traffic_host_rate_limit_kbps(
                 cs_uint16 port_id,
                 m2r_prio_traffic_t prio,
                 m2r_port_line_rate_t line_rate,
                 cs_uint32 kbps_line_rate)
/* INPUTS     : o Port Id                                       */
/*              o Traffic priority type                         */
/*              o Port Line Rate                                */
/*              o Requested bandwidth kbps                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the rate limit for the specified priority for Host       */
/* sourced traffic based on requested bandwidth kbps.           */
/*                                                              */
/* [prio] could have the following values:                      */
/* M2R_TRAFFIC_PRIO_LOW,          M2R_TRAFFIC_PRIO_MED,         */
/* M2R_TRAFFIC_PRIO_HI,           M2R_TRAFFIC_PRIO_A0,          */
/* M2R_TRAFFIC_PRIO_CTL,          M2R_TRAFFIC_PRIO_HIRES        */
/* Among the above values,  M2R_TRAFFIC_PRIO_LOW and            */
/* M2R_TRAFFIC_PRIO_HI are the only valid traffic priorities in */
/* SRP mode.                                                    */
/*                                                              */
/* For MILAN2, [line_rate] can only be the followings:          */
/* M2R_PORT_RATE_OC192C,          M2R_PORT_RATE_OC48C,          */
/* M2R_PORT_RATE_10G                                            */
/*                                                              */
/* Parameters [kbps_line_rate] specify how many amount of line  */
/* bandwidth is requested, from 0 to line rate (MAX=OC-48C).    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16     tmr_val, delta, max ;
  cs_uint32     lr ;
  cs_uint32     scale_up, scale_dn ;
  cs_uint16     a, b, c ;

  if (kbps_line_rate > M2R_GET_KBPS_LR_RATE(line_rate)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
          " : kbps(%d) should not be larger than %d\n",
          kbps_line_rate, M2R_GET_KBPS_LR_RATE(line_rate)) ;
    return(CS_ERROR) ;
  }

  if ( kbps_line_rate == 0 ) {
    /* Disable the traffic with the specified priority */
    return ( m2r_rmac_set_host_rate_limit_params(port_id, prio,
                    1, 0, 0) ) ;
  }

  /*
   * Since packets are going out of RingMAC block with 64-bit per clock,
   * the total bandwaith lr is calculated as follows. But the effective
   * bandwidth is dependant on the line rate configuration.
   */
  lr = 64 * M2R_CORE_CLOCK ;
  scale_up = 65535 * 100 / lr ;

  if ( kbps_line_rate * 8 * scale_up / 100000 < 1 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
      " : pratical line rate(%d) is too small to be limited\n",
      kbps_line_rate) ;
    return(CS_ERROR) ;
  }

  if ( kbps_line_rate * 8 / 1000 < 1000 ) {
    a = (cs_uint16)(kbps_line_rate * 8 * scale_up / 100000) ;
    b = (cs_uint16)(lr * scale_up / 100) ;
  } else if (kbps_line_rate * 8 / 1000 > 65535)  {
    scale_dn = kbps_line_rate * 8 / 65535 ;
    b = (cs_uint16) (lr * 1000 / scale_dn) ;
    a = 65535 ;
  } else {
    b = (cs_uint16)lr ;
    a = (cs_uint16)(kbps_line_rate * 8 / 1000) ;
  }

  if ( (prio != M2R_TRAFFIC_PRIO_MED) &&
       (prio != M2R_TRAFFIC_PRIO_A0) ) {

    if ( a > M2R_RMAC_MAX_BURST_DEFAULT ) {
      scale_dn = a / M2R_RMAC_MAX_BURST_DEFAULT ;
      a = a / scale_dn ;
      b = b / scale_dn ;
    }

    c = (cs_uint16) M2R_GET_MAX_SHARE_FACTOR( a, b ) ;
    if ( c > 1 ) {
      a = a / c ;
      b = b / c ;
    }
  } else {
    c = ( a > b ) ? a : b ;
    if ( c < 32000 ) {
      scale_up = 32000 * 100 / c ;
      a = (cs_uint16)(a * scale_up / 100) ;
      b = (cs_uint16)(b * scale_up / 100) ;
    }
  }

  tmr_val = b - 1 ;
  delta   = a ;
  max     = (prio == M2R_TRAFFIC_PRIO_MED) ?
                0xFFFF : ((a > M2R_RMAC_MAX_BURST_DEFAULT) ?
                             a : M2R_RMAC_MAX_BURST_DEFAULT) ;

  return ( m2r_rmac_set_host_rate_limit_params(port_id, prio,
                    tmr_val, delta, max) ) ;
}

/****************************************************************/
/* $rtn_hdr_start SET HOST TX RATE LIMIT BASED ON PERCENTAGE    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_prio_traffic_host_rate_limit(cs_uint16 port_id,
		   m2r_prio_traffic_t prio, m2r_port_line_rate_t line_rate,
		   cs_uint8 percentage)
/* INPUTS     : o Port Id                                       */
/*              o Traffic priority type                         */
/*              o Port Line Rate                                */
/*              o Requested bandwidth percentage                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the rate limit for the specified priority for Host       */
/* sourced traffic based on requested bandwidth percentage.     */
/*                                                              */
/* [prio] could have the following values:                      */
/* M2R_TRAFFIC_PRIO_LOW,          M2R_TRAFFIC_PRIO_MED,         */
/* M2R_TRAFFIC_PRIO_HI,           M2R_TRAFFIC_PRIO_A0,          */
/* M2R_TRAFFIC_PRIO_CTL,          M2R_TRAFFIC_PRIO_HIRES        */
/* Among the above values,  M2R_TRAFFIC_PRIO_LOW and            */
/* M2R_TRAFFIC_PRIO_HI are the only valid traffic priorities in */
/* SRP mode.                                                    */
/*                                                              */
/* For MILAN2, [line_rate] can only be the followings:          */
/* M2R_PORT_RATE_OC192C,          M2R_PORT_RATE_OC48C,          */
/* M2R_PORT_RATE_10G                                            */
/*                                                              */
/* Parameters [percentage] specify how many amount of line      */
/* bandwidth is requested, from 0 to 100.                       */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32     kbps ;
 
  if (percentage > 100) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		  " : percentage(%d) should not be larger than 100\n", 
		  percentage) ; 
    return(CS_ERROR) ;
  }

  if ( line_rate == M2R_PORT_RATE_OC192 || 
       line_rate == M2R_PORT_RATE_OC48 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
      ": In SPR and RPR mode, line can only be at single channel mode\n") ; 
    return(CS_ERROR) ;
  }

  kbps = M2R_GET_KBPS_LR_RATE(line_rate) * percentage / 100 ;
 
  return ( m2r_rmac_set_prio_traffic_host_rate_limit_kbps(port_id, prio, 
                          line_rate, kbps) ) ;
}

/****************************************************************/
/* $rtn_hdr_start  CONTROL HOST TX RATE LIMITER                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status m2r_rmac_ctl_host_rate_limiter(cs_uint16 port_id,
					 m2r_prio_traffic_t prio,
					 cs_boolean always_pass,
					 cs_boolean always_block)
/* INPUTS     : o Port Id                                       */
/*              o Traffic Priority                              */
/*              o TRUE(let all packets go through) or FALSE     */
/*              o TRUE(no packets allowed) or FALSE             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls the function of the specified rate limiter block.   */
/* Normally, the rate-limiter operates as per the programmed    */
/* rate parameters.                                             */
/* These controls override any rate parameter settings.         */ 
/*                                                              */
/* The [always_pass] argument when set to TRUE will always NOT  */ 
/* backpressure the HOST for the channel corresponding  to the  */ 
/* specified traffic priority, and let all packets go through.  */
/* This corresponds to the *_OFF bit in the registers.          */  
/*                                                              */
/* The [always_block] argument when set to TRUE will always     */ 
/* backpressure the HOST for the channel corresponding to the   */ 
/* specified traffic priority, and so no packets get through.   */
/* This corresponds to the *_DIS bit in the registers, and this */
/* overrides the [always_pass] setting.                         */
/* Note: The hardware does not support the [always_block] option*/ 
/* control packets(ie. prio = M2R_TRAFFIC_PRIO_CTL) in which    */
/* [always_block] arg is ignored.                               */ 
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;
  m2r_port_mac_mode_t	mac_mode ;
  DR_MICRO_IF_CR1_t	cr1 ;
  DR_RPR_CRLMT_t	crlmt ;
  DR_RPR_CR2A_t		cr2a ; 

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }                                      
  if (mac_mode == M2R_PORT_IN_SRP_MODE) {
    if ( (prio != M2R_TRAFFIC_PRIO_LOW) && 
	 (prio != M2R_TRAFFIC_PRIO_HI) ) {
      CS_HNDL_ERROR( port_id, EM2R_RMAC_INVALID_IN_SRP,
		     ": invalid priority(%d)\n", prio) ;
      return(CS_ERROR) ;
    }
  }
 
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  switch (prio) {
    default : 
    case M2R_TRAFFIC_PRIO_LOW :
      cr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
      cr1.bf.LO_RATE_OFF = (always_pass) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;

      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.LO_DIS = (always_block) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;
      break ;	

    case M2R_TRAFFIC_PRIO_MED :
      cr2a.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2A.wrd) ;
      cr2a.bf.MED_RATE_OFF = (always_pass) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2A.wrd, cr2a.wrd) ;
 
      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.MED_DIS = (always_block) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;      
      break ;

    case M2R_TRAFFIC_PRIO_HI : 
      cr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
      cr1.bf.HI_RATE_OFF = (always_pass) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;

      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.HI_DIS = (always_block) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;
      break ;
      
    case M2R_TRAFFIC_PRIO_A0 : 
      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.HIA0_OFF = (always_pass) ? 1 : 0 ;
      crlmt.bf.HIA0_DIS = (always_block) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;      
      break ;

    case M2R_TRAFFIC_PRIO_CTL : 
      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.CTL_OFF = (always_pass) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;
      /* no always_block option for Control packets */
      break ;

    case M2R_TRAFFIC_PRIO_HIRES : 
      cr2a.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2A.wrd) ;
      cr2a.bf.HIRES_OFF = (always_pass) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2A.wrd, cr2a.wrd) ;
      /* no always_block option for unreserved packets */
      break ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONTROL SUBCLASS A0/1 MARKING                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_classA_marking(cs_uint16 port_id, m2r_ctl_t ctl) 
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls how to remark the classA traffic with A0/1.         */
/*                                                              */  
/* When [ctl] is M2R_DISABLE, RingMAC block won't do anything   */
/* with marking, instead, it applys rate limiting function to   */
/* the class A traffic based on pre-marking by the host.        */
/*                                                              */
/* When [ctl] is M2R_ENABLE, RingMAC block will ignore marking  */
/* flag in received classA packets and mark A0/1 based on       */
/* allocated subclassA0 bandwidth consumption.                  */
/*                                                              */
/* NOTE: Class A remarking should be enabled all the time except*/
/* for test purpose to call this API.                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		 dev_num, sl_num ; 
  M2R_t        * pDev ;
  DR_RPR_CR2A_t  cr2a ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
	 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr2a.wrd = 
     M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2A.wrd);
  if (ctl == M2R_ENABLE) {
    cr2a.bf.ACLASS_OFF = 0;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2A.wrd,
                 cr2a.wrd);
  } else {
    cr2a.bf.ACLASS_OFF = 1;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2A.wrd,
                 cr2a.wrd);
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONTROL SUBCLASS B0/1 MARKING                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_classB_marking(cs_uint16 port_id, m2r_ctl_t ctl, 
                                       cs_boolean host_premark)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/*              o TRUE(recognize host marking) or FALSE(ignore) */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls how to remark the classB traffic with B0/1.         */
/*                                                              */  
/* When [ctl] is M2R_DISABLE, RingMAC block won't do anything   */
/* with marking, instead, it applys rate limiting function to   */
/* the class B traffic based on pre-marking by the host. In this*/
/* mode, [host_premark] has no effect.                          */
/*                                                              */
/* When [ctl] is M2R_ENABLE and [host_premark] is also TRUE,    */
/* RingMAC block will mark the received in-profile classB0      */
/* packets as B0 and out-profile classB0 packets as B1, but     */
/* still treat received classB1 packets as classB1 traffic.     */
/*                                                              */
/* When [ctl] is M2R_ENABLE and [host_premark] is FALSE,        */
/* RingMAC block will ignore marking flag in received classB    */
/* packets and mark B0/1 based on classB traffic in-profile or  */
/* out-profile situation.                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		 dev_num, sl_num ; 
  M2R_t        * pDev ;
  DR_RPR_CR2A_t  cr2a ;
  DR_RPR_CRLMT_t crlmt ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
	 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr2a.wrd = 
     M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2A.wrd);
  if (ctl == M2R_ENABLE) {
    cr2a.bf.MED_IOP_OFF = 0;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2A.wrd,
                 cr2a.wrd);
    crlmt.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd);
    crlmt.bf.MED_MODE = (host_premark) ? 1 : 0;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd,
                 crlmt.wrd);
  } else {
    cr2a.bf.MED_IOP_OFF = 1;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2A.wrd,
                 cr2a.wrd);
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start CONTROL HOST TX RATE LIMITER CREDIT RESETTING */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_reset_host_rate_limiter(cs_uint16 port_id, 
                     m2r_prio_traffic_t prio, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Traffic Priority                              */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control if resetting the rate limiting credit of the traffic */
/* with the specified priority when no corresponding HOST       */
/* packets waiting.                                             */
/*                                                              */  
/* The [prio] allowed by this API are the followings:           */
/* M2R_TRAFFIC_PRIO_A0,         M2R_TRAFFIC_PRIO_HI,            */
/* M2R_TRAFFIC_PRIO_MED,        M2R_TRAFFIC_PRIO_LOW            */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		      dev_num, sl_num ; 
  M2R_t         	  * pDev ;
  m2r_port_mac_mode_t	mac_mode ;
  DR_RPR_CRLMT_t	crlmt ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }                                      
  if (mac_mode == M2R_PORT_IN_SRP_MODE) {
    if ( (prio != M2R_TRAFFIC_PRIO_LOW) && 
	 (prio != M2R_TRAFFIC_PRIO_HI) ) {
      CS_HNDL_ERROR( port_id, EM2R_RMAC_INVALID_IN_SRP,
		     ": invalid priority(%d)\n", prio) ;
      return(CS_ERROR) ;
    }
  }
 
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  switch (prio) {
    default : 
    case M2R_TRAFFIC_PRIO_LOW :
      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.LO_RESET = (ctl==M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;
      break ;	

    case M2R_TRAFFIC_PRIO_MED :
      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.MED_RESET = (ctl==M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;
      break ;

    case M2R_TRAFFIC_PRIO_HI : 
      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.HI_RESET = (ctl==M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;
      break ;
      
    case M2R_TRAFFIC_PRIO_A0 : 
      crlmt.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd) ;
      crlmt.bf.HIA0_RESET = (ctl==M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd, crlmt.wrd) ;
      break ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    SET UNRESERVED RATE LIMITER CONTROL        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_unrsrvd_rate_limiter(cs_uint16 port_id, 
                     m2r_ctl_t med_ctl, m2r_ctl_t hi_ctl,
                     m2r_ctl_t tblo_ctl, m2r_ctl_t ctloff_ctl )
/* INPUTS     : o Port Id                                       */
/*              o Enable Medium Priority backpressure           */
/*              o Enable High Priority backpressure             */
/*              o Enable STQ backpressure                       */
/*              o Disable counting of control packets           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the Unreserved(Downstream) rate limiter which counts    */
/* all packets that are not of classA0 (i.e. classA1, classB,   */
/* classC) from the transit path (TB) and from the host.        */
/*                                                              */  
/* [med_ctl] enable medium priority backpressure from unreserved*/
/* rate limiter.                                                */
/*                                                              */  
/* [hi_ctl] enable high priority backpressure from unreserved   */
/* rate limiter.                                                */
/*                                                              */
/* [tblo_ctl] enable STQ backpressure from unreserved rate      */
/* limiter.                                                     */
/*                                                              */  
/* [ctloff_ctl] disable counting of control packets in the      */
/* unreserved rate limiter.                                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		 dev_num, sl_num ; 
  M2R_t        * pDev ;
  DR_RPR_CR2A_t  cr2a ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
	 (M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr2a.wrd = 
     M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2A.wrd);

  cr2a.bf.HIRES_HMED = (med_ctl == M2R_ENABLE) ? 1 : 0;
  cr2a.bf.HIRES_HHI = (hi_ctl == M2R_ENABLE) ? 1 : 0;
  cr2a.bf.HIRES_TBLO = (tblo_ctl == M2R_ENABLE) ? 1 : 0;
  cr2a.bf.HIRES_CTLOFF = (ctloff_ctl == M2R_ENABLE) ? 1 : 0;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2A.wrd,
               cr2a.wrd);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start       SET CONTROL RATE LIMITER                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_control_rate_limiter(cs_uint16 port_id,
                     m2r_ctl_rate_limiter_mode_t mode )
/* INPUTS     : o Port Id                                       */
/*              o Control packet rate limiting mode             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the rate limiting mode for control packets.             */
/*                                                              */
/* [mode] has the following options:                            */
/* M2R_CTL_A0_CREDIT    - control packet also takes A0 credit   */
/* M2R_CTL_CLASS_CREDIT - control packet also takes associated  */
/*                        service class credit                  */
/* M2R_CTL_ONLY         - control packet only takes control rate*/
/*                        limiter credit                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		 dev_num, sl_num ; 
  M2R_t        * pDev ;
  DR_RPR_CRLMT_t crlmt ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
			                 CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  crlmt.wrd = 
     M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CRLMT.wrd);

  switch (mode) {
  	case M2R_CTL_A0_CREDIT:
  	  crlmt.bf.CTLA0_CREDIT = 1;
  	  crlmt.bf.CTL_CLASS = 0;
  	  break;

  	case M2R_CTL_CLASS_CREDIT:
  	  crlmt.bf.CTLA0_CREDIT = 0;
  	  crlmt.bf.CTL_CLASS = 1;
  	  break;

  	case M2R_CTL_ONLY:
  	  crlmt.bf.CTLA0_CREDIT = 0;
  	  crlmt.bf.CTL_CLASS = 0;
  	  break;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CRLMT.wrd,
               crlmt.wrd);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}




/****************************************************************/
/* $rtn_hdr_start  SET HOST PACKET SIZE FILTERS                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_host_set_pkt_size_filters(cs_uint16 port_id,
					      cs_uint16 rxh_giant_sz, 
		        cs_uint8 txh_runt_sz, cs_uint16 txh_giant_sz )
/* INPUTS     : o Port Id                                       */
/*              o RXH Giant Packet size(in bytes: 14-bit value) */
/*              o TXH Runt Packet size(in bytes: 6-bit value)   */
/*              o TXH Giant Packet size(in bytes: 14-bit value) */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set RingMAC Host runt and giant size packet filters.         */
/*                                                              */  
/* The [rxh_giant_sz] parameter indicates the maximum packet    */
/* size that will be be allowed through the RXH section.        */
/* A value of 0 will disable this filter.                       */
/*                                                              */
/* The [txh_runt_sz] parameter indicates the minimum packet size*/
/* that will be be allowed through the TXH section.             */
/* A value of 0 will disable this filter.                       */
/*                                                              */
/* The [txh_giant_sz] parameter indicates the maximum packet size*/
/* that will be be allowed through the TXH section.             */
/* A value of 0 will disable this filter.                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (rxh_giant_sz > 0x3fFF) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
	    " : rxh_giant_sz(0x%x) s/b <= 0x3fff\n", rxh_giant_sz) ; 
    return(CS_ERROR) ;
  }
  
  if (txh_runt_sz > 0x7F) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		  " : txh_runt_sz(0x%x) s/b <= 0x7f\n", txh_runt_sz) ; 
    return(CS_ERROR) ;
  }

  if (txh_giant_sz > 0x3FFF) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
	    " : txh_giant_sz(0x%x) s/b <= 0x3fff\n", txh_giant_sz) ; 
    return(CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RXH_GIANTSZ.wrd, 
	       (cs_reg) rxh_giant_sz) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TXH_RUNTSZ.wrd, 
	       (cs_reg) txh_runt_sz) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TXH_GIANTSZ.wrd,
	       (cs_reg) txh_giant_sz) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/*************************** FRAMER  **************************/

/****************************************************************/
/* $rtn_hdr_start  SET RXF PACKET SIZE FILTERS                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_rxf_pkt_size_filters(cs_uint16 port_id,
		  cs_uint8 rxf_runt_sz, cs_uint16 rxf_giant_sz)
/* INPUTS     : o Port Id                                       */
/*              o Runt Packet size(in bytes: 6-bit value)       */
/*              o Giant Packet size(in bytes: 14-bit value)     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set RXF runt and giant size packet filters.                  */
/*                                                              */  
/* The [rxf_runt_sz] parameter indicates the minimum packet size*/
/* that will be be allowed through the RXF section.             */
/* A value of 0 will disable this filter.                       */ 
/*                                                              */  
/* The [rxf_giant_sz] parameter indicates the maximum packet    */
/* size that will be be allowed through the RXF section.        */
/* A value of 0 will disable this filter.                       */ 
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (rxf_runt_sz > 0x7F) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		  " : runt_sz(0x%x) s/b <= 0x7f\n", rxf_runt_sz) ; 
    return(CS_ERROR) ;
  }
  
  if (rxf_giant_sz > 0x7FFF) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		  " : giant_sz(0x%x) s/b <= 0x7fff\n", rxf_giant_sz) ; 
    return(CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RXF_RUNTSZ.wrd, 
	       (cs_reg) rxf_runt_sz) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RXF_GIANTSZ.wrd, 
	       (cs_reg) rxf_giant_sz) ;
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start        RXF TIMER CONTROL                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_rxf_timer_mark_err_packet(cs_uint16 port_id, 
						  m2r_ctl_t ctl) 
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control if marking packet as error packet when RXF timer is  */
/* expired.                                                     */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;
  DR_FRAMER_CR5_t       cr5 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr5.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR5.wrd);
  cr5.bf.NO_ERR_RXF_TIMER = (ctl==M2R_ENABLE) ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR5.wrd, cr5.wrd) ;
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}			 

                             
/****************************************************************/
/* $rtn_hdr_start        RXF STOMP FEATURE CONTROL              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_rxf_stomp(cs_uint16 port_id, m2r_ctl_t stomp_ctl,
                                  cs_boolean err_stomp_host_flag) 
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/*              o TRUE or FALSE                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets stomp control of RXF.                                   */
/*                                                              */
/* When [stomp_ctl] is M2R_ENABLE, received packets with a bad  */
/* crc have the old crc removed, and an inverted version of the */
/* correct crc added to the packets. Stomping is only done on   */
/* data packet.                                                 */
/*                                                              */
/* [err_stomp_host_flag] indicates if marking stomped packets   */
/* destined to HOST as error packets.  This parameter works only*/
/* when [stomp_ctl] is M2R_ENABLE.                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;
  DR_FRAMER_CR5_t       cr5 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr5.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR5.wrd);
  cr5.bf.STOMP = (stomp_ctl==M2R_ENABLE) ? 1 : 0 ;
  cr5.bf.ERR_STOMP_HOST = (err_stomp_host_flag==TRUE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR5.wrd, cr5.wrd) ;
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}			 

/****************************************************************/
/* $rtn_hdr_start       TXF RATE SHAPER CONTROL                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_ctl_txf_rate_shape(cs_uint16 port_id, m2r_ctl_t ctl) 
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the txf rate shaper.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;
  DR_FRAMER_CR5_t       cr5 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr5.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR5.wrd);
  cr5.bf.TXF_RATESHAPE = (ctl==M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR5.wrd, cr5.wrd) ;
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}			 
                             
/****************************************************************/
/* $rtn_hdr_start  SET TXF RATE SHAPER PARAMETERS               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_rmac_set_txf_rate_shape_params(cs_uint16 port_id, 
			  cs_uint16 tmr_val, cs_uint16 delta, 
					      cs_uint16 max_accum) 
/* INPUTS     : o Port Id                                       */
/*              o Timer value                                   */ 
/*              o Delta                                         */ 
/*              o Maximum accumulated value                     */ 
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets rate shaping parameters for ALL traffic going out of    */
/* this RingMAC to the Framer.                                  */
/*                                                              */
/* The [tmr_val], [delta], and [max_accum] are parameters that  */
/* needs to be specified to get the desired rate limit.         */
/* Refer to the HW spec for more info.                          */
/*                                                              */
/* Note: This is different from the priority(or class) basesd   */
/* rate-limiting used by SRP/RPR to control traffic coming from */
/* Host.                                                        */ 
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TXF_RATETIME.wrd,
	       tmr_val) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TXF_RATEDELTA.wrd,
	       delta) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TXF_RATEMAX.wrd, 
	       max_accum) ;
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}			 
                             
/****************************************************************/
/* $rtn_hdr_start  SET TXF RATE SHAPER BASED ON BANDWIDTH USAGE */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_set_app_txf_rate_shape(cs_uint16 port_id,
                                           cs_uint32 kbps_line_rate)
/* INPUTS     : o Port Id                                       */
/*              o Pratical Port Line Rate in kbps               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Limit the bandwidth usage at RingMAC TXF interface based on  */
/* the user-specified pratical line rate in unit of kbps.       */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16     tmr_val, delta, max ;
  cs_uint32     lr ;
  cs_uint32     scale_up, scale_dn ;
  cs_uint16     a, b, c ;

  if ( kbps_line_rate == 0 ) {
    /* Disable the traffic */
    return ( m2r_rmac_set_txf_rate_shape_params(port_id, 1, 0, 0) ) ;
  }

  /*
   * Since packets are going out of RingMAC block with 64-bit per clock,
   * the total bandwaith lr is calculated as follows. But the effective
   * bandwidth is dependant on the line rate configuration.
   */
  lr = 64 * M2R_CORE_CLOCK ;
  scale_up = 65535 * 100 / lr ;

  if ( kbps_line_rate * 8 * scale_up / 100000 < 1 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
      " : pratical line rate(%d) is too small to be limited\n",
      kbps_line_rate) ;
    return(CS_ERROR) ;
  }

  if ( kbps_line_rate * 8 / 1000 < 1000 ) {
    a = (cs_uint16)(kbps_line_rate * 8 * scale_up / 100000) ;
    b = (cs_uint16) lr * scale_up / 100 ;
  } else if (kbps_line_rate * 8 / 1000 > 65535)  {
    scale_dn = kbps_line_rate * 8 / 65535 ;
    b = (cs_uint16) lr * 1000 / scale_dn ;
    a = 65535 ;
  } else {
    b = (cs_uint16)lr ;
    a = (cs_uint16)(kbps_line_rate * 8 / 1000) ;
  }

  if ( a > M2R_RMAC_MAX_BURST_DEFAULT ) {
    scale_dn = a / M2R_RMAC_MAX_BURST_DEFAULT ;
    a = a / scale_dn ;
    b = b / scale_dn ;
  }

  c = (cs_uint16) M2R_GET_MAX_SHARE_FACTOR( a, b ) ;

  if ( c > 1 ) {
        a = a / c ;
        b = b / c ;
  }

  /* actual register value should be 1 less than real value */ 
  tmr_val = b - 1 ;
  delta   = a ;
  max     = (a > M2R_RMAC_MAX_BURST_DEFAULT) ?
                  a : M2R_RMAC_MAX_BURST_DEFAULT ;

  return ( m2r_rmac_set_txf_rate_shape_params(port_id,
                                tmr_val, delta, max) ) ;
}

/********************* RingMAC Diagnostics  *********************/
/****************************************************************/
/* $rtn_hdr_start CONTROL IF SENDING MISMATCH MULTICAST TO HOST */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status m2r_rmac_ctl_mismatch_multicast(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls whether the multicast packet with a multicast des-  */
/* -tination address not getting match in the CAM or Coarse CAM */
/* gets sent to the HOST.                                       */
/*                                                              */
/* Note: This API is usually used for diagnostics purpose.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		    dev_num, sl_num ; 
  M2R_t             * pDev ;
  DR_MICRO_IF_CR1_t cr1 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr1.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
  
  cr1.bf.MULTI_MATCH = (ctl==M2R_ENABLE) ? 1 : 0 ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET FMRO DEBUG BUS FOR RINGMAC               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Milan2                                          */
cs_status m2r_set_rmac_fmro(cs_uint16 port_id,
		       cs_uint8 blk1, cs_uint8 view1,
                       cs_uint8 blk0, cs_uint8 view0)
/* INPUTS     : o Port Id                                       */
/*              o Block-1                                       */
/*              o View-1                                        */
/*              o Block-0                                       */
/*              o View-0                                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */ 
/* DESCRIPTION:                                                 */
/* Sets the Fault Monitoring Registered Output(FMRO) selection  */
/* for which signals to be routed to the output pins.           */ 
/* Refer to the HW spec for more information and the possible   */
/* values for the blocks and views.                             */
/* NOTE - Signals from other blocks can also be routed out but  */
/* this API currently supports only RingMAC section.            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                dev_num, sl_num ;
  M2R_t                    * pDev ;
  MPIF_Mode_t	           mpif_mode ;
  DR_MICRO_IF_FMRO_SEL_t   fmro_sel ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_MPIF ) ;
  mpif_mode.wrd = M2R_REG_READ( &pDev->MicroIF.Mode.wrd ) ;
  mpif_mode.bf.DiagBus = DiagBus_RPR ;
  M2R_REG_WRITE( &pDev->MicroIF.Mode.wrd, mpif_mode.wrd ) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_MPIF) ;

  fmro_sel.wrd = M2R_REG_READ( &pDev->slice[sl_num].RingMAC.FMRO_SEL.wrd ) ;
  fmro_sel.wrd = ( ((blk1 & 0x1F) << 11) | ((view1 & 7) << 8) |
		   ((blk0 & 0x1F) << 3)  | (view0 & 7) ) ;
  M2R_REG_WRITE( &pDev->slice[sl_num].RingMAC.FMRO_SEL.wrd, fmro_sel.wrd ) ;

  return (CS_OK) ;
}


/********************* RingMAC Private APIs ************************/
/******************************************************************* 
 * Setting MAC mode selection to SRP or RPR.                       *
 * This is PRIVATE - it is called by  m2r_set_port_mac_mode().     *
 *******************************************************************/
cs_status  m2r_rmac_set_mac_mode_sel(cs_uint16 port_id, 
				     m2r_port_mac_mode_t mode)
{
  cs_uint16		dev_num, sl_num ; 
  M2R_t         	* pDev ;
  DR_MICRO_IF_CR0_t	cr0 ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
  cr0.bf.RPRMODE = (mode == M2R_PORT_IN_RPR_MODE) ? 1 : 0 ; 
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return(CS_OK) ;
}

/***************************************************************** 
 * Get MAC mode selection to SRP or RPR.                         *
 * This is PRIVATE.                                              *
 *****************************************************************/
m2r_port_mac_mode_t m2r_rmac_get_mac_mode_sel(cs_uint16 port_id)
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;
  DR_MICRO_IF_CR0_t	cr0 ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
  if (cr0.bf.RPRMODE) return (M2R_PORT_IN_RPR_MODE) ;
  else return(M2R_PORT_IN_SRP_MODE) ;
}


/******************************************************************
 * Control of the setting of WS(or "ps" bit as in RPR standard)   *
 * for packets that are sourced from the Host.                    *
 * The control bits are in the MAC Rewrite block.                 *
 ******************************************************************/
cs_status  m2r_rmac_ctl_ws_for_pkts_from_host(cs_uint16 port_id,
					      cs_boolean setws,
					      cs_uint8   ws)
{
  cs_uint16			dev_num, sl_num ;
  M2R_t				* pDev ;
  DR_MAC_HEAD_MAC_MODE_t       	macmode ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  macmode.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd) ;
  macmode.bf.SETWS = (setws) ? 1 : 0 ;
  macmode.bf.WS    = ws ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd, macmode.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/******************************************************************
 * Control of striping the frame with "we=0" and wrong ring-id    *
 * out of the Ring while supporting the edge node to send the     *
 * packets with "we=0" as well when deploying WRAPPING protection *
 * method.                                                        *
 ******************************************************************/
cs_status  m2r_rmac_ctl_we_in_wrap(cs_uint16 port_id, 
                                   cs_boolean strip_we,
                                   cs_boolean mate_we_check)
{
  cs_uint16                     dev_num, sl_num ;
  M2R_t                         * pDev ;
  DR_MICRO_IF_CR1_t             cr1 ;
  DR_MATE_CR3_t                 cr3 ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
  cr1.bf.STRIP_WE = (strip_we) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;

  cr3.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR3_.wrd) ;
  cr3.bf.WE_DROP_OFF = (mate_we_check) ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR3_.wrd, cr3.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}



