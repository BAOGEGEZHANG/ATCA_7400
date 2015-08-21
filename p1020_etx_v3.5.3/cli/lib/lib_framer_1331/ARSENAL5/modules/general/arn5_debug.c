/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_debug.c
 *
 * Contains many of the debug utilities and functions that are useful for
 * testing purposes.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifdef POSIX_PTHREAD
#include <pthread.h>
#endif

#include "cs_types.h"
#include "cs_rtos.h"
#include "cs_utils.h"

#include "arn5_error.h"
#include "arn5_cb.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_gen_data.h"
#include "arn5_gen_api.h"
#include "arn5_mpif_api.h"
#include "arn5_pprbs.h"

#include "arn5_debug.h"


void arn5_dev_sim_cfg_params(cs_uint16 dev_id, arn5_dev_cfg_t * pDevCfg) ;

/*******************************************
 *                    STATIC               *
 *******************************************/
static
cs_status get_drvr_reg_addr(cs_uint8 dev_id, cs_uint8 inst_num,
                        cs_uint32 reg_offset,
                        volatile cs_reg  ** p_drvr_reg_addr) ;
static
unsigned long crc32_table[256];
static
void gen_crc32_table(void) ;



/****************************************************************/
/* $rtn_hdr_start  READ REGISTER                                */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_uint16 arn5_reg_read(cs_uint8 dev_id,
                        cs_uint32 reg_offset, cs_uint8 inst_num)
/* INPUTS     : o Device Id                                     */
/*              o Register offset                               */
/*              o Instance Number(if applicable)                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Register value                                  */
/* DESCRIPTION:                                                 */
/* Reads the value from the specified register.                 */
/* Note that the register offset is the actual register OFFSET  */
/* as given in the register spec, without any shifting!         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  volatile cs_reg   * drvr_reg_addr ;
  cs_uint32         err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_PRINT("%s() ERROR: Invalid dev-id(%d) - error code(0x%x)\n",
             __FUNCTION__, dev_id, err_code) ;
    return (0xdead) ;
  }

  if ( get_drvr_reg_addr(dev_id, inst_num, reg_offset,
                         &drvr_reg_addr) != CS_OK ) {
    return (0xdead) ;
  }

  return ( CS_REG_READ(drvr_reg_addr) ) ;
}


/****************************************************************/
/* $rtn_hdr_start  WRITE REGISTER                               */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_reg_write(cs_uint8 dev_id,
                        cs_uint32 reg_offset, cs_uint8 inst_num,
                        cs_uint16 data)
/* INPUTS     : o Device Id                                     */
/*              o Register offset                               */
/*              o Instance Number(if applicable)                */
/*              o Data to be written                            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Register value                                  */
/* DESCRIPTION:                                                 */
/* Writes the specified value into the register.                */
/* Note that the register offset is the actual register OFFSET  */
/* as given in the register spec, without any shifting!         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  volatile cs_reg * drvr_reg_addr ;
  cs_uint32       err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_PRINT("%s() ERROR: Invalid dev-id(%d) - error code(0x%x)\n",
             __FUNCTION__, dev_id, err_code) ;
    return (CS_ERROR) ;
  }

  if ( get_drvr_reg_addr(dev_id, inst_num, reg_offset,
                         &drvr_reg_addr) != CS_OK ) {
    return (CS_ERROR) ;
  }

  CS_REG_WRITE(drvr_reg_addr, data) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DEVICE DEBUG INITIALIZATION                  */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_dev_debug_init(cs_uint8 dev_id,
                        arn5_host_if_t host_if)
/* INPUTS     : o Device Id                                     */
/*              o Host Interface(SPI42 or SPI3)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This API is mainly for Cortina INTERNAL use!                 */
/*                                                              */
/* The [host_if] parameter is one of the following:             */
/* ARN5_HOST_IF_SPI42   or   ARN5_HOST_IF_SPI3                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_dev_cfg_t * pDevCfg = NULL ;
  cs_status     status = CS_OK ;

  pDevCfg = (arn5_dev_cfg_t *) CS_MALLOC( sizeof(arn5_dev_cfg_t) ) ;
  if (pDevCfg == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_MALLOC_FAILED, NULL)  ;
    return (CS_ERROR) ;
  }

  pDevCfg->summ.host_if          = host_if ;

  if ( arn5_dev_get_default_cfg(dev_id, pDevCfg) != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

   if ( CS_IN_SIM_ENV() ) {
    arn5_dev_sim_cfg_params(dev_id, pDevCfg) ;
  }

  if ( arn5_dev_init(dev_id, pDevCfg) != CS_OK ) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  RTN_EXIT :
    if (pDevCfg) {
      CS_FREE(pDevCfg) ;
    }

  return (status) ;
}


void arn5_dev_sim_cfg_params(cs_uint16 dev_id, arn5_dev_cfg_t * pDevCfg)
{
  /*
   * Only for testing the driver in sim environment - set any
   * additional configuration parameters which are needed only
   * for sim. These are mainly to speed up the init code.
   */

  return ;
}

/****************************************************************/
/* $rtn_hdr_start  PORT DEBUG INITIALIZATION                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_uint16  arn5_port_debug_init(cs_uint8 dev_id,
                        cs_uint8 slice_num,
                        arn5_port_line_rate_t line_rate,
                        arn5_port_prot_t l2_prot)
/* INPUTS     : o Device Id                                     */
/*              o Slice Number [0..7]                           */
/*              o Line Rate                                     */
/*              o Layer 2 protocol                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Port-Id(port handle)                            */
/* DESCRIPTION:                                                 */
/* This API is mainly for Cortina INTERNAL use!                 */
/*                                                              */
/* The [line_rate] parameter is one of the following:           */
/* ARN5_PORT_RATE_OC48C     or    ARN5_PORT_RATE_OC12C      or  */
/* ARN5_PORT_RATE_OC3C      or    ARN5_PORT_RATE_GIGE           */
/*                                                              */
/* The [l2_prot] parameter is one of the following:             */
/* ARN5_PORT_PROT_POS       or    ARN5_PORT_PROT_PPP        or  */
/* ARN5_PORT_PROT_LAPS      or    ARN5_PORT_PROT_GFP        or  */
/* ARN5_PORT_PROT_ATM       or    ARN5_PORT_PROT_CLEAR_CH   or  */
/* ARN5_PORT_PROT_ETHERNET                                      */
/*                                                              */
/* Note - This API takes dev_id as parameter and on successful  */
/* completion creates a port-handle(or port_id).                */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             port_id = ARN5_INVALID_PORT_ID ;
  arn5_port_cfg_t       * pCfg = NULL ;
  cs_uint32             err_code ;
  cs_status             status = CS_OK ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if (slice_num >= ARN5_MAX_NUM_PORTS) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        ": slice_num(%d) s/b 0..7!\n")  ;
    return (CS_ERROR) ;
  }

  /*
   * If the device hasn't been initialized, initialize it to
   * some default state.
   */
  if (arn5_dev_get_state(dev_id) != ARN5_DEV_STATE_INITIALIZED) {
    CS_PRINT("Dev-%d: Doing default initalization of device\n",
             dev_id) ;
    CS_PRINT("\tHost= SPI42\n") ;
    if ( arn5_dev_debug_init(dev_id, ARN5_HOST_IF_SPI42)
         != CS_OK ) {
      return (CS_ERROR) ;
    }
  }

  pCfg = (arn5_port_cfg_t *) CS_MALLOC( sizeof(arn5_port_cfg_t) ) ;
  if (pCfg == NULL) {
    CS_PRINT("%s() Error: MALLOC failed\n", __FUNCTION__) ;
    return (CS_ERROR) ;
  }

  CS_MEMSET( (void *)pCfg, 0, sizeof(arn5_port_cfg_t) ) ;
  pCfg->summ.line_rate    = line_rate ;
  pCfg->summ.l2_prot      = l2_prot ;

  port_id = arn5_port_open(dev_id, pCfg, slice_num) ;
  if (port_id == ARN5_INVALID_PORT_ID) {
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  CS_PRINT("Port-0x%04x: Doing DEBUG init of the port...\n", port_id) ;

  if ( arn5_port_init(port_id, pCfg) != CS_OK ) {
    CS_PRINT("ERROR during port initialization\n") ;
    arn5_port_close(port_id) ;
    status = CS_ERROR ;
    goto RTN_EXIT ;
  }

  RTN_EXIT :
  if (pCfg) CS_FREE(pCfg) ;
  if (status == CS_OK) {
    CS_PRINT("\n\t ARN5 Debug initialization done(Port-Id is 0x%04x)\n",
             port_id) ;
  }
  return (port_id) ;
}


/****************************************************************/
/* $rtn_hdr_start  PORT PACKET BERT(PPRBS) DEBUG INITIALIZATION */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_port_debug_init(cs_uint16 port_id,
                        cs_boolean fixPld, arn5_pprbs_pld_t pldType,
                        cs_uint16 burstSize, cs_uint16 rate,
                        cs_uint16 minSize, cs_uint16 maxSize,
                        cs_ctl_t  host_genCtl, cs_ctl_t  host_chkrCtl,
                        cs_ctl_t  line_genCtl, cs_ctl_t  line_chkrCtl)
/* INPUTS     : o Port Id                                       */
/*              o Fixed or PRBS payload                         */
/*              o Payload Type                                  */
/*              o Burst size(0 for continuous)                  */
/*              o Packet generator rate                         */
/*              o Min pkt size to be generated                  */
/*              o Host Gen enable (CS_ENABLE or CS_DISABLE)     */
/*              o Host Chkr enable(CS_ENABLE or CS_DISABLE)     */
/*              o Line Gen enable (CS_ENABLE or CS_DISABLE)     */
/*              o Line Chkr enable(CS_ENABLE or CS_DISABLE)     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : ----                                            */
/* DESCRIPTION:                                                 */
/* Debug initialization for Packet BERT(PPRBS) block.           */
/* This should be called only after port init has been done.    */
/*                                                              */
/* The [pldType] parameter specifies what the payload           */
/* type is and should be one of the following,                  */
/* ARN5_PPRBS_PLD_PKT  or  ARN5_PPRBS_PLD_ATM  or               */
/* ARN5_PPRBS_PLD_MULTICAST.                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_pprbs_cfg_t  cfg ;

  cfg.chan[ARN5_PPRBS_HOST_CHAN].fixPld =
    cfg.chan[ARN5_PPRBS_LINE_CHAN].fixPld = fixPld ;
  cfg.chan[ARN5_PPRBS_HOST_CHAN].pldType =
    cfg.chan[ARN5_PPRBS_LINE_CHAN].pldType = pldType ;
  cfg.chan[ARN5_PPRBS_HOST_CHAN].burstSize =
    cfg.chan[ARN5_PPRBS_LINE_CHAN].burstSize = burstSize ;
  cfg.chan[ARN5_PPRBS_HOST_CHAN].rate =
    cfg.chan[ARN5_PPRBS_LINE_CHAN].rate = rate ;
  cfg.chan[ARN5_PPRBS_HOST_CHAN].minSize =
    cfg.chan[ARN5_PPRBS_LINE_CHAN].minSize = minSize ;
  cfg.chan[ARN5_PPRBS_HOST_CHAN].maxSize =
    cfg.chan[ARN5_PPRBS_LINE_CHAN].maxSize = maxSize ;
  cfg.chan[ARN5_PPRBS_HOST_CHAN].genCtl  = host_genCtl ;
  cfg.chan[ARN5_PPRBS_LINE_CHAN].genCtl  = line_genCtl ;
  cfg.chan[ARN5_PPRBS_HOST_CHAN].chkrCtl = host_chkrCtl ;
  cfg.chan[ARN5_PPRBS_LINE_CHAN].chkrCtl = line_chkrCtl ;

  cfg.irq_ctl_tbl[ARN5_GRP_PPRBS_Interrupt] = ARN5_GRP_PPRBS_Interrupt_ALL ;

  if ( arn5_pprbs_init_cfg(port_id, &cfg) != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}



/****************************************************************/
/* $rtn_hdr_start  DUMP DRIVER CONTROL BLOCK                    */
/* CATEGORY   : Driver                                          */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void arn5_dump_drvr_cb()
/* INPUTS     : ----                                            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : ----                                            */
/* DESCRIPTION:                                                 */
/* Dumps the Driver Control Block. This API is provided for     */
/* debug purpose only.                                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32    ii ;
  cs_uint16    print_cntr = 0 ;

  if ( ARN5_DRVR_IS_LOADED() ) {
    CS_PRINT("\n") ;
    CS_PRINT("\t\tArsenal5 Driver Control Block(@ 0x%08x)\n",
             (cs_uint32)g_pARN5) ;
    CS_PRINT("drvr_loaded= %s\n",
             (g_pARN5->drvr_loaded) ? "yes" : "no") ;
    CS_PRINT("device cb table:\n") ;
    for (ii = 0; ii < ARN5_MAX_NUM_DEVICES; ii++) {
      if (g_pARN5->p_dev_cb_tbl[ii]) {
        CS_PRINT("dev_cb[%02d]= 0x%08x, ", ii,
                 (cs_uint32)g_pARN5->p_dev_cb_tbl[ii]) ;
        if ((print_cntr % 4) == 3) CS_PRINT("\n") ;
        print_cntr++ ;
      }
    }

    if (print_cntr == 0) CS_PRINT("\t No valid devices!\n") ;
    CS_PRINT("\n") ;

    CS_PRINT("drvr_flag= 0x%04x\n", g_pARN5->drvr_flag) ;
  }
  return ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP DEVICE CONTROL BLOCK                    */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void arn5_dump_dev_cb(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : ----                                            */
/* DESCRIPTION:                                                 */
/* Dumps the Device Control Block. This API is provided for     */
/* debug purpose only.                                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32     err_code , i ;
  arn5_dev_cb_t  * pdevcb ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_PRINT("Dev-%d ERROR: Port-Id(%d) is invalid!\n", dev_id) ;
    return ;
  }

  pdevcb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  CS_PRINT("\n") ;
  CS_PRINT("\t ****************************************************\n") ;
  CS_PRINT("\t Device-%d Control Block(@ 0x%x, size = 0x%x bytes)\n",
           pdevcb->dev_id, pdevcb, sizeof(arn5_dev_cb_t)) ;
  CS_PRINT("\t ****************************************************\n") ;
  CS_PRINT("State = %d (Invalid(0), Registered(1), Init'ed(2))\n",
           pdevcb->state) ;
  CS_PRINT("Configuration Summary @ 0x%x\n", &pdevcb->summ) ;
  CS_PRINT("Base Address = 0x%x\n", pdevcb->base_addr) ;
  CS_PRINT("Semaphores:\n") ;
  CS_PRINT("\tmpif= 0x%x, host_if= 0x%x\n",
           (cs_uint32)pdevcb->sem.mpif, (cs_uint32)pdevcb->sem.host_if) ;
  CS_PRINT("Debug flag = 0x%x\n", pdevcb->debug_flag) ;
  CS_PRINT("Num of OC48 = %d\n", pdevcb->oc48_num) ;
  CS_PRINT("Num of OC12 = %d\n", pdevcb->oc12_num) ;
  CS_PRINT("Num of OC3  = %d\n", pdevcb->oc3_num) ;
  CS_PRINT("Num of GIGE = %d\n", pdevcb->gige_num) ;
  CS_PRINT("Total Rate Unit = %d\n", pdevcb->total_rate_units) ;
  CS_PRINT("PCB pointer array @ 0x%x\n", pdevcb->ppcb) ;
  CS_PRINT("Statistics control block @ 0x%x\n", &pdevcb->stats_cb) ;
  CS_PRINT("Statistics data-struct   @ 0x%x\n", &pdevcb->stats) ;
  CS_PRINT("-------------------------------------------------------\n") ;
  CS_PRINT("\n") ;

  { /* Dump the register map also */
    ARN5_t   * pDev, * pReg ;

    pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
    pReg = 0 ;

    CS_PRINT("\t\t** Arsenal5 Register Map **\n\n") ;
    CS_PRINT(
    "Block            Byte Addr      Byte Offset     Reg Offset\n"
    ) ;
    CS_PRINT("MPIF           : 0x%08x     0x%08x      0x%08x\n",
             &pDev->MicroIF,
             &pReg->MicroIF,
             ((int)&pReg->MicroIF) / 2) ;
    CS_PRINT("SPI4P2         : 0x%08x     0x%08x      0x%08x\n",
             &pDev->SPI4P2,
             &pReg->SPI4P2,
             ((int)&pReg->SPI4P2) / 2) ;
    CS_PRINT("SPI42A         : 0x%08x     0x%08x      0x%08x\n",
             &pDev->SPI4P2a,
             &pReg->SPI4P2a,
             ((int)&pReg->SPI4P2a) / 2) ;
    for (i = 0 ; i < ARN5_MAX_NUM_PORTS; i++) {
      CS_PRINT("slice[%1d]       : 0x%08x     0x%08x      0x%08x\n", i,
               &pDev->slice[i],
               &pReg->slice[i],
               ((int)&pReg->slice[i]) / 2) ;
      CS_PRINT("slice[%1d].LIF   : 0x%08x     0x%08x      0x%08x\n", i,
               &pDev->slice[i].LIF,
               &pReg->slice[i].LIF,
               ((int)&pReg->slice[i].LIF) / 2) ;
      CS_PRINT("slice[%1d].PPRBS : 0x%08x     0x%08x      0x%08x\n", i,
               &pDev->slice[i].PPRBS,
               &pReg->slice[i].PPRBS,
               ((int)&pReg->slice[i].PPRBS) / 2) ;
      CS_PRINT("slice[%1d].GigE  : 0x%08x     0x%08x      0x%08x\n", i,
               &pDev->slice[i].GigE,
               &pReg->slice[i].GigE,
               ((int)&pReg->slice[i].GigE) / 2) ;
      CS_PRINT("slice[%1d].Framer: 0x%08x     0x%08x      0x%08x\n", i,
               &pDev->slice[i].Framer,
               &pReg->slice[i].Framer,
               ((int)&pReg->slice[i].Framer) / 2) ;
    }
  }

  return ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP PORT CONTROL BLOCK                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
void arn5_dump_port_cb(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : ----                                            */
/* DESCRIPTION:                                                 */
/* Dumps the Port Control Block(PCB). This is provided for      */
/* debug purpose only.                                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_port_cb_t * ppcb ;
  cs_uint32     err_code ;

  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {
    CS_PRINT("Port-0x%x ERROR: Port-Id(0x%x) is invalid!\n", port_id) ;
    return ;
  }

  ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  CS_PRINT("\n") ;
  CS_PRINT("\t ****************************************************\n") ;
  CS_PRINT(
     "\t Port-0x%x Control Block(@ 0x%x, size = 0x%x bytes)\n",
     ppcb->port_id, ppcb, sizeof(arn5_port_cb_t)
     ) ;
  CS_PRINT("\t ****************************************************\n") ;
  CS_PRINT(
  "State = %d (Invalid(0), Opened(1), Data Inited(2), HW Inited(3))\n",
  ppcb->state
  ) ;
  CS_PRINT("Configuration Summary @ 0x%x\n", &ppcb->summ) ;
  CS_PRINT("Statistics Control block @ 0x%x\n",
           &ppcb->stats_cb) ;
  CS_PRINT("Statistics data-struct   @ 0x%x\n", &ppcb->stats) ;
  CS_PRINT("-------------------------------------------------------\n") ;
  CS_PRINT("\n") ;

  return ;
}


/**********************************
 *     CRC16(HEC) CALCULATOR      *
 **********************************/
#define CCITT_CRC_GEN   0x1021
static  cs_boolean  hec_bit_reversal = TRUE ;
static  cs_boolean  hec_inverted     = TRUE ;

static
cs_uint8 reverse_bits_in_byte(cs_uint8 dataByte)
{
  cs_uint8  ii, dataOut = 0 ;

  for (ii = 0; ii < 8; ii++) {
    dataOut = ((dataByte >> ii) & 1) ^ ((dataOut << 1) & 0xfe) ;
  }
  return (dataOut) ;
}


cs_uint16  arn5_get_crc16(cs_uint8 * buf, cs_uint32 len)
{
  register cs_uint32 crc = 0xffff ;
  cs_uint32          dataByte ;
  cs_uint16          byte_idx, ii ;

  if (len != 16) {
    CS_PRINT("ERROR: len = %d\n", len) ;
    return(0) ;
  }

  for (byte_idx = 0; byte_idx < len; byte_idx++) {
    dataByte = buf[byte_idx] ;

    if (hec_bit_reversal) {
      dataByte = reverse_bits_in_byte(dataByte) ;
    }

    dataByte <<= 8 ;
    ii = 8 ;
    for (ii = 0; ii < 8; ii++) {
      register long bit = dataByte;
      dataByte += dataByte;
      bit ^= crc;
      crc += crc;
      if (bit &= 0x8000)
        crc ^= CCITT_CRC_GEN;
    } ;

  }

  if (hec_inverted) {
    crc = crc ^ 0xffff ;
  }

  if (hec_bit_reversal) {
    cs_uint8  crcbyte ;
    cs_uint32 dataOut = 0 ;

    for (ii = 0; ii < 2; ii++) {
      crcbyte = ( crc >> (8 * ii) ) & 0xff ;
      dataOut = (reverse_bits_in_byte(crcbyte) << 24) ^
                 ((dataOut >> 8) & 0xffffffff) ;
    }
    crc = (unsigned short)
          ( ((dataOut & 0xffff0000) >> 16) & 0xffff ) ;
  }

  return crc;
}


/***********************************
 *     CRC32  CALCULATOR           *
 ***********************************/
/*
 * Generate CRC32 lookup table
 */
static
void gen_crc32_table(void)
{
  unsigned long crc, polynomial;
  int   ii, jj;

  polynomial = 0xedb88320L;
  for (ii = 0; ii < 256; ii++) {
    crc = ii ;
    for (jj = 8; jj > 0; jj--) {
      if (crc & 1) {
        crc = (crc >> 1) ^ polynomial ;
      }
      else {
        crc >>= 1;
      }

      crc32_table[ii] = crc;
    }
  }
}

/*
 * Compute CRC32 value
 */
cs_uint32 arn5_get_crc32(cs_uint8 * buf, cs_uint32 len)
{
  cs_uint32              crc, temp ;
  cs_uint32              ii ;
  static cs_boolean      init_done = FALSE ;

  if (!init_done) {
    gen_crc32_table() ;
    init_done = TRUE ;
  }

  crc = 0xffffffff;
  for (ii = 0; ii < len; ii++) {
    crc  = ( crc>>8 ) ^ crc32_table[ (crc ^ buf[ii]) & 0xff ];
  }

  crc = crc ^ 0xffffffff ;
  temp = crc ;
  crc = ( ((crc >>  0) & 0xff) << 24 |
          ((crc >>  8) & 0xff) << 16 |
          ((crc >> 16) & 0xff) <<  8 |
          ((crc >> 24) & 0xff) <<  0 ) ;
  return (crc) ;

}


/*********************************************************************
 *         DELAY UTILITY FOR SIMULATION                              *
 * When running in RTL simulation, the delay needed should be        *
 * corresponding to the sim cycles and not to the cpu cycles in sim. *
 * For example, when doing sw initiated hard reset, we want to       *
 * make sure that the reset pulse is asserted long enough in terms   *
 * of simulation time.                                               *
 * Currently, this delay needed for simulation is implemented by     *
 * doing repeated reads of the jtag-id from the chip. For now, the   *
 * numbers used are just swag's - it's an hack and there is no       *
 * secret formula or logical reasoning for them.                     *
 *********************************************************************/
void arn5_sim_delay(cs_uint8 dev_id, cs_boolean is_in_msec, cs_uint32 delay)
{
  volatile cs_uint32  jtag_id ;
  cs_uint16           ii, loop_cnt ;

  CS_PRINT(
     "Delay implementation for sim: Read JTAG Id regs a few times\n"
     ) ;
  if (is_in_msec) {
    loop_cnt = 3 + (delay/10) ;
  }
  else {
    loop_cnt = 1 ;
  }

  for (ii = 0; ii < loop_cnt; ii++) {
    jtag_id = arn5_get_chip_jtag_id(dev_id) ;
  }
}


#ifdef POSIX_PTHREAD
static
void * arn5_irq_poll(void * dev_arg) ;

/****************************************************************/
/* $rtn_hdr_start INTERRUPT POLLING USING DEBUG(POSIX) THREAD   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_start_irq_polling(cs_uint8 dev_id,
                                 cs_uint32 msec_poll_intvl,
                                 cs_uint32 blk_id)
/* INPUTS     : o Device Id                                     */
/*              o Polling Interval in milli-seconds             */
/*              o Blocks to poll on                             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Start a debug thread for polling interrupts at device-level. */
/*                                                              */
/* The [blk_id] parameter specifies which block(s) to be        */
/* monitored. They can be one or or'ed value of the following-  */
/*                                                              */
/* Shared blocks, ie. ARN5_ID_HOST_IF, ARN5_ID_MPIF             */
/*                                                              */
/* Port-specific blocks, iet. ARN5_ID_FRAMER, ARN5_ID_LIF,      */
/* ARN5_ID_ETH, ARN5_ID_PPRBS                                   */
/*                                                              */
/* or,                                                          */
/* ARN5_ID_ALL_BLOCKS (for all the blocks in the chip)          */
/*                                                              */
/* NOTE -                                                       */
/* This API requires the POSIX Thread(pthread) library to be    */
/* linked with and is provided only for debugging purpose.The   */
/* debug thread is inactive by default. It gets spawned only    */
/* on calling this API.                                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  arn5_dev_cb_t  * pdevcb = NULL ;
  cs_uint8      devId ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pdevcb =  ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  devId  = pdevcb->dev_id ;

  if (pdevcb->dbt.irq_poll.tid) { /* check before starting one */
    CS_PRINT(
     "dev-%d Error: irq polling debug thread(tid=0x%x) already running!\n",
     dev_id, pdevcb->dbt.irq_poll.tid
     ) ;
    return (CS_ERROR) ;
  }

  pdevcb->dbt.irq_poll.msec_poll_intvl = msec_poll_intvl ;
  pdevcb->dbt.irq_poll.blk_id          = blk_id ;
  pdevcb->dbt.irq_poll.poll            = TRUE ;

  if ( pthread_create(&pdevcb->dbt.irq_poll.tid, NULL,
                      arn5_irq_poll,
                      (void *) &devId) ) {
    CS_PRINT("dev-%d Error in creating the irq polling thread!\n",
             dev_id) ;
    return (CS_ERROR) ;
  }

  CS_PRINT("dev-0x%04x: IRQ Polling thread(tid: 0x%x) started...\n",
           dev_id, pdevcb->dbt.irq_poll.tid) ;

  return (CS_OK) ;
}
#endif /* POSIX_PTHREAD */


#ifdef POSIX_PTHREAD
/****************************************************************/
/* $rtn_hdr_start STOP IRQ POLLING DEBU THREAD                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_stop_irq_polling(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable the debug thread processing for IRQ polling.         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32  dev_blks, port_blks ;
  arn5_dev_cb_t  * pdevcb = NULL ;
  void          * th_ret = NULL ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pdevcb =  ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;

  if ( pdevcb && pdevcb->dbt.irq_poll.tid ) {
    pdevcb->dbt.irq_poll.poll = FALSE ;
    pthread_join(pdevcb->dbt.irq_poll.tid, &th_ret)  ;
    CS_PRINT("dev-%d: Debug thread(tid: 0x%x) cancelled...\n",
             dev_id, pdevcb->dbt.irq_poll.tid) ;
  }

  pdevcb->dbt.irq_poll.tid = 0 ;


  dev_blks  = (pdevcb->dbt.irq_poll.blk_id &
               ARN5_DEV_SHRD_BLKS_ID) ;
  port_blks =    (pdevcb->dbt.irq_poll.blk_id &
                  ARN5_PORT_BLKS_ID) ;

  if (dev_blks) {
    arn5_dev_disable_blk_irq(dev_id, dev_blks) ;
  }

  if (port_blks) {
    cs_uint8  port_num ;

    for (port_num = 0; port_num < 8; port_num++) {
      cs_uint16     port_id ;
      arn5_port_cb_t * ppcb = NULL ;

      port_id = ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, port_num) ;
      ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;
      if ( (ppcb) &&
           (ppcb->state == ARN5_PORT_STATE_HW_INIT_DONE) ) {
        if ( !(arn5_mpif_is_in_gige_mode(port_id)) ) {
          port_blks &= ~ARN5_ID_ETH ;
        }
        arn5_port_disable_blk_irq(port_id, port_blks) ;
      }
    }
  }

  return (CS_OK) ;
}
#endif /* POSIX_PTHREAD */


/*****************************************
 *          STATIC FUNCTIONS             *
 *****************************************/
static
cs_status get_drvr_reg_addr(cs_uint8 dev_id, cs_uint8 inst_num,
                            cs_uint32 reg_offset,
                            volatile cs_reg  ** p_drvr_reg_addr)
{
  cs_uint32   drvr_reg_offset ;
  cs_uint32   dev_base ;
  cs_status   status = CS_OK ;

  dev_base = (cs_uint32) ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  drvr_reg_offset = reg_offset * sizeof(cs_reg) ;

  /* GIGE, FRAMER, LIF, PPRBS - "slice" registers */
  if ( ARN5_IS_SLICE_REG(drvr_reg_offset) ) {
    if (inst_num > ARN5_MAX_NUM_PORTS) {
      CS_PRINT("%s(): ERROR - Instance number s/b 0..7\n",
               __FUNCTION__) ;
      status = CS_ERROR ;
    }
    else {
      *p_drvr_reg_addr = ( (volatile cs_reg *)
                           (dev_base + (ARN5_SLICE_SIZE * inst_num) +
                            drvr_reg_offset) ) ;
    }
    goto RTN_EXIT ;
  }


  /* Other registers(MicroIF, SPI, etc.) */
  *p_drvr_reg_addr = ( (volatile cs_reg *)
                       (dev_base + drvr_reg_offset) ) ;

  RTN_EXIT :
    return (status) ;
}


#ifdef POSIX_PTHREAD
/*****************************************************
 * Main loop for IRQ polling debug thread            *
 *****************************************************/
static
void * arn5_irq_poll(void * dev_arg)
{
  cs_uint32  dev_blks, port_blks ;
  arn5_dev_cb_t   * pdevcb = NULL ;
  cs_uint8       dev_id = *( (cs_uint8 *) dev_arg ) ;
  pdevcb         = ARN5_DEV_ID_TO_DEVCB_PTR( dev_id ) ;

  dev_blks = (pdevcb->dbt.irq_poll.blk_id &
              ARN5_DEV_SHRD_BLKS_ID) ;
  port_blks = (pdevcb->dbt.irq_poll.blk_id &
               ARN5_PORT_BLKS_ID) ;

  if (dev_blks) {
    if ( arn5_dev_enable_blk_irq(dev_id, dev_blks)
         != CS_OK ) {
      CS_PRINT(
               "dev-%d: %s() ERROR - cannot enable block IRQ's(0x%x)!!\n",
               dev_id, __FUNCTION__, dev_blks
               ) ;
      return (NULL) ;
    }
  }

  if (port_blks) {
    cs_uint8  port_num ;

      for (port_num = 0; port_num < 8; port_num++) {
        cs_uint16     port_id ;
        arn5_port_cb_t * ppcb = NULL ;

        port_id = ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, port_num) ;
        ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

        if ( (ppcb) &&
             (ppcb->state == ARN5_PORT_STATE_HW_INIT_DONE) ) {
          if ( !(arn5_mpif_is_in_gige_mode(port_id)) ) {
            port_blks &= ~ARN5_ID_ETH ;
          }
          arn5_port_enable_blk_irq(port_id, port_blks) ;
        }
      }
  }

  while ( pdevcb->dbt.irq_poll.poll ) { /* irq polling main loop starts here */
    arn5_isr_handler( dev_id ) ;

    ARN5_MDELAY( dev_id,
                 pdevcb->dbt.irq_poll.msec_poll_intvl ) ;
  }

  return ( NULL );
}
#endif /* POSIX_PTHREAD */



