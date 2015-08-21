/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_soh.c
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
#include "m2r_framer_soh.h"

/****************************************************************/
/*   Internal functions                                         */
/****************************************************************/
static
cs_status m2r_framer_fill_section_trace_buffer(M2R_t * pDev,
                                               cs_uint16 sl_num,
                                               cs_uint8 * traceMsg,
                                               m2r_dir_t dir) ;
static cs_status
m2r_framer_fill_path_trace_buffer(M2R_t * pDev, cs_uint16 sl_num,
                                  cs_uint8 * traceMsg,
                                  m2r_framer_stream_t channel,
                                  m2r_dir_t dir) ;
static
cs_status m2r_framer_get_ber_params(m2r_framer_signal_ber_t ber,
                                    cs_uint16 *delay,
                                    cs_uint16 *scale,
                                    cs_uint16 *leak,
                                    m2r_framer_rateType_t rateType) ;
static
cs_uint16 m2r_framer_calc_ber_thold(m2r_framer_signal_ber_t ref_ber,
                                    cs_uint16 ref_thold,
                                    m2r_framer_signal_ber_t tgt_ber) ;


/****************************************************************/
/*   Intenal table to keep the TX overhead sources options      */
/****************************************************************/
typedef struct {
  cs_uint8  mask ;
  cs_uint8  src_val[M2R_FR_MAX_TX_OVRHD_SRC] ;
} _m2r_framer_tx_ovrhd_src ;

#define M2R_FR_TX_OVRHD_SRC_CPU         (0x01)
#define M2R_FR_TX_OVRHD_SRC_TOH         (0x02)
#define M2R_FR_TX_OVRHD_SRC_DCC         (0x04)
#define M2R_FR_TX_OVRHD_SRC_HDLC        (0x08)
#define M2R_FR_TX_OVRHD_SRC_APS         (0x10)
#define M2R_FR_TX_OVRHD_SRC_RX_ALM      (0x20)

const static _m2r_framer_tx_ovrhd_src
                   xmit_ovrhd_src_tbl[M2R_FR_MAX_OVRHD_BYTE_TYPE] = {
/**********************************************************************
        | Mask  | Register |  TOH  |  DCC  | HDLC |  APS  |  RX Alarm |
**********************************************************************/
  CS_DI([ M2R_FR_OVRHD_C2 ] =)
        { 0x01, {   0,         0,     0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_G1 ] =)
        { 0x01, {   0,         0,     0,      0,      0,       0,    }},

  CS_DI([ M2R_FR_OVRHD_K1 ] =)
        { 0x13, {   0,       0x02,    0,      0,    0x03,      0     }},

  CS_DI([ M2R_FR_OVRHD_K2 ] =)
        { 0x13, {   0,       0x02,    0,      0,    0x03,      0     }},

  CS_DI([ M2R_FR_OVRHD_K3 ] =)
        { 0x11, {   0,         0,     0,      0,    0x01,      0     }},

  CS_DI([ M2R_FR_OVRHD_K4 ] =)
        { 0x11, {   0,         0,     0,      0,    0x01,      0     }},

  CS_DI([ M2R_FR_OVRHD_S1 ] =)
        { 0x13, {   0,       0x02,    0,      0,    0x03,      0     }},

  CS_DI([ M2R_FR_OVRHD_Z2 ] =)
        { 0x03, {   0,       0x01,    0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_Z3 ] =)
        { 0x01, {   0,         0,     0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_Z4 ] =)
        { 0x01, {   0,         0,     0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_Z5 ] =)
        { 0x01, {   0,         0,     0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_F1 ] =)
        { 0x03, {   0,       0x01,    0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_E1 ] =)
        { 0x03, {   0,       0x01,    0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_E2 ] =)
        { 0x03, {   0,       0x01,    0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_SDCC ] =)
        { 0x0f, {   0,       0x02,   0x01,  0x03,     0,       0     }},

  CS_DI([ M2R_FR_OVRHD_LDCC ] =)
        { 0x0f, {   0,       0x02,   0x01,  0x03,     0,       0     }},

  CS_DI([ M2R_FR_OVRHD_J0 ] =)
        { 0x03, {   0,       0x01,    0,      0,      0,       0     }},

  CS_DI([ M2R_FR_OVRHD_J1 ] =)
        { 0x03, {   0,       0x01,    0,      0,      0,       0     }}
} ;

#define M2R_FR_GET_OH_SRC_SEL_VAL(OH_BYTE, SRC)          \
        xmit_ovrhd_src_tbl[(OH_BYTE)].src_val[(SRC)]

/****************************************************************/
/* $rtn_hdr_start  CONFIGURES THE DCC PORT                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_cfg_dcc_port(cs_uint16 port_id,
                                  m2r_framer_dcc_port_mode_t  sel,
                                  m2r_dir_t direction, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o DCC Port configuration                        */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_ENABLE (1) OR M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable DCC Port to carry DCC bytes from Section/Line */
/* Sonet frame overhead.                                        */
/*                                                              */
/* The [sel]  parameter is specified as one of the following:   */
/* M2R_FR_SECTION_DCC(0) or M2R_FR_LINE_DCC(1) or               */
/* M2R_FR_DCC_SEC_LINE(2)                                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                 sl_num ;
  M2R_t                   * pDev ;
  cs_boolean                rx_dir , tx_dir ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, direction) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  if (rx_dir) {
    FR_RECVOVERHEAD_Ctrl_t    ctrl_rx ;
    ctrl_rx.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd);
    ctrl_rx.bf.DCCmode = (ctl == M2R_DISABLE) ? DCCmode_DIS_DCC : sel ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd,
                 ctrl_rx.wrd);
  }

  if (tx_dir) {
    FR_XMITOVERHEAD_Ctrl_t    ctrl_tx ;
    ctrl_tx.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd);
    ctrl_tx.bf.DCCmode = (ctl == M2R_DISABLE) ? DCCmode_DIS_DCC : sel ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
                 ctrl_tx.wrd);
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  CONFIGURES THE APS PORT                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_ctl_aps_port(cs_uint16 port_id,
                                  m2r_aps_port_sel_t sel,
                                  m2r_dir_t  direction, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o APS Port configuration                        */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the APS Port. Also sets the mode of the APS   */
/* Port when enabled to carry K1/K2/K3/K4 and S1 bytes or       */
/* K1/K2/K3/K4/S1 and Alarms bytes to the mate framer APS port. */
/*                                                              */
/* The [sel]  parameter is specified as one of the following:   */
/* M2R_FR_APS_BYTES or M2R_FR_ALARMS or M2R_FR_APS_ALL          */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, direction) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  if (rx_dir) {
    FR_RECVOVERHEAD_Ctrl_t   ctrl_rx ;
    ctrl_rx.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd);
    if ( ctl == M2R_ENABLE ) {
      ctrl_rx.bf.APSmode = sel ;
    }
    ctrl_rx.bf.APSen = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd,
                 ctrl_rx.wrd);
  }

  if (tx_dir) {
    FR_XMITOVERHEAD_Ctrl_t   ctrl_tx ;
    FR_XMITOVERHEAD_LineAlarmDefectCtrl_t apsMode ;
    if (ctl == M2R_ENABLE) {
      apsMode.wrd =
          M2R_REG_READ(&pDev->slice[sl_num].Framer.LineAlarmDefectCtrl.wrd);
      apsMode.bf.APSMode = sel ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LineAlarmDefectCtrl.wrd,
                   apsMode.wrd);
    }
    ctrl_tx.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd);
    ctrl_tx.bf.APSenable = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
                 ctrl_tx.wrd);
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SELECT APS PORT DATA SOURCE                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_sel_aps_port_src(cs_uint16 port_id,
                                      cs_boolean cpu_reg,
                                      m2r_dir_t  direction)
/* INPUTS     : o Port Id                                       */
/*              o TRUE (CPU Register) or FALSE                  */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select the source of APS port data.                          */
/* When [cpu_reg] is TRUE for RX direction, the microprocessor  */
/* registers source the data on the Receive APS Port, else      */
/* source of APS data is the sonet receiver alarm processor.    */
/* When [cpu_reg] is TRUE for TX direction, the microprocessor  */
/* registers override all APS port data or receive data that    */
/* might be used to insert alarms and APS bytes in the transmit */
/* stream, else when [cpu_reg] is FALSE, transmit APS port's    */
/* alarm and APS protection bytes may be inserted in the SONET  */
/* transmit stream as dictated by individual overhead byte      */
/* controls.                                                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, direction) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  if (rx_dir) {
    FR_RECVOVERHEAD_Ctrl_t   ctrl_rx ;
    ctrl_rx.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd);
    ctrl_rx.bf.APSUpOverRide = (cpu_reg) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd,
                 ctrl_rx.wrd);
  }

  if (tx_dir) {
    FR_XMITOVERHEAD_Ctrl_t   ctrl_tx ;
    ctrl_tx.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd);
    ctrl_tx.bf.APSUpOverRide = (cpu_reg) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
                 ctrl_tx.wrd);
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE RX APS PORT DATA VALUE               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_insert_aps_rx_port(cs_uint16 port_id,
                       m2r_framer_aps_port_sect_line_data_t field,
                       cs_uint8 val, m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o APS Port Data Type                            */
/*              o APS data field value                          */
/*              o Stream ID                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the RX APS Port data values to be sent over the RX APS   */
/* port when APS Port source is configured to be CPU registers. */
/* When APS port data is selected to be sourced from CPU        */
/* registers using m2r_framer_sel_aps_port_src() API, these     */
/* data values shall be carried over the APS port rather than   */
/* ones determined by the RX alarm processor.                   */
/*                                                              */
/* The [aps_data] can be one of the following for section/line: */
/* M2R_FR_APS_PORT_insAISL or M2R_FR_APS_PORT_insRDIL or        */
/* M2R_FR_APS_PORT_dSTU or M2R_FR_APS_PORT_dSTM or              */
/* M2R_FR_APS_PORT_dRDIL or M2R_FR_APS_PORT_dSD or              */
/* M2R_FR_APS_PORT_dAISL or M2R_FR_APS_PORT_dSF or              */
/* M2R_FR_APS_PORT_dLOF or M2R_FR_APS_PORT_dSEF or              */
/* M2R_FR_APS_PORT_dLOS or M2R_FR_APS_PORT_REIL or              */
/* M2R_FR_APS_PORT_B2ErCnt                                      */
/* The [aps_data] can be one of the following for Path :        */
/* M2R_FR_APS_PORT_insRDIP or M2R_FR_APS_PORT_dLOPConcat or     */
/* M2R_FR_APS_PORT_dPSLUI or M2R_FR_APS_PORT_dPSLM or           */
/* M2R_FR_APS_PORT_dPTU or M2R_FR_APS_PORT_dPTM or              */
/* M2R_FR_APS_PORT_dUNEQP or M2R_FR_APS_PORT_dRDIP or           */
/* M2R_FR_APS_PORT_dPAIS or M2R_FR_APS_PORT_dLOP or             */
/* M2R_FR_APS_PORT_REIP or M2R_FR_APS_PORT_B3ErCnt              */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint32                sl_num, aps_data = field ;
  M2R_t                  * pDev ;
  FR_RECVOVERHEAD_alarm1_t alarm1 ;
  FR_RECVOVERHEAD_alarm2_t alarm2 ;
  cs_reg                 * regaddr ;
  FR_RECVOVERHEAD_alarm3Path_t alarm3 ;
  FR_RECVOVERHEAD_alarm4Path_t alarm4 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel >= M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  if ((aps_data >= M2R_FR_APS_PORT_insAISL) &&
      (aps_data <= M2R_FR_APS_PORT_dLOS)) {

    alarm1.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_alarm1.wrd) ;

    switch (aps_data) {
      case M2R_FR_APS_PORT_insAISL :
        alarm1.bf.insAISL = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_insRDIL :
        alarm1.bf.insRDIL = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dSTU :
        alarm1.bf.dSTU  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dSTM :
        alarm1.bf.dSTM  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dRDIL :
        alarm1.bf.dRDIL  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dSD :
        alarm1.bf.dSD  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dAISL :
        alarm1.bf.dAISL  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dSF :
        alarm1.bf.dSF  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dLOF :
        alarm1.bf.dLOF  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dSEF :
        alarm1.bf.dSEF  = (val & 0x01) ;
      break ;
      case M2R_FR_APS_PORT_dLOS :
        alarm1.bf.dLOS  = (val & 0x01) ;
      break ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_alarm1.wrd,
                 alarm1.wrd) ;
  }

  if ((aps_data >= M2R_FR_APS_PORT_REIL) &&
      (aps_data <= M2R_FR_APS_PORT_B2ErCnt)) {

    alarm2.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_alarm2.wrd) ;

    switch (aps_data) {
      case M2R_FR_APS_PORT_REIL :
        alarm2.bf.REIL = val ;
      break ;

      case M2R_FR_APS_PORT_B2ErCnt :
        alarm2.bf.B2ErCnt = val ;
      break ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_alarm2.wrd,
                 alarm2.wrd) ;
  }

  if ((aps_data >= M2R_FR_APS_PORT_insRDIP) &&
      (aps_data <= M2R_FR_APS_PORT_dLOP)) {

    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.recvOverhead_alarm3Path.wrd +
                           (channel * M2R_FR_RECVOVERHEAD_alarm3Path_STRIDE) ;

    alarm3.wrd = M2R_REG_READ(regaddr) ;
    switch (aps_data) {
      case M2R_FR_APS_PORT_insRDIP :
        alarm3.bf.insRDIP = val ;
      break ;
      case M2R_FR_APS_PORT_dLOPConcat :
        alarm3.bf.dLOPConcat = val ;
      break ;
      case M2R_FR_APS_PORT_dPSLUI :
        alarm3.bf.dPSLUI = val ;
      break ;
      case M2R_FR_APS_PORT_dPSLM :
        alarm3.bf.dPSLM = val ;
      break ;
      case M2R_FR_APS_PORT_dPTU :
        alarm3.bf.dPTU = val ;
      break ;
      case M2R_FR_APS_PORT_dPTM :
        alarm3.bf.dPTM = val ;
      break ;
      case M2R_FR_APS_PORT_dUNEQP :
        alarm3.bf.dUNEQP = val ;
      break ;
      case M2R_FR_APS_PORT_dRDIP :
        alarm3.bf.dRDIP = val ;
      break ;
      case M2R_FR_APS_PORT_dPAIS :
        alarm3.bf.dPAIS = val ;
      break ;
      case M2R_FR_APS_PORT_dLOP :
        alarm3.bf.dLOP = val ;
      break ;
    }

    M2R_REG_WRITE(regaddr, alarm3.wrd) ;
  }

  if ((aps_data >= M2R_FR_APS_PORT_REIP) &&
      (aps_data <= M2R_FR_APS_PORT_B3ErCnt)) {

    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.recvOverhead_alarm4Path.wrd +
                           (channel * M2R_FR_RECVOVERHEAD_alarm4Path_STRIDE) ;

    alarm4.wrd = M2R_REG_READ(regaddr) ;
    switch (aps_data) {
      case M2R_FR_APS_PORT_REIP :
        alarm4.bf.REIP = val ;
      break ;
      case M2R_FR_APS_PORT_B3ErCnt :
        alarm4.bf.B3ErCnt = val ;
      break ;
    }
    M2R_REG_WRITE(regaddr, alarm4.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE SONET OVERHEAD BYTE                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_set_overheadByte(cs_uint16 port_id,
                                      m2r_framer_ovrhd_byte_t ovrhd,
                                      cs_uint8 val,
                                      m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o Overhead Byte Type                            */
/*              o Overhead Byte value                           */
/*              o Stream ID                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the SONET Overhead byte to be sent in the transmit       */
/* stream.                                                      */
/* The [ovrhd] parameter is specified as following:             */
/* M2R_FR_OVRHD_C2 or M2R_FR_OVRHD_G1 or M2R_FR_OVRHD_K1 or     */
/* M2R_FR_OVRHD_K2 or M2R_FR_OVRHD_K3 or M2R_FR_OVRHD_K4 or     */
/* M2R_FR_OVRHD_S1 or M2R_FR_OVRHD_Z2 or M2R_FR_OVRHD_Z3 or     */
/* M2R_FR_OVRHD_Z4 or M2R_FR_OVRHD_Z5 or M2R_FR_OVRHD_F1 or     */
/* M2R_FR_OVRHD_E1 or M2R_FR_OVRHD_E2 or M2R_FR_OVRHD_SDCC or   */
/* M2R_FR_OVRHD_LDCC or M2R_FR_OVRHD_J0 or M2R_FR_OVRHD_J1      */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/* For section overhead bytes , parameter channel is dont care. */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel >= M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);
  switch (ovrhd) {
    case M2R_FR_OVRHD_C2 : {
        FR_XMITOVERHEAD_PathC2Z5_t   C2_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathC2Z5.wrd +
                           (channel * M2R_FR_XMITOVERHEAD_PathC2Z5_STRIDE) ;
        C2_byte.wrd = M2R_REG_READ( regaddr ) ;
        C2_byte.bf.C2 = val ;
        M2R_REG_WRITE( regaddr, C2_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_G1 : {
        FR_XMITOVERHEAD_alarm3Path_t G1_byte ;
        regaddr = (cs_reg *) (&pDev->slice[sl_num].Framer.
                                xmitOverhead_alarm3Path.wrd) +
                           (channel * M2R_FR_XMITOVERHEAD_alarm3Path_STRIDE) ;
        G1_byte.wrd = M2R_REG_READ( regaddr ) ;
        G1_byte.bf.insRDIP = val ;
        M2R_REG_WRITE( regaddr , G1_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_K1 : {
        FR_XMITOVERHEAD_APS1_t K1_byte ;
        K1_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.
                                   xmitOverhead_APS1.wrd );
        K1_byte.bf.K1 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_APS1.wrd,
                     K1_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_K2 : {
        FR_XMITOVERHEAD_APS1_t K2_byte ;
        K2_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.
                                   xmitOverhead_APS1.wrd );
        K2_byte.bf.K2 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_APS1.wrd,
                     K2_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_K3 : {
        FR_XMITOVERHEAD_APS2_t K3_byte ;
        K3_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.
                                   xmitOverhead_APS2.wrd );
        K3_byte.bf.K3 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_APS2.wrd,
                     K3_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_K4 : {
        FR_XMITOVERHEAD_APS2_t K4_byte ;
        K4_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.
                                   xmitOverhead_APS2.wrd );
        K4_byte.bf.K4 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_APS2.wrd,
                     K4_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_S1 : {
        FR_XMITOVERHEAD_SyncStatus_t  S1_byte ;
        S1_byte.wrd =
            M2R_REG_READ( &pDev->slice[sl_num].Framer.SyncStatus.wrd );
        S1_byte.bf.S1 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.SyncStatus.wrd,
                     S1_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_Z2 : {
        FR_XMITOVERHEAD_Growth_t     Z2_byte ;
        Z2_byte.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.Growth.wrd) ;
        Z2_byte.bf.Z2 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.Growth.wrd, Z2_byte.wrd) ;
      }
      break ;

    case M2R_FR_OVRHD_Z3 : {
        FR_XMITOVERHEAD_PathGrowth_t Z3_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathGrowth.wrd +
                        (channel * M2R_FR_XMITOVERHEAD_PathGrowth_STRIDE) ;
        Z3_byte.wrd = M2R_REG_READ( regaddr ) ;
        Z3_byte.bf.Z3 = val ;
        M2R_REG_WRITE(regaddr, Z3_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_Z4 : {
        FR_XMITOVERHEAD_PathGrowth_t Z4_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathGrowth.wrd +
                        (channel * M2R_FR_XMITOVERHEAD_PathGrowth_STRIDE) ;
        Z4_byte.wrd = M2R_REG_READ( regaddr ) ;
        Z4_byte.bf.Z4 = val ;
        M2R_REG_WRITE(regaddr, Z4_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_Z5 : {
        FR_XMITOVERHEAD_PathC2Z5_t   Z5_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathC2Z5.wrd +
                        (channel * M2R_FR_XMITOVERHEAD_PathC2Z5_STRIDE) ;
        Z5_byte.wrd = M2R_REG_READ( regaddr ) ;
        Z5_byte.bf.Z5 = val ;
        M2R_REG_WRITE(regaddr, Z5_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_F1 : {
        FR_XMITOVERHEAD_UserChannel_t F1_byte ;
        F1_byte.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.UserChannel.wrd) ;
        F1_byte.bf.F1 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.UserChannel.wrd,
                     F1_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_E1 : {
        FR_XMITOVERHEAD_OrderWire_t   E1_byte ;
        E1_byte.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.OrderWire.wrd) ;
        E1_byte.bf.E1 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.OrderWire.wrd,
                     E1_byte.wrd ) ;
      }
      break ;

    case M2R_FR_OVRHD_E2 : {
        FR_XMITOVERHEAD_OrderWire_t   E2_byte ;
        E2_byte.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.OrderWire.wrd) ;
        E2_byte.bf.E2 = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.OrderWire.wrd,
                     E2_byte.wrd ) ;
      }
      break ;
    case M2R_FR_OVRHD_SDCC : {
        FR_XMITOVERHEAD_DCC_t         DCC_bytes ;
        DCC_bytes.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.DCC.wrd) ;
        DCC_bytes.bf.Sect = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.DCC.wrd,
                     DCC_bytes.wrd ) ;
      }
      break ;
    case M2R_FR_OVRHD_LDCC : {
        FR_XMITOVERHEAD_DCC_t         DCC_bytes ;
        DCC_bytes.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.DCC.wrd) ;
        DCC_bytes.bf.Line = val ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.DCC.wrd, DCC_bytes.wrd ) ;
      }
      break ;
    case M2R_FR_OVRHD_J0 :
    case M2R_FR_OVRHD_J1 :
    default :
      CS_HNDL_ERROR(port_id, EM2R_INVALID_TOH_BYTE, NULL);
      break ;

  }
  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return ( CS_OK );
}

/****************************************************************/
/* $rtn_hdr_start  READ THE OVERHEAD BYTE RECEIVED              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_uint8  m2r_framer_get_overheadByte(cs_uint16 port_id,
                                      m2r_framer_ovrhd_byte_t ovrhd,
                                      m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o Overhead Byte Type                            */
/*              o Stream ID                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reads the received overhead byte from the SONET RX stream.   */
/* The overhead byte is read from the Framer register. Some     */
/* of these overhead bytes debounced in hardware while other    */
/* are updated with every SONET Frame received.                 */
/* The [ovrhd] parameter is specified as following:             */
/* M2R_FR_OVRHD_C2 or M2R_FR_OVRHD_G1 or M2R_FR_OVRHD_K1 or     */
/* M2R_FR_OVRHD_K2 or M2R_FR_OVRHD_K3 or M2R_FR_OVRHD_K4 or     */
/* M2R_FR_OVRHD_S1 or M2R_FR_OVRHD_Z2 or M2R_FR_OVRHD_Z3 or     */
/* M2R_FR_OVRHD_Z4 or M2R_FR_OVRHD_Z5 or M2R_FR_OVRHD_F1 or     */
/* M2R_FR_OVRHD_E1 or M2R_FR_OVRHD_E2 or M2R_FR_OVRHD_SDCC or   */
/* M2R_FR_OVRHD_LDCC or M2R_FR_OVRHD_J0 or M2R_FR_OVRHD_J1      */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel >= M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  switch (ovrhd) {
    case M2R_FR_OVRHD_C2 : {
        FR_SONET32RX_FilteredG1C2_t  C2_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.FilteredG1C2.wrd +
                           (channel * M2R_FR_SONET32RX_FilteredG1C2_STRIDE) ;
        C2_byte.wrd = M2R_REG_READ( regaddr ) ;
        return ( C2_byte.bf.FilteredC2 ) ;
      }

    case M2R_FR_OVRHD_G1 : {
        FR_SONET32RX_FilteredG1C2_t  G1_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.FilteredG1C2.wrd +
                           (channel * M2R_FR_SONET32RX_FilteredG1C2_STRIDE) ;
        G1_byte.wrd = M2R_REG_READ( regaddr ) ;
        return ( G1_byte.bf.FilteredG1 ) ;
      }

    case M2R_FR_OVRHD_K1 : {
        FR_SONET32RX_K1K2_t  K1_byte ;
        K1_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.K1K2.wrd ) ;
        return ( K1_byte.bf.K1) ;
      }

    case M2R_FR_OVRHD_K2 : {
        FR_SONET32RX_K1K2_t  K2_byte ;
        K2_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.K1K2.wrd ) ;
        return ( K2_byte.bf.K2) ;
      }

    case M2R_FR_OVRHD_K3 : {
        FR_SONET32RX_K3K4_t  K3_byte ;
        K3_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.K3K4.wrd ) ;
        return ( K3_byte.bf.K3) ;
      }

    case M2R_FR_OVRHD_K4 : {
        FR_SONET32RX_K3K4_t  K4_byte ;
        K4_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.K3K4.wrd ) ;
        return ( K4_byte.bf.K4) ;
      }

    case M2R_FR_OVRHD_S1 : {
        FR_SONET32RX_Z1S1_t  Z1S1_byte ;
        Z1S1_byte.wrd = M2R_REG_READ( &pDev->slice[sl_num].Framer.Z1S1.wrd ) ;
        return ( Z1S1_byte.bf.Z1S1) ;
      }

    case M2R_FR_OVRHD_Z3 : {
        FR_RECVOVERHEAD_pathOverheadB_t Z3_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.pathOverheadB.wrd +
                      (channel * M2R_FR_RECVOVERHEAD_pathOverheadB_STRIDE) ;
        Z3_byte.wrd = M2R_REG_READ( regaddr ) ;
        return (Z3_byte.bf.Z3 ) ;
      }

    case M2R_FR_OVRHD_Z4 : {
        FR_RECVOVERHEAD_pathOverheadB_t Z4_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.pathOverheadB.wrd +
                      (channel * M2R_FR_RECVOVERHEAD_pathOverheadB_STRIDE) ;
        Z4_byte.wrd = M2R_REG_READ( regaddr ) ;
        return (Z4_byte.bf.Z4 ) ;
      }

    case M2R_FR_OVRHD_Z5 : {
        FR_RECVOVERHEAD_pathOverheadC_t Z5_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.pathOverheadB.wrd +
                      (channel * M2R_FR_RECVOVERHEAD_pathOverheadC_STRIDE) ;
        Z5_byte.wrd = M2R_REG_READ( regaddr ) ;
        return (Z5_byte.bf.Z5 ) ;
      }
    case M2R_FR_OVRHD_F1 : {
        FR_RECVOVERHEAD_SectRow2_t F1_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.SectRow2.wrd ;
        F1_byte.wrd = M2R_REG_READ( regaddr ) ;
        return (F1_byte.bf.F1 ) ;
      }
    case M2R_FR_OVRHD_E1 : {
        FR_RECVOVERHEAD_SectRow2_t E1_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.SectRow2.wrd ;
        E1_byte.wrd = M2R_REG_READ( regaddr ) ;
        return (E1_byte.bf.E1 ) ;
      }
    case M2R_FR_OVRHD_E2 : {
        FR_RECVOVERHEAD_LineRow9_t E2_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.LineRow9.wrd ;
        E2_byte.wrd = M2R_REG_READ( regaddr ) ;
        return (E2_byte.bf.E2 ) ;
      }
    case M2R_FR_OVRHD_Z2 :
    case M2R_FR_OVRHD_SDCC :
    case M2R_FR_OVRHD_LDCC :
    case M2R_FR_OVRHD_J0 :
    case M2R_FR_OVRHD_J1 :
      CS_HNDL_ERROR(port_id, EM2R_INVALID_TOH_BYTE, NULL);
      return (0) ;
  }
  return (0);
}

/****************************************************************/
/* $rtn_hdr_start  SET THE EXPECTED PATH LABEL ( C2 byte )      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_set_rx_path_label(cs_uint16 port_id,
                      cs_uint8 c2_byte, m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o Expected Path label ( C2 byte )               */
/*              o Stream ID                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the expected C2 byte in the received path overhead.      */
/* This configured C2 byte is compared with the accepted path   */
/* signal level extracted from the receive stream. A path signal*/
/* label mismatch (PSLMis) is declared if the accepted path     */
/* signal label differs from the expected path label signal.    */
/* C2 byte can be user defined or can be chosen from the        */
/* following values :                                           */
/* M2R_SONET_C2_ATM or  M2R_SONET_C2_HDLC  or                   */
/* M2R_SONET_C2_UNEQ or M2R_SONET_C2_NON_SPECIFIC               */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  M2R_t                  * pDev ;
  volatile cs_reg        * regaddr ;
  cs_uint16                sl_num ;
  cs_uint32                str_start, str_end, str_iter;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    FR_SONET32RX_PathLabel_t  C2_byte ;
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathLabel.wrd +
                       (channel * M2R_FR_SONET32RX_PathLabel_STRIDE) ;

    M2R_REG_READ( regaddr ) ;
    C2_byte.bf.C2 = c2_byte ;
    M2R_REG_WRITE( regaddr , C2_byte.wrd ) ;
  }
  return ( CS_OK ) ;
}

/****************************************************************/
/* $rtn_hdr_start  SELECT THE SOURCE OF TX OVERHEAD BYTE        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_set_tx_ovrhd_src(cs_uint16 port_id,
                                    m2r_framer_ovrhd_byte_t ovrhd,
                                    m2r_framer_tx_ovrhd_src_t src,
                                    m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o Overhead byte Type                            */
/*              o Overhead byte source                          */
/*              o Stream ID                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select the source of SONET overhead byte to be sent in TX    */
/* Frame. SONET Overhead bytes can come from various sources    */
/* like CPU register, TOH ( Transport Overhead Port), APS Port, */
/* DCC Port etc. Not all sources are valid for every overhead   */
/* Byte. This API selects one of the possible sources for the   */
/* overhead byte.                                               */
/*                                                              */
/* The [ovrhd] parameter is specified as following:             */
/* M2R_FR_OVRHD_C2 or M2R_FR_OVRHD_G1 or M2R_FR_OVRHD_K1 or     */
/* M2R_FR_OVRHD_K2 or M2R_FR_OVRHD_K3 or M2R_FR_OVRHD_K4 or     */
/* M2R_FR_OVRHD_S1 or M2R_FR_OVRHD_Z2 or M2R_FR_OVRHD_Z3 or     */
/* M2R_FR_OVRHD_Z4 or M2R_FR_OVRHD_Z5 or M2R_FR_OVRHD_F1 or     */
/* M2R_FR_OVRHD_E1 or M2R_FR_OVRHD_E2 or M2R_FR_OVRHD_SDCC or   */
/* M2R_FR_OVRHD_LDCC or M2R_FR_OVRHD_J0 or M2R_FR_OVRHD_J1      */
/*                                                              */
/* The [src] parameter is specified as following:               */
/* M2R_FR_OVRHD_SRC_CPU or M2R_FR_OVRHD_SRC_TOH_PORT or         */
/* M2R_FR_OVRHD_SRC_DCC_PORT or M2R_FR_OVRHD_SRC_HDLC_PORT or   */
/* M2R_FR_OVRHD_SRC_APS_PORT or M2R_FR_OVRHD_SRC_RX_ALARM       */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  M2R_t                  * pDev ;
  cs_uint16                sl_num ;
  cs_uint8                 mask = 0 ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (channel >= M2R_SONET_STREAM_ALL ) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL);
    return ( CS_ERROR ) ;
  }

  if (ovrhd >= M2R_FR_MAX_OVRHD_BYTE_TYPE) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_TOH_BYTE, NULL);
    return ( CS_ERROR ) ;
  }

  /*
   * Do Sanity check to see if the overhead byte can be controlled
   * through the user specified port option.
   */
  switch (src) {
    case M2R_FR_OVRHD_SRC_CPU  :
      mask = M2R_FR_TX_OVRHD_SRC_CPU ;
      break ;
    case M2R_FR_OVRHD_SRC_TOH_PORT :
      mask = M2R_FR_TX_OVRHD_SRC_TOH ;
      break ;
    case M2R_FR_OVRHD_SRC_DCC_PORT :
      mask = M2R_FR_TX_OVRHD_SRC_DCC ;
      break ;
    case M2R_FR_OVRHD_SRC_HDLC_PORT :
      mask = M2R_FR_TX_OVRHD_SRC_HDLC ;
      break ;
    case M2R_FR_OVRHD_SRC_APS_PORT :
      mask = M2R_FR_TX_OVRHD_SRC_APS ;
      break ;
    case M2R_FR_OVRHD_SRC_RX_ALARM :
      mask = M2R_FR_TX_OVRHD_SRC_RX_ALM ;
      break ;
    default :
      CS_HNDL_ERROR(port_id, EM2R_INVALID_OVRHD_SRC, NULL);
      return ( CS_ERROR ) ;
  }

  if (( xmit_ovrhd_src_tbl[ovrhd].mask & mask ) == 0) {
    CS_HNDL_ERROR(port_id, EM2R_OVRHD_SRC_NOT_VALID,
                  "Overhead byte can't be sourced from the specified port\n");
    return (CS_ERROR);
  }

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  switch ( ovrhd ) {
    case M2R_FR_OVRHD_K1 :
    case M2R_FR_OVRHD_K2 :
      {
        FR_XMITOVERHEAD_ByteCtrl1_t K1K2_src ;
        K1K2_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        K1K2_src.bf.K1K2Src = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     K1K2_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_K3 :
    case M2R_FR_OVRHD_K4 :
      {
        FR_XMITOVERHEAD_ByteCtrl1_t K3K4_src ;
        K3K4_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        K3K4_src.bf.K3K4Src = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     K3K4_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_S1 :
      {
        FR_XMITOVERHEAD_ByteCtrl1_t S1_src ;
        S1_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        S1_src.bf.S1Src = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     S1_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_Z2 :
      {
        FR_XMITOVERHEAD_ByteCtrl2_t Z2_src ;
        Z2_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
        Z2_src.bf.Z2Src = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                     Z2_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_Z3 :
    case M2R_FR_OVRHD_Z4 :
    case M2R_FR_OVRHD_Z5 : {
      }
      break ;
    case M2R_FR_OVRHD_F1 :
      {
        FR_XMITOVERHEAD_ByteCtrl1_t F1_src ;
        F1_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        F1_src.bf.F1Src = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     F1_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_E1 :
      {
        FR_XMITOVERHEAD_ByteCtrl1_t E1_src ;
        E1_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        E1_src.bf.E1Src = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     E1_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_E2 :
      {
        FR_XMITOVERHEAD_ByteCtrl2_t E2_src ;
        E2_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
        E2_src.bf.E2Src = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                     E2_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_SDCC :
      {
        FR_XMITOVERHEAD_ByteCtrl1_t SecDcc_src ;
        SecDcc_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        SecDcc_src.bf.SecDCCSrc = M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     SecDcc_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_LDCC :
      {
        FR_XMITOVERHEAD_ByteCtrl1_t lineDcc_src ;
        lineDcc_src.wrd =
            M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        lineDcc_src.bf.lineDCCSrc =
            M2R_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     lineDcc_src.wrd) ;
      }
      break ;
    case M2R_FR_OVRHD_G1 :
    default :
      break ;
  }
  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return ( CS_OK ) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE EXPECTED TRACE MESSAGE               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_fill_trace_buf(cs_uint16 port_id,
                                    cs_boolean sectionTrace,
                                    cs_uint8 * traceMsg,
                                    m2r_framer_stream_t channel,
                                    m2r_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SECTION TRACE ) or FALSE ( PATH TRACE) */
/*              o Pointer to the trace message                  */
/*              o Stream ID ( Only needed for Path trace )      */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the section (J0)/path (J1) expected trace buffer. */
/* If short trace message (i.e. 16 bytes long) is used,         */
/* Application is responsible for passing the 64 byte buffer in */
/* which the 16 byte message is repeated 4 times.               */
/* This message is compared against the section/Path trace      */
/* message coming in RX Stream, if there is a mis-match STM     */
/* Section Trace Mismatch or PTM ( path trace mismatch) defect  */
/* is declared.                                                 */
/* NOTE : parameter [channel] is dont care in case of section   */
/* trace.                                                       */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/*                                                              */
/* Note : The utility API m2r_framer_encode_trace_msg_util()    */
/* can help to encode the proper trace message if needed.       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (traceMsg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_FR_NULL_TRACE_MSG, NULL);
    return ( CS_ERROR ) ;
  }

  if (sectionTrace) {
    m2r_framer_fill_section_trace_buffer(pDev, sl_num, traceMsg, dir) ;
  }
  else {
    if (channel >  M2R_SONET_STREAM_ALL ) {
      CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
      return (CS_ERROR);
    }
    M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel);

    for (str_iter = str_start; str_iter < str_end; str_iter++) {
      m2r_framer_fill_path_trace_buffer(pDev, sl_num,
                                  traceMsg, str_iter, dir) ;
    }
  }
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  GET THE RECEIVED TRACE MESSAGE               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_get_trace_msg(cs_uint16 port_id,
                                    cs_boolean sectionTrace,
                                    cs_uint8 * traceMsg,
                                    m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SECTION TRACE ) or FALSE ( PATH TRACE) */
/*              o Pointer to the user allocated buffer          */
/*              o Stream ID ( Only needed for Path trace )      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the trace message from trace buffer.  It is the received */
/* trace message in saving mode and expected trace message in   */
/* checking mode.                                               */
/*                                                              */
/* The [sectionTrace] identifies whether section trace message  */
/* or path trace message is to be copied back.                  */
/* Note : User should always provide pointer to buffer which is */
/* at least 64 bytes in length.                                 */
/* parameter [channel] is dont care in case of section trace.   */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/*                                                              */
/* Note : In the saving mode, the received trace message may    */
/* appear to be out of order.  Software needs to do further     */
/* processing(decode the underline protocol) to get human       */
/* readable format.  See the following utility API's for more   */
/* information, m2r_framer_decode_trace_msg_util() and          */
/* m2r_framer_verify_trace_msg_util().                          */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  volatile cs_reg        * regaddr ;
  cs_uint16                offset ;
  FR_SONET32RX_pathTraceSelect_t  pathSelect ;
  FR_SONET32RX_SectionLineControl_t section_prvsn ;
  FR_SONET32RX_PathProvision_t  path_prvsn ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (traceMsg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_FR_NULL_TRACE_MSG, NULL);
    return ( CS_ERROR ) ;
  }

  if (sectionTrace) {
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxSectionTraceExp.wrd ;
  }
  else {
    if (channel >= M2R_SONET_STREAM_ALL ) {
      CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
      return (CS_ERROR);
    }
    pathSelect.bf.pathNumber = channel ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.pathTraceSelect.wrd,
                 pathSelect.wrd) ;
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxPathTraceExp.wrd ;
  }

  for ( offset = 0; offset < 32; offset++) {
    cs_uint16   regdata ;

    regdata = M2R_REG_READ((regaddr+offset)) ;
    traceMsg[offset * 2] = regdata >> 8 ; /* lo byte in 15:8 */
    traceMsg[(offset * 2) + 1] = regdata & 0xff ; /* hi byte in 7:0 */
  }

  if (sectionTrace) {
    section_prvsn.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.SectionLineControl.wrd);
    if (section_prvsn.bf.J0Mode) {
      section_prvsn.bf.J0Mode = 0 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                   section_prvsn.wrd) ;
      section_prvsn.bf.J0Mode = 1 ;
      M2R_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                   section_prvsn.wrd) ;
    }
  }
  else {
    regaddr = &pDev->slice[sl_num].Framer.PathProvision.wrd +
        (channel * M2R_FR_SONET32RX_PathProvision_STRIDE) ;
    path_prvsn.wrd = M2R_REG_READ(regaddr) ;
    if (path_prvsn.bf.J1Mode) {
      path_prvsn.bf.J1Mode = 0 ;
      M2R_REG_WRITE(regaddr, path_prvsn.wrd) ;
      path_prvsn.bf.J1Mode = 1 ;
      M2R_REG_WRITE(regaddr, path_prvsn.wrd) ;
    }
  }
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE SECTION/PATH TRACE MODE              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_set_rx_trace_mode(cs_uint16 port_id,
                                       cs_boolean sectionTrace,
                                       cs_boolean chkMode,
                                       cs_boolean shortTr,
                                       m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SECTION ) or FALSE ( PATH )            */
/*              o TRUE ( Checking Mode ) or FALSE (SAVE MODE)   */
/*              o TRUE(16-byte) or FALSE(64-byte)               */
/*              o Stream ID                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the receive Section/Path Trace mode. In the checking mode*/
/* of operation an incoming section trace message is compared   */
/* against the message stored by the microprocessor in the      */
/* Expected Section Trace Buffer. In Saving-mode, J0 message is */
/* stored in the incoming Section Trace buffer and an interrupt */
/* is generated when a complete message is received.            */
/* Note : parameter [channel] is dont care for section trace.   */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/*                                                              */
/* Changing the length format(16/64) at run-time requires a soft*/
/* reset of SONET Rx overhead logic(RxLogicReset bit in         */
/* FR_GLOBAL_MasterReset).                                      */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  if ( sectionTrace ) {
    FR_SONET32RX_SectionLineControl_t section_prvsn ;

    section_prvsn.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.SectionLineControl.wrd);
    section_prvsn.bf.J0Mode = (chkMode) ? 0 : 1 ;
    section_prvsn.bf.J0Len = (shortTr) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                 section_prvsn.wrd) ;
  }
  else {
    cs_uint32                str_start, str_end, str_iter ;
    FR_SONET32RX_PathProvision_t  path_prvsn ;

    if (channel > M2R_SONET_STREAM_ALL ) {
      CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
      return (CS_ERROR);
    }
    M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel);
    for (str_iter = str_start; str_iter < str_end; str_iter++) {
      regaddr = &pDev->slice[sl_num].Framer.PathProvision.wrd +
          (str_iter * M2R_FR_SONET32RX_PathProvision_STRIDE) ;
      path_prvsn.wrd = M2R_REG_READ(regaddr) ;
      path_prvsn.bf.J1Mode = (chkMode) ? 0 : 1 ;
      path_prvsn.bf.J1Len = (shortTr) ? 1 : 0 ;
      M2R_REG_WRITE(regaddr, path_prvsn.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE TX SECTION/PATH TRACE MODE           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_sel_tx_trace_msg_src(cs_uint16 port_id,
                                          cs_boolean sectionTrace,
                                       m2r_framer_tx_trace_src_t src)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SECTION ) OR FALSE ( PATH )            */
/*              o TRACE SOURCE                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set transmit Section/Path Trace message's source.            */
/* If parameter [sectionTrace] is TRUE, this API selects the    */
/* source of section Trace message to be sent in TX Stream. The */
/* sources for section trace message can either be from trace   */
/* buffer or fixed message ( 0x01 ) pattern or from TOH.        */
/* For path trace, source of the trace message can be from the  */
/* the trace buffer or from TOH. Changing the J1 source ( path )*/
/* affects the source of all the paths if in channelized mode.  */
/*                                                              */
/* The [src] can be one of the following :                      */
/* M2R_FR_TX_TRACE_BUFFER or M2R_FR_TX_TRACE_TOH or             */
/* M2R_FR_TX_TRACE_FIXED                                        */
/*                                                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint8                 bit_ctl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(sectionTrace) {
    FR_XMITOVERHEAD_ByteCtrl1_t J0Src ;
    switch(src) {
      case M2R_FR_TX_TRACE_BUFFER :
        bit_ctl = J0Src_JOSRC_TRBUF ;
        break ;
      case M2R_FR_TX_TRACE_TOH :
        bit_ctl = J0Src_J0SRC_TOH ;
        break ;
      case M2R_FR_TX_TRACE_FIXED :
        bit_ctl = J0Src_J0SRC_DEF ;
        break ;
      default :
        CS_HNDL_ERROR(port_id, EM2R_INVALID_OVRHD_SRC, NULL);
        return (CS_ERROR) ;
    }
    M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

    J0Src.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
    J0Src.bf.J0Src = bit_ctl ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                 J0Src.wrd) ;

    M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  }
  else { /* Path Trace */
    FR_XMITOVERHEAD_ByteCtrl2_t J1Src ;

    switch(src) {
      case M2R_FR_TX_TRACE_BUFFER :
        bit_ctl = 1 ;
        break ;
      case M2R_FR_TX_TRACE_TOH :
        bit_ctl = 0 ;
        break ;
      default :
        CS_HNDL_ERROR(port_id, EM2R_INVALID_OVRHD_SRC, NULL);
        return (CS_ERROR) ;
    }

    M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

    J1Src.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
    J1Src.bf.J1Src = bit_ctl ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                 J1Src.wrd) ;

    M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  CONTROLS THE ZO BYTE IN SECTION OVERHEAD     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_set_Z0_mode(cs_uint16 port_id,
                                 cs_boolean fix_ptrn, cs_uint8 Z0)
/* INPUTS     : o Port Id                                       */
/*              o TRUE for Fixed Z0 pattern else FALSE          */
/*              o Z0 byte in case of fix Z0 pattern             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select the Z0 overhead byte mode. By default the Z0 byte is  */
/* is sent according the GR spec. But instead, a fixed pattern  */
/* can be sent in Z0 byte. If parameter [fix_ptrn] is TRUE, the */
/* value in parameter [Z0] is sent in SONET overhead Z0 byte.   */
/* Default value of Z0 byte in fixed pattern mode is            */
/* M2R_SONET_FIX_ZO_DEF ( 0xCC )                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                    sl_num ;
  M2R_t                       * pDev ;
  FR_SONET32RX_Configuration_t  cfg ;
  FR_SONET32RX_Z0_byte_t        Z0_byte ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if ( fix_ptrn ) {
    Z0_byte.wrd = 0;
    Z0_byte.bf.Z0_Byte_Value = Z0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Z0_byte.wrd,
                 Z0_byte.wrd) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);
  cfg.wrd = M2R_REG_READ(
              &pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
  cfg.bf.Fixed_Z0 = (fix_ptrn) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd,
               cfg.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET SIGNAL DEGRADE(SD), FAIL(SF) THRESHOLDS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_status m2r_framer_set_signal_thresholds(cs_uint16 port_id,
                               m2r_framer_signal_ber_t sd_declare,
                               m2r_framer_signal_ber_t sd_clear,
                               m2r_framer_signal_ber_t sf_declare,
                               m2r_framer_signal_ber_t sf_clear)
/* INPUTS     : o Port Id                                       */
/*              o signal quality(in BER) for SD declaring       */
/*              o signal quality(in BER) for SD clearing        */
/*              o signal quality(in BER) for SF declaring       */
/*              o signal quality(in BER) for SF clearing        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the Line Signal Degrade(SD) and Signal Fail(SF)   */
/* thresholds for BER.                                          */
/*                                                              */
/* The parameters, [sd_declare] and [sf_declare] specify the    */
/* BER thresholds at or above which the Signal Degrade(SD) or   */
/* Signal Fail(SF) is declared.                                 */
/*                                                              */
/* The parameters, [sd_clear] and [sf_clear] specify the BER    */
/* thresholds at or below which the Signal Degrade(SD) or       */
/* Signal Fail(SF) is cleared.                                  */
/*                                                              */
/* NOTE -                                                       */
/* As per the hardware implementation, the values of the        */
/* various thresholds MUST satisfy the following:               */
/* o [sd_declare] should be in the range of 10e-5 to 10e-8(ie.  */
/*   M2R_FR_BER_5 to M2R_FR_BER_8).                             */
/* o The other threshold parameters should be within 2 levels of*/
/*   [sd_declare]. For example, if [sd_declare] is M2R_FR_BER_5,*/
/*   then the other thresholds must be in the range of          */
/*   M2R_FR_BER_3 to M2R_FR_BER_7.                              */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                         * pDev ;
  m2r_framer_signal_ber_t       ref_ber ;
  cs_uint16                     ref_thold ;
  cs_uint16                     sd_declare_thold, sd_clear_thold ;
  cs_uint16                     sf_declare_thold, sf_clear_thold ;
  cs_uint16                     delay, leak, scale ;
  FR_SONET32RX_LBERAcc_t        lberAcc ;
  FR_SONET32RX_LBER_t           lber ;
  cs_boolean                    error = FALSE ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (sd_clear < sd_declare) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_MIN_GT_MAX,
                  ": sd_clear(%d) s/b >= sd_declare(%d)\n",
                  sd_clear, sd_declare) ;
    return (CS_ERROR) ;
  }

  if (sf_clear < sf_declare) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_MIN_GT_MAX,
                  ": sf_clear(%d) s/b >= sf_declare(%d)\n",
                  sf_clear, sf_declare) ;
    return (CS_ERROR) ;
  }

  /* Use SD declare value as reference */
  ref_ber = sd_declare ; /* enumerated value */

  /* Verify that other thresholds are not too far off from ref */
  error = ((sd_clear - ref_ber) > 2) ? TRUE : FALSE ;
  if (!error) {
    if (sf_declare > ref_ber) {
      error = ((sf_declare - ref_ber) > 2) ? TRUE : FALSE ;
   }
    else {
      error = ((ref_ber - sf_declare) > 2) ? TRUE : FALSE ;
    }
  }
  if (!error) {
    if (sf_clear > ref_ber) {
      error = ((sf_clear - ref_ber) > 2) ? TRUE : FALSE ;
    }
    else {
      error = ((ref_ber - sf_clear) > 2) ? TRUE : FALSE ;
    }
  }

  if (error) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
    ": sd_declare(%d), sd_clear(%d), sf_declare(%d), sf_clear(%d)\n",
     sd_declare, sd_clear, sf_declare, sf_clear) ;
    return (CS_ERROR) ;
  }

  /* determine the line rate first and get desired parameter sttings */
  /* according to the line rate                                      */
  if (m2r_framer_get_ber_params(ref_ber, &delay, &scale, &leak,
        m2r_mpif_is_in_oc48_mode(port_id) ? M2R_FR_RATETYPE_OC48 : M2R_FR_RATETYPE_OC192)
        == CS_ERROR ) {
    return (CS_ERROR) ;
  }

 SCALE_AGAIN:
  /* leak value, scale, delay */
  lberAcc.wrd = 0 ;
  lberAcc.bf.Scale = scale ;
  lberAcc.bf.AccInterval = delay ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LBERAcc.wrd, lberAcc.wrd) ;

  lber.wrd = 0;
  lber.bf.leakValue = leak ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LBER.wrd, lber.wrd) ;

  /* Set the thresholds */
  ref_thold = leak;

  sd_declare_thold = ref_thold ; /* use ref for SD declare */
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LBERDegradeDeclaring.wrd,
               sd_declare_thold) ;

  sd_clear_thold = m2r_framer_calc_ber_thold(ref_ber, ref_thold,
                                             sd_clear) ;
  if (sd_clear_thold < 2) {
    /* just in case we lose too much resolution, redo the calc */
    delay <<= 1;
    leak <<= 1;
    goto SCALE_AGAIN;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LBERDegradeClearing.wrd,
               sd_clear_thold) ;

  sf_declare_thold = m2r_framer_calc_ber_thold(ref_ber, ref_thold,
                                               sf_declare) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LBERFailureDeclaring.wrd,
               sf_declare_thold) ;

  sf_clear_thold = m2r_framer_calc_ber_thold(ref_ber, ref_thold,
                                             sf_clear) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LBERFailureClearing.wrd,
               sf_clear_thold) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET PATH SIGNAL FAIL(SF) THRESHOLDS          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_status m2r_framer_set_path_signal_thresholds(cs_uint16 port_id,
                               m2r_framer_signal_ber_t sf_declare,
                               m2r_framer_signal_ber_t sf_clear)
/* INPUTS     : o Port Id                                       */
/*              o signal quality(in BER) for SF declaring       */
/*              o signal quality(in BER) for SF clearing        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the Path Signal Fail(SF) thresholds for BER.      */
/* One setting for ALL paths.                                   */
/* The parameter, [sf_declare] specify the BER thresholds at or */
/* above which the Signal Fail(SF) is declared.                 */
/*                                                              */
/* The parameter, [sf_clear] specify the BER thresholds at or   */
/* below which the Signal Fail(SF) is cleared.                  */
/*                                                              */
/* NOTE -                                                       */
/* As per the hardware implementation, the values of the        */
/* various thresholds MUST satisfy the following:               */
/* o thresholds should be in the range of 10e-5 to 10e-9(ie.    */
/*   M2R_FR_BER_5 to M2R_FR_BER_9).                             */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  M2R_t                         * pDev ;
  m2r_framer_signal_ber_t       ref_ber ;
  cs_uint16                     ref_thold ;
  cs_uint16                     sf_declare_thold, sf_clear_thold ;
  cs_uint16                     delay, leak, scale ;
  FR_SONET32RX_PBERAcc_t        pberAcc ;
  FR_SONET32RX_PBER_t           pber ;
  m2r_framer_rateType_t         rateType ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (sf_clear < sf_declare) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_MIN_GT_MAX,
                  ": sf_clear(%d) s/b >= sf_declare(%d)\n",
                  sf_clear, sf_declare) ;
    return (CS_ERROR) ;
  }

  ref_ber = sf_clear ; /* enumerated value */

  /* determine the path rate first and get desired parameter sttings */
  /* according to the path rate                                      */
  if (m2r_mpif_is_in_oc48_mode(port_id)) {
    rateType = m2r_framer_is_in_channelised_mode(port_id) ?
               M2R_FR_RATETYPE_STS12C : M2R_FR_RATETYPE_STS48C;
  }
  else {
    rateType = m2r_framer_is_in_channelised_mode(port_id) ?
               M2R_FR_RATETYPE_STS48C : M2R_FR_RATETYPE_STS192C;
  }

  if (m2r_framer_get_ber_params(ref_ber, &delay, &scale, &leak, rateType)
        == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* leak value, scale, delay */
  pberAcc.wrd = 0 ;
  pberAcc.bf.Scale = scale ;
  pberAcc.bf.AccInterval = delay ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.PBERAcc.wrd, pberAcc.wrd) ;

  pber.wrd = 0;
  pber.bf.leakValue = leak ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.PBER.wrd, pber.wrd) ;

  /* Set the thresholds */
  ref_thold = leak;

  sf_declare_thold = m2r_framer_calc_ber_thold(ref_ber, ref_thold,
                                               sf_declare) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.PBERFailureDeclaring.wrd,
               sf_declare_thold) ;

  sf_clear_thold = ref_thold;

  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.PBERFailureClearing.wrd,
               sf_clear_thold) ;

  return (CS_OK) ;
}


/* =============================================== */
/* The following is for trace message utils        */
/* They can be used independently to the driver    */
/* =============================================== */
#define CRC7_POLYNOMIAL 0x89            /* x^7 + x^3 + 1 */

static unsigned char crc7_syndrome_table[256] ;
static cs_boolean crc7_syndrome_table_done = FALSE ;

/*
 * Generate a table of CRC-7 syndromes for x^7 * each possible input byte
 */
static
void gen_crc7_syndrome_table (void)
{
  int i, j, syndrome ;

  for (i = 0;  i < 256;  ++i) {
    syndrome = ((i & 0x80) != 0) ? i ^ CRC7_POLYNOMIAL : i ;
    for (j = 0;  j < 7;  ++j) {
      if (((syndrome <<= 1) & 0x80) != 0) {
        syndrome ^= CRC7_POLYNOMIAL ;
      }
    }
    crc7_syndrome_table[i] = (unsigned char) syndrome ;
  }
  crc7_syndrome_table_done = TRUE ;
}

/*
 * Insert the CRC-7 into a 16-byte E.164 trace message
 */
static
void insert_crc7 (unsigned char * trace_message)
{
  unsigned char crc7_accum = 0 ;
  int i ;

  if (!crc7_syndrome_table_done) {
    gen_crc7_syndrome_table() ;
  }

  trace_message[0] &= ~0x7f ;
  for (i = 0;  i < 16;  ++i) {
    crc7_accum =
      crc7_syndrome_table[(crc7_accum << 1) ^ trace_message[i]] ;
  }
  trace_message[0] ^= crc7_accum;
}

/*
 * Check whether a 16-byte E.164 trace message has a valid CRC-7 field
 */
static
cs_boolean crc7_OK (const unsigned char * trace_message)
{
  unsigned char crc7_accum ;
  int i ;

  if (!crc7_syndrome_table_done) {
    gen_crc7_syndrome_table() ;
  }

  crc7_accum = crc7_syndrome_table[trace_message[0] & ~0x7f];
  for (i = 1;  i < 16;  ++i) {
    crc7_accum =
      crc7_syndrome_table[(crc7_accum << 1) ^ trace_message[i]];
  }
  return (crc7_accum == (trace_message[0] & 0x7f));
}


/****************************************************************/
/* $rtn_hdr_start  TRACE MESSAGE ENCODE UTIL                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_status m2r_framer_encode_trace_msg_util(cs_uint8 * raw_trace_msg,
                        cs_uint8 * encoded_trace_msg,
                        cs_uint8 len)
/* INPUTS     : o Raw Trace Message Buffer                      */
/*              o Encoded Trace Message Buffer                  */
/*              o Message Length (16 or 64)                     */
/* OUTPUTS    : o Encoded Trace Message                         */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This utility will encode the the raw trace message to its    */
/* proper format as follows.                                    */
/* [len] == 16 bytes :                                          */
/*     Added one byte (0x80 | CRC7) in front of the actual      */
/* message (15 bytes) taken from raw trace message buffer.      */
/* In theory, the added CRC7 should be that of a previous trace */
/* message.  The assumption here is that trace message is       */
/* repetative and all CRC7's are the same.                      */
/*                                                              */
/* [len] == 64 bytes :                                          */
/*     Added <CR> <LF> at the end of 62 byte trace message taken*/
/* from raw trace message buffer.                               */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16 ii ;

  if ((len != 16) && (len != 64)) {
    CS_PRINT("len (%s) s/b 16 or 64.\n", len) ;
    return (CS_ERROR) ;
  }

  if (raw_trace_msg == NULL) {
    CS_PRINT("Pointer raw_trace_msg is NULL\n") ;
    return (CS_ERROR) ;
  }

  if (encoded_trace_msg == NULL) {
    CS_PRINT("Pointer encoded_trace_msg is NULL\n") ;
    return (CS_ERROR) ;
  }

  if (len == 16) {
    encoded_trace_msg[0] = 0x80 ;
    for (ii = 1; ii < len ; ii++) {
      encoded_trace_msg[ii] = raw_trace_msg[ii-1] ;
    }
    insert_crc7(encoded_trace_msg) ;
  }
  else {
    for (ii = 0; ii < (len - 2) ; ii++) {
      encoded_trace_msg[ii] = raw_trace_msg[ii] ;
    }
    encoded_trace_msg[len - 2] = 0x0d ; /* <CR> */
    encoded_trace_msg[len - 1] = 0x0a ; /* <CR> */
  }
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  TRACE MESSAGE DECODE UTIL                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_status m2r_framer_decode_trace_msg_util(cs_uint8 * raw_trace_msg,
                        cs_uint8 * decoded_trace_msg,
                        cs_uint8 len)
/* INPUTS     : o Raw Trace Message Buffer                      */
/*              o Decoded Trace Message Buffer                  */
/*              o Message Length (16 or 64)                     */
/* OUTPUTS    : o Decoded Trace Message                         */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This utility will decode the the raw trace message and put   */
/* the properly terminated trace message in the given buffer.   */
/*                                                              */
/* [len] == 16 bytes :                                          */
/*     Locate unique byte with MSB == 1; this byte is the       */
/* starting of the actual trace message.                        */
/*                                                              */
/* [len] == 64 bytes :                                          */
/*     Locate <CR><LF> as the end of message.                   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16 ii, first_fit = 0xffff ;
  cs_boolean not_unique = FALSE ;

  if ((len != 16) && (len != 64)) {
    CS_PRINT("len (%s) s/b 16 or 64.\n", len) ;
    return (CS_ERROR) ;
  }

  if (raw_trace_msg == NULL) {
    CS_PRINT("Pointer raw_trace_msg is NULL\n") ;
    return (CS_ERROR) ;
  }

  if (decoded_trace_msg == NULL) {
    CS_PRINT("Pointer decoded_trace_msg is NULL\n") ;
    return (CS_ERROR) ;
  }

  if (len == 16) {
    for (ii = 0; ii < len; ii++) {
      if (raw_trace_msg[ii] & 0x80) {
        if (first_fit == 0xffff) {
          first_fit = ii ;
        }
        else {
          not_unique = TRUE ;
        }
      }
    }

    if (not_unique || (first_fit == 0xffff)) {
      CS_PRINT("Can't uniquely locate the head of message\n") ;
      return (CS_ERROR) ;
    }

    for (ii = 0; ii < len; ii++) {
      decoded_trace_msg[ii] = raw_trace_msg[(first_fit + ii) % len] ;
    }
  }
  else {
    for (ii = 0; ii < len; ii++) {
      if (raw_trace_msg[ii] == 0x0d /* <CR> */) {
        if (first_fit == 0xffff) {
          first_fit = ii ;
        }
        else {
          not_unique = TRUE ;
        }
      }
    }

    if (not_unique || (first_fit == 0xffff) ||
        (raw_trace_msg[(first_fit + 1) % len] != 0x0a /* <LF> */)) {
      CS_PRINT("Can't uniquely locate the end of message\n") ;
      return (CS_ERROR) ;
    }

    for (ii = 0; ii < len; ii++) {
      decoded_trace_msg[ii] = raw_trace_msg[(first_fit + 2 + ii) % len] ;
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  TRACE MESSAGE VERIFICATION UTIL              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_boolean m2r_framer_verify_trace_msg_util(cs_uint8 * trace_msg,
                        cs_uint8 len)
/* INPUTS     : o Trace Message Buffer                          */
/*              o Message Length (16 or 64)                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : FRUE or FALSE                                   */
/* DESCRIPTION:                                                 */
/* This utility will verify the trace message based on the      */
/* following criteria.                                          */
/*                                                              */
/* [len] == 16 bytes :                                          */
/*     MSB of the first trace message == 1 and verify CRC7      */
/*                                                              */
/* [len] == 64 bytes :                                          */
/*     Verify <CR><LF> are at the end of message.               */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_boolean ret = FALSE ;
  if ((len != 16) && (len != 64)) {
    CS_PRINT("len (%s) s/b 16 or 64.\n", len) ;
    return (FALSE) ;
  }

  if (trace_msg == NULL) {
    CS_PRINT("Pointer trace_msg is NULL\n") ;
    return (FALSE) ;
  }

  if (len == 16) {
    if (trace_msg[0] & 0x80) {
      ret = crc7_OK(trace_msg) ;
    }
  }
  else {
    ret = (trace_msg[62] == 0x0d) && (trace_msg[63] == 0x0a) ;
  }

  return (ret) ;
}


/****************************************************************/
/*   Internal functions                                         */
/****************************************************************/
static
cs_status m2r_framer_fill_section_trace_buffer(M2R_t * pDev,
                                               cs_uint16 sl_num,
                                               cs_uint8 * traceMsg,
                                               m2r_dir_t dir)
{
  volatile cs_reg        * regaddr ;
  cs_uint8                 offset, *msg = traceMsg ;
  cs_uint16                wrd ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  if ( rx_dir ) {
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxSectionTraceExp.wrd ;

    for ( offset = 0; offset < 32; offset++) {
      wrd = (*msg << 8) | (*(msg + 1));  /* lo byte in 15:8, hi in 7:0*/
      msg += 2 ;
      M2R_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }

  if ( tx_dir ) {
    msg = traceMsg ;
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.SectionTrace.wrd ;

    for ( offset = 0; offset < 32; offset++) {
      wrd = (*msg << 8) | (*(msg + 1)); /* lo byte in 15:8, hi in 7:0*/
      msg += 2 ;
      M2R_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }
  return (CS_OK) ;
}

static cs_status
m2r_framer_fill_path_trace_buffer(M2R_t * pDev, cs_uint16 sl_num,
                                  cs_uint8 * traceMsg,
                                  m2r_framer_stream_t channel,
                                  m2r_dir_t dir)
{
  volatile cs_reg        * regaddr ;
  cs_uint8                 offset, *msg;
  cs_uint16                wrd ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  if ( rx_dir ) {
    /* Select the path trace buffer */
    FR_SONET32RX_pathTraceSelect_t  pathSelect ;

    msg = traceMsg ;

    pathSelect.wrd = 0 ;
    pathSelect.bf.pathNumber = channel ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.pathTraceSelect.wrd,
                 pathSelect.wrd) ;

    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxPathTraceExp.wrd ;

    for ( offset = 0; offset < 32; offset++) {
        wrd = (*msg << 8) | (*(msg + 1)); /* lo byte in 15:8, hi in 7:0*/
        msg += 2 ;
        M2R_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }

  if ( tx_dir ) {
    FR_XMITOVERHEAD_PathBuffer_t  pathSelect ;

    msg = traceMsg ;

    pathSelect.wrd = 0 ;
    pathSelect.bf.Select = channel ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.PathBuffer.wrd,
                 pathSelect.wrd) ;

    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.PathTrace.wrd ;

    for ( offset = 0; offset < 32; offset++) {
        wrd = (*msg << 8) | (*(msg + 1)); /* lo byte in 15:8, hi in 7:0*/
        msg += 2 ;
        M2R_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }
  return (CS_OK) ;
}

static
cs_status m2r_framer_get_ber_params(m2r_framer_signal_ber_t ber,
                                    cs_uint16 *delay,
                                    cs_uint16 *scale,
                                    cs_uint16 *leak,
                                    m2r_framer_rateType_t rateType)
{
  struct _BER_TABLE {
    cs_uint16 delay;
    cs_uint16 scale;
    cs_uint16 leak;
  } ber_table[3][5] = {{{   1,  3, 98},  /* OC192 */
                        {   8,  3, 78},
                        {  32,  3, 32},
                        { 256,  3, 25},
                        {2048,  3, 20}},
                       {{   2,  3, 49},  /* OC48 */
                        {  16,  3, 39},
                        {  64,  3, 16},
                        { 512,  3, 13},
                        {4096,  3, 10}},
                       {{   8,  3, 49},  /* OC12 */
                        {  64,  3, 39},
                        { 256,  3, 16},
                        {2048,  3, 13},
                        {8192,  3,  5}}} ;

  cs_uint16 index;

  switch ( ber ) {
    case M2R_FR_BER_5:
    case M2R_FR_BER_6:
    case M2R_FR_BER_7:
    case M2R_FR_BER_8:
    case M2R_FR_BER_9:
      index = ber - M2R_FR_BER_5;
      *delay = ber_table[rateType][index].delay;
      *scale = ber_table[rateType][index].scale;
      *leak = ber_table[rateType][index].leak;
      break;

    default :
      CS_PRINT("%s() ERROR: Invalid desired BER rate(%d)\n",
               __FUNCTION__, ber) ;
      return (CS_ERROR) ;
  }
  return (CS_OK) ;
}


/*
 * Calculate BER thresholds from the given reference BER threshold.
 * Since there is only one set of parameters for the leaky bucket
 * algorithm in the hardware, the other thresholds need to be
 * calculated to be multiplied-up by power of 10 or divided-down
 * by power of 10.
 */
static
cs_uint16 m2r_framer_calc_ber_thold(m2r_framer_signal_ber_t ref_ber,
                                    cs_uint16 ref_thold,
                                    m2r_framer_signal_ber_t tgt_ber)
{
  cs_uint16     delta, ii ;
  cs_boolean    div_down = FALSE ;
  cs_uint16     tgt_thold = ref_thold ;

  if (ref_ber > tgt_ber) {
    delta    = (ref_ber - tgt_ber) ;
    div_down = FALSE ;
  }
  else {
    delta    = (tgt_ber - ref_ber) ;
    div_down = TRUE ;
  }

  for (ii = 0; ii < delta; ii++) {
    if (div_down) {
      tgt_thold /= 10 ;
    }
    else {
      tgt_thold *= 10 ;
    }
  }

  return (tgt_thold) ;
}


/****************************************************************/
/* $rtn_hdr_start  FILL TRACE BUFFER WITH DEFINED PATTERN       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_debug_fill_trace_buf(cs_uint16 port_id,
                                          cs_uint8 pattern,
                                          cs_boolean section,
                                          m2r_framer_stream_t channel,
                                          m2r_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o Pattern Type                                  */
/*              o Section Trace ( TRUE ) or PATH TRACE          */
/*              o Channel ID ( valid for path trace only )      */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Fill the trace buffer with pattern specified by pattern.     */
/* [pattern] can be one of the following :                      */
/* 0 - Incrementing pattern                                     */
/* 1 - fixed pattern of 0x55                                    */
/* 2 - walking ones pattern                                     */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16 len = 64, ii ;
  cs_uint8  * traceMsg ;

  traceMsg = (cs_uint8*) CS_MALLOC(len) ;

  switch (pattern) {
    case 0 :
      for(ii = 0; ii < len; ii++) {
        *(traceMsg+ii) = (ii + 1) ;
      }
      break ;
    case 1 :
      for(ii = 0; ii < len; ii++) {
        *(traceMsg+ii) = (0x55) ;
      }
      break ;
    case 2 :
      for(ii = 0; ii < len; ii++) {
        *(traceMsg+ii) = (1 << (ii % 8)) ;
      }
      break ;
  }

  m2r_framer_fill_trace_buf(port_id, section,
                            traceMsg, channel, dir) ;

  CS_FREE(traceMsg) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  DUMP TRACE BUFFER                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_debug_get_trace_msg(cs_uint16 port_id,
                                         cs_boolean section,
                                         m2r_framer_stream_t channel)
/* INPUTS     : o Port Id                                       */
/*              o Section Trace ( TRUE ) or PATH TRACE          */
/*              o Channel ID ( valid for path trace only )      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Dump received trace message .                                */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3                                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint8   traceMsg[64] ;
  cs_uint16  ii ;

  if (m2r_framer_get_trace_msg(port_id, section,
			       &traceMsg[0], channel) == CS_ERROR) {
    return (CS_ERROR) ;
  }

  CS_PRINT("\n\t Receive Trace Message buffer(all 64 bytes):") ;
  for (ii = 0; ii < 64; ii++) {
    if ((ii % 16) == 0) CS_PRINT("\n0x%04x :",ii) ;
    CS_PRINT("  %02x", traceMsg[ii]) ;
  }

  CS_PRINT("\n") ;
  return (CS_OK) ;
}

