/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_stats.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for SPI Statistics.
 */
#ifndef __ARN5_SPI_STATS_H__
#define __ARN5_SPI_STATS_H__


/*****************************************************************
 *             SPI DEVICE STATISTICS                             *
 * For counters in SPI block are shared by all the ports         *
 *****************************************************************/

/*********************************************************************
 *              DEVICE UNIT STATS ENUM's                             *
 *********************************************************************/

typedef enum {                          /*    Tx             Rx        */
  ARN5_SPI_DEV_STATS_SYNC_LOS  = 0,     /* txloscnt,      rxloscnt     */
  ARN5_SPI_DEV_STATS_DIP,               /* dip4errcnt,    dip2errcnt   */
  ARN5_SPI_DEV_STATS_BURST_SIZE_ERR,    /* txburstsizeerr,  ----       */
  ARN5_SPI_DEV_STATS_SOPDISTERR,        /* txsopdisterr,    ----       */
  ARN5_SPI_DEV_STATS_SEQERR,            /* txseqerr,        ----       */
  ARN5_SPI_DEV_STATS_RSVCTRLWD,         /* txrsvctrlwderr,  ----       */
  /* all */
  ARN5_SPI_DEV_STATS_ALL,
} arn5_spi_dev_stats_enum_t ;




/*********************************************************************
 *               SECTION-ID ENUM's FOR SPI BLOCK                     *
 *********************************************************************/
typedef enum {
  ARN5_SPI_DEV_STATS_SECT_GEN = 0,
  /*** ALL ****/
  ARN5_SPI_DEV_STATS_SECT_ALL,
} arn5_spi_dev_stats_sect_enum_t ;



/********************************************************************
 *              SPI DEVICE STATISTICS DATA-STRUCTURE                *
 ********************************************************************/
typedef struct {
  arn5_stats_data_t  general[ ARN5_SPI_DEV_STATS_ALL ] ;
} arn5_spi_dev_stats_t ;



/*****************************************************************
 *             SPI PORT STATISTICS                               *
 * These are statistics pertaining to configured logical         *
 * channels within each port. Sections id's are not static from  *
 * the code points of view but will be determined as soon as     *
 * the port is initialized.                                      *
 *****************************************************************/

/*********************************************************************
 *                PORT UNIT STATS ENUM's                             *
 *********************************************************************/

/*
 * HOST, LINE channel specific statistics
 */
typedef enum {                          /*     Tx,             Rx         */
  ARN5_SPI_STATS_CHAN_PKTS = 0,         /* tx pkt cnt,     rx pkt cnt     */
  ARN5_SPI_STATS_CHAN_BYTES,            /* tx byte cnt,    rx byte cnt    */
  ARN5_SPI_STATS_CHAN_ERR_PKTS,         /* tx err pkt cnt, rx_err pkt cnt */
  /* all */
  ARN5_SPI_STATS_CHAN_SECT_ALL,
} arn5_spi_stats_chan_enum_t ;



/*********************************************************************
 *                    SECTION STATS DATASTRUCTURES                   *
 *********************************************************************/
/*
 * SPI Chan Specific Statistics(up to 64 sections)
 */
typedef struct {
  arn5_stats_data_t      pkts ;
  arn5_stats_data_t      bytes ;
  arn5_stats_data_t      err_pkts ;
} arn5_spi_chan_stats_t ;


/********************************************************************
 *                SPI PORT STATISTICS DATA-STRUCTURE                *
 ********************************************************************/
typedef struct {
  arn5_spi_chan_stats_t    chan_stats ;
} arn5_spi_stats_t ;


#endif /* __ARN5_SPI_STATS_H__ */

