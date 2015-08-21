/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_stats.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_stats_priv.h"
#include "arn5_spi_stats.h"
#include "arn5_spi_priv.h"


#define ARN5_SPI_STATS_TX_ONLY   ( ARN5_STATS_FLAG_TX_VALID    | \
                                   ARN5_STATS_FLAG_TX_READ_CLR )

#define ARN5_SPI_STATS_RX_ONLY   ( ARN5_STATS_FLAG_RX_VALID    | \
                                   ARN5_STATS_FLAG_RX_READ_CLR )

#define ARN5_SPI_STATS_TX_N_RX   ( ARN5_SPI_STATS_TX_ONLY    | \
                                   ARN5_SPI_STATS_RX_ONLY )


/* Forward declarations */
static
void arn5_spi_stats_spl_update_unit_data(cs_uint16 port_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        cs_dir_t dir, cs_boolean sect_context) ;

static
void arn5_spi_stats_spl_clear_unit_data(cs_uint16 port_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        cs_dir_t dir, cs_boolean sect_context) ;

static
void arn5_spi_stats_spl_print_unit_data(cs_uint16 port_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        cs_dir_t dir, cs_boolean sect_context) ;


/*****************************************************************
 *             SPI DEVICE STATISTICS                             *
 * For counters in SPI block are shared by all the ports         *
 *****************************************************************/

/*******************************************
 *          SPI DEVICE STATS INFO          *
 *******************************************/
/*
 * Note: All fields may in the info table may not be used.
 */
const static arn5_stats_info_t
arn5_spi_dev_stats_tbl [ARN5_SPI_DEV_STATS_ALL] = {
  [ARN5_SPI_DEV_STATS_SYNC_LOS] = {
    "Loss of Sync",
    ARN5_SPI_REG_OFST(txloscnt),        /* Tx counter */
    ARN5_SPIA_REG_OFST(rxloscnt),       /* Rx counter */
    1,                                  /* 16-bits    */
    0,                                  /* stride     */
    (ARN5_SPI_STATS_TX_N_RX)            /* flag       */
  },

  [ARN5_SPI_DEV_STATS_DIP] = {
    "DIP Errors(DIP2, DIP4)",
    ARN5_SPI_REG_OFST(dip4errcnt),      /* Tx counter */
    ARN5_SPIA_REG_OFST(dip2errcnt),     /* Rx counter */
    1,                                  /* 16-bits    */
    0,                                  /* stride     */
    (ARN5_SPI_STATS_TX_N_RX)            /* flag       */
  },

  [ARN5_SPI_DEV_STATS_BURST_SIZE_ERR] = {
    "FIFO Burst Size Errors",
    ARN5_SPI_REG_OFST(txburstsizeerr),  /* Tx counter */
    0,                                  /* Rx counter */
    1,                                  /* 16-bits    */
    0,                                  /* stride     */
    (ARN5_SPI_STATS_TX_ONLY)            /* flag       */
  },

  [ARN5_SPI_DEV_STATS_SOPDISTERR] = {
    "SOP Distance Errors",
    ARN5_SPI_REG_OFST(txsopdisterr),    /* Tx counter */
    0,                                  /* Rx counter */
    1,                                  /* 16-bits    */
    0,                                  /* stride     */
    (ARN5_SPI_STATS_TX_ONLY)            /* flag       */
  },

  [ARN5_SPI_DEV_STATS_SEQERR] = {
    "Cntl Word Seq Errors",
    ARN5_SPI_REG_OFST(txseqerr),        /* Tx counter */
    0,                                  /* Rx counter */
    1,                                  /* 16-bits    */
    0,                                  /* stride     */
    (ARN5_SPI_STATS_TX_ONLY)            /* flag       */
  },

  [ARN5_SPI_DEV_STATS_RSVCTRLWD] = {
    "Rsrvd Cntl Word Errors",
    ARN5_SPI_REG_OFST(txrsvctrlwderr),  /* Tx counter */
    0,                                  /* Rx counter */
    1,                                  /* 16-bits    */
    0,                                  /* stride     */
    (ARN5_SPI_STATS_TX_ONLY)            /* flag       */
  },
} ;




/****************************************************************/
/* $_hdr_start   INIT DEVICE RELATED SPI STATS                  */
/* CATEGORY   : Device                                          */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_init_dev_stats(cs_uint8 dev_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initialize Device related SPI statistics. This API will      */
/* register the HOST_IF block and its stats                     */
/* control tables with the port stats.                          */
/* $_hdr_end.                                                   */
/****************************************************************/
{
  arn5_stats_blk_cb_t  * p_blk_cb ;
  arn5_stats_data_t    * p_data ;
  arn5_dev_stats_t     * p_dev_stats ;

  /* When we reach this point, all tables used are properly updated */
  /* i.e. device level init must ensure this                        */

  /***** Register HOST_IF block *****/
  p_blk_cb = arn5_stats_register_blk_cb(dev_id, ARN5_DEVICE,
                 ARN5_ID_HOST_IF,
                 TRUE,    /* ctl_enabled */
                 NULL,    /* pGet_func */
                 NULL,    /* pPrint func */
                 NULL) ;  /* pClear_func */
  if (p_blk_cb == NULL) {
    CS_PRINT("dev-%d: HOST_IF block NOT registered!\n", dev_id) ;
    return (CS_ERROR) ;
  }

  p_dev_stats = ARN5_DEV_GET_STATS_PTR(dev_id) ;
  if (p_dev_stats == NULL) {
    CS_PRINT("dev-%d p_dev_stats is NULL\n", dev_id) ;
    return (CS_ERROR) ;
  }

  p_data = p_dev_stats->host_if.general ;

  if (arn5_stats_add_sect_tbl_entry(dev_id, ARN5_DEVICE, ARN5_ID_HOST_IF,
                                    "SPI Host Stats General Section",
                                    ARN5_SPI_DEV_STATS_SECT_GEN,
                                    0, 0, TRUE,
                                    ARN5_SPI_DEV_STATS_ALL,
                                    p_data,
                                    arn5_spi_dev_stats_tbl)
       != CS_OK ) {
    CS_PRINT("Dev-%d ERROR: Cannot add section stats!\n", dev_id) ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}



/*****************************************************************
 *             SPI PORT STATISTICS                               *
 * These are statistics pertaining to configured logical         *
 * channels within each port. Sections id's are not static from  *
 * the code points of view but will be determined as soon as     *
 * the port is initialized.                                      *
 *****************************************************************/

/******* INDIVIDUAL SECTION STATS ***************/
/*
 * HOST_IF statistics
 */

/* use [physical chan number][sect] to index into this table */
/* all the rest entries are programmably initialized         */
static arn5_stats_info_t
arn5_spi_stats_chan_tbl [ARN5_SPI_STATS_CHAN_SECT_ALL] = {

  [ ARN5_SPI_STATS_CHAN_PKTS ] = {
    "Pkts",                             /* name        */
    ARN5_SPI_REG_OFST(txcore_pm_ctrl),  /* Tx counter  */
    ARN5_SPI_REG_OFST(rxcore_pm_ctrl),  /* Rx counter  */
    0,                                  /* 16-bit wrds, pm offset */
    0,                                  /* stride, phy chan */
    (ARN5_SPI_STATS_TX_N_RX      \
     | ARN5_STATS_FLAG_SPL_GET   \
     | ARN5_STATS_FLAG_SPL_PRINT \
     | ARN5_STATS_FLAG_SPL_CLEAR )      /* flag        */
  },

  [ ARN5_SPI_STATS_CHAN_BYTES ] = {
    "Bytes",                            /* name        */
    ARN5_SPI_REG_OFST(txcore_pm_ctrl),  /* Tx counter  */
    ARN5_SPI_REG_OFST(rxcore_pm_ctrl),  /* Rx counter  */
    0,                                  /* don't care  */
    0,                                  /* stride, phy chan */
    (ARN5_SPI_STATS_TX_N_RX      \
     | ARN5_STATS_FLAG_SPL_GET   \
     | ARN5_STATS_FLAG_SPL_PRINT \
     | ARN5_STATS_FLAG_SPL_CLEAR )      /* flag        */
  },

  [ ARN5_SPI_STATS_CHAN_ERR_PKTS] = {
    "Err Pkts",                         /* name        */
    ARN5_SPI_REG_OFST(txcore_pm_ctrl),  /* Tx counter  */
    ARN5_SPI_REG_OFST(rxcore_pm_ctrl),  /* Rx counter  */
    0,                                  /* don't care  */
    0,                                  /* stride, phy chan */
    (ARN5_SPI_STATS_TX_N_RX      \
     | ARN5_STATS_FLAG_SPL_GET   \
     | ARN5_STATS_FLAG_SPL_PRINT \
     | ARN5_STATS_FLAG_SPL_CLEAR )      /* flag        */
  },
} ;


/************************************************************************
 * External function declaration
 ***********************************************************************/



/***************************************************************
 *  Internal STATS special handler                             *
 ***************************************************************/
static
void arn5_spi_stats_spl_update_unit_data(cs_uint16 port_id,
                cs_uint16 sect_id,
                cs_uint16 unit_id,
                cs_dir_t dir, cs_boolean sect_context)
{
  /* This API is only called internally, before it is called, all
     the sanity checks have passed already */
  arn5_port_stats_cb_t      * stats_port_cb ;
  arn5_stats_blk_cb_t       * stats_blk_cb ;
  arn5_stats_sect_cb_t      * stats_sect_cb ;
  const arn5_stats_info_t   * p_unit_info ;
  arn5_stats_data_t         * pPkts, * pBytes, * pErrPkts ;
  ARN5_t                    * pDev ;
  cs_uint16                 slNum = 0 ;
  volatile cs_reg           * ctl_reg_addr, * data_reg_addr ;
  cs_uint16                 hwData ;
  cs_uint8                  devId ;

  /*
   * If we are doing consecutively for each counter in the section(ie. pkts,
   * bytes, errors) which is indicated by sect_context flag being TRUE,  then the
   * update needs to be done only for the first one(ie. pkts) and the subsequent
   * calls for pkts and errors can be treated as no-ops.
   *
   * Otherwise, doing an unit update(bytes or bytes or errors), then retrieve from
   * hardware and update all the 3 counters in the driver.
   */
  if ( (sect_context) && (unit_id != ARN5_SPI_STATS_CHAN_PKTS) ) {
    return ;
  }

  stats_port_cb = ARN5_PORT_GET_STATS_CB(port_id) ;
  stats_blk_cb = stats_port_cb->host_if ;
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;

  p_unit_info = stats_sect_cb->info_tbl + unit_id ;
  /* assumption, the three pointer are consecutive */
  switch (unit_id) {
    case ARN5_SPI_STATS_CHAN_PKTS :
      pPkts = stats_sect_cb->pStats + unit_id ;
      pBytes = pPkts + 1 ;
      pErrPkts = pBytes + 1 ;
      break;

    case ARN5_SPI_STATS_CHAN_BYTES :
      pBytes = stats_sect_cb->pStats + unit_id ;
      pPkts = pBytes - 1 ;
      pErrPkts = pBytes + 1 ;
      break;

    default :
      pErrPkts = stats_sect_cb->pStats + unit_id ;
      pBytes = pErrPkts - 1 ;
      pPkts = pBytes - 1 ;
      break;
  }

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  slNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;

  if ( (ARN5_STATS_UNIT_IS_RX_VALID(p_unit_info->flags)) &&
       ((dir == CS_RX) || (dir == CS_RX_AND_TX)) ) {
    ctl_reg_addr = (volatile cs_reg *)
      ARN5_STATS_GET_RX_REG_ADDR(pDev, 0, stats_sect_cb->stride_size,
                                p_unit_info) ;
    data_reg_addr = ctl_reg_addr + 7 ; /* pm_data0 = pm_ctrl + 7 */

    /* write to pm_ctrl and read all data registers */
    /* rx need to use logical chan due to design */
    hwData = arn5_physical2logical_chan_num(devId, CS_RX, slNum) ; /* read, log_chan */
    CS_REG_WRITE(ctl_reg_addr, hwData) ;

    hwData = CS_REG_READ(&data_reg_addr[-6]) ; /* pm_data6 */
    pErrPkts->rx += ((cs_uint64)hwData << 16) ;
    hwData = CS_REG_READ(&data_reg_addr[-5]) ; /* pm_data5 */
    pErrPkts->rx += (cs_uint64)hwData ;
    hwData = CS_REG_READ(&data_reg_addr[-4]) ;  /* pm_data4 */
    pPkts->rx += ((cs_uint64)hwData << 16) ;
    hwData = CS_REG_READ(&data_reg_addr[-3]) ;  /* pm_data3 */
    pPkts->rx += hwData ;
    hwData = CS_REG_READ(&data_reg_addr[-2]) ;  /* pm_data2 */
    pBytes->rx += ((cs_uint64)(hwData & 0xffff) << 32) ;
    hwData = CS_REG_READ(&data_reg_addr[-1]) ;  /* pm_data1 */
    pBytes->rx += ((cs_uint64)hwData << 16) ;
    hwData = CS_REG_READ(&data_reg_addr[0]) ;  /* pm_data0 */
    pBytes->rx += (cs_uint64)hwData ;
  }

  if ( (ARN5_STATS_UNIT_IS_TX_VALID(p_unit_info->flags)) &&
       ((dir == CS_TX) || (dir == CS_RX_AND_TX)) ) {
    ctl_reg_addr = (volatile cs_reg *)
      ARN5_STATS_GET_TX_REG_ADDR(pDev, 0, stats_sect_cb->stride_size,
                                p_unit_info) ;
    data_reg_addr = ctl_reg_addr + 7 ; /* pm_data0 = pm_ctrl + 7 */


    /* write to pm_ctrl */
    hwData = slNum ;    /* read, phy_chan */
    CS_REG_WRITE(ctl_reg_addr, hwData) ;

    hwData = CS_REG_READ(&data_reg_addr[-6]) ; /* pm_data6 */
    pErrPkts->tx += ((cs_uint64)hwData << 16) ;
    hwData = CS_REG_READ(&data_reg_addr[-5]) ; /* pm_data5 */
    pErrPkts->tx += (cs_uint64)hwData ;
    hwData = CS_REG_READ(&data_reg_addr[-4]) ;  /* pm_data4 */
    pPkts->tx += ((cs_uint64)hwData << 16) ;
    hwData = CS_REG_READ(&data_reg_addr[-3]) ;  /* pm_data3 */
    pPkts->tx += hwData ;
    hwData = CS_REG_READ(&data_reg_addr[-2]) ;  /* pm_data2 */
    pBytes->tx += ((cs_uint64)(hwData & 0xffff) << 32) ;
    hwData = CS_REG_READ(&data_reg_addr[-1]) ;  /* pm_data1 */
    pBytes->tx += ((cs_uint64)hwData << 16) ;
    hwData = CS_REG_READ(&data_reg_addr[0]) ;  /* pm_data0 */
    pBytes->tx += (cs_uint64)hwData ;
  }
}


static
void arn5_spi_stats_spl_clear_unit_data(cs_uint16 port_id,
                cs_uint16 sect_id,
                cs_uint16 unit_id,
                cs_dir_t  dir, cs_boolean sect_context)
{
  /* This API is only called internally, before it is called, all
     the sanity checks have passed already */
  arn5_port_stats_cb_t       * stats_port_cb ;
  arn5_stats_blk_cb_t        * stats_blk_cb ;
  arn5_stats_sect_cb_t       * stats_sect_cb ;
  arn5_stats_data_t          * pUnitData ;

  /*
   * The hw counters are cleared on read. No need to write to them,
   * eventhough they are RW.
   */
  arn5_spi_stats_spl_update_unit_data(port_id, sect_id, unit_id,
                                     dir, sect_context);
  stats_port_cb = ARN5_PORT_GET_STATS_CB(port_id) ;
  stats_blk_cb = stats_port_cb->host_if ;
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;

  pUnitData = stats_sect_cb->pStats + unit_id ;

  if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
    pUnitData->rx = 0 ;
  }

  if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
    pUnitData->tx = 0 ;
  }
}


static
void arn5_spi_stats_spl_print_unit_data(cs_uint16 port_id,
                cs_uint16 sect_id,
                cs_uint16 unit_id,
                cs_dir_t dir, cs_boolean sect_context)
{
  arn5_port_stats_cb_t      * stats_port_cb ;
  arn5_stats_blk_cb_t       * stats_blk_cb ;
  arn5_stats_sect_cb_t      * stats_sect_cb ;
  const arn5_stats_info_t   * p_info ;
  arn5_stats_data_t         * p_data ;
  arn5_dev_cb_t             * pDevCb ;
  cs_uint8                    phyChan, rxLogChan, txLogChan, devId ;
  cs_uint8                    name[] = "ch xx (log rx/tx xx/xx) Pkts     " ;


  /* First update the stats counters */
  arn5_spi_stats_spl_update_unit_data(port_id, sect_id,
                                     unit_id, dir, sect_context) ;

  stats_port_cb = ARN5_PORT_GET_STATS_CB(port_id) ;
  pDevCb = ARN5_PORT_ID_TO_DEVCB_PTR(port_id) ;

  stats_blk_cb = stats_port_cb->host_if ;
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;

  p_info = stats_sect_cb->info_tbl + unit_id ;
  p_data = stats_sect_cb->pStats + unit_id ;

  phyChan = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  rxLogChan = arn5_physical2logical_chan_num(devId, CS_RX, phyChan) ;
  txLogChan = arn5_physical2logical_chan_num(devId, CS_TX, phyChan) ;

  name[3] = (phyChan / 10) + 0x30 ;
  name[4] = (phyChan % 10) + 0x30 ;
  name[17] = (rxLogChan / 10) + 0x30 ;
  name[18] = (rxLogChan % 10) + 0x30 ;
  name[20] = (txLogChan / 10) + 0x30 ;
  name[21] = (txLogChan % 10) + 0x30 ;

  if (unit_id == ARN5_SPI_STATS_CHAN_BYTES ) {
    name[24] = 'B' ;
    name[25] = 'y' ;
    name[26] = 't' ;
    name[27] = 'e' ;
    name[28] = 's' ;
    name[29] = 0 ;
  }
  else if (unit_id == ARN5_SPI_STATS_CHAN_ERR_PKTS) {
    name[24] = 'E' ;
    name[25] = 'r' ;
    name[26] = 'r' ;
    name[27] = 'P' ;
    name[28] = 'k' ;
    name[29] = 't' ;
    name[30] = 's' ;
    name[31] = 0 ;
  }

  arn5_stats_print_unit_cntr(name, p_info->flags, p_data) ;
}


/****************************************************************/
/* $_hdr_start   INIT PORT RELATED SPI STATS                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : SPI                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_spi_init_port_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initialize port related SPI statistics. This API will        */
/* register the HOST_IF block and its stats                     */
/* control tables with the port stats.                          */
/* $_hdr_end.                                                   */
/****************************************************************/
{
  arn5_stats_blk_cb_t   * pBlkCb ;
  arn5_dev_cb_t         * pDevCb ;
  arn5_stats_data_t     * pData ;
  arn5_port_stats_t     * pPortStats ;
  cs_uint8              sl_num ;

  /* When we reach this point, all tables used are properly updated */
  /* i.e. device level init must ensure this                        */

  /* Register HOST_IF block */
  pBlkCb = arn5_stats_register_blk_cb(port_id, ARN5_PORT,
                 ARN5_ID_HOST_IF,
                 TRUE,    /* ctl_enabled */
                 arn5_spi_stats_spl_update_unit_data, /* pGet_func */
                 arn5_spi_stats_spl_print_unit_data,  /* pPrint func */
                 arn5_spi_stats_spl_clear_unit_data) ;/* pClear_func */
  if (pBlkCb == NULL) {
    CS_PRINT("port-%d: HOST_IF block NOT registered!\n", port_id) ;
    return (CS_ERROR) ;
  }

  /* register for all sections related to this port */

  /* host */
  pDevCb = ARN5_PORT_ID_TO_DEVCB_PTR(port_id) ;
  pPortStats = ARN5_PORT_GET_STATS_PTR(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;

  pData = &pPortStats->host_if.chan_stats.pkts ;
  if (arn5_stats_add_sect_tbl_entry(port_id, ARN5_PORT, ARN5_ID_HOST_IF,
                                    "SPI Host Channel Stats Section",
                                    sl_num,
                                    0, 0, TRUE,
                                    ARN5_SPI_STATS_CHAN_SECT_ALL,
                                    pData,
                                    arn5_spi_stats_chan_tbl)
       != CS_OK ) {
    CS_PRINT("Port-%d ERROR: Cannot add section stats!\n", port_id) ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

