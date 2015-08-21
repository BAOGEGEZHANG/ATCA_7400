/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_debug.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file and contains some top-level debug
 * utilities.
 */

#ifndef __ARN5_DEBUG_H__
#define __ARN5_DEBUG_H__

#include  "cs_types.h"
#include  "arn5_public.h"
#include  "arn5_gen_data.h"


/**************************************
 *  Externs                           *
 **************************************/
extern
cs_uint16 arn5_reg_read(cs_uint8 dev_id,
                        cs_uint32 reg_offset, cs_uint8 inst_num);
extern
cs_status arn5_reg_write(cs_uint8 dev_id,
                        cs_uint32 reg_offset, cs_uint8 inst_num,cs_uint16 data);

extern
cs_status  arn5_dev_debug_init(cs_uint8 dev_id,
                        arn5_host_if_t host_if) ;
extern
cs_uint16  arn5_port_debug_init(cs_uint8 dev_id,
                        cs_uint8 slice_num,
                        arn5_port_line_rate_t line_rate,
                        arn5_port_prot_t l2_prot) ;
extern
void arn5_dump_drvr_cb() ;
extern
void arn5_dump_dev_cb(cs_uint8 dev_id) ;
extern
void arn5_dump_port_cb(cs_uint16 port_id) ;

#ifdef POSIX_PTHREAD
extern
cs_status arn5_start_irq_polling(cs_uint8 dev_id,
                                cs_uint32 msec_poll_intvl,
                                cs_uint32 blk_id) ;
extern
cs_status arn5_stop_irq_polling(cs_uint8 dev_id) ;
#endif

extern
cs_uint16  arn5_get_crc16(cs_uint8 * buf, cs_uint32 len) ;
extern
cs_uint32 arn5_get_crc32(cs_uint8 * buf, cs_uint32 len) ;


#endif /* __ARN5_DEBUG_H__ */
