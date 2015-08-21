/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_spi42_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines and APIs for MILAN2 SPI4.2 Statistics.
 */
#ifndef __M2R_SPI42_STATS_H__
#define __M2R_SPI42_STATS_H__


/**********************************************
 *      UNIT STATS ENUM's (PER SECTION)       *
 **********************************************/
typedef enum {
  M2R_SPI42_STATS_SYNC_LOS  = 0,   /* RXLOSCNT,      TXLOSCNT     */
  M2R_SPI42_STATS_SYNC_DIP,	   /* DIP2ERRCNT,    DIP4ERRCNT   */
  /* all */
  M2R_SPI42_STATS_SYNC_SECT_ALL
} m2r_spi42_stats_sync_enum_t ;

typedef enum {
  M2R_SPI42_STATS_PROTO_MISSEOP = 0, /* ------,        TXMISSEOP    */
  M2R_SPI42_STATS_PROTO_MISSSOP,     /* ------,        TXMISSSOP    */
  M2R_SPI42_STATS_PROTO_MISSVLD,     /* ------,        TXMISSVLD    */
  M2R_SPI42_STATS_PROTO_SOPDISTERR,  /* ------,        TXSOPDISTERR */
  M2R_SPI42_STATS_PROTO_RSVCTRLWD,   /* ------,        TXRSVCTRLWD  */
  M2R_SPI42_STATS_PROTO_ADDRERR,     /* RXADDRERRCNT,  TXADDRERRCNT */
  /* all */
  M2R_SPI42_STATS_PROTO_SECT_ALL
} m2r_spi42_stats_proto_enum_t ;

typedef enum {
  M2R_SPI42_STATS_CHAN_FIFO_OVFLW = 0, /* RXFIFOOVFLWCNT0, TXFIFOOVFLWCNT0 */
  M2R_SPI42_STATS_CHAN_FIFO_PERR,      /* RXFIFOPERRR0,    ------          */
  M2R_SPI42_STATS_CHAN_NUM_SOP,	       /* RXCH0_SOP_MS,    TXCH0_SOP_MS    */
  M2R_SPI42_STATS_CHAN_NUM_EOP,	       /* RXCH0_EOP_MS,    TXCH0_EOP_MS    */
  M2R_SPI42_STATS_CHAN_NUM_ERR,	       /* RXCH0_ERR_MS,    TXCH0_ERR_MS    */
  M2R_SPI42_STATS_CHAN_NUM_BYTES,      /* RXCH0_BYTE_C2,   TXCH0_BYTE_C2   */
  /* all */
  M2R_SPI42_STATS_CHAN_SECT_ALL
} m2r_spi42_stats_chan_enum_t ;


/***************************************
 *   SECTION STATS DATASTRUCTURES      *
 ***************************************/
typedef struct {
  m2r_stats_data_t	sync[M2R_SPI42_STATS_SYNC_SECT_ALL] ;
} m2r_spi42_stats_sync_t ;

typedef struct {
  m2r_stats_data_t	proto[M2R_SPI42_STATS_PROTO_SECT_ALL] ;
} m2r_spi42_stats_proto_t ;

typedef struct {
  m2r_stats_data_t	chan0[M2R_SPI42_STATS_CHAN_SECT_ALL] ;
} m2r_spi42_stats_chan0_t ;

typedef struct {
  m2r_stats_data_t	chan1[M2R_SPI42_STATS_CHAN_SECT_ALL] ;
} m2r_spi42_stats_chan1_t ;

typedef struct {
  m2r_stats_data_t	chan2[M2R_SPI42_STATS_CHAN_SECT_ALL] ;
} m2r_spi42_stats_chan2_t ;

typedef struct {
  m2r_stats_data_t	chan3[M2R_SPI42_STATS_CHAN_SECT_ALL] ;
} m2r_spi42_stats_chan3_t ;


/***********************************************
 * Section Id enums for SPI4.2 block           *
 ***********************************************/
typedef enum {
  M2R_SPI42_STATS_SYNC_SECT	= 0,
  M2R_SPI42_STATS_PROTO_SECT,
  M2R_SPI42_STATS_CHAN0_SECT,
  M2R_SPI42_STATS_CHAN1_SECT,
  M2R_SPI42_STATS_CHAN2_SECT,
  M2R_SPI42_STATS_CHAN3_SECT,
  /* all */
  M2R_SPI42_STATS_SECT_ALL
} m2r_spi42_stats_sect_enum_t ;

/************************************************
 * STATS datastructure for SPI4.2               *
 ************************************************/
typedef struct {
  m2r_spi42_stats_sync_t       	sync ;
  m2r_spi42_stats_proto_t      	proto ;
  m2r_spi42_stats_chan0_t       chan0 ;
  m2r_spi42_stats_chan1_t      	chan1 ;
  m2r_spi42_stats_chan2_t       chan2 ;
  m2r_spi42_stats_chan3_t	chan3 ;
} m2r_spi42_stats_t ;









#if 0
typedef struct {
  m2r_stats_data_t	los ;          /* RXLOSCNT,      TXLOSCNT     */
  m2r_stats_data_t	dip ;          /* DIP2ERRCNT,    DIP4ERRCNT   */
} m2r_spi42_stats_sync_sect_t ;

typedef struct {
  m2r_stats_data_t      miss_eop ;     /* RXMISSEOP,     TXMISSEOP    */
  m2r_stats_data_t      miss_sop ;     /* RXMISSSOP,     TXMISSSOP    */
  m2r_stats_data_t      miss_vld ;     /* RXMISSVLD,     TXMISSVLD    */
  m2r_stats_data_t	dist_err ;     /* ------,        TXSOPDISTERR */
  m2r_stats_data_t      rsvd_ctl_wd;   /* ------,        TXRSVCTRLWD  */
  m2r_stats_data_t      addr_err ;     /* RXADDRERRCNT,  TXADDRERRCNT */
} m2r_spi42_stats_proto_sect_t ;

typedef struct {
  m2r_stats_data_t	fifo_ovf ; /* RXFIFOOVFLWCNT0, TXFIFOOVFLWCNT0 */
  m2r_stats_data_t	fifo_perr ;/* RXFIFOPERRR0,    ------          */
  m2r_stats_data_t	num_sop ;  /* RXCH0_SOP_MS,    TXCH0_SOP_MS    */
  m2r_stats_data_t	num_eop ;  /* RXCH0_EOP_MS,    TXCH0_EOP_MS    */
  m2r_stats_data_t      num_err ;  /* RXCH0_ERR_MS,    TXCH0_ERR_MS    */
  m2r_stats_data_t	num_bytes ;/* RXCH0_BYTE_C2,   TXCH0_BYTE_C2   */
} m2r_spi42_stats_chan0_sect_t ;


#endif


#endif /* __M2R_SPI42_STATS_H__ */



