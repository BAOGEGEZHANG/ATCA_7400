
/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_mpif_irq.h
 *
 * MPIF IRQ Tables and APIs
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#ifndef __ARN5_MPIF_IRQ_H__
#define __ARN5_MPIF_IRQ_H__


/************************************************************************
 * MPIF IRQ leaf defines
 ************************************************************************/
/* Leaf Interrupt Bit fields for ARN5_MPIF_MPIF_INTERRUPT */
#define ARN5_MPIF_MPIF_INTERRUPT_buserrI                         0x0001
#define ARN5_GRP_MPIF_MPIF_INTERRUPT_ALL                         0x0001
#define ARN5_GRP_MPIF_MPIF_INTERRUPT_SPL                         0x0001

/* Leaf Interrupt Bit fields for ARN5_MPIF_GPIORInt0 */
#define ARN5_MPIF_GPIORInt0_RI0_7                                0x0080
#define ARN5_MPIF_GPIORInt0_RI0_6                                0x0040
#define ARN5_MPIF_GPIORInt0_RI0_5                                0x0020
#define ARN5_MPIF_GPIORInt0_RI0_4                                0x0010
#define ARN5_MPIF_GPIORInt0_RI0_3                                0x0008
#define ARN5_MPIF_GPIORInt0_RI0_2                                0x0004
#define ARN5_MPIF_GPIORInt0_RI0_1                                0x0002
#define ARN5_MPIF_GPIORInt0_RI0_0                                0x0001
#define ARN5_GRP_MPIF_GPIORInt0_ALL                              0x00ff

/* Leaf Interrupt Bit fields for ARN5_MPIF_GPIORInt1 */
#define ARN5_MPIF_GPIORInt0_RI1_7                                0x0080
#define ARN5_MPIF_GPIORInt0_RI1_6                                0x0040
#define ARN5_MPIF_GPIORInt0_RI1_5                                0x0020
#define ARN5_MPIF_GPIORInt0_RI1_4                                0x0010
#define ARN5_MPIF_GPIORInt0_RI1_3                                0x0008
#define ARN5_MPIF_GPIORInt0_RI1_2                                0x0004
#define ARN5_MPIF_GPIORInt0_RI1_1                                0x0002
#define ARN5_MPIF_GPIORInt0_RI1_0                                0x0001
#define ARN5_GRP_MPIF_GPIORInt1_ALL                              0x00ff

/* Leaf Interrupt Bit fields for ARN5_MPIF_GPIOFInt0 */
#define ARN5_MPIF_GPIOFInt0_FI0_7                                0x0080
#define ARN5_MPIF_GPIOFInt0_FI0_6                                0x0040
#define ARN5_MPIF_GPIOFInt0_FI0_5                                0x0020
#define ARN5_MPIF_GPIOFInt0_FI0_4                                0x0010
#define ARN5_MPIF_GPIOFInt0_FI0_3                                0x0008
#define ARN5_MPIF_GPIOFInt0_FI0_2                                0x0004
#define ARN5_MPIF_GPIOFInt0_FI0_1                                0x0002
#define ARN5_MPIF_GPIOFInt0_FI0_0                                0x0001
#define ARN5_GRP_MPIF_GPIOFInt0_ALL                              0x00ff

/* Leaf Interrupt Bit fields for ARN5_MPIF_GPIOFInt1 */
#define ARN5_MPIF_GPIOFInt0_FI1_7                                0x0080
#define ARN5_MPIF_GPIOFInt0_FI1_6                                0x0040
#define ARN5_MPIF_GPIOFInt0_FI1_5                                0x0020
#define ARN5_MPIF_GPIOFInt0_FI1_4                                0x0010
#define ARN5_MPIF_GPIOFInt0_FI1_3                                0x0008
#define ARN5_MPIF_GPIOFInt0_FI1_2                                0x0004
#define ARN5_MPIF_GPIOFInt0_FI1_1                                0x0002
#define ARN5_MPIF_GPIOFInt0_FI1_0                                0x0001
#define ARN5_GRP_MPIF_GPIOFInt1_ALL                              0x00ff

typedef enum {
  ARN5_GRP_MPIF_MPIF_INTERRUPT,
  ARN5_GRP_MPIF_GPIORInt0,
  ARN5_GRP_MPIF_GPIORInt1,
  ARN5_GRP_MPIF_GPIOFInt0,
  ARN5_GRP_MPIF_GPIOFInt1,
  ARN5_MPIF_MAX_GRP
} arn5_mpif_irq_grp_t ;

/************************************************************************
 * MPIF IRQ intermidiate defines
 ************************************************************************/
/* none */

#endif /* __ARN5_MPIF_IRQ_H__ */

