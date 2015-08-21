/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systgems Incorporated is   */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_ms.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * Header file for Microsequencer.
 */
#ifndef __M2R_RMAC_MS_H__
#define __M2R_RMAC_MS_H__

#include "cs_types.h"
#include "m2r_registers.h"
#include "m2r_public.h"

#include "m2r_rmac_cam.h"
#include "m2r_rmac_rpr_fa.h"
#include "m2r_rmac_srp_fa.h"

/************ MS ******************/
typedef struct {
  /* Timer 0 running mode */
  cs_boolean  t0_oneShot ;
  /* Timer 1 running mode */
  cs_boolean  t1_oneShot ;
  
  /* RPR only */
  /* The following 3 settings should not be changed to any values */
  /*  different from the default configurations.                  */
  cs_boolean  conservMode ;
  cs_boolean  longFaPacketType ;
  cs_uint8    faPacketLength ;

  cs_boolean  wrappingProtection ;

  /* provisioning */
  /* Micro-sequencer MAC SA used in generating fairness packet */
  cs_mac      my_sa ;
   /* reserved message field for generating SRP usage packet */
  cs_uint16   rsvd_msg ;

  /* keepalive Max timeout value */
  cs_uint16   keepAliveMaxTimer ;
  /* specify whether support activeweight detection */
  m2r_ctl_t   Active_W_En ;
  /* specify whether generating Multi-choke fairness message */
  m2r_ctl_t   Gen_MCFF ;

  /* aging Interval */
  cs_uint32   t0_cnt ;
  /* advertisement Interval */
  cs_uint32   t1_cnt ;

  /* provision */
  cs_uint8    ttl_to_wrap ; /* distance to wrap point */
  /* total station number used in generating fairness packet */
  cs_uint8    ttl_prov ;
  /*
   * The following registers are used to pass some prov-specific
   * parameters into M-CODE. Currently, prov1/prov0 are used to 
   * pass the link rate (bytes per aging interval) into M-CODE for
   * SRP MS operation mode, but pass the link rate in bytes
   * per ageCoef aging interval into M-CODE for RPR MS 
   * operation modes.
   */
  cs_uint16   prov1 ;
  cs_uint16   prov0 ;
  /*  This indicates the total percentage of BW is assigned to
      the reserved subclass A0 traffic */
  cs_uint8    resv_rate ;
  cs_uint32   max_rcv_rate ; /* unused currently */
  /************ SRPF control **************/
  /* Maximum allowable usage (in terms of 16 byte) */
  cs_uint16   max_allow ;
  
  /* Fairness algorithm coefficients, but the real values
     are the following number of power of 2  */
  /* SRP only use 'ageCoef' */
  cs_uint8    agecoeff ;
  cs_uint8    rampUpcoeff ;
  cs_uint8    rampDncoeff ;
  cs_uint8    my_weight ;
  /* NOTE: This could be negative value! */
  cs_int8     ratecoeff ;
  /* 'starvecoeff' is used to derive RPR fairness variable 
     'starveFactor' by the following equation:
       [starveFactor] = 1/2**[starvecoeff],
     where 'starvecoeff' ranges from [0, 7] */
  cs_uint8    starvecoeff ;

  /* The followings are low pass filtering coefficients */
  /* In SRP, these coefs are set to different values;
     In RPR, these coefs are set to an identical parameter lpCoef */
  cs_uint8    lp_allow ;
  cs_uint8    lp_fwd ;
  cs_uint8    lp_mu ;
  cs_uint8    lp_nrXmit ;

  /* These parameters are only used for single TB case */
  /* rate at or above which congestion on the outbound link is
     declared */
  /* Note: This value take the percentage of unreserved BW !*/
  cs_uint8   rate_high_th ;
  /* rate at or above which congestion on the outbound link is
     imminent */
  /* Note: This value take the percentage of unreserved BW !*/
  cs_uint8   rate_low_th ;
  /* Indicates the maximum amount of time any classA add traffic 
     can wait to be transmitted before indicating a defect. */
  cs_uint32  classAAccessDelayTimerThreshold ;
  /* Indicates the maximum amount of time any classB add traffic
     can wait to be transmitted before indicating congestion */
  cs_uint32  classBAccessDelayTimerThreshold ;
  /* Indicates the maximum amount of time any classC add traffic
     can wait to be transmitted before indicating congestion */
  cs_uint32  classCAccessDelayTimerThreshold ;
  
  /* These parameters are only used for dual TB case */
  /* stqHighThresh specifies high threshold of STQ; 
     when STQ occupancy goes beyond this value,
     no fairness eligible packets can be added from the HOST.
     This is RPR and SRP standard compliance variable */
  cs_uint16 stqHighThresh ;
  /* stqLowThresh specifies low threshold of STQ; 
     when STQ occupancy goes below this value,
     fairness eligible packet add rate from HOST can be increased
     This is RPR and SRP standard compliance variable */
  cs_uint16 stqLowThresh ;
  /* stqMedThresh specifies medium threshold of STQ; 
     when STQ occupancy goes beyond this value,
     fairness eligible packet add rate from HOST should be ramped down.
     This is RPR standard compliance variable */
  cs_uint16 stqMedThresh ;
  /* stqFullThresh specifies the threshold of STQ indicating STQ is
     almost full.  When STQ occupancy goes beyond this value,
     no packets can be added from the HOST.
     This is RPR standard compliance variable */
  cs_uint16 stqFullThresh ;

  /* Multi-choke fairness packet control */
  m2r_ctl_t   mcfair_en ;
  cs_boolean  mcfair_t1 ;
  cs_uint8    mcfair_th ;    /* report interval */

  /* Activeweight control */
  m2r_ctl_t   actweight_en ;
  cs_boolean  actweight_t1 ;
  cs_uint8    actweight_th ; /* used only for conserv fa algorithm */

  /* Round-trip timer control */
  /* Note: When using CORTINA-provided micro-code, these settings  */
  /*       are used to program "FRTT" initial value with the       */
  /*       following restrictions:                                 */
  /*       - 'rtt_en' should always be "M2R_DISABLE";              */
  /*       - 'rtt_t1' should always be "FALSE";                    */
  /*       - 'rtt_th' indicates how many times of agingInterval;   */
  m2r_ctl_t   rtt_en ;
  cs_boolean  rtt_t1 ;
  cs_uint32   rtt_th ;       /* used only for conserv fa algorithm */

} m2r_rmac_ms_cfg_t ;

typedef struct {
  m2r_rmac_ms_mode_t ms_mode ;

  /* Common parts for both RPR and SRP */
  cs_uint16          agingInterval ;
  cs_uint16          advertisementInterval ;

  cs_uint8           ttl_prov ;
  
  cs_uint32          prov ;

  cs_uint16          max_allow ;

  cs_uint8           agecoeff ;

  /* In SRP, these coefs are set to different values;
     In RPR, these coefs are set to an identical parameter lpCoef */
  cs_uint8           lp_allow ;
  cs_uint8           lp_fwd ;
  cs_uint8           lp_mu ;
  cs_uint8           lp_nrXmit ;
  
  /* RPR-only parts */
  cs_boolean         wrappingProtection ;
  cs_uint16          keepAliveMaxTimer ;
  cs_ctl_t           Active_W_En ;
  cs_ctl_t           Gen_MCFF ;
  cs_uint8           ttl_to_wrap ;

  cs_uint32          unresvd_rate ;

  cs_uint8           rampUpcoeff ;
  cs_uint8           rampDncoeff ;
  cs_uint8           my_weight ;
  /* NOTE: This could be negative value! */
  cs_int8            ratecoeff ;
  /* 'starvecoeff' is used to derive RPR fairness variable
     'starveFactor' by the following equation:
       [starveFactor] = 1/2**[starvecoeff],
     where 'starvecoeff' ranges from [0, 7] */
  cs_uint8           starvecoeff ;

  /* These parameters are only used for single TB case */
  /* rate at or above which congestion on the outbound link is
     declared */
  cs_uint32          rate_high_th ;
  /* rate at or above which congestion on the outbound link is
     imminent */
  cs_uint32          rate_low_th ;
  /* Indicates the maximum amount of time any classA add traffic
     can wait to be transmitted before indicating a defect. */
  cs_uint32          classAAccessDelayTimerThreshold ;
  /* Indicates the maximum amount of time any classB add traffic
     can wait to be transmitted before indicating congestion */
  cs_uint32          classBAccessDelayTimerThreshold ;
  /* Indicates the maximum amount of time any classC add traffic
     can wait to be transmitted before indicating congestion */
  cs_uint32          classCAccessDelayTimerThreshold ;

  /* These parameters are only used for dual TB case */
  /* stqHighThresh specifies high threshold of STQ;
     when STQ occupancy goes beyond this value,
     no fairness eligible packets can be added from the HOST.
     This is RPR and SRP standard compliance variable */
  cs_uint16          stqHighThresh ;
  /* stqLowThresh specifies low threshold of STQ;
     when STQ occupancy goes below this value,
     fairness eligible packet add rate from HOST can be increased
     This is RPR and SRP standard compliance variable */
  cs_uint16          stqLowThresh ;
  /* stqMedThresh specifies medium threshold of STQ;
     when STQ occupancy goes beyond this value,
     fairness eligible packet add rate from HOST should be ramped down.
     This is RPR standard compliance variable */
  cs_uint16          stqMedThresh ;
  /* stqFullThresh specifies the threshold of STQ indicating STQ is
     almost full.  When STQ occupancy goes beyond this value,
     no packets can be added from the HOST.
     This is RPR standard compliance variable */
  cs_uint16          stqFullThresh ;

  /* Multi-choke fairness packet control */
  cs_ctl_t           mcfair_en ;
  cs_boolean         mcfair_t1 ;
  cs_uint8           mcfair_th ;    /* report interval */

  /* Activeweight control */
  cs_ctl_t           actweight_en ;
  cs_boolean         actweight_t1 ;
  cs_uint8           actweight_th ; /* used only for conserv fa algorithm */

  /* Round-trip timer control */
  /* Note: When using CORTINA-provided micro-code, these settings  */
  /*       are used to program "FRTT" initial value with the       */
  /*       following restrictions:                                 */
  /*       - 'rtt_en' should always be "CS_DISABLE";               */
  /*       - 'rtt_t1' should always be "FALSE";                    */
  /*       - 'rtt_th' indicates how many times of agingInterval;   */
  cs_ctl_t           rtt_en ;
  cs_boolean         rtt_t1 ;
  cs_uint32          rtt_th ;       /* used only for conserv fa algorithm */

  /* SRP-only parts */
  cs_uint16          rsvd_msg ;

  /* Status */
  cs_ctl_t           t0_en ;
  cs_ctl_t           t1_en ;

  cs_uint16          ms_sr ;
  cs_uint8           fms_sema ;

  cs_uint32          lr_rate ;
  cs_uint32          advertisementRatio ;

  /* RPR-only */
  cs_uint8           fms_sema_status ;
} m2r_rmac_ms_runcfg_t ;

typedef struct {
  cs_mac	mac_sa ;
  cs_boolean longFaPacketType ;
  union {
  	struct {
  	  cs_uint16 fa_ctl ;
  	  cs_uint16 fa_val ;
  	} shortpkt ;
  	cs_uint32 fa_hec_val ;
  } payload ;
} m2r_mc_fa_msg_t ;

typedef struct {
  cs_uint32 my_usage ;
  cs_uint32 fwd_rate ;
  cs_uint32 my_usage_cng ;
  cs_uint32 fwd_rate_cng ;
  cs_uint32 nrxmit_rate ;
} m2r_rmac_ms_runtime_fa_t ;

/* Semaphore encodings */
#define M2R_MS_SEMA_RESET_TB_STATS         0x01
#define M2R_MS_SEMA_READING_TB_STATS       0x02
#define M2R_MS_SEMA_REQUEST_TTL_TO_CONGEST 0x20     


typedef struct {
  cs_boolean ms_time_out ;
  cs_boolean ms_instr_par ;
  cs_uint8   ms_send_sema ;
} m2r_rmac_ms_int_t ;

/*
 * Register definitions
 */ 
#define M2R_SHORT_FA_PACKET_LENGTH		16
#define M2R_LONG_FA_PACKET_BASE_LENGTH		24
#define M2R_LONG_FA_PACKET_MAX_LENGTH  		32

#define M2R_MS_TIMER_CLOCK_CYCLE_PER_COUNT	10

#define M2R_MS_DR_BASE_TYPE             	4
#define M2R_MS_PGM_STORE_SIZE           	512
#define M2R_MS_DATA_STORE_SIZE          	64

#define M2R_MS_TIMEOUT_MASK			0x0001
#define M2R_MS_INSTR_STORE_PARITY_ERR_MASK      0x0004

#define M2R_MS_MAX_PC                           0x1FF
#define M2R_MS_MAX_JUMP_IDX			0x1FF
#define M2R_MS_IX_BITMAP_ALL                    0x1F

/*
 * Default configuration definitions
 */
/* default MAC-SA is 10.10.10.10.10.x  (x is port id = 0...15) */
#define M2R_RMAC_DEFAULT_MACSA_BYTE5        10
#define M2R_RMAC_DEFAULT_MACSA_BYTE4        10
#define M2R_RMAC_DEFAULT_MACSA_BYTE3        10
#define M2R_RMAC_DEFAULT_MACSA_BYTE2        10
#define M2R_RMAC_DEFAULT_MACSA_BYTE1        10
#define M2R_RMAC_DEFAULT_MACSA_BYTE0        0 /* replace with port_id */

#define M2R_MS_FAST_AGING_INTERVAL          100 /* usec */
#define	M2R_MS_SLOW_AGING_INTERVAL          400 /* usec */
/*
#define M2R_MS_ADVERTISEMENT_RATIO_DEFAULT  0.00125
*/
#define M2R_MS_ADVERTISEMENT_RATIO_DEFAULT  125 /* in unit of 0.00001 */

#define M2R_MS_TTL_PROV_DEFAULT		    255
#define M2R_MS_RSVD_MSG_DEFAULT		    0x0000
#define M2R_MS_TTL_TO_WRAP_DEFAULT	    0

#define	M2R_MS_AGECOEFF_DEFAULT		    2
#define M2R_MS_RAMPUPCOEFF_DEFAULT	    6
#define	M2R_MS_MY_WEIGHT_DEFAULT	    0 

#define M2R_MS_LP_ALLOW_DEFAULT		    6
#define	M2R_MS_LP_FWD_DEFAULT		    6
#define	M2R_MS_LP_MU_DEFAULT		    9
#define	M2R_MS_LP_NRXMIT_DEFAULT	    9

/*
 * RPR-specific default parameters
 */
#define M2R_MS_RPR_AGECOEFF_DEFAULT         2
#define M2R_MS_RPR_LPCOEFF_DEFAULT          6

#define M2R_MS_KEEPALIVE_TIMEOUT_MSEC_DEFAULT    3 /* msec */
#define M2R_MS_STARVECOEFF_DEFAULT               1
#define M2R_MS_RAMPDNCOEFF_DEFAULT               6

#define	M2R_MS_TB_HTHRESH_DEFAULT	    0x800 /* 256 bytes */
#define	M2R_MS_TB_MTHRESH_DEFAULT	    0x280 /* 256 bytes */
#define	M2R_MS_TB_LTHRESH_DEFAULT	    0x180 /* 256 bytes */

#define M2R_MS_RPR_DQT_A_MAX                25500
#define M2R_MS_DQT_B_DEFAULT                1000
#define M2R_MS_DQT_C_DEFAULT                10000

#define	M2R_MS_MCFAIR_TH_DEFAULT	    10
#define	M2R_MS_ACTWEIGHT_TH_DEFAULT	    64
#define M2R_MS_FRTT_TIME_MSEC_DEFAULT       1000 /* msec */

#define M2R_MS_MCFAIR_MAX_TH		    0xFF
#define	M2R_MS_ACTWEIGHT_MAX_TH		    0xFF
#define M2R_MS_RTT_MAX_TH	            0x3FFFFFFF

#define M2R_MS_PROV1_DEFAULT		    0x0000
#define M2R_MS_PROV0_DEFAULT    	    0x7a12

/* Some legacy value */
#define M2R_MS_TPXG_PROV_VALUE              0x1f400

#define M2R_MS_MAX_RCV_RATE     	    0xffffffff
#define M2R_MS_MAX_ALLOW_DEFAULT 	    0xffff

/* Default link rate in unit of Mbps for different PHY */
#define M2R_MS_LR_KBPS_OC192                9953280
#define M2R_MS_LR_KBPS_GE                   1000000
#define M2R_MS_LR_KBPS_10GE 		    10000000
#define M2R_MS_LR_KBPS_OC48 		    2488320
#define M2R_MS_LR_KBPS_OC12 		    622080

/* Default LINK_RATE in unit of bytes per agingCoef agingInterval
   for different PHY */
/* The following is compliant with RPR standard */
#define	M2R_MS_LINK_RATE_OC3		    62208
#define	M2R_MS_LINK_RATE_OC12		    62208
#define M2R_MS_LINK_RATE_GE		    50000
#define	M2R_MS_LINK_RATE_OC48		    62208
#define	M2R_MS_LINK_RATE_OC192              62208
#define	M2R_MS_LINK_RATE_10GE		    62500

/*
 * Driver definitions
 */
#define M2R_MS_CODE_MAX_TYPE		    5
#define M2R_MS_FILE_LINE_SIZE  		    80

/** Global Const array for storing pre-programmed M-codes **/
extern
const 
cs_uint32 ms_ucode_pgm[M2R_MS_CODE_MAX_TYPE][M2R_MS_PGM_STORE_SIZE] ;
extern
const 
cs_uint32 ms_ucode_data[M2R_MS_CODE_MAX_TYPE][M2R_MS_DATA_STORE_SIZE] ;

/***************** Initialization *******************/
extern
cs_status  m2r_ms_init_cfg(cs_uint16 port_id, m2r_port_summ_t * pSumm, 
			   m2r_rmac_ms_cfg_t * pCfg);
extern
cs_status  m2r_ms_load_default(cs_uint16 port_id, 
                               m2r_rmac_ms_mode_t mode);
extern
cs_status  m2r_ms_load_pgm_or_data(cs_uint16 port_id,
                                   m2r_rmac_mem_id_t memid, char * fn,
                                   cs_uint32 num_entries);

/************************ Provision *****************************/
extern
cs_status  m2r_ms_set_timer_mode(cs_uint16 port_id, 
                                 cs_boolean t0_oneShot,
                                 cs_boolean t1_oneShot);
extern
cs_status  m2r_ms_set_timer_val(cs_uint16 port_id, m2r_ctl_t t0_en,
                                cs_uint32 t0_cnt, m2r_ctl_t t1_en,
                                cs_uint32 t1_cnt);
extern
cs_status  m2r_ms_set_mac_sa(cs_uint16 port_id, cs_uint16 sa5_4,
                             cs_uint16 sa3_2, cs_uint16 sa1_0);
extern
cs_status  m2r_ms_get_mac_sa(cs_uint16 port_id, cs_mac * pMySa);
extern
cs_status  m2r_ms_set_rsvd_msg(cs_uint16 port_id, cs_uint16 data);

extern
cs_status  m2r_ms_set_ttl_to_wrap(cs_uint16 port_id, cs_uint8 distance);
extern
cs_status  m2r_ms_set_unresv_rate(cs_uint16 port_id, cs_uint32 data);
extern
cs_status  m2r_ms_set_prov(cs_uint16 port_id, cs_uint16 prov1,
                           cs_uint16 prov0);
extern
cs_status  m2r_ms_set_max_rcv_rate(cs_uint16 port_id, cs_uint32 data);
extern
cs_status  m2r_ms_set_rate_thresh(cs_uint16 port_id, cs_uint32 rate_h,
                                  cs_uint32 rate_l);
extern
cs_status  m2r_ms_set_rpr_stq_thresh(cs_uint16 port_id, 
                                     cs_uint16 stqHighThresh,
                                     cs_uint16 stqLowThresh, 
                                     cs_uint16 stqMedThresh,
                                     cs_uint16 stqFullThresh) ;
extern
cs_status  m2r_ms_set_srp_stq_thresh(cs_uint16 port_id, 
                                     cs_uint16 stqHighThresh,
                                     cs_uint16 stqLowThresh) ;
extern
cs_status  m2r_ms_set_fa_weight(cs_uint16 port_id, cs_uint8 weight);

extern
cs_status  m2r_ms_set_fa_coeff1(cs_uint16 port_id, cs_uint8 agecoeff,
                                cs_int8 ratecoeff);
extern
cs_status  m2r_ms_set_fa_coeff2(cs_uint16 port_id, cs_uint8 rampUpcoeff, 
                                cs_uint8 rampDncoeff,
                                cs_uint8 starvecoeff);
extern
cs_status  m2r_ms_set_fa_keepalive_max_timer(cs_uint16 port_id, 
                                             cs_uint16 thresh);
extern
cs_status  m2r_ms_ctl_fa_conserv_active_weight(cs_uint16 port_id, 
                                               m2r_ctl_t ctl);
extern
cs_status  m2r_ms_set_fa_pkt_we_field(cs_uint16 port_id,
                                      cs_boolean wrapping);
extern
cs_status  m2r_ms_ctl_gen_mc_fa(cs_uint16 port_id, m2r_ctl_t ctl);

extern
cs_status  m2r_ms_set_fa_lp_coeff(cs_uint16 port_id, cs_uint8 lp_allow,
                                  cs_uint8 lp_fwd, cs_uint8 lp_mu, 
                                  cs_uint8 lp_nrXmit);
extern
cs_status  m2r_ms_set_mcfair_timer(cs_uint16 port_id, m2r_ctl_t ctl,
                                   cs_boolean timer1, cs_uint8 thresh);
extern
cs_status  m2r_ms_set_actvwght_timer(cs_uint16 port_id, m2r_ctl_t ctl,
                                     cs_boolean timer1, cs_uint8 thresh);
extern
cs_status  m2r_ms_set_rtt_timer(cs_uint16 port_id, m2r_ctl_t ctl,
                                cs_boolean timer1, cs_uint32 thresh);
extern
cs_status  m2r_ms_set_rpr_frtt(cs_uint16 port_id, cs_uint32 thresh);

/************************ Control *******************************/
extern
cs_status  m2r_ms_ctl_stall(cs_uint16 port_id, m2r_ctl_t ctl);
extern
cs_status  m2r_ms_ctl_par_chk(cs_uint16 port_id, m2r_ctl_t all_chk,
                              m2r_ctl_t in_chk, m2r_ctl_t ext_chk);
extern
cs_status  m2r_ms_ctl_send_fa_pkt(cs_uint16 port_id, m2r_ctl_t ctl);
extern
cs_status  m2r_ms_read_int_status(cs_uint16 port_id, 
                                  m2r_rmac_ms_int_t * ps,
                                  cs_boolean clear);
extern
cs_uint16  m2r_ms_read_int_bitmap(cs_uint16 port_id, cs_boolean clear) ;
extern
cs_boolean  m2r_ms_check_mc_fa_new_msg(cs_uint16 port_id, 
                                       cs_uint8 * psrc);
extern
cs_status  m2r_ms_read_mc_fa_msg(cs_uint16 port_id, 
                                 cs_uint8 new_src,
                                 cs_uint8 ri, 
                                 m2r_mc_fa_msg_t * pmsg);
extern
cs_status  m2r_ms_clr_mc_fa_msg(cs_uint16 port_id, cs_uint8 ttl,
				cs_uint8 ri);
extern
cs_uint32 m2r_ms_get_running_advertisement_ratio(cs_uint16 port_id);
extern
cs_uint32 m2r_ms_get_running_resvdrate(cs_uint16 port_id) ;
extern
cs_status  m2r_ms_get_running_fa_var(cs_uint16 port_id, 
				     m2r_rmac_ms_runtime_fa_t * pFa);
extern
cs_status m2r_ms_get_last_head_of_congest(cs_uint16 port_id,
                                           cs_mac * pCGSa);

/**********************   Semaphore  ****************************/
extern
cs_status  m2r_ms_reset_tb_stats(cs_uint16 port_id) ;
extern
cs_uint8  m2r_ms_get_ttl_to_congest(cs_uint16 port_id) ;

/************************   Dump  *******************************/
extern
cs_status  m2r_ms_dump_mc_fa_msg(cs_uint16 port_id, 
				 cs_uint8 new_src);
extern
cs_status  m2r_ms_dump_new_mc_fa_msg(cs_uint16 port_id);
extern
cs_status  m2r_ms_dump_running_fa_var(cs_uint16 port_id);
extern
cs_status  m2r_ms_dump_current_in_fa_msg(cs_uint16 port_id);

/*********************** Micro-code Develope ********************/
extern
cs_status  m2r_ms_set_running_step(cs_uint16 port_id, cs_uint16 cnt);
extern
cs_status  m2r_ms_set_idx(cs_uint16 port_id, cs_uint8 mask, 
                          cs_uint8 idx);
extern
cs_status  m2r_ms_set_jmp_idx(cs_uint16 port_id, cs_uint16 idx);
extern
cs_status  m2r_ms_set_pc(cs_uint16 port_id, cs_uint16 cnt);

extern 
cs_status  m2r_ms_send_sema(cs_uint16 port_id, cs_uint8 sema_tms);
extern
cs_status  m2r_ms_get_sema(cs_uint16 port_id, cs_uint8 * psema_fms,
                           cs_uint8 * psema_tms);
extern
cs_status  m2r_ms_clear_out_sema(cs_uint16 port_id, 
                                 cs_uint8 clr_fms_mask);


/************************ Diagnostics ***************************/
extern  cs_uint16  m2r_ms_get_error_status(cs_uint16 port_id);
extern  cs_uint16  m2r_ms_get_pc(cs_uint16 port_id);



#endif /* __M2R_RMAC_MS_H__ */

