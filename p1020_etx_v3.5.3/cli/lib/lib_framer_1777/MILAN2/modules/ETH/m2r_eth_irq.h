/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_eth_irq.h
 *
 * XETH IRQ defines and Private APIs.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2R_ETH_IRQ_H__
#define __M2R_ETH_IRQ_H__

/***********************************************************************
 * XETH IRQ leaf defines
 **********************************************************************/
#define M2R_ETH_MP_Interrupt_TxDataComBadParI     (0x2000)
#define M2R_ETH_MP_Interrupt_TxDataComBadFrmI     (0x1000)
#define M2R_ETH_MP_Interrupt_TxDataComUndFlwI     (0x0800)
#define M2R_ETH_MP_Interrupt_TxDataComOvrFlwI     (0x0400)
#define M2R_ETH_MP_Interrupt_RxDataComBadFrmI     (0x0200)
#define M2R_ETH_MP_Interrupt_RxDataComOvrFlwI     (0x0100)
#define M2R_ETH_MP_Interrupt_RxDataMemParErrI     (0x0080)
#define M2R_ETH_MP_Interrupt_TxDataMemParErrI     (0x0040)
#define M2R_ETH_MP_Interrupt_RxMessMemParErrI     (0x0020)
#define M2R_ETH_MP_Interrupt_TxMessMemParErrI     (0x0010)
#define M2R_ETH_MP_Interrupt_RxNoClkI             (0x0002)
#define M2R_ETH_MP_Interrupt_TxNoClkI             (0x0001)
#define M2R_GRP_ETH_MP_Interrupt_ALL              (0x3ff3)

/* Feature 1 */
#define M2R_ETH_TX_Interrupt_GoodPktSatI          (0x0004)
#define M2R_ETH_TX_Interrupt_ErrorPktSatI         (0x0002)
#define M2R_ETH_TX_Interrupt_ErrorPktI            (0x0001)

#define M2R_GRP_ETH_TX_Interrupt_ALL   (                   \
/* Feature 1 */                                  \
                    M2R_ETH_TX_Interrupt_GoodPktSatI   |   \
                    M2R_ETH_TX_Interrupt_ErrorPktSatI  |   \
                    M2R_ETH_TX_Interrupt_ErrorPktI)

/* Feature 1 */
#define M2R_ETH_RX_Interrupt_0_DropPktSatI        (0x0800)
#define M2R_ETH_RX_Interrupt_0_DropPktI           (0x0400)
#define M2R_ETH_RX_Interrupt_0_ErrorCRCSatI       (0x0040)
#define M2R_ETH_RX_Interrupt_0_ErrorCRCI          (0x0020)
#define M2R_ETH_RX_Interrupt_0_GoodPktSatI        (0x0004)
#define M2R_ETH_RX_Interrupt_0_ErrorPktSatI       (0x0002)
#define M2R_ETH_RX_Interrupt_0_ErrorPktI          (0x0001)

#define M2R_GRP_ETH_RX_Interrupt_0_ALL         (               \
/* Feature 1 */                                      \
                    M2R_ETH_RX_Interrupt_0_DropPktSatI     |   \
                    M2R_ETH_RX_Interrupt_0_DropPktI        |   \
                    M2R_ETH_RX_Interrupt_0_ErrorCRCSatI    |   \
                    M2R_ETH_RX_Interrupt_0_ErrorCRCI       |   \
                    M2R_ETH_RX_Interrupt_0_GoodPktSatI     |   \
                    M2R_ETH_RX_Interrupt_0_ErrorPktSatI    |   \
                    M2R_ETH_RX_Interrupt_0_ErrorPktI   )

/* Feature 1 */

/***********************************************************************
 * XETH IRQ GROUPS
 **********************************************************************/
typedef enum {
  M2R_GRP_ETH_MP_Interrupt     = 0,
  M2R_GRP_ETH_TX_Interrupt,
  M2R_GRP_ETH_RX_Interrupt_0,
/* Feature 1 */

  M2R_ETH_MAX_GRP
} m2r_eth_irq_grp_t ;

extern
cs_status m2r_eth_get_default_irq_cfg(cs_uint16 port_id,
                                      m2r_port_summ_t * pSumm,
                                      m2r_eth_cfg_t * pCfg) ;


extern
cs_status m2r_eth_init_irq(cs_uint16 port_id, m2r_eth_cfg_t * pCfg) ;

extern
cs_status m2r_eth_isr(cs_uint16 port_id) ;


#endif /* __M2R_ETH_IRQ_H__ */

