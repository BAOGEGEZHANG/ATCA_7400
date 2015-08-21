
/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_irq.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_gen_data.h"
#include "arn5_spi_data.h"
#include "arn5_spi_priv.h"
#include "arn5_irq_priv.h"



/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
static const arn5_irq_ctl_t arn5_spi_irq_tbl[ARN5_SPI_MAX_GRP] = {
  /* Common */
  [ARN5_GRP_SPI4P2_RXIF_intrpt] = {
    ARN5_SPI_REG_OFST(rxif_intrpt),
    0,
    ARN5_SPI_REG_OFST(rxif_intrpten),
    ARN5_GRP_SPI4P2_RXIF_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2_TXIF_txintrpt] = {
    ARN5_SPI_REG_OFST(txintrpt),
    0,
    ARN5_SPI_REG_OFST(txintrpten),
    ARN5_GRP_SPI4P2_TXIF_txintrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  /* Host */
  [ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt] = {
    ARN5_SPIA_REG_OFST(afifo_intrpt),
    0,
    ARN5_SPIA_REG_OFST(afifo_intrpten),
    ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt] = {
    ARN5_SPIA_REG_OFST(misseop_intrpt),
    0,
    ARN5_SPIA_REG_OFST(misseop_intrpten),
    ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt] = {
    ARN5_SPIA_REG_OFST(misssop_intrpt),
    0,
    ARN5_SPIA_REG_OFST(misssop_intrpten),
    ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_intrpt] = {
    ARN5_SPIA_REG_OFST(txint_intrpt),
    0,
    ARN5_SPIA_REG_OFST(txint_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt] = {
    ARN5_SPIA_REG_OFST(txint_fifo_ovrflw_intrpt),
    0,
    ARN5_SPIA_REG_OFST(txint_fifo_ovrflw_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt] = {
    ARN5_SPIA_REG_OFST(fifo_abort_pkt_intrpt),
    0,
    ARN5_SPIA_REG_OFST(fifo_abort_pkt_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt] = {
    ARN5_SPIA_REG_OFST(fifo_drop_pkt_intrpt),
    0,
    ARN5_SPIA_REG_OFST(fifo_drop_pkt_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt] = {
    ARN5_SPIA_REG_OFST(txint_fifo_eccerr_intrpt),
    0,
    ARN5_SPIA_REG_OFST(txint_fifo_eccerr_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt] = {
    ARN5_SPIA_REG_OFST(txint_fifo_eccfix_intrpt),
    0,
    ARN5_SPIA_REG_OFST(txint_fifo_eccfix_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_perr_intrpt] = {
    ARN5_SPIA_REG_OFST(txint_perr_intrpt),
    0,
    ARN5_SPIA_REG_OFST(txint_perr_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_perr_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt] = {
    ARN5_SPIA_REG_OFST(sop_eop_err_intrpt),
    0,
    ARN5_SPIA_REG_OFST(sop_eop_err_intrpten),
    ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_RXINT_rxintrpt] = {
    ARN5_SPIA_REG_OFST(rxintrpt),
    0,
    ARN5_SPIA_REG_OFST(rxintrpten),
    ARN5_GRP_SPI4P2A_RXINT_rxintrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt] = {
    ARN5_SPIA_REG_OFST(rxint_fifo_ovrflw_intrpt),
    0,
    ARN5_SPIA_REG_OFST(rxint_fifo_ovrflw_intrpten),
    ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt] = {
    ARN5_SPIA_REG_OFST(rxint_fifo_eccerr_intrpt),
    0,
    ARN5_SPIA_REG_OFST(rxint_fifo_eccerr_intrpten),
    ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt] = {
    ARN5_SPIA_REG_OFST(rxint_fifo_eccfix_intrpt),
    0,
    ARN5_SPIA_REG_OFST(rxint_fifo_eccfix_intrpten),
    ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_RXINT_perr_intrpt] = {
    ARN5_SPIA_REG_OFST(rxint_perr_intrpt),
    0,
    ARN5_SPIA_REG_OFST(rxint_perr_intrpten),
    ARN5_GRP_SPI4P2A_RXINT_perr_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_GRP_SPI4P2A_MEMBIST_intrpt] = {
    ARN5_SPIA_REG_OFST(membist_intrpt),
    0,
    ARN5_SPIA_REG_OFST(membist_intrpten),
    ARN5_GRP_SPI4P2A_MEMBIST_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
} ;

static const arn5_irq_ctl_t arn5_spi_imed_irq_tbl[ARN5_SPI_MAX_IMED] = {
  /* Core */
  [ARN5_IMED_SPI4P2_CORE_intrpt] = {
    ARN5_SPI_REG_OFST(core_intrpt),
    0,
    ARN5_SPI_REG_OFST(core_intrpten),
    ARN5_IMED_SPI4P2_CORE_intrpt_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  /* Host */
  [ARN5_IMED_SPI4P2A_RX_RPRIF_intrpt_vec] = {
    ARN5_SPIA_REG_OFST(rx_rprif_intrpt_vec),
    0,
    ARN5_SPIA_REG_OFST(rx_rprif_intrpt_vec_en),
    ARN5_IMED_SPI4P2A_RX_RPRIF_intrpt_vec_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_IMED_SPI4P2A_TXINT_intrpt_vec] = {
    ARN5_SPIA_REG_OFST(txint_intrpt_vec),
    0,
    ARN5_SPIA_REG_OFST(txint_intrpt_vec_en),
    ARN5_IMED_SPI4P2A_TXINT_intrpt_vec_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
  [ARN5_IMED_SPI4P2A_RXINT_intrpt_vec] = {
    ARN5_SPIA_REG_OFST(rxint_intrpt_vec),
    0,
    ARN5_SPIA_REG_OFST(rxint_intrpt_vec_en),
    ARN5_IMED_SPI4P2A_RXINT_intrpt_vec_ALL,
    0,
    ARN5_SPI_IRQ_FLAGS,
    0, 0 },
} ;

/**************** IRQ ID NAME TABLES **********************/
static const cs_uint8 * arn5_spi_irq_grp_name_tbl[ARN5_SPI_MAX_GRP] =
{
  [ARN5_GRP_SPI4P2_RXIF_intrpt]                  = "RXIF_intrpt",
  [ARN5_GRP_SPI4P2_TXIF_txintrpt]                = "TXIF_txintrpt",
  [ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt]       = "TX_RPRIF_afifo_intrpt",
  [ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt]     = "RX_RPRIF_misseop_intrpt",
  [ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt]     = "RX_RPRIF_misssop_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_intrpt]                = "TXINT_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt]    = "TXINT_fifo_ovrflw_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt] = "TXINT_fifo_abort_pkt_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt]  = "TXINT_fifo_drop_pkt_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt]    = "TXINT_fifo_eccerr_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt]    = "TXINT_fifo_eccfix_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_perr_intrpt]           = "TXINT_perr_intrpt",
  [ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt]    = "TXINT_sop_eop_err_intrpt",
  [ARN5_GRP_SPI4P2A_RXINT_rxintrpt]              = "RXINT_rxintrpt",
  [ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt]    = "RXINT_fifo_ovrflw_intrpt",
  [ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt]    = "RXINT_fifo_eccerr_intrpt",
  [ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt]    = "RXINT_fifo_eccfix_intrpt",
  [ARN5_GRP_SPI4P2A_RXINT_perr_intrpt]           = "RXINT_perr_intrpt",
  [ARN5_GRP_SPI4P2A_MEMBIST_intrpt]              = "MEMBIST_intrpt",
} ;

static const cs_uint8 * arn5_spi_irq_imed_name_tbl[ARN5_SPI_MAX_IMED] =
{
  [ARN5_IMED_SPI4P2_CORE_intrpt]                 = "CORE_intrpt",
  [ARN5_IMED_SPI4P2A_RX_RPRIF_intrpt_vec]        = "RX_RPRIF_intrpt_vec",
  [ARN5_IMED_SPI4P2A_TXINT_intrpt_vec]           = "TXINT_intrpt_vec",
  [ARN5_IMED_SPI4P2A_RXINT_intrpt_vec]           = "RXINT_intrpt_vec",
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2_CORE_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2_CORE_intrpt_rx_rprif_intrpt_vec,   "rx_rprif_intrpt_vec"},
  {ARN5_SPI4P2_CORE_intrpt_rxint_intrpt_vec,      "rxint_intrpt_vec"},
  {ARN5_SPI4P2_CORE_intrpt_rxint_rxintrpt,        "rxint_rxintrpt"},
  {ARN5_SPI4P2_CORE_intrpt_membist_intrpt,        "membist_intrpt"},
  {ARN5_SPI4P2_CORE_intrpt_tx_rprif_afifo_intrpt, "tx_rprif_afifo_intrpt"},
  {ARN5_SPI4P2_CORE_intrpt_txint_intrpt_vec,      "txint_intrpt_vec"},
  {ARN5_SPI4P2_CORE_intrpt_txif_txintrpt,         "txif_txintrpt"},
  {ARN5_SPI4P2_CORE_intrpt_rxif_intrpt,           "rxif_intrpt"},
} ;

/* Common */
static const arn5_irq_id_name_t
ARN5_SPI4P2_RXIF_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2_RXIF_intrpt_Rxspi42_perr,                 "Rxspi42_perr"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2_TXIF_txintrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2_TXIF_txintrpt_txbuf_fifo_ovrflw,          "txbuf_fifo_ovrflw"},
  {ARN5_SPI4P2_TXIF_txintrpt_txspi3_sx_err,              "txspi3_sx_err"},
  {ARN5_SPI4P2_TXIF_txintrpt_txspi3_adr_err,             "txspi3_adr_err"},
  {ARN5_SPI4P2_TXIF_txintrpt_txspi3_perr,                "txspi3_perr"},
  {ARN5_SPI4P2_TXIF_txintrpt_txrsvctrlwderr,             "txrsvctrlwderr"},
  {ARN5_SPI4P2_TXIF_txintrpt_txseqerr,                   "txseqerr"},
  {ARN5_SPI4P2_TXIF_txintrpt_txsopdisterr,               "txsopdisterr"},
  {ARN5_SPI4P2_TXIF_txintrpt_txburstsizeerr,             "txburstsizeerr"},
  {ARN5_SPI4P2_TXIF_txintrpt_dip4err,                    "dip4err"},
  {ARN5_SPI4P2_TXIF_txintrpt_txlosyn,                    "txlosyn"},
} ;

/* Host */
static const arn5_irq_id_name_t
ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_afifo1_perr,       "afifo1_perr"},
  {ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_afifo0_perr,       "afifo0_perr"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_7,          "misseop_chan_7"},
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_6,          "misseop_chan_6"},
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_5,          "misseop_chan_5"},
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_4,          "misseop_chan_4"},
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_3,          "misseop_chan_3"},
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_2,          "misseop_chan_2"},
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_1,          "misseop_chan_1"},
  {ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_0,          "misseop_chan_0"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_7,          "misssop_chan_7"},
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_6,          "misssop_chan_6"},
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_5,          "misssop_chan_5"},
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_4,          "misssop_chan_4"},
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_3,          "misssop_chan_3"},
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_2,          "misssop_chan_2"},
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_1,          "misssop_chan_1"},
  {ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_0,          "misssop_chan_0"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_intrpt_q3_fifo_ovf,                "q3_fifo_ovf"},
  {ARN5_SPI4P2A_TXINT_intrpt_q3_fifo_und,                "q3_fifo_und"},
  {ARN5_SPI4P2A_TXINT_intrpt_addrerr,                    "addrerr"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_7,         "fifo_ovrflw_chan_7"},
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_6,         "fifo_ovrflw_chan_6"},
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_5,         "fifo_ovrflw_chan_5"},
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_4,         "fifo_ovrflw_chan_4"},
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_3,         "fifo_ovrflw_chan_3"},
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_2,         "fifo_ovrflw_chan_2"},
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_1,         "fifo_ovrflw_chan_1"},
  {ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_0,         "fifo_ovrflw_chan_0"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_7,      "fifo_abort_pkt_chan_7"},
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_6,      "fifo_abort_pkt_chan_6"},
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_5,      "fifo_abort_pkt_chan_5"},
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_4,      "fifo_abort_pkt_chan_4"},
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_3,      "fifo_abort_pkt_chan_3"},
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_2,      "fifo_abort_pkt_chan_2"},
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_1,      "fifo_abort_pkt_chan_1"},
  {ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_0,      "fifo_abort_pkt_chan_0"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_7,       "fifo_drop_pkt_chan_7"},
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_6,       "fifo_drop_pkt_chan_6"},
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_5,       "fifo_drop_pkt_chan_5"},
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_4,       "fifo_drop_pkt_chan_4"},
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_3,       "fifo_drop_pkt_chan_3"},
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_2,       "fifo_drop_pkt_chan_2"},
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_1,       "fifo_drop_pkt_chan_1"},
  {ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_0,       "fifo_drop_pkt_chan_0"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_txbuf_err,      "txbuf_err"},
  {ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_txmem_err,      "txmem_err"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_txbuf_fix,      "txbuf_fix"},
  {ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_txmem_fix,      "txmem_fix"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_perr_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_perr_intrpt_pm_perr,               "pm_perr"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_0,        "sop_eop_err_1_chan_00"},
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_1,        "sop_eop_err_1_chan_01"},
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_2,        "sop_eop_err_1_chan_02"},
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_3,        "sop_eop_err_1_chan_03"},
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_4,        "sop_eop_err_1_chan_04"},
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_5,        "sop_eop_err_1_chan_05"},
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_6,        "sop_eop_err_1_chan_06"},
  {ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_7,        "sop_eop_err_1_chan_07"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RXINT_rxintrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RXINT_rxintrpt_perr,                     "perr"},
  {ARN5_SPI4P2A_RXINT_rxintrpt_dip2err,                  "dip2err"},
  {ARN5_SPI4P2A_RXINT_rxintrpt_rxlosyn,                  "rxlosyn"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_pf2_overrun_error,      "pf2_overrun_error"},
  {ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_pf1_overrun,    "pf1_overrun"},
  {ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_pf0_overrun,    "pf0_overrun"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_pf2_err,        "pf2_err"},
  {ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_pf1_err,        "pf1_err"},
  {ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_pf0_err,        "pf0_err"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_pf2_fix,        "pf2_fix"},
  {ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_pf1_fix,        "pf1_fix"},
  {ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_pf0_fix,        "pf0_fix"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RXINT_perr_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RXINT_perr_intrpt_pm_perr,               "pm_perr"},
  {ARN5_SPI4P2A_RXINT_perr_intrpt_bb3_perr,              "bb3_perr"},
  {ARN5_SPI4P2A_RXINT_perr_intrpt_bb2_perr,              "bb2_perr"},
  {ARN5_SPI4P2A_RXINT_perr_intrpt_bb1_perr,              "bb1_perr"},
  {ARN5_SPI4P2A_RXINT_perr_intrpt_bb0_perr,              "bb0_perr"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_MEMBIST_intrpt_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_MEMBIST_intrpt_sb_bist_int,              "sb_bist_int"},
  {ARN5_SPI4P2A_MEMBIST_intrpt_mb_bist_int,              "mb_bist_int"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_spi4p1_intrpt,       "spi4p1_intrpt"},
  {ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_misssop_intrpt,      "misssop_intrpt"},
  {ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_misseop_intrpt,      "misseop_intrpt"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_TXINT_intrpt_vec_irq_id_name_tbl[] = {

  {ARN5_SPI4P2A_TXINT_intrpt_vec_sop_eop_err_intrpt,     "sop_eop_err_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_pkt_timeout_intrpt,     "pkt_timeout_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_pkt_eaten_intrpt,       "pkt_eaten_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_perr_intrpt,            "perr_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_eccfix_intrpt,     "fifo_eccfix_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_eccerr_intrpt,     "fifo_eccerr_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_drop_pkt_intrpt,   "fifo_drop_pkt_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_abort_pkt_intrpt,  "fifo_abort_pkt_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_ovrflw_intrpt,     "fifo_ovrflw_intrpt"},
  {ARN5_SPI4P2A_TXINT_intrpt_vec_intrpt,                 "intrpt"},
} ;

static const arn5_irq_id_name_t
ARN5_SPI4P2A_RXINT_intrpt_vec_irq_id_name_tbl[] = {
  {ARN5_SPI4P2A_RXINT_intrpt_vec_perr_intrpt,            "perr_intrpt"},
  {ARN5_SPI4P2A_RXINT_intrpt_vec_fifo_eccfix_intrpt,     "fifo_eccfix_intrpt"},
  {ARN5_SPI4P2A_RXINT_intrpt_vec_fifo_eccerr_intrpt,     "fifo_eccerr_intrpt"},
  {ARN5_SPI4P2A_RXINT_intrpt_vec_fifo_ovrflw_intrpt,     "fifo_ovrflw_intrpt"},
} ;

/************************************************************************
 * GET DEFAULT SPI  IRQ CONFIGURATION
 ***********************************************************************/
cs_status arn5_spi_dev_get_default_irq_cfg(cs_uint8 dev_id,
                        arn5_dev_summ_t * pSumm,
                        arn5_spi_dev_cfg_t * pCfg)
{
  cs_uint16 ii ;

  if (ARN5_OVRD_DEF_IRQ_TO_ENB) {
    for (ii = 0; ii < ARN5_SPI_MAX_GRP; ii++) {
      pCfg->irq_ctl_tbl[ii] = arn5_spi_irq_tbl[ii].irq_map;
    }
  }
  else {
    /* default irq cfg */

    /* index 0 (ARN5_GRP_SPI4P2_TXIF_txintrpt) has more SPI42/3 specifics */
    for (ii = 1; ii < ARN5_SPI_MAX_GRP; ii++) {
      pCfg->irq_ctl_tbl[ii] = arn5_spi_irq_tbl[ii].irq_map;
    }

    switch (pSumm->host_if) {
      default :
      case ARN5_HOST_IF_SPI42 :
        pCfg->irq_ctl_tbl[ARN5_GRP_SPI4P2_TXIF_txintrpt] =
          ARN5_SPI4P2_TXIF_txintrpt_txbuf_fifo_ovrflw |
          ARN5_SPI4P2_TXIF_txintrpt_txrsvctrlwderr    |
          ARN5_SPI4P2_TXIF_txintrpt_txseqerr          |
          ARN5_SPI4P2_TXIF_txintrpt_txsopdisterr      |
          ARN5_SPI4P2_TXIF_txintrpt_txburstsizeerr    |
          ARN5_SPI4P2_TXIF_txintrpt_dip4err           |
          ARN5_SPI4P2_TXIF_txintrpt_txlosyn ;
        break ;

      case ARN5_HOST_IF_SPI3:
        pCfg->irq_ctl_tbl[ARN5_GRP_SPI4P2_TXIF_txintrpt] =
          ARN5_SPI4P2_TXIF_txintrpt_txbuf_fifo_ovrflw |
          ARN5_SPI4P2_TXIF_txintrpt_txspi3_sx_err     |
          ARN5_SPI4P2_TXIF_txintrpt_txspi3_adr_err    |
          ARN5_SPI4P2_TXIF_txintrpt_txspi3_perr ;

        pCfg->irq_ctl_tbl[ARN5_GRP_SPI4P2A_RXINT_rxintrpt] =
          ARN5_SPI4P2A_RXINT_rxintrpt_perr ;
        break ;
    }
  }
  return (CS_OK) ;
}

/************************************************************************
 * INIT SPI IRQ
 ***********************************************************************/
cs_status arn5_spi_dev_init_irq_cfg(cs_uint8 dev_id, arn5_spi_dev_cfg_t * pCfg)
{
  cs_uint32 ii ;

  /* Register SPI4P2A IRQ table. */
  ARN5_IRQ_RET_IF_ERROR(ARN5_REGISTER_IRQ_TBL(dev_id, ARN5_ID_HOST_IF,
                       arn5_spi_irq_tbl, arn5_spi_irq_grp_name_tbl,
                       ARN5_SPI_MAX_GRP, NULL), CS_ERROR) ;

  ARN5_IRQ_RET_IF_ERROR(ARN5_REGISTER_IMED_TBL(dev_id, ARN5_ID_HOST_IF,
                       arn5_spi_imed_irq_tbl, arn5_spi_irq_imed_name_tbl,
                       ARN5_SPI_MAX_IMED, NULL), CS_ERROR) ;



  for (ii = 0; ii < ARN5_SPI_MAX_GRP; ii++) {
    if (pCfg->irq_ctl_tbl[ii]) {
      /* Clear any interrupts */
      ARN5_IRQ_RET_IF_ERROR(arn5_dev_clear_leaf_irq(dev_id, ARN5_ID_HOST_IF, ii,
                         arn5_spi_irq_tbl[ii].irq_map), CS_ERROR) ;

      /* Enable The IRQ requested to be enabled by default */
      ARN5_IRQ_RET_IF_ERROR(arn5_dev_enable_leaf_irq(dev_id, ARN5_ID_HOST_IF, ii,
                          pCfg->irq_ctl_tbl[ii]), CS_ERROR) ;
    }
  }
  /* Enable all the intermediate interrupt controls by default */
  for (ii = 0; ii < ARN5_SPI_MAX_IMED; ii++) {
    ARN5_IRQ_RET_IF_ERROR(arn5_enable_imed_irq(dev_id, ARN5_ID_HOST_IF, ii,
                        arn5_spi_imed_irq_tbl[ii].irq_map), CS_ERROR) ;
  }
  return (CS_OK) ;
}

/************************************************************************
 * ISR ROUTINE FOR SPI BLOCK
 ***********************************************************************/
cs_status arn5_spi_dev_isr(cs_uint8 dev_id)
{
  ARN5_t                   * pDev ;
  ARN5_SPI4P2_CORE_intrpt_t  core_intrpt ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  core_intrpt.wrd = CS_REG_READ(&pDev->SPI4P2.core_intrpt.wrd) ;

  /* Core */
  if (core_intrpt.bf.rxif_intrpt) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                  ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2_RXIF_intrpt,
                  &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2_RXIF_intrpt],
                  arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2_RXIF_intrpt],
                  ARN5_SPI4P2_RXIF_intrpt_irq_id_name_tbl,
                  sizeof(ARN5_SPI4P2_RXIF_intrpt_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }
  if (core_intrpt.bf.txif_txintrpt) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                  ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2_TXIF_txintrpt,
                  &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2_TXIF_txintrpt],
                  arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2_TXIF_txintrpt],
                  ARN5_SPI4P2_TXIF_txintrpt_irq_id_name_tbl,
                  sizeof(ARN5_SPI4P2_TXIF_txintrpt_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }
  /* Host */
  if (core_intrpt.bf.tx_rprif_afifo_intrpt) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                  ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt,
                  &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt],
                  arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt],
                  ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_irq_id_name_tbl,
                  sizeof(ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }
  if (core_intrpt.bf.rx_rprif_intrpt_vec) {
    ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_t intrpt_vec ;
    intrpt_vec.wrd = CS_REG_READ(&pDev->SPI4P2a.rx_rprif_intrpt_vec.wrd) ;

    if (intrpt_vec.bf.misseop_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt],
                    ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.misssop_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt],
                    ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
  }
  if (core_intrpt.bf.txint_intrpt_vec) {
    ARN5_SPI4P2A_TXINT_intrpt_vec_t intrpt_vec ;
    intrpt_vec.wrd = CS_REG_READ(&pDev->SPI4P2a.txint_intrpt_vec.wrd) ;

    if (intrpt_vec.bf.intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_intrpt],
                    ARN5_SPI4P2A_TXINT_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.fifo_ovrflw_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt],
                    ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.fifo_abort_pkt_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt],
                    ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.fifo_drop_pkt_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt],
                    ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.fifo_eccerr_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt],
                    ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.fifo_eccfix_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt],
                    ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.perr_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_perr_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_perr_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_perr_intrpt],
                    ARN5_SPI4P2A_TXINT_perr_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_perr_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.sop_eop_err_intrpt0) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt],
                    ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt_irq_id_name_tbl,
                    sizeof(ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
  }
  if (core_intrpt.bf.rxint_rxintrpt) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                  ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_RXINT_rxintrpt,
                  &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_RXINT_rxintrpt],
                  arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_RXINT_rxintrpt],
                  ARN5_SPI4P2A_RXINT_rxintrpt_irq_id_name_tbl,
                          sizeof(ARN5_SPI4P2A_RXINT_rxintrpt_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }
  if (core_intrpt.bf.rxint_intrpt_vec) {
    ARN5_SPI4P2A_RXINT_intrpt_vec_t intrpt_vec ;
    intrpt_vec.wrd = CS_REG_READ(&pDev->SPI4P2a.rxint_intrpt_vec.wrd) ;

    if (intrpt_vec.bf.fifo_ovrflw_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt],
                    ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_irq_id_name_tbl,
                            sizeof(ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.fifo_eccerr_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt],
                    ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_irq_id_name_tbl,
                            sizeof(ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.fifo_eccfix_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt],
                    ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_irq_id_name_tbl,
                            sizeof(ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (intrpt_vec.bf.perr_intrpt) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                    ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_RXINT_perr_intrpt,
                    &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_RXINT_perr_intrpt],
                    arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_RXINT_perr_intrpt],
                    ARN5_SPI4P2A_RXINT_perr_intrpt_irq_id_name_tbl,
                            sizeof(ARN5_SPI4P2A_RXINT_perr_intrpt_irq_id_name_tbl) /
                    sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
  }
  if (core_intrpt.bf.membist_intrpt) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev,
                  ARN5_ID_HOST_IF, ARN5_GRP_SPI4P2A_MEMBIST_intrpt,
                  &arn5_spi_irq_tbl[ARN5_GRP_SPI4P2A_MEMBIST_intrpt],
                  arn5_spi_irq_grp_name_tbl[ARN5_GRP_SPI4P2A_MEMBIST_intrpt],
                  ARN5_SPI4P2A_MEMBIST_intrpt_irq_id_name_tbl,
                          sizeof(ARN5_SPI4P2A_MEMBIST_intrpt_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }
  return (CS_OK) ;
}

