/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_mpif_api.h
 * Header file for MPIF block
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_MPIF_API_H__
#define __ARN5_MPIF_API_H__

#include "cs_types.h"
#include "cs_rtos.h"

typedef enum {
  ARN5_DIAG_BUS_NONE = 0,
  ARN5_DIAG_BUS_BRIDGE,
  ARN5_DIAG_BUS_FRAMER_0,
  ARN5_DIAG_BUS_FRAMER_1,
  ARN5_DIAG_BUS_FRAMER_2,
  ARN5_DIAG_BUS_FRAMER_3,
  ARN5_DIAG_BUS_FRAMER_4,
  ARN5_DIAG_BUS_FRAMER_5,
  ARN5_DIAG_BUS_FRAMER_6,
  ARN5_DIAG_BUS_FRAMER_7,
  ARN5_DIAG_BUS_GIGE_0,
  ARN5_DIAG_BUS_GIGE_1,
  ARN5_DIAG_BUS_GIGE_2,
  ARN5_DIAG_BUS_GIGE_3,
  ARN5_DIAG_BUS_GIGE_4,
  ARN5_DIAG_BUS_GIGE_5,
  ARN5_DIAG_BUS_GIGE_6,
  ARN5_DIAG_BUS_GIGE_7,
}arn5_diag_bus_t ;

typedef enum {
  ARN5_SMH_PORT_0,
  ARN5_SMH_PORT_1,
} arn5_smh_port_t ;

typedef enum {
  ARN5_SMH_DATA_TYPE_NONE,
  ARN5_SMH_DATA_TYPE_TOH,
  ARN5_SMH_DATA_TYPE_DCC,
} arn5_smh_data_type_sel_t ;


extern
cs_uint32  arn5_get_chip_jtag_id(cs_uint8 dev_id) ;
extern
cs_status  arn5_do_bist(cs_uint8 dev_id, cs_boolean mbist,
                        cs_boolean sbist) ;
extern
cs_boolean  arn5_is_bist_done(cs_uint8 dev_id, cs_boolean mbist,
                        cs_boolean sbist) ;
extern
cs_status  arn5_check_bist_status(cs_uint8 dev_id, cs_boolean mbist,
                        cs_boolean sbist) ;
extern
cs_status  arn5_dev_hard_reset(cs_uint8 dev_id, arn5_module_id_t mid,
                        cs_reset_action_t act) ;
extern
cs_status  arn5_port_hard_reset(cs_uint16 port_id,
                        arn5_module_id_t mid,
                        cs_reset_action_t act) ;
extern
cs_status  arn5_slice_ctl_pwr_down(cs_uint8 dev_id, cs_uint8 slice_num,
                        cs_ctl_t ctl) ;
extern
cs_boolean  arn5_is_slice_pwr_down(cs_uint8 dev_id,
                        cs_uint8 slice_num) ;
extern
cs_status  arn5_mpif_cfg_diag_bus(cs_uint8 dev_id,
                        arn5_diag_bus_t diag_bus) ;
extern
cs_status  arn5_mpif_cfg_gpio_alm_status(cs_uint8 dev_id,
                        cs_uint8 alm_status_map,
                        cs_uint8 mask) ;
extern
cs_status  arn5_mpif_cfg_gpio_io(cs_uint8 dev_id, cs_uint16 io_map,
                        cs_uint16 mask) ;
extern
cs_uint16  arn5_mpif_read_gpio_input(cs_uint8 dev_id, cs_uint16 mask) ;
extern
cs_status  arn5_mpif_write_gpio_output(cs_uint8 dev_id,
                        cs_uint16 data, cs_uint16 mask) ;
extern
cs_status  arn5_mpif_cfg_smh_port(cs_uint8 dev_id,
                        arn5_smh_port_t smh_port,
                        cs_uint8 slice_num,
                        arn5_smh_data_type_sel_t type,
                        cs_dir_t dir) ;

#endif /* __ARN5_MPIF_API_H__ */

