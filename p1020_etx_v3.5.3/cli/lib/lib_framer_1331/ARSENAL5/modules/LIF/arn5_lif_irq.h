/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_lif_irq.h
 *
 * LIF IRQ Tables and APIs
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#ifndef __ARN5_LIF_IRQ_H__
#define __ARN5_LIF_IRQ_H__

#include "cs_types.h"

/********************************************************************
 * LIF IRQ
 ********************************************************************/
/* IRQ Leaf defines for ARN5_GRP_LIF_INTERRUPT */
#define ARN5_LIF_INTERRUPT_PRBS_RX_EFi       (0x0010)
#define ARN5_LIF_INTERRUPT_PRBS_SYNCi        (0x0008)
#define ARN5_LIF_INTERRUPT_NO_DATAi          (0x0004)
#define ARN5_LIF_INTERRUPT_IN_LOCKi          (0x0002)
#define ARN5_LIF_INTERRUPT_SRX_LOCKDETIi     (0x0001)
#define ARN5_GRP_LIF_INTERRUPT_ALL           (0x001f)

typedef enum {
  ARN5_GRP_LIF_INTERRUPT = 0,

  ARN5_LIF_MAX_GRP
} arn5_lif_irq_grp_t ;


#endif /* __M2_LIF_IRQ_H__ */


