/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_port.c
 * "Port" related code is implemented here.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifdef POSIX_PTHREAD
/* Feature #c -> */
#include <pthread.h>
/* Feature #c <- */
#endif

#include "cs_types.h"
#include "cs_rtos.h"
#include "m2r_error.h"

#include "m2r_cb.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats.h"
#include "m2r_gen.h"
#include "m2r_debug.h"

static  cs_boolean dump_mif_pkt = TRUE ;
static  cs_boolean clear_rmac_mems = TRUE ;

static
unsigned long crc32_table[256];
static
void gen_crc32_table(void) ;
static
cs_status m2r_debug_setup_cam_entries(cs_uint16 port_id,
				      cs_uint16 sa_start,
				      cs_uint16 da_start) ;
static
void m2r_insert_hdr_parity(cs_uint16 * pHdr) ;

/*  Externs */
extern
cs_status  m2r_spi42_init_stats(cs_uint16 port_id,
                                cs_uint8 num_channels) ;
extern
cs_status  m2r_rmac_init_stats(cs_uint16 port_id,
                               m2r_port_mac_mode_t mac_mode) ;
extern
cs_status  m2r_eth_init_stats(cs_uint16 port_id) ;







/**********************************************
 * Fast init(for lab use)
 **********************************************/
/****************************************************************/
/* $rtn_hdr_start  FAST INITIALIZATION(DEBUG USE ONLY!)         */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Both                                            */
cs_status  m2r_port_fast_init(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This API is for debug purpose only!                          */
/* This does a fake driver initialization of the specified port.*/
/* There is NO hardware access made but it makes the driver SW  */
/* to allocate and initialize certain data-structures.          */
/* This is provided so that some of the API's can be called     */
/* without doing the actual driver initialization.              */
/* This API is provided mainly for lab purpose.                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_port_cfg_t	* pCfg = NULL ;

  if (port_id > M2R_MAX_NUM_PORTS) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_ID_OUT_OF_RANGE, NULL) ;
    return (CS_ERROR) ;
  }

  CS_PRINT("Doing FAST init for port %d\n", port_id) ;
  pCfg = (m2r_port_cfg_t *) CS_MALLOC( sizeof(m2r_port_cfg_t) ) ;
  if (pCfg == NULL) {
    CS_PRINT("%s(): ERROR - malloc() failed!\n", __FUNCTION__) ;
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *)pCfg, 0, sizeof(m2r_port_cfg_t) ) ;
  pCfg->summ.mac_mode   = M2R_PORT_IN_SRP_MODE ;
  pCfg->summ.line_rate  = M2R_PORT_RATE_OC192C ;
  pCfg->summ.lif_mode   = M2R_PORT_IF_SFI4_1 ;
  pCfg->summ.l2_prot    = M2R_PORT_PROT_POS ;

  pCfg->rmac.cam.acctg_stats_mem_malloced = TRUE ;
  m2r_port_init_data(port_id, pCfg) ;

  if (pCfg) CS_FREE(pCfg) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  FAST(DEBUG) STATS INITIALIZATION             */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Both                                            */
cs_status  m2r_port_fast_stats_init(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This API is mainly for Cortina INTERNAL use!                 */
/* This API is provided so as SW driver can set up the          */
/* statistics data structures.                                  */
/* When called, the driver retrieves the running configuration  */
/* from hardware and based on that does registration of the     */
/* appropriate control blocks with the stats manager. It is     */
/*                                                              */
/* Ideally, this API will not be called as the port init API,   */
/* m2r_port_init() would take care of allocating the daatstructs*/
/* and the registration of them.                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_port_runcfg_t * pRunCfg = NULL ;
  cs_uint32	    err_code = 0 ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, ": port_id= %d\n",
                  port_id) ;
    return(CS_ERROR) ;
  }

  pRunCfg = (m2r_port_runcfg_t *)
            CS_MALLOC( sizeof(m2r_port_runcfg_t) ) ;

  if (pRunCfg == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED, NULL ) ;
    return (CS_ERROR) ;
  }

  if ( m2r_port_get_running_cfg(port_id, pRunCfg) == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  if (pRunCfg->spi42_valid) {
    m2r_spi42_runcfg_t	* p = &pRunCfg->spi42 ;
    cs_uint32           num_chan ;

    num_chan = (p->tx_cal.len == 0) ? 1 : 4 ;
    if ( m2r_spi42_init_stats(port_id, num_chan) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg->lif_valid) {
    m2r_lif_runcfg_t	* p = &pRunCfg->lif ;

    if ( m2r_lif_init_stats(port_id, p->interface_sel,
			    p->sonet_mode) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }


  if (pRunCfg->framer_valid) {
    m2r_framer_runcfg_t * p = &pRunCfg->framer ;
    cs_uint32           strm ;
    m2r_framer_svc_type_t svc ;

    strm = (p->channelized_mode) ? 4 : 0 ;
    svc  = m2r_framer_get_svc_type(port_id, 0) ;

    if ( m2r_framer_init_stats(port_id, strm, svc) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg->eth_valid) {
    if ( m2r_eth_init_stats(port_id) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg->rmac_valid) {
    if ( m2r_rmac_xgxs_init_stats(port_id) == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    if ( m2r_rmac_init_stats(port_id, pRunCfg->summ.mac_mode)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pRunCfg) {
    CS_FREE(pRunCfg) ;
  }

  return (CS_OK) ;
}


#define M2R_CAM_SA_DEBUG_ENTRIES_START	(0x080)
#define M2R_CAM_DA_DEBUG_ENTRIES_START  (0x180)

/****************************************************************/
/* $rtn_hdr_start  PORT DEBUG INITIALIZATION                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status  m2r_port_debug_init(cs_uint16 port_id,
                               m2r_port_mac_mode_t mac_mode,
                               m2r_port_line_rate_t line_rate,
                               m2r_port_prot_t l2_prot,
                               m2r_port_lif_mode_t lifCfg,
			       m2r_rmac_ms_mode_t rmac_ms_mode)
/* INPUTS     : o Port Id                                       */
/*              o MAC Mode                                      */
/*              o Line Rate                                     */
/*              o Framer mapper protocol                        */
/*              o LIF Mode                                      */
/*              o RingMAC - Microsequencer mode                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This API is mainly for Cortina INTERNAL use!                 */
/*                                                              */
/* The [mac_mode] parameter is one of the following:            */
/* M2R_PORT_IN_SRP_MODE,                M2R_PORT_IN_RPR_MODE    */
/* or M2R_PORT_IN_BYPASS_MODE.                                  */
/* Note: For Palermo, M2R_PORT_IN_BYPASS_MODE is the ONLY valid */
/* option.                                                      */
/*                                                              */
/* The [line_rate] parameter is one of the following for MILAN2:*/
/* M2R_PORT_RATE_OC192C or M2R_PORT_RATE_OC192  or              */
/* M2R_PORT_RATE_OC48C  or M2R_PORT_RATE_OC48   or              */
/* M2R_PORT_RATE_10G                                            */
/*                                                              */
/* The [l2_prot] parameter is one of the following:             */
/* M2R_PORT_PROT_POS       or      M2R_PORT_PROT_PPP      or    */
/* M2R_PORT_PROT_ATM       or      M2R_PORT_PROT_GFP      or    */
/* M2R_PORT_PROT_CLEAR_CH  or      M2R_PORT_PROT_ETHERNET       */
/*                                                              */
/* The [lifCfg] parameter is one of the following               */
/* For MILAN2 -                                                 */
/* M2R_PORT_IF_SFI4_1 or M2R_PORT_IF_SFI4_2 or M2R_PORT_IF_XFP  */
/*                                                              */
/* The [rmac_ms_mode] parameter is one of the following:        */
/* M2R_MS_SRP,                                                  */
/* M2R_MS_RPR_AGGRESSIVE_DUAL_TB,                               */
/* M2R_MS_RPR_AGGRESSIVE_SINGLE_TB,                             */
/* M2R_MS_RPR_CONSERVATIVE_DUAL_TB,                             */
/* M2R_MS_RPR_CONSERVATIVE_SINGLE_TB,  M2R_MS_UNKNOWN           */
/* For Bypass mode(ie. Framer only mode), use M2R_MS_UNKNOWN.   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_port_cfg_t	* pCfg = NULL ;
  m2r_dev_cb_t		* pdevcb ;
  cs_status             status = CS_OK ;

  if (port_id > M2R_MAX_NUM_PORTS) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_ID_OUT_OF_RANGE, NULL) ;
    return (CS_ERROR) ;
  }

  /* Init the device if needed */
  pdevcb = M2R_PORT_ID_TO_DEVCB_PTR(port_id) ;

  if (pdevcb->state == M2R_DEV_STATE_INVALID) {
    CS_HNDL_ERROR( port_id, EM2R_DEV_NOT_REGD, NULL ) ;
    return (CS_ERROR) ;
  }
  else if (pdevcb->state == M2R_DEV_STATE_REGISTERED) {
    if ( m2r_dev_init( M2R_PORT_ID_TO_DEV_NUM(port_id) )
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  pCfg = (m2r_port_cfg_t *) CS_MALLOC( sizeof(m2r_port_cfg_t) ) ;
  if (pCfg == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED, NULL ) ;
    return (CS_ERROR) ;
  }

  CS_PRINT(
   "Doing DEBUG init for port %d from user specified configuraion\n",
    port_id
	  ) ;
  CS_MEMSET( (void *)pCfg, 0, sizeof(m2r_port_cfg_t) ) ;

  pCfg->summ.mac_mode     = mac_mode ;
  pCfg->summ.line_rate    = line_rate ;
  pCfg->summ.l2_prot      = l2_prot ;
  pCfg->summ.lif_mode     = lifCfg ;
  pCfg->summ.rmac_ms_mode = rmac_ms_mode ;

  if ( m2r_port_get_default_cfg(port_id, pCfg)  == CS_ERROR ) {
    CS_PRINT("ERROR getting DEFAULT configuration for port %d\n",
             port_id) ;
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  if ( m2r_port_init(port_id, pCfg) == CS_ERROR ) {
    CS_PRINT("ERROR during port-%d initialization\n", port_id) ;
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  if ( (mac_mode == M2R_PORT_IN_SRP_MODE) ||
       (mac_mode == M2R_PORT_IN_RPR_MODE) ) {
    if ( m2r_debug_setup_cam_entries(port_id,
				     M2R_CAM_SA_DEBUG_ENTRIES_START,
				     M2R_CAM_DA_DEBUG_ENTRIES_START)
	 == CS_ERROR ) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }
  }


  RTN_EXIT :
  if (pCfg) CS_FREE(pCfg) ;
  if (status == CS_OK) {
    CS_PRINT("\t **** M2R Debug initialization done ***\n") ;
  }

  return (status) ;
}

/*********************************************************
 * Set up some canned CAM entries for port debug init    *
 * For now, keep this code in modules/general.           *
 *********************************************************/
static
cs_status m2r_debug_setup_cam_entries(cs_uint16 port_id,
				      cs_uint16 sa_start,
				      cs_uint16 da_start)
{
  cs_uint16	sz ;
  /*                                                     *
   * Rx CAM SA entries:                                  *
   *   o SA (C0.00.00.00.00.00) with NE = 0, SAM = 1     *
   *   o SA (00.00.00.00.DE.AD) with NE = 1, SAM = 1     *
   *   o SA (DE.AD.DE.AD.DE.AD) with SR = 1, SAM = 1     *
   *                                                     */
  m2r_rx_cam_entry_t	rx_cam_sa_tbl[] = {
    { /* SA with NE = 0, SAM = 1 */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { FALSE, TRUE, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00} },
      /* SR,   NE,    SAM,  DAH,   hop_count */
      { FALSE, FALSE, TRUE, FALSE, 255 },
      /* preserve da_cntr, sa_cntr */
      FALSE, FALSE,
    },

    { /* SA with NE = 1, SAM = 1 */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { FALSE, TRUE, {0x00, 0x00, 0x00, 0x00, 0xde, 0xad} },
      /* SR,   NE,   SAM,  DAH,  hop_count */
      { FALSE, TRUE, TRUE, FALSE, 255 },
      /* preserve da_cntr, sa_cntr */
      FALSE, FALSE,
    },

    { /* SA with SR = 1, SAM = 1 */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { FALSE, TRUE, {0xde, 0xad, 0xde, 0xad, 0xde, 0xad} },
      /* SR,   NE,   SAM,  DAH,  hop_count */
      { TRUE, FALSE, TRUE, FALSE, 255 },
      /* preserve da_cntr, sa_cntr */
      FALSE, FALSE,
    }
  } ;


  /*                                                      *
   * Rx CAM DA entries:                                   *
   *   o Multicast DA(FF.00.00.00.00.00), with DAH = 1    *
   *   o Unicast   DA(F0.00.00.00.00.00), with DAH = 1    *
   *                                                      */
  m2r_rx_cam_entry_t	rx_cam_da_tbl[] = {
    { /* Multicast DA with DAH = 1 */
      /* DA,  SA,   addr5,  4,    3,    2,    1,    0  */
      { TRUE, FALSE, {0xff, 0x00, 0x00, 0x00, 0x00, 0x00} },
      /* SR,   NE,   SAM,   DAH,  hop_count */
      { FALSE, FALSE, FALSE, TRUE, 0 },
      /* preserve da_cntr, sa_cntr */
      FALSE, FALSE,
    },

    { /* Unicast DA with DAH = 1 */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { TRUE, FALSE, {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00} },
      /* SR,   NE,   SAM,   DAH,  hop_count */
      { FALSE, FALSE, FALSE, TRUE, 0 },
      /* preserve da_cntr, sa_cntr */
      FALSE, FALSE,
    }
  } ;

  /*                                            *
   * Tx CAM SA entries:                         *
   *   o SA (C0.00.00.00.00.00) with SAH = 1    *
   *   o SA (00.00.00.00.DE.AD) with SAH = 1    *
   *                                            */
  m2r_tx_cam_entry_t    tx_cam_sa_tbl[] = {
    { /* SA with SAH = 0 */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { FALSE, TRUE, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00} },
      /* MaxActive, DAM(TB), DAM(Host),  SAH,   Weight */
      {  1,         FALSE,    FALSE,    TRUE,   1  },
      /* preserve da_cntr, preserve_sa_cntr */
      FALSE,               FALSE
    },

    { /* SA with SAH = 1 */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { FALSE, TRUE, {0x00, 0x00, 0x00, 0x00, 0xde, 0xad} },
      /* MaxActive, DAM(TB), DAM(Host),  SAH,   Weight */
      {  1,         FALSE,    FALSE,    TRUE,   1  },
      /* preserve da_cntr, preserve_sa_cntr */
      FALSE,               FALSE
    }
  } ;

  /*                                                                *
   * Tx CAM DA entries:                                             *
   *   o Multicast DA (FF.00.00.00.00.00) with DAMode = 1 (TB only) *
   *   o Unicast   DA (F0.00.00.00.00.00) with DAMode = 3 (Host+TB) *
   *                                                                */
  m2r_tx_cam_entry_t    tx_cam_da_tbl[] = {
    { /* Multicast DA with DAMode = 1 (TB only) */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { TRUE, FALSE, {0xff, 0x00, 0x00, 0x00, 0x00, 0x00} },
      /* MaxActive, DAM(TB), DAM(Host),  SAH,   Weight */
      {  1,         TRUE,    FALSE,      FALSE, 1  },
      /* preserve da_cntr, preserve_sa_cntr */
      FALSE,               FALSE
    },

    { /* Unicast DA with DAMode = 3 (Host and TB) */
      /* DA,   SA,   addr5,  4,    3,    2,    1,    0  */
      { TRUE, FALSE, {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00} },
      /* MaxActive, DAM(TB), DAM(Host),  SAH,   Weight */
      {  1,         TRUE,    TRUE,      FALSE,  1 },
      /* preserve da_cntr, preserve_sa_cntr */
      FALSE,               FALSE
    }
  } ;


  /* program the various debug CAM entries as in the table */
  sz = sizeof(rx_cam_sa_tbl) / sizeof(m2r_rx_cam_entry_t) ;
  if ( m2r_add_rx_cam_entries(port_id, sa_start, sz, rx_cam_sa_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  sz = sizeof(rx_cam_da_tbl) / sizeof(m2r_rx_cam_entry_t) ;
  if ( m2r_add_rx_cam_entries(port_id, da_start, sz, rx_cam_da_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  sz = sizeof(tx_cam_sa_tbl) / sizeof(m2r_tx_cam_entry_t) ;
  if ( m2r_add_tx_cam_entries(port_id, sa_start, sz, tx_cam_sa_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  sz = sizeof(tx_cam_da_tbl) / sizeof(m2r_tx_cam_entry_t) ;
  if ( m2r_add_tx_cam_entries(port_id, da_start, sz, tx_cam_da_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP PORT CONTROL BLOCK                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status m2r_dump_port_cb(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Dumps the Port Control Block(PCB). This is provided for debug*/
/* purpose only.                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_dev_cb_t 	* pdevcb ;
  cs_uint16	dev_num ;
  cs_uint16     sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id) ;
  m2r_port_cb_t * ppcb ;

  if (port_id > M2R_MAX_NUM_PORTS) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_ID_OUT_OF_RANGE, NULL) ;
    return (CS_ERROR) ;
  }

  dev_num = M2R_PORT_ID_TO_DEV_ID(port_id) ;
  pdevcb = M2R_PORT_ID_TO_DEVCB_PTR(port_id) ;

  if (pdevcb == NULL) {
    CS_PRINT("port-%d: ERROR - pdevcb is NULL\n", port_id) ;
    return (CS_ERROR) ;
  }

  ppcb = pdevcb->ppcb[sl_num] ;
  if ( pdevcb->state == M2R_DEV_STATE_INVALID ) {
    CS_HNDL_ERROR( port_id, EM2R_DEV_NOT_REGD,
		   "Port CB at 0x%x\n", ppcb ) ;
    return (CS_ERROR) ;
  }
  CS_PRINT("\n") ;
  CS_PRINT("\t ********************************************\n") ;
  CS_PRINT("\t Port-%d Control Block(dev_num = %d, slice = %d)\n",
	   port_id, dev_num, sl_num) ;
  CS_PRINT("\t ********************************************\n") ;
  CS_PRINT("Device Control Block at    0x%x\n", pdevcb) ;
  if (ppcb == NULL) {
    CS_PRINT("ppcb= NULL (Port CB has not been allocated!)\n") ;
    return(CS_ERROR) ;
  }
  else {
    CS_PRINT("Port Control Block(pcb) at 0x%x (size = 0x%x)\n",
	     ppcb, sizeof(m2r_port_cb_t)) ;
  }
  CS_PRINT("port_id = %d\n", port_id) ;
  CS_PRINT(
    "state   = %d ( Reset(0), Data Init done(1), HW Cfg done(2) )\n",
    ppcb->state) ;
  CS_PRINT("\n") ;
  CS_PRINT("\tConfiguration Summary (summ @ 0x%x): \n", &ppcb->summ) ;

  CS_PRINT("mac mode    = %d ( SRP(0), RPR(1), Bypass(2) )\n",
	   ppcb->summ.mac_mode) ;
  CS_PRINT(
  "line rate   = %d ( OC192c(0), OC192(1), OC48c(2), OC48(3), 10GE(4) )\n",
  ppcb->summ.line_rate) ;

  CS_PRINT("L2 Protocol = %d ( POS(0), PPP(1), ATM(2), GFP(3), CCH(4) )\n",
	   ppcb->summ.l2_prot) ;

  CS_PRINT("LIF Mode    = %d ( SFI4.1(0), SFI4.2(1), XFP(2) )\n",
	   ppcb->summ.lif_mode) ;

  CS_PRINT(
  "MSeq mode   = %d ( SRP(0), RPR-SA(1), RPR-LA(2), RPR-SC(3), RPR-LC(4) )\n",
  ppcb->summ.rmac_ms_mode) ;

  CS_PRINT("\n") ;
  CS_PRINT("Statistics Control Block Table at 0x%x\n",
	   &ppcb->stats_cb) ;
  CS_PRINT("Port Statistics datastruct at     0x%x (size = 0x%x)\n",
	   &ppcb->stats, sizeof(m2r_port_stats_t)) ;
  CS_PRINT("Port RingMAC Acctg Stats Ptr =    0x%x (size = 0x%x)\n",
	   ppcb->p_rmac_acctg_stats,
	   (ppcb->p_rmac_acctg_stats) ? sizeof(m2r_rmac_acctg_stats_t) :
	   0) ;
  CS_PRINT("Port IRQ Control block table at   0x%x\n",
	   &ppcb->irq) ;

  CS_PRINT("\n") ;
  return (CS_OK) ;
}


/**********************************
 *     CRC16(HEC) CALCULATOR      *
 **********************************/
#define CCITT_CRC_GEN   0x1021
static  cs_boolean  hec_bit_reversal = TRUE ;
static  cs_boolean  hec_inverted     = TRUE ;

static
cs_uint8 reverse_bits_in_byte(cs_uint8 dataByte)
{
  cs_uint8  ii, dataOut = 0 ;

  for (ii = 0; ii < 8; ii++) {
    dataOut = ((dataByte >> ii) & 1) ^ ((dataOut << 1) & 0xfe) ;
  }
  return (dataOut) ;
}


cs_uint16  m2r_get_crc16(cs_uint8 * buf, cs_uint32 len)
{
  register cs_uint32 crc = 0xffff ;
  cs_uint32          dataByte ;
  cs_uint16          byte_idx, ii ;

  if (len != 16) {
    CS_PRINT("ERROR: len = %d\n", len) ;
    return(0) ;
  }

  for (byte_idx = 0; byte_idx < len; byte_idx++) {
    dataByte = buf[byte_idx] ;

    if (hec_bit_reversal) {
      dataByte = reverse_bits_in_byte(dataByte) ;
    }

    dataByte <<= 8 ;
    ii = 8 ;
    for (ii = 0; ii < 8; ii++) {
      register long bit = dataByte;
      dataByte += dataByte;
      bit ^= crc;
      crc += crc;
      if (bit &= 0x8000)
        crc ^= CCITT_CRC_GEN;
    } ;

  }

  if (hec_inverted) {
    crc = crc ^ 0xffff ;
  }

  if (hec_bit_reversal) {
    cs_uint8  crcbyte ;
    cs_uint32 dataOut = 0 ;

    for (ii = 0; ii < 2; ii++) {
      crcbyte = ( crc >> (8 * ii) ) & 0xff ;
      dataOut = (reverse_bits_in_byte(crcbyte) << 24) ^
                 ((dataOut >> 8) & 0xffffffff) ;
    }
    crc = (unsigned short)
          ( ((dataOut & 0xffff0000) >> 16) & 0xffff ) ;
  }

  return crc;
}


/***********************************
 *     CRC32  CALCULATOR           *
 ***********************************/
/*
 * Generate CRC32 lookup table
 */
static
void gen_crc32_table(void)
{
  unsigned long crc, polynomial;
  int	ii, jj;

  polynomial = 0xedb88320L;
  for (ii = 0; ii < 256; ii++) {
    crc = ii ;
    for (jj = 8; jj > 0; jj--) {
      if (crc & 1) {
	crc = (crc >> 1) ^ polynomial ;
      }
      else {
	crc >>= 1;
      }

      crc32_table[ii] = crc;
    }
  }
}

/*
 * Compute CRC32 value
 */
cs_uint32 m2r_get_crc32(cs_uint8 * buf, cs_uint32 len)
{
  cs_uint32              crc, temp ;
  cs_uint32              ii ;
  static cs_boolean      init_done = FALSE ;

  if (!init_done) {
    gen_crc32_table() ;
    init_done = TRUE ;
  }

  crc = 0xffffffff;
  for (ii = 0; ii < len; ii++) {
    crc  = ( crc>>8 ) ^ crc32_table[ (crc ^ buf[ii]) & 0xff ];
  }

  crc = crc ^ 0xffffffff ;
  temp = crc ;
  crc = ( ((crc >>  0) & 0xff) << 24 |
          ((crc >>  8) & 0xff) << 16 |
          ((crc >> 16) & 0xff) <<  8 |
          ((crc >> 24) & 0xff) <<  0 ) ;
  return (crc) ;

}


#ifndef RELEASE_PLATFORM
/******************************************************************
 *               FOR CORTINA SIM USE ONLY                         *
 * Utility to provide the delay necessary for running on RTL sim. *
 * For hard-reset, we need to have some delay to ensure           *
 * 1) The reset pulse stays active long enough                    *
 * 2) After the reset is de-asserted, for RingMAC and other blocks*
 *    to settle down.                                             *
 * Currently, the delay is implemented by doing a fixed number of *
 * repeated reads of the JTAG-Id registers.                       *
 * For Milan2 -  3 JTAG-Id reads                                  *
 * to provide sufficient delay for hard reset to work properly.   *
 * Each call to get JTAG-Id, in effect, reads 2 registers.        *
 * The delay argument in the call to this function is ignored.    *
 * For the real hardware, the driver provides the adequate delay. *
 ******************************************************************/
void m2r_sim_hreset_delay(cs_uint16 dev_id, cs_uint32 delay)
{
  volatile cs_uint32  jtag_id ;
  cs_uint16	      i, loop_cnt ;

  loop_cnt = 3 ;

  CS_PRINT("Hard reset delay - read JTAG Id registers a few times\n") ;
  for (i = 0; i < loop_cnt; i++) {
   jtag_id = m2r_get_chip_jtag_id(dev_id) ;
  }
}
#endif


#ifdef POSIX_PTHREAD
/* Feature #c -> */
extern
cs_status  m2r_rmac_debug_send_rcvd_cpu_ctl_pkts(cs_uint16 port_id) ;
extern
cs_status  m2r_rmac_debug_send_rcvd_cpu_ips_pkts(cs_uint16 port_id) ;
extern
void  m2r_rmac_check_and_print_new_ips_msg(cs_uint16 port_id) ;


/*********************************************************************
 * MAIN loop for the DEBUG Thread/processing                         *
 *********************************************************************/
static void * m2r_debug_thread_process(void * port_arg)
{
  m2r_port_cb_t  * ppcb ;
  cs_uint16	 port_id = (cs_uint32) port_arg ;

  ppcb = M2R_PORT_ID_TO_PCB_PTR( port_id ) ;

#ifndef RELEASE_PLATFORM
  if (IN_HW_ENV()) {
    if (ppcb->debug_thread.blk_irq_poll) {
      /* Disable PCI interrupt so that we can do polling */
      cs_lab_phy_enable_isr( 0 );
      if ( m2r_port_enable_blk_irq(port_id,
				   ppcb->debug_thread.blk_irq_poll)
	   == CS_ERROR ) {
        CS_PRINT("port-%d: %s() ERROR - cannot enable block IRQ's!!\n",
	         port_id, __FUNCTION__) ;
        return(NULL) ;
      }
    }
  }
#endif

  while ( ppcb->debug_thread.poll ) {
    if (ppcb->debug_thread.send_rcvd_ctl_pkts) {
      m2r_rmac_debug_send_rcvd_cpu_ctl_pkts( port_id ) ;
    }

    if (ppcb->debug_thread.send_rcvd_ips_pkts) {
      m2r_rmac_debug_send_rcvd_cpu_ips_pkts( port_id ) ;
    }

    if (ppcb->debug_thread.get_ips_msg_in_ttl_mode) {
      m2r_rmac_check_and_print_new_ips_msg(port_id) ;
    }

    if (ppcb->debug_thread.blk_print_stats) {
      CS_PRINT( "\033[H\033[J" ) ;	/* clear screen */
      m2r_print_blk_stats(port_id, ppcb->debug_thread.blk_print_stats ) ;
    }

#ifndef RELEASE_PLATFORM
    if (IN_HW_ENV()) {
      if (ppcb->debug_thread.blk_irq_poll) {
        m2r_isr_handler( M2R_PORT_ID_TO_DEV_ID(port_id) ) ;
      }
    }
#endif

    CS_MDELAY( ppcb->debug_thread.msec_poll_intvl ) ;  /* milli-seconds */
  }

  return ( NULL );
}
/* Feature #c <- */
#endif /* POSIX_PTHREAD */

#ifdef POSIX_PTHREAD
/* Feature #c -> */
/****************************************************************/
/* $rtn_hdr_start START A NEW THREAD FOR DEBUG PROCESSING       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status m2r_start_debug_thread(cs_uint16 port_id,
				 cs_uint32 msec_poll_intvl,
				 cs_boolean send_rcvd_ctl_pkts,
				 cs_boolean send_rcvd_ips_pkts,
				 cs_boolean get_ips_msg_in_ttl_mode,
				 cs_boolean blk_print_stats,
				 cs_uint32  blk_irq_poll)
/* INPUTS     : o Port Id                                       */
/*              o Polling Interval in milli-seconds             */
/*              o Send back received CTL packets(to cpu)        */
/*              o Send back received IPS packets(to cpu)        */
/*              o Get new IPS messages when in TTL mode(for RPR)*/
/*              o Block print statistics                        */
/*              o Do IRQ polling                                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Start a debug thread for miscellaneous debug processing.     */
/* The various debug processing can optionally selected.        */
/* The debug thread wakes up periodically(as specified by the   */
/* polling interval) to do the processing.                      */
/*                                                              */
/* The [blk_print_stats] and [blk_irq_poll] parameters specify  */
/* which blocks for which statistics and interrupt polling      */
/* should be done. It is a bit-map and is specified as one of   */
/* the following -                                              */
/* For Milan2 -                                                 */
/* M2R_ID_SPI42, M2R_ID_PPRBS, M2R_ID_FRAMER, M2R_ID_ETH  or    */
/* M2R_ID_RMAC,  M2R_ID_XGXS,  M2R_ID_LIF or M2R_ID_ALL_BLOCKS  */
/*                                                              */
/* The debug thread can be disabled by calling                  */
/* m2r_stop_debug_thread().                                     */
/* Also, the thread is disabled if the port were to be          */
/* re-initialized.                                              */
/*                                                              */
/* NOTE -                                                       */
/* The API is provided solely for debugging purpose and the     */
/* debug thread is iqnftive by default. It gets spawned only    */
/* on calling this API.                                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	 err_code = 0 ;
  m2r_port_cb_t  * ppcb ;
  cs_uint32      port_arg = (cs_uint32) port_id ;


  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if ( ppcb ) {
    ppcb->debug_thread.poll               = TRUE ;
    ppcb->debug_thread.msec_poll_intvl    = msec_poll_intvl ;
    ppcb->debug_thread.send_rcvd_ctl_pkts = send_rcvd_ctl_pkts ;
    ppcb->debug_thread.send_rcvd_ips_pkts = send_rcvd_ips_pkts ;
    ppcb->debug_thread.blk_irq_poll       = blk_irq_poll ;
    ppcb->debug_thread.get_ips_msg_in_ttl_mode =
      get_ips_msg_in_ttl_mode ;
    ppcb->debug_thread.blk_print_stats        = blk_print_stats ;
    if ( pthread_create( &ppcb->debug_thread.tid, NULL,
			 m2r_debug_thread_process,
			 (void *) port_arg) ) {
      CS_PRINT("port-%d: ERROR creating Debug thread!", port_id);
      return (CS_ERROR) ;
    }

    CS_PRINT("Port-%d: A new Debug thread(tid: 0x%x) started...\n",
	     port_id, ppcb->debug_thread.tid) ;
  }

  return (CS_OK) ;
}
/* Feature #c <- */
#endif /* POSIX_PTHREAD */


#ifdef POSIX_PTHREAD
/* Feature #c -> */
/****************************************************************/
/* $rtn_hdr_start STOP THE DEBUG THREAD                         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status m2r_stop_debug_thread(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable the debug thread processing.                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	 err_code = 0 ;
  m2r_port_cb_t  * ppcb ;
  void           * th_ret = NULL ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if ( ppcb && ppcb->debug_thread.tid ) {
    ppcb->debug_thread.poll = FALSE ;
    pthread_join(ppcb->debug_thread.tid, &th_ret)  ;
    CS_PRINT("Port-%d: Debug thread(tid: 0x%x) cancelled...\n",
	     port_id, ppcb->debug_thread.tid) ;
    ppcb->debug_thread.tid = 0 ;
  }

#ifndef RELEASE_PLATFORM
  if (IN_HW_ENV()) {
    if (ppcb && ppcb->debug_thread.blk_irq_poll) { /* if IRQ polling earlier */
      m2r_port_disable_blk_irq(port_id,
			       ppcb->debug_thread.blk_irq_poll) ;
      ppcb->debug_thread.blk_irq_poll = 0 ;

      cs_lab_phy_enable_isr( 1 ); /* re-enable PCI interrupt */
    }
  }
#endif

  return (CS_OK) ;
}
/* Feature #c <- */
#endif /* POSIX_PTHREAD */



static
void m2r_insert_hdr_parity(cs_uint16 * pHdr)
{
  cs_uint16  bit_id, num_ones = 0 ;
  cs_uint16  data = (*pHdr) & 0xfffe ;

  while (data) {
    num_ones++ ;
    bit_id = data & (~(data-1)) ;
    data &= ~(bit_id) ;
  }

  if (num_ones % 2) {
    *pHdr &= 0xfffe ;   /* odd no of 1's, set p = 0 */
  }
  else {
    *pHdr |= 1 ; /* even no of 1's, set p = 1 */
  }
}


/****************************************************************/
/* $rtn_hdr_start  GET SRP RING CONTROL HEADER                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_uint16  m2r_get_srp_hdr(cs_uint8 ttl, cs_uint8 ri, cs_uint8 mode,
			   cs_uint8 prio)
/* INPUTS     : o TTL (0 - 255)                                 */
/*              o Ring Id (0 or 1)                              */
/*              o Mode                                          */
/*              o Priority (0 - 7)                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : 16-bit SRP Ring Control Header                  */
/* DESCRIPTION:                                                 */
/* Composes the 16-bit SRP Ring Control header and computes and */
/* inserts the parity(odd-parity) in bit-0.                     */
/*                                                              */
/* The [mode] parameter is one of the following values:         */
/* 4 - Control packet(pass to Host),                            */
/* 5 - Control packet(locally buffered for the Host)            */
/* 6 - Usage packet                                             */
/* 7 - Data packet                                              */
/* other values - reserved                                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16  hdr = 0 ;

  hdr = ( (ttl << 8) | (ri << 7) | (mode << 4) | (prio << 1) ) ;
  m2r_insert_hdr_parity(&hdr) ;

  return(hdr) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET RPR RING CONTROL HEADER                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_uint16  m2r_get_rpr_hdr(cs_uint8 ttl, cs_uint8 ri, cs_uint8 fe,
			   cs_uint8 pt, cs_uint8 sc,
			   cs_uint8 we)
/* INPUTS     : o TTL (0 - 255)                                 */
/*              o Ring Id (0 or 1)                              */
/*              o Fairness Eligible(0 or 1)                     */
/*              o Payload Type                                  */
/*              o Service Class                                 */
/*              o Wrap Eligible (0 or 1)                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : 16-bit RPR Ring Control Header                  */
/* DESCRIPTION:                                                 */
/* Composes the 16-bit RPR Ring Control header.                 */
/* For Control and Data packets, the parity bit will be set to  */
/* 0, and for Idle and Fairness packets, it will be computed    */
/* and inserted.                                                */
/*                                                              */
/* The [pt] parameter is specified to be one of the following   */
/* values,                                                      */
/* 0 (Idle), 1 (Control), 2 (Fairness), or 3 (Data).            */
/*                                                              */
/* The [sc] parameter is specified to be one of the following   */
/* values,                                                      */
/* 0 (Class-C), 1 (Class B), 2 (Class A, sub-class A1), or      */
/* 3( Class A, sub-class A0).                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16  hdr = 0 ;

  hdr = ( (ttl << 8) | (ri << 7) | (fe << 6) |
	  (pt << 4)  | (sc << 2) | (we << 1) ) ;

  if (pt == 0 || pt == 2) {
    m2r_insert_hdr_parity(&hdr) ;
  }

  return(hdr) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET RPR EXTENDED CONTROL HEADER              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_uint16  m2r_get_rpr_ext_hdr(cs_uint8 ttl, cs_uint8 ext, cs_uint8 ft,
			       cs_uint8 ws, cs_uint8 strict)
/* INPUTS     : o TTL (0 - 255)                                 */
/*              o Extended Frame (0 or 1)                       */
/*              o Flood Type (0 - 3)                            */
/*              o Passed Source(0 or 1)                         */
/*              o Strict Packet(0 or 1)                         */
/* OUTPUTS    : ----                                            */
/* RETURNS    : 16-bit RPR Extended Control Header              */
/* DESCRIPTION:                                                 */
/* Composes the 16-bit RPR Extended Control Header.             */
/*                                                              */
/* The [ft] parameter specifies the Flood Type and is specified */
/* to be one of the following:                                  */
/* 0 (no flood), 1 (uni-directional), 2 (bi-directional), and   */
/* 3 (reserved).                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16  ext_hdr = 0 ;

  ext_hdr = ( (ttl << 8) | (ext << 7) | (ft << 5) | (ws << 4) |
	      (strict << 3) ) ;
  return (ext_hdr) ;
}


static
cs_status  m2r_insert_payload_data(cs_uint16 port_id,
				   cs_uint8 * buf, cs_uint16 payld_sz,
				   m2r_data_pattern_t pat,
				   cs_uint32 rep_data)
{
  cs_uint32  ii ;

  /* Payload */
  switch (pat) {
    default :
      case DATA_PATTERN_INCR_BYTE:
	for (ii = 0; ii < payld_sz; ii++) {
	  buf[ii] = ii ;
	}
	break ;

      case DATA_PATTERN_DECR_BYTE :
	for (ii = 0; ii < payld_sz; ii++) {
	  buf[ii] = 0xff - (ii & 0xff) ;
	}
	break ;

      case DATA_PATTERN_REPEATING :
	for (ii = 0; ii < payld_sz; ii++) {
	  buf[ii] = (rep_data >> ((3 - (ii % 4)) * 8)) & 0xff ;
	}
	break ;

#if 0
      case DATA_PATTER_RANDOM :
        /* not supported for now */
	break ;
#endif
    }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DEBUG UTILITY TO SEND PKTS VIA CTL BUFFER    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Both                                            */
cs_status  m2r_debug_send_test_pkt_via_ctl_buffer(cs_uint16 port_id,
						  cs_uint16 hdr,
						  cs_uint16 da_54,
						  cs_uint16 da_32,
						  cs_uint16 da_10,
						  cs_uint16 sa_54,
						  cs_uint16 sa_32,
						  cs_uint16 sa_10,
						  cs_uint16 ext_hdr,
						  cs_uint16 proto,
						  cs_uint8  ctl_type,
						  cs_uint8  ctl_ver,
						  cs_uint16 len,
						  m2r_data_pattern_t pat,
                                                  cs_uint32 rep_data,
						  cs_uint16 num_pkts)
/* INPUTS     : o Port Id                                       */
/*              o SRP/RPR(2-bytes) header                       */
/*              o DA [byte5, byte4]                             */
/*              o DA [byte3, byte2]                             */
/*              o DA [byte1, byte0]                             */
/*              o SA [byte5, byte4]                             */
/*              o SA [byte3, byte2]                             */
/*              o SA [byte1, byte0]                             */
/*              o RPR Extended control header(2 bytes)          */
/*              o Protocol Type(2 bytes)                        */
/*              o Control Type (should be 0)                    */
/*              o Control Version (should be 0)                 */
/*              o Packet length(incldg hdr, crc)                */
/*              o Data pattern type                             */
/*              o Repeating data[4 bytes]                       */
/*              o Number of packets                             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Debug utility to send one or more specified packets using    */
/* Send Control Buffer in RingMAC. Only Control(including IPS)  */
/* and Data packets can be sent. Parameters that are not        */
/* relevant for the specified packet type will be ignored.      */
/*                                                              */
/* The [pat] parameter is specified to be one of the following: */
/* DATA_PATTERN_INCR_BYTE,   DATA_PATTERN_DECR_BYTE, or         */
/* DATA_PATTERN_REPEATING.                                      */
/* The [rep_data] is a 4-byte pattern that will be repeated     */
/* if the [pat] parameter is set to DATA_PATTERN_REPEATING.     */
/*                                                              */
/* HEC(crc16) and FCS(crc32) will be computed by this API.      */
/* RPR extended control field parameter is ignored for SRP.     */
/*                                                              */
/* The [proto] parameter corresponds to Protocol Type field in  */
/* Data packets. It is as specified by the Standard(ie. 0x0800, */
/* 0x8847, 0x8848, etc. for HDLC encapsulation).                */
/* For SRP control packets, the Protocol Type field will always */
/* be set to 0x2007.                                            */
/*                                                              */
/* The [ctl_ver] and [ctl_type] parameters correspond to the    */
/* Control Version and Control Type fields in Control(and IPS)  */
/* packets. Control Version should be 0 and Control Type should */
/* be set as specified by the Standard.                         */
/* The packet length specified will include the headers and FCS.*/
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	err_code, crc32 ;
  cs_uint8	* buf = NULL ;
  cs_uint16     hec, payld_start, payld_sz, fcs_start, fcs_range ;
  cs_uint16     ii, pkt_idx, val ;
  m2r_port_mac_mode_t  mac_mode ;
  cs_status	status = CS_OK ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if ( (len < 24) || (len > M2R_RMAC_CTL_SEND_BUF_SZ) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  "Invalid length(0x%x) - s/b 24 - 8K byte range\n",
		  len) ;
    return (CS_ERROR) ;
  }

  buf = (cs_uint8 *) CS_MALLOC(len) ;
  if (buf == NULL) {
    CS_PRINT("%s(): malloc() failed\n", __FUNCTION__) ;
    return (CS_ERROR) ;
  }

  mac_mode = M2R_GET_MAC_MODE(port_id) ;

  switch (mac_mode) {
    case M2R_PORT_IN_BYPASS_MODE :
      CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
      status = CS_ERROR ;
      goto rtn_exit ;
      break ;

    default :
    case M2R_PORT_IN_SRP_MODE :
      buf[0] = hdr >> 8 ;   /* SRP Ring Hdr */
      buf[1] = hdr & 0xff ;
      val = (hdr >> 4) & 0x7 ;
      if (val == 4 || val == 5) {  /* SRP CONTROL Packet */
	M2R_COPY_MAC_ADDR((&buf[2]), 0, 0, 0) ; /* DA = all 0's */
	M2R_COPY_MAC_ADDR((&buf[8]), 0, 0, 0) ; /* SA = all 0's */
	buf[14] = 0x20 ;      /* protocol type  */
	buf[15] = 0x07 ;
	buf[16] = ctl_ver ;   /* Control Version  */
	buf[17] = ctl_type ;  /* Control Type     */
	buf[18] = 0 ;         /* Control Checksum */
	buf[19] = 0 ;
	buf[20] = 0x00 ;      /* Control TTL      */
	buf[21] = 0xff ;
	payld_start = 22 ;
      }

      else if ( val == 7 ) {/* SRP DATA Packet */
	M2R_COPY_MAC_ADDR((&buf[2]), da_54, da_32, da_10) ; /* DA */
	M2R_COPY_MAC_ADDR((&buf[8]), sa_54, sa_32, sa_10) ; /* SA */
	buf[14] = proto >> 8 ;  /* protocol type */
	buf[15] = proto & 0xff ;
	payld_start = 16 ;
      }

      else { /* Other SRP Packet type - exit */
	CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		      "Valid for Control/Data pkts only!\n") ;
	status = CS_ERROR ;
	goto rtn_exit ;
      }

      payld_sz    = len - (payld_start + 4) ;
      fcs_start   = 2 ;
      fcs_range   = len - (fcs_start + 4) ;
      break ;

    case M2R_PORT_IN_RPR_MODE :
      buf[0] = hdr >> 8 ;   /* RPR Ring Hdr */
      buf[1] = hdr & 0xff ;
      M2R_COPY_MAC_ADDR((&buf[2]), da_54, da_32, da_10) ; /* DA */
      M2R_COPY_MAC_ADDR((&buf[8]), sa_54, sa_32, sa_10) ; /* SA */
      buf[14] = ext_hdr >> 8 ; /* extended control hdr */
      buf[15] = ext_hdr & 0xff ;
      hec = m2r_get_crc16(buf, 16) ;
      buf[16] = hec >> 8 ;     /* HEC */
      buf[17] = hec & 0xff ;

      val = (hdr >> 4) & 0x3 ;
      if ( val == 3 ) { /* RPR Data Packet */
	val = (ext_hdr >> 7) & 1 ;
	if (val) { /* Data pkt with E = 1 */
	  payld_start = 18 ;
	}
	else { /* Data pkt with E = 0 */
	  buf[18] = proto >> 8 ;  /* protocol type */
	  buf[19] = proto & 0xff ;
	  payld_start = 20 ;
	}
      }

      else if (val == 1) { /* RPR Control packet */
	buf[18] = ctl_type ;
	buf[19] = ctl_ver ;
	payld_start = 20 ;
      }

      else { /* Other RPR Packet type - exit */
	CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		      "Valid for Control/Data pkts only!\n") ;
	status = CS_ERROR ;
	goto rtn_exit ;
      }

      payld_sz    = len - (payld_start + 4) ;
      fcs_start   = 18 ;
      fcs_range   = len - (fcs_start + 4) ;
      break ;
  }

  /* Loop through and send one packet at a time */
  for (pkt_idx = 0; pkt_idx < num_pkts; pkt_idx++) {
    if ( m2r_insert_payload_data(port_id, &buf[payld_start],
				 payld_sz, pat, rep_data)
	 == CS_ERROR ) {
      status = CS_ERROR ;
      goto rtn_exit ;
    }

    crc32 = m2r_get_crc32( &buf[fcs_start], fcs_range ) ;
    buf[len-4] = (crc32 >> 24) & 0xff ;
    buf[len-3] = (crc32 >> 16) & 0xff ;
    buf[len-2] = (crc32 >> 8 ) & 0xff ;
    buf[len-1] = crc32 & 0xff ;

    /* Finished composing the packet - send it */
    if ( m2r_rmac_send_ctl_pkt(port_id, buf, len)
	 == CS_ERROR ) {
      CS_PRINT("ERROR generating packet 0x%x (len= 0x%x bytes)\n",
	       pkt_idx, len) ;
      status = CS_ERROR ;
      goto rtn_exit ;
    }
  }

  if (dump_mif_pkt) {
    CS_PRINT("\n\t-----------------------------------------------\n") ;
    CS_PRINT("\n\t Generated %d packets(len= %d bytes):\n",
	     num_pkts, len) ;
    for (ii = 0; ii < len; ii++) {
      if (ii % 16 == 0) CS_PRINT("\n0x%04x :", ii) ;
      CS_PRINT("%02x ", buf[ii]) ;
    }
    CS_PRINT("\n") ;
    m2r_pkt_decode_dump(mac_mode, buf, len) ;
  }

  rtn_exit :
    if (buf) CS_FREE(buf) ;
  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start  DEBUG UTILITY TO SEND EXTENDED(RPR-ETH) PKTS */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Milan2                                          */
cs_status  m2r_debug_send_extended_rpr_pkts(cs_uint16 port_id,
					    cs_uint16 rpr_hdr,
					    cs_uint16 rpr_da_54,
					    cs_uint16 rpr_da_32,
					    cs_uint16 rpr_da_10,
					    cs_uint16 rpr_sa_54,
					    cs_uint16 rpr_sa_32,
					    cs_uint16 rpr_sa_10,
					    cs_uint16 ext_hdr,
					    cs_uint16 eth_da_54,
					    cs_uint16 eth_da_32,
					    cs_uint16 eth_da_10,
					    cs_uint16 eth_sa_54,
					    cs_uint16 eth_sa_32,
					    cs_uint16 eth_sa_10,
					    cs_uint16 eth_da_stride,
					    cs_uint16 eth_sa_stride,
					    cs_uint16 ether_type,
					    cs_boolean vlan_pkt,
					    cs_uint8  user_prio,
					    cs_uint16 vlan_id,
					    m2r_data_pattern_t pat,
					    cs_uint32 rep_data,
					    cs_uint16 len,
					    cs_uint16 num_pkts)
/* INPUTS     : o Port Id                                       */
/*              o RPR(2-bytes) header                           */
/*              o RPR DA[byte5, byte4]                          */
/*              o RPR DA[byte3, byte2]                          */
/*              o RPR DA[byte1, byte0]                          */
/*              o RPR SA[byte5, byte4]                          */
/*              o RPR SA[byte3, byte2]                          */
/*              o RPR SA[byte1, byte0]                          */
/*              o RPR Extended control header(2 bytes)          */
/*              o Ethernet DA [byte5, byte4]                    */
/*              o Ethernet DA [byte3, byte2]                    */
/*              o Ethernet DA [byte1, byte0]                    */
/*              o Ethernet SA [byte5, byte4]                    */
/*              o Ethernet SA [byte3, byte2]                    */
/*              o Ethernet SA [byte1, byte0]                    */
/*              o DA stride [0...64K-1]                         */
/*              o SA stride [0...64K-1]                         */
/*              o Ethernet Type                                 */
/*              o VLAN packet(TRUE or FALSE)                    */
/*              o User Priority[0..7]                           */
/*              o VLAN-Id [0...4K-1]                            */
/*              o Data pattern type                             */
/*              o Repeating data[4 bytes]                       */
/*              o Packet length(including hdr, crc)             */
/*              o Number of packets                             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Debug utility to send one or more RPR-Extended packets(ie.   */
/* RPR encapsulated Ethernet frames) using the Control Send     */
/* buffer.                                                      */
/*                                                              */
/* The [eth_da_stride] and [eth_sa_stride] are strides for      */
/* incrementing Ethernet DA and SA MAC addresses. Currently,    */
/* this feature is yet to be supported.                         */
/*                                                              */
/* The [vlan_pkt] should be set to TRUE if it is a VLAN packet. */
/*                                                              */
/* The [ether_type] is the Ethernet type field. If it is a VLAN */
/* packet then a value of 0x8100 will be used for the first     */
/* ether type field in the packet, and this user specified one  */
/* will be used for the second one(ie. the protocol type).      */
/*                                                              */
/* The [user_prio] and [vlan_id] are valid and used only for    */
/* VLAN packets. They specify the user priority field and       */
/* the VLAN Id.                                                 */
/*                                                              */
/* The [pat] parameter is specified to be one of the following: */
/* DATA_PATTERN_INCR_BYTE or DATA_PATTERN_DECR_BYTE,.           */
/*                                                              */
/* The [rep_data] is a 4-byte pattern that will be repeated     */
/* if the [pat] parameter is set to DATA_PATTERN_REPEATING.     */
/*                                                              */
/* HEC(crc16) and FCS(crc32) will be computed by this API.      */
/*                                                              */
/* The packet length specified will include the headers and FCS.*/
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32     err_code, crc32 ;
  cs_uint8      * buf = NULL ;
  cs_uint16     hec, payld_start, payld_sz, fcs_start, fcs_range ;
  cs_uint16     ii, pkt_idx ;
  m2r_port_mac_mode_t  mac_mode ;
  cs_boolean    print_done = FALSE ;
  cs_status     status = CS_OK ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if ( (len < 40) || (len > M2R_RMAC_CTL_SEND_BUF_SZ) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
                  "Invalid length(%d) - s/b 40 - 2K byte range\n",
                  len) ;
    return (CS_ERROR) ;
  }

  if ( !(ext_hdr & 0x0080) ) { /* E-bit is 0 */
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
                  "ext_hdr(0x%04x) - should have E-bit!\n", ext_hdr) ;
    return (CS_ERROR) ;
  }

  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if ( (mac_mode == M2R_PORT_IN_BYPASS_MODE) ||
       (mac_mode == M2R_PORT_IN_SRP_MODE) ) {
    CS_PRINT("RPR extended packets not valid in Bypass/SRP mode\n") ;
    return (CS_ERROR) ;
  }

  buf = (cs_uint8 *) CS_MALLOC(len) ;
  if (buf == NULL) {
    CS_PRINT("%s(): malloc() failed\n", __FUNCTION__) ;
    return (CS_ERROR) ;
  }

  /*
   * non-vlan extended RPR packet :
   *   00+-------------------+
   *     |     RPR-Hdr(2)    |
   *   02+-------------------+
   *     |     RPR-DA(6)     |
   *   08+-------------------+
   *     |     RPR-SA(6)     |
   *   14+-------------------+
   *     |     RPR-ExtHdr(2) |
   *   16+-------------------+
   *     |     RPR-HEC(2)    |
   *   18+-------------------+
   *     |     Eth-DA(6)     |
   *   24+-------------------+
   *     |     Eth-SA(6)     |
   *   30+-------------------+
   *     |     Ether Type    |
   *   32+-------------------+
   *     |                   |
   *     =     Eth payload   =
   *     |                   |
   *     +-------------------+
   *     |     FCS(4)        |
   *     +-------------------+
   *
   *
   * vlan extended RPR packet:
   *   00+-------------------+
   *     |     RPR-Hdr(2)    |
   *   02+-------------------+
   *     |     RPR-DA(6)     |
   *   08+-------------------+
   *     |     RPR-SA(6)     |
   *   14+-------------------+
   *     |     RPR-ExtHdr(2) |
   *   16+-------------------+
   *     |     RPR-HEC(2)    |
   *   18+-------------------+
   *     |     Eth-DA(6)     |
   *   24+-------------------+
   *     |     Eth-SA(6)     |
   *   30+-------------------+
   *     |     0x8100        |
   *   32+-------------------+
   *     |Prio|C|  VLAN-Id   |
   *   34+-------------------+
   *     |    Ether Type     |
   *   36+-------------------+
   *     |                   |
   *     =     Eth payload   =
   *     |                   |
   *     +-------------------+
   *     |     FCS(4)        |
   *     +-------------------+
   *
   */

  buf[0] = rpr_hdr >> 8 ;   /* RPR Ring Hdr */
  buf[1] = rpr_hdr & 0xff ;

  /* RPR MAC address */
  M2R_COPY_MAC_ADDR((&buf[2]), rpr_da_54, rpr_da_32, rpr_da_10) ;
  M2R_COPY_MAC_ADDR((&buf[8]), rpr_sa_54, rpr_sa_32, rpr_sa_10) ;
  buf[14] = ext_hdr >> 8 ;
  buf[15] = ext_hdr & 0xff ;
  hec = m2r_get_crc16(buf, 16) ;
  buf[16] = hec >> 8 ;     /* HEC */
  buf[17] = hec & 0xff ;

  /* Ethernet MAC address */
  M2R_COPY_MAC_ADDR((&buf[18]), eth_da_54, eth_da_32, eth_da_10) ;
  M2R_COPY_MAC_ADDR((&buf[24]), eth_sa_54, eth_sa_32, eth_sa_10) ;

  if (vlan_pkt) {
    buf[30] = 0x81 ;
    buf[31] = 0x00 ;
    buf[32] = ( ((user_prio & 0x7) << 5) | (0 << 4) |
		((vlan_id & 0xfff) >> 8) ) ;
    buf[33] = vlan_id & 0xff ;
    buf[34] = ether_type >> 8 ;
    buf[35] = ether_type & 0xff ;
    payld_start = 36 ;
    payld_sz    = len - (payld_start + 4) ;
  }
  else {
    buf[30] = ether_type >> 8 ;
    buf[31] = ether_type & 0xff ;
    payld_start = 32 ;
    payld_sz    = len - (payld_start +  4) ;
  }

  fcs_start   = 18 ;
  fcs_range   = len - (fcs_start + 4) ;

  for (pkt_idx = 0; pkt_idx < num_pkts; pkt_idx++) {
    if ( pkt_idx && eth_da_stride ) {
      M2R_ADD_TO_MAC_ADDR(&eth_da_54, &eth_da_32, &eth_da_10,
			  eth_da_stride) ;
      M2R_COPY_MAC_ADDR((&buf[18]), eth_da_54, eth_da_32, eth_da_10) ;
    }

    if ( pkt_idx && eth_sa_stride ){
      M2R_ADD_TO_MAC_ADDR(&eth_sa_54, &eth_sa_32, &eth_sa_10,
			  eth_sa_stride) ;
      M2R_COPY_MAC_ADDR((&buf[24]), eth_sa_54, eth_sa_32, eth_sa_10) ;
   }

    if ( m2r_insert_payload_data(port_id, &buf[payld_start], payld_sz,
				 pat, rep_data) == CS_ERROR ) {
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }

    crc32 = m2r_get_crc32( &buf[fcs_start], fcs_range ) ;
    buf[len-4] = (crc32 >> 24) & 0xff ;
    buf[len-3] = (crc32 >> 16) & 0xff ;
    buf[len-2] = (crc32 >>  8) & 0xff ;
    buf[len-1] = crc32 & 0xff ;

    /* Finished composing the packet - send it */
    if ( m2r_rmac_send_ctl_pkt(port_id, buf, len) == CS_ERROR ) {
      CS_PRINT("ERROR generating packet 0x%x(len= 0x%x bytes)\n",
	       pkt_idx, len) ;
      status = CS_ERROR ;
      goto RTN_EXIT ;
    }

    if ( dump_mif_pkt && !(print_done) ) {
      CS_PRINT("\n\t-----------------------------------------------\n") ;
      if ( !(eth_da_stride) && !(eth_sa_stride) ) {
	CS_PRINT("\n\tGenerated %d RPR extended packet(len= %d bytes)\n",
		 num_pkts, len) ;
	print_done = TRUE ;
      }
      else {
	CS_PRINT("\n\tGenerated RPR extended packet #%d(len= %d bytes)\n",
		 pkt_idx, len) ;
      }

      for (ii = 0; ii < len; ii++) {
	if (ii % 16 == 0) CS_PRINT("\n0x%04x: ", ii) ;
	CS_PRINT("%02x ", buf[ii]) ;
      }
      CS_PRINT("\n") ;
      m2r_pkt_decode_dump(M2R_PORT_IN_RPR_MODE, buf, len) ;
    }
  }

  RTN_EXIT :
    if (buf) CS_FREE(buf) ;
  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start  DEBUG UTILITY TO DECODE/DUMP SRP/RPR FRAMES  */
/* CATEGORY   : Driver                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Milan2                                          */
void m2r_pkt_decode_dump(m2r_port_mac_mode_t mac_mode, cs_uint8 * buf,
			  cs_uint16 pkt_len)
/* INPUTS     : o MAC mode                                      */
/*              o Pointer to packet  bufferRPR(2-bytes)         */
/*              o Packet length                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Debug utility to decode and dump SRP and RPR frames.         */
/* Currently, only RPR frames are supported.                    */
/* This API can decode all kinds of RPR frames(control, data -  */
/* non-extended, extended(vlan/non-vlan), fairness and idle     */
/* frames).                                                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  char    * str = NULL ;
  cs_uint16 payld_start= 0, payld_sz = 0;
  cs_uint8  pkt_type, tmp ;
  cs_boolean ext_data_pkt = FALSE ;

  switch (mac_mode) {
    case M2R_PORT_IN_RPR_MODE :
      pkt_type = (buf[1] & 0x30) >> 4 ;
      switch (pkt_type) {
	case 0 : str = "Idle" ;  break ;
        case 1 : str = "Control" ; break ;
	case 2 : str = "Fairness" ; break ;
	case 3 : default : str = "Data" ; break ;
      }

      CS_PRINT("\n\tRPR %s Frame of length= %d bytes\n", str, pkt_len) ;
      CS_PRINT("\tttl= 0x%02x, ", buf[0]) ;
      CS_PRINT("ri= %d, ", (buf[1] & 0x80) ? 1 : 0) ;
      CS_PRINT("fe= %d, ", (buf[1] & 0x40) ? 1 : 0) ;
      CS_PRINT("pt= %d(%s), ", (buf[1] & 0x30) >> 4, str) ;
      tmp = (buf[1] & 0x0c) >> 2 ;
      CS_PRINT("sc= %d", tmp) ;
      switch (tmp) {
        case 0 : CS_PRINT("(Class-C), ") ; break ;
        case 1 : CS_PRINT("(Class-B), ") ; break ;
        case 2 : CS_PRINT("(Class-A1), "); break ;
        default: CS_PRINT("(Class-A0), "); break ;
      }

      CS_PRINT("we= %d, ", (buf[1] & 0x02) >> 1) ;
      CS_PRINT("p = %d\n",  buf[1] & 0x01) ;

      if (pkt_type == 1 || pkt_type ==3 ) { /* only control or data */
	CS_PRINT(
	   "\tRPR DA MAC address= %02x.%02x.%02x.%02x.%02x.%02x\n",
	   buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]
	) ;
      }

      CS_PRINT(
	  "\tRPR SA MAC address= %02x.%02x.%02x.%02x.%02x.%02x\n",
	  buf[8], buf[9], buf[10], buf[11], buf[12], buf[13]
	  ) ;

      /* Idle packets - no more header info to process */
      if (pkt_type == 0) { /* idle packets are 16-bytes */
	payld_start = 8 ;
	payld_sz    = 4 ;
	/* all done */
	goto hdr_done ;
      }

      /* Fairness packets */
      if (pkt_type == 2) { /* fairness packets are 16-bytes */
	CS_PRINT("\tversion= %d, fairness value= 0x%04x\n",
		 (buf[8] & 0xe0 >> 4),
		 (buf[10] << 8) | buf[11]) ;
	payld_start = 8 ;
	payld_sz    = 4 ;
	goto hdr_done ;
      }


      CS_PRINT("\tbase_ttl= 0x%02x, ", buf[14]) ;
      ext_data_pkt = (buf[15] & 0x80) ? TRUE : FALSE ;
      CS_PRINT("E(xtended)= %d, ", (ext_data_pkt) ? 1 : 0) ;
      tmp = (buf[15] & 0x60) >> 5 ;
      CS_PRINT("fi = %d", tmp) ;
      switch (tmp) {
        case 0 : CS_PRINT("(no flood), ") ;      break ;
        case 1 : CS_PRINT("(uni-dir flood), ") ; break ;
        case 2 : CS_PRINT("(bi-dir flood), ") ;  break ;
        default: CS_PRINT("(reserved), ") ;      break ;
      }

      CS_PRINT("ps= %d, ", (buf[15] & 0x10) >> 4) ;
      CS_PRINT("so(strict)= %d\n",(buf[15] & 0x08) >> 3) ;
      CS_PRINT("\tHeader Checksum(hec)= 0x%04x\n",
	       (buf[16] << 8 | buf[17])) ;

      switch (pkt_type) {
        case 0 : /* idle packet */
	  break ;

        case 1 : /* control packet */
	  {
	    const char * ctl_tbl[12] = {
	      CS_DI([0]  =) "Unknown",
	      CS_DI([1]  =) "Station Attribute Discovery",
	      CS_DI([2]  =) "Topology and Portection Protocol",
	      CS_DI([3]  =) "Topology and Protection Checksum",
	      CS_DI([4]  =) "LRTT request",
	      CS_DI([5]  =) "LRTT response",
	      CS_DI([6]  =) "FDD",
	      CS_DI([7]  =) "OAM echo request",
	      CS_DI([8]  =) "OAM echo response",
	      CS_DI([9]  =) "OAM flush",
	      CS_DI([10] =) "OAM organization specific",
	      CS_DI([11] =) "Reserved" } ;

	    CS_PRINT("\tcontrolType= 0x%02x (%s), controlVersion= 0x%02x\n",
		     buf[18], ctl_tbl[((buf[18] < 11) ? buf[18] : 11)], buf[19]) ;
	    payld_start = 20 ;
	    payld_sz    = pkt_len - (payld_start + 4) ;
	  }
	  break ;

        case 2 : /* fairness packet */
	  break ;

        case 3 : /* data packet */
	  {
	    cs_uint16  ether_type ;

	    if (ext_data_pkt) { /* Extended data packet */
	      CS_PRINT(
		"\tEthernet DA MAC address= %02x.%02x.%02x.%02x.%02x.%02x\n",
		buf[18], buf[19], buf[20], buf[21], buf[22], buf[23]
		) ;
	      CS_PRINT(
		"\tEthernet SA MAC address= %02x.%02x.%02x.%02x.%02x.%02x\n",
		buf[24], buf[25], buf[26], buf[27], buf[28], buf[29]
		) ;
	      ether_type = (buf[30] << 8) | buf[31] ;

	      if (ether_type == 0x8100) { /* vlan packet */
		CS_PRINT(
		  "\tVLAN Packet(user_priority= %d, vlan_id= 0x%03x)\n",
		  (buf[32] & 0xe0) >> 5,
		  (((buf[32] & 0x0f) << 8) | buf[33])
		  ) ;
		ether_type = (buf[34] << 8) | buf[35] ;
		payld_start = 36 ;
	      }
	      else { /* non-vlan packet */
		payld_start = 32 ;
	      }
	      CS_PRINT("\tEther Type= 0x%04x\n", ether_type) ;
	      payld_sz    = pkt_len - (payld_start + 4) ;
	    }

	    else { /* non-extended data packet */
	      CS_PRINT("Protocol Type= 0x%04x\n",
		       ((buf[18] << 8) | buf[19])) ;
	      payld_start = 20 ;
	      payld_sz    = pkt_len - (payld_start + 4) ;
	    }
	  }
	  break ;

        default : break ; /* not supported */
      } /* switch (pkt_type) */

      break ;

    default : break ;/* not supported */

  } /* switch (mac_mode) */

  hdr_done :
    CS_PRINT("\n\tpayload starts @ byte offset 0x%04x for 0x%04x bytes\n",
	     payld_start, payld_sz) ;
  CS_PRINT("\n\n") ;
  return ;
}


/****************************************************************/
/* $rtn_hdr_start CONTROL CLEARING OF RMAC MEMORIES AT INIT     */
/* CATEGORY   : Driver                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP	      : Milan2                                          */
void m2r_ctl_clear_rmac_mems(cs_ctl_t ctl)
/* INPUTS     : CS_ENABLE or CS_DISABLE                         */
/* OUTPUTS    : ----                                            */
/* RETURNS    : void                                            */
/* DESCRIPTION:                                                 */
/* This API can be used for controlling whether the memories    */
/* associated with the RingMAC get cleared or not during port   */
/* initialization.                                              */
/* Since the memories are parity protected, they should be      */
/* initialized before reading from them. If not, then memory    */
/* parity error interrupts may be detected during run-time.     */
/*                                                              */
/* By default, the driver's port initialization will initialize */
/* them by clearing.                                            */
/* For the purpose of speeding up initialization, this debug API*/
/* is provided to control this driver initialization of the     */
/* memories. This should be done only as a temporary debug      */
/* solution and is not recommended to bypass the clearing of    */
/* the memories.                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  clear_rmac_mems = (ctl == CS_ENABLE) ? TRUE : FALSE ;
}


cs_boolean m2r_rmac_mems_must_be_cleared(void)
{
  return(clear_rmac_mems) ;
}



