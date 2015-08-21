/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif_pcs6.h
 *
 * LIF: Public structures and exported API's for applications.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2_LIF_PCS6_H__
#define __M2_LIF_PCS6_H__

#include "cs_types.h"

/**********************************************************************
 * Some defines for default values
 **********************************************************************/
#define M2R_LIF_PCS_BER_HIGH_DEF_THR               (16)
#define M2R_LIF_PCS_BER_HIGH_MIN_THR               (9)
#define M2R_LIF_PCS_BER_HIGH_MAX_THR               (23)

#define M2R_LIF_PCS_OOS_DEF_THR                    (16)
#define M2R_LIF_PCS_OOS_MIN_THR                    (9)
#define M2R_LIF_PCS_OOS_MAX_THR                    (23)

#define M2R_LIF_PCS_IN_SYNC_DEF_THR                (64)
#define M2R_LIF_PCS_IN_SYNC_MIN_THR                (33)
#define M2R_LIF_PCS_IN_SYNC_MAX_THR                (95)

/**********************************************************************
 * PCS64/66 IRQ
 **********************************************************************/
#define M2R_LIF_PCS6_TX_TxInt_GoodPktSatI       (0x0004)
#define M2R_LIF_PCS6_TX_TxInt_ErrorPktSatI      (0x0002)
#define M2R_LIF_PCS6_TX_TxInt_ErrorPktI         (0x0001)

#define M2R_GRP_LIF_PCS6_TX_TxInt_ALL           (0x0007)

#define M2R_LIF_PCS6_RX_RxInt_PatSyncI          (0x0800)
#define M2R_LIF_PCS6_RX_RxInt_RxSOSI            (0x0400)
#define M2R_LIF_PCS6_RX_RxInt_BerHighI          (0x0200)
#define M2R_LIF_PCS6_RX_RxInt_SyncDetI          (0x0100)
#define M2R_LIF_PCS6_RX_RxInt_ErrorBERSatI      (0x0080)
#define M2R_LIF_PCS6_RX_RxInt_ErrorPatSatI      (0x0040)
#define M2R_LIF_PCS6_RX_RxInt_ErrorPatI         (0x0020)
#define M2R_LIF_PCS6_RX_RxInt_ErrorDecSatI      (0x0010)
#define M2R_LIF_PCS6_RX_RxInt_ErrorDecI         (0x0008)
#define M2R_LIF_PCS6_RX_RxInt_GoodPktSatI       (0x0004)
#define M2R_LIF_PCS6_RX_RxInt_ErrorPktSatI      (0x0002)
#define M2R_LIF_PCS6_RX_RxInt_ErrorPktI         (0x0001)

#define M2R_GRP_LIF_PCS6_RX_RxInt_ALL           (0x0fff)


/**********************************************************************
 * LIF PCS6 PRBS Modes
 **********************************************************************/
typedef enum {
  M2R_LIF_PCS_PRBS_31,
  M2R_LIF_PCS_PRBS_RANDOM,
  M2R_LIF_PCS_SQ1,
  M2R_LIF_PCS_SQ2,
  M2R_LIF_PCS_SQ4,
  M2R_LIF_PCS_SQ8,
  M2R_LIF_PCS_PULSE0,
  M2R_LIF_PCS_PULSE1,
  M2R_LIF_PCS_DAT_LOW,
  M2R_LIF_PCS_DAT_HIGH
} m2r_lif_pcs_prbs_t ;

typedef enum {
  M2R_LIF_PCS_PRBS_RND_SEED_A,
  M2R_LIF_PCS_PRBS_RND_SEED_B,
  M2R_LIF_PCS_PRBS_RND_DATA
} m2r_lif_prbs_rnd_seed_t ;

/**********************************************************************
 * LIF PCS6 APIs
 **********************************************************************/
extern cs_boolean m2r_lif_pcs6_is_sync(cs_uint16 port_id) ;

extern cs_uint16 m2r_lif_pcs6_set_in_sync_threshold(cs_uint16 port_id,
                                             cs_uint8 thrshold) ;

extern cs_uint16 m2r_lif_pcs6_set_ber_threshold(cs_uint16 port_id,
                                                cs_uint8 thrshold) ;

extern cs_status m2r_lif_pcs6_set_oos_threshold(cs_uint16 port_id,
                                         cs_uint8 thrshold) ;

extern cs_uint16 m2r_lif_pcs6_get_ber(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern cs_status m2r_lif_pcs6_ctl_scrambling(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern cs_status m2r_lif_pcs6_cfg_sos(cs_uint16 port_id, cs_uint8 byte1,
                                             cs_uint8 byte2, cs_uint8 byte3) ;

extern cs_status m2r_lif_pcs6_ctl_sos(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern cs_uint32 m2r_lif_pcs6_get_sos(cs_uint16 port_id) ;

extern cs_status m2r_lif_pcs6_ctl_test_pattern(cs_uint16 port_id,
                                        m2r_lif_pcs_prbs_t pattern,
                                        m2r_dir_t dir,
                                        m2r_ctl_t ctl) ;

extern cs_status m2r_lif_set_prbs_rnd_seed(cs_uint16 port_id,
                                    m2r_dir_t dir,
                                    m2r_lif_prbs_rnd_seed_t type,
                                    cs_uint64 seed_A,
                                    cs_uint64 seed_B,
                                    cs_uint64 data) ;

extern cs_status m2r_lif_pcs6_inj_error(cs_uint16 port_id) ;

extern cs_uint16 m2r_lif_pcs6_get_prbs_errors(cs_uint16 port_id) ;


#endif /* __M2_LIF_PCS6_H__ */

