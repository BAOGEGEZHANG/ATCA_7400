/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_cfg.c
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

static
cs_status  m2r_rmac_loopback_init_cfg(cs_uint16 port_id, 
				      m2r_rmac_cfg_t * pCfg) ;
static
cs_status  m2r_rmac_set_txh_fifo_full_thold(cs_uint16 port_id, 
					    cs_uint16 full_thold) ;

/******************* RingMAC Configuration **********************/
/****************************************************************/
/* $rtn_hdr_start  RINGMAC INITIALIZATION                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_rmac_init_cfg(cs_uint16 port_id, 
			    m2r_port_summ_t * pSumm, 
			    m2r_rmac_cfg_t * pCfg) 
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port configuration summary struct  */
/* 		o Pointer to RMAC configuration structure       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the RMAC block of the specified port        */ 
/* with the given configuration.                                */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32		i ;
  DR_MICRO_IF_CR2_t 	cr2 ;
  DR_MATE_CR3_t         cr3 ;
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, ": RingMAC cfg\n") ;
    return(CS_ERROR) ;
  }
    
  if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }

  CS_PRINT("\t Initializing RINGMAC... \n") ;

  /* Unlock prot registers- and leave it that way after config */
  if ( m2r_rmac_lock(port_id, FALSE) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_set_mac_mode_sel(port_id, pSumm->mac_mode) 
       == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  if ( CS_IN_CUSTOMER_ENV() || CS_IN_LAB_ENV() ) {
    if ( m2r_rmac_mems_must_be_cleared() ||
	 !m2r_is_warm_start_in_progress() ) {  
      if ( m2r_rmac_init_mem(port_id) == CS_ERROR ) {
        return (CS_ERROR) ;
      }
    }
    else {
      CS_PRINT(
          "\n\t***NOTE***: RingMAC memory initialization SKIPPED!\n\n"
          ) ;
    }
  }

  if ( m2r_rmac_set_ring_id(port_id, pCfg->ring_id, pCfg->check_ring_id)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  
  /* Init configuration will always set it in PASSTHROUGH! */
  if ( m2r_rmac_set_ring_mode(port_id, M2R_RING_PASSTHROUGH)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_set_ttl_action(port_id, pCfg->ttl_action) 
       == CS_ERROR ) {
    return (CS_ERROR) ; 
  }

  if ( m2r_rmac_set_saa_mode(port_id, pCfg->saa_enbl) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if (pSumm->mac_mode == M2R_PORT_IN_SRP_MODE) { /* SRP only */
    if ( m2r_rmac_set_srp_prio_level(port_id, pCfg->srp_prio_level)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if ( m2r_rmac_strict_mode(port_id, pCfg->strict_mode)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) { /* RPR only */
    if ( m2r_rmac_hecmsb_mode(port_id, pCfg->hec_msb)
	   == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }
  
  if ( m2r_rmac_loopback_init_cfg(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /*******************
   *      TB         *
   *******************/  
  if ( m2r_rmac_set_tb_params(port_id, pCfg->tb.one_tb, 
			      pCfg->tb.stq_base, pCfg->tb.stq_sz, 
			      pCfg->tb.ptq_base, pCfg->tb.ptq_sz)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  if (!pCfg->tb.one_tb) {
    if ( m2r_rmac_ctl_stq_hthresh(port_id, pCfg->tb.stq_hthresh_ctl)
         == CS_ERROR ) {
      return (CS_ERROR) ;
    }
    if ( m2r_rmac_set_stq_hthresh_boundry(port_id, 
                                          pCfg->tb.stq_hthresh_boundry)
         == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }
  if ( m2r_rmac_set_ptq_thresh(port_id, pCfg->tb.ptq_thresh)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /***********************************
   * MATE                            *
   ***********************************/
  if ( m2r_rmac_set_flood_ctl(port_id, pCfg->mate.ft_enbl,
			      pCfg->mate.ft_type,
			      pCfg->mate.ft_ttl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* 
   * The from mate(FRM) FIFO underflow checking should be enabled *
   * only when XAUI reference clock is at nominal rate(156.25MHz) *
   */
  if ( m2r_rmac_ctl_from_mate_underflow_check(port_id, 
					pCfg->mate.frm_underflow_check)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /***********************************
   * MATE XGXS                       *
   ***********************************/
   m2r_rmac_xgxs_set_ref_clock(port_id, pCfg->mate.xgxs_refclk) ;
   if(pCfg->mate.xgxs_tx_rx_loopback) {
     m2r_rmac_xgxs_ctl_tx_rx_loopback(port_id, M2R_ENABLE) ;
   }

   if(pCfg->mate.xgxs_rx_tx_loopback) {
     m2r_rmac_xgxs_ctl_rx_tx_loopback(port_id, M2R_ENABLE) ;
   }
   m2r_rmac_xgxs_init_stats(port_id) ;

  /********************* 
   *****   HOST   ******
   *********************/
  /* For Milan2: need to set TXH Fifo FULL thold to 0x24 */
  if ( m2r_rmac_set_txh_fifo_full_thold(port_id, 0x24) 
       == CS_ERROR ) { 
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_set_num_host_channel(port_id, 
                pCfg->host.single_channel, pCfg->host.single_channel_id,
                pCfg->host.num_pri_channels )
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if (pSumm->mac_mode == M2R_PORT_IN_SRP_MODE) {
    if ( m2r_rmac_set_srp_host_prio_thresh(port_id, 
					   pCfg->host.srp_med_pri,
                                           pCfg->host.srp_hi_pri)
         == CS_ERROR ) {
      return(CS_ERROR) ;
    }
  }

  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) { /* RPR only */
    m2r_ctl_t 	ctl = ( (pCfg->host.oam_on_ch0) ? M2R_ENABLE 
			: M2R_DISABLE ) ;

    if ( m2r_rmac_ctl_rpr_oam_on_ch0(port_id, ctl) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if ( m2r_rmac_host_crc_strip_or_insert(port_id, 
					 pCfg->host.rx_crc_strip,
					 pCfg->host.tx_crc_insert)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) { /* RPR only */
    if ( m2r_rmac_accept_wpkts_on_wrong_ri(port_id, 
			 pCfg->host.accept_wrpd_ucast_on_wrong_ri,
			 pCfg->host.accept_wrpd_mcast_on_wrong_ri)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    if ( m2r_rmac_accept_flooded_data_pkts(port_id, 
					pCfg->host.accept_flooded_pkts)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if ( m2r_rmac_ctl_hdr_strip(port_id, pCfg->host.hdr_strip_sel,
			      pCfg->host.hdr_strip_ppp_enable)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_hdr_strip_ctl_new_pkt_type(port_id, 
		      pCfg->host.hdr_strip_new_pkt_enable,
		      pCfg->host.hdr_strip_new_pkt_mode,
		      pCfg->host.hdr_strip_new_pkt_protocol)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_host_ctl_ri(port_id, pCfg->host.ctl_ri) 
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_host_set_pkt_size_filters(port_id, 
                                          pCfg->host.rxh_giant_sz, 
					  pCfg->host.txh_runt_sz, 
					  pCfg->host.txh_giant_sz )
       == CS_ERROR ) {
    return (CS_ERROR) ; 
  }

  /*
   * Host sourced packets will always get WS bit set to 0(unless
   * the MAC is in WRAP mode).
   */
  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) {
    m2r_rmac_ctl_ws_for_pkts_from_host(port_id, TRUE, 0) ;
  }
  
  /************************
   *  Framer              *
   ************************/
  if ( m2r_rmac_set_rxf_pkt_size_filters(port_id, 
					 pCfg->framer.rxf_runt_sz, 
                                         pCfg->framer.rxf_giant_sz)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  if ( m2r_rmac_ctl_rxf_stomp(port_id, pCfg->framer.stomp_ctl,
                              pCfg->framer.err_stomp_host_flag)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  
  /************************
   * IPS/CONTROL PACKETS  *
   ************************/
  /*
   * By default, ENABLE the Rx buffers(to CPU) for IPS and CTL. The
   * destination of these packets(Host vs cpu) can configured using
   * default destination control bits and Control CAM setting.
   */
  if ( m2r_rmac_enable_ips_ctl_pkt_rxbuf(port_id, TRUE, TRUE)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_set_ips_ctl_pkt_rx_default_dstn(port_id,
			    pCfg->ips_ctl.ips_buf_default_destn,
                            pCfg->ips_ctl.ctl_buf_default_destn)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  for (i = 0; i < 4; i++) {
    if ( m2r_rmac_add_ips_ctl_cam_entry(port_id, i, 
				   pCfg->ips_ctl.ctl_cam[i].dec_mode,
				   pCfg->ips_ctl.ctl_cam[i].ctl_mask,
				   pCfg->ips_ctl.ctl_cam[i].ctl_type)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /* IPS in TTL based mode - for RPR only */
  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) {
    if ( m2r_rmac_enable_ips_ttl_mode(port_id, 
				      pCfg->ips_ctl.ips_ttl_mode)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    if ( m2r_rmac_set_ips_ttl_masks(port_id, 
				    pCfg->ips_ctl.ips_ttl_0_mask,
                                    pCfg->ips_ctl.ips_ttl_max_mask,
                                    pCfg->ips_ctl.ips_ttl_max_stations)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }

    if ( m2r_rmac_set_ips_timer(port_id, pCfg->ips_ctl.ips_ttl_tmr_cnt)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /************************
   *         CAM          *
   ************************/
  if ( m2r_rmac_cam_init_cfg(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /************************
   *     MAC RE-WRITE     *
   ************************/
  m2r_rmac_ctl_rewrite(port_id,
               ((pCfg->mrw_enabled == TRUE) ? M2R_ENABLE 
		: M2R_DISABLE)) ;

  /*
   * Done with configuring the registers. SRESET(logic reset) the
   * block.
   */
  if (m2r_rmac_soft_reset(port_id, M2R_RMAC_SRESET_ALL, 
                           M2R_RESET_TOGGLE) 
       == CS_ERROR) {
    return (CS_ERROR) ;
  }

  /************************
   *     Micro-sequencer  *
   ************************/
  if ( m2r_ms_init_cfg(port_id, pSumm, &pCfg->ms) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /************************
   *  IDLE PACKET CONTROL *
   ************************/ 
  if ( m2r_rmac_debug_write_tx_idle_pkt(port_id)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_set_tx_idle_timers(port_id, 
				   pCfg->idle_pkt.idle_slow_tmr_cnt, 
				   pCfg->idle_pkt.idle_fast_tmr_cnt)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_rmac_set_tx_idle_thold(port_id, pCfg->idle_pkt.idle_thold) 
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Init RingMAC default IRQ */
  m2r_rmac_init_irq(port_id, pCfg) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  /* Enable RingMAC dummy interrupt bit */
  cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;
  cr2.bf.IRQ_ENB = 1 ; 
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR2_.wrd, cr2.wrd) ;

  /* Turn on [WS_ALL] bit to treat control packets the same as data packets */
  /* in WRAP mode per RPR standard.                                         */
  cr3.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR3_.wrd) ;
  cr3.bf.WS_ALL = 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR3_.wrd, cr3.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  /* Initialize statistics */
  if ( m2r_rmac_init_stats(port_id, pSumm->mac_mode) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET DEFAULT RINGMAC CONFIGURATION           	*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_rmac_get_default_cfg(cs_uint16 port_id, 
				   m2r_port_summ_t * pSumm,
				   m2r_rmac_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/* 	        o Pointer to RMAC configuration structure       */
/* OUTPUTS    : o RingMAC default config                        */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the driver's default RMAC configuration for the          */
/* specified port and for the specified summary config info.    */
/*                                                              */
/* The [pSumm] parameter is a pointer to the configuration      */
/* Summary section, which has the caller provided config summary*/
/* info. This is used in returning the appropriate default      */
/* configuration for the RingMAC block.                         */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure. The API will fill in this structure with     */
/* default settings based on the config summary info.           */
/*                                                              */
/* The API expects both datastrutues(pSumm and pCfg) already    */
/* allocated, and the config summary filled in pSumm.           */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	i ;
  cs_uint32	tb_sz, port_factor ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, ": RingMAC cfg\n") ;
    return(CS_ERROR) ;
  }
    
  if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }

  /* Even ports on Ringlet 1, and odd ports on Ringlet 0 */
  pCfg->ring_id        = (port_id % 2) ? 0 : 1 ;
  pCfg->check_ring_id  = TRUE ; 
  pCfg->ttl_action     = M2R_DECR_TTL_RINGID_OR_NODE_WRAP ;
  pCfg->saa_enbl       = FALSE ;
  pCfg->srp_prio_level = 4 ;
  pCfg->strict_mode    = FALSE ;
  /* In the RPR standard, HEC should be calced in LSB.
   * But the current IXIA is still using the wrong one,
   * we need to set HEC_MSB as FALSE to satisfy diag
   */
  pCfg->hec_msb        = FALSE ;

  /* Various loopbacks in RingMAC */
  pCfg->txh_to_rxh_lpbk  = FALSE ;
  pCfg->txf_to_rxf_lpbk  = FALSE ;
  pCfg->rxf_to_mate_lpbk = FALSE ;
  pCfg->rxf_to_host_lpbk = FALSE ;
  pCfg->tom_to_frm_lpbk  = FALSE ;
  pCfg->frm_to_tom_lpbk  = FALSE ;


  /*************  TB  **************/
  tb_sz                  = 0x00100000 ;  /* 1 MB */
  port_factor		 = 0x0 ;

  if ((pSumm->rmac_ms_mode != M2R_MS_RPR_AGGRESSIVE_SINGLE_TB)
     && (pSumm->rmac_ms_mode != M2R_MS_RPR_CONSERVATIVE_SINGLE_TB)) {

    pCfg->tb.one_tb        = FALSE ; 
    /* 
     * Configure PTQ and STQ sizes in the ratio of 
     * PTQ : STQ = 128 : (4K - 128). 
     */
    pCfg->tb.stq_base      = (0x0 | port_factor)  ;
    pCfg->tb.stq_sz        = ( (M2R_NUM_OF_256B_WORDS_IN_TB(tb_sz) *
			        (4096-128)/4096) - 1 ) ;

    pCfg->tb.ptq_base      = pCfg->tb.stq_base + pCfg->tb.stq_sz + 1 ; 
    pCfg->tb.ptq_sz        =  ( (M2R_NUM_OF_256B_WORDS_IN_TB(tb_sz) *
			         128/4096) - 1 ) ;
  
    pCfg->tb.stq_hthresh_ctl = M2R_ENABLE ;
    pCfg->tb.stq_hthresh_boundry = M2R_RMAC_STQ_HTHRESH_BOUNDRY_DEFAULT ;

  } else {
    pCfg->tb.one_tb        = TRUE ;
    pCfg->tb.stq_base      = 0x0 ;
    pCfg->tb.stq_sz        = 0 ;
    pCfg->tb.ptq_base      = (0x0 | port_factor) ;
    pCfg->tb.ptq_sz        = M2R_NUM_OF_256B_WORDS_IN_TB(tb_sz) - 1 ;

    pCfg->tb.stq_hthresh_ctl = M2R_DISABLE ;
    pCfg->tb.stq_hthresh_boundry = 0 ;
  }

  pCfg->tb.ptq_thresh = M2R_RMAC_PTQ_THRESH_DFT(pCfg->tb.ptq_sz) ;

  /************* MS ****************/
  if ( m2r_ms_get_default_cfg( port_id, pSumm, &pCfg->tb, 
                               &pCfg->ms) == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  /************** Mac re-write **********/
  pCfg->mrw_enabled = FALSE ;

  /******        MATE           *****/
  pCfg->mate.ft_enbl = FALSE ; /* assume flood bits are set externally*/
  pCfg->mate.ft_type = M2R_RMAC_FLOOD_TYPE_UNI_DIR ;
  pCfg->mate.ft_ttl = 255 ;
  /* 
   * The from mate(FRM) FIFO underflow checking should be enabled *
   * only when XAUI reference clock is at nominal rate(156.25MHz) *
   */
  pCfg->mate.frm_underflow_check = FALSE ;

  /* MATE XGXS */
  pCfg->mate.xgxs_rx_tx_loopback = FALSE ;
  pCfg->mate.xgxs_tx_rx_loopback = FALSE ;
  pCfg->mate.xgxs_refclk  = M2R_XGXS_XAUI_REF_CLK_156 ;

  /****** IDLE PACKET CONTROL *******/
  pCfg->idle_pkt.idle_slow_tmr_cnt = 0 ;	/* disabled */
  pCfg->idle_pkt.idle_fast_tmr_cnt = 0 ;	/* disabled */
  pCfg->idle_pkt.idle_thold        = 0 ; 

  /************* HOST ***************/
  pCfg->host.single_channel = FALSE ;
  pCfg->host.num_pri_channels = 4 ;
  pCfg->host.single_channel_id = 0 ;        /* doesn't matter */
  if (pSumm->mac_mode == M2R_PORT_IN_SRP_MODE) {
    pCfg->host.srp_med_pri = 4 ;
    pCfg->host.srp_hi_pri = 5 ;
  }

  pCfg->host.oam_on_ch0     = TRUE ; /* OAM ctl pkts to ch0 - #801 */

  pCfg->host.rx_crc_strip   = TRUE ;
  pCfg->host.tx_crc_insert  = TRUE ;
  pCfg->host.accept_wrpd_ucast_on_wrong_ri  = TRUE ;
  pCfg->host.accept_wrpd_mcast_on_wrong_ri  = TRUE  ;
  pCfg->host.accept_flooded_pkts = FALSE ;

  /* MAC header stripping for Rx packets to host */
  pCfg->host.hdr_strip_sel  = M2R_RMAC_HDR_STRIP_DISABLED ;
  pCfg->host.hdr_strip_ppp_enable     = FALSE ;
  pCfg->host.hdr_strip_new_pkt_enable = FALSE ;
  pCfg->host.hdr_strip_new_pkt_mode   = 0 ;
  pCfg->host.hdr_strip_new_pkt_protocol = 0x0000 ;

  pCfg->host.ctl_ri         = M2R_RMAC_RI_INSERT_N_WFLIP ;

  pCfg->host.rxh_giant_sz   = (9 * 1024) ;  
  pCfg->host.txh_runt_sz    = 0 ;
  pCfg->host.txh_giant_sz   = (9 * 1024) ;  
  
  /************ FRAMER *************/
  pCfg->framer.rxf_runt_sz    = 23 ; /* data packets only */
  pCfg->framer.rxf_giant_sz   = (9 * 1024) ;
  pCfg->framer.stomp_ctl = M2R_DISABLE ;
  pCfg->framer.err_stomp_host_flag = FALSE ;
  
  /************ IPS/CTL *************/
  pCfg->ips_ctl.ips_buf_default_destn = TRUE ;
  pCfg->ips_ctl.ctl_buf_default_destn = FALSE ;
 
  for (i = 0; i < 4; i++) {
    pCfg->ips_ctl.ctl_cam[i].dec_mode = 0 ;
    pCfg->ips_ctl.ctl_cam[i].ctl_mask = 0 ;
    pCfg->ips_ctl.ctl_cam[i].ctl_type = 0 ;
  }
 
  /* IPS in TTL based mode (RPR only) */
  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) {
    pCfg->ips_ctl.ips_ttl_mode        = TRUE ;
  }
  else {
    pCfg->ips_ctl.ips_ttl_mode        = FALSE ;
  }
    
  pCfg->ips_ctl.ips_ttl_0_mask        = TRUE ;
  pCfg->ips_ctl.ips_ttl_max_mask      = TRUE ;
  pCfg->ips_ctl.ips_ttl_max_stations  = 0xff ;
  pCfg->ips_ctl.ips_ttl_tmr_cnt       = 0 ;


  /*        CAM                 */

  /* Set up two lookup(Rx) CAM entries */
  {
    m2r_rx_cam_entry_t 	* p ;
    
    pCfg->cam.rx_cam_start  = 0 ;
    pCfg->cam.rx_cam_num    = 2 ;

    /* entry 0: SA entry with my MAC address */
    p = &pCfg->cam.rx_cam_tbl[0] ;

    p->macaddr.da  = FALSE ;
    p->macaddr.sa  = TRUE ;
    p->macaddr.mac = pCfg->ms.my_sa ;
    p->status.sr   = (pCfg->saa_enbl) ? TRUE : FALSE ; /*always accept*/
    p->status.ne   = TRUE ;
    p->status.sam  = TRUE ;
    p->status.dah  = FALSE ;
    p->status.hop_count = 255 ;
    p->prsrv_da_cntr = p->prsrv_sa_cntr = FALSE ;

    /* entry 1: DA entry with my MAC address */    
    p = &pCfg->cam.rx_cam_tbl[1] ;

    p->macaddr.da  = TRUE ;
    p->macaddr.sa  = FALSE ;
    p->macaddr.mac = pCfg->ms.my_sa ;
    p->status.sr   = FALSE ;
    p->status.ne   = FALSE ;
    p->status.sam  = FALSE ;
    p->status.dah  = TRUE  ; 
    p->status.hop_count = 0 ;
    p->prsrv_da_cntr = p->prsrv_sa_cntr = FALSE ;
  }

  /* Malloc memory for driver to maintain acctg sram stats */
  pCfg->cam.acctg_stats_mem_malloced = TRUE ; 

  /* Make all multi-cast entries invalid */
  for (i = 0; i < 8; i++) {
    pCfg->cam.mcast_cam[i].valid = FALSE ;
  }

  /* Get the Default IRQ Configuration */
  m2r_rmac_get_default_irq_cfg(port_id, pSumm, pCfg) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET RMAC RUNNING CONFIGURATION               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_rmac_get_running_cfg(cs_uint16 port_id,
				   m2r_port_summ_t * pRunSumm,
				   m2r_rmac_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/*		o Pointer to RMAC config structure              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time RMAC hardware configuration     */ 
/* of the specified port.                                       */
/*                                                              */
/* The [pRunSumm] parameter is a pointer to the running config  */
/* summary section.                                             */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the block's          */
/* configuration data-structure.                                */
/*                                                              */
/* This API will fill in the appropriate fields in the          */
/* configuration summary section and all of RingMAC             */
/* run-time configuration.                                      */
/* The caller is responsible for allocating memory for the      */
/* configuration structures.                                    */
/* $rtn_hdr_end							*/
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  if ( (pRunSumm == NULL) || (pRunCfg == NULL) ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_INVALID_USER_ARG,
                   "pRunSumm = 0x%x, pRunCfg = 0x%x\n",
                   pRunSumm, pRunCfg) ;
    return (CS_ERROR) ;
  } 
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* General */
  {
    DR_MICRO_IF_CR2_t     cr2 ;
    
    cr2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ; 
    pRunCfg->ring_id = cr2.bf.RING_ID ;
    pRunCfg->saa_enbl = cr2.bf.SAA_MODE ;
    pRunCfg->ring_mode = m2r_rmac_get_ring_mode(port_id) ;
    m2r_ms_get_mac_sa(port_id, &pRunCfg->my_sa) ;
  }

  /* TB */
  {
    DR_MICRO_IF_CR0_t     cr0 ; 

    cr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

    pRunCfg->one_tb = cr0.bf.ONETB ;
    pRunCfg->stq_base = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.STQ_BASE.wrd) ;
    pRunCfg->stq_sz =
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.STQ_MAX.wrd) ;
    pRunCfg->ptq_base =
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.PTQ_BASE.wrd) ;
    pRunCfg->ptq_sz = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.PTQ_MAX.wrd) ;
  }

  /* Host */
  {
    DR_MICRO_IF_CR1_t  cr1 ;
    DR_HOST_CR4_t      cr4 ;
    DR_HOST_CR4A_t     cr4a ;

    cr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
    cr4.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4.wrd) ;
    cr4a.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR4A.wrd) ;

    pRunCfg->mrw_enabled = (cr1.bf.MAC_REWRITE) ? TRUE : FALSE ;    
    pRunCfg->rxh_giant_sz =
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RXH_GIANTSZ.wrd) ;
    pRunCfg->txh_runt_sz =
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TXH_RUNTSZ.wrd) ;
    pRunCfg->txh_giant_sz = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TXH_GIANTSZ.wrd) ; 
    pRunCfg->rxh_chan_mode = cr4a.bf.HOSTNUMCHAN ;
    pRunCfg->rxh_chan_pri_en = cr4a.bf.HOSTPRIEN ;
    pRunCfg->rxh_chan_sel = cr4.bf.RXH_CHANNEL ;
  }

  /* Framer */
  {
    DR_FRAMER_CR5_t       cr5 ;

    pRunCfg->rxf_runt_sz = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RXF_RUNTSZ.wrd) ;
    pRunCfg->rxf_giant_sz = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RXF_GIANTSZ.wrd) ;
    cr5.wrd = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR5.wrd) ;
    pRunCfg->stomp_ctl = (cr5.bf.STOMP) ? TRUE : FALSE ;
  }

  /* IPS/CTL */
  {
    DR_TBD_IPS_CTL_CR_t   cr ;
    DR_TBD_STATIONS_t     stations ;

    cr.wrd = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IPS_CTL_CR.wrd) ;
    pRunCfg->ips_buf_default_destn = 
      (cr.bf.IPS_RCV_DEFAULT_DEST) ? TRUE : FALSE ;
    pRunCfg->ctl_buf_default_destn = 
      (cr.bf.CTL_RCV_DEFAULT_DEST) ? TRUE : FALSE ;
    pRunCfg->ips_ttl_mode = 
      (cr.bf.TTL_MODE) ? TRUE : FALSE ;
    stations.wrd = 
      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.STATIONS.wrd) ;
    pRunCfg->ips_ttl_max_mask =
      (stations.bf.MASK_MAX) ? TRUE : FALSE ;
    pRunCfg->ips_ttl_max_stations = stations.bf.MAX ;
  }

  /* Mate */
  {
    XGXS_MP_MnCntrl0_t   xgxs_ctrl ;

    xgxs_ctrl.wrd = CS_REG_2READ(&pDev->XGXS.MnCntrl0.wrd) ;
    pRunCfg->xgxs_tx_rx_loopback =
      (xgxs_ctrl.bf.xgxs_tx_rx_lb) ? TRUE : FALSE ;
    pRunCfg->xgxs_rx_tx_loopback =
      (xgxs_ctrl.bf.xgxs_rx_tx_lb) ? TRUE : FALSE ;

    pRunCfg->xgxs_alignment_detected = 
      m2r_rmac_xgxs_is_align_detected(port_id) ;
  }

  /* Loopbacks */
  {
    DR_MICRO_IF_DIAG_CR_t cr ;
    
    cr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DIAG_CR.wrd) ;
    pRunCfg->txh_to_rxh_lpbk =
      (cr.bf.TXH_TO_RXH) ? TRUE : FALSE ;
    pRunCfg->txf_to_rxf_lpbk =
      (cr.bf.TXF_TO_RXF) ? TRUE : FALSE ;
    pRunCfg->rxf_to_mate_lpbk =
      (cr.bf.RXF_TO_TOM) ? TRUE : FALSE ;
    pRunCfg->rxf_to_host_lpbk =
      (cr.bf.RXF_TO_RXH) ? TRUE : FALSE ;
    pRunCfg->tom_to_frm_lpbk =
      (cr.bf.TOM_TO_FRM) ? TRUE : FALSE ;
    pRunCfg->frm_to_tom_lpbk =
      (cr.bf.FRM_TO_TOM) ? TRUE : FALSE ;
  }

  /* Micro-Sequencer */
  if ( m2r_ms_get_running_cfg(port_id, &pRunCfg->ms)
       != CS_OK ) {
    return (CS_ERROR) ;
  } 

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP RINGMAC RUNNING CONFIGURATION           */     
/* CATEGORY   : Port                                            */     
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_dump_running_cfg(cs_uint16 port_id,
				    m2r_rmac_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*		o Pointer to RMAC config structure              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Prints the current run-time hardware configuration for the   */
/* this block.                                                  */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the running          */
/* configuration data-structure allocated by the caller with    */
/* the configuration data from HW already retrieved and filled  */
/* in, prior to calling this API.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( pRunCfg == NULL ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  CS_PRINT("Ring(let) Id           = %d\n", 
	   pRunCfg->ring_id) ;
  CS_PRINT("Ring mode              = %s\n",
	   M2R_GET_RING_MODE_NAME(pRunCfg->ring_mode)) ;
  CS_PRINT("My Mac Address         = %.2x.%.2x.%.2x.%.2x.%.2x.%.2x\n",
       pRunCfg->my_sa.byte5, pRunCfg->my_sa.byte4,
       pRunCfg->my_sa.byte3, pRunCfg->my_sa.byte2,
       pRunCfg->my_sa.byte1, pRunCfg->my_sa.byte0) ;
  
  /******* TB ********/
  CS_PRINT("\n\t *** Transit Buffer ***\n") ;
  CS_PRINT("Type                   = %s\n",
	   (pRunCfg->one_tb) ? "Single" : "Dual") ;
  CS_PRINT("PTQ Base               = 0x%04x (256-byte boundary)\n",
	   pRunCfg->ptq_base) ;
  CS_PRINT("PTQ Size               = 0x%04x (or 0x%04x bytes)\n",
	   pRunCfg->ptq_sz, (pRunCfg->ptq_sz + 1) * 256) ; 

  if (!pRunCfg->one_tb) {
    CS_PRINT("STQ Base               = 0x%04x (256-byte boundary)\n",
	     pRunCfg->stq_base) ;
    CS_PRINT("STQ Size               = 0x%04x (or 0x%04x bytes)\n",
	     pRunCfg->stq_sz, (pRunCfg->stq_sz + 1) * 256) ;
  }

  /******* Host ********/
  CS_PRINT("\n\t *** Host ***\n") ;

  CS_PRINT("MAC Rewrite mode       = %s\n", 
	   (pRunCfg->mrw_enabled) ? "ENABLED" : "Disabled") ;
  CS_PRINT("RXH Giant pkt filter   = 0x%x bytes\n", 
	   pRunCfg->rxh_giant_sz) ;
  CS_PRINT("TXH Runt pkt filter    = 0x%x bytes\n", 
	   pRunCfg->txh_runt_sz) ;
  CS_PRINT("TXH Giant pkt filter   = 0x%x bytes\n",
	   pRunCfg->txh_giant_sz) ;
  CS_PRINT("RXH Channel mode       = %d ",	   
	   pRunCfg->rxh_chan_mode + 1) ;
  switch (pRunCfg->rxh_chan_mode) {
    default : 
    case 0 : 
      CS_PRINT("(chan0: all pkts)\n") ; break ;
    case 1 : 
      CS_PRINT("(chan0: CTL, chan1: DATA)\n") ; break ;
    case 2 : 
      CS_PRINT("(chan0: CTL & Hi, chan1: Med, chan2: Low)\n") ; break ;
    case 3:
      CS_PRINT("(chan0: CTL, chan1: Hi, chan2: Med, chan3: Low)\n") ; 
      break ;
  }
  CS_PRINT("RXH Chan prio enable   = %d ", pRunCfg->rxh_chan_pri_en) ;
  if (!pRunCfg->rxh_chan_pri_en) {
    CS_PRINT("(chan select: %d)", pRunCfg->rxh_chan_sel) ;
  }
  CS_PRINT("\n") ;

  /******* Framer ********/
  CS_PRINT("\n\t *** Framer ***\n") ;
  CS_PRINT("RXF Runt pkt filter    = 0x%x bytes\n", 
	   pRunCfg->rxf_runt_sz) ;
  CS_PRINT("Giant pkt size         = 0x%x bytes\n",
	    pRunCfg->rxf_giant_sz) ;
  CS_PRINT("CRC Stomp data pkts    = %s\n", 
	   (pRunCfg->stomp_ctl) ? "Enabled" : "Disabled") ;

  /******* IPS/CTL *******/
  CS_PRINT("\n\t *** IPS/CTL ***\n") ;
  CS_PRINT("CTL pkts default dest  = %s\n",
	   (pRunCfg->ctl_buf_default_destn) ? "CPU Buffer" :
	   "Host") ;
  CS_PRINT("IPS pkts default dest  = %s\n", 
	   (pRunCfg->ips_buf_default_destn) ? "CPU Buffer" :
	   "Host") ;
  CS_PRINT("IPS TTL-based mode     = %s\n", 
	   (pRunCfg->ips_ttl_mode) ? "Enabled" : "Disabled") ;
  CS_PRINT("IPS TTL-Max check      = %s (Max stations= 0x%x)\n",
	   (pRunCfg->ips_ttl_max_mask) ? "Enabled": "Disabled",
	   pRunCfg->ips_ttl_max_stations) ;

  /******* Loopbacks *****/
  CS_PRINT("\n\t *** Loopbacks ***\n") ;
  CS_PRINT("RMAC TXH to RXH LB     = %s\n",
           (pRunCfg->txh_to_rxh_lpbk) ? "ON" : "OFF") ;
  CS_PRINT("RMAC TXF to RXF LB     = %s\n",
           (pRunCfg->txf_to_rxf_lpbk) ? "ON" : "OFF") ;
  CS_PRINT("RMAC RXF to MATE LB    = %s\n",
           (pRunCfg->rxf_to_mate_lpbk) ? "ON" : "OFF") ;
  CS_PRINT("RMAC RXF to HOST LB    = %s\n",
           (pRunCfg->rxf_to_host_lpbk) ? "ON" : "OFF") ;
  CS_PRINT("RMAC TOM to FRM LB     = %s\n",
           (pRunCfg->tom_to_frm_lpbk) ? "ON" : "OFF") ;
  CS_PRINT("RMAC FRM to TOM LB     = %s\n",
           (pRunCfg->frm_to_tom_lpbk) ? "ON" : "OFF") ;

  /******* Mate **********/
  CS_PRINT("\n\t *** Mate ***\n") ;
  CS_PRINT("XGXS Interface status  = %s\n",
	   (pRunCfg->xgxs_alignment_detected) ?
	   "in-sync(aligned)" : "no sync(un-aligned)!!") ;
  CS_PRINT("Mate XGXS Tx to Rx LB  = %s\n",
           (pRunCfg->xgxs_tx_rx_loopback) ? "ON" : "OFF") ;
  CS_PRINT("Mate XGXS Rx to Tx LB  = %s\n",
           (pRunCfg->xgxs_rx_tx_loopback) ? "ON" : "OFF") ;

  /* Micro-Sequencer */
  if ( m2r_ms_dump_running_cfg(port_id, pRunCfg)
       != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
} 


/********************* RingMAC Private APIs ************************/

/*****************************************************************
 * Init configuration for RingMAC loopbacks.                     *
 * This is PRIVATE                                               *
 *****************************************************************/
static
cs_status  m2r_rmac_loopback_init_cfg(cs_uint16 port_id, 
				      m2r_rmac_cfg_t * pCfg)
{
  if ( m2r_rmac_loopback(port_id, M2R_TXH_TO_RXH, pCfg->txh_to_rxh_lpbk)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  if ( m2r_rmac_loopback(port_id, M2R_TXF_TO_RXF, pCfg->txf_to_rxf_lpbk)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  if ( m2r_rmac_loopback(port_id, M2R_RXF_TO_MATE, pCfg->rxf_to_mate_lpbk)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  if ( m2r_rmac_loopback(port_id, M2R_RXF_TO_HOST, pCfg->rxf_to_host_lpbk)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  if ( m2r_rmac_loopback(port_id, M2R_TOM_TO_FRM, pCfg->tom_to_frm_lpbk)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  if ( m2r_rmac_loopback(port_id, M2R_FRM_TO_TOM, pCfg->frm_to_tom_lpbk)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/******************************************************************
 * Set TXH FIFO threshold to assert "FULL" condition backpressure *
 * to SPI42 from sending more data into RingMAC.                  *
 * For Milan2: The default value is 0x28, but it is better to give*
 * more margin for SPI4.2 to react to the backpressure. So, lower *
 * the threshold to 0x24 so that the full signal is sent earlier. *
 ******************************************************************/
static
cs_status  m2r_rmac_set_txh_fifo_full_thold(cs_uint16 port_id, 
					    cs_uint16 full_thold)
{
  cs_uint16		dev_num, sl_num ;
  M2R_t         	* pDev ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TXH_FULL_ASSERT_TH.wrd,
	       full_thold) ;

  return(CS_OK) ;
}
