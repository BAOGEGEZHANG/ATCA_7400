/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_irq_priv.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * THIS FILE CONTAINS IRQ API FOR PRIVATE USE.
 *
 */
#ifndef __M2R_IRQ_PRIV_H__
#define __M2R_IRQ_PRIV_H__

#include "cs_types.h"
#include "m2r_public.h"
#include "m2r_irq.h"

/*************************************************************************
 * ISR GROUP CONTROL STRUCTURE
 ************************************************************************/
#define M2R_IRQ_FLG_RW1C            (0x0001)
#define M2R_IRQ_FLG_1_TO_ENABLE     (0x0100) /* 1 means enable          */
#define M2R_IRQ_FLG_DEV             (0x0800) /* dev-level for shared blk*/
#define M2R_IRQ_FLG_SPL_INT         (0x1000) /* Special handling needed */
#define M2R_IRQ_FLG_SPL_CLR         (0x2000) /* Special handling needed */

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
} m2r_irq_ctl_t ;

/* Read the Interrupt register value */
#define M2R_IRQ_GET_INT_REG_VAL(IRQ_CB, DEVADDR, SLICE)                 \
            M2R_REG_READ((volatile cs_reg*)((DEVADDR)+                   \
              ((SLICE) * M2R_SLICE_SIZE) + (IRQ_CB)->offset_intr_reg))

#define M2R_IRQ_SET_INT_REG(IRQ_CB, DEVADDR, SLICE, VAL)                \
          M2R_REG_WRITE((volatile cs_reg*)((DEVADDR)+                    \
          ((SLICE) * M2R_SLICE_SIZE) + (IRQ_CB)->offset_intr_reg), (VAL))

/* Read the mask register value */
#define M2R_IRQ_GET_MASK_REG_VAL(IRQ_CB, DEVADDR, SLICE)                \
            M2R_REG_READ((volatile cs_reg*)((DEVADDR)+                   \
              ((SLICE) * M2R_SLICE_SIZE) + (IRQ_CB)->offset_mask_reg))

#define M2R_IRQ_SET_MASK_REG(IRQ_CB, DEVADDR, SLICE, VAL)               \
          M2R_REG_WRITE((volatile cs_reg*)((DEVADDR)+                    \
          ((SLICE) * M2R_SLICE_SIZE) + (IRQ_CB)->offset_mask_reg), (VAL))


/* Read the Status register value */
#define M2R_IRQ_GET_STATUS_REG_VAL(IRQ_CB, DEVADDR, SLICE)              \
            M2R_REG_READ((volatile cs_reg*)((DEVADDR)+                   \
              ((SLICE) * M2R_SLICE_SIZE) + (IRQ_CB)->offset_status_reg))

#define M2R_IRQ_SET_STATUS_REG(IRQ_CB, DEVADDR, SLICE, VAL)              \
         M2R_REG_WRITE((volatile cs_reg*)((DEVADDR)+                      \
         ((SLICE) * M2R_SLICE_SIZE) + (IRQ_CB)->offset_status_reg), (VAL))


/*************************************************************************
 * SPECIAL IRQ HANDLER:
 *
 * To provide flexibility to do some special processing from the generic
 * group handler, the individual block IRQ tables can have either
 * M2R_IRQ_FLG_SPL_INT or M2R_IRQ_FLG_SPL_CLR flag set for the leaf group
 * entry. The special handler will be then called for additional
 * processing or clearing of the interrupt.
 *
 * The special handler returns a status(bit-map of flags) which the
 * generic handler uses.
 ************************************************************************/
/* Return status from special handler */
/* -nothing for now- */

/* Special handler calling option */
#define M2R_IRQ_SPL_CLR                 (0x0010)
#define M2R_IRQ_SPL_INT                 (0x0020)
/* #define M2R_IRQ_SPL_ENBL                (0x0040)  *unused* */
/* #define M2R_IRQ_SPL_DISBL               (0x0080)  *unused* */

typedef struct {
  cs_uint16         irq_oper ;
  cs_uint16         port_id ;
  union {
    struct {
      m2r_irq_info_t  * irq_info ;
      cs_uint16         irq_reg_val ;
    } irq_hndl ;

    struct {
      cs_uint16     grp_id ;
      cs_uint16     spl_clr_bits ;

    } irq_clr ;
  } op_mode ;
} m2r_irq_spl_info_t ;


/*************************************************************************
 * macro to fill special IRQ clear related info for the handler.
 ************************************************************************/
#define M2R_IRQ_FILL_SPL_CLR_INFO(P_INFO, PORTID, GRPID, SPL_CLR_BITS)  \
{                                                                       \
  (P_INFO)->irq_oper = M2R_IRQ_SPL_CLR ;                                \
  (P_INFO)->port_id = (PORTID) ;                                        \
  (P_INFO)->op_mode.irq_clr.grp_id  = (GRPID) ;                         \
  (P_INFO)->op_mode.irq_clr.spl_clr_bits = (SPL_CLR_BITS) ;             \
}

/*************************************************************************
 * macro to fill special IRQ related info for the handler.
 ************************************************************************/
#define M2R_IRQ_FILL_SPL_INT_INFO(P_INFO, P_IRQ_INFO, PORTID, IRQ_VAL)  \
{                                                                       \
  (P_INFO)->irq_oper = M2R_IRQ_SPL_INT ;                                \
  (P_INFO)->port_id  = (PORTID) ;                                       \
  (P_INFO)->op_mode.irq_hndl.irq_info = (P_IRQ_INFO) ;                  \
  (P_INFO)->op_mode.irq_hndl.irq_reg_val = (IRQ_VAL) ;                  \
}

/*************************************************************************
 * SPECIAL IRQ HANDLER PROTOTYPE
 ************************************************************************/
typedef cs_uint16 (*M2R_IRQ_SPL_HNDLR)(m2r_irq_spl_info_t * irq_spl_info) ;

/*************************************************************************
 * IRQ CONTROL BLOCK FOR MODULE
 ************************************************************************/
typedef struct {
  const m2r_irq_ctl_t *     tbl ;
  const char **             name_tbl ;
  cs_uint16                 tbl_size ;
  M2R_IRQ_SPL_HNDLR         spl_hndlr ;
  const m2r_irq_ctl_t *     imed_tbl ;     /* for intermediate int level */
  const char **             imed_name_tbl ;
  cs_uint16                 imed_tbl_size ;
  M2R_IRQ_SPL_HNDLR         imed_spl_hndlr ;
} m2r_blk_irq_cb_t ;

/*************************************************************************
 * PORT IRQ CONTROL BLOCK
 ************************************************************************/
typedef struct {
  m2r_blk_irq_cb_t          mpif ;
  m2r_blk_irq_cb_t          spi42 ;
  m2r_blk_irq_cb_t          pprbs ;
  m2r_blk_irq_cb_t          framer ;
  m2r_blk_irq_cb_t          rmac ;
  m2r_blk_irq_cb_t          lif ;
  m2r_blk_irq_cb_t          eth ;
  m2r_blk_irq_cb_t          xgxs ;
} m2r_port_irq_cb_t ;

/********************
 * irq id name      *
 ********************/
typedef struct {
   cs_uint16    id ;
   const char * name ;
} m2r_irq_id_name_t ;

CS_INLINE
const char * M2R_GET_IRQ_ID_NAME(const m2r_irq_id_name_t * tbl,
				     cs_uint16 max,
				     cs_uint16 irq_id)
{
  cs_uint16   ii ;

  for (ii = 0; ii < max; ii++) {
    if (tbl[ii].id == irq_id) {
      return( tbl[ii].name ) ;
    }
  }

  return("????") ;

#ifdef CS_DONT_USE_INLINE
  if ( 0 ) { /* don't modify this - this keeps compiler happy */
    M2R_GET_IRQ_ID_NAME(tbl, max, irq_id) ;
  }
#endif
}


/*********************************************************
 * IRQ Private APIs
 *********************************************************/
extern
cs_status M2R_REGISTER_IRQ_TBL(cs_uint16 port_id,
			       m2r_module_id_t mod_id,
			       const m2r_irq_ctl_t * tbl,
			       const char ** name_tbl,
			       cs_uint16  tbl_size,
			       M2R_IRQ_SPL_HNDLR irq_spl_hndlr) ;

extern
cs_status M2R_UNREGISTER_IRQ_TBL(cs_uint16 port_id,
				 m2r_module_id_t mod_id) ;
extern
cs_status M2R_REGISTER_IMED_TBL(cs_uint16 port_id,
			       m2r_module_id_t mod_id,
			       const m2r_irq_ctl_t * tbl,
			       const char ** imed_name_tbl,
			       cs_uint16  tbl_size,
			       M2R_IRQ_SPL_HNDLR irq_spl_hndlr) ;

extern
cs_status M2R_UNREGISTER_IMED_TBL(cs_uint16 port_id,
				 m2r_module_id_t mod_id) ;
extern
cs_status m2r_irq_grp_hndlr(cs_uint16 port_id, cs_uint32 dev_addr,
                            cs_uint32 mod_id, cs_uint32 grp_id,
                            const m2r_irq_ctl_t * irq_ctl,
			    const char * grp_name,
			    const m2r_irq_id_name_t * irq_id_name_tbl,
			    cs_uint16 num_irq_ids) ;

extern
cs_status m2r_irq_dump_tbl_entry(cs_uint16 port_id,
                                 cs_uint32 dev_addr,
                                 const m2r_irq_ctl_t * irq_ctl) ;

extern
cs_status m2r_enable_imed_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                              cs_uint32 imed_id, cs_uint16 irq_enbl_bits) ;

extern
cs_status m2r_disable_imed_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                               cs_uint32 imed_id, cs_uint16 irq_disabl_bits) ;
#endif /* __M2R_IRQ_PRIV_H__ */





















