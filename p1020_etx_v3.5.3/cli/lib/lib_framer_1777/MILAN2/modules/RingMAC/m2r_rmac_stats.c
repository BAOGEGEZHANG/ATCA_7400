/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_rmac_stats.h"
#include "m2r_rmac_priv.h"


/* Forward declarations */
static
cs_status m2r_rmac_register_sect_stats(cs_uint16 port_id,
				       cs_uint16 sect_id,
				       cs_boolean sect_enbl) ;

#define M2R_RMAC_STATS_TX_ONLY   ( M2R_STATS_FLAG_TX_VALID    | \
                                   M2R_STATS_FLAG_TX_READ_CLR )

#define M2R_RMAC_STATS_RX_ONLY   ( M2R_STATS_FLAG_RX_VALID    | \
                                   M2R_STATS_FLAG_RX_READ_CLR )

#define M2R_RMAC_STATS_TX_N_RX   ( M2R_RMAC_STATS_TX_ONLY    | \
                                   M2R_RMAC_STATS_RX_ONLY )

/******* INDIVIDUAL SECTION STATS ***************/

/*************************************************
 * RINGMAC HOST(3 sections)
 *************************************************/
/*
 * HOST-General statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_host_gen_tbl [ M2R_RMAC_STATS_HOST_GEN_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_HOST_SHORT_PKTS ] =) {
    "Short Pkts",
    M2R_RMAC_REG_OFST(TXH_SHRT_PKT),    /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)             /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_RXH_OVFLW ] =) {
    "FIFO Overflow",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_OVERFLOW1),   /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DPERR ] =) {
    "Data Parity Error",
    M2R_RMAC_REG_OFST(TXH_DPERR),       /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_SRP_PERR ] =) {
    "SRP Parity Error",
    M2R_RMAC_REG_OFST(TXH_SRP_PERR),    /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_CTRL_PERR ] =) {
    "Control Parity Error",
    M2R_RMAC_REG_OFST(TXH_CTRL_PERR),   /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_ERR ] =) {
    "TXHERR pin Error",
    M2R_RMAC_REG_OFST(TXH_ERR),         /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_PKT_ERR ] =) {
    "TXH Pkt Error",
    M2R_RMAC_REG_OFST(TXH_PKT_ERR),     /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_STSIG_ERR ] =) {
    "RXH STSIG Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_STSIG_ERR),   /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_GIANT ] =) {
    "Giant Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_GIANT),       /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_RUNT ] =) {
    "Runt Pkts",
    M2R_RMAC_REG_OFST(TXH_RUNT),        /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_PROT_ERR ] =) {
    "Protocol Errored Pkts",
    M2R_RMAC_REG_OFST(TXH_PROT_CNT),    /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  }
} ;


/*
 * HOST-Non-wrap Performance Monitors statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_host_pm_tbl [ M2R_RMAC_STATS_HOST_PM_SECT_ALL ] = {
  /* Control Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_CTL_UCAST_PKTS ] =) {
    "Control ucast pkts",
    M2R_RMAC_REG_OFST(TXH_CTRL_UNI_PKT1),         /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_CTRL_UNI_PKT1),         /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX          \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_CTL_UCAST_BYTES ] =) {
    "Control ucast bytes",
    M2R_RMAC_REG_OFST(TXH_CTRL_UNI_BYTE2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_CTRL_UNI_BYTE2),        /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_CTL_MCAST_PKTS ] =) {
    "Control mcast pkts",
    M2R_RMAC_REG_OFST(TXH_CTRL_MULTI_PKT1),       /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_CTRL_MULTI_PKT1),       /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_CTL_MCAST_BYTES ] =) {
    "Control mcast bytes",
    M2R_RMAC_REG_OFST(TXH_CTRL_MULTI_BYTE2),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_CTRL_MULTI_BYTE2),      /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* A0 Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_A0_UCAST_PKTS ] =) {
    "A0 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_RES_UNI_PKT1),       /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_RES_UNI_PKT1),       /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A0_UCAST_BYTES ] =) {
    "A0 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_RES_UNI_BYTE2),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_RES_UNI_BYTE2),      /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A0_MCAST_PKTS ] =) {
    "A0 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_RES_MULTI_PKT1),     /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_RES_MULTI_PKT1),     /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A0_MCAST_BYTES ] =) {
    "A0 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_RES_MULTI_BYTE2),    /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_RES_MULTI_BYTE2),    /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* A1 Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_A1_UCAST_PKTS ] =) {
    "A1 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_UNI_PKT1),           /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_UNI_PKT1),           /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A1_UCAST_BYTES ] =) {
    "A1 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_UNI_BYTE2),          /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_UNI_BYTE2),          /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A1_MCAST_PKTS ] =) {
    "A1 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_MULTI_PKT1),         /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_MULTI_PKT1),         /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A1_MCAST_BYTES ] =) {
    "A1 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_MULTI_BYTE2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_HI_MULTI_BYTE2),        /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* B0 Packets(CIR) */
  CS_DI([ M2R_RMAC_STATS_HOST_B0_UCAST_PKTS ] =) {
    "B0 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_UNI_PKT1),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_CIR_UNI_PKT1),      /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B0_UCAST_BYTES ] =) {
    "B0 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_UNI_BYTE2),     /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_CIR_UNI_BYTE2),     /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B0_MCAST_PKTS ] =) {
    "B0 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_MULTI_PKT1),    /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_CIR_MULTI_PKT1),    /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B0_MCAST_BYTES ] =) {
    "B0 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_MULTI_BYTE2),   /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_CIR_MULTI_BYTE2),   /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* B1 Packets(EIR) */
  CS_DI([ M2R_RMAC_STATS_HOST_B1_UCAST_PKTS ] =) {
    "B1 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_UNI_PKT1),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_EIR_UNI_PKT1),      /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B1_UCAST_BYTES ] =) {
    "B1 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_UNI_BYTE2),     /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_EIR_UNI_BYTE2),     /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B1_MCAST_PKTS ] =) {
    "B1 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_MULTI_PKT1),    /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_EIR_MULTI_PKT1),    /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B1_MCAST_BYTES ] =) {
    "B1 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_MULTI_BYTE2),   /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MED_EIR_MULTI_BYTE2),   /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Class-C Packets(low priority) */
  CS_DI([ M2R_RMAC_STATS_HOST_UCAST_PKTS ] =) {
    "Class-C ucast pkts",
    M2R_RMAC_REG_OFST(TXH_UNI_PKT1),              /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_UNI_PKT1),              /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_UCAST_BYTES ] =) {
    "Class-C ucast bytes",
    M2R_RMAC_REG_OFST(TXH_UNI_BYTE2),             /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_UNI_BYTE2),             /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_MCAST_PKTS ] =) {
    "Class-C mcast pkts",
    M2R_RMAC_REG_OFST(TXH_MULTI_PKT1),            /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MULTI_PKT1),            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_MCAST_BYTES ] =) {
    "Class-C mcast bytes",
    M2R_RMAC_REG_OFST(TXH_MULTI_BYTE2),           /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_MULTI_BYTE2),           /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Host Fairness Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_UCAST_PKTS ] =) {
    "Fairness ucast pkts",
    M2R_RMAC_REG_OFST(TXH_FAIR_UNI_PKT1),         /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_FAIR_UNI_PKT1),         /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_UCAST_BYTES ] =) {
    "Fairness ucast bytes",
    M2R_RMAC_REG_OFST(TXH_FAIR_UNI_BYTE2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_FAIR_UNI_BYTE2),        /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_MCAST_PKTS ] =) {
    "Fairness mcast pkts",
    M2R_RMAC_REG_OFST(TXH_FAIR_MULTI_PKT1),       /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_FAIR_MULTI_PKT1),       /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_MCAST_BYTES ] =) {
    "Fairness mcast bytes",
    M2R_RMAC_REG_OFST(TXH_FAIR_MULTI_BYTE2),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXH_FAIR_MULTI_BYTE2),      /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  }
} ;


/*
 * HOST-WRAP Performance Monitors statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_host_wrap_tbl [ M2R_RMAC_STATS_HOST_WRAP_SECT_ALL ] = {
  /* Control Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_CTL_WUCAST_PKTS ] =) {
    "Control ucast pkts",
    M2R_RMAC_REG_OFST(TXH_CTRL_WUNI_PKT1),        /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_CTL_WUCAST_BYTES ] =) {
    "Control ucast bytes",
    M2R_RMAC_REG_OFST(TXH_CTRL_WUNI_BYTE2),       /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_CTL_WMCAST_PKTS ] =) {
    "Control mcast pkts",
    M2R_RMAC_REG_OFST(TXH_CTRL_WMULTI_PKT1),      /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_CTL_WMCAST_BYTES ] =) {
    "Control mcast bytes",
    M2R_RMAC_REG_OFST(TXH_CTRL_WMULTI_BYTE2),     /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* A0 Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_A0_WUCAST_PKTS ] =) {
    "A0 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_RES_WUNI_PKT1),      /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A0_WUCAST_BYTES ] =) {
    "A0 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_RES_WUNI_BYTE2),     /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A0_WMCAST_PKTS ] =) {
    "A0 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_RES_WMULTI_PKT1),    /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A0_WMCAST_BYTES ] =) {
    "A0 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_RES_WMULTI_BYTE2),   /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* A1 Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_A1_WUCAST_PKTS ] =) {
    "A1 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_WUNI_PKT1),          /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A1_WUCAST_BYTES ] =) {
    "A1 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_WUNI_BYTE2),         /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A1_WMCAST_PKTS ] =) {
    "A1 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_HI_WMULTI_PKT1),        /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_A1_WMCAST_BYTES ] =) {
    "A1 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_HI_WMULTI_BYTE2),       /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },


  /* B0 Packets(CIR) */
  CS_DI([ M2R_RMAC_STATS_HOST_B0_WUCAST_PKTS ] =) {
    "B0 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_WUNI_PKT1),     /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B0_WUCAST_BYTES ] =) {
    "B0 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_WUNI_BYTE2),    /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B0_WMCAST_PKTS ] =) {
    "B0 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_WMULTI_PKT1),   /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B0_WMCAST_BYTES ] =) {
    "B0 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_CIR_WMULTI_BYTE2),  /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* B1 Packets(EIR) */
  CS_DI([ M2R_RMAC_STATS_HOST_B1_WUCAST_PKTS ] =) {
    "B1 ucast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_WUNI_PKT1),     /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B1_WUCAST_BYTES ] =) {
    "B1 ucast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_WUNI_BYTE2),    /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B1_WMCAST_PKTS ] =) {
    "B1 mcast pkts",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_WMULTI_PKT1),   /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_B1_WMCAST_BYTES ] =) {
    "B1 mcast bytes",
    M2R_RMAC_REG_OFST(TXH_MED_EIR_WMULTI_BYTE2),  /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Class-C Packets(low priority) */
  CS_DI([ M2R_RMAC_STATS_HOST_WUCAST_PKTS ] =) {
    "Class-C ucast pkts",
    M2R_RMAC_REG_OFST(TXH_WUNI_PKT1),             /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_WUCAST_BYTES ] =) {
    "Class-C ucast bytes",
    M2R_RMAC_REG_OFST(TXH_WUNI_BYTE2),            /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_WMCAST_PKTS ] =) {
    "Class-C mcast pkts",
    M2R_RMAC_REG_OFST(TXH_WMULTI_PKT1),           /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_WMCAST_BYTES ] =) {
    "Class-C mcast bytes",
    M2R_RMAC_REG_OFST(TXH_WMULTI_BYTE2),          /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Host Fairness Packets */
  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_WUCAST_PKTS ] =) {
    "Fairness ucast pkts",
    M2R_RMAC_REG_OFST(TXH_FAIR_WUNI_PKT1),        /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_WUCAST_BYTES ] =) {
    "Fairness ucast bytes",
    M2R_RMAC_REG_OFST(TXH_FAIR_WUNI_BYTE2),       /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_WMCAST_PKTS ] =) {
    "Fairness mcast pkts",
    M2R_RMAC_REG_OFST(TXH_FAIR_WMULTI_PKT1),      /* Tx counter  */
    0,                                            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_FAIR_WMCAST_BYTES ] =) {
    "Fairness mcast bytes",
    M2R_RMAC_REG_OFST(TXH_FAIR_WMULTI_BYTE2),     /* Tx counter  */
    0,                                            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  }
} ;


/*************************************************
 * RINGMAC FRAMER(2 sections)
 *************************************************/
/*
 * FRAMER-General statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_frmr_gen_tbl [ M2R_RMAC_STATS_FRMR_GEN_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_FRMR_SHORT_PKT ] =) {
    "Short Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_SHRT_PKT),    /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_CRC_STOMP ] =) {
    "CRC Stomped Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RX_CRC_STOMP1),   /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_CRC_ERR ] =) {
    "CRC Errored Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RX_CRC_ERR1),     /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_DPERR ] =) {
    "Data Parity Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_DPERR),       /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_SRP_PERR ] =) {
    "SRP/RPR Parity Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_SRP_PERR),    /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_CTRL_PERR ] =) {
    "Control Parity Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_CTRL_PERR),   /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_ERR ] =) {
    "RXFERR signal Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_ERR),         /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_STSIG_ERR ] =) {
    "TXF STSIG Error",
    M2R_RMAC_REG_OFST(TXF_STSIG_ERR),   /* Tx counter  */
    0,                                  /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_RUNT ] =) {
    "Runt Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_RUNT),        /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_GIANT ] =) {
    "Giant Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_GPKT_ERR),    /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_PROTO_ERR ] =) {
    "Protocol Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_PROTPKT),     /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_MCAST_SA ] =) {
    "SA Multicast Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MCASTSA_PKT1),/* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_DELETED_PKTS ] =) {
    "Deleted Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_DELETED_PKT1),/* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_DELETED_BYTES] =) {
    "Deleted Bytes",
    0,                                    /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_DELETED_BYTE2), /* Rx counter  */
    3,                                    /* 16-bit wrds */
    0,                                    /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)      /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_BAD_PKTS ] =) {
    "Bad Pkts",
    M2R_RMAC_REG_OFST(TXF_BAD_PKT1),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_BAD_PKT1),      /* Rx counter  */
    2,                                    /* 16-bit wrds */
    0,                                    /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)      /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_BAD_BYTES] =) {
    "Bad Bytes",
    M2R_RMAC_REG_OFST(TXF_BAD_BYTE2),     /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_BAD_BYTE2),     /* Rx counter  */
    3,                                    /* 16-bit wrds */
    0,                                    /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)      /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_RPR_RX_HEC_ERR ] =) {
    "RXF HEC Mismatch Counts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(RX_HEC_ERR1),     /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_RPR_FRM_HEC_ERR ] =) {
    "FRM Hec Mismatch Counts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(FRM_HEC_ERR),     /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  }
} ;


/*
 * FRAMER Performance Monitors statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_frmr_pm_tbl [ M2R_RMAC_STATS_FRMR_PM_SECT_ALL ] = {
  /* Control Packets */
  CS_DI([ M2R_RMAC_STATS_FRMR_CTL_UCAST_PKTS ] =) {
    "Control ucast pkts",
    M2R_RMAC_REG_OFST(TXF_CTRL_UNI_PKT1),         /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_CTRL_UNI_PKT1),         /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_CTL_UCAST_BYTES ] =) {
    "Control ucast bytes",
    M2R_RMAC_REG_OFST(TXF_CTRL_UNI_BYTE2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_CTRL_UNI_BYTE2),        /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_CTL_MCAST_PKTS ] =) {
    "Control mcast pkts",
    M2R_RMAC_REG_OFST(TXF_CTRL_MULTI_PKT1),       /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_CTRL_MULTI_PKT1),       /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_CTL_MCAST_BYTES ] =) {
    "Control mcast bytes",
    M2R_RMAC_REG_OFST(TXF_CTRL_MULTI_BYTE2),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_CTRL_MULTI_BYTE2),      /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* A0 Packets */
  CS_DI([ M2R_RMAC_STATS_FRMR_A0_UCAST_PKTS ] =) {
    "A0 ucast pkts",
    M2R_RMAC_REG_OFST(TXF_HI_RES_UNI_PKT1),       /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_RES_UNI_PKT1),       /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_A0_UCAST_BYTES ] =) {
    "A0 ucast bytes",
    M2R_RMAC_REG_OFST(TXF_HI_RES_UNI_BYTE2),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_RES_UNI_BYTE2),      /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_A0_MCAST_PKTS ] =) {
    "A0 mcast pkts",
    M2R_RMAC_REG_OFST(TXF_HI_RES_MULTI_PKT1),     /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_RES_MULTI_PKT1),     /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_A0_MCAST_BYTES ] =) {
    "A0 mcast bytes",
    M2R_RMAC_REG_OFST(TXF_HI_RES_MULTI_BYTE2),    /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_RES_MULTI_BYTE2),    /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* A1 Packets */
  CS_DI([ M2R_RMAC_STATS_FRMR_A1_UCAST_PKTS ] =) {
    "A1 ucast pkts",
    M2R_RMAC_REG_OFST(TXF_HI_UNI_PKT1),           /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_UNI_PKT1),           /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_A1_UCAST_BYTES ] =) {
    "A1 ucast bytes",
    M2R_RMAC_REG_OFST(TXF_HI_UNI_BYTE2),          /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_UNI_BYTE2),          /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_A1_MCAST_PKTS ] =) {
    "A1 mcast pkts",
    M2R_RMAC_REG_OFST(TXF_HI_MULTI_PKT1),         /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_MULTI_PKT1),         /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_A1_MCAST_BYTES ] =) {
    "A1 mcast bytes",
    M2R_RMAC_REG_OFST(TXF_HI_MULTI_BYTE2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_HI_MULTI_BYTE2),        /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },


  /* B0 Packets(CIR) */
  CS_DI([ M2R_RMAC_STATS_FRMR_B0_UCAST_PKTS ] =) {
    "B0 ucast pkts",
    M2R_RMAC_REG_OFST(TXF_MED_CIR_UNI_PKT1),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_CIR_UNI_PKT1),      /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_B0_UCAST_BYTES ] =) {
    "B0 ucast bytes",
    M2R_RMAC_REG_OFST(TXF_MED_CIR_UNI_BYTE2),     /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_CIR_UNI_BYTE2),     /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_B0_MCAST_PKTS ] =) {
    "B0 mcast pkts",
    M2R_RMAC_REG_OFST(TXF_MED_CIR_MULTI_PKT1),    /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_CIR_MULTI_PKT1),    /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_B0_MCAST_BYTES ] =) {
    "B0 mcast bytes",
    M2R_RMAC_REG_OFST(TXF_MED_CIR_MULTI_BYTE2),   /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_CIR_MULTI_BYTE2),   /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* B1 Packets(EIR) */
  CS_DI([ M2R_RMAC_STATS_FRMR_B1_UCAST_PKTS ] =) {
    "B1 ucast pkts",
    M2R_RMAC_REG_OFST(TXF_MED_EIR_UNI_PKT1),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_EIR_UNI_PKT1),      /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_B1_UCAST_BYTES ] =) {
    "B1 ucast bytes",
    M2R_RMAC_REG_OFST(TXF_MED_EIR_UNI_BYTE2),     /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_EIR_UNI_BYTE2),     /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_B1_MCAST_PKTS ] =) {
    "B1 mcast pkts",
    M2R_RMAC_REG_OFST(TXF_MED_EIR_MULTI_PKT1),    /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_EIR_MULTI_PKT1),    /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_B1_MCAST_BYTES ] =) {
    "B1 mcast bytes",
    M2R_RMAC_REG_OFST(TXF_MED_EIR_MULTI_BYTE2),   /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MED_EIR_MULTI_BYTE2),   /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Class-C Packets(low priority) */
  CS_DI([ M2R_RMAC_STATS_FRMR_UCAST_PKTS ] =) {
    "Class-C ucast pkts",
    M2R_RMAC_REG_OFST(TXF_UNI_PKT1),              /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_UNI_PKT1),              /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_UCAST_BYTES ] =) {
    "Class-C ucast bytes",
    M2R_RMAC_REG_OFST(TXF_UNI_BYTE2),             /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_UNI_BYTE2),             /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_MCAST_PKTS ] =) {
    "Class-C mcast pkts",
    M2R_RMAC_REG_OFST(TXF_MULTI_PKT1),            /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MULTI_PKT1),            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_MCAST_BYTES ] =) {
    "Class-C mcast bytes",
    M2R_RMAC_REG_OFST(TXF_MULTI_BYTE2),           /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_MULTI_BYTE2),           /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Fairness Packets */
  CS_DI([ M2R_RMAC_STATS_FRMR_FAIR_UCAST_PKTS ] =) {
    "Fairness ucast pkts",
    M2R_RMAC_REG_OFST(TXF_FAIR_UNI_PKT1),         /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_FAIR_UNI_PKT1),         /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_FAIR_UCAST_BYTES ] =) {
    "Fairness ucast bytes",
    M2R_RMAC_REG_OFST(TXF_FAIR_UNI_BYTE2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_FAIR_UNI_BYTE2),        /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_FAIR_MCAST_PKTS ] =) {
    "Fairness mcast pkts",
    M2R_RMAC_REG_OFST(TXF_FAIR_MULTI_PKT1),       /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_FAIR_MULTI_PKT1),       /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_FAIR_MCAST_BYTES ] =) {
    "Fairness mcast bytes",
    M2R_RMAC_REG_OFST(TXF_FAIR_MULTI_BYTE2),      /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_FAIR_MULTI_BYTE2),      /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Flooded Packets */
  CS_DI([ M2R_RMAC_STATS_FRMR_FLOOD_PKTS ] =) {
    "Flooded pkts",
    M2R_RMAC_REG_OFST(TXF_FLOOD_PKT1),            /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_FLOOD_PKT1),            /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_FLOOD_BYTES ] =) {
    "Flooded bytes",
    M2R_RMAC_REG_OFST(TXF_FLOOD_BYTE2),           /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_FLOOD_BYTE2),           /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Non-flooded Packets */
  CS_DI([ M2R_RMAC_STATS_FRMR_NOFLOOD_PKTS ] =) {
    "Non-flooded pkts",
    M2R_RMAC_REG_OFST(TXF_NOFLOOD_PKT1),          /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_NOFLOOD_PKT1),          /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_NOFLOOD_BYTES ] =) {
    "Non-flooded bytes",
    M2R_RMAC_REG_OFST(TXF_NOFLOOD_BYTE2),         /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_NOFLOOD_BYTE2),         /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  },

  /* Idle Packets */
  CS_DI([ M2R_RMAC_STATS_FRMR_IDLE_PKTS ] =) {
    "Idle pkts",
    M2R_RMAC_REG_OFST(TXF_IDLE_PKT1),             /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_IDLE_PKT1),             /* Rx counter  */
    2,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)              /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_FRMR_IDLE_BYTES ] =) {
    "Idle bytes",
    M2R_RMAC_REG_OFST(TXF_IDLE_BYTE2),            /* Tx counter  */
    M2R_RMAC_REG_OFST(RXF_IDLE_BYTE2),            /* Rx counter  */
    3,                                            /* 16-bit wrds */
    0,                                            /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)              /* flag        */
  }
} ;


/*
 * MATE-General statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_mate_gen_tbl [ M2R_RMAC_STATS_MATE_GEN_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_MATE_RUNT ] =) {
    "FROM Mate Short Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(FRM_RUNT),        /* Rx counter  */
    1,                                  /* 8-bit field */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_GIANT ] =) {
    "FROM Mate Giant Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(FRM_GIANT),       /* Rx counter  */
    1,                                  /* 8-bit field */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_OVERFLOW ] =) {
    "From Mate Overflow",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(FRM_OVERFLOW),    /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_UNDERFLOW ] =) {
    "From Mate Underflow",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(FRM_UNDERFLOW),   /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_CRC_ERR ] =) {
    "From Mate CRC Error",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(FRM_CRC_ERR),     /* Rx counter  */
    1,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_CTRL_SQNC_ERR ] =) {
    "From Mate Ctl Seq Error",
    0,                                    /* Tx counter  */
    M2R_RMAC_REG_OFST(FRM_CTRL_SQNC_ERR), /* Rx counter  */
    1,                                    /* 16-bit wrds */
    0,                                    /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)              /* flag        */
  }
} ;


/*
 * MATE-Pm statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_mate_pm_tbl [ M2R_RMAC_STATS_MATE_PM_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_MATE_WE_PKTS ] =) {
    "To Mate WE Dropped Pkts",
    M2R_RMAC_REG_OFST(TOM_WE_PKT1),     /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_WE_BYTES ] =) {
    "To Mate WE Dropped Bytes",
    M2R_RMAC_REG_OFST(TOM_WE_BYTE2),    /* Tx counter  */
    0,                                  /* Rx counter  */
    3,                                  /* 40-bit val  */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_SETWS_PKTS ] =) {
    "To Mate SETWS Pkts",
    M2R_RMAC_REG_OFST(TOM_SETWS_PKT1),  /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_FT_PKTS ] =) {
    "To Mate FT Pkts",
    M2R_RMAC_REG_OFST(TOM_FT_PKT1),     /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_WS_PKTS ] =) {
    "To Mate WS Dropped Pkts",
    M2R_RMAC_REG_OFST(TOM_WS_PKT1),     /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MATE_WS_BYTES ] =) {
    "To Mate WS Dropped Bytes",
    M2R_RMAC_REG_OFST(TOM_WS_BYTE2),    /* Tx counter  */
    0,                                  /* Rx counter  */
    3,                                  /* 40-bit val  */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)    /* flag        */
  }
} ;


/*
 * SRPF-Pm statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_srpf_pm_tbl [ M2R_RMAC_STATS_SRPF_PM_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_SRPF_TB_HI_PKTS ] =) {
    "Hi Pri Data Pkts",
    M2R_RMAC_REG_OFST(TB_HI_PKT1),      /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_SRPF_TB_HI_BYTES ] =) {
    "Hi Pri Data Bytes",
    M2R_RMAC_REG_OFST(TB_HI_BYTE2),     /* Tx counter  */
    0,                                  /* Rx counter  */
    3,                                  /* 40-bit val  */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_SRPF_TB_LO_PKTS ] =) {
    "Lo Pri Data Pkts",
    M2R_RMAC_REG_OFST(TB_LO_PKT1),      /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_SRPF_TB_LO_BYTES ] =) {
    "Lo Pri Data Bytes",
    M2R_RMAC_REG_OFST(TB_LO_BYTE2),     /* Tx counter  */
    0,                                  /* Rx counter  */
    3,                                  /* 40-bit val  */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_SRPF_TB_UNI_PKTS ] =) {
    "Uni Data Pkts",
    M2R_RMAC_REG_OFST(TB_UNI_PKT1),     /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_SRPF_TB_UNI_BYTES ] =) {
    "Uni Data Bytes",
    M2R_RMAC_REG_OFST(TB_UNI_BYTE2),    /* Tx counter  */
    0,                                  /* Rx counter  */
    3,                                  /* 40-bit val  */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_SRPF_TB_MULTI_PKTS ] =) {
    "Multi Data Pkts",
    M2R_RMAC_REG_OFST(TB_MULTI_PKT1),   /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)    /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_SRPF_TB_MULTI_BYTES ] =) {
    "Multi Data Bytes",
    M2R_RMAC_REG_OFST(TB_MULTI_BYTE2),  /* Tx counter  */
    0,                                  /* Rx counter  */
    3,                                  /* 40-bit val  */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)    /* flag        */
  }
} ;


/*
 * TB-General statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_tb_gen_tbl [ M2R_RMAC_STATS_TB_GEN_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_TB_HI_ECCFIX ] =) {
    "Hi Pri TB fixable ecc err",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(HTB_DPERR),       /* Rx counter  */
    1,                                  /* msb 8-bit field */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_SPL_GET \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_ECCERR ] =) {
    "Hi Pri TB unfixable ecc err",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(HTB_DPERR),       /* Rx counter  */
    1,                                  /* lsb 8-bit field */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_SPL_GET \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_ECCFIX ] =) {
    "Lo Pri TB fixable ecc err",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(LTB_DPERR),       /* Rx counter  */
    1,                                  /* msb 8-bit field */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_SPL_GET \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_ECCERR ] =) {
    "Lo Pri TB unfixable ecc err",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(LTB_DPERR),       /* Rx counter  */
    1,                                  /* lsb 8-bit field */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_SPL_GET \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_DROP_PKT ] =) {
    "Hi Pri TB Dropped Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(HTB_DROP_PKT),    /* Rx counter  */
    1,                                  /*16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_DROP_PKT ] =) {
    "Lo Pri TB Dropped Pkts",
    0,                                  /* Tx counter  */
    M2R_RMAC_REG_OFST(LTB_DROP_PKT),    /* Rx counter  */
    1,                                  /*16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)            /* flag        */
  }
} ;


/*
 * TB-Pm statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_tb_pm_tbl [ M2R_RMAC_STATS_TB_PM_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_TB_HI_DEL_ACC ] =) {
    "Hi Pri TB Delay Time Accum",
    M2R_RMAC_REG_OFST(HI_TB_DEL_ACC2),      /* Tx counter  */
    0,                                      /* Rx counter  */
    3,                                      /* 48-bit val  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_DEL_CNT ] =) {
    "Hi Pri TB Delay Pkts",
    M2R_RMAC_REG_OFST(HI_TB_DEL_CNT1),      /* Tx counter  */
    0,                                      /* Rx counter  */
    2,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)                /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_DEL_MIN ] =) {
    "Hi Pri TB Min Delay(ns)",
    M2R_RMAC_REG_OFST(HI_TB_DEL_MIN1),      /* Tx counter  */
    0,                                      /* Rx counter  */
    2,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY     \
     | M2R_STATS_FLAG_NO_ACCUM  \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_DEL_MAX ] =) {
    "Hi Pri TB Max Delay(ns)",
    M2R_RMAC_REG_OFST(HI_TB_DEL_MAX1),      /* Tx counter  */
    0,                                      /* Rx counter  */
    2,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM  \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_DEL_AVG ] =) {
    "Hi Pri TB AVG Delay(ns)",
    0,                                      /* Tx counter  */
    0,                                      /* Rx counter  */
    0,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM  \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_DEL_ACC ] =) {
    "Lo Pri TB Delay Time Accum",
    M2R_RMAC_REG_OFST(LO_TB_DEL_ACC2),      /* Tx counter  */
    0,                                      /* Rx counter  */
    3,                                      /* 48-bit val  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_DEL_CNT ] =) {
    "Lo Pri TB Delay Pkts",
    M2R_RMAC_REG_OFST(LO_TB_DEL_CNT1),      /* Tx counter  */
    0,                                      /* Rx counter  */
    2,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)                /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_DEL_MIN ] =) {
    "Lo Pri TB Min Delay(ns)",
    M2R_RMAC_REG_OFST(LO_TB_DEL_MIN1),      /* Tx counter  */
    0,                                      /* Rx counter  */
    2,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY     \
     | M2R_STATS_FLAG_NO_ACCUM  \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_DEL_MAX ] =) {
    "Lo Pri TB Max Delay(ns)",
    M2R_RMAC_REG_OFST(LO_TB_DEL_MAX1),      /* Tx counter  */
    0,                                      /* Rx counter  */
    2,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY     \
     | M2R_STATS_FLAG_NO_ACCUM  \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_DEL_AVG ] =) {
    "Lo Pri TB AVG Delay(ns)",
    0,                                      /* Tx counter  */
    0,                                      /* Rx counter  */
    0,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_ONLY     \
     | M2R_STATS_FLAG_NO_ACCUM  \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_KILL_PKTS ] =) {
    "PTQ Flush Killed Pkts",
    M2R_RMAC_REG_OFST(TBH_KILL_PC1),        /* Tx counter  */
    M2R_RMAC_REG_OFST(TINH_KILL_PC1),       /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)        /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_HI_KILL_BYTES ] =) {
    "PTQ Flush Killed Bytes",
    M2R_RMAC_REG_OFST(TBH_KILL_BC2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(TINH_KILL_BC2),       /* Rx counter  */
    3,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)        /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_KILL_PKTS ] =) {
    "STQ Flush Killed Pkts",
    M2R_RMAC_REG_OFST(TBL_KILL_PC1),        /* Tx counter  */
    M2R_RMAC_REG_OFST(TINL_KILL_PC1),       /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)        /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TB_LO_KILL_BYTES ] =) {
    "STQ Flush Killed Bytes",
    M2R_RMAC_REG_OFST(TBL_KILL_BC2),        /* Tx counter  */
    M2R_RMAC_REG_OFST(TINL_KILL_BC2),       /* Rx counter  */
    3,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_TX_N_RX         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)        /* flag        */
  }
} ;


/*
 * MRW-Pm statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_mrw_pm_tbl [ M2R_RMAC_STATS_MRW_PM_SECT_ALL ] = {

  CS_DI([ M2R_RMAC_STATS_MRW_SADA_PKT ] =) {
    "Reflected(SA=DA) dropped pkts",
    M2R_RMAC_REG_OFST(MAC_SADA_PKT1),   /* Tx counter  */
    0,                                  /* Rx counter  */
    2,                                  /* 16-bit wrds */
    0,                                  /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)            /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MRW_REWRITE_PKT ] =) {
    "MAC Rewritten Pkts",
    M2R_RMAC_REG_OFST(MAC_REWRITE_PKT1),   /* Tx counter  */
    0,                                     /* Rx counter  */
    2,                                     /* 16-bit wrds */
    0,                                     /* stride      */
    (M2R_RMAC_STATS_TX_ONLY)               /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MRW_FLUSH_HOST_PKT ] =) {
    "Flushed Host Pkts",
    M2R_RMAC_REG_OFST(FLUSH_HOST_PKT1),    /* Tx counter  */
    0,                                     /* Rx counter  */
    2,                                     /* 16-bit wrds */
    0,                                     /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MRW_FLUSH_HOST_BYTE ] =) {
    "Flushed Host Bytes",
    M2R_RMAC_REG_OFST(FLUSH_HOST_BYTE2),   /* Tx counter  */
    0,                                     /* Rx counter  */
    3,                                     /* 40-bit val  */
    0,                                     /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MRW_STRICTFLUSH_PKT ] =) {
    "TSN Flushed Host Pkts",
    M2R_RMAC_REG_OFST(STRICTFLUSH_PKT1),   /* Tx counter  */
    0,                                     /* Rx counter  */
    2,                                     /* 16-bit wrds */
    0,                                     /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MRW_STRICTFLUSH_BYTE ] =) {
    "TSN Flushed Host Bytes",
    M2R_RMAC_REG_OFST(STRICTFLUSH_BYTE2),  /* Tx counter  */
    0,                                     /* Rx counter  */
    3,                                     /* 40-bit val  */
    0,                                     /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MRW_MACDROP_PKT ] =) {
    "Dropped Host Pkts",
    M2R_RMAC_REG_OFST(MACDROP_PKT1),       /* Tx counter  */
    0,                                     /* Rx counter  */
    2,                                     /* 16-bit wrds */
    0,                                     /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)       /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MRW_MACDROP_BYTE ] =) {
    "Dropped Host Bytes",
    M2R_RMAC_REG_OFST(MACDROP_BYTE2),      /* Tx counter  */
    0,                                     /* Rx counter  */
    3,                                     /* 40-bit val  */
    0,                                     /* stride      */
    (M2R_RMAC_STATS_TX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)       /* flag        */
  }
} ;


/*
 * TBD-General statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_tbd_gen_tbl [ M2R_RMAC_STATS_TBD_GEN_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_TBD_IPS_RCV_DROP_PKT ] =) {
    "Rcv Buf Full Dropped IPS Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(IPS_RCV_DROP_COUNT1),   /* Rx counter  */
    2,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                  /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_IPS_RCV_ERR_PKT ] =) {
    "Dropped Error IPS Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(IPS_RCV_ERR_COUNT1),    /* Rx counter  */
    2,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                  /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_CTL_RCV_DROP_PKT ] =) {
    "Rcv Buf Full Dropped CTL Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(CTL_RCV_DROP_COUNT1),   /* Rx counter  */
    2,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                  /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_CTL_RCV_ERR_PKT ] =) {
    "Dropped Error CTL Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(CTL_RCV_ERR_COUNT1),    /* Rx counter  */
    2,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                  /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_RING_ID_ERR ] =) {
    "Ring ID Error Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(RING_ID_ERR),           /* Rx counter  */
    1,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                  /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_RUNT_USG ] =) {
    "TBD Runt Usage Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(TBD_RUNT_USG),          /* Rx counter  */
    1,                                        /* 8-bit val   */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                  /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_SA_STRIP_PKT ] =) {
    "SA Stripped Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(SA_STRIP_PKT1),         /* Rx counter  */
    2,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)          /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_SA_STRIP_BYTE ] =) {
    "SA Stripped Bytes",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(SA_STRIP_BYTE2),        /* Rx counter  */
    3,                                        /* 40-bit val  */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)          /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_TTL_DROP_PKT ] =) {
    "TTL expire Dropped Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(TTL_DROP_PKT1),         /* Rx counter  */
    2,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)          /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_TTL_DROP_BYTE ] =) {
    "TTL expire Dropped Bytes",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(TTL_DROP_BYTE2),        /* Rx counter  */
    3,                                        /* 40-bit val  */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)          /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_STRICT_STRIP_PKT ] =) {
    "Failed Strict Stripped Pkts",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(STRICT_STRIP_PKT1),     /* Rx counter  */
    2,                                        /* 16-bit wrds */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_NEXT_UNIT_TIE)          /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_TBD_STRICT_STRIP_BYTE ] =) {
    "Failed Strict Stripped Bytes",
    0,                                        /* Tx counter  */
    M2R_RMAC_REG_OFST(STRICT_STRIP_BYTE2),    /* Rx counter  */
    3,                                        /* 40-bit val  */
    0,                                        /* stride      */
    (M2R_RMAC_STATS_RX_ONLY         \
     | M2R_STATS_FLAG_LAST_UNIT_TIE)          /* flag        */
  }
} ;


/*
 * MS-Pm statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_ms_pm_tbl [ M2R_RMAC_STATS_MS_PM_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_MS_LTB_MIN_DEP ] =) {
    "Lo Pri TB Min Depth(16B)",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(LTB_MIN_DEP1),        /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM)             /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MS_LTB_MAX_DEP ] =) {
    "Lo Pri TB Max Depth(16B)",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(LTB_MAX_DEP1),        /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM)             /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MS_LTB_ACC_DEP ] =) {
    "Lo Pri TB Accum Depth(16B)",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(LTB_ACC_DEP2),        /* Rx counter  */
    3,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM)             /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MS_LTB_PKT ] =) {
    "Lo Pri TB Depth Sample times",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(LTB_PKT_CNT1),        /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MS_HTB_MIN_DEP ] =) {
    "Hi Pri TB Min Depth(16B)",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(HTB_MIN_DEP1),        /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM)             /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MS_HTB_MAX_DEP ] =) {
    "Hi Pri TB Max Depth(16B)",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(HTB_MAX_DEP1),        /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM)             /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MS_HTB_ACC_DEP ] =) {
    "Hi Pri TB Accum Depth(16B)",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(HTB_ACC_DEP2),        /* Rx counter  */
    3,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY \
     | M2R_STATS_FLAG_NO_ACCUM)             /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MS_HTB_PKT ] =) {
    "Hi Pri TB Depth Sample times",
    0,                                      /* Tx counter  */
    M2R_RMAC_REG_OFST(HTB_PKT_CNT1),        /* Rx counter  */
    2,                                      /* 16-bit wrds  */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY)                /* flag        */
  }
} ;

/*
 * MCOARSE-CAM statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_mcoarse_cam_tbl [ M2R_RMAC_STATS_MCOARSE_CAM_SECT_ALL ] = {
  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_0_CNTR ] =) {
    "MC CAM Entry 0 Hit",
    0,                                      /* Pkt counter : entry_num  */
    0,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_1_CNTR ] =) {
    "MC CAM Entry 1 Hit",
    1,                                      /* Pkt counter : entry_num  */
    1,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_2_CNTR ] =) {
    "MC CAM Entry 2 Hit",
    2,                                      /* Pkt counter : entry_num  */
    2,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_3_CNTR ] =) {
    "MC CAM Entry 3 Hit",
    3,                                      /* Pkt counter : entry_num  */
    3,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_4_CNTR ] =) {
    "MC CAM Entry 4 Hit",
    4,                                      /* Pkt counter : entry_num  */
    4,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_5_CNTR ] =) {
    "MC CAM Entry 5 Hit",
    5,                                      /* Pkt counter : entry_num  */
    5,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_6_CNTR ] =) {
    "MC CAM Entry 6 Hit",
    6,                                      /* Pkt counter : entry_num  */
    6,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_MCOARSE_CAM_ENTRY_7_CNTR ] =) {
    "MC CAM Entry 7 Hit",
    7,                                      /* Pkt counter : entry_num  */
    7,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride      */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  }
} ;

/*
 * Host-Drop-CAM statistics
 */
const static m2r_stats_info_t
m2r_rmac_stats_hdrop_cam_tbl [ M2R_RMAC_STATS_HOST_DROP_CAM_SECT_ALL ] = {
  /* Channel 0 packet with cam hit */
  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_0_CNTR ] =) {
    "HD CAM Entry 0 Hit C0",
    0,                                      /* Pkt counter : entry_num  */
    0,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_0_CNTR ] =) {
    "HD CAM Entry 1 Hit C0",
    1,                                      /* Pkt counter : entry_num  */
    1,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_0_CNTR ] =) {
    "HD CAM Entry 2 Hit C0",
    2,                                      /* Pkt counter : entry_num  */
    2,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_0_CNTR ] =) {
    "HD CAM Entry 3 Hit C0",
    3,                                      /* Pkt counter : entry_num  */
    3,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_0_CNTR ] =) {
    "HD CAM Entry 4 Hit C0",
    4,                                      /* Pkt counter : entry_num  */
    4,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_0_CNTR ] =) {
    "HD CAM Entry 5 Hit C0",
    5,                                      /* Pkt counter : entry_num  */
    5,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_0_CNTR ] =) {
    "HD CAM Entry 6 Hit C0",
    6,                                      /* Pkt counter : entry_num  */
    6,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_0_CNTR ] =) {
    "HD CAM Entry 7 Hit C0",
    7,                                      /* Pkt counter : entry_num  */
    7,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_0_CNTR ] =) {
    "HD CAM Entry 8 Hit C0",
    8,                                      /* Pkt counter : entry_num  */
    8,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_0_CNTR ] =) {
    "HD CAM Entry 9 Hit C0",
    9,                                      /* Pkt counter : entry_num  */
    9,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_0_CNTR ] =) {
    "HD CAM Entry 10 Hit C0",
    10,                                     /* Pkt counter : entry_num  */
    10,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_0_CNTR ] =) {
    "HD CAM Entry 11 Hit C0",
    11,                                     /* Pkt counter : entry_num  */
    11,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_0_CNTR ] =) {
    "HD CAM Entry 12 Hit C0",
    12,                                     /* Pkt counter : entry_num  */
    12,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_0_CNTR ] =) {
    "HD CAM Entry 13 Hit C0",
    13,                                     /* Pkt counter : entry_num  */
    13,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_0_CNTR ] =) {
    "HD CAM Entry 14 Hit C0",
    14,                                     /* Pkt counter : entry_num  */
    14,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_0_CNTR ] =) {
    "HD CAM Entry 15 Hit C0",
    15,                                     /* Pkt counter : entry_num  */
    15,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  /* Channel 1 packet with cam hit */
  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_1_CNTR ] =) {
    "HD CAM Entry 0 Hit C1",
    0,                                      /* Pkt counter : entry_num  */
    0,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_1_CNTR ] =) {
    "HD CAM Entry 1 Hit C1",
    1,                                      /* Pkt counter : entry_num  */
    1,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_1_CNTR ] =) {
    "HD CAM Entry 2 Hit C1",
    2,                                      /* Pkt counter : entry_num  */
    2,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_1_CNTR ] =) {
    "HD CAM Entry 3 Hit C1",
    3,                                      /* Pkt counter : entry_num  */
    3,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_1_CNTR ] =) {
    "HD CAM Entry 4 Hit C1",
    4,                                      /* Pkt counter : entry_num  */
    4,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_1_CNTR ] =) {
    "HD CAM Entry 5 Hit C1",
    5,                                      /* Pkt counter : entry_num  */
    5,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_1_CNTR ] =) {
    "HD CAM Entry 6 Hit C1",
    6,                                      /* Pkt counter : entry_num  */
    6,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_1_CNTR ] =) {
    "HD CAM Entry 7 Hit C1",
    7,                                      /* Pkt counter : entry_num  */
    7,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_1_CNTR ] =) {
    "HD CAM Entry 8 Hit C1",
    8,                                      /* Pkt counter : entry_num  */
    8,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_1_CNTR ] =) {
    "HD CAM Entry 9 Hit C1",
    9,                                      /* Pkt counter : entry_num  */
    9,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_1_CNTR ] =) {
    "HD CAM Entry 10 Hit C1",
    10,                                     /* Pkt counter : entry_num  */
    10,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_1_CNTR ] =) {
    "HD CAM Entry 11 Hit C1",
    11,                                     /* Pkt counter : entry_num  */
    11,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_1_CNTR ] =) {
    "HD CAM Entry 12 Hit C1",
    12,                                     /* Pkt counter : entry_num  */
    12,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_1_CNTR ] =) {
    "HD CAM Entry 13 Hit C1",
    13,                                     /* Pkt counter : entry_num  */
    13,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_1_CNTR ] =) {
    "HD CAM Entry 14 Hit C1",
    14,                                     /* Pkt counter : entry_num  */
    14,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_1_CNTR ] =) {
    "HD CAM Entry 15 Hit C1",
    15,                                     /* Pkt counter : entry_num  */
    15,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  /* Channel 2 packet with cam hit */
  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_2_CNTR ] =) {
    "HD CAM Entry 0 Hit C2",
    0,                                      /* Pkt counter : entry_num  */
    0,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_2_CNTR ] =) {
    "HD CAM Entry 1 Hit C2",
    1,                                      /* Pkt counter : entry_num  */
    1,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_2_CNTR ] =) {
    "HD CAM Entry 2 Hit C2",
    2,                                      /* Pkt counter : entry_num  */
    2,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_2_CNTR ] =) {
    "HD CAM Entry 3 Hit C2",
    3,                                      /* Pkt counter : entry_num  */
    3,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_2_CNTR ] =) {
    "HD CAM Entry 4 Hit C2",
    4,                                      /* Pkt counter : entry_num  */
    4,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_2_CNTR ] =) {
    "HD CAM Entry 5 Hit C2",
    5,                                      /* Pkt counter : entry_num  */
    5,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_2_CNTR ] =) {
    "HD CAM Entry 6 Hit C2",
    6,                                      /* Pkt counter : entry_num  */
    6,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_2_CNTR ] =) {
    "HD CAM Entry 7 Hit C2",
    7,                                      /* Pkt counter : entry_num  */
    7,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_2_CNTR ] =) {
    "HD CAM Entry 8 Hit C2",
    8,                                      /* Pkt counter : entry_num  */
    8,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_2_CNTR ] =) {
    "HD CAM Entry 9 Hit C2",
    9,                                      /* Pkt counter : entry_num  */
    9,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_2_CNTR ] =) {
    "HD CAM Entry 10 Hit C2",
    10,                                     /* Pkt counter : entry_num  */
    10,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_2_CNTR ] =) {
    "HD CAM Entry 11 Hit C2",
    11,                                     /* Pkt counter : entry_num  */
    11,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_2_CNTR ] =) {
    "HD CAM Entry 12 Hit C2",
    12,                                     /* Pkt counter : entry_num  */
    12,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_2_CNTR ] =) {
    "HD CAM Entry 13 Hit C2",
    13,                                     /* Pkt counter : entry_num  */
    13,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_2_CNTR ] =) {
    "HD CAM Entry 14 Hit C2",
    14,                                     /* Pkt counter : entry_num  */
    14,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_2_CNTR ] =) {
    "HD CAM Entry 15 Hit C2",
    15,                                     /* Pkt counter : entry_num  */
    15,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  /* Channel 3 packet with cam hit */
  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_0_CHNL_3_CNTR ] =) {
    "HD CAM Entry 0 Hit C3",
    0,                                      /* Pkt counter : entry_num  */
    0,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_1_CHNL_3_CNTR ] =) {
    "HD CAM Entry 1 Hit C3",
    1,                                      /* Pkt counter : entry_num  */
    1,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_2_CHNL_3_CNTR ] =) {
    "HD CAM Entry 2 Hit C3",
    2,                                      /* Pkt counter : entry_num  */
    2,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_3_CHNL_3_CNTR ] =) {
    "HD CAM Entry 3 Hit C3",
    3,                                      /* Pkt counter : entry_num  */
    3,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_4_CHNL_3_CNTR ] =) {
    "HD CAM Entry 4 Hit C3",
    4,                                      /* Pkt counter : entry_num  */
    4,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_5_CHNL_3_CNTR ] =) {
    "HD CAM Entry 5 Hit C3",
    5,                                      /* Pkt counter : entry_num  */
    5,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_6_CHNL_3_CNTR ] =) {
    "HD CAM Entry 6 Hit C3",
    6,                                      /* Pkt counter : entry_num  */
    6,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_7_CHNL_3_CNTR ] =) {
    "HD CAM Entry 7 Hit C3",
    7,                                      /* Pkt counter : entry_num  */
    7,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_8_CHNL_3_CNTR ] =) {
    "HD CAM Entry 8 Hit C3",
    8,                                      /* Pkt counter : entry_num  */
    8,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_9_CHNL_3_CNTR ] =) {
    "HD CAM Entry 9 Hit C3",
    9,                                      /* Pkt counter : entry_num  */
    9,                                      /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_10_CHNL_3_CNTR ] =) {
    "HD CAM Entry 10 Hit C3",
    10,                                     /* Pkt counter : entry_num  */
    10,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_11_CHNL_3_CNTR ] =) {
    "HD CAM Entry 11 Hit C3",
    11,                                     /* Pkt counter : entry_num  */
    11,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_12_CHNL_3_CNTR ] =) {
    "HD CAM Entry 12 Hit C3",
    12,                                     /* Pkt counter : entry_num  */
    12,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_13_CHNL_3_CNTR ] =) {
    "HD CAM Entry 13 Hit C3",
    13,                                     /* Pkt counter : entry_num  */
    13,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_14_CHNL_3_CNTR ] =) {
    "HD CAM Entry 14 Hit C3",
    14,                                     /* Pkt counter : entry_num  */
    14,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_ENTRY_15_CHNL_3_CNTR ] =) {
    "HD CAM Entry 15 Hit C3",
    15,                                     /* Pkt counter : entry_num  */
    15,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  /* Channel 0 packet with no cam hit */
  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_0_CNTR ] =) {
    "HD CAM No Hit C0",
    64,                                     /* Pkt counter : entry_num  */
    64,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    0,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_1_CNTR ] =) {
    "HD CAM No Hit C1",
    65,                                     /* Pkt counter : entry_num  */
    65,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    1,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_2_CNTR ] =) {
    "HD CAM No Hit C2",
    66,                                     /* Pkt counter : entry_num  */
    66,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    2,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  },

  CS_DI([ M2R_RMAC_STATS_HOST_DROP_CAM_NO_HIT_CHNL_3_CNTR ] =) {
    "HD CAM No Hit C3",
    67,                                     /* Pkt counter : entry_num  */
    67,                                     /* Byte counter : entry_num  */
    5,                                      /* 16-bit wrds */
    3,                                      /* stride : chnnel id */
    (M2R_RMAC_STATS_RX_ONLY     \
     | M2R_STATS_FLAG_SPL_GET   \
     | M2R_STATS_FLAG_SPL_PRINT \
     | M2R_STATS_FLAG_SPL_CLEAR )           /* flag        */
  }
} ;


/************ SECTION TABLE **********************/
/*
 * This section table is used in setting up the various
 * Section CB's for the RingMAC block.
 */
const static m2r_stats_sect_tbl_entry_t
rmac_stats_sect_tbl[M2R_RMAC_STATS_SECT_ALL] = {

  CS_DI([M2R_RMAC_STATS_HOST_GEN_SECT] =) {
    "RingMAC Host-General Stats Section",
    M2R_OFST(m2r_rmac_stats_t, host_gen),     /* sect_offset */
    0,                                        /* stride_id   */
    m2r_rmac_stats_host_gen_tbl,              /* info_tbl    */
    M2R_RMAC_STATS_HOST_GEN_SECT_ALL },       /* num_unit    */

  CS_DI([M2R_RMAC_STATS_HOST_PM_SECT] =) {
    "RingMAC Host-PM Stats Section",
    M2R_OFST(m2r_rmac_stats_t, host_pm),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_host_pm_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_HOST_PM_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_HOST_WRAP_SECT] =) {
    "RingMAC Host WRAP Stats Section",
    M2R_OFST(m2r_rmac_stats_t, host_wrap),      /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_host_wrap_tbl,               /* info_tbl    */
    M2R_RMAC_STATS_HOST_WRAP_SECT_ALL },        /* num_unit    */

  CS_DI([M2R_RMAC_STATS_FRMR_GEN_SECT] =) {
    "RingMAC Framer-General Stats Section",
    M2R_OFST(m2r_rmac_stats_t, frmr_gen),       /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_frmr_gen_tbl,                /* info_tbl    */
    M2R_RMAC_STATS_FRMR_GEN_SECT_ALL },         /* num_unit    */

  CS_DI([M2R_RMAC_STATS_FRMR_PM_SECT] =) {
    "RingMAC Framer-PM Stats Section",
    M2R_OFST(m2r_rmac_stats_t, frmr_pm),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_frmr_pm_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_FRMR_PM_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_MATE_GEN_SECT] =) {
    "RingMAC Mate-General Stats Section",
    M2R_OFST(m2r_rmac_stats_t, mate_gen),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_mate_gen_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_MATE_GEN_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_MATE_PM_SECT] =) {
    "RingMAC Mate-PM Stats Section",
    M2R_OFST(m2r_rmac_stats_t, mate_pm),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_mate_pm_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_MATE_PM_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_SRPF_PM_SECT] =) {
    "RingMAC SRPF-PM Stats Section",
    M2R_OFST(m2r_rmac_stats_t, srpf_pm),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_srpf_pm_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_SRPF_PM_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_TB_GEN_SECT] =) {
    "RingMAC TB-General Stats Section",
    M2R_OFST(m2r_rmac_stats_t, tb_gen),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_tb_gen_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_TB_GEN_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_TB_PM_SECT] =) {
    "RingMAC TB-PM Stats Section",
    M2R_OFST(m2r_rmac_stats_t, tb_pm),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_tb_pm_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_TB_PM_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_MRW_PM_SECT] =) {
    "RingMAC MRW-PM Stats Section",
    M2R_OFST(m2r_rmac_stats_t, mrw_pm),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_mrw_pm_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_MRW_PM_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_TBD_GEN_SECT] =) {
    "RingMAC TBD-General Stats Section",
    M2R_OFST(m2r_rmac_stats_t, tbd_gen),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_tbd_gen_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_TBD_GEN_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_MS_PM_SECT] =) {
    "RingMAC MS-PM Stats Section",
    M2R_OFST(m2r_rmac_stats_t, ms_pm),        /* sect_offset */
    0,                                          /* stride_id   */
    m2r_rmac_stats_ms_pm_tbl,                 /* info_tbl    */
    M2R_RMAC_STATS_MS_PM_SECT_ALL },          /* num_unit    */

  CS_DI([M2R_RMAC_STATS_MCOARSE_CAM_SECT] =) {
    "RingMAC MCoarse CAM Section",
    M2R_OFST(m2r_rmac_stats_t, mc_cam),       /* sect_offset */
    0,                                        /* stride_id   */
    m2r_rmac_stats_mcoarse_cam_tbl,           /* info_tbl    */
    M2R_RMAC_STATS_MCOARSE_CAM_SECT_ALL },    /* num_unit    */

  CS_DI([M2R_RMAC_STATS_HOST_DROP_CAM_SECT] =) {
    "RingMAC Host Drop CAM Section",
    M2R_OFST(m2r_rmac_stats_t, hd_cam),       /* sect_offset */
    0,                                        /* stride_id   */
    m2r_rmac_stats_hdrop_cam_tbl,             /* info_tbl    */
    M2R_RMAC_STATS_HOST_DROP_CAM_SECT_ALL }   /* num_unit    */
} ;

/************************************************************************
 * External function declaration
 ***********************************************************************/
extern cs_boolean m2r_drvr_is_stats_in_hex_fmt(void) ;
extern cs_boolean m2r_drvr_is_mask_zero_cntr(void) ;

/***************************************************************
 *  Internal RingMAC STATS special handler                     *
 ***************************************************************/
void m2r_rmac_stats_spl_update_unit_data(cs_uint16 port_id,
                                   cs_uint16 sect_id,
                                   cs_uint16 unit_id)
{
  /* This API is only called internally, before it is called, all
     the sanity checks have passed already */
  m2r_port_stats_cb_t       * stats_port_cb ;
  m2r_stats_blk_cb_t        * stats_blk_cb ;
  m2r_stats_sect_cb_t       * stats_sect_cb ;
  const m2r_stats_info_t    * p_unit_info ;
  m2r_stats_data_t          * p_unit_data ;
  cs_uint16                 sl_num ;
  M2R_t                     * pDev ;
  DR_TB_HTB_DPERR_t         htb_dperr ;
  DR_TB_LTB_DPERR_t         ltb_dperr ;
  DR_TB_TB_INC_FREQ_t       tb_inc_freq ;
  m2r_rmac_cntr_t           rmac_cntr ;
  cs_uint16                 entry_num, channel_id ;

  stats_port_cb = M2R_GET_STATS_CB(port_id) ;
  stats_blk_cb = stats_port_cb->rmac ;
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;

  p_unit_info = stats_sect_cb->info_tbl + unit_id ;
  p_unit_data = stats_sect_cb->pStats + unit_id ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* TB-General special STATs handler */
  if ( sect_id == M2R_RMAC_STATS_TB_GEN_SECT ) {
  	switch (unit_id) {
  	  case M2R_RMAC_STATS_TB_HI_ECCFIX :
  	  	htb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.HTB_DPERR.wrd) ;
  	  	p_unit_data->rx += htb_dperr.bf.ECCFIX ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_HI_ECCERR;
  	  	p_unit_data->rx += htb_dperr.bf.ECCERR ;
  	  	break;
  	  case M2R_RMAC_STATS_TB_HI_ECCERR :
  	  	htb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.HTB_DPERR.wrd) ;
  	  	p_unit_data->rx += htb_dperr.bf.ECCERR ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_HI_ECCFIX;
  	  	p_unit_data->rx += htb_dperr.bf.ECCFIX ;
  	  	break;
  	  case M2R_RMAC_STATS_TB_LO_ECCFIX :
  	  	ltb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.LTB_DPERR.wrd) ;
  	  	p_unit_data->rx += ltb_dperr.bf.ECCFIX ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_LO_ECCERR;
  	  	p_unit_data->rx += ltb_dperr.bf.ECCERR ;
  	  	break;
  	  case M2R_RMAC_STATS_TB_LO_ECCERR :
  	  	ltb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.LTB_DPERR.wrd) ;
  	  	p_unit_data->rx += ltb_dperr.bf.ECCERR ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_LO_ECCFIX;
  	  	p_unit_data->rx += ltb_dperr.bf.ECCFIX ;
  	  	break;
  	}
  }

  /* TB-PM special STATs handler */
  if ( sect_id == M2R_RMAC_STATS_TB_PM_SECT ) {
    cs_uint64     cntr = 0, time_ns = 0 ;
    cs_uint16     reg_val ;
    cs_uint8      i ;
    volatile cs_reg   * regaddr = NULL ;
    cs_uint64     del_acc, pkt_acc;

    tb_inc_freq.wrd
      = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.TB_INC_FREQ.wrd) ;
    switch (unit_id) {
      case M2R_RMAC_STATS_TB_HI_DEL_ACC :
        regaddr = (volatile cs_reg*)
                   M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, 0, p_unit_info) ;
        for (i = 0; i < 3; i++) {
          reg_val = M2R_REG_READ( &regaddr[i] ) ;
          cntr += ( ((cs_uint64) reg_val) << (16 * (2 - i)) ) ;
        }

        /* Get exact time */
        /*
        time_ns = cntr * (tb_inc_freq.bf.HTB_INC_FREQ + 1) * (1000 / M2R_CORE_CLOCK);
        p_unit_data->tx += time_ns ;
        */
        p_unit_data->tx += cntr ;
      	break;
      case M2R_RMAC_STATS_TB_HI_DEL_MIN :
        regaddr = (volatile cs_reg*)
                   M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, 0, p_unit_info) ;
        for (i = 0; i < 2; i++) {
          reg_val = M2R_REG_READ( &regaddr[i] ) ;
          cntr += ( ((cs_uint64) reg_val) << (16 * (1 - i)) ) ;
        }

        /* Get exact time */
        time_ns = cntr * (tb_inc_freq.bf.HTB_INC_FREQ + 1) * (1000 / M2R_CORE_CLOCK);
        if ( (time_ns < p_unit_data->tx) || (p_unit_data->tx == 0) ) {
          p_unit_data->tx = time_ns ;
        }
      	break;
      case M2R_RMAC_STATS_TB_HI_DEL_MAX :
        regaddr = (volatile cs_reg*)
                   M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, 0, p_unit_info) ;
        for (i = 0; i < 2; i++) {
          reg_val = M2R_REG_READ( &regaddr[i] ) ;
          cntr += ( ((cs_uint64) reg_val) << (16 * (1 - i)) ) ;
        }

        /* Get exact time */
        time_ns = cntr * (tb_inc_freq.bf.HTB_INC_FREQ + 1) * (1000 / M2R_CORE_CLOCK);
        if ( (time_ns > p_unit_data->tx) || (p_unit_data->tx == 0) ) {
          p_unit_data->tx = time_ns ;
        }
      	break;
      case M2R_RMAC_STATS_TB_HI_DEL_AVG :
 		p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_HI_DEL_ACC;
 		del_acc = p_unit_data->tx;
 		p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_HI_DEL_CNT;
 		pkt_acc = p_unit_data->tx;
 		p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_HI_DEL_AVG;
 		if (pkt_acc == 0) {
 		  p_unit_data->tx = 0;
 		} else {
              	    p_unit_data->tx = (cs_uint64)((del_acc * (tb_inc_freq.bf.HTB_INC_FREQ + 1)
                             		* (1000 / M2R_CORE_CLOCK)) / pkt_acc);
        }
      	break;
      case M2R_RMAC_STATS_TB_LO_DEL_ACC :
        regaddr = (volatile cs_reg*)
                   M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, 0, p_unit_info) ;
        for (i = 0; i < 3; i++) {
          reg_val = M2R_REG_READ( &regaddr[i] ) ;
          cntr += ( ((cs_uint64) reg_val) << (16 * (2 - i)) ) ;
        }

        /* Get exact time */
        /*
        time_ns = cntr * (tb_inc_freq.bf.LTB_INC_FREQ + 1) * (1000 / M2R_CORE_CLOCK);
        p_unit_data->tx += time_ns ;
        */
        p_unit_data->tx += cntr ;
      	break;
      case M2R_RMAC_STATS_TB_LO_DEL_MIN :
        regaddr = (volatile cs_reg*)
                   M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, 0, p_unit_info) ;
        for (i = 0; i < 2; i++) {
          reg_val = M2R_REG_READ( &regaddr[i] ) ;
          cntr += ( ((cs_uint64) reg_val) << (16 * (1 - i)) ) ;
        }

        /* Get exact time */
        time_ns = cntr * (tb_inc_freq.bf.LTB_INC_FREQ + 1) * (1000 / M2R_CORE_CLOCK);
        if ( (time_ns < p_unit_data->tx) || (p_unit_data->tx == 0) ) {
          p_unit_data->tx = time_ns ;
        }
      	break;
      case M2R_RMAC_STATS_TB_LO_DEL_MAX :
        regaddr = (volatile cs_reg*)
                   M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, 0, p_unit_info) ;
        for (i = 0; i < 2; i++) {
          reg_val = M2R_REG_READ( &regaddr[i] ) ;
          cntr += ( ((cs_uint64) reg_val) << (16 * (1 - i)) ) ;
        }

        /* Get exact time */
        time_ns = cntr * (tb_inc_freq.bf.LTB_INC_FREQ + 1) * (1000 / M2R_CORE_CLOCK);
        if ( (time_ns > p_unit_data->tx) || (p_unit_data->tx == 0) ) {
          p_unit_data->tx = time_ns ;
        }
      	break;
      case M2R_RMAC_STATS_TB_LO_DEL_AVG :
 		p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_LO_DEL_ACC;
 		del_acc = p_unit_data->tx;
 		p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_LO_DEL_CNT;
 		pkt_acc = p_unit_data->tx;
 		p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_LO_DEL_AVG;
 		if (pkt_acc == 0) {
 	  		p_unit_data->tx = 0;
 		} else {
 	  		p_unit_data->tx = (cs_uint64)((del_acc * (tb_inc_freq.bf.LTB_INC_FREQ + 1)
 	                     * (1000 / M2R_CORE_CLOCK)) / pkt_acc);
        }
      	break;
    }
  }

  /* Multicast coarse cam STATs special handler */
  if ( sect_id == M2R_RMAC_STATS_MCOARSE_CAM_SECT ) {
  	/* p_unit_info->tx_reg_ofst/rx_reg_ofst indicates entery number */
  	entry_num = p_unit_info->rx_reg_ofst ;
  	m2r_rmac_get_coarse_cntr(port_id, entry_num, &rmac_cntr) ;

    /* Update Pkt counter */
    p_unit_data->tx += rmac_cntr.pkts ;
    /* Update Byte counter */
    p_unit_data->rx += rmac_cntr.bytes ;
  }

  /* Host Packet Drop cam STATs special handler */
  if ( sect_id == M2R_RMAC_STATS_HOST_DROP_CAM_SECT ) {
  	/* p_unit_info->rx_reg_ofst indicates entery number */
  	/* p_unit_info->stride indicates channel number */
  	entry_num = p_unit_info->rx_reg_ofst ;
  	channel_id = p_unit_info->stride ;
  	if (entry_num < M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES) {
  	  /* STATs for hit*/
  	  m2r_rmac_get_host_drop_channel_cntr(port_id, TRUE, channel_id,
                                              entry_num, &rmac_cntr) ;
      /* Update Pkt counter */
      p_unit_data->tx += rmac_cntr.pkts ;
      /* Update Byte counter */
      p_unit_data->rx += rmac_cntr.bytes ;
  	} else {
  	  /* STATs for no hit*/
  	  m2r_rmac_get_host_drop_channel_cntr(port_id, FALSE, channel_id,
                                              entry_num, &rmac_cntr) ;
      /* Update Pkt counter */
      p_unit_data->tx += rmac_cntr.pkts ;
      /* Update Byte counter */
      p_unit_data->rx += rmac_cntr.bytes ;
  	}
  }

}

void m2r_rmac_stats_spl_print_unit_cntr(cs_uint16 port_id,
                                   cs_uint16 sect_id,
                                   cs_uint16 unit_id)
{
  /* This API is only called internally, before it is called, all
     the sanity checks have passed already */
  m2r_port_stats_cb_t       * stats_port_cb ;
  m2r_stats_blk_cb_t        * stats_blk_cb ;
  m2r_stats_sect_cb_t       * stats_sect_cb ;
  const m2r_stats_info_t    * p_unit_info ;
  m2r_stats_data_t          * p_unit_data ;
  cs_uint16                 entry_num, channel_id ;
  cs_uint32                 urx, lrx, utx, ltx ;

  stats_port_cb = M2R_GET_STATS_CB(port_id) ;
  stats_blk_cb = stats_port_cb->rmac ;
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;

  p_unit_info = stats_sect_cb->info_tbl + unit_id ;
  p_unit_data = stats_sect_cb->pStats + unit_id ;

  /* TB-General special STATs handler */
  if ( sect_id == M2R_RMAC_STATS_TB_GEN_SECT ) {

  	m2r_rmac_stats_spl_update_unit_data(port_id, sect_id, unit_id);

  	switch (unit_id) {
  	  case M2R_RMAC_STATS_TB_HI_ECCFIX :
  	  case M2R_RMAC_STATS_TB_HI_ECCERR :
  	  case M2R_RMAC_STATS_TB_LO_ECCFIX :
  	  case M2R_RMAC_STATS_TB_LO_ECCERR :
                if (m2r_drvr_is_stats_in_hex_fmt() == TRUE ) {
                  urx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->rx) ;
                  lrx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->rx) ;
                   
                  if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
                    if (p_unit_data->rx) {
                      if (urx>0) {
                        CS_PRINT("\t%-40s :  %8x%08xH %18s\n", p_unit_info->name,
                                 urx, lrx, "N/A") ;
                      } else {
                        CS_PRINT("\t%-40s :  %16xH %18s\n", p_unit_info->name,
                                 lrx, "N/A") ;
                      }
                    }
                  } else {
                    if (urx>0) {
                      CS_PRINT("\t%-40s :  %8x%08xH %18s\n", p_unit_info->name,
                               urx, lrx, "N/A") ;
                    } else {
                      CS_PRINT("\t%-40s :  %16xH %18s\n", p_unit_info->name,
                               lrx, "N/A") ;
                    }
                  }
                }
                else {
                  urx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->rx) ;
                  lrx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->rx) ;

                  if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
                    if (p_unit_data->rx) {
                      if (urx>0) {
                        CS_PRINT("\t%-40s :  %9u%09u %18s\n", p_unit_info->name,
                                 urx, lrx, "N/A") ;
                      } else {
                        CS_PRINT("\t%-40s :  %18u %18s\n", p_unit_info->name,
                                 lrx, "N/A") ;
                      }
                    }
                  } else {
                    if (urx>0) {
                      CS_PRINT("\t%-40s :  %9u%09u %18s\n", p_unit_info->name,
                               urx, lrx, "N/A") ;
                    } else {
                      CS_PRINT("\t%-40s :  %18u %18s\n", p_unit_info->name,
                               lrx, "N/A") ;
                    }
                  }
                }
  	  	break;
  	}

  }

  /* TB-PM special STATs handler */
  if ( sect_id == M2R_RMAC_STATS_TB_PM_SECT ) {
    switch (unit_id) {
      case M2R_RMAC_STATS_TB_HI_DEL_ACC:
      case M2R_RMAC_STATS_TB_HI_DEL_MIN:
      case M2R_RMAC_STATS_TB_HI_DEL_MAX:
      case M2R_RMAC_STATS_TB_HI_DEL_AVG:
      case M2R_RMAC_STATS_TB_LO_DEL_ACC:
      case M2R_RMAC_STATS_TB_LO_DEL_MIN:
      case M2R_RMAC_STATS_TB_LO_DEL_MAX:
      case M2R_RMAC_STATS_TB_LO_DEL_AVG:
        m2r_rmac_stats_spl_update_unit_data(port_id, sect_id, unit_id);
        if (m2r_drvr_is_stats_in_hex_fmt() == TRUE ) {
          utx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->tx) ;
          ltx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->tx) ;

          if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
            if (p_unit_data->tx) {
              if (utx>0) {
                CS_PRINT("\t%-40s :  %18s %8x%08xH\n", p_unit_info->name,
                         "N/A", utx, ltx) ;
              } else {
                CS_PRINT("\t%-40s :  %18s %16xH\n", p_unit_info->name,
                         "N/A", ltx) ;
              }
            }
          } else {
            if (utx>0) {
              CS_PRINT("\t%-40s :  %18s %8x%08xH\n", p_unit_info->name,
                       "N/A", utx, ltx) ;
            } else {
              CS_PRINT("\t%-40s :  %18s %16xH\n", p_unit_info->name,
                       "N/A", ltx) ;
            }
          }
        }
        else {
          utx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->tx) ;
          ltx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->tx) ;

          if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
            if (p_unit_data->tx) {
              if (utx>0) {
                CS_PRINT("\t%-40s :  %18s %9u%09u\n", p_unit_info->name,
                         "N/A", utx, ltx) ;
              } else {
                CS_PRINT("\t%-40s :  %18s %18u\n", p_unit_info->name,
                         "N/A", ltx) ;
              }
            }
          } else {
            if (utx>0) {
              CS_PRINT("\t%-40s :  %18s %9u%09u\n", p_unit_info->name,
                       "N/A", utx, ltx) ;
            } else {
              CS_PRINT("\t%-40s :  %18s %18u\n", p_unit_info->name,
                       "N/A", ltx) ;
            }
          }
        }
        break;
    }
  }

  /* Multicast coarse cam STATs special handler */
  if ( sect_id == M2R_RMAC_STATS_MCOARSE_CAM_SECT ) {
  	/* p_unit_info->rx_reg_ofst indicates entery number */
  	entry_num = p_unit_info->rx_reg_ofst ;

  	/* update STATs first */
  	m2r_rmac_stats_spl_update_unit_data(port_id, sect_id, unit_id);

  	if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
  	  if (p_unit_data->tx || p_unit_data->rx) {
  	    /* dump CAM entry */
  	    CS_PRINT("Multicast Coarse CAM Entry %d STATs =>\n", entry_num) ;
  	    m2r_rmac_dump_mcast_coarse_cam_entries(port_id, entry_num, 1) ;

            if (m2r_drvr_is_stats_in_hex_fmt() == TRUE ) {
              urx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->rx) ;
              lrx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->rx) ;
              utx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->tx) ;
              ltx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->tx) ;

              if (utx>0) {
                CS_PRINT("\t%-36s Pkts :  %8x%08xH %18s\n", p_unit_info->name,
                         utx, ltx, "N/A") ;
              } else {
                CS_PRINT("\t%-36s Pkts :  %16xH %18s\n", p_unit_info->name,
                         ltx, "N/A") ;
              }

              if (urx>0) {
                CS_PRINT("\t%-35s Bytes :  %8x%08xH %18s\n", p_unit_info->name,
                         urx, lrx, "N/A") ;
              } else {
                CS_PRINT("\t%-35s Bytes :  %16xH %18s\n", p_unit_info->name,
                         lrx, "N/A") ;
              }
            } else {
              urx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->rx) ;
              lrx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->rx) ;
              utx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->tx) ;
              ltx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->tx) ;

              if (utx>0) {
                CS_PRINT("\t%-36s Pkts :  %9u%09u %18s\n", p_unit_info->name,
                         utx, ltx, "N/A") ;
              } else {
                CS_PRINT("\t%-36s Pkts :  %18u %18s\n", p_unit_info->name,
                         ltx, "N/A") ;
              }

              if (urx>0) {
                CS_PRINT("\t%-35s Bytes :  %9u%09u %18s\n", p_unit_info->name,
                         urx, lrx, "N/A") ;
              } else {
                CS_PRINT("\t%-35s Bytes :  %18u %18s\n", p_unit_info->name,
                         lrx, "N/A") ;
              }
            }
  	  }
    } else {
  	  /* dump CAM entry */
  	  CS_PRINT("Multicast Coarse CAM Entry %d STATs =>\n", entry_num) ;
  	  m2r_rmac_dump_mcast_coarse_cam_entries(port_id, entry_num, 1) ;

          if (m2r_drvr_is_stats_in_hex_fmt() == TRUE ) {
            urx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->rx) ;
            lrx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->rx) ;
            utx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->tx) ;
            ltx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->tx) ;

            if (utx>0) {
              CS_PRINT("\t%-36s Pkts :  %8x%08xH %18s\n", p_unit_info->name,
                       utx, ltx, "N/A") ;
            } else {
              CS_PRINT("\t%-36s Pkts :  %16xH %18s\n", p_unit_info->name,
                       ltx, "N/A") ;
            }

            if (urx>0) {
              CS_PRINT("\t%-35s Bytes :  %8x%08xH %18s\n", p_unit_info->name,
                       urx, lrx, "N/A") ;
            } else {
              CS_PRINT("\t%-35s Bytes :  %16xH %18s\n", p_unit_info->name,
                       lrx, "N/A") ;
            }
          } else {
            urx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->rx) ;
            lrx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->rx) ;
            utx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->tx) ;
            ltx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->tx) ;

            if (utx>0) {
              CS_PRINT("\t%-36s Pkts :  %9u%09u %18s\n", p_unit_info->name,
                       utx, ltx, "N/A") ;
            } else {
              CS_PRINT("\t%-36s Pkts :  %18u %18s\n", p_unit_info->name,
                       ltx, "N/A") ;
            }

            if (urx>0) {
              CS_PRINT("\t%-35s Bytes :  %9u%09u %18s\n", p_unit_info->name,
                       urx, lrx, "N/A") ;
            } else {
              CS_PRINT("\t%-35s Bytes :  %18u %18s\n", p_unit_info->name,
                       lrx, "N/A") ;
            }
          }
    }

  }

  /* Host Packet Drop cam STATs special handler */
  if ( sect_id == M2R_RMAC_STATS_HOST_DROP_CAM_SECT ) {
  	/* p_unit_info->rx_reg_ofst indicates entery number */
  	/* p_unit_info->stride indicates channel number */
  	entry_num = p_unit_info->rx_reg_ofst ;
  	channel_id = p_unit_info->stride ;

  	/* update STATs first */
  	m2r_rmac_stats_spl_update_unit_data(port_id, sect_id, unit_id);

  	if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
  	  if (p_unit_data->tx || p_unit_data->rx) {
  	    /* dump CAM entry */
  	    if (entry_num < M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES) {
              CS_PRINT("HOST Channel %d Pkt Drop CAM Entry %d =>\n",
                       channel_id, entry_num) ;
              m2r_rmac_dump_host_drop_cam_entries(port_id, entry_num, 1) ;
  	    } else {
              CS_PRINT("HOST Channel %d Pkt Drop No CAM Hit =>\n",
                       channel_id) ;
  	    }

            if (m2r_drvr_is_stats_in_hex_fmt() == TRUE ) {
              urx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->rx) ;
              lrx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->rx) ;
              utx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->tx) ;
              ltx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->tx) ;

              if (utx>0) {
                CS_PRINT("\t%-36s Pkts :  %8x%08xH %18s\n", p_unit_info->name,
                         utx, ltx, "N/A") ;
              } else {
                CS_PRINT("\t%-36s Pkts :  %16xH %18s\n", p_unit_info->name,
                         ltx, "N/A") ;
              }

              if (urx>0) {
                CS_PRINT("\t%-35s Bytes :  %8x%08xH %18s\n", p_unit_info->name,
                         urx, lrx, "N/A") ;
              } else {
                CS_PRINT("\t%-35s Bytes :  %16xH %18s\n", p_unit_info->name,
                         lrx, "N/A") ;
              }
            } else {
              urx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->rx) ;
              lrx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->rx) ;
              utx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->tx) ;
              ltx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->tx) ;

              if (utx>0) {
                CS_PRINT("\t%-36s Pkts :  %9u%09u %18s\n", p_unit_info->name,
                         utx, ltx, "N/A") ;
              } else {
                CS_PRINT("\t%-36s Pkts :  %18u %18s\n", p_unit_info->name,
                         ltx, "N/A") ;
              }

              if (urx>0) {
                CS_PRINT("\t%-35s Bytes :  %9u%09u %18s\n", p_unit_info->name,
                         urx, lrx, "N/A") ;
              } else {
                CS_PRINT("\t%-35s Bytes :  %18u %18s\n", p_unit_info->name,
                         lrx, "N/A") ;
              }
            }
  	  }
    } else {
  	  /* dump CAM entry */
  	  if (entry_num < M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES) {
            CS_PRINT("HOST Channel %d Pkt Drop CAM Entry %d =>\n",
                     channel_id, entry_num) ;
            m2r_rmac_dump_host_drop_cam_entries(port_id, entry_num, 1) ;
  	  } else {
            CS_PRINT("HOST Channel %d Pkt Drop No CAM Hit =>\n",
                     channel_id) ;
  	  }

          if (m2r_drvr_is_stats_in_hex_fmt() == TRUE ) {
            urx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->rx) ;
            lrx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->rx) ;
            utx = M2R_STATS_GET_UPPER_32BIT_HEX(p_unit_data->tx) ;
            ltx = M2R_STATS_GET_LOWER_32BIT_HEX(p_unit_data->tx) ;

            if (utx>0) {
              CS_PRINT("\t%-36s Pkts :  %8x%08xH %18s\n", p_unit_info->name,
                       utx, ltx, "N/A") ;
            } else {
              CS_PRINT("\t%-36s Pkts :  %16xH %18s\n", p_unit_info->name,
                       ltx, "N/A") ;
            }

            if (urx>0) {
              CS_PRINT("\t%-35s Bytes :  %8x%08xH %18s\n", p_unit_info->name,
                       urx, lrx, "N/A") ;
            } else {
              CS_PRINT("\t%-35s Bytes :  %16xH %18s\n", p_unit_info->name,
                       lrx, "N/A") ;
            }
          } else {
            urx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->rx) ;
            lrx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->rx) ;
            utx = M2R_STATS_GET_BILLIONS_DEC(p_unit_data->tx) ;
            ltx = M2R_STATS_GET_BILLIONS_MOD_DEC(p_unit_data->tx) ;

            if (utx>0) {
              CS_PRINT("\t%-36s Pkts :  %9u%09u %18s\n", p_unit_info->name,
                       utx, ltx, "N/A") ;
            } else {
              CS_PRINT("\t%-36s Pkts :  %18u %18s\n", p_unit_info->name,
                       ltx, "N/A") ;
            }

            if (urx>0) {
              CS_PRINT("\t%-35s Bytes :  %9u%09u %18s\n", p_unit_info->name,
                       urx, lrx, "N/A") ;
            } else {
              CS_PRINT("\t%-35s Bytes :  %18u %18s\n", p_unit_info->name,
                       lrx, "N/A") ;
            }
          }
    }

  }
}

void m2r_rmac_stats_spl_clear_unit_data(cs_uint16 port_id,
                                  cs_uint16 sect_id,
                                  cs_uint16 unit_id)
{
  /* This API is only called internally, before it is called, all
     the sanity checks have passed already */
  m2r_port_stats_cb_t       * stats_port_cb ;
  m2r_stats_blk_cb_t        * stats_blk_cb ;
  m2r_stats_sect_cb_t       * stats_sect_cb ;
  const m2r_stats_info_t    * p_unit_info ;
  m2r_stats_data_t          * p_unit_data ;
  cs_uint16                 sl_num ;
  M2R_t                     * pDev ;
  DR_TB_HTB_DPERR_t         htb_dperr ;
  DR_TB_LTB_DPERR_t         ltb_dperr ;
  m2r_rmac_cntr_t           rmac_cntr ;
  cs_uint16                 entry_num, channel_id ;

  stats_port_cb = M2R_GET_STATS_CB(port_id) ;
  stats_blk_cb = stats_port_cb->rmac ;
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;

  p_unit_info = stats_sect_cb->info_tbl + unit_id ;
  p_unit_data = stats_sect_cb->pStats + unit_id ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* TB-General special STATs handler */
  if ( sect_id == M2R_RMAC_STATS_TB_GEN_SECT ) {
  	switch (unit_id) {
  	  case M2R_RMAC_STATS_TB_HI_ECCFIX :
  	  	htb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.HTB_DPERR.wrd) ;
  	  	p_unit_data->rx = 0 ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_HI_ECCERR;
  	  	p_unit_data->rx += htb_dperr.bf.ECCERR ;
  	  	break;
  	  case M2R_RMAC_STATS_TB_HI_ECCERR :
  	  	htb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.HTB_DPERR.wrd) ;
  	  	p_unit_data->rx = 0 ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_HI_ECCFIX;
  	  	p_unit_data->rx += htb_dperr.bf.ECCFIX ;
  	  	break;
  	  case M2R_RMAC_STATS_TB_LO_ECCFIX :
  	  	ltb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.LTB_DPERR.wrd) ;
  	  	p_unit_data->rx = 0 ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_LO_ECCERR;
  	  	p_unit_data->rx += ltb_dperr.bf.ECCERR ;
  	  	break;
  	  case M2R_RMAC_STATS_TB_LO_ECCERR :
  	  	ltb_dperr.wrd
  	  	  = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.LTB_DPERR.wrd) ;
  	  	p_unit_data->rx = 0 ;
  	  	p_unit_data = stats_sect_cb->pStats + M2R_RMAC_STATS_TB_LO_ECCFIX;
  	  	p_unit_data->rx += ltb_dperr.bf.ECCFIX ;
  	  	break;
  	}
  }

  /* TB-PM special STATs handler */
  if ( sect_id == M2R_RMAC_STATS_TB_PM_SECT ) {
    switch (unit_id) {
      case M2R_RMAC_STATS_TB_HI_DEL_AVG:
      case M2R_RMAC_STATS_TB_LO_DEL_AVG:
        p_unit_data->tx = 0 ;
        break;
    }
  }

  /* Multicast coarse cam STATs special handler */
  if ( sect_id == M2R_RMAC_STATS_MCOARSE_CAM_SECT ) {
  	/* p_unit_info->tx_reg_ofst/rx_reg_ofst indicates entery number */
  	entry_num = p_unit_info->rx_reg_ofst ;

  	/* Clear HW counters */
  	m2r_rmac_get_coarse_cntr(port_id, entry_num, &rmac_cntr) ;

    /* Clear Pkt counter */
    p_unit_data->tx = 0 ;
    /* Clear Byte counter */
    p_unit_data->rx = 0 ;
  }

  /* Host Packet Drop cam STATs special handler */
  if ( sect_id == M2R_RMAC_STATS_HOST_DROP_CAM_SECT ) {
  	/* p_unit_info->rx_reg_ofst indicates entery number */
  	/* p_unit_info->stride indicates channel number */
  	entry_num = p_unit_info->rx_reg_ofst ;
  	channel_id = p_unit_info->stride ;

  	/* Clear HW counters */
  	if (entry_num < M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES) {
  	  /* STATs for hit*/
  	  m2r_rmac_get_host_drop_channel_cntr(port_id, TRUE, channel_id,
                                              entry_num, &rmac_cntr) ;
  	} else {
  	  /* STATs for no hit*/
  	  m2r_rmac_get_host_drop_channel_cntr(port_id, FALSE, channel_id,
                                              entry_num, &rmac_cntr) ;
  	}

    /* Clear Pkt counter */
    p_unit_data->tx = 0 ;
    /* Clear Byte counter */
    p_unit_data->rx = 0 ;
  }
}

/************************************************************************/
/* $rtn_hdr_start     CLEAR MULTICAST COARSE CAM STATS                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : public                                                  */
/* BLOCK      : RMAC                                                    */
/* CHIP       : Both                                                    */
cs_status m2r_rmac_stats_mcoarse_cam_clear_units(cs_uint16 port_id,
                                            cs_uint32 start,
                                            cs_uint32 num_entries)
/* INPUTS     : o Port Id                                               */
/*              o First entry                                           */
/*              o Num of entries                                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clear the multicast coarse cam stats. It clears the hardware         */
/* multicast coarse cam counters and also the internal stats structure  */
/* which accumulate the stats for specified cam entries.                */
/*                                                                      */
/* Note: When the application resets the multicast coarse cam entries or*/
/* add the new entries, it should call this API to reset the            */
/* corresponding STATs.                                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32 i ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  for (i = start; i < (start + num_entries); i++) {
  	m2r_rmac_stats_spl_clear_unit_data(port_id, M2R_RMAC_STATS_MCOARSE_CAM_SECT, i) ;
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start         CLEAR HOST PACKET DROP CAM STATS              */
/* CATEGORY   : Port                                                    */
/* ACCESS     : public                                                  */
/* BLOCK      : RMAC                                                    */
/* CHIP       : Both                                                    */
cs_status m2r_rmac_stats_host_drop_cam_clear_units(cs_uint16 port_id,
                                              cs_uint32 start,
                                              cs_uint32 num_entries)
/* INPUTS     : o Port Id                                               */
/*              o First entry                                           */
/*              o Num of entries                                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clear the host packet drop cam stats. It clears the hardware host pkt*/
/* drop cam counters and also the internal stats structure which        */
/* accumulate the stats for specified cam entries.                      */
/*                                                                      */
/* Note: When the application resets the host packet drop cam entries or*/
/* add the new entries, it should call this API to reset the            */
/* corresponding STATs.                                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32 i, j ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start, num_entries,
			    M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES,
			    __FUNCTION__, __LINE__)
	== CS_ERROR ) {
    return (CS_ERROR) ;
  }

  for (j = 0; j < M2R_RMAC_HOST_MAX_NUM_OF_CHANNEL; j++) {
    for (i = start; i < (start + num_entries); i++) {
  	  m2r_rmac_stats_spl_clear_unit_data(port_id,
			M2R_RMAC_STATS_HOST_DROP_CAM_SECT,
  	                (j * M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES) + i) ;
    }
  }

  return (CS_OK) ;
}

/***************************************************************
 *  Utility to register and add the section CB to the block CB *
 ***************************************************************/
static
cs_status m2r_rmac_register_sect_stats(cs_uint16 port_id,
				       cs_uint16 sect_id,
				       cs_boolean sect_enbl)
{
  m2r_stats_data_t   * p_data ;
  m2r_port_stats_t   * p_port_stats ;
  m2r_rmac_stats_t   * p_rmac_stats ;
  const m2r_stats_sect_tbl_entry_t * tbl = NULL ;

  p_port_stats = M2R_GET_STATS_PTR(port_id) ;
  if (p_port_stats == NULL) {
    CS_PRINT("port-%d p_port_stats is NULL\n", port_id) ;
    return (CS_ERROR) ;
  }
  p_rmac_stats = &p_port_stats->rmac ;

  tbl = &rmac_stats_sect_tbl[sect_id] ;
  p_data = (m2r_stats_data_t *)
                (((cs_uint64)p_rmac_stats) + tbl->sect_offset) ;

  if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_RMAC,
                                    tbl->sect_name, sect_id,
                                    tbl->stride_id, sect_enbl,
                                    tbl->num_unit,
                                    p_data, tbl->info_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   INIT RINGMAC STATS                          */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status  m2r_rmac_init_stats(cs_uint16 port_id,
			       m2r_port_mac_mode_t mac_mode)
/* INPUTS     : o Port Id                                       */
/*              o MAC mode(SRP or RPR)                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initialize RingMAC statistics. This API will register the    */
/* RingMAC block and its stats control tables with the port     */
/* stats.                                                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_stats_blk_cb_t    * p_blk_cb ;
  cs_boolean            enbl ;
  cs_uint16		ii ;

  /* Register RingMAC block */
  p_blk_cb = m2r_stats_register_blk_cb(port_id,
                                       M2R_ID_RMAC,
                                       TRUE,    /* ctl_enabled */
                                       m2r_rmac_stats_spl_update_unit_data, /* pGet_func   */
                                       m2r_rmac_stats_spl_print_unit_cntr,  /* pPrint_func */
                                       m2r_rmac_stats_spl_clear_unit_data) ;/* pClear_func */
  if (p_blk_cb == NULL) {
    CS_PRINT("port-%d: RingMAC block NOT registered!\n", port_id) ;
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < M2R_RMAC_STATS_SECT_ALL; ii++) {
    enbl = TRUE ;
    if ( m2r_rmac_register_sect_stats(port_id, ii, enbl)
       == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}

