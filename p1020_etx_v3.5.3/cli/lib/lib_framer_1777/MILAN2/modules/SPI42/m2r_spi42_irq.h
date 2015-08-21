/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_spi42_irq.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_SPI42_IRQ_H__
#define __M2R_SPI42_IRQ_H__

#include "cs_types.h"


#define M2R_SPI42_PORT_ALL_CHANNELS             0x000f
/********************************************************
 * INTERRUPT BIT(IRQ-ID) DEFINES FOR VARIOUS IRQ GROUPS *
 ********************************************************/

/* M2R_GRP_SPI42_RXINTRPTS */
#define M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW3     	0x0800
#define M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW2     	0x0400
#define M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW1     	0x0200
#define M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW0     	0x0100
#define M2R_SPI42_RXINTRPTS_RXMISSSOP	      	0x0020
#define M2R_SPI42_RXINTRPTS_RXMISSEOP	      	0x0010
#define M2R_SPI42_RXINTRPTS_RXADDRERR	      	0x0008
#define M2R_SPI42_RXINTRPTS_DIP2ERR	      	0x0002
#define M2R_SPI42_RXINTRPTS_RXLOSYN	      	0x0001
#define M2R_GRP_SPI42_RXINTRPTS_ALL	      	0x0f3b

/* M2R_GRP_SPI42_TXINTRPTS */
#define M2R_SPI42_TXINTRPTS_TXRSVCTRLWD		0x0100
#define M2R_SPI42_TXINTRPTS_TXSOPDISTERR       	0x0080
#define M2R_SPI42_TXINTRPTS_TXMISSVLD		0x0040
#define M2R_SPI42_TXINTRPTS_TXMISSSOP		0x0020
#define M2R_SPI42_TXINTRPTS_TXMISSEOP		0x0010
#define M2R_SPI42_TXINTRPTS_TXADDRERR		0x0008
#define M2R_SPI42_TXINTRPTS_TXBURSTSIZEERR	0x0004
#define M2R_SPI42_TXINTRPTS_DIP4ERR		0x0002
#define M2R_SPI42_TXINTRPTS_TXLOSYN		0x0001
#define M2R_GRP_SPI42_TXINTRPTS_ALL		0x01ff

/* M2R_GRP_SPI42_TXFIFOINTRPTS */
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW3	0x0800
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW2	0x0400
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW1	0x0200
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW0	0x0100
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR3	0x0080
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR2	0x0040
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR1	0x0020
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR0	0x0010
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX3	0x0008
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX2	0x0004
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX1	0x0002
#define M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX0	0x0001
#define M2R_GRP_SPI42_TXFIFOINTRPTS_ALL		0x0fff

/* M2R_GRP_SPI4P2_RXINTRPTS2 */
#define M2R_SPI42_RXINTRPTS2_RXFIFOPERR3	0x0008
#define M2R_SPI42_RXINTRPTS2_RXFIFOPERR2	0x0004
#define M2R_SPI42_RXINTRPTS2_RXFIFOPERR1	0x0002
#define M2R_SPI42_RXINTRPTS2_RXFIFOPERR0	0x0001
#define M2R_GRP_SPI42_RXINTRPTS2_ALL		0x000f


/********************************************************
 *                   IRQ GROUP-ID's                     *
 ********************************************************/
typedef enum {
  M2R_GRP_SPI42_RXINTRPTS	= 0,
  M2R_GRP_SPI42_TXINTRPTS,
  M2R_GRP_SPI42_TXFIFOINTRPTS,
  M2R_GRP_SPI42_RXINTRPTS2,
  /* all */
  M2R_SPI42_MAX_GRP
} m2r_spi42_irq_grp_t ;


#endif  /* __M2R_SPI42_IRQ_H__ */






