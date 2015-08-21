/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_common.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PRIVATE header file - for driver use only!
 * This contains defines, macros, inline functions, etc.. which
 * are used internally by code in other modules.
 */
#ifndef __M2R_COMMON_H__
#define __M2R_COMMON_H__

#ifndef CS_DONT_USE_VAR_ARGS
#	include <stdarg.h>
#endif

#include "cs_types.h"
#include "cs_utils.h"

#include "m2r_print.h"
#include "m2r_error.h"
#include "m2r_registers.h"
#include "m2r_cb.h"

#include "m2r_public.h"

extern
m2r_dev_cb_t * m2r_get_dev_tbl(void) ;
extern
m2r_drvr_cb_t       * pM2R ;

#define M2R_REG_READ(addr) CS_REG_READ(addr) 
#define M2R_REG_WRITE(addr, data)     {    		\
  if (!pM2R->warm_start) CS_REG_WRITE(addr, data) ;     \
  }  

#define M2R_DRVR_IS_LOADED() ( pM2R && pM2R->drvr_loaded )
#define M2R_RESET_MSEC_DELAY ( 2 )   /* 2 milli-seconds */
/******************************************************
 ******************      PORT      ********************
 * Before using any of the mapping functions, the     *
 * code should first check if the port is valid.      *
 ******************************************************/

/*
 * port_id -> dev_num
 */
#define M2R_PORT_ID_TO_DEV_NUM(port_id) 			\
    ( (port_id) / M2R_NUM_PORTS_PER_ASIC )
/*
 * port_id -> dev_id
 */
#define M2R_PORT_ID_TO_DEV_ID(port_id) 				\
    ( M2R_PORT_ID_TO_DEV_NUM(port_id) )

/*
 * port_id -> slice_num
 */
#define M2R_PORT_ID_TO_SLICE_NUM(port_id)			\
    ( (port_id) % M2R_NUM_PORTS_PER_ASIC )

/*
 * port_id -> (devcb *)
 */
#define M2R_PORT_ID_TO_DEVCB_PTR(port_id) 			\
    ( &((m2r_get_dev_tbl())[ M2R_PORT_ID_TO_DEV_ID(port_id) ]) )

/*
 * port_id -> (pcb *)
 */
#define M2R_PORT_ID_TO_PCB_PTR(port_id)				\
    ( (M2R_PORT_ID_TO_DEVCB_PTR(port_id))->			\
      ppcb[ M2R_PORT_ID_TO_SLICE_NUM(port_id) ] )

/*
 * port_id -> device base addr
 */
#define M2R_PORT_ID_TO_DEV_BASE(port_id)	( (M2R_t *)   	\
    (M2R_PORT_ID_TO_DEVCB_PTR(port_id))->base_addr )

/*
 * dev_num, slice_num -> port_id
 */
#define M2R_DEV_SLICE_NUM_TO_PORT_ID(dev_num, sl_num)		\
  ( (dev_num * M2R_NUM_PORTS_PER_ASIC) + sl_num )

/*
 * port_id -> dev_num, slice_num, base_addr
 */
CS_INLINE void M2R_GET_PORT_PARAMS(cs_uint16 port_id,
				   cs_uint16 * pdev_num,
				   cs_uint16 * psl_num,
				   M2R_t ** pbase_addr)
{
  *pdev_num 	= M2R_PORT_ID_TO_DEV_NUM(port_id) ;
  *psl_num  	= M2R_PORT_ID_TO_SLICE_NUM(port_id) ;
  *pbase_addr 	= M2R_PORT_ID_TO_DEV_BASE(port_id) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_PORT_PARAMS(port_id, pdev_num, psl_num, pbase_addr) ;
  }
#endif
}

/*
 * CHECK FOR VALID PORT_ID
 * In the code, use the macro below instead of the inline function!
 */
CS_INLINE cs_boolean __M2R_IS_PORT_VALID(cs_uint16 port_id,
					 cs_uint32 * perr_code)
{
  if ( !(M2R_DRVR_IS_LOADED()) ) {
    *perr_code = EM2R_DRVR_NOT_LOADED ;
    return (FALSE) ;
  }

  if (port_id > M2R_MAX_NUM_PORTS) {
    *perr_code = EM2R_PORT_ID_OUT_OF_RANGE ;
    return (FALSE) ;
  }

  if (M2R_PORT_ID_TO_PCB_PTR(port_id) == NULL) {
    *perr_code = EM2R_PORT_UNINITIALIZED ;
    return (FALSE) ;
  }

  return (TRUE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    __M2R_IS_PORT_VALID(port_id, perr_code)  ;
  }
#endif
}

#define M2R_IS_PORT_VALID(port_id, err_code)        \
    ( __M2R_IS_PORT_VALID(port_id, &err_code) )


CS_INLINE CS_SEM *  M2R_GET_PORT_SEM_PTR(cs_uint16 port_id,
					 m2r_module_id_t mod_id)
{
  m2r_port_cb_t		* p = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  switch (mod_id) {
    case M2R_ID_MPIF  : return( &p->sem.mpif ) ;
    case M2R_ID_SPI42 : return( &p->sem.spi42 ) ;
    case M2R_ID_PPRBS : return( &p->sem.pprbs ) ;
    case M2R_ID_FRAMER: return( &p->sem.framer ) ;
    case M2R_ID_RMAC  : return( &p->sem.rmac ) ;
    case M2R_ID_XGXS  : return( &p->sem.xgxs ) ;
    case M2R_ID_ETH  : return( &p->sem.eth ) ;
    case M2R_ID_LIF   : return( &p->sem.lif  ) ;
    default :
      return (NULL) ; /* error */
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_PORT_SEM_PTR(port_id, mod_id) ;  ;
  }
#endif
}

CS_INLINE cs_int8 * M2R_GET_MOD_NAME(cs_uint16 mod_id)
{
  cs_int8 * mod_name = NULL ;
  switch(mod_id) {
      case M2R_ID_MPIF   : mod_name = "MPIF" ;
        break ;
      case M2R_ID_SPI42  : mod_name = "SPI4.2" ;
        break ;
      case M2R_ID_PPRBS  : mod_name = "PPRBS" ;
        break ;
      case M2R_ID_FRAMER : mod_name = "FRAMER" ;
        break ;
      case M2R_ID_RMAC   : mod_name = "RINGMAC" ;
        break ;
      case M2R_ID_XGXS   : mod_name = "XGXS" ;
        break ;
      case M2R_ID_ETH   : mod_name = "XETH" ;
        break ;
      case M2R_ID_LIF    : mod_name = "LIF" ;
        break ;
      default :
          mod_name = "Unknown block" ;
  }
  return (mod_name) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_MOD_NAME(mod_id) ;  ;
  }
#endif
}


CS_INLINE char * M2R_GET_MAC_MODE_NAME(m2r_port_mac_mode_t mm)
{
  switch (mm) {
    default :
    case M2R_PORT_IN_SRP_MODE: return("SRP") ;
    case M2R_PORT_IN_RPR_MODE: return("RPR") ;
    case M2R_PORT_IN_BYPASS_MODE: return("MAC bypass(Framer only)") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_MAC_MODE_NAME(mm) ;  ;
  }
#endif
}

CS_INLINE char * M2R_GET_LINE_RATE_NAME(m2r_port_line_rate_t lr)
{
  switch (lr) {
    default                  :
    case M2R_PORT_RATE_OC192C: return("OC-192c (concatanated)") ;
    case M2R_PORT_RATE_OC192 : return("OC-192 (4 oc48c streams)") ;
    case M2R_PORT_RATE_OC48C : return("OC-48c (concatanated)") ;
    case M2R_PORT_RATE_OC48  : return("OC-48 (4 oc12c streams)") ;
    case M2R_PORT_RATE_10G   : return("10 GigE") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_LINE_RATE_NAME(lr) ;
  }
#endif
}


CS_INLINE char * M2R_GET_L2_PROT_NAME(m2r_port_prot_t l2_prot)
{
  switch (l2_prot) {
    default                     :
    case M2R_PORT_PROT_POS      : return("POS") ;
    case M2R_PORT_PROT_PPP      : return("PPP") ;
    case M2R_PORT_PROT_ATM      : return("ATM") ;
    case M2R_PORT_PROT_GFP      : return("GFP") ;
    case M2R_PORT_PROT_CLEAR_CH : return("Clear Channel") ;
    case M2R_PORT_PROT_ETHERNET : return("Ethernet") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_L2_PROT_NAME(l2_prot) ;
  }
#endif
}

CS_INLINE char * M2R_GET_LIF_MODE_NAME(m2r_port_lif_mode_t lif)
{
  switch (lif) {
    default                     :
    case M2R_PORT_IF_SFI4_1     : return("SFI4.1") ;
    case M2R_PORT_IF_SFI4_2     : return("SFI4.2") ;
    case M2R_PORT_IF_XFP        : return("XFP") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_LIF_MODE_NAME(lif) ;
  }
#endif
}


CS_INLINE char * M2R_GET_MS_MODE_NAME(m2r_rmac_ms_mode_t ms)
{
  switch (ms) {
    default                            :
    case M2R_MS_SRP                    :
      return("SRP") ;
    case M2R_MS_RPR_AGGRESSIVE_DUAL_TB      :
      return("RPR Aggressive Dual TB mode") ;
    case M2R_MS_RPR_AGGRESSIVE_SINGLE_TB    :
      return("RPR Aggressive Single TB mode") ;
    case M2R_MS_RPR_CONSERVATIVE_DUAL_TB    :
      return("RPR Conservative Dual TB mode") ;
    case M2R_MS_RPR_CONSERVATIVE_SINGLE_TB  :
      return("RPR Conservative Single TB mode") ;
    case M2R_MS_UNKNOWN                :
      return("Unknown(not used)") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_MS_MODE_NAME(ms) ;
  }
#endif
}

#define  M2R_PORT_LOCK(port_id, mod_id)		{		\
    CS_SEM    * psem = M2R_GET_PORT_SEM_PTR(port_id, mod_id) ; 	\
                                                                \
    if ( CS_SEM_TAKE(psem) == CS_ERROR ) {		       	\
      CS_HNDL_ERROR(port_id, EM2R_PORT_SEM_TAKE_FAILED, NULL) ;	\
         return (CS_ERROR) ;				       	\
    }								\
}

#define M2R_PORT_UNLOCK(port_id, mod_id) {			\
     CS_SEM   * psem =  M2R_GET_PORT_SEM_PTR(port_id, mod_id) ;	\
                                                                \
     CS_SEM_GIVE(psem) ;					\
                                                                \
}


/******************************************************
 ******************      DEVICE     *******************
 * Before using any of the mapping functions, the     *
 * code should first check if the device is valid.    *
 ******************************************************/
/*
 * dev_id -> dev_num
 */
#define M2R_DEV_ID_TO_DEV_NUM(dev_id)	( (cs_uint16)(dev_id) )

/*
 * dev_id -> (devcb *)
 */
#define M2R_DEV_ID_TO_DEVCB_PTR(dev_id)				\
    ( &((m2r_get_dev_tbl())[ M2R_DEV_ID_TO_DEV_NUM(dev_id) ]) )

/*
 * dev_id -> device base addr
 */
#define M2R_DEV_ID_TO_DEV_BASE(dev_id)         	( (M2R_t *)    	\
    (M2R_DEV_ID_TO_DEVCB_PTR(dev_id))->base_addr )

/*
 * CHECK FOR VALID DEVICE
 * In the code, use the macro below instead of the inline function!
 */
CS_INLINE cs_boolean  __M2R_IS_DEVICE_VALID(cs_uint16 dev_id,
					    cs_uint32 * perr_code)
{
  m2r_dev_cb_t * pdevcb ;

  if ( !(M2R_DRVR_IS_LOADED()) ) {
    *perr_code = EM2R_DRVR_NOT_LOADED ;
    return (FALSE) ;
  }

  if (dev_id > CS_MAX_NUM_DEVICES) {
    *perr_code = EM2R_DEV_ID_OUT_OF_RANGE ;
    return (FALSE) ;
  }

  pdevcb = M2R_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  if (pdevcb->state == M2R_DEV_STATE_INVALID) {
    *perr_code = EM2R_DEV_NOT_REGD ;
    return (FALSE) ;
  }

  return (TRUE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    __M2R_IS_DEVICE_VALID(dev_id, perr_code) ;
  }
#endif
}

#define M2R_IS_DEVICE_VALID(dev_id, err_code)         \
   ( __M2R_IS_DEVICE_VALID(dev_id, &err_code) )

#define  M2R_DEV_LOCK(dev_id, mod_id)         {                 \
    CS_SEM    * psem = M2R_GET_DEV_SEM_PTR(dev_id, mod_id) ; 	\
                                                                \
    if ( CS_SEM_TAKE(psem) == CS_ERROR ) {                      \
      CS_HNDL_ERROR(dev_id, EM2R_DEV_SEM_TAKE_FAILED, NULL) ;   \
         return (CS_ERROR) ;                                    \
    }                                                           \
}

#define M2R_DEV_UNLOCK(dev_id, mod_id) {                        \
    CS_SEM    * psem = M2R_GET_DEV_SEM_PTR(dev_id, mod_id) ; 	\
                                                                \
    CS_SEM_GIVE(psem) ;                                         \
}

/*****************************************
 ************    DRIVER     **************
 *****************************************/
extern cs_boolean m2r_drvr_is_loaded(void) ;
extern cs_boolean m2r_drvr_is_esc_code_enbld(void) ;

/*****************************************
 ************    OTHERS     **************
 *****************************************/
/*
 * General in-line utility for getting max factor of two integers
 */
CS_INLINE cs_uint32 M2R_GET_MAX_SHARE_FACTOR(cs_uint32 num1,
					     cs_uint32 num2)
{
  cs_uint32 a, b, c=1;

  if ( num1 == 0 || num2 == 0 )
    return 0 ;

  if ( num1 == num2 )
    return num1 ;

  if ( num1 > num2 ) {
  	a = num1 ;
  	b = num2 ;
  } else {
  	a = num2 ;
  	b = num1 ;
  }

  while (c) {
    c = a % b ;
    a = b ;
    b = c ;
  }

  return ( a ) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_MAX_SHARE_FACTOR(num1, num2) ;
  }
#endif
}


/*****************************
 * UTILITIES TO GET MAC MODE *
 *****************************/
extern m2r_port_mac_mode_t  m2r_get_port_mac_mode(cs_uint16 port_id) ;
#define M2R_GET_MAC_MODE_FROM_PCB(port_id)        \
        ( (M2R_PORT_ID_TO_PCB_PTR(port_id))->summ.mac_mode )


#define M2R_GET_MAC_MODE_FROM_HW(port_id)         \
        ( m2r_get_port_mac_mode(port_id) )


CS_INLINE m2r_port_mac_mode_t  M2R_GET_MAC_MODE(cs_uint16 port_id)
{
#ifdef RELEASE_PLATFORM
  return ( M2R_GET_MAC_MODE_FROM_PCB(port_id) ) ;

#else /* internal use */
  return ( M2R_GET_MAC_MODE_FROM_HW(port_id) ) ;
#endif

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_MAC_MODE(port_id) ;
  }
#endif
}


/****************************
 * UTILITY TO GET LINE RATE *
 ****************************/
#define M2R_GET_LINE_RATE_FROM_PCB(port_id)	\
        ( (M2R_PORT_ID_TO_PCB_PTR(port_id))->summ.line_rate )

extern cs_boolean m2r_mpif_is_in_eth_mode(cs_uint16 port_id) ;
extern cs_boolean m2r_mpif_is_in_oc48_mode(cs_uint16 port_id) ;

CS_INLINE
m2r_port_line_rate_t M2R_GET_LINE_RATE_FROM_HW(cs_uint16 port_id)
{
  m2r_port_line_rate_t line_rate ;
  cs_boolean           framer_in_channel_mode = FALSE ;

  if ( m2r_mpif_is_in_eth_mode(port_id) ) {
    line_rate = M2R_PORT_RATE_10G ;
  }
  else {
    framer_in_channel_mode =
      m2r_framer_is_in_channelised_mode(port_id) ;
    if ( m2r_mpif_is_in_oc48_mode(port_id) ) {
      line_rate = ((framer_in_channel_mode) ?
          M2R_PORT_RATE_OC48 : M2R_PORT_RATE_OC48C) ;
    }
    else {
      line_rate = ((framer_in_channel_mode) ?
          M2R_PORT_RATE_OC192 : M2R_PORT_RATE_OC192C ) ;
    }
  }
  return (line_rate) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_LINE_RATE_FROM_HW(port_id) ;
  }
#endif
}

CS_INLINE m2r_port_line_rate_t M2R_GET_LINE_RATE(cs_uint16 port_id)
{
#ifdef RELEASE_PLATFORM
  return( M2R_GET_LINE_RATE_FROM_PCB(port_id) ) ;
#else
  return( M2R_GET_LINE_RATE_FROM_HW(port_id) ) ;
#endif

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_LINE_RATE(port_id) ;
  }
#endif
}


/*************************************
 * UTILITIES TO GET L2 PROTOCOL TYPE *
 *************************************/
#define M2R_GET_L2_PROT_FROM_PCB(port_id)        \
        ( (M2R_PORT_ID_TO_PCB_PTR(port_id))->summ.l2_prot )

CS_INLINE m2r_port_prot_t M2R_GET_L2_PROT_FROM_HW(cs_uint16 port_id)
{
  if ( m2r_mpif_is_in_eth_mode(port_id) ) {
    return (M2R_PORT_PROT_ETHERNET) ;
  }
  else {
    return m2r_framer_get_svc_type(port_id, 0) ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_L2_PROT_FROM_HW(port_id) ;
  }
#endif
}

/****************************
 * UTILITY TO GET LIF MODE  *
 ****************************/
#define M2R_GET_LIF_MODE_FROM_PCB(port_id)	\
        ( (M2R_PORT_ID_TO_PCB_PTR(port_id))->summ.lif_mode )

extern m2r_port_lif_mode_t m2r_lif_get_lif_mode(cs_uint16 port_id) ;

#define M2R_GET_LIF_MODE_FROM_HW(port_id)       \
             m2r_lif_get_lif_mode(port_id)

CS_INLINE m2r_port_lif_mode_t M2R_GET_LIF_MODE(cs_uint16 port_id)
{
#ifdef RELEASE_PLATFORM
  return ( M2R_GET_LIF_MODE_FROM_PCB(port_id) ) ;
#else
  return ( M2R_GET_LIF_MODE_FROM_HW(port_id) ) ;
#endif

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_LIF_MODE(port_id) ;
  }
#endif
}


/***********************************
 * UTILITY TO GET RINGMAC MS MODE  *
 ***********************************/
#define M2R_GET_RMAC_MS_MODE_FROM_PCB(port_id)   \
        ( (M2R_PORT_ID_TO_PCB_PTR(port_id))->summ.rmac_ms_mode )

extern
m2r_rmac_ms_mode_t m2r_ms_get_running_mode(cs_uint16 port_id) ;
#define M2R_GET_RMAC_MS_MODE_FROM_HW(port_id)    \
        ( m2r_ms_get_running_mode(port_id) )

CS_INLINE m2r_rmac_ms_mode_t M2R_GET_RMAC_MS_MODE(cs_uint16 port_id)
{
#ifdef RELEASE_PLATFORM
  return ( M2R_GET_RMAC_MS_MODE_FROM_PCB(port_id) ) ;
#else
  return ( M2R_GET_RMAC_MS_MODE_FROM_HW(port_id) ) ;
#endif

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_RMAC_MS_MODE(port_id) ;
  }
#endif
}


/**********************************************************
 * UTILITY TO GET SONET MODE                              *
 * Return TRUE if Sonet mode, else FALSE(for Ethernet).   *
 **********************************************************/
#define M2R_GET_SONET_MODE_FROM_PCB(PORTID) \
           (((M2R_PORT_ID_TO_PCB_PTR((PORTID)))->summ.l2_prot == \
              M2R_PORT_PROT_ETHERNET) ? FALSE : TRUE)

#define M2R_GET_SONET_MODE_FROM_HW(PORTID)  \
            (m2r_mpif_is_in_eth_mode((PORTID)) ? FALSE : TRUE)

CS_INLINE cs_boolean M2R_GET_SONET_MODE(cs_uint16 port_id)
{
#ifdef RELEASE_PLATFORM
  m2r_port_cb_t	* p ;
  p = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
  return (M2R_GET_SONET_MODE_FROM_PCB(port_id)) ;
#else
  return M2R_GET_SONET_MODE_FROM_HW(port_id) ;
#endif

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_SONET_MODE(port_id) ;
  }
#endif
}

#define M2R_IS_SONET_MODE(PORTID)    M2R_GET_SONET_MODE((PORTID))
#define M2R_IS_ETHERNET_MODE(PORTID) ( !(M2R_IS_SONET_MODE(PORTID)) )

/****************************************
 * UTILTITY TO CHECK TABLE/MEM BOUNDS   *
 ****************************************/
CS_INLINE cs_status M2R_TBL_BOUNDS_CHECK(cs_uint16 port_id,
					 cs_uint32 start, cs_uint32 num,
					 cs_uint32 tbl_size,
					 char * func_name,
					 cs_uint32 line_num)
{
  cs_uint32   err_code = 0 ;

  if( start >= tbl_size ) {
    err_code = EM2R_PORT_TBL_NUM_INVALID ;
  }
  else if( num > tbl_size ) {
    err_code = EM2R_PORT_TBL_NUM_INVALID ;
  }
  else if( (start+num) > tbl_size ) {
    err_code = EM2R_PORT_TBL_NUM_INVALID ;
  }

  if (err_code) {
    CS_HNDL_ERROR(port_id, err_code,
    ": start(0x%x), num(0x%x), tbl_size(0x%x), called from %s(), line %d\n",
		  start, num, tbl_size, func_name, line_num) ;
    return (CS_ERROR) ;
  }
  return (CS_OK) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_TBL_BOUNDS_CHECK(port_id, start, num, tbl_size, func_name,
			 line_num) ;
  }
#endif
}


/*********************************************************
 *************** STATS control block related function ****
 *********************************************************/
CS_INLINE m2r_port_stats_cb_t * M2R_GET_STATS_CB(cs_uint16 port_id)
{
  m2r_port_cb_t		* p = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if (!p) {
    return (NULL) ; /* error */
  }
  return (&p->stats_cb) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_STATS_CB(port_id) ;
  }
#endif
}

CS_INLINE m2r_port_stats_t * M2R_GET_STATS_PTR(cs_uint16 port_id)
{
  m2r_port_cb_t		* p = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if (!p) {
    return (NULL) ; /* error */
  }
  return (&p->stats) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_STATS_PTR(port_id) ;
  }
#endif
}

/*********************************************************
 *************** IRQ control block related function ****
 *********************************************************/
CS_INLINE m2r_port_irq_cb_t * M2R_GET_IRQ_CB(cs_uint16 port_id)
{
  m2r_port_cb_t		* p = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if (!p) {
    return (NULL) ; /* error */
  }
  return (&p->irq) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_IRQ_CB(port_id) ;
  }
#endif
}

/*********************************************************
 *************** LOG LEVEL related inline functions ******
 *********************************************************/
#define M2R_IS_DEBUG_ENABLED(DEV_NUM, MOD_ID)              \
  ((pM2R->dev_tbl[dev_num].debug_flag & (MOD_ID)) ? TRUE : FALSE)

CS_INLINE void CS_DEBUG(cs_uint16 dev_num, m2r_module_id_t mod_id,
                        cs_int8 * fmt, ...)
{
#ifndef CS_DONT_USE_VAR_ARGS
  va_list     VarArg;
  char        log_str[256];

  if (M2R_IS_DEBUG_ENABLED(dev_num, mod_id)) {
    va_start(VarArg, fmt);
    vsprintf(log_str, fmt, VarArg) ;
    va_end(VarArg) ;
    CS_PRINT(log_str) ;
  }

#else
  /* don't print anything for now */
#endif /* CS_DONT_USE_VAR_ARGS */

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_DEBUG(dev_num, mod_id, fmt) ;
  }
#endif
}


/***************************************************************
 ***************  OTHER GENERAL MACROS AND UTILITIES ***********
 ***************************************************************/

/*
 * Block and register offset(into M2R_t datastruct!) macros
 */
#define M2R_OFST(s_t, m)      ( (cs_uint32) (&(((s_t *)0)->m)) )

#define M2R_SLICE_OFST()      M2R_OFST(M2R_t, slice)

#define M2R_SLICE_SIZE          (sizeof(slice_t))

#define M2R_IS_SLICE_REG(r)                                     \
       (  ((r) >= M2R_SLICE_OFST()) &&                          \
          ((r) < (M2R_SLICE_OFST() + sizeof(slice_t))) )


/* SPI */
#define M2R_SPI42_OFST()      M2R_OFST(M2R_t, SPI4P2)
#define M2R_IS_SPI42_REG(r)                                     \
          ( ((r) >= M2R_SPI42_OFST()) &&                        \
            ((r) < (M2R_SPI42_OFST() + sizeof(SPI4P2_t))) )


#define M2R_SPI42_REG_OFST(r) (M2R_SPI42_OFST() + M2R_OFST(SPI4P2_t, r))

/* PPRBS */
#define M2R_PPRBS_OFST()      ( M2R_SLICE_OFST() +              \
                                M2R_OFST(slice_t, PPRBS) )
#define M2R_IS_PPRBS_REG(r)                                     \
          ( ((r) >= M2R_PPRBS_OFST()) &&                         \
            ((r) < (M2R_PPRBS_OFST() + sizeof(PPRBS_t))) )

#define M2R_PPRBS_REG_OFST(r)  (M2R_PPRBS_OFST() + M2R_OFST(PPRBS_t, r))

/* RingMAC */
#define M2R_RMAC_OFST()	      ( M2R_SLICE_OFST() +              \
			        M2R_OFST(slice_t, RingMAC) )
#define M2R_IS_RMAC_REG(r)                                      \
          ( ((r) >= M2R_RMAC_OFST()) &&                         \
            ((r) < (M2R_RMAC_OFST() + sizeof(RingMAC_t))) )

#define M2R_RMAC_REG_OFST(r)  (M2R_RMAC_OFST() + M2R_OFST(RingMAC_t, r))

#define M2R_LIF_REG_OFFSET(REG)     \
            (M2R_OFST(M2R_t, LIF) + M2R_OFST(LIF_t, REG))

#define M2R_LIF_PCS_REG_OFFSET(REG)                             \
            (M2R_OFST(M2R_t, LIF) + M2R_OFST(LIF_t, PCS6) +     \
             M2R_OFST(PCS6_t, REG))

#define M2R_LIF_XGXS_REG_OFFSET(REG)                            \
           ( M2R_OFST(M2R_t, LIF) + M2R_OFST(LIF_t, XGXS) +     \
              M2R_OFST(XGXS_t, REG) )

#define M2R_ETH_REG_OFFSET(REG)        \
        (M2R_OFST(M2R_t, XETH) + M2R_OFST(XETH_t, REG))


#define M2R_XGXS_REG_OFFSET(REG)        \
        ( M2R_OFST(M2R_t, XGXS) + M2R_OFST(XGXS_t, REG) )

#define  M2R_COPY_MAC_ADDR( BUF, BYTE_54, BYTE_32, BYTE_10 )  {          \
  (BUF)[0] = (BYTE_54) >> 8 ; (BUF)[1] = (BYTE_54) & 0xff ;              \
  (BUF)[2] = (BYTE_32) >> 8 ; (BUF)[3] = (BYTE_32) & 0xff ;              \
  (BUF)[4] = (BYTE_10) >> 8 ; (BUF)[5] = (BYTE_10) & 0xff ;              \
}


/*****************************************************************
 * Macro to get offset of Framer register
 *****************************************************************/
#define M2R_FR_REG_OFFSET(REG, STR, STRIDE)                       \
        ( M2R_OFST(M2R_t, slice) + M2R_OFST(slice_t, Framer) +    \
          M2R_OFST(Framer_t, REG) + (STR * STRIDE * sizeof(cs_reg)))

/***********************************
 *   Some externs                  *
 ***********************************/
extern
cs_uint16  m2r_get_crc16(cs_uint8 * buf, cs_uint32 len) ;
extern
cs_uint32  m2r_get_crc32(cs_uint8 * buf, cs_uint32 len) ;
extern
cs_uint16  m2r_get_srp_hdr(cs_uint8 ttl, cs_uint8 ri, cs_uint8 mode,
                           cs_uint8 prio) ;
extern
cs_uint16  m2r_get_rpr_hdr(cs_uint8 ttl, cs_uint8 ri, cs_uint8 fe,
                           cs_uint8 pt, cs_uint8 sc,
                           cs_uint8 we) ;
extern
cs_uint16  m2r_get_rpr_ext_hdr(cs_uint8 ttl, cs_uint8 ext, cs_uint8 ft,
                               cs_uint8 ws, cs_uint8 strict) ;
extern
cs_boolean m2r_rmac_mems_must_be_cleared(void) ;

/* Dev utilities */
extern
cs_boolean m2r_dev_is_shared_blk_init_done(cs_uint16 dev_id,
					   m2r_module_id_t mod_id) ;
extern
cs_uint16  m2r_dev_get_shared_blk_mstr_port_id(cs_uint16 dev_id,
					       m2r_module_id_t mod_id) ;
extern
void  m2r_dev_set_shared_blk_init_done(cs_uint16 dev_id,
				       m2r_module_id_t mod_id,
				       cs_uint16 port_id) ;
extern
cs_status  m2r_wait_for_bist_done(cs_uint16 dev_id, cs_boolean mbist,
				  cs_boolean sbist, cs_uint32 msec_timeout) ;
extern
cs_status  m2r_mpif_ctl_bypass_mode(cs_uint16 port_id, m2r_ctl_t ctl) ;
extern
cs_boolean m2r_mpif_is_in_mac_bypass(cs_uint16 port_id) ;
extern
cs_status  m2r_rmac_set_mac_mode_sel(cs_uint16 port_id,
				     m2r_port_mac_mode_t mode) ;
extern
m2r_port_mac_mode_t m2r_rmac_get_mac_mode_sel(cs_uint16 port_id) ;
extern
cs_status m2r_port_init_data(cs_uint16 port_id, m2r_port_cfg_t * pCfg) ;
extern
void m2r_sim_hreset_delay(cs_uint16 port_id, cs_uint32 delay) ;
extern
cs_status m2r_dump_port_cb(cs_uint16 port_id) ;

#ifdef POSIX_PTHREAD
/* Feature #c -> */
extern
cs_status m2r_start_debug_thread(cs_uint16 port_id,
				 cs_uint32 msec_poll_intvl,
				 cs_boolean send_rcvd_ctl_pkts,
				 cs_boolean send_rcvd_ips_pkts,
				 cs_boolean get_ips_msg_in_ttl_mode,
				 cs_boolean blk_print_stats,
				 cs_uint32  blk_irq_poll) ;
extern
cs_status m2r_stop_debug_thread(cs_uint16 port_id) ;
/* Feature #c <- */
#endif

cs_status  m2r_lif_setup_default(cs_uint16 port_id,
				 m2r_port_summ_t * pSumm) ;

/******************************************************
 ****************** SOME WORKAROUND *******************
 ******************************************************/
/*
 * Register read for #020204
 */
CS_INLINE  cs_reg CS_REG_2READ(volatile cs_reg * pReg)
{
  cs_reg ret ;
  cs_uint32 key ;

  key = CS_INTERRUPT_LOCK() ;
  ret = M2R_REG_READ(pReg) ;
  ret = M2R_REG_READ(pReg) ;
  CS_INTERRUPT_UNLOCK(key) ;

  return (ret) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    CS_REG_2READ(pReg) ;
  }
#endif
}

#endif /* __M2R_COMMON_H__ */














