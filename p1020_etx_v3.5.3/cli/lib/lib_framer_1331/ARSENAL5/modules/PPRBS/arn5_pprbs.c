/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_pprbs.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_stats.h"
#include "arn5_irq_priv.h"

#include "arn5_pprbs.h"
#include "arn5_pprbs_stats.h"
#include "arn5_pprbs_priv.h"



/****************************************************************/
/* Macros and internal functions                                */
/****************************************************************/
#define ARN5_PPRBS_GET_DEVICE_ADDR(PORTID, BASE_ADDR, SLICE, RETCODE)   \
{                                                                \
  cs_uint32     error;                                           \
  if (ARN5_IS_PORT_VALID(PORTID, &error) != TRUE) {              \
    CS_HNDL_ERROR((PORTID), error, ": port_id= %d\n", (PORTID)); \
    return (RETCODE) ;                                           \
  }                                                              \
  (BASE_ADDR) = ARN5_PORT_ID_TO_DEV_BASE((PORTID));              \
  (SLICE)     = ARN5_PORT_ID_TO_PORT_NUM((PORTID));              \
}


#define ARN5_PPRBS_IS_VALID_CHAN(PORTID, CHAN, RETCODE)          \
{                                                                \
  if((CHAN) > ARN5_PPRBS_CHAN_ALL) {                             \
    CS_HNDL_ERROR((PORTID), EARN5_PPRBS_INVALID_CHANNEL, NULL);  \
    return (RETCODE) ;                                           \
  }                                                              \
}

/*
 * Calculate the rate. The function returns the rate in precent of the
 * line rate. For example a return value of 95 means 95%.
 */
cs_uint16  arn5_pprbs_cal_rate(cs_uint16 min, cs_uint16 max,
                        cs_boolean sz_disable, cs_uint16 sz_val,
                        cs_uint16 ipg)
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
cs_uint16  arn5_pprbs_cal_ipg(cs_uint16 rate, cs_uint16 min, cs_uint16 max,
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


/*--------------------------------------------------------------*/
/* PPRBS is intended to be a debug/test function. Thus no need  */
/* to concern any configuration during chip init as OFF(chip    */
/* default) is just what we needed. To use this function, call  */
/* relevent API's or arn5_pprbs_init_cfg() directly             */
/*--------------------------------------------------------------*/

/****************************************************************/
/* $_hdr_start  PPRBS INITIALIZATION                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_init_cfg(cs_uint16 port_id,
                        arn5_pprbs_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to PPRBS configuration structure      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the PPRBS block of the specified port with  */
/* the given configuration. User may call this API explicitly   */
/* after arn5_port_init() only when pprbs function is needed.   */
/* The caller is responsible for allocating memory for the      */
/* configuration structure, populating it (for both channels).  */
/* $_hdr_end                                                    */
/****************************************************************/
{
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;
  cs_uint32             i ;
  arn5_pprbs_chan_cfg_t * pChanCfg ;

  if(pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_CFG_NULL, "\n") ;
    return (CS_ERROR) ;
  }

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  CS_PRINT("\t Initializing Packet PRBS... \n") ;
  /* Disable Pkt Generator and checker */
  arn5_pprbs_ctl_pktGen(port_id, ARN5_PPRBS_CHAN_ALL, CS_DISABLE) ;
  arn5_pprbs_ctl_pktChkr(port_id, ARN5_PPRBS_CHAN_ALL, CS_DISABLE) ;

  /*
   * Clear the counters and stats.
   */
  arn5_pprbs_reset_stats(port_id, ARN5_PPRBS_CHAN_ALL, TRUE, TRUE) ;

  for (i = ARN5_PPRBS_HOST_CHAN; i < ARN5_PPRBS_CHAN_ALL; i++) {
    pChanCfg = &pCfg->chan[i] ;
    /*
     * Configure the payload, traffic parameters.
     */
    arn5_pprbs_ctl_payload(port_id, (arn5_pprbs_chan_t) i,
                pChanCfg->pldType) ;

    arn5_pprbs_cfg_traffic(port_id, (arn5_pprbs_chan_t) i,
                pChanCfg->fixPld,
                pChanCfg->burstSize,
                pChanCfg->rate,
                pChanCfg->minSize,
                pChanCfg->maxSize) ;

    arn5_pprbs_ctl_pktGen(port_id, (arn5_pprbs_chan_t) i ,
                pChanCfg->genCtl) ;
    arn5_pprbs_ctl_pktChkr(port_id, (arn5_pprbs_chan_t) i ,
                pChanCfg->chkrCtl) ;
  }

  /* register IRQ */
  if (arn5_pprbs_init_irq(port_id, pCfg) != CS_OK) {
    return (CS_ERROR) ;
  }

  /* Init Stats */
  if (arn5_pprbs_init_stats(port_id) != CS_OK) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start    GET RUNNING PPRBS CONFIGURATION               */
/* CATEGORY   : API                                             */
/* ACCESS     : Privat                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_pprbs_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to running config data structure      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time PPRBS hardware configuration    */
/* of the specified port.                                       */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the port's           */
/* configuration data-structure.                                */
/*                                                              */
/* This API will fill in the all the fields in the block        */
/* run-time configuration datastructure.                        */
/* $_hdr_end                                                    */
/****************************************************************/
{
  ARN5_PPRBS_GenCntl0_t chanCtrl0 ;
  ARN5_PPRBS_GenCntl1_t chanCtrl1 ;
  ARN5_PPRBS_ChkCntl0_t chkrCtl0 ;
  ARN5_PPRBS_ChkCntl1_t chkrCtl1 ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;
  arn5_pprbs_runcfg_t   * pCfg = &pRunCfg->pprbs ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  chkrCtl0.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
  pCfg->hostChkr = (chkrCtl0.bf.enable == 1) ;

  chkrCtl1.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
  pCfg->lineChkr = (chkrCtl1.bf.enable == 1) ;

  chanCtrl0.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
  pCfg->hostGen = (chanCtrl0.bf.enable == 1) ;

  chanCtrl1.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
  pCfg->lineGen = (chanCtrl1.bf.enable == 1) ;

  if (pCfg->hostChkr || pCfg->lineChkr ||
      pCfg->hostGen  || pCfg->lineGen) {
    pRunCfg->pprbs_valid = TRUE ;
  }
  else {
    pRunCfg->pprbs_valid = FALSE ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start   DUMP RUNNING PPRBS CONFIGURATION               */
/* CATEGORY   : API                                             */
/* ACCESS     : Private                                         */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_pprbs_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to configuration data structure       */
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
/* $_hdr_end                                                    */
/****************************************************************/
{
  cs_int8               * enbl = "Enabled" ;
  cs_int8               * disabl = "Disabled" ;
  arn5_pprbs_runcfg_t   * pCfg = &pRunCfg->pprbs ;

  CS_PRINT("Host channel Generator  : %s\n",
           (pCfg->hostGen ? enbl : disabl)) ;
  CS_PRINT("Host channel Checker    : %s\n",
           (pCfg->hostChkr ? enbl : disabl)) ;
  CS_PRINT("Line channel Generator  : %s\n",
           (pCfg->lineGen ? enbl : disabl)) ;
  CS_PRINT("Line channel Checker    : %s\n",
           (pCfg->lineChkr ? enbl : disabl)) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  RESET THE STATS REGISTERS                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_reset_stats(cs_uint16 port_id,
                        arn5_pprbs_chan_t chan, cs_boolean pktGen,
                        cs_boolean checker)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/*              o TRUE ( Clear Generater ) Stats                */
/*              o TRUE ( Clear checker   ) Stats                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Resets the stats/counter registers.                          */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    if (checker) {
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkErr0.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteMSB0.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteLSB0.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketMSB0.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketLSB0.wrd, 0) ;
    }
    if (pktGen) {
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteMSB0.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteLSB0.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketMSB0.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketLSB0.wrd, 0) ;
    }
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    if (checker) {
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkErr1.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteMSB1.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkByteLSB1.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketMSB1.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkPacketLSB1.wrd, 0) ;
    }
    if (pktGen) {
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteMSB1.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenByteLSB1.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketMSB1.wrd, 0) ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenPacketLSB1.wrd, 0) ;
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONFIGURES THE PPRBS TRAFFIC PARAMETERS      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_cfg_traffic(cs_uint16 port_id, arn5_pprbs_chan_t chan,
                        cs_boolean fixpld, cs_uint16 burstSize,
                        cs_uint16 rate, cs_uint16 minSize,
                        cs_uint16 maxSize)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/*              o Fiexd Payload or PRBS payload                 */
/*              o Burst Size ( 0 for continuous generation )    */
/*              o Pkt Generation Rate                           */
/*              o Min Pkt Size                                  */
/*              o Max Pkt Size                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the PPRBS traffic properties.                     */
/*                                                              */
/* [rate] specifies packet generation rate in percentage of line*/
/* rate.                                                        */
/* for example: a value of 90 means 90% of line rate.           */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* NOTE : A burstSize of 0 ( Zero ) means continuous burst.     */
/* If the maxPkt and minPkt are same a fixed size packet will be*/
/* generated.                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  cs_uint16     ipg = 0 ;
  cs_boolean    sz_disable ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if(rate > 100) {
    CS_HNDL_ERROR(port_id, EARN5_PPRBS_INVALID_RATE, "rate = %d\n", rate);
  }
  sz_disable = (minSize == maxSize ) ;
  ipg = arn5_pprbs_cal_ipg(rate, minSize, maxSize, sz_disable, (minSize % 8)) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_PPRBS) ;

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenCntl0_t   genCtrl ;
    ARN5_PPRBS_GenNum0_t    burstCtl ;
    ARN5_PPRBS_ChkCntl0_t   chkCtrl ;

    genCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
    genCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    if ( sz_disable ) {
      genCtrl.bf.sz_val = (minSize % 8) ;
    }
    genCtrl.bf.sz_disable = (sz_disable) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd, genCtrl.wrd) ;

    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMinLen0.wrd, (minSize >> 3)) ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMaxLen0.wrd, (maxSize >> 3)) ;

    /* Program the IPG value to achieve the desired rate. */
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG0.wrd, ipg) ;

    /*
     * Set the burst length. A value of Zero means continuous burst.
     */
    burstCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenNum0.wrd) ;
    burstCtl.bf.num = burstSize ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenNum0.wrd, burstCtl.wrd) ;

    /*
     * Configure the Checker for expected payload.
     */
    chkCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
    chkCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkCtrl.wrd) ;
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenCntl1_t   genCtrl ;
    ARN5_PPRBS_GenNum1_t    burstCtl ;
    ARN5_PPRBS_ChkCntl1_t   chkCtrl ;

    genCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
    genCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    if ( sz_disable ) {
      genCtrl.bf.sz_val = (minSize % 8) ;
    }
    genCtrl.bf.sz_disable = sz_disable ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd, genCtrl.wrd) ;

    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMinLen1.wrd, (minSize>>3)) ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenMaxLen1.wrd, (maxSize>>3)) ;

    /*
     * Program the IPG after calculating IPG.
     */
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG1.wrd, ipg) ;


    /*
     * Set the burst length. A value of Zero means continuous burst.
     */
    burstCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenNum1.wrd) ;
    burstCtl.bf.num = burstSize ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenNum1.wrd, burstCtl.wrd) ;

    /*
     * Configure the Checker for expected payload.
     */
    chkCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
    chkCtrl.bf.fixed_format = (fixpld) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkCtrl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_PPRBS);
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SELECT THE PPRBS TRAFFIC PAYLOAD             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_ctl_payload(cs_uint16 port_id,
                        arn5_pprbs_chan_t chan, arn5_pprbs_pld_t pld)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/*              o Payload type ( ATM or multicast or Packet )   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the PPRBS generator to generate user specified    */
/* payload like ATM or Packet or MULTICAST traffic.             */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* The [pld] parameter is specified as following:               */
/* ARN5_PPRBS_PLD_PKT or ARN5_PPRBS_PLD_MULTICAST or            */
/* ARN5_PPRBS_PLD_ATM                                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  cs_uint8      atm, multicast ;

  switch ( pld ) {
    case ARN5_PPRBS_PLD_ATM :
      atm = 1 ;
      multicast = 0 ;
      break ;

    case ARN5_PPRBS_PLD_MULTICAST :
      atm = 0 ;
      multicast = 1 ;
      break ;

    case ARN5_PPRBS_PLD_PKT :
    default :
      atm = 0 ;
      multicast = 0 ;
      break ;
  }

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_PPRBS) ;

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenCntl0_t   chanCtrl ;
    ARN5_PPRBS_ChkCntl0_t   chkrCtrl ;

    chkrCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
    chkrCtrl.bf.atm = atm ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkrCtrl.wrd) ;

    chanCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
    chanCtrl.bf.atm = atm;
    chanCtrl.bf.multicast = multicast ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd, chanCtrl.wrd) ;
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenCntl1_t   chanCtrl ;
    ARN5_PPRBS_ChkCntl1_t   chkrCtrl ;

    chkrCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
    chkrCtrl.bf.atm = atm ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkrCtrl.wrd) ;

    chanCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
    chanCtrl.bf.atm = atm;
    chanCtrl.bf.multicast = multicast ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd, chanCtrl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_PPRBS);
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PPRBS GENERATOR               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_ctl_pktGen(cs_uint16 port_id,
                        arn5_pprbs_chan_t chan, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/*              o CS_ENABLE or CS_DISABLE                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the PPRBS generator.                          */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* NOTE : user should call arn5_pprbs_cfg_traffic() API to      */
/* configure traffic parameters before enabling the Generator.  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;

  /* note that data will always go to first physical SPI channel */

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_PPRBS) ;

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenCntl0_t   chanCtrl ;
    chanCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd) ;
    chanCtrl.bf.enable = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl0.wrd, chanCtrl.wrd) ;
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenCntl1_t   chanCtrl ;
    chanCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd) ;
    chanCtrl.bf.enable = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenCntl1.wrd, chanCtrl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_PPRBS);
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONFIGURE THE INTER PACKET GAP               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_ctl_ipg(cs_uint16 port_id, arn5_pprbs_chan_t chan,
                        cs_uint16 ipg)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/*              o IPG                                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the inter-packet gap (the number of coreclk cycles*/
/* to wait between generated packets)                           */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenIPG0_t    chanIpg ;
    chanIpg.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenIPG0.wrd) ;
    chanIpg.bf.ipg = ipg ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG0.wrd, chanIpg.wrd) ;
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_GenIPG1_t    chanIpg ;
    chanIpg.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenIPG1.wrd) ;
    chanIpg.bf.ipg = ipg ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenIPG1.wrd, chanIpg.wrd) ;
  }
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  INJECT ERROR IN PPRBS TRANSMIT STREAM        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_inject_err(cs_uint16 port_id, arn5_pprbs_chan_t chan,
                        cs_boolean singleErr, cs_uint16 duration)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/*              o TRUE ( Single Error ) else FALSE              */
/*              o Duration (us) for which errors will be        */
/*                generated                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the PPRBS generator to inject errors. If Single   */
/* Error needs to be generated Set the singleErr to TRUE else   */
/* errors will be injected continuously for the duration        */
/* specified in microseconds.                                   */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* NOTE : This function might block for the "duration " if      */
/* continueous errors need to be generated and duration is      */
/* non-zero.                                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  ARN5_PPRBS_GenErr1_t    injErr0, injErr1 ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_PPRBS) ;

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    injErr0.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenErr0.wrd) ;
    if (singleErr) {
      injErr0.bf.single_err = 1 ;
    }
    else {
      injErr0.bf.cont_err = 1 ;
    }
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr0.wrd, injErr0.wrd) ;
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    injErr1.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.GenErr1.wrd) ;
    if (singleErr) {
      injErr1.bf.single_err = 1 ;
    }
    else {
      injErr1.bf.cont_err = 1 ;
    }
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr1.wrd, injErr1.wrd) ;
  }

  /*
   * If error injection is for specific time wait.
   */
  if(singleErr) {
    CS_UDELAY(100) ;
    if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
      injErr0.bf.single_err = 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr0.wrd, injErr0.wrd) ;
    }

    if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
      injErr1.bf.single_err = 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr1.wrd, injErr1.wrd) ;
    }
  }
  else {
    CS_UDELAY(duration) ;

    if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
      injErr0.bf.cont_err = 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr0.wrd, injErr0.wrd) ;
    }

    if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
      injErr1.bf.cont_err = 0 ;
      CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.GenErr1.wrd, injErr1.wrd) ;
    }
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_PPRBS);
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PPRBS CHECKER                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_ctl_pktChkr(cs_uint16 port_id,
                        arn5_pprbs_chan_t chan, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/*              o CS_ENABLE or CS_DISABLE                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the PPRBS checker.                            */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;

  /* note that data will always come from first physical SPI channel */

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_PPRBS) ;

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_ChkCntl0_t chkrCtl ;
    chkrCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
    chkrCtl.bf.enable = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkrCtl.wrd) ;
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_ChkCntl1_t chkrCtl ;
    chkrCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
    chkrCtl.bf.enable = (ctl == CS_ENABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkrCtl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_PPRBS);
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE CHECKER INTO HUNT STATE              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_resync(cs_uint16 port_id, arn5_pprbs_chan_t chan)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the Checker into the HUNT state .                        */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num, timeout = 10 ;
  cs_status     status = CS_OK ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_PPRBS) ;

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_ChkCntl0_t chkrCtl ;
    ARN5_PPRBS_ChkSync0_t chkrSync ;

    chkrCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd) ;
    chkrCtl.bf.resync = 1 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkrCtl.wrd) ;

    chkrSync.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync0.wrd) ;
    timeout = 10 ;
    while (timeout && (chkrSync.bf.sync)) {
      CS_UDELAY(1) ;
      timeout-- ;
      chkrSync.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync0.wrd) ;
    }
    if (!timeout) {
      CS_HNDL_ERROR(port_id, EARN5_PPRBS_RESYNC_FAILURE,
                     "Host channel Checker failed to enter HUNT State\n");
      status = CS_ERROR ;
    }

    /* release resync control */
    chkrCtl.bf.resync = 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl0.wrd, chkrCtl.wrd) ;
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    ARN5_PPRBS_ChkCntl1_t chkrCtl ;
    ARN5_PPRBS_ChkSync1_t chkrSync ;

    chkrCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd) ;
    chkrCtl.bf.resync = 1 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkrCtl.wrd) ;

    chkrSync.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync1.wrd) ;
    timeout = 10 ;
    while (timeout && (chkrSync.bf.sync)) {
      CS_UDELAY(1) ;
      timeout-- ;
      chkrSync.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync1.wrd) ;
    }
    if (!timeout) {
      CS_HNDL_ERROR(port_id, EARN5_PPRBS_RESYNC_FAILURE,
                "Line channel Checker failed to enter HUNT State\n");
      status = CS_ERROR ;
    }

    chkrCtl.bf.resync = 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].PPRBS.ChkCntl1.wrd, chkrCtl.wrd) ;
  }
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_PPRBS);
  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start  CHECK THE CHECKER SYNC STATUS                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_chk_in_sync(cs_uint16 port_id,
                        arn5_pprbs_chan_t chan, cs_boolean * pSyncStat)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/* OUTPUTS    : Sync stat (TRUE or FALSE)                       */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Checks if the PPRBS checker is in SYNC state.                */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN                                         */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  cs_boolean    inSync = FALSE ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if (chan == ARN5_PPRBS_HOST_CHAN) {
    ARN5_PPRBS_ChkSync0_t chkrSync ;
    chkrSync.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync0.wrd) ;
    inSync = chkrSync.bf.sync ;
  }
  else {
    ARN5_PPRBS_ChkSync1_t chkrSync ;
    chkrSync.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkSync1.wrd) ;
    inSync = chkrSync.bf.sync ;
  }

  if (pSyncStat != NULL) *pSyncStat = (inSync) ? TRUE : FALSE ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  RETURN BER STATUS                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_get_ber(cs_uint16 port_id,
                        arn5_pprbs_chan_t chan, cs_uint64 * pBer)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/* OUTPUTS    : BER (# of good bits/error bit)                  */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Calculates the BER based on the total numbers of errors      */
/* detected by the checker.                                     */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN                                         */
/* ARN5_PPRBS_LINE_CHAN channel refers to PPRBS traffic to/from */
/* line side.                                                   */
/* ARN5_PPRBS_HOST_CHAN channel refers to PPRBS traffic to/from */
/* SPI interface.                                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  cs_uint64     total_bits, ber = 0 ;

  ARN5_PPRBS_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PPRBS_IS_VALID_CHAN(port_id, chan, CS_ERROR);

  if (chan == ARN5_PPRBS_HOST_CHAN) {
    ARN5_PPRBS_ChkErr0_t total_bit_errs ;
    ARN5_PPRBS_ChkByteMSB0_t bytes_msb;
    ARN5_PPRBS_ChkByteLSB0_t bytes_lsb ;

    total_bit_errs.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkErr0.wrd) ;
    bytes_msb.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteMSB0.wrd) ;
    bytes_lsb.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteLSB0.wrd);

    total_bits = ((bytes_msb.wrd << 16 ) | (bytes_lsb.wrd)) * 8 ;
    if (total_bit_errs.wrd) {
      ber = (total_bits/total_bit_errs.wrd) ;
    }
  }
  else {
    ARN5_PPRBS_ChkErr1_t total_bit_errs ;
    ARN5_PPRBS_ChkByteMSB1_t bytes_msb;
    ARN5_PPRBS_ChkByteLSB1_t bytes_lsb ;

    total_bit_errs.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkErr1.wrd) ;
    bytes_msb.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteMSB1.wrd) ;
    bytes_lsb.wrd = CS_REG_READ(&pDev->slice[sl_num].PPRBS.ChkByteLSB1.wrd);

    total_bits = ((bytes_msb.wrd << 16 ) | (bytes_lsb.wrd)) * 8 ;
    if(total_bit_errs.wrd) {
      ber = total_bits/total_bit_errs.wrd ;
    }
  }

  if (pBer != NULL) *pBer = ber ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP PPRBS CHECKERS' STATUS                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : PPRBS                                           */
/* CHIP       : Arsenal5                                        */
cs_status arn5_pprbs_dump_checker(cs_uint16 port_id,
                        arn5_pprbs_chan_t chan)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id                                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Dump the status of PPRBS Checkers                            */
/*                                                              */
/* The [chan] parameter is specified as ARN5_PPRBS_HOST_CHAN or */
/* ARN5_PPRBS_LINE_CHAN or ARN5_PPRBS_CHAN_ALL                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint64     ber ;
  cs_boolean    inSync ;
  cs_status     status ;
  cs_uint32     uc, lc ;

  if ((chan == ARN5_PPRBS_HOST_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    if ((status = arn5_pprbs_chk_in_sync(port_id, ARN5_PPRBS_HOST_CHAN, &inSync)) != CS_OK) {
      return (CS_ERROR) ;
    }
    if ((status = arn5_pprbs_get_ber(port_id, ARN5_PPRBS_HOST_CHAN, &ber)) != CS_OK) {
      return (CS_ERROR) ;
    }
    CS_PRINT("Host Chan : %s In Sync;  Ber = 0x", (inSync ? "" : "Not")) ;
    uc = (cs_uint32)(ber >> 32) ;
    lc = (cs_uint32)(ber & 0xFFFFFFFF) ;
    if (uc > 0) {
      CS_PRINT("%8x%08x\n", uc, lc) ;
    }
    else {
      CS_PRINT("%016x\n", lc) ;
    }
  }

  if ((chan == ARN5_PPRBS_LINE_CHAN) || (chan == ARN5_PPRBS_CHAN_ALL)) {
    if ((status = arn5_pprbs_chk_in_sync(port_id, ARN5_PPRBS_LINE_CHAN, &inSync)) != CS_OK) {
      return (CS_ERROR) ;
    }
    if ((status = arn5_pprbs_get_ber(port_id, ARN5_PPRBS_LINE_CHAN, &ber)) != CS_OK) {
      return (CS_ERROR) ;
    }
    CS_PRINT("Line Chan : %s In Sync;  Ber = 0x", (inSync ? "" : "Not")) ;
    uc = (cs_uint32)(ber >> 32) ;
    lc = (cs_uint32)(ber & 0xFFFFFFFF) ;
    if (uc > 0) {
      CS_PRINT("%8x%08x\n", uc, lc) ;
    }
    else {
      CS_PRINT("%016x\n", lc) ;
    }
  }

  return (CS_OK) ;
}

