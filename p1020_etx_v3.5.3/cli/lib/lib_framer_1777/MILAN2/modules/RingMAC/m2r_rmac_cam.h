/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systgems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_cam.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * Header file for CAM, SRAM memories.
 */
#ifndef __M2R_RMAC_CAM_H__
#define __M2R_RMAC_CAM_H__

#include "cs_types.h"
#include "m2r_public.h"

/*
 * Enum's for various memories in RingMAC.
 * Do NOT use the enum values directly to write to registers as
 * they are not the same! Use the mapping utility to get the
 * value to be written.
 */
typedef enum {
  M2R_RX_CAM                 = 0,    /* or Lookup CAM */
  M2R_RX_STATUS_SRAM,
  M2R_RX_ACCTG_SA_SRAM,	             /* bank-0  */
  M2R_RX_ACCTG_DA_SRAM,              /* bank-1  */
  M2R_TX_CAM,                        /* Stat CAM */
  M2R_TX_STATUS_SRAM	    = 5,
  M2R_TX_ACCTG_SA_SRAM,             /* bank-0  */
  M2R_TX_ACCTG_DA_SRAM,             /* bank-1  */
  M2R_MS_DATA_STORE,
  M2R_MS_CORE_STORE,
  M2R_MS_PGM_STORE	    = 10,
  M2R_MCAST_COARSE_CAM,
  M2R_MCAST_COARSE_CNTRS,
  M2R_MAC_REWRITE_SRAM,
  M2R_MS_MCHOKE_SRAM,
  M2R_HOST_PKT_DROP_CAM    = 15,
  M2R_HOST_PKT_DROP_CNTRS
} m2r_rmac_mem_id_t ;

#define M2R_NUM_OF_ADDR_CAM_ENTRIES            1024
#define M2R_ADDR_CAM_SA_START                     0
#define M2R_ADDR_CAM_SA_NUM_ENTRIES             256
#define M2R_ADDR_CAM_DA_ONLY_NUM_ENTRIES  (M2R_NUM_OF_ADDR_CAM_ENTRIES - \
                                           M2R_ADDR_CAM_SA_NUM_ENTRIES)

typedef struct {
  cs_boolean    da ;    /* The MAC addr is DA */
  cs_boolean    sa ;    /* The MAC addr is SA */
  cs_mac        mac ;
} m2r_cam_macaddr_t ;

/*  Lookup(Rx) cam's status SRAM entry */
typedef struct {
  cs_boolean    sr ;        /* SA Accept or Reject (dep on SAA mode) */
  cs_boolean    ne ;        /* 1: SA = This node's address */
  cs_boolean    sam ;       /* 1: SA monitoring for accounting */
  cs_boolean    dah ;       /* 1: DA =  Host address */
  cs_uint8	hop_count ; /* num of hops from SA */
} m2r_rx_cam_status_t ;

/* Lookup(Rx) CAM entry */
typedef struct {
  m2r_cam_macaddr_t     macaddr ;
  m2r_rx_cam_status_t   status ;
  cs_boolean            prsrv_da_cntr ;  /* don't clear DA acctg sram */
  cs_boolean            prsrv_sa_cntr ;  /* don't clear SA acctg sram */
} m2r_rx_cam_entry_t ;

/*  Stat(Tx) cam's status SRAM entry */
typedef struct {
  cs_uint8	max_active ;
  cs_boolean	da_mode_tb ;   /* on DA match, count pkts from TB */
  cs_boolean	da_mode_host ; /* on DA match, count pkts from Host */
  cs_boolean	sah ;          /* on SA match, count pkts from Host */
  cs_uint8	weight ;
} m2r_tx_cam_status_t ;

/* Stat(Tx) CAM entry */
typedef struct {
  m2r_cam_macaddr_t    macaddr ;
  m2r_tx_cam_status_t  status ;
  cs_boolean           prsrv_sa_cntr ;
  cs_boolean           prsrv_da_cntr ;
} m2r_tx_cam_entry_t ;


#define M2R_NUM_OF_MCAST_COARSE_CAM_ENTRIES	   8
#define M2R_NUM_OF_MAC_REWRITE_SRAM_ENTRIES	1024
#define M2R_NUM_OF_MCHOKE_CAM_ENTRIES            512
#define M2R_NUM_OF_MS_DATA_STORE_ENTRIES          64
#define M2R_NUM_OF_MS_CORE_STORE_ENTRIES           6
#define M2R_NUM_OF_MS_PGM_STORE_ENTRIES          512

/* Multicast Coarse Filter CAM */
typedef struct {
  cs_mac     key ;
  cs_mac     mask ;
  cs_boolean accept ; /* accept or reject */
  cs_boolean valid ;
} m2r_rmac_mcast_coarse_cam_t ;

#define M2R_NUM_OF_HOST_PKT_DROP_CAM_ENTRIES	16
#define M2R_NUM_OF_HOST_PKT_DROP_CNTR_ENTRIES	69
#define M2R_HOST_PKT_DROP_CAM_NOHIT_CNTR_BASE_ENTRY 64
#define M2R_HOST_PKT_DROP_CNTR_SAT_ENTRY        68

/* Host Pkt Drop Coarse Filter CAM */
typedef struct {
  cs_mac     key ;
  cs_mac     mask ;
  cs_boolean valid ;
} m2r_rmac_host_pkt_drop_cam_t ;

typedef struct {
  cs_uint64   pkts ;
  cs_uint64   bytes ;
} m2r_rmac_cntr_t ;

/* Accounting SRAM packet statistics */
typedef struct {
  m2r_rmac_cntr_t  rx_sa[ M2R_ADDR_CAM_SA_NUM_ENTRIES ] ;
  m2r_rmac_cntr_t  rx_da[ M2R_NUM_OF_ADDR_CAM_ENTRIES ] ;
  m2r_rmac_cntr_t  tx_sa[ M2R_ADDR_CAM_SA_NUM_ENTRIES ] ;
  m2r_rmac_cntr_t  tx_da[ M2R_NUM_OF_ADDR_CAM_ENTRIES ] ;
} m2r_rmac_acctg_stats_t ;


extern
cs_status m2r_rmac_init_mem(cs_uint16 port_id) ;
extern
cs_status m2r_rmac_reset_mem(cs_uint16 port_id,
			     m2r_rmac_mem_id_t mem_id,
		     cs_uint32 start, cs_uint32 num_entries) ;
extern
cs_status  m2r_add_rx_cam_entry(cs_uint16 port_id, cs_uint32 entry_num,
		cs_uint8 da, cs_uint8 sa, cs_uint16 mac5_4,
		cs_uint16 mac3_2, cs_uint16 mac1_0,
		cs_boolean sr, cs_boolean ne, cs_boolean sam,
		cs_boolean dah, cs_uint8 hop_count,
		cs_boolean prsrv_da_cntr, cs_boolean prsrv_sa_cntr) ;
extern
cs_status  m2r_add_rx_cam_entries(cs_uint16 port_id, cs_uint32 start,
			       cs_uint32 num, m2r_rx_cam_entry_t * tbl) ;

extern
cs_status  m2r_add_tx_cam_entry(cs_uint16 port_id, cs_uint32 entry_num,
		cs_uint8 da, cs_uint8 sa, cs_uint16 mac5_4,
		cs_uint16 mac3_2, cs_uint16 mac1_0,
		cs_uint8 max_active, cs_boolean da_mode_tb,
		cs_boolean da_mode_host, cs_boolean sah,
		cs_uint8 weight,
		cs_boolean prsrv_da_cntr, cs_boolean prsrv_sa_cntr) ;
extern
cs_status  m2r_add_tx_cam_entries(cs_uint16 port_id, cs_uint32 start,
			   cs_uint32 num, m2r_tx_cam_entry_t * tbl) ;
extern
cs_uint32  m2r_rmac_probe_cam_entry(cs_uint16 port_id,
				    m2r_rmac_mem_id_t cam_id,
				    m2r_cam_macaddr_t * pmac_mask,
				    m2r_cam_macaddr_t * pmac_addr) ;
extern
cs_uint32  m2r_rmac_debug_probe_cam_entry(cs_uint16 port_id,
	       m2r_rmac_mem_id_t cam_id, cs_uint16 mask_dasa,
               cs_uint16 mask5_4, cs_uint16 mask3_2, cs_uint16 mask1_0,
	       cs_uint16 mac_dasa,
	       cs_uint16 mac5_4, cs_uint16 mac3_2, cs_uint16 mac1_0) ;
extern
cs_status  m2r_rmac_dump_cam_entries(cs_uint16 port_id,
				     m2r_rmac_mem_id_t cam_id,
				     cs_uint16 start, cs_uint16 num) ;
/************* Multi-cast Coarse CAM ********************/
extern
cs_uint32  m2r_rmac_add_mcast_coarse_cam_entries(cs_uint16 port_id,
                                cs_uint8 start, cs_uint8 num,
				m2r_rmac_mcast_coarse_cam_t * tbl) ;
extern
cs_uint32  m2r_rmac_add_mcast_coarse_cam_entry(cs_uint16 port_id,
               cs_uint8 entry_num,
               cs_uint16 key5_4, cs_uint16 key3_2, cs_uint16 key1_0,
               cs_uint16 mask5_4, cs_uint16 mask3_2, cs_uint16 mask1_0,
	       cs_boolean accept, cs_boolean valid) ;
extern
cs_status  m2r_rmac_get_mcast_coarse_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num,
				   m2r_rmac_mcast_coarse_cam_t * p) ;
extern
cs_status  m2r_rmac_dump_mcast_coarse_cam_entries(cs_uint16 port_id,
						  cs_uint16 start,
						  cs_uint16 num) ;
/************* Host Pkt Drop Coarse CAM ********************/
extern
cs_uint32  m2r_rmac_add_host_drop_cam_entries(cs_uint16 port_id,
			        cs_uint8 start, cs_uint8 num,
			        m2r_rmac_host_pkt_drop_cam_t * tbl) ;
extern
cs_uint32  m2r_rmac_add_host_drop_cam_entry(cs_uint16 port_id,
	       cs_uint8 entry_num,
               cs_uint16 key5_4, cs_uint16 key3_2, cs_uint16 key1_0,
               cs_uint16 mask5_4, cs_uint16 mask3_2, cs_uint16 mask1_0,
               cs_boolean valid) ;
extern
cs_status  m2r_rmac_get_host_drop_cam_entry(cs_uint16 port_id,
					       cs_uint16 entry_num,
				    m2r_rmac_host_pkt_drop_cam_t * p) ;
extern
cs_status  m2r_rmac_dump_host_drop_cam_entries(cs_uint16 port_id,
						 cs_uint16 start,
						 cs_uint16 num) ;

/* Accounting Statistics (lookup and stat SRAM's) for SA/DA */
extern
cs_status  m2r_rmac_get_acctg_stats(cs_uint16 port_id,
			       m2r_rmac_mem_id_t sram_id,
			       cs_uint16 start, cs_uint16 num,
			       m2r_rmac_cntr_t * p_user_stats) ;
extern
cs_status  m2r_rmac_print_acctg_stats(cs_uint16 port_id,
				      m2r_rmac_mem_id_t sram_id,
				      cs_uint16 start,
				      cs_uint16 num_entries) ;
extern
cs_status  m2r_rmac_clear_acctg_stats(cs_uint16 port_id,
				      m2r_rmac_mem_id_t sram_id,
				      cs_uint16 start,
				      cs_uint16 num_entries) ;
extern
cs_status  m2r_rmac_load_acctg_stats(cs_uint16 port_id,
                                     m2r_rmac_mem_id_t sram_id,
                                     cs_uint16 start, cs_uint16 num,
                                     m2r_rmac_cntr_t * p_user_stats) ;


#endif /* __M2R_RMAC_CAM_H__ */
