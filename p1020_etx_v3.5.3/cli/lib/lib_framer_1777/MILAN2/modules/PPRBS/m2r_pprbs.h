/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_pprbs.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * This is the exported header file for this module.
 */
#ifndef __M2R_PPRBS_H__
#define __M2R_PPRBS_H__

#include "cs_types.h"
#include "m2r_public.h"

/**********************************************************************
 * Defines for default values
 **********************************************************************/
#define M2R_PPRBS_MIN_PKT_SIZE_DEF          (16)
#define M2R_PPRBS_MAX_PKT_SIZE_DEF          (0x007f)

/**********************************************************************
 * PPRBS CHANNEL :
 *   M2R_PPRBS_HOST_CHAN channel refers to generator and checker which
 *   send and receive packets from the HOST side i.e to/from SPI4.2
 *   M2R_PPRBS_LINE_CHAN channel refers to generator and checker which
 *   send and receive packets from/to LINE side i.e. towards LIF.
 **********************************************************************/
typedef  enum {
  M2R_PPRBS_HOST_CHAN   = 0x01,
  M2R_PPRBS_LINE_CHAN   = 0x02,
  M2R_PPRBS_CHAN_ALL    = 0x03
} m2r_pprbs_chan_t ;



/**********************************************************************
 * PPRBS IRQ leaf definitions
 **********************************************************************/
#define M2R_PPRBS_Interrupt_RolloverTxPacket1I     (0x8000)
#define M2R_PPRBS_Interrupt_RolloverTxByte1I       (0x4000)
#define M2R_PPRBS_Interrupt_SentPackets1I          (0x2000)
#define M2R_PPRBS_Interrupt_RolloverPacket1I       (0x1000)
#define M2R_PPRBS_Interrupt_RolloverByte1I         (0x0800)
#define M2R_PPRBS_Interrupt_Rollover1I             (0x0400)
#define M2R_PPRBS_Interrupt_BitError1I             (0x0200)
#define M2R_PPRBS_Interrupt_SyncState1I            (0x0100)
#define M2R_PPRBS_Interrupt_RolloverTxPacket0I     (0x0080)
#define M2R_PPRBS_Interrupt_RolloverTxByte0I       (0x0040)
#define M2R_PPRBS_Interrupt_SentPackets0I          (0x0020)
#define M2R_PPRBS_Interrupt_RolloverPacket0I       (0x0010)
#define M2R_PPRBS_Interrupt_RolloverByte0I         (0x0008)
#define M2R_PPRBS_Interrupt_Rollover0I             (0x0004)
#define M2R_PPRBS_Interrupt_BitError0I             (0x0002)
#define M2R_PPRBS_Interrupt_SyncState0I            (0x0001)

#define M2R_GRP_PPRBS_Interrupt_ALL                (0xffff)

#define M2R_PPRBS_STATS_IRQ                        (0xdcdc)

/**********************************************************************
 * PPRBS IRQ GROUPS
 **********************************************************************/
typedef enum {
  M2R_GRP_PPRBS_Interrupt   = 0,
  M2R_PPRBS_MAX_GRP
} m2r_pprbs_irq_grp_t ;



extern  cs_status m2r_pprbs_reset_stats(cs_uint16 port_id,
                                        m2r_pprbs_chan_t chan,
                                        cs_boolean pktGen,
                                        cs_boolean checker) ;

extern  cs_status m2r_pprbs_cfg_traffic(cs_uint16 port_id,
                                        m2r_pprbs_chan_t  chan,
                                        cs_boolean fixpld,
                                        cs_uint16 burstSize,
                                        cs_uint16 rate,
                                        cs_uint16 minSize,
                                        cs_uint16 maxSize) ;

extern  cs_status m2r_pprbs_sel_internal_chan(cs_uint16 port_id,
                                              m2r_pprbs_chan_t  chan,
                                              cs_uint8 intChan,
                                              m2r_dir_t dir) ;

extern  cs_status m2r_pprbs_ctl_pktGen(cs_uint16 port_id,
                                       m2r_pprbs_chan_t chan,
                                       m2r_ctl_t ctl) ;

extern  cs_status m2r_pprbs_ctl_ipg(cs_uint16 port_id,
                                    m2r_pprbs_chan_t  chan,
                                    cs_uint16 ipg) ;

extern  cs_status m2r_pprbs_inject_err(cs_uint16 port_id,
                                       m2r_pprbs_chan_t  chan,
                                       cs_boolean signleErr,
                                       cs_uint16 duration) ;

extern  cs_status m2r_pprbs_ctl_pktChkr(cs_uint16 port_id,
                                        m2r_pprbs_chan_t chan,
                                        m2r_ctl_t ctl) ;

extern  cs_status m2r_pprbs_ctl_rewrite(cs_uint16 port_id,
                                m2r_pprbs_chan_t chan, m2r_ctl_t ctl) ;

extern  cs_status m2r_pprbs_resync(cs_uint16 port_id,
                                   m2r_pprbs_chan_t chan) ;

extern  cs_boolean  m2r_pprbs_chk_in_sync(cs_uint16 port_id,
                                          m2r_pprbs_chan_t  chan) ;

extern  cs_uint64  m2r_pprbs_get_ber(cs_uint16 port_id,
                                     m2r_pprbs_chan_t chan) ;

extern  cs_status m2r_pprbs_get_stats(cs_uint16 port_id,
                                      m2r_pprbs_chan_t chan) ;

extern  cs_status m2r_pprbs_isr(cs_uint16 port_id) ;

#endif /* __M2R_PPRBS_H__ */



