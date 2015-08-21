/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_isr.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"

#include "arn5_irq_priv.h"
#include "arn5_irq.h"

#include "arn5_mpif_irq.h"
#include "arn5_mpif_priv.h"
#include "arn5_pprbs_irq.h"
#include "arn5_pprbs_priv.h"
#include "arn5_spi_irq.h"
#include "arn5_spi_priv.h"
#include "arn5_eth_irq.h"
#include "arn5_eth_priv.h"
#include "arn5_lif_irq.h"
#include "arn5_lif_priv.h"
#include "arn5_framer_irq.h"
#include "arn5_framer_priv.h"
/*
Warning: calling these functions even with a device level
module id still requires a port_id

print_irq_info
arn5_irq_get_blk_cb
arn5_clear_leaf_irq
arn5_set_blk_irq
arn5_irq_grp_hndlr
ARN5_REGISTER_IRQ_TBL
ARN5_UNREGISTER_IRQ_TBL
ARN5_REGISTER_IMED_TBL
ARN5_UNREGISTER_IMED_TBL
arn5_irq_dump_tbl_entry
arn5_enable_imed_irq
arn5_disable_imed_irq
*/

#define ARN5_MOD_LOCK(is_dev, dpid, mod_id)     \
  if (is_dev) {                                 \
    ARN5_DEV_LOCK(dpid, mod_id) ;               \
  }                                             \
  else {                                        \
    ARN5_PORT_LOCK(dpid, mod_id) ;              \
  }

#define ARN5_MOD_UNLOCK(is_dev, dpid, mod_id)   \
  if (is_dev) {                                 \
    ARN5_DEV_UNLOCK(dpid, mod_id) ;             \
  }                                             \
  else {                                        \
    ARN5_PORT_UNLOCK(dpid, mod_id) ;            \
  }

/***********************************************************************
 * Internal APIs
 ***********************************************************************/
#ifndef RELEASE_PLATFORM
static
void  print_irq_info(cs_uint16 dpid, arn5_irq_info_t * pIrq_info) ;
#endif

static
arn5_blk_irq_cb_t * arn5_irq_get_blk_cb(cs_uint16 dpid,
                        arn5_module_id_t mod_id) ;

static
cs_status arn5_clear_leaf_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_clr_bits) ;

static
cs_status arn5_set_blk_irq(cs_uint16 port_id,
                        cs_uint32 mod_set_bits, cs_boolean enable)
{
  ARN5_t        * pDev ;
  cs_uint8      dev_id ;
  cs_uint16     sl_num ;
  cs_status     ret = CS_ERROR ;

  ARN5_MPIF_GlobalIntEnable_t   irq_enbl ;
  ARN5_MPIF_Channel0IntEnable_t channelIntEnable ;
  volatile cs_reg               * pReg ;

  cs_uint32 dev_mod_set_bits  = mod_set_bits & ARN5_DEV_SHRD_BLKS_ID ;
  cs_uint32 port_mod_set_bits = mod_set_bits & ARN5_PORT_BLKS_ID ;

  dev_id = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;
  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;

  irq_enbl.wrd = CS_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;

  if ( dev_mod_set_bits & ARN5_ID_HOST_IF ) {
    irq_enbl.bf.SPI4P2E = enable ? 1 : 0 ;
  }
  if ( dev_mod_set_bits & ARN5_ID_MPIF ) {
    irq_enbl.bf.MPIFE = enable ? 1 : 0 ;
    irq_enbl.bf.GPIOE = enable ? 1 : 0 ;
  }

  CS_REG_WRITE(&pDev->MicroIF.GlobalIntEnable.wrd, irq_enbl.wrd) ;

  if (port_mod_set_bits == 0) {
    ret = CS_OK ;
    goto SET_IRQ_EXIT ;
  }

  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  pReg = (cs_reg *) &pDev->MicroIF.Channel0IntEnable.wrd + sl_num * 2 ;
  channelIntEnable.wrd = CS_REG_READ(pReg) ;

  if (port_mod_set_bits & ARN5_ID_FRAMER) {
    channelIntEnable.bf.Framer0E = enable ? 1 : 0 ;
  }

  if (port_mod_set_bits & ARN5_ID_ETH) {
    channelIntEnable.bf.GIGE0E = enable ? 1 : 0 ;
  }

  if (port_mod_set_bits & ARN5_ID_LIF) {
    channelIntEnable.bf.LIF0E = enable ? 1 : 0 ;
  }

  if (port_mod_set_bits & ARN5_ID_PPRBS) {
    channelIntEnable.bf.PPRBS0E = enable ? 1 : 0 ;
  }

  CS_REG_WRITE(pReg, channelIntEnable.wrd) ;

  /* everything looks good */
  ret = CS_OK ;

SET_IRQ_EXIT :

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;
  return (ret) ;
}


/************************************************************************/
/* $rtn_hdr_start     ENABLE PORT IRQ                                   */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_enable_irq(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable the Port Level IRQ.                                           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     sl_num ;
  cs_uint8      devId ;
  ARN5_t        * pDev ;
  ARN5_MPIF_GlobalIntEnable_t   port_irq ;

  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;

  ARN5_DEV_LOCK(devId, ARN5_ID_MPIF) ;

  port_irq.wrd = CS_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;
  switch (sl_num) {
    case 0 :
      port_irq.bf.Channel0E = 1 ;
      break ;
    case 1 :
      port_irq.bf.Channel1E = 1 ;
      break ;
    case 2 :
      port_irq.bf.Channel2E = 1 ;
      break ;
    case 3 :
      port_irq.bf.Channel3E = 1 ;
      break ;
    case 4 :
      port_irq.bf.Channel4E = 1 ;
      break ;
    case 5 :
      port_irq.bf.Channel5E = 1 ;
      break ;
    case 6 :
      port_irq.bf.Channel6E = 1 ;
      break ;
    case 7 :
      port_irq.bf.Channel7E = 1 ;
      break ;
  }

  CS_REG_WRITE(&pDev->MicroIF.GlobalIntEnable.wrd, port_irq.wrd) ;

  ARN5_DEV_UNLOCK(devId, ARN5_ID_MPIF) ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start    DISABLE PORT IRQ                                   */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_disable_irq(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Disable the Port Level IRQ.                                          */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     sl_num ;
  cs_uint8      devId ;
  ARN5_t        * pDev ;
  ARN5_MPIF_GlobalIntEnable_t   port_irq ;

  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;

  ARN5_DEV_LOCK(devId, ARN5_ID_MPIF) ;

  port_irq.wrd = CS_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;
  switch (sl_num) {
    case 0 :
      port_irq.bf.Channel0E = 0 ;
      break ;
    case 1 :
      port_irq.bf.Channel1E = 0 ;
      break ;
    case 2 :
      port_irq.bf.Channel2E = 0 ;
      break ;
    case 3 :
      port_irq.bf.Channel3E = 0 ;
      break ;
    case 4 :
      port_irq.bf.Channel4E = 0 ;
      break ;
    case 5 :
      port_irq.bf.Channel5E = 0 ;
      break ;
    case 6 :
      port_irq.bf.Channel6E = 0 ;
      break ;
    case 7 :
      port_irq.bf.Channel7E = 0 ;
      break ;
  }

  CS_REG_WRITE(&pDev->MicroIF.GlobalIntEnable.wrd, port_irq.wrd) ;

  ARN5_DEV_UNLOCK(devId, ARN5_ID_MPIF) ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start     ENABLE BLOCK IRQ                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_enable_blk_irq(cs_uint16 port_id,
                        cs_uint32 mod_enbl_bits)
/* INPUTS     : o Port Id                                               */
/*              o Module ID bitmask                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable the BLOCK level IRQ for the modules specified in the bimask   */
/* [mod_enbl_bits] specify the modules for which the IRQ is to be       */
/* enabled. The Mask is the Logical OR of the individual Module's ID.   */
/*                                                                      */
/* The [mod_enbl_bits] parameter can be logical Mask of the values      */
/* defined in ARN5_PORT_BLKS_ID, i.e.                                   */
/* ARN5_ID_FRAMER, ARN5_ID_ETH, ARN5_ID_LIF, ARN5_ID_PPRBS              */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (mod_enbl_bits & ~ARN5_PORT_BLKS_ID) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_blk_irq(port_id, mod_enbl_bits, TRUE) ;
}


/************************************************************************/
/* $rtn_hdr_start    DISABLE BLOCK IRQ                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_disable_blk_irq(cs_uint16 port_id,
                        cs_uint32 mod_disabl_bits)
/* INPUTS     : o Port Id                                               */
/*              o Module ID bitmask                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Disable the BLOCK level IRQ for the modules specified in the bimask  */
/* [mod_enbl_bits] specify the modules for which the IRQ is to be       */
/* disabled. The Mask is the Logical OR of the individual Module's ID.  */
/*                                                                      */
/* The [mod_enbl_bits] parameter can be logical Mask of the values      */
/* defined in ARN5_PORT_BLKS_ID, i.e.                                   */
/* ARN5_ID_FRAMER, ARN5_ID_ETH, ARN5_ID_LIF, ARN5_ID_PPRBS              */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (mod_disabl_bits & ~ARN5_PORT_BLKS_ID) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_blk_irq(port_id, mod_disabl_bits, FALSE) ;
}


/************************************************************************/
/* $rtn_hdr_start    GET BLOCK IRQ ENABLE MASK                          */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_uint32 arn5_port_get_blk_irq_enbl(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : Bitmask of the Blocks for which IRQ is Enabled          */
/* DESCRIPTION:                                                         */
/* Get the bitmask of the blocks for which the IRQ is enabled.          */
/* The return value contains the bitmask of the modules, it could be    */
/* Logical OR mask of any of the modules defined in ARN5_PORT_BLKS_ID.  */
/* i.e. ARN5_ID_FRAMER, ARN5_ID_ETH, ARN5_ID_LIF, ARN5_ID_PPRBS         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     mod_enbl_bits = 0 ;
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;

  ARN5_MPIF_Channel0IntEnable_t channelIntEnable ;
  volatile cs_reg               * pReg ;

  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;
  pReg = (cs_reg *) &pDev->MicroIF.Channel0IntEnable.wrd + sl_num * 2 ;
  channelIntEnable.wrd = CS_REG_READ(pReg) ;

  if (channelIntEnable.bf.Framer0E) {
    mod_enbl_bits |= ARN5_ID_FRAMER ;
  }

  if (channelIntEnable.bf.GIGE0E) {
    mod_enbl_bits |= ARN5_ID_ETH ;
  }

  if (channelIntEnable.bf.LIF0E) {
    mod_enbl_bits |= ARN5_ID_LIF ;
  }

  if (channelIntEnable.bf.PPRBS0E) {
    mod_enbl_bits |= ARN5_ID_PPRBS ;
  }

  return (mod_enbl_bits) ;
}


/************************************************************************/
/* $rtn_hdr_start     ENABLE DEVICE BLOCK IRQ                           */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_enable_blk_irq(cs_uint8 dev_id,
                        cs_uint32 mod_enbl_bits)
/* INPUTS     : o Device Id                                             */
/*              o Module ID bitmask                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable the BLOCK level IRQ for the modules specified in the bimask   */
/* [mod_enbl_bits] specify the modules for which the IRQ is to be       */
/* enabled. The Mask is the Logical OR of the individual Module's ID.   */
/*                                                                      */
/* The [mod_enbl_bits] parameter can be logical Mask of the values      */
/* defined in ARN5_DEV_SHRD_BLKS_ID, i.e.                               */
/* ARN5_ID_HOST_IF, ARN5_ID_MPIF                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (mod_enbl_bits & ~ARN5_DEV_SHRD_BLKS_ID) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_blk_irq(ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, 0),
                         mod_enbl_bits, TRUE) ;
}


/************************************************************************/
/* $rtn_hdr_start    DISABLE DEVICE BLOCK IRQ                           */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_disable_blk_irq(cs_uint8 dev_id,
                        cs_uint32 mod_disabl_bits)
/* INPUTS     : o Device Id                                             */
/*              o Module ID bitmask                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Disable the BLOCK level IRQ for the modules specified in the bimask  */
/* [mod_enbl_bits] specify the modules for which the IRQ is to be       */
/* disabled. The Mask is the Logical OR of the individual Module's ID.  */
/*                                                                      */
/* The [mod_enbl_bits] parameter can be logical Mask of the values      */
/* defined in ARN5_DEV_SHRD_BLKS_ID, i.e.                               */
/* ARN5_ID_HOST_IF, ARN5_ID_MIF                                         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (mod_disabl_bits & ~ARN5_DEV_SHRD_BLKS_ID) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_blk_irq(ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, 0),
                         mod_disabl_bits, FALSE) ;
}


/************************************************************************/
/* $rtn_hdr_start    GET DEVICE BLOCK IRQ ENABLE MASK                   */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_uint32 arn5_dev_get_blk_irq_enbl(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : Bitmask of the Blocks for which IRQ is Enabled          */
/* DESCRIPTION:                                                         */
/* Get the bitmask of the blocks for which the IRQ is enabled.          */
/* The return value contains the bitmask of the modules, it could be    */
/* Logical OR mask of any of the modules defined in                     */
/* ARN5_DEV_SHRD_BLKS_ID, i.e.                                          */
/* ARN5_ID_HOST_IF, ARN5_ID_MIF                                         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     mod_enbl_bits = 0 ;
  ARN5_t        * pDev ;

  ARN5_MPIF_GlobalIntEnable_t      irq_enbl ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  irq_enbl.wrd = CS_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;

  if (irq_enbl.bf.SPI4P2E) {
    mod_enbl_bits |= ARN5_ID_HOST_IF ;
  }

  if (irq_enbl.bf.GPIOE || irq_enbl.bf.MPIFE ) {
    mod_enbl_bits |= ARN5_ID_MPIF ;
  }

  return (mod_enbl_bits) ;
}


static
cs_status arn5_set_leaf_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_set_bits,
                        cs_boolean enable)
{
  arn5_blk_irq_cb_t     * blk_irq_cb ;
  cs_uint32             sl_num ;
  const arn5_irq_ctl_t  * irq_cb ;
  ARN5_t                * pDev ;
  volatile cs_reg       * irq_mask_reg ;
  volatile cs_reg       irq_mask ;

  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;
  cs_uint16  port_id = is_dev ? ARN5_DEV_PORT_NUM_TO_PORT_ID(dpid, 0) : dpid ;
  cs_uint8   dev_id = is_dev ? dpid : ARN5_PORT_ID_TO_DEV_ID(port_id) ;

  if (is_dev) {
    ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;
  }
  else {
    ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  }

  if ((mod_id & ~ARN5_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id)) == NULL) ||
     (blk_irq_cb->tbl == NULL)) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_TBL_NULL : EARN5_PORT_TBL_NULL,
                  "IRQ table pointer is NULL for %s group_id %d\n",
                  ARN5_GET_MOD_NAME(mod_id), grp_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (grp_id >= blk_irq_cb->tbl_size) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_RANGE_ERR : EARN5_PORT_RANGE_ERR,
                  "Table size is : %d for %s \n",
                  blk_irq_cb->tbl_size, ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to arn5_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->tbl + grp_id ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;

  if (sl_num > irq_cb->num_of_strides ) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_RANGE_ERR : EARN5_PORT_RANGE_ERR,
                  "Num of strides for %s is %d <= slice %d\n",
                  ARN5_GET_MOD_NAME(mod_id), irq_cb->num_of_strides, sl_num ) ;
    return (CS_ERROR) ;
  }

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_set_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit(s) may be used. "
             "Yours: 0x%04x Valid: 0x%04x\n",
             irq_set_bits, irq_cb->irq_map) ;
  }

  irq_mask_reg = (volatile cs_reg *)(((cs_uint32) pDev) +
                 (sl_num * irq_cb->stride_size) + irq_cb->offset_mask_reg) ;

  ARN5_MOD_LOCK(is_dev, dpid, mod_id) ;
  irq_mask = CS_REG_READ(irq_mask_reg) ;

  /* if setting 0 means disable */
  if ( irq_cb->flags & ARN5_IRQ_FLG_1_TO_ENABLE ) {
    if (enable) {
      irq_mask |= (irq_set_bits) ;
    }
    else {
      irq_mask = (irq_mask & (~irq_set_bits)) ;
    }
  }
  else { /* 1 means disable */
    if (enable) {
      irq_mask = (irq_mask & (~irq_set_bits)) ;
    }
    else {
      irq_mask |= (irq_set_bits) ;
    }
  }
  CS_REG_WRITE(irq_mask_reg, irq_mask) ;
  ARN5_MOD_UNLOCK(is_dev, dpid, mod_id) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start    ENABLE INTERRUPT SOURCE                            */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_enable_leaf_irq(cs_uint16 port_id, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_enbl_bits)

/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be enabled             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API enables the IRQs. Group ID specifies the IRQ group. IRQ     */
/* group refers to a IRQ register for instance register the particular  */
/* mod. Exact group definitions can be found in individual module's     */
/* header files. irq_enabl_bits is the bitmask composed of the          */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_enbl_bits is the mask of IRQ leaves ( sources ) . for exact      */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_enbl_bits identifies IRQ source.        */
/*                                                                      */
/* The [mod_id] parameter is specified in ARN5_PORT_BLKS_ID, i.e.       */
/* i.e. ARN5_ID_FRAMER, ARN5_ID_ETH, ARN5_ID_LIF, ARN5_ID_PPRBS         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (mod_id & ~ARN5_PORT_BLKS_ID) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_leaf_irq(port_id, mod_id, grp_id, irq_enbl_bits, TRUE) ;
}


/************************************************************************/
/* $rtn_hdr_start    DISABLE INTERRUPT SOURCE                           */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_disable_leaf_irq(cs_uint16 port_id, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_disabl_bits)

/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be disabled            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API disables the IRQs. Group ID specifies the IRQ group. IRQ    */
/* group refers to a IRQ register for instance register in a particular */
/* mod. Exact group definitions can be found in individual module's     */
/* header files. irq_disabl_bits is the bitmask composed of the         */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_disabl_bits is the mask of IRQ leaves ( sources ) . for exact    */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_disabl_bits identifies IRQ source.      */
/*                                                                      */
/* The [mod_id] parameter is specified in ARN5_PORT_BLKS_ID, i.e.       */
/* i.e. ARN5_ID_FRAMER, ARN5_ID_ETH, ARN5_ID_LIF, ARN5_ID_PPRBS         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (mod_id & ~ARN5_PORT_BLKS_ID) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_leaf_irq(port_id, mod_id, grp_id, irq_disabl_bits, 0) ;
}


/************************************************************************/
/* $rtn_hdr_start    CLEAR INTERRUPT SOURCE                             */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_leaf_irq(cs_uint16 port_id, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_clr_bits)

/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be cleared             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API clears the IRQs. Group ID specifies the IRQ group. IRQ      */
/* group refers to a IRQ register for instance register in a particular */
/* mod. Exact group definitions can be found in individual module's     */
/* header files. irq_clr_bits is the bitmask composed of the            */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_clr_bits is the mask of IRQ leaves ( sources ) . for exact       */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_clr_bits identifies IRQ source.         */
/*                                                                      */
/* The [mod_id] parameter is specified in ARN5_PORT_BLKS_ID, i.e.       */
/* i.e. ARN5_ID_FRAMER, ARN5_ID_ETH, ARN5_ID_LIF, ARN5_ID_PPRBS         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (mod_id & ~ARN5_PORT_BLKS_ID) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_clear_leaf_irq(port_id, mod_id, grp_id, irq_clr_bits) ;
}


/************************************************************************/
/* $rtn_hdr_start    ENABLE INTERRUPT SOURCE                            */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_enable_leaf_irq(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_enbl_bits)

/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be enabled             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API enables the IRQs. Group ID specifies the IRQ group. IRQ     */
/* group refers to a IRQ register for instance register in a particular */
/* mod. Exact group definitions can be found in individual module's     */
/* header files. irq_enabl_bits is the bitmask composed of the          */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_enbl_bits is the mask of IRQ leaves ( sources ) . for exact      */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_enbl_bits identifies IRQ source.        */
/*                                                                      */
/* The [mod_id] parameter is specified in ARN5_DEV_SHRD_BLKS_ID, i.e.   */
/* ARN5_ID_HOST_IF, ARN5_ID_MPIF                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (mod_id & ~ARN5_DEV_SHRD_BLKS_ID) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_leaf_irq(dev_id, mod_id, grp_id, irq_enbl_bits, TRUE) ;
}


/************************************************************************/
/* $rtn_hdr_start    DISABLE INTERRUPT SOURCE                           */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_disable_leaf_irq(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_disabl_bits)

/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be disabled            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API disables the IRQs. Group ID specifies the IRQ group. IRQ    */
/* group refers to a IRQ register for instance register in a particular */
/* mod. Exact group definitions can be found in individual module's     */
/* header files. irq_disabl_bits is the bitmask composed of the         */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_disabl_bits is the mask of IRQ leaves ( sources ) . for exact    */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_disabl_bits identifies IRQ source.      */
/*                                                                      */
/* The [mod_id] parameter is specified in ARN5_DEV_SHRD_BLKS_ID, i.e.   */
/* ARN5_ID_HOST_IF, ARN5_ID_MPIF                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (mod_id & ~ARN5_DEV_SHRD_BLKS_ID) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_set_leaf_irq(dev_id, mod_id, grp_id, irq_disabl_bits, FALSE) ;
}


/************************************************************************/
/* $rtn_hdr_start    CLEAR INTERRUPT SOURCE                             */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_leaf_irq(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_clr_bits)

/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be cleared             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API clears the IRQs. Group ID specifies the IRQ group. IRQ      */
/* group refers to a IRQ register for instance register in a particular */
/* mod. Exact group definitions can be found in individual module's     */
/* header files. irq_clr_bits is the bitmask composed of the            */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_clr_bits is the mask of IRQ leaves ( sources ) . for exact       */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_clr_bits identifies IRQ source.         */
/*                                                                      */
/* The [mod_id] parameter is specified in ARN5_DEV_SHRD_BLKS_ID, i.e.   */
/* ARN5_ID_HOST_IF, ARN5_ID_MPIF                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  if (mod_id & ~ARN5_DEV_SHRD_BLKS_ID) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
      ": invalid block id(s)\n") ;
    return (CS_ERROR) ;
  }

  return arn5_clear_leaf_irq(dev_id, mod_id, grp_id, irq_clr_bits) ;
}


static
cs_status arn5_clear_leaf_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                        cs_uint32 grp_id, cs_uint16 irq_clr_bits)
{
  arn5_blk_irq_cb_t     * blk_irq_cb ;
  cs_uint32             sl_num ;
  const arn5_irq_ctl_t  * irq_cb ;
  ARN5_t                * pDev ;
  volatile cs_reg       * irq_reg ;
  volatile cs_reg       clear_irq ;

  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;
  cs_uint16  port_id = is_dev ? ARN5_DEV_PORT_NUM_TO_PORT_ID(dpid, 0) : dpid ;
  cs_uint8   dev_id = is_dev ? dpid : ARN5_PORT_ID_TO_DEV_ID(port_id) ;

  if (is_dev) {
    ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;
  }
  else {
    ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  }

  if ((mod_id & ~ARN5_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id)) == NULL) ||
     (blk_irq_cb->tbl == NULL)) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_TBL_NULL : EARN5_PORT_TBL_NULL,
                  "IRQ table pointer is NULL for %s group_id %d\n",
                  ARN5_GET_MOD_NAME(mod_id), grp_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (grp_id >= blk_irq_cb->tbl_size) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_RANGE_ERR : EARN5_PORT_RANGE_ERR,
                  "Table size is : %d for %s \n",
                  blk_irq_cb->tbl_size, ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to arn5_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->tbl + grp_id ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;

  if (sl_num > irq_cb->num_of_strides ) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_RANGE_ERR : EARN5_PORT_RANGE_ERR,
                  "Num of strides for %s is %d <= slice %d\n",
                  ARN5_GET_MOD_NAME(mod_id), irq_cb->num_of_strides, sl_num ) ;
    return (CS_ERROR) ;
  }

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_clr_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit(s) may be used. "
             "Yours: 0x%04x Valid: 0x%04x\n",
             irq_clr_bits, irq_cb->irq_map) ;
  }

  ARN5_MOD_LOCK(is_dev, dpid, mod_id) ;

  if ( ( irq_cb->flags & ARN5_IRQ_FLG_SPL_CLR ) &&
       ( irq_clr_bits & irq_cb->spl_map)) {
    arn5_irq_spl_info_t irq_spl_info ;
    arn5_blk_irq_cb_t * blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id) ;

    if (blk_irq_cb && blk_irq_cb->spl_hndlr) {
      ARN5_IRQ_FILL_SPL_CLR_INFO(&irq_spl_info, dpid, grp_id, irq_clr_bits) ;
      blk_irq_cb->spl_hndlr(&irq_spl_info) ;
    }
    /* Special handler should mask the bits taken care by itself */
    irq_clr_bits = irq_spl_info.op_mode.irq_clr.spl_clr_bits ;
  }

  irq_reg = (volatile cs_reg *)(((cs_uint32) pDev) +
                 (sl_num * irq_cb->stride_size) + irq_cb->offset_intr_reg) ;

  if ( irq_cb->flags & ARN5_IRQ_FLG_RW1C ) { /* RWIC */
    clear_irq = (irq_clr_bits & irq_cb->irq_map) ;
    CS_REG_WRITE(irq_reg, clear_irq) ;
  }
  else {
    CS_PRINT("%s:%s 0x%04x, mod_id 0x%x, grp_id 0x%x: SW ERROR - cannot clear interrupt!\n",
              __FUNCTION__, (is_dev ? "dev_id" : "port_id"), dpid, mod_id, grp_id) ;
    return (CS_ERROR) ;
  }

  ARN5_MOD_UNLOCK(is_dev, dpid, mod_id) ;

  return (CS_OK) ;
}


/************************************************************************/
/*                    COMMON GROUP IRQ HANDLER                          */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_irq_grp_hndlr(cs_uint16 dpid, cs_uint32 dev_addr,
                        cs_uint32 mod_id, cs_uint32 grp_id,
                        const arn5_irq_ctl_t * irq_ctl,
                        const cs_uint8 * grp_name,
                        const arn5_irq_id_name_t * irq_id_name_tbl,
                        cs_uint16 num_irq_ids)
/* INPUTS     : o Device or Port Id                                     */
/*              o Device base address                                   */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o Pointer to IRQ Group control block                    */
/*              o Interrupt Group Id Name                               */
/*              o Pointer to IRQ Id name table                          */
/*              o Number of IRQ Id's                                    */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This is common IRQ group handler function. It reads the IRQs in the  */
/* group passed and notifies the user though CS_IRQ_USER_HANDLER        */
/* about the IRQ and its status. The purpose of this function is to     */
/* provide most of the common functionality needed in processing IRQ.   */
/* It walks through a IRQ group and processes any interrupts that are   */
/* out-standing. CS_IRQ_USER_HANDLER is called to provide user specific */
/* functionality in the sense it can be re-written by customer to map   */
/* to their environment.                                                */
/************************************************************************/
{
  volatile cs_reg   irq_reg_val, irq_clr_mask ;
  volatile cs_reg   status_reg_val = 0, irq_mask_reg_val = 0 ;
  cs_uint16         iter, irq_id ;
  arn5_irq_info_t   irq_info ;
  cs_uint8          sl_num = 0 ;
  cs_uint16         spl_proc_status = 0 ;
  arn5_blk_irq_cb_t * blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id) ;

  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;

  if (irq_ctl == NULL) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_TBL_NULL : EARN5_PORT_TBL_NULL,
                  "IRQ control table is NULL\n" ) ;
    return (CS_ERROR) ;
  }

  if (!is_dev) {
    sl_num = ARN5_PORT_ID_TO_PORT_NUM(dpid) ;
  }

  irq_reg_val = (ARN5_IRQ_GET_INT_REG_VAL(irq_ctl, dev_addr,
                 sl_num, irq_ctl->stride_size) &
                 irq_ctl->irq_map) ;

  /* No IRQ pending in the group, bail out */
  if (!irq_reg_val) {
    goto RTN_EXIT ;
  }

  /* Store the Clear mask to clear the IRQ */
  irq_clr_mask = irq_reg_val ;
  irq_mask_reg_val = (ARN5_IRQ_GET_MASK_REG_VAL(irq_ctl, dev_addr,
		      sl_num, irq_ctl->stride_size) &
                      (irq_ctl->irq_map)) ;

  /*
   * Clear leaf interrupts first before any status is retrieved by the
   * interrupt handling code. The special handlers may later clear the
   * source of the interrupt but clear all the leaf interrupts here.
   */
  if ( irq_clr_mask &&
       (irq_ctl->flags & ARN5_IRQ_FLG_RW1C) ) {
    ARN5_IRQ_SET_INT_REG(irq_ctl, dev_addr, sl_num, irq_ctl->stride_size,
			 irq_clr_mask) ;
  }

  /* Mask out any interrupts that are active but were not enabled */
  if ( irq_ctl->flags & ARN5_IRQ_FLG_1_TO_ENABLE ) { /* if 1 to enable */
    irq_reg_val &= irq_mask_reg_val ;
  }
  else { /* value 0 means enable */
    irq_reg_val &= (~irq_mask_reg_val) ;
  }

  /*
   * Get IRQ status(as long as there is a corresponding status register).
   * The status (bit) specific to a particular irq_id will be included in
   * the irq info package to the application.
   */
  if (irq_ctl->offset_status_reg) {
    status_reg_val = ARN5_IRQ_GET_STATUS_REG_VAL(irq_ctl, dev_addr,
                       sl_num, irq_ctl->stride_size) ;
  }

  irq_info.chip         = CS_CHIP_ARSENAL5 ;
  irq_info.mod_id       = mod_id ;
  irq_info.grp_id       = grp_id ;
  irq_info.grp_name     = grp_name ;

  /* Process each pending interrupt in the leaf */
  for (iter = irq_reg_val; iter; ) {
    irq_id = (iter & (~(iter - 1))) ;
    irq_info.irq_id  = irq_id ;
    irq_info.irq_id_name = ARN5_GET_IRQ_ID_NAME(irq_id_name_tbl, num_irq_ids,
                                               irq_id) ;
    irq_info.flags   = is_dev ? CS_IRQ_INFO_SHRD_DEVICE_FLAG : 0 ;
    irq_info.status  = 0 ;
    spl_proc_status  = 0 ; /* value not used currently */

    /* Special clearing */
    if ( (irq_ctl->flags & ARN5_IRQ_FLG_SPL_CLR) &&
	 (irq_id & irq_ctl->spl_map) &&
	 (blk_irq_cb && blk_irq_cb->spl_hndlr) ) {
      arn5_irq_spl_info_t irq_spl_info ;

      ARN5_IRQ_FILL_SPL_CLR_INFO(&irq_spl_info, dpid, grp_id, irq_id) ;
      spl_proc_status |= blk_irq_cb->spl_hndlr(&irq_spl_info) ;
    }

    /* Special handling */
    if ( (irq_ctl->flags & ARN5_IRQ_FLG_SPL_INT) &&
	 (irq_id & irq_ctl->spl_map) &&
         (blk_irq_cb && blk_irq_cb->spl_hndlr) ) {
      arn5_irq_spl_info_t irq_spl_info ;

      ARN5_IRQ_FILL_SPL_INT_INFO(&irq_spl_info, &irq_info,
				 dpid, irq_reg_val) ;
      spl_proc_status |= blk_irq_cb->spl_hndlr(&irq_spl_info) ;
    }

    /* Check if special handler already included the status info.
     * Otherwise include it in irq_info.*/
    if ( !(irq_info.flags & ARN5_IRQ_INFO_STATUS_FLAG) ) {
      if (irq_ctl->offset_status_reg) {
	ARN5_IRQ_INFO_SET_STATUS(irq_info.flags) ;
	irq_info.status = (status_reg_val & irq_id) ? 1 : 0 ;
      }
    }

    /* Finally, pass on the packaged info for this irq_id to the user.*/
#ifndef RELEASE_PLATFORM
    print_irq_info(dpid, &irq_info) ;
#endif /* RELEASE_PLATFORM */

    CS_IRQ_USER_HANDLER(dpid, &irq_info) ;

    iter &= ~irq_id ;
  }

RTN_EXIT:
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start     INTERRUPT HANDLER ENTRY POINT                     */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_isr_handler(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This is the driver's top-level interrupt handler and main entry      */
/* point for handling ALL the interrupts for the specified device.      */
/*                                                                      */
/* The customer system software should call this ISR upon detecting     */
/* the interrupt on the INTN pin on the this chip.                      */
/*                                                                      */
/* This handler identifies all the pending interrupts for EACH of the   */
/* valid modules in this chip. It is done by traversing the interrupt   */
/* heirarchy. It calls individual module's IRQ routine if the module    */
/* has any pending IRQ to be serviced. Each module's ISR then processes */
/* any IRQ in its module and sending the notification to user provided  */
/* IRQ handler with information such as the port_id, IRQ Group Source   */
/* module ID's, IRQ leaf ID and its current status.                     */
/*                                                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  ARN5_t                        * pDev ;
  ARN5_MPIF_GlobalInt_t         gblInt ;
  ARN5_MPIF_GlobalIntEnable_t   irq_enbl ;
  cs_uint16                     port_id, i, mask ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  gblInt.wrd = CS_REG_READ(&pDev->MicroIF.GlobalInt.wrd) ;
  irq_enbl.wrd = CS_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;

  if ( ((irq_enbl.bf.MPIFE) && (gblInt.bf.MPIFI)) ||
       ((irq_enbl.bf.GPIOE) && (gblInt.bf.GPIOI)) ) {
    arn5_mpif_dev_isr(dev_id) ;
  }

  if ( (irq_enbl.bf.SPI4P2E) && (gblInt.bf.SPI4P2I) ) {
    arn5_spi_dev_isr(dev_id) ;
  }

  mask = 1 ;
  for (i = 0 ; i < ARN5_MAX_NUM_PORTS; i++, mask <<= 1 ) {
    if ( (irq_enbl.wrd & mask) && (gblInt.wrd & mask) ) {
      ARN5_MPIF_Channel0Int_t channel0Int ;
      ARN5_MPIF_Channel0IntEnable_t channel0IntEnable ;

      channel0Int.wrd = CS_REG_READ(
                (cs_reg *)&pDev->MicroIF.Channel0Int.wrd + (i * 2)) ;
      channel0IntEnable.wrd = CS_REG_READ(
                (cs_reg *)&pDev->MicroIF.Channel0IntEnable.wrd + (i * 2)) ;

      port_id = ARN5_DEV_PORT_NUM_TO_PORT_ID(dev_id, i) ;

      if ( (channel0Int.bf.Framer0I) && (channel0IntEnable.bf.Framer0E) ) {
        arn5_framer_isr(port_id) ;
      }
      if ( (channel0Int.bf.GIGE0I) && (channel0IntEnable.bf.GIGE0E) ) {
        arn5_eth_isr(port_id) ;
      }
      if ( (channel0Int.bf.LIF0I) && (channel0IntEnable.bf.LIF0E) ) {
        arn5_lif_isr(port_id) ;
      }
      if ( (channel0Int.bf.PPRBS0I) && (channel0IntEnable.bf.PPRBS0E) ) {
        arn5_pprbs_isr(port_id) ;
      }
    }
  }

  return ( CS_OK ) ;
}


#ifndef RELEASE_PLATFORM
static
void  print_irq_info(cs_uint16 dpid, arn5_irq_info_t * pIrq_info)
{
  cs_int8  * mod_name = ARN5_GET_MOD_NAME(pIrq_info->mod_id) ;
  cs_boolean is_dev = (pIrq_info->mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;

  CS_PRINT("=> IRQ(%s 0x%04x): %-8s %23s.%-15s(bit: 0x%04x)",
           is_dev ? "Dev" : "Port",
           dpid, mod_name, pIrq_info->grp_name,
           pIrq_info->irq_id_name,
           pIrq_info->irq_id ) ;

  if (pIrq_info->flags & ARN5_IRQ_INFO_STATUS_FLAG) {
    CS_PRINT(" Status:%d", pIrq_info->status) ;
  }
  if (pIrq_info->flags & ARN5_IRQ_INFO_PARAM1_FLAG) {
    CS_PRINT(" Param1:0x%08x", pIrq_info->param1) ;
  }
  if (pIrq_info->flags & ARN5_IRQ_INFO_PARAM2_FLAG) {
    CS_PRINT(" Param2:0x%08x", pIrq_info->param2) ;
  }
  CS_PRINT("\n") ;
  return  ;
}
#endif /* RELEASE_PLATFORM */


/*********************************************************
 * Private IRQ APIs
 *********************************************************/
cs_status ARN5_REGISTER_IRQ_TBL(cs_uint16 dpid,
                        arn5_module_id_t mod_id,
                        const arn5_irq_ctl_t * tbl,
                        const cs_uint8 ** name_tbl,
                        cs_uint16  tbl_size,
                        IRQ_SPL_HNDLR irq_spl_hndlr)
{
  arn5_blk_irq_cb_t * blk_irq_cb ;

  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;

  if (!tbl || (tbl_size == 0)) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_TBL_NULL : EARN5_PORT_TBL_NULL,
                  "Module ID : %d\n",
                  mod_id) ;
    return (CS_ERROR) ;
  }

  blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id) ;
  blk_irq_cb->tbl       = tbl ;
  blk_irq_cb->name_tbl  = name_tbl ;
  blk_irq_cb->tbl_size  = tbl_size ;
  blk_irq_cb->spl_hndlr = irq_spl_hndlr ;

  return (CS_OK) ;
}


cs_status ARN5_UNREGISTER_IRQ_TBL(cs_uint16 dpid,
                        arn5_module_id_t mod_id)
{
  arn5_blk_irq_cb_t * blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id) ;
  blk_irq_cb->tbl       = NULL ;
  blk_irq_cb->name_tbl  = NULL ;
  blk_irq_cb->tbl_size  = 0 ;
  blk_irq_cb->spl_hndlr = NULL ;

  return (CS_OK) ;
}


cs_status ARN5_REGISTER_IMED_TBL(cs_uint16 dpid,
                        arn5_module_id_t mod_id,
                        const arn5_irq_ctl_t * imed_tbl,
                        const cs_uint8 ** imed_name_tbl,
                        cs_uint16  imed_tbl_size,
                        IRQ_SPL_HNDLR imed_spl_hndlr)
{
  arn5_blk_irq_cb_t * blk_irq_cb ;

  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;

  if (!imed_tbl || (imed_tbl_size == 0)) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_TBL_NULL : EARN5_PORT_TBL_NULL,
                  "Module ID : %d\n",
                  mod_id) ;
    return (CS_ERROR) ;
  }

  blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id) ;
  blk_irq_cb->imed_tbl       = imed_tbl ;
  blk_irq_cb->imed_name_tbl  = imed_name_tbl ;
  blk_irq_cb->imed_tbl_size  = imed_tbl_size ;
  blk_irq_cb->imed_spl_hndlr = imed_spl_hndlr ;

  return (CS_OK) ;
}


cs_status ARN5_UNREGISTER_IMED_TBL(cs_uint16 dpid,
                        arn5_module_id_t mod_id)
{
  arn5_blk_irq_cb_t *  blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id) ;
  blk_irq_cb->imed_tbl       = NULL ;
  blk_irq_cb->imed_name_tbl  = NULL ;
  blk_irq_cb->imed_tbl_size  = 0 ;
  blk_irq_cb->imed_spl_hndlr = NULL ;

  return (CS_OK) ;
}

static
arn5_blk_irq_cb_t * arn5_irq_get_blk_cb(cs_uint16 dpid,
                        arn5_module_id_t mod_id)
{
  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;
  arn5_blk_irq_cb_t * blk_irq_cb = NULL ;

  if (is_dev) {
    arn5_dev_irq_cb_t * dev_irq_cb = ARN5_GET_DEV_IRQ_CB(dpid) ;

    if (!dev_irq_cb) {
      CS_HNDL_ERROR(dpid, EARN5_DEV_NULL_PTR, NULL) ;
      return (NULL) ;
    }
    switch ( mod_id ) {
      case ARN5_ID_MPIF:
        blk_irq_cb = &dev_irq_cb->mpif ;
        break ;
      case ARN5_ID_HOST_IF:
        blk_irq_cb = &dev_irq_cb->spi_host_if ;
        break ;
      default:
        break ;
    }
  }
  else {
    arn5_port_irq_cb_t * port_irq_cb = ARN5_GET_PORT_IRQ_CB(dpid) ;

    if (!port_irq_cb) {
      CS_HNDL_ERROR(dpid, EARN5_PORT_NOT_OPEN, NULL) ;
      return (NULL) ;
    }
    switch ( mod_id ) {
      case ARN5_ID_FRAMER:
        blk_irq_cb = &port_irq_cb->framer ;
        break ;
      case ARN5_ID_ETH:
        blk_irq_cb = &port_irq_cb->eth ;
        break ;
      case ARN5_ID_LIF:
        blk_irq_cb = &port_irq_cb->lif ;
        break ;
      case ARN5_ID_PPRBS:
        blk_irq_cb = &port_irq_cb->pprbs ;
        break ;

      default:
        break ;
    }
  }
  return (blk_irq_cb) ;
}


extern cs_boolean arn5_drvr_is_mask_zero_cntr(void) ;

cs_status arn5_irq_dump_tbl_entry(cs_uint16 dpid,  cs_boolean is_dev,
                        cs_uint32 dev_addr,
                        const arn5_irq_ctl_t * irq_ctl,
                        const cs_uint8 * label)
{
  volatile cs_reg       * irq_reg_addr, irq_reg_val = 0  ;
  volatile cs_reg       irq_mask_reg_val = 0, status_reg_val = 0 ;
  cs_uint8              sl_num = 0 ;

  if (!is_dev) {
    sl_num = ARN5_PORT_ID_TO_SLICE_NUM(dpid) ;
  }
  irq_reg_addr = (volatile cs_reg*)((sl_num * irq_ctl->stride_size)
    + irq_ctl->offset_intr_reg) ;

  irq_reg_val = (ARN5_IRQ_GET_INT_REG_VAL(irq_ctl, dev_addr, sl_num,
                                         irq_ctl->stride_size) &
                 irq_ctl->irq_map) ;
  irq_mask_reg_val = (ARN5_IRQ_GET_MASK_REG_VAL(irq_ctl, dev_addr, sl_num,
                                               irq_ctl->stride_size) &
                      irq_ctl->irq_map) ;

  if (irq_ctl->offset_status_reg) {
    status_reg_val = ARN5_IRQ_GET_STATUS_REG_VAL(irq_ctl, dev_addr, sl_num,
                                                irq_ctl->stride_size) ;
  }
  else {
    status_reg_val = 0 ;
  }

  if ((arn5_drvr_is_mask_zero_cntr() == FALSE) ||
       irq_reg_val || status_reg_val) {
    if (irq_ctl->offset_status_reg) {
      CS_PRINT("%-32s 0x%08x =>  0x%04x  %c  | 0x%04x  | 0x%04x\n",
             label,
             irq_reg_addr, irq_mask_reg_val,
             (irq_ctl->flags & ARN5_IRQ_FLG_1_TO_ENABLE) ? 'E' : 'M',
             irq_reg_val, status_reg_val) ;
    }
    else {
      CS_PRINT("%-32s 0x%08x =>  0x%04x  %c  | 0x%04x  | n/a\n",
             label,
             irq_reg_addr, irq_mask_reg_val,
             (irq_ctl->flags & ARN5_IRQ_FLG_1_TO_ENABLE) ? 'E' : 'M',
             irq_reg_val) ;
    }
  }

  return (CS_OK) ;
}


cs_status arn5_irq_dump_tbl(cs_uint16 dpid,
                        arn5_module_id_t mod_id,
                        arn5_blk_irq_cb_t * blk_cb)
{
  cs_uint32             dev_addr, ii ;
  const arn5_irq_ctl_t  * irq_ctl ;

  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;

  CS_PRINT("\nIRQ dump for %s on %s 0x%04x\n", ARN5_GET_MOD_NAME(mod_id),
           is_dev ? "Dev" : "Port", dpid) ;

  CS_PRINT(
"------------------------------------------------------------------------------\n"
" IRQ Group Name                  Reg Offset => Mask/Enable |  Value  | Status \n"
"------------------------------------------------------------------------------\n") ;

  dev_addr = (cs_uint32) (is_dev ? ARN5_DEV_ID_TO_DEV_BASE(dpid) :
    ARN5_PORT_ID_TO_DEV_BASE(dpid)) ;
  irq_ctl = blk_cb->tbl ;

  for (ii = 0; ii < blk_cb->tbl_size; ii++, irq_ctl++) {
    const cs_uint8 * str ;
    if (blk_cb->name_tbl != NULL) {
      str = (blk_cb->name_tbl)[ii] ;
    }
    else {
      str = " " ;
    }

    arn5_irq_dump_tbl_entry(dpid, is_dev, dev_addr, irq_ctl, str) ;
  }

  irq_ctl = blk_cb->imed_tbl ;
  if (irq_ctl == NULL) {
    return (CS_OK) ;
  }

  CS_PRINT(
 "-IMED------------------------------------------------------------------------\n") ;

  for (ii = 0; ii < blk_cb->imed_tbl_size; ii++, irq_ctl++) {
    const cs_uint8 * str ;
    if (blk_cb->imed_name_tbl != NULL) {
      str = (blk_cb->imed_name_tbl)[ii] ;
    }
    else {
      str = " " ;
    }

    arn5_irq_dump_tbl_entry(dpid, is_dev, dev_addr, irq_ctl, str) ;
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start    PRINT/DUMP ALL IRQ ENABLE AND STATUS REG VALUES    */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Private                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_irq_dump_all(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Print all the IRQs thats are enabled.                                */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  arn5_blk_irq_cb_t     *blk_cb ;

  const arn5_module_id_t mods[] = {
    ARN5_ID_FRAMER,
    ARN5_ID_ETH,
    ARN5_ID_LIF,
    ARN5_ID_PPRBS,
  } ;
  int ii ;

  ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  for (ii = 0; ii < sizeof(mods) / sizeof(mods[0]); ii++) {
    blk_cb = arn5_irq_get_blk_cb(port_id, mods[ii] ) ;
    if (blk_cb->tbl && blk_cb->tbl_size) {
      arn5_irq_dump_tbl(port_id, mods[ii] , blk_cb) ;
    }
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    PRINT/DUMP ALL IRQ ENABLE AND STATUS REG VALUES    */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Private                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_irq_dump_all(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Print all the IRQs thats are enabled.                                */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  arn5_blk_irq_cb_t     * blk_cb ;

  const arn5_module_id_t mods[] = {
    ARN5_ID_MPIF,
    ARN5_ID_HOST_IF,
  } ;
  int ii ;

  ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;

  for (ii = 0; ii < sizeof(mods) / sizeof(mods[0]); ii++) {
    blk_cb = arn5_irq_get_blk_cb(dev_id, mods[ii] ) ;
    if (blk_cb->tbl && blk_cb->tbl_size) {
      arn5_irq_dump_tbl(dev_id, mods[ii] , blk_cb) ;
    }
  }

  return (CS_OK) ;
}


static cs_status arn5_set_imed_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                        cs_uint32 imed_id, cs_uint16 irq_set_bits,
                        cs_boolean enable)
{
  arn5_blk_irq_cb_t     * blk_irq_cb ;
  cs_uint32             sl_num ;
  const arn5_irq_ctl_t  * irq_cb ;
  ARN5_t                * pDev ;
  volatile cs_reg       * irq_mask_reg ;
  volatile cs_reg       irq_mask ;

  cs_boolean is_dev = (mod_id & ARN5_DEV_SHRD_BLKS_ID) != 0 ;
  cs_uint16  port_id = is_dev ? ARN5_DEV_PORT_NUM_TO_PORT_ID(dpid, 0) : dpid ;
  cs_uint8   dev_id = is_dev ? dpid : ARN5_PORT_ID_TO_DEV_ID(port_id) ;

  if (is_dev) {
    ARN5_INVALID_DEV_EXIT(dev_id, CS_ERROR) ;
  }
  else {
    ARN5_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  }

  if ((mod_id & ~ARN5_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = arn5_irq_get_blk_cb(dpid, mod_id)) == NULL) ||
     (blk_irq_cb->imed_tbl == NULL)) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_TBL_NULL : EARN5_PORT_TBL_NULL,
                  "IMED table pointer is NULL for %s group_id %d\n",
                  ARN5_GET_MOD_NAME(mod_id), imed_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (imed_id >= blk_irq_cb->imed_tbl_size) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_RANGE_ERR : EARN5_PORT_RANGE_ERR,
                  "Table size is : %d for %s \n",
                  blk_irq_cb->imed_tbl_size, ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to arn5_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->imed_tbl + imed_id ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_PORT_NUM(port_id) ;

  if (sl_num > irq_cb->num_of_strides ) {
    CS_HNDL_ERROR(dpid, is_dev ? EARN5_DEV_RANGE_ERR : EARN5_PORT_RANGE_ERR,
                  "Num of strides for %s is %d <= slice %d\n",
                  ARN5_GET_MOD_NAME(mod_id), irq_cb->num_of_strides, sl_num ) ;
    return (CS_ERROR) ;
  }

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_set_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit(s) may be used. "
             "Yours: 0x%04x Valid: 0x%04x\n",
             irq_set_bits, irq_cb->irq_map) ;
  }

  irq_mask_reg = (volatile cs_reg *)(((cs_uint32) pDev) +
                 (sl_num * irq_cb->stride_size) + irq_cb->offset_mask_reg) ;

  ARN5_MOD_LOCK(is_dev, dpid, mod_id) ;
  irq_mask = CS_REG_READ(irq_mask_reg) ;

  /* if setting 0 means disable */
  if ( irq_cb->flags & ARN5_IRQ_FLG_1_TO_ENABLE ) {
    if (enable) {
      irq_mask |= (irq_set_bits) ;
    }
    else {
      irq_mask = (irq_mask & (~irq_set_bits)) ;
    }
  }
  else { /* 1 means disable */
    if (enable) {
      irq_mask = (irq_mask & (~irq_set_bits)) ;
    }
    else {
      irq_mask |= (irq_set_bits) ;
    }
  }
  CS_REG_WRITE(irq_mask_reg, irq_mask) ;
  ARN5_MOD_UNLOCK(is_dev, dpid, mod_id) ;

  return (CS_OK) ;
}


cs_status arn5_enable_imed_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                              cs_uint32 imed_id, cs_uint16 irq_enabl_bits)
{
  return arn5_set_imed_irq(dpid, mod_id, imed_id, irq_enabl_bits, 1) ;
}


cs_status arn5_disable_imed_irq(cs_uint16 dpid, arn5_module_id_t mod_id,
                               cs_uint32 imed_id, cs_uint16 irq_disabl_bits)
{
  return arn5_set_imed_irq(dpid, mod_id, imed_id, irq_disabl_bits, 0) ;
}


