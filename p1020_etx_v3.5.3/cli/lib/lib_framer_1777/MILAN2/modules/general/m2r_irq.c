/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_milan2_isr.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_irq_priv.h"
#include "m2r_irq.h"
#include "m2r_framer_irq.h"
#include "m2r_lif_irq.h"
#include "m2r_eth_irq.h"
#include "m2r_rmac_irq.h"
#include "m2r_spi42_priv.h"

/***********************************************************************
 * Internal APIs
 ***********************************************************************/
static
m2r_blk_irq_cb_t * m2r_irq_get_blk_cb(cs_uint16 port_id,
				      m2r_module_id_t mod_id) ;

#ifndef RELEASE_PLATFORM
static
void  print_irq_info(cs_uint16 port_id, m2r_irq_info_t * pIrq_info) ;
#endif


/************************************************************************/
/* $rtn_hdr_start     ENABLE BLOCK IRQ                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : MILAN2                                                  */
cs_status m2r_port_enable_blk_irq(cs_uint16 port_id,
				  cs_uint32 mod_enbl_bits)
/* INPUTS     : o Port Id                                               */
/*              o Module ID bitmask                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable the BLOCK level IRQ for the modules specified in the bimask   */
/* [mod_enbl_bits] specify the modules for which the IRQ is to be       */
/* enabled. The Mask is the Logical OR of the individual Module's ID.   */
/* For example to enable LIF and XETH IRQ, the mask should be logical   */
/* OR of M2R_ID_LIF and M2R_ID_ETH .                                    */
/*                                                                      */
/* The [mod_enbl_bits] parameter can be logical Mask of the following:  */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or M2R_ID_ETH  or    */
/* M2R_ID_RMAC   or M2R_ID_XGXS or M2R_ID_LIF or an or'ed combination   */
/* of any of the above blocks.                                          */
/*                                                                      */
/* Note: The [mod_enbl_bits] can be specified as M2R_ID_ALL_BLOCKS, in  */
/* which case, it will only enable for all the blocks that are valid    */
/* for the given chip configuration. So, interrupt from Ethernet block  */
/* will not be enabled if in Sonet mode, and vice-versa, or RingMAC     */
/* interrupt will not be enabled if in RingMAC-bypass.                  */
/*                                                                      */
/* Note: M2R_ID_MPIF block does not have any interrupt sources in it.   */
/* Eventhough M2R_ID_ALL_BLOCKS includes it, the bit corresponding to   */
/* M2R_ID_MPIF block in the bitmask will be ignored.                    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  M2R_t               * pDev ;
  MPIF_GlobalIntEnable_t blk_irq ;

  /* Validate the Port ID */
  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  /* Get Base address of the device */
  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  M2R_PORT_LOCK(port_id, M2R_ID_MPIF);

  /* Enable the IRQ on the Blocks requested */
  blk_irq.wrd = M2R_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;
  if (mod_enbl_bits & M2R_ID_SPI42) {
    blk_irq.bf.SPI4P2E = 1 ;
  }
  if ( M2R_IS_ETHERNET_MODE(port_id) && (mod_enbl_bits & M2R_ID_ETH) ) {
    blk_irq.bf.XETHE = 1 ;
  }
  if (mod_enbl_bits & M2R_ID_XGXS) {
    blk_irq.bf.XGXSE = 1 ;
  }
  if (mod_enbl_bits & M2R_ID_PPRBS) {
    blk_irq.bf.PPRBSE = 1 ;
  }
  if ( M2R_IS_SONET_MODE(port_id) && (mod_enbl_bits & M2R_ID_FRAMER) ) {
    blk_irq.bf.FramerE = 1 ;
  }
  if ( (M2R_GET_MAC_MODE(port_id) != M2R_PORT_IN_BYPASS_MODE) &&
       (mod_enbl_bits & M2R_ID_RMAC) ) {
    blk_irq.bf.RingMACE = 1 ;
  }
  if (mod_enbl_bits & M2R_ID_LIF) {
    blk_irq.bf.LIFE = 1 ;
  }
  M2R_REG_WRITE(&pDev->MicroIF.GlobalIntEnable.wrd, blk_irq.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_MPIF);
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    DISABLE BLOCK IRQ                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : MILAN2                                                  */
cs_status m2r_port_disable_blk_irq(cs_uint16 port_id,
				   cs_uint32 mod_disabl_bits)
/* INPUTS     : o Port Id                                               */
/*              o Module ID bitmask                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Disable the BLOCK level IRQ for the modules specified in the bimask  */
/* of the module IDs passed.                                            */
/* [mod_disabl_bits] specify the modules for which the IRQ is to be     */
/* disabled. The Mask is the Logical OR of the individual Module's ID.  */
/* For example to disable LIF and XETH IRQ, the mask should be logical  */
/* OR of M2R_ID_LIF and M2R_ID_ETH .                                    */
/*                                                                      */
/* The [mod_disabl_bits] parameter can be logical Mask of the following:*/
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or M2R_ID_ETH  or    */
/* M2R_ID_RMAC   or M2R_ID_XGXS or M2R_ID_LIF or M2R_ID_ALL_BLOCKS      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  M2R_t               * pDev ;
  MPIF_GlobalIntEnable_t blk_irq ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  M2R_PORT_LOCK(port_id, M2R_ID_MPIF);

  blk_irq.wrd = M2R_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;
  if (mod_disabl_bits & M2R_ID_SPI42) {
      blk_irq.bf.SPI4P2E = 0 ;
  }
  if (mod_disabl_bits & M2R_ID_PPRBS) {
      blk_irq.bf.PPRBSE = 0 ;
  }
  if (mod_disabl_bits & M2R_ID_FRAMER) {
      blk_irq.bf.FramerE = 0 ;
  }
  if (mod_disabl_bits & M2R_ID_RMAC) {
      blk_irq.bf.RingMACE = 0 ;
  }
  if (mod_disabl_bits & M2R_ID_XGXS) {
      blk_irq.bf.XGXSE = 0 ;
  }
  if (mod_disabl_bits & M2R_ID_LIF) {
      blk_irq.bf.LIFE = 0 ;
  }
  if (mod_disabl_bits & M2R_ID_ETH ) {
      blk_irq.bf.XETHE = 0 ;
  }
  M2R_REG_WRITE(&pDev->MicroIF.GlobalIntEnable.wrd, blk_irq.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_MPIF);
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    GET BLOCK IRQ ENABLE MASK                          */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : MILAN2                                                  */
cs_uint32 m2r_port_get_blk_irq_enbl(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : Bitmask of the Blocks for which IRQ is Enabled          */
/* DESCRIPTION:                                                         */
/* Get the bitmask of the blocks for which the IRQ is enabled.          */
/* The return value contains the bitmask of the modules, it could be    */
/* Logical OR mask of any of the following modules:                     */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or M2R_ID_ETH  or    */
/* M2R_ID_RMAC   or M2R_ID_XGXS or M2R_ID_LIF or M2R_ID_ALL_BLOCKS      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32              error = 0 ;
  M2R_t               *  pDev ;
  cs_uint32              mod_irq = 0 ;
  MPIF_GlobalIntEnable_t blk_irq ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id);
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  blk_irq.wrd = M2R_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;

  if (blk_irq.bf.SPI4P2E) {
    mod_irq |= M2R_ID_SPI42 ;
  }
  if (blk_irq.bf.PPRBSE) {
    mod_irq |= M2R_ID_PPRBS ;
  }
  if (blk_irq.bf.FramerE) {
    mod_irq |= M2R_ID_FRAMER ;
  }
  if (blk_irq.bf.RingMACE) {
    mod_irq |= M2R_ID_RMAC ;
  }
  if (blk_irq.bf.XGXSE) {
    mod_irq |= M2R_ID_XGXS ;
  }
  if (blk_irq.bf.LIFE) {
    mod_irq |= M2R_ID_LIF ;
  }
  if (blk_irq.bf.XETHE) {
    mod_irq |= M2R_ID_ETH  ;
  }

  return (mod_irq) ;
}


/************************************************************************/
/* $rtn_hdr_start    ENABLE INTERRUPT SOURCE                            */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_enable_leaf_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                         cs_uint32 grp_id, cs_uint16 irq_enbl_bits)

/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be enabled             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API enables the IRQs. Group ID specifies the IRQ group. IRQ     */
/* group refers to a IRQ register for instance INTERRUPT1 register in   */
/* LIF. Exact group definitions can be found in individual module's     */
/* header files. irq_enabl_bits is the bitmask composed of the          */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_enbl_bits is the mask of IRQ leaves ( sources ) . for exact      */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_enbl_bits identifies IRQ source. for    */
/* instance if irq_enbl_bits is 0x8000 in group INTERRUPT2 in LIF       */
/* refers to iSFI41PERR iRQ to be enabled.                              */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_MPIF   or M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or  */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  m2r_blk_irq_cb_t    * blk_irq_cb ;
  cs_uint32             error, sl_num ;
  const m2r_irq_ctl_t * irq_cb ;
  M2R_t               * pDev ;
  volatile cs_reg     * irq_mask_reg ;
  volatile cs_reg       irq_mask ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id);
    return (CS_ERROR) ;
  }

  if((mod_id >= M2R_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id)) == NULL) ||
     (blk_irq_cb->tbl == NULL)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL,
                  "IRQ Table pointer is NULL for %s group_id %d\n",
                  M2R_GET_MOD_NAME(mod_id), grp_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (grp_id >= blk_irq_cb->tbl_size) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR,
                  "Table Size is : %d for %s \n",
                  blk_irq_cb->tbl_size, M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to m2r_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->tbl + grp_id ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_enbl_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit may be used. Yours: 0x%04x Valid bits: 0x%04x\n",
             irq_enbl_bits, irq_cb->irq_map) ;
  }

  M2R_PORT_LOCK(port_id, mod_id);

  irq_mask_reg = (volatile cs_reg *)(((cs_uint64)pDev) +
                 (sl_num * M2R_SLICE_SIZE) + irq_cb->offset_mask_reg) ;

  irq_mask = M2R_REG_READ(irq_mask_reg) ;
  /* if setting 1 means enable */
  if ( irq_cb->flags & M2R_IRQ_FLG_1_TO_ENABLE ) {
    /* enable the user requested IRQs */
    irq_mask |= (irq_enbl_bits) ;
  }
  else { /* 0 means enable */
    irq_mask = (irq_mask & (~irq_enbl_bits)) ;
  }
  M2R_REG_WRITE(irq_mask_reg, irq_mask) ;

  M2R_PORT_UNLOCK(port_id, mod_id);
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start    DISABLE INTERRUPT SOURCE                           */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_disable_leaf_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                          cs_uint32 grp_id, cs_uint16 irq_disabl_bits)

/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be disabled            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API disables the IRQs. Group ID specifies the IRQ group. IRQ    */
/* group refers to a IRQ register for instance INTERRUPT1 register in   */
/* LIF. Exact group definitions can be found in individual module's     */
/* header files. irq_disabl_bits is the bitmask composed of the         */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_disabl_bits is the mask of IRQ leaves ( sources ) . for exact    */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_disabl_bits identifies IRQ source. for  */
/* instance if irq_disabl_bits is 0x8000 in group INTERRUPT2 in LIF     */
/* refers to iSFI41PERR IRQ to be disabled.                             */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_MPIF   or M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or  */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  m2r_blk_irq_cb_t  * blk_irq_cb ;
  cs_uint32           error, sl_num ;
  const m2r_irq_ctl_t  * irq_cb ;
  M2R_t             * pDev ;
  volatile cs_reg   * irq_mask_reg ;
  volatile cs_reg     irq_mask ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id);
    return (CS_ERROR) ;
  }

  if((mod_id >= M2R_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id)) == NULL) ||
     (blk_irq_cb->tbl == NULL)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL,
                  "IRQ Table pointer is NULL for %s group_id %d\n",
                  M2R_GET_MOD_NAME(mod_id), grp_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (grp_id >= blk_irq_cb->tbl_size) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR,
                  "Table Size is : %d for %s \n",
                  blk_irq_cb->tbl_size, M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to m2r_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->tbl + grp_id ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_disabl_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit may be used. Yours: 0x%04x Valid bits: 0x%04x\n",
             irq_disabl_bits, irq_cb->irq_map) ;
  }

  M2R_PORT_LOCK(port_id, mod_id);

  irq_mask_reg = (volatile cs_reg *)(((cs_uint64)pDev) +
                 (sl_num * M2R_SLICE_SIZE) + irq_cb->offset_mask_reg) ;

  irq_mask = M2R_REG_READ(irq_mask_reg) ;
  /* if setting 0 means disable */
  if ( irq_cb->flags & M2R_IRQ_FLG_1_TO_ENABLE ) {
    irq_mask = (irq_mask & (~irq_disabl_bits)) ;
  }
  else { /* 1 means disable */
    irq_mask |= (irq_disabl_bits) ;
  }
  M2R_REG_WRITE(irq_mask_reg, irq_mask) ;

  M2R_PORT_UNLOCK(port_id, mod_id);
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start    CLEAR INTERRUPT SOURCE                             */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_leaf_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                          cs_uint32 grp_id, cs_uint16 irq_clr_bits)

/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Interrupt Group ID                                    */
/*              o bit mask of the IRQ sources to be cleared             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* This API clears the IRQs. Group ID specifies the IRQ group. IRQ      */
/* group refers to a IRQ register for instance INTERRUPT1 register in   */
/* LIF. Exact group definitions can be found in individual module's     */
/* header files. irq_clr_bits is the bitmask composed of the            */
/* individual IRQ sources ( each IRQ bit in interrupt register ).       */
/* irq_clr_bits is the mask of IRQ leaves ( sources ) . for exact       */
/* definition of the IRQ leaves, refer to the module's header file.     */
/* Each bit which is set in irq_clr_bits identifies IRQ source. for     */
/* instance if irq_clr_bits is 0x8000 in group INTERRUPT2 in LIF        */
/* refers to iSFI41PERR IRQ to be cleared.                              */

/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_MPIF   or M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or  */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  m2r_blk_irq_cb_t  * blk_irq_cb ;
  cs_uint32           error ;
  const m2r_irq_ctl_t  * irq_cb ;
  M2R_t             * pDev ;
  cs_uint8            sl_num ;
  volatile cs_reg   * irq_reg ;
  volatile cs_reg     clear_irq ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id);
    return (CS_ERROR) ;
  }

  if((mod_id >= M2R_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id)) == NULL) ||
     (blk_irq_cb->tbl == NULL)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL, NULL ) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (grp_id >= blk_irq_cb->tbl_size) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR,
                  "Table Size is : %d for mod_id %d \n",
                  blk_irq_cb->tbl_size, mod_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to m2r_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->tbl + grp_id ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_clr_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit may be used. Yours: 0x%04x Valid bits: 0x%04x\n",
             irq_clr_bits, irq_cb->irq_map) ;
  }

  M2R_PORT_LOCK(port_id, mod_id);

  if(( irq_cb->flags & M2R_IRQ_FLG_SPL_CLR ) &&
     ( irq_clr_bits & irq_cb->spl_map)) {
    m2r_irq_spl_info_t irq_spl_info ;
    m2r_blk_irq_cb_t * blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id) ;

    if (blk_irq_cb && blk_irq_cb->spl_hndlr) {
      M2R_IRQ_FILL_SPL_CLR_INFO(&irq_spl_info, port_id, grp_id, irq_clr_bits) ;
      blk_irq_cb->spl_hndlr(&irq_spl_info) ;
    }
    /* Special handler should mask the bits taken care by itself */
    irq_clr_bits = irq_spl_info.op_mode.irq_clr.spl_clr_bits ;
  }

  irq_reg = (volatile cs_reg *)(((cs_uint64)pDev) +
                 (sl_num * M2R_SLICE_SIZE) + irq_cb->offset_intr_reg) ;

  if ( irq_cb->flags & M2R_IRQ_FLG_RW1C ) { /* RWIC */
    clear_irq = (irq_clr_bits & irq_cb->irq_map) ;
    M2R_REG_WRITE(irq_reg, clear_irq) ;
  }
  else {
    CS_PRINT("port-%d - %s(): SW ERROR - cannot clear interrupt!\n",
	      port_id, __FUNCTION__) ;
    return (CS_ERROR) ;
  }

  M2R_PORT_UNLOCK(port_id, mod_id);
  return (CS_OK) ;
}


/************************************************************************/
/*                    COMMON GROUP IRQ HANDLER                          */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_irq_grp_hndlr(cs_uint16 port_id, cs_uint32 dev_addr,
                            cs_uint32 mod_id, cs_uint32 grp_id,
                            const m2r_irq_ctl_t * irq_ctl,
			    const char * grp_name,
			    const m2r_irq_id_name_t * irq_id_name_tbl,
			    cs_uint16 num_irq_ids)
/* INPUTS     : o Port Id                                               */
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
  m2r_irq_info_t    irq_info ;
  cs_uint8          sl_num ;
  cs_uint16         spl_proc_status = 0 ;
  m2r_blk_irq_cb_t  * blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id) ;

  if (irq_ctl == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL,
                  "IRQ control table is NULL\n" ) ;
    return (CS_ERROR) ;
  }
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  irq_reg_val = (M2R_IRQ_GET_INT_REG_VAL(irq_ctl, dev_addr, sl_num) &
                 irq_ctl->irq_map) ;

  /* No IRQ pending in the group, bail out */
  if(!irq_reg_val) {
    goto RTN_EXIT ;
  }

  /* Store the Clear mask to clear the IRQ */
  irq_clr_mask = irq_reg_val ;
  irq_mask_reg_val = (M2R_IRQ_GET_MASK_REG_VAL(irq_ctl, dev_addr, sl_num) &
                      (irq_ctl->irq_map)) ;

  /*
   * Clear leaf interrupts first before any status is retrieved by the
   * interrupt handling code. The special handlers may later clear the
   * source of the interrupt but clear all the leaf interrupts here.
   */
  if ( irq_clr_mask &&
      (irq_ctl->flags & M2R_IRQ_FLG_RW1C) ) {
    M2R_IRQ_SET_INT_REG(irq_ctl, dev_addr, sl_num, irq_clr_mask) ;
  }

  /* Mask out any interrupts that are not enabled for processing */
  if ( irq_ctl->flags & M2R_IRQ_FLG_1_TO_ENABLE ) { /* if 1 to enable */
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
    status_reg_val = M2R_IRQ_GET_STATUS_REG_VAL(irq_ctl, dev_addr, sl_num) ;
  }

  irq_info.chip    	= CS_CHIP_MILAN2 ;
  irq_info.mod_id  	= mod_id ;
  irq_info.grp_id  	= grp_id ;
  irq_info.grp_name	= grp_name ;

  /* Process each pending interrupt in the leaf */
  for (iter = irq_reg_val; iter; ) {
    irq_id = (iter & (~(iter - 1))) ;
    irq_info.irq_id  = irq_id ;
    irq_info.irq_id_name = M2R_GET_IRQ_ID_NAME(irq_id_name_tbl, num_irq_ids,
					       irq_id) ;
    irq_info.flags   = 0 ;
    irq_info.status  = 0 ;
    spl_proc_status  = 0 ; /* value not used currently */

    /* Special clearing */
    if ( (irq_ctl->flags & M2R_IRQ_FLG_SPL_CLR) &&
	 (irq_id & irq_ctl->spl_map) &&
	 (blk_irq_cb && blk_irq_cb->spl_hndlr) ) {
      m2r_irq_spl_info_t irq_spl_info ;

      M2R_IRQ_FILL_SPL_CLR_INFO(&irq_spl_info, port_id, grp_id, irq_id) ;
      spl_proc_status |= blk_irq_cb->spl_hndlr(&irq_spl_info) ;
    }

    /* Special handling */
    if ( (irq_ctl->flags & M2R_IRQ_FLG_SPL_INT) &&
	 (irq_id & irq_ctl->spl_map) &&
         (blk_irq_cb && blk_irq_cb->spl_hndlr) ) {
      m2r_irq_spl_info_t irq_spl_info ;

      M2R_IRQ_FILL_SPL_INT_INFO(&irq_spl_info, &irq_info,
				port_id, irq_reg_val) ;
      spl_proc_status |= blk_irq_cb->spl_hndlr(&irq_spl_info) ;
    }

    /* Check if special handler already included the status info.
     * Otherwise include it in irq_info.*/
    if ( !(irq_info.flags & M2R_IRQ_INFO_STATUS_FLAG) ) {
      if (irq_ctl->offset_status_reg) {
	M2R_IRQ_INFO_SET_STATUS(irq_info.flags) ;
	irq_info.status = (status_reg_val & irq_id) ? 1 : 0 ;
      }
    }

    /* Finally pass on the packaged info for this irq_id to the user */
#ifndef RELEASE_PLATFORM
    print_irq_info(port_id, &irq_info) ;
#endif /* RELEASE PLATFORM */

    CS_IRQ_USER_HANDLER(port_id, &irq_info) ;

    iter &= ~irq_id ;	/* done with this interrupt */
  }

RTN_EXIT:
  return (CS_OK);
}


/************************************************************************/
/* $rtn_hdr_start     INTERRUPT HANDLER ENTRY POINT                     */
/* CATEGORY   : Device                                                  */
/* ACCESS     : Public                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : MILAN2                                                  */
cs_status m2r_isr_handler(cs_uint16 dev_id)
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
/* valid ports in this chip. It is done by traversing the interrupt     */
/* heirarchy. It calls individual block's IRQ routine if the block      */
/* has any pending IRQ to be serviced. Each Block's ISR then processes  */
/* any IRQ in its block and sending the notification to user provided   */
/* IRQ handler with information such as the port_id, IRQ Group Source   */
/* module ID's, IRQ leaf ID and its current status.                     */
/*                                                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  cs_uint32            errCode ;
  MPIF_GlobalInt_t     gblInt ;
  MPIF_GlobalIntEnable_t irq_enbl ;
  cs_uint16            port_id ;
  cs_uint16            dev_num ;

  if ( !(M2R_IS_DEVICE_VALID(dev_id, errCode)) ) {
    CS_HNDL_ERROR(dev_id, errCode, NULL) ;
    return ( CS_ERROR ) ;
  }

  dev_num = M2R_DEV_ID_TO_DEV_NUM(dev_id) ;
  port_id = M2R_DEV_SLICE_NUM_TO_PORT_ID(dev_num, 0) ;
  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  gblInt.wrd = M2R_REG_READ(&pDev->MicroIF.GlobalInt.wrd) ;
  irq_enbl.wrd = M2R_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;

  if( (irq_enbl.bf.LIFE) && (gblInt.bf.LIFI) ) {
    m2r_lif_isr(port_id) ;
  }

  if( (irq_enbl.bf.XETHE) && (gblInt.bf.XETHI) ) {
    m2r_eth_isr(port_id) ;
  }

  if( (irq_enbl.bf.FramerE) && (gblInt.bf.FramerI) ) {
    m2r_framer_isr(port_id) ;
  }

  if( (irq_enbl.bf.RingMACE) && (gblInt.bf.RingMACI) ) {
    m2r_rmac_isr(port_id) ;
  }

  if( (irq_enbl.bf.SPI4P2E) && (gblInt.bf.SPI4P2I) ) {
    m2r_spi42_isr(port_id) ;
  }

  if( (irq_enbl.bf.XGXSE) && (gblInt.bf.XGXSI) ) {
    m2r_rmac_xgxs_isr(port_id) ;
  }

  if( (irq_enbl.bf.PPRBSE) && (gblInt.bf.PPRBSI) ) {
    m2r_pprbs_isr(port_id) ;
  }

  return ( CS_OK ) ;
}

#ifndef RELEASE_PLATFORM
static
void  print_irq_info(cs_uint16 port_id, m2r_irq_info_t * pIrq_info)
{
  cs_int8 * mod_name = M2R_GET_MOD_NAME(pIrq_info->mod_id) ;

  CS_PRINT( "=> IRQ(Port%d): %-8s %23s.%-15s(bit: 0x%04x)",
            port_id, mod_name, pIrq_info->grp_name,
            pIrq_info->irq_id_name,
            pIrq_info->irq_id ) ;

  if (pIrq_info->flags & M2R_IRQ_INFO_STATUS_FLAG) {
    CS_PRINT(" Status:%d", pIrq_info->status) ;
  }
  if (pIrq_info->flags & M2R_IRQ_INFO_PARAM1_FLAG) {
    CS_PRINT(" Param1:0x%08x", pIrq_info->param1) ;
  }
  if (pIrq_info->flags & M2R_IRQ_INFO_PARAM2_FLAG) {
    CS_PRINT(" Param2:0x%08x", pIrq_info->param2) ;
  }
  CS_PRINT("\n");

  /* print detail info of RMAC USEQ_INT */
  if ( (pIrq_info->mod_id == M2R_ID_RMAC)
      && ( pIrq_info->grp_id == M2R_GRP_RMAC_MICRO_IF_IRQ2 )
      && ( pIrq_info->irq_id == M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT ) ) {
    CS_PRINT( "==****=> IRQ(Port%d): Micro-sequencer INT status <=****==\n",
              port_id);
    if ( pIrq_info->param1 & M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_MS_TIMEOUT ) {
      CS_PRINT("    SRC: MS_TIMEOUT !!\n");
    }
    if ( pIrq_info->param1 & M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_MS_INST_PAR_ERR ) {
      CS_PRINT("    SRC: MS_INST_PAR_ERR !!\n");
    }
    if ( pIrq_info->param1 & M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_IAMTAIL ) {
      if ( pIrq_info->param2 & M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_IAMTAIL ) {
        cs_mac cgst_mac ;

        m2r_ms_get_last_head_of_congest(port_id, &cgst_mac);

        CS_PRINT("    SRC: I am Tail !!\n");
        CS_PRINT("        => Head of Congest MAC: %02x.%02x.%02x.%02x.%02x.%02x\n",
                 cgst_mac.byte5, cgst_mac.byte4,
                 cgst_mac.byte3, cgst_mac.byte2,
                 cgst_mac.byte1, cgst_mac.byte0);
      } else {
        CS_PRINT("    SRC: I am not Tail !!\n");
      }
    }
    if ( pIrq_info->param1 & M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_KEEPALIVE_FAIL ) {
      if ( pIrq_info->param2 & M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_KEEPALIVE_FAIL ) {
        CS_PRINT("    SRC: KeepAlive Fail !!\n");
      } else {
        CS_PRINT("    SRC: KeepAlive Back !!\n");
      }
    }
    if ( pIrq_info->param1 & M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT_NEW_TTL_TO_CONGEST ) {
      CS_PRINT("    SRC: NEW_TTL_TO_CONGEST !!\n");
    }
  }

  return  ;
}
#endif /* RELEASE_PLATFORM */

/*********************************************************
 * Private IRQ APIs
 *********************************************************/
cs_status M2R_REGISTER_IRQ_TBL(cs_uint16 port_id,
                               m2r_module_id_t mod_id,
                               const m2r_irq_ctl_t * tbl,
                               const char ** name_tbl,
                               cs_uint16  tbl_size,
                               M2R_IRQ_SPL_HNDLR irq_spl_hndlr)
{
  m2r_blk_irq_cb_t *  blk_irq_cb ;

  if (!tbl || (tbl_size == 0)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL, "Module ID : %d\n",
                  mod_id) ;
    return (CS_ERROR) ;
  }

  blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id) ;
  blk_irq_cb->tbl       = tbl ;
  blk_irq_cb->name_tbl = name_tbl ;
  blk_irq_cb->tbl_size  = tbl_size ;
  blk_irq_cb->spl_hndlr = irq_spl_hndlr ;

  return (CS_OK) ;
}

cs_status M2R_UNREGISTER_IRQ_TBL(cs_uint16 port_id,
                                 m2r_module_id_t mod_id)
{
  m2r_blk_irq_cb_t *  blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id) ;
  blk_irq_cb->tbl       = NULL ;
  blk_irq_cb->name_tbl  = NULL ;
  blk_irq_cb->tbl_size  = 0 ;
  blk_irq_cb->spl_hndlr = NULL ;

  return (CS_OK) ;
}

cs_status M2R_REGISTER_IMED_TBL(cs_uint16 port_id,
                               m2r_module_id_t mod_id,
                               const m2r_irq_ctl_t * imed_tbl,
                               const char ** imed_name_tbl,
                               cs_uint16  imed_tbl_size,
                               M2R_IRQ_SPL_HNDLR imed_spl_hndlr)
{
  m2r_blk_irq_cb_t *  blk_irq_cb ;

  if (!imed_tbl || (imed_tbl_size == 0)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL, "Module ID : %d\n",
                  mod_id) ;
    return (CS_ERROR) ;
  }

  blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id) ;
  blk_irq_cb->imed_tbl       = imed_tbl ;
  blk_irq_cb->imed_name_tbl  = imed_name_tbl ;
  blk_irq_cb->imed_tbl_size  = imed_tbl_size ;
  blk_irq_cb->imed_spl_hndlr = imed_spl_hndlr ;

  return (CS_OK) ;
}

cs_status M2R_UNREGISTER_IMED_TBL(cs_uint16 port_id,
                                 m2r_module_id_t mod_id)
{
  m2r_blk_irq_cb_t *  blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id) ;
  blk_irq_cb->imed_tbl       = NULL ;
  blk_irq_cb->imed_name_tbl  = NULL ;
  blk_irq_cb->imed_tbl_size  = 0 ;
  blk_irq_cb->imed_spl_hndlr = NULL ;

  return (CS_OK) ;
}

static m2r_blk_irq_cb_t * m2r_irq_get_blk_cb(cs_uint16 port_id,
                                            m2r_module_id_t mod_id)
{
  m2r_port_irq_cb_t * port_irq_cb = M2R_GET_IRQ_CB(port_id) ;
  m2r_blk_irq_cb_t *  blk_irq_cb ;

  if (!port_irq_cb) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_UNINITIALIZED, NULL) ;
    return (NULL) ;
  }


  switch ( mod_id ) {
    case M2R_ID_MPIF  :
      blk_irq_cb = &port_irq_cb->mpif ;
      break ;
    case M2R_ID_SPI42 :
      blk_irq_cb = &port_irq_cb->spi42 ;
      break ;
    case M2R_ID_PPRBS :
      blk_irq_cb = &port_irq_cb->pprbs ;
      break ;
    case M2R_ID_FRAMER:
      blk_irq_cb = &port_irq_cb->framer ;
      break ;
    case M2R_ID_RMAC  :
      blk_irq_cb = &port_irq_cb->rmac ;
      break ;
    case M2R_ID_XGXS  :
      blk_irq_cb = &port_irq_cb->xgxs ;
      break ;
    case M2R_ID_ETH   :
      blk_irq_cb = &port_irq_cb->eth ;
      break ;
    case M2R_ID_LIF   :
      blk_irq_cb = &port_irq_cb->lif ;
      break ;
    default :
      blk_irq_cb = NULL ;
  }
  return (blk_irq_cb) ;
}

extern cs_boolean m2r_drvr_is_mask_zero_cntr(void) ;

cs_status m2r_irq_dump_tbl_entry(cs_uint16 port_id,
                                 cs_uint32 dev_addr,
                                 const m2r_irq_ctl_t * irq_ctl)
{
  volatile cs_reg  * irq_reg_addr, irq_reg_val = 0  ;
  volatile cs_reg    irq_mask_reg_val = 0, status_reg_val = 0 ;
  cs_uint8           sl_num ;

  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  irq_reg_addr = (volatile cs_reg*)(dev_addr +
              (sl_num * M2R_SLICE_SIZE) + irq_ctl->offset_intr_reg) ;
  irq_reg_val = (M2R_IRQ_GET_INT_REG_VAL(irq_ctl, dev_addr, sl_num) &
                 irq_ctl->irq_map) ;

  irq_mask_reg_val = (M2R_IRQ_GET_MASK_REG_VAL(irq_ctl, dev_addr, sl_num) &
                      irq_ctl->irq_map) ;

  if (irq_ctl->offset_status_reg) {
    status_reg_val = M2R_IRQ_GET_STATUS_REG_VAL(irq_ctl, dev_addr, sl_num) ;
  }
  else {
    status_reg_val = 0 ;
  }

  if ((m2r_drvr_is_mask_zero_cntr() == FALSE) ||
       irq_reg_val || status_reg_val) {
    if (irq_ctl->offset_status_reg) {
      CS_PRINT("0x%08x => 0x%08x : 0x%08x : 0x%08x\n",
             irq_ctl->offset_intr_reg, irq_mask_reg_val, irq_reg_val,
	     status_reg_val) ;
    }
    else {
      CS_PRINT("0x%08x => 0x%08x : 0x%08x : --- \n",
             irq_ctl->offset_intr_reg, irq_mask_reg_val, irq_reg_val) ;
    }
  }

  return (CS_OK) ;
}

cs_status m2r_irq_dump_tbl(cs_uint16 port_id,
                           m2r_module_id_t mod_id,
                           m2r_blk_irq_cb_t * blk_cb)
{
  cs_uint32             dev_addr, ii ;
  const m2r_irq_ctl_t * irq_ctl ;

  CS_PRINT("\nIRQ dump for %s on Port-%d", M2R_GET_MOD_NAME(mod_id), port_id) ;
  if ((mod_id == M2R_ID_SPI42) || (mod_id == M2R_ID_RMAC)) {
    CS_PRINT(" (uses Enable bits)\n");
  }
  else {
    CS_PRINT(" (uses Mask bits)\n");
  }

  CS_PRINT("--------------------------------------------------------------------\n") ;
  CS_PRINT("IRQ Group Name                  Reg Offset : Mask/Enable:   Value    :   Status\n") ;
  dev_addr = (cs_uint64) M2R_PORT_ID_TO_DEV_BASE(port_id);
  irq_ctl = blk_cb->tbl ;

  for (ii = 0; ii < blk_cb->tbl_size; ii++, irq_ctl++) {
    const char * str;
    if (blk_cb->name_tbl != NULL) {
      str = (blk_cb->name_tbl)[ii] ;
    }
    else {
      str = " ";
    }

    CS_PRINT("%-30s ", str) ;
    m2r_irq_dump_tbl_entry(port_id, dev_addr, irq_ctl) ;
  }

  irq_ctl = blk_cb->imed_tbl ;

  if (irq_ctl != NULL) {
    CS_PRINT("\nIntermediate IRQ dump for %s on Port-%d",
	     M2R_GET_MOD_NAME(mod_id), port_id) ;
    if ((mod_id == M2R_ID_SPI42) || (mod_id == M2R_ID_RMAC)) {
      CS_PRINT(" (uses Enable bits)\n");
    }
    else {
      CS_PRINT(" (uses Mask bits)\n");
    }

    CS_PRINT("--------------------------------------------------------------------\n") ;
    CS_PRINT("IMED Name                       Reg Offset : Mask/Enable:   Value    :   Status\n") ;

    for (ii = 0; ii < blk_cb->imed_tbl_size; ii++, irq_ctl++) {
      const char * str;
      if (blk_cb->imed_name_tbl != NULL) {
        str = (blk_cb->imed_name_tbl)[ii] ;
      }
      else {
        str = " ";
      }

      CS_PRINT("%-30s ", str) ;
      m2r_irq_dump_tbl_entry(port_id, dev_addr, irq_ctl) ;
    }
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    PRINT/DUMP ALL IRQ ENABLE AND STATUS REG VALUES    */
/* CATEGORY   : Port                                                    */
/* ACCESS     : Private                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_irq_dump_all(cs_uint16 port_id)

/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Print all the IRQs thats are enabled.                                */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  m2r_blk_irq_cb_t * blk_cb ;
  cs_uint32          err_code = 0 ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, " : port_id= %d\n",
                  port_id) ;
    return(CS_ERROR) ;
  }

  blk_cb = m2r_irq_get_blk_cb(port_id, M2R_ID_FRAMER) ;
  if (blk_cb->tbl && blk_cb->tbl_size) {
    m2r_irq_dump_tbl(port_id, M2R_ID_FRAMER, blk_cb) ;
  }

  CS_PRINT("\nIRQ dump for M2R_ID_LIF on Port-%d (uses Mask bits)\n", port_id) ;
  CS_PRINT("--------------------------------------------------------------------\n") ;
  CS_PRINT("IRQ Group Name                  Reg Offset : Mask/Enable:   Value    :   Status\n") ;
  m2r_lif_dump_irq(port_id) ;

  blk_cb = m2r_irq_get_blk_cb(port_id, M2R_ID_SPI42) ;
  if (blk_cb->tbl && blk_cb->tbl_size) {
    m2r_irq_dump_tbl(port_id, M2R_ID_SPI42, blk_cb) ;
  }

  blk_cb = m2r_irq_get_blk_cb(port_id, M2R_ID_RMAC) ;
  if (blk_cb->tbl && blk_cb->tbl_size) {
    m2r_irq_dump_tbl(port_id, M2R_ID_RMAC, blk_cb) ;
  }

  blk_cb = m2r_irq_get_blk_cb(port_id, M2R_ID_PPRBS) ;
  if (blk_cb->tbl && blk_cb->tbl_size) {
    m2r_irq_dump_tbl(port_id, M2R_ID_PPRBS, blk_cb) ;
  }

  blk_cb = m2r_irq_get_blk_cb(port_id, M2R_ID_XGXS) ;
  if (blk_cb->tbl && blk_cb->tbl_size) {
    m2r_irq_dump_tbl(port_id, M2R_ID_XGXS, blk_cb) ;
  }
  blk_cb = m2r_irq_get_blk_cb(port_id, M2R_ID_ETH ) ;
  if (blk_cb->tbl && blk_cb->tbl_size) {
    m2r_irq_dump_tbl(port_id, M2R_ID_ETH , blk_cb) ;
  }

  return (CS_OK) ;
}


cs_status m2r_enable_imed_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                         cs_uint32 imed_id, cs_uint16 irq_enbl_bits)
{
  m2r_blk_irq_cb_t    * blk_irq_cb ;
  cs_uint32             error, sl_num ;
  const m2r_irq_ctl_t * irq_cb ;
  M2R_t               * pDev ;
  volatile cs_reg     * irq_mask_reg ;
  volatile cs_reg       irq_mask ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id);
    return (CS_ERROR) ;
  }

  if((mod_id >= M2R_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id)) == NULL) ||
     (blk_irq_cb->imed_tbl == NULL)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL,
                  "IMED Table pointer is NULL for %s group_id %d\n",
                  M2R_GET_MOD_NAME(mod_id), imed_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (imed_id >= blk_irq_cb->imed_tbl_size) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR,
                  "Table Size is : %d for %s \n",
                  blk_irq_cb->imed_tbl_size, M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to m2r_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->imed_tbl + imed_id ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_enbl_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit may be used. Yours: 0x%04x Valid bits: 0x%04x\n",
             irq_enbl_bits, irq_cb->irq_map) ;
  }

  M2R_PORT_LOCK(port_id, mod_id);

  irq_mask_reg = (volatile cs_reg *)(((cs_uint64)pDev) +
                 (sl_num * M2R_SLICE_SIZE) + irq_cb->offset_mask_reg) ;

  irq_mask = M2R_REG_READ(irq_mask_reg) ;
  /* if setting 1 means enable */
  if ( irq_cb->flags & M2R_IRQ_FLG_1_TO_ENABLE ) {
    /* enable the user requested IRQs */
    irq_mask |= (irq_enbl_bits) ;
  }
  else { /* 0 means enable */
    irq_mask = (irq_mask & (~irq_enbl_bits)) ;
  }
  M2R_REG_WRITE(irq_mask_reg, irq_mask) ;

  M2R_PORT_UNLOCK(port_id, mod_id);
  return (CS_OK) ;
}



cs_status m2r_disable_imed_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                          cs_uint32 imed_id, cs_uint16 irq_disabl_bits)
{
  m2r_blk_irq_cb_t  * blk_irq_cb ;
  cs_uint32           error, sl_num ;
  const m2r_irq_ctl_t  * irq_cb ;
  M2R_t             * pDev ;
  volatile cs_reg   * irq_mask_reg ;
  volatile cs_reg     irq_mask ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id);
    return (CS_ERROR) ;
  }

  if((mod_id >= M2R_ID_ALL_BLOCKS) ||
     ((blk_irq_cb = m2r_irq_get_blk_cb(port_id, mod_id)) == NULL) ||
     (blk_irq_cb->imed_tbl == NULL)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NULL,
                  "IMED Table pointer is NULL for %s group_id %d\n",
                  M2R_GET_MOD_NAME(mod_id), imed_id) ;
    return (CS_ERROR) ;
  }

  /*
   * Validate the grp_id. Compare against the table size. The valid range
   * is from 0 to size-1 for that module.
   */
  if (imed_id >= blk_irq_cb->imed_tbl_size) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR,
                  "Table Size is : %d for %s \n",
                  blk_irq_cb->imed_tbl_size, M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the pointer to m2r_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */
  irq_cb = blk_irq_cb->imed_tbl + imed_id ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* verify the interested bits are in our definition */
  /* give warning but keep going even if out of scope */
  if (irq_disabl_bits & (~(irq_cb->irq_map))) {
    CS_PRINT("Warning!! Invalid bit may be used. Yours: 0x%04x Valid bits: 0x%04x\n",
             irq_disabl_bits, irq_cb->irq_map) ;
  }

  M2R_PORT_LOCK(port_id, mod_id);

  irq_mask_reg = (volatile cs_reg *)(((cs_uint64)pDev) +
                 (sl_num * M2R_SLICE_SIZE) + irq_cb->offset_mask_reg) ;

  irq_mask = M2R_REG_READ(irq_mask_reg) ;
  /* if setting 0 means disable */
  if ( irq_cb->flags & M2R_IRQ_FLG_1_TO_ENABLE ) {
    irq_mask = (irq_mask & (~irq_disabl_bits)) ;
  }
  else { /* 1 means disable */
    irq_mask |= (irq_disabl_bits) ;
  }
  M2R_REG_WRITE(irq_mask_reg, irq_mask) ;

  M2R_PORT_UNLOCK(port_id, mod_id);
  return (CS_OK) ;
}




