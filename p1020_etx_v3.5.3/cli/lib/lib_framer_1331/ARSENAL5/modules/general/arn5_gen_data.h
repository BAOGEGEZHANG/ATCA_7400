/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_gen_data.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __ARN5_GEN_DATA_H__
#define __ARN5_GEN_DATA_H__

#include "cs_types.h"
#include "arn5_public.h"

#include "arn5_mpif_data.h"
#include "arn5_spi_data.h"
#include "arn5_framer.h"
#include "arn5_eth_data.h"
#include "arn5_lif.h"
#include "arn5_pprbs.h"


/***********************************************
 * Device-level Init Configuration
 ***********************************************/
typedef struct {
  arn5_dev_summ_t       summ ;
  arn5_gpio_cfg_t       gpio ;
  arn5_mpif_cfg_t       mpif ;
  arn5_spi_dev_cfg_t    spi ;
} arn5_dev_cfg_t ;


/***********************************************
 * Device-level Running Configuration          *
 ***********************************************/
typedef struct {
  cs_uint32             jtag_id ;
  arn5_dev_summ_t       summ ;
  arn5_gpio_cfg_t       gpio ;
  arn5_spi_dev_runcfg_t spi ;
} arn5_dev_runcfg_t ;


/***********************************************
 * Port configuration                          *
 ***********************************************/
typedef struct {
  arn5_port_summ_t      summ ;

  arn5_spi_port_cfg_t   spi ;

  cs_boolean            framer_valid ;
  arn5_framer_cfg_t     framer ;

  cs_boolean            lif_valid ;
  arn5_lif_cfg_t        lif ;

  cs_boolean            eth_valid ;
  arn5_eth_cfg_t        eth ;
} arn5_port_cfg_t ;


typedef struct {
  cs_uint8              dev_id ;
  arn5_port_summ_t      summ ;

  arn5_spi_port_runcfg_t spi;

  cs_boolean            framer_valid ;
  arn5_framer_runcfg_t  framer ;

  cs_boolean            lif_valid ;
  arn5_lif_runcfg_t     lif ;

  cs_boolean            eth_valid ;
  arn5_eth_runcfg_t     eth;

  cs_boolean            pprbs_valid;
  arn5_pprbs_runcfg_t   pprbs ;
} arn5_port_runcfg_t ;


#endif  /* __ARN5_GEN_DATA_H__ */

