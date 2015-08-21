/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_cam.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_rmac.h"
#include "m2r_rmac_cam.h"
#include "m2r_rmac_priv.h"


/****************************************************************
 *   Forward declaration of internal utilty functions           *
 ****************************************************************/
static
cs_status  m2r_rmac_reset_addr_cam(cs_uint16 port_id,
				   m2r_rmac_mem_id_t mem_id,
				   cs_uint32 start,
				   cs_uint32 num_entries) ;
static
cs_status  m2r_rmac_reset_status_sram(cs_uint16 port_id,
				      m2r_rmac_mem_id_t mem_id,
				      cs_uint32 start,
				      cs_uint32 num_entries) ;
static
cs_status  m2r_rmac_reset_acctg_sram(cs_uint16 port_id,
				     m2r_rmac_mem_id_t mem_id,
				     cs_uint32 start,
				     cs_uint32 num_entries) ;
static
cs_status  m2r_rmac_reset_mseq_mem(cs_uint16 port_id,
                                   m2r_rmac_mem_id_t mem_id,
                                   cs_uint32 start,
                                   cs_uint32 num_entries) ;
static
cs_status  m2r_rmac_reset_coarse_cam(cs_uint16 port_id,
                                     m2r_rmac_mem_id_t mem_id,
                                     cs_uint32 start,
                                     cs_uint32 num_entries) ;
static
cs_status  m2r_rmac_reset_mchoke_sram(cs_uint16 port_id,
                                      cs_uint32 start,
                                      cs_uint32 num_entries) ;
static
cs_status  m2r_rmac_reset_host_drop_cam(cs_uint16 port_id,
                                        m2r_rmac_mem_id_t mem_id,
                                        cs_uint32 start,
                                        cs_uint32 num_entries) ;
static
cs_status  m2r_rmac_reset_host_drop_cntr(cs_uint16 port_id,
                                        cs_uint32 start,
                                        cs_uint32 num_entries) ;
static
cs_status  m2r_write_to_addr_cam(cs_uint16 port_id,
				 m2r_rmac_mem_id_t cam_id,
				 cs_uint32 entry_num,
				 m2r_cam_macaddr_t * p) ;
static
void  m2r_print_addr_cam_entry(cs_uint16 port_id,
			       m2r_rmac_mem_id_t cam_id,
			       cs_uint16 entry_num,
			       m2r_rmac_gram_t * pgram,
			       cs_uint16 status_bits) ;
static
cs_status  m2r_rmac_read_acctg_sram_cntr(cs_uint16 port_id,
					 m2r_rmac_mem_id_t sram_id,
					 cs_uint16 entry_num,
					 m2r_rmac_cntr_t * pcntr) ;
static
cs_status  m2r_rmac_update_acctg_drvr_stats(cs_uint16 port_id,
				       m2r_rmac_mem_id_t sram_id,
				       cs_uint16 start, cs_uint16 num) ;

static
m2r_rmac_cntr_t * m2r_rmac_get_acctg_sram_stats_tbl_ptr(
				       cs_uint16 port_id,
				       m2r_rmac_mem_id_t sram_id) ;

/* Externs */
extern
cs_boolean m2r_drvr_is_stats_in_hex_fmt(void) ;
extern
cs_status m2r_access_status_sram_16bit_entry(cs_uint16 port_id,
                  m2r_rmac_mem_id_t ssram_id, m2r_rw_t op,
                       cs_uint32 entry_num, cs_uint16 * pbits) ;
extern
cs_status m2r_access_status_sram_4bit_entry(cs_uint16 port_id,
                             m2r_rmac_mem_id_t ssram_id, m2r_rw_t op,
                             cs_uint32 entry_num, cs_uint16 * pbits) ;

/****************************************************************/
/* $rtn_hdr_start  INITIALIZE RINGMAC MEMORIES                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_rmac_init_mem(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reset, clear or invalidate ALL the various memories(cams,    */
/* sram's) in RingMAC block.                                    */
/* For individiual memory reset, use m2r_rmac_reset_mem() API.  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  CS_PRINT("Initializing RingMAC memories.\n") ;

  if ( CS_IS_REG_NAME_DUMP_ON() ) {
    CS_PRINT(
      "Suppressing reg dump temporarily to keep dump size small\n"
    ) ;
    CS_TURN_OFF_REG_NAME_DUMP() ;
  }

  /* Lookup(Rx) section */
  if ( m2r_rmac_reset_mem(port_id, M2R_RX_CAM,
			  0, M2R_NUM_OF_ADDR_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_RX_STATUS_SRAM,
			  0, M2R_NUM_OF_ADDR_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_RX_ACCTG_SA_SRAM,
			  0, M2R_ADDR_CAM_SA_NUM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_RX_ACCTG_DA_SRAM,
			  0, M2R_NUM_OF_ADDR_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Stat(Tx) section */
  if ( m2r_rmac_reset_mem(port_id, M2R_TX_CAM,
			  0, M2R_NUM_OF_ADDR_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_TX_STATUS_SRAM,
			  0, M2R_NUM_OF_ADDR_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_TX_ACCTG_SA_SRAM,
			  0, M2R_ADDR_CAM_SA_NUM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_TX_ACCTG_DA_SRAM,
			  0, M2R_NUM_OF_ADDR_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* MS */
  if ( m2r_rmac_reset_mem(port_id, M2R_MS_DATA_STORE,
			  0, M2R_NUM_OF_MS_DATA_STORE_ENTRIES)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_MS_CORE_STORE,
			  0, M2R_NUM_OF_MS_CORE_STORE_ENTRIES)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_reset_mem(port_id, M2R_MS_PGM_STORE,
			  0, M2R_NUM_OF_MS_PGM_STORE_ENTRIES)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Coarse Filter CAM */
  if ( m2r_rmac_reset_mem(port_id, M2R_MCAST_COARSE_CAM,
			  0, M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }
  /* counter */
  if ( m2r_rmac_reset_mem(port_id, M2R_MCAST_COARSE_CNTRS,
			  0, M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Host Pkt Drop Coarse Filter CAM */
  if ( m2r_rmac_reset_mem(port_id, M2R_HOST_PKT_DROP_CAM,
			  0, M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }
  /* counter */
  if ( m2r_rmac_reset_mem(port_id, M2R_HOST_PKT_DROP_CNTRS,
			  0, M2R_NUM_OF_HOST_PKT_DROP_CNTR_ENTRIES) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* MAC Rewrite Index SRAM */
  if ( m2r_rmac_reset_mem(port_id, M2R_MAC_REWRITE_SRAM,
			  0, M2R_NUM_OF_MAC_REWRITE_SRAM_ENTRIES ) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Multi-choke SRAM */
  if ( m2r_rmac_reset_mem(port_id, M2R_MS_MCHOKE_SRAM,
			  0, M2R_NUM_OF_MCHOKE_CAM_ENTRIES ) ==
       CS_ERROR ) {
    return (CS_ERROR) ;
  }

  CS_RESTORE_REG_NAME_DUMP() ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  RESET/INVALIDATE/CLEAR CAM & SRAM ENTRIES    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_rmac_reset_mem(cs_uint16 port_id,
			     m2r_rmac_mem_id_t mem_id,
                     cs_uint32 start, cs_uint32 num_entries)
/* INPUTS     : o Port Id                                       */
/*    		o Memory d                                      */
/*              o First entry                                   */
/*              o Num of entries                                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reset, clear or invalidate the entries for the specified     */
/* memory(CAM, SRAM).                                           */
/*                                                              */
/* The [which_mem] parameter is one of the following:           */
/* M2R_RX_CAM,                      M2R_RX_STATUS_SRAM,         */
/* M2R_RX_ACCTG_SA_SRAM,            M2R_RX_ACCTG_DA_SRAM,       */
/* M2R_TX_CAM,                      M2R_TX_STATUS_SRAM,         */
/* M2R_TX_ACCTG_SA_SRAM,            M2R_TX_ACCTG_DA_SRAM,       */
/* M2R_MS_DATA_STORE,               M2R_MS_CORE_STORE,          */
/* M2R_MS_PGM_STORE,                M2R_MCAST_COARSE_CAM,       */
/* M2R_MCAST_COARSE_CNTRS,          M2R_MAC_REWRITE_SRAM,       */
/* M2R_MS_MCHOKE_SRAM,              M2R_HOST_PKT_DROP_CAM,      */
/* M2R_HOST_PKT_DROP_CNTRS.                                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t        		* pDev ;
  cs_status		ret = CS_ERROR ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  switch (mem_id) {
    case M2R_RX_CAM :
    case M2R_TX_CAM :
      ret = m2r_rmac_reset_addr_cam(port_id, mem_id, start, num_entries) ;
      break ;

    case M2R_RX_STATUS_SRAM :
    case M2R_TX_STATUS_SRAM :
      ret = m2r_rmac_reset_status_sram(port_id, mem_id, start,
				       num_entries) ;
      break ;

    case M2R_RX_ACCTG_SA_SRAM :
    case M2R_RX_ACCTG_DA_SRAM :
    case M2R_TX_ACCTG_SA_SRAM :
    case M2R_TX_ACCTG_DA_SRAM :
      ret = m2r_rmac_reset_acctg_sram(port_id, mem_id, start,
				      num_entries) ;
      break ;

    case M2R_MS_DATA_STORE :
    case M2R_MS_CORE_STORE :
    case M2R_MS_PGM_STORE  :
      ret = m2r_rmac_reset_mseq_mem(port_id, mem_id, start,
				    num_entries) ;
      break ;

    case M2R_MCAST_COARSE_CAM :
      ret = m2r_rmac_reset_coarse_cam(port_id, mem_id, start,
				      num_entries) ;
      break ;

    case M2R_MCAST_COARSE_CNTRS :
      ret = m2r_rmac_reset_coarse_cntr(port_id, start,
				      num_entries) ;
      break ;

    case M2R_MS_MCHOKE_SRAM :
      ret = m2r_rmac_reset_mchoke_sram(port_id, start, num_entries) ;
      break ;

    case M2R_HOST_PKT_DROP_CAM :
      ret = m2r_rmac_reset_host_drop_cam(port_id, mem_id, start,
					 num_entries) ;
      break ;

    case M2R_HOST_PKT_DROP_CNTRS :
      ret = m2r_rmac_reset_host_drop_cntr(port_id, start,
					 num_entries) ;
      break ;

    case M2R_MAC_REWRITE_SRAM :
      ret = m2r_rmac_rewrite_reset_entries(port_id, start,
					 num_entries) ;
      break ;

    default :
      break ;
  }
  return (ret) ;
}


/********************************************************
 * Driver init CAM configuration                        *
 ********************************************************/
cs_status  m2r_rmac_cam_init_cfg(cs_uint16 port_id,
				 m2r_rmac_cfg_t * pCfg)
{
  /*
   * Configure the last entry in the Lookup(Rx) and Stat(Tx) cam's for
   * Broadcast DA(FF.FF.FF.FF.FF.FF)
   */
  m2r_rx_cam_entry_t	rx_cam ;
  m2r_tx_cam_entry_t  tx_cam ;

  rx_cam.macaddr.da       = 1 ;
  rx_cam.macaddr.sa       = 0 ;
  rx_cam.macaddr.mac.byte5 = rx_cam.macaddr.mac.byte4 =
    rx_cam.macaddr.mac.byte3 = rx_cam.macaddr.mac.byte2 =
    rx_cam.macaddr.mac.byte1 = rx_cam.macaddr.mac.byte0 = 0xff ;
  rx_cam.status.sr        = 0 ;
  rx_cam.status.ne        = 0 ;
  rx_cam.status.sam       = 0 ;
  rx_cam.status.dah       = 1 ;
  rx_cam.status.hop_count = 1 ;
  rx_cam.prsrv_da_cntr = rx_cam.prsrv_sa_cntr = FALSE ;
  if ( m2r_add_rx_cam_entries(port_id, (M2R_NUM_OF_ADDR_CAM_ENTRIES - 1),
			      1, &rx_cam)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  tx_cam.macaddr.da        = 1 ;
  tx_cam.macaddr.sa        = 0 ;
  tx_cam.macaddr.mac.byte5 = tx_cam.macaddr.mac.byte4 =
    tx_cam.macaddr.mac.byte3 = tx_cam.macaddr.mac.byte2 =
    tx_cam.macaddr.mac.byte1 = tx_cam.macaddr.mac.byte0 = 0xff ;
  tx_cam.status.max_active = 1 ;
  tx_cam.status.da_mode_tb = 0 ;
  tx_cam.status.da_mode_host = 1 ; /* host bcast pkts only */
  tx_cam.status.sah        = 0 ;
  tx_cam.status.weight     = 1 ;
  tx_cam.prsrv_sa_cntr = tx_cam.prsrv_da_cntr = FALSE ;
  if ( m2r_add_tx_cam_entries(port_id, (M2R_NUM_OF_ADDR_CAM_ENTRIES - 1),
			      1, &tx_cam)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /*
   * Configure a few lookup cam entries(up to 4, for this node's MAC *
   * address) at init time.                                         *
   */
  if (pCfg->cam.rx_cam_num) {
    if (pCfg->cam.rx_cam_num > 4)  {
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		    ": rx_cam_num(%d) s/b 1...4\n",
		    pCfg->cam.rx_cam_num) ;
      return (CS_ERROR) ;
    }

    if ( m2r_add_rx_cam_entries(port_id, pCfg->cam.rx_cam_start,
				pCfg->cam.rx_cam_num,
				pCfg->cam.rx_cam_tbl)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /* Multi-cast coarse filter cam */
  if ( m2r_rmac_add_mcast_coarse_cam_entries(port_id, 0, 8,
					     pCfg->cam.mcast_cam)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ADD A LOOKUP(RX) CAM ENTRY                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_add_rx_cam_entry(cs_uint16 port_id, cs_uint32 entry_num,
		cs_uint8 da, cs_uint8 sa, cs_uint16 mac5_4,
		cs_uint16 mac3_2, cs_uint16 mac1_0,
		cs_boolean sr, cs_boolean ne, cs_boolean sam,
		cs_boolean dah, cs_uint8 hop_count,
                cs_boolean prsrv_da_cntr, cs_boolean prsrv_sa_cntr)
/* INPUTS     : o Port Id                                       */
/*              o Entry number[0..1023]                         */
/*              o 1 if it is DA address                         */
/*              o 1 if it is SA address                         */
/*              o MAC address bytes(5:4)                        */
/* 		o MAC address bytes(3:2)                        */
/*              o MAC address bytes(1:0)                        */
/*              o SR (SAA/Reject) bit in status SRAM            */
/*              o NE bit in status SRAM                         */
/*              o SAM bit in status SRAM                        */
/*              o DAH bit in status SRAM                        */
/*              o Hop count in status SRAM(for entries < 256)   */
/*              o 1(preserve DA acctg stats) or 0(clear)        */
/* 		o 1(preserve SA acctg stats) or 0(clear)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Add a lookup CAM entry. Since, the corresponding entry in the*/
/* Status SRAM is conceptually an extension of the CAM entry,   */
/* it will also be configured.                                  */
/*                                                              */
/* Note that the driver's initialization, programs the last     */
/* entry in the CAM(ie. 1023), to be Broadcast DA.              */
/*                                                              */
/* This is a private API - mainly provided for lab use.         */
/*                                                              */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_reset_mem().                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rx_cam_entry_t	rx_ce ;

  rx_ce.macaddr.da          = da ;
  rx_ce.macaddr.sa          = sa ;
  rx_ce.macaddr.mac.byte5   = mac5_4 >> 8 ;
  rx_ce.macaddr.mac.byte4   = mac5_4 & 0xff ;
  rx_ce.macaddr.mac.byte3   = mac3_2 >> 8 ;
  rx_ce.macaddr.mac.byte2   = mac3_2 & 0xff ;
  rx_ce.macaddr.mac.byte1   = mac1_0 >> 8 ;
  rx_ce.macaddr.mac.byte0   = mac1_0 & 0xff ;
  rx_ce.status.sr           = sr ;
  rx_ce.status.ne           = ne ;
  rx_ce.status.sam          = sam ;
  rx_ce.status.dah          = dah ;
  rx_ce.status.hop_count    = hop_count ;
  rx_ce.prsrv_sa_cntr       = prsrv_da_cntr ;
  rx_ce.prsrv_da_cntr       = prsrv_sa_cntr ;

  return ( m2r_add_rx_cam_entries(port_id, entry_num, 1, &rx_ce) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  ADD LOOKUP(RX) CAM ENTRIES                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_add_rx_cam_entries(cs_uint16 port_id, cs_uint32 start,
                          cs_uint32 num, m2r_rx_cam_entry_t * tbl)
/* INPUTS     : o Port Id                                       */
/* 	        o Starting entry[0..1023]                       */
/*              o Num of entries[0..1023]                       */
/*              o Table of CAM entries                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Program one or more entries in the Lookup CAM. Since, the    */
/* corresponding entry in the Status SRAM is conceptually an    */
/* extension of the CAM entry, it will also be programmed.      */
/*                                                              */
/* Note that the driver's initialization, programs the last     */
/* entry in the CAM(ie. 1023) to be Broadcast DA.               */
/*                                                              */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_reset_mem().                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;
  cs_uint32		ii ;

  if (tbl == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "tbl is NULL\n") ;
    return (CS_ERROR) ;
  }

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num,
			    M2R_NUM_OF_ADDR_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* process each entry in the table */
  for (ii = start; ii < (start+num); ii++) {
    m2r_rx_cam_entry_t	* p = &tbl[ii - start] ;
    cs_status		ret ;
    cs_uint16		bits = 0 ;

    if ( !(p->macaddr.da) && !(p->macaddr.sa) ) {
      CS_DEBUG(dev_num, M2R_ID_RMAC,
	       "Rx CAM entry(0x%x) invalidated!\n", ii) ;
    }
    else {
      CS_DEBUG(dev_num, M2R_ID_RMAC,
	       "Adding Rx CAM Entry(0x%x)....\n", ii) ;
    }

    if ( (p->macaddr.sa) && (ii >= M2R_ADDR_CAM_SA_NUM_ENTRIES) ) {
      CS_HNDL_ERROR(port_id, EM2R_CAM_ENTRY_INVALID_SA, ":entry(0x%x)\n",
		    ii) ;
      return (CS_ERROR) ;
    }

    ret = m2r_write_to_addr_cam(port_id, M2R_RX_CAM, ii, &p->macaddr) ;
    if (ret == CS_ERROR) return (CS_ERROR) ;

    if (ii < M2R_ADDR_CAM_SA_NUM_ENTRIES) { /* 16-bit entry */
      bits |= ( (p->status.sr)  ? 1 : 0 ) << 11 ;
      bits |= ( (p->status.ne)  ? 1 : 0 ) << 10 ;
      bits |= ( (p->status.sam) ? 1 : 0 ) <<  9 ;
      bits |= ( (p->status.dah) ? 1 : 0 ) <<  8 ;
      bits |= p->status.hop_count ;
      ret = m2r_access_status_sram_16bit_entry(port_id,
			 M2R_RX_STATUS_SRAM, M2R_WRITE, ii, &bits) ;
    }
    else { /* 4-bit entry - for DA only */
      bits |= ( (p->status.dah) ? 1 : 0 ) << 0 ;
      ret = m2r_access_status_sram_4bit_entry(port_id,
			 M2R_RX_STATUS_SRAM, M2R_WRITE, ii, &bits) ;
    }
    if (ret == CS_ERROR) return (CS_ERROR) ;

    if ( (p->macaddr.da) && !(p->prsrv_da_cntr) ) {
      if ( m2r_rmac_clear_acctg_stats(port_id,
				      M2R_RX_ACCTG_DA_SRAM, ii, 1)
	   == CS_ERROR ) {
	return (CS_ERROR) ;
      }
    }

    if ( (p->macaddr.sa) && !(p->prsrv_sa_cntr) ) {
      if ( m2r_rmac_clear_acctg_stats(port_id,
				      M2R_RX_ACCTG_SA_SRAM, ii, 1)
	   == CS_ERROR ) {
	return (CS_ERROR) ;
      }
    }

  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ADD A STAT(TX) CAM ENTRY                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_add_tx_cam_entry(cs_uint16 port_id, cs_uint32 entry_num,
		cs_uint8 da, cs_uint8 sa, cs_uint16 mac5_4,
		cs_uint16 mac3_2, cs_uint16 mac1_0,
		cs_uint8 max_active, cs_boolean da_mode_tb,
		cs_boolean da_mode_host, cs_boolean sah,
		cs_uint8 weight,
                cs_boolean prsrv_da_cntr, cs_boolean prsrv_sa_cntr)
/* INPUTS     : o Port Id                                       */
/*              o Entry number[0..1023]                         */
/*              o 1 if it is DA address                         */
/*              o 1 if it is SA address                         */
/*              o MAC address bytes(5:4)                        */
/* 		o MAC address bytes(3:2)                        */
/*              o MAC address bytes(1:0)                        */
/*              o MaxActive value in Status SRAM                */
/*              o DATB   - DAMode[1] bit in Status SRAM         */
/*              o DAHost - DAMode[0] bit in Status SRAM         */
/*              o SAH bit in status SRAM                        */
/*              o Weight value in Status SRAM                   */
/*              o 1(preserve DA acctg stats) or 0(clear)        */
/* 		o 1(preserve SA acctg stats) or 0(clear)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Add a stat CAM entry. Since, the corresponding entry in the  */
/* Status SRAM is conceptually an extension of the CAM entry,   */
/* it will also be configured.                                  */
/*                                                              */
/* Note that the driver's initialization, programs the last     */
/* entry in the CAM(ie. 1023 ) to be Broadcast DA.              */
/*                                                              */
/* This is a private API - mainly provided for lab use.         */
/*                                                              */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_reset_mem().                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_tx_cam_entry_t	tx_ce ;

  tx_ce.macaddr.da             = da ;
  tx_ce.macaddr.sa             = sa ;
  tx_ce.macaddr.mac.byte5      = mac5_4 >> 8 ;
  tx_ce.macaddr.mac.byte4      = mac5_4 & 0xff ;
  tx_ce.macaddr.mac.byte3      = mac3_2 >> 8 ;
  tx_ce.macaddr.mac.byte2      = mac3_2 & 0xff ;
  tx_ce.macaddr.mac.byte1      = mac1_0 >> 8 ;
  tx_ce.macaddr.mac.byte0      = mac1_0 & 0xff ;
  tx_ce.status.max_active      = max_active ;
  tx_ce.status.da_mode_host    = da_mode_host ;
  tx_ce.status.da_mode_tb      = da_mode_tb ;
  tx_ce.status.sah             = sah ;
  tx_ce.status.weight          = weight ;
  tx_ce.prsrv_sa_cntr          = prsrv_da_cntr ;
  tx_ce.prsrv_da_cntr          = prsrv_sa_cntr ;

  return ( m2r_add_tx_cam_entries(port_id, entry_num, 1, &tx_ce) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  ADD STAT(Tx) CAM ENTRIES                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_add_tx_cam_entries(cs_uint16 port_id, cs_uint32 start,
                          cs_uint32 num, m2r_tx_cam_entry_t * tbl)
/* INPUTS     : o Port Id                                       */
/* 	        o Starting entry[0..1023]                       */
/*              o Num of entries[0..1023]                       */
/*              o Table of CAM entries                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Program one or more entries in the Stat CAM. Since, the      */
/* corresponding entry in the Status SRAM is conceptually an    */
/* extension of the CAM entry, it will also be programmed.      */
/*                                                              */
/* Note that the driver's initialization, programs the last     */
/* entry in the CAM(ie. 1023) to be Broadcast DA.               */
/*                                                              */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_reset_mem().                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;
  cs_uint32		ii ;

  if (tbl == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "tbl is NULL\n") ;
    return (CS_ERROR) ;
  }

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num,
			    M2R_NUM_OF_ADDR_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* process each entry in the table */
  for (ii = start; ii < (start+num); ii++) {
    m2r_tx_cam_entry_t	* p = &tbl[ii - start] ;
    cs_status		ret ;
    cs_uint16		bits = 0 ;

    if ( !(p->macaddr.da) && !(p->macaddr.sa) ) {
      CS_DEBUG(dev_num, M2R_ID_RMAC,
	       "Tx CAM entry(0x%x) invalidated!\n", ii) ;
    }
    else {
      CS_DEBUG(dev_num, M2R_ID_RMAC,
	       "Adding Tx CAM entry(0x%x)....\n", ii) ;
    }

    if ( (p->macaddr.sa) && (ii >= M2R_ADDR_CAM_SA_NUM_ENTRIES) ) {
      CS_HNDL_ERROR(port_id, EM2R_CAM_ENTRY_INVALID_SA,
		    ":entry(0x%x)\n",
		    ii) ;
      return (CS_ERROR) ;
    }

    ret = m2r_write_to_addr_cam(port_id, M2R_TX_CAM, ii, &p->macaddr) ;
    if (ret == CS_ERROR) return (CS_ERROR) ;

    if (ii < M2R_ADDR_CAM_SA_NUM_ENTRIES) { /* 16-bit entry */
      bits |= (p->status.max_active & 0xf) << 12 ;
      bits |= ( (p->status.da_mode_tb)   ? 1 : 0 ) << 10 ;
      bits |= ( (p->status.da_mode_host) ? 1 : 0 ) <<  9 ;
      bits |= ( (p->status.sah)          ? 1 : 0 ) <<  8 ;
      bits |= p->status.weight ;
      ret = m2r_access_status_sram_16bit_entry(port_id,
		     M2R_TX_STATUS_SRAM, M2R_WRITE, ii, &bits) ;
    }
    else { /* 4-bit entry - for DA only */
      bits |= ( (p->status.da_mode_tb)   ? 1 : 0 ) << 2 ;
      bits |= ( (p->status.da_mode_host) ? 1 : 0 ) << 1 ;
      ret = m2r_access_status_sram_4bit_entry(port_id,
		     M2R_TX_STATUS_SRAM, M2R_WRITE, ii, &bits) ;
    }

    if (ret == CS_ERROR) return (CS_ERROR) ;

    if ( (p->macaddr.da) && !(p->prsrv_da_cntr) ) {
      if ( m2r_rmac_clear_acctg_stats(port_id,
				      M2R_TX_ACCTG_DA_SRAM, ii, 1)
	   == CS_ERROR ) {
	return (CS_ERROR) ;
      }
    }

    if ( (p->macaddr.sa) && !(p->prsrv_sa_cntr) ) {
      if ( m2r_rmac_clear_acctg_stats(port_id,
				      M2R_TX_ACCTG_SA_SRAM, ii, 1)
	   == CS_ERROR ) {
	return (CS_ERROR) ;
      }
    }

  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP TX OR RX CAM ENTRIES                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_dump_cam_entries(cs_uint16 port_id,
				     m2r_rmac_mem_id_t cam_id,
				     cs_uint16 start, cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX_CAM or M2R_TX_CAM                      */
/*              o First entry                                   */
/*              o Num of consecutive entries                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the specified entries and prints them.             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;
  m2r_rmac_gram_t     	gram ;
  cs_uint32		cam, status_sram, i ;
  cs_uint16		status_bits ;
  m2r_port_mac_mode_t 	mac_mode ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if (mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
  }

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num,
			    M2R_NUM_OF_ADDR_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }


  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (cam_id == M2R_RX_CAM) {
    cam         = SEL_LOOKUP_CAM ;
    status_sram = SEL_LOOKUP_STATUS ;
  }
  else {
    cam         = SEL_STAT_CAM ;
    status_sram = SEL_STAT_STATUS ;
  }

  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  for (i = start; i < (start + num); i++) {
    /* get cam entry */
    gram.GRAM_CR.bf.SEL = cam ;
    gram.GRAM_CR.bf.GRAM_A = (i % M2R_NUM_OF_ADDR_CAM_ENTRIES) ;
    gram.GRAM_CR.bf.GRWN = GRWN_READ ;
    m2r_rmac_read_gram_regs(port_id, &gram) ;

    /* get status ram entry corresponding to this cam entry */
    if (i < M2R_ADDR_CAM_SA_NUM_ENTRIES) {
      if (m2r_access_status_sram_16bit_entry(port_id, status_sram,
					     M2R_READ, i, &status_bits)
	  == CS_ERROR) {
	return (CS_ERROR) ;
      }
    }

    else { /* entries are shared by SA/DA */
      if (m2r_access_status_sram_4bit_entry(port_id, status_sram,
					    M2R_READ, i, &status_bits)
	  == CS_ERROR) {
	return (CS_ERROR) ;
      }
    }

    m2r_print_addr_cam_entry(port_id, cam_id, i, &gram, status_bits) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  PROBE ADDRESS(Tx or Rx) CAM ENTRY            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint32  m2r_rmac_probe_cam_entry(cs_uint16 port_id,
				    m2r_rmac_mem_id_t cam_id,
				    m2r_cam_macaddr_t * pmac_mask,
				    m2r_cam_macaddr_t * pmac_addr)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX_CAM or M2R_TX_CAM                      */
/*              o Mask for DA/SA bits and MAC address           */
/*              o DA/SA bits and MAC address                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Probe for a specific MAC address in the Lookup(Rx) or        */
/* Stat(Tx) CAM.                                                */
/*                                                              */
/* The [pmac_mask] parameter specifies the mask to be applied   */
/* for doing the CAM search (1 - mask off, 0 - include). This   */
/* mask is applied for DA/SA as well as the address bits.       */
/*                                                              */
/* The [pmac_addr] parameter specifies DA/SA field and the      */
/* address bits.                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;
  cs_reg		dasa = 0 ;
  DR_STAT_CAM_CR_t      cam_cr ;
  DR_STAT_CAM_SR_t      cam_sr ;
  cs_uint32             idx = -1 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, (-1)) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, (-1)) ;

  if (cam_id != M2R_RX_CAM && cam_id != M2R_TX_CAM) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": cam_id(%d) should be %d or %d\n",
		  M2R_RX_CAM, M2R_TX_CAM) ;
    return (-1) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  /* Enable probe */
  cam_cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CAM_CR.wrd) ;
  cam_cr.bf.PROBE = 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CAM_CR.wrd, cam_cr.wrd) ;

  /* set up mask for MAC address */
  M2R_REG_WRITE( &pDev->slice[sl_num].RingMAC.MASK0.wrd,
		(pmac_mask->mac.byte1 << 8 | pmac_mask->mac.byte0) ) ;
  M2R_REG_WRITE( &pDev->slice[sl_num].RingMAC.MASK1.wrd,
		(pmac_mask->mac.byte3 << 8 | pmac_mask->mac.byte2) ) ;
  M2R_REG_WRITE( &pDev->slice[sl_num].RingMAC.MASK2.wrd,
		(pmac_mask->mac.byte5 << 8 | pmac_mask->mac.byte4) ) ;

  dasa |= (pmac_mask->da) ? 2 : 0 ;
  dasa |= (pmac_mask->sa) ? 1 : 0 ;
  M2R_REG_WRITE( &pDev->slice[sl_num].RingMAC.MASK3.wrd, dasa << 6) ;

  /* set up search MAC address */
  if ( m2r_write_to_addr_cam(port_id, cam_id, 0, pmac_addr) ==
       CS_ERROR ) {
    return (-1) ;
  }

  /* check for a "hit" */
  cam_sr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CAM_SR.wrd) ;
  if (cam_sr.bf.HIT) idx = cam_sr.bf.HIT_A ;

  /* Disable probe before returning */
  cam_cr.bf.PROBE = 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CAM_CR.wrd, cam_cr.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (idx) ;
}

/****************************************************************/
/* $rtn_hdr_start  PROBE ADDRESS(Tx or Rx) CAM ENTRY            */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_uint32  m2r_rmac_debug_probe_cam_entry(cs_uint16 port_id,
	       m2r_rmac_mem_id_t cam_id, cs_uint16 mask_dasa,
               cs_uint16 mask5_4, cs_uint16 mask3_2, cs_uint16 mask1_0,
	       cs_uint16 mac_dasa,
	       cs_uint16 mac5_4, cs_uint16 mac3_2, cs_uint16 mac1_0)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX_CAM or M2R_TX_CAM                      */
/*              * Mask DA,SA bits                               */
/*              * Mask MAC address bytes 5,4                    */
/*              * Mask MAC address bytes 3,2                    */
/*              * Mask MAC address bytes 1,0                    */
/*              * DA, SA bits                                   */
/*              * MAC address bytes 5,4                         */
/*              * MAC address bytes 3,2                         */
/*              * MAC address bytes 1,0                         */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Debug routine(for lab use) to probe for a specific address   */
/* in the Lookup(Rx) or Stat(Tx) CAM.                           */
/* The mask is applied in the CAM search(1 - mask off,          */
/* 0 - to include).                                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_cam_macaddr_t 	mac_mask ;
  m2r_cam_macaddr_t 	mac_addr ;

  /* Mask */
  mac_mask.da = (mask_dasa & 2) ? TRUE : FALSE ;
  mac_mask.sa = (mask_dasa & 1) ? TRUE : FALSE ;
  mac_mask.mac.byte5 = mask5_4 >> 8 ;
  mac_mask.mac.byte4 = mask5_4 & 0xff ;
  mac_mask.mac.byte3 = mask3_2 >> 8 ;
  mac_mask.mac.byte2 = mask3_2 & 0xff ;
  mac_mask.mac.byte1 = mask1_0 >> 8 ;
  mac_mask.mac.byte0 = mask1_0 & 0xff ;

  /* address */
  mac_addr.da = (mac_dasa & 2) ? TRUE : FALSE ;
  mac_addr.sa = (mac_dasa & 1) ? TRUE : FALSE ;
  mac_addr.mac.byte5 = mac5_4 >> 8 ;
  mac_addr.mac.byte4 = mac5_4 & 0xff ;
  mac_addr.mac.byte3 = mac3_2 >> 8 ;
  mac_addr.mac.byte2 = mac3_2 & 0xff ;
  mac_addr.mac.byte1 = mac1_0 >> 8 ;
  mac_addr.mac.byte0 = mac1_0 & 0xff ;

  return ( m2r_rmac_probe_cam_entry(port_id, cam_id,
				    &mac_mask, &mac_addr) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  ADD MULTICAST COARSE FILTER CAM ENTRIES      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint32  m2r_rmac_add_mcast_coarse_cam_entries(cs_uint16 port_id,
			        cs_uint8 start, cs_uint8 num,
			        m2r_rmac_mcast_coarse_cam_t * tbl)
/* INPUTS     : o Port Id                                       */
/*              o Starting entry[0..7]                          */
/*              o Num of entries[0..7]                          */
/*              o Table of CAM entries                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Program one or more entries in the Multi-cast Coarse Filter  */
/* CAM. This is a Ternary CAM of size 8 entries, which is used  */
/* to filter Multi-cast packets. The Multi-cast groups that the */
/* Host belongs to is programmed so that those Multi-cast pkts  */
/* are copied to the Host(besides being forwarded to TB).       */
/*                                                              */
/* The entries are priority-indexed and if there multiple       */
/* hits on this CAM, then the lower numbered entry will be used.*/
/*                                                              */
/* Each CAM entry has -                                         */
/*   o 48-bit Key MAC address(which is the base MAC DA),        */
/*   o 48-bit Mask(to mask out any bits in the MAC DA that      */
/*     should be excluded in the search),                       */
/*   o Accept/Reject(ARN) (which decides whether the packet     */
/*     should be accepted or rejected, if it is a "hit").       */
/*   o Valid bit to indicate the validity of the entry itself.  */
/*                                                              */
/* Received Multi-cast packets will be first looked up in the   */
/* Lookup CAM and if there is a DA-match then the decision is   */
/* made without doing a lookup in the Coarse Filter CAM.        */
/*                                                              */
/* The [valid] member of cam entry structure needs to be TRUE to*/
/* set up a new valid CAM entry!                                */
/* A cam entry can be invalidated either by having the [valid]  */
/* parameter set to FALSE or by calling m2r_rmac_reset_mem().   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	i ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num,
			    M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  for (i = start; i < (start+num); i++) {
    m2r_rmac_gram_t		gram ;
    m2r_rmac_mcast_coarse_cam_t * p = &tbl[i-start] ;

    CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

    /* write key */
    gram.GRAM_D0.wrd = (p->key.byte1 << 8) | p->key.byte0 ;
    gram.GRAM_D1.wrd = (p->key.byte3 << 8) | p->key.byte2 ;
    gram.GRAM_D2.wrd = (p->key.byte5 << 8) | p->key.byte4 ;
    gram.GRAM_CR.bf.SEL   = SEL_LKP_COARSE_CAM ;
    gram.GRAM_CR.bf.GRWN  = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = i ;
    m2r_rmac_write_gram_regs(port_id, &gram) ;

    /* write mask, accept/reject(ARN) bit, and valid bit */
    gram.GRAM_D0.wrd = (p->mask.byte1 << 8) | p->mask.byte0 ;
    gram.GRAM_D1.wrd = (p->mask.byte3 << 8) | p->mask.byte2 ;
    gram.GRAM_D2.wrd = (p->mask.byte5 << 8) | p->mask.byte4 ;
    gram.GRAM_D3.wrd = (p->accept) ? 1 : 0 ; /* 1: accept, 0: reject */
    gram.GRAM_D3.wrd |= (p->valid) ? (1 << 1) : 0 ;
    gram.GRAM_CR.bf.SEL   = SEL_LKP_COARSE_CAM ;
    gram.GRAM_CR.bf.GRWN  = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = i | (1 << 10) ; /* A[10] s/b 1 */
    m2r_rmac_write_gram_regs(port_id, &gram) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ADD A MULTICAST COARSE FILTER CAM ENTRY      */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint32  m2r_rmac_add_mcast_coarse_cam_entry(cs_uint16 port_id,
	       cs_uint8 entry_num,
               cs_uint16 key5_4, cs_uint16 key3_2, cs_uint16 key1_0,
               cs_uint16 mask5_4, cs_uint16 mask3_2, cs_uint16 mask1_0,
               cs_boolean accept, cs_boolean valid)
/* INPUTS     : o Port Id                                       */
/*              o CAM entry num (0...7)                         */
/*              o Key  - bytes [5:4]                            */
/*              o Key  - bytes [3:2]                            */
/*              o Key  - bytes [1:0]                            */
/*              o Mask - bytes [5:4]                            */
/*              o Mask - bytes [3:2]                            */
/*              o Mask - bytes [1:0]                            */
/*              o TRUE(if accept) or FALSE(if reject)           */
/*              o TRUE(valid) or FALSE(invalid)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Program one or more entries in the Multi-cast Coarse Filter  */
/* CAM. This is a Ternary CAM of size 8 entries, which is used  */
/* to filter Multi-cast packets. The Multi-cast groups that the */
/* Host belongs to is programmed so that those Multi-cast pkts  */
/* are copied to the Host(besides being forwarded to TB).       */
/*                                                              */
/* The entries are priority-indexed and if there multiple       */
/* hits on this CAM, then the lower numbered entry will be used.*/
/*                                                              */
/* Each CAM entry has -                                         */
/*   o 48-bit Key MAC address(which is the base MAC DA),        */
/*   o 48-bit Mask(to mask out any bits in the MAC DA that      */
/*     should be excluded in the search),                       */
/*   o Accept/Reject(ARN) (which decides whether the packet     */
/*     should be accepted or rejected, if it is a "hit").       */
/*   o Valid bit to indicate the validity of the entry itself.  */
/*                                                              */
/* Received Multi-cast packets will be first looked up in the   */
/* Lookup CAM and if there is a DA-match then the decision is   */
/* made without doing a lookup in the Coarse Filter CAM.        */
/*                                                              */
/* The [valid] parameter needs to be TRUE to set up a new valid */
/* CAM entry!                                                   */
/* A cam entry can be invalidated either by having the [valid]  */
/* parameter set to FALSE or by calling m2r_rmac_reset_mem().   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_mcast_coarse_cam_t   mcast_cam ;

  mcast_cam.key.byte0  = key1_0 & 0xff ;
  mcast_cam.key.byte1  = key1_0 >> 8 ;
  mcast_cam.key.byte2  = key3_2 & 0xff ;
  mcast_cam.key.byte3  = key3_2 >> 8 ;
  mcast_cam.key.byte4  = key5_4 & 0xff ;
  mcast_cam.key.byte5  = key5_4 >> 8 ;

  mcast_cam.mask.byte0 = mask1_0 & 0xff ;
  mcast_cam.mask.byte1 = mask1_0 >> 8 ;
  mcast_cam.mask.byte2 = mask3_2 & 0xff ;
  mcast_cam.mask.byte3 = mask3_2 >> 8 ;
  mcast_cam.mask.byte4 = mask5_4 & 0xff ;
  mcast_cam.mask.byte5 = mask5_4 >> 8 ;

  mcast_cam.accept = accept ;
  mcast_cam.valid  = valid ;

  return ( m2r_rmac_add_mcast_coarse_cam_entries(port_id, entry_num, 1,
						 &mcast_cam) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET MCAST COARSE FILTER CAM ENTRIES          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_get_mcast_coarse_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num,
				    m2r_rmac_mcast_coarse_cam_t * p)
/* INPUTS     : o Port Id                                       */
/*              o CAM entry number (0...7)                      */
/*              o Pointer to CAM entry                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the specified Multicast Coarse CAM entry.          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_gram_t	gram ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if (entry_num > 7) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "entry_num(%d) should be 0...7\n", entry_num) ;
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  /* get key */
  gram.GRAM_CR.bf.SEL   = SEL_LKP_COARSE_CAM ;
  gram.GRAM_CR.bf.GRWN  = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = entry_num ;
  m2r_rmac_read_gram_regs(port_id, &gram) ;

  p->key.byte0 = gram.GRAM_D0.wrd & 0xff ;
  p->key.byte1 = gram.GRAM_D0.wrd >> 8 ;
  p->key.byte2 = gram.GRAM_D1.wrd & 0xff ;
  p->key.byte3 = gram.GRAM_D1.wrd >> 8 ;
  p->key.byte4 = gram.GRAM_D2.wrd & 0xff ;
  p->key.byte5 = gram.GRAM_D2.wrd >> 8 ;

  /* get mask */
  gram.GRAM_CR.bf.SEL   = SEL_LKP_COARSE_CAM ;
  gram.GRAM_CR.bf.GRWN  = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = entry_num | (1 << 10) ; /* A[10] s/b 1 */
  m2r_rmac_read_gram_regs(port_id, &gram) ;
  p->mask.byte0 = gram.GRAM_D0.wrd & 0xff ;
  p->mask.byte1 = gram.GRAM_D0.wrd >> 8 ;
  p->mask.byte2 = gram.GRAM_D1.wrd & 0xff ;
  p->mask.byte3 = gram.GRAM_D1.wrd >> 8 ;
  p->mask.byte4 = gram.GRAM_D2.wrd & 0xff ;
  p->mask.byte5 = gram.GRAM_D2.wrd >> 8 ;

  /* accept and valid bits */
  p->accept = (gram.GRAM_D3.wrd & 1) ? TRUE : FALSE ;
  p->valid  = (gram.GRAM_D3.wrd & 2) ? TRUE : FALSE ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP MCAST COARSE FILTER CAM ENTRIES         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_dump_mcast_coarse_cam_entries(cs_uint16 port_id,
						 cs_uint16 start,
						 cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/*              o First entry                                   */
/*              o Num of consecutive entries                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the specified entries and prints them.             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	ii ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num,
			    M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  for (ii = start; ii < (start+num); ii++) {
    m2r_rmac_mcast_coarse_cam_t mccam ;

    if ( m2r_rmac_get_mcast_coarse_cam_entry(port_id, ii, &mccam)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    /* Print key */
    CS_PRINT("0x%1x: Key: %02x.%02x.%02x.%02x.%02x.%02x, ",
	     ii,
	     mccam.key.byte5, mccam.key.byte4,
	     mccam.key.byte3, mccam.key.byte2,
	     mccam.key.byte1, mccam.key.byte0) ;

    /* Print Mask */
    CS_PRINT("Mask: %02x.%02x.%02x.%02x.%02x.%02x, ",
	     mccam.mask.byte5, mccam.mask.byte4,
	     mccam.mask.byte3, mccam.mask.byte2,
	     mccam.mask.byte1, mccam.mask.byte0) ;

    /* Print ARN and valid flags */
    CS_PRINT("%7s, %7s\n",
	     (mccam.accept) ? "Accept" : "Reject",
	     (mccam.valid)  ? "Valid"  : "INVALID") ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start     ADD HOST PKT DROP COARSE CAM ENTRIES      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint32  m2r_rmac_add_host_drop_cam_entries(cs_uint16 port_id,
			        cs_uint8 start, cs_uint8 num,
			        m2r_rmac_host_pkt_drop_cam_t * tbl)
/* INPUTS     : o Port Id                                       */
/*              o Starting entry[0..15]                         */
/*              o Num of entries[0..15]                         */
/*              o Table of CAM entries                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Program one or more entries in the Host Packet Drop Coarse   */
/* CAM. This is a Ternary CAM with size of 16 entries, which    */
/* is used to determine which dropped packets destined to the   */
/* HOST should be counted.  This allows one to determine which  */
/* station on the ring is trying to send too much traffic to the*/
/* local station.                                               */
/*                                                              */
/* The entries are priority-indexed and if there multiple       */
/* hits on this CAM, then the lower numbered entry will be used.*/
/*                                                              */
/* Each CAM entry has -                                         */
/*   o 48-bit Key MAC address(which is the base MAC SA),        */
/*   o 48-bit Mask(to mask out any bits in the MAC SA that      */
/*     should be excluded in the search),                       */
/*   o Valid bit to indicate the validity of the entry itself.  */
/*                                                              */
/* The [valid] member of cam entry structure needs to be TRUE to*/
/* set up a new valid CAM entry!                                */
/* A cam entry can be invalidated either by having the [valid]  */
/* parameter set to FALSE or by calling m2r_rmac_reset_mem().   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	i ;
  cs_uint16     vbits = 0, bits = 0;
  m2r_rmac_gram_t	gram ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num,
			    M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  for (i = start; i < (start+num); i++) {
    m2r_rmac_host_pkt_drop_cam_t * p = &tbl[i-start] ;

    CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

    /* write key & mask */
    gram.GRAM_D0.wrd = (p->key.byte1 << 8) | p->key.byte0 ;
    gram.GRAM_D1.wrd = (p->key.byte3 << 8) | p->key.byte2 ;
    gram.GRAM_D2.wrd = (p->key.byte5 << 8) | p->key.byte4 ;
    gram.GRAM_D3.wrd = (p->mask.byte1 << 8) | p->mask.byte0 ;
    gram.GRAM_D4.wrd = (p->mask.byte3 << 8) | p->mask.byte2 ;
    gram.GRAM_D5.wrd = (p->mask.byte5 << 8) | p->mask.byte4 ;
    gram.GRAM_CR.bf.SEL   = SEL_HOST_PKT_DROP_CAM ;
    gram.GRAM_CR.bf.GRWN  = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = i ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }

    vbits |= (p->valid) ? (1 << i) : 0 ;
    bits |= (1 << i) ;
  }

  /* write valid bit */
  gram.GRAM_CR.bf.SEL   = SEL_HOST_PKT_DROP_CAM ;
  gram.GRAM_CR.bf.GRAM_A = 0x10 ;
  gram.GRAM_CR.bf.GRWN = GRWN_READ ;

  m2r_rmac_read_host_drop_gram_regs(port_id, &gram) ;

  gram.GRAM_D0.wrd &= ((~bits) & 0xffff ) ;
  gram.GRAM_D0.wrd |= vbits ;
  gram.GRAM_CR.bf.GRWN = GRWN_WRITE ;

  if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start     ADD A HOST PKT DROP COARSE CAM ENTRY      */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint32  m2r_rmac_add_host_drop_cam_entry(cs_uint16 port_id,
	       cs_uint8 entry_num,
               cs_uint16 key5_4, cs_uint16 key3_2, cs_uint16 key1_0,
               cs_uint16 mask5_4, cs_uint16 mask3_2, cs_uint16 mask1_0,
               cs_boolean valid)
/* INPUTS     : o Port Id                                       */
/*              o CAM entry num (0...15)                        */
/*              o Key  - bytes [5:4]                            */
/*              o Key  - bytes [3:2]                            */
/*              o Key  - bytes [1:0]                            */
/*              o Mask - bytes [5:4]                            */
/*              o Mask - bytes [3:2]                            */
/*              o Mask - bytes [1:0]                            */
/*              o TRUE(valid) or FALSE(invalid)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Program one or more entries in the Host Packet Drop Coarse   */
/* CAM. This is a Ternary CAM with size of 16 entries, which    */
/* is used to determine which dropped packets destined to the   */
/* HOST should be counted.  This allows one to determine which  */
/* station on the ring is trying to send too much traffic to the*/
/* local station.                                               */
/*                                                              */
/* The entries are priority-indexed and if there multiple       */
/* hits on this CAM, then the lower numbered entry will be used.*/
/*                                                              */
/* Each CAM entry has -                                         */
/*   o 48-bit Key MAC address(which is the base MAC SA),        */
/*   o 48-bit Mask(to mask out any bits in the MAC SA that      */
/*     should be excluded in the search),                       */
/*   o Valid bit to indicate the validity of the entry itself.  */
/*                                                              */
/* The [valid] parameter needs to be TRUE to set up a new valid */
/* CAM entry!                                                   */
/* A cam entry can be invalidated either by having the [valid]  */
/* parameter set to FALSE or by calling m2r_rmac_reset_mem().   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_host_pkt_drop_cam_t   host_drop_cam ;

  host_drop_cam.key.byte0  = key1_0 & 0xff ;
  host_drop_cam.key.byte1  = key1_0 >> 8 ;
  host_drop_cam.key.byte2  = key3_2 & 0xff ;
  host_drop_cam.key.byte3  = key3_2 >> 8 ;
  host_drop_cam.key.byte4  = key5_4 & 0xff ;
  host_drop_cam.key.byte5  = key5_4 >> 8 ;

  host_drop_cam.mask.byte0 = mask1_0 & 0xff ;
  host_drop_cam.mask.byte1 = mask1_0 >> 8 ;
  host_drop_cam.mask.byte2 = mask3_2 & 0xff ;
  host_drop_cam.mask.byte3 = mask3_2 >> 8 ;
  host_drop_cam.mask.byte4 = mask5_4 & 0xff ;
  host_drop_cam.mask.byte5 = mask5_4 >> 8 ;

  host_drop_cam.valid  = valid ;

  return ( m2r_rmac_add_host_drop_cam_entries(port_id, entry_num, 1,
						 &host_drop_cam) ) ;
}


/****************************************************************/
/* $rtn_hdr_start    GET HOST PKT DROP COARSE CAM ENTRIES       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_get_host_drop_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num,
				    m2r_rmac_host_pkt_drop_cam_t * p)
/* INPUTS     : o Port Id                                       */
/*              o CAM entry number (0...15)                     */
/*              o Pointer to CAM entry                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the specified Host Pkt Drop Coarse CAM entry.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_gram_t	gram ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if (entry_num > 15) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "entry_num(%d) should be 0...15\n", entry_num) ;
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  /* get key & mask */
  gram.GRAM_CR.bf.SEL   = SEL_HOST_PKT_DROP_CAM ;
  gram.GRAM_CR.bf.GRWN  = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = entry_num ;

  m2r_rmac_read_host_drop_gram_regs(port_id, &gram) ;

  p->key.byte0 = gram.GRAM_D0.wrd & 0xff ;
  p->key.byte1 = gram.GRAM_D0.wrd >> 8 ;
  p->key.byte2 = gram.GRAM_D1.wrd & 0xff ;
  p->key.byte3 = gram.GRAM_D1.wrd >> 8 ;
  p->key.byte4 = gram.GRAM_D2.wrd & 0xff ;
  p->key.byte5 = gram.GRAM_D2.wrd >> 8 ;
  p->mask.byte0 = gram.GRAM_D3.wrd & 0xff ;
  p->mask.byte1 = gram.GRAM_D3.wrd >> 8 ;
  p->mask.byte2 = gram.GRAM_D4.wrd & 0xff ;
  p->mask.byte3 = gram.GRAM_D4.wrd >> 8 ;
  p->mask.byte4 = gram.GRAM_D5.wrd & 0xff ;
  p->mask.byte5 = gram.GRAM_D5.wrd >> 8 ;

  /* valid bits */
  gram.GRAM_CR.bf.SEL   = SEL_HOST_PKT_DROP_CAM ;
  gram.GRAM_CR.bf.GRWN  = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = 0x10 ;

  m2r_rmac_read_host_drop_gram_regs(port_id, &gram) ;

  p->valid  = (gram.GRAM_D0.wrd & (1 << entry_num)) ? TRUE : FALSE ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP HOST PKT DROP COARSE CAM ENTRIES        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_dump_host_drop_cam_entries(cs_uint16 port_id,
						 cs_uint16 start,
						 cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/*              o First entry                                   */
/*              o Num of consecutive entries                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the specified entries and prints them.             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	ii ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num,
			    M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  for (ii = start; ii < (start+num); ii++) {
    m2r_rmac_host_pkt_drop_cam_t hdcam ;

    if ( m2r_rmac_get_host_drop_cam_entry(port_id, ii, &hdcam)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    /* Print key */
    CS_PRINT("0x%1x: Key: %02x.%02x.%02x.%02x.%02x.%02x, ",
	     ii,
	     hdcam.key.byte5, hdcam.key.byte4,
	     hdcam.key.byte3, hdcam.key.byte2,
	     hdcam.key.byte1, hdcam.key.byte0) ;

    /* Print Mask */
    CS_PRINT("Mask: %02x.%02x.%02x.%02x.%02x.%02x, ",
	     hdcam.mask.byte5, hdcam.mask.byte4,
	     hdcam.mask.byte3, hdcam.mask.byte2,
	     hdcam.mask.byte1, hdcam.mask.byte0) ;

    /* Print valid flags */
    CS_PRINT("%7s\n",
	     (hdcam.valid)  ? "Valid"  : "INVALID") ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET ACCOUNTING SRAM STATS                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_get_acctg_stats(cs_uint16 port_id,
				m2r_rmac_mem_id_t sram_id,
				cs_uint16 start, cs_uint16 num,
				m2r_rmac_cntr_t * p_user_stats)
/* INPUTS     : o Port Id                                       */
/* 	        o SRAM Id                                       */
/*              o First entry                                   */
/*              o Num of consecutive entries                    */
/*              o Caller provided buffer for stats              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the SA or DA accounting counters associated with   */
/* a particular address CAM.                                    */
/*                                                              */
/* The [sram_id] parameter specifies which accounting SRAM and  */
/* is one of the following:                                     */
/* M2R_RX_ACCTG_SA_SRAM,            M2R_RX_ACCTG_DA_SRAM,       */
/* M2R_TX_ACCTG_SA_SRAM,       or   M2R_TX_ACCTG_DA_SRAM,       */
/*                                                              */
/* The [start] and [num] parameters specify the starting entry  */
/* and the number of consecutive entries to be gotten.          */
/*                                                              */
/* The [p_user_stats] is a pointer to caller provided structure */
/* for the driver to return the statistics.                     */
/*                                                              */
/* NOTE - The SA addresses can be programmed only in the first  */
/* 256 locations and so the SRAM's corresponding to SA entries  */
/* can only be 0 to 255. The DA entries can be 0 to 1023.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_cntr_t 	* p_drvr_stats ;
  cs_uint16		tbl_sz, ii, idx ;
  m2r_port_cb_t   	* ppcb ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if (p_user_stats == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ":p_user_stats is NULL\n") ;
    return (CS_ERROR) ;
  }

  tbl_sz = M2R_RMAC_GET_ACCTG_SRAM_SIZE(sram_id) ;
  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num, tbl_sz,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
  if (ppcb->acctg_stats_malloced) {
    /* Retrieve the hw counters and update the driver stats */
    if ( m2r_rmac_update_acctg_drvr_stats(port_id, sram_id, start, num)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    /* Copy the driver stats to the user buffer */
    p_drvr_stats = m2r_rmac_get_acctg_sram_stats_tbl_ptr(port_id,
						       sram_id) ;
    for (ii = start; ii < (start+num); ii++) {
      idx = (ii - start) ;
      p_user_stats[idx] = p_drvr_stats[ii] ;
    }
  }

  else { /* no memory allocated for acctg stats */
    for (ii = start; ii < (start+num); ii++) {
      idx = (ii - start) ;
      if ( m2r_rmac_read_acctg_sram_cntr(port_id, sram_id, ii,
					 &p_user_stats[idx])
	   == CS_ERROR ) {
	return(CS_ERROR) ;
      }
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  PRINT ACCOUNTING SRAM STATS                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_print_acctg_stats(cs_uint16 port_id,
				      m2r_rmac_mem_id_t sram_id,
				      cs_uint16 start,
				      cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/* 	        o SRAM Id                                       */
/*              o First entry                                   */
/*              o Num of consecutive entries                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the SA or DA accounting counters associated with   */
/* a particular address CAM and prints them.                    */
/*                                                              */
/* The [sram_id] parameter specifies which accounting SRAM and  */
/* is one of the following:                                     */
/* M2R_RX_ACCTG_SA_SRAM,            M2R_RX_ACCTG_DA_SRAM,       */
/* M2R_TX_ACCTG_SA_SRAM,       or   M2R_TX_ACCTG_DA_SRAM,       */
/*                                                              */
/* The [start] and [num] parameters specify the starting entry  */
/* and the number of consecutive entries to be gotten.          */
/*                                                              */
/* NOTE - The SA addresses can be programmed only in the first  */
/* 256 locations and so the SRAM's corresponding to SA entries  */
/* can only be 0 to 255. The DA entries can be 0 to 1023.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_cntr_t 	* p_drvr_stats = NULL ;
  cs_uint16		tbl_sz, ii ;
  m2r_port_cb_t   	* ppcb ;
  cs_uint32             upkts, lpkts, ubytes, lbytes;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  tbl_sz = M2R_RMAC_GET_ACCTG_SRAM_SIZE(sram_id) ;
  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num, tbl_sz,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
  if (ppcb->acctg_stats_malloced) {
    /* Retrieve the hw counters and update the driver stats */
    if ( m2r_rmac_update_acctg_drvr_stats(port_id, sram_id, start, num)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
    p_drvr_stats = m2r_rmac_get_acctg_sram_stats_tbl_ptr(port_id,
							 sram_id) ;
  }

  /* Print the driver stats */
  CS_PRINT(
  "****************************************************************\n"
  ) ;
  CS_PRINT("\tport-%d   %s statistics\n", port_id,
	   M2R_GET_ACCTG_SRAM_NAME(sram_id) ) ;
  CS_PRINT(
  "****************************************************************\n"
  ) ;

  for (ii = start; ii < (start+num); ii++) {
    m2r_rmac_cntr_t 	* p, cntr ;

    if (ppcb->acctg_stats_malloced) {
      p = &p_drvr_stats[ii] ;
    }
    else {  /* no memory allocated for acctg stats */
      if ( m2r_rmac_read_acctg_sram_cntr(port_id, sram_id, ii, &cntr)
	   == CS_ERROR ) {
	return (CS_ERROR) ;
      }
      p = &cntr ;
    }

    if ( m2r_drvr_is_stats_in_hex_fmt() ) {
      upkts = M2R_STATS_GET_UPPER_32BIT_HEX(p->pkts) ;
      lpkts = M2R_STATS_GET_LOWER_32BIT_HEX(p->pkts) ;
      ubytes = M2R_STATS_GET_UPPER_32BIT_HEX(p->bytes) ;
      lbytes = M2R_STATS_GET_LOWER_32BIT_HEX(p->bytes) ;

      if ((upkts>0)&&(ubytes>0)) {
        CS_PRINT("(0x%03x)   pkts= %10x%08xH bytes= %10x%08xH\n",
                 ii, upkts, lpkts, ubytes, lbytes) ;
      } else if (upkts>0) {
        CS_PRINT("(0x%03x)   pkts= %10x%08xH bytes= %18xH\n",
                 ii, upkts, lpkts, lbytes) ;
      } else if (ubytes>0) {
        CS_PRINT("(0x%03x)   pkts= %18xH bytes= %10x%08xH\n",
                 ii, lpkts, ubytes, lbytes) ;
      } else {
        CS_PRINT("(0x%03x)   pkts= %18xH bytes= %18xH\n",
                 ii, lpkts, lbytes) ;
      }
    }
    else {
      upkts = M2R_STATS_GET_BILLIONS_DEC(p->pkts) ;
      lpkts = M2R_STATS_GET_BILLIONS_MOD_DEC(p->pkts) ;
      ubytes = M2R_STATS_GET_BILLIONS_DEC(p->bytes) ;
      lbytes = M2R_STATS_GET_BILLIONS_MOD_DEC(p->bytes) ;

      if ((upkts>0)&&(ubytes>0)) {
        CS_PRINT("(0x%03x)   pkts= %11u%09u bytes= %11u%09u\n",
                 ii, upkts, lpkts, ubytes, lbytes) ;
      } else if (upkts>0) {
        CS_PRINT("(0x%03x)   pkts= %11u%09u bytes= %20u\n",
                 ii, upkts, lpkts, lbytes) ;
      } else if (ubytes>0) {
        CS_PRINT("(0x%03x)   pkts= %20u bytes= %11u%09u\n",
                 ii, lpkts, ubytes, lbytes) ;
      } else {
        CS_PRINT("(0x%03x)   pkts= %20u bytes= %20u\n",
                 ii, lpkts, lbytes) ;
      }
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CLEAR ACCOUNTING SRAM STATS                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_clear_acctg_stats(cs_uint16 port_id,
				      m2r_rmac_mem_id_t sram_id,
				      cs_uint16 start,
				      cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/* 	        o SRAM Id                                       */
/*              o First entry                                   */
/*              o Num of consecutive entries                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Clears the SA or DA accounting counters associated with      */
/* a particular address CAM.                                    */
/*                                                              */
/* The [sram_id] parameter specifies which accounting SRAM and  */
/* is one of the following:                                     */
/* M2R_RX_ACCTG_SA_SRAM,            M2R_RX_ACCTG_DA_SRAM,       */
/* M2R_TX_ACCTG_SA_SRAM,       or   M2R_TX_ACCTG_DA_SRAM,       */
/*                                                              */
/* The [start] and [num] parameters specify the starting entry  */
/* and the number of consecutive entries to be gotten.          */
/*                                                              */
/* NOTE - The SA addresses can be programmed only in the first  */
/* 256 locations and so the SRAM's corresponding to SA entries  */
/* can only be 0 to 255. The DA entries can be 0 to 1023.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_cntr_t 	* p_drvr_stats ;
  cs_uint16		tbl_sz, ii ;
  m2r_port_cb_t   	* ppcb ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  tbl_sz = M2R_RMAC_GET_ACCTG_SRAM_SIZE(sram_id) ;
  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num, tbl_sz,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /*
   * Read from hw counters to clear them
   */
  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
  if (ppcb->acctg_stats_malloced) {
    if ( m2r_rmac_update_acctg_drvr_stats(port_id, sram_id, start, num)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    /* Clear the driver stats */
    p_drvr_stats = m2r_rmac_get_acctg_sram_stats_tbl_ptr(port_id,
							 sram_id) ;
    for (ii = start; ii < (start+num); ii++) {
      p_drvr_stats[ii].pkts = p_drvr_stats[ii].bytes = 0 ;
    }
  }

  else {  /* no memory allocated for acctg stats */
    m2r_rmac_cntr_t cntr ;

    for (ii = start; ii < (start+num); ii++) {
      if ( m2r_rmac_read_acctg_sram_cntr(port_id, sram_id, ii, &cntr)
	   == CS_ERROR ) {
	return (CS_ERROR) ;
      }
    }
  }

  return (CS_OK) ;
}



/****************************************************************/
/* $rtn_hdr_start  LOAD ACCOUNTING SRAM STATS                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status  m2r_rmac_load_acctg_stats(cs_uint16 port_id,
				     m2r_rmac_mem_id_t sram_id,
				     cs_uint16 start, cs_uint16 num,
				     m2r_rmac_cntr_t * p_user_stats)
/* INPUTS     : o Port Id                                       */
/* 	        o SRAM Id                                       */
/*              o First entry                                   */
/*              o Num of consecutive entries                    */
/*              o Caller provided buffer for stats              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Loads the SA or DA accounting counters associated with       */
/* specific address CAM entries. Normally, after driver (cold)  */
/* initialization, these counters are cleared.                  */
/* provided so that software has the flexibility of having      */
/* This API is provided so that the software has the flexibility*/
/* in order to have the driver maintain counters from a non-zero*/
/* starting point - for eg., in warm start, the application     */
/* could save the driver statistics, do warm reboot and call    */
/* this API to reload it.                                       */
/*                                                              */
/* The [sram_id] parameter specifies which accounting SRAM and  */
/* is one of the following:                                     */
/* M2R_RX_ACCTG_SA_SRAM,            M2R_RX_ACCTG_DA_SRAM,       */
/* M2R_TX_ACCTG_SA_SRAM,       or   M2R_TX_ACCTG_DA_SRAM,       */
/*                                                              */
/* The [start] and [num] parameters specify the starting entry  */
/* and the number of consecutive entries to be gotten.          */
/*                                                              */
/* The [p_user_stats] is a pointer to caller provided structure */
/* for the driver to return the statistics.                     */
/*                                                              */
/* NOTE - The SA addresses can be programmed only in the first  */
/* 256 locations and so the SRAM's corresponding to SA entries  */
/* can only be 0 to 255. The DA entries can be 0 to 1023.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_cntr_t 	* p_drvr_stats ;
  cs_uint16		tbl_sz ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if (p_user_stats == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  tbl_sz = M2R_RMAC_GET_ACCTG_SRAM_SIZE(sram_id) ;
  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num, tbl_sz,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  p_drvr_stats = m2r_rmac_get_acctg_sram_stats_tbl_ptr(port_id,
						       sram_id) ;
  if (p_drvr_stats) {
    CS_MEMCPY(&p_drvr_stats[start], p_user_stats, 
	      sizeof(m2r_rmac_cntr_t) * num) ;
    return (CS_OK) ;
  }
  else {
    CS_HNDL_ERROR( port_id, EM2R_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }
}


/*****************************************************************
 *          INTERNAL UTILITIES AND FUNCTIONS                     *
 *****************************************************************/

/********************************
 * Reset address(Rx and Tx) cam *
 ********************************/
static
cs_status  m2r_rmac_reset_addr_cam(cs_uint16 port_id,
				   m2r_rmac_mem_id_t mem_id,
				   cs_uint32 start,
				   cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		ii ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_ADDR_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /*
   * Clear all the bits. DA and SA bits set to 0 indicates that the
   * entry is invalid.
   */
  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;
  gram.GRAM_CR.bf.SEL    = M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
  for (ii = start; ii < (start + num_entries); ii++) {
    gram.GRAM_CR.bf.GRAM_A = ii ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  return(CS_OK) ;
}


/********************************
 * Reset status(Rx and Tx) sram *
 ********************************/
static
cs_status  m2r_rmac_reset_status_sram(cs_uint16 port_id,
				      m2r_rmac_mem_id_t mem_id,
			       cs_uint32 start, cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint16		bits ;
  cs_uint32  		entry_num, entries_left ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_ADDR_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;
  gram.GRAM_CR.bf.SEL   = M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  gram.GRAM_CR.bf.GRWN  = GRWN_WRITE ;

  entry_num    = start ;
  entries_left = num_entries ;

  if ( start < M2R_ADDR_CAM_SA_NUM_ENTRIES ) { /* lower 256 entries */
    while ( (entries_left) &&
	    (entry_num < M2R_ADDR_CAM_SA_NUM_ENTRIES) ) {

      if ( (entry_num % 4) || (entries_left < 4) ) {
	bits = 0 ;
	if ( m2r_access_status_sram_16bit_entry(port_id, mem_id,
						M2R_WRITE,
						entry_num, &bits)
	     == CS_ERROR ) {
	  return (CS_ERROR) ;
	}
	entry_num++ ;
	entries_left-- ;
      }

      else {
	gram.GRAM_CR.bf.GRAM_A = entry_num ;
	if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
	  return (CS_ERROR) ;
	}
	entry_num    += 4 ;
	entries_left -= 4 ;
      }
    }
  }

  else { /* for entries beyond 256 */
    while ( (entries_left) &&
	    (entry_num < M2R_NUM_OF_ADDR_CAM_ENTRIES) ) {

      if ( (entry_num % 16) || (entries_left < 16) ) {
	bits = 0 ;
	if (m2r_access_status_sram_4bit_entry(port_id, mem_id,
					      M2R_WRITE,
					      entry_num, &bits)
	    == CS_ERROR ) {
	  return (CS_ERROR) ;
	}
	entry_num++ ;
	entries_left-- ;
      }

      else {
	gram.GRAM_CR.bf.GRAM_A = entry_num ;
	if ( m2r_rmac_write_gram_regs(port_id, & gram) == CS_ERROR ) {
	  return (CS_ERROR) ;
	}
	entry_num    += 16 ;
	entries_left -= 16 ;
      }
    }
  }

  return (CS_OK) ;
}


/*************************************
 * Reset accounting(Rx and Tx) sram  *
 *************************************/
static
cs_status  m2r_rmac_reset_acctg_sram(cs_uint16 port_id,
				     m2r_rmac_mem_id_t mem_id,
				     cs_uint32 start,
				     cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		ii ;

  if (mem_id == M2R_RX_ACCTG_SA_SRAM ||
      mem_id == M2R_TX_ACCTG_SA_SRAM ) {
    if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			      M2R_ADDR_CAM_SA_NUM_ENTRIES,
			      __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
    }
  }
  else {
    if (  M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			       M2R_NUM_OF_ADDR_CAM_ENTRIES,
			       __FUNCTION__, __LINE__)
	  == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /*
   * The accounting SRAM entries are clear-on-read
   */
  gram.GRAM_CR.bf.SEL    = M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  for (ii = start; ii < (start + num_entries); ii++) {
    gram.GRAM_CR.bf.GRAM_A = ii ;
    if ( m2r_rmac_read_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/*************************************************
 * Reset Microsequencer(data, core and program)  *
 *************************************************/
static
cs_status  m2r_rmac_reset_mseq_mem(cs_uint16 port_id,
				   m2r_rmac_mem_id_t mem_id,
				   cs_uint32 start,
				   cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		i ;
  cs_uint32		sz ;

  switch (mem_id) {
    case M2R_MS_DATA_STORE :
      sz = M2R_NUM_OF_MS_DATA_STORE_ENTRIES ;
      break ;
    case M2R_MS_CORE_STORE :
      sz = M2R_NUM_OF_MS_CORE_STORE_ENTRIES ;
      break ;
    default :
    case M2R_MS_PGM_STORE  :
      sz = M2R_NUM_OF_MS_PGM_STORE_ENTRIES ;
      break ;
  }

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    sz, __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_ms_ctl_stall(port_id, M2R_ENABLE) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;
  gram.GRAM_CR.bf.SEL    = M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;

  for (i = start; i < (start + num_entries); i++) {
    gram.GRAM_CR.bf.GRAM_A = i ;
    if ( mem_id == M2R_MS_CORE_STORE ) {
      gram.GRAM_CR.bf.GRAM_A |= (1 << 10) ; /* A[10] s/b 1 */
    }
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/*************************************************
 * Reset Mcast Coarse Filter CAM & Counter       *
 *************************************************/
static
cs_status  m2r_rmac_reset_coarse_cam(cs_uint16 port_id,
				     m2r_rmac_mem_id_t mem_id,
				     cs_uint32 start,
				     cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		i ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;
  gram.GRAM_CR.bf.SEL    = M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
  for (i = start; i < (start + num_entries); i++) {
    /* clear key */
    gram.GRAM_CR.bf.GRAM_A = i ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
    /* clear control */
    gram.GRAM_CR.bf.GRAM_A = i | (1 << 10) ; /* A[10] s/b 1 */
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  /* Reset STATs counters -> decided by APP
  m2r_rmac_stats_mcoarse_cam_clear_units(port_id,
                                         start,
                                         num_entries) ;
   */

  return (CS_OK) ;
}

cs_status  m2r_rmac_reset_coarse_cntr(cs_uint16 port_id,
				     cs_uint32 start,
				     cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		i ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /*
   * The Multicast coarse counters are clear-on-read
   */
  gram.GRAM_CR.bf.SEL    = SEL_LKP_COARSE_CAM ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  for (i = start; i < (start + num_entries); i++) {
    gram.GRAM_CR.bf.GRAM_A = i | (1 << 9) ; /* A[10:9] s/b b01 */
    if ( m2r_rmac_read_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}

cs_status  m2r_rmac_get_coarse_cntr(cs_uint16 port_id,
				     cs_uint32 entry_num,
				     m2r_rmac_cntr_t * pcntr)
{
  m2r_rmac_gram_t	gram ;

  if ( entry_num > M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES-1 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
   ": Multicast coarse cntr entry number(%d) should be less than %d\n",
		  entry_num, M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES) ;
  	return (CS_ERROR);
  }

  gram.GRAM_CR.bf.SEL    = SEL_LKP_COARSE_CAM ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = entry_num | (1 << 9) ;
  if ( m2r_rmac_read_gram_regs(port_id, &gram)  == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  /*
   * Counter is clear on read
   */
  pcntr->pkts = ( gram.GRAM_D0.wrd | (gram.GRAM_D1.wrd << 16) ) ;
  pcntr->bytes = ( ((cs_uint64)gram.GRAM_D2.wrd) |
		   (((cs_uint64)gram.GRAM_D3.wrd) << 16) |
		   (((cs_uint64) (gram.GRAM_D4.wrd & 0xFF)) << 32) ) ;   

  return (CS_OK) ;
}


/**********************************
 * Reset MS Multichoke SRAM       *
 **********************************/
static
cs_status  m2r_rmac_reset_mchoke_sram(cs_uint16 port_id,
				      cs_uint32 start,
				      cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		i ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_MCHOKE_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;
  gram.GRAM_CR.bf.SEL    = SEL_USEQ_MCHOKE_RAM ;
  gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
  for (i = start; i < (start + num_entries); i++) {
    gram.GRAM_CR.bf.GRAM_A = i ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}

/***********************************************
 * Reset Host Drop Coarse Filter CAM & Counter *
 ***********************************************/
static
cs_status  m2r_rmac_reset_host_drop_cam(cs_uint16 port_id,
					m2r_rmac_mem_id_t mem_id,
					cs_uint32 start,
					cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		i ;
  cs_uint16     bits = 0 ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* clear key & mask */
  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;
  gram.GRAM_CR.bf.SEL    = M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
  for (i = start; i < (start + num_entries); i++) {
    gram.GRAM_CR.bf.GRAM_A = i ;
    bits |= (1 << i) ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  /* read & mod & write to clear valid bits */
  gram.GRAM_CR.bf.SEL = M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  gram.GRAM_CR.bf.GRAM_A = 0x10 ;
  gram.GRAM_CR.bf.GRWN = GRWN_READ ;

  m2r_rmac_read_gram_regs(port_id, &gram) ;

  gram.GRAM_D0.wrd &= ((~bits) & 0xffff) ;
  gram.GRAM_CR.bf.GRWN = GRWN_WRITE ;

  if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  /* Reset STATs counters -> decided by app
  m2r_rmac_stats_host_drop_cam_clear_units(port_id,
                                           start,
                                           num_entries) ;
   */

  return (CS_OK) ;
}

static
cs_status  m2r_rmac_reset_host_drop_cntr(cs_uint16 port_id,
					cs_uint32 start,
					cs_uint32 num_entries)
{
  m2r_rmac_gram_t	gram ;
  cs_uint32		i ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_HOST_PKT_DROP_CNTR_ENTRIES,
			    __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /*
   * The Host Packet drop coarse counters are clear-on-read
   */
  gram.GRAM_CR.bf.SEL    = SEL_HOST_PKT_DROP_CNTRS ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  for (i = start; i < (start + num_entries); i++) {
    gram.GRAM_CR.bf.GRAM_A = i ;

    if ( m2r_rmac_read_host_drop_gram_regs(port_id, &gram)
	 == CS_ERROR ) {
      return(CS_ERROR) ;
    }

  }

  return (CS_OK) ;
}


static
cs_status  m2r_rmac_get_host_drop_cntr(cs_uint16 port_id,
				       cs_uint32 entry_num,
                                       m2r_rmac_cntr_t * pcntr)
{
  m2r_rmac_gram_t	gram ;

  /*
   * only the first 68 entries are used for host drop packet counters,
   * the last entry, 69, is used to indicate saturation status of
   * the counter and not valid for this API.
   */
  if ( entry_num > M2R_NUM_OF_HOST_PKT_DROP_CNTR_ENTRIES-2 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
      ": HOST pkt drop cntr entry number(%d) should be less than %d\n",
		  entry_num, M2R_NUM_OF_HOST_PKT_DROP_CNTR_ENTRIES-1) ;
  	return (CS_ERROR);
  }

  gram.GRAM_CR.bf.SEL    = SEL_HOST_PKT_DROP_CNTRS ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = entry_num ;

  if ( m2r_rmac_read_host_drop_gram_regs(port_id, &gram)  == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  /*
   * Counter is clear on read
   * In addtition, byte counter is 40-bit wide.
   */
   
  pcntr->pkts = ( gram.GRAM_D0.wrd | (gram.GRAM_D1.wrd << 16) ) ;
  pcntr->bytes = ( ((cs_uint64)gram.GRAM_D2.wrd)
                   | (((cs_uint64)gram.GRAM_D3.wrd) << 16)
                   | (((cs_uint64)(gram.GRAM_D4.wrd & 0xff)) << 32) ) ;

  
  return (CS_OK) ;
}

cs_status  m2r_rmac_clear_host_drop_sat(cs_uint16 port_id)
{
  return (m2r_rmac_reset_host_drop_cntr(port_id,
                        M2R_HOST_PKT_DROP_CNTR_SAT_ENTRY, 1));
}

cs_status m2r_rmac_reset_host_drop_channel_cntr(cs_uint16 port_id,
                   cs_boolean cam_hit, cs_uint8 channel_id,
                   cs_uint32 start, cs_uint32 num_entries)
{
  if ( channel_id > M2R_RMAC_HOST_MAX_NUM_OF_CHANNEL-1 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": HOST channel id(%d) should be less than 4\n",
		  channel_id) ;
  	return (CS_ERROR);
  }

  if ( cam_hit ) {
    if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			      M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES,
			      __FUNCTION__, __LINE__)
	     == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    if (m2r_rmac_reset_host_drop_cntr(port_id,
	  ((M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES * channel_id) + start),
	  num_entries)
	== CS_ERROR) {
      return (CS_ERROR) ;
    }

  }
  else {
    if (m2r_rmac_reset_host_drop_cntr(port_id,
	 (M2R_HOST_PKT_DROP_CAM_NOHIT_CNTR_BASE_ENTRY + channel_id), 1)
	== CS_ERROR) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


cs_status m2r_rmac_get_host_drop_channel_cntr(cs_uint16 port_id,
                   cs_boolean cam_hit, cs_uint8 channel_id,
                   cs_uint32 entry_num,
                   m2r_rmac_cntr_t * pcntr)
{
  if ( channel_id > M2R_RMAC_HOST_MAX_NUM_OF_CHANNEL-1 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": HOST channel id(%d) should be less than 4\n",
		  channel_id) ;
  	return (CS_ERROR);
  }

  if ( cam_hit ) {
    if ( entry_num > M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES-1 ) {
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
       ": HOST pkt drop cntr entry number(%d) should be less than %d\n",
		    entry_num, M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES) ;
      return (CS_ERROR);
    }

    if (m2r_rmac_get_host_drop_cntr(port_id,
       ((M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES * channel_id) + entry_num),
       pcntr)
	== CS_ERROR) {
      return (CS_ERROR) ;
    }

  }
  else {
    if (m2r_rmac_get_host_drop_cntr(port_id,
	    (M2R_HOST_PKT_DROP_CAM_NOHIT_CNTR_BASE_ENTRY + channel_id),
	    pcntr)
	== CS_ERROR) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}

/**************************************************************
 *       CHECK IF MULTICAST COARSE CAM ENTRY IS VALID
 **************************************************************/
cs_boolean m2r_rmac_check_mcast_coarse_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num)
{
  m2r_rmac_gram_t	gram ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, FALSE) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, FALSE) ;

  if (entry_num > 7) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "entry_num(%d) should be 0...7\n", entry_num) ;
    return (FALSE) ;
  }

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  /* get mask */
  gram.GRAM_CR.bf.SEL   = SEL_LKP_COARSE_CAM ;
  gram.GRAM_CR.bf.GRWN  = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = entry_num | (1 << 10) ; /* A[10] s/b 1 */
  m2r_rmac_read_gram_regs(port_id, &gram) ;

  return ((gram.GRAM_D3.wrd & 2)) ;
}

/**************************************************************
 *       CHECK IF HOST PKT DROP COARSE CAM ENTRY IS VALID
 **************************************************************/
cs_boolean m2r_rmac_check_host_drop_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num)
{
  m2r_rmac_gram_t	gram ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, FALSE) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, FALSE) ;

  if (entry_num > 15) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "entry_num(%d) should be 0...15\n", entry_num) ;
    return (FALSE) ;
  }

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  /* valid bits */
  gram.GRAM_CR.bf.SEL   = SEL_HOST_PKT_DROP_CAM ;
  gram.GRAM_CR.bf.GRWN  = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = 0x10 ;

  m2r_rmac_read_host_drop_gram_regs(port_id, &gram) ;

  return ((gram.GRAM_D0.wrd & (1 << entry_num))) ;
}


/**************************************************************
 *            WRITE TO ADDRESS(Rx or Tx) CAM
 **************************************************************/
static
cs_status  m2r_write_to_addr_cam(cs_uint16 port_id,
				 m2r_rmac_mem_id_t cam_id,
				 cs_uint32 entry_num,
				 m2r_cam_macaddr_t * p)
{
  m2r_rmac_gram_t       gram ;

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  gram.GRAM_D0.wrd = ( p->mac.byte0 | (p->mac.byte1 << 8) ) ;
  gram.GRAM_D1.wrd = ( p->mac.byte2 | (p->mac.byte3 << 8) ) ;
  gram.GRAM_D2.wrd = ( p->mac.byte4 | (p->mac.byte5 << 8) ) ;
  if (p->da) {
    gram.GRAM_D3.wrd |= ( 1 << 7 ) ;  /* bit 55 */
  }
  if (p->sa) {
    gram.GRAM_D3.wrd |= ( 1 << 6 ) ;  /* bit 54 */
  }

  gram.GRAM_CR.bf.SEL    = ( (cam_id == M2R_RX_CAM) ?
			     SEL_LOOKUP_CAM : SEL_STAT_CAM ) ;
  gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
  gram.GRAM_CR.bf.GRAM_A = entry_num ;

  return ( m2r_rmac_write_gram_regs(port_id, &gram) ) ;
}


/***************************************************************
 *         READ/WRITE ACCESS OF STATUS SRAM(16-bit entry)
 * This is for status entries 0...255
 * The Status SRAM's word is 64-bits long and each word can hold
 * 4 status entries. There are 64 (ie. 256/4) words to
 * accommodate all 256 status entries.
 ****************************************************************/
cs_status m2r_access_status_sram_16bit_entry(cs_uint16 port_id,
		  m2r_rmac_mem_id_t ssram_id, m2r_rw_t op,
		  cs_uint32 entry_num, cs_uint16 * pbits)
{
  m2r_rmac_gram_t       gram ;
  cs_uint32	       	sel, word_loc, entry_idx ;

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;
  /*
   * For write operation:
   * Need to do read, modify, write of the 64-bit word in SRAM.
   * word_loc is the address of the word(it is addressed in terms of
   * status entry offset, ie. 0, 4, 8...252) which gets written to
   * the address field in GRAM_CR register.
   */
  word_loc = entry_num & 0x3FC ;
  sel = ( (ssram_id == M2R_RX_STATUS_SRAM) ? SEL_LOOKUP_STATUS
          : SEL_STAT_STATUS ) ;

  gram.GRAM_CR.bf.SEL    = sel ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = word_loc ;
  if (m2r_rmac_read_gram_regs(port_id, &gram) == CS_ERROR) {
    return (CS_ERROR) ;
  }

  /* entry_idx is the entry offset within the 64-bit word */
  entry_idx = entry_num - word_loc ;
  if (op == M2R_READ) {
    switch (entry_idx) {
      default:
      case 0 : *pbits = gram.GRAM_D0.wrd ; break ;
      case 1 : *pbits = gram.GRAM_D1.wrd ; break ;
      case 2 : *pbits = gram.GRAM_D2.wrd ; break ;
      case 3 : *pbits = gram.GRAM_D3.wrd ; break ;
    }
    return(CS_OK) ;
  }

  else { /* M2R_WRITE */
    switch (entry_idx) {
    default:
      case 0 : gram.GRAM_D0.wrd = *pbits ; break ;
      case 1 : gram.GRAM_D1.wrd = *pbits ; break ;
      case 2 : gram.GRAM_D2.wrd = *pbits ; break ;
      case 3 : gram.GRAM_D3.wrd = *pbits ; break ;
    }
    gram.GRAM_CR.bf.SEL    = sel ;
    gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = word_loc ;
    return ( m2r_rmac_write_gram_regs(port_id, &gram) ) ;
  }
}

/***************************************************************
 *          READ/WRITE ACCESS OF STATUS SRAM(4-bit entry)
 * This is for entries 256...1023
 * The Status SRAM's word is 64-bits long and each word can hold
 * 16 status entries. There are 48 (ie. 768/16) words to
 * accommodate all (1024-256 = 768) entries.
 ****************************************************************/
cs_status m2r_access_status_sram_4bit_entry(cs_uint16 port_id,
			     m2r_rmac_mem_id_t ssram_id, m2r_rw_t op,
			     cs_uint32 entry_num, cs_uint16 * pbits)
{
  m2r_rmac_gram_t     	gram ;
  cs_uint32	       	sel, word_loc, entry_idx ;
  cs_reg		* p = NULL ;
  cs_uint32		shift ;

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  /*
   * For write operation:
   * Need to do read-modify-write of the 64-bit word in SRAM.
   * word_loc is the address of the word(it is addressed in terms of
   * status entry offset, ie. 252, 256, 260,...) which gets written to
   * the address field in GRAM_CR register.
   */
  word_loc = entry_num & 0x3F0 ;
  sel = ( (ssram_id == M2R_RX_STATUS_SRAM) ? SEL_LOOKUP_STATUS
          : SEL_STAT_STATUS ) ;

  gram.GRAM_CR.bf.SEL    = sel ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = word_loc ;
  if (m2r_rmac_read_gram_regs(port_id, &gram) == CS_ERROR) {
    return (CS_ERROR) ;
  }

  /* entry_idx is the entry offset within the 64-bit word */
  entry_idx = entry_num - word_loc ;

  /*
   * There are 4 GRAM Data registers(D0 thru D3) and each one
   * holds 4 (4-bit) status entries.
   */
  switch ( entry_idx / 4 ) {
    case 0 : p = (cs_reg *)&gram.GRAM_D0.wrd ; break ;
    case 1 : p = (cs_reg *)&gram.GRAM_D1.wrd ; break ;
    case 2 : p = (cs_reg *)&gram.GRAM_D2.wrd ; break ;
    case 3 : p = (cs_reg *)&gram.GRAM_D3.wrd ; break ;
  }

  shift = (entry_idx % 4) * 4 ;

  if (op == M2R_READ) {
    *pbits = ( (*p) >> shift ) & 0xf ;
    return(CS_OK) ;
  }

  else { /* M2R_WRITE */
    /*
     * Now, get to the right nibble within the GRAM data register,
     * and replace it.
     */
    *p   &= ~( 0x000F << shift ) ;
    *p   |= (*pbits << shift) ;

    gram.GRAM_CR.bf.SEL    = sel ;
    gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = word_loc ;
    return ( m2r_rmac_write_gram_regs(port_id, &gram) ) ;
  }
}

/*************************************************************
 * Update driver accounting stats(for lookup and stat SRAM's *
 *************************************************************/
static
cs_status  m2r_rmac_update_acctg_drvr_stats(cs_uint16 port_id,
					m2r_rmac_mem_id_t sram_id,
					cs_uint16 start, cs_uint16 num)

{
  m2r_rmac_cntr_t 	* p_drvr_stats ;
  cs_uint16  		ii ;

  if ( !(M2R_RMAC_IS_SRAM_ID_VALID(sram_id)) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ":sram_id(0x%x) is INVALID\n", sram_id) ;
    return (CS_ERROR) ;
  }

  p_drvr_stats = m2r_rmac_get_acctg_sram_stats_tbl_ptr(port_id,
						       sram_id) ;
  if (p_drvr_stats == NULL) {
    CS_PRINT("port-%d: **ERROR** p_drvr_stats is NULL ...\n", port_id) ;
    CS_PRINT("        sram_id(%d), start(0x%x), num(0x%x)\n",
	     sram_id, start, num) ;
    m2r_dump_port_cb(port_id) ;
    return (CS_ERROR) ;
  }

  for (ii = start; ii < (start+num); ii++) {
    m2r_rmac_cntr_t  cntr ;

    if ( m2r_rmac_read_acctg_sram_cntr(port_id, sram_id, ii, &cntr)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    p_drvr_stats[ii].pkts  += cntr.pkts ;
    p_drvr_stats[ii].bytes += cntr.bytes ;
  }

  return (CS_OK) ;
}

/*************************************************************
 * Get Packet and Byte counter from Accounting Stats entry   *
 *************************************************************/
static
cs_status  m2r_rmac_read_acctg_sram_cntr(cs_uint16 port_id,
					 m2r_rmac_mem_id_t sram_id,
					 cs_uint16 entry_num,
					 m2r_rmac_cntr_t * pcntr)
{
  m2r_rmac_gram_t	gram ;

  pcntr->pkts = pcntr->bytes = 0 ;
  CS_MEMSET ( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  gram.GRAM_CR.bf.SEL    = M2R_RMAC_MEMID_TO_MEMSEL(sram_id) ;
  gram.GRAM_CR.bf.GRAM_A = entry_num ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  m2r_rmac_read_gram_regs(port_id, &gram) ;

  /* accounting counters are cleared on read. */
  pcntr->pkts  = ( gram.GRAM_D0.wrd | (gram.GRAM_D1.wrd << 16) ) ;
  pcntr->bytes = ( (cs_uint64)gram.GRAM_D2.wrd |
		   (((cs_uint64)gram.GRAM_D3.wrd) << 16) |
		   (((cs_uint64)gram.GRAM_D4.wrd & 0xff) << 32)
		   ) ;		   
   		   
  return (CS_OK) ;
}

/**************************************************************
 * Method to get acctg stats entry in statistics structure    *
 * (currently, it is a contiguous table malloc'ed at init)    *
 **************************************************************/
static
m2r_rmac_cntr_t * m2r_rmac_get_acctg_sram_stats_tbl_ptr(
					 cs_uint16 port_id,
					 m2r_rmac_mem_id_t sram_id)
{
  m2r_port_cb_t   * p = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
  m2r_rmac_cntr_t * q = NULL ;

  if (p == NULL) {
    CS_PRINT("port-%d: ERROR - port CB ptr is NULL\n", port_id) ;
    return (NULL) ;
  }

  switch (sram_id) {
    case M2R_RX_ACCTG_SA_SRAM :
      q = ( &p->p_rmac_acctg_stats->rx_sa[0] ) ; break ;
    case M2R_RX_ACCTG_DA_SRAM :
      q = ( &p->p_rmac_acctg_stats->rx_da[0] ) ; break ;
    case M2R_TX_ACCTG_SA_SRAM :
      q = ( &p->p_rmac_acctg_stats->tx_sa[0] ) ; break ;
    default :
    case M2R_TX_ACCTG_DA_SRAM :
      q = ( &p->p_rmac_acctg_stats->tx_da[0] ) ; break ;
  }
  return (q) ;
}


/**********************************
 *   Write to GRAM registers
 **********************************/
cs_status  m2r_rmac_write_gram_regs(cs_uint16 port_id,
				    m2r_rmac_gram_t * pgram)
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK(port_id, M2R_ID_RMAC) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_D5.wrd,
	       pgram->GRAM_D5.wrd) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_D4.wrd,
	       pgram->GRAM_D4.wrd) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_D3.wrd,
	       pgram->GRAM_D3.wrd) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_D2.wrd,
	       pgram->GRAM_D2.wrd) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_D1.wrd,
	       pgram->GRAM_D1.wrd) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_D0.wrd,
	       pgram->GRAM_D0.wrd) ;

  /* finally, write to CR register */
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_CR.wrd,
	       pgram->GRAM_CR.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_RMAC) ;
  return (CS_OK) ;
}


/**********************************
 *   Read from GRAM registers     *
 **********************************/
cs_status  m2r_rmac_read_gram_regs(cs_uint16 port_id,
				   m2r_rmac_gram_t * pgram)
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK(port_id, M2R_ID_RMAC) ;

  /* Write to GRAM CR register before reading the data registers */
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_CR.wrd,
	       pgram->GRAM_CR.wrd) ;

  pgram->GRAM_D5.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D5.wrd) ;
  pgram->GRAM_D4.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D4.wrd) ;
  pgram->GRAM_D3.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D3.wrd) ;
  pgram->GRAM_D2.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D2.wrd) ;
  pgram->GRAM_D1.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D1.wrd) ;
  pgram->GRAM_D0.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D0.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_RMAC) ;
  return (CS_OK) ;
}

/***************************************************
 *   Read from Host Drop Cntr/CAM GRAM registers   *
 ***************************************************/
cs_status  m2r_rmac_read_host_drop_gram_regs(cs_uint16 port_id,
                                             m2r_rmac_gram_t * pgram)
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  DR_MICRO_IF_GRAM_CR_t tGRAM_CR ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK(port_id, M2R_ID_RMAC) ;

  /* Adam on 10/24/03 -> Access Non-existing Host Drop CAM first */
  if (pgram->GRAM_CR.bf.SEL  == SEL_HOST_PKT_DROP_CNTRS) {
    tGRAM_CR.bf.SEL = SEL_HOST_PKT_DROP_CAM;
    tGRAM_CR.bf.GRAM_A = 17 ;
    tGRAM_CR.bf.GRWN = GRWN_READ ;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_CR.wrd,
                  tGRAM_CR.wrd) ;
  }

  /* Write to GRAM CR register before reading the data registers */
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_CR.wrd,
               pgram->GRAM_CR.wrd) ;

  /* Adam on 10/24/03 -> Need to trigger twice for action to work
   * on host drop cam */
  if (pgram->GRAM_CR.bf.SEL  == SEL_HOST_PKT_DROP_CAM) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.GRAM_CR.wrd,
           pgram->GRAM_CR.wrd) ;
  }

  pgram->GRAM_D5.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D5.wrd) ;
  pgram->GRAM_D4.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D4.wrd) ;
  pgram->GRAM_D3.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D3.wrd) ;
  pgram->GRAM_D2.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D2.wrd) ;
  pgram->GRAM_D1.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D1.wrd) ;
  pgram->GRAM_D0.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.GRAM_D0.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_RMAC) ;
  return (CS_OK) ;
}

/****************************************************
 *  Dump Rx/Tx CAM(and associated Status SRAM) entry
 ****************************************************/
static
void  m2r_print_addr_cam_entry(cs_uint16 port_id,
			       m2r_rmac_mem_id_t cam_id,
			       cs_uint16 entry_num,
			       m2r_rmac_gram_t * pgram,
			       cs_uint16 status_bits)
{
  char * str ;
  cs_boolean	sa, da ;

  da = pgram->GRAM_D3.wrd >> 7 ;
  sa = pgram->GRAM_D3.wrd >> 6 ;
  switch ((da<<1) | sa) {
   default:
   case 0 : str = "INVLD" ; break ;
   case 1 : str = "   SA" ; break ;
   case 2 : str = "DA   " ; break ;
   case 3 : str = "DA|SA" ; break ;
  }

  /* Print MAC address */
  CS_PRINT("0x%03x: %s %02x.%02x.%02x.%02x.%02x.%02x : ",
	   entry_num, str,
	   pgram->GRAM_D2.wrd >> 8, pgram->GRAM_D2.wrd & 0xFF,
	   pgram->GRAM_D1.wrd >> 8, pgram->GRAM_D1.wrd & 0xFF,
	   pgram->GRAM_D0.wrd >> 8, pgram->GRAM_D0.wrd & 0xFF) ;

  /* Print associated Status SRAM entry */
  if (cam_id == M2R_RX_CAM) {
    if (entry_num < M2R_ADDR_CAM_SA_NUM_ENTRIES) {
      CS_PRINT("SR=%d,NE=%d,SAM=%d,DAH=%d,HopCount=0x%02x",
	       (status_bits >> 11) & 1, (status_bits >> 10) & 1,
	       (status_bits >>  9) & 1, (status_bits >>  8) & 1,
	       status_bits & 0xff) ;
    }
    else {
      CS_PRINT("SR=%d,NE=%d,SAM=%d,DAH=%d",
	       (status_bits >> 3) & 1, (status_bits >> 2) & 1,
	       (status_bits >> 1) & 1, status_bits & 1) ;
    }
  }

  else { /* M2R_TX_CAM */
    if (entry_num < M2R_ADDR_CAM_SA_NUM_ENTRIES) {
      CS_PRINT("MaxActv=0x%1x,DATB=%d,DAH=%d,SAH=%d,Wt=0x%02x",
	       (status_bits >> 12) & 0xf, (status_bits >> 10) & 1,
	       (status_bits >> 9)  &   1, (status_bits >>  8) & 1,
	       status_bits & 0xff) ;
    }
    else {
      CS_PRINT("            DATB=%d,DAH=%d,SAH=%d",
	       (status_bits >> 2) & 1, (status_bits >> 1) & 1,
	       status_bits & 1) ;
    }
  }
  CS_PRINT(" (0x%04x)", status_bits) ;

  CS_PRINT("\n") ;
  return ;
}




