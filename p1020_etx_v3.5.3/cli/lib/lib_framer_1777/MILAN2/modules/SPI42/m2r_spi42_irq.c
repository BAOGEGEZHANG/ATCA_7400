/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_spi42.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_spi42_irq.h"

#define M2R_SPI42_IRQ_CTL_FLAGS     ( M2R_IRQ_FLG_RW1C |         \
                                      M2R_IRQ_FLG_1_TO_ENABLE )

/*****************************************************************/
/*                        IRQ  TABLE                             */
/*****************************************************************/
const m2r_irq_ctl_t  m2r_spi42_irq_tbl[M2R_SPI42_MAX_GRP] = {
  CS_DI([M2R_GRP_SPI42_RXINTRPTS]  =) {
    M2R_SPI42_REG_OFST(RXINTRPTS),
    0,
    M2R_SPI42_REG_OFST(RXINTRPTEN),
    M2R_GRP_SPI42_RXINTRPTS_ALL,
    0,
    M2R_SPI42_IRQ_CTL_FLAGS },

  CS_DI([M2R_GRP_SPI42_TXINTRPTS]  =) {
    M2R_SPI42_REG_OFST(TXINTRPTS),
    0,
    M2R_SPI42_REG_OFST(TXINTRPTEN),
    M2R_GRP_SPI42_TXINTRPTS_ALL,
    0,
    M2R_SPI42_IRQ_CTL_FLAGS },

  CS_DI([M2R_GRP_SPI42_TXFIFOINTRPTS]  =) {
    M2R_SPI42_REG_OFST(TXFIFOINTRPTS),
    0,
    M2R_SPI42_REG_OFST(TXFIFOINTRPTEN),
    M2R_GRP_SPI42_TXFIFOINTRPTS_ALL,
    0,
    M2R_SPI42_IRQ_CTL_FLAGS },

  CS_DI([M2R_GRP_SPI42_RXINTRPTS2]  =) {
    M2R_SPI42_REG_OFST(RXINTRPTS2),
    0,
    M2R_SPI42_REG_OFST(RXINTRPTEN2),
    M2R_GRP_SPI42_RXINTRPTS2_ALL,
    0,
    M2R_SPI42_IRQ_CTL_FLAGS }
} ;


const char * m2r_spi42_irq_grp_name_tbl[M2R_SPI42_MAX_GRP] =
{
  CS_DI([M2R_GRP_SPI42_RXINTRPTS] 	=) "RXINTRPTS",
  CS_DI([M2R_GRP_SPI42_TXINTRPTS] 	=) "TXINTRPTS",
  CS_DI([M2R_GRP_SPI42_TXFIFOINTRPTS] =) "TXFIFOINTRPTS",
  CS_DI([M2R_GRP_SPI42_RXINTRPTS2]	=) "RXINTRPTS2"
} ;


/**************** IRQ ID NAME TABLES **********************/
const m2r_irq_id_name_t
M2R_GRP_SPI42_RXINTRPTS_irq_id_name_tbl[] = {
  {M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW3,   "RXFIFOOVRFLW3"},
  {M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW2,   "RXFIFOOVRFLW2"},
  {M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW1,   "RXFIFOOVRFLW1"},
  {M2R_SPI42_RXINTRPTS_RXFIFOOVRFLW0,   "RXFIFOOVRFLW0"},
  {M2R_SPI42_RXINTRPTS_RXMISSSOP,       "RXMISSSOP"},
  {M2R_SPI42_RXINTRPTS_RXMISSEOP,       "RXMISSEOP"},
  {M2R_SPI42_RXINTRPTS_RXADDRERR,       "RXADDRERR"},
  {M2R_SPI42_RXINTRPTS_DIP2ERR,         "DIP2ERR"},
  {M2R_SPI42_RXINTRPTS_RXLOSYN,         "RXLOSYN"}
} ;

const m2r_irq_id_name_t
M2R_GRP_SPI42_TXINTRPTS_irq_id_name_tbl[] = {
  {M2R_SPI42_TXINTRPTS_TXRSVCTRLWD,     "TXRSVCTRLWD"},
  {M2R_SPI42_TXINTRPTS_TXSOPDISTERR,    "TXSOPDISTERR"},
  {M2R_SPI42_TXINTRPTS_TXMISSVLD,       "TXMISSVLD"},
  {M2R_SPI42_TXINTRPTS_TXMISSSOP,       "TXMISSSOP"},
  {M2R_SPI42_TXINTRPTS_TXMISSEOP,       "TXMISSEOP"},
  {M2R_SPI42_TXINTRPTS_TXADDRERR,       "TXADDRERR"},
  {M2R_SPI42_TXINTRPTS_TXBURSTSIZEERR,  "TXBURSTSIZEERR"},
  {M2R_SPI42_TXINTRPTS_DIP4ERR,         "DIP4ERR"},
  {M2R_SPI42_TXINTRPTS_TXLOSYN,         "TXLOSYN"}
} ;

const m2r_irq_id_name_t
M2R_GRP_SPI42_TXFIFOINTRPTS_irq_id_name_tbl[] = {
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW3,       "TXFIFOOVRFLW3"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW2,       "TXFIFOOVRFLW2"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW1,       "TXFIFOOVRFLW1"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOOVRFLW0,       "TXFIFOOVRFLW0"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR3,       "TXFIFOECCERR3"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR2,       "TXFIFOECCERR2"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR1,       "TXFIFOECCERR1"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCERR0,       "TXFIFOECCERR0"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX3,       "TXFIFOECCFIX3"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX2,       "TXFIFOECCFIX2"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX1,       "TXFIFOECCFIX1"},
  {M2R_SPI42_TXFIFOINTRPTS_TXFIFOECCFIX0,       "TXFIFOECCFIX0"}
} ;

const m2r_irq_id_name_t
M2R_GRP_SPI42_RXINTRPTS2_irq_id_name_tbl[] = {
  {M2R_SPI42_RXINTRPTS2_RXFIFOPERR3,    "RXFIFOPERR3"},
  {M2R_SPI42_RXINTRPTS2_RXFIFOPERR2,    "RXFIFOPERR2"},
  {M2R_SPI42_RXINTRPTS2_RXFIFOPERR1,    "RXFIFOPERR1"},
  {M2R_SPI42_RXINTRPTS2_RXFIFOPERR0,    "RXFIFOPERR0"}
} ;

/****************************************************************/
/* This is the ISR routine for the spi4.2 block                 */
/****************************************************************/
cs_status m2r_spi42_isr(cs_uint16 port_id)
{
  M2R_t              * pDev ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_SPI42,
		    M2R_GRP_SPI42_RXINTRPTS,
		    &m2r_spi42_irq_tbl[M2R_GRP_SPI42_RXINTRPTS],
	   	    m2r_spi42_irq_grp_name_tbl[M2R_GRP_SPI42_RXINTRPTS],
		    M2R_GRP_SPI42_RXINTRPTS_irq_id_name_tbl,
		    sizeof(M2R_GRP_SPI42_RXINTRPTS_irq_id_name_tbl) /
		    sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_SPI42,
		    M2R_GRP_SPI42_TXINTRPTS,
		    &m2r_spi42_irq_tbl[M2R_GRP_SPI42_TXINTRPTS],
		    m2r_spi42_irq_grp_name_tbl[M2R_GRP_SPI42_TXINTRPTS],
		    M2R_GRP_SPI42_TXINTRPTS_irq_id_name_tbl,
		    sizeof(M2R_GRP_SPI42_TXINTRPTS_irq_id_name_tbl) /
		    sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_SPI42,
		    M2R_GRP_SPI42_TXFIFOINTRPTS,
		    &m2r_spi42_irq_tbl[M2R_GRP_SPI42_TXFIFOINTRPTS],
		    m2r_spi42_irq_grp_name_tbl[M2R_GRP_SPI42_TXFIFOINTRPTS],
		    M2R_GRP_SPI42_TXFIFOINTRPTS_irq_id_name_tbl,
		    sizeof(M2R_GRP_SPI42_TXFIFOINTRPTS_irq_id_name_tbl) /
		    sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_SPI42,
		    M2R_GRP_SPI42_RXINTRPTS2,
		    &m2r_spi42_irq_tbl[M2R_GRP_SPI42_RXINTRPTS2],
		    m2r_spi42_irq_grp_name_tbl[M2R_GRP_SPI42_RXINTRPTS2],
		    M2R_GRP_SPI42_RXINTRPTS2_irq_id_name_tbl,
		    sizeof(M2R_GRP_SPI42_RXINTRPTS2_irq_id_name_tbl) /
		    sizeof(m2r_irq_id_name_t)) ;

  return(CS_OK) ;
}


/****************************************************************
 * Initialize SPI4.2 Interrupts                                 *
 * - Register the IRQ tables                                    *
 * - Enable the block level SPI4.2 interrupt(in CONTROL reg)    *
 * - Clear all and selectively enable IRQ leaf-level controls   *
 ****************************************************************/
cs_status m2r_spi42_init_irq(cs_uint16 port_id, cs_uint16 * irq_ctl_tbl)
{
  M2R_t                 * pDev ;
  SPI4P2_CONTROL_t      ctl ;
  cs_uint16		ii ;

  /* Register the IRQ tables */
  M2R_REGISTER_IRQ_TBL(port_id, M2R_ID_SPI42, m2r_spi42_irq_tbl,
                       m2r_spi42_irq_grp_name_tbl, M2R_SPI42_MAX_GRP, NULL) ;

  /* Enable block level SPI4.2 interrupt(in CONTROL reg) */
  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  M2R_PORT_LOCK(port_id, M2R_ID_SPI42) ;
  ctl.wrd = M2R_REG_READ(&pDev->SPI4P2.CONTROL.wrd) ;
  ctl.bf.INTRPTEN = 1 ;
  M2R_REG_WRITE(&pDev->SPI4P2.CONTROL.wrd, ctl.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_SPI42) ;

  /* Clear all and selectively enable IRQ leaf-level controls */
  for (ii = 0; ii < M2R_SPI42_MAX_GRP; ii++) {
    if ( m2r_clear_leaf_irq(port_id, M2R_ID_SPI42, ii,
			    m2r_spi42_irq_tbl[ii].irq_map)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
    if ( m2r_enable_leaf_irq(port_id, M2R_ID_SPI42, ii,
			     irq_ctl_tbl[ii])
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}












