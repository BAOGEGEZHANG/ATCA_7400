/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_priv.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PRIVATE header file.
 */
#ifndef __ARN5_SPI_PRIV_H__
#define __ARN5_SPI_PRIV_H__

#include "cs_types.h"
#include "arn5_public.h"
#include "arn5_common.h"

#include "arn5_spi_data.h"

#define ARN5_SPI_GET_PCOMM(dev_id)                        \
        (&(ARN5_DEV_ID_TO_DEV_BASE(dev_id))->SPI4P2 )


#define ARN5_SPI_DEV_LOCK(dev_id) {                       \
        ARN5_DEV_LOCK(dev_id, ARN5_ID_HOST_IF) ;          \
}


#define ARN5_SPI_DEV_UNLOCK(dev_id) {                     \
        ARN5_DEV_UNLOCK(dev_id, ARN5_ID_HOST_IF) ;        \
}



/* Private externs */
extern
cs_boolean arn5_spi_dev_is_in_spi3_mode(cs_uint8 dev_id) ;
extern
cs_boolean arn5_spi_dev_is_in_spi42_mode(cs_uint8 dev_id) ;
extern
void arn5_spi_commit_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status arn5_spi_dev_init_cfg(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg) ;
extern
cs_status arn5_spi_dev_get_default_cfg(cs_uint8 dev_id,
                        arn5_dev_cfg_t * pDevCfg) ;
extern
cs_status arn5_spi_dev_get_running_cfg(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg) ;
extern
cs_status arn5_spi_dev_dump_running_cfg(cs_uint8 dev_id,
                        arn5_dev_runcfg_t * pRunCfg) ;
extern
cs_status arn5_spi_init_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status arn5_spi_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status arn5_spi_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;
extern
cs_status arn5_spi_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;
extern
cs_uint8 arn5_logical2physical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint8 logical_chan_num) ;
extern
cs_uint8 arn5_physical2logical_chan_num(cs_uint8 dev_id,
                        cs_dir_t dir, cs_uint8 physical_chan_num) ;
extern
cs_status arn5_spi_drop_chan_data_ctl(cs_uint8 dev_id,
                        cs_uint8 phy_chan,
                        cs_boolean ctl) ;
extern
cs_status arn5_spi_dev_drop_chan_ctl_reconcile(cs_uint8 dev_id) ;


/*****************************************************************
 * SPI IRQ API
 *****************************************************************/
extern
cs_status arn5_spi_dev_get_default_irq_cfg(cs_uint8 dev_id,
                        arn5_dev_summ_t * pSumm,
                        arn5_spi_dev_cfg_t * pCfg);

extern
cs_status arn5_spi_dev_init_irq_cfg(cs_uint8 dev_id,
                        arn5_spi_dev_cfg_t * pCfg) ;

extern cs_status arn5_spi_dev_isr(cs_uint8 dev_id) ;


/*****************************************************************
 * SPI STATS API
 *****************************************************************/
extern
cs_status  arn5_spi_init_port_stats(cs_uint16 port_id) ;
extern
cs_status  arn5_spi_init_dev_stats(cs_uint8 dev_id) ;


#endif /* __ARN5_SPI_PRIV_H__ */

