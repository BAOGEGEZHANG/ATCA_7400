/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_soh.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "arn5_registers.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_framer.h"
#include "arn5_framer_priv.h"
#include "arn5_framer_soh.h"

/****************************************************************/
/*   Internal functions                                         */
/****************************************************************/
static
cs_status  arn5_framer_fill_section_trace_buffer(ARN5_t * pDev,
                        cs_uint16 sl_num, cs_uint8 * traceMsg,
                        cs_dir_t dir) ;
static
cs_status  arn5_framer_fill_path_trace_buffer(ARN5_t * pDev,
                        cs_uint16 sl_num, cs_uint8 * traceMsg,
                        cs_dir_t dir) ;
static
cs_status  arn5_framer_get_ber_params(arn5_framer_signal_ber_t ber,
                        cs_uint16 *delay, cs_uint16 *scale,
                        cs_uint16 *leak,
                        arn5_framer_stsType_t stsType,
                        cs_boolean more_low_rate) ;
static
cs_uint16  arn5_framer_calc_ber_thold(arn5_framer_signal_ber_t ref_ber,
                        cs_uint16 ref_thold,
                        arn5_framer_signal_ber_t tgt_ber) ;

/****************************************************************/
/*   Intenal table to keep the TX overhead sources options      */
/****************************************************************/
typedef struct {
  cs_uint8  mask ;
  cs_uint8  src_val[ARN5_FR_MAX_TX_OVRHD_SRC] ;
} _arn5_framer_tx_ovrhd_src ;

#define ARN5_FR_TX_OVRHD_SRC_CPU         (0x01)
#define ARN5_FR_TX_OVRHD_SRC_TOH         (0x02)
#define ARN5_FR_TX_OVRHD_SRC_DCC         (0x04)
#define ARN5_FR_TX_OVRHD_SRC_HDLC        (0x08)
#define ARN5_FR_TX_OVRHD_SRC_RX_ALM      (0x20)

const static _arn5_framer_tx_ovrhd_src
                   xmit_ovrhd_src_tbl[ARN5_FR_MAX_OVRHD_BYTE_TYPE] = {
/**************************************************************
        | Mask  | Register |  TOH  |  DCC  | HDLC |  RX Alarm |
***************************************************************/
  [ ARN5_FR_OVRHD_C2 ] =
        { 0x01, {   0,         0,     0,      0,      0     }},

  [ ARN5_FR_OVRHD_G1 ] =
        { 0x01, {   0,         0,     0,      0,      0,    }},

  [ ARN5_FR_OVRHD_K1 ] =
        { 0x13, {   0,       0x02,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_K2 ] =
        { 0x13, {   0,       0x02,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_K3 ] =
        { 0x11, {   0,         0,     0,      0,      0     }},

  [ ARN5_FR_OVRHD_K4 ] =
        { 0x11, {   0,         0,     0,      0,      0     }},

  [ ARN5_FR_OVRHD_S1 ] =
        { 0x13, {   0,       0x02,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_Z2 ] =
        { 0x03, {   0,       0x01,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_Z3 ] =
        { 0x01, {   0,         0,     0,      0,      0     }},

  [ ARN5_FR_OVRHD_Z4 ] =
        { 0x01, {   0,         0,     0,      0,      0     }},

  [ ARN5_FR_OVRHD_Z5 ] =
        { 0x01, {   0,         0,     0,      0,      0     }},

  [ ARN5_FR_OVRHD_F1 ] =
        { 0x03, {   0,       0x01,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_E1 ] =
        { 0x03, {   0,       0x01,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_E2 ]
        { 0x03, {   0,       0x01,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_SDCC ] =
        { 0x0f, {   0,       0x02,   0x01,  0x03,     0     }},

  [ ARN5_FR_OVRHD_LDCC ] =
        { 0x0f, {   0,       0x02,   0x01,  0x03,     0     }},

  [ ARN5_FR_OVRHD_J0 ] =
        { 0x03, {   0,       0x01,    0,      0,      0     }},

  [ ARN5_FR_OVRHD_J1 ] =
        { 0x03, {   0,       0x01,    0,      0,      0     }}
} ;

#define ARN5_FR_GET_OH_SRC_SEL_VAL(OH_BYTE, SRC)          \
        xmit_ovrhd_src_tbl[(OH_BYTE)].src_val[(SRC)]

/****************************************************************/
/* $rtn_hdr_start  CONFIGURES THE DCC PORT                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_cfg_dcc_port(cs_uint16 port_id,
                        arn5_framer_dcc_port_mode_t  sel,
                        cs_dir_t direction, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o DCC Port configuration                        */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_ENABLE (1) OR CS_DISABLE (0)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable DCC Port to carry DCC bytes from Section/Line */
/* Sonet frame overhead.                                        */
/*                                                              */
/* The [sel]  parameter is specified as one of the following:   */
/* ARN5_FR_SECTION_DCC(0) or ARN5_FR_LINE_DCC(1) or             */
/* ARN5_FR_DCC_SEC_LINE(2)                                      */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  cs_boolean            rx_dir , tx_dir ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, direction) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

  if (rx_dir) {
    ARN5_FR_RECVOVERHEAD_Ctrl_t    ctrl_rx ;
    ctrl_rx.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd);
    ctrl_rx.bf.DCCmode = (ctl == CS_DISABLE) ? ARN5_DCCmode_DIS_DCC : sel ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd,
                 ctrl_rx.wrd);
  }

  if (tx_dir) {
    ARN5_FR_XMITOVERHEAD_Ctrl_t    ctrl_tx ;
    ctrl_tx.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd);
    ctrl_tx.bf.DCCmode = (ctl == CS_DISABLE) ? ARN5_DCCmode_DIS_DCC : sel ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
                 ctrl_tx.wrd);
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE SONET OVERHEAD BYTE                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_set_overheadByte(cs_uint16 port_id,
                        arn5_framer_ovrhd_byte_t ovrhd, cs_uint8 val)
/* INPUTS     : o Port Id                                       */
/*              o Overhead Byte Type                            */
/*              o Overhead Byte value                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the SONET Overhead byte to be sent in the transmit       */
/* stream.                                                      */
/* The [ovrhd] parameter is specified as following:             */
/* ARN5_FR_OVRHD_C2 or ARN5_FR_OVRHD_G1 or ARN5_FR_OVRHD_K1 or  */
/* ARN5_FR_OVRHD_K2 or ARN5_FR_OVRHD_K3 or ARN5_FR_OVRHD_K4 or  */
/* ARN5_FR_OVRHD_S1 or ARN5_FR_OVRHD_Z2 or ARN5_FR_OVRHD_Z3 or  */
/* ARN5_FR_OVRHD_Z4 or ARN5_FR_OVRHD_Z5 or ARN5_FR_OVRHD_F1 or  */
/* ARN5_FR_OVRHD_E1 or ARN5_FR_OVRHD_E2 or ARN5_FR_OVRHD_SDCC or*/
/* ARN5_FR_OVRHD_LDCC or ARN5_FR_OVRHD_J0 or ARN5_FR_OVRHD_J1   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);
  switch (ovrhd) {
    case ARN5_FR_OVRHD_C2 :
      {
        ARN5_FR_XMITOVERHEAD_PathC2Z5_t   C2_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathC2Z5.wrd ;
        C2_byte.wrd = CS_REG_READ( regaddr ) ;
        C2_byte.bf.C2 = val ;
        CS_REG_WRITE( regaddr, C2_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_G1 :
      {
        ARN5_FR_XMITOVERHEAD_alarm3Path_t G1_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.alarm3Path.wrd ;
        G1_byte.wrd = CS_REG_READ( regaddr ) ;
        G1_byte.bf.insRDIP = val ;
        CS_REG_WRITE( regaddr , G1_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_K1 :
      {
        ARN5_FR_XMITOVERHEAD_APS1_t K1_byte ;
        K1_byte.wrd = CS_REG_READ(
                        &pDev->slice[sl_num].Framer.APS1.wrd);
        K1_byte.bf.K1 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.APS1.wrd,
                     K1_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_K2 :
      {
        ARN5_FR_XMITOVERHEAD_APS1_t K2_byte ;
        K2_byte.wrd = CS_REG_READ(
                        &pDev->slice[sl_num].Framer.APS1.wrd);
        K2_byte.bf.K2 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.APS1.wrd,
                     K2_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_K3 :
      {
        ARN5_FR_XMITOVERHEAD_APS2_t K3_byte ;
        K3_byte.wrd = CS_REG_READ(
                        &pDev->slice[sl_num].Framer.APS2.wrd);
        K3_byte.bf.K3 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.APS2.wrd,
                     K3_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_K4 :
      {
        ARN5_FR_XMITOVERHEAD_APS2_t K4_byte ;
        K4_byte.wrd = CS_REG_READ(
                        &pDev->slice[sl_num].Framer.APS2.wrd);
        K4_byte.bf.K4 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.APS2.wrd,
                     K4_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_S1 :
      {
        ARN5_FR_XMITOVERHEAD_SyncStatus_t  S1_byte ;
        S1_byte.wrd =
            CS_REG_READ( &pDev->slice[sl_num].Framer.SyncStatus.wrd);
        S1_byte.bf.S1 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.SyncStatus.wrd,
                     S1_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_Z2 :
      {
        ARN5_FR_XMITOVERHEAD_Growth_t     Z2_byte ;
        Z2_byte.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.Growth.wrd) ;
        Z2_byte.bf.Z2 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.Growth.wrd, Z2_byte.wrd) ;
      }
      break ;

    case ARN5_FR_OVRHD_Z3 :
      {
        ARN5_FR_XMITOVERHEAD_PathGrowth_t Z3_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathGrowth.wrd ;
        Z3_byte.wrd = CS_REG_READ( regaddr ) ;
        Z3_byte.bf.Z3 = val ;
        CS_REG_WRITE(regaddr, Z3_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_Z4 :
      {
        ARN5_FR_XMITOVERHEAD_PathGrowth_t Z4_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathGrowth.wrd ;
        Z4_byte.wrd = CS_REG_READ( regaddr ) ;
        Z4_byte.bf.Z4 = val ;
        CS_REG_WRITE(regaddr, Z4_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_Z5 :
      {
        ARN5_FR_XMITOVERHEAD_PathC2Z5_t   Z5_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathC2Z5.wrd ;
        Z5_byte.wrd = CS_REG_READ( regaddr ) ;
        Z5_byte.bf.Z5 = val ;
        CS_REG_WRITE(regaddr, Z5_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_F1 :
      {
        ARN5_FR_XMITOVERHEAD_UserChannel_t F1_byte ;
        F1_byte.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.UserChannel.wrd) ;
        F1_byte.bf.F1 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.UserChannel.wrd,
                     F1_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_E1 :
      {
        ARN5_FR_XMITOVERHEAD_OrderWire_t   E1_byte ;
        E1_byte.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.OrderWire.wrd) ;
        E1_byte.bf.E1 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.OrderWire.wrd,
                     E1_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_E2 :
      {
        ARN5_FR_XMITOVERHEAD_OrderWire_t   E2_byte ;
        E2_byte.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.OrderWire.wrd) ;
        E2_byte.bf.E2 = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.OrderWire.wrd,
                     E2_byte.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_SDCC :
      {
        ARN5_FR_XMITOVERHEAD_DCC_t         DCC_bytes ;
        DCC_bytes.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.DCC.wrd) ;
        DCC_bytes.bf.Sect = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.DCC.wrd,
                     DCC_bytes.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_LDCC :
      {
        ARN5_FR_XMITOVERHEAD_DCC_t         DCC_bytes ;
        DCC_bytes.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.DCC.wrd) ;
        DCC_bytes.bf.Line = val ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.DCC.wrd, DCC_bytes.wrd ) ;
      }
      break ;

    case ARN5_FR_OVRHD_J0 :
    case ARN5_FR_OVRHD_J1 :
    default :
      CS_HNDL_ERROR(port_id, EARN5_INVALID_TOH_BYTE, NULL);
      break ;

  }
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  return ( CS_OK );
}


/****************************************************************/
/* $rtn_hdr_start  READ THE OVERHEAD BYTE RECEIVED              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_uint8  arn5_framer_get_overheadByte(cs_uint16 port_id,
                        arn5_framer_ovrhd_byte_t ovrhd)
/* INPUTS     : o Port Id                                       */
/*              o Overhead Byte Type                            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reads the received overhead byte from the SONET RX stream.   */
/* The overhead byte is read from the Framer register. Some     */
/* of these overhead bytes debounced in hardware while other    */
/* are updated with every SONET Frame received.                 */
/* The [ovrhd] parameter is specified as following:             */
/* ARN5_FR_OVRHD_C2 or ARN5_FR_OVRHD_G1 or ARN5_FR_OVRHD_K1 or  */
/* ARN5_FR_OVRHD_K2 or ARN5_FR_OVRHD_K3 or ARN5_FR_OVRHD_K4 or  */
/* ARN5_FR_OVRHD_S1 or ARN5_FR_OVRHD_Z2 or ARN5_FR_OVRHD_Z3 or  */
/* ARN5_FR_OVRHD_Z4 or ARN5_FR_OVRHD_Z5 or ARN5_FR_OVRHD_F1 or  */
/* ARN5_FR_OVRHD_E1 or ARN5_FR_OVRHD_E2 or ARN5_FR_OVRHD_SDCC or*/
/* ARN5_FR_OVRHD_LDCC or ARN5_FR_OVRHD_J0 or ARN5_FR_OVRHD_J1   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  switch (ovrhd) {
    case ARN5_FR_OVRHD_C2 :
      {
        ARN5_FR_SONET32RX_FilteredG1C2_t  C2_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.FilteredG1C2.wrd ;
        C2_byte.wrd = CS_REG_READ( regaddr ) ;
        return ( C2_byte.bf.FilteredC2 ) ;
      }

    case ARN5_FR_OVRHD_G1 :
      {
        ARN5_FR_SONET32RX_FilteredG1C2_t  G1_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.FilteredG1C2.wrd ;
        G1_byte.wrd = CS_REG_READ( regaddr ) ;
        return ( G1_byte.bf.FilteredG1 ) ;
      }

    case ARN5_FR_OVRHD_K1 :
      {
        ARN5_FR_SONET32RX_K1K2_t  K1_byte ;
        K1_byte.wrd = CS_REG_READ( &pDev->slice[sl_num].Framer.K1K2.wrd ) ;
        return ( K1_byte.bf.K1) ;
      }

    case ARN5_FR_OVRHD_K2 :
      {
        ARN5_FR_SONET32RX_K1K2_t  K2_byte ;
        K2_byte.wrd = CS_REG_READ( &pDev->slice[sl_num].Framer.K1K2.wrd ) ;
        return ( K2_byte.bf.K2) ;
      }

    case ARN5_FR_OVRHD_K3 :
      {
        ARN5_FR_SONET32RX_K3K4_t  K3_byte ;
        K3_byte.wrd = CS_REG_READ( &pDev->slice[sl_num].Framer.K3K4.wrd ) ;
        return ( K3_byte.bf.K3) ;
      }

    case ARN5_FR_OVRHD_K4 :
      {
        ARN5_FR_SONET32RX_K3K4_t  K4_byte ;
        K4_byte.wrd = CS_REG_READ( &pDev->slice[sl_num].Framer.K3K4.wrd ) ;
        return ( K4_byte.bf.K4) ;
      }

    case ARN5_FR_OVRHD_S1 :
      {
        ARN5_FR_SONET32RX_Z1S1_t  Z1S1_byte ;
        Z1S1_byte.wrd = CS_REG_READ( &pDev->slice[sl_num].Framer.Z1S1.wrd ) ;
        return ( Z1S1_byte.bf.Z1S1) ;
      }

    case ARN5_FR_OVRHD_Z3 :
      {
        ARN5_FR_RECVOVERHEAD_pathOverheadB_t Z3_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.pathOverheadB.wrd ;
        Z3_byte.wrd = CS_REG_READ( regaddr ) ;
        return (Z3_byte.bf.Z3 ) ;
      }

    case ARN5_FR_OVRHD_Z4 :
      {
        ARN5_FR_RECVOVERHEAD_pathOverheadB_t Z4_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.pathOverheadB.wrd ;
        Z4_byte.wrd = CS_REG_READ( regaddr ) ;
        return (Z4_byte.bf.Z4 ) ;
      }

    case ARN5_FR_OVRHD_Z5 :
      {
        ARN5_FR_RECVOVERHEAD_pathOverheadC_t Z5_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.pathOverheadC.wrd ;
        Z5_byte.wrd = CS_REG_READ( regaddr ) ;
        return (Z5_byte.bf.Z5 ) ;
      }

    case ARN5_FR_OVRHD_F1 :
      {
        ARN5_FR_RECVOVERHEAD_SectRow2_t F1_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.SectRow2.wrd ;
        F1_byte.wrd = CS_REG_READ( regaddr ) ;
        return (F1_byte.bf.F1 ) ;
      }

    case ARN5_FR_OVRHD_E1 :
      {
        ARN5_FR_RECVOVERHEAD_SectRow2_t E1_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.SectRow2.wrd ;
        E1_byte.wrd = CS_REG_READ( regaddr ) ;
        return (E1_byte.bf.E1 ) ;
      }

    case ARN5_FR_OVRHD_E2 :
      {
        ARN5_FR_RECVOVERHEAD_LineRow9_t E2_byte ;
        regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.LineRow9.wrd ;
        E2_byte.wrd = CS_REG_READ( regaddr ) ;
        return (E2_byte.bf.E2 ) ;
      }

    case ARN5_FR_OVRHD_Z2 :
    case ARN5_FR_OVRHD_SDCC :
    case ARN5_FR_OVRHD_LDCC :
    case ARN5_FR_OVRHD_J0 :
    case ARN5_FR_OVRHD_J1 :
      CS_HNDL_ERROR(port_id, EARN5_INVALID_TOH_BYTE, NULL);
      return (0) ;
  }
  return (0);
}


/****************************************************************/
/* $rtn_hdr_start  SET THE EXPECTED PATH LABEL ( C2 byte )      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_set_rx_path_label(cs_uint16 port_id,
                      cs_uint8 c2_byte)
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
/* ARN5_SONET_C2_ATM or  ARN5_SONET_C2_HDLC  or                 */
/* ARN5_SONET_C2_UNEQ or ARN5_SONET_C2_NON_SPECIFIC             */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint16             sl_num ;
  ARN5_FR_SONET32RX_PathLabel_t  C2_byte ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathLabel.wrd ;

  CS_REG_READ( regaddr ) ;
  C2_byte.bf.C2 = c2_byte ;
  CS_REG_WRITE( regaddr , C2_byte.wrd ) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  return ( CS_OK ) ;
}


/****************************************************************/
/* $rtn_hdr_start  SELECT THE SOURCE OF TX OVERHEAD BYTE        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_set_tx_ovrhd_src(cs_uint16 port_id,
                        arn5_framer_ovrhd_byte_t ovrhd,
                        arn5_framer_tx_ovrhd_src_t src)
/* INPUTS     : o Port Id                                       */
/*              o Overhead byte Type                            */
/*              o Overhead byte source                          */
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
/* ARN5_FR_OVRHD_C2 or ARN5_FR_OVRHD_G1 or ARN5_FR_OVRHD_K1 or  */
/* ARN5_FR_OVRHD_K2 or ARN5_FR_OVRHD_K3 or ARN5_FR_OVRHD_K4 or  */
/* ARN5_FR_OVRHD_S1 or ARN5_FR_OVRHD_Z2 or ARN5_FR_OVRHD_Z3 or  */
/* ARN5_FR_OVRHD_Z4 or ARN5_FR_OVRHD_Z5 or ARN5_FR_OVRHD_F1 or  */
/* ARN5_FR_OVRHD_E1 or ARN5_FR_OVRHD_E2 or ARN5_FR_OVRHD_SDCC or*/
/* ARN5_FR_OVRHD_LDCC or ARN5_FR_OVRHD_J0 or ARN5_FR_OVRHD_J1   */
/*                                                              */
/* The [src] parameter is specified as following:               */
/* ARN5_FR_OVRHD_SRC_CPU or ARN5_FR_OVRHD_SRC_TOH_PORT or       */
/* ARN5_FR_OVRHD_SRC_DCC_PORT or ARN5_FR_OVRHD_SRC_HDLC_PORT or */
/* ARN5_FR_OVRHD_SRC_RX_ALARM                                   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  ARN5_t                 * pDev ;
  cs_uint16             sl_num ;
  cs_uint8              mask = 0 ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (ovrhd >= ARN5_FR_MAX_OVRHD_BYTE_TYPE) {
    CS_HNDL_ERROR(port_id, EARN5_INVALID_TOH_BYTE, NULL);
    return ( CS_ERROR ) ;
  }

  /*
   * Do Sanity check to see if the overhead byte can be controlled
   * through the user specified port option.
   */
  switch (src) {
    case ARN5_FR_OVRHD_SRC_CPU  :
      mask = ARN5_FR_TX_OVRHD_SRC_CPU ;
      break ;
    case ARN5_FR_OVRHD_SRC_TOH_PORT :
      mask = ARN5_FR_TX_OVRHD_SRC_TOH ;
      break ;
    case ARN5_FR_OVRHD_SRC_DCC_PORT :
      mask = ARN5_FR_TX_OVRHD_SRC_DCC ;
      break ;
    case ARN5_FR_OVRHD_SRC_HDLC_PORT :
      mask = ARN5_FR_TX_OVRHD_SRC_HDLC ;
      break ;
    case ARN5_FR_OVRHD_SRC_RX_ALARM :
      mask = ARN5_FR_TX_OVRHD_SRC_RX_ALM ;
      break ;
    default :
      CS_HNDL_ERROR(port_id, EARN5_INVALID_OVRHD_SRC, NULL);
      return ( CS_ERROR ) ;
  }

  if (( xmit_ovrhd_src_tbl[ovrhd].mask & mask ) == 0) {
    CS_HNDL_ERROR(port_id, EARN5_OVRHD_SRC_NOT_VALID,
                  "Overhead byte can't be sourced from the specified port\n");
    return (CS_ERROR);
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

  switch ( ovrhd ) {
    case ARN5_FR_OVRHD_K1 :
    case ARN5_FR_OVRHD_K2 :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl1_t K1K2_src ;
        K1K2_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        K1K2_src.bf.K1K2Src = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     K1K2_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_K3 :
    case ARN5_FR_OVRHD_K4 :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl1_t K3K4_src ;
        K3K4_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        K3K4_src.bf.K3K4Src = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     K3K4_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_S1 :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl1_t S1_src ;
        S1_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        S1_src.bf.S1Src = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     S1_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_Z2 :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl2_t Z2_src ;
        Z2_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
        Z2_src.bf.Z2Src = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                     Z2_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_Z3 :
    case ARN5_FR_OVRHD_Z4 :
    case ARN5_FR_OVRHD_Z5 : {
      }
      break ;
    case ARN5_FR_OVRHD_F1 :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl1_t F1_src ;
        F1_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        F1_src.bf.F1Src = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     F1_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_E1 :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl1_t E1_src ;
        E1_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        E1_src.bf.E1Src = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     E1_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_E2 :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl2_t E2_src ;
        E2_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
        E2_src.bf.E2Src = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                     E2_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_SDCC :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl1_t SecDcc_src ;
        SecDcc_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        SecDcc_src.bf.SecDCCSrc = ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     SecDcc_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_LDCC :
      {
        ARN5_FR_XMITOVERHEAD_ByteCtrl1_t lineDcc_src ;
        lineDcc_src.wrd =
            CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
        lineDcc_src.bf.lineDCCSrc =
            ARN5_FR_GET_OH_SRC_SEL_VAL(ovrhd, src) ;
        CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                     lineDcc_src.wrd) ;
      }
      break ;
    case ARN5_FR_OVRHD_G1 :
    default :
      break ;
  }
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  return ( CS_OK ) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE EXPECTED TRACE MESSAGE               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_fill_trace_buf(cs_uint16 port_id,
                        cs_boolean sectionTrace, cs_uint8 * traceMsg,
                        cs_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SECTION TRACE ) or FALSE ( PATH TRACE) */
/*              o Pointer to the trace message                  */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                */
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
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (traceMsg == NULL) {
    CS_HNDL_ERROR(port_id, EARN5_FR_NULL_TRACE_MSG, NULL);
    return ( CS_ERROR ) ;
  }

  if (sectionTrace) {
    arn5_framer_fill_section_trace_buffer(pDev, sl_num, traceMsg, dir) ;
  }
  else {
    arn5_framer_fill_path_trace_buffer(pDev, sl_num, traceMsg, dir) ;
  }
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET THE RECEIVED TRACE MESSAGE               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_get_trace_msg(cs_uint16 port_id,
                        cs_boolean sectionTrace, cs_uint8 * traceMsg)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SECTION TRACE ) or FALSE ( PATH TRACE) */
/*              o Pointer to the user allocated buffer          */
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
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  cs_uint16             offset ;
  ARN5_FR_SONET32RX_SectionLineControl_t section_prvsn ;
  ARN5_FR_SONET32RX_PathProvision_t  path_prvsn ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (traceMsg == NULL) {
    CS_HNDL_ERROR(port_id, EARN5_FR_NULL_TRACE_MSG, NULL);
    return ( CS_ERROR ) ;
  }

  if (sectionTrace) {
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxSectionTraceExp.wrd ;
  }
  else {
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxPathTraceExp.wrd ;
  }

  for ( offset = 0; offset < 32; offset++) {
    cs_uint16   regdata ;

    regdata = CS_REG_READ((regaddr+offset)) ;
    traceMsg[offset * 2] = regdata >> 8 ; /* lo byte in 15:8 */
    traceMsg[(offset * 2) + 1] = regdata & 0xff ; /* hi byte in 7:0 */
  }

  if (sectionTrace) {
    section_prvsn.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.SectionLineControl.wrd);
    if (section_prvsn.bf.J0Mode) {
      section_prvsn.bf.J0Mode = 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                   section_prvsn.wrd) ;
      section_prvsn.bf.J0Mode = 1 ;
      CS_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                   section_prvsn.wrd) ;
    }
  }
  else {
    regaddr = &pDev->slice[sl_num].Framer.PathProvision.wrd ;
    path_prvsn.wrd = CS_REG_READ(regaddr) ;
    if (path_prvsn.bf.J1Mode) {
      path_prvsn.bf.J1Mode = 0 ;
      CS_REG_WRITE(regaddr, path_prvsn.wrd) ;
      path_prvsn.bf.J1Mode = 1 ;
      CS_REG_WRITE(regaddr, path_prvsn.wrd) ;
    }
  }
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE SECTION/PATH TRACE MODE              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_set_rx_trace_mode(cs_uint16 port_id,
                        cs_boolean sectionTrace, cs_boolean chkMode,
                        cs_boolean shortTr)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SECTION ) or FALSE ( PATH )            */
/*              o TRUE ( Checking Mode ) or FALSE (SAVE MODE)   */
/*              o TRUE(16-byte) or FALSE(64-byte)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the receive Section/Path Trace mode. In the checking mode*/
/* of operation an incoming section trace message is compared   */
/* against the message stored by the microprocessor in the      */
/* Expected Section Trace Buffer. In Saving-mode, J0 message is */
/* stored in the incoming Section Trace buffer and an interrupt */
/* is generated when a complete message is received.            */
/*                                                              */
/* Changing the length format(16/64) at run-time requires a soft*/
/* reset of SONET Rx overhead logic(RxLogicReset bit in         */
/* ARN5_FR_GLOBAL_MasterReset).                                 */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

  if ( sectionTrace ) {
    ARN5_FR_SONET32RX_SectionLineControl_t section_prvsn ;

    section_prvsn.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.SectionLineControl.wrd);
    section_prvsn.bf.J0Mode = (chkMode) ? 0 : 1 ;
    section_prvsn.bf.J0Len = (shortTr) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                 section_prvsn.wrd) ;
  }
  else {
    ARN5_FR_SONET32RX_PathProvision_t  path_prvsn ;

    regaddr = &pDev->slice[sl_num].Framer.PathProvision.wrd ;
    path_prvsn.wrd = CS_REG_READ(regaddr) ;
    path_prvsn.bf.J1Mode = (chkMode) ? 0 : 1 ;
    path_prvsn.bf.J1Len = (shortTr) ? 1 : 0 ;
    CS_REG_WRITE(regaddr, path_prvsn.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE TX SECTION/PATH TRACE MODE           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_sel_tx_trace_msg_src(cs_uint16 port_id,
                        cs_boolean sectionTrace,
                        arn5_framer_tx_trace_src_t src)
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
/* ARN5_FR_TX_TRACE_BUFFER or ARN5_FR_TX_TRACE_TOH or           */
/* ARN5_FR_TX_TRACE_FIXED                                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  cs_uint8              bit_ctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(sectionTrace) {
    ARN5_FR_XMITOVERHEAD_ByteCtrl1_t J0Src ;
    switch(src) {
      case ARN5_FR_TX_TRACE_BUFFER :
        bit_ctl = ARN5_J0Src_JOSRC_TRBUF ;
        break ;
      case ARN5_FR_TX_TRACE_TOH :
        bit_ctl = ARN5_J0Src_J0SRC_TOH ;
        break ;
      case ARN5_FR_TX_TRACE_FIXED :
        bit_ctl = ARN5_J0Src_J0SRC_DEF ;
        break ;
      default :
        CS_HNDL_ERROR(port_id, EARN5_INVALID_OVRHD_SRC, NULL);
        return (CS_ERROR) ;
    }
    ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

    J0Src.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd) ;
    J0Src.bf.J0Src = bit_ctl ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl1.wrd,
                 J0Src.wrd) ;

    ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  }
  else { /* Path Trace */
    ARN5_FR_XMITOVERHEAD_ByteCtrl2_t J1Src ;

    switch(src) {
      case ARN5_FR_TX_TRACE_BUFFER :
        bit_ctl = 1 ;
        break ;
      case ARN5_FR_TX_TRACE_TOH :
        bit_ctl = 0 ;
        break ;
      default :
        CS_HNDL_ERROR(port_id, EARN5_INVALID_OVRHD_SRC, NULL);
        return (CS_ERROR) ;
    }

    ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

    J1Src.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd) ;
    J1Src.bf.J1Src = bit_ctl ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.ByteCtrl2.wrd,
                 J1Src.wrd) ;

    ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONTROLS THE ZO BYTE IN SECTION OVERHEAD     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_set_Z0_mode(cs_uint16 port_id,
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
/* ARN5_SONET_FIX_ZO_DEF ( 0xCC )                               */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                 * pDev ;
  ARN5_FR_SONET32RX_Configuration_t  cfg ;
  ARN5_FR_SONET32RX_Z0_byte_t        Z0_byte ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if ( fix_ptrn ) {
    Z0_byte.wrd = 0;
    Z0_byte.bf.Z0_Byte_Value = Z0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Z0_byte.wrd,
                 Z0_byte.wrd) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);
  cfg.wrd = CS_REG_READ(
              &pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
  cfg.bf.Fixed_Z0 = (fix_ptrn) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd,
               cfg.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET SIGNAL DEGRADE(SD), FAIL(SF) THRESHOLDS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_set_signal_thresholds(cs_uint16 port_id,
                        arn5_framer_signal_ber_t sd_declare,
                        arn5_framer_signal_ber_t sd_clear,
                        arn5_framer_signal_ber_t sf_declare,
                        arn5_framer_signal_ber_t sf_clear)
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
/*   ARN5_FR_BER_5 to ARN5_FR_BER_8).                           */
/* o The other threshold parameters should be within 2 levels of*/
/*   [sd_declare]. For example, if [sd_declare] is              */
/*   ARN5_FR_BER_5  the other thresholds must be in the range of*/
/*   ARN5_FR_BER_3 to ARN5_FR_BER_7.                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  ARN5_t                        * pDev ;
  arn5_framer_signal_ber_t      ref_ber ;
  cs_uint16                     ref_thold ;
  cs_uint16                     sd_declare_thold, sd_clear_thold ;
  cs_uint16                     sf_declare_thold, sf_clear_thold ;
  cs_uint16                     delay, leak, scale ;
  ARN5_FR_SONET32RX_LBERAcc_t   lberAcc ;
  ARN5_FR_SONET32RX_LBER_t      lber ;
  cs_boolean                    error = FALSE ;
  arn5_framer_stsType_t         stsType ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (sd_clear < sd_declare) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_MIN_GT_MAX,
                  ": sd_clear(%d) s/b >= sd_declare(%d)\n",
                  sd_clear, sd_declare) ;
    return (CS_ERROR) ;
  }

  if (sf_clear < sf_declare) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_MIN_GT_MAX,
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
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
    ": sd_declare(%d), sd_clear(%d), sf_declare(%d), sf_clear(%d)\n",
     sd_declare, sd_clear, sf_declare, sf_clear) ;
    return (CS_ERROR) ;
  }

  if (arn5_mpif_is_in_oc12_mode(port_id)) {
    stsType = ARN5_FR_STS_TYPE_STS12C ;
  }
  else if (arn5_mpif_is_in_oc3_mode(port_id)) {
    stsType = ARN5_FR_STS_TYPE_STS3C ;
  }
  else {
    stsType = ARN5_FR_STS_TYPE_STS48C ;
  }

  if (arn5_framer_get_ber_params(ref_ber, &delay, &scale, &leak,
        stsType, ((sd_clear - ref_ber) == 2)) != CS_OK ) {
    return (CS_ERROR) ;
  }

 SCALE_AGAIN:
  /* leak value, scale, delay */
  lberAcc.wrd = 0 ;
  lberAcc.bf.Scale = scale ;
  lberAcc.bf.AccInterval = delay ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.LBERAcc.wrd, lberAcc.wrd) ;

  lber.wrd = 0;
  lber.bf.leakValue = leak ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.LBER.wrd, lber.wrd) ;

  /* Set the thresholds */
  ref_thold = leak;

  sd_declare_thold = ref_thold ; /* use ref for SD declare */
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.LBERDegradeDeclaring.wrd,
               sd_declare_thold) ;

  sd_clear_thold = arn5_framer_calc_ber_thold(ref_ber, ref_thold,
                                             sd_clear) ;
  if (sd_clear_thold < 2) {
    /* just in case we lose too much resolution, redo the calc */
    delay <<= 1;
    leak <<= 1;
    goto SCALE_AGAIN;
  }

  CS_REG_WRITE(&pDev->slice[sl_num].Framer.LBERDegradeClearing.wrd,
               sd_clear_thold) ;

  sf_declare_thold = arn5_framer_calc_ber_thold(ref_ber, ref_thold,
                                               sf_declare) ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.LBERFailureDeclaring.wrd,
               sf_declare_thold) ;

  sf_clear_thold = arn5_framer_calc_ber_thold(ref_ber, ref_thold,
                                             sf_clear) ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.LBERFailureClearing.wrd,
               sf_clear_thold) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET PATH SIGNAL FAIL(SF) THRESHOLDS          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_set_path_signal_thresholds(cs_uint16 port_id,
                        arn5_framer_signal_ber_t sf_declare,
                        arn5_framer_signal_ber_t sf_clear)
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
/*   ARN5_FR_BER_5 to ARN5_FR_BER_9).                           */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                     sl_num ;
  ARN5_t                         * pDev ;
  arn5_framer_signal_ber_t       ref_ber ;
  cs_uint16                     ref_thold ;
  cs_uint16                     sf_declare_thold, sf_clear_thold ;
  cs_uint16                     delay, leak, scale ;
  ARN5_FR_SONET32RX_PBERAcc_t    pberAcc ;
  ARN5_FR_SONET32RX_PBER_t       pber ;
  arn5_framer_stsType_t         stsType ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (sf_clear < sf_declare) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_MIN_GT_MAX,
                  ": sf_clear(%d) s/b >= sf_declare(%d)\n",
                  sf_clear, sf_declare) ;
    return (CS_ERROR) ;
  }

  ref_ber = sf_clear ; /* enumerated value */

  /* determine the path rate first and get desired parameter sttings */
  /* according to the path rate                                      */
  if (arn5_mpif_is_in_oc12_mode(port_id)) {
    stsType = ARN5_FR_STS_TYPE_STS12C ;
  }
  else if (arn5_mpif_is_in_oc3_mode(port_id)) {
    stsType = ARN5_FR_STS_TYPE_STS3C ;
  }
  else {
    stsType = ARN5_FR_STS_TYPE_STS48C ;
  }

  if (arn5_framer_get_ber_params(ref_ber, &delay, &scale, &leak,
        stsType, TRUE) != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* leak value, scale, delay */
  pberAcc.wrd = 0 ;
  pberAcc.bf.Scale = scale ;
  pberAcc.bf.AccInterval = delay ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.PBERAcc.wrd, pberAcc.wrd) ;

  pber.wrd = 0;
  pber.bf.leakValue = leak ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.PBER.wrd, pber.wrd) ;

  /* Set the thresholds */
  ref_thold = leak;

  sf_declare_thold = arn5_framer_calc_ber_thold(ref_ber, ref_thold,
                                               sf_declare) ;

  CS_REG_WRITE(&pDev->slice[sl_num].Framer.PBERFailureDeclaring.wrd,
               sf_declare_thold) ;

  sf_clear_thold = ref_thold;

  CS_REG_WRITE(&pDev->slice[sl_num].Framer.PBERFailureClearing.wrd,
               sf_clear_thold) ;

  return (CS_OK) ;
}


/****************************************************************/
/*   Internal functions                                         */
/****************************************************************/
static
cs_status  arn5_framer_fill_section_trace_buffer(ARN5_t * pDev,
                        cs_uint16 sl_num, cs_uint8 * traceMsg,
                        cs_dir_t dir)
{
  volatile cs_reg       * regaddr ;
  cs_uint8              offset, *msg = traceMsg ;
  cs_uint16             wrd ;
  cs_boolean            rx_dir , tx_dir ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  if ( rx_dir ) {
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxSectionTraceExp.wrd ;

    for ( offset = 0; offset < 32; offset++) {
      wrd = (*msg << 8) | (*(msg + 1)); /* lo byte in 15:8, hi in 7:0*/
      msg += 2 ;
      CS_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }

  if ( tx_dir ) {
    msg = traceMsg ;
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.SectionTrace.wrd ;

    for ( offset = 0; offset < 32; offset++) {
      wrd = (*msg << 8) | (*(msg + 1)); /* lo byte in 15:8, hi in 7:0*/
      msg += 2 ;
      CS_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }
  return (CS_OK) ;
}

static
cs_status  arn5_framer_fill_path_trace_buffer(ARN5_t * pDev,
                        cs_uint16 sl_num,
                        cs_uint8 * traceMsg, cs_dir_t dir)
{
  volatile cs_reg       * regaddr ;
  cs_uint8              offset, *msg;
  cs_uint16             wrd ;
  cs_boolean            rx_dir , tx_dir ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  if ( rx_dir ) {
    msg = traceMsg ;
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.RxPathTraceExp.wrd ;

    for ( offset = 0; offset < 32; offset++) {
        wrd = (*msg << 8) | (*(msg + 1)); /* lo byte in 15:8, hi in 7:0*/
        msg += 2 ;
        CS_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }

  if ( tx_dir ) {
    msg = traceMsg ;
    regaddr =
        (cs_reg *)&pDev->slice[sl_num].Framer.PathTrace.wrd ;

    for ( offset = 0; offset < 32; offset++) {
        wrd = (*msg << 8) | (*(msg + 1)); /* lo byte in 15:8, hi in 7:0*/
        msg += 2 ;
        CS_REG_WRITE((regaddr+offset), wrd ) ;
    }
  }
  return (CS_OK) ;
}


/* OC3 need special care here; we use two table to avoid
 * leak value overflow beyond 16 bits; at the same time
 * ensures the value is big enough so other thresholds can
 * be ratioed down from it
 */
static
cs_status  arn5_framer_get_ber_params(arn5_framer_signal_ber_t ber,
                        cs_uint16 *delay, cs_uint16 *scale,
                        cs_uint16 *leak, arn5_framer_stsType_t sts_type,
                        cs_boolean more_low_rate)
{
  struct _BER_TABLE {
    cs_uint16 delay;
    cs_uint16 scale;
    cs_uint16 leak;
  } ber_table[4][5] = {{{   2,  3, 49},  /* OC48 */
                        {  16,  3, 39},
                        {  64,  3, 16},
                        { 512,  3, 13},
                        {4096,  3, 10}},
                       {{   8,  3, 49},  /* OC12 */
                        {  64,  3, 39},
                        { 256,  3, 16},
                        {2048,  3, 13},
                        {8192,  3,  5}},
                       {{  32,  3, 49},  /* OC3 hi */
                        { 256,  3, 39},
                        {1024,  3, 16},
                        {4096,  3,  7},
                        {32768, 1,  2}}, /* this entry never used */
                                         /* OC3 low */
                       {{   8,  1, 49},  /* this entry never used */
                        {  64,  1, 39},
                        { 256,  1, 16},
                        {1024,  1,  7},
                        {32768, 1,  2}}} ;

  cs_uint16 stsIdx ;
  cs_uint16 index = ber - ARN5_FR_BER_5;

  switch (sts_type) {
    case ARN5_FR_STS_TYPE_STS12C :
      stsIdx = 1 ;
      break;

    case ARN5_FR_STS_TYPE_STS3C :
      if (((ber == ARN5_FR_BER_7) && more_low_rate) ||
          (ber >= ARN5_FR_BER_8)) {
        stsIdx = 3 ;
      }
      else {
        stsIdx = 2 ;
      }
      break;

    default :
      stsIdx = 0 ;
      break;
  }

  switch ( ber ) {
    case ARN5_FR_BER_5:
    case ARN5_FR_BER_6:
    case ARN5_FR_BER_7:
    case ARN5_FR_BER_8:
    case ARN5_FR_BER_9:
      *delay = ber_table[stsIdx][index].delay;
      *scale = ber_table[stsIdx][index].scale;
      *leak = ber_table[stsIdx][index].leak;
      break ;

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
cs_uint16 arn5_framer_calc_ber_thold(arn5_framer_signal_ber_t ref_ber,
                        cs_uint16 ref_thold,
                        arn5_framer_signal_ber_t tgt_ber)
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
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_debug_fill_trace_buf(cs_uint16 port_id,
                        cs_uint8 pattern, cs_boolean section,
                        cs_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o Pattern Type                                  */
/*              o Section Trace ( TRUE ) or PATH TRACE          */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Fill the trace buffer with pattern specified by pattern.     */
/* [pattern] can be one of the following :                      */
/* 0 - Incrementing pattern                                     */
/* 1 - fixed pattern of 0x55                                    */
/* 2 - walking ones pattern                                     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16 len = 64, ii ;
  cs_int8  * traceMsg ;

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

  arn5_framer_fill_trace_buf(port_id, section, traceMsg, dir) ;

  CS_FREE(traceMsg) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  DUMP TRACE BUFFER                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_debug_get_trace_msg(cs_uint16 port_id,
                        cs_boolean section)
/* INPUTS     : o Port Id                                       */
/*              o Section Trace ( TRUE ) or PATH TRACE          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Dump received trace message .                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint8   traceMsg[64] ;
  cs_uint16   ii ;

  if (arn5_framer_get_trace_msg(port_id, section,
                  (cs_int8*)&traceMsg[0]) != CS_OK) {
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

