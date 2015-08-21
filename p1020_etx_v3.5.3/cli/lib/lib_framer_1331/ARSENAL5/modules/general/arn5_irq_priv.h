/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_irq_priv.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * THIS FILE CONTAINS IRQ API FOR PRIVATE USE.
 *
 */
#ifndef __ARN5_IRQ_PRIV_H__
#define __ARN5_IRQ_PRIV_H__

#include "cs_types.h"
#include "arn5_public.h"
#include "arn5_irq.h"

/*************************************************************************
 * ISR GROUP CONTROL STRUCTURE
 ************************************************************************/
#define ARN5_IRQ_FLG_RW1C            (0x0001)
#define ARN5_IRQ_FLG_1_TO_ENABLE     (0x0100) /* 1 means enable          */
#define ARN5_IRQ_FLG_DEV             (0x0800) /* dev-level for shared blk*/
#define ARN5_IRQ_FLG_SPL_INT         (0x1000) /* Special handling needed */
#define ARN5_IRQ_FLG_SPL_CLR         (0x2000) /* Special handling needed */

/*************************************************************************
 * IRQ CONTROL STRUCTURE
 ************************************************************************/
typedef struct {
  cs_uint32         offset_intr_reg ;   /* Interrupt register offset    */
  cs_uint32         offset_status_reg ; /* Status register offset       */
  cs_uint32         offset_mask_reg ;   /* Mask register offset         */
  cs_uint16         irq_map ;           /* mask of the valid irq bits   */
  cs_uint16         spl_map ;           /* mask of the special irq bits */
  cs_uint16         flags ;             /* flags                        */
  cs_uint8          num_of_strides;     /* Number of strides            */
  cs_uint32         stride_size ;       /* Size of stride (if any)      */
} arn5_irq_ctl_t ;

/* Read the Interrupt register value */
#define ARN5_IRQ_GET_INT_REG_VAL(IRQ_CB, DEVADDR, SLICE, SLICE_SIZE)      \
            CS_REG_READ((volatile cs_reg*)((DEVADDR)+                    \
              ((SLICE) * (SLICE_SIZE)) + (IRQ_CB)->offset_intr_reg))

#define ARN5_IRQ_SET_INT_REG(IRQ_CB, DEVADDR, SLICE, SLICE_SIZE, VAL)     \
          CS_REG_WRITE((volatile cs_reg*)((DEVADDR)+                     \
          ((SLICE) * (SLICE_SIZE)) + (IRQ_CB)->offset_intr_reg), (VAL))

/* Read the mask register value */
#define ARN5_IRQ_GET_MASK_REG_VAL(IRQ_CB, DEVADDR, SLICE, SLICE_SIZE)     \
            CS_REG_READ((volatile cs_reg*)((DEVADDR)+                    \
              ((SLICE) * (SLICE_SIZE)) + (IRQ_CB)->offset_mask_reg))

#define ARN5_IRQ_SET_MASK_REG(IRQ_CB, DEVADDR, SLICE, SLICE_SIZE, VAL)    \
          CS_REG_WRITE((volatile cs_reg*)((DEVADDR)+                     \
          ((SLICE) * (SLICE_SIZE)) + (IRQ_CB)->offset_mask_reg), (VAL))


/* Read the Status register value */
#define ARN5_IRQ_GET_STATUS_REG_VAL(IRQ_CB, DEVADDR, SLICE, SLICE_SIZE)   \
            CS_REG_READ((volatile cs_reg*)((DEVADDR)+                    \
              ((SLICE) * (SLICE_SIZE)) + (IRQ_CB)->offset_status_reg))

#define ARN5_IRQ_SET_STATUS_REG(IRQ_CB, DEVADDR, SLICE, SLICE_SIZE, VAL)  \
         CS_REG_WRITE((volatile cs_reg*)((DEVADDR)+                      \
         ((SLICE) * (SLICE_SIZE)) + (IRQ_CB)->offset_status_reg), (VAL))

/*************************************************************************
 * SPECIAL IRQ HANDLER:
 * To provide flexibility to do some special processing from the generic
 * group handler, the individual block IRQ tables can have either
 * ROMA_IRQ_FLG_SPL_INT or ROMA_IRQ_FLG_SPL_CLR flag set for the leaf
 * group entry. The special handler will be then called for additional 
 * processing or clearing of the interrupt.
 * 
 * The special handler returns a status(bit-map of flags) which the 
 * generic handler uses.
************************************************************************/
/* Return status from special handler */
/* -nothing for now- */

/* Special handler calling option */
#define ARN5_IRQ_SPL_CLR                 (0x0010)
#define ARN5_IRQ_SPL_INT                 (0x0020)
/* #define ARN5_IRQ_SPL_ENBL                (0x0040)  *unused* */
/* #define ARN5_IRQ_SPL_DISBL               (0x0080)  *unused* */

typedef struct {
  cs_uint16         irq_oper ;
  cs_uint16         dpid ;
  union {
    struct {
      arn5_irq_info_t * irq_info ;
      cs_uint16      irq_reg_val ;
      cs_uint16      irq_status_val ;
    } irq_hndl ;

    struct {
      cs_uint16     grp_id ;
      cs_uint16     spl_clr_bits ;

    } irq_clr ;
  } op_mode ;
} arn5_irq_spl_info_t ;


/*************************************************************************
 * macro to fill special IRQ clear related info for the handler.
 ************************************************************************/
#define ARN5_IRQ_FILL_SPL_CLR_INFO(P_INFO, DPID, GRPID, SPL_CLR_BITS)   \
{                                                                       \
  (P_INFO)->irq_oper = ARN5_IRQ_SPL_CLR ;                               \
  (P_INFO)->dpid = (DPID) ;                                             \
  (P_INFO)->op_mode.irq_clr.grp_id  = (GRPID) ;                         \
  (P_INFO)->op_mode.irq_clr.spl_clr_bits = (SPL_CLR_BITS) ;             \
}

/*************************************************************************
 * macro to fill special IRQ related info for the handler.
 ************************************************************************/
#define ARN5_IRQ_FILL_SPL_INT_INFO(P_INFO, P_IRQ_INFO, DPID, IRQ_VAL)   \
{                                                                       \
  (P_INFO)->irq_oper = ARN5_IRQ_SPL_INT ;                               \
  (P_INFO)->dpid  = (DPID) ;                                            \
  (P_INFO)->op_mode.irq_hndl.irq_info = (P_IRQ_INFO) ;                  \
  (P_INFO)->op_mode.irq_hndl.irq_reg_val = (IRQ_VAL) ;                  \
}

/*************************************************************************
 * SPECIAL IRQ HANDLER PROTOTYPE
 ************************************************************************/
typedef cs_uint16 (*IRQ_SPL_HNDLR)(arn5_irq_spl_info_t * irq_spl_info) ;

/*************************************************************************
 * IRQ CONTROL BLOCK FOR MODULE
 ************************************************************************/
typedef struct {
  const arn5_irq_ctl_t *     tbl ;
  const cs_uint8 **         name_tbl ;
  cs_uint16                 tbl_size ;
  IRQ_SPL_HNDLR             spl_hndlr ;
  const arn5_irq_ctl_t *     imed_tbl ;    /* for intermediate int level */
  const cs_uint8 **         imed_name_tbl ;
  cs_uint16                 imed_tbl_size ;
  IRQ_SPL_HNDLR             imed_spl_hndlr ;
} arn5_blk_irq_cb_t ;

/*************************************************************************
 * DEVICE IRQ CONTROL BLOCK
 ************************************************************************/
typedef struct {
  arn5_blk_irq_cb_t          mpif ;
  arn5_blk_irq_cb_t          spi_host_if ;
} arn5_dev_irq_cb_t ;

/*************************************************************************
 * PORT IRQ CONTROL BLOCK
 ************************************************************************/
typedef struct {
  arn5_blk_irq_cb_t          framer ;
  arn5_blk_irq_cb_t          eth ;
  arn5_blk_irq_cb_t          lif ;
  arn5_blk_irq_cb_t          pprbs ;
} arn5_port_irq_cb_t ;

/********************
 * irq id name      *
 ********************/
typedef struct {
   cs_uint16                id ;
   const cs_uint8           * name ;
} arn5_irq_id_name_t ;

CS_INLINE
const cs_uint8 * ARN5_GET_IRQ_ID_NAME(const arn5_irq_id_name_t * tbl,
                                     cs_uint16 max,
                                     cs_uint16 irq_id)
{
  cs_uint16   ii ;

  for (ii = 0; ii < max; ii++) {
    if (tbl[ii].id == irq_id) {
      return( tbl[ii].name ) ;
    }
  }

  return("???") ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    ARN5_GET_IRQ_ID_NAME(tbl, max, irq_id) ;
  }
#endif
}

#define ARN5_IRQ_RET_IF_ERROR(f, c) { if ((f) != CS_OK) return (c); }

/*********************************************************
 * IRQ Private APIs
 *********************************************************/
extern
cs_status  ARN5_REGISTER_IRQ_TBL(cs_uint16 dpid,
                                arn5_module_id_t mod_id,
                                const arn5_irq_ctl_t * tbl,
                                const cs_uint8 ** name_tbl,
                                cs_uint16  tbl_size,
                                IRQ_SPL_HNDLR irq_spl_hndlr) ;

extern
cs_status  ARN5_UNREGISTER_IRQ_TBL(cs_uint16 dpid,
                                arn5_module_id_t mod_id) ;
extern
cs_status  ARN5_REGISTER_IMED_TBL(cs_uint16 dpid,
                                arn5_module_id_t mod_id,
                                const arn5_irq_ctl_t * tbl,
                                const cs_uint8 ** imed_name_tbl,
                                cs_uint16  tbl_size,
                                IRQ_SPL_HNDLR irq_spl_hndlr) ;

extern
cs_status  ARN5_UNREGISTER_IMED_TBL(cs_uint16 dpid,
                                arn5_module_id_t mod_id) ;
extern
cs_status  arn5_irq_grp_hndlr(cs_uint16 dpid, cs_uint32 dev_addr,
                                cs_uint32 mod_id, cs_uint32 grp_id,
                                const arn5_irq_ctl_t * irq_ctl,
                                const cs_uint8 * grp_name,
                                const arn5_irq_id_name_t * irq_id_name_tbl,
                                cs_uint16 num_irq_ids) ;

extern
cs_status  arn5_irq_dump_tbl_entry(cs_uint16 dpid, cs_boolean is_dev,
                                cs_uint32 dev_addr,
                                const arn5_irq_ctl_t * irq_ctl,
                                const cs_uint8 * label) ;

extern
cs_status  arn5_enable_imed_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                                cs_uint32 imed_id, cs_uint16 irq_enbl_bits) ;

extern
cs_status  arn5_disable_imed_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                                cs_uint32 imed_id, cs_uint16 irq_disabl_bits) ;

#endif /* __ARN5_IRQ_PRIV_H__ */

