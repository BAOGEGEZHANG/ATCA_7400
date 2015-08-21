/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_milan2_mpif.h
 * Header file for Milan2's MPIF block
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2R_MILAN2_MPIF_H__
#define __M2R_MILAN2_MPIF_H__

#include "cs_types.h"
#include "cs_rtos.h"


/******************   DEVICE ***********************************/
CS_INLINE cs_boolean M2R_IS_JTAG_ID_CORRECT(cs_uint32 jtag_id) 
{
  if ( (((jtag_id & 0x30000000) == 0x20000000) || 
	((jtag_id & 0x30000000) == 0x30000000))
       && 
       (jtag_id & 0x00001000)
       && 
       ((jtag_id & 0x00000FFF) == 0x000003E5) ) {
    return (TRUE) ;
  }

  return (FALSE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_IS_JTAG_ID_CORRECT(jtag_id)  ; 
  }  
#endif
}

extern
cs_uint32  m2r_get_chip_jtag_id(cs_uint16 dev_id) ;
extern 
cs_status  m2r_do_bist(cs_uint16 dev_id, cs_boolean mbist, 
		      cs_boolean sbist) ;
extern 
cs_boolean  m2r_is_bist_done(cs_uint16 dev_id, cs_boolean mbist,
			     cs_boolean sbist) ;
extern 
cs_status  m2r_check_bist_status(cs_uint16 dev_id, cs_boolean mbist,
				 cs_boolean sbist) ;

extern
cs_status  m2r_mpif_ctl_eth_mode(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_boolean m2r_mpif_is_in_eth_mode(cs_uint16 port_id) ;

extern
cs_status  m2r_mpif_ctl_oc48_mode(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern
cs_boolean m2r_mpif_is_in_oc48_mode(cs_uint16 port_id) ;

/******************   PORT  ***********************************/

extern 
cs_status  m2r_port_hard_reset(cs_uint16 port_id, m2r_module_id_t mid,
			       m2r_reset_action_t act) ;

#endif /* __M2R_MILAN2_MPIF_H__ */
