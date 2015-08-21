/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_stats.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PRIVATE header file.
 * It contains defines and APIs for MILAN2 Statistics.
 */
#ifndef __M2R_STATS_PRIV_H__
#define __M2R_STATS_PRIV_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_stats.h"

#define M2R_STATS_MAX_SECT          (64)

/************************************************************************
 * STATISTICS OPERATION TYPE
 ************************************************************************/
typedef enum {
  M2R_STATS_GET,
  M2R_STATS_PRINT,
  M2R_STATS_CLEAR
} m2r_stats_op_type_t ;

/************************************************************************
 * INTERRUPTS ENUMS RELATED TO STATISTICS
 ************************************************************************/
typedef enum {
  M2R_STATS_CTR_OVRFLOW,
  M2R_STATS_CTR_ROLLOVR,
  M2R_STATS_CTR_SATURATE,
  M2R_STATS_CTR_MSB_0_TO_1
} m2r_stats_cntr_irq_t ;

/************************************************************************
 * STATISTICS CONTROL BLOCK
 ************************************************************************/
typedef struct {
  char                * name ;        /* user-friendly cntr name string */
  cs_uint32             tx_reg_ofst ; /* register offset                */
  cs_uint32             rx_reg_ofst ; /* register offset                */
  cs_uint16             num_regs    ; /* num of regs make up the cntr   */
  cs_uint16             stride ;      /* stride value                   */
  cs_uint32             flags ;       /* Flags to characterize the stat */
} m2r_stats_info_t ;

#define M2R_STATS_FLAG_RX_VALID         (0x00000001)
#define M2R_STATS_FLAG_TX_VALID         (0x00000002)

#define M2R_STATS_FLAG_SPL_GET          (0x00000010)
#define M2R_STATS_FLAG_SPL_PRINT        (0x00000020)
#define M2R_STATS_FLAG_SPL_CLEAR        (0x00000040)

#define M2R_STATS_FLAG_RX_READ_CLR      (0x00001000)
#define M2R_STATS_FLAG_TX_READ_CLR      (0x00002000)

#define M2R_STATS_FLAG_NO_ACCUM         (0x00010000)

/* Adam on 10/28/03 -> */
#define M2R_STATS_FLAG_NEXT_UNIT_TIE	(0x10000000)
#define M2R_STATS_FLAG_LAST_UNIT_TIE	(0x20000000)

#define M2R_STATS_FLAG_TX_RX_VALID      (M2R_STATS_FLAG_RX_VALID    |   \
                                         M2R_STATS_FLAG_TX_VALID    )

#define M2R_STATS_FLAG_TX_RX_READ_CLR   (M2R_STATS_FLAG_RX_READ_CLR |   \
                                         M2R_STATS_FLAG_TX_READ_CLR )

/************************************************************************
 * Macro for determining if RX is valid
 ************************************************************************/
#define M2R_STATS_UNIT_IS_RX_VALID(FLAG)    \
                    ((FLAG) & M2R_STATS_FLAG_RX_VALID)

/************************************************************************
 * Macro for determining if TX is valid
 ************************************************************************/
#define M2R_STATS_UNIT_IS_TX_VALID(FLAG)    \
                    ((FLAG) & M2R_STATS_FLAG_TX_VALID)

/* Adam on 10/28/03 ->*/
/************************************************************************
 * Macro for determining if M2R_STATS_FLAG_NEXT_UNIT_TIE is valid
 ************************************************************************/
#define M2R_STATS_UNIT_IS_NEXT_TIE(FLAG)    \
                    ((FLAG) & M2R_STATS_FLAG_NEXT_UNIT_TIE)

/************************************************************************
 * Macro for determining if M2R_STATS_FLAG_LAST_UNIT_TIE is valid
 ************************************************************************/
#define M2R_STATS_UNIT_IS_LAST_TIE(FLAG)    \
                    ((FLAG) & M2R_STATS_FLAG_LAST_UNIT_TIE)

/************************************************************************
 * Macro for determining if there is another unit is tied with itself
 ************************************************************************/
#define M2R_STATS_UNIT_IS_TIE_VALID(FLAG)    \
                    ((M2R_STATS_UNIT_IS_NEXT_TIE(FLAG))		\
                    ||(M2R_STATS_UNIT_IS_LAST_TIE(FLAG)))
/* Adam <- */

/************************************************************************
 * Macro to convert 64bit data into 32bit data to support the OS with
 * 32bit-only printing capability without losing precisions
 ************************************************************************/
#define M2R_STATS_GET_UPPER_32BIT_HEX(CNTS)  \
                    ((cs_uint32)((CNTS)>>32))
#define M2R_STATS_GET_LOWER_32BIT_HEX(CNTS)  \
                    ((cs_uint32)((CNTS)&0xFFFFFFFF))

#define M2R_STATS_GET_BILLIONS_DEC(CNTS)  \
                    ((cs_uint32)((CNTS)/1000000000))
#define M2R_STATS_GET_BILLIONS_MOD_DEC(CNTS)  \
                    ((cs_uint32)((CNTS)%1000000000))

/************************************************************************
 * Macro to get the RX and TX registers address
 ************************************************************************/
#define M2R_STATS_GET_RX_REG_ADDR(BASEADDR, SLICE, STRIDE, P_INFO)      \
            (((cs_uint64)(BASEADDR)) + ((SLICE) * M2R_SLICE_SIZE) +     \
             ((P_INFO)->rx_reg_ofst) +                                  \
             ((STRIDE) * ((P_INFO)->stride) * sizeof(cs_reg)))

#define M2R_STATS_GET_TX_REG_ADDR(BASEADDR, SLICE, STRIDE, P_INFO)      \
            (((cs_uint64)(BASEADDR)) + ((SLICE) * M2R_SLICE_SIZE) +     \
             ((P_INFO)->tx_reg_ofst) +                                  \
             ((STRIDE) * ((P_INFO)->stride) * sizeof(cs_reg)))

/************************************************************************
 * STATISTICS SECTION CONTROL BLOCK
 ************************************************************************/
typedef struct {
  char                   * sect_name ;  /* Section Name                 */
  cs_boolean               enabled ;    /* Stats enabled/Disabled state */
  cs_uint8                 stride_id ;
  cs_uint16                num_entries ;/* Number of counter in section */
  m2r_stats_data_t       * pStats ;     /* Pointer to stats data        */
  cs_boolean             * enbl_tbl ;   /* Enable/Disable control tbl   */
  const m2r_stats_info_t * info_tbl ;   /* Info table for counters      */
} m2r_stats_sect_cb_t ;

/************************************************************************
 * STATISTICS BLOCK CONTROL BLOCK AND SPECIAL HANDLER PROTOTYPE
 ************************************************************************/
typedef void (*m2r_stats_get_func)(cs_uint16 port_id, cs_uint16 sect_id,
                                   cs_uint16 unit_id) ;

typedef void (*m2r_stats_print_func)(cs_uint16 port_id, cs_uint16 sect_id,
                                     cs_uint16 unit_id) ;

typedef void (*m2r_stats_clear_func)(cs_uint16 port_id, cs_uint16 sect_id,
                                     cs_uint16 unit_id) ;

typedef struct {
  cs_int8                * blk_name ;   /* BLock Name                   */
  cs_boolean               enabled ;    /* Stats enabled/Disable State  */
  cs_uint16                num_sect ;   /* Number of sections           */
  m2r_stats_sect_cb_t    * p_sect_cb[M2R_STATS_MAX_SECT] ;
  m2r_stats_get_func       get_func ;   /* Special get function pointer */
  m2r_stats_print_func     print_func ; /* Special Print function ptr   */
  m2r_stats_clear_func     clear_func ; /* Special clear function ptr   */
} m2r_stats_blk_cb_t ;

/*************************************************************
 * PORT STATS CONTROL BLOCK
 *************************************************************/
typedef struct {
  m2r_stats_blk_cb_t   * spi42 ;
  m2r_stats_blk_cb_t   * pprbs ;
  m2r_stats_blk_cb_t   * framer ;
  m2r_stats_blk_cb_t   * rmac ;
  m2r_stats_blk_cb_t   * lif ;
  m2r_stats_blk_cb_t   * eth ;
  m2r_stats_blk_cb_t   * xgxs ;
} m2r_port_stats_cb_t ;

/*************************************************************
 * STRUCTURE TO HOLD SECTION INFO
 *************************************************************/
typedef struct {
  cs_int8       * sect_name ;
  cs_uint32       sect_offset ;
  cs_uint8        stride_id ;
  const m2r_stats_info_t * info_tbl ;
  cs_uint16       num_unit ;
} m2r_stats_sect_tbl_entry_t ;

/************************************************************************
 * APIs
 ************************************************************************/
extern
m2r_stats_blk_cb_t * m2r_stats_register_blk_cb(cs_uint16 port_id,
                                           m2r_module_id_t mod_id,
                                           cs_boolean ctl_enabled,
                                           m2r_stats_get_func pGet_func,
                                           m2r_stats_print_func pPrint_func,
                                           m2r_stats_clear_func pClear_func) ;

extern
cs_status m2r_stats_deregister_blk_cb(cs_uint16 port_id,
                                             m2r_module_id_t mod_id) ;

extern
cs_status m2r_stats_clean_all(cs_uint16 port_id) ;

extern
cs_status m2r_stats_add_sect_tbl_entry(cs_uint16 port_id,
                                 m2r_module_id_t mod_id,
                                 cs_int8 * sect_name,
                                 cs_uint16 sect_id,
                                 cs_uint8  stride_id,
                                 cs_boolean ctl_enabled,
                                 cs_uint16 num_child,
                                 m2r_stats_data_t * p_data,
                                 const m2r_stats_info_t * p_info_tbl) ;

extern
cs_status m2r_stats_remove_sect_tbl_entry(cs_uint16 port_id,
                                          m2r_module_id_t mod_id,
                                          cs_uint16 sect_id) ;

#endif /* __M2R_STATS_PRIV_H__ */


