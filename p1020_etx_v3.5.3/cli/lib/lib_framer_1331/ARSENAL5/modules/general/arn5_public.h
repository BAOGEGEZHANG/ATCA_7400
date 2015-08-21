/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_public.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains basic API argument data types, defines, etc..
 */
#ifndef __ARN5_PUBLIC_H__
#define __ARN5_PUBLIC_H__

#include "cs_types.h"


#define ARN5_INVALID_PORT_ID     (0xFFFF)
#define ARN5_INVALID_CHAN_NUM    (0xFF)
#define ARN5_MAX_NUM_DEVICES     (1)  /* can be bumped up to 256 */
#define ARN5_MAX_NUM_CHANNELS    (8)


typedef enum {
  ARN5_HOST_IF_SPI42    = 0,
  ARN5_HOST_IF_SPI3,
} arn5_host_if_t ;


/*
 * Device configuration summary
 */
typedef struct {
  arn5_host_if_t        host_if ;
} arn5_dev_summ_t ;

typedef enum {
  ARN5_PORT_RATE_OC48C = 0,
  ARN5_PORT_RATE_OC12C,
  ARN5_PORT_RATE_OC3C,
  ARN5_PORT_RATE_GIGE,
} arn5_port_line_rate_t ;

typedef enum {
  ARN5_PORT_PROT_POS = 0,
  ARN5_PORT_PROT_PPP,
  ARN5_PORT_PROT_LAPS,
  ARN5_PORT_PROT_ATM,
  ARN5_PORT_PROT_GFP,
  ARN5_PORT_PROT_CLEAR_CH,     /* clear channel */
  ARN5_PORT_PROT_ETHERNET,
} arn5_port_prot_t ;


typedef enum {
  ARN5_ID_MPIF        = 0x0001,
  ARN5_ID_HOST_IF     = 0x0002,
  ARN5_ID_FRAMER      = 0x0004,
  ARN5_ID_LIF         = 0x0008,
  ARN5_ID_ETH         = 0x0010,
  ARN5_ID_PPRBS       = 0x0020,
  ARN5_ID_ALL_BLOCKS  = 0x00FF,
} arn5_module_id_t ;

#define ARN5_DEV_SHRD_BLKS_ID  (ARN5_ID_HOST_IF | ARN5_ID_MPIF)
#define ARN5_PORT_BLKS_ID      (ARN5_ID_FRAMER | ARN5_ID_LIF |  \
                                ARN5_ID_ETH | ARN5_ID_PPRBS)


typedef struct {
  arn5_port_line_rate_t  line_rate ;
  arn5_port_prot_t       l2_prot ;
} arn5_port_summ_t ;


typedef struct {
  /* for GPIO[7..0, once alm_status is enabled, the corresponding     */
  /* io_map bits will be overwritten as outputs                       */
  cs_uint8      alm_status_map ;    /* GPIO[7..0], 1 : alarm status   */
  cs_uint16     io_map ;            /* GPIO[15..0], 1 : output        */
  cs_uint16     output_values ;     /* initial outputs for GPIO[15..0]*/
} arn5_gpio_cfg_t ;


/*******************************************************************
 * STATISTICS DATA ( THIS IS ATMOIC ENTITY OF STATS INFRASTRUCTURE )
 *******************************************************************/
typedef struct {
  cs_uint64     tx ;    /* TX COUNTER VALUE */
  cs_uint64     rx ;    /* RX COUNTER VALUE */
} arn5_stats_data_t ;



#endif  /* __ARN5_PUBLIC_H__ */


