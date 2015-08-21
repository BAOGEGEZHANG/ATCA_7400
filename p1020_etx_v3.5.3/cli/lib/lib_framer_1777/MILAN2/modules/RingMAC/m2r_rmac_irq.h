/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_irq.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_RMAC_IRQ_H__
#define __M2R_RMAC_IRQ_H__

#include "cs_types.h"

/*****************************************************************
 * RingMAC Top IRQ Map defines
 *****************************************************************/
/* Bit fields for M2R_RMAC_MICRO_IF_IRQMAP */
#define M2R_RMAC_MICRO_IF_IRQMAP_IRQ0			(0x0001)
#define M2R_RMAC_MICRO_IF_IRQMAP_IRQ1			(0x0002)
#define M2R_RMAC_MICRO_IF_IRQMAP_IRQ2			(0x0004)
#define M2R_RMAC_MICRO_IF_IRQMAP_IRQ3			(0x0008)
#define M2R_RMAC_MICRO_IF_IRQMAP_IRQ4			(0x0010)
#define M2R_RMAC_MICRO_IF_IRQMAP_IRQ5			(0x0020)

/*****************************************************************
 * RingMAC IRQ leaf defines
 *****************************************************************/
/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ0 */
#define M2R_RMAC_MICRO_IF_IRQ0_RXH_OVERFLOW	      	(0x8000)
#define M2R_RMAC_MICRO_IF_IRQ0_RXH_GIANT       		(0x4000)
#define M2R_RMAC_MICRO_IF_IRQ0_RXH_PKTDROP	     	(0x2000)
#define M2R_RMAC_MICRO_IF_IRQ0_RXH_DROPSAT	      	(0x1000)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_FIFO_OVFL   		(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_CTRL_PERR   		(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_SRP_PERR    		(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_SHRT_PKT    		(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_PKT_EMPTY   		(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_PKT_GIANT   		(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_RUNT        		(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_ERR         		(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ0_TXH_DPERR       		(0x0001)
#define M2R_GRP_RMAC_MICRO_IF_IRQ0_ALL			(0xF5FD)
#define M2R_GRP_RMAC_MICRO_IF_IRQ0_SPL	         	(0x1000)

/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ1 */
#define M2R_RMAC_MICRO_IF_IRQ1_RX_TRAF           	(0x8000)
#define M2R_RMAC_MICRO_IF_IRQ1_RX_TIMER           	(0x4000)
#define M2R_RMAC_MICRO_IF_IRQ1_RX_CRC_ERR      		(0x2000)
#define M2R_RMAC_MICRO_IF_IRQ1_RX_HEC_ERR      		(0x1000)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_IDLE_SA     		(0x0800)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_FIFO_OVFL   		(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_CTRL_PERR   		(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_SRP_PERR    		(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_SHRT_PKT    		(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_PKT_EMPTY   		(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_PKT_GIANT   		(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_RUNT        		(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_ERR         		(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ1_RXF_DPERR       		(0x0001)
#define M2R_GRP_RMAC_MICRO_IF_IRQ1_ALL           	(0xFDFD)

/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ2 */
#define M2R_RMAC_MICRO_IF_IRQ2_LKPCAM_MULTIHIT		(0x8000)
#define M2R_RMAC_MICRO_IF_IRQ2_STATCAM_MULTIHIT		(0x4000)
#define M2R_RMAC_MICRO_IF_IRQ2_USEQ_FAIR         	(0x2000)
#define M2R_RMAC_MICRO_IF_IRQ2_BIST_ERR           	(0x1000)
#define M2R_RMAC_MICRO_IF_IRQ2_MAC_PERR           	(0x0800)
#define M2R_RMAC_MICRO_IF_IRQ2_HW_RESET           	(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT           	(0x0200)
#define M2R_RMAC_MICRO_IF_IRQ2_WDOG               	(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ2_WPASS              	(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ2_FPASS              	(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ2_SPSAT              	(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ2_SBSAT              	(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ2_LPSAT              	(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ2_LBSAT              	(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ2_TTLS               	(0x0002)
#define M2R_RMAC_MICRO_IF_IRQ2_WR2_LOCK           	(0x0001)
#define M2R_GRP_RMAC_MICRO_IF_IRQ2_ALL	         	(0xFFFF)
#define M2R_GRP_RMAC_MICRO_IF_IRQ2_SPL	         	(0x0200)

/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ3 */
#define M2R_RMAC_MICRO_IF_IRQ3_IPS_RI_TTL_MAX		(0x8000)
#define M2R_RMAC_MICRO_IF_IRQ3_EXTTB_DPERR        	(0x4000)
#define M2R_RMAC_MICRO_IF_IRQ3_EXTTB_OVFL         	(0x2000)
#define M2R_RMAC_MICRO_IF_IRQ3_PTQ_KILL_DONE     	(0x1000)
#define M2R_RMAC_MICRO_IF_IRQ3_STQ_KILL_DONE      	(0x0800)
#define M2R_RMAC_MICRO_IF_IRQ3_TBD_IPS            	(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ3_RING_ERR           	(0x0200)
#define M2R_RMAC_MICRO_IF_IRQ3_LTBO               	(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ3_HTBO               	(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ3_TBD_RUNT_USG       	(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ3_LTB_DPERR          	(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ3_HTB_DPERR          	(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ3_CTL_RCV_INT        	(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ3_CTL_SND_INT        	(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ3_IPS_RCV_INT        	(0x0002)
#define M2R_RMAC_MICRO_IF_IRQ3_IPS_SND_INT        	(0x0001)
#define M2R_GRP_RMAC_MICRO_IF_IRQ3_ALL            	(0xFFFF)

/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ4 */
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_HEC_ERR        	(0x4000)
#define M2R_RMAC_MICRO_IF_IRQ4_TOM_UNDERFLOW      	(0x2000)
#define M2R_RMAC_MICRO_IF_IRQ4_MATE_WRAP          	(0x1000)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_CTRL_SQNC_ERR  	(0x0800)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_OVERFLOW       	(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_UNDERFLOW      	(0x0200)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_SERDES         	(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_CRC_ERR        	(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_CRC_ERR_SAT    	(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_GIANT          	(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_GIANT_SAT      	(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_LOS            	(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_LOS_SAT        	(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_RUNT           	(0x0002)
#define M2R_RMAC_MICRO_IF_IRQ4_FRM_RUNT_SAT       	(0x0001)
#define M2R_GRP_RMAC_MICRO_IF_IRQ4_ALL            	(0x7FFF)

/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ5 */
#define M2R_RMAC_MICRO_IF_IRQ5_TBD_WE             	(0x0800)
#define M2R_RMAC_MICRO_IF_IRQ5_TBD_NOIPS          	(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ5_RXF_MCASTSA        	(0x0200)
#define M2R_RMAC_MICRO_IF_IRQ5_DQTA_TO            	(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1            	(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0            	(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ5_NEW_TTL_CONGEST    	(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ5_OLD_TSN_FLUSHED   	(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ5_MAC_PKTLEN_ERR     	(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ5_TBH_ECC            	(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ5_TBL_ECC            	(0x0002)
#define M2R_RMAC_MICRO_IF_IRQ5_EXTTB_ECC          	(0x0001)
#define M2R_GRP_RMAC_MICRO_IF_IRQ5_ALL           	(0x0FFF)
#define M2R_GRP_RMAC_MICRO_IF_IRQ5_SPL	         	(0x00C0)

/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ5_MEMPAR1 */
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_MATE_FIFO	(0x8000)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_USEQ_IRAM 	(0x4000)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_MAC_REW_F	(0x2000)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_MAC_REW_E  	(0x1000)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_USEQ_MCHK_F	(0x0800)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_USEQ_MCHK_E	(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_STAT_ACCT_F	(0x0200)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_STAT_ACCT_E	(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_LKP_ACCT_F	(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_LKP_ACCT_E	(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_CTL_RCV_F 	(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_CTL_RCV_E  	(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_CTL_SEND_F 	(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_CTL_SEND_E 	(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_IPS_RCV_F  	(0x0002)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_IPS_RCV_E  	(0x0001)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_ALL          (0xFFFF)

/* Bit fields for M2R_GRP_RMAC_MICRO_IF_IRQ5_MEMPAR0 */
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_STAT_STAT   	(0x2000)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_LKP_STAT    	(0x1000)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_TXH_SYNC    	(0x0800)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_RXF_SYNC    	(0x0400)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_TB_STQ      	(0x0200)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_TB_PTQ      	(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_MATE_TOMSYNC	(0x0080)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_MATE_TOMIN	(0x0040)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_MATE_FRMSYNC	(0x0020)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_MATE_FRMSPI 	(0x0010)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_RXH_DRPCNT  	(0x0008)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_TXH_RUNT  	(0x0004)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_RXF_RUNT   	(0x0002)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_IPS_SEND    	(0x0001)
#define M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_ALL          (0x3FFF)

/* Bit fields for USEQ_INT */
#define M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_MS_TIMEOUT	(0x0200)
#define M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_MS_INST_PAR_ERR	(0x0100)
#define M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_IAMTAIL		(0x0001)
#define M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_KEEPALIVE_FAIL	(0x0002)
#define M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_NEW_TTL_TO_CONGEST	(0x0004)

typedef enum {
  M2R_GRP_RMAC_MICRO_IF_IRQ0	= 0,
  M2R_GRP_RMAC_MICRO_IF_IRQ1,
  M2R_GRP_RMAC_MICRO_IF_IRQ2,
  M2R_GRP_RMAC_MICRO_IF_IRQ3,
  M2R_GRP_RMAC_MICRO_IF_IRQ4,
  M2R_GRP_RMAC_MICRO_IF_IRQ5,
  
  M2R_RMAC_MAX_GRP
} m2r_rmac_irq_grp_t ;

/***********************************************************************
 * RMAC XGXS IRQ LEAF DEFINITIONS
 ***********************************************************************/
#define M2R_XGXS_MP_MnInt0_LckDet3I    (0x0800)
#define M2R_XGXS_MP_MnInt0_LckDet2I    (0x0400)
#define M2R_XGXS_MP_MnInt0_LckDet1I    (0x0200)
#define M2R_XGXS_MP_MnInt0_LckDet0I    (0x0100)
#define M2R_XGXS_MP_MnInt0_DatDet3I    (0x0080)
#define M2R_XGXS_MP_MnInt0_DatDet2I    (0x0040)
#define M2R_XGXS_MP_MnInt0_DatDet1I    (0x0020)
#define M2R_XGXS_MP_MnInt0_DatDet0I    (0x0010)
#define M2R_XGXS_MP_MnInt0_NoClk3I     (0x0008)
#define M2R_XGXS_MP_MnInt0_NoClk2I     (0x0004)
#define M2R_XGXS_MP_MnInt0_NoClk1I     (0x0002)
#define M2R_XGXS_MP_MnInt0_NoClk0I     (0x0001)
#define M2R_GRP_XGXS_MP_MnInt0_ALL     (0x0fff)

#define M2R_XGXS_MP_MnInt1_RxNoClkI    (0x0200)
#define M2R_XGXS_MP_MnInt1_TxNoClkI    (0x0100)
#define M2R_XGXS_MP_MnInt1_RxPCPAI     (0x0080)
#define M2R_XGXS_MP_MnInt1_RxLBPAI     (0x0040)
#define M2R_XGXS_MP_MnInt1_TxLnPAI     (0x0020)
#define M2R_XGXS_MP_MnInt1_TxLBPAI     (0x0010)
#define M2R_XGXS_MP_MnInt1_RxL3PAI     (0x0008)
#define M2R_XGXS_MP_MnInt1_RxL2PAI     (0x0004)
#define M2R_XGXS_MP_MnInt1_RxL1PAI     (0x0002)
#define M2R_XGXS_MP_MnInt1_RxL0PAI     (0x0001)
#define M2R_GRP_XGXS_MP_MnInt1_ALL     (0x03ff)

#define M2R_XGXS_TX_TxInt_GoodPktSatI  (0x0004)
#define M2R_XGXS_TX_TxInt_ErrorPktSatI (0x0002)
#define M2R_XGXS_TX_TxInt_ErrorPktI    (0x0001)
#define M2R_GRP_XGXS_TX_TxInt_ALL       (0x0007)

#define M2R_XGXS_RX_RxInt_PatLockDetI   (0x2000)
#define M2R_XGXS_RX_RxInt_AlignDetI     (0x1000)
#define M2R_XGXS_RX_RxInt_SyncDet3I     (0x0800)
#define M2R_XGXS_RX_RxInt_SyncDet2I     (0x0400)
#define M2R_XGXS_RX_RxInt_SyncDet1I     (0x0200)
#define M2R_XGXS_RX_RxInt_SyncDet0I     (0x0100)
#define M2R_XGXS_RX_RxInt_PatErrSatI    (0x0040)
#define M2R_XGXS_RX_RxInt_PatErrI       (0x0020)
#define M2R_XGXS_RX_RxInt_InvCodeSatI   (0x0010)
#define M2R_XGXS_RX_RxInt_InvCodeI      (0x0008)
#define M2R_XGXS_RX_RxInt_GoodPktSatI   (0x0004)
#define M2R_XGXS_RX_RxInt_ErrorPktSatI  (0x0002)
#define M2R_XGXS_RX_RxInt_ErrorPktI     (0x0001)
#define M2R_GRP_XGXS_RX_RxInt_ALL       (0x3f7f)

/************************************************************************
 * XGXS IRQ GROUPS
 ************************************************************************/
typedef enum {
  /* XGXS Groups */
  M2R_GRP_XGXS_MP_MnInt0,
  M2R_GRP_XGXS_MP_MnInt1,
  M2R_GRP_XGXS_TX_TxInt,
  M2R_GRP_XGXS_RX_RxInt,

  M2R_XGXS_MAX_GRP
} m2r_xgxs_irq_grp_t ;

/*****************************************************************
 * RingMAC IRQ API
 *****************************************************************/

extern 
cs_status m2r_rmac_get_default_irq_cfg(cs_uint16 port_id, 
                                              m2r_port_summ_t * pSumm, 
                                              m2r_rmac_cfg_t * pCfg);

extern 
cs_status m2r_rmac_init_irq(cs_uint16 port_id, 
                                   m2r_rmac_cfg_t * pCfg);

extern 
cs_status m2r_rmac_isr(cs_uint16 port_id);

extern
cs_status m2r_rmac_xgxs_isr(cs_uint16 port_id) ;

#endif /* __M2R_RMAC_IRQ_H__ */
