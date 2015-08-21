/***********************************************************************/
/* This file contains unpublished documentation and software           */ 
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_eth
 *
 * Milan driver libraries pertaining to XETH block
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_registers.h"
#include "m2r_public.h"
#include "m2r_eth.h"
#include "m2r_eth_irq.h"
#include "m2r_eth_stats.h"

/************************************************************************/
/*    Macros and defines                                                */
/************************************************************************/
#define M2R_XETH_GET_DEV_BASEADDR(PORTID, BASE_ADDR, RETCODE)     \
{                                                                 \
  cs_uint32 error;                                                \
  if(M2R_IS_PORT_VALID(PORTID, error) != TRUE) {                  \
    CS_HNDL_ERROR((PORTID), error, ": port_id = %d\n", (PORTID)); \
    return (RETCODE) ;                                            \
  }                                                               \
  if(M2R_IS_SONET_MODE((PORTID)) == TRUE) {                       \
    CS_HNDL_ERROR((PORTID), EM2R_INVALID_IN_SONET_MODE,           \
                  "XETH is powered DOWN in SONET Mode\n") ;       \
    return (RETCODE) ;                                            \
  }                                                               \
  (BASE_ADDR) = M2R_PORT_ID_TO_DEV_BASE((PORTID));                \
}

/****************************************************************/
/* $rtn_hdr_start    CONFIGURE  XETH                            */
/* CATEGORY   : API                                             */
/* ACCESS     : Public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status  m2r_eth_init_cfg(cs_uint16 port_id, 
                   m2r_port_summ_t * pSumm, m2r_eth_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port configuration summary struct  */
/*              o Pointer to XETH configuration data structure  */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the XETH  as per the parameters in the            */
/* configuration structure(pCfg is the pointer to the structure)*/
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16 ctr = 0, delay = 10 ;
  cs_status status = CS_OK ;

  if ((pCfg == NULL) || (pSumm == NULL)) {
    CS_HNDL_ERROR(port_id,
              ((!pSumm) ? EM2R_PORT_SUMM_NULL : EM2R_PORT_CFG_NULL),
                  NULL) ;
    return(CS_ERROR) ;
  }

  CS_PRINT("\t Initializing ETHERNET...\n") ;
  m2r_eth_set_fifo_full_threshold(port_id, pCfg->fifo_high_thrshold) ;
  m2r_eth_set_burst_threshold(port_id, pCfg->burstLen) ;

/* Feature 1 */

  if ( m2r_eth_init_stats(port_id) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  while (((status = m2r_eth_get_clock_status(port_id)) == CS_ERROR) &&
         (ctr++ < delay)) {
#ifndef RELEASE_PLATFORM
    if (IN_SIM_ENV()) {
      m2r_sim_hreset_delay(port_id, 10) ;
    } else {
      CS_MDELAY(10) ;
    }
#else
    CS_MDELAY(10) ;
#endif
  }

  /* Dont touch registers in TX and RX blocks since the clocks
   * status indicates failure */
  if (status == CS_ERROR) {
    CS_HNDL_ERROR(port_id, EM2R_XETH_CLK_ERROR, "\n") ;
    return (CS_ERROR) ;
  }

  if ( m2r_eth_init_irq(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start    GET DEFAULT XETH CONFIGURATION             */
/* CATEGORY   : API                                             */
/* ACCESS     : Public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status  m2r_eth_get_default_cfg(cs_uint16 port_id,
                   m2r_port_summ_t * pSumm, m2r_eth_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port configuration summary struct  */
/*              o Pointer to XETH configuration data structure  */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Fill in the default configuration parameters for XETH.       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ((pCfg == NULL) || (pSumm == NULL)) {
    CS_HNDL_ERROR(port_id,
              ((!pSumm) ? EM2R_PORT_SUMM_NULL : EM2R_PORT_CFG_NULL),
                  NULL) ;
    return(CS_ERROR) ;
  }

  pCfg->burstLen = M2R_XETH_BURST_LEN_DEF ;
  pCfg->fifo_high_thrshold = M2R_XETH_FIFO_FULL_THRSHOLD_DEF ;

/* Feature 1 */

  m2r_eth_get_default_irq_cfg(port_id, pSumm, pCfg) ;

  return ( CS_OK ) ;
}

/****************************************************************/
/* $rtn_hdr_start    GET RUNNING XETH CONFIGURATION             */
/* CATEGORY   : API                                             */
/* ACCESS     : Public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status  m2r_eth_get_running_cfg(cs_uint16 port_id,
				   m2r_port_summ_t * pSumm,
				   m2r_eth_runcfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/*              o Pointer to configuration data structure       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time XETH hardware configuration     */
/* of the specified port.                                       */
/*                                                              */
/* The [pRunSumm] parameter is a pointer to the running config  */
/* summary section.                                             */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the block's          */
/* configuration data-structure.                                */
/*                                                              */
/* This API will fill in the all the fields in the block        */
/* run-time configuration datastructure.                        */
/* The caller is responsible for allocating memory for the      */
/* configuration structures.                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t              * pDev ;
/* Feature 1 */
  XETH_MP_Control_t    xethCtrl ;
  XETH_MP_TxBurstLength_t  burstLen ;

  if ((pCfg == NULL) || (pSumm == NULL)) {
    CS_HNDL_ERROR(port_id,
                  ((!pSumm) ? EM2R_PORT_SUMM_NULL : EM2R_PORT_CFG_NULL),
                  NULL) ;
    return(CS_ERROR) ;
  }

  M2R_XETH_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_ETH);

/* Feature 1 */

  xethCtrl.wrd = CS_REG_2READ(&pDev->XETH.mp_Control.wrd) ;
  pCfg->fifo_thrshold = xethCtrl.bf.TxPacketSkid ;

  burstLen.wrd = CS_REG_2READ(&pDev->XETH.TxBurstLength.wrd) ;
  pCfg->burst = burstLen.bf.TxBurstLength ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_ETH);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   DUMP RUNNING XETH CONFIGURATION             */
/* CATEGORY   : API                                             */
/* ACCESS     : Public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status  m2r_eth_dump_running_cfg(cs_uint16 port_id, 
				   m2r_eth_runcfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Prints the current run-time hardware configuration for the   */
/* this block.                                                  */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the running          */
/* configuration data-structure allocated by the caller with    */
/* the configuration data from HW already retrieved and filled  */
/* in, prior to calling this API.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return(CS_ERROR) ;
  }

/* Feature 1 */

  CS_PRINT("Burst Length        : %d\n", pCfg->burst) ;
  CS_PRINT("FIFO Full threshold : %d\n", pCfg->fifo_thrshold) ;
  return(CS_OK) ;
}

/* Feature 1 */

/****************************************************************/
/* $rtn_hdr_start  SET THE FIFO HIGH THRESHOLD VALUE            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status m2r_eth_set_fifo_full_threshold(cs_uint16 port_id,
                                           cs_uint8 threshold)
/* INPUTS     : o Port Id                                       */
/*              o Threshold value ( valid range 1 to 63 )       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the high threshold value for the FIFO. This value is the */
/* minimum number of the words of space in the input FIFO before*/
/* full is asserted.                                            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  M2R_t              * pDev ;
  XETH_MP_Control_t    xethCtrl ;

  M2R_XETH_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_ETH);
  xethCtrl.wrd = CS_REG_2READ(&pDev->XETH.mp_Control.wrd) ;
  xethCtrl.bf.TxPacketSkid = ( threshold & 0x3f ) ;
  M2R_REG_WRITE(&pDev->XETH.mp_Control.wrd, xethCtrl.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_ETH);

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE FILL THRESHOLD VALUE                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status m2r_eth_set_burst_threshold(cs_uint16 port_id,
                                       cs_uint8 threshold)
/* INPUTS     : o Port Id                                       */
/*              o Threshold value ( valid range 0 to 255 )      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the fill threshold value. This register specifies the    */
/* number of bytes to be available in the datacom transmit input*/
/* FIFO before the packet processing engine starts mapping a    */
/* given packet in the Ethernet payload envelope.               */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  M2R_t              * pDev ;
  XETH_MP_TxBurstLength_t  burstLen ;

  M2R_XETH_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  burstLen.wrd = CS_REG_2READ(&pDev->XETH.TxBurstLength.wrd) ;
  burstLen.bf.TxBurstLength = threshold ;
  M2R_REG_WRITE(&pDev->XETH.TxBurstLength.wrd, burstLen.wrd) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE DROPPING OF SHORT PKTS        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status m2r_eth_set_drop_runt_pkt(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) OR M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the dropping of packets which are eight or    */
/* fewer octets in length.                                      */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  M2R_t              * pDev ;
  XETH_MP_Control_t    xethCtrl ;

  M2R_XETH_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_ETH);

  xethCtrl.wrd = CS_REG_2READ(&pDev->XETH.mp_Control.wrd) ;
  xethCtrl.bf.TxDropShort = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->XETH.mp_Control.wrd, xethCtrl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_ETH);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start GET THE CURRENT CLOCK STATUS IN XETH BLOCK    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : ETHERNET                                        */
/* CHIP       : MILAN2                                          */
cs_status m2r_eth_get_clock_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the current clock status ( RX clock and TX clock ). A    */
/* return value of CS_ERROR indicates that clocks are absent.   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  M2R_t              * pDev ;
  XETH_MP_Status_t     clkStatus ;

  M2R_XETH_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;

  clkStatus.wrd = CS_REG_2READ(&pDev->XETH.mp_Status.wrd) ;

  if (clkStatus.bf.tx_no_clock || clkStatus.bf.rx_no_clock) {
    return (CS_ERROR) ;
  }
  else {
    return (CS_OK) ;
  }
}

/* Feature 1 */

/* Feature 1 */


