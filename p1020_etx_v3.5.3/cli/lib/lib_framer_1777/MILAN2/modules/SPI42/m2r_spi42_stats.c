/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_spi42_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_spi42_stats.h"

/* Forward declarations */
static
cs_status m2r_spi42_register_sect_stats(cs_uint16 port_id,
					cs_uint16 sect_id,
					cs_boolean sect_enbl) ;

#define M2R_SPI42_STATS_TX_ONLY   ( M2R_STATS_FLAG_TX_VALID    | \
                                    M2R_STATS_FLAG_TX_READ_CLR )

#define M2R_SPI42_STATS_RX_ONLY	  ( M2R_STATS_FLAG_RX_VALID    | \
                                    M2R_STATS_FLAG_RX_READ_CLR )

#define M2R_SPI42_STATS_TX_N_RX   ( M2R_SPI42_STATS_TX_ONLY    | \
                                    M2R_SPI42_STATS_RX_ONLY )

/******* INDIVIDUAL SECTION STATS ***************/

/*************************************************
 * SPI4.2 SYNC STATS SECTION                     *
 *************************************************/
const static m2r_stats_info_t
m2r_spi42_stats_sync_tbl [M2R_SPI42_STATS_SYNC_SECT_ALL] = {
  CS_DI([M2R_SPI42_STATS_SYNC_LOS] =) {
    "Loss of Sync",
    M2R_SPI42_REG_OFST(TXLOSCNT),	/* Tx counter */
    M2R_SPI42_REG_OFST(RXLOSCNT), 	/* Rx counter */
    1,					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		/* flag       */
  },

  CS_DI([M2R_SPI42_STATS_SYNC_DIP] =) {
    "DIP Errors(DIP2, DIP4)",
    M2R_SPI42_REG_OFST(DIP4ERRCNT),	/* Tx counter */
    M2R_SPI42_REG_OFST(DIP2ERRCNT),	/* Rx counter */
    1, 					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		/* flag       */
  }
} ;


/*************************************************
 * SPI4.2 PROTOCOL VIOLATION ERROR STATS SECTION *
 *************************************************/
const static m2r_stats_info_t
m2r_spi42_stats_proto_tbl [M2R_SPI42_STATS_PROTO_SECT_ALL] = {
  CS_DI([M2R_SPI42_STATS_PROTO_MISSEOP] =) {
    "Missing EOP",
    M2R_SPI42_REG_OFST(TXMISSEOP),	/* Tx counter */
    0,	                                /* Rx counter */
    1, 					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_ONLY)           /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_PROTO_MISSSOP] =) {
    "Missing SOP",
    M2R_SPI42_REG_OFST(TXMISSSOP),	/* Tx counter */
    0,                                  /* Rx counter */
    1, 					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_ONLY)           /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_PROTO_MISSVLD] =) {
    "Missing VALID",
    M2R_SPI42_REG_OFST(TXMISSVLD),	/* Tx counter */
    0,                                  /* Rx counter */
    1, 					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_ONLY)           /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_PROTO_SOPDISTERR] =) {
    "SOP Distance Error",
    M2R_SPI42_REG_OFST(TXSOPDISTERR),	/* Tx counter */
    0,					/* Rx counter */
    1, 					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_ONLY)           /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_PROTO_RSVCTRLWD] =) {
    "Reserved CTL words",
    M2R_SPI42_REG_OFST(TXRSVCTRLWD),	/* Tx counter */
    0,					/* Rx counter */
    1, 					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_ONLY)           /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_PROTO_ADDRERR] =) {
    "Channel Addr Error",
    M2R_SPI42_REG_OFST(TXADDRERRCNT),	/* Tx counter */
    M2R_SPI42_REG_OFST(RXADDRERRCNT),	/* Rx counter */
    1, 					/* 16-bits    */
    0,					/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		/* flag       */
  }
} ;


/*************************************************
 * SPI4.2 CHANNEL-0 STATS SECTION                *
 *************************************************/
const static m2r_stats_info_t
m2r_spi42_stats_chan0_tbl [M2R_SPI42_STATS_CHAN_SECT_ALL] = {
  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_OVFLW] =) {
    "FIFO Overflow",
    M2R_SPI42_REG_OFST(TXFIFOOVFLWCNT0),	/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOOVFLWCNT0), 	/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_PERR] =) {
    "FIFO Parity Error",
    0,						/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOPERR0), 		/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_RX_ONLY)                   /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_SOP] =) {
    "SOP Count",
    M2R_SPI42_REG_OFST(TXCH0_SOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH0_SOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_EOP] =) {
    "EOP Count",
    M2R_SPI42_REG_OFST(TXCH0_EOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH0_EOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_ERR] =) {
    "Error Count",
    M2R_SPI42_REG_OFST(TXCH0_ERR_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH0_ERR_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_BYTES] =) {
    "Byte Count",
    M2R_SPI42_REG_OFST(TXCH0_BYTE_C2),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH0_BYTE_C2), 		/* Rx counter */
    3, 						/* 48-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  }
} ;


/*************************************************
 * SPI4.2 CHANNEL-1 STATS SECTION                *
 *************************************************/
const static m2r_stats_info_t
m2r_spi42_stats_chan1_tbl [M2R_SPI42_STATS_CHAN_SECT_ALL] = {
  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_OVFLW] =) {
    "FIFO Overflow",
    M2R_SPI42_REG_OFST(TXFIFOOVFLWCNT1),	/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOOVFLWCNT1), 	/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_PERR] =) {
    "FIFO Parity Error",
    0,						/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOPERR1), 		/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_RX_ONLY)                   /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_SOP] =) {
    "SOP Count",
    M2R_SPI42_REG_OFST(TXCH1_SOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH1_SOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_EOP] =) {
    "EOP Count",
    M2R_SPI42_REG_OFST(TXCH1_EOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH1_EOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_ERR] =) {
    "Error Count",
    M2R_SPI42_REG_OFST(TXCH1_ERR_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH1_ERR_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_BYTES] =) {
    "Byte Count",
    M2R_SPI42_REG_OFST(TXCH1_BYTE_C2),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH1_BYTE_C2), 		/* Rx counter */
    3, 						/* 48-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  }
} ;


/*************************************************
 * SPI4.2 CHANNEL-2 STATS SECTION                *
 *************************************************/
const static m2r_stats_info_t
m2r_spi42_stats_chan2_tbl [M2R_SPI42_STATS_CHAN_SECT_ALL] = {
  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_OVFLW] =) {
    "FIFO Overflow",
    M2R_SPI42_REG_OFST(TXFIFOOVFLWCNT2),	/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOOVFLWCNT2), 	/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_PERR] =) {
    "FIFO Parity Error",
    0,						/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOPERR2), 		/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_RX_ONLY)                   /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_SOP] =) {
    "SOP Count",
    M2R_SPI42_REG_OFST(TXCH2_SOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH2_SOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_EOP] =) {
    "EOP Count",
    M2R_SPI42_REG_OFST(TXCH2_EOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH2_EOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_ERR] =) {
    "Error Count",
    M2R_SPI42_REG_OFST(TXCH2_ERR_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH2_ERR_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_BYTES] =) {
    "Byte Count",
    M2R_SPI42_REG_OFST(TXCH2_BYTE_C2),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH2_BYTE_C2), 		/* Rx counter */
    3, 						/* 48-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  }
} ;

/*************************************************
 * SPI4.2 CHANNEL-3 STATS SECTION                *
 *************************************************/
const static m2r_stats_info_t
m2r_spi42_stats_chan3_tbl [M2R_SPI42_STATS_CHAN_SECT_ALL] = {
  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_OVFLW] =) {
    "FIFO Overflow",
    M2R_SPI42_REG_OFST(TXFIFOOVFLWCNT3),	/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOOVFLWCNT3), 	/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)   	        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_FIFO_PERR] =) {
    "FIFO Parity Error",
    0,						/* Tx counter */
    M2R_SPI42_REG_OFST(RXFIFOPERR3), 		/* Rx counter */
    1, 						/* 16-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_RX_ONLY)                   /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_SOP] =) {
    "SOP Count",
    M2R_SPI42_REG_OFST(TXCH3_SOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH3_SOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_EOP] =) {
    "EOP Count",
    M2R_SPI42_REG_OFST(TXCH3_EOP_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH3_EOP_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_ERR] =) {
    "Error Count",
    M2R_SPI42_REG_OFST(TXCH3_ERR_MS),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH3_ERR_MS), 		/* Rx counter */
    2, 						/* 32-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  },

  CS_DI([M2R_SPI42_STATS_CHAN_NUM_BYTES] =) {
    "Byte Count",
    M2R_SPI42_REG_OFST(TXCH3_BYTE_C2),		/* Tx counter */
    M2R_SPI42_REG_OFST(RXCH3_BYTE_C2), 		/* Rx counter */
    3, 						/* 48-bits    */
    0,						/* stride     */
    (M2R_SPI42_STATS_TX_N_RX)		        /* flag       */
  }
} ;


/************ SECTION TABLE **********************/
/*
 * This section table is used in setting up the various
 * Section CB's for the SPI4.2 block.
 */
const static m2r_stats_sect_tbl_entry_t
spi42_stats_sect_tbl[M2R_SPI42_STATS_SECT_ALL] = {
    CS_DI([M2R_SPI42_STATS_SYNC_SECT] =) {
      "SPI4.2 SYNC Section",                    /* name        */
      M2R_OFST(m2r_spi42_stats_t, sync),        /* sect_offset */
      0,                                        /* stride_id   */
      m2r_spi42_stats_sync_tbl,                 /* info_tbl    */
      M2R_SPI42_STATS_SYNC_SECT_ALL },          /* num_unit    */

    CS_DI([M2R_SPI42_STATS_PROTO_SECT] =) {
      "SPI4.2 PROTOCOL Section",
      M2R_OFST(m2r_spi42_stats_t, proto),
      0,
      m2r_spi42_stats_proto_tbl,
      M2R_SPI42_STATS_PROTO_SECT_ALL },

    CS_DI([M2R_SPI42_STATS_CHAN0_SECT] =) {
      "SPI4.2 CHANNEL-0 Section",
      M2R_OFST(m2r_spi42_stats_t, chan0),
      0,
      m2r_spi42_stats_chan0_tbl,
      M2R_SPI42_STATS_CHAN_SECT_ALL },

    CS_DI([M2R_SPI42_STATS_CHAN1_SECT] =) {
      "SPI4.2 CHANNEL-1 Section",
      M2R_OFST(m2r_spi42_stats_t, chan1),
      0,
      m2r_spi42_stats_chan1_tbl,
      M2R_SPI42_STATS_CHAN_SECT_ALL },

    CS_DI([M2R_SPI42_STATS_CHAN2_SECT] =) {
      "SPI4.2 CHANNEL-2 Section",
      M2R_OFST(m2r_spi42_stats_t, chan2),
      0,
      m2r_spi42_stats_chan2_tbl,
      M2R_SPI42_STATS_CHAN_SECT_ALL },

    CS_DI([M2R_SPI42_STATS_CHAN3_SECT] =) {
      "SPI4.2 CHANNEL-3 Section",
      M2R_OFST(m2r_spi42_stats_t, chan3),
      0,
      m2r_spi42_stats_chan3_tbl,
      M2R_SPI42_STATS_CHAN_SECT_ALL }
} ;



/***************************************************************
 *  Utility to register and add the section CB to the block CB *
 ***************************************************************/
static
cs_status m2r_spi42_register_sect_stats(cs_uint16 port_id,
					cs_uint16 sect_id,
					cs_boolean sect_enbl)
{
  m2r_stats_data_t   * p_data = NULL ;
  m2r_port_stats_t   * p_port_stats = NULL ;
  m2r_spi42_stats_t  * p_spi42_stats = NULL ;
  const m2r_stats_sect_tbl_entry_t * tbl = NULL ;

  p_port_stats = M2R_GET_STATS_PTR(port_id) ;
  if (p_port_stats == NULL) {
    CS_PRINT("port-%d p_port_stats is NULL\n", port_id) ;
    return (CS_ERROR) ;
  }
  p_spi42_stats = &p_port_stats->spi42 ;

  tbl = &spi42_stats_sect_tbl[sect_id] ;
  if ( tbl->sect_offset >= sizeof(m2r_spi42_stats_t) ) {
    CS_PRINT("%s() SW ERROR- offset(0x%x) s/b < table-size(0x%x)\n",
             __FUNCTION__,
             tbl->sect_offset, sizeof(m2r_spi42_stats_t) ) ;
    return (CS_ERROR) ;
  }

  p_data = (m2r_stats_data_t *)
                (((cs_uint64)p_spi42_stats) + tbl->sect_offset) ;

  if ( m2r_stats_add_sect_tbl_entry(port_id, M2R_ID_SPI42,
				    tbl->sect_name, sect_id,
				    tbl->stride_id, sect_enbl,
				    tbl->num_unit,
				    p_data, tbl->info_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }


  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   INIT SPI4.2 STATS                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : PRIVATE                                         */
/* BLOCK      : SPI42                                           */
/* CHIP       : MILAN2                                          */
cs_status  m2r_spi42_init_stats(cs_uint16 port_id,
				cs_uint8 num_channels)
/* INPUTS     : o Port Id                                       */
/*              o Number of channels (1...4)                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Initialize SPI4.2 statistics. This API will register the     */
/* SPI4.2 block and its stats control tables with the port      */
/* stats.                                                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_stats_blk_cb_t 	* p_blk_cb ;
  cs_boolean		enbl ;

  /* Register SPI4.2 block */
  p_blk_cb = m2r_stats_register_blk_cb(port_id,
				       M2R_ID_SPI42,
				       TRUE,	/* ctl_enabled */
				       NULL,    /* pGet_func   */
				       NULL,  	/* pPrint_func */
				       NULL) ;	/* pClear_func */
  if (p_blk_cb == NULL) {
    CS_PRINT("port-%d: SPI4.2 block NOT registered!\n", port_id) ;
    return (CS_ERROR) ;
  }

  /* Register all sections, but enable only the appropriate ones */
  if ( m2r_spi42_register_sect_stats(port_id,
				     M2R_SPI42_STATS_SYNC_SECT,
				     TRUE)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_register_sect_stats(port_id,
				     M2R_SPI42_STATS_PROTO_SECT,
				     TRUE)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_register_sect_stats(port_id,
				     M2R_SPI42_STATS_CHAN0_SECT,
				     TRUE)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  enbl = (num_channels > 1) ? TRUE : FALSE ;
  if ( m2r_spi42_register_sect_stats(port_id,
				     M2R_SPI42_STATS_CHAN1_SECT,
				     enbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  enbl = (num_channels > 2) ? TRUE : FALSE ;
  if ( m2r_spi42_register_sect_stats(port_id,
				     M2R_SPI42_STATS_CHAN2_SECT,
				     enbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  enbl = (num_channels > 3) ? TRUE : FALSE ;
  if ( m2r_spi42_register_sect_stats(port_id,
				     M2R_SPI42_STATS_CHAN3_SECT,
				     enbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

