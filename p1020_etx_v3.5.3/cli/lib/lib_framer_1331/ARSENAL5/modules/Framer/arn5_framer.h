/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_FRAMER_H__
#define __ARN5_FRAMER_H__

#include "cs_types.h"
#include "arn5_public.h"
#include "arn5_registers.h"

/***********************************************************************
 * Framer Clock Domain
 ***********************************************************************/
typedef enum {
  ARN5_FR_DOMAIN_SONET_RX        = 0x0800,   /* 311MHz     */
  ARN5_FR_DOMAIN_SONET_TX        = 0x0200,   /* 311MHz     */
  ARN5_FR_DOMAIN_DATACOM         = 0x0100,   /* 200MHz     */
  ARN5_FR_ALL_DOMAINS            = 0x0b00,   /* select all */
} arn5_framer_domain_t ;

/***********************************************************************
 * STS TYPE
 ***********************************************************************/
typedef enum {
  ARN5_FR_STS_TYPE_STS48C = ARN5_STSType_STS_48C,       /* STS-48C      */
  ARN5_FR_STS_TYPE_STS12C = ARN5_STSType_STS_12C,       /* STS-12C      */
  ARN5_FR_STS_TYPE_STS3C = ARN5_STSType_STS_3C,         /* STS-3C       */
} arn5_framer_stsType_t ;


/***************************************************************
 * Service types supported by Framer
 ***************************************************************/
typedef enum {
  ARN5_FR_SVC_POS_PPP    = 0,
  ARN5_FR_SVC_ATM,
  ARN5_FR_SVC_POS_HDLC,
  ARN5_FR_SVC_GFP,
  ARN5_FR_SVC_LAPS,
  ARN5_FR_SVC_PRBS,
  ARN5_FR_SVC_CLR_CH
} arn5_framer_svc_type_t ;

/***************************************************************
 * Some defines for the default values
 ***************************************************************/
#define ARN5_FRAMER_RX_ATM_BURST_DEF     (7)
#define ARN5_FRAMER_RX_PKT_BURST_DEF     (8)
/*
#define ARN5_FRAMER_PKT_MIN_SIZE_DEF     (8)
#define ARN5_FRAMER_PKT_MAX_SIZE_DEF     (0xffff)
*/

#define ARN5_FRAMER_PKT_MIN_SIZE_DEF     (40)
#define ARN5_FRAMER_PKT_MAX_SIZE_DEF     (1600)
/***************************************************************
 * SONET overhead related defines and enums
 ***************************************************************/
#define ARN5_SONET_SECTION_OVRHD         (0x01)
#define ARN5_SONET_LINE_OVRHD            (0x02)
#define ARN5_SONET_PATH_OVRHD            (0x04)

/***************************************************************
 * C2 byte
 ***************************************************************/
#define ARN5_SONET_C2_UNEQ               (0x00)
#define ARN5_SONET_C2_NON_SPECIFIC       (0x01)
#define ARN5_SONET_C2_ATM                (0x13)
#define ARN5_SONET_C2_HDLC               (0x16)
#define ARN5_SONET_C2_LAPS               (0x18)
#define ARN5_SONET_C2_GFP                (0x1B)

#define ARN5_SONET_FIX_ZO_DEF            (0xCC)

#define ARN5_SONET_MAX_TRACE_LEN         (64)

/***************************************************************
 * Section/Path trace options. In save mode , the trace message
 * is stored in the trace buffer and interrupt is generated.
 * In check mode , incoming trace message is checked against
 * the message stored in trace buffer.
 ***************************************************************/
typedef enum {
  ARN5_FR_MODE_CHECK =   0,
  ARN5_FR_MODE_SAVE  =   1
} arn5_sonet_trace_option_t ;

/***************************************************************
 * Sonet Overhead bytes sources for TX Stream
 ***************************************************************/
typedef enum {
  ARN5_FR_OVRHD_SRC_CPU      = 0,
  ARN5_FR_OVRHD_SRC_TOH_PORT,
  ARN5_FR_OVRHD_SRC_DCC_PORT,
  ARN5_FR_OVRHD_SRC_HDLC_PORT,
  ARN5_FR_OVRHD_SRC_RX_ALARM
} arn5_framer_tx_ovrhd_src_t ;

#define ARN5_FR_MAX_TX_OVRHD_SRC         (ARN5_FR_OVRHD_SRC_RX_ALARM + 1)

/***************************************************************
 * Sonet alarm sources for TX Stream
 ***************************************************************/
typedef enum {
  ARN5_FR_ALM_SRC_CPU      = 0,
  ARN5_FR_ALM_SRC_TOH,
  ARN5_FR_ALM_SRC_APS_PORT,
  ARN5_FR_ALM_SRC_RX_ALARM,
  ARN5_FR_MAX_ALM_SRC,
} arn5_framer_alm_src_t ;

/***************************************************************
 * TX Section Trace Message source options
 ***************************************************************/
typedef enum {
  ARN5_FR_TX_TRACE_BUFFER    = 0,/* Trace Buffer         */
  ARN5_FR_TX_TRACE_TOH,          /* from TOH             */
  ARN5_FR_TX_TRACE_FIXED         /* fix pattern (0x01)   */
} arn5_framer_tx_trace_src_t ;

/***************************************************************
 * Framer DCC PORT MODE
 ***************************************************************/
typedef enum {
  ARN5_FR_SECTION_DCC  = ARN5_DCCmode_SEC_DCC,   /* Send Section DCC channels   */
  ARN5_FR_LINE_DCC     = ARN5_DCCmode_LIN_DCC,   /* Send Line DCC channels      */
  ARN5_FR_DCC_SEC_LINE = ARN5_DCCmode_SEC_LIN_DCC  /* Section and Line DCC      */
} arn5_framer_dcc_port_mode_t ;


/***************************************************************
 * Framer Signal Degrade threshold
 ***************************************************************/
typedef enum {
  ARN5_FR_BER_3      = 3,
  ARN5_FR_BER_4,
  ARN5_FR_BER_5,
  ARN5_FR_BER_6,
  ARN5_FR_BER_7,
  ARN5_FR_BER_8,
  ARN5_FR_BER_9,
  ARN5_FR_BER_10,
} arn5_framer_signal_ber_t ;

/***************************************************************
 * Framer Data Scramber selection
 ***************************************************************/
typedef enum {
  ARN5_FR_SCRM_X43 = 0,
  ARN5_FR_SCRM_X47,
} arn5_framer_data_scrambler_sel_t ;

/***************************************************************
 * Framer GFP Mode
 ***************************************************************/
typedef enum {
  ARN5_FR_GFP_TRANSPARENT = 0,
  ARN5_FR_GFP_NORMAL
} arn5_framer_gfp_mode_t ;

typedef enum {
  ARN5_GFP_SYNC_STATUS = 0,  /* GFP SYNC STATUS                  */
  ARN5_GFP_LOSS_CLIENT,      /* loss of client frame is detected */
  ARN5_GFP_LOSS_SYNC         /* loss of sync frame is detected   */
} arn5_framer_gfp_rx_status_t ;

/***************************************************************
 * Default config structure for POS Mode
 ***************************************************************/
typedef struct {
  cs_boolean            crc32 ;        /* CRC16 or CRC32      */
  cs_boolean            pktFCS ;       /* FCS needed          */
  cs_boolean            passFCS ;      /* Pass FCS to HOST    */
  cs_uint32             max_pktSize ;  /* Max packet Size     */
  cs_uint32             min_pktSize ;  /* Min packet Size     */
} arn5_framer_pos_mode_cfg_t ;

/****************************************************************
 * Default config structure for ATM Mode
 ****************************************************************/
typedef struct {
  cs_boolean            check_hec ;    /* HEC Checking needed */
  cs_boolean            drop_idle ;    /* Drop IDLE Cells     */
} arn5_framer_atm_mode_cfg_t ;

/*****************************************************************
 * Default config structure for GFP Mode
 ****************************************************************/
typedef struct {
  arn5_framer_gfp_mode_t  mode ;       /* GFP Mode            */
  cs_boolean            cut_through ;  /* CUT-THROUGH MODE    */
  cs_uint32             max_pktSize ;  /* Max packet Size     */
  cs_uint8              min_pktSize ;  /* Min packet Size     */
} arn5_framer_gfp_mode_cfg_t ;


/*****************************************************************
 * signal threshold data structure
 *****************************************************************/
typedef struct {
  arn5_framer_signal_ber_t      declare ;
  arn5_framer_signal_ber_t      clear ;
} arn5_framer_signal_thresholds_t ;


/*****************************************************************
 * config structure for Framer.
 *****************************************************************/
typedef struct {
  cs_boolean            sonet_mode ;   /* SONET or SDH mode    */
  arn5_framer_signal_thresholds_t   line_sd ;
  arn5_framer_signal_thresholds_t   line_sf ;
  arn5_framer_signal_thresholds_t   path_sf ;


  arn5_framer_svc_type_t  svc ;  /* Service Type(POS, ATM or GFP)*/
  CS_UNION {
    arn5_framer_pos_mode_cfg_t   pos_cfg ;      /* POS configuration */
    arn5_framer_gfp_mode_cfg_t   gfp_cfg ;      /* gfp configuration */
    arn5_framer_atm_mode_cfg_t   atm_cfg ;      /* ATM configuration */
  } mode ;
  cs_boolean            line_loopback ;
  cs_boolean            payload_scrambling ;    /* Scrambling needed */
  cs_boolean            sonet_scrambling ;
  cs_boolean            enbl_dcc_prt;           /* DCC Port Enabled  */
  arn5_framer_dcc_port_mode_t    dcc_cfg ;      /* DCC port mode     */
  cs_uint16             irq_ctl_tbl[64] ;       /* IRQ Table         */
} arn5_framer_cfg_t ;

/*****************************************************************
 * running config structure for Framer.
 *****************************************************************/
typedef struct {
  cs_boolean            sonet_mode ;   /* SONET or SDH mode   */
  union {
      struct pos_cfg {
        cs_boolean      crc32 ;        /* CRC16 or CRC32      */
        cs_boolean      pktFCS ;       /* FCS needed          */
        cs_boolean      pass_fcs ;     /* FCS Passed to HOST  */
        cs_boolean      payload_scrambling ;  /* Scambling enabled */
      } pos_cfg ;  /* POS configuration */

      struct gfp_cfg {
        arn5_framer_gfp_mode_t  mode ;          /* GFP Mode          */
        cs_boolean      cut_through ;           /* CUT-THROUGH MODE  */
        cs_boolean      payload_scrambling ;    /* Scambling enabled */
      } gfp_cfg ;  /* gfp configuration */

  } mode ;
  cs_boolean            line_loopback ;
} arn5_framer_runcfg_t ;


/*****************************************************************
 * Framer APIs
 *****************************************************************/
extern
cs_status  arn5_framer_sel_sonet_sdh(cs_uint16 port_id,
                        cs_boolean sel_sonet) ;
extern
cs_status  arn5_framer_ctl_scrambling(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_ctl_short_frame(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_set_spe_service_type(cs_uint16 port_id,
                        arn5_framer_svc_type_t svc) ;
extern
cs_status  arn5_framer_set_tx_fifo_high_mark(cs_uint16 port_id,
                        cs_boolean atm, cs_uint8  threshold) ;
extern
cs_status  arn5_framer_pkt_cfg_max_min_size(cs_uint16 port_id,
                        cs_uint32 maxPktSize,
                        cs_uint8 minPktSize, cs_dir_t dir) ;
extern
cs_status  arn5_framer_ctl_line_loopback(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_sreset_sequence(cs_uint16 port_id,
                        cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_ctl_domain_sreset(cs_uint16 port_id,
                        arn5_framer_domain_t domain, cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_ctl_overhead_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_ctl_sonet_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_ctl_mapper_stream_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_status  arn5_framer_ctl_dcom_stream_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl) ;
extern
cs_boolean  arn5_framer_get_sync_status(cs_uint16 port_id) ;
extern
cs_boolean arn5_framer_get_rx_data_path_status(cs_uint16 port_id) ;
extern
arn5_port_prot_t  arn5_framer_get_svc_type(cs_uint16 port_id) ;
extern
cs_boolean  arn5_framer_sel_payload_scrambler(cs_uint16 port_id,
                        arn5_framer_data_scrambler_sel_t sel) ;

#endif /* __ARN5_FRAMER_H__ */
