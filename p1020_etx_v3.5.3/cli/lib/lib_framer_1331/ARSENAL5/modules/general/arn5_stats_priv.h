/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_stats_priv.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PRIVATE header file.
 * It contains defines and APIs for Statistics.
 */
#ifndef __ARN5_STATS_PRIV_H__
#define __ARN5_STATS_PRIV_H__

#include "cs_types.h"
#include "arn5_public.h"
#include "arn5_stats.h"

#define ARN5_STATS_MAX_SECT          (64)


typedef enum {
  ARN5_DEVICE   = 0,
  ARN5_PORT,
} arn5_dev_or_port_t ;

/************************************************************************
 * STATISTICS OPERATION TYPE
 ************************************************************************/
typedef enum {
  ARN5_STATS_GET,
  ARN5_STATS_PRINT,
  ARN5_STATS_CLEAR
} arn5_stats_op_type_t ;

/************************************************************************
 * INTERRUPTS ENUMS RELATED TO STATISTICS
 ************************************************************************/
typedef enum {
  ARN5_STATS_CTR_OVRFLOW,
  ARN5_STATS_CTR_ROLLOVR,
  ARN5_STATS_CTR_SATURATE,
  ARN5_STATS_CTR_MSB_0_TO_1
} arn5_stats_cntr_irq_t ;

/************************************************************************
 * STATISTICS CONTROL BLOCK
 ************************************************************************/
typedef struct {
  cs_uint8            * name ;        /* user-friendly cntr name string */
  cs_uint32             tx_reg_ofst ; /* register offset                */
  cs_uint32             rx_reg_ofst ; /* register offset                */
  cs_uint16             num_regs    ; /* num of regs make up the cntr   */
  cs_uint16             stride ;      /* stride value                   */
  cs_uint32             flags ;       /* Flags to characterize the stat */
} arn5_stats_info_t ;

#define ARN5_STATS_FLAG_RX_VALID         (0x00000001)
#define ARN5_STATS_FLAG_TX_VALID         (0x00000002)

#define ARN5_STATS_FLAG_SPL_GET          (0x00000010)
#define ARN5_STATS_FLAG_SPL_PRINT        (0x00000020)
#define ARN5_STATS_FLAG_SPL_CLEAR        (0x00000040)

#define ARN5_STATS_FLAG_PROTECT_RX       (0x00000100)
#define ARN5_STATS_FLAG_PROTECT_TX       (0x00000200)


#define ARN5_STATS_FLAG_RX_READ_CLR      (0x00001000)
#define ARN5_STATS_FLAG_TX_READ_CLR      (0x00002000)

/* NO_CLR hw counters can't be cleared and will roll over automatically */
#define ARN5_STATS_FLAG_NO_CLR_RX        (0x00004000)
#define ARN5_STATS_FLAG_NO_CLR_TX        (0x00008000)

#define ARN5_STATS_FLAG_NO_ACCUM         (0x00010000)

#define ARN5_STATS_FLAG_NEXT_UNIT_TIE    (0x10000000)
#define ARN5_STATS_FLAG_LAST_UNIT_TIE    (0x20000000)

#define ARN5_STATS_FLAG_TX_RX_VALID      (ARN5_STATS_FLAG_RX_VALID    |   \
                                         ARN5_STATS_FLAG_TX_VALID    )

#define ARN5_STATS_FLAG_TX_RX_READ_CLR   (ARN5_STATS_FLAG_RX_READ_CLR |   \
                                         ARN5_STATS_FLAG_TX_READ_CLR )

#define ARN5_STATS_FLAG_NO_CLR_TX_RX     (ARN5_STATS_FLAG_NO_CLR_RX   |   \
                                         ARN5_STATS_FLAG_NO_CLR_TX   )

#define ARN5_STATS_FLAG_PROTECT_TX_RX    (ARN5_STATS_FLAG_PROTECT_RX  |   \
                                         ARN5_STATS_FLAG_PROTECT_TX  )

/************************************************************************
 * Macro for determining if RX is valid
 ************************************************************************/
#define ARN5_STATS_UNIT_IS_RX_VALID(FLAG)    \
                    ((FLAG) & ARN5_STATS_FLAG_RX_VALID)

/************************************************************************
 * Macro for determining if TX is valid
 ************************************************************************/
#define ARN5_STATS_UNIT_IS_TX_VALID(FLAG)    \
                    ((FLAG) & ARN5_STATS_FLAG_TX_VALID)

/************************************************************************
 * Macro for determining if ARN5_STATS_FLAG_NEXT_UNIT_TIE is valid
 ************************************************************************/
#define ARN5_STATS_UNIT_IS_NEXT_TIE(FLAG)    \
                    ((FLAG) & ARN5_STATS_FLAG_NEXT_UNIT_TIE)

/************************************************************************
 * Macro for determining if ARN5_STATS_FLAG_LAST_UNIT_TIE is valid
 ************************************************************************/
#define ARN5_STATS_UNIT_IS_LAST_TIE(FLAG)    \
                    ((FLAG) & ARN5_STATS_FLAG_LAST_UNIT_TIE)

/*********************************************************************
 * Macro for determining if the block is registered
 *********************************************************************/
#define ARN5_STATS_IS_BLK_REGISTERED(DPID, DP, MODID)      \
        ( (arn5_stats_get_blk_cb((DPID), (DP), (MODID))) ? TRUE : FALSE )

/************************************************************************
 * Macro for determining if there is another unit is tied with itself
 ************************************************************************/
#define ARN5_STATS_UNIT_IS_TIE_VALID(FLAG)                 \
                    ((ARN5_STATS_UNIT_IS_NEXT_TIE(FLAG))   \
                    ||(ARN5_STATS_UNIT_IS_LAST_TIE(FLAG)))


/************************************************************************
 * Macro to convert 64bit data into 32bit data to support the OS with
 * 32bit-only printing capability without losing precisions
 ************************************************************************/
#define ARN5_STATS_GET_UPPER_32BIT_HEX(CNTS)  \
                    ((cs_uint32)((CNTS)>>32))
#define ARN5_STATS_GET_LOWER_32BIT_HEX(CNTS)  \
                    ((cs_uint32)((CNTS)&0xFFFFFFFF))

#define ARN5_STATS_GET_BILLIONS_DEC(CNTS)     \
                    ((cs_uint32)((CNTS)/1000000000))
#define ARN5_STATS_GET_BILLIONS_MOD_DEC(CNTS) \
                    ((cs_uint32)((CNTS)%1000000000))


/************************************************************************
 * Macro to get the RX and TX registers address
 ************************************************************************/
#define ARN5_STATS_GET_RX_REG_ADDR(BASEADDR, SLICE, SLICE_SIZE, P_INFO) \
            (((cs_uint32)(BASEADDR)) + ((SLICE) * SLICE_SIZE) +        \
             ((P_INFO)->rx_reg_ofst))

#define ARN5_STATS_GET_TX_REG_ADDR(BASEADDR, SLICE, SLICE_SIZE, P_INFO) \
            (((cs_uint32)(BASEADDR)) + ((SLICE) * SLICE_SIZE) +        \
             ((P_INFO)->tx_reg_ofst))


/* the work area for couters, dynamic allocated to save memory usage    */
/* addition should be at least in 32 bit boundary and bits can be reused*/
/* as long as they are exclusive in usage.                              */
typedef struct {
  arn5_stats_data_t       prev_data ;    /* storage for last hw counters */
} arn5_stats_work_area_t ;


/************************************************************************
 * STATISTICS SECTION CONTROL BLOCK
 ************************************************************************/
typedef struct {
  cs_uint8                * sect_name ; /* Section Name                 */
  cs_boolean              enabled ;     /* Stats enabled/Disabled state */
  cs_uint8                stride_id ;   /* Stride number for addr cal   */
  cs_uint32               stride_size ; /* the size of a stride         */
  cs_uint16               num_entries ; /* Number of counter in section */
  arn5_stats_data_t        * pStats ;    /* Pointer to stats data        */
  cs_boolean              * enbl_tbl ;  /* Enable/Disable control tbl   */
  const arn5_stats_info_t  * info_tbl ;  /* Info table for counters      */
  arn5_stats_work_area_t   * work_area_tbl ; /* working area tbl         */
} arn5_stats_sect_cb_t ;


/************************************************************************
 * STATISTICS BLOCK CONTROL BLOCK AND SPECIAL HANDLER PROTOTYPE
 ************************************************************************/
typedef void (*arn5_stats_get_func)(cs_uint16 dpid, cs_uint16 sect_id,
                        cs_uint16 unit_id, cs_dir_t dir,
                        cs_boolean sect_context) ;

typedef void (*arn5_stats_print_func)(cs_uint16 dpid, cs_uint16 sect_id,
                        cs_uint16 unit_id, cs_dir_t dir,
                        cs_boolean sect_context) ;

typedef void (*arn5_stats_clear_func)(cs_uint16 dpid, cs_uint16 sect_id,
                        cs_uint16 unit_id, cs_dir_t dir,
                        cs_boolean sect_context) ;

typedef struct {
  cs_int8              * blk_name ; /* BLock Name                   */
  cs_boolean           enabled ;    /* Stats enabled/Disable State  */
  cs_uint16            num_sect ;   /* Number of sections           */
  arn5_stats_sect_cb_t  * p_sect_cb[ARN5_STATS_MAX_SECT] ;
  arn5_stats_get_func   get_func ;   /* Special get function pointer */
  arn5_stats_print_func print_func ; /* Special Print function ptr   */
  arn5_stats_clear_func clear_func ; /* Special clear function ptr   */
} arn5_stats_blk_cb_t ;


/*************************************************************
 * DEVICE STATS CONTROL BLOCK
 *************************************************************/
typedef struct {
  arn5_stats_blk_cb_t   * host_if ;
} arn5_dev_stats_cb_t ;


/*************************************************************
 * PORT STATS CONTROL BLOCK
 *************************************************************/
typedef struct {
  arn5_stats_blk_cb_t   * host_if ;
  arn5_stats_blk_cb_t   * framer ;
  arn5_stats_blk_cb_t   * eth ;
  arn5_stats_blk_cb_t   * lif ;
  arn5_stats_blk_cb_t   * pprbs ;
} arn5_port_stats_cb_t ;


/*************************************************************
 * STRUCTURE TO HOLD SECTION INFO
 *************************************************************/
typedef struct {
  cs_int8                 * sect_name ;
  cs_uint32               sect_offset ;
  cs_uint8                stride_id ;
  const arn5_stats_info_t  * info_tbl ;
  cs_uint16               num_unit ;
} arn5_stats_sect_tbl_entry_t ;


/************************************************************************
 * APIs
 ************************************************************************/
extern
arn5_stats_blk_cb_t * arn5_stats_register_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_boolean ctl_enabled,
                        arn5_stats_get_func pGet_func,
                        arn5_stats_print_func pPrint_func,
                        arn5_stats_clear_func pClear_func) ;
extern
cs_status arn5_stats_deregister_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id) ;
extern
cs_status arn5_stats_clean_all(cs_uint16 dpid,
                        arn5_dev_or_port_t dp) ;
extern
cs_status arn5_stats_add_sect_tbl_entry(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_int8 * sect_name,
                        cs_uint16 sect_id,
                        cs_uint8  stride_id,
                        cs_uint32 stride_size,
                        cs_boolean ctl_enabled,
                        cs_uint16 num_child,
                        arn5_stats_data_t * p_data,
                        const arn5_stats_info_t * p_info_tbl) ;
extern
cs_status arn5_stats_remove_sect_tbl_entry(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id) ;
extern
cs_status arn5_stats_sect_cmn_op(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        arn5_stats_sect_cb_t * stats_sect_cb,
                        arn5_stats_op_type_t op,
                        cs_dir_t dir) ;
extern
cs_status arn5_stats_unit_cmn_op(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        arn5_stats_sect_cb_t * sect_cb,
                        arn5_stats_op_type_t op,
                        cs_dir_t dir,
                        cs_boolean sect_context) ;
extern
arn5_stats_blk_cb_t * arn5_stats_get_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id) ;
extern
void arn5_stats_print_blk_hdr(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        cs_int8 * blk_name) ;
extern
void arn5_stats_print_sect_hdr(cs_uint16 dpid, arn5_dev_or_port_t dp,
                        cs_int8 * name) ;
extern
void arn5_stats_print_unit_cntr(const cs_uint8 * name, cs_uint32 flags,
                        arn5_stats_data_t * p_data) ;
extern
void arn5_set_sect_print_flag(cs_uint32 val) ;
extern
cs_uint32 arn5_get_sect_print_flag(void) ;


#endif /* __ARN5_STATS_PRIV_H__ */

