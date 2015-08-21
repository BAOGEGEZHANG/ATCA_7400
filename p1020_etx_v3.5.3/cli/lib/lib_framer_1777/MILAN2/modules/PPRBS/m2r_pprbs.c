/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_pprbs.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_pprbs.h"
#include "m2r_irq_priv.h"
#include "m2r_stats.h"
#include "m2r_pprbs_stats.h"

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const m2r_irq_ctl_t  m2r_pprbs_irq_tbl[M2R_PPRBS_MAX_GRP] = {
  CS_DI([M2R_GRP_PPRBS_Interrupt] =) {
      M2R_PPRBS_REG_OFST(Interrupt),
      0,
      M2R_PPRBS_REG_OFST(IntMask),
      M2R_GRP_PPRBS_Interrupt_ALL, 0, M2R_IRQ_FLG_RW1C }
} ;

const char * m2r_ppbs_irq_grp_name_tbl[M2R_PPRBS_MAX_GRP] =
{
  CS_DI([M2R_GRP_PPRBS_Interrupt]           =) "PPRBS_Interrupt"
} ;

/**************** IRQ ID NAME TABLES **********************/
const m2r_irq_id_name_t
M2R_GRP_PPRBS_Interrupt_irq_id_name_tbl[] = {
  {M2R_PPRBS_Interrupt_RolloverTxPacket1I,      "RolloverTxPacket1I"},
  {M2R_PPRBS_Interrupt_RolloverTxByte1I,        "RolloverTxByte1I"},
  {M2R_PPRBS_Interrupt_SentPackets1I,   	"SentPackets1I"},
  {M2R_PPRBS_Interrupt_RolloverPacket1I,        "RolloverPacket1I"},
  {M2R_PPRBS_Interrupt_RolloverByte1I,  	"RolloverByte1I"},
  {M2R_PPRBS_Interrupt_Rollover1I,      	"Rollover1I"},
  {M2R_PPRBS_Interrupt_BitError1I,      	"BitError1I"},
  {M2R_PPRBS_Interrupt_SyncState1I,     	"SyncState1I"},
  {M2R_PPRBS_Interrupt_RolloverTxPacket0I,      "RolloverTxPacket0I"},
  {M2R_PPRBS_Interrupt_RolloverTxByte0I,        "RolloverTxByte0I"},
  {M2R_PPRBS_Interrupt_SentPackets0I,   	"SentPackets0I"},
  {M2R_PPRBS_Interrupt_RolloverPacket0I,        "RolloverPacket0I"},
  {M2R_PPRBS_Interrupt_RolloverByte0I,  	"RolloverByte0I"},
  {M2R_PPRBS_Interrupt_Rollover0I,      	"Rollover0I"},
  {M2R_PPRBS_Interrupt_BitError0I,      	"BitError0I"},
  {M2R_PPRBS_Interrupt_SyncState0I,     	"SyncState0I"}
} ;


/****************************************************************/
/* Macros and internal functions                                */
/****************************************************************/
#define M2R_PPRBS_GET_DEVICE_ADDR(PORTID, BASE_ADDR, SLICE, RETCODE)   \
{                                                                \
  cs_uint32 error;                                               \
  if(M2R_IS_PORT_VALID(PORTID, error) != TRUE) {                 \
    CS_HNDL_ERROR((PORTID), error, ": port_id= %d\n", (PORTID)); \
    return (RETCODE) ;                                           \
  }                                                              \
  (BASE_ADDR) = M2R_PORT_ID_TO_DEV_BASE((PORTID));               \
  (SLICE)     = M2R_PORT_ID_TO_SLICE_NUM((PORTID));              \
}

#define M2R_PPRBS_IS_VALID_CHAN(PORTID, CHAN, RETCODE)           \
{                                                                \
  if((CHAN) > M2R_PPRBS_CHAN_ALL) {                              \
    CS_HNDL_ERROR((PORTID), EM2R_PPRBS_INVALID_CHANNEL, NULL);   \
    return (RETCODE) ;                                           \
  }                                                              \
}

/*
 * Calculate the rate. The function returns the rate in precent of the
 * line rate. For example a return value of 95 means 95%.
 */
cs_uint16  m2r_pprbs_cal_rate(cs_uint16 min, cs_uint16 max,
                 cs_boolean sz_disable, cs_uint16 sz_val, cs_uint16 ipg)
{
  cs_uint32  hdr_beat, pkt_beat, ipg_beat, last_beat , total_beat;
  cs_uint32  total_perfect_beat, perfect_last_beat ;
  cs_uint16  last_beat_bytes ;

  hdr_beat = (max - min +1) * 3 ;
  pkt_beat =  ((max * (max+1)) - ((min-1)*min))/2 ;
  ipg_beat = (ipg * (max-min+1)) ;
  last_beat = (max-min+1) ;

  if(sz_disable == 0){
      last_beat_bytes = 36 ;
  } else {
      last_beat_bytes = 8 * sz_val ;
  }
  perfect_last_beat = ((max-min+1) * (last_beat_bytes)/64) ;

  total_beat = hdr_beat + pkt_beat + ipg_beat + last_beat ;
  total_perfect_beat = hdr_beat + pkt_beat + perfect_last_beat ;

  return (total_perfect_beat * 100)/total_beat ;
}

/*
 * Calculate the IPG given max, mix pkt Size, rate required, sz_disable and
 * sz_val.
 */
cs_uint16  m2r_pprbs_cal_ipg(cs_uint16 rate, cs_uint16 min, cs_uint16 max,
                             cs_boolean sz_disable, cs_uint16 sz_val)
{
  cs_uint32  hdr_beat, pkt_beat, ipg_beat, last_beat , total_pld_beat ;
  cs_uint32  total_perfect_beat, perfect_last_beat ;
  cs_uint16  last_beat_bytes, ipg ;

  if ( rate == 0 ) {
    ipg = 0xffff ;
    goto EXIT ;
  }
  else if ( rate == 100 )
  {
    ipg = 0 ;
    goto EXIT ;
  }

  hdr_beat = (max - min +1) * 3 ;
  pkt_beat =  ((max * (max+1)) - ((min-1)*min))/2 ;
  /* ipg_beat = (ipg * (max-min+1)) ; */
  last_beat = (max-min+1) ;

  if(sz_disable == 0){
      last_beat_bytes = 36 ;
  } else {
      last_beat_bytes = 8 * sz_val ;
  }
  perfect_last_beat = ((max-min+1) * (last_beat_bytes)/64) ;

  total_perfect_beat = hdr_beat + pkt_beat + perfect_last_beat ;
  total_pld_beat = hdr_beat + pkt_beat + last_beat ;

  ipg_beat = (total_perfect_beat * 100)/(rate) - total_pld_beat;

  ipg = (ipg_beat/(max - min + 1)) ;
EXIT:
  return ipg ;
}

#if 0
/************************************************************************
 * PPRBS IRQ SPECIAL HANDLER
 ***********************************************************************/
cs_uint16 m2r_pprbs_irq_spl_hndlr(cs_uint16 port_id,
                                  cs_uint16 grp_id,
                                  cs_uint16 irq_id,
                                  m2r_irq_info_t * info,
                                  cs_uint16 irq_reg_val,
                                  cs_uint16 status_reg_val)
{
  cs_uint16 status = 0 ;

  switch (irq_id) {
    case M2R_PPRBS_Interrupt_RolloverTxPacket1I :
    case M2R_PPRBS_Interrupt_RolloverTxPacket0I :
      sect_id = (irq_id == M2R_PPRBS_Interrupt_RolloverTxPacket1I) ?
                  M2R_PPRBS_STATS_LINE_SECT : M2R_PPRBS_STATS_HOST_SECT ;
      unit_id = M2R_PPRBS_CHAN_STATS_PKT ;
      dir     = M2R_TX ;
    break ;

    case M2R_PPRBS_Interrupt_RolloverTxByte1I :
    case M2R_PPRBS_Interrupt_RolloverTxByte0I :
      sect_id = (irq_id == M2R_PPRBS_Interrupt_RolloverTxByte1I) ?
                  M2R_PPRBS_STATS_LINE_SECT : M2R_PPRBS_STATS_HOST_SECT ;
      unit_id = M2R_PPRBS_CHAN_STATS_BYTES ;
      dir     = M2R_TX ;
    break ;

    case M2R_PPRBS_Interrupt_RolloverPacket1I :
    case M2R_PPRBS_Interrupt_RolloverPacket0I :
      sect_id = (irq_id == M2R_PPRBS_Interrupt_RolloverPacket1I) ?
                  M2R_PPRBS_STATS_LINE_SECT : M2R_PPRBS_STATS_HOST_SECT ;
      unit_id = M2R_PPRBS_CHAN_STATS_PKT ;
      dir     = M2R_RX ;
    break ;

    case M2R_PPRBS_Interrupt_RolloverByte1I :
    case M2R_PPRBS_Interrupt_RolloverByte0I :
      sect_id = (irq_id == M2R_PPRBS_Interrupt_RolloverByte1I) ?
                  M2R_PPRBS_STATS_LINE_SECT : M2R_PPRBS_STATS_HOST_SECT ;
      unit_id = M2R_PPRBS_CHAN_STATS_BYTES ;
      dir     = M2R_RX ;
    break ;

    case M2R_PPRBS_Interrupt_Rollover1I :
    case M2R_PPRBS_Interrupt_Rollover0I :
      sect_id = (irq_id == M2R_PPRBS_Interrupt_Rollover1I) ?
                  M2R_PPRBS_STATS_LINE_SECT : M2R_PPRBS_STATS_HOST_SECT ;
      unit_id = M2R_PPRBS_CHAN_STATS_ERR ;
      dir     = M2R_RX ;
    break ;
  }

  return (status) ;
}
#endif



/****************************************************************/
/* $rtn_hdr_start  RESET THE STATS REGISTERS                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_reset_stats(cs_uint16 port_id,
      m2r_pprbs_chan_t chan, cs_boolean pktGen, cs_boolean checker)
/* INPUTS     : o Port Id                                       */
/*              o PPRBS CHANNEL ID                              */
/*              o TRUE ( Clear Generater ) Stats                */
/*              o TRUE ( Clear checker   ) Stats                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Resets the stats/counter registers.                          */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if (chan & M2R_PPRBS_HOST_CHAN) {
    if (checker) {
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkErr0.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteMSB0.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteLSB0.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketMSB0.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketLSB0.wrd, 0) ;
    }
    if (pktGen) {
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteMSB0.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteLSB0.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketMSB0.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketLSB0.wrd, 0) ;
    }
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    if (checker) {
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkErr1.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteMSB1.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteLSB1.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketMSB1.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketLSB1.wrd, 0) ;
    }
    if (pktGen) {
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteMSB1.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteLSB1.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketMSB1.wrd, 0) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketLSB1.wrd, 0) ;
    }
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  CONFIGURES THE PPRBS TRAFFIC PARAMETERS      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_cfg_traffic(cs_uint16 port_id, m2r_pprbs_chan_t chan,
                cs_boolean fixpld, cs_uint16 burstSize, cs_uint16 rate,
                                cs_uint16 minSize, cs_uint16 maxSize)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/*              o Fiexd Payload or PRBS payload                 */
/*              o Burst Size ( 0 for continuous generation )    */
/*              o Pkt Generation Rate                           */
/*              o Min Pkt Size                                  */
/*              o Max Pkt Size                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the PPRBS traffic properties. If burstSize is 0   */
/* means continuous generation of packets. [rate] specifies     */
/* packet generation rate in percentage of line rate.           */
/* for example: a value of 90 means 90% of line rate.           */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* NOTE : A burstSize of 0 ( Zero ) means continuous burst.     */
/* If the maxPkt and minPkt are same a fixed size packet will be*/
/* generated.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;
  cs_uint16            ipg = 0 ;
  cs_boolean           sz_disable ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if(rate > 100) {
    CS_HNDL_ERROR(port_id, EM2R_PPRBS_INVALID_RATE, "rate = %d\n", rate);
  }
  sz_disable = (minSize == maxSize ) ;
  ipg = m2r_pprbs_cal_ipg(rate, minSize, maxSize, sz_disable, (minSize % 8)) ;

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_GenCntl0_t   genCtrl ;
    PPRBS_GenNum0_t    burstCtl ;
    PPRBS_ChkCntl0_t   chkCtrl ;

    genCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
    genCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    if ( sz_disable ) {
      genCtrl.bf.sz_val = (minSize % 8) ;
    }
    genCtrl.bf.sz_disable = (sz_disable) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd, genCtrl.wrd) ;

    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMinLen0.wrd, (minSize >> 3)) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMaxLen0.wrd, (maxSize >> 3)) ;

    /* Program the IPG value to achieve the desired rate. */
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG0.wrd, ipg) ;

    /*
     * Set the burst length. A value of Zero means continuous burst.
     */
    burstCtl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenNum0.wrd) ;
    burstCtl.bf.num = burstSize ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenNum0.wrd, burstCtl.wrd) ;

    /*
     * Configure the Checker for expected payload.
     */
    chkCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
    chkCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkCtrl.wrd) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_GenCntl1_t   genCtrl ;
    PPRBS_GenNum1_t    burstCtl ;
    PPRBS_ChkCntl1_t   chkCtrl ;

    genCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
    genCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    if ( sz_disable ) {
      genCtrl.bf.sz_val = (minSize % 8) ;
    }
    genCtrl.bf.sz_disable = sz_disable ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd, genCtrl.wrd) ;

    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMinLen1.wrd, (minSize>>3)) ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMaxLen1.wrd, (maxSize>>3)) ;

    /*
     * Program the IPG after calculating IPG.
     */
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG1.wrd, ipg) ;


    /*
     * Set the burst length. A value of Zero means continuous burst.
     */
    burstCtl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenNum1.wrd) ;
    burstCtl.bf.num = burstSize ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenNum1.wrd, burstCtl.wrd) ;

    /*
     * Configure the Checker for expected payload.
     */
    chkCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
    chkCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkCtrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start SELECT PPRBS INTERNAL CHANNEL                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : MILAN2                                          */
cs_status m2r_pprbs_sel_internal_chan(cs_uint16 port_id,
                                      m2r_pprbs_chan_t chan,
                                      cs_uint8 intChan,
                                      m2r_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o Host and/or Line Channel                      */
/*              o Internal Channel [0..3]                       */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selectes the internal channel to send and monitor Pkts.      */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/*                                                              */
/* When the RingMAC is in bypass mode (i.e. in framer only app),*/
/* the internal channels to use in M2R_PPRBS_LINE_CHAN should   */
/* always set to 0 unless the framer is set to channelized mode */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16           sl_num ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_GenCntl0_t   chanCtrl ;
    PPRBS_ChkCntl0_t   chkCtrl ;

    if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
      chanCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
      chanCtrl.bf.chan = (intChan % 4) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd, chanCtrl.wrd) ;
    }

    if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
      chkCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
      chkCtrl.bf.chan = (intChan % 4) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkCtrl.wrd) ;
    }
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_GenCntl1_t   chanCtrl ;
    PPRBS_ChkCntl1_t   chkCtrl ;

    if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
      chanCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
      chanCtrl.bf.chan = (intChan % 4) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd, chanCtrl.wrd) ;
    }

    if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
      chkCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
      chkCtrl.bf.chan = (intChan % 4) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkCtrl.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PPRBS GENERATOR               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_ctl_pktGen(cs_uint16 port_id,
                               m2r_pprbs_chan_t chan, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the PPRBS generator.                          */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* NOTE : user should call m2r_pprbs_cfg_traffic() API to       */
/* configure traffic parameters before enabling the Generator.  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16           sl_num ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_GenCntl0_t   chanCtrl ;
    chanCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
    chanCtrl.bf.enable = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd, chanCtrl.wrd) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_GenCntl1_t   chanCtrl ;
    chanCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
    chanCtrl.bf.enable = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd, chanCtrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  CONFIGURE THE INTER PACKET GAP               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_ctl_ipg(cs_uint16 port_id, m2r_pprbs_chan_t chan,
                            cs_uint16 ipg)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/*              o IPG                                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the IPG for the generator.                        */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_GenIPG0_t    chanIpg ;
    chanIpg.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenIPG0.wrd) ;
    chanIpg.bf.ipg = ipg ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG0.wrd, chanIpg.wrd) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_GenIPG1_t    chanIpg ;
    chanIpg.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenIPG1.wrd) ;
    chanIpg.bf.ipg = ipg ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG1.wrd, chanIpg.wrd) ;
  }
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  INJECT ERROR IN PPRBS TRANSMIT STREAM        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_inject_err(cs_uint16 port_id, m2r_pprbs_chan_t chan,
                               cs_boolean singleErr, cs_uint16 duration)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/*              o TRUE ( Single Error ) else FALSE              */
/*              o Duration for which errors will be generated   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the PPRBS generator to inject errors. If Single   */
/* Error needs to be generated Set the singleErr to TRUE else   */
/* errors will be injected continuously for the duration        */
/* specified in microseconds.                                   */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* NOTE : This function might block for the "duration " if      */
/* continueous errors need to be generated and duration is      */
/* non-zero.                                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;
  PPRBS_GenErr1_t    injErr0, injErr1 ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    injErr0.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenErr0.wrd) ;
    if (singleErr) {
      injErr0.bf.single_err = 1 ;
    }
    else {
      injErr0.bf.cont_err = 1 ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr0.wrd, injErr0.wrd) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    injErr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenErr1.wrd) ;
    if (singleErr) {
      injErr1.bf.single_err = 1 ;
    }
    else {
      injErr1.bf.cont_err = 1 ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr1.wrd, injErr1.wrd) ;
  }

  /*
   * If error injection is for specific time wait.
   */
  if(singleErr) {
    CS_UDELAY(100) ;
    if (chan & M2R_PPRBS_HOST_CHAN) {
      injErr0.bf.single_err = 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr0.wrd, injErr0.wrd) ;
    }

    if (chan & M2R_PPRBS_LINE_CHAN) {
      injErr1.bf.single_err = 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr1.wrd, injErr1.wrd) ;
    }
  }
  else {
    CS_UDELAY(duration) ;

    if (chan & M2R_PPRBS_HOST_CHAN) {
      injErr0.bf.cont_err = 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr0.wrd, injErr0.wrd) ;
    }

    if (chan & M2R_PPRBS_LINE_CHAN) {
      injErr1.bf.cont_err = 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr1.wrd, injErr1.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PPRBS CHECKER                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_ctl_pktChkr(cs_uint16 port_id,
                               m2r_pprbs_chan_t chan, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/*              o M2R_ENABLE or M2R_DISABLE                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the PPRBS checker.                            */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_ChkCntl0_t chkrCtl ;
    chkrCtl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
    chkrCtl.bf.enable = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkrCtl.wrd) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_ChkCntl1_t chkrCtl ;
    chkrCtl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
    chkrCtl.bf.enable = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkrCtl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE MAC REWRITE MODE FOR PPRBS    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_ctl_rewrite(cs_uint16 port_id,
                                m2r_pprbs_chan_t chan, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable rewrite Mode in PPRBS. When enabled the       */
/* packets are generated with DR ( Do Rewrite ) bit set to 1.   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16           sl_num ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_GenCntl0_t   chanCtrl ;
    chanCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
    chanCtrl.bf.rewrite = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd, chanCtrl.wrd) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_GenCntl1_t   chanCtrl ;
    chanCtrl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
    chanCtrl.bf.rewrite = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd, chanCtrl.wrd) ;
  }
  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE CHECKER INTO HUNT STATE              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_resync(cs_uint16 port_id, m2r_pprbs_chan_t chan)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the Checker into the HUNT state .                        */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num, timeout = 10 ;
  cs_status            status = CS_OK ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_ChkCntl0_t chkrCtl ;
    PPRBS_ChkSync0_t chkrSync ;

    chkrCtl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
    chkrCtl.bf.resync = 1 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkrCtl.wrd) ;

    chkrSync.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync0.wrd) ;
    timeout = 10 ;
    while (timeout && (chkrSync.bf.sync)) {
      CS_UDELAY(1) ;
      timeout-- ;
      chkrSync.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync0.wrd) ;
    }
    if (!timeout) {
      CS_HNDL_ERROR(port_id, EM2R_PPRBS_RESYNC_FAILURE,
                     "Host channel Checker failed to enter HUNT State\n");
      status = CS_ERROR ;
    }
    /* release resync bit to allow sycn to take place */
    chkrCtl.bf.resync = 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkrCtl.wrd) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_ChkCntl1_t chkrCtl ;
    PPRBS_ChkSync1_t chkrSync ;

    chkrCtl.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
    chkrCtl.bf.resync = 1 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkrCtl.wrd) ;

    chkrSync.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync1.wrd) ;
    timeout = 10 ;
    while (timeout && (chkrSync.bf.sync)) {
      CS_UDELAY(1) ;
      timeout-- ;
      chkrSync.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync1.wrd) ;
    }
    if (!timeout) {
      CS_HNDL_ERROR(port_id, EM2R_PPRBS_RESYNC_FAILURE,
                "Line channel Checker failed to enter HUNT State\n");
      status = CS_ERROR ;
    }
    /* release resync bit to allow sycn to take place */
    chkrCtl.bf.resync = 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkrCtl.wrd) ;
  }
  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS);
  return (status) ;
}

/****************************************************************/
/* $rtn_hdr_start  CHECK THE CHECKER SYNC STATUS                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_boolean  m2r_pprbs_chk_in_sync(cs_uint16 port_id, m2r_pprbs_chan_t chan)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Checks if the PPRBS checker is in SYNC state.                */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;
  cs_boolean           inSync = FALSE ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if (chan & M2R_PPRBS_HOST_CHAN) {
    PPRBS_ChkSync0_t chkrSync ;
    chkrSync.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync0.wrd) ;
    inSync = chkrSync.bf.sync ;
  }
  else
  if (chan & M2R_PPRBS_LINE_CHAN) {
    PPRBS_ChkSync1_t chkrSync ;
    chkrSync.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync1.wrd) ;
    inSync = chkrSync.bf.sync ;
  }

  return ((inSync) ? TRUE : FALSE) ;
}

/****************************************************************/
/* $rtn_hdr_start  RETURN BER STATUS                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_uint64  m2r_pprbs_get_ber(cs_uint16 port_id, m2r_pprbs_chan_t chan)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Calculates the BER based on the total number of error        */
/* detected by the checker.                                     */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* A return value of Zero (0) means no bit errors were detected.*/
/* Also the actual BER is 1/return value. The function actually */
/* return the number of good bits per error bit.                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;
  cs_uint64            total_bits, ber = 0 ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN)
  {
    PPRBS_ChkErr0_t total_bit_errs ;
    PPRBS_ChkByteMSB0_t bytes_msb;
    PPRBS_ChkByteLSB0_t bytes_lsb ;

    total_bit_errs.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkErr0.wrd) ;
    bytes_msb.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteMSB0.wrd) ;
    bytes_lsb.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteLSB0.wrd);

    total_bits = ((bytes_msb.wrd << 16 ) | (bytes_lsb.wrd)) * 8 ;
    if(total_bit_errs.wrd) {
      ber = (total_bits/total_bit_errs.wrd) ;
    }
  }
  else
  {
    PPRBS_ChkErr1_t total_bit_errs ;
    PPRBS_ChkByteMSB1_t bytes_msb;
    PPRBS_ChkByteLSB1_t bytes_lsb ;

    total_bit_errs.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkErr1.wrd) ;
    bytes_msb.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteMSB1.wrd) ;
    bytes_lsb.wrd = M2R_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteLSB1.wrd);

    total_bits = ((bytes_msb.wrd << 16 ) | (bytes_lsb.wrd)) * 8 ;
    if(total_bit_errs.wrd) {
	ber = total_bits/total_bit_errs.wrd ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS) ;
  return (ber) ;
}
/****************************************************************/
/* $rtn_hdr_start  RETURN PPRBS STATS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Both                                            */
cs_status m2r_pprbs_get_stats(cs_uint16 port_id, m2r_pprbs_chan_t chan)
/* INPUTS     : o Port Id                                       */
/*              o CHANNEL 0 OR CHANNEL 1                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Prints the statistics of PPRBS.                              */
/* The [chan] parameter is specified as following:              */
/* M2R_PPRBS_HOST_CHAN (1) or M2R_PPRBS_LINE_CHAN (2) or        */
/* M2R_PPRBS_CHAN_ALL (3)                                       */
/* M2R_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from  */
/* line side ( LIF ).                                           */
/* M2R_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from  */
/* SPI4.2 interface.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
  cs_uint16            sl_num ;

  M2R_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  M2R_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_PPRBS) ;

  if (chan & M2R_PPRBS_HOST_CHAN) {
    m2r_print_sect_stats(port_id, M2R_ID_PPRBS, M2R_PPRBS_STATS_HOST_SECT) ;
  }

  if (chan & M2R_PPRBS_LINE_CHAN) {
    m2r_print_sect_stats(port_id, M2R_ID_PPRBS, M2R_PPRBS_STATS_LINE_SECT) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_PPRBS) ;
  return (CS_OK) ;
}


/****************************************************************/
/*                  ISR ROUTINE FOR PPRBS  BLOCK                */
/****************************************************************/
cs_status m2r_pprbs_isr(cs_uint16 port_id)
{
  M2R_t              * pDev ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_PPRBS,
                    M2R_GRP_PPRBS_Interrupt,
                    &m2r_pprbs_irq_tbl[M2R_GRP_PPRBS_Interrupt],
		    (const char *) "PPRBS_Interrupt",
		    M2R_GRP_PPRBS_Interrupt_irq_id_name_tbl,
		    sizeof(M2R_GRP_PPRBS_Interrupt_irq_id_name_tbl) /
		    sizeof(m2r_irq_id_name_t)) ;

  return(CS_OK) ;
}



