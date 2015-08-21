/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_FRAMER_H__
#define __M2R_FRAMER_H__

#include "cs_types.h"
#include "m2r_registers.h"
#include "m2r_public.h"
#include "m2r_framer_cmn.h"

/***********************************************************************
 * Framer Clock Domain
 ***********************************************************************/
typedef enum {
  M2R_FR_DOMAIN_SONET_RX        = 0x0800,   /* 311MHz     */
  M2R_FR_DOMAIN_SONET_TX        = 0x0200,   /* 311MHz     */
  M2R_FR_DOMAIN_DATACOM         = 0x0100,   /* 200MHz     */
  M2R_FR_ALL_DOMAINS            = 0x0b00    /* select all */
} m2r_framer_domain_t ;

/***********************************************************************
 * STS MODES
 ***********************************************************************/
typedef enum {
  M2R_FR_MODE_OC192c = 0,       /* oc192c Mode                        */
  M2R_FR_MODE_OC192,            /* oc192 channelized Mode (4 x oc48c) */
  M2R_FR_MODE_OC48c,            /* oc48c Mode                         */
  M2R_FR_MODE_OC48              /* oc48 channelized Mode (4 x oc12c)  */
} m2r_framer_stsType_t ;

typedef enum {
  M2R_FR_RATETYPE_STS192C = 0,  /* STS-192C Mode                  */
  M2R_FR_RATETYPE_STS48C,       /* STS-48C Mode                   */
  M2R_FR_RATETYPE_STS12C,       /* STS-12C Mode                   */
  M2R_FR_RATETYPE_OC192 = M2R_FR_RATETYPE_STS192C,
  M2R_FR_RATETYPE_OC48 =  M2R_FR_RATETYPE_STS48C,
  M2R_FR_RATETYPE_OC12 =  M2R_FR_RATETYPE_STS12C
} m2r_framer_rateType_t ;

/***************************************************************
 * Service types supported by Framer
 ***************************************************************/
typedef enum {
  M2R_FR_SVC_POS_PPP    = 0,
  M2R_FR_SVC_ATM,
  M2R_FR_SVC_POS_HDLC,
  M2R_FR_SVC_GFP,
  M2R_FR_SVC_PRBS,
  M2R_FR_SVC_CLR_CH
} m2r_framer_svc_type_t ;

/***************************************************************
 * Some defines for the default values
 ***************************************************************/
#define M2R_FRAMER_RX_ATM_BURST_DEF     (7)
#define M2R_FRAMER_RX_PKT_BURST_DEF     (8)

#define M2R_FRAMER_PKT_MIN_SIZE_DEF     (8)
#define M2R_FRAMER_PKT_MAX_SIZE_DEF     (0xffff)

/***************************************************************
 * SONET overhead related defines and enums
 ***************************************************************/
#define M2R_SONET_SECTION_OVRHD         (0x01)
#define M2R_SONET_LINE_OVRHD            (0x02)
#define M2R_SONET_PATH_OVRHD            (0x04)

/***************************************************************
 * C2 byte
 ***************************************************************/
#define M2R_SONET_C2_UNEQ               (0x00)
#define M2R_SONET_C2_NON_SPECIFIC       (0x01)
#define M2R_SONET_C2_ATM                (0x13)
#define M2R_SONET_C2_HDLC               (0x16)
#define M2R_SONET_C2_GFP                (0x1B)

#define M2R_SONET_FIX_ZO_DEF            (0xCC)

#define M2R_SONET_MAX_TRACE_LEN         (64)

/***************************************************************
 * Section/Path trace options. In save mode , the trace message
 * is stored in the trace buffer and interrupt is generated.
 * In check mode , incoming trace message is checked against
 * the message stored in trace buffer.
 ***************************************************************/
typedef enum {
  M2R_FR_MODE_CHECK =   0,
  M2R_FR_MODE_SAVE  =   1
} m2r_sonet_trace_option_t ;

/***************************************************************
 * Sonet Overhead bytes sources for TX Stream
 ***************************************************************/
typedef enum {
  M2R_FR_OVRHD_SRC_CPU      = 0,
  M2R_FR_OVRHD_SRC_TOH_PORT,
  M2R_FR_OVRHD_SRC_DCC_PORT,
  M2R_FR_OVRHD_SRC_HDLC_PORT,
  M2R_FR_OVRHD_SRC_APS_PORT,
  M2R_FR_OVRHD_SRC_RX_ALARM
} m2r_framer_tx_ovrhd_src_t ;

#define M2R_FR_MAX_TX_OVRHD_SRC         (M2R_FR_OVRHD_SRC_RX_ALARM + 1)

/***************************************************************
 * Sonet alarm sources for TX Stream
 ***************************************************************/
typedef enum {
  M2R_FR_ALM_SRC_CPU      = 0,
  M2R_FR_ALM_SRC_TOH,
  M2R_FR_ALM_SRC_APS_PORT,
  M2R_FR_ALM_SRC_RX_ALARM,
  M2R_FR_MAX_ALM_SRC
} m2r_framer_alm_src_t ;

/***************************************************************
 * TX Section Trace Message source options
 ***************************************************************/
typedef enum {
  M2R_FR_TX_TRACE_BUFFER    = 0,/* Trace Buffer         */
  M2R_FR_TX_TRACE_TOH,          /* from TOH             */
  M2R_FR_TX_TRACE_FIXED         /* fix pattern (0x01)   */
} m2r_framer_tx_trace_src_t ;

/***************************************************************
 * Framer DCC PORT MODE
 ***************************************************************/
typedef enum {
  M2R_FR_SECTION_DCC  = DCCmode_SEC_DCC,   /* Send Section DCC channels   */
  M2R_FR_LINE_DCC     = DCCmode_LIN_DCC,   /* Send Line DCC channels      */
  M2R_FR_DCC_SEC_LINE = DCCmode_SEC_LIN_DCC  /* Section and Line DCC      */
} m2r_framer_dcc_port_mode_t ;

/***************************************************************
 * Framer APS PORT MODE
 ***************************************************************/
typedef enum {
  M2R_FR_APS_BYTES = APSmode_APS_K1_K4_S1, /* K1/K2/K3/K4 and Z1/S1 bytes */
  M2R_FR_ALARMS    = APSmode_APS_RECV_ALM, /* Send Alarms only            */
  M2R_FR_APS_ALL   = APSmode_APS_K1_ALM    /* both APS bytes and Alarms   */
} m2r_aps_port_sel_t ;

/***************************************************************
 * Framer Signal Degrade threshold
 ***************************************************************/
typedef enum {
  M2R_FR_BER_3      = 3,
  M2R_FR_BER_4,
  M2R_FR_BER_5,
  M2R_FR_BER_6,
  M2R_FR_BER_7,
  M2R_FR_BER_8,
  M2R_FR_BER_9,
  M2R_FR_BER_10
} m2r_framer_signal_ber_t ;

/***************************************************************
 * Framer GFP Mode
 ***************************************************************/
typedef enum {
  M2R_FR_GFP_TRANSPARENT = 0,
  M2R_FR_GFP_NORMAL
} m2r_framer_gfp_mode_t ;

typedef enum {
  M2R_GFP_SYNC_STATUS = 0,  /* GFP SYNC STATUS                  */
  M2R_GFP_LOSS_CLIENT,      /* loss of client frame is detected */
  M2R_GFP_LOSS_SYNC         /* loss of sync frame is detected   */
} m2r_framer_gfp_rx_status_t ;

#define M2R_FR_GFP_UPI_RPR              (0x0A)

/***************************************************************
 * Default config structure for POS Mode
 ***************************************************************/
typedef struct {
  cs_boolean             crc32 ;        /* CRC16 or CRC32      */
  cs_boolean             pktFCS ;       /* FCS needed          */
  cs_boolean             passFCS ;      /* Pass FCS to HOST    */

  /* Set drop_long and drop_short flags to TRUE always!!   */
  cs_boolean             drop_long ;    /* Drop long packets   */
  cs_boolean             drop_short ;   /* Drop Short packets  */
  cs_uint32              max_pktSize ;  /* Max packet Size     */
  cs_uint32              min_pktSize ;  /* Min packet Size     */
} m2r_framer_pos_mode_cfg_t ;

/****************************************************************
 * Default config structure for ATM Mode
 ****************************************************************/
typedef struct {
  cs_boolean             check_hec ;    /* HEC Checking needed  */
  cs_boolean             drop_idle ;    /* Drop IDLE Cells      */
} m2r_framer_atm_mode_cfg_t ;

/*****************************************************************
 * Default config structure for GFP Mode
 ****************************************************************/
typedef struct {
  m2r_framer_gfp_mode_t  mode ;         /* GFP Mode             */
  cs_boolean             cut_through ;  /* CUT-THROUGH MODE     */

  /* Set drop_long and drop_short flags to TRUE always!!   */
  cs_boolean             drop_long ;    /* Drop long packets    */
  cs_boolean             drop_short ;   /* Drop Short packets   */
  cs_uint32              max_pktSize ;  /* Max packet Size      */
  cs_uint8               min_pktSize ;  /* Min packet Size      */
} m2r_framer_gfp_mode_cfg_t ;

/*****************************************************************
 * Default config structure for Framer.
 *****************************************************************/
typedef struct {
  cs_boolean             sonet_mode ;   /* SONET or SDH mode    */
  m2r_framer_stsType_t   stsType ;      /* STS Mode             */


  m2r_framer_svc_type_t  svc ;  /* Service Type(POS, ATM or GFP)*/
  union {
    m2r_framer_pos_mode_cfg_t   pos_cfg ;  /* POS configuration */
    m2r_framer_gfp_mode_cfg_t   gfp_cfg ;  /* gfp configuration */
    m2r_framer_atm_mode_cfg_t   atm_cfg ;  /* ATM configuration */
  } mode ;
  cs_boolean             terminal_loopback ;
  cs_boolean             line_loopback ;
  cs_boolean             section_loopback ;
  cs_boolean             loop_timing ;      /* obsolete, no effect */
  cs_uint8               datacomBus_burst ; /* Datacom bus burst */
  cs_boolean             payload_scrambling ;  /* Scrambling needed  */
  cs_boolean             sonet_scrambling ;
  cs_boolean             enbl_dcc_prt;      /* DCC Port Enabled  */
  m2r_framer_dcc_port_mode_t      dcc_cfg ; /* DCC port mode     */
  m2r_ctl_t              enbl_aps_prt ;     /* APS Port enabled  */
  m2r_aps_port_sel_t     aps_cfg ;          /* APS port mode     */
  cs_uint16              irq_ctl_tbl[64] ;  /* IRQ Table*/
  /* gpio bit 7..0 <-> channel 7..0 */
  cs_uint8               gpio_cfg;          /* I/O config, 0 - input */
  cs_uint8               gpio_init_out;     /* initial outputs   */
} m2r_framer_cfg_t ;

/*****************************************************************
 * running config structure for Framer.
 *****************************************************************/
typedef struct {
  cs_boolean             sync_status ;  /* Framer is in sync */
  cs_boolean             sonet_mode ;   /* SONET or SDH mode    */
  cs_boolean             channelized_mode ;
  union {
      struct pos_cfg {
        cs_boolean     crc32 ;        /* CRC16 or CRC32      */
        cs_boolean     pktFCS ;       /* FCS needed          */
        cs_boolean     pass_fcs ;     /* FCS Passed to HOST  */
        cs_boolean     payload_scrambling ;  /* Scambling enabled */
      } pos_cfg ;  /* POS configuration */

      struct gfp_cfg {
        m2r_framer_gfp_mode_t  mode ;         /* GFP Mode             */
        cs_boolean             cut_through ;  /* CUT-THROUGH MODE     */
        cs_boolean     payload_scrambling ;  /* Scambling enabled */
      } gfp_cfg ;  /* gfp configuration */

  } mode[M2R_SONET_STREAM_ALL] ;
  cs_boolean           terminal_loopback ;
  cs_boolean           line_loopback ;
  cs_boolean           section_loopback ;
  cs_boolean           loop_timing ;
  cs_uint8             datacomBus_burst[M2R_SONET_STREAM_ALL] ;
  cs_uint8             gpio_cfg;      /* I/O config, 0 - input */
} m2r_framer_runcfg_t ;


/*****************************************************************
 * Framer APIs
 *****************************************************************/
extern  cs_status m2r_framer_init_cfg(cs_uint16 port_id,
                    m2r_port_summ_t * pSumm, m2r_framer_cfg_t * pCfg) ;

extern  cs_status m2r_framer_get_default_cfg(cs_uint16 port_id,
                    m2r_port_summ_t * pSumm, m2r_framer_cfg_t * pCfg) ;

extern  cs_status m2r_framer_get_running_cfg(cs_uint16 port_id,
                    m2r_port_summ_t * pSumm, m2r_framer_runcfg_t * pCfg) ;

extern  cs_status  m2r_framer_dump_running_cfg(cs_uint16 port_id,
                                   m2r_framer_runcfg_t * pCfg) ;

extern  cs_status m2r_framer_sel_sonet_sdh(cs_uint16 port_id,
                                           cs_boolean sel_sonet) ;

extern  cs_status m2r_framer_ctl_scrambling(cs_uint16 port_id,
                                    m2r_dir_t dir, m2r_ctl_t ctl) ;

extern  cs_status  m2r_framer_sel_sts_mode(cs_uint16 port_id,
                                            m2r_framer_stsType_t sts_t) ;

extern  cs_status m2r_framer_ctl_short_frame(cs_uint16 port_id,
                                             m2r_ctl_t ctl) ;

extern  cs_status m2r_framer_set_spe_service_type(cs_uint16 port_id,
                    m2r_framer_stream_t channel, m2r_framer_svc_type_t svc) ;

extern  cs_status  m2r_framer_set_rx_burst_len(cs_uint16 port_id,
                              m2r_framer_stream_t channel, cs_uint8  burst) ;

extern  cs_status  m2r_framer_set_tx_fifo_high_mark(cs_uint16 port_id,
                                       m2r_framer_stream_t channel,
                                       cs_boolean atm, cs_uint8  threshold) ;

extern
cs_status  m2r_framer_pkt_cfg_max_min_size(cs_uint16 port_id,
                                m2r_framer_stream_t channel,
                                cs_uint32 maxPktSize,
                                cs_uint8 minPktSize, m2r_dir_t dir) ;

extern  cs_status m2r_framer_ctl_loop_timing(cs_uint16 port_id,
                                             m2r_ctl_t ctl) ;

extern  cs_status m2r_framer_ctl_terminal_loopback(cs_uint16 port_id,
                                                   m2r_ctl_t ctl) ;

extern  cs_status m2r_framer_ctl_line_loopback(cs_uint16 port_id,
                                               m2r_ctl_t ctl) ;

extern  cs_status m2r_framer_ctl_sonet_repeater(cs_uint16 port_id,
                                    m2r_ctl_t ctl, cs_boolean relay) ;

extern  cs_status m2r_framer_sreset_sequence(cs_uint16 port_id,
                                             m2r_ctl_t ctl) ;

extern  cs_status  m2r_framer_ctl_domain_sreset(cs_uint16 port_id,
                             m2r_framer_domain_t domain, m2r_ctl_t ctl) ;

extern  cs_status  m2r_framer_ctl_overhead_sreset(cs_uint16 port_id,
                                         m2r_dir_t dir, m2r_ctl_t ctl) ;

extern  cs_status  m2r_framer_ctl_sonet_sreset(cs_uint16 port_id,
                                          m2r_dir_t dir, m2r_ctl_t ctl) ;

extern  cs_status  m2r_framer_ctl_mapper_stream_sreset(cs_uint16 port_id,
             m2r_framer_stream_t channel, m2r_dir_t dir, m2r_ctl_t ctl) ;

extern  cs_status  m2r_framer_ctl_dcom_stream_sreset(cs_uint16 port_id,
             m2r_framer_stream_t channel, m2r_dir_t dir, m2r_ctl_t ctl) ;

extern  cs_boolean m2r_framer_get_sync_status(cs_uint16 port_id) ;

extern  m2r_port_prot_t m2r_framer_get_svc_type(cs_uint16 port_id,
                                        m2r_framer_stream_t channel) ;

extern  cs_boolean m2r_framer_is_in_channelised_mode(cs_uint16 port_id) ;

extern  cs_status m2r_framer_cfg_gpio(cs_uint16 port_id,
                                      cs_uint8 config,
                                      cs_uint8 mask);

extern  cs_uint8 m2r_framer_get_gpio(cs_uint16 port_id, cs_uint8 mask);

extern  cs_status m2r_framer_set_gpio(cs_uint16 port_id,
                                      cs_uint8 data,
                                      cs_uint8 mask);

#endif /* __M2R_FRAMER_H__ */


