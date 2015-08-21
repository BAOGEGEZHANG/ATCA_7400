/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_priv.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PRIVATE header file.
 */
#ifndef __ARN5_FRAMER_PRIV_H__
#define __ARN5_FRAMER_PRIV_H__

#include "cs_types.h"

/*Fill device base address and slice number for the given port. */
#define ARN5_FR_GET_DEVICE_ADDR(PORTID, BASE_ADDR, SLICE, RETCODE) \
{                                                                 \
  cs_uint32 error;                                                \
  if (ARN5_IS_PORT_VALID(PORTID, &error) != TRUE) {                \
    CS_HNDL_ERROR((PORTID), error, ": port_id = %d\n", (PORTID)); \
    return (RETCODE) ;                                            \
  }                                                               \
  if (ARN5_IS_SONET_MODE((PORTID)) == FALSE) {                     \
    CS_HNDL_ERROR((PORTID), EARN5_INVALID_IN_ETH_MODE,             \
                  "Framer is powered DOWN in ethernet Mode\n") ;  \
    return (RETCODE) ;                                            \
  }                                                               \
  (BASE_ADDR) = ARN5_PORT_ID_TO_DEV_BASE((PORTID));                \
  (SLICE)     = ARN5_PORT_ID_TO_SLICE_NUM((PORTID));               \
}


/* Set rx_dir and tx_dir depending upon the dir_ctl passed. */
#define ARN5_FR_PARSE_DIR_CTL(RX_DIR, TX_DIR, DIR_CTL)           \
  (RX_DIR) = TRUE ;                                             \
  (TX_DIR) = TRUE ;                                             \
  if ((DIR_CTL) == CS_TX) {                                     \
      (RX_DIR) = FALSE;                                         \
  }                                                             \
  else                                                          \
  if ((DIR_CTL) == CS_RX) {                                     \
      (TX_DIR) = FALSE;                                         \
  }


#define ARN5_FR_TXHDLC__STRIDE   (ARN5_FR_TXHDLC_STRIDE)
#define ARN5_FR_RXHDLC__STRIDE   (ARN5_FR_RXHDLC_STRIDE)



/*********************************************************
 * Framer Private APIs
 *********************************************************/
extern
cs_boolean  arn5_framer_pkt_is_x86_enabled(cs_uint16 port_id) ;
extern
cs_status  arn5_framer_init_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status  arn5_framer_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status  arn5_framer_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;
extern
cs_status  arn5_framer_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;


/*****************************************************************
 * Framer IRQ API
 *****************************************************************/
extern
cs_status  arn5_framer_get_default_irq_cfg(cs_uint16 port_id,
                        arn5_port_summ_t * pSumm,
                        arn5_framer_cfg_t * pCfg) ;

extern
cs_status  arn5_framer_init_irq_cfg(cs_uint16 port_id,
                        arn5_port_summ_t * pSumm,
                        arn5_framer_cfg_t * pCfg) ;

extern cs_status  arn5_framer_isr(cs_uint16 port_id) ;


/*****************************************************************
 * Framer STATS API
 *****************************************************************/
extern
cs_status  arn5_framer_init_stats(cs_uint16 port_id,
                        arn5_framer_svc_type_t svc) ;
extern
cs_status  arn5_framer_init_strm_stats(cs_uint16 port_id,
                        arn5_framer_svc_type_t svc) ;

#endif /* __ARN5_FRAMER_PRIV_H__ */

