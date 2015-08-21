/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth_irq.h
 *
 * ETH IRQ Tables and APIs
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#ifndef __ARN5_ETH_IRQ_H__
#define __ARN5_ETH_IRQ_H__


/************************************************************************
 * SPI4P2F IRQ leaf defines
 ************************************************************************/
/* Leaf Interrupt Bit fields for ARN5_GIGE_EI_ECORE_TOP_INTERRUPT */
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_TX_UFLWi             0x0800
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_TX_OFLWi             0x0400
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_RX_UFLWi             0x0200
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_RX_OFLWi             0x0100
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_MAC_PROT_ERR_WRi        0x0040
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_MAC_PROT_ERR_RDi        0x0020
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_PCS_AN_DONEi            0x0004
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_PCS_LINKi               0x0002
#define ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT_ALL                 0x0f66

/* Leaf Interrupt Bit fields for ARN5_GIGE_EI_LINE_INTERRUPT */
#define ARN5_GIGE_EI_LINE_INTERRUPT_PRBS_RX_EFi                  0x0010
#define ARN5_GIGE_EI_LINE_INTERRUPT_PRBS_NOSYNCi                 0x0008
#define ARN5_GIGE_EI_LINE_INTERRUPT_NO_DATAi                     0x0004
#define ARN5_GIGE_EI_LINE_INTERRUPT_IN_LOCKi                     0x0002
#define ARN5_GIGE_EI_LINE_INTERRUPT_RX_LOCKDETIi                 0x0001
#define ARN5_GRP_GIGE_EI_LINE_INTERRUPT_ALL                      0x001f

/* Leaf Interrupt Bit fields for ARN5_GIGE_MISC_CONV_INTERRUPT */
#define ARN5_GIGE_MISC_CONV_INTERRUPT_TX_SHORT_DROP_MSBi         0x0400
#define ARN5_GIGE_MISC_CONV_INTERRUPT_TX_SHORT_DROPi             0x0200
#define ARN5_GIGE_MISC_CONV_INTERRUPT_TX_FIFO_UNDERFLOWi         0x0100
#define ARN5_GIGE_MISC_CONV_INTERRUPT_TX_FIFO_PAR_ERRi           0x0080
#define ARN5_GIGE_MISC_CONV_INTERRUPT_TX_OVERFLOWi               0x0040
#define ARN5_GIGE_MISC_CONV_INTERRUPT_TX_BADFRAMEi               0x0020
#define ARN5_GIGE_MISC_CONV_INTERRUPT_TX_MEM_PAR_ERRi            0x0010
#define ARN5_GIGE_MISC_CONV_INTERRUPT_RX_DROPi                   0x0002
#define ARN5_GIGE_MISC_CONV_INTERRUPT_RX_ABORTi                  0x0001
#define ARN5_GRP_GIGE_MISC_CONV_INTERRUPT_ALL                    0x07f3

typedef enum {
  ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT,
  ARN5_GRP_GIGE_EI_LINE_INTERRUPT,
  ARN5_GRP_GIGE_MISC_CONV_INTERRUPT,
  ARN5_ETH_MAX_GRP
} arn5_eth_irq_grp_t ;


/* Intermidiate Interrupt Bit fields for ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT */
#define ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_EI_ECOREi            0x0002
#define ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_EI_LINEi             0x0001
#define ARN5_IMED_GIGE_EI_TOP_ETHERNET_INTERRUPT_ALL             0x0003

/* Intermidiate Interrupt Bit fields for ARN5_GIGE_MISC_INTERRUPT */
#define ARN5_GIGE_MISC_INTERRUPT_EI_CONVi                        0x0002
#define ARN5_GIGE_MISC_INTERRUPT_EIi                             0x0001
#define ARN5_IMED_GIGE_MISC_INTERRUPT_ALL                        0x0003

typedef enum {
  ARN5_IMED_GIGE_EI_TOP_ETHERNET_INTERRUPT,
  ARN5_IMED_GIGE_MISC_INTERRUPT,
  ARN5_ETH_MAX_IMED
} arn5_eth_imed_irq_grp_t ;

#endif /* __ARN5_ETH_IRQ_H__ */

