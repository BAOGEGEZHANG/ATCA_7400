/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for RINGMAC Statistics.
 */
#ifndef __M2R_RMAC_STATS_H__
#define __M2R_RMAC_STATS_H__


/*********************************************************************
 *                 UNIT STATS ENUM's (PER SECTION)                   *
 *********************************************************************/

/********************************
 * RINGMAC-HOST (3 sections)    *
 ********************************/
/*
 * HOST-General statistics
 */
typedef enum {		               /*     Tx,             Rx      */
  M2R_RMAC_STATS_HOST_SHORT_PKTS = 0,  /* TXH_SHRT_PKT,      ------   */
  M2R_RMAC_STATS_HOST_RXH_OVFLW,       /* ------,       RXH_OVERFLOW1 */
  M2R_RMAC_STATS_HOST_DPERR,	       /* TXH_DPERR,         ------   */
  M2R_RMAC_STATS_HOST_SRP_PERR,        /* TXH_SRP_PERR,      ------   */
  M2R_RMAC_STATS_HOST_CTRL_PERR,       /* TXH_CTRL_PERR,     ------   */
  M2R_RMAC_STATS_HOST_ERR,             /* TXH_ERR,           ------   */
  M2R_RMAC_STATS_HOST_PKT_ERR,	       /* TXH_PKT_ERR,       ------   */
  M2R_RMAC_STATS_HOST_STSIG_ERR,       /* ------,      RXH_STSIG_ERR  */
  M2R_RMAC_STATS_HOST_GIANT,           /* ------,          RXH_GIANT  */
  M2R_RMAC_STATS_HOST_RUNT,            /* TXH_RUNT,          -----    */
  M2R_RMAC_STATS_HOST_PROT_ERR,	       /* TXH_PROT_CNT,      -----    */
  /* all */
  M2R_RMAC_STATS_HOST_GEN_SECT_ALL
} m2r_rmac_stats_host_gen_enum_t ;

/*
 * HOST-Non-wrap Performance Monitors statistics
 */
typedef enum {		               /*     Tx,             Rx      */
  /* Control packets */
  M2R_RMAC_STATS_HOST_CTL_UCAST_PKTS,  /* TXH_CTRL_UNI_PKT1, RXH_CTRL_UNI_PKT1 */
  M2R_RMAC_STATS_HOST_CTL_UCAST_BYTES, /* TXH_CTRL_UNI_BYTE2, RXH_CTRL_UNI_BYTE2 */
  M2R_RMAC_STATS_HOST_CTL_MCAST_PKTS,  /* TXH_CTRL_MULTI_PKT1, RXH_CTRL_MULTI_PKT1 */
  M2R_RMAC_STATS_HOST_CTL_MCAST_BYTES, /* TXH_CTRL_UNI_BYTE2, RXH_CTRL_UNI_BYTE2 */
  /* A0 packets */
  M2R_RMAC_STATS_HOST_A0_UCAST_PKTS,  /* TXH_HI_RES_UNI_PKT1,RXH_HI_RES_UNI_PKT1 */
  M2R_RMAC_STATS_HOST_A0_UCAST_BYTES, /* TXH_HI_RES_UNI_BYTE2,RXH_HI_RES_UNI_BYTE2 */
  M2R_RMAC_STATS_HOST_A0_MCAST_PKTS,  /* TXH_HI_RES_MULTI_PKT1,RXH_HI_RES_MULTI_PKT1 */
  M2R_RMAC_STATS_HOST_A0_MCAST_BYTES, /* TXH_HI_RES_MULTI_BYTE2, RXH_HI_RES_MULTI_BYTE2*/
  /* A1 packets */
  M2R_RMAC_STATS_HOST_A1_UCAST_PKTS,  /* TXH_HI_UNI_PKT1,RXH_HI_UNI_PKT1 */
  M2R_RMAC_STATS_HOST_A1_UCAST_BYTES, /* TXH_HI_UNI_BYTE2,RXH_HI_UNI_BYTE2 */
  M2R_RMAC_STATS_HOST_A1_MCAST_PKTS,  /* TXH_HI_MULTI_PKT1,RXH_HI_MULTI_PKT1 */
  M2R_RMAC_STATS_HOST_A1_MCAST_BYTES, /* TXH_HI_MULTI_BYTE2, RXH_HI_MULTI_BYTE2*/
  /* B0 packets(CIR) */
  M2R_RMAC_STATS_HOST_B0_UCAST_PKTS,/*TXH_MED_CIR_UNI_PKT1,RXH_MED_CIR_UNI_PKT1 */
  M2R_RMAC_STATS_HOST_B0_UCAST_BYTES,/*TXH_MED_CIR_UNI_BYTE2,RXH_MED_CIR_UNI_BYTE2*/
  M2R_RMAC_STATS_HOST_B0_MCAST_PKTS,/*TXH_MED_CIR_MULTI_PKT1,RXH_MED_CIR_MULTI_PKT1 */
  M2R_RMAC_STATS_HOST_B0_MCAST_BYTES,/*TXH_MED_CIR_MULTI_BYTE2,RXH_MED_CIR_MULTI_BYTE2*/
  /* B1 packets(EIR) */
  M2R_RMAC_STATS_HOST_B1_UCAST_PKTS,/*TXH_MED_EIR_UNI_PKT1,RXH_MED_EIR_UNI_PKT1 */
  M2R_RMAC_STATS_HOST_B1_UCAST_BYTES,/*TXH_MED_EIR_UNI_BYTE2,RXH_MED_EIR_UNI_BYTE2*/
  M2R_RMAC_STATS_HOST_B1_MCAST_PKTS,/*TXH_MED_EIR_MULTI_PKT1,RXH_MED_EIR_MULTI_PKT1 */
  M2R_RMAC_STATS_HOST_B1_MCAST_BYTES,/*TXH_MED_EIR_MULTI_BYTE2,RXH_MED_EIR_MULTI_BYTE2*/
  /* Low(class C) priority packets */
  M2R_RMAC_STATS_HOST_UCAST_PKTS,   /* TXH_UNI_PKT1,   RXH_UNI_PKT1 */
  M2R_RMAC_STATS_HOST_UCAST_BYTES,  /* TXH_UNI_BYTE2,  RXH_UNI_BYTE2 */
  M2R_RMAC_STATS_HOST_MCAST_PKTS,   /* TXH_MULTI_PKT1, RXH_MULTI_PKT1 */
  M2R_RMAC_STATS_HOST_MCAST_BYTES,  /* TXH_MULTI_BYTE2,RXH_MULTI_BYTE2 */
  /* Host Fairness packets */
  M2R_RMAC_STATS_HOST_FAIR_UCAST_PKTS, /* TXH_FAIR_UNI_PKT1, RXH_FAIR_UNI_PKT1 */
  M2R_RMAC_STATS_HOST_FAIR_UCAST_BYTES,/* TXH_FAIR_UNI_BYTE1,RXH_FAIR_UNI_BYTE1*/
  M2R_RMAC_STATS_HOST_FAIR_MCAST_PKTS, /* TXH_FAIR_MULTI_PKT1, RXH_FAIR_MULTI_PKT1 */
  M2R_RMAC_STATS_HOST_FAIR_MCAST_BYTES,/* TXH_FAIR_MULTI_BYTE1,RXH_FAIR_MULTI_BYTE1*/
  /* all */
  M2R_RMAC_STATS_HOST_PM_SECT_ALL
} m2r_rmac_stats_host_pm_enum_t ;


/*
 * HOST-WRAP Performance Monitors statistics
 */
typedef enum {		               /*     Tx,             Rx      */
  /* Control packets */
  M2R_RMAC_STATS_HOST_CTL_WUCAST_PKTS,  /* TXH_CTRL_WUNI_PKT1,  ----- */
  M2R_RMAC_STATS_HOST_CTL_WUCAST_BYTES, /* TXH_CTRL_WUNI_BYTE2, ----- */
  M2R_RMAC_STATS_HOST_CTL_WMCAST_PKTS,  /* TXH_CTRL_WMULTI_PKT1,  ----- */
  M2R_RMAC_STATS_HOST_CTL_WMCAST_BYTES, /* TXH_CTRL_WMULTI_BYTE2, ----- */
  /* A0 packets */
  M2R_RMAC_STATS_HOST_A0_WUCAST_PKTS, /* TXH_HI_RES_WUNI_PKT1, ----   */
  M2R_RMAC_STATS_HOST_A0_WUCAST_BYTES,/* TXH_HI_RES_WUNI_BYTE2, ----  */
  M2R_RMAC_STATS_HOST_A0_WMCAST_PKTS, /* TXH_HI_RES_WMULTI_PKT1, ----   */
  M2R_RMAC_STATS_HOST_A0_WMCAST_BYTES,/* TXH_HI_RES_WMULTI_BYTE2, ----  */
  /* A1 packets */
  M2R_RMAC_STATS_HOST_A1_WUCAST_PKTS,  /* TXH_HI_WUNI_PKT1, ----- */
  M2R_RMAC_STATS_HOST_A1_WUCAST_BYTES, /* TXH_HI_WUNI_BYTE2, ----- */
  M2R_RMAC_STATS_HOST_A1_WMCAST_PKTS,  /* TXH_HI_WMULTI_PKT1, ----- */
  M2R_RMAC_STATS_HOST_A1_WMCAST_BYTES, /* TXH_HI_WMULTI_BYTE2, ---- */
  /* B0 packets */
  M2R_RMAC_STATS_HOST_B0_WUCAST_PKTS, /* TXH_MED_CIR_WUNI_PKT1, ----  */
  M2R_RMAC_STATS_HOST_B0_WUCAST_BYTES,/* TXH_MED_CIR_WUNI_BYTE2, ---- */
  M2R_RMAC_STATS_HOST_B0_WMCAST_PKTS, /* TXH_MED_CIR_WMULTI_PKT1, ----  */
  M2R_RMAC_STATS_HOST_B0_WMCAST_BYTES,/* TXH_MED_CIR_WMULTI_BYTE2, ---- */
  /* B1 packets */
  M2R_RMAC_STATS_HOST_B1_WUCAST_PKTS, /* TXH_MED_EIR_WUNI_PKT1, ----  */
  M2R_RMAC_STATS_HOST_B1_WUCAST_BYTES,/* TXH_MED_EIR_WUNI_BYTE2, ---- */
  M2R_RMAC_STATS_HOST_B1_WMCAST_PKTS, /* TXH_MED_EIR_WMULTI_PKT1, ----  */
  M2R_RMAC_STATS_HOST_B1_WMCAST_BYTES,/* TXH_MED_EIR_WMULTI_BYTE2, ---- */
  /* Low(class C) priority packets */
  M2R_RMAC_STATS_HOST_WUCAST_PKTS,  /* TXH_WUNI_PKT1,   ---- */
  M2R_RMAC_STATS_HOST_WUCAST_BYTES, /* TXH_WUNI_BYTE2,  ---- */
  M2R_RMAC_STATS_HOST_WMCAST_PKTS,  /* TXH_WMULTI_PKT1, ---- */
  M2R_RMAC_STATS_HOST_WMCAST_BYTES, /* TXH_WMULTI_BYTE2, ---- */
  /* Host fairness packets */
  M2R_RMAC_STATS_HOST_FAIR_WUCAST_PKTS, /* TXH_FAIR_WUNI_PKT1, -----  */
  M2R_RMAC_STATS_HOST_FAIR_WUCAST_BYTES,/* TXH_FAIR_WUNI_BYTE1, ----- */
  M2R_RMAC_STATS_HOST_FAIR_WMCAST_PKTS, /* TXH_FAIR_WMULTI_PKT1, -----  */
  M2R_RMAC_STATS_HOST_FAIR_WMCAST_BYTES,/* TXH_FAIR_WMULTI_BYTE1, ----- */

  /* all */
  M2R_RMAC_STATS_HOST_WRAP_SECT_ALL
} m2r_rmac_stats_host_wrap_enum_t ;


/*
 * FRAMER unit statistics
 */
typedef enum {
  M2R_RMAC_STATS_FRMR_SHORT_PKT,   /* ----, RX_SHRT_PKT   */
  M2R_RMAC_STATS_FRMR_CRC_STOMP,   /* ----, RX_CRC_STOMP1 */
  M2R_RMAC_STATS_FRMR_CRC_ERR,	   /* ----, RX_CRC_ERR1   */
  M2R_RMAC_STATS_FRMR_DPERR,	   /* ----, RXF_DPERR     */
  M2R_RMAC_STATS_FRMR_SRP_PERR,    /* ----, RXF_SRP_PERR  */
  M2R_RMAC_STATS_FRMR_CTRL_PERR,   /* ----, RXF_CTRL_PERR */
  M2R_RMAC_STATS_FRMR_ERR,         /* ----, RXF_ERR       */
  M2R_RMAC_STATS_FRMR_STSIG_ERR,   /* TXF_STSIG_ERR, ---- */
  M2R_RMAC_STATS_FRMR_RUNT,        /* ----, RXF_RUNT      */
  M2R_RMAC_STATS_FRMR_GIANT,       /* ----, RXF_GPKT_ERR  */
  M2R_RMAC_STATS_FRMR_PROTO_ERR,   /* ----, RXF_PROTPKT   */
  M2R_RMAC_STATS_FRMR_MCAST_SA,	   /* ----, RXF_MCASTSA_PKT1 */
  M2R_RMAC_STATS_FRMR_DELETED_PKTS,/* ----, RXF_DELETED_PKT1 */
  M2R_RMAC_STATS_FRMR_DELETED_BYTES,/* ----, RXF_DELETED_BYTE2 */
  M2R_RMAC_STATS_FRMR_BAD_PKTS,    /* TXF_BAD_PKT1, RXF_BAD_PKT1  */
  M2R_RMAC_STATS_FRMR_BAD_BYTES,   /* TXF_BAD_BYTE2, RXF_BAD_BYTE2 */
  M2R_RMAC_STATS_RPR_RX_HEC_ERR,   /* ----, RX_HEC_ERR1 */
  M2R_RMAC_STATS_RPR_FRM_HEC_ERR,  /* ----, FRM_HEC_ERR */

  /* all */
  M2R_RMAC_STATS_FRMR_GEN_SECT_ALL
} m2r_rmac_stats_frmr_gen_enum_t ;

typedef enum {
  /* Control packets */
  M2R_RMAC_STATS_FRMR_CTL_UCAST_PKTS,  /* TXF_CTRL_UNI_PKT1, RXF_CTRL_UNI_PKT1 */
  M2R_RMAC_STATS_FRMR_CTL_UCAST_BYTES, /* TXF_CTRL_UNI_BYTE2, RXF_CTRL_UNI_BYTE2 */
  M2R_RMAC_STATS_FRMR_CTL_MCAST_PKTS,  /* TXF_CTRL_MULTI_PKT1, RXF_CTRL_MULTI_PKT1 */
  M2R_RMAC_STATS_FRMR_CTL_MCAST_BYTES, /* TXF_CTRL_UNI_BYTE2, RXF_CTRL_UNI_BYTE2 */
  /* A0 packets */
  M2R_RMAC_STATS_FRMR_A0_UCAST_PKTS,  /* TXF_HI_RES_UNI_PKT1,RXF_HI_RES_UNI_PKT1 */
  M2R_RMAC_STATS_FRMR_A0_UCAST_BYTES, /* TXF_HI_RES_UNI_BYTE2,RXF_HI_RES_UNI_BYTE2 */
  M2R_RMAC_STATS_FRMR_A0_MCAST_PKTS,  /* TXF_HI_RES_MULTI_PKT1,RXF_HI_RES_MULTI_PKT1 */
  M2R_RMAC_STATS_FRMR_A0_MCAST_BYTES, /* TXF_HI_RES_MULTI_BYTE2, RXF_HI_RES_MULTI_BYTE2*/
  /* A1 packets */
  M2R_RMAC_STATS_FRMR_A1_UCAST_PKTS,  /* TXF_HI_UNI_PKT1,RXF_HI_UNI_PKT1 */
  M2R_RMAC_STATS_FRMR_A1_UCAST_BYTES, /* TXF_HI_UNI_BYTE2,RXF_HI_UNI_BYTE2 */
  M2R_RMAC_STATS_FRMR_A1_MCAST_PKTS,  /* TXF_HI_MULTI_PKT1,RXF_HI_MULTI_PKT1 */
  M2R_RMAC_STATS_FRMR_A1_MCAST_BYTES, /* TXF_HI_MULTI_BYTE2, RXF_HI_MULTI_BYTE2*/
  /* B0 packets(CIR) */
  M2R_RMAC_STATS_FRMR_B0_UCAST_PKTS,/*TXF_MED_CIR_UNI_PKT1,RXF_MED_CIR_UNI_PKT1 */
  M2R_RMAC_STATS_FRMR_B0_UCAST_BYTES,/*TXF_MED_CIR_UNI_BYTE2,RXF_MED_CIR_UNI_BYTE2*/
  M2R_RMAC_STATS_FRMR_B0_MCAST_PKTS,/*TXF_MED_CIR_MULTI_PKT1,RXF_MED_CIR_MULTI_PKT1 */
  M2R_RMAC_STATS_FRMR_B0_MCAST_BYTES,/*TXF_MED_CIR_MULTI_BYTE2,RXF_MED_CIR_MULTI_BYTE2*/
  /* B1 packets(EIR) */
  M2R_RMAC_STATS_FRMR_B1_UCAST_PKTS,/*TXF_MED_EIR_UNI_PKT1,RXF_MED_EIR_UNI_PKT1 */
  M2R_RMAC_STATS_FRMR_B1_UCAST_BYTES,/*TXF_MED_EIR_UNI_BYTE2,RXF_MED_EIR_UNI_BYTE2*/
  M2R_RMAC_STATS_FRMR_B1_MCAST_PKTS,/*TXF_MED_EIR_MULTI_PKT1,RXF_MED_EIR_MULTI_PKT1 */
  M2R_RMAC_STATS_FRMR_B1_MCAST_BYTES,/*TXF_MED_EIR_MULTI_BYTE2,RXF_MED_EIR_MULTI_BYTE2*/
  /* Low(class C) priority packets */
  M2R_RMAC_STATS_FRMR_UCAST_PKTS,   /* TXF_UNI_PKT1,   RXF_UNI_PKT1 */
  M2R_RMAC_STATS_FRMR_UCAST_BYTES,  /* TXF_UNI_BYTE2,  RXF_UNI_BYTE2 */
  M2R_RMAC_STATS_FRMR_MCAST_PKTS,   /* TXF_MULTI_PKT1, RXF_MULTI_PKT1 */
  M2R_RMAC_STATS_FRMR_MCAST_BYTES,  /* TXF_MULTI_BYTE2,RXF_MULTI_BYTE2 */
  /* Fairness packets */
  M2R_RMAC_STATS_FRMR_FAIR_UCAST_PKTS, /* TXF_FAIR_UNI_PKT1, RXF_FAIR_UNI_PKT1 */
  M2R_RMAC_STATS_FRMR_FAIR_UCAST_BYTES,/* TXF_FAIR_UNI_BYTE1,RXF_FAIR_UNI_BYTE1*/
  M2R_RMAC_STATS_FRMR_FAIR_MCAST_PKTS, /* TXF_FAIR_MULTI_PKT1, RXF_FAIR_MULTI_PKT1 */
  M2R_RMAC_STATS_FRMR_FAIR_MCAST_BYTES,/* TXF_FAIR_MULTI_BYTE1,RXF_FAIR_MULTI_BYTE1*/
  /* Flooded packets */
  M2R_RMAC_STATS_FRMR_FLOOD_PKTS,  /* TXF_FLOOD_PKT1,  RXF_FLOOD_PKT1 */
  M2R_RMAC_STATS_FRMR_FLOOD_BYTES, /* TXF_FLOOD_BYTE2, RXF_FLOOD_BYTE2 */
  /* Non-flooded packets */
  M2R_RMAC_STATS_FRMR_NOFLOOD_PKTS,  /* TXF_NOFLOOD_PKT1,  RXF_NOFLOOD_PKT1 */
  M2R_RMAC_STATS_FRMR_NOFLOOD_BYTES, /* TXF_NOFLOOD_BYTE2, RXF_NOFLOOD_BYTE2 */
  /* Idle packets */
  M2R_RMAC_STATS_FRMR_IDLE_PKTS,  /* TXF_IDLE_PKT1,  RXF_IDLE_PKT1 */
  M2R_RMAC_STATS_FRMR_IDLE_BYTES, /* TXF_IDLE_BYTE2, RXF_IDLE_BYTE2 */

  /* all */
  M2R_RMAC_STATS_FRMR_PM_SECT_ALL
} m2r_rmac_stats_frmr_pm_enum_t ;

typedef enum {
  M2R_RMAC_STATS_MATE_RUNT,          /* ----, FRM_RUNT           */
  M2R_RMAC_STATS_MATE_GIANT,         /* ----, FRM_GIANT          */
  M2R_RMAC_STATS_MATE_OVERFLOW,      /* ----, FRM_OVERFLOW       */
  M2R_RMAC_STATS_MATE_UNDERFLOW,     /* ----, FRM_UNDERFLOW      */
  M2R_RMAC_STATS_MATE_CRC_ERR,       /* ----, FRM_CRC_ERR        */
  M2R_RMAC_STATS_MATE_CTRL_SQNC_ERR, /* ----, FRM_CTRL_SQNC_ERR  */

  /* all */
  M2R_RMAC_STATS_MATE_GEN_SECT_ALL
} m2r_rmac_stats_mate_gen_enum_t ;

typedef enum {
  M2R_RMAC_STATS_MATE_WE_PKTS,       /* TOM_WE_PKT1, ----     */
  M2R_RMAC_STATS_MATE_WE_BYTES,      /* TOM_WE_BYTE2, ----     */
  M2R_RMAC_STATS_MATE_SETWS_PKTS,    /* TOM_SETWS_PKT1, ----     */
  M2R_RMAC_STATS_MATE_FT_PKTS,       /* TOM_FT_PKT1, ----     */
  M2R_RMAC_STATS_MATE_WS_PKTS,       /* TOM_WS_PKT1, ----     */
  M2R_RMAC_STATS_MATE_WS_BYTES,      /* TOM_WS_BYTE2, ----     */

  /* all */
  M2R_RMAC_STATS_MATE_PM_SECT_ALL
} m2r_rmac_stats_mate_pm_enum_t ;

typedef enum {
  M2R_RMAC_STATS_SRPF_TB_HI_PKTS,       /* TB_HI_PKT1, ----     */
  M2R_RMAC_STATS_SRPF_TB_HI_BYTES,      /* TB_HI_BYTE2, ----     */
  M2R_RMAC_STATS_SRPF_TB_LO_PKTS,       /* TB_LO_PKT1, ----     */
  M2R_RMAC_STATS_SRPF_TB_LO_BYTES,      /* TB_LO_BYTE2, ----     */
  M2R_RMAC_STATS_SRPF_TB_UNI_PKTS,      /* TB_UNI_PKT1, ----     */
  M2R_RMAC_STATS_SRPF_TB_UNI_BYTES,     /* TB_UNI_BYTE2, ----     */
  M2R_RMAC_STATS_SRPF_TB_MULTI_PKTS,    /* TB_MULTI_PKT1, ----     */
  M2R_RMAC_STATS_SRPF_TB_MULTI_BYTES,   /* TB_MULTI_BYTE2, ----     */

  /* all */
  M2R_RMAC_STATS_SRPF_PM_SECT_ALL
} m2r_rmac_stats_srpf_pm_enum_t ;

typedef enum {
  M2R_RMAC_STATS_TB_HI_ECCFIX,       /* ----, HTB_DPERR : ECCFIX : spl */
  M2R_RMAC_STATS_TB_HI_ECCERR,       /* ----, HTB_DPERR : ECCERR : spl */
  M2R_RMAC_STATS_TB_LO_ECCFIX,       /* ----, LTB_DPERR : ECCFIX : spl */
  M2R_RMAC_STATS_TB_LO_ECCERR,       /* ----, LTB_DPERR : ECCERR : spl */
  M2R_RMAC_STATS_TB_HI_DROP_PKT,     /* ----, HTB_DROP_PKT  */
  M2R_RMAC_STATS_TB_LO_DROP_PKT,     /* ----, LTB_DROP_PKT  */

  /* all */
  M2R_RMAC_STATS_TB_GEN_SECT_ALL
} m2r_rmac_stats_tb_gen_enum_t ;

typedef enum {
  M2R_RMAC_STATS_TB_HI_DEL_ACC,     /* HI_TB_DEL_ACC2, ---- */
  M2R_RMAC_STATS_TB_HI_DEL_CNT,     /* HI_TB_DEL_CNT1, ---- */
  M2R_RMAC_STATS_TB_HI_DEL_MIN,     /* HI_TB_DEL_MIN1, ---- */
  M2R_RMAC_STATS_TB_HI_DEL_MAX,     /* HI_TB_DEL_MAX1, ---- */

  M2R_RMAC_STATS_TB_HI_DEL_AVG,     /* SOFTWARE, ---- */

  M2R_RMAC_STATS_TB_LO_DEL_ACC,     /* LO_TB_DEL_ACC2, ---- */
  M2R_RMAC_STATS_TB_LO_DEL_CNT,     /* LO_TB_DEL_CNT1, ---- */
  M2R_RMAC_STATS_TB_LO_DEL_MIN,     /* LO_TB_DEL_MIN1, ---- */
  M2R_RMAC_STATS_TB_LO_DEL_MAX,     /* LO_TB_DEL_MAX1, ---- */

  M2R_RMAC_STATS_TB_LO_DEL_AVG,     /* SOFTWARE, ---- */

  M2R_RMAC_STATS_TB_HI_KILL_PKTS,    /* TBH_KILL_PC1, TINH_KILL_PC1 */
  M2R_RMAC_STATS_TB_HI_KILL_BYTES,   /* TBH_KILL_BC2, TINH_KILL_BC2 */
  M2R_RMAC_STATS_TB_LO_KILL_PKTS,    /* TBL_KILL_PC1, TINL_KILL_PC1 */
  M2R_RMAC_STATS_TB_LO_KILL_BYTES,   /* TBL_KILL_BC2, TINL_KILL_BC2 */

  /* all */
  M2R_RMAC_STATS_TB_PM_SECT_ALL
} m2r_rmac_stats_tb_pm_enum_t ;

typedef enum {
  M2R_RMAC_STATS_MRW_SADA_PKT,         /* MAC_SADA_PKT1, ---- */
  M2R_RMAC_STATS_MRW_REWRITE_PKT,      /* MAC_REWRITE_PKT1, ---- */
  M2R_RMAC_STATS_MRW_FLUSH_HOST_PKT,   /* FLUSH_HOST_PKT1, ---- */
  M2R_RMAC_STATS_MRW_FLUSH_HOST_BYTE,  /* FLUSH_HOST_BYTE2, ---- */
  M2R_RMAC_STATS_MRW_STRICTFLUSH_PKT,  /* STRICTFLUSH_PKT1, ---- */
  M2R_RMAC_STATS_MRW_STRICTFLUSH_BYTE, /* STRICTFLUSH_BYTE2, ---- */
  M2R_RMAC_STATS_MRW_MACDROP_PKT,      /* MACDROP_PKT1, ---- */
  M2R_RMAC_STATS_MRW_MACDROP_BYTE,     /* MACDROP_BYTE2, ---- */

  /* all */
  M2R_RMAC_STATS_MRW_PM_SECT_ALL
} m2r_rmac_stats_mrw_pm_enum_t ;

typedef enum {
  M2R_RMAC_STATS_TBD_IPS_RCV_DROP_PKT,   /* ----, IPS_RCV_DROP_COUNT1 */
  M2R_RMAC_STATS_TBD_IPS_RCV_ERR_PKT,    /* ----, IPS_RCV_ERR_COUNT1 */
  M2R_RMAC_STATS_TBD_CTL_RCV_DROP_PKT,   /* ----, CTL_RCV_DROP_COUNT1 */
  M2R_RMAC_STATS_TBD_CTL_RCV_ERR_PKT,    /* ----, CTL_RCV_ERR_COUNT1 */
  M2R_RMAC_STATS_TBD_RING_ID_ERR,        /* ----, RING_ID_ERR */
  M2R_RMAC_STATS_TBD_RUNT_USG,           /* ----, TBD_RUNT_USG */
  M2R_RMAC_STATS_TBD_SA_STRIP_PKT,       /* ----, SA_STRIP_PKT1 */
  M2R_RMAC_STATS_TBD_SA_STRIP_BYTE,      /* ----, SA_STRIP_BYTE2 */
  M2R_RMAC_STATS_TBD_TTL_DROP_PKT,       /* ----, TTL_DROP_PKT1 */
  M2R_RMAC_STATS_TBD_TTL_DROP_BYTE,      /* ----, TTL_DROP_BYTE2 */
  M2R_RMAC_STATS_TBD_STRICT_STRIP_PKT,   /* ----, STRICT_STRIP_PKT1 */
  M2R_RMAC_STATS_TBD_STRICT_STRIP_BYTE,  /* ----, STRICT_STRIP_BYTE2 */

  /* all */
  M2R_RMAC_STATS_TBD_GEN_SECT_ALL
} m2r_rmac_stats_tbd_gen_enum_t ;

typedef enum {
  M2R_RMAC_STATS_MS_LTB_MIN_DEP,   /* ----, LTB_MIN_DEP1 : unit of 32 bytes */
  M2R_RMAC_STATS_MS_LTB_MAX_DEP,   /* ----, LTB_MAX_DEP1 : unit of 32 bytes */
  M2R_RMAC_STATS_MS_LTB_ACC_DEP,   /* ----, LTB_ACC_DEP2 : unit of 32 bytes */
  M2R_RMAC_STATS_MS_LTB_PKT,       /* ----, LTB_PKT_CNT1 : unit of 32 bytes */
  M2R_RMAC_STATS_MS_HTB_MIN_DEP,   /* ----, HTB_MIN_DEP1 : unit of 32 bytes */
  M2R_RMAC_STATS_MS_HTB_MAX_DEP,   /* ----, HTB_MAX_DEP1 : unit of 32 bytes */
  M2R_RMAC_STATS_MS_HTB_ACC_DEP,   /* ----, HTB_ACC_DEP2 : unit of 32 bytes */
  M2R_RMAC_STATS_MS_HTB_PKT,       /* ----, HTB_PKT_CNT1 : unit of 32 bytes */

  /* all */
  M2R_RMAC_STATS_MS_PM_SECT_ALL
} m2r_rmac_stats_ms_pm_enum_t ;

typedef enum {
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_0_CNTR,  /* MCCAM_ENTRY_0_PKT, MCCAM_ENTRY_0_BYTE  */
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_1_CNTR,  /* MCCAM_ENTRY_1_PKT, MCCAM_ENTRY_1_BYTE  */
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_2_CNTR,  /* MCCAM_ENTRY_2_PKT, MCCAM_ENTRY_2_BYTE  */
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_3_CNTR,  /* MCCAM_ENTRY_3_PKT, MCCAM_ENTRY_3_BYTE  */
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_4_CNTR,  /* MCCAM_ENTRY_4_PKT, MCCAM_ENTRY_4_BYTE  */
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_5_CNTR,  /* MCCAM_ENTRY_5_PKT, MCCAM_ENTRY_5_BYTE  */
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_6_CNTR,  /* MCCAM_ENTRY_6_PKT, MCCAM_ENTRY_6_BYTE  */
  M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_7_CNTR,  /* MCCAM_ENTRY_7_PKT, MCCAM_ENTRY_7_BYTE  */

  /* all */
  M2R_RMAC_STATS_MCOARSE_CAM_SECT_ALL
} m2r_rmac_stats_mcoarse_cam_enum_t ;

typedef enum {
  /* Channel 0 packet with cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_0_CNTR,  /* HDCAM_ENTRY_0_C_0_PKT,
                                                        HDCAM_ENTRY_0_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_0_CNTR,  /* HDCAM_ENTRY_1_C_0_PKT,
                                                        HDCAM_ENTRY_1_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_0_CNTR,  /* HDCAM_ENTRY_2_C_0_PKT,
                                                        HDCAM_ENTRY_2_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_0_CNTR,  /* HDCAM_ENTRY_3_C_0_PKT,
                                                        HDCAM_ENTRY_3_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_0_CNTR,  /* HDCAM_ENTRY_4_C_0_PKT,
                                                        HDCAM_ENTRY_4_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_0_CNTR,  /* HDCAM_ENTRY_5_C_0_PKT,
                                                        HDCAM_ENTRY_5_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_0_CNTR,  /* HDCAM_ENTRY_6_C_0_PKT,
                                                        HDCAM_ENTRY_6_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_0_CNTR,  /* HDCAM_ENTRY_7_C_0_PKT,
                                                        HDCAM_ENTRY_7_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_0_CNTR,  /* HDCAM_ENTRY_8_C_0_PKT,
                                                        HDCAM_ENTRY_8_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_0_CNTR,  /* HDCAM_ENTRY_9_C_0_PKT,
                                                        HDCAM_ENTRY_9_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_0_CNTR, /* HDCAM_ENTRY_10_C_0_PKT,
                                                        HDCAM_ENTRY_10_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_0_CNTR, /* HDCAM_ENTRY_11_C_0_PKT,
                                                        HDCAM_ENTRY_11_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_0_CNTR, /* HDCAM_ENTRY_12_C_0_PKT,
                                                        HDCAM_ENTRY_12_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_0_CNTR, /* HDCAM_ENTRY_13_C_0_PKT,
                                                        HDCAM_ENTRY_13_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_0_CNTR, /* HDCAM_ENTRY_14_C_0_PKT,
                                                        HDCAM_ENTRY_14_C_0_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_0_CNTR, /* HDCAM_ENTRY_15_C_0_PKT,
                                                        HDCAM_ENTRY_15_C_0_BYTE  */

  /* Channel 1 packet with cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_1_CNTR,  /* HDCAM_ENTRY_0_C_1_PKT,
                                                        HDCAM_ENTRY_0_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_1_CNTR,  /* HDCAM_ENTRY_1_C_1_PKT,
                                                        HDCAM_ENTRY_1_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_1_CNTR,  /* HDCAM_ENTRY_2_C_1_PKT,
                                                        HDCAM_ENTRY_2_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_1_CNTR,  /* HDCAM_ENTRY_3_C_1_PKT,
                                                        HDCAM_ENTRY_3_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_1_CNTR,  /* HDCAM_ENTRY_4_C_1_PKT,
                                                        HDCAM_ENTRY_4_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_1_CNTR,  /* HDCAM_ENTRY_5_C_1_PKT,
                                                        HDCAM_ENTRY_5_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_1_CNTR,  /* HDCAM_ENTRY_6_C_1_PKT,
                                                        HDCAM_ENTRY_6_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_1_CNTR,  /* HDCAM_ENTRY_7_C_1_PKT,
                                                        HDCAM_ENTRY_7_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_1_CNTR,  /* HDCAM_ENTRY_8_C_1_PKT,
                                                        HDCAM_ENTRY_8_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_1_CNTR,  /* HDCAM_ENTRY_9_C_1_PKT,
                                                        HDCAM_ENTRY_9_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_1_CNTR, /* HDCAM_ENTRY_10_C_1_PKT,
                                                        HDCAM_ENTRY_10_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_1_CNTR, /* HDCAM_ENTRY_11_C_1_PKT,
                                                        HDCAM_ENTRY_11_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_1_CNTR, /* HDCAM_ENTRY_12_C_1_PKT,
                                                        HDCAM_ENTRY_12_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_1_CNTR, /* HDCAM_ENTRY_13_C_1_PKT,
                                                        HDCAM_ENTRY_13_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_1_CNTR, /* HDCAM_ENTRY_14_C_1_PKT,
                                                        HDCAM_ENTRY_14_C_1_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_1_CNTR, /* HDCAM_ENTRY_15_C_1_PKT,
                                                        HDCAM_ENTRY_15_C_1_BYTE  */

  /* Channel 2 packet with cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_2_CNTR,  /* HDCAM_ENTRY_0_C_2_PKT,
                                                        HDCAM_ENTRY_0_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_2_CNTR,  /* HDCAM_ENTRY_1_C_2_PKT,
                                                        HDCAM_ENTRY_1_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_2_CNTR,  /* HDCAM_ENTRY_2_C_2_PKT,
                                                        HDCAM_ENTRY_2_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_2_CNTR,  /* HDCAM_ENTRY_3_C_2_PKT,
                                                        HDCAM_ENTRY_3_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_2_CNTR,  /* HDCAM_ENTRY_4_C_2_PKT,
                                                        HDCAM_ENTRY_4_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_2_CNTR,  /* HDCAM_ENTRY_5_C_2_PKT,
                                                        HDCAM_ENTRY_5_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_2_CNTR,  /* HDCAM_ENTRY_6_C_2_PKT,
                                                        HDCAM_ENTRY_6_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_2_CNTR,  /* HDCAM_ENTRY_7_C_2_PKT,
                                                        HDCAM_ENTRY_7_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_2_CNTR,  /* HDCAM_ENTRY_8_C_2_PKT,
                                                        HDCAM_ENTRY_8_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_2_CNTR,  /* HDCAM_ENTRY_9_C_2_PKT,
                                                        HDCAM_ENTRY_9_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_2_CNTR, /* HDCAM_ENTRY_10_C_2_PKT,
                                                        HDCAM_ENTRY_10_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_2_CNTR, /* HDCAM_ENTRY_11_C_2_PKT,
                                                        HDCAM_ENTRY_11_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_2_CNTR, /* HDCAM_ENTRY_12_C_2_PKT,
                                                        HDCAM_ENTRY_12_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_2_CNTR, /* HDCAM_ENTRY_13_C_2_PKT,
                                                        HDCAM_ENTRY_13_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_2_CNTR, /* HDCAM_ENTRY_14_C_2_PKT,
                                                        HDCAM_ENTRY_14_C_2_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_2_CNTR, /* HDCAM_ENTRY_15_C_2_PKT,
                                                        HDCAM_ENTRY_15_C_2_BYTE  */

  /* Channel 3 packet with cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_3_CNTR,  /* HDCAM_ENTRY_0_C_3_PKT,
                                                        HDCAM_ENTRY_0_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_3_CNTR,  /* HDCAM_ENTRY_1_C_3_PKT,
                                                        HDCAM_ENTRY_1_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_3_CNTR,  /* HDCAM_ENTRY_2_C_3_PKT,
                                                        HDCAM_ENTRY_2_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_3_CNTR,  /* HDCAM_ENTRY_3_C_3_PKT,
                                                        HDCAM_ENTRY_3_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_3_CNTR,  /* HDCAM_ENTRY_4_C_3_PKT,
                                                        HDCAM_ENTRY_4_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_3_CNTR,  /* HDCAM_ENTRY_5_C_3_PKT,
                                                        HDCAM_ENTRY_5_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_3_CNTR,  /* HDCAM_ENTRY_6_C_3_PKT,
                                                        HDCAM_ENTRY_6_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_3_CNTR,  /* HDCAM_ENTRY_7_C_3_PKT,
                                                        HDCAM_ENTRY_7_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_3_CNTR,  /* HDCAM_ENTRY_8_C_3_PKT,
                                                        HDCAM_ENTRY_8_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_3_CNTR,  /* HDCAM_ENTRY_9_C_3_PKT,
                                                        HDCAM_ENTRY_9_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_3_CNTR, /* HDCAM_ENTRY_10_C_3_PKT,
                                                        HDCAM_ENTRY_10_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_3_CNTR, /* HDCAM_ENTRY_11_C_3_PKT,
                                                        HDCAM_ENTRY_11_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_3_CNTR, /* HDCAM_ENTRY_12_C_3_PKT,
                                                        HDCAM_ENTRY_12_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_3_CNTR, /* HDCAM_ENTRY_13_C_3_PKT,
                                                        HDCAM_ENTRY_13_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_3_CNTR, /* HDCAM_ENTRY_14_C_3_PKT,
                                                        HDCAM_ENTRY_14_C_3_BYTE  */
  M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_3_CNTR, /* HDCAM_ENTRY_15_C_3_PKT,
                                                        HDCAM_ENTRY_15_C_3_BYTE  */

  /* Channel 0 packet with no cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_0_CNTR,   /* HDCAM_NO_HIT_C_0_PKT,
                                                        HDCAM_NO_HIT_C_0_BYTE  */
  /* Channel 1 packet with no cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_1_CNTR,   /* HDCAM_NO_HIT_C_1_PKT,
                                                        HDCAM_NO_HIT_C_1_BYTE  */
  /* Channel 2 packet with no cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_2_CNTR,   /* HDCAM_NO_HIT_C_2_PKT,
                                                        HDCAM_NO_HIT_C_2_BYTE  */
  /* Channel 3 packet with no cam hit */
  M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_3_CNTR,   /* HDCAM_NO_HIT_C_3_PKT,
                                                        HDCAM_NO_HIT_C_3_BYTE  */

  /* all */
  M2R_RMAC_STATS_HOST_DROP_CAM_SECT_ALL
} m2r_rmac_stats_hdrop_cam_enum_t ;


/*********************************************************************
 *                    SECTION STATS DATASTRUCTURES                   *
 *********************************************************************/
/*
 * RINGMAC-HOST Statistics(3 sections)
 */
typedef struct {
  m2r_stats_data_t      tbl[ M2R_RMAC_STATS_HOST_GEN_SECT_ALL ] ;
} m2r_rmac_stats_host_gen_t ;

typedef struct {
  m2r_stats_data_t      tbl[ M2R_RMAC_STATS_HOST_PM_SECT_ALL ] ;
} m2r_rmac_stats_host_pm_t ;

typedef struct {
  m2r_stats_data_t      tbl[ M2R_RMAC_STATS_HOST_WRAP_SECT_ALL ] ;
} m2r_rmac_stats_host_wrap_t ;

/*
 * RINGMAC-FRAMER Statistics(2 sections)
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_FRMR_GEN_SECT_ALL ] ;
} m2r_rmac_stats_frmr_gen_t ;

typedef struct {
  m2r_stats_data_t      tbl[ M2R_RMAC_STATS_FRMR_PM_SECT_ALL ] ;
} m2r_rmac_stats_frmr_pm_t ;


/*
 * RINGMAC-MATE Statistics
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_MATE_GEN_SECT_ALL ] ;
} m2r_rmac_stats_mate_gen_t ;

typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_MATE_PM_SECT_ALL ] ;
} m2r_rmac_stats_mate_pm_t ;

/*
 * RINGMAC-SRPF Statistics
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_SRPF_PM_SECT_ALL ] ;
} m2r_rmac_stats_srpf_pm_t ;

/*
 * RINGMAC-TB Statistics
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_TB_GEN_SECT_ALL ] ;
} m2r_rmac_stats_tb_gen_t ;

typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_TB_PM_SECT_ALL ] ;
} m2r_rmac_stats_tb_pm_t ;

/*
 * RINGMAC-MRW Statistics
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_MRW_PM_SECT_ALL ] ;
} m2r_rmac_stats_mrw_pm_t ;

/*
 * RINGMAC-TBD Statistics
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_TBD_GEN_SECT_ALL ] ;
} m2r_rmac_stats_tbd_gen_t ;

/*
 * RINGMAC-MS Statistics
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_MS_PM_SECT_ALL ] ;
} m2r_rmac_stats_ms_pm_t ;

/*
 * RINGMAC-COARSE-CAM Statistics
 */
typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_MCOARSE_CAM_SECT_ALL ] ;
} m2r_rmac_stats_mcoarse_cam_t ;

typedef struct {
  m2r_stats_data_t	tbl[ M2R_RMAC_STATS_HOST_DROP_CAM_SECT_ALL ] ;
} m2r_rmac_stats_hdrop_cam_t ;


/*********************************************************************
 *               SECTION-ID ENUM's FOR RINGMAC BLOCK                 *
 *********************************************************************/
typedef enum {
  /* RingMAC-Host (3 sections) */
  M2R_RMAC_STATS_HOST_GEN_SECT     = 0,
  M2R_RMAC_STATS_HOST_PM_SECT,
  M2R_RMAC_STATS_HOST_WRAP_SECT,
  /* RingMAC-Framer */
  M2R_RMAC_STATS_FRMR_GEN_SECT,
  M2R_RMAC_STATS_FRMR_PM_SECT,
  /* RingMAC-Mate */
  M2R_RMAC_STATS_MATE_GEN_SECT,
  M2R_RMAC_STATS_MATE_PM_SECT,
  /* RingMAC-SRPF */
  M2R_RMAC_STATS_SRPF_PM_SECT,
  /* RingMAC-TB */
  M2R_RMAC_STATS_TB_GEN_SECT,
  M2R_RMAC_STATS_TB_PM_SECT,
  /* RingMAC-MRW */
  M2R_RMAC_STATS_MRW_PM_SECT,
  /* RingMAC-TBD */
  M2R_RMAC_STATS_TBD_GEN_SECT,
  /* RingMAC-MS */
  M2R_RMAC_STATS_MS_PM_SECT,
  /* RingMAC-Coarse-CAM */
  M2R_RMAC_STATS_MCOARSE_CAM_SECT,
  M2R_RMAC_STATS_HOST_DROP_CAM_SECT,

  /*** ALL ****/
  M2R_RMAC_STATS_SECT_ALL
} m2r_rmac_stats_sect_enum_t ;


/********************************************************************
 *                  RINGMAC STATISTICS DATA-STRUCTURE               *
 ********************************************************************/
typedef struct {
  m2r_rmac_stats_host_gen_t	    host_gen ;
  m2r_rmac_stats_host_pm_t      host_pm ;
  m2r_rmac_stats_host_wrap_t    host_wrap ;
  m2r_rmac_stats_frmr_gen_t	    frmr_gen ;
  m2r_rmac_stats_frmr_pm_t	    frmr_pm ;
  m2r_rmac_stats_mate_gen_t		mate_gen ;
  m2r_rmac_stats_mate_pm_t      mate_pm ;
  m2r_rmac_stats_srpf_pm_t      srpf_pm ;
  m2r_rmac_stats_tb_gen_t       tb_gen ;
  m2r_rmac_stats_tb_pm_t        tb_pm ;
  m2r_rmac_stats_mrw_pm_t       mrw_pm ;
  m2r_rmac_stats_tbd_gen_t      tbd_gen ;
  m2r_rmac_stats_ms_pm_t        ms_pm ;
  m2r_rmac_stats_mcoarse_cam_t  mc_cam ;
  m2r_rmac_stats_hdrop_cam_t    hd_cam ;
} m2r_rmac_stats_t ;

/********************************************************************
 *                  RINGMAC STATISTICS APIs                         *
 ********************************************************************/
extern
cs_status m2r_rmac_stats_mcoarse_cam_clear_units(cs_uint16 port_id,
                                            cs_uint32 start,
                                            cs_uint32 num_entries) ;
extern
cs_status m2r_rmac_stats_host_drop_cam_clear_units(cs_uint16 port_id,
                                              cs_uint32 start,
                                              cs_uint32 num_entries) ;

#endif /* __M2R_RMAC_STATS_H__ */




