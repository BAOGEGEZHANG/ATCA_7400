/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif_irq.h
 *
 * LIF IRQ Tables and APIs
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#ifndef __M2_LIF_IRQ_H__
#define __M2_LIF_IRQ_H__

/************************************************************************
 * LIF IRQ leaf defines
 ************************************************************************/
/* M2R_GRP_LIF_INTERRUPT1 group individual interrupt bit masks */
#define M2R_LIF_INTERRUPT1_DATA_DT0         (0x1000) /* Data Detect : 0 */
#define M2R_LIF_INTERRUPT1_DATA_DT1         (0x2000) /* Data Detect : 1 */
#define M2R_LIF_INTERRUPT1_DATA_DT2         (0x4000) /* Data Detect : 2 */
#define M2R_LIF_INTERRUPT1_DATA_DT3         (0x8000) /* Data Detect : 3 */
#define M2R_LIF_INTERRUPT1_SYNC0            (0x0100) /* Lane sync : 0   */
#define M2R_LIF_INTERRUPT1_SYNC1            (0x0200) /* Lane sync : 1   */
#define M2R_LIF_INTERRUPT1_SYNC2            (0x0400) /* Lane sync : 2   */
#define M2R_LIF_INTERRUPT1_SYNC3            (0x0800) /* Lane sync : 3   */
#define M2R_LIF_INTERRUPT1_BAD_VCOCLK0      (0x0010) /* Lane VCO Clk : 0*/
#define M2R_LIF_INTERRUPT1_BAD_VCOCLK1      (0x0020) /* Lane VCO Clk : 1*/
#define M2R_LIF_INTERRUPT1_BAD_VCOCLK2      (0x0040) /* Lane VCO Clk : 2*/
#define M2R_LIF_INTERRUPT1_BAD_VCOCLK3      (0x0080) /* Lane VCO Clk : 3*/
#define M2R_LIF_INTERRUPT1_LOCK0            (0x0001) /* Lane Lock : 0   */
#define M2R_LIF_INTERRUPT1_LOCK1            (0x0002) /* Lane Lock : 1   */
#define M2R_LIF_INTERRUPT1_LOCK2            (0x0004) /* Lane Lock : 2   */
#define M2R_LIF_INTERRUPT1_LOCK3            (0x0008) /* Lane Lock : 3   */
#define M2R_GRP_LIF_INTERRUPT1_ALL          (0xffff)

/* M2R_GRP_LIF_INTERRUPT2 group individual interrupt bit masks */
#define M2R_LIF_INTERRUPT2_SFI41PERR        (0x8000)
#define M2R_LIF_INTERRUPT2_PRBSCNT31        (0x4000)
#define M2R_LIF_INTERRUPT2_PRBSERR          (0x2000)
#define M2R_LIF_INTERRUPT2_XGDATA           (0x1000)
#define M2R_LIF_INTERRUPT2_XGBAD_VCOCLK     (0x0800)
#define M2R_LIF_INTERRUPT2_XGLOCK           (0x0400)
#define M2R_LIF_INTERRUPT2_TXCK_MISS        (0x0200)
#define M2R_LIF_INTERRUPT2_RXCK_MISS        (0x0100)
#define M2R_LIF_INTERRUPT2_TxLnPAI          (0x0080)
#define M2R_LIF_INTERRUPT2_TxLBPAI          (0x0040)
#define M2R_LIF_INTERRUPT2_RxPCPAI          (0x0020)
#define M2R_LIF_INTERRUPT2_RxLBPAI          (0x0010)
#define M2R_LIF_INTERRUPT2_RxL3PAI          (0x0008)
#define M2R_LIF_INTERRUPT2_RxL2PAI          (0x0004)
#define M2R_LIF_INTERRUPT2_RxL1PAI          (0x0002)
#define M2R_LIF_INTERRUPT2_RxL0PAI          (0x0001)

#define M2R_GRP_LIF_INTERRUPT2_ALL          (0xffff)

/************************************************************************
 * LIF IRQ GROUPS
 ************************************************************************/
typedef enum {
  M2R_GRP_LIF_INTERRUPT1 = 0,
  M2R_GRP_LIF_INTERRUPT2,

  /* PCS Groups */
  M2R_GRP_LIF_PCS6_TX_TxInt,
  M2R_GRP_LIF_PCS6_RX_RxInt,

  /* XGXS Groups */
  M2R_GRP_LIF_XGXS_MP_MnInt0,
  M2R_GRP_LIF_XGXS_MP_MnInt1,
  M2R_GRP_LIF_XGXS_TX_TxInt,
  M2R_GRP_LIF_XGXS_RX_RxInt,

  M2R_LIF_MAX_GRP
} m2r_lif_irq_grp_t ;

extern
cs_status m2r_lif_get_default_irq_cfg(cs_uint16 port_id,
                                      m2r_port_summ_t * pSumm,
                                      m2r_lif_cfg_t * pCfg) ;


extern
cs_status m2r_lif_init_irq(cs_uint16 port_id, m2r_lif_cfg_t * pCfg) ;

extern cs_status m2r_lif_isr(cs_uint16 port_id) ;

extern cs_status m2r_lif_dump_irq(cs_uint16 port_id) ;

#endif /* __M2_LIF_IRQ_H__ */

