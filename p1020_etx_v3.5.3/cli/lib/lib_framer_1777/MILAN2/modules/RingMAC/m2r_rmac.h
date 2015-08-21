/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_RMAC_H__
#define __M2R_RMAC_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_rmac_ms.h"
#include "m2r_rmac_mrw.h"

typedef enum {
  M2R_DECR_TTL_ALWAYS       = 0,
  M2R_DECR_TTL_RINGID,
  M2R_DECR_TTL_RINGID_OR_NODE_WRAP,
  M2R_TTL_UNCHANGED
} m2r_ttl_action_t ;

typedef enum {
  M2R_TRAFFIC_PRIO_LOW	= 0,
  M2R_TRAFFIC_PRIO_MED,
  M2R_TRAFFIC_PRIO_HI,
  M2R_TRAFFIC_PRIO_A0, /* RPR hi-priority reserved */
  M2R_TRAFFIC_PRIO_CTL,
  M2R_TRAFFIC_PRIO_HIRES /* RPR unreserved data traffic */
} m2r_prio_traffic_t ;

typedef enum {
  M2R_SERVICE_CLASS_A  = 0,
  M2R_SERVICE_CLASS_B,
  M2R_SERVICE_CLASS_C
} m2r_service_class_t ;

typedef enum {
  M2R_CTL_A0_CREDIT = 0, /* ctl packet also takes A0 credit  */
  M2R_CTL_CLASS_CREDIT,  /* ctl packet also takes associated *
                          * service class credit             */
  M2R_CTL_ONLY           /* ctl packet only takes control    *
                          * rate limiter credit              */
} m2r_ctl_rate_limiter_mode_t ;

typedef enum {
  M2R_CTL_PKT_RPR_CT_STATION_ATD    = 0x01,
  M2R_CTL_PKT_SRP_CT_TOPO           = 0x01,
  M2R_CTL_PKT_RPR_CT_TOPO_PROT      = 0x02,
  M2R_CTL_PKT_SRP_CT_IPS            = 0x02,
  M2R_CTL_PKT_RPR_CT_TOPO_CHKSUM    = 0x03,
  M2R_CTL_PKT_RPR_CT_LRTT_REQ       = 0x04,
  M2R_CTL_PKT_RPR_CT_LRTT_RSP       = 0x05,
  M2R_CTL_PKT_RPR_CT_FDD            = 0x06,
  M2R_CTL_PKT_RPR_CT_OAM_ECHO_REQ   = 0x07,
  M2R_CTL_PKT_RPR_CT_OAM_ECHO_RSP   = 0x08,
  M2R_CTL_PKT_RPR_CT_OAM_FLUSH      = 0x09,
  M2R_CTL_PKT_RPR_CT_OAM_ORG        = 0x0A
} m2r_ctl_pkt_ct_t ;

typedef enum {
  M2R_TXH_TO_RXH = 0, /* Host side loopback */
  M2R_TXF_TO_RXF,     /* Line side loopback */
  M2R_RXF_TO_MATE,    /* RXF to Mate - all line traffic to mate */
  M2R_RXF_TO_HOST,    /* RXF to Host - all line traffic to host */
  M2R_TOM_TO_FRM,     /* All traffic going to mate looped back  */
  M2R_FRM_TO_TOM      /* All traffic from mate looped back      */
} m2r_rmac_lpbk_sel_t ;

typedef struct {
  cs_uint8      dec_mode ;
  cs_uint16     ctl_mask ;
  cs_uint16     ctl_type ;
} m2r_ctl_cam_entry_t ;

typedef enum {
  M2R_RING_NORMAL         = 0,
  M2R_RING_PASSTHROUGH,
  M2R_RING_WRAP,
  M2R_RING_TRANSPARENT,
  M2R_RING_SNIFFER_PASSIVE,
  M2R_RING_SNIFFER_ACTIVE
} m2r_ring_mode_t ;

typedef enum {
  M2R_FLUSH_WRONG_RI = 0,
  M2R_FLUSH_STRICT,
  M2R_FLUSH_ALL
} m2r_flush_type_t ;

typedef struct {
  cs_mac	mac_sa ;
  cs_uint32     payload ;
} m2r_ips_new_msg_t ;

typedef enum {
  M2R_RMAC_HDR_STRIP_DISABLED           = 0,
  M2R_RMAC_HDR_STRIP_AND_ENCAP,
  M2R_RMAC_HDR_STRIP_NO_ENCAP
} m2r_rmac_hdr_strip_sel_t ;

typedef enum {
  M2R_RMAC_RI_INSERT_N_WFLIP	= 0, /* RI insert, and flip it in Wrap */
  M2R_RMAC_RI_WFLIP,                 /* Flip existing RI when in Wrap  */
  M2R_RMAC_RI_INSERT,	             /* RI inserted always             */
  M2R_RMAC_RI_UNCHANGED	             /* RI is untouched always         */
} m2r_rmac_host_ctl_ri_t ;

typedef enum {
  M2R_RMAC_FLOOD_TYPE_NONE    = 0,
  M2R_RMAC_FLOOD_TYPE_UNI_DIR = 1,
  M2R_RMAC_FLOOD_TYPE_BI_DIR  = 2,
  M2R_RMAC_FLOOD_TYPE_RSRVD   = 3
} m2r_rmac_flood_type_t ;


/***** RingMAC configuration data structure ****/

/************ RINGMAC MATE ******************/
typedef struct {
  cs_boolean ft_enbl ;
  m2r_rmac_flood_type_t ft_type ;
  cs_uint8   ft_ttl ;
  cs_boolean frm_underflow_check ;

  m2r_xgxs_xaui_ref_clk xgxs_refclk ;
  cs_boolean    xgxs_tx_rx_loopback ;
  cs_boolean    xgxs_rx_tx_loopback ;
  cs_uint16     xgxs_irq_ctl_tbl[8] ;

} m2r_rmac_mate_cfg_t ;

/************ IDLE PACKET CONTROL  ******************/
typedef struct {
  /* Slow timer count for Idle packet generation */
  cs_uint32	idle_slow_tmr_cnt ;
  /* Fast timer count for Idle packet generation */
  cs_uint32	idle_fast_tmr_cnt ;
  /* idle_thold: when HiTB depth exceeds this, SLOW timer is used */
  cs_uint16	idle_thold ;
} m2r_rmac_idle_pkt_cfg_t ;

/************* HOST ***************/
typedef struct {
  /* single_channel indicates if using only 1 specific host channel*/
  cs_boolean    single_channel;
  /* single_channel_id specifies selected single host channel id*/
  cs_uint8      single_channel_id;
  /* num_pri_channels specifies the number of receiving host priority
     channels */
  cs_uint8      num_pri_channels ;
  /* srp_med_pri specifies the medium packet priority threshold
     when mapping to host channel in SRP mode only */
  cs_uint8      srp_med_pri ;
  /* srp_med_pri specifies the high packet priority threshold
     when mapping to host channel in SRP mode only */
  cs_uint8      srp_hi_pri ;
  /* oam_on_ch0 specifies if OAM control pkts are recvd on Host ch0 */
  cs_boolean    oam_on_ch0 ;
  /* rx_crc_strip specifies CRC strip control for Rx pkts to Host */
  cs_boolean	rx_crc_strip ;
  /* tx_crc_insert specifies CRC insert control for Tx pkts from Host */
  cs_boolean	tx_crc_insert ;
  /* Host accepts wrapped uni-cast packets w/ Wrong RI */
  cs_boolean	accept_wrpd_ucast_on_wrong_ri ;
  /* Host accepts wrapped multi-cast packets w/ Wrong RI */
  cs_boolean	accept_wrpd_mcast_on_wrong_ri ;
  /* Host accepts flooded packets regardless of CAM lookup results */
  cs_boolean	accept_flooded_pkts ;
  /* SRP/RPR header strip control for packets to Host */
  m2r_rmac_hdr_strip_sel_t hdr_strip_sel ;
  /* After stripping, encapsulate with PPP or HDLC hdr */
  cs_boolean	hdr_strip_ppp_enable ;
  /* Add new packet type for the header strip control */
  cs_boolean	hdr_strip_new_pkt_enable ;
  cs_uint8      hdr_strip_new_pkt_mode ;
  cs_uint16     hdr_strip_new_pkt_protocol ;

  /* RI control */
  m2r_rmac_host_ctl_ri_t ctl_ri ;
  /* Packet filters */
  /* rxh_giant_sz specifies max pkt size allowed TO Host */
  cs_uint16	rxh_giant_sz ;
  /* txh_runt_sz specifies min pkt size allowed FROM Host */
  cs_uint8	txh_runt_sz ;
  /* txh_giant_sz specifies max pkt size allowed FROM Host */
  cs_uint16	txh_giant_sz ;
} m2r_rmac_host_cfg_t ;

/************* FRAMER ***************/
typedef struct {
  /* Receive Packet filters in RXF */
  /* rxf_runt_sz specifies min pkt size FROM framer */
  cs_uint8 	rxf_runt_sz ;
  /* rxf_giant_sz specifies max pkt size FROM framer */
  cs_uint16 	rxf_giant_sz ;

  /* stomp_ctl specifies if enabling stomp feature */
  m2r_ctl_t     stomp_ctl ;
  /* err_stomp_host_flag specifies if marking stomped packet as err
     packet */
  cs_boolean    err_stomp_host_flag ;
} m2r_rmac_framer_cfg_t ;

/************ TB ******************/
typedef struct {
  /* one_tb specifies whether it is single TB(TRUE) or dual TB(FALSE) */
  cs_boolean    one_tb ;
  /* stq_base specifies starting offset of STQ (s/b 0 for single TB) */
  cs_uint16     stq_base ;
  /* stq_sz specifies size of STQ(s/b 0 for single TB ) */
  cs_uint16     stq_sz ;
  /* ptq_base specifies starting offset of PTQ */
  cs_uint16     ptq_base ;
  /* ptq_sz specifies size of PTQ */
  cs_uint16     ptq_sz ;
  /* ptq_thresh specifies IPS sending threshold of PTQ */
  cs_uint16 ptq_thresh ;
  /* stq_hthresh_ctl specifies if turning off the limitation of
     stq_hthresh */
  m2r_ctl_t stq_hthresh_ctl ;
  /* stq_hthresh_boundry specifies how close stq buffer depth can close
     to high threshold in unit of 4KB */
  cs_uint8 stq_hthresh_boundry ;
} m2r_rmac_tb_cfg_t ;

/************* IPS/CTL PACKET CONTROL ***************/
typedef struct {
  /* Default destinations for IPS and other CTL packets */
  /* if TRUE - to buffer(to CPU), FALSE - to Host */
  cs_boolean	ips_buf_default_destn ;
  cs_boolean	ctl_buf_default_destn ;

  /* ctl_cam specifies the CAM entry - dec_mode, ctl_mask & ctl_type */
  m2r_ctl_cam_entry_t	ctl_cam[4] ;

  /* ips_ttl_* are for IPS in TTL based mode (for RPR only) */
  /* ips_ttl_mode specifies if RPR IPS control is in TTL based mode */
  cs_boolean    ips_ttl_mode ;
  /* ips_ttl_0_mask = TRUE will mask off checking for TTL=0 */
  cs_boolean    ips_ttl_0_mask ;
  /* ips_ttl_max_mask = TRUE will mask off checking for     */
  /*                    ips_ttl_max_stations                */
  cs_boolean    ips_ttl_max_mask ;
  /* ips_ttl_max_stations set to no. of stations on the ring */
  cs_uint8      ips_ttl_max_stations ;
  /* Timer to detect lack of received IPS pkts (0 value disables it) */
  cs_uint32     ips_ttl_tmr_cnt ;
} m2r_rmac_ips_ctl_cfg_t ;

/************* CAM *****************/
typedef struct {
  /* Config a few lookup(Rx) CAM entries(up to 4) for this node addr */
  /* Other entries can be configed after going into NORMAL ring mode */

  cs_uint32		rx_cam_start ;
  cs_uint16		rx_cam_num ;
  m2r_rx_cam_entry_t	rx_cam_tbl[4] ;

  /* Flag to whether malloc 64KB of memory per port for driver        */
  /* maintaining the accounting SRAM stats.                           */
  cs_boolean		acctg_stats_mem_malloced ;

  /* Multi-cast coarse cam filter for multicast DA lookup/filter      */
  /* Key addr, Mask value, accept/reject and valid bit are configured */
  m2r_rmac_mcast_coarse_cam_t	mcast_cam[8] ;

} m2r_rmac_cam_cfg_t ;


/****************************************
 *        RingMAC Configuration         *
 ****************************************/
typedef struct {
  /* ring-Id is based on which ringlet packets are received from */
  cs_uint8	ring_id ;
  /* check_ring_id controls detection of RING_ERR interrupt */
  cs_boolean	check_ring_id ;
  /* ttl_action specifies the decrement control of TTL */
  m2r_ttl_action_t  ttl_action ;
  /* saa_enbl specifies global SA filter control(SA Accept/Reject) */
  cs_boolean	saa_enbl ;
  /* srp_prio_level specifies priority-slicing for SRP packets */
  cs_uint8	srp_prio_level ;
  /* strict_mode specifies checking for RPR pkts with S bit set */
  cs_boolean	strict_mode ;
  /* set HEC calculation mode: TRUE performs HEC in MSB; otherwise,
     in LSB */
  cs_boolean    hec_msb ;
  /* The following flags specifies various loopbacks in RingMAC */
  cs_boolean	txh_to_rxh_lpbk ;
  cs_boolean	txf_to_rxf_lpbk ;
  cs_boolean	rxf_to_mate_lpbk ;
  cs_boolean	rxf_to_host_lpbk ;
  cs_boolean	tom_to_frm_lpbk ;
  cs_boolean	frm_to_tom_lpbk ;
  /* MAC Rewrite(TXH) enable flag     */
  cs_boolean mrw_enabled ;

  /* Micro-sequencer           */
  m2r_rmac_ms_cfg_t  ms ;
  /* Mate                      */
  m2r_rmac_mate_cfg_t mate ;
  /* Idle packet processing    */
  m2r_rmac_idle_pkt_cfg_t idle_pkt ;
  /* Transit Buffer            */
  m2r_rmac_tb_cfg_t tb ;
  /* Host(TXH and RXH)         */
  m2r_rmac_host_cfg_t host ;
  /* Framer(TXF and RXF)       */
  m2r_rmac_framer_cfg_t framer ;
  /* IPS/CTL packet processing */
  m2r_rmac_ips_ctl_cfg_t ips_ctl ;
  /* CAM's, SRAM's             */
  m2r_rmac_cam_cfg_t cam ;
  /* IRQ Table                 */
  cs_uint16  irq_ctl_tbl[8] ;
} m2r_rmac_cfg_t ;


/*********************************************
 *     RingMAC Running Configuration         *
 *********************************************/
typedef struct {
  cs_uint8	        ring_id ;
  m2r_ring_mode_t       ring_mode ; /* ring operational mode */
  cs_boolean            saa_enbl ;
  cs_mac                my_sa ;
  /********** TB ************/
  cs_boolean            one_tb ;
  cs_uint16             stq_base ;
  cs_uint16             stq_sz ;
  cs_uint16             ptq_base ;
  cs_uint16             ptq_sz ;
  /********* Host ***********/
  cs_boolean            mrw_enabled ;
  cs_uint16             rxh_giant_sz ;
  cs_uint16             txh_runt_sz ;
  cs_uint16             txh_giant_sz ;
  cs_uint8              rxh_chan_mode ;
  cs_uint8              rxh_chan_pri_en ;
  cs_uint8              rxh_chan_sel ; /* single channel mode only */
  /********* Framer *********/
  cs_uint8              rxf_runt_sz ;
  cs_uint16             rxf_giant_sz ;
  m2r_ctl_t             stomp_ctl ;
  /******** IPS/CTL *********/
  cs_boolean            ips_buf_default_destn ;
  cs_boolean            ctl_buf_default_destn ;
  cs_boolean            ips_ttl_mode ;
  cs_boolean            ips_ttl_max_mask ;
  cs_uint8              ips_ttl_max_stations ;
  /******** Mate ************/
  cs_boolean    xgxs_alignment_detected ;
  cs_boolean    xgxs_tx_rx_loopback ;
  cs_boolean    xgxs_rx_tx_loopback ;
  /******** Loopbacks *******/
  cs_boolean    txh_to_rxh_lpbk ;
  cs_boolean    txf_to_rxf_lpbk ;
  cs_boolean    rxf_to_mate_lpbk ;
  cs_boolean    rxf_to_host_lpbk ;
  cs_boolean    tom_to_frm_lpbk ;
  cs_boolean    frm_to_tom_lpbk ;
  /******** Rate Limiter ********/
  /******** MS           ********/
  m2r_rmac_ms_runcfg_t       ms ;
} m2r_rmac_runcfg_t ;


/**************************************************************
 * MACROs
 **************************************************************/

/*** SRESET ***/
#define M2R_RMAC_SRESET_MATE		0x0008
#define M2R_RMAC_SRESET_MS		0x0001
#define	M2R_RMAC_SRESET_ALL		0x0002

/*** ips/control packets ***/
#define  M2R_RMAC_CTL_SEND_BUF_SZ	(8 * 1024) /* bytes */
#define  M2R_RMAC_CTL_RX_BUF_SZ		(8 * 1024) /* bytes */
#define  M2R_RMAC_IPS_SEND_BUF_SZ	(512)      /* bytes */
#define  M2R_RMAC_IPS_RX_BUF_SZ		(8 * 256)  /* bytes */
#define  M2R_RMAC_IPS_CTL_PKT_LEN_MASK	(0x1FFF)   /* 12:0  */
#define  M2R_RMAC_IPS_CTL_PKT_ERR_MASK	(0x8000)


/****************** RingMAC APIs ****************************/
extern
cs_status m2r_rmac_init_cfg(cs_uint16 port_id,
			    m2r_port_summ_t * pSumm,
			    m2r_rmac_cfg_t * pCfg) ;
extern
cs_status m2r_rmac_get_default_cfg(cs_uint16 port_id,
				   m2r_port_summ_t * pSumm,
				   m2r_rmac_cfg_t * pCfg) ;
extern
cs_status m2r_rmac_get_running_cfg(cs_uint16 port_id,
				   m2r_port_summ_t * pSumm,
				   m2r_rmac_runcfg_t * pCfg) ;
extern
cs_status m2r_rmac_dump_running_cfg(cs_uint16 port_id,
				    m2r_rmac_runcfg_t * pRunCfg) ;
extern
cs_status  m2r_rmac_soft_reset(cs_uint16 port_id, cs_uint32 sel,
                               m2r_reset_action_t act) ;
extern
cs_status  m2r_rmac_lock(cs_uint16 port_id, cs_boolean lock) ;
extern
cs_status  m2r_rmac_set_ring_id(cs_uint16 port_id, cs_uint8 ring_id,
				cs_boolean check_ring_id) ;
extern
cs_status  m2r_rmac_set_ring_mode(cs_uint16 port_id,
				  m2r_ring_mode_t  ring_mode) ;
extern
m2r_ring_mode_t  m2r_rmac_get_ring_mode(cs_uint16 port_id) ;
extern
cs_status  m2r_rmac_hw_ctl_ring_mode(cs_uint16 port_id,
				  m2r_ctl_t hw_wrap_off,
				  m2r_ctl_t hw_wrap_on,
				  m2r_ctl_t hw_pass_off,
				  m2r_ctl_t hw_pass_on);
extern
cs_status  m2r_rmac_set_ttl_action(cs_uint16 port_id,
				   m2r_ttl_action_t ttl_action) ;
extern
cs_status  m2r_rmac_set_saa_mode(cs_uint16 port_id,
				 cs_boolean saa_enbl) ;
extern
cs_status  m2r_rmac_set_srp_prio_level(cs_uint16 port_id,
				       cs_uint8 prio_level) ;
extern
cs_status  m2r_rmac_strict_mode(cs_uint16 port_id,
				cs_boolean strict_mode) ;
extern
cs_status  m2r_rmac_hecmsb_mode(cs_uint16 port_id,
				cs_boolean hecmsb_mode) ;
extern
cs_status  m2r_rmac_set_srp_ttl(cs_uint16 port_id, cs_boolean overwrite,
				cs_uint8 ttl);
extern
cs_status  m2r_rmac_set_tb_flush_ctl(cs_uint16 port_id,
				  cs_boolean ptq,
				  m2r_flush_type_t flush_type,
				  m2r_ctl_t ctl,
				  cs_uint8 deassert_bp_host_chnl_map) ;
extern
cs_status  m2r_rmac_set_host_flush_ctl(cs_uint16 port_id,
                                  m2r_ctl_t ctl,
                                  cs_uint8 deassert_bp_host_chnl_map) ;
extern
cs_status  m2r_rmac_lock_wdog(cs_uint16 port_id);
extern
cs_status  m2r_rmac_set_wdog_timeout(cs_uint16 port_id,
                                     cs_uint32 tmr_val);
extern
cs_status  m2r_rmac_ctl_wdog(cs_uint16 port_id, m2r_ctl_t ctl);
extern
cs_status  m2r_rmac_wdog_kickstart(cs_uint16 port_id);
extern
cs_status  m2r_rmac_loopback(cs_uint16 port_id,
			     m2r_rmac_lpbk_sel_t sel, m2r_ctl_t ctl) ;


/********************         TB             ********************/
extern
cs_status  m2r_rmac_set_tb_params(cs_uint16 port_id, cs_boolean one_tb,
				  cs_uint16 stq_base, cs_uint16 stq_sz,
				  cs_uint16 ptq_base, cs_uint16 ptq_sz) ;
extern
cs_status  m2r_rmac_set_onetb_queue_delay_timer(cs_uint16 port_id,
                                                m2r_service_class_t sc,
                                                cs_uint32 timer_val) ;
extern
cs_status  m2r_rmac_set_stq_thresh(cs_uint16 port_id,
                                   cs_uint16 stq_hthresh,
				   cs_uint16 stq_lthresh,
                                   cs_uint16 stq_mthresh);
extern
cs_status  m2r_rmac_set_stq_hthresh_boundry(cs_uint16 port_id,
                                            cs_uint8 n_4KBs);
extern
cs_status  m2r_rmac_ctl_stq_hthresh(cs_uint16 port_id, m2r_ctl_t ctl);
extern
cs_status  m2r_rmac_set_ptq_thresh(cs_uint16 port_id,
                                   cs_uint16 ptq_thresh);

/********************        SRPF            ********************/
extern
cs_status  m2r_rmac_set_srpf_max_allow(cs_uint16 port_id,
                                       cs_uint16 usage);

/********************       MATE            *********************/
extern
cs_status  m2r_rmac_set_flood_ctl(cs_uint16 port_id,
				  cs_boolean ft_enbl,
				  m2r_rmac_flood_type_t ft_type,
				  cs_uint8 ft_ttl) ;
extern
cs_status  m2r_rmac_ctl_from_mate_underflow_check(cs_uint16 port_id,
						  cs_boolean enbl_chk) ;

/********************   IDLE PACKET CONTROL  ********************/
extern
cs_status  m2r_rmac_set_tx_idle_timers(cs_uint16 port_id,
				       cs_uint32 slow_tmr_cnt,
				       cs_uint32 fast_tmr_cnt) ;
extern
cs_status  m2r_rmac_set_tx_idle_thold(cs_uint16 port_id,
				      cs_uint16 thold) ;
extern
cs_status  m2r_rmac_write_tx_idle_pkt(cs_uint16 port_id,
				      cs_uint8 * ppkt,
				      cs_uint8 pktlen) ;
extern
cs_status  m2r_rmac_debug_write_tx_idle_pkt(cs_uint16 port_id) ;


/************************** HOST *******************************/
extern
cs_status  m2r_rmac_set_num_host_channel(cs_uint16 port_id,
                                         cs_boolean single_channel,
                                         cs_uint8 single_channel_id,
                                         cs_uint8 num_pri_channels );
extern
cs_status  m2r_rmac_ctl_rpr_oam_on_ch0(cs_uint16 port_id,
                                       m2r_ctl_t ctl) ;
extern
cs_status  m2r_rmac_set_srp_host_prio_thresh(cs_uint16 port_id,
                                             cs_uint8 med_pri,
                                             cs_uint8 hi_pri);
extern
cs_status  m2r_rmac_accept_wpkts_on_wrong_ri(cs_uint16 port_id,
		                             cs_boolean accept_ucast,
                                             cs_boolean accept_mcast) ;

extern
cs_status  m2r_rmac_host_crc_strip_or_insert(cs_uint16 port_id,
			                     cs_boolean rx_strip,
                                             cs_boolean tx_insert) ;
extern
cs_status  m2r_rmac_accept_flooded_data_pkts(cs_uint16 port_id,
					     cs_boolean accept) ;
extern
cs_status  m2r_rmac_ctl_hdr_strip(cs_uint16 port_id,
				  m2r_rmac_hdr_strip_sel_t sel,
				  cs_boolean ppp_enbl) ;
extern
cs_status  m2r_rmac_hdr_strip_ctl_new_pkt_type(cs_uint16 port_id,
					       cs_boolean enbl,
					       cs_uint8 mode,
					       cs_uint16 proto) ;
extern
cs_status  m2r_rmac_host_ctl_ri(cs_uint16 port_id,
				m2r_rmac_host_ctl_ri_t ctl) ;
extern
cs_status  m2r_rmac_set_host_rate_limit_params(cs_uint16 port_id,
	                                       m2r_prio_traffic_t prio,
                                               cs_uint16 tmr_val,
		                               cs_uint16 delta,
                                               cs_uint16 max_accum) ;

extern
cs_status  m2r_rmac_set_prio_traffic_host_rate_limit(cs_uint16 port_id,
		                        m2r_prio_traffic_t prio,
                                        m2r_port_line_rate_t line_rate,
		                        cs_uint8 percentage);
extern
cs_status m2r_rmac_ctl_host_rate_limiter(cs_uint16 port_id,
					 m2r_prio_traffic_t prio,
					 cs_boolean always_pass,
					 cs_boolean always_block) ;
extern
cs_status  m2r_rmac_ctl_classA_marking(cs_uint16 port_id, m2r_ctl_t ctl);
extern
cs_status  m2r_rmac_ctl_classB_marking(cs_uint16 port_id, m2r_ctl_t ctl,
                                       cs_boolean host_premark);
extern
cs_status  m2r_rmac_ctl_reset_host_rate_limiter(cs_uint16 port_id,
                                                m2r_prio_traffic_t prio,
                                                m2r_ctl_t ctl);
extern
cs_status  m2r_rmac_set_unrsrvd_rate_limiter(cs_uint16 port_id,
                                             m2r_ctl_t med_ctl,
                                             m2r_ctl_t hi_ctl,
                                             m2r_ctl_t tblo_ctl,
                                             m2r_ctl_t ctloff_ctl );
extern
cs_status  m2r_rmac_set_control_rate_limiter(cs_uint16 port_id,
                              m2r_ctl_rate_limiter_mode_t mode );


extern
cs_status  m2r_rmac_host_set_pkt_size_filters(cs_uint16 port_id,
					      cs_uint16 rxh_giant_sz,
		                              cs_uint8 txh_runt_sz,
                                              cs_uint16 txh_giant_sz ) ;


/************************ FRAMER ****************************/

extern
cs_status  m2r_rmac_set_rxf_pkt_size_filters(cs_uint16 port_id,
			                     cs_uint8 runt_sz,
                                             cs_uint16 giant_sz) ;
extern
cs_status  m2r_rmac_ctl_rxf_timer_mark_err_packet(cs_uint16 port_id,
                                                  m2r_ctl_t ctl) ;
extern
cs_status  m2r_rmac_ctl_rxf_stomp(cs_uint16 port_id,
                                  m2r_ctl_t stomp_ctl,
                                  cs_boolean err_stomp_host_flag) ;

extern
cs_status  m2r_rmac_ctl_txf_rate_shape(cs_uint16 port_id,
                                       m2r_ctl_t ctl) ;
extern
cs_status  m2r_rmac_set_txf_rate_shape_params(cs_uint16 port_id,
			                      cs_uint16 tmr_val,
                                              cs_uint16 delta,
                                              cs_uint16 max_accum) ;
extern
cs_status  m2r_rmac_set_app_txf_rate_shape(cs_uint16 port_id,
                                           cs_uint32 kbps_line_rate) ;

/**************** CONTROL AND IPS PACKETS *******************/
extern
cs_status  m2r_rmac_enable_ips_ctl_pkt_rxbuf(cs_uint16 port_id,
					     cs_boolean ips_buf,
					     cs_boolean ctl_buf) ;
extern
cs_status m2r_rmac_set_ips_ctl_pkt_rx_default_dstn(cs_uint16 port_id,
						   cs_boolean ips_buf,
						   cs_boolean ctl_buf) ;
extern
cs_status  m2r_rmac_add_ips_ctl_cam_entry(cs_uint16 port_id,
					  cs_uint8 num,
					  cs_uint8 dec_mode,
					  cs_uint16 ctl_mask,
					  cs_uint16 ctl_type) ;
extern
cs_uint16  m2r_rmac_is_ips_ctl_sendbuf_busy(cs_uint16 port_id,
					    cs_boolean ips_buf) ;
extern
cs_status  m2r_rmac_send_ctl_pkt(cs_uint16 port_id, cs_uint8 * ppkt,
				 cs_uint16 pktlen) ;
extern
cs_status  m2r_rmac_write_pkt_to_ctl_send_buf(cs_uint16 port_id,
					      cs_uint8 * ppkt,
					      cs_uint16 pkt_len) ;
extern
cs_status  m2r_rmac_send_same_ctl_pkt(cs_uint16 port_id,
				      cs_uint16 pkt_len) ;
extern
cs_status  m2r_rmac_send_ips_pkt(cs_uint16 port_id, cs_uint8 * ppkt,
				 cs_uint16 pktlen) ;
extern
cs_status  m2r_rmac_write_pkt_to_ips_send_buf(cs_uint16 port_id,
					      cs_uint8 * ppkt,
					      cs_uint16 pkt_len) ;
extern
cs_status  m2r_rmac_send_same_ips_pkt(cs_uint16 port_id,
				      cs_uint16 pkt_len) ;
extern
cs_status  m2r_rmac_read_ctl_pkts_from_rxbuf(cs_uint16 port_id,
			cs_uint8 * buf, cs_uint16 * pbuf_len,
		        cs_uint16  * pnum_pkts, cs_uint16 pkt_limit) ;
extern
cs_status  m2r_rmac_read_ips_pkts_from_rxbuf(cs_uint16 port_id,
			cs_uint8 * buf, cs_uint16 * pbuf_len,
		        cs_uint16  * pnum_pkts, cs_uint16 pkt_limit) ;
extern
cs_status  m2r_rmac_init_ips_buf_in_ttl_mode(cs_uint16 port_id) ;
extern
cs_status  m2r_rmac_clear_ips_buf_entries(cs_uint16 port_id, cs_uint16 start,
					  cs_uint16 num_entries) ;
extern
cs_status  m2r_rmac_enable_ips_ttl_mode(cs_uint16 port_id,
					cs_boolean ttl_mode) ;
extern
cs_status  m2r_rmac_set_ips_ttl_masks(cs_uint16 port_id,
				      cs_boolean ttl_0_mask,
				      cs_boolean ttl_max_mask,
				      cs_uint8 ttl_max_stations) ;
extern
cs_status  m2r_rmac_set_ips_timer(cs_uint16 port_id,
				  cs_uint32 tmr_cnt) ;
extern
cs_boolean  m2r_rmac_is_ips_new_msg_rcvd(cs_uint16 port_id,
					 cs_uint8 * pnew) ;
extern
cs_uint16  m2r_rmac_get_ips_new_src(cs_uint16 port_id) ;
extern
cs_status  m2r_rmac_read_ips_new_msg(cs_uint16 port_id, cs_uint8 new_src,
				     m2r_ips_new_msg_t * pmsg) ;
extern
cs_status  m2r_rmac_dump_ips_new_msg(cs_uint16 port_id,
				     cs_uint8 new_src) ;

/********************* RingMAC Diagnostics  *********************/
extern
cs_status m2r_rmac_ctl_mismatch_multicast(cs_uint16 port_id,
                                          m2r_ctl_t ctl);
extern
cs_status m2r_set_rmac_fmro(cs_uint16 port_id,
			    cs_uint8 blk1, cs_uint8 view1,
			    cs_uint8 blk0, cs_uint8 view0) ;

/********************* MS config ********************************/
extern
cs_status m2r_ms_get_default_cfg(cs_uint16 port_id,
                                 m2r_port_summ_t * pSumm,
                                 m2r_rmac_tb_cfg_t * pTBCfg,
                                 m2r_rmac_ms_cfg_t * pCfg);
extern
cs_status  m2r_ms_init_cfg(cs_uint16 port_id, m2r_port_summ_t * pSumm,
                           m2r_rmac_ms_cfg_t * pCfg);
extern
cs_status m2r_ms_get_rpr_fa_prov(cs_uint16 port_id,
                                 m2r_port_summ_t * pSumm,
                                 m2r_rmac_tb_cfg_t * pTBCfg,
                                 m2r_rmac_rpr_fa_cfg_t * pProv,
                                 m2r_rmac_ms_cfg_t * pCfg);
extern
cs_status m2r_ms_get_srp_fa_prov(cs_uint16 port_id,
                                 m2r_port_summ_t * pSumm,
                                 m2r_rmac_tb_cfg_t * pTBCfg,
                                 m2r_rmac_srp_fa_cfg_t * pProv,
                                 m2r_rmac_ms_cfg_t * pCfg);

#endif /* __M2R_RMAC_H__ */
