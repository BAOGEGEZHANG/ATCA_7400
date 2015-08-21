/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_spi42.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PRIVATE header file.
 */
#ifndef __M2R_SPI42_PRIV_H__
#define __M2R_SPI42_PRIV_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_common.h"

#include "m2r_spi42.h"

#define  M2R_SPI42_INVALID_PORT_EXIT(port_id, ret_val)              \
{                                                                   \
  cs_uint32             err_code = 0 ;                              \
                                                                    \
  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {                  \
    CS_HNDL_ERROR(port_id, err_code, " : port_id= %d\n",            \
                  port_id) ;                                        \
    return(ret_val) ;                                               \
  }                                                                 \
}


/* Private externs */
extern
cs_status m2r_spi42_init_irq(cs_uint16 port_id, 
			     cs_uint16 * irq_ctl_tbl) ;
extern
cs_status m2r_spi42_enable_leaf_irq(cs_uint16 port_id, cs_uint32 dev_addr,
                                    cs_uint32 grp_id,
                                    const m2r_irq_ctl_t * pirq_cb,
                                    cs_uint16 irq_enbl_bits) ;
extern
cs_status m2r_spi42_disable_leaf_irq(cs_uint16 port_id, cs_uint32 dev_addr,
                                     cs_uint32 grp_id,
                                     const m2r_irq_ctl_t * pirq_cb,
                                     cs_uint16 irq_disable_bits) ;
extern
cs_status m2r_spi42_clear_leaf_irq(cs_uint16 port_id, cs_uint32 dev_addr, 
                                   cs_uint32 grp_id,
                                   const m2r_irq_ctl_t * pirq_cb,
                                   cs_uint16 irq_clr_bits) ;
extern
cs_status m2r_spi42_isr(cs_uint16 dev_id) ;
extern 
cs_status  m2r_spi42_init_stats(cs_uint16 port_id,
				cs_uint8 num_channels) ;






#endif /* __M2R_SPI42_H__ */
