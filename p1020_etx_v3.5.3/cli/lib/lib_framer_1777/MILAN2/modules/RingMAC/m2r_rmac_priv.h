/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_priv.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PRIVATE header file - for driver use only!
 */
#ifndef __M2R_RMAC_PRIV_H__
#define __M2R_RMAC_PRIV_H__

#include "cs_types.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_rmac.h"
#include "m2r_rmac_cam.h"


#define  M2R_RMAC_INVALID_PORT_EXIT(port_id, ret_val)               \
{                                                                   \
  cs_uint32             err_code = 0 ;                              \
                                                                    \
  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {                  \
    CS_HNDL_ERROR(port_id, err_code, " : port_id= %d\n",            \
		  port_id) ;                                        \
    return(ret_val) ;                                               \
  }                                                                 \
}


#define M2R_INVALID_IN_BYPASS	0x01
#define M2R_INVALID_IN_SRP	0x02
#define M2R_INVALID_IN_RPR	0x04

#define  M2R_RMAC_INVALID_MODE_EXIT(port_id, invld_modes, ret_val)  \
{                                                                   \
  m2r_port_mac_mode_t  mode = M2R_GET_MAC_MODE(port_id) ;           \
                                                                    \
  if ( (mode == M2R_PORT_IN_BYPASS_MODE) &&                         \
       ((invld_modes) & M2R_INVALID_IN_BYPASS) ) {                  \
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;     \
    return(ret_val) ;                                               \
  }                                                                 \
  if ( (mode == M2R_PORT_IN_SRP_MODE) &&                            \
       ((invld_modes) & M2R_INVALID_IN_SRP) ) {                     \
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_SRP, NULL) ;        \
    return(ret_val) ;                                               \
  }                                                                 \
  if ( (mode == M2R_PORT_IN_RPR_MODE) &&                            \
       ((invld_modes) & M2R_INVALID_IN_RPR) ) {                     \
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_RPR, NULL) ;        \
    return(ret_val) ;                                               \
  }                                                                 \
}

#define M2R_NUM_OF_256B_WORDS_IN_TB(tb_sz)    	\
	( (tb_sz) / 256 )

CS_INLINE char * M2R_GET_RING_MODE_NAME(m2r_ring_mode_t rmode)
{
  switch (rmode) {
    case M2R_RING_NORMAL         : return ("Normal") ;         break ;
    default :
    case M2R_RING_PASSTHROUGH    : return ("Passthrough") ;    break ;
    case M2R_RING_WRAP           : return ("Wrapped") ;        break ;
    case M2R_RING_TRANSPARENT    : return ("Transparent") ;    break ;
    case M2R_RING_SNIFFER_PASSIVE: return ("Sniffer-Passive"); break ;
    case M2R_RING_SNIFFER_ACTIVE : return ("Sniffer-Active") ; break ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_RING_MODE_NAME(rmode) ;
  }
#endif

}

extern
cs_status  m2r_rmac_set_mac_mode_sel(cs_uint16 port_id,
				     m2r_port_mac_mode_t mode) ;
extern
cs_status  m2r_rmac_ctl_ws_for_pkts_from_host(cs_uint16 port_id,
					      cs_boolean setws,
					      cs_uint8   ws) ;
extern
cs_status  m2r_rmac_ctl_we_in_wrap(cs_uint16 port_id,
                                   cs_boolean strip_we,
                                   cs_boolean mate_we_check) ;



/*****************************************************
 *       RINGMAC MEMORIES(CAM, SRAM)                 *
 *****************************************************/
typedef struct {
  DR_MICRO_IF_GRAM_CR_t GRAM_CR ;
  DR_MICRO_IF_GRAM_D5_t GRAM_D5 ;
  DR_MICRO_IF_GRAM_D4_t GRAM_D4 ;
  DR_MICRO_IF_GRAM_D3_t GRAM_D3 ;
  DR_MICRO_IF_GRAM_D2_t GRAM_D2 ;
  DR_MICRO_IF_GRAM_D1_t GRAM_D1 ;
  DR_MICRO_IF_GRAM_D0_t GRAM_D0 ;
} m2r_rmac_gram_t ;

extern  cs_status  m2r_rmac_write_gram_regs(cs_uint16 port_id,
					    m2r_rmac_gram_t * pgram) ;
extern  cs_status  m2r_rmac_read_gram_regs(cs_uint16 port_id,
					   m2r_rmac_gram_t * pgram) ;
extern  cs_status  m2r_rmac_read_host_drop_gram_regs(cs_uint16 port_id,
                                                     m2r_rmac_gram_t * pgram) ;
extern
cs_status  m2r_rmac_cam_init_cfg(cs_uint16 port_id,
				 m2r_rmac_cfg_t * pCfg) ;
extern
cs_status  m2r_rmac_get_coarse_cntr(cs_uint16 port_id,
				     cs_uint32 entry_num,
				     m2r_rmac_cntr_t * pcntr) ;
extern
cs_status  m2r_rmac_reset_coarse_cntr(cs_uint16 port_id,
				     cs_uint32 start,
				     cs_uint32 num_entries) ;
extern
cs_status m2r_rmac_reset_host_drop_channel_cntr(cs_uint16 port_id,
                   cs_boolean cam_hit, cs_uint8 channel_id,
                   cs_uint32 start, cs_uint32 num_entries) ;
extern
cs_status m2r_rmac_get_host_drop_channel_cntr(cs_uint16 port_id,
                   cs_boolean cam_hit, cs_uint8 channel_id,
                   cs_uint32 entry_num,
                   m2r_rmac_cntr_t * pcntr) ;
extern
cs_status  m2r_rmac_clear_host_drop_sat(cs_uint16 port_id) ;

extern
cs_boolean m2r_rmac_check_mcast_coarse_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num) ;
extern
cs_boolean m2r_rmac_check_host_drop_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num) ;

CS_INLINE cs_uint16 M2R_RMAC_GET_ACCTG_SRAM_SIZE(m2r_rmac_mem_id_t
						 sram_id)
{
   if ( (sram_id == M2R_RX_ACCTG_SA_SRAM) ||
	(sram_id == M2R_TX_ACCTG_SA_SRAM) ) {
     return (M2R_ADDR_CAM_SA_NUM_ENTRIES) ;
   }
   else {
     return (M2R_NUM_OF_ADDR_CAM_ENTRIES) ;
   }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_RMAC_GET_ACCTG_SRAM_SIZE(sram_id) ;
  }
#endif
}

#define M2R_RMAC_IS_SRAM_ID_VALID(sram_id)         \
        ( (sram_id == M2R_RX_ACCTG_SA_SRAM) ||     \
	  (sram_id == M2R_RX_ACCTG_DA_SRAM) ||     \
	  (sram_id == M2R_TX_ACCTG_SA_SRAM) ||     \
	  (sram_id == M2R_TX_ACCTG_DA_SRAM) )

CS_INLINE char * M2R_GET_ACCTG_SRAM_NAME(m2r_rmac_mem_id_t sram_id)
{
  switch(sram_id) {
   case M2R_RX_ACCTG_SA_SRAM :
     return ("Rx(lookup) SA Accounting SRAM") ;
   case M2R_RX_ACCTG_DA_SRAM :
     return ("Rx(lookup) DA Accounting SRAM") ;
   case M2R_TX_ACCTG_SA_SRAM :
     return ("Tx(stat) SA Accounting SRAM") ;
   default:
   case M2R_TX_ACCTG_DA_SRAM :
     return ("Tx(stat) DA Accounting SRAM") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_ACCTG_SRAM_NAME(sram_id) ;
  }
#endif
}


CS_INLINE cs_uint32 M2R_RMAC_MEMID_TO_MEMSEL(m2r_rmac_mem_id_t mem_id)
{
  switch (mem_id) {
    case M2R_RX_CAM : 		return (SEL_LOOKUP_CAM) ;
    case M2R_RX_STATUS_SRAM :	return (SEL_LOOKUP_STATUS) ;
    case M2R_RX_ACCTG_SA_SRAM :	return (SEL_LOOKUP_ACCT_SRC) ;
    case M2R_RX_ACCTG_DA_SRAM :	return (SEL_LOOKUP_ACCT_DST) ;
    case M2R_TX_CAM :		return (SEL_STAT_CAM) ;
    case M2R_TX_STATUS_SRAM : 	return (SEL_STAT_STATUS) ;
    case M2R_TX_ACCTG_SA_SRAM :	return (SEL_STAT_ACCT_SRC) ;
    case M2R_TX_ACCTG_DA_SRAM :	return (SEL_STAT_ACCT_DST) ;
    case M2R_MS_DATA_STORE : /* and, A[10] should be 0 */
    case M2R_MS_CORE_STORE : /* and, A[10] should be 1 */
                                return (SEL_USEQ_DC_STORE) ;
    case M2R_MS_PGM_STORE : return (SEL_USEQ_PGM_STORE);
    case M2R_MCAST_COARSE_CAM: /* A[10:9] = b00 | A[10:9] = b10 */
    case M2R_MCAST_COARSE_CNTRS: /* A[10:9] = b01 */
                                return (SEL_LKP_COARSE_CAM) ;
    case M2R_MAC_REWRITE_SRAM :	return (SEL_MACREW_IDX_RAM) ;
    case M2R_MS_MCHOKE_SRAM :	return (SEL_USEQ_MCHOKE_RAM) ;
    case M2R_HOST_PKT_DROP_CAM :   return (SEL_HOST_PKT_DROP_CAM) ;
    case M2R_HOST_PKT_DROP_CNTRS : return (SEL_HOST_PKT_DROP_CNTRS) ;
    default : return (0) ;  /* error */
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_RMAC_MEMID_TO_MEMSEL(mem_id) ;
  }
#endif
}


#define M2R_RMAC_WATCHDOG_1MS_TMR_CNT  M2R_CORE_CLOCK * 1000
#define M2R_RMAC_WDOG_MICRO_SEC_TO_TMR_CNT(VAL)            \
            ((VAL) * (M2R_RMAC_WATCHDOG_1MS_TMR_CNT / 1000))

#define M2R_RMAC_DQT_1US_TMR_CNT  M2R_CORE_CLOCK / 10
#define M2R_RMAC_DQT_MICRO_SEC_TO_TMR_CNT(VAL)            \
            ((VAL) * M2R_RMAC_DQT_1US_TMR_CNT)

/******* TB *************/
#define M2R_RMAC_PTQ_THRESH_DFT(PTQ_SZ)                   \
            ((PTQ_SZ) * 0x60 / 0x7f)
#define M2R_RMAC_STQ_HTHRESH_BOUNDRY_DEFAULT  0x3

/******* Rate Limiter ***********/
#define M2R_RMAC_MAX_BURST_DEFAULT      12288 /* 1.5 MTU */
/* DELTA value for setting Rate limiter should be divisible by 8 */
#define M2R_RMAC_RATE_LIMIT_DELTA_BASE  8

/******* Host ***********/
#define M2R_RMAC_HOST_MAX_NUM_OF_CHANNEL 4

#define M2R_RMAC_CTL_SEND_BUF_TOTAL_WORDS  (M2R_RMAC_CTL_SEND_BUF_SZ /8)
#define M2R_RMAC_CTL_RX_BUF_TOTAL_WORDS    (M2R_RMAC_CTL_RX_BUF_SZ /8)
#define M2R_RMAC_IPS_SEND_BUF_TOTAL_WORDS  (M2R_RMAC_IPS_SEND_BUF_SZ /8)
#define M2R_RMAC_IPS_RX_BUF_TOTAL_WORDS    (256)
#define M2R_RMAC_RX_BUF                     0
#define M2R_RMAC_SEND_BUF                   1
#define M2R_RMAC_CTL_PKT                    0
#define M2R_RMAC_IPS_PKT		    1
/****** MS *************/
/* The followings are the macro definition of "rateCoef" values in *
 * power of 2 used by CORTINA driver for easy reference            */
#define M2R_MS_RPR_RATECOEF_TWO            1
#define M2R_MS_RPR_RATECOEF_ONE_HALF      -1
#define M2R_MS_RPR_RATECOEF_ONE_FOURTH    -2
#define M2R_MS_RPR_RATECOEF_ONE_EIGHTH    -3
#define M2R_MS_RPR_RATECOEF_ERROR         -120

/* Convert RPR Draft suggested "rateCoef" to CORTINA ucode form */
#define M2R_MS_GET_UCODE_RATECOEFF(RATECOEFF)      ((RATECOEFF)+3)
#define M2R_MS_GET_RPR_RATECOEFF(UCODE_RATECOEFF)  ((UCODE_RATECOEFF)-3)

#define M2R_MS_SET_REG_DATA(WRD, MSB, LSB, VAL)       \
            ((~(((1<<((MSB)-(LSB)+1))-1)<<(LSB))&(WRD)) \
            |((((1<<((MSB)-(LSB)+1))-1)&(VAL))<<(LSB)))
#define M2R_MS_GET_UCODE_AGED_TIMER(TIMER_MS, AGING_US) \
            ( (TIMER_MS) * 1000 / (AGING_US) )

CS_INLINE cs_uint32 M2R_GET_KBPS_LR_RATE(m2r_port_line_rate_t line_rate)
{
  switch (line_rate) {
    case M2R_PORT_RATE_OC192C :
    case M2R_PORT_RATE_OC192 :
        return (M2R_MS_LR_KBPS_OC192) ;
    case M2R_PORT_RATE_10G :
        return (M2R_MS_LR_KBPS_10GE) ;
    case M2R_PORT_RATE_OC48C :
    case M2R_PORT_RATE_OC48 :
        return (M2R_MS_LR_KBPS_OC48) ;
  }

  return (0);

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_KBPS_LR_RATE(line_rate) ;
  }
#endif
}

/* This inline function returns "rateCoef" referenced in RPR standard */
/* in power of 2 and could be negative value based on different       */
/* physical line rate.                                                */
CS_INLINE cs_int8 M2R_GET_MS_RATECOEFF(m2r_port_line_rate_t line_rate)
{
  switch (line_rate) {
    case M2R_PORT_RATE_OC192C :
    case M2R_PORT_RATE_OC192 :
        return (M2R_MS_RPR_RATECOEF_TWO) ;
    case M2R_PORT_RATE_10G :
        return (M2R_MS_RPR_RATECOEF_TWO) ;
    case M2R_PORT_RATE_OC48C :
    case M2R_PORT_RATE_OC48 :
        return (M2R_MS_RPR_RATECOEF_ONE_HALF) ;
  }

  return (M2R_MS_RPR_RATECOEF_ERROR);

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_MS_RATECOEFF(line_rate) ;
  }
#endif
}

/* This inline function calculate the default DQT_A threshold based */
/* on the equation 7.18 in RPR IEEE standard 802.17-2004:           */
/*   Min(Round100(2 * numStations * mtuSize * 8 / lineRate, 25.5)   */
CS_INLINE cs_uint32 M2R_MS_GET_DEFAULT_DQT_A(cs_uint32 lr_kbps,
                                             cs_uint16 mtu_size,
                                             cs_uint8 numStations)
{
   cs_uint32 timer_A ;

   /* round at 100us */
   timer_A = ((2 * numStations * mtu_size * 8) * 10) / lr_kbps;
   timer_A *= 100; /* Convert to us */

   if (timer_A < M2R_MS_RPR_DQT_A_MAX) {
     return (timer_A) ;
   } else {
     return (M2R_MS_RPR_DQT_A_MAX);
   }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_MS_GET_DEFAULT_DQT_A(lr_kbps, mtu_size, numStations) ;
  }
#endif
}

extern
cs_status m2r_ms_get_running_cfg(cs_uint16 port_id,
                                 m2r_rmac_ms_runcfg_t * pCfg);
extern
cs_status m2r_ms_dump_running_cfg(cs_uint16 port_id,
                                  m2r_rmac_runcfg_t * pRmacRunCfg);

extern
m2r_rmac_ms_mode_t m2r_ms_get_running_mode(cs_uint16 port_id) ;

extern
cs_status  m2r_rmac_init_stats(cs_uint16 port_id,
			       m2r_port_mac_mode_t mac_mode) ;



#endif /* __M2R_RMAC_PRIV_H__ */







