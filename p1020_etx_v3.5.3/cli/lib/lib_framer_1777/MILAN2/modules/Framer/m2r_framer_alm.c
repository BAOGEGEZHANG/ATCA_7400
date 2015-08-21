/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_alm.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_framer.h"
#include "m2r_framer_priv.h"
#include "m2r_framer_cmn.h"
#include "m2r_framer_alm.h"

/******************* Alarm source table *******************/
typedef struct {
  cs_uint8  mask ;
  cs_uint8  src_val[4] ;
} _m2r_framer_tx_alarm_src ;

#define _M2R_FR_TX_ALM_SRC_CPU          (0x01)
#define _M2R_FR_TX_ALM_SRC_TOH          (0x02)
#define _M2R_FR_TX_ALM_SRC_APS          (0x10)
#define _M2R_FR_TX_ALM_SRC_RX_ALM       (0x20)

#define M2R_FR_GET_ALM_SRC_SEL_VAL(OH_BYTE, SRC)          \
        xmit_alarm_src_tbl[(OH_BYTE)].src_val[(SRC)]

const static _m2r_framer_tx_alarm_src
                   xmit_alarm_src_tbl[MAX_FR_ALARM_TYPE] = {
/*******************************************************
        | Mask  | Register |  TOH  |  APS  |  RX Alarm |
*******************************************************/
  CS_DI([ M2R_FR_ALM_RDI_P ] =)
        { 0x31, {   2,         0,     0,       1     }},
  CS_DI([ M2R_FR_ALM_REI_P ] =)
        { 0x31, {   2,         0,     0,       1     }},
  CS_DI([ M2R_FR_ALM_AIS_P ] =)
        { 0x00, {   0,         0,     0,       0     }},
  CS_DI([ M2R_FR_ALM_RDI_L ] =)
        { 0x31, {   2,         0,     3,       0     }},
  CS_DI([ M2R_FR_ALM_REI_L ] =)
        { 0x31, {   2,         0,     0,       1     }},
  CS_DI([ M2R_FR_ALM_AIS_L ] =)
        { 0x11, {   0,         0,     1,       0     }}
} ;


/****************************************************************/
/* $rtn_hdr_start SELECT TX ALARM SOURCE                        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_alm_set_alm_src(cs_uint16 port_id,
                                    m2r_framer_alm_t alarm,
                                    m2r_framer_alm_src_t src)
/* INPUTS     : o Port Id                                       */
/*              o Alarm Type                                    */
/*              o Alarm Source                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select the source of TX alarm.                               */
/*                                                              */
/* The [alarm] parameter is specified as following:             */
/* M2R_FR_ALM_AIS_P or M2R_FR_ALM_RDI_P or M2R_FR_ALM_REI_P or  */
/* M2R_FR_ALM_AIS_L or M2R_FR_ALM_RDI_L or M2R_FR_ALM_REI_L     */
/*                                                              */
/* The [src] parameter is specified as following:               */
/* M2R_FR_ALM_SRC_CPU   or   M2R_FR_ALM_SRC_APS_PORT   or       */
/* M2R_FR_ALM_SRC_RX_ALARM                                      */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num, mask, alm_type = alarm ;
  M2R_t                 * pDev ;

  switch (src) {
    case M2R_FR_ALM_SRC_CPU  :
      mask = _M2R_FR_TX_ALM_SRC_CPU ;
      break ;

    case M2R_FR_ALM_SRC_APS_PORT :
      mask = _M2R_FR_TX_ALM_SRC_APS ;
      break ;

    case M2R_FR_ALM_SRC_RX_ALARM :
      mask = _M2R_FR_TX_ALM_SRC_RX_ALM ;
      break ;

    default :
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
        ": Invalid alarm source");
      return ( CS_ERROR ) ;
  }

  if (( xmit_alarm_src_tbl[alarm].mask & mask ) == 0) {
    CS_HNDL_ERROR(port_id, EM2R_OVRHD_SRC_NOT_VALID,
                  "Alarm can't be sourced from the specified port\n");
    return (CS_ERROR);
  }

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  switch (alm_type) {
    case M2R_FR_ALM_RDI_P : {
      FR_XMITOVERHEAD_ByteCtrl2_t byteCtl2 ;
      byteCtl2.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
      byteCtl2.bf.RDIPSrc = M2R_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                   byteCtl2.wrd) ;
    }
    break ;
    case M2R_FR_ALM_REI_P : {
      FR_XMITOVERHEAD_ByteCtrl2_t byteCtl2 ;
      byteCtl2.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
      byteCtl2.bf.REIPSrc = M2R_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                   byteCtl2.wrd) ;
    }
    break ;
    case M2R_FR_ALM_AIS_L : {
      FR_XMITOVERHEAD_LineAlarmDefectCtrl_t lineCtl ;
      lineCtl.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.LineAlarmDefectCtrl.wrd) ;
      lineCtl.bf.AISLSrc = M2R_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LineAlarmDefectCtrl.wrd,
                   lineCtl.wrd) ;
    }
    break ;
    case M2R_FR_ALM_RDI_L : {
      FR_XMITOVERHEAD_ByteCtrl1_t  byteCtl1 ;
      byteCtl1.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
      byteCtl1.bf.K2RDISrc = M2R_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                   byteCtl1.wrd) ;
    }
    case M2R_FR_ALM_REI_L : {
      FR_XMITOVERHEAD_ByteCtrl2_t byteCtl2 ;
      byteCtl2.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
      byteCtl2.bf.M1Src = M2R_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                   byteCtl2.wrd) ;
    }
    break ;
    case M2R_FR_ALM_AIS_P :
    break ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start INSERT LINE ALARM IN TX STREAM                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_alm_insert_line_alarm(cs_uint16 port_id,
                                           m2r_framer_alm_t alm_type,
                                           cs_uint8 b2_err_cnt,
                                           m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Alarm Type                                    */
/*              o B2 Error count in case of REI-L alarm         */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable insertion of SONET Line alarm in the TX       */
/* Stream.                                                      */
/* When enabled, SONET Line alarms are sourced from the CPU     */
/* registers.                                                   */
/* Note : User should also call the m2r_framer_alm_ctl_alm_src()*/
/* API to enable sourcing of alarms from the CPU registers.     */
/*                                                              */
/* The [alm_type] parameter can be one of the following :       */
/* M2R_FR_ALM_AIS_L or M2R_FR_ALM_RDI_L or M2R_FR_ALM_REI_L     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num, alarm = alm_type ;
  M2R_t                       * pDev ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  switch (alarm) {
    case M2R_FR_ALM_AIS_L : {
      FR_XMITOVERHEAD_alarm1_t      alarm1 ;

      alarm1.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_alarm1.wrd) ;
      alarm1.bf.insAISL = (ctl == M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_alarm1.wrd,
                   alarm1.wrd) ;
    }
    break ;
    case M2R_FR_ALM_RDI_L : {
      FR_XMITOVERHEAD_alarm1_t      alarm1 ;

      alarm1.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_alarm1.wrd) ;
      alarm1.bf.insRDIL = (ctl == M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_alarm1.wrd,
                   alarm1.wrd) ;
    }
    break ;
    case M2R_FR_ALM_REI_L : {
      FR_XMITOVERHEAD_alarm2_t      alarm2 ;

      alarm2.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_alarm2.wrd) ;
      alarm2.bf.B2ErCnt = (ctl == M2R_ENABLE) ? b2_err_cnt : 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_alarm2.wrd,
                   alarm2.wrd) ;
    }
    break ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start INSERT PATH LEVEL SONET ALARM IN TX STREAM    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_alm_insert_path_alarm(cs_uint16 port_id,
                                           m2r_framer_stream_t channel,
                                           m2r_framer_alm_t alm_type,
                                           cs_uint8 b3_err_cnt,
                                           m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o Alarm Type                                    */
/*              o B3 Error count in case of REI-P alarm         */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable insertion of SONET path alarm in the TX       */
/* Stream.                                                      */
/* When enabled, SONET path alarms are sourced from the CPU     */
/* registers.                                                   */
/* Note : User should also call the m2r_framer_alm_ctl_alm_src()*/
/* API to enable sourcing of alarms from the CPU registers.     */
/*                                                              */
/* The [alm_type] parameter can be one of the following :       */
/* M2R_FR_ALM_AIS_P or M2R_FR_ALM_RDI_P or M2R_FR_ALM_REI_P     */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  volatile cs_reg             * regaddr ;
  cs_uint32                     str_start, str_end, str_iter, alarm = alm_type ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel);

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    switch (alarm) {
      case M2R_FR_ALM_AIS_P:
      break ;
      case M2R_FR_ALM_RDI_P: {
        FR_XMITOVERHEAD_alarm3Path_t  alarm3 ;

        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.xmitOverhead_alarm3Path.wrd +
                          (channel * M2R_FR_XMITOVERHEAD_alarm3Path_STRIDE) ;
        alarm3.wrd = M2R_REG_READ(regaddr) ;
        alarm3.bf.insRDIP = (ctl == M2R_ENABLE) ? 4 : 1 ;
        M2R_REG_WRITE(regaddr, alarm3.wrd) ;
      }
      break ;
      case M2R_FR_ALM_REI_P: {
        FR_XMITOVERHEAD_alarm4Path_t  alarm4 ;

        regaddr = (cs_reg *)
            &pDev->slice[sl_num].Framer.xmitOverhead_alarm4Path.wrd +
             (channel * M2R_FR_XMITOVERHEAD_alarm4Path_STRIDE) ;
        alarm4.wrd = M2R_REG_READ(regaddr) ;
        alarm4.bf.B3ErCnt = (ctl == M2R_ENABLE) ? b3_err_cnt : 0 ;
        M2R_REG_WRITE(regaddr, alarm4.wrd) ;
      }
      break ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE ENHANCED RDI-P MODE            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_alm_ctl_enh_rdi_p_mode(cs_uint16 port_id,
                                      m2r_framer_stream_t channel,
                                      m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable Enhanced RDI-P Mode. When ENABLED for RX Side */
/* RDI-P received in G1 Byte is decoded and interpreted         */
/* according to enhanced RDI-P codes.                           */
/* When ENABLED for TX Side, RDI-P is inserted in TX Stream     */
/* encoded as enhanced RDI-P else 1 bit RDI is inserted.        */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  volatile cs_reg             * regaddr ;
  cs_uint32                     str_start, str_end, str_iter ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel);

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
      FR_SONET32RX_PathProvision_t  rx_prvsn ;

      regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvision.wrd +
                    (str_iter * M2R_FR_SONET32RX_PathProvision_STRIDE) ;
      rx_prvsn.wrd = M2R_REG_READ(regaddr) ;
      rx_prvsn.bf.EnhRDI = (ctl == M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, rx_prvsn.wrd) ;
    }

    if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
      FR_XMITOVERHEAD_PathAlarmDefectCtrl_t tx_prvsn ;

      regaddr = (cs_reg *)
          &pDev->slice[sl_num].Framer.PathAlarmDefectCtrl.wrd +
          (str_iter * M2R_FR_XMITOVERHEAD_PathAlarmDefectCtrl_STRIDE) ;

      tx_prvsn.wrd = M2R_REG_READ(regaddr) ;
      tx_prvsn.bf.EnhRDI = (ctl == M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, tx_prvsn.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE AIS-L AUTO INSERTION MODE      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_alm_ctl_auto_ins_mode(cs_uint16 port_id,
                                           m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable Auto AIS-L insertion mode.                    */
/* autoInsertion mode controls the insertion of the Line AIS    */
/* under line alarm conditions - LOS, LOF.                      */
/* When ENABLED, Line AIS is inserted in the transmit stream    */
/* automatically when LOS and LOF are detected.                 */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                   sl_num ;
  M2R_t                     * pDev ;
  volatile cs_reg           * regaddr ;
  FR_SONET32RX_SecLineAlarmtoTxControl_t sectLineCtl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  sectLineCtl.wrd = M2R_REG_READ(regaddr) ;

  sectLineCtl.bf.AutoIns = (ctl == M2R_ENABLE) ? 1 : 0 ;

  M2R_REG_WRITE(regaddr, sectLineCtl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE RDI-P INSERTION ON DEFECT      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_insert_rdi_p_on_defect(cs_uint16 port_id,
                                m2r_framer_path_defect_t defect,
                                m2r_framer_stream_t channel,
                                m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Path Defect Type                              */
/*              o Stream ID                                     */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the mapping of path level defect specified to */
/* insert Path RDI.                                             */
/* When ENABLED for receive direction, If the defect specified  */
/* is observed on the SONET RX Stream, RDI-P is inserted by the */
/* alarm processor in SONET TX Stream.                          */
/*                                                              */
/* When enabled for transmit direction, RDI-P is inserted in    */
/* TX stream if the mate framer detects the [defect] and notfies*/
/* Framer through APS Port.                                     */
/*                                                              */
/* The [defect] parameter is specified as following (RX) :      */
/* M2R_FR_PATH_DEF_PTU or M2R_FR_PATH_DEF_PTM or                */
/* M2R_FR_PATH_DEF_PSLU or M2R_FR_PATH_DEF_PSLM or              */
/* M2R_FR_PATH_DEF_UNEQP or M2R_FR_PATH_DEF_ALL                 */
/*                                                              */
/* The [defect] parameter is specified as following (TX) :      */
/* M2R_FR_PATH_DEF_PTU or M2R_FR_PATH_DEF_PTM or                */
/* M2R_FR_PATH_DEF_PSLU or M2R_FR_PATH_DEF_PSLM or              */
/* M2R_FR_PATH_DEF_AIS or                                       */
/* M2R_FR_PATH_DEF_UNEQP  or M2R_FR_PATH_DEF_ALL                */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                   sl_num ;
  M2R_t                     * pDev ;
  volatile cs_reg           * regaddr ;
  cs_uint32                   str_start, str_end, str_iter ;
  cs_uint8                    bit_ctl = ((ctl == M2R_ENABLE) ? 1 : 0) ;
  FR_SONET32RX_PRDITxInsert_t rx_p_rdi ;
  FR_XMITOVERHEAD_PathAlarmDefectCtrl_t tx_p_rdi ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel);

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {

    if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
      regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PRDITxInsert.wrd +
                    (str_iter * M2R_FR_SONET32RX_PRDITxInsert_STRIDE) ;
      rx_p_rdi.wrd = M2R_REG_READ(regaddr) ;

      if (defect & M2R_FR_PATH_DEF_PTU) {
        rx_p_rdi.bf.PTU2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_PTM) {
        rx_p_rdi.bf.PTM2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_PSLU) {
        rx_p_rdi.bf.PSLU2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_PSLM) {
        rx_p_rdi.bf.PSLM2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_UNEQP) {
        rx_p_rdi.bf.UNEQP2RDI = bit_ctl ;
      }
      M2R_REG_WRITE(regaddr, rx_p_rdi.wrd) ;
    }

    if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
      regaddr = (cs_reg *)
                 &pDev->slice[sl_num].Framer.PathAlarmDefectCtrl.wrd +
                 (str_iter * M2R_FR_XMITOVERHEAD_PathAlarmDefectCtrl_STRIDE) ;

      tx_p_rdi.wrd = M2R_REG_READ(regaddr) ;

      if (defect & M2R_FR_PATH_DEF_PTU) {
        tx_p_rdi.bf.PTU2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_PTM) {
        tx_p_rdi.bf.PTM2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_PSLU) {
        tx_p_rdi.bf.PSLU2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_PSLM) {
        tx_p_rdi.bf.PSLM2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_UNEQP) {
        tx_p_rdi.bf.UNEQP2RDI = bit_ctl ;
      }

      if (defect & M2R_FR_PATH_DEF_AIS) {
        tx_p_rdi.bf.PAIS2RDI = bit_ctl ;
      }
      M2R_REG_WRITE(regaddr, tx_p_rdi.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE PAYLOAD PROCESSING ON DEFECT   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_drop_pld_on_path_defect(cs_uint16 port_id,
                                m2r_framer_path_defect_t defect,
                                m2r_framer_stream_t channel,
                                m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Path Defect Type                              */
/*              o Stream ID                                     */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable processing of SONET payload when path level   */
/* defect is observed. When enabled and path defect is observed,*/
/* the entire payload in the current Frame is dropped. If       */
/* disabled (by default) path defects has no effect of paylaod  */
/* processing.                                                  */
/*                                                              */
/* The [defect] parameter is specified as following:            */
/* M2R_FR_PATH_DEF_PTU or M2R_FR_PATH_DEF_PTM or                */
/* M2R_FR_PATH_DEF_PSLU or M2R_FR_PATH_DEF_PSLM or              */
/* M2R_FR_PATH_DEF_UNEQP  or M2R_FR_PATH_DEF_ALL                */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16          sl_num ;
  M2R_t            * pDev ;
  volatile cs_reg  * regaddr ;
  cs_uint32          str_start, str_end, str_iter ;
  cs_uint8           bit_ctl = ((ctl == M2R_ENABLE) ? 1 : 0) ;
  FR_SONET32RX_PRDITxInsert_t  prdi_drop_ctl;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel);

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PRDITxInsert.wrd +
                  (str_iter * M2R_FR_SONET32RX_PRDITxInsert_STRIDE) ;
    prdi_drop_ctl.wrd = M2R_REG_READ(regaddr) ;

    if (defect & M2R_FR_PATH_DEF_PTU) {
      prdi_drop_ctl.bf.PTU2Drop = bit_ctl ;
    }

    if (defect & M2R_FR_PATH_DEF_PTM) {
      prdi_drop_ctl.bf.PTM2Drop = bit_ctl ;
    }

    if (defect & M2R_FR_PATH_DEF_PSLU) {
      prdi_drop_ctl.bf.PSLU2Drop = bit_ctl ;
    }

    if (defect & M2R_FR_PATH_DEF_PSLM) {
      prdi_drop_ctl.bf.PSLM2Drop = bit_ctl ;
    }

    if (defect & M2R_FR_PATH_DEF_UNEQP) {
      prdi_drop_ctl.bf.UNEQP2Drop = bit_ctl ;
    }

    M2R_REG_WRITE(regaddr, prdi_drop_ctl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start GET THE CURRENT STATUS OF PATH LEVEL DEFECTS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_uint16 m2r_framer_alm_get_path_status(cs_uint16 port_id,
                                         m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Bitmap of the currently active defects          */
/* DESCRIPTION:                                                 */
/* Get the Path status. This API returns the bitmap of the      */
/* alarms/defects that are currently active. The bitmap should  */
/* be intrepreted as bitmask of the m2r_framer_path_defect_t.   */
/* Zero is returned if No defects are active.                   */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16          sl_num, status = 0 ;
  M2R_t            * pDev ;
  volatile cs_reg  * regaddr ;
  FR_SONET32RX_APSPathTxInsert_t path_def ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel >= M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.APSPathTxInsert.wrd +
                (channel * M2R_FR_SONET32RX_APSPathTxInsert_STRIDE) ;
  path_def.wrd = M2R_REG_READ(regaddr) ;

  if (path_def.bf.dPTU)
  {
    status |= M2R_FR_PATH_DEF_PTU ;
  }
  if (path_def.bf.dPTM)
  {
    status |= M2R_FR_PATH_DEF_PTM ;
  }
  if (path_def.bf.dPSLU)
  {
    status |= M2R_FR_PATH_DEF_PSLU ;
  }
  if (path_def.bf.dPSLM)
  {
    status |= M2R_FR_PATH_DEF_PSLM ;
  }
  if (path_def.bf.dUNEQP)
  {
    status |= M2R_FR_PATH_DEF_UNEQP ;
  }
  if (path_def.bf.dLOP)
  {
    status |= M2R_FR_PATH_DEF_LOP ;
  }
  if (path_def.bf.dPAIS)
  {
    status |= M2R_FR_PATH_DEF_AIS ;
  }
  if (path_def.bf.dPRDI)
  {
    status |= M2R_FR_PATH_DEF_RDI ;
  }

  return (status) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE LINE RDI INSERTION ON DEFECT  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_insert_rdi_l_on_defect(cs_uint16 port_id,
                             m2r_framer_sect_line_defect_t defect,
                             m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Defect Type                                   */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable RDI-L insertion in SONET TX Stream, when      */
/* defect specified in the [defect] parameter is active.        */
/*                                                              */
/* The [defect] parameter is specified as following:            */
/* M2R_FR_SECT_LINE_DEF_STM or M2R_FR_SECT_LINE_DEF_STU or      */
/* M2R_FR_SECT_LINE_DEF_BERSF or M2R_FR_SECT_LINE_DEF_SD or     */
/* M2R_FR_SECT_LINE_DEF_ALL                                     */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                   sl_num ;
  M2R_t                     * pDev ;
  volatile cs_reg           * regaddr ;
  cs_uint8                    bit_ctl = ((ctl == M2R_ENABLE) ? 1 : 0) ;
  FR_SONET32RX_SecLineAlarmtoTxControl_t rdi_tx_ctl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  rdi_tx_ctl.wrd = M2R_REG_READ(regaddr) ;

  if (defect & M2R_FR_SECT_LINE_DEF_STM) {
    rdi_tx_ctl.bf.STM2RDIL = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_STU) {
    rdi_tx_ctl.bf.STU2RDIL = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_BERSF) {
    rdi_tx_ctl.bf.BERSF2RDIL = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_SD) {
    rdi_tx_ctl.bf.SD2RDIL = bit_ctl ;
  }

  M2R_REG_WRITE(regaddr, rdi_tx_ctl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE LINE AIS INSERTION ON DEFECT  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_insert_ais_l_on_defect(cs_uint16 port_id,
                             m2r_framer_sect_line_defect_t defect,
                             m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Defect Type                                   */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable AIS-L insertion in SONET TX Stream when       */
/* defect condition specified in the [defect] parameter is      */
/* active.                                                      */
/*                                                              */
/* The [defect] parameter is specified as following:            */
/* M2R_FR_SECT_LINE_DEF_STM or M2R_FR_SECT_LINE_DEF_STU or      */
/* M2R_FR_SECT_LINE_DEF_BERSF or M2R_FR_SECT_LINE_DEF_SD or     */
/* M2R_FR_SECT_LINE_DEF_ALL                                     */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                   sl_num ;
  M2R_t                     * pDev ;
  volatile cs_reg           * regaddr ;
  cs_uint8                    bit_ctl = ((ctl == M2R_ENABLE) ? 1 : 0) ;
  FR_SONET32RX_SecLineAlarmtoTxControl_t rdi_tx_ctl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  rdi_tx_ctl.wrd = M2R_REG_READ(regaddr) ;

  if (defect & M2R_FR_SECT_LINE_DEF_STM) {
    rdi_tx_ctl.bf.STM2AISL = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_STU) {
    rdi_tx_ctl.bf.STU2AISL = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_BERSF) {
    rdi_tx_ctl.bf.BERSF2AISL = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_SD) {
    rdi_tx_ctl.bf.SD2AISL = bit_ctl ;
  }

  M2R_REG_WRITE(regaddr, rdi_tx_ctl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PAYLOAD PROCESSING ON DEFECT  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_drop_pld_on_line_defect(cs_uint16 port_id,
                             m2r_framer_sect_line_defect_t defect,
                             m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Defect Type                                   */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable processing of SONET paylaod when defect       */
/* specified is observed. When enabled and defect specified is  */
/* observed, the entire payload is dropped with no further      */
/* processing of  the payload. When Disabled, defect condition  */
/* has no effect on further processing of the payload.          */
/*                                                              */
/* The [defect] parameter is specified as following:            */
/* M2R_FR_SECT_LINE_DEF_STM or M2R_FR_SECT_LINE_DEF_STU or      */
/* M2R_FR_SECT_LINE_DEF_BERSF or M2R_FR_SECT_LINE_DEF_SD or     */
/* M2R_FR_SECT_LINE_DEF_ALL                                     */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                   sl_num ;
  M2R_t                     * pDev ;
  volatile cs_reg           * regaddr ;
  cs_uint8                    bit_ctl = ((ctl == M2R_ENABLE) ? 1 : 0) ;
  FR_SONET32RX_SecLineAlarmtoTxControl_t rdi_tx_ctl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  rdi_tx_ctl.wrd = M2R_REG_READ(regaddr) ;

  if (defect & M2R_FR_SECT_LINE_DEF_STM) {
    rdi_tx_ctl.bf.STM2Drop = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_STU) {
    rdi_tx_ctl.bf.STU2Drop = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_BERSF) {
    rdi_tx_ctl.bf.BERSF2Drop = bit_ctl ;
  }

  if (defect & M2R_FR_SECT_LINE_DEF_SD) {
    rdi_tx_ctl.bf.SD2Drop = bit_ctl ;
  }

  M2R_REG_WRITE(regaddr, rdi_tx_ctl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start GET SECTION/LINE DEFECT STATUS                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_uint16 m2r_framer_alm_get_sect_line_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Bitmap of the currently active defects          */
/* DESCRIPTION:                                                 */
/* Get the status of any section or line defects that are       */
/* active currently. The return value is the bitmap of the      */
/* m2r_framer_sect_line_defect_t enums.                         */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                   sl_num, status = 0 ;
  M2R_t                     * pDev ;
  FR_SONET32RX_APSTxSecLineAlarm_t  sect_line_alm ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  sect_line_alm.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.APSTxSecLineAlarm.wrd) ;

  if (sect_line_alm.bf.dSTU) {
    status |= M2R_FR_SECT_LINE_DEF_STU ;
  }

  if (sect_line_alm.bf.dSTM) {
    status |= M2R_FR_SECT_LINE_DEF_STM ;
  }

  if (sect_line_alm.bf.dRDIL) {
    status |= M2R_FR_SECT_LINE_DEF_RDI ;
  }

  if (sect_line_alm.bf.dSD) {
    status |= M2R_FR_SECT_LINE_DEF_SD ;
  }

  if (sect_line_alm.bf.dAISL) {
    status |= M2R_FR_SECT_LINE_DEF_AIS ;
  }

  if (sect_line_alm.bf.dSF) {
    status |= M2R_FR_SECT_LINE_DEF_SF ;
  }

  if (sect_line_alm.bf.dLOF) {
    status |= M2R_FR_SECT_LINE_DEF_LOF ;
  }

  if (sect_line_alm.bf.dSEF) {
    status |= M2R_FR_SECT_LINE_DEF_SEF ;
  }

  if (sect_line_alm.bf.dLOS) {
    status |= M2R_FR_SECT_LINE_DEF_LOS ;
  }

  return (status) ;
}

/****************************************************************/
/* $rtn_hdr_start SELECT ALM OUTPUT TO PIN SNTRXALMPORT         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_alm_sel_rxalmport_src(cs_uint16 port_id,
                        m2r_framer_rxalmport_src_t sel)
/* INPUTS     : o Port Id                                       */
/*              o Rx Alm Source                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selects the alarm to be output on an external sntRxAlmPort   */
/* pin.                                                         */
/*                                                              */
/* [sel] : M2R_FR_RXALMPORT_LOS                                 */
/*         M2R_FR_RXALMPORT_LOF                                 */
/*         M2R_FR_RXALMPORT_AIS_L                               */
/*         M2R_FR_RXALMPORT_SF                                  */
/*         M2R_FR_RXALMPORT_AIS_P0                              */
/*         M2R_FR_RXALMPORT_AIS_P1                              */
/*         M2R_FR_RXALMPORT_AIS_P2                              */
/*         M2R_FR_RXALMPORT_AIS_P3                              */
/*         M2R_FR_RXALMPORT_LOP_P0                              */
/*         M2R_FR_RXALMPORT_LOP_P1                              */
/*         M2R_FR_RXALMPORT_LOP_P2                              */
/*         M2R_FR_RXALMPORT_LOP_P3                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                       * pDev ;
  FR_SONET32RX_Configuration_t  cfg ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  cfg.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
  cfg.bf.rxAlmPort = sel ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd, cfg.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

