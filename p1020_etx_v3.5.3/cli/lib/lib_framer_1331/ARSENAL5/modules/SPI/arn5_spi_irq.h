
/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_spi_irq.h
 *
 * SPI IRQ Tables and APIs
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#ifndef __ARN5_SPI_IRQ_H__
#define __ARN5_SPI_IRQ_H__


/************************************************************************
 * SPI IRQ leaf defines
 ************************************************************************/
/* Leaf Interrupt Bit fields for ARN5_SPI4P2_RXIF_intrpt */
#define ARN5_SPI4P2_RXIF_intrpt_Rxspi42_perr                    0x0001
#define ARN5_GRP_SPI4P2_RXIF_intrpt_ALL                         0x0001


/************************************************************************
 * SPI IRQ leaf defines
 ************************************************************************/
/* Leaf Interrupt Bit fields for ARN5_SPI4P2_TXIF_txintrpt */
#define ARN5_SPI4P2_TXIF_txintrpt_txbuf_fifo_ovrflw             0x8000
#define ARN5_SPI4P2_TXIF_txintrpt_txspi3_sx_err                 0x0400
#define ARN5_SPI4P2_TXIF_txintrpt_txspi3_adr_err                0x0200
#define ARN5_SPI4P2_TXIF_txintrpt_txspi3_perr                   0x0100
#define ARN5_SPI4P2_TXIF_txintrpt_txrsvctrlwderr                0x0020
#define ARN5_SPI4P2_TXIF_txintrpt_txseqerr                      0x0010
#define ARN5_SPI4P2_TXIF_txintrpt_txsopdisterr                  0x0008
#define ARN5_SPI4P2_TXIF_txintrpt_txburstsizeerr                0x0004
#define ARN5_SPI4P2_TXIF_txintrpt_dip4err                       0x0002
#define ARN5_SPI4P2_TXIF_txintrpt_txlosyn                       0x0001
#define ARN5_GRP_SPI4P2_TXIF_txintrpt_ALL                       0x873f


/************************************************************************
 * SPI IRQ intermidiate defines
 ************************************************************************/
/* Intermidiate Interrupt Bit fields for ARN5_SPI4P2_CORE_intrpt */
#define ARN5_SPI4P2_CORE_intrpt_rxif_intrpt                     0x0800
#define ARN5_SPI4P2_CORE_intrpt_rx_rprif_intrpt_vec             0x0400
#define ARN5_SPI4P2_CORE_intrpt_rxint_intrpt_vec                0x0200
#define ARN5_SPI4P2_CORE_intrpt_rxint_rxintrpt                  0x0100
#define ARN5_SPI4P2_CORE_intrpt_membist_intrpt                  0x0040
#define ARN5_SPI4P2_CORE_intrpt_tx_rprif_afifo_intrpt           0x0004
#define ARN5_SPI4P2_CORE_intrpt_txint_intrpt_vec                0x0002
#define ARN5_SPI4P2_CORE_intrpt_txif_txintrpt                   0x0001
#define ARN5_IMED_SPI4P2_CORE_intrpt_ALL                        0x0f47

#define ARN5_SPI_IRQ_FLAGS       ( ARN5_IRQ_FLG_1_TO_ENABLE | \
                                  ARN5_IRQ_FLG_RW1C )


/************************************************************************
 * SPIA IRQ leaf defines
 ************************************************************************/
/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt */
#define ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_afifo1_perr          0x0002
#define ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_afifo0_perr          0x0001
#define ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt_ALL              0x0003

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt */
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_7             0x0080
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_6             0x0040
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_5             0x0020
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_4             0x0010
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_3             0x0008
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_2             0x0004
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_1             0x0002
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_chan_0             0x0001
#define ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt_ALL            0x00ff

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt */
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_7             0x0080
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_6             0x0040
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_5             0x0020
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_4             0x0010
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_3             0x0008
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_2             0x0004
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_1             0x0002
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_chan_0             0x0001
#define ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt_ALL            0x00ff

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_intrpt */
#define ARN5_SPI4P2A_TXINT_intrpt_q3_fifo_ovf                   0x0004
#define ARN5_SPI4P2A_TXINT_intrpt_q3_fifo_und                   0x0002
#define ARN5_SPI4P2A_TXINT_intrpt_addrerr                       0x0001
#define ARN5_GRP_SPI4P2A_TXINT_intrpt_ALL                       0x0007

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt */
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_7            0x0080
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_6            0x0040
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_5            0x0020
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_4            0x0010
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_3            0x0008
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_2            0x0004
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_1            0x0002
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_chan_0            0x0001
#define ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt_ALL           0x00ff

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt */
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_7         0x0080
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_6         0x0040
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_5         0x0020
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_4         0x0010
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_3         0x0008
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_2         0x0004
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_1         0x0002
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_chan_0         0x0001
#define ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_ALL        0x00ff

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt */
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_7          0x0080
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_6          0x0040
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_5          0x0020
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_4          0x0010
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_3          0x0008
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_2          0x0004
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_1          0x0002
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_chan_0          0x0001
#define ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_ALL         0x00ff

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt */
#define ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_txbuf_err         0x0002
#define ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_txmem_err         0x0001
#define ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt_ALL           0x0003

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt */
#define ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_txbuf_fix         0x0002
#define ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_txmem_fix         0x0001
#define ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt_ALL           0x0003

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_perr_intrpt */
#define ARN5_SPI4P2A_TXINT_perr_intrpt_pm_perr                  0x0100
#define ARN5_GRP_SPI4P2A_TXINT_perr_intrpt_ALL                  0x0100


/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt */
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_0           0x0001
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_1           0x0002
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_2           0x0004
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_3           0x0008
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_4           0x0010
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_5           0x0020
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_6           0x0040
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt0_chan_7           0x0080
#define ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt_ALL           0x00ff

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_RXINT_rxintrpt */
#define ARN5_SPI4P2A_RXINT_rxintrpt_perr                        0x0004
#define ARN5_SPI4P2A_RXINT_rxintrpt_dip2err                     0x0002
#define ARN5_SPI4P2A_RXINT_rxintrpt_rxlosyn                     0x0001
#define ARN5_GRP_SPI4P2A_RXINT_rxintrpt_ALL                     0x0007

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt */
#define ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_pf2_overrun_error 0x0004
#define ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_pf1_overrun       0x0002
#define ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_pf0_overrun       0x0001
#define ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt_ALL           0x0007

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt */
#define ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_pf2_err           0x0004
#define ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_pf1_err           0x0002
#define ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_pf0_err           0x0001
#define ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt_ALL           0x0007

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt */
#define ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_pf2_fix           0x0004
#define ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_pf1_fix           0x0002
#define ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_pf0_fix           0x0001
#define ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt_ALL           0x0007

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_RXINT_perr_intrpt */
#define ARN5_SPI4P2A_RXINT_perr_intrpt_pm_perr                  0x0100
#define ARN5_SPI4P2A_RXINT_perr_intrpt_bb3_perr                 0x0008
#define ARN5_SPI4P2A_RXINT_perr_intrpt_bb2_perr                 0x0004
#define ARN5_SPI4P2A_RXINT_perr_intrpt_bb1_perr                 0x0002
#define ARN5_SPI4P2A_RXINT_perr_intrpt_bb0_perr                 0x0001
#define ARN5_GRP_SPI4P2A_RXINT_perr_intrpt_ALL                  0x010f

/* Leaf Interrupt Bit fields for ARN5_SPI4P2A_MEMBIST_intrpt */
#define ARN5_SPI4P2A_MEMBIST_intrpt_sb_bist_int                 0x0002
#define ARN5_SPI4P2A_MEMBIST_intrpt_mb_bist_int                 0x0001
#define ARN5_GRP_SPI4P2A_MEMBIST_intrpt_ALL                     0x0003

typedef enum {
  ARN5_GRP_SPI4P2_RXIF_intrpt = 0,
  ARN5_GRP_SPI4P2_TXIF_txintrpt,
  ARN5_GRP_SPI4P2A_TX_RPRIF_afifo_intrpt,
  ARN5_GRP_SPI4P2A_RX_RPRIF_misseop_intrpt,
  ARN5_GRP_SPI4P2A_RX_RPRIF_misssop_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_fifo_ovrflw_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_fifo_abort_pkt_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_fifo_drop_pkt_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_fifo_eccerr_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_fifo_eccfix_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_perr_intrpt,
  ARN5_GRP_SPI4P2A_TXINT_sop_eop_err_intrpt,
  ARN5_GRP_SPI4P2A_RXINT_rxintrpt,
  ARN5_GRP_SPI4P2A_RXINT_fifo_ovrflw_intrpt,
  ARN5_GRP_SPI4P2A_RXINT_fifo_eccerr_intrpt,
  ARN5_GRP_SPI4P2A_RXINT_fifo_eccfix_intrpt,
  ARN5_GRP_SPI4P2A_RXINT_perr_intrpt,
  ARN5_GRP_SPI4P2A_MEMBIST_intrpt,
  ARN5_SPI_MAX_GRP
} arn5_SPI_irq_grp_t ;

/************************************************************************
 * SPIA IRQ intermidiate defines
 ************************************************************************/
/* Intermidiate Interrupt Bit fields for ARN5_SPI4P2A_RX_RPRIF_intrpt_vec */
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_spi4p1_intrpt          0x0004
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_misssop_intrpt         0x0002
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_misseop_intrpt         0x0001
/* leave debug int out */
#define ARN5_IMED_SPI4P2A_RX_RPRIF_intrpt_vec_ALL               0x0003

/* Intermidiate Interrupt Bit fields for ARN5_SPI4P2A_TXINT_intrpt_vec */
#define ARN5_SPI4P2A_TXINT_intrpt_vec_sop_eop_err_intrpt        0x1000
#define ARN5_SPI4P2A_TXINT_intrpt_vec_pkt_timeout_intrpt        0x0400
#define ARN5_SPI4P2A_TXINT_intrpt_vec_pkt_eaten_intrpt          0x0080
#define ARN5_SPI4P2A_TXINT_intrpt_vec_perr_intrpt               0x0040
#define ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_eccfix_intrpt        0x0020
#define ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_eccerr_intrpt        0x0010
#define ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_drop_pkt_intrpt      0x0008
#define ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_abort_pkt_intrpt     0x0004
#define ARN5_SPI4P2A_TXINT_intrpt_vec_fifo_ovrflw_intrpt        0x0002
#define ARN5_SPI4P2A_TXINT_intrpt_vec_intrpt                    0x0001
#define ARN5_IMED_SPI4P2A_TXINT_intrpt_vec_ALL                  0xf4ff

/* Intermidiate Interrupt Bit fields for ARN5_SPI4P2A_RXINT_intrpt_vec */
#define ARN5_SPI4P2A_RXINT_intrpt_vec_perr_intrpt               0x0008
#define ARN5_SPI4P2A_RXINT_intrpt_vec_fifo_eccfix_intrpt        0x0004
#define ARN5_SPI4P2A_RXINT_intrpt_vec_fifo_eccerr_intrpt        0x0002
#define ARN5_SPI4P2A_RXINT_intrpt_vec_fifo_ovrflw_intrpt        0x0001
#define ARN5_IMED_SPI4P2A_RXINT_intrpt_vec_ALL                  0x000f

typedef enum {
  ARN5_IMED_SPI4P2_CORE_intrpt,
  ARN5_IMED_SPI4P2A_RX_RPRIF_intrpt_vec,
  ARN5_IMED_SPI4P2A_TXINT_intrpt_vec,
  ARN5_IMED_SPI4P2A_RXINT_intrpt_vec,
  ARN5_SPI_MAX_IMED
} arn5_SPI_imed_irq_grp_t ;

#endif /* __ARN5_SPI_IRQ_H__ */

