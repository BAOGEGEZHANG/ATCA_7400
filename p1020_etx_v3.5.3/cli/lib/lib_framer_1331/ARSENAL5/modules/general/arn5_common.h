/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_common.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PRIVATE header file - for driver use only!
 * This contains defines, macros, inline functions, etc.. which
 * are used internally by code in other modules.
 */
#ifndef __ARN5_COMMON_H__
#define __ARN5_COMMON_H__

#ifndef CS_DONT_USE_VAR_ARGS
#       include <stdarg.h>
#endif

#include "cs_types.h"
#include "cs_utils.h"

#include "arn5_print.h"
#include "arn5_error.h"
#include "arn5_registers.h"
#include "arn5_cb.h"

#include "arn5_public.h"
#include "arn5_gen_data.h"



extern
#ifdef RELEASE_PLATFORM
const
#endif
cs_uint8        g_ARN5_ver[] ;


#define ARN5_DRVR_IS_LOADED() ( (g_pARN5) && (g_pARN5->drvr_loaded) )
#define ARN5_GET_DEVCB_TBL()    ( g_pARN5->p_dev_cb_tbl )

#define ARN5_RESET_MSEC_DELAY ( 2 )   /* 2 milli-seconds */

/* Forward declarations */
CS_INLINE cs_boolean  ARN5_IS_DEVICE_VALID(cs_uint8 dev_id,
                                          cs_uint32 * perr_code) ;

/*
 * Some basic defines
 */
#define ARSENAL5_JTAG_ID             0x1000F3E5
#define ARN5_IS_JTAG_ID_CORRECT(x)   ( x == ARSENAL5_JTAG_ID )
#define ARN5_CORE_CLOCK              (50)   /* MHz */
#define ARN5_GET_MODE_INFO_FROM_HW   1      /* else from PCB */

/******************************************************
 ******************      PORT      ********************
 * Before using any of the mapping functions, the     *
 * code should first check if the port is valid.      *
 ******************************************************/
/*
 * port_id -> dev_id
 */
#define ARN5_PORT_ID_TO_DEV_ID(port_id)                          \
         ( ((port_id) >> 8) & 0xff )

/*
 * port_id -> port_num
 */
#define ARN5_PORT_ID_TO_PORT_NUM(port_id)                        \
           ( (port_id) & 0xff )
/*
 * port_id -> slice_num
 */
#define ARN5_PORT_ID_TO_SLICE_NUM(port_id)                       \
            ARN5_PORT_ID_TO_PORT_NUM(port_id)

/*
 * port_id -> (devcb *)
 */
#define ARN5_PORT_ID_TO_DEVCB_PTR(port_id)                       \
    ( (ARN5_GET_DEVCB_TBL())[ ARN5_PORT_ID_TO_DEV_ID(port_id) ] )

/*
 * port_id -> (pcb *)
 */
#define ARN5_PORT_ID_TO_PCB_PTR(port_id)                         \
    ( (ARN5_PORT_ID_TO_DEVCB_PTR(port_id))->                     \
      ppcb[ ARN5_PORT_ID_TO_PORT_NUM(port_id) ] )

/*
 * port_id -> device base addr
 */
#define ARN5_PORT_ID_TO_DEV_BASE(port_id)        ( (ARN5_t *)     \
    (ARN5_PORT_ID_TO_DEVCB_PTR(port_id))->base_addr )

/*
 * dev_id, port_num -> port_id
 */
#define ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, port_num)           \
  ( (dev_id << 8) | port_num )


/*
 * port_id -> slice_num, base_addr
 */
CS_INLINE void ARN5_GET_PORT_PARAMS(cs_uint16 port_id,
                                   cs_uint16 * psl_num,
                                   ARN5_t ** pbase_addr)
{
  *psl_num      = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;
  *pbase_addr   = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_PORT_PARAMS(port_id, psl_num, pbase_addr) ;
  }
#endif
}

/*
 * CHECK FOR VALID PORT_ID
 * In the code, use the macro below instead of the inline function!
 */
CS_INLINE cs_boolean ARN5_IS_PORT_VALID(cs_uint16 port_id,
                                       cs_uint32 * perr_code)
{
  cs_uint8  port_num = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;
  cs_uint8  dev_id   = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  arn5_port_cb_t  * ppcb ;


  /* verify that device is valid before checking for port validity */
  if ( !(ARN5_IS_DEVICE_VALID(dev_id, perr_code)) ) {
    *perr_code = EARN5_PORT_INVALID_USER_ARG ;
    return (FALSE) ;
  }

  if (port_num >= ARN5_MAX_NUM_PORTS) {
    *perr_code = EARN5_PORT_ID_OUT_OF_RANGE ;
    return (FALSE) ;
  }

  ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;
  if ( (ppcb == NULL) ||
       (ppcb->state == ARN5_PORT_STATE_INVALID) ) {
    *perr_code = EARN5_PORT_NOT_OPEN ;
    return (FALSE) ;
  }

  return (TRUE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_IS_PORT_VALID(port_id, perr_code)  ;
  }
#endif
}


CS_INLINE CS_SEM *  ARN5_GET_PORT_SEM_PTR(cs_uint16 port_id,
                                         arn5_module_id_t mod_id)
{
  arn5_port_cb_t         * p = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  switch (mod_id) {
    case ARN5_ID_FRAMER  : return( &p->sem.framer ) ;
    case ARN5_ID_LIF     : return( &p->sem.lif ) ;
    case ARN5_ID_ETH     : return( &p->sem.eth ) ;
    case ARN5_ID_PPRBS   : return( &p->sem.pprbs ) ;
    default :
      return (NULL) ; /* error */
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_PORT_SEM_PTR(port_id, mod_id) ;
  }
#endif
}

CS_INLINE cs_int8 * ARN5_GET_MOD_NAME(cs_uint16 mod_id)
{
  cs_int8 * mod_name = NULL ;
  switch(mod_id) {
    case ARN5_ID_MPIF     : mod_name = "MPIF" ;    break ;
    case ARN5_ID_HOST_IF  : mod_name = "HOST_IF" ; break ;
    case ARN5_ID_FRAMER   : mod_name = "FRAMER"  ; break ;
    case ARN5_ID_LIF      : mod_name = "LIF"     ; break ;
    case ARN5_ID_ETH      : mod_name = "GIGE"    ; break ;
    case ARN5_ID_PPRBS    : mod_name = "PPRBS"   ; break ;

    default              : mod_name = "???" ;     break ;
  }
  return (mod_name) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_MOD_NAME(mod_id) ;  ;
  }
#endif
}


CS_INLINE cs_uint8 * ARN5_GET_LINE_RATE_NAME(arn5_port_line_rate_t lr)
{
  switch (lr) {
    default                  :
    case ARN5_PORT_RATE_OC48C : return("OC-48c (concatanated)") ;
    case ARN5_PORT_RATE_OC12C : return("OC-12c (concatanated)") ;
    case ARN5_PORT_RATE_OC3C  : return("OC-3c (concatanated)") ;
    case ARN5_PORT_RATE_GIGE  : return("1 GigE") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_LINE_RATE_NAME(lr) ;
  }
#endif
}

CS_INLINE cs_uint8 * ARN5_GET_L2_PROT_NAME(arn5_port_prot_t l2_prot)
{
  switch (l2_prot) {
    default                     :
    case ARN5_PORT_PROT_POS      : return("POS") ;
    case ARN5_PORT_PROT_PPP      : return("PPP") ;
    case ARN5_PORT_PROT_LAPS     : return("LAPS") ;
    case ARN5_PORT_PROT_ATM      : return("ATM") ;
    case ARN5_PORT_PROT_GFP      : return("GFP") ;
    case ARN5_PORT_PROT_CLEAR_CH : return("Clear Channel") ;
    case ARN5_PORT_PROT_ETHERNET : return("Ethernet") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_L2_PROT_NAME(l2_prot) ;
  }
#endif
}

CS_INLINE cs_uint8 * ARN5_GET_HOST_IF_NAME(arn5_host_if_t host_if)
{
  switch (host_if) {
    default                     :
    case ARN5_HOST_IF_SPI42      : return("SPI4.2") ;
    case ARN5_HOST_IF_SPI3       : return("SPI3") ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_HOST_IF_NAME(lif) ;
  }
#endif
}


#define  ARN5_PORT_LOCK(port_id, mod_id)         {               \
    CS_SEM * psem = ARN5_GET_PORT_SEM_PTR(port_id, mod_id) ;     \
                                                                \
    if ( CS_SEM_TAKE(psem) != CS_OK ) {                      \
      CS_HNDL_ERROR(port_id, EARN5_PORT_SEM_TAKE_FAILED, NULL) ; \
         return (CS_ERROR) ;                                    \
    }                                                           \
}

#define ARN5_PORT_UNLOCK(port_id, mod_id) {                      \
    CS_SEM * psem =  ARN5_GET_PORT_SEM_PTR(port_id, mod_id) ;    \
    CS_SEM_GIVE(psem) ;                                         \
}


/******************************************************
 ******************      DEVICE     *******************
 * Before using any of the mapping functions, the     *
 * code should first check if the device is valid.    *
 ******************************************************/
/*
 * dev_id -> (devcb *)
 */
#define ARN5_DEV_ID_TO_DEVCB_PTR(dev_id)                         \
    ( (ARN5_GET_DEVCB_TBL())[dev_id] )

/*
 * dev_id -> device base addr
 */
#define ARN5_DEV_ID_TO_DEV_BASE(dev_id)          ( (ARN5_t *)     \
    (ARN5_DEV_ID_TO_DEVCB_PTR(dev_id))->base_addr )

/*
 * CHECK FOR VALID DEVICE
 * In the code, use the macro below instead of the inline function!
 */
CS_INLINE cs_boolean  ARN5_IS_DEVICE_VALID(cs_uint8 dev_id,
                                          cs_uint32 * perr_code)
{
  arn5_dev_cb_t * pdevcb ;

  if ( !(ARN5_DRVR_IS_LOADED()) ) {
    *perr_code = EARN5_DRVR_NOT_LOADED ;
    return (FALSE) ;
  }

  if (dev_id >= ARN5_MAX_NUM_DEVICES) {
    *perr_code = EARN5_DEV_ID_OUT_OF_RANGE ;
    return (FALSE) ;
  }

  pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  if ( (pdevcb == NULL) || (pdevcb->state == ARN5_DEV_STATE_INVALID) ) {
    *perr_code = EARN5_DEV_NOT_REGD ;
    return (FALSE) ;
  }

  return (TRUE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_IS_DEVICE_VALID(dev_id, perr_code) ;
  }
#endif
}


/* Semaphores at device-level  */
CS_INLINE CS_SEM *  ARN5_GET_DEV_SEM_PTR(cs_uint8 dev_id,
                                        arn5_module_id_t mod_id)
{
  arn5_dev_cb_t  * pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  switch (mod_id) {
    default :
    case ARN5_ID_MPIF    : return( &pdevcb->sem.mpif )  ; break ;
    case ARN5_ID_HOST_IF : return( &pdevcb->sem.host_if ) ; break ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_DEV_SEM_PTR(dev_id, mod_id) ;
  }
#endif
}

#define  ARN5_DEV_LOCK(dev_id, mod_id)         {                 \
    CS_SEM * psem = ARN5_GET_DEV_SEM_PTR(dev_id, mod_id) ;       \
                                                                \
    if ( CS_SEM_TAKE(psem) != CS_OK ) {                      \
      CS_HNDL_ERROR(dev_id, EARN5_DEV_SEM_TAKE_FAILED, NULL) ;   \
         return (CS_ERROR) ;                                    \
    }                                                           \
}

#define ARN5_DEV_UNLOCK(dev_id, mod_id) {                        \
    CS_SEM * psem = ARN5_GET_DEV_SEM_PTR(dev_id, mod_id) ;       \
    CS_SEM_GIVE(psem) ;                                         \
}


/*****************************************
 ************    DRIVER     **************
 *****************************************/
CS_INLINE cs_boolean arn5_drvr_is_stats_in_hex_fmt(void)
{
  if ( ARN5_DRVR_IS_LOADED() ) {
    return( (g_pARN5->drvr_flag & ARN5_DRVR_STATS_IN_HEX) ? TRUE
            : FALSE) ;
  }

  return (FALSE) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    arn5_drvr_is_stats_in_hex_fmt() ;
  }
#endif
}


CS_INLINE cs_boolean arn5_drvr_is_mask_zero_cntr(void)
{
   if ( ARN5_DRVR_IS_LOADED() ) {
     return( (g_pARN5->drvr_flag & ARN5_DRVR_STATS_MASK_ZERO) ? TRUE
             : FALSE ) ;
   }
   else {
    return (FALSE) ;
   }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    arn5_drvr_is_mask_zero_cntr() ;
  }
#endif
}

CS_INLINE cs_boolean arn5_drvr_is_esc_code_enbld(void)
{
  if ( ARN5_DRVR_IS_LOADED() ) {
    return( (g_pARN5->drvr_flag & ARN5_DRVR_ESC_CODE_PRINT) ? TRUE
            : FALSE ) ;
  }
  else {
    return (FALSE) ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    arn5_drvr_is_esc_code_enbld() ;
  }
#endif
}

/****************************
 * UTILITY TO GET LINE RATE *
 ****************************/
#define ARN5_GET_LINE_RATE_FROM_PCB(port_id)     \
        ( (ARN5_PORT_ID_TO_PCB_PTR(port_id))->summ.line_rate )

extern
cs_boolean  arn5_mpif_is_in_gige_mode(cs_uint16 port_id) ;
extern
cs_boolean  arn5_mpif_is_in_oc48_mode(cs_uint16 port_id) ;
extern
cs_boolean  arn5_mpif_is_in_oc12_mode(cs_uint16 port_id) ;
extern
cs_boolean  arn5_mpif_is_in_oc3_mode(cs_uint16 port_id) ;


CS_INLINE
arn5_port_line_rate_t ARN5_GET_LINE_RATE_FROM_HW(cs_uint16 port_id)
{
  arn5_port_line_rate_t line_rate ;

  if ( arn5_mpif_is_in_gige_mode(port_id) ) {
    line_rate = ARN5_PORT_RATE_GIGE ;
  }
  else if (arn5_mpif_is_in_oc48_mode(port_id)) {
    line_rate = ARN5_PORT_RATE_OC48C ;
  }
  else {
    line_rate = arn5_mpif_is_in_oc12_mode(port_id) ?
      ARN5_PORT_RATE_OC12C : ARN5_PORT_RATE_OC3C ;
  }

  return (line_rate) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_LINE_RATE_FROM_HW(port_id) ;
  }
#endif
}


CS_INLINE arn5_port_line_rate_t ARN5_GET_LINE_RATE(cs_uint16 port_id)
{
#if (ARN5_GET_MODE_INFO_FROM_HW)
  return( ARN5_GET_LINE_RATE_FROM_HW(port_id) ) ;
#else
  return( ARN5_GET_LINE_RATE_FROM_PCB(port_id) ) ;
#endif

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_LINE_RATE(port_id) ;
  }
#endif
}


/*************************************
 * UTILITIES TO GET L2 PROTOCOL TYPE *
 *************************************/
#define ARN5_GET_L2_PROT_FROM_PCB(port_id)        \
        ( (ARN5_PORT_ID_TO_PCB_PTR(port_id))->summ.l2_prot )

CS_INLINE arn5_port_prot_t ARN5_GET_L2_PROT_FROM_HW(cs_uint16 port_id)
{

  if ( arn5_mpif_is_in_gige_mode(port_id) ) {
    return (ARN5_PORT_PROT_ETHERNET) ;
  }
  else {
    return arn5_framer_get_svc_type(port_id) ;
  }

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_L2_PROT_FROM_HW(port_id) ;
  }
#endif
}

/**********************************************************
 * UTILITY TO GET SONET MODE                              *
 * Return TRUE if Sonet mode, else FALSE(for Ethernet).   *
 **********************************************************/
#define ARN5_GET_SONET_MODE_FROM_PCB(PORTID) \
           (((ARN5_PORT_ID_TO_PCB_PTR((PORTID)))->summ.l2_prot == \
              ARN5_PORT_PROT_ETHERNET) ? FALSE : TRUE)

#define ARN5_GET_SONET_MODE_FROM_HW(PORTID)  \
            (arn5_mpif_is_in_gige_mode((PORTID)) ? FALSE : TRUE)

CS_INLINE cs_boolean ARN5_GET_SONET_MODE(cs_uint16 port_id)
{
#if (ARN5_GET_MODE_INFO_FROM_HW)
  return ARN5_GET_SONET_MODE_FROM_HW(port_id) ;
#else
  arn5_port_cb_t * p ;
  p = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;
  return (ARN5_GET_SONET_MODE_FROM_PCB(port_id)) ;
#endif

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_SONET_MODE(port_id) ;
  }
#endif
}

#define ARN5_IS_SONET_MODE(PORTID)    ARN5_GET_SONET_MODE((PORTID))
#define ARN5_IS_ETHERNET_MODE(PORTID) ( !(ARN5_GET_SONET_MODE((PORTID))) )


/****************************************
 * UTILTITY TO CHECK TABLE/MEM BOUNDS   *
 ****************************************/
CS_INLINE cs_status ARN5_DEV_TBL_BOUNDS_CHECK(cs_uint8 dev_id,
                                         cs_uint32 start, cs_uint32 num,
                                         cs_uint32 tbl_size,
                                         cs_uint8 * func_name,
                                         cs_uint32 line_num)
{
  cs_uint32   err_code = 0 ;

  if( start >= tbl_size ) {
    err_code = EARN5_DEV_TBL_NUM_INVALID ;
  }
  else if( num > tbl_size ) {
    err_code = EARN5_DEV_TBL_NUM_INVALID ;
  }
  else if( (start+num) > tbl_size ) {
    err_code = EARN5_DEV_TBL_NUM_INVALID ;
  }

  if (err_code) {
    CS_HNDL_ERROR(dev_id, err_code,
    ": start(0x%x), num(0x%x), tbl_size(0x%x), called from %s(), line %d\n",
                  start, num, tbl_size, func_name, line_num) ;
    return (CS_ERROR) ;
  }
  return (CS_OK) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_DEV_TBL_BOUNDS_CHECK(dev_id, start, num, tbl_size, func_name,
                         line_num) ;
  }
#endif
}


CS_INLINE cs_status ARN5_PORT_TBL_BOUNDS_CHECK(cs_uint16 port_id,
                                         cs_uint32 start, cs_uint32 num,
                                         cs_uint32 tbl_size,
                                         cs_uint8 * func_name,
                                         cs_uint32 line_num)
{
  cs_uint32   err_code = 0 ;

  if( start >= tbl_size ) {
    err_code = EARN5_PORT_TBL_NUM_INVALID ;
  }
  else if( num > tbl_size ) {
    err_code = EARN5_PORT_TBL_NUM_INVALID ;
  }
  else if( (start+num) > tbl_size ) {
    err_code = EARN5_PORT_TBL_NUM_INVALID ;
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
    ARN5_PORT_TBL_BOUNDS_CHECK(port_id, start, num, tbl_size, func_name,
                         line_num) ;
  }
#endif
}


/*********************************************************
 *************** STATS control block related function ****
 *********************************************************/
CS_INLINE arn5_dev_stats_cb_t * ARN5_DEV_GET_STATS_CB(cs_uint8 dev_id)
{
  arn5_dev_cb_t  * p = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if (!p) return (NULL) ; /* error */
  return (&p->stats_cb) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_DEV_GET_STATS_CB(dpid) ;
  }
#endif
}


CS_INLINE arn5_port_stats_cb_t * ARN5_PORT_GET_STATS_CB(cs_uint16 port_id)
{
  arn5_port_cb_t  * p = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  if (!p) return (NULL) ; /* error */
  return (&p->stats_cb) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_PORT_GET_STATS_CB(dpid) ;
  }
#endif
}


CS_INLINE arn5_dev_stats_t * ARN5_DEV_GET_STATS_PTR(cs_uint8 dev_id)
{
  arn5_dev_cb_t    * p = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if (!p) {
    return (NULL) ; /* error */
  }
  return (&p->stats) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_DEV_GET_STATS_PTR(dev_id) ;
  }
#endif
}


CS_INLINE arn5_port_stats_t * ARN5_PORT_GET_STATS_PTR(cs_uint16 port_id)
{
  arn5_port_cb_t         * p = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  if (!p) {
    return (NULL) ; /* error */
  }
  return (&p->stats) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_PORT_GET_STATS_PTR(port_id) ;
  }
#endif
}

/*********************************************************
 *************** IRQ control block related function ****
 *********************************************************/
CS_INLINE arn5_dev_irq_cb_t * ARN5_GET_DEV_IRQ_CB(cs_uint8 dev_id)
{
  arn5_dev_cb_t * p = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if (!p) {
    return (NULL) ; /* error */
  }
  return (&p->irq) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_DEV_IRQ_CB(dev_id) ;
  }
#endif
}

CS_INLINE arn5_port_irq_cb_t * ARN5_GET_PORT_IRQ_CB(cs_uint16 port_id)
{
  arn5_port_cb_t * p = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  if (!p) {
    return (NULL) ; /* error */
  }
  return (&p->irq) ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_PORT_IRQ_CB(port_id) ;
  }
#endif
}

/*********************************************************
 *************** LOG LEVEL related inline functions ******
 *********************************************************/
#define ARN5_IS_DEBUG_ENABLED(DEV_ID, MOD_ID)              \
  ( ((ARN5_DEV_ID_TO_DEVCB_PTR(DEV_ID))->debug_flag & (MOD_ID)) ? TRUE : FALSE )


CS_INLINE void CS_DEBUG(cs_uint16 dev_id, arn5_module_id_t mod_id,
                        cs_int8 * fmt, ...)
{
#ifndef CS_DONT_USE_VAR_ARGS
  va_list     VarArg;
  char        log_str[256];

  if (ARN5_IS_DEBUG_ENABLED(dev_id, mod_id)) {
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
    CS_DEBUG(dev_id, mod_id, fmt) ;
  }
#endif
}

/***************************************************************
 ***************  OTHER GENERAL MACROS AND UTILITIES ***********
 ***************************************************************/
#define ARN5_OFST(s_t, m)        ( (cs_uint32) (&(((s_t *)0)->m)) )

#define ARN5_MPIF_OFST()         ARN5_OFST(ARN5_t, MicroIF)
#define ARN5_IS_MPIF_REG(r)      ( ((r) >= ARN5_MPIF_OFST()) &&   \
                ((r) < (ARN5_MPIF_OFST() + sizeof(ARN5_MicroIF_t))) )
#define ARN5_MPIF_REG_OFST(r)    ( ARN5_MPIF_OFST() +             \
                                  ARN5_OFST(ARN5_MicroIF_t, r) )

/* SPI4P2 */
#define ARN5_SPI_OFST()          ARN5_OFST(ARN5_t, SPI4P2)
#define ARN5_SPI_REG_OFST(r)     ( ARN5_SPI_OFST() +              \
                                ARN5_OFST(ARN5_SPI4P2_t, r) )
#define ARN5_IS_SPI_REG(r)       ( ((r) >= ARN5_SPI_OFST()) &&  \
                ((r) < (ARN5_SPI_OFST() + sizeof(ARN5_SPI4P2_t))) )
#define ARN5_SPIA_OFST()         ARN5_OFST(ARN5_t, SPI4P2a)
#define ARN5_SPIA_REG_OFST(r)    ( ARN5_SPIA_OFST() +               \
                                  ARN5_OFST(ARN5_SPI4P2a_t, r) )
/* SPIA - TXCH */
#define ARN5_SPIA_TXCH_OFST()    ARN5_OFST(ARN5_t, SPI4P2a.almost_full)
#define ARN5_SPIA_TXCH_INSTANCES (8)
#define ARN5_IS_SPIA_TXCH_REG(r) ( ((r) >= ARN5_SPIA_TXCH_OFST()) &&  \
                ((r) < (ARN5_SPIA_TXCH_OFST() + 2 * sizeof(cs_reg))) )

/* slice */
#define ARN5_SLICE_OFST()        ARN5_OFST(ARN5_t, slice)
#define ARN5_SLICE_SIZE          ( sizeof(ARN5_slice_t) )
#define ARN5_NUM_OF_SLICES       8
#define ARN5_IS_SLICE_REG(r)                                     \
                (  ((r) >= ARN5_SLICE_OFST()) &&                 \
                   ((r) < (ARN5_SLICE_OFST() +                   \
                     ARN5_SLICE_SIZE * ARN5_NUM_OF_SLICES)) )

/* LIF */
#define ARN5_LIF_OFST()          ARN5_OFST(ARN5_t, slice[0].LIF)
#define ARN5_LIF_REG_OFST(r)     ( ARN5_LIF_OFST() +              \
                                  ARN5_OFST(ARN5_LIF_t, r) )
/* Framer */
#define ARN5_FR_OFST()           ARN5_OFST(ARN5_t, slice[0].Framer)
#define ARN5_FR_REG_OFST(r, s_no, s_size)                        \
                ( ARN5_FR_OFST() + ARN5_OFST(ARN5_Framer_t, r) +   \
                (s_no * s_size * sizeof(cs_reg)) )

/* GigE */
#define ARN5_ETH_OFST()          ARN5_OFST(ARN5_t, slice[0].GigE)
#define ARN5_ETH_REG_OFST(r)     ( ARN5_ETH_OFST() +              \
                                  ARN5_OFST(ARN5_GigE_t, r) )
#define ARN5_ETH_EI_OFST()       ARN5_OFST(ARN5_t, slice[0].GigE.EI)
#define ARN5_ETH_EI_REG_OFST(r)  ( ARN5_ETH_EI_OFST() +           \
                                  ARN5_OFST(ARN5_EI_t, r) )
#define ARN5_IS_ETH_EI_REG(r)    ( ((r) >= ARN5_ETH_EI_OFST()) && \
                ((r) < (ARN5_ETH_EI_OFST() + sizeof(ARN5_EI_t))) )

/* PPRBS */
#define ARN5_PPRBS_OFST()        ARN5_OFST(ARN5_t, slice[0].PPRBS)
#define ARN5_PPRBS_REG_OFST(r)   ( ARN5_PPRBS_OFST() +            \
                                ARN5_OFST(ARN5_PPRBS_t, r) )



#define  ARN5_COPY_MAC_ADDR( BUF, BYTE_54, BYTE_32, BYTE_10 )  { \
  (BUF)[0] = (BYTE_54) >> 8 ; (BUF)[1] = (BYTE_54) & 0xff ;     \
  (BUF)[2] = (BYTE_32) >> 8 ; (BUF)[3] = (BYTE_32) & 0xff ;     \
  (BUF)[4] = (BYTE_10) >> 8 ; (BUF)[5] = (BYTE_10) & 0xff ;     \
}

/* exit a function upon detecting invalid device id */
#define  ARN5_INVALID_DEV_EXIT(dev_id, ret_val)                  \
{                                                               \
  cs_uint32             err_code = 0 ;                          \
                                                                \
  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {            \
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;                     \
    return(ret_val) ;                                           \
  }                                                             \
}

/* exit a function upon detecting invalid port id */
#define  ARN5_INVALID_PORT_EXIT(port_id, ret_val)                \
{                                                               \
  cs_uint32             err_code = 0 ;                          \
                                                                \
  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {             \
    CS_HNDL_ERROR(port_id, err_code, NULL) ;                    \
    return(ret_val) ;                                           \
  }                                                             \
}

/* Get the most significant bit in binary */
CS_INLINE cs_int8 ARN5_GET_MSB_POS(cs_uint32 data)
{
  cs_uint32 w = data ;
  cs_int8  i = 0 ;

  while ( w ) {
    w = w >> 1 ;
    i++;
  }
  return(i - 1) ;
}

/*
 * Delay utilities
 */
extern void arn5_sim_delay(cs_uint8 dev_id, cs_boolean is_in_msec,
                          cs_uint32 delay) ;

#define ARN5_UDELAY(dev_id, usecs)  {                            \
  if ( CS_IN_SIM_ENV() ) arn5_sim_delay(dev_id, FALSE, usecs) ;  \
  else CS_UDELAY(usecs) ;                                       \
}

#define ARN5_MDELAY(dev_id, msecs)            {                 \
  if ( CS_IN_SIM_ENV() ) arn5_sim_delay(dev_id, TRUE, msecs) ;  \
  else CS_MDELAY(msecs) ;                                      \
}


/****************************************************************
 ****************************************************************
 *   EXTERNS FOR UTILITIES AND INTERNAL FUNCTIONS               *
 ****************************************************************
 ****************************************************************/

/*
 *      Miscellaneous
 */
extern
cs_uint16  arn5_get_crc16(cs_uint8 * buf, cs_uint32 len) ;
extern
cs_uint32  arn5_get_crc32(cs_uint8 * buf, cs_uint32 len) ;


/*
 * DRIVER Utilities
 */


/*
 * DEVICE utilities
 */
extern
cs_status  arn5_dev_create_port_id(cs_uint8 dev_id,
                                  arn5_port_summ_t * pSumm,
                                  cs_uint16 * port_id,
                                  arn5_port_cb_t ** p_ppcb,
                                  cs_uint8 slice_num) ;
extern
void arn5_dev_delete_port_id(cs_uint16 port_id) ;

extern
cs_uint32  arn5_dev_get_state(cs_uint8 dev_id) ;


/*
 * MPIF utilities
 */
extern
cs_status  arn5_wait_for_bist_done(cs_uint8 dev_id, cs_boolean mbist,
                        cs_boolean sbist, cs_uint32 msec_timeout) ;
extern
cs_status  arn5_slice_hard_reset(cs_uint8 dev_id, cs_uint8 slice_num,
                        arn5_module_id_t mid,
                        cs_reset_action_t act) ;


#ifdef POSIX_PTHREAD
extern
cs_status arn5_start_irq_polling(cs_uint8 dev_id,
                                 cs_uint32 msec_poll_intvl,
                                 cs_uint32 blk_id) ;
extern
cs_status arn5_stop_irq_polling(cs_uint8 dev_id) ;
#endif /* POSIX_PTHREAD */

#endif /* __ARN5_COMMON_H__ */

