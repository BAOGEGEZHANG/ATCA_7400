/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_ms.c
 *
 * M2R driver libraries pertaining to Micro-sequencer in RingMAC block.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_rmac_priv.h"
#include "m2r_rmac.h"
#include "m2r_rmac_ms.h"

static cs_status m2r_ms_ctl_new_ttl_to_congest_sema_out(cs_uint16 port_id,
                                                m2r_ctl_t ctl);
static cs_status  m2r_ms_set_fa_pkt_type(cs_uint16 port_id,
                                  cs_boolean long_pkt,
                                  cs_uint8 long_pkt_length);
static cs_status  m2r_ms_set_fa_algorithm(cs_uint16 port_id, 
                                          cs_boolean conserv);

/************* Initialization ***********************************/
/****************************************************************/
/* $rtn_hdr_start    MICRO-SEQUENCER INITIALIZATION             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_init_cfg(cs_uint16 port_id,
			   m2r_port_summ_t * pSumm, 
			   m2r_rmac_ms_cfg_t * pCfg) 
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port configuration summary struct  */
/*              o Pointer to Micro-sequencer config structure   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the Micro-sequencer block of the specified  */
/* port with the given configuration.                           */
/*                                                              */
/* The [pSumm] parameter is a pointer to the configuration      */
/* Summary section, which has the caller provided config summary*/
/* info. This is used in returning the appropriate default      */
/* configuration for the Micro-sequencer block.                 */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/*                                                              */
/* Note: In order to make Micro-sequencer work correctly, the   */
/* caller also need to set up correctly the thresholds of TB    */
/* and ttl value inserted into fairness packet.                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  m2r_rmac_ms_mode_t    mode ;
  cs_uint32             unresvd_rate, lr_rate ;
  cs_uint16             agingInterval ;
  cs_uint64             tmp1 ;
  
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, 
                  ": RingMAC Micro-sequencer cfg\n") ;
    return(CS_ERROR) ;
  }
    
  if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }

  if (((pSumm->mac_mode == M2R_PORT_IN_SRP_MODE)
      &&(pSumm->rmac_ms_mode != M2R_MS_SRP)) ||
      ((pSumm->mac_mode == M2R_PORT_IN_RPR_MODE)
      &&((pSumm->rmac_ms_mode == M2R_MS_SRP)
        ||(pSumm->rmac_ms_mode == M2R_MS_UNKNOWN)))) {
    CS_HNDL_ERROR(port_id, EM2R_MS_UNMATCHED_CFG_MODE, NULL) ;
    return(CS_ERROR) ;
  }

  /*
   * When agingInterval is changed, some config parameters need
   * to be changed accordingly
   */
  agingInterval = pCfg->t0_cnt / (M2R_CORE_CLOCK / 10) ;

  tmp1 = (cs_uint64) agingInterval
         * M2R_GET_KBPS_LR_RATE(pSumm->line_rate) ;
  lr_rate = (cs_uint32)(tmp1 / (1000 * 8)) ;

  /* prov0/1 is configured as lr_rate of bytes per ageCoef
     agingInterval for pre-initialized RPR mode of Micro-code */
  if ( pSumm->rmac_ms_mode == M2R_MS_SRP ) {
    pCfg->prov1 = ((lr_rate) & 0xffff0000) >> 16 ;
    pCfg->prov0 = (lr_rate) & 0x0000ffff ;
  } else {
    pCfg->prov1 = ((lr_rate << pCfg->agecoeff) & 0xffff0000) >> 16 ;
    pCfg->prov0 = (lr_rate << pCfg->agecoeff) & 0x0000ffff ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /*
   * stall MS before programming it
   */
  if ( m2r_ms_ctl_stall(port_id, M2R_ENABLE) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /*
   * program base registers
   */
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE0_TYPE.wrd, 
               M2R_MS_DR_BASE_TYPE) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE0_INST.wrd, 0 ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE1_TYPE.wrd, 
               M2R_MS_DR_BASE_TYPE) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE1_INST.wrd, 1 ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE2_TYPE.wrd, 
               M2R_MS_DR_BASE_TYPE) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE2_INST.wrd, 2 ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE3_TYPE.wrd, 
               M2R_MS_DR_BASE_TYPE) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.BASE3_INST.wrd, 3 ) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  /*
   * determine operating mode
   */
  mode = pSumm->rmac_ms_mode ;

  /*
   * setup Timer value but keep them disabled
   */
  if (m2r_ms_set_timer_val(port_id, M2R_DISABLE, pCfg->t0_cnt,
                       M2R_DISABLE, pCfg->t1_cnt) == CS_ERROR) {
  	return (CS_ERROR) ;
  }
  
  /*
   * set operating mode and timer mode
   */
  if (mode != M2R_MS_SRP) {
    if (m2r_ms_set_fa_pkt_type(port_id, pCfg->longFaPacketType,
                               pCfg->faPacketLength) == CS_ERROR) {
      return (CS_ERROR) ;
    }
    if (m2r_ms_set_fa_algorithm(port_id, pCfg->conservMode)
          == CS_ERROR) {
      return (CS_ERROR) ;
    }
  }
  
  if (m2r_ms_set_timer_mode(port_id, pCfg->t0_oneShot,
                             pCfg->t1_oneShot) == CS_ERROR) {
    return (CS_ERROR);
  }
  
  if (m2r_ms_set_mcfair_timer(port_id, pCfg->mcfair_en,
                              pCfg->mcfair_t1, pCfg->mcfair_th) 
        == CS_ERROR) {
    return (CS_ERROR) ;
  }
  
  if (m2r_ms_set_actvwght_timer(port_id, pCfg->actweight_en,
                           pCfg->actweight_t1, pCfg->actweight_th) 
        == CS_ERROR) {
    return (CS_ERROR) ;
  }
  
  if (m2r_ms_set_rtt_timer(port_id, pCfg->rtt_en,
                           pCfg->rtt_t1, pCfg->rtt_th) == CS_ERROR) {
    return (CS_ERROR) ;
  }

  /*
   * provision
   */
  if (m2r_ms_set_mac_sa(port_id,
                        ((pCfg->my_sa.byte5 & 0x00ff) << 8) 
                         | pCfg->my_sa.byte4,
                        ((pCfg->my_sa.byte3 & 0x00ff) << 8) 
                         | pCfg->my_sa.byte2,
                        ((pCfg->my_sa.byte1 & 0x00ff) << 8) 
                         | pCfg->my_sa.byte0) == CS_ERROR) {
    return (CS_ERROR) ;
  }  
  
  if ( m2r_rmac_set_srp_ttl(port_id, FALSE, 
                   pCfg->ttl_prov) == CS_ERROR ) {
    return (CS_ERROR);
  }
  
  if (m2r_ms_set_ttl_to_wrap(port_id, pCfg->ttl_to_wrap) 
        == CS_ERROR) {
    return (CS_ERROR) ;
  }

  if (m2r_ms_set_prov(port_id, pCfg->prov1,
                      pCfg->prov0) == CS_ERROR) {
    return (CS_ERROR) ;
  }
  
  if (m2r_ms_set_max_rcv_rate(port_id, pCfg->max_rcv_rate) 
        == CS_ERROR) {
    return (CS_ERROR) ;
  }
  
  if (m2r_ms_set_fa_coeff1(port_id, pCfg->agecoeff,
                           pCfg->ratecoeff) 
        == CS_ERROR) {
    return (CS_ERROR) ;
  }

  if (m2r_ms_set_fa_lp_coeff(port_id, pCfg->lp_allow,
                pCfg->lp_fwd, pCfg->lp_mu, pCfg->lp_nrXmit) 
        == CS_ERROR) {
    return (CS_ERROR) ;
  }
  
  if (mode == M2R_MS_SRP) {
    unresvd_rate = ((((cs_uint32)pCfg->prov1 << 16) | pCfg->prov0) 
                     << pCfg->agecoeff) 
                 * (100-pCfg->resv_rate) / 100 ;

    if (m2r_ms_set_rsvd_msg(port_id, pCfg->rsvd_msg) == CS_ERROR) {
      return (CS_ERROR) ;
    }

    if (m2r_ms_set_srp_stq_thresh(port_id, pCfg->stqHighThresh,
                                  pCfg->stqLowThresh)
        == CS_ERROR) {
      return (CS_ERROR) ;
    }
  } else {
    unresvd_rate = (((cs_uint32)pCfg->prov1 << 16) | pCfg->prov0) 
                 * (100-pCfg->resv_rate) / 100 ;

    if (m2r_ms_set_fa_coeff2(port_id, pCfg->rampUpcoeff,
                             pCfg->rampDncoeff, pCfg->starvecoeff)
        == CS_ERROR) {
      return (CS_ERROR) ;
    }

    if (m2r_ms_set_fa_weight(port_id, pCfg->my_weight) == CS_ERROR) {
      return (CS_ERROR) ;
    }

    if (m2r_ms_set_fa_keepalive_max_timer(port_id,
                                          pCfg->keepAliveMaxTimer)
        == CS_ERROR) {
      return (CS_ERROR) ;
    }
          
    if (m2r_ms_ctl_fa_conserv_active_weight(port_id,
                                            pCfg->Active_W_En)
        == CS_ERROR) {
      return (CS_ERROR) ;
    }

    if (m2r_ms_set_fa_pkt_we_field(port_id,
                                   pCfg->wrappingProtection)
        == CS_ERROR) {
      return (CS_ERROR) ;
    }

    if (m2r_ms_ctl_gen_mc_fa(port_id, pCfg->Gen_MCFF)
        == CS_ERROR) {
      return (CS_ERROR) ;
    }

    if ((mode == M2R_MS_RPR_AGGRESSIVE_DUAL_TB)
        ||(mode == M2R_MS_RPR_CONSERVATIVE_DUAL_TB)) {
      if (m2r_ms_set_rpr_stq_thresh(port_id, pCfg->stqHighThresh,
                                    pCfg->stqLowThresh, 
                                    pCfg->stqMedThresh,
                                    pCfg->stqFullThresh)
          == CS_ERROR) {
        return (CS_ERROR) ;
      }
    } else {
      if (m2r_ms_set_rate_thresh(port_id, 
                                (pCfg->rate_high_th) * unresvd_rate / 100,
                                (pCfg->rate_low_th) * unresvd_rate / 100) 
          == CS_ERROR) {
        return (CS_ERROR) ;
      }

      if (m2r_rmac_set_onetb_queue_delay_timer(port_id, M2R_SERVICE_CLASS_A,
                                pCfg->classAAccessDelayTimerThreshold)
          == CS_ERROR) {
        return (CS_ERROR) ;
      }
      if (m2r_rmac_set_onetb_queue_delay_timer(port_id, M2R_SERVICE_CLASS_B,
                                pCfg->classBAccessDelayTimerThreshold)
          == CS_ERROR) {
        return (CS_ERROR) ;
      }
      if (m2r_rmac_set_onetb_queue_delay_timer(port_id, M2R_SERVICE_CLASS_C,
                                pCfg->classCAccessDelayTimerThreshold)
          == CS_ERROR) {
        return (CS_ERROR) ;
      }
    }
  }
 
  if (m2r_ms_set_unresv_rate(port_id, unresvd_rate) 
        == CS_ERROR) {
    return (CS_ERROR) ;
  }

  /************************
   *    SRPF Control      *
   ************************/
  if (m2r_rmac_set_srpf_max_allow(port_id, 
                      pCfg->max_allow) == CS_ERROR) {
    return (CS_ERROR);
  }

  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) {
    /* Enable NEW_TTL_TO_CONGEST SEMA interrupt for MILAN2 */
    if (m2r_ms_ctl_new_ttl_to_congest_sema_out(port_id,
                        M2R_ENABLE) == CS_ERROR) {
      return (CS_ERROR);
    }
  }

  /*
   * Load Micro-code
   */
#ifndef RELEASE_PLATFORM
  if (!IN_TEST_ENV() && !IN_SIM_ENV()) {
    if (m2r_ms_load_default(port_id, mode) == CS_ERROR) {
      return (CS_ERROR) ;
    }
  }
#else
  if (m2r_ms_load_default(port_id, mode) == CS_ERROR) {
    return (CS_ERROR) ;
  }
#endif

  /*
   * reset PC
   */
  if (m2r_ms_set_pc(port_id, 0x0000) == CS_ERROR) {
    return (CS_ERROR) ;
  }

  /*
   * enable MS parity check
   */
  if (m2r_ms_ctl_par_chk(port_id, M2R_ENABLE, M2R_ENABLE,
                         M2R_ENABLE) == CS_ERROR) {
    return (CS_ERROR) ;
  }
  
  /*
   * unstall MS
   */
  if ( m2r_ms_ctl_stall(port_id, M2R_DISABLE) == CS_ERROR ) {
    return (CS_ERROR) ;
  } /* unstall_ctl */

  /*
   * enable timer T1, but keep timer T1 disabled
   */
  if (m2r_ms_set_timer_val(port_id, M2R_ENABLE, 0,
                           M2R_DISABLE, 0) == CS_ERROR) {
    return (CS_ERROR) ;
  }

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start  GET DEFAULT MICRO-SEQUENCER CONFIGURATION    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_ms_get_default_cfg(cs_uint16 port_id, 
				 m2r_port_summ_t * pSumm,
				 m2r_rmac_tb_cfg_t * pTBCfg,
				 m2r_rmac_ms_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/*              o Pointer to TB(Transit Buffer) config info     */
/*              o Pointer to Micro-sequencer config structure   */
/* OUTPUTS    : o Micro-sequencer default config                */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the driver's default Micro-sequencer configuration for   */
/* the specified port and for the specified summary config      */
/* info.                                                        */
/*                                                              */
/* The [pSumm] parameter is a pointer to the configuration      */
/* Summary section, which has the caller provided config summary*/
/* info. This is used in returning the appropriate default      */
/* configuration for the Micro-sequencer block.                 */
/*                                                              */
/* The [pTBCfg] parameter is a pointer to the Transit Buffer    */
/* configuration structure, which has the caller provided TB    */
/* config information. This is used in returning the appropriate*/
/* default configuration for the Micro-sequencer block.         */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure. The API will fill in this structure with     */
/* default settings based on the config summary info.           */
/*                                                              */
/* The API expects both datastrutues(pSumm and pCfg) already    */
/* allocated, and the config summary filled in pSumm.           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32 lr_rate = M2R_MS_PROV0_DEFAULT;
  cs_uint16 agingInterval;
  cs_uint64 tmp1, tmp2;
  
  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, 
                  ": RingMAC Micro-sequencer cfg\n") ;
    return(CS_ERROR) ;
  }
    
  if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }

  if (((pSumm->mac_mode == M2R_PORT_IN_SRP_MODE)
      &&(pSumm->rmac_ms_mode != M2R_MS_SRP)) ||
      ((pSumm->mac_mode == M2R_PORT_IN_RPR_MODE)
      &&((pSumm->rmac_ms_mode == M2R_MS_SRP)
         ||(pSumm->rmac_ms_mode == M2R_MS_UNKNOWN)))) {
    CS_HNDL_ERROR(port_id, EM2R_MS_UNMATCHED_CFG_MODE, NULL) ;
    return(CS_ERROR) ;
  }

  /* Only short format of fairness packet is what RPR standard */
  /*   addresses                                               */
  pCfg->longFaPacketType = FALSE ;
  pCfg->faPacketLength = M2R_SHORT_FA_PACKET_LENGTH ;

  pCfg->my_sa.byte5 = M2R_RMAC_DEFAULT_MACSA_BYTE5 ;
  pCfg->my_sa.byte4 = M2R_RMAC_DEFAULT_MACSA_BYTE4 ;
  pCfg->my_sa.byte3 = M2R_RMAC_DEFAULT_MACSA_BYTE3 ;
  pCfg->my_sa.byte2 = M2R_RMAC_DEFAULT_MACSA_BYTE2 ;
  pCfg->my_sa.byte1 = M2R_RMAC_DEFAULT_MACSA_BYTE1 ;
  pCfg->my_sa.byte0 = port_id & 0x00FF ;
  
  pCfg->t0_oneShot = FALSE ;
  pCfg->t1_oneShot = FALSE ;
  
  /* This is required for all MS operation modes */
  pCfg->rtt_en = M2R_DISABLE ;
  pCfg->rtt_t1 = FALSE ;

  /* By default, all RPR MS operation modes enable the      */
  /* generation of Multi-choke fairness packets.            */
  if (pSumm->rmac_ms_mode != M2R_MS_SRP) {
    pCfg->Gen_MCFF = M2R_ENABLE ;
    pCfg->mcfair_en = M2R_ENABLE ;
    pCfg->mcfair_t1 = TRUE ;
    pCfg->mcfair_th = M2R_MS_MCFAIR_TH_DEFAULT ;
  } else {
    pCfg->mcfair_en = M2R_DISABLE ;
    pCfg->mcfair_t1 = TRUE ;
    pCfg->mcfair_th = M2R_MS_MCFAIR_MAX_TH ;
    pCfg->actweight_en = M2R_DISABLE ;
    pCfg->actweight_t1 = FALSE ;
    pCfg->actweight_th = M2R_MS_ACTWEIGHT_MAX_TH ;
    pCfg->rtt_th = M2R_MS_RTT_MAX_TH ;
  }

  pCfg->wrappingProtection = TRUE ;
  pCfg->rsvd_msg = M2R_MS_RSVD_MSG_DEFAULT ;
  pCfg->ttl_to_wrap = M2R_MS_TTL_TO_WRAP_DEFAULT ;
  pCfg->ttl_prov = M2R_MS_TTL_PROV_DEFAULT ;
  pCfg->max_rcv_rate = M2R_MS_MAX_RCV_RATE ;
  pCfg->my_weight = M2R_MS_MY_WEIGHT_DEFAULT ;

  pCfg->rampUpcoeff = M2R_MS_RAMPUPCOEFF_DEFAULT;
  pCfg->rampDncoeff = M2R_MS_RAMPDNCOEFF_DEFAULT; 
  pCfg->starvecoeff = M2R_MS_STARVECOEFF_DEFAULT;

  if ( pSumm->rmac_ms_mode == M2R_MS_SRP ) {
    pCfg->agecoeff = M2R_MS_AGECOEFF_DEFAULT;
    pCfg->lp_allow = M2R_MS_LP_ALLOW_DEFAULT ;
    pCfg->lp_fwd = M2R_MS_LP_FWD_DEFAULT ;
    pCfg->lp_mu = M2R_MS_LP_MU_DEFAULT ;
    pCfg->lp_nrXmit = M2R_MS_LP_NRXMIT_DEFAULT ;
  } else {
    pCfg->agecoeff = M2R_MS_RPR_AGECOEFF_DEFAULT;
    pCfg->lp_allow = M2R_MS_RPR_LPCOEFF_DEFAULT ;
    pCfg->lp_fwd = M2R_MS_RPR_LPCOEFF_DEFAULT ;
    pCfg->lp_mu = M2R_MS_RPR_LPCOEFF_DEFAULT ;
    pCfg->lp_nrXmit = M2R_MS_RPR_LPCOEFF_DEFAULT ;
  }

  /************* SRPF Control **************/
  pCfg->max_allow = M2R_MS_MAX_ALLOW_DEFAULT;

  /* According to RPR standard, only OC3 line rate 
     uses M2R_MS_SLOW_AGING_INTERVAL as aging
     interval, which is not supported by 
     MILAN2 */
  agingInterval = M2R_MS_FAST_AGING_INTERVAL;
  pCfg->ratecoeff = M2R_GET_MS_RATECOEFF(pSumm->line_rate);

  pCfg->t0_cnt = agingInterval * (M2R_CORE_CLOCK / 10) ;

  /* Comment this out to support MP without floating capability
  pCfg->t1_cnt = (cs_uint32)(( (pCfg->faPacketLength * 8.0) 
                  /((M2R_GET_KBPS_LR_RATE(pSumm->line_rate)
                     /1000.0) 
                   * M2R_MS_ADVERTISEMENT_RATIO_DEFAULT))
                  * (M2R_CORE_CLOCK / 10) ) ;
  lr_rate = (cs_uint32)(agingInterval
                       * (M2R_GET_KBPS_LR_RATE(pSumm->line_rate)
                          /1000.0)
                       / 8) ;
   */
  tmp1 = (cs_uint64) pCfg->faPacketLength * 8 * 1000 
         * (M2R_CORE_CLOCK / 10) * 100000 ;
  tmp2 = (cs_uint64) M2R_GET_KBPS_LR_RATE(pSumm->line_rate) 
         * M2R_MS_ADVERTISEMENT_RATIO_DEFAULT ;
 
  pCfg->t1_cnt = (cs_uint32)(tmp1 / tmp2) ;
  
  tmp1 = (cs_uint64) agingInterval 
         * M2R_GET_KBPS_LR_RATE(pSumm->line_rate) ;
		 
  lr_rate = (cs_uint32)(tmp1 / (1000 * 8)) ;
                       
  /* prov0/1 is configured as lr_rate of bytes per ageCoef 
     agingInterval for pre-initialized RPR mode of Micro-code */
  if ( pSumm->rmac_ms_mode == M2R_MS_SRP ) {
    pCfg->prov1 = ((lr_rate) & 0xffff0000) >> 16 ;
    pCfg->prov0 = (lr_rate) & 0x0000ffff ;
  } else {
    pCfg->prov1 = ((lr_rate << pCfg->agecoeff) & 0xffff0000) >> 16 ;
    pCfg->prov0 = (lr_rate << pCfg->agecoeff) & 0x0000ffff ;
    pCfg->rtt_th 
      = M2R_MS_GET_UCODE_AGED_TIMER(M2R_MS_FRTT_TIME_MSEC_DEFAULT,
                                    agingInterval);
    pCfg->keepAliveMaxTimer
      = M2R_MS_GET_UCODE_AGED_TIMER(M2R_MS_KEEPALIVE_TIMEOUT_MSEC_DEFAULT,
                                    agingInterval);
  }

  pCfg->resv_rate = 0 ;
  
  switch (pSumm->rmac_ms_mode) {
    case M2R_MS_SRP:
        pCfg->conservMode = FALSE ;

        if (pTBCfg->one_tb == TRUE) {
          CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
             ": Single TB cfg is not supported by M2R_MS_SRP!\n") ;
          return(CS_ERROR) ;
        }
        pCfg->stqHighThresh = pTBCfg->stq_sz 
                              - (M2R_RMAC_SRP_MTU / 256) ;
        pCfg->stqLowThresh = pCfg->stqHighThresh / 4 ;

        /* Disable Delay Queue Timer */
        pCfg->classAAccessDelayTimerThreshold = 0 ;
        pCfg->classBAccessDelayTimerThreshold = 0 ;
        pCfg->classCAccessDelayTimerThreshold = 0 ;

        break ;
    case M2R_MS_RPR_AGGRESSIVE_DUAL_TB:
        pCfg->conservMode = FALSE ;
        
        if (pTBCfg->one_tb == TRUE) {
          CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
             ": Single TB cfg is not supported by M2R_MS_RPR_AGGRESSIVE_DUAL_TB!\n") ;
          return(CS_ERROR) ;
        }

        pCfg->stqFullThresh = pTBCfg->stq_sz - (M2R_RMAC_RPR_MTU / 256) ;
        pCfg->stqHighThresh = pCfg->stqFullThresh / 4 ;
        pCfg->stqLowThresh = pCfg->stqHighThresh / 2 ;
        pCfg->stqMedThresh = (pCfg->stqHighThresh
                             + pCfg->stqLowThresh) / 2 ;

        /* Disable Delay Queue Timer */
        pCfg->classAAccessDelayTimerThreshold = 0 ;
        pCfg->classBAccessDelayTimerThreshold = 0 ;
        pCfg->classCAccessDelayTimerThreshold = 0 ;

        /* For aggressive rate adjustment, active weight should */
        /* not be enabled.                                      */                                     
        pCfg->Active_W_En = M2R_DISABLE ;
        pCfg->actweight_en = M2R_DISABLE ;
  	pCfg->actweight_t1 = FALSE ;
  	pCfg->actweight_th = M2R_MS_ACTWEIGHT_MAX_TH ;

        break ;
    case M2R_MS_RPR_AGGRESSIVE_SINGLE_TB:
        pCfg->conservMode = FALSE ;
        
        if (pTBCfg->one_tb == FALSE) {
          CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
             ": Dual TB cfg is not supported by M2R_MS_RPR_AGGRESSIVE_SINGLE_TB!\n") ;
          return(CS_ERROR) ;
        }

        pCfg->rate_high_th = 95 ;
        pCfg->rate_low_th = 90 ;

        pCfg->classAAccessDelayTimerThreshold 
          = M2R_MS_GET_DEFAULT_DQT_A(M2R_GET_KBPS_LR_RATE(pSumm->line_rate),
                                     M2R_RMAC_RPR_MTU, pCfg->ttl_prov) ;
        pCfg->classBAccessDelayTimerThreshold = M2R_MS_DQT_B_DEFAULT ;
        pCfg->classCAccessDelayTimerThreshold = M2R_MS_DQT_C_DEFAULT ;
        
        /* For aggressive rate adjustment, active weight should */
        /* not be enabled.                                      */                                     
        pCfg->Active_W_En = M2R_DISABLE ;
        pCfg->actweight_en = M2R_DISABLE ;
  	pCfg->actweight_t1 = FALSE ;
  	pCfg->actweight_th = M2R_MS_ACTWEIGHT_MAX_TH ;

        break ;
    case M2R_MS_RPR_CONSERVATIVE_DUAL_TB:
        pCfg->conservMode = TRUE ;
        
        if (pTBCfg->one_tb == TRUE) {
          CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
             ": Single TB cfg is not supported by M2R_MS_RPR_AGGRESSIVE_DUAL_TB!\n") ;
          return(CS_ERROR) ;
        }

        pCfg->stqFullThresh = pTBCfg->stq_sz - (M2R_RMAC_RPR_MTU / 256) ;
        pCfg->stqHighThresh = pCfg->stqFullThresh / 4 ;
        pCfg->stqLowThresh = pCfg->stqHighThresh / 2 ;
        pCfg->stqMedThresh = (pCfg->stqHighThresh
                             + pCfg->stqLowThresh) / 2 ;

        /* Disable Delay Queue Timer */
        pCfg->classAAccessDelayTimerThreshold = 0 ;
        pCfg->classBAccessDelayTimerThreshold = 0 ;
        pCfg->classCAccessDelayTimerThreshold = 0 ;

        pCfg->Active_W_En = M2R_ENABLE ;
        pCfg->actweight_en = M2R_ENABLE ;
        pCfg->actweight_t1 = FALSE ;
        pCfg->actweight_th = M2R_MS_ACTWEIGHT_TH_DEFAULT ;

        break ;
    case M2R_MS_RPR_CONSERVATIVE_SINGLE_TB:
        pCfg->conservMode = TRUE ;
        
        if (pTBCfg->one_tb == FALSE) {
          CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
             ": Dual TB cfg is not supported by M2R_MS_RPR_CONSERVATIVE_SINGLE_TB!\n") ;
          return(CS_ERROR) ;
        }

        pCfg->rate_high_th = 95 ;
        pCfg->rate_low_th = 90 ;

        pCfg->classAAccessDelayTimerThreshold
          = M2R_MS_GET_DEFAULT_DQT_A(M2R_GET_KBPS_LR_RATE(pSumm->line_rate),
                                     M2R_RMAC_RPR_MTU, pCfg->ttl_prov) ;
        pCfg->classBAccessDelayTimerThreshold = M2R_MS_DQT_B_DEFAULT ;
        pCfg->classCAccessDelayTimerThreshold = M2R_MS_DQT_C_DEFAULT ;
        
        pCfg->Active_W_En = M2R_ENABLE ;
        pCfg->actweight_en = M2R_ENABLE ;
        pCfg->actweight_t1 = FALSE ;
        pCfg->actweight_th = M2R_MS_ACTWEIGHT_TH_DEFAULT ;

        break ;
    case M2R_MS_UNKNOWN:
        CS_HNDL_ERROR(port_id, EM2R_MS_UNKNOWN_CFG_MODE, NULL) ;
        return (CS_ERROR) ;
  }

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    LOAD MICRO-SEQUENCER FROM TABLE            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_load_default(cs_uint16 port_id, 
                               m2r_rmac_ms_mode_t mode)
/* INPUTS     : o Port Id                                       */
/*              o Micro-sequencer Operating Mode                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Load MS microcode and data from pre-initialized table/array. */
/* Load microcode instructions into Program Store SRAM(512 x    */
/* 24bits),and microcode data into Data Store SRAM(64 x 32bits).*/
/*                                                              */
/* [mode] parameter select the following MS operating mode:     */
/* M2R_MS_SRP,                                                  */
/* M2R_MS_RPR_AGGRESSIVE_DUAL_TB,                               */
/* M2R_MS_RPR_AGGRESSIVE_SINGLE_TB,                             */
/* M2R_MS_RPR_CONSERVATIVE_DUAL_TB,                             */
/* M2R_MS_RPR_CONSERVATIVE_SINGLE_TB,                           */
/*                                                              */
/* Note : this API will only write the data to registers to     */
/* load the micro-sequencer. The caller is responsible for      */
/* stalling the micro-sequencer or do any other setup needed    */
/* before calling this.                                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint8      j = (cs_uint8) mode ;
  cs_uint32     i ;
  cs_uint32     data ;
  m2r_rmac_gram_t   gram ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  /* Micro-sequencer program */
  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;

/* Adam on 10/13/03 -> Added for debug */
#if 1
  CS_PRINT("Loading Micro-code program...\n") ;
#endif

  for (i = 0; i < M2R_MS_PGM_STORE_SIZE; i++) {
    data                   = ms_ucode_pgm[j][i] ;
    gram.GRAM_D0.wrd       = (data & 0x0000ffff) >> 0  ;
    gram.GRAM_D1.wrd       = (data & 0xffff0000) >> 16 ;
    gram.GRAM_CR.bf.SEL    = SEL_USEQ_PGM_STORE ;
    gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = i ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
      return(CS_ERROR) ;
    }
  }

  /* Micro-sequencer data */
  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;

/* Adam on 10/13/03 -> Added for debug */
#if 1
  CS_PRINT("Loading Micro-code data...\n") ;
#endif

  for (i = 0; i < M2R_MS_DATA_STORE_SIZE; i++) {
    data                   = ms_ucode_data[j][i] ;
    gram.GRAM_D0.wrd       = (data & 0x0000ffff) >> 0  ;
    gram.GRAM_D1.wrd       = (data & 0xffff0000) >> 16 ;
    gram.GRAM_CR.bf.SEL    = SEL_USEQ_DC_STORE ;
    gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = i ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
      return(CS_ERROR) ;
    }
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}


#ifndef CS_DONT_USE_STDLIB
/****************************************************************/
/* $rtn_hdr_start    LOAD MICROSEQUENCER CODE AND DATA FROM FILE*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_load_pgm_or_data(cs_uint16 port_id,
                                   m2r_rmac_mem_id_t memid, char * fn,
                                   cs_uint32 num_entries)
/* INPUTS     : o Port Id                                       */
/*              o Mem Id(Program Store or Data Store)           */
/*              o File name string                              */
/*              o Number of entries(512 for Pgm, 64 for Data)   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Load MS microcode or data from a text file.                  */
/* Download either microcode instructions into Program Store    */
/* SRAM(512 x 24bits), or microcode data into Data Store        */
/* SRAM(64 x 32bits).                                           */
/*                                                              */
/* The [memid] parameter is either M2R_MS_PGM_STORE or          */
/* M2R_MS_DATA_STORE.                                           */
/*                                                              */
/* Note : this API will only write the data to registers to     */
/* load the micro-sequencer. The caller is responsible for      */
/* stalling the micro-sequencer or do any other setup needed    */
/* before calling this.                                         */
/* By default, the microsequencer program and data will be      */
/* loaded from a pre-initialized table in the driver.           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32     count = 0, data, ret ;
  char          line[M2R_MS_FILE_LINE_SIZE] ;
  FILE          * fp ;
  m2r_rmac_gram_t   gram ;
  cs_status     status = CS_OK ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  fp = fopen(fn, "r") ;
  if (fp == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_MS_FILE_OPEN_ERROR, ": %s\n", fn) ;
    return(CS_ERROR) ;
  }

  CS_MEMSET( (void *) &gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  while (count < num_entries) {
    if ( fgets(line, M2R_MS_FILE_LINE_SIZE, fp) == NULL ) {
      CS_HNDL_ERROR(port_id, EM2R_MS_INVALID_FILE_ENTRIES,
                    ": File(%s) contains only %d valid entries\n", 
                    fn, count) ;;
      status = CS_ERROR ; /* unexpected EOF */
      break ;
    }

    ret = sscanf(line, "%x", &data) ;
    if (ret != 1) {
      continue ; /* skip this line */
    }

    gram.GRAM_D0.wrd       = (data & 0x0000ffff) >> 0  ;
    gram.GRAM_D1.wrd       = (data & 0xffff0000) >> 16 ;
    gram.GRAM_CR.bf.SEL    = M2R_RMAC_MEMID_TO_MEMSEL(memid) ;
    gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
    gram.GRAM_CR.bf.GRAM_A = count ;
    if ( m2r_rmac_write_gram_regs(port_id, &gram)  == CS_ERROR ) {
      status = CS_ERROR ;
      break ;
    }

    count++ ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  fclose(fp) ;
  return(status) ;
}
#endif /* CS_DONT_USE_STDLIB */



/************************ Provision *****************************/
/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER TIMER MODE             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_timer_mode(cs_uint16 port_id, 
                                 cs_boolean t0_oneShot,
                                 cs_boolean t1_oneShot)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(T0 One Shot) or FALSE(T0 Continuous)     */
/*              o TRUE(T1 One Shot) or FALSE(T1 Continuous)     */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the mode of Micro-sequencer timer T0 and T1.            */
/*                                                              */
/* Note: Timer T0 is used to determine the interval for fariness*/
/* algorithm to re-calculate fairness(usage)-related coefficient*/
/* and variables(aging Interval).  Timer T1 is used to determine*/
/* the interval for fairness algorithm to generate the single-  */
/* choke fairness packet(advertisement Interval).               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_TIMER_CR_t      timer_cr;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  timer_cr.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TIMER_CR.wrd) ;

  timer_cr.bf.T0MODE = (t0_oneShot) ? 1 : 0;
  timer_cr.bf.T1MODE = (t1_oneShot) ? 1 : 0;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TIMER_CR.wrd, timer_cr.wrd);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER TIMER                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_timer_val(cs_uint16 port_id, m2r_ctl_t t0_en,
                            cs_uint32 t0_cnt, m2r_ctl_t t1_en,
                            cs_uint32 t1_cnt)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE(1) or M2R_DISABLE(0)               */
/*              o Value for timer T0                            */
/*              o M2R_ENABLE(1) or M2R_DISABLE(0)               */
/*              o Value for timer T1                            */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the counter of Micro-sequencer timer T0 and T1 and      */
/* enable/disable the timer T0/1.                               */
/*                                                              */
/* Note:                                                        */
/*    1. Timer T0 is used to determine the interval for fariness*/
/*       algorithm to re-calculate fairness(usage)-related co-  */
/*       -efficient and variables(aging Interval).              */
/*    2. Timer T1 is used to determine the interval for fairness*/
/*       algorithm to generate the single-choke fairness packet */
/*       (advertisement Interval).                              */
/*    3. In SRP/RPR normal operation modes, calling this API to */
/*       disable Timer T0/T1 is not recommended.                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_TIMER_CR_t      timer_cr;
  cs_boolean            disable = FALSE ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  timer_cr.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TIMER_CR.wrd) ;

  /* Disable Timer first if needed */
  if ( (t0_cnt == 0) && (t1_cnt == 0) ) {
    if ( ((timer_cr.bf.T0ENAB && (t0_en==M2R_ENABLE))
        || (!timer_cr.bf.T0ENAB && (t0_en==M2R_DISABLE)))
        && ((timer_cr.bf.T1ENAB && (t1_en==M2R_ENABLE))
          || (!timer_cr.bf.T1ENAB && (t1_en==M2R_DISABLE))) ) {
      M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
      return (CS_OK) ;
    }
  }
  if ( timer_cr.bf.T0ENAB && (t0_cnt > 0) ) {
    timer_cr.bf.T0ENAB = 0;
    disable = TRUE ;
  }
  if ( timer_cr.bf.T1ENAB && (t1_cnt > 0) ) {
    timer_cr.bf.T1ENAB = 0;
    disable = TRUE ;
  }

  if (disable == TRUE) {
    /* disable Timer */
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TIMER_CR.wrd,
                 timer_cr.wrd);
  }

  /* Since the timer can be explictly disabled, value "0" of
   * the timer counter is not accepted and the old value remains
   * untouched.
   */
  if ( t1_cnt > 0 ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.T1_CNT0.wrd, 
                 (t1_cnt & 0x0000ffff));
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.T1_CNT1.wrd, 
                 ((t1_cnt & 0xffff0000)>>16));
  }
  
  if ( t0_cnt > 0 ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.T0_CNT0.wrd, 
                 (t0_cnt & 0x0000ffff));
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.T0_CNT1.wrd, 
                 ((t0_cnt & 0xffff0000)>>16));
  }
  
  timer_cr.bf.T1ENAB = (t1_en==M2R_ENABLE) ? 1 : 0 ;
  timer_cr.bf.T0ENAB = (t0_en==M2R_ENABLE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TIMER_CR.wrd, timer_cr.wrd);

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER SOURCE MAC ADDRESS     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_mac_sa(cs_uint16 port_id, cs_uint16 sa5_4,
                              cs_uint16 sa3_2, cs_uint16 sa1_0)
/* INPUTS     : o Port Id                                       */
/*              o MAC SA - bytes 5:4                            */
/*              o MAC SA - bytes 3:2                            */
/*              o MAC SA - bytes 1:0                            */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets Source Address for Micro-sequencer to use for fairness  */
/* (usage) packet generation.                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MY_SA0.wrd, sa1_0) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MY_SA1.wrd, sa3_2) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MY_SA2.wrd, sa5_4) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER RESERVED MESSAGE       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_rsvd_msg(cs_uint16 port_id, cs_uint16 data)
/* INPUTS     : o Port Id                                       */
/*              o Reserved message value                        */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets Reserved Message for use by Micro-sequencer.            */
/*                                                              */
/* Note : This API is used by the application to fill the       */
/* reserved message field when generating SRP usage packet, but */
/* in RPR operation mode, this API should not be called when    */
/* using pre-initialized RPR Micro-code along with current M2R  */
/* Driver package !!!                                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd, data) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER TTL-TO-WRAP            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_ttl_to_wrap(cs_uint16 port_id, cs_uint8 distance)
/* INPUTS     : o Port Id                                       */
/*              o Distance to wrap point                        */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets ttl_to_wrap for Micro-sequencer to use for fairness     */
/* (usage) packet generation.                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_TTL_TO_WRAP_t   ttl_to_wrap;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  ttl_to_wrap.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TTL_TO_WRAP.wrd) ;

  ttl_to_wrap.bf.TTL_TO_WRAP = distance;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.TTL_TO_WRAP.wrd, 
               ttl_to_wrap.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER UNRESERVED RATE        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_unresv_rate(cs_uint16 port_id, cs_uint32 data)
/* INPUTS     : o Port Id                                       */
/*              o Unreserved Rate provisioned for this ring     */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Provisions the unreserved rate for the Micro-sequencer to    */
/* correctly generate the fairness and usage packets.           */
/*                                                              */
/* This API should be called again when the total reserved      */
/* bandwidth (subclass A0) is changed after port initialization.*/
/*                                                              */
/* Note: When loading the Micro-sequencer with the Cortina-     */
/* provided Micro-code along with M2R Driver package, the       */
/* unreserved rate value should be in unit of bytes per ageCoef */
/* agingInterval.                                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.UNRESV_RATE1.wrd, 
               ((data & 0x000f0000)>>16));
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.UNRESV_RATE0.wrd, 
               (data & 0x0000ffff));

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER PROV 1/0               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_prov(cs_uint16 port_id, cs_uint16 prov1,
                           cs_uint16 prov0)
/* INPUTS     : o Port Id                                       */
/*              o Value for PROV1 register                      */
/*              o Value for PROV0 register                      */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the value of register PROV1/0.                           */
/*                                                              */
/* Note:                                                        */
/* 1. Register PROV1/0 is used to provision the Max line rate   */
/* (in unit of bytes per ageCoef agingInterval for RPR mode and */
/* in unit of bytes per agingInterval for SRP mode) to the pre- */
/* initialized Micro-code along with M2R Driver package.  For   */
/* other Micro-codes, refer to the corresponding programming    */
/* manual about how to set up register PROV1/0.                 */
/*                                                              */
/* 2. When using Cortina-provided Micro-code along with Driver  */
/* package, if this API needs to be called to re-program a new  */
/* Max line rate value due to AGECOEF change, API 'm2r_ms_set_  */
/* pc' needs to be called following that to restart the Micro-  */
/* code from program counter 0, otherwise, new setting won't    */
/* have effect.                                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PROV1.wrd, prov1);
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PROV0.wrd, prov0);

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER MAX RECEIVE RATE       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_max_rcv_rate(cs_uint16 port_id, cs_uint32 data)
/* INPUTS     : o Port Id                                       */
/*              o Max Received Rate                             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the max received rate used by the Micro-code.           */
/*                                                              */
/* Note: This API is provided for customized use in the Micro-  */
/* code.  In the current version of pre-initialzed Micro-code   */
/* along with M2R Driver package, this API has no effect.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAX_RCV_RATE1.wrd, 
               ((data & 0x000f0000)>>16));
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAX_RCV_RATE0.wrd, 
               (data & 0x0000ffff));

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start  SET MICRO-SEQUENCER RATE_HIGH/LOW THRESHOLD  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_rate_thresh(cs_uint16 port_id, cs_uint32 rate_h,
                                  cs_uint32 rate_l)
/* INPUTS     : o Port Id                                       */
/*              o Outbound link rate high threshold             */
/*              o Outbound link rate low threshold              */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the outbound link rate high and low threshold.          */
/*                                                              */
/* Note: This API is used to provision the RATE_HIGH/RATE_LOW   */
/* parameters used by RPR operating modes of pre-initialized    */
/* Micro-code for SINGLE TB configuration.  The threshold unit  */
/* is bytes per ageCoef agingInterval.  For the other version of*/
/* Micro-codes, refer to the corresponding programming manual   */
/* about how to use these two values.                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RATE_HIGH_TH1.wrd, 
               ((rate_h & 0x000f0000)>>16));
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RATE_HIGH_TH0.wrd, 
               (rate_h & 0x0000ffff));

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RATE_LOW_TH1.wrd, 
               ((rate_l & 0x000f0000)>>16));
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RATE_LOW_TH0.wrd, 
               (rate_l & 0x0000ffff));

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start      SET MICRO-SEQUENCER RPR STQ THRESHOLD    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_rpr_stq_thresh(cs_uint16 port_id, 
                                     cs_uint16 stqHighThresh,
                                     cs_uint16 stqLowThresh, 
                                     cs_uint16 stqMedThresh,
                                     cs_uint16 stqFullThresh)
/* INPUTS     : o Port Id                                       */
/*              o High threshold of STQ                         */
/*              o Low threshold of STQ                          */
/*              o Medium threshold of STQ                       */
/*              o Full threshold of STQ                         */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the RPR standard compliant STQ thresholds.  They are all*/
/* in unit of 256 bytes.                                        */
/*                                                              */
/* When STQ goes beyond [stqHighThresh], no fairness eligible   */
/* packets can be added from the HOST.  This corresponds to     */
/* the medium threshold of STQ in MILAN2 hardware imple-        */
/* -mentation.                                                  */
/*                                                              */
/* When STQ goes below [stqLowThresh], the add rate of fairness */
/* eligible packets from the HOST can be ramped up.             */
/*                                                              */
/* When STQ goes beyond [stqMedThresh], the add rate of fairness*/
/* eligible packets from the HOST should be ramped down.        */
/*                                                              */
/* When STQ comes close to [stqFullThresh], no packets can be   */
/* added from the HOST.                                         */
/*                                                              */
/* Note:                                                        */
/* 1. This API is used to provision RPR standard compliant      */
/* variables which are then translated into the appropriate     */
/* MILAN2 implementation register settings.                     */
/* 2. This API requires the support of CORTINA micro-codes.     */
/* 3. When Micro-sequencer is running, calling this API will not*/
/* have effect unless calling API 'm2r_ms_set_pc' to restart    */
/* Cortina-provided RPR Micro-code from program counter 0 after */
/* this API.  However, during the restart of Micro-code, the low*/
/* priority packets from the host will be back-pressured.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  if ( m2r_rmac_set_stq_thresh(port_id, stqFullThresh,
			       stqFullThresh, stqHighThresh)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SPARE_REG1.wrd, 
               stqLowThresh) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SPARE_REG0.wrd, 
               stqMedThresh) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start      SET MICRO-SEQUENCER SRP STQ THRESHOLD    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_srp_stq_thresh(cs_uint16 port_id, 
                                     cs_uint16 stqHighThresh,
                                     cs_uint16 stqLowThresh)
/* INPUTS     : o Port Id                                       */
/*              o High threshold of STQ                         */
/*              o Low threshold of STQ                          */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the SRP standard compliant STQ thresholds.  They are all*/
/* in unit of 256 bytes.                                        */
/*                                                              */
/* When STQ comes close to [stqHighThresh], no packets can be   */
/* added from the HOST.                                         */
/*                                                              */
/* When STQ goes beyond [stqLowThresh], no fairness eligible    */
/* packets can be added from the HOST.  This corresponds to     */
/* the medium threshold of STQ in MILAN2 hardware imple-        */
/* -mentation.                                                  */
/*                                                              */
/* Note: This API is used to provision RPR standard compliant   */
/* variables which are then translated into the appropriate     */
/* MILAN2 implementation register settings.                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  return ( m2r_rmac_set_stq_thresh(port_id, stqHighThresh,
			       stqLowThresh, stqLowThresh) );
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER FAIRNESS WEIGHT        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_fa_weight(cs_uint16 port_id, cs_uint8 weight)
/* INPUTS     : o Port Id                                       */
/*              o Fairness weight of my station                 */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the fairness weight for my station.                     */
/*                                                              */
/* Note:                                                        */
/* 1. The real value of coefficients used by the Micro-code     */
/* is the power of two.                                         */
/* 2. When Micro-sequencer is running, calling this API will not*/
/* have effect unless calling API 'm2r_ms_set_pc' to restart    */
/* Cortina-provided RPR Micro-code from program counter 0 after */
/* this API.  However, during the restart of Micro-code, the low*/
/* priority packets from the host will be back-pressured.       */
/* 3. When deploying conservative RPR fairness algorithm with   */
/* activeWeight enabled, it is user application's responsibility*/
/* to pass the updated local weight information to the other    */
/* stations on the ring in order to update the corresponding    */
/* information on those stations accordingly.                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_MY_WEIGHT_t     my_weight;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  my_weight.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_WEIGHT.wrd) ;

  my_weight.bf.MY_WEIGHT = weight;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MY_WEIGHT.wrd, 
               my_weight.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER COEFFICIENTS 1         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_fa_coeff1(cs_uint16 port_id, cs_uint8 agecoeff,
                                cs_int8 ratecoeff)
/* INPUTS     : o Port Id                                       */
/*              o Set AGECOEFF of fairness algorithm            */
/*              o Set RATECOEFF of fairness algorithm           */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the basic fairness algorithm related coefficients.      */
/*                                                              */
/* For SRP application, only AGECOEFF is used and it corresponds*/
/* to 'ageCoef' referenced in SRP draft.                        */
/*                                                              */
/* For RPR application, these two coefficients correspond to    */
/* 'ageCoef' and 'rateCoef' referenced in RPR standard.  When   */
/* deploying AGGRESSIVE fairness rate adjustment, besides this  */
/* API, another API 'm2r_ms_set_fa_coeff2' is also needed to    */
/* provision another coefficient 'rampUpCoef' referenced in RPR */
/* standard.  When deploying CONSERVATIVE fairness rate adjust- */
/* -ment, both APIs are needed to provision all required coeff- */
/* -icients.                                                    */
/*                                                              */
/* Note:                                                        */
/* 1. The real value of coefficients used by the Micro-code     */
/* is the power of two, i.e., [ratecoeff] could be negative.    */
/*                                                              */
/* 2. When Micro-sequencer is running, calling this API will not*/
/* have effect unless calling API 'm2r_ms_set_pc' to restart    */
/* Cortina-provided SRP/RPR Micro-code from program counter 0   */
/* after this API.  However, during the restart of Micro-code,  */
/* the low priority packets from the host will be back-pressured.*/
/*                                                              */
/* 3. When AGECOEFF is changed, before restarting Micro-code,   */
/* for SRP application, unresvd_rate needs to be re-calculated  */
/* and then re-programmed by calling API 'm2r_ms_set_unresv_rate';*/
/* for RPR application, both line rate provision and unresvd_rate*/
/* need to be re-calculated and re-programmed by calling API    */
/* 'm2r_ms_set_prov' and API 'm2r_ms_set_unresv_rate'.          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_COEFF_t         coeff ;
  DR_MS_COEFF2_t        coeff2 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if ( (agecoeff > 0xf)
       || (ratecoeff > M2R_MS_GET_RPR_RATECOEFF(0xf)) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
     ": both <agecoeff> and <ratecoeff> should not be greater than 0xf!\n") ;
    return(CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  coeff.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF.wrd) ;

  coeff.bf.AGECOEFF = agecoeff ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.COEFF.wrd, 
               coeff.wrd) ;

  coeff2.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF2.wrd) ;

  coeff2.bf.RATECOEFF = M2R_MS_GET_UCODE_RATECOEFF(ratecoeff) ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.COEFF2.wrd, 
               coeff2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER COEFFICIENTS 2         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_fa_coeff2(cs_uint16 port_id, cs_uint8 rampUpcoeff,
                                cs_uint8 rampDncoeff,
                                cs_uint8 starvecoeff)
/* INPUTS     : o Port Id                                       */
/*              o Set RAMPCOEFF of fairness algorithm           */
/*              o Set RampDownCoeff of fairness algorithm       */
/*              o Set STARVECOEFF of fairness algorithm         */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the other RPR coefficients.                             */
/*                                                              */
/* [rampUpcoeff] and [rampDncoeff] correspond to 'rampUpCoef'   */
/* and 'rampDnCoef' referenced in RPR standard.  Please note    */
/* that 'rampDnCoef' is only used for conservative rate adjust- */
/* -ment.                                                       */
/*                                                              */
/* [starvecoeff] is used to calculate 'starveFactor' referenced */
/* in RPR standard. Here is the equation:                       */
/*   'starveFactor' = 1/2**[starvecoeff]                        */
/* [starvecoeff] takes value from [0, 7] according to standard. */
/* Please note that 'starveFactor' is only used for conservative*/
/* rate adjustment.                                             */
/*                                                              */
/* Note:                                                        */
/* 1. The real value of [rampUpcoeff] and [rampDncoeff] used    */
/* by the Micro-code is the power of two.                       */
/* 2. When Micro-sequencer is running, calling this API will not*/
/* have effect unless calling API 'm2r_ms_set_pc' to restart    */
/* Cortina-provided RPR Micro-code from program counter 0 after */
/* this API.  However, during the restart of Micro-code, the low*/
/* priority packets from the host will be back-pressured.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_COEFF2_t        coeff2 ;
  DR_MS_RSVD_MSG_t      rsvd_msg ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if ( (rampUpcoeff > 0xf) || (rampDncoeff > 0xf)
       || (starvecoeff > 0x7) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, NULL) ;
    return(CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  coeff2.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF2.wrd) ;

  coeff2.bf.RAMPCOEFF = rampUpcoeff ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.COEFF2.wrd, 
               coeff2.wrd) ;

  rsvd_msg.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd) ;

  rsvd_msg.wrd
     = M2R_MS_SET_REG_DATA(rsvd_msg.wrd, 11, 8, rampDncoeff) ;  
  rsvd_msg.wrd
     = M2R_MS_SET_REG_DATA(rsvd_msg.wrd, 14, 12, starvecoeff) ;  

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd, 
               rsvd_msg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER KEEPALIVEMAXTIMER      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_fa_keepalive_max_timer(cs_uint16 port_id, 
                                             cs_uint16 thresh)
/* INPUTS     : o Port Id                                       */
/*              o Number of aging interval                      */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the timer for Micro-sequencer to trigger keepAlive-     */
/* -TimeOut interrupt.                                          */
/*                                                              */
/* Note:                                                        */
/* 1. When Micro-sequencer is running, calling this API will not*/
/* have effect unless calling API 'm2r_ms_set_pc' to restart    */
/* Cortina-provided RPR Micro-code from program counter 0 after */
/* this API.  However, during the restart of Micro-code, the low*/
/* priority packets from the host will be back-pressured.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_SPARE_REG7_t    spare_reg ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if (thresh > 0xfff) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, NULL) ;
    return(CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  spare_reg.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG7.wrd) ;
  
  spare_reg.wrd
     = M2R_MS_SET_REG_DATA(spare_reg.wrd, 12, 1, thresh) ;  

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SPARE_REG7.wrd, 
               spare_reg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    ENABLE/DISABLE ACTIVEWEIGHT DETECTION      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_ctl_fa_conserv_active_weight(cs_uint16 port_id, 
                                               m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable active weight detection when Micro-code is    */
/* operated as RPR conservative mode.                           */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_RSVD_MSG_t      rsvd_msg ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  rsvd_msg.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd) ;

  if ( ctl == M2R_ENABLE ) {
  	rsvd_msg.wrd |= 0x0002 ;
  } else {
  	rsvd_msg.wrd &= ~0x0002 ;
  }
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd, 
               rsvd_msg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SETUP WE FIELD IN GENERATED FAIRNESS PKT   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_fa_pkt_we_field(cs_uint16 port_id,
                                      cs_boolean wrapping)
/* INPUTS     : o Port Id                                       */
/*              o TRUE (Wrapping) or FALSE (Steering)           */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Setup the we bit field of RPR fairness packet generated by   */
/* the Micro-code.                                              */
/*                                                              */
/* Note:                                                        */
/* This API is only valid when using the RPR Micro-code along   */
/* with CORTINA drivers.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_RSVD_MSG_t      rsvd_msg ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  rsvd_msg.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd) ;

  if ( wrapping ) {
        rsvd_msg.wrd |= 0x8000 ;
  } else {
        rsvd_msg.wrd &= ~0x8000 ;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd,
               rsvd_msg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    ENABLE/DISABLE MC FAIRNESS PACKET GEN      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_ctl_gen_mc_fa(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable generating multi-choke fairness packets when  */
/* Micro-code is operated as RPR mode.                          */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_RSVD_MSG_t      rsvd_msg ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  rsvd_msg.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd) ;

  if ( ctl == M2R_ENABLE ) {
  	rsvd_msg.wrd |= 0x0001 ;
  } else {
  	rsvd_msg.wrd &= ~0x0001 ;
  }
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd, 
               rsvd_msg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER LOW PASS COEFFICIENTS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_fa_lp_coeff(cs_uint16 port_id, cs_uint8 lp_allow,
                                  cs_uint8 lp_fwd, cs_uint8 lp_mu, 
                                  cs_uint8 lp_nrXmit)
/* INPUTS     : o Port Id                                       */
/*              o Set low-pass coefficient to calc allow usage  */
/*              o Set low-pass coefficient to calc forward rate */
/*              o Set low-pass coefficient to calc my usage     */
/*              o Set low-pass coefficient to calc nrXmit       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets low-pass coefficients used for the calculation of fair- */
/* ness variables.                                              */
/*                                                              */
/* [lp_allow] sets the coefficient 'LP_ALLOW' in SRP used to    */
/* calculate 'allow_usage'. This is not used in RPR application.*/
/*                                                              */
/* [lp_fwd] sets the coefficient 'LP_FWD_RATE' in SRP used to   */
/* calculate 'lp_fwd_rate'.  In RPR, this is used to calculate  */
/* 'lpfwRate' and 'lpfwRateCongested', but takes the value of   */
/* 'lpCoef'.                                                    */
/*                                                              */
/* [lp_mu] sets the coefficient 'LP_MY_USAGE' in SRP used to    */
/* calculate 'lp_my_usage'.  In RPR, this is used to calculate  */
/* 'lpAddRate', but takes the value of 'lpCoef'.                */
/*                                                              */
/* [lp_nrXmit] is used to calculate 'lpNrXmitRate' in RPR, but  */
/* takes the value of 'lpCoef'.  This is not used in SRP app-   */
/* -lication.                                                   */
/*                                                              */
/* Note: The real value of low-pass coefficients used by the    */
/* Micro-code is the power of two.                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_COEFF_t         coeff ;
  DR_MS_COEFF2_t        coeff2 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if ( (lp_allow > 0xf) || (lp_fwd > 0xf) || (lp_mu > 0xf)
       || (lp_nrXmit > 0xf) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, NULL) ;
    return(CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  coeff.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF.wrd) ;

  coeff.bf.LP_ALLOW = lp_allow ;
  coeff.bf.LP_FWD = lp_fwd ;
  coeff.bf.LP_MU = lp_mu ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.COEFF.wrd, 
               coeff.wrd) ;

  coeff2.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF2.wrd) ;

  coeff2.bf.LP_NRXMIT = lp_nrXmit ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.COEFF2.wrd, 
               coeff2.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MS MULTI-CHOKE FAIRNESS TIMER          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_mcfair_timer(cs_uint16 port_id, m2r_ctl_t ctl,
                                   cs_boolean timer1, cs_uint8 thresh)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*              o TRUE(use timer T1) or FALSE(use timer T0)     */
/*              o Number of T0 or T1 expired counts             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the timer for the generation of Multi-choke fairness    */
/* packet by Micro-sequencer.                                   */
/*                                                              */
/* After [thresh] number of T0 or T1 determined by [timer1]     */
/* expired counts, hardware logic will signal MC_TO bit in      */
/* MS_IO register SR to inform Micro-code it is time to generate*/
/* Multi-choke fairness packet.                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_MCFAIR_CR_t     mcfair_cr ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  mcfair_cr.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MCFAIR_CR.wrd) ;
     
  /*
   * disable timer first if currently enabled
   */
  if ( mcfair_cr.bf.MC_EN && (ctl==M2R_ENABLE) ) {
  	mcfair_cr.bf.MC_EN = 0 ;
  	M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MCFAIR_CR.wrd,
  	             mcfair_cr.wrd) ;
  }

  mcfair_cr.bf.MC_EN = (ctl==M2R_ENABLE) ? 1 : 0 ;
  mcfair_cr.bf.MC_T0_T1 = (timer1==TRUE) ? 1 : 0 ;
  mcfair_cr.bf.MCFAIR_TH = thresh ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MCFAIR_CR.wrd, 
               mcfair_cr.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MS ACTIVEWEIGHT TIMER                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_actvwght_timer(cs_uint16 port_id, m2r_ctl_t ctl,
                                     cs_boolean timer1, cs_uint8 thresh)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*              o TRUE(use timer T1) or FALSE(use timer T0)     */
/*              o Number of T0 or T1 expired counts             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the timer for the calculation of active weight used by  */
/* fairness algorithm.                                          */
/*                                                              */
/* After [thresh] number of T0 or T1 determined by [timer1]     */
/* expired counts, hardware logic will signal ACT_TO bit in     */
/* MS_IO register SR to inform Micro-code it is time to         */
/* calculate the active weight.                                 */
/*                                                              */
/* Note:                                                        */
/* 1. The Activeweight timer only works with RPR conservative   */
/* operating mode of pre-initialized Micro-codes.  In the other */
/* words, the Activeweight timer should not be enabled when not */
/* deploying RPR conservative fairness algorithm with active-   */
/* -weight enabled.                                             */
/* 2. When deploying RPR conservative fairness algorithm, in    */
/* order to enable activeweight calculation, besides calling    */
/* this API, API 'm2r_ms_ctl_fa_conserv_active_weight' is also  */
/* needed to call.                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_ACTVWGHT_CR_t   actvwght_cr ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  actvwght_cr.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.ACTVWGHT_CR.wrd) ;

  /*
   * disable timer first if currently enabled
   */
  if ( actvwght_cr.bf.AW_EN && (ctl==M2R_ENABLE) ) {
  	actvwght_cr.bf.AW_EN = 0 ;
  	M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.ACTVWGHT_CR.wrd,
  	             actvwght_cr.wrd) ;
  }

  actvwght_cr.bf.AW_EN = (ctl==M2R_ENABLE) ? 1 : 0 ;
  actvwght_cr.bf.AW_T0_T1 = (timer1==TRUE) ? 1 : 0 ;
  actvwght_cr.bf.ACTVWGHT_TH = thresh ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.ACTVWGHT_CR.wrd, 
               actvwght_cr.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MS RTT TIMER                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_rtt_timer(cs_uint16 port_id, m2r_ctl_t ctl,
                                cs_boolean timer1, cs_uint32 thresh)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*              o TRUE(use timer T1) or FALSE(use timer T0)     */
/*              o Number of T0 or T1 expired counts             */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the RTT timer used by RPR conservative fairness         */
/* algorithm.                                                   */
/*                                                              */
/* After [thresh] number of T0 or T1 determined by [timer1]     */
/* expired counts, hardware logic will signal RTT_TO bit in     */
/* MS_IO register SR to inform Micro-code RTT timer has expired.*/
/*                                                              */
/* Note:                                                        */
/* 1. When using Cortina-provided RPR Micro-code, RTT timer     */
/* should be disabled, moreover, using API 'm2r_ms_set_rpr_frtt'*/
/* will be more appropriate.                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_RTT1_CR_t       rtt1_cr ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  rtt1_cr.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RTT1_CR.wrd) ;

  /*
   * disable timer first if currently enabled
   */
  if ( rtt1_cr.bf.RTT_EN && (ctl==M2R_ENABLE) ) {
  	rtt1_cr.bf.RTT_EN = 0 ;
  	M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RTT1_CR.wrd,
  	             rtt1_cr.wrd) ;
  }

  rtt1_cr.bf.RTT_EN = (ctl==M2R_ENABLE) ? 1 : 0 ;
  rtt1_cr.bf.RTT_T0_T1 = (timer1==TRUE) ? 1 : 0 ;
  rtt1_cr.bf.RTT_TH = (thresh & 0x3fff0000) >> 16 ;
  
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RTT0_CR.wrd, 
               (thresh & 0x0000ffff)) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.RTT1_CR.wrd, 
               rtt1_cr.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET RPR FRTT COUNT                         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_rpr_frtt(cs_uint16 port_id, cs_uint32 thresh)
/* INPUTS     : o Port Id                                       */
/*              o Times of agingInterval                        */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the FRTT count used by conservative fairness rate       */
/* adjustment algorithm in RPR application.                     */
/*                                                              */
/* According to RPR standard, FRTT should be calculated as      */
/* follows:                                                     */
/*                                                              */
/*      FRTT = FDD + LRTT                                       */
/*                                                              */
/* Both FDD and LRTT should be taken care of by user RPR stack. */
/* When there is congestion happening or changing, it is user   */
/* application's responsibility to calculate and update FRTT.   */
/*                                                              */
/* [thresh] indicates number of times of agingInterval.         */
/*                                                              */
/* Note: This API can only be used with CORTINA-provided RPR    */
/* Micro-code.                                                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  return ( m2r_ms_set_rtt_timer(port_id, M2R_DISABLE,
                                FALSE, thresh) ) ;
}

/****************************************************************/
/* $rtn_hdr_start  GET MS CONFIGURATION WITH RPR PROVISIONS     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_ms_get_rpr_fa_prov(cs_uint16 port_id,
		                 m2r_port_summ_t * pSumm,
		                 m2r_rmac_tb_cfg_t * pTBCfg,
			         m2r_rmac_rpr_fa_cfg_t * pProv,
			         m2r_rmac_ms_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/*              o Pointer to TB(Transit Buffer) config info     */
/*              o Pointer to RPR fairness provision info        */
/*              o Pointer to Micro-sequencer config structure   */
/* OUTPUTS    : o member 'rmac_ms_mode' of [pSumm]              */
/*              o Micro-sequencer request config                */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the Micro-sequencer configuration structure according to */
/* specified RPR standard-compliant fairness provision data     */
/* structure.                                                   */
/*                                                              */
/* The [pSumm] parameter is a pointer to the configuration      */
/* Summary section, which has the caller provided config summary*/
/* info. This is used in returning the appropriate default      */
/* configuration for the Micro-sequencer block.                 */
/*                                                              */
/* The [pTBCfg] parameter is a pointer to the Transit Buffer    */
/* configuration structure, which has the caller provided TB    */
/* config information. This is used in returning the appropriate*/
/* default configuration for the Micro-sequencer block.         */
/*                                                              */
/* The [pProv] parameter is a pointer to the RPR standard-      */
/* compliant fairness provision info.                           */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure. The API will fill in this structure with     */
/* specified RPR standard-compliant fairness provision data     */
/* structure.                                                   */
/*                                                              */
/* The API expects data structures(pSumm, pProv and pCfg)       */
/* already allocated, the config summary is filled in pSumm and */
/* provison datas are filled in pProv.                          */
/* $rtn_hdr_end		                                        */
/****************************************************************/
{
  cs_uint32 lr_rate = M2R_MS_PROV0_DEFAULT;
  cs_uint8  w, i;
  cs_uint16 agingInterval;
  cs_uint64 tmp1, tmp2;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, 
                  ": RingMAC Micro-sequencer cfg\n") ;
    return(CS_ERROR) ;
  }
    
  if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }

  if (pSumm->mac_mode == M2R_PORT_IN_SRP_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_SRP, NULL) ;
    return(CS_ERROR) ;
  }

  if (pTBCfg->one_tb == TRUE) {
    if (pProv->myDualQueueStation) {
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
         ": Single TB does not matched RPR cfg request!\n") ;
      return(CS_ERROR) ;
    }
  } else {
    if (!pProv->myDualQueueStation) {
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
         ": Dual TB does not matched RPR cfg request!\n") ;
      return(CS_ERROR) ;
    }
  }


  /* Only short format of fairness packet is what RPR standard */
  /*   addresses                                               */
  pCfg->longFaPacketType = FALSE ;
  pCfg->faPacketLength = M2R_SHORT_FA_PACKET_LENGTH ;
  pCfg->conservMode = pProv->conservativeRateAdjustment ;

  pCfg->my_sa.byte5 = M2R_RMAC_DEFAULT_MACSA_BYTE5 ;
  pCfg->my_sa.byte4 = M2R_RMAC_DEFAULT_MACSA_BYTE4 ;
  pCfg->my_sa.byte3 = M2R_RMAC_DEFAULT_MACSA_BYTE3 ;
  pCfg->my_sa.byte2 = M2R_RMAC_DEFAULT_MACSA_BYTE2 ;
  pCfg->my_sa.byte1 = M2R_RMAC_DEFAULT_MACSA_BYTE1 ;
  pCfg->my_sa.byte0 = port_id & 0x00FF ;
  
  pCfg->t0_oneShot = FALSE ;
  pCfg->t1_oneShot = FALSE ;
  
  /* This is required for all MS operation modes */
  pCfg->rtt_en = M2R_DISABLE ;
  pCfg->rtt_t1 = FALSE ;

  /* By default, all RPR MS operation modes enable the      */
  /* generation of Multi-choke fairness packets.            */
  pCfg->Gen_MCFF = M2R_ENABLE ;
  pCfg->mcfair_en = M2R_ENABLE ;
  pCfg->mcfair_t1 = TRUE ;
  pCfg->mcfair_th = pProv->reportCoef ;

  pCfg->wrappingProtection = TRUE ;
  pCfg->ttl_to_wrap = M2R_MS_TTL_TO_WRAP_DEFAULT ;
  pCfg->ttl_prov = M2R_MS_TTL_PROV_DEFAULT ;
  pCfg->max_rcv_rate = M2R_MS_MAX_RCV_RATE ;

  /*
   * Due to implementation concern
   */
  if ( pProv->localWeight == 0 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		    " : local weight can not be zero\n") ;
    return (CS_ERROR) ;
  }
  if ( pProv->localWeight == 1 ) {
    pCfg->my_weight = 1;
  } else {
    w = pProv->localWeight ;
    i = 0 ;
    while ( w ) {
      w = w >> 1 ;
      i++;
    }
    pCfg->my_weight = i - 1 ;
  }

  pCfg->max_allow = pProv->maxAllowedRate / 16;

  pCfg->agecoeff = pProv->ageCoef;
  pCfg->rampUpcoeff = pProv->rampUpCoef;
  pCfg->rampDncoeff = pProv->rampDnCoef; 
  pCfg->starvecoeff = pProv->starveCoef;

  pCfg->lp_allow = pProv->lpCoef ;
  pCfg->lp_fwd = pProv->lpCoef ;
  pCfg->lp_mu = pProv->lpCoef ;
  pCfg->lp_nrXmit = pProv->lpCoef ;

  /************************************************
   * According to RPR standard, only OC3 line rate*
   * uses M2R_MS_SLOW_AGING_INTERVAL as aging     *
   * interval, which is not supported by          *
   * MILAN2                                       *
   ************************************************/
  agingInterval = M2R_MS_FAST_AGING_INTERVAL;
  pCfg->ratecoeff = M2R_GET_MS_RATECOEFF(pSumm->line_rate);
  pCfg->t0_cnt = agingInterval * (M2R_CORE_CLOCK / 10) ;

  /* Comment this out to support MP without floating capability
  pCfg->t1_cnt = (cs_uint32)(( (pCfg->faPacketLength * 8.0) 
                  /((M2R_GET_KBPS_LR_RATE(pSumm->line_rate)
                     /1000.0) 
                   * pProv->advertisementRatio))
                  * (M2R_CORE_CLOCK / 10) ) ;
  lr_rate = (cs_uint32)(agingInterval 
                       * (M2R_GET_KBPS_LR_RATE(pSumm->line_rate)
                          /1000.0) 
                       / 8) ;
   */
  tmp1 = (cs_uint64) pCfg->faPacketLength * 8 * 1000 
         * (M2R_CORE_CLOCK / 10) * 100000 ;
  tmp2 = (cs_uint64) M2R_GET_KBPS_LR_RATE(pSumm->line_rate) 
         * pProv->advertisementRatio ;
		 
  pCfg->t1_cnt = (cs_uint32)(tmp1 / tmp2) ;
  
  tmp1 = (cs_uint64) agingInterval 
         * M2R_GET_KBPS_LR_RATE(pSumm->line_rate) ;
  
  lr_rate = (cs_uint32)(tmp1 / (1000 * 8)) ;

  /* prov0/1 is configured as lr_rate of bytes per ageCoef    *
   * agingInterval for pre-initialized RPR mode of Micro-code */
  pCfg->prov1 = ((lr_rate << pCfg->agecoeff) & 0xffff0000) >> 16 ;
  pCfg->prov0 = (lr_rate << pCfg->agecoeff) & 0x0000ffff ;
  pCfg->rtt_th 
    = M2R_MS_GET_UCODE_AGED_TIMER(M2R_MS_FRTT_TIME_MSEC_DEFAULT,
                                    agingInterval);
  pCfg->keepAliveMaxTimer
    = M2R_MS_GET_UCODE_AGED_TIMER(pProv->keepaliveTimeoutValue,
                                    agingInterval);

  pCfg->resv_rate = ((agingInterval * pProv->reservedRate) / 8) 
                    * 100 / lr_rate;

  if (pProv->myDualQueueStation) {

    /* Disable Delay Queue Timer */
    pCfg->classAAccessDelayTimerThreshold = 0 ;
    pCfg->classBAccessDelayTimerThreshold = 0 ;
    pCfg->classCAccessDelayTimerThreshold = 0 ;

    if (!pCfg->conservMode) {
        pCfg->stqFullThresh = pTBCfg->stq_sz - (M2R_RMAC_RPR_MTU / 256) ;
        pCfg->stqHighThresh = pCfg->stqFullThresh / 4 ;
        pCfg->stqLowThresh = pCfg->stqHighThresh / 2 ;
        pCfg->stqMedThresh = (pCfg->stqHighThresh
                             + pCfg->stqLowThresh) / 2 ;

        /* For aggressive rate adjustment, active weight should */
        /* not be enabled.                                      */                                     
        pCfg->Active_W_En = M2R_DISABLE ;
        pCfg->actweight_en = M2R_DISABLE ;
        pCfg->actweight_t1 = FALSE ;
        pCfg->actweight_th = M2R_MS_ACTWEIGHT_MAX_TH ;
        
        pSumm->rmac_ms_mode = M2R_MS_RPR_AGGRESSIVE_DUAL_TB ;
    } else {
        pCfg->stqFullThresh = pTBCfg->stq_sz - (M2R_RMAC_RPR_MTU / 256) ;
        pCfg->stqHighThresh = pCfg->stqFullThresh / 4 ;
        pCfg->stqLowThresh = pCfg->stqHighThresh / 2 ;
        pCfg->stqMedThresh = (pCfg->stqHighThresh
                             + pCfg->stqLowThresh) / 2 ;

        pCfg->Active_W_En
          = (pProv->activeWeightsDetection) ? M2R_ENABLE : M2R_DISABLE ;
        pCfg->actweight_en = pCfg->Active_W_En ;
        pCfg->actweight_t1 = FALSE ;
        pCfg->actweight_th = pProv->activeWeightsCoef ;

        pSumm->rmac_ms_mode = M2R_MS_RPR_CONSERVATIVE_DUAL_TB ;
    }
  } else {

    pCfg->classAAccessDelayTimerThreshold
       = M2R_MS_GET_DEFAULT_DQT_A(M2R_GET_KBPS_LR_RATE(pSumm->line_rate),
                                  M2R_RMAC_RPR_MTU, pCfg->ttl_prov) ;
    pCfg->classBAccessDelayTimerThreshold = M2R_MS_DQT_B_DEFAULT ;
    pCfg->classCAccessDelayTimerThreshold = M2R_MS_DQT_C_DEFAULT ;

    if (!pCfg->conservMode) {

        pCfg->rate_high_th = 95 ;
        pCfg->rate_low_th = 90 ;

        /* For aggressive rate adjustment, active weight should */
        /* not be enabled.                                      */                                     
        pCfg->Active_W_En = M2R_DISABLE ;
        pCfg->actweight_en = M2R_DISABLE ;
        pCfg->actweight_t1 = FALSE ;
        pCfg->actweight_th = M2R_MS_ACTWEIGHT_MAX_TH ;

        pSumm->rmac_ms_mode = M2R_MS_RPR_AGGRESSIVE_SINGLE_TB ;
    } else {

        pCfg->rate_high_th = 95 ;
        pCfg->rate_low_th = 90 ;

        pCfg->Active_W_En
          = (pProv->activeWeightsDetection) ? M2R_ENABLE : M2R_DISABLE ;
        pCfg->actweight_en = pCfg->Active_W_En ;
        pCfg->actweight_t1 = FALSE ;
        pCfg->actweight_th = pProv->activeWeightsCoef ;

        pSumm->rmac_ms_mode = M2R_MS_RPR_CONSERVATIVE_SINGLE_TB ;
    }
  }
  
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start  GET MS CONFIGURATION WITH SRP PROVISIONS     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : RMAC                                            */
/* CHIP	      : Both                                            */
cs_status m2r_ms_get_srp_fa_prov(cs_uint16 port_id,
		                 m2r_port_summ_t * pSumm,
		                 m2r_rmac_tb_cfg_t * pTBCfg,
		                 m2r_rmac_srp_fa_cfg_t * pProv,
		                 m2r_rmac_ms_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/*              o Pointer to TB(Transit Buffer) config info     */
/*              o Pointer to SRP fairness provision info        */
/*              o Pointer to Micro-sequencer config structure   */
/* OUTPUTS    : o member 'rmac_ms_mode' of [pSumm]              */
/*              o Micro-sequencer request config                */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the Micro-sequencer configuration structure according to */
/* specified SRP fairness provision data structure.             */
/*                                                              */
/* The [pSumm] parameter is a pointer to the configuration      */
/* Summary section, which has the caller provided config summary*/
/* info. This is used in returning the appropriate default      */
/* configuration for the Micro-sequencer block.                 */
/*                                                              */
/* The [pTBCfg] parameter is a pointer to the Transit Buffer    */
/* configuration structure, which has the caller provided TB    */
/* config information. This is used in returning the appropriate*/
/* default configuration for the Micro-sequencer block.         */
/*                                                              */
/* The [pProv] parameter is a pointer to the SRP fairness       */
/* provision info.                                              */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure. The API will fill in this structure with     */
/* specified SRP fairness provision data structure.             */
/*                                                              */
/* The API expects data structures(pSumm, pProv and pCfg)       */
/* already allocated, the config summary is filled in pSumm and */
/* provison datas are filled in pProv.                          */
/* $rtn_hdr_end		                                        */
/****************************************************************/
{
  cs_uint32 lr_rate = M2R_MS_PROV0_DEFAULT;
  cs_uint64 tmp1, tmp2;
  
  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, 
                  ": RingMAC Micro-sequencer cfg\n") ;
    return(CS_ERROR) ;
  }
    
  if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
    return(CS_ERROR) ;
  }

  if (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) {
    CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_RPR, NULL) ;
    return(CS_ERROR) ;
  }

  if (pTBCfg->one_tb == TRUE) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
       ": Single TB cfg is not supported by M2R_MS_SRP!\n") ;
    return(CS_ERROR) ;
  }

  pCfg->conservMode = FALSE ;
  pCfg->longFaPacketType = FALSE ;
  pCfg->faPacketLength = M2R_SHORT_FA_PACKET_LENGTH ;
        
  pCfg->t0_oneShot = FALSE ;
  pCfg->t1_oneShot = FALSE ;

  pCfg->my_sa.byte5 = M2R_RMAC_DEFAULT_MACSA_BYTE5 ;
  pCfg->my_sa.byte4 = M2R_RMAC_DEFAULT_MACSA_BYTE4 ;
  pCfg->my_sa.byte3 = M2R_RMAC_DEFAULT_MACSA_BYTE3 ;
  pCfg->my_sa.byte2 = M2R_RMAC_DEFAULT_MACSA_BYTE2 ;
  pCfg->my_sa.byte1 = M2R_RMAC_DEFAULT_MACSA_BYTE1 ;
  pCfg->my_sa.byte0 = port_id & 0x00FF ;

  pCfg->rsvd_msg = M2R_MS_RSVD_MSG_DEFAULT ;
  pCfg->ttl_to_wrap = M2R_MS_TTL_TO_WRAP_DEFAULT ;
  pCfg->ttl_prov = M2R_MS_TTL_PROV_DEFAULT ;
  pCfg->max_rcv_rate = M2R_MS_MAX_RCV_RATE ;

  pCfg->my_weight = M2R_MS_MY_WEIGHT_DEFAULT ;
  pCfg->max_allow = pProv->max_usage / 16;

  pCfg->agecoeff = pProv->agecoeff;

  pCfg->lp_allow = pProv->lp_allow ;
  pCfg->lp_fwd = pProv->lp_fwd_rate ;
  pCfg->lp_mu = pProv->lp_my_usage ;
  pCfg->lp_nrXmit = M2R_MS_LP_NRXMIT_DEFAULT ;

  pCfg->ratecoeff = M2R_GET_MS_RATECOEFF(pSumm->line_rate);
/* Comment this out to support MP without floating capability
  pCfg->t0_cnt = (cs_uint32)(((pProv->decay_interval * 8)
                             / (M2R_GET_KBPS_LR_RATE(pSumm->line_rate)
                                /1000.0)) 
                    * (M2R_CORE_CLOCK / 10)) ;
  pCfg->t1_cnt = (cs_uint32)(( (pCfg->faPacketLength * 8.0) 
                  /((M2R_GET_KBPS_LR_RATE(pSumm->line_rate)
                     /1000.0) 
                   * M2R_MS_ADVERTISEMENT_RATIO_DEFAULT))
                  * (M2R_CORE_CLOCK / 10) ) ;
*/
  tmp1 = (cs_uint64) pProv->decay_interval * 8 * 1000 
         * (M2R_CORE_CLOCK / 10) ;
  		 
  pCfg->t0_cnt 
    = (cs_uint32)(tmp1 / M2R_GET_KBPS_LR_RATE(pSumm->line_rate)) ;
  
  
  tmp1 = (cs_uint64) pCfg->faPacketLength * 8 * 1000 
         * (M2R_CORE_CLOCK / 10) * 100000 ;
  tmp2 = (cs_uint64) M2R_GET_KBPS_LR_RATE(pSumm->line_rate) 
         * M2R_MS_ADVERTISEMENT_RATIO_DEFAULT ; 
 	 
  pCfg->t1_cnt = (cs_uint32)(tmp1 / tmp2) ;

  /* FIXME: Adam on 10/23/03 -> Need to figure out how to translate*
   *       decay_interval to lr_rate                               */
  lr_rate = pProv->decay_interval ;

  pSumm->rmac_ms_mode = M2R_MS_SRP ;
  
  pCfg->mcfair_en = M2R_DISABLE ;
  pCfg->mcfair_t1 = TRUE ;
  pCfg->mcfair_th = M2R_MS_MCFAIR_MAX_TH ;

  pCfg->actweight_en = M2R_DISABLE ;
  pCfg->actweight_t1 = FALSE ;
  pCfg->actweight_th = M2R_MS_ACTWEIGHT_MAX_TH ;
  
  pCfg->rtt_en = M2R_DISABLE ;
  pCfg->rtt_t1 = FALSE ;
  pCfg->rtt_th = M2R_MS_RTT_MAX_TH ;

  /* prov0/1 is configured as one sixteenth of lr_rate 
     for pre-initialized Micro-code */
  pCfg->prov1 = ((lr_rate) & 0xffff0000) >> 16 ;
  pCfg->prov0 = (lr_rate) & 0x0000ffff ;

  pCfg->resv_rate = 0 ;

  pCfg->stqHighThresh = pTBCfg->stq_sz - (M2R_RMAC_SRP_MTU / 256) ;
  pCfg->stqLowThresh = pCfg->stqHighThresh / 4 ;

  /* Disable Delay Queue Timer */
  pCfg->classAAccessDelayTimerThreshold = 0 ;
  pCfg->classBAccessDelayTimerThreshold = 0 ;
  pCfg->classCAccessDelayTimerThreshold = 0 ;

  return (CS_OK);
}

/************************ Control *******************************/
/****************************************************************/
/* $rtn_hdr_start    MICRO-SEQUENCER STALL CONTROL              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_ctl_stall(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables(stalls)/disables(unstalls) of the Micro-sequencer.   */
/*                                                              */
/* Note:                                                        */
/*    1. When calling this API to stall Micro-sequencer, timer  */
/*       T0/T1 will be disabled and stay disabled thereafter to */
/*       prevent Micro-sequencer from generating fatal MS TIME  */
/*       OUT interrupt ;                                        */
/*    2. When calling this API to unstall Micro-sequencer, it is*/
/*       the responsibility of user application to take care of */
/*       timer T0/T1 before and after.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_OPTIONS_t       options;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  /* Disable timer first when stalling MS */
  if ( ctl == M2R_ENABLE ) {
    if (m2r_ms_set_timer_val( port_id, M2R_DISABLE, 0, M2R_DISABLE, 0)
          == CS_ERROR) {
      return (CS_ERROR) ;
    }
  } 

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  options.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd) ;

  options.bf.STALL = (ctl == M2R_ENABLE) ? 1 : 0;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, options.wrd);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    MICRO-SEQUENCER PARITY CHECK CONTROL       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_ctl_par_chk(cs_uint16 port_id, m2r_ctl_t all_chk,
                              m2r_ctl_t in_chk, m2r_ctl_t ext_chk)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/disables Micro-sequencer parity check feature.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_OPTIONS_t       options;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  options.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd) ;

  options.bf.ALL_CHK_EN = (all_chk == M2R_ENABLE) ? 1 : 0;
  options.bf.IN_PAR_ENB = (in_chk == M2R_ENABLE) ? 1 : 0;
  options.bf.EX_PAR_ENB = (ext_chk == M2R_ENABLE) ? 1 : 0;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, options.wrd);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    CONTROL OF SENDING FA/USG PACKETS          */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_ctl_send_fa_pkt(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enables/disables sending of usage/fairness packets.          */
/*                                                              */
/* NOTE:                                                        */
/*    1. This API requires the support of CORTINA Micro-code.   */
/*    2. This API must be called after port initialization to   */
/*       enable sending usage/fairness packets because fairness */
/*       is disabled at that moment;                            */
/*    3. When the port is in normal operation modes other than  */
/*       pass-thru mode, calling this API to disable sending    */
/*       fairness packet is not desirable behavior.             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  return (m2r_ms_set_timer_val(port_id, M2R_ENABLE, 0, ctl, 0));
}

/************************   Dump  *******************************/
/****************************************************************/
/* $rtn_hdr_start  DUMP MULTI-CHOKE MESSAGE AT SPECIFIED ENTRY  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_dump_mc_fa_msg(cs_uint16 port_id, cs_uint8 new_src)
/* INPUTS     : o Port Id                                       */
/*              o New Multi-choke message source[0...255]       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the Multi-choke message at specified entry and     */
/* prints it.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16          dev_num, sl_num ;
  M2R_t              * pDev ;
  DR_MICRO_IF_CR2_t  cr2 ;
  m2r_mc_fa_msg_t    msg[2] ;
  cs_uint8           i ;
  
  for ( i=0; i<2; i++ ) {
    if ( m2r_ms_read_mc_fa_msg(port_id, new_src, i, &msg[i])
         == CS_ERROR ) {
       return (CS_ERROR) ;
    }
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  cr2.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;

  CS_PRINT("\n") ;
  CS_PRINT("\t *********************************************\n") ;
  CS_PRINT("\t Port-%d received Multi-choke fairness packets\n", 
           port_id) ; 
  CS_PRINT("\t (dev_num = %d, slice = %d, ring_id = %d, TTL = %d)\n", 
           dev_num, sl_num, (1-cr2.bf.RING_ID), new_src) ; 
  CS_PRINT("\t *********************************************\n") ;
  
  for ( i=0; i<2; i++ ) {
  	CS_PRINT("\t Packet for Ring-%d\n", i) ;
    CS_PRINT("\t Source Address : %.2X.%.2X.%.2X.%.2X.%.2X.%.2X\n", 
             msg[i].mac_sa.byte5, msg[i].mac_sa.byte4,
             msg[i].mac_sa.byte3, msg[i].mac_sa.byte2,
             msg[i].mac_sa.byte1, msg[i].mac_sa.byte0) ;
    if ( msg[i].longFaPacketType ) {
      CS_PRINT("\t This is long format MC fairness packet with HEC\n") ;
      CS_PRINT("\t 1st 16-bit word of fairness ctrl value : 0x%.4X\n", 
               msg[i].payload.fa_hec_val >> 16 ) ;
      CS_PRINT("\t 2nd 16-bit word of fairness ctrl value : 0x%.4X\n", 
               msg[i].payload.fa_hec_val & 0x0000ffff ) ;
    } else {
      CS_PRINT("\t This is 16-byte short format MC fairness packet\n") ;
      CS_PRINT("\t Fairness control : 0x%.4X\n", 
               msg[i].payload.shortpkt.fa_ctl) ;
      CS_PRINT("\t   Fairness value : 0x%.4X\n", 
               msg[i].payload.shortpkt.fa_val) ;
    }
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start         DUMP NEW MULTI-CHOKE MESSAGE          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_dump_new_mc_fa_msg(cs_uint16 port_id) 
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves all new Multi-choke messages and prints them.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint8           new_src ;

  while (m2r_ms_check_mc_fa_new_msg(port_id, &new_src)
      == TRUE) {
    m2r_ms_dump_mc_fa_msg(port_id, new_src) ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  DUMP RUNNING FAIRNESS VARIABLES              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_dump_running_fa_var(cs_uint16 port_id) 
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the running fairness variable values and print it. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                dev_num, sl_num ;
  M2R_t                    * pDev ;
  m2r_rmac_ms_runtime_fa_t Fa ;
  DR_MICRO_IF_CR2_t        cr2 ;
  
  if ( m2r_ms_get_running_fa_var(port_id, &Fa)
	   == CS_ERROR ) {
	return (CS_ERROR) ;
  }
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  cr2.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;

  CS_PRINT("\n") ;
  CS_PRINT("\t *********************************************\n") ;
  CS_PRINT("\t Port-%d running fairness variables on Ring-%d\n", 
	   port_id, cr2.bf.RING_ID) ; 
  CS_PRINT("\t (dev_num = %d, slice = %d)\n", dev_num, sl_num) ; 
  CS_PRINT("\t *********************************************\n") ;
  CS_PRINT("\t        My Usage  :  0x%.5X\n", Fa.my_usage) ; 
  CS_PRINT("\t        Fwd Rate  :  0x%.5X\n", Fa.fwd_rate) ; 
  CS_PRINT("\t My Usage Congest :  0x%.5X\n", Fa.my_usage_cng) ; 
  CS_PRINT("\t Fwd Rate Congest :  0x%.5X\n", Fa.fwd_rate_cng) ; 
  CS_PRINT("\t      NRXMIT RATE :  0x%.5X\n", Fa.nrxmit_rate) ; 
  CS_PRINT("\n") ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start           Reset TB STATS                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_reset_tb_stats(cs_uint16 port_id) 
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reset transit buffer statistics.                             */
/*                                                              */
/* NOTE: This API requires the support of CORTINA Micro-code.   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint8  sema_fms, sema_tms ;
  cs_status status = CS_OK ;
  cs_uint8  delay, timeout = 5 ;

  /* request */
  if ( m2r_ms_send_sema(port_id, M2R_MS_SEMA_RESET_TB_STATS)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  
  /* wait for the result */
  delay = 0 ;
  status = m2r_ms_get_sema(port_id, &sema_fms, &sema_tms) ;
  while ( sema_tms && (status == CS_OK) ) {
    if ( delay > timeout ) break ;
    CS_MDELAY( 1 ) ;
    delay += 1 ;
    status = m2r_ms_get_sema(port_id, &sema_fms, &sema_tms) ;
  }

  if ( delay > timeout ) {
    CS_HNDL_ERROR(port_id, EM2R_MS_SEMA_REQ_NO_RESPONSE, 
		    " : Request to reset TB stats!\n") ;
    return ( CS_ERROR ) ;
  }

  return ( status ) ;
}

/****************************************************************/
/* $rtn_hdr_start  REQUEST TTL_TO_CONGEST FROM MICRO-SEQUENCER  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_uint8  m2r_ms_get_ttl_to_congest(cs_uint16 port_id) 
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieve TTL_TO_CONGEST from Micro-sequencer.                */
/*                                                              */
/* NOTE: This API requires the support of CORTINA Micro-code.   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  cs_uint8  sema_fms, sema_tms ;
  cs_status status = CS_OK ;
  cs_uint8      delay, timeout = 5 ;
  DR_MS_SPARE_REG6_t   spare_reg6 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, 0) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_SRP,
                             0) ;

  /* request */
  if ( m2r_ms_send_sema(port_id, M2R_MS_SEMA_REQUEST_TTL_TO_CONGEST)
       == CS_ERROR ) {
    return (0) ;
  }
  
  /* wait for the result */
  delay = 0 ;
  status = m2r_ms_get_sema(port_id, &sema_fms, &sema_tms) ;
  while ( sema_tms && (status == CS_OK) ) {
    if ( delay > timeout ) break ;
    CS_MDELAY( 1 ) ;
    delay += 1 ;
    status = m2r_ms_get_sema(port_id, &sema_fms, &sema_tms) ;
  }

  if ( delay > timeout ) {
    CS_HNDL_ERROR(port_id, EM2R_MS_SEMA_REQ_NO_RESPONSE, 
		    " : Request to get TTL_TO_CONGEST!\n") ;
    return ( 0 ) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  spare_reg6.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG6.wrd) ;
  
  return ( (cs_uint8)(spare_reg6.wrd & 0x00ff) ) ;
}

/****************************************************************/
/* $rtn_hdr_start  DUMP CURRENTLY BEING PROCESSED FAIRNESS MSG  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_dump_current_in_fa_msg(cs_uint16 port_id) 
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the currently being processed fairness message by  */
/* Micro-code and print it.                                     */
/*                                                              */
/* Note: In current chip design, Micro-code should write a value*/
/* (any value) to ms_io register IN_MSG_CLR to indicate old     */
/* fairness message has been processed and newly received       */
/* fairness message can be written into IN_USG buffer, otherwise*/
/* the newly received fairness message will be dropped.         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0 ;
  DR_MICRO_IF_CR2_t     cr2 ;
  DR_MS_IN_USG4_t       usg4 ;
  DR_MS_IN_USG3_t       usg3 ;
  DR_MS_IN_USG2_t       usg2 ;
  DR_MS_IN_USG1_t       usg1 ;
  DR_MS_IN_USG0_t       usg0 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
  cr2.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR2_.wrd) ;

  usg4.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IN_USG4.wrd) ;
  usg3.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IN_USG3.wrd) ;
  usg2.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IN_USG2.wrd) ;
  usg1.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IN_USG1.wrd) ;
  usg0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IN_USG0.wrd) ;

  if ( cr0.bf.RPRMODE ) {
  	/* This is RPR mode */
    CS_PRINT("\n") ;

    CS_PRINT("\t *******************************************");
    CS_PRINT("***************\n") ;

    CS_PRINT("\t Port-%d now processed RPR fairness pkt on Ring-%d\n", 
	         port_id, (1-cr2.bf.RING_ID)) ; 
    CS_PRINT("\t (dev_num = %d, slice = %d)\n", dev_num, sl_num) ;

    CS_PRINT("\t *******************************************");
    CS_PRINT("***************\n") ;
    
    if ( cr0.bf.UTYPE ) {
      CS_PRINT("\t Current fairness packet is %d-byte with HEC\n", 
               (16 + cr0.bf.ULEN)) ;
      CS_PRINT("\t 1st 16-bit word of fairness control value : 0x%.4X\n",
               usg4.wrd) ;
      CS_PRINT("\t 2nd 16-bit word of fairness control value : 0x%.4X\n",
               usg3.wrd) ;
      CS_PRINT("\t 3rd 16-bit word of fairness control value : 0x%.4X\n",
               usg2.wrd) ;
      CS_PRINT("\t 4th 16-bit word of fairness control value : 0x%.4X\n",
               usg1.wrd) ;
      CS_PRINT("\t 5th 16-bit word of fairness control value : 0x%.4X\n",
               usg0.wrd) ;
    } else {
      CS_PRINT("\t Current fairness packet is 16-byte short format\n") ;
      CS_PRINT("\t Source Address : %.2X.%.2X.%.2X.%.2X.%.2X.%.2X\n", 
               usg4.wrd >> 8, usg4.wrd & 0x00ff,
               usg3.wrd >> 8, usg3.wrd & 0x00ff,
               usg2.wrd >> 8, usg2.wrd & 0x00ff) ;
      CS_PRINT("\t Version : %d\n", usg1.wrd >> 13) ;
      CS_PRINT("\t Reserved Message : %.4X\n", usg1.wrd & 0x1FFF) ;
      CS_PRINT("\t Fairness control value : 0x%.4X\n", usg0.wrd) ;
    }
  	
  } else {
  	/* This is SRP mode */
    CS_PRINT("\n") ;
    CS_PRINT("\t *******************************************");
    CS_PRINT("***************\n") ;

    CS_PRINT("\t Port-%d now processed SRP Usage pkt on Ring-%d\n", 
	         port_id, (1-cr2.bf.RING_ID)) ; 
    CS_PRINT("\t (dev_num = %d, slice = %d)\n", dev_num, sl_num) ; 

    CS_PRINT("\t *******************************************");
    CS_PRINT("***************\n") ;

    CS_PRINT("\t Usage packet is 16-byte fixed format\n") ;
    CS_PRINT("\t Source Address : %.2X.%.2X.%.2X.%.2X.%.2X.%.2X\n", 
             usg4.wrd >> 8, usg4.wrd & 0x00ff,
             usg3.wrd >> 8, usg3.wrd & 0x00ff,
             usg2.wrd >> 8, usg2.wrd & 0x00ff) ;
    CS_PRINT("\t Reserved Message : %.4X\n", usg1.wrd) ;
    CS_PRINT("\t Usage value : 0x%.4X\n", usg0.wrd) ;
  }

  return (CS_OK) ;
}

/************************ Diagnostics ***************************/
/****************************************************************/
/* $rtn_hdr_start  FETCH MICRO-SEQUENCER INTERRUPT STATUS       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_read_int_status(cs_uint16 port_id, 
                                  m2r_rmac_ms_int_t * ps,
                                  cs_boolean clear)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to MS interrupt status struct         */
/*              o Clear INT(TRUE) or leave INT(FALSE)           */
/* OUTPUTS    : MS interrupt status struct                      */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieves the current Micro-sequencer interrupt status.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_CR_SR_t         cr_sr;
  DR_MS_ERR_t           err;
  DR_MS_SEMA4_t         sema4;
  DR_MS_OPTIONS_t       options;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr_sr.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR_SR.wrd) ;
  ps->ms_time_out = (cr_sr.bf.TIMEOUT) ? TRUE : FALSE ;
  
  options.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd) ;
  err.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.ERR.wrd) ;
  ps->ms_instr_par = (options.bf.IN_PAR_ENB & err.bf.I_PER) 
                     ? TRUE : FALSE ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  sema4.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SEMA4.wrd) ;
  ps->ms_send_sema = sema4.bf.FMS ;

  if ( (clear == TRUE) && (sema4.bf.FMS) ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SEMA4.wrd, sema4.wrd);
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start  FETCH MICRO-SEQUENCER INTERRUPT STATUS       */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_uint16  m2r_ms_read_int_bitmap(cs_uint16 port_id, cs_boolean clear)
/* INPUTS     : o Port Id                                       */
/*              o Clear INT(TRUE) or leave INT(FALSE)           */
/* OUTPUTS    : ---                                             */
/* RETURNS    : Micro-sequencer interrupt src bitmap            */
/* DESCRIPTION:                                                 */
/* Retrieves the current Micro-sequencer interrupt source.      */
/*                                                              */
/* Bit 9 of the return value indicates if MS Time-Out interrupt */
/* happened.                                                    */
/*                                                              */
/* Bit 8 of the return value indicates if MS INSTRUCTION PARITY */
/* ERROR interrupt happened.                                    */
/*                                                              */
/* Bits [7:0] indicate the semaphores sending by MS Ucode.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num, bits = 0 ;
  M2R_t                 * pDev ;
  DR_MS_CR_SR_t         cr_sr;
  DR_MS_ERR_t           err;
  DR_MS_SEMA4_t         sema4;
  DR_MS_OPTIONS_t       options;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, 0xFFFF) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, 0xFFFF) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  cr_sr.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR_SR.wrd) ;
  bits |= (cr_sr.bf.TIMEOUT) ? (1<<9) : 0 ;
  
  options.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd) ;
  err.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.ERR.wrd) ;
  bits |= (options.bf.IN_PAR_ENB & err.bf.I_PER) ? (1<<8) : 0 ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  sema4.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SEMA4.wrd) ;
  bits |= sema4.bf.FMS ;

  if ( (clear == TRUE) && (sema4.bf.FMS) ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SEMA4.wrd, sema4.wrd);
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (bits);
}

/****************************************************************/
/* $rtn_hdr_start  CHECK IF NEW MULTI-CHOKE FAIRNESS HAS COMED  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_boolean  m2r_ms_check_mc_fa_new_msg(cs_uint16 port_id, 
                                       cs_uint8 * psrc)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to buffer index data location         */
/* OUTPUTS    : location index in Multi-choke buffer for new msg*/
/* RETURNS    : TRUE or FALSE                                   */
/* DESCRIPTION:                                                 */
/* Checks if new multi-choke fairness packets have been rece-   */
/* -ived.  If returning TRUE, the lowest buffer index for new   */
/* packet will be stored in the data location pointed by [psrc].*/
/*                                                              */
/* Note: This API is usually called by USER interrupt handling  */
/* routine when the interrupt indicating new multi-choke message*/
/* has comed happens.                                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16               dev_num, sl_num ;
  M2R_t                   * pDev ;
  DR_MS_NEW_MULTICHOKE_t  new_multi;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, FALSE) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
			    (M2R_INVALID_IN_BYPASS 
                             | M2R_INVALID_IN_SRP),
			    FALSE) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  new_multi.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.NEW_MULTICHOKE.wrd) ;
  
  if ( new_multi.bf.MULTICHOKE_NEW ) {
  	*psrc = new_multi.bf.MULTICHOKE_SRC ;
  	return (TRUE) ;
  }
  
  return (FALSE) ;
}

/****************************************************************/
/* $rtn_hdr_start  FETCH NEW MULTI-CHOKE FAIRNESS PACKET        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_read_mc_fa_msg(cs_uint16 port_id, cs_uint8 new_src,
			         cs_uint8 ri, m2r_mc_fa_msg_t * pmsg)
/* INPUTS     : o Port Id                                       */
/*              o New Multi-choke fairness message src[0...255] */
/*              o Ring ID where MC fairness msg was circulating */
/*              o Pointer to MC fairness message struct         */
/* OUTPUTS    : MAC SA(6 Bytes) and payload(4 Bytes)            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reads the Multi-choke fairness message circulating on the    */
/* specified ringnet from the Multi-choke buffer.               */
/*                                                              */
/* [new_src] indexes where the Multi-choke fairness message was */
/* stored in the receiving buffer.                              */
/*                                                              */
/* [ri] specifies the ring id Multi-choke fairness message was  */
/* originally circulating on.  Each entry of multi-choke buffer */
/* consists of two banks corresponding to each ringnet          */
/* respectively.                                                */
/*                                                              */
/* [pmsg] points to Multi-choke fairness message structure      */
/* allocated by the applications.                               */
/*                                                              */
/* Note: When the application handles the interrupt indicating  */
/* new Multi-choke fairness message has received, this API      */
/* should be called twice targeting both ringnet and it is the  */
/* application's responsibility to find out which one is the    */
/* newly coming one.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16		    dev_num, sl_num ;
  M2R_t             * pDev ;
  m2r_rmac_gram_t   gram ;
  DR_MICRO_IF_CR0_t cr0 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
			     (M2R_INVALID_IN_BYPASS 
                              | M2R_INVALID_IN_SRP),
			     CS_ERROR) ;

  if ( ri > 1 ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
		    " : Ringnet ID can only be 0 or 1\n") ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  cr0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
    
  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  gram.GRAM_CR.bf.SEL    
       = M2R_RMAC_MEMID_TO_MEMSEL(M2R_MS_MCHOKE_SRAM) ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = (ri << 8) | new_src;
  if ( m2r_rmac_read_gram_regs(port_id, &gram)  == CS_ERROR ) {
    M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
    return(CS_ERROR) ;
  }

  pmsg->mac_sa.byte5 = (gram.GRAM_D2.wrd & 0xff00) >> 8 ;
  pmsg->mac_sa.byte4 = gram.GRAM_D2.wrd & 0x00ff ;
  pmsg->mac_sa.byte3 = (gram.GRAM_D1.wrd & 0xff00) >> 8 ;
  pmsg->mac_sa.byte2 = gram.GRAM_D1.wrd & 0x00ff ;
  pmsg->mac_sa.byte1 = (gram.GRAM_D0.wrd & 0xff00) >> 8 ;
  pmsg->mac_sa.byte0 = gram.GRAM_D0.wrd & 0x00ff ;
  
  if ( cr0.bf.UTYPE ) {
  	pmsg->longFaPacketType = TRUE ;
  	pmsg->payload.fa_hec_val = (gram.GRAM_D4.wrd << 16) 
                                   | gram.GRAM_D3.wrd ;
  } else {
  	pmsg->longFaPacketType = FALSE ;
  	pmsg->payload.shortpkt.fa_ctl = gram.GRAM_D4.wrd ;
  	pmsg->payload.shortpkt.fa_val = gram.GRAM_D3.wrd ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    CLEAR MULTI-CHOKE FAIRNESS PACKET          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_clr_mc_fa_msg(cs_uint16 port_id, cs_uint8 ttl,
				cs_uint8 ri)
/* INPUTS     : o Port Id                                       */
/*              o Packet TTL                                    */
/*              o Ring ID where MC fairness msg was circulating */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Clear the Multi-choke fairness message storage with the      */
/* specified TTL value.                                         */
/*                                                              */
/* [ttl] indexes where the Multi-choke fairness message was     */
/* stored in the receiving buffer.                              */
/*                                                              */
/* [ri] specifies the ring id Multi-choke fairness message was  */
/* originally circulating on.  Each entry of multi-choke buffer */
/* consists of two banks corresponding to each ringnet          */
/* respectively.                                                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  return ( m2r_rmac_reset_mem(port_id, M2R_MS_MCHOKE_SRAM,
                          ((ri << 8) | ttl), 1) ) ;
}

/****************************************************************/
/* $rtn_hdr_start  GET RUNNING ADVERTISEMENT RATIO SETTING      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint32 m2r_ms_get_running_advertisement_ratio(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : current advertisement ratio                     */
/* DESCRIPTION:                                                 */
/* Returns the current fairness advertisement ratio used by     */
/* Micro-sequencer in the unit of 0.00001.                      */
/*                                                              */
/* Note: This API assumes timer T1 is used for advertisement    */
/* interval.                                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                * pDev ;
  cs_uint32             t1_cnt, ad_ratio ;
  m2r_port_line_rate_t lr_rate ;
  cs_uint64            tmp1, tmp2 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, 0xFFFFFFFF) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             0xFFFFFFFF) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  t1_cnt
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.T1_CNT1.wrd) ;
  t1_cnt = (t1_cnt<<16)
         + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.T1_CNT0.wrd) ;

  lr_rate = M2R_GET_LINE_RATE( port_id ) ;

/* Comment this out to support MP without floating capability
  ad_ratio = (cs_uint32)(( (16 * 8.0)
              /((M2R_GET_KBPS_LR_RATE(lr_rate)
                 /1000.0)
               * t1_cnt))
              * (M2R_CORE_CLOCK / 10)
              * 100000 ) ;
 */
  tmp1 = (cs_uint64) 16 * 8 * (M2R_CORE_CLOCK / 10) * 100000 * 1000 ;
  tmp2 = (cs_uint64) M2R_GET_KBPS_LR_RATE(lr_rate) * t1_cnt ;
  
  ad_ratio = (cs_uint32)(tmp1 / tmp2) ;

  return (ad_ratio) ;
}

/****************************************************************/
/* $rtn_hdr_start  GET RUNNING RESERVED RATE PROVISION          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint32 m2r_ms_get_running_resvdrate(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : current provisioned reserved rate setting       */
/* DESCRIPTION:                                                 */
/* Returns the current total reserved bandwidth (subclass A0)   */
/* setting in kbps for the corresponding ringlet.               */
/*                                                              */
/* Note:                                                        */
/* 1. This API assumes timer T0 is used for agingInterval.      */
/* 2. The return value of this API is different from the RPR MIB*/
/* attribute 'rprFairnessReservedRate' that is defined as 'the  */
/* amount of A0 traffic that this station adds that ringlet in  */
/* Mb/s units.'                                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  m2r_port_line_rate_t  lr_rate ;
  cs_uint32             lr, resvd_rate, unresvd_rate ;
  cs_uint16             agingInterval ;
  DR_MS_COEFF_t         coef ;
  cs_uint16             tmp ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, 0xFFFFFFFF) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, 
                             0xFFFFFFFF) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  lr_rate = M2R_GET_LINE_RATE( port_id ) ;
  lr = M2R_GET_KBPS_LR_RATE(lr_rate) ;

  unresvd_rate = 
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.UNRESV_RATE1.wrd) ;
  tmp =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.UNRESV_RATE0.wrd) ; 

  unresvd_rate = (unresvd_rate << 16) | tmp ;

  /* According to RPR standard, only OC3 line rate
     uses M2R_MS_SLOW_AGING_INTERVAL as aging
     interval, which is not supported by
     MILAN2 */
  agingInterval = M2R_MS_FAST_AGING_INTERVAL;

  coef.wrd =
    M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF.wrd) ;

  resvd_rate = (lr - ((unresvd_rate * 1000) 
                      / ((1 << coef.bf.AGECOEFF) * agingInterval)) * 8) ;
 
  return (resvd_rate) ; 
}

/****************************************************************/
/* $rtn_hdr_start  FETCH RUNNING FAIRNESS VARIABLES             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */ 
cs_status  m2r_ms_get_running_fa_var(cs_uint16 port_id, 
				     m2r_rmac_ms_runtime_fa_t * pFa)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to fairness runtime variable structure*/
/* OUTPUTS    : fairness runtime variable values                */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time fairness variable values of the */
/* specified port.                                              */
/* The caller is responsible for allocating memory for the      */
/* runtime fairness variable structure and the driver will      */
/* access the hardware and provide the caller with the current  */
/* values.                                                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_SRPF_MY_USAGE1_t   my_usage1 ;
  DR_SRPF_MY_USAGE0_t   my_usage0 ;
  DR_SRPF_FWD_RATE1_t   fwd_rate1 ;
  DR_SRPF_FWD_RATE0_t   fwd_rate0 ;
  DR_SRPF_MY_USAGE_CNG1_t my_usage_cng1 ;
  DR_SRPF_MY_USAGE_CNG0_t my_usage_cng0 ;
  DR_SRPF_FWD_RATE_CNG1_t fwd_rate_cng1 ;
  DR_SRPF_FWD_RATE_CNG0_t fwd_rate_cng0 ;
  DR_SRPF_NRXMIT_RATE1_t  nrxmit_rate1 ;
  DR_SRPF_NRXMIT_RATE0_t  nrxmit_rate0 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  my_usage1.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_USAGE1.wrd) ;
  my_usage0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_USAGE0.wrd) ;
  pFa->my_usage = (my_usage1.bf.MY_USAGE1 << 16) 
                  | my_usage0.bf.MY_USAGE0 ;

  fwd_rate1.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.FWD_RATE1.wrd) ;
  fwd_rate0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.FWD_RATE0.wrd) ;
  pFa->fwd_rate = (fwd_rate1.bf.FWD_RATE1 << 16) 
                  | fwd_rate0.bf.FWD_RATE0 ;

  my_usage_cng1.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_USAGE_CNG1.wrd) ;
  my_usage_cng0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_USAGE_CNG0.wrd) ;
  pFa->my_usage_cng = (my_usage_cng1.bf.MY_USAGE_CNG1 << 16) 
                      | my_usage_cng0.bf.MY_USAGE_CNG0 ;

  fwd_rate_cng1.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.FWD_RATE_CNG1.wrd) ;
  fwd_rate_cng0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.FWD_RATE_CNG0.wrd) ;
  pFa->fwd_rate_cng = (fwd_rate_cng1.bf.FWD_RATE_CNG1 << 16) 
                      | fwd_rate_cng0.bf.FWD_RATE_CNG0 ;

  nrxmit_rate1.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.NRXMIT_RATE1.wrd) ;
  nrxmit_rate0.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.NRXMIT_RATE0.wrd) ;
  pFa->nrxmit_rate = (nrxmit_rate1.bf.NRXMIT_RATE1 << 16) 
                      | nrxmit_rate0.bf.NRXMIT_RATE0 ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    GET MAC ADDRESS OF LAST HEAD OF CONGEST    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_ms_get_last_head_of_congest(cs_uint16 port_id,
                                           cs_mac * pCGSa)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to my MAC Address                     */
/* OUTPUTS    : o Pointer to my MAC Address                     */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieve the Source Address of last head of congest received */
/* by Micro-sequencer when working in RPR operation mode.       */
/*                                                              */
/* The API expects datastrutue(pMySa) has already been          */
/* allocated.                                                   */
/*                                                              */
/* NOTE: This API only works with Cortina-provided RPR Micro-   */
/* -code.                                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  cs_uint16             t_sa ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                    M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP,
                              CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  t_sa 
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG4.wrd) ;
  pCGSa->byte5 = (t_sa & 0xff00) >> 8;
  pCGSa->byte4 = (t_sa & 0x00ff);

  t_sa
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG3.wrd) ;
  pCGSa->byte3 = (t_sa & 0xff00) >> 8;
  pCGSa->byte2 = (t_sa & 0x00ff);

  t_sa
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG2.wrd) ;
  pCGSa->byte1 = (t_sa & 0xff00) >> 8;
  pCGSa->byte0 = (t_sa & 0x00ff);

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    GET MICRO-SEQUENCER SOURCE MAC ADDRESS     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_get_mac_sa(cs_uint16 port_id, cs_mac * pMySa)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to my MAC Address                     */
/* OUTPUTS    : o Pointer to my MAC Address                     */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieve the Source Address used by Micro-sequencer to       */
/* generate the fairness (usage) packet.                        */
/*                                                              */
/* The API expects datastrutue(pMySa) has already been          */
/* allocated.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  cs_uint16             t_sa ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  t_sa = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_SA0.wrd) ;
  pMySa->byte1 = (t_sa & 0xff00) >> 8;
  pMySa->byte0 = (t_sa & 0x00ff);

  t_sa = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_SA1.wrd) ;
  pMySa->byte3 = (t_sa & 0xff00) >> 8;
  pMySa->byte2 = (t_sa & 0x00ff);

  t_sa = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_SA2.wrd) ;
  pMySa->byte5 = (t_sa & 0xff00) >> 8;
  pMySa->byte4 = (t_sa & 0x00ff);

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    MICRO-SEQUENCER GET ERROR STATUS           */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint16  m2r_ms_get_error_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : Bitmap error status                             */
/* DESCRIPTION:                                                 */
/* Returns a bitmap of the error status in Microsequencer.      */
/* A '1' in the bitmap indicates the corresponding error        */
/* was detected. The definition of the bits in bitmap is as     */
/* follows:                                                     */
/*                                                              */
/* M2R_MS_INSTR_STORE_PARITY_ERR_MASK (bit 2) - Parity error    */
/* detected with Instruction Store.                             */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, 0xFFFF) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, 0xFFFF) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  return( M2R_REG_READ(&pDev->slice[sl_num].RingMAC.ERR.wrd) ) ;
}

/****************************************************************/
/* $rtn_hdr_start    MICRO-SEQUENCER GET PROGRAM COUNTER(PC)    */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint16  m2r_ms_get_pc(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : Program Counter(PC)                             */
/* DESCRIPTION:                                                 */
/* Returns Microsequencer's Program Counter(PC).                */
/*                                                              */
/* Note: When the application wants to debug the Micro-         */
/* sequencer using this API, it should call API to stall the    */
/* Micro-sequencer first.                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, 0xFFFF) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, 0xFFFF) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  return( M2R_REG_READ(&pDev->slice[sl_num].RingMAC.PC.wrd) ) ;
}

/*********************** Micro-code Develope ********************/
/****************************************************************/
/* $rtn_hdr_start    MICRO-SEQUENCER RUNNING CONTROL            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_running_step(cs_uint16 port_id, cs_uint16 cnt)
/* INPUTS     : o Port Id                                       */
/*              o The count value for single step               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the count value for single step running of Micro-       */
/* sequencer.                                                   */
/*                                                              */
/* When [cnt] is a value other than 0xFFFF, the Micro-sequencer */
/* will execute for [cnt] clocks and then be stalled. When [cnt]*/
/* has a value of '0x0001', it makes Micro-sequencer single     */
/* stepping.                                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;

  /* Stall Micro-sequencer first */
  if ( m2r_ms_ctl_stall(port_id, M2R_ENABLE)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CCR.wrd, cnt);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER RELATIVE ADDRESSING    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_idx(cs_uint16 port_id, cs_uint8 mask, 
                          cs_uint8 idx)
/* INPUTS     : o Port Id                                       */
/*              o Index selection mask value                    */
/*              o Index register value                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the index and mask value for relative addressing.       */
/*                                                              */
/* [mask] provides up to 5-bit mask.                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_OPTIONS_t       options;
  cs_boolean            stall;
  DR_MS_IX_t            ix;
  DR_MS_IX_SEL_t        sel;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  if ( mask & (~M2R_MS_IX_BITMAP_ALL) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
            " : MS relative address index bitmap is up to 0x%.2X\n",
                  M2R_MS_IX_BITMAP_ALL) ;
    return (CS_ERROR) ;
  }
  
  if ( idx > M2R_MS_IX_BITMAP_ALL ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
            " : MS relative address index should be less than 0x%.2X\n",
                  M2R_MS_IX_BITMAP_ALL) ;
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* stall Micro-sequencer first if MS is unstalling */
  options.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd);
  stall = (options.bf.STALL) ? TRUE : FALSE;

  if ( stall == FALSE ) {
    /* Disable timer first */
    m2r_ms_set_timer_val(port_id, M2R_DISABLE, 0, M2R_DISABLE, 0);
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  if ( stall == FALSE ) {
    options.bf.STALL = 1;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, 
                 options.wrd);
  }

  ix.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IX.wrd);
  ix.bf.IX = idx;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IX.wrd, ix.wrd);

  sel.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IX_SEL.wrd);
  sel.bf.IX_SEL = mask;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IX_SEL.wrd, sel.wrd);
  
  /* unstall MS if MS is previously unstalling */
  if ( stall == FALSE ) {
    options.bf.STALL = 0;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, 
                 options.wrd);
  }
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  if ( stall == FALSE ) {
    /* Enable timer then */
    m2r_ms_set_timer_val(port_id, M2R_ENABLE, 0, M2R_ENABLE, 0);
  }

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER JUMP INDEX             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_jmp_idx(cs_uint16 port_id, cs_uint16 idx) 
/* INPUTS     : o Port Id                                       */
/*              o Jump Index selection value                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the jump index value for relative jump instruction.     */
/*                                                              */
/* [idx] supports the value up to 0x1ff.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_OPTIONS_t       options;
  cs_boolean            stall;
  DR_MS_JIX_t           jix;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  if ( idx > M2R_MS_MAX_JUMP_IDX ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
            " : MS relative jump index should be less than 0x%.4X\n",
                  M2R_MS_MAX_JUMP_IDX) ;
    return (CS_ERROR) ;
  }
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* stall Micro-sequencer first if MS is unstalling */
  options.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd);
  stall = (options.bf.STALL) ? TRUE : FALSE;

  if ( stall == FALSE ) {
    /* Disable timer first */
    m2r_ms_set_timer_val(port_id, M2R_DISABLE, 0, M2R_DISABLE, 0);
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  if ( stall == FALSE ) {
    options.bf.STALL = 1;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, 
                 options.wrd);
  }

  jix.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.JIX.wrd);
  jix.bf.JIX = idx;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.JIX.wrd, jix.wrd);

  /* unstall MS if MS is previously unstalling */
  if ( stall == FALSE ) {
    options.bf.STALL = 0;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, 
                 options.wrd);
  }
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  if ( stall == FALSE ) {
    /* Enable timer then */
    m2r_ms_set_timer_val(port_id, M2R_ENABLE, 0, M2R_ENABLE, 0);
  }

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SET MICRO-SEQUENCER PC                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_set_pc(cs_uint16 port_id, cs_uint16 cnt) 
/* INPUTS     : o Port Id                                       */
/*              o Specified new Program Counter                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the Micro-sequencer program counter.                    */
/*                                                              */
/* [cnt] supports the value up to 0x1ff.                        */
/*                                                              */
/* Please note that during this API call, the low priority      */
/* packets from the host will be back-pressured.                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_OPTIONS_t       options;
  cs_boolean            stall;
  DR_MS_PC_t            pc;
  m2r_ctl_t             t0_ctl=M2R_DISABLE, t1_ctl=M2R_DISABLE;
  DR_MS_TIMER_CR_t      timer_cr;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  if ( cnt > M2R_MS_MAX_PC ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG, 
            " : MS program counter should be less than 0x%.4X\n",
                  M2R_MS_MAX_PC) ;
    return (CS_ERROR) ;
  }
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  /* stall Micro-sequencer first if MS is unstalling */
  options.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd);
  stall = (options.bf.STALL) ? TRUE : FALSE;

  if ( stall == FALSE ) {
    /* Disable timer first */
    timer_cr.wrd
       = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TIMER_CR.wrd) ;
    t0_ctl = (timer_cr.bf.T0ENAB == 0) ? M2R_DISABLE : M2R_ENABLE ;
    t1_ctl = (timer_cr.bf.T1ENAB == 0) ? M2R_DISABLE : M2R_ENABLE ;

    m2r_ms_set_timer_val(port_id, M2R_DISABLE, 0, M2R_DISABLE, 0);
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  if ( stall == FALSE ) {
    options.bf.STALL = 1;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, 
                 options.wrd);
  }

  pc.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.PC.wrd);
  pc.bf.PCT = cnt;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.PC.wrd, pc.wrd);

  /* unstall MS if MS is previously unstalling */
  if ( stall == FALSE ) {
    options.bf.STALL = 0;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.OPTIONS.wrd, 
                 options.wrd);
  }
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;

  if ( stall == FALSE ) {
    /* Enable timer then */
    m2r_ms_set_timer_val(port_id, t0_ctl, 0, t1_ctl, 0);
  }

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    SEND SEMAPHORE TO MICRO-SEQUENCER          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_send_sema(cs_uint16 port_id, cs_uint8 sema_tms)
/* INPUTS     : o Port Id                                       */
/*              o Semaphore value                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Send semaphore to Micro-sequencer, which is the effective    */
/* way for the Micro-processor to communicate with Micro-code   */
/* executed by Micro-sequencer.  The semaphone value is per-bit */
/* based.  Whenever the semaphore bit is set, only Micro-code   */
/* can clear it.                                                */
/*                                                              */
/* Note: This API needs the support of Micro-code.  Current     */
/* version of Micro-code does not respond any semaphore sent by */
/* the Mico-processor, i.e., this API has no effect on current  */
/* Micro-sequencer.                                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_SEMA4_t         sema4;
  cs_uint8              ltms;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  sema4.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SEMA4.wrd) ;

  if ( (ltms = (sema4.bf.TMS & sema_tms)) ) {
    CS_HNDL_ERROR(port_id, EM2R_MS_UNCLEAR_SEMA, 
                 ": conflict semaphore 0x%.2x\n", ltms) ;
    M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
    return(CS_ERROR) ;
  }

  sema4.bf.TMS |= sema_tms;
  /* Leave FMS as ZERO since semaphore from MS could be cleared *
   * by writing '1', which should not be done here.             */
  sema4.bf.FMS = 0;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SEMA4.wrd, sema4.wrd);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    GET SEMAPHORE STATUS                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_get_sema(cs_uint16 port_id, cs_uint8 * psema_fms,
                           cs_uint8 * psema_tms)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to Semaphore from Micro-sequencer     */
/*              o Pointer to Semaphore to Micro-sequencer       */
/* OUTPUTS    : o Semaphore value sent from Micro-sequencer     */
/*              o Semaphore value sent to Micro-sequencer       */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Retrieve the current semaphore values.                       */
/*                                                              */
/* Note: The application can use this API together with the     */
/* API 'm2r_ms_send_sema' to implement the blocking synchronized*/
/* communication with Micro-code.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_SEMA4_t         sema4;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  sema4.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SEMA4.wrd) ;
     
  *psema_fms = (cs_uint8)sema4.bf.FMS;
  *psema_tms = (cs_uint8)sema4.bf.TMS;

  return (CS_OK);
}

/****************************************************************/
/* $rtn_hdr_start    CLEAR SEMAPHORE STATUS SENT BY MS          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_ms_clear_out_sema(cs_uint16 port_id, 
                                 cs_uint8 clr_fms_mask)
/* INPUTS     : o Port Id                                       */
/*              o Bit Mask for clearing SEMA val from MS        */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Clears the semaphore bits sent by Micro-sequencer based on   */
/* specified bit mask.                                          */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MS_SEMA4_t         sema4;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS,
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  sema4.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SEMA4.wrd) ;
     
  sema4.bf.FMS &= clr_fms_mask;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SEMA4.wrd, sema4.wrd);
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/*
 * PRIVATE APIs
 */
/****************************************************************/
/*           GET MICRO-SEQUENCER RUNNING CONFIGURATION          */
/****************************************************************/
cs_status m2r_ms_get_running_cfg(cs_uint16 port_id,
                                  m2r_rmac_ms_runcfg_t * pRunCfg)
{
  cs_uint16                   dev_num, sl_num ;
  M2R_t                       * pDev ;

  DR_MS_TIMER_CR_t            timer_cr;
  DR_MICRO_IF_SRP_HDR_t       srp_hdr;
  DR_MS_COEFF_t               coeff ;
  DR_MS_COEFF2_t              coeff2 ;
  DR_MS_RSVD_MSG_t            rsvd_msg ;
  DR_MS_SEMA4_t               sema4;
  
  cs_uint16                   data ;
  cs_uint32                   tmp ;
  cs_uint64                   tmp1, tmp2 ; 
  
  if ( (pRunCfg->ms_mode = m2r_ms_get_running_mode(port_id))
       == M2R_MS_UNKNOWN ) {
    return (CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;  
  
  timer_cr.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TIMER_CR.wrd) ;
  
  if ( timer_cr.bf.T0ENAB ) {
    pRunCfg->t0_en = CS_ENABLE ;
  } else {
    pRunCfg->t0_en = CS_DISABLE ;
  }
  if ( timer_cr.bf.T1ENAB ) {
    pRunCfg->t1_en = CS_ENABLE ;
  } else {
    pRunCfg->t1_en = CS_DISABLE ;
  }

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.T0_CNT0.wrd) ;
  tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.T0_CNT1.wrd) ;
  tmp = (tmp << 16 ) + data;

  pRunCfg->agingInterval
    = tmp / (M2R_CORE_CLOCK / 10) ;

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.T1_CNT0.wrd) ;
  tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.T1_CNT1.wrd) ;
  tmp = (tmp << 16 ) + data;

  pRunCfg->advertisementInterval
    = tmp / (M2R_CORE_CLOCK / 10) ;

  pRunCfg->prov
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.PROV1.wrd) ;
  pRunCfg->prov = (pRunCfg->prov<<16)
    + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.PROV0.wrd) ;

  srp_hdr.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SRP_HDR.wrd) ;
  pRunCfg->ttl_prov = srp_hdr.bf.SRP_TTL ;

  pRunCfg->max_allow
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MAX_ALLOW.wrd) ;

  coeff.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF.wrd) ;

  pRunCfg->agecoeff = coeff.bf.AGECOEFF ;
  pRunCfg->lp_allow = coeff.bf.LP_ALLOW ;
  pRunCfg->lp_fwd = coeff.bf.LP_FWD ;
  pRunCfg->lp_mu = coeff.bf.LP_MU ;

  coeff2.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.COEFF2.wrd) ;
  pRunCfg->lp_nrXmit = coeff2.bf.LP_NRXMIT ;

  rsvd_msg.wrd
    = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RSVD_MSG.wrd) ;

  if ( pRunCfg->ms_mode == M2R_MS_SRP ) {
    pRunCfg->rsvd_msg = rsvd_msg.wrd ;

    pRunCfg->stqHighThresh
      = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TB_HTHRESH.wrd) ;
    pRunCfg->stqLowThresh
      = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TB_LTHRESH.wrd) ;

    pRunCfg->lr_rate = ( pRunCfg->prov * 1000 * 8 )
                       / pRunCfg->agingInterval ;
  } else {
    pRunCfg->wrappingProtection
      = (rsvd_msg.wrd & 0x8000) ? TRUE : FALSE ;
    pRunCfg->Gen_MCFF
      = (rsvd_msg.wrd & 0x0001) ? CS_ENABLE : CS_DISABLE ;
    pRunCfg->Active_W_En
      = (rsvd_msg.wrd & 0x0002) ? CS_ENABLE : CS_DISABLE ;

    pRunCfg->rampDncoeff
      = (rsvd_msg.wrd >> 8) & 0xf ;
    pRunCfg->starvecoeff
      = (rsvd_msg.wrd >> 12) & 0x7 ;

    {
      DR_MS_SPARE_REG7_t      spare_reg7 ;

      spare_reg7.wrd
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG7.wrd) ;

      /* number of T0 */
      pRunCfg->keepAliveMaxTimer = (spare_reg7.wrd >> 1) & 0xfff ;
    }

    {
      DR_MS_TTL_TO_WRAP_t     ttl_to_wrap;

      ttl_to_wrap.wrd
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TTL_TO_WRAP.wrd) ;

      pRunCfg->ttl_to_wrap = ttl_to_wrap.bf.TTL_TO_WRAP ;
    }

    {
      tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.UNRESV_RATE1.wrd) ;
      tmp = (tmp << 16)
          + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.UNRESV_RATE0.wrd) ;
      pRunCfg->unresvd_rate = tmp ;
    }

    pRunCfg->rampUpcoeff = coeff2.bf.RAMPCOEFF ;
    pRunCfg->ratecoeff = M2R_MS_GET_RPR_RATECOEFF(coeff2.bf.RATECOEFF) ;

    {
      DR_MS_MY_WEIGHT_t     my_weight;

      my_weight.wrd
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MY_WEIGHT.wrd) ;

      pRunCfg->my_weight = my_weight.bf.MY_WEIGHT ;

    }

    if ( (pRunCfg->ms_mode == M2R_MS_RPR_AGGRESSIVE_SINGLE_TB) ||
         (pRunCfg->ms_mode == M2R_MS_RPR_CONSERVATIVE_SINGLE_TB) ) {
      tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RATE_HIGH_TH1.wrd) ;
      tmp = (tmp<<16)
          + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RATE_HIGH_TH0.wrd) ;
      pRunCfg->rate_high_th = tmp ;

      tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RATE_LOW_TH1.wrd) ;
      tmp = (tmp<<16)
          + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RATE_LOW_TH0.wrd) ;
      pRunCfg->rate_low_th = tmp ;

      tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DQT_A1.wrd) ;
      tmp = (tmp<<16)
          + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DQT_A0.wrd) ;
      pRunCfg->classAAccessDelayTimerThreshold
        = tmp / M2R_RMAC_DQT_1US_TMR_CNT ;

      tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DQT_B1.wrd) ;
      tmp = (tmp<<16)
          + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DQT_B0.wrd) ;
      pRunCfg->classBAccessDelayTimerThreshold
        = tmp / M2R_RMAC_DQT_1US_TMR_CNT ;

      tmp = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DQT_C1.wrd) ;
      tmp = (tmp<<16)
          + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.DQT_C0.wrd) ;
      pRunCfg->classCAccessDelayTimerThreshold
        = tmp / M2R_RMAC_DQT_1US_TMR_CNT ;
    } else {
      pRunCfg->stqFullThresh
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TB_HTHRESH.wrd) ;
      pRunCfg->stqHighThresh
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TB_MTHRESH.wrd) ;
      pRunCfg->stqLowThresh
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG1.wrd) ;
      pRunCfg->stqMedThresh
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG0.wrd) ;
    }

    {
      DR_MS_MCFAIR_CR_t mcfair_cr ;

      mcfair_cr.wrd
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MCFAIR_CR.wrd) ;

      pRunCfg->mcfair_en
        = (mcfair_cr.bf.MC_EN) ? CS_ENABLE : CS_DISABLE ;
      pRunCfg->mcfair_t1
        = (mcfair_cr.bf.MC_T0_T1) ? TRUE : FALSE ;
      pRunCfg->mcfair_th = mcfair_cr.bf.MCFAIR_TH ;
    }

    {
      DR_MS_ACTVWGHT_CR_t   actvwght_cr ;

      actvwght_cr.wrd
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.ACTVWGHT_CR.wrd) ;

      pRunCfg->actweight_en
        = (actvwght_cr.bf.AW_EN) ? CS_ENABLE : CS_DISABLE ;
      pRunCfg->actweight_t1
        = (actvwght_cr.bf.AW_T0_T1) ? TRUE : FALSE ;
      pRunCfg->actweight_th = actvwght_cr.bf.ACTVWGHT_TH ;
    }

    {
      DR_MS_RTT1_CR_t       rtt1_cr ;

      rtt1_cr.wrd
        = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RTT1_CR.wrd) ;

      pRunCfg->rtt_en
        = (rtt1_cr.bf.RTT_EN) ? CS_ENABLE : CS_DISABLE ;
      pRunCfg->rtt_t1
        = (rtt1_cr.bf.RTT_T0_T1) ? TRUE : FALSE ;
      tmp = rtt1_cr.bf.RTT_TH ;
      tmp = (tmp << 16)
          + M2R_REG_READ(&pDev->slice[sl_num].RingMAC.RTT0_CR.wrd) ;
      pRunCfg->rtt_th = tmp ;
    }

    pRunCfg->lr_rate = ((pRunCfg->prov >> pRunCfg->agecoeff) * 1000 * 8)
                       / pRunCfg->agingInterval ;
  }

  tmp1 = (cs_uint64) 16 * 8 * 100000 * 1000 ;
  tmp2 = (cs_uint64) (pRunCfg->lr_rate) * (pRunCfg->advertisementInterval) ;

  pRunCfg->advertisementRatio = (cs_uint32)(tmp1 / tmp2) ;

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR_SR.wrd) ;
  pRunCfg->ms_sr = data ;

  sema4.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SEMA4.wrd) ;
  pRunCfg->fms_sema = sema4.bf.FMS ;

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG5.wrd) ;
  pRunCfg->fms_sema_status = data & 0xff ;

  return (CS_OK) ;
}

/****************************************************************/
/*           DUMP MICRO-SEQUENCER RUNNING CONFIGURATION         */
/****************************************************************/
cs_status m2r_ms_dump_running_cfg(cs_uint16 port_id,
                                  m2r_rmac_runcfg_t * pRmacRunCfg)
{
  cs_uint8             ttl_to_congest ;
  cs_mac               hoc_mac ;
  m2r_rmac_ms_runcfg_t * pRunCfg ;
  cs_uint32            tmp=0 ;
  cs_uint32            resvd_rate=0 ;

  if ( pRmacRunCfg == NULL ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pRunCfg = &pRmacRunCfg->ms ;

  if (pRunCfg->ms_mode == M2R_MS_UNKNOWN) {
    return (CS_ERROR) ;
  }

  if (m2r_ms_get_last_head_of_congest(port_id, &hoc_mac)
        != CS_OK) {
    return (CS_ERROR) ;
  }

  ttl_to_congest = m2r_ms_get_ttl_to_congest(port_id) ;

  CS_PRINT("\n\t *** Micro-sequencer ***\n") ;

  CS_PRINT("Ring Link Rate         = %d kbps\n",
           pRunCfg->lr_rate) ;

  CS_PRINT("AgingInterval          = %d us\n",
           pRunCfg->agingInterval) ;
  CS_PRINT("AdvertisementInterval  = %d us\n",
           pRunCfg->advertisementInterval) ;
  CS_PRINT("AdvertisementRatio     = %d x 0.00001\n",
           pRunCfg->advertisementRatio) ;

  CS_PRINT("TTL_PROV               = %d\n",
           pRunCfg->ttl_prov) ;
  CS_PRINT("MS_MAX_ALLOW           = %d bytes per ageCoef agingInterval\n",
           (pRunCfg->max_allow * 16)) ;

  if (pRunCfg->ms_mode == M2R_MS_SRP) {
    CS_PRINT("MS_PROV                = %d bytes per agingInterval\n",
             pRunCfg->prov) ;

    CS_PRINT("SRP ageCoef            = %d\n",
             (1 << pRunCfg->agecoeff)) ;
    CS_PRINT("SRP lp_allow           = %d\n",
             (1 << pRunCfg->lp_allow)) ;
    CS_PRINT("SRP lp_fwd             = %d\n",
             (1 << pRunCfg->lp_fwd)) ;
    CS_PRINT("SRP lp_mu              = %d\n",
             (1 << pRunCfg->lp_mu)) ;
    CS_PRINT("SRP lp_nrXmit          = %d\n",
             (1 << pRunCfg->lp_nrXmit)) ;

    tmp = pRunCfg->stqHighThresh * 256 ;
    CS_PRINT("STQ High Threshold     = %d (0x%x) bytes\n",
             tmp, tmp) ;
    tmp = pRunCfg->stqLowThresh * 256 ;
    CS_PRINT("STQ Low Threshold      = %d (0x%x) bytes\n",
             tmp, tmp) ;

    CS_PRINT("SRP Reserved Msg       = 0x%.4X\n",
             pRunCfg->rsvd_msg) ;
  } else {
    resvd_rate = (pRunCfg->lr_rate - ((pRunCfg->unresvd_rate * 1000)
               / ((1 << pRunCfg->agecoeff) * pRunCfg->agingInterval)) * 8) ;

    CS_PRINT("RPR reservedRate       = %d kbps\n",
             resvd_rate) ;

    CS_PRINT("MS_PROV                = %d bytes per ageCoef agingInterval\n",
             pRunCfg->prov) ;
    CS_PRINT("UnreservedRate         = %d bytes per ageCoef agingInterval\n",
             pRunCfg->unresvd_rate) ;

    CS_PRINT("RPR ageCoef            = %d\n",
             (1 << pRunCfg->agecoeff)) ;
    CS_PRINT("RPR rampDnCoef         = %d\n",
             (1 << pRunCfg->rampDncoeff)) ;
    CS_PRINT("RPR rampUpCoef         = %d\n",
             (1 << pRunCfg->rampUpcoeff)) ;
    CS_PRINT("RPR starveFactor       = 1/%d\n",
             (1 << pRunCfg->starvecoeff)) ;
    if ( pRunCfg->ratecoeff >= 0 ) {
      CS_PRINT("RPR rateCoef           = %d\n",
               (1 << pRunCfg->ratecoeff)) ;
    } else {
      CS_PRINT("RPR rateCoef           = 1/%d\n",
               (1 << (-pRunCfg->ratecoeff))) ;
    }

    CS_PRINT("RPR lp_fwd (lpCoef)    = %d\n",
             (1 << pRunCfg->lp_fwd)) ;
    CS_PRINT("RPR lp_mu  (lpCoef)    = %d\n",
             (1 << pRunCfg->lp_mu)) ;
    CS_PRINT("RPR lp_nrXmit (lpCoef) = %d\n",
             (1 << pRunCfg->lp_nrXmit)) ;

    CS_PRINT("MY Local Weight        = %d \n",
             (1 << pRunCfg->my_weight)) ;

    if ( (pRunCfg->ms_mode == M2R_MS_RPR_CONSERVATIVE_DUAL_TB) ||
         (pRunCfg->ms_mode == M2R_MS_RPR_AGGRESSIVE_DUAL_TB) ) {
      tmp = pRunCfg->stqFullThresh * 256 ;
      CS_PRINT("STQ Full Threshold     = %d (0x%x) bytes\n",
               tmp, tmp) ;
      tmp = pRunCfg->stqHighThresh * 256 ;
      CS_PRINT("STQ High Threshold     = %d (0x%x) bytes\n",
               tmp, tmp) ;
      tmp = pRunCfg->stqLowThresh * 256 ;
      CS_PRINT("STQ Low Threshold      = %d (0x%x) bytes\n",
               tmp, tmp) ;
      tmp = pRunCfg->stqMedThresh * 256 ;
      CS_PRINT("STQ Medium Threshold   = %d (0x%x) bytes\n",
               tmp, tmp) ;
    } else {
      CS_PRINT("RPR rateHighThreshold  = %d bytes per ageCoef agingInterval\n",
               pRunCfg->rate_high_th) ;
      CS_PRINT("RPR rateLowThreshold   = %d bytes per ageCoef agingInterval\n",
               pRunCfg->rate_low_th) ;

      CS_PRINT("RPR classAAccessDelayTimerThreshold = %d us\n",
               pRunCfg->classAAccessDelayTimerThreshold) ;
      CS_PRINT("RPR classBAccessDelayTimerThreshold = %d us\n",
               pRunCfg->classBAccessDelayTimerThreshold) ;
      CS_PRINT("RPR classCAccessDelayTimerThreshold = %d us\n",
               pRunCfg->classCAccessDelayTimerThreshold) ;
    }

    CS_PRINT("Fairness Wrappable     = %s\n",
             ((pRunCfg->wrappingProtection) ? "TRUE" : "FALSE")) ;

    CS_PRINT("KeepAlive Max Timer    = %d us\n",
             (pRunCfg->keepAliveMaxTimer * pRunCfg->agingInterval)) ;

    CS_PRINT("TTL_TO_WRAP            = %d \n",
             pRunCfg->ttl_to_wrap) ;

    CS_PRINT("MCFF Fairness Gen      = %s\n",
             ((pRunCfg->Gen_MCFF == CS_ENABLE)
               ? "ENABLED" : "DISABLED")) ;

    CS_PRINT("ActiveWeightsDetection = %s\n",
             ((pRunCfg->Active_W_En == CS_ENABLE)
               ? "ENABLED" : "DISABLED")) ;

    CS_PRINT("MCFF Timer Status      = %s\n",
             ((pRunCfg->mcfair_en == CS_ENABLE)
               ? "ENABLED" : "DISABLED")) ;
    CS_PRINT("MCFF Timer Mode        = %s\n",
             ((pRunCfg->mcfair_t1) ? "Timer_T1" : "Timer_T0"));
    CS_PRINT("RPR ReportCoef         = %d times of timer %s expired\n",
             pRunCfg->mcfair_th,
             ((pRunCfg->mcfair_t1) ? "T1" : "T0"));

    CS_PRINT("ACWT Timer Status      = %s\n",
             ((pRunCfg->actweight_en == CS_ENABLE)
               ? "ENABLED" : "DISABLED")) ;
    CS_PRINT("ACWT Timer Mode        = %s\n",
             ((pRunCfg->actweight_t1) ? "Timer_T1" : "Timer_T0"));
    CS_PRINT("RPR ActiveWeightsCoef  = %d times of timer %s expired\n",
             pRunCfg->actweight_th,
             ((pRunCfg->actweight_t1) ? "T1" : "T0"));

    CS_PRINT("RTT Timer Status       = %s\n",
             ((pRunCfg->rtt_en == CS_ENABLE)
               ? "ENABLED" : "DISABLED")) ;
    CS_PRINT("RTT Timer Mode         = %s\n",
             ((pRunCfg->rtt_t1) ? "Timer_T1" : "Timer_T0"));
    CS_PRINT("RPR Frtt               = %d times of timer %s expired\n",
             pRunCfg->rtt_th,
             ((pRunCfg->rtt_t1) ? "T1" : "T0"));

    if ((pRmacRunCfg->ring_mode == M2R_RING_NORMAL) ||
        (pRmacRunCfg->ring_mode == M2R_RING_WRAP)) {
      CS_PRINT("MS KeepAlive Status    = %s\n",
               ((pRunCfg->fms_sema_status & 0x2) ? "Failed" : "Alive")) ;
      CS_PRINT("MS iAmTail             = %s\n",
               ((pRunCfg->fms_sema_status & 0x1) ? "TRUE" : "FALSE")) ;
      CS_PRINT("TTL_TO_CONGEST         = %d \n",
               ttl_to_congest) ;
      CS_PRINT("Last Head of Congest   = %.2x.%.2x.%.2x.%.2x.%.2x.%.2x\n",
               hoc_mac.byte5, hoc_mac.byte4,
               hoc_mac.byte3, hoc_mac.byte2,
               hoc_mac.byte1, hoc_mac.byte0) ;
    }
  }

  CS_PRINT("Timer T0 Status        = %s\n",
           ((pRunCfg->t0_en == CS_ENABLE)
             ? "ENABLED" : "DISABLED")) ;
  CS_PRINT("Timer T1 Status        = %s\n",
           ((pRunCfg->t1_en == CS_ENABLE)
             ? "ENABLED" : "DISABLED")) ;

  CS_PRINT("MS_TIMEOUT             = %s\n",
           ((pRunCfg->ms_sr & 0x1) ? "TRUE" : "FALSE")) ;

  return (CS_OK) ;
}

m2r_rmac_ms_mode_t m2r_ms_get_running_mode(cs_uint16 port_id)
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0 ;
  cs_uint32             err_code=0 ;
  m2r_port_mac_mode_t  mode = M2R_GET_MAC_MODE(port_id) ;
  
  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    return(M2R_MS_UNKNOWN) ;
  }
  if (mode == M2R_PORT_IN_BYPASS_MODE) {
    return(M2R_MS_UNKNOWN) ;
  }
  
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;
  
  cr0.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;
     
  if (cr0.bf.RPRMODE == 0)
    return (M2R_MS_SRP) ;

  if (cr0.bf.ONETB) {
  	if (cr0.bf.CONSERV) {
  	  return (M2R_MS_RPR_CONSERVATIVE_SINGLE_TB) ;
    } else {
  	  return (M2R_MS_RPR_AGGRESSIVE_SINGLE_TB) ;
	}
  } else {
  	if (cr0.bf.CONSERV) {
  	  return (M2R_MS_RPR_CONSERVATIVE_DUAL_TB) ;
    } else {
  	  return (M2R_MS_RPR_AGGRESSIVE_DUAL_TB) ;
	}
  }
   
}

/* Set NEW_TTL_TO_CONGEST SEMAphore control */
static cs_status m2r_ms_ctl_new_ttl_to_congest_sema_out(cs_uint16 port_id,
                                                m2r_ctl_t ctl)
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  cs_uint16             data ;
  cs_boolean            reg_access = TRUE ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, 
                      M2R_INVALID_IN_BYPASS | M2R_INVALID_IN_SRP,
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  data = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG7.wrd) ;

  if ( ctl == M2R_ENABLE ) {
    if ( data & 0x1 ) {
      reg_access = FALSE ;
    } else {
      data |= 0x0001 ;
    }
  } else {
    if ( data & 0x1 ) {
      data &= 0xfffe ;
    } else {
      reg_access = FALSE ;
    }
  }

  if ( reg_access ) {
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.SPARE_REG7.wrd,
                 data);
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* _rtn_hdr_start    SET RPR FAIRNESS PACKET TYPE               */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
static cs_status  m2r_ms_set_fa_pkt_type(cs_uint16 port_id,
                                  cs_boolean long_pkt,
                                  cs_uint8 long_pkt_length)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(Long Packet) or FALSE(Short Packet)      */
/*              o Long packet length                            */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the type of RPR fairness packet generated by Micro-     */
/* sequencer.                                                   */
/*                                                              */
/* If [long_pkt] is FALSE, [long_pkt_length] is ignored.        */
/*                                                              */
/* Note: This API is only valid when RingMAC works in RPR mode. */
/* In SRP mode, the usage(fairness) packet has a fixed length   */
/* 16byte.  In RPR mode, fairness packet has two kinds of       */
/* fomat: short packet(16byte) and long packet(24byte-32byte).  */
/* However, in RPR IEEE standard 802.17-2004, only short packet */
/* is valid.                                                    */
/* _rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id,
                             (M2R_INVALID_IN_BYPASS
                              | M2R_INVALID_IN_SRP),
                             CS_ERROR) ;

  if (long_pkt && ( long_pkt_length<M2R_LONG_FA_PACKET_BASE_LENGTH
     || long_pkt_length>M2R_LONG_FA_PACKET_MAX_LENGTH )) {
    CS_HNDL_ERROR(port_id, EM2R_MS_INVALID_FA_PKT_LENGTH,
                  ": %d\n", long_pkt_length) ;
    return(CS_ERROR) ;
  }

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr0.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

  if (long_pkt) {
    cr0.bf.UTYPE = 1;
    cr0.bf.ULEN = long_pkt_length-M2R_LONG_FA_PACKET_BASE_LENGTH;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd);
  } else {
    cr0.bf.UTYPE = 0;
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd);
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}

/****************************************************************/
/* _rtn_hdr_start    SET RPR FAIRNESS ALGORITHM MODE            */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
static cs_status  m2r_ms_set_fa_algorithm(cs_uint16 port_id, 
                                          cs_boolean conserv)
/* INPUTS     : o Port Id                                       */
/*              o TRUE(Conservative) or FALSE(Aggressive)       */
/* OUTPUTS    : ---                                             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the mode of RPR fairness algorithm applied onto Micro-  */
/* sequencer.                                                   */
/*                                                              */
/* Note: This API is only valid when RingMAC works in RPR mode. */
/* _rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             dev_num, sl_num ;
  M2R_t                 * pDev ;
  DR_MICRO_IF_CR0_t     cr0;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id,
                             (M2R_INVALID_IN_BYPASS
                              | M2R_INVALID_IN_SRP),
                             CS_ERROR) ;

  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  cr0.wrd
     = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR0_.wrd) ;

  cr0.bf.CONSERV = (conserv) ? 1 : 0;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR0_.wrd, cr0.wrd);

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK);
}


