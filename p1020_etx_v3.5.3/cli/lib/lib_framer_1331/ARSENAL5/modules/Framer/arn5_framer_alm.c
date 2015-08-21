/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_alm.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_framer.h"
#include "arn5_framer_priv.h"
#include "arn5_framer_alm.h"

/******************* Alarm source table *******************/
typedef struct {
  cs_uint8  mask ;
  cs_uint8  src_val[4] ;
} _arn5_framer_tx_alarm_src ;

#define _ARN5_FR_TX_ALM_SRC_CPU         (0x01)
#define _ARN5_FR_TX_ALM_SRC_TOH         (0x02)
#define _ARN5_FR_TX_ALM_SRC_APS         (0x10)
#define _ARN5_FR_TX_ALM_SRC_RX_ALM      (0x20)

#define ARN5_FR_GET_ALM_SRC_SEL_VAL(OH_BYTE, SRC)          \
        xmit_alarm_src_tbl[(OH_BYTE)].src_val[(SRC)]

const static _arn5_framer_tx_alarm_src
                   xmit_alarm_src_tbl[MAX_FR_ALARM_TYPE] = {
/*******************************************************
        | Mask  | Register |  TOH  |  APS  |  RX Alarm |
*******************************************************/
  [ ARN5_FR_ALM_RDI_P ] =
        { 0x21, {   2,         0,     0,       1     }},
  [ ARN5_FR_ALM_REI_P ] =
        { 0x21, {   2,         0,     0,       1     }},
  [ ARN5_FR_ALM_AIS_P ] =
        { 0x00, {   0,         0,     0,       0     }},
  [ ARN5_FR_ALM_RDI_L ] =
        { 0x21, {   2,         0,     3,       0     }},
  [ ARN5_FR_ALM_REI_L ] =
        { 0x21, {   2,         0,     0,       1     }},
  [ ARN5_FR_ALM_AIS_L ] =
        { 0x01, {   0,         0,     1,       0     }},
} ;


/****************************************************************/
/* $rtn_hdr_start SELECT TX ALARM SOURCE                        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_alm_set_alm_src(cs_uint16 port_id,
                        arn5_framer_alm_t alarm,
                        arn5_framer_alm_src_t src)
/* INPUTS     : o Port Id                                       */
/*              o Alarm Type                                    */
/*              o Alarm Source                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select the source of TX alarm.                               */
/*                                                              */
/* The [alarm] parameter is specified as following:             */
/* ARN5_FR_ALM_AIS_P or ARN5_FR_ALM_RDI_P or ARN5_FR_ALM_REI_P or*/
/* ARN5_FR_ALM_AIS_L or ARN5_FR_ALM_RDI_L or ARN5_FR_ALM_REI_L  */
/*                                                              */
/* The [src] parameter is specified as following:               */
/* ARN5_FR_OVRHD_SRC_CPU  or  ARN5_FR_OVRHD_SRC_RX_ALARM        */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num, mask, alm_type = alarm ;
  ARN5_t                * pDev ;

  switch (src) {
    case ARN5_FR_ALM_SRC_CPU  :
      mask = _ARN5_FR_TX_ALM_SRC_CPU ;
      break ;

    case ARN5_FR_ALM_SRC_RX_ALARM :
      mask = _ARN5_FR_TX_ALM_SRC_RX_ALM ;
      break ;

    default :
      CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
        ": Invalid alarm source");
      return ( CS_ERROR ) ;
  }

  if (( xmit_alarm_src_tbl[alarm].mask & mask ) == 0) {
    CS_HNDL_ERROR(port_id, EARN5_OVRHD_SRC_NOT_VALID,
                  "Alarm can't be sourced from the specified port\n");
    return (CS_ERROR);
  }

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  switch (alm_type) {
    case ARN5_FR_ALM_RDI_P : {
      ARN5_FR_XMITOVERHEAD_ByteCtrl2_t byteCtl2 ;
      byteCtl2.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
      byteCtl2.bf.RDIPSrc = ARN5_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                   byteCtl2.wrd) ;
    }
    break ;
    case ARN5_FR_ALM_REI_P : {
      ARN5_FR_XMITOVERHEAD_ByteCtrl2_t byteCtl2 ;
      byteCtl2.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
      byteCtl2.bf.REIPSrc = ARN5_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                   byteCtl2.wrd) ;
    }
    break ;
    case ARN5_FR_ALM_AIS_L : {
      ARN5_FR_XMITOVERHEAD_LineAlarmDefectCtrl_t lineCtl ;
      lineCtl.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.LineAlarmDefectCtrl.wrd) ;
      lineCtl.bf.AISLSrc = ARN5_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.LineAlarmDefectCtrl.wrd,
                   lineCtl.wrd) ;
    }
    break ;
    case ARN5_FR_ALM_RDI_L : {
      ARN5_FR_XMITOVERHEAD_ByteCtrl1_t  byteCtl1 ;
      byteCtl1.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
      byteCtl1.bf.K2RDISrc = ARN5_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                   byteCtl1.wrd) ;
    }
    case ARN5_FR_ALM_REI_L : {
      ARN5_FR_XMITOVERHEAD_ByteCtrl2_t byteCtl2 ;
      byteCtl2.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
      byteCtl2.bf.M1Src = ARN5_FR_GET_ALM_SRC_SEL_VAL(alarm, src) ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                   byteCtl2.wrd) ;
    }
    break ;
    case ARN5_FR_ALM_AIS_P :
    break ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;

}


/****************************************************************/
/* $rtn_hdr_start INSERT LINE ALARM IN TX STREAM                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_alm_insert_line_alarm(cs_uint16 port_id,
                        arn5_framer_alm_t alm_type,
                        cs_uint8 b2_err_cnt, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Alarm Type                                    */
/*              o B2 Error count in case of REI-L alarm         */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable insertion of SONET Line alarm in the TX       */
/* Stream.                                                      */
/* When enabled, SONET Line alarms are sourced from the CPU     */
/* registers.                                                   */
/* Note : User should also call arn5_framer_alm_ctl_alm_src()   */
/* API to enable sourcing of alarms from the CPU registers.     */
/*                                                              */
/* The [alm_type] parameter can be one of the following :       */
/* ARN5_FR_ALM_AIS_L or ARN5_FR_ALM_RDI_L or ARN5_FR_ALM_REI_L  */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16     sl_num, alarm = alm_type ;
  ARN5_t        * pDev ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  switch (alarm) {
    case ARN5_FR_ALM_AIS_L : {
      ARN5_FR_XMITOVERHEAD_alarm1_t      alarm1 ;

      alarm1.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.alarm1.wrd) ;
      alarm1.bf.insAISL = (ctl == CS_ENABLE) ? 1 : 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.alarm1.wrd,
                   alarm1.wrd) ;
    }
    break ;
    case ARN5_FR_ALM_RDI_L : {
      ARN5_FR_XMITOVERHEAD_alarm1_t      alarm1 ;

      alarm1.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.alarm1.wrd) ;
      alarm1.bf.insRDIL = (ctl == CS_ENABLE) ? 1 : 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.alarm1.wrd,
                   alarm1.wrd) ;
    }
    break ;
    case ARN5_FR_ALM_REI_L : {
      ARN5_FR_XMITOVERHEAD_alarm2_t      alarm2 ;

      alarm2.wrd =
          CS_REG_READ(&pDev->slice[sl_num].Framer.alarm2.wrd) ;
      alarm2.bf.B2ErCnt = (ctl == CS_ENABLE) ? b2_err_cnt : 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.alarm2.wrd,
                   alarm2.wrd) ;
    }
    break ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start INSERT PATH LEVEL SONET ALARM IN TX STREAM    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_alm_insert_path_alarm(cs_uint16 port_id,
                        arn5_framer_alm_t alm_type,
                        cs_uint8 b3_err_cnt, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Alarm Type                                    */
/*              o B3 Error count in case of REI-P alarm         */
/*              o CS_ENABLE (1) or CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable insertion of SONET path alarm in the TX       */
/* Stream.                                                      */
/* When enabled, SONET path alarms are sourced from the CPU     */
/* registers.                                                   */
/* Note : User should also call arn5_framer_alm_ctl_alm_src()   */
/* API to enable sourcing of alarms from the CPU registers.     */
/*                                                              */
/* The [alm_type] parameter can be one of the following :       */
/* ARN5_FR_ALM_AIS_P or ARN5_FR_ALM_RDI_P or ARN5_FR_ALM_REI_P  */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint32             alarm = alm_type ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  switch (alarm) {
    case ARN5_FR_ALM_AIS_P:
    break ;
    case ARN5_FR_ALM_RDI_P: {
      ARN5_FR_XMITOVERHEAD_alarm3Path_t  alarm3 ;

      regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.alarm3Path.wrd ;
      alarm3.wrd = CS_REG_READ(regaddr) ;
      alarm3.bf.insRDIP = (ctl == CS_ENABLE) ? 4 : 1 ;
      CS_REG_WRITE(regaddr, alarm3.wrd) ;
    }
    break ;
    case ARN5_FR_ALM_REI_P: {
      ARN5_FR_XMITOVERHEAD_alarm4Path_t  alarm4 ;

      regaddr = (cs_reg *)
          &pDev->slice[sl_num].Framer.alarm4Path.wrd ;
      alarm4.wrd = CS_REG_READ(regaddr) ;
      alarm4.bf.B3ErCnt = (ctl == CS_ENABLE) ? b3_err_cnt : 0 ;
      CS_REG_WRITE(regaddr, alarm4.wrd) ;
    }
    break ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE ENHANCED RDI-P MODE            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_alm_ctl_enh_rdi_p_mode(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable Enhanced RDI-P Mode. When ENABLED for RX Side */
/* RDI-P received in G1 Byte is decoded and interpreted         */
/* according to enhanced RDI-P codes.                           */
/* When ENABLED for TX Side, RDI-P is inserted in TX Stream     */
/* encoded as enhanced RDI-P else 1 bit RDI is inserted.        */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
    ARN5_FR_SONET32RX_PathProvision_t  rx_prvsn ;

    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvision.wrd ;
    rx_prvsn.wrd = CS_REG_READ(regaddr) ;
    rx_prvsn.bf.EnhRDI = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, rx_prvsn.wrd) ;
  }

  if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
    ARN5_FR_XMITOVERHEAD_PathAlarmDefectCtrl_t tx_prvsn ;

    regaddr = (cs_reg *)
        &pDev->slice[sl_num].Framer.PathAlarmDefectCtrl.wrd ;

    tx_prvsn.wrd = CS_REG_READ(regaddr) ;
    tx_prvsn.bf.EnhRDI = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, tx_prvsn.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE AIS-L AUTO INSERTION MODE      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_alm_ctl_auto_ins_mode(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
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
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_SONET32RX_SecLineAlarmtoTxControl_t sectLineCtl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  sectLineCtl.wrd = CS_REG_READ(regaddr) ;

  sectLineCtl.bf.AutoIns = (ctl == CS_ENABLE) ? 1 : 0 ;

  CS_REG_WRITE(regaddr, sectLineCtl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE RDI-P INSERTION ON DEFECT      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_insert_rdi_p_on_defect(cs_uint16 port_id,
                        arn5_framer_path_defect_t defect,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Path Defect Type                              */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
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
/* ARN5_FR_PATH_DEF_PTU or ARN5_FR_PATH_DEF_PTM or              */
/* ARN5_FR_PATH_DEF_PSLU or ARN5_FR_PATH_DEF_PSLM or            */
/* ARN5_FR_PATH_DEF_UNEQP or ARN5_FR_PATH_DEF_ALL               */
/*                                                              */
/* The [defect] parameter is specified as following (TX) :      */
/* ARN5_FR_PATH_DEF_PTU or ARN5_FR_PATH_DEF_PTM or              */
/* ARN5_FR_PATH_DEF_PSLU or ARN5_FR_PATH_DEF_PSLM or            */
/* ARN5_FR_PATH_DEF_AIS or                                      */
/* ARN5_FR_PATH_DEF_UNEQP  or ARN5_FR_PATH_DEF_ALL              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint8              bit_ctl = ((ctl == CS_ENABLE) ? 1 : 0) ;
  ARN5_FR_SONET32RX_PRDITxInsert_t rx_p_rdi ;
  ARN5_FR_XMITOVERHEAD_PathAlarmDefectCtrl_t tx_p_rdi ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PRDITxInsert.wrd ;
    rx_p_rdi.wrd = CS_REG_READ(regaddr) ;

    if (defect & ARN5_FR_PATH_DEF_PTU) {
      rx_p_rdi.bf.PTU2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_PTM) {
      rx_p_rdi.bf.PTM2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_PSLU) {
      rx_p_rdi.bf.PSLU2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_PSLM) {
      rx_p_rdi.bf.PSLM2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_UNEQP) {
      rx_p_rdi.bf.UNEQP2RDI = bit_ctl ;
    }
    CS_REG_WRITE(regaddr, rx_p_rdi.wrd) ;
  }

  if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
    regaddr = (cs_reg *)
               &pDev->slice[sl_num].Framer.PathAlarmDefectCtrl.wrd ;

    tx_p_rdi.wrd = CS_REG_READ(regaddr) ;

    if (defect & ARN5_FR_PATH_DEF_PTU) {
      tx_p_rdi.bf.PTU2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_PTM) {
      tx_p_rdi.bf.PTM2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_PSLU) {
      tx_p_rdi.bf.PSLU2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_PSLM) {
      tx_p_rdi.bf.PSLM2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_UNEQP) {
      tx_p_rdi.bf.UNEQP2RDI = bit_ctl ;
    }

    if (defect & ARN5_FR_PATH_DEF_AIS) {
      tx_p_rdi.bf.PAIS2RDI = bit_ctl ;
    }
    CS_REG_WRITE(regaddr, tx_p_rdi.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE PAYLOAD PROCESSING ON DEFECT   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_drop_pld_on_path_defect(cs_uint16 port_id,
                        arn5_framer_path_defect_t defect,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Path Defect Type                              */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
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
/* ARN5_FR_PATH_DEF_PTU or ARN5_FR_PATH_DEF_PTM or              */
/* ARN5_FR_PATH_DEF_PSLU or ARN5_FR_PATH_DEF_PSLM or            */
/* ARN5_FR_PATH_DEF_UNEQP  or ARN5_FR_PATH_DEF_ALL              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint8              bit_ctl = ((ctl == CS_ENABLE) ? 1 : 0) ;
  ARN5_FR_SONET32RX_PRDITxInsert_t  prdi_drop_ctl;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PRDITxInsert.wrd ;
  prdi_drop_ctl.wrd = CS_REG_READ(regaddr) ;

  if (defect & ARN5_FR_PATH_DEF_PTU) {
    prdi_drop_ctl.bf.PTU2Drop = bit_ctl ;
  }

  if (defect & ARN5_FR_PATH_DEF_PTM) {
    prdi_drop_ctl.bf.PTM2Drop = bit_ctl ;
  }

  if (defect & ARN5_FR_PATH_DEF_PSLU) {
    prdi_drop_ctl.bf.PSLU2Drop = bit_ctl ;
  }

  if (defect & ARN5_FR_PATH_DEF_PSLM) {
    prdi_drop_ctl.bf.PSLM2Drop = bit_ctl ;
  }

  if (defect & ARN5_FR_PATH_DEF_UNEQP) {
    prdi_drop_ctl.bf.UNEQP2Drop = bit_ctl ;
  }

  CS_REG_WRITE(regaddr, prdi_drop_ctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start GET THE CURRENT STATUS OF PATH LEVEL DEFECTS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_uint16 arn5_framer_alm_get_path_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Bitmap of the currently active defects          */
/* DESCRIPTION:                                                 */
/* Get the Path status. This API returns the bitmap of the      */
/* alarms/defects that are currently active. The bitmap should  */
/* be intrepreted as bitmask of the arn5_framer_path_defect_t.  */
/* Zero is returned if No defects are active.                   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num, status = 0 ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_SONET32RX_APSPathTxInsert_t path_def ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.APSPathTxInsert.wrd ;
  path_def.wrd = CS_REG_READ(regaddr) ;

  if (path_def.bf.dPTU)
  {
    status |= ARN5_FR_PATH_DEF_PTU ;
  }
  if (path_def.bf.dPTM)
  {
    status |= ARN5_FR_PATH_DEF_PTM ;
  }
  if (path_def.bf.dPSLU)
  {
    status |= ARN5_FR_PATH_DEF_PSLU ;
  }
  if (path_def.bf.dPSLM)
  {
    status |= ARN5_FR_PATH_DEF_PSLM ;
  }
  if (path_def.bf.dUNEQP)
  {
    status |= ARN5_FR_PATH_DEF_UNEQP ;
  }
  if (path_def.bf.dLOP)
  {
    status |= ARN5_FR_PATH_DEF_LOP ;
  }
  if (path_def.bf.dPAIS)
  {
    status |= ARN5_FR_PATH_DEF_AIS ;
  }
  if (path_def.bf.dPRDI)
  {
    status |= ARN5_FR_PATH_DEF_RDI ;
  }

  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE LINE RDI INSERTION ON DEFECT  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_insert_rdi_l_on_defect(cs_uint16 port_id,
                        arn5_framer_sect_line_defect_t defect,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Defect Type                                   */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable RDI-L insertion in SONET TX Stream, when      */
/* defect specified in the [defect] parameter is active.        */
/*                                                              */
/* The [defect] parameter is specified as following:            */
/* ARN5_FR_SECT_LINE_DEF_STM or ARN5_FR_SECT_LINE_DEF_STU or    */
/* ARN5_FR_SECT_LINE_DEF_BERSF or ARN5_FR_SECT_LINE_DEF_SD or   */
/* ARN5_FR_SECT_LINE_DEF_ALL                                    */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint8              bit_ctl = ((ctl == CS_ENABLE) ? 1 : 0) ;
  ARN5_FR_SONET32RX_SecLineAlarmtoTxControl_t rdi_tx_ctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  rdi_tx_ctl.wrd = CS_REG_READ(regaddr) ;

  if (defect & ARN5_FR_SECT_LINE_DEF_STM) {
    rdi_tx_ctl.bf.STM2RDIL = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_STU) {
    rdi_tx_ctl.bf.STU2RDIL = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_BERSF) {
    rdi_tx_ctl.bf.BERSF2RDIL = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_SD) {
    rdi_tx_ctl.bf.SD2RDIL = bit_ctl ;
  }

  CS_REG_WRITE(regaddr, rdi_tx_ctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE LINE AIS INSERTION ON DEFECT  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_insert_ais_l_on_defect(cs_uint16 port_id,
                        arn5_framer_sect_line_defect_t defect,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Defect Type                                   */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable AIS-L insertion in SONET TX Stream when       */
/* defect condition specified in the [defect] parameter is      */
/* active.                                                      */
/*                                                              */
/* The [defect] parameter is specified as following:            */
/* ARN5_FR_SECT_LINE_DEF_STM or ARN5_FR_SECT_LINE_DEF_STU or    */
/* ARN5_FR_SECT_LINE_DEF_BERSF or ARN5_FR_SECT_LINE_DEF_SD or   */
/* ARN5_FR_SECT_LINE_DEF_ALL                                    */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint8              bit_ctl = ((ctl == CS_ENABLE) ? 1 : 0) ;
  ARN5_FR_SONET32RX_SecLineAlarmtoTxControl_t rdi_tx_ctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  rdi_tx_ctl.wrd = CS_REG_READ(regaddr) ;

  if (defect & ARN5_FR_SECT_LINE_DEF_STM) {
    rdi_tx_ctl.bf.STM2AISL = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_STU) {
    rdi_tx_ctl.bf.STU2AISL = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_BERSF) {
    rdi_tx_ctl.bf.BERSF2AISL = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_SD) {
    rdi_tx_ctl.bf.SD2AISL = bit_ctl ;
  }

  CS_REG_WRITE(regaddr, rdi_tx_ctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PAYLOAD PROCESSING ON DEFECT  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_drop_pld_on_line_defect(cs_uint16 port_id,
                        arn5_framer_sect_line_defect_t defect,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Defect Type                                   */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
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
/* ARN5_FR_SECT_LINE_DEF_STM or ARN5_FR_SECT_LINE_DEF_STU or    */
/* ARN5_FR_SECT_LINE_DEF_BERSF or ARN5_FR_SECT_LINE_DEF_SD or   */
/* ARN5_FR_SECT_LINE_DEF_ALL                                    */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint8              bit_ctl = ((ctl == CS_ENABLE) ? 1 : 0) ;
  ARN5_FR_SONET32RX_SecLineAlarmtoTxControl_t rdi_tx_ctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr = (cs_reg *)
             &pDev->slice[sl_num].Framer.SecLineAlarmtoTxControl.wrd ;
  rdi_tx_ctl.wrd = CS_REG_READ(regaddr) ;

  if (defect & ARN5_FR_SECT_LINE_DEF_STM) {
    rdi_tx_ctl.bf.STM2Drop = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_STU) {
    rdi_tx_ctl.bf.STU2Drop = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_BERSF) {
    rdi_tx_ctl.bf.BERSF2Drop = bit_ctl ;
  }

  if (defect & ARN5_FR_SECT_LINE_DEF_SD) {
    rdi_tx_ctl.bf.SD2Drop = bit_ctl ;
  }

  CS_REG_WRITE(regaddr, rdi_tx_ctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start GET SECTION/LINE DEFECT STATUS                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_uint16 arn5_framer_alm_get_sect_line_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Bitmap of the currently active defects          */
/* DESCRIPTION:                                                 */
/* Get the status of any section or line defects that are       */
/* active currently. The return value is the bitmap of the      */
/* arn5_framer_sect_line_defect_t enums.                        */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16     sl_num, status = 0 ;
  ARN5_t        * pDev ;
  ARN5_FR_SONET32RX_APSTxSecLineAlarm_t  sect_line_alm ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  sect_line_alm.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.APSTxSecLineAlarm.wrd) ;

  if (sect_line_alm.bf.dSTU) {
    status |= ARN5_FR_SECT_LINE_DEF_STU ;
  }

  if (sect_line_alm.bf.dSTM) {
    status |= ARN5_FR_SECT_LINE_DEF_STM ;
  }

  if (sect_line_alm.bf.dRDIL) {
    status |= ARN5_FR_SECT_LINE_DEF_RDI ;
  }

  if (sect_line_alm.bf.dSD) {
    status |= ARN5_FR_SECT_LINE_DEF_SD ;
  }

  if (sect_line_alm.bf.dAISL) {
    status |= ARN5_FR_SECT_LINE_DEF_AIS ;
  }

  if (sect_line_alm.bf.dSF) {
    status |= ARN5_FR_SECT_LINE_DEF_SF ;
  }

  if (sect_line_alm.bf.dLOF) {
    status |= ARN5_FR_SECT_LINE_DEF_LOF ;
  }

  if (sect_line_alm.bf.dSEF) {
    status |= ARN5_FR_SECT_LINE_DEF_SEF ;
  }

  if (sect_line_alm.bf.dLOS) {
    status |= ARN5_FR_SECT_LINE_DEF_LOS ;
  }

  return (status) ;
}



/****************************************************************/
/* $rtn_hdr_start SELECT ALM OUTPUT TO PIN SNTRXALMPORT         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_alm_sel_rxalmport_src(cs_uint16 port_id,
                        arn5_framer_rxalmport_src_t sel)
/* INPUTS     : o Port Id                                       */
/*              o Rx Alm Source                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selects the alarm to be output on an external sntRxAlmPort   */
/* pin.                                                         */
/*                                                              */
/* [sel] : ARN5_FR_RXALMPORT_LOS                                */
/*         ARN5_FR_RXALMPORT_LOF                                */
/*         ARN5_FR_RXALMPORT_AIS_L                              */
/*         ARN5_FR_RXALMPORT_SF                                 */
/*         ARN5_FR_RXALMPORT_AIS_P                              */
/*         ARN5_FR_RXALMPORT_LOP_P                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16     sl_num ;
  ARN5_t        * pDev ;
  ARN5_FR_SONET32RX_Configuration_t  cfg ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  cfg.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
  cfg.bf.rxAlmPort = sel ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd, cfg.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}

