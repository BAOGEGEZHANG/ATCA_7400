
/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_pprbs_irq.h
 *
 * PPRBS IRQ Tables and APIs
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#ifndef __ARN5_PPRBS_IRQ_H__
#define __ARN5_PPRBS_IRQ_H__


/************************************************************************
 * PPRBS IRQ leaf defines
 ************************************************************************/
/* Leaf Interrupt Bit fields for ARN5_PPRBS_Interrupt */
#define ARN5_PPRBS_Interrupt_RolloverTxPacket1I                  0x8000
#define ARN5_PPRBS_Interrupt_RolloverTxByte1I                    0x4000
#define ARN5_PPRBS_Interrupt_SentPackets1I                       0x2000
#define ARN5_PPRBS_Interrupt_RolloverPacket1I                    0x1000
#define ARN5_PPRBS_Interrupt_RolloverByte1I                      0x0800
#define ARN5_PPRBS_Interrupt_Rollover1I                          0x0400
#define ARN5_PPRBS_Interrupt_BitError1I                          0x0200
#define ARN5_PPRBS_Interrupt_SyncState1I                         0x0100
#define ARN5_PPRBS_Interrupt_RolloverTxPacket0I                  0x0080
#define ARN5_PPRBS_Interrupt_RolloverTxByte0I                    0x0040
#define ARN5_PPRBS_Interrupt_SentPackets0I                       0x0020
#define ARN5_PPRBS_Interrupt_RolloverPacket0I                    0x0010
#define ARN5_PPRBS_Interrupt_RolloverByte0I                      0x0008
#define ARN5_PPRBS_Interrupt_Rollover0I                          0x0004
#define ARN5_PPRBS_Interrupt_BitError0I                          0x0002
#define ARN5_PPRBS_Interrupt_SyncState0I                         0x0001
#define ARN5_GRP_PPRBS_Interrupt_ALL                             0xffff

typedef enum {
  ARN5_GRP_PPRBS_Interrupt,
  ARN5_PPRBS_MAX_GRP
} arn5_pprbs_irq_grp_t ;

#endif /* __ARN5_PPRBS_IRQ_H__ */
