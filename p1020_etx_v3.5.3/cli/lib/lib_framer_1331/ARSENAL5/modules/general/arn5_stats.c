/**********************************************************************/
/* This file contains unpublished documentation and software          */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,*/
/* in whole or in part, of the information in this file without a     */
/* written consent of an officer of Cortina Systems Incorporated is   */
/* strictly prohibited.                                               */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                */
/**********************************************************************/
/*
 * arn5_stats.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_stats_priv.h"
#include "arn5_stats.h"

static cs_uint32 _arn5_sect_print_flag = 0 ;

/*********************************************************************
 * External function declaration
 *********************************************************************/
extern cs_boolean arn5_drvr_is_stats_in_hex_fmt(void) ;
extern cs_boolean arn5_drvr_is_mask_zero_cntr(void) ;

/*********************************************************************
 * Internal function declaration
 *********************************************************************/
static
cs_boolean arn5_stats_is_spl_hndlr_valid(
                        const arn5_stats_info_t * p_unit_info,
                        arn5_stats_op_type_t op) ;
static
cs_status arn5_stats_spl_handler(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        arn5_stats_op_type_t op,
                        cs_dir_t dir,
                        cs_boolean sect_context) ;
static
cs_status arn5_stats_update_unit_data(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 stride_id,
                        cs_uint32 stride_size,
                        const arn5_stats_info_t * p_unit_info,
                        arn5_stats_data_t * p_unit_data,
                        arn5_stats_work_area_t * p_work_area,
                        cs_dir_t dir) ;
static
cs_status arn5_stats_clear_unit_data(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 stride_id,
                        cs_uint32 stride_size,
                        const arn5_stats_info_t * p_unit_info,
                        arn5_stats_data_t * p_unit_data,
                        arn5_stats_work_area_t * p_work_area,
                        cs_dir_t dir) ;

static
void arn5_stats_print_unit_cntr_in_hex(const cs_uint8 * name, cs_uint32 flags,
                        cs_uint64 rx, cs_uint64 tx) ;

static
void arn5_stats_print_unit_cntr_in_dec(const cs_uint8 * name, cs_uint32 flags,
                        cs_uint64 rx, cs_uint64 tx) ;

static
cs_status arn5_stats_set_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        arn5_stats_blk_cb_t * blk_cb) ;


/*********************************************************************
 * PRIVATE APIs definitions
 *********************************************************************/
/*********************************************************************/
/*                    REGISTER STATS BLOCK CB                        */
/* CATEGORY   : Device/Port                                          */
/* ACCESS     : PRIVATE                                              */
/* BLOCK      : General                                              */
/* CHIP       : Arsenal5                                             */
arn5_stats_blk_cb_t * arn5_stats_register_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_boolean ctl_enabled,
                        arn5_stats_get_func pGet_func,
                        arn5_stats_print_func pPrint_func,
                        arn5_stats_clear_func pClear_func)
/* INPUTS     : o Device or Port Id                                  */
/*              o ARN5_DEVICE or ARN5_PORT                           */
/*              o Module ID                                          */
/*              o TRUE ( Stats Enabled ) or FALSE                    */
/*              o Pointer to special handler for GET                 */
/*              o Pointer to special handler for PRINT               */
/*              o Pointer to special handler for CLEAR               */
/* OUTPUTS    : ----                                                 */
/* RETURNS    : CS_OK or CS_ERROR                                    */
/* DESCRIPTION:                                                      */
/* Register the Block CB. This is an internal API to be invoked by   */
/* individual modules to register thier block CB.                    */
/* This API will do tha allocation of memory for the block CB and    */
/* if sanity checks pass, returns the pointer to arn5_stats_blk_cb_t */
/* back to the called. The individual blocks can the go on adding    */
/* their sections to the block_cb.                                   */
/*                                                                   */
/* the [mod_id] parameter can be logical mask of the following:      */
/* if ARN5_DEVICE :                                                  */
/* ARN5_ID_HOST_IF                                                   */
/* if ARN5_PORT   :                                                  */
/* ARN5_ID_HOST_IF or ARN5_ID_FRAMER or  ARN5_ID_ETH or              */
/* ARN5_ID_LIF     or ARN5_ID_PPRBS.                                 */
/*********************************************************************/
{
  cs_uint16            ii ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  stats_blk_cb = arn5_stats_get_blk_cb(dpid, dp, mod_id) ;

  /* If block CB is NULL, Allocate it */
  if (stats_blk_cb == NULL) {

    /* Allocate memory for STATS block CB */
    stats_blk_cb = (arn5_stats_blk_cb_t *)
      CS_MALLOC(sizeof(arn5_stats_blk_cb_t)) ;
    if (stats_blk_cb == NULL) {
      cs_uint32 err_code = ( (dp == ARN5_DEVICE) ?
                             EARN5_DEV_MALLOC_FAILED :
                             EARN5_PORT_MALLOC_FAILED ) ;
      CS_HNDL_ERROR( dpid, err_code, NULL ) ;
      return (NULL) ;
    }

    /* Clear out the pointer table to STATS section CB */
    for(ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
      stats_blk_cb->p_sect_cb[ii] = NULL ;
    }
  }

  stats_blk_cb->blk_name = ARN5_GET_MOD_NAME(mod_id) ;
  stats_blk_cb->enabled = ctl_enabled ;
  stats_blk_cb->num_sect = 0 ;
  stats_blk_cb->get_func = pGet_func ;
  stats_blk_cb->print_func = pPrint_func ;
  stats_blk_cb->clear_func = pClear_func ;
  arn5_stats_set_blk_cb(dpid, dp, mod_id, stats_blk_cb) ;

  return (stats_blk_cb) ;
}


/*********************************************************************/
/*                 UNREGISTER STATS BLOCK CB                         */
/* CATEGORY   : Device/Port                                          */
/* ACCESS     : PRIVATE                                              */
/* BLOCK      : General                                              */
/* CHIP       : Arsenal5                                             */
cs_status arn5_stats_deregister_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id)
/* INPUTS     : o Device or Port Id                                  */
/*              o ARN5_DEVICE or ARN5_PORT                           */
/*              o Module ID                                          */
/* OUTPUTS    : ----                                                 */
/* RETURNS    : CS_OK or CS_ERROR                                    */
/* DESCRIPTION:                                                      */
/* Unregister the Block CB. This is an internal API to be invoked    */
/* to deregister the block CB. This API shall free any memory        */
/* allocated for the block CB and section CBs. This should be called */
/* ideally everytime before the Port init to clean up any resources  */
/* allocated from last time.                                         */
/*                                                                   */
/* the [mod_id] parameter can be logical mask of the following:      */
/* if ARN5_DEVICE :                                                  */
/* ARN5_ID_HOST_IF                                                   */
/* if ARN5_PORT   :                                                  */
/* ARN5_ID_HOST_IF or ARN5_ID_FRAMER or  ARN5_ID_ETH or              */
/* ARN5_ID_LIF     or ARN5_ID_PPRBS.                                 */
/*********************************************************************/
{
  cs_uint16            ii ;
  arn5_stats_blk_cb_t * stats_blk_cb ;

  stats_blk_cb = arn5_stats_get_blk_cb(dpid, dp, mod_id) ;
  if (stats_blk_cb == NULL) {
    return (CS_OK) ; /* this stats blk not registered */
  }

  for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    arn5_stats_remove_sect_tbl_entry(dpid, dp, mod_id, ii) ;
  }

  CS_FREE(stats_blk_cb) ;
  arn5_stats_set_blk_cb(dpid, dp, mod_id, NULL) ;

  return (CS_OK) ;
}


/*********************************************************************/
/*                 CLEAN STATS BLOCK CB                              */
/* CATEGORY   : Device/Port                                          */
/* ACCESS     : PRIVATE                                              */
/* BLOCK      : General                                              */
/* CHIP       : Arsenal5                                             */
cs_status arn5_stats_clean_all(cs_uint16 dpid,
                        arn5_dev_or_port_t dp)
/* INPUTS     : o Device or Port Id                                  */
/*              o ARN5_DEVICE or ARN5_PORT                           */
/* OUTPUTS    : ----                                                 */
/* RETURNS    : CS_OK or CS_ERROR                                    */
/* DESCRIPTION:                                                      */
/* Unregister the All Block CB. This is an internal API to be invoked*/
/* to deregister the block CB. This API shall free any memory        */
/* allocated for the block CB and section CBs. This should be called */
/* ideally everytime before the Port init to clean up any resources  */
/* allocated from last time.                                         */
/*********************************************************************/
{
  if (dp == ARN5_DEVICE) {
    arn5_stats_deregister_blk_cb(dpid, dp, ARN5_ID_HOST_IF) ;
  }
  else { /* ARN5_PORT */
    arn5_stats_deregister_blk_cb(dpid, dp, ARN5_ID_HOST_IF) ;
    arn5_stats_deregister_blk_cb(dpid, dp, ARN5_ID_FRAMER) ;
    arn5_stats_deregister_blk_cb(dpid, dp, ARN5_ID_ETH) ;
    arn5_stats_deregister_blk_cb(dpid, dp, ARN5_ID_LIF) ;
    arn5_stats_deregister_blk_cb(dpid, dp, ARN5_ID_PPRBS) ;
  }
  return (CS_OK) ;
}


/*********************************************************************/
/*                 ADD SECTION CB TO BLOCK CB                        */
/* CATEGORY   : Device/Port                                          */
/* ACCESS     : PRIVATE                                              */
/* BLOCK      : General                                              */
/* CHIP       : Arsenal5                                             */
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
                        const arn5_stats_info_t * p_info_tbl)
/* INPUTS     : o Device or Port Id                                  */
/*              o ARN5_DEVICE or ARN5_PORT                           */
/*              o Section Name                                       */
/*              o Section ID                                         */
/*              o Section Size                                       */
/*              o Number of counter blocks in this section           */
/*              o Pointer to Section stats data structure            */
/*              o Pointer to the Section Info Table                  */
/* OUTPUTS    : ----                                                 */
/* RETURNS    : CS_OK or CS_ERROR                                    */
/* DESCRIPTION:                                                      */
/* Add Section CB to block CB. Individual modules must call this API */
/* to register thier Section tables. Section ID identifies the       */
/* section stats.                                                    */
/* The [sect_id] is the section ID of the section stats. Æach Module */
/* should register thier section stats by calling this API for each  */
/* section they wish to regsiter.                                    */
/* The [stride_id] identifies the stride.                            */
/* The [stride_size] is the stride size that used to determine where */
/* the next stride is                                                */
/* The [ctl_enabled] is to enbl/disable statistics gathering/printing*/
/* for this section table. If [ctl_enabled] is FALSE, stats are not  */
/* accumulated for this section.                                     */
/* The [num_child] is the number of STAT unit data entries in this   */
/* section.                                                          */
/* The [p_data] is pointer to user allocated data structure where the*/
/* statistics will be accumulated.                                   */
/* The [p_info_tbl] is pointer to section info table. This table     */
/* should be an array of stats unit info entries and contain info    */
/* about the register offset , flags which dictate how the registers */
/* will be read/cleared and other info like if special handling is   */
/* required to get the stats for this unit.                          */
/*********************************************************************/
{
  cs_uint16            ii ;
  cs_uint32            err_code ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;
  cs_boolean           need_work_area = FALSE ;

  stats_blk_cb = arn5_stats_get_blk_cb(dpid, dp, mod_id) ;

  if ((stats_blk_cb == NULL) || (sect_id >= ARN5_STATS_MAX_SECT)) {
    err_code = ( (dp == ARN5_DEVICE) ? EARN5_DEV_TBL_NULL :
                 EARN5_PORT_TBL_NULL ) ;
    CS_HNDL_ERROR(dpid, err_code, NULL ) ;
    return (CS_ERROR) ;
  }

  /*
   * Get the Pointer to Section CB entry.
   * If it is NULL, allocate a new entry.
   */
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;
  if (stats_sect_cb == NULL) {
    if ((stats_sect_cb = (arn5_stats_sect_cb_t *)
                    CS_MALLOC(sizeof(arn5_stats_sect_cb_t))) == NULL) {
      err_code = ( (dp == ARN5_DEVICE) ? EARN5_DEV_MALLOC_FAILED :
                   EARN5_PORT_MALLOC_FAILED ) ;
      CS_HNDL_ERROR(dpid, err_code, NULL ) ;
      goto RTN_CLEANUP;
    }

    if ((stats_sect_cb->enbl_tbl = (cs_boolean *)
                CS_MALLOC(num_child * sizeof(cs_boolean))) == NULL ) {
      err_code = ( (dp == ARN5_DEVICE) ? EARN5_DEV_MALLOC_FAILED :
                   EARN5_PORT_MALLOC_FAILED ) ;
      CS_HNDL_ERROR(dpid, err_code, NULL ) ;
      goto RTN_CLEANUP;
    }

    /* determine we need scratch or not */
    for (ii = 0; ii < num_child; ii++) {
      /* add checking conditions here   */
      if (p_info_tbl[ii].flags & ARN5_STATS_FLAG_NO_CLR_TX_RX) {
        need_work_area = TRUE ;
        break ;
      }
    }

    if (need_work_area) {
      if ((stats_sect_cb->work_area_tbl = (arn5_stats_work_area_t *)
                  CS_MALLOC(num_child * sizeof(arn5_stats_work_area_t))) == NULL ) {
        err_code = ( (dp == ARN5_DEVICE) ? EARN5_DEV_MALLOC_FAILED :
                     EARN5_PORT_MALLOC_FAILED ) ;
        CS_HNDL_ERROR(dpid, err_code, NULL ) ;
        goto RTN_CLEANUP;
      }
    }
    else {
      stats_sect_cb->work_area_tbl = NULL ;
    }

    stats_blk_cb->p_sect_cb[sect_id] = stats_sect_cb ;
    for (ii = 0; ii < num_child; ii++) {
      /* enable all by default */
      *(stats_sect_cb->enbl_tbl + ii) = TRUE ;
      if (need_work_area) {
        stats_sect_cb->work_area_tbl[ii].prev_data.rx = 0 ;
        stats_sect_cb->work_area_tbl[ii].prev_data.tx = 0 ;
      }
    }
  }

  stats_sect_cb->sect_name   = sect_name ;
  stats_sect_cb->enabled     = ctl_enabled ;
  stats_sect_cb->stride_id   = stride_id ;
  stats_sect_cb->stride_size = stride_size ;
  stats_sect_cb->num_entries = num_child ;
  stats_sect_cb->pStats      = p_data ;
  stats_sect_cb->info_tbl    = p_info_tbl ;
  stats_blk_cb->num_sect++ ;
  return (CS_OK) ;

RTN_CLEANUP:
  if (stats_sect_cb) {
    if (stats_sect_cb->enbl_tbl) {
      CS_FREE(stats_sect_cb->enbl_tbl) ;
    }
    if (stats_sect_cb->work_area_tbl) {
      CS_FREE(stats_sect_cb->work_area_tbl) ;
    }
    CS_FREE(stats_sect_cb) ;
  }
  return (CS_ERROR) ;
}


/*********************************************************************/
/*                 REMOVE SECTION CB FROM BLOCK CB                   */
/* CATEGORY   : Device/Port                                          */
/* ACCESS     : PRIVATE                                              */
/* BLOCK      : General                                              */
/* CHIP       : Arsenal5                                             */
cs_status  arn5_stats_remove_sect_tbl_entry(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id)
/* INPUTS     : o Device or Port Id                                  */
/*              o ARN5_DEVICE or ARN5_PORT                           */
/*              o Module ID                                          */
/*              o Section ID                                         */
/* OUTPUTS    : ----                                                 */
/* RETURNS    : CS_OK or CS_ERROR                                    */
/* DESCRIPTION:                                                      */
/* Removes the section table from the stats block CB and frees up any*/
/* memory allocated for the section CB.                              */
/*********************************************************************/
{
  cs_uint32            err_code ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  stats_blk_cb = arn5_stats_get_blk_cb(dpid, dp, mod_id) ;

  if ((stats_blk_cb == NULL) || (sect_id >= ARN5_STATS_MAX_SECT)) {
    err_code = ( (dp == ARN5_DEVICE) ? EARN5_DEV_TBL_NULL :
                 EARN5_PORT_TBL_NULL ) ;
    CS_HNDL_ERROR(dpid, err_code, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;
  if (stats_sect_cb) {
    CS_FREE(stats_sect_cb->enbl_tbl) ;
    if (stats_sect_cb->work_area_tbl != NULL) {
      CS_FREE(stats_sect_cb->work_area_tbl) ;
    }
    CS_FREE(stats_sect_cb) ;
    stats_blk_cb->p_sect_cb[sect_id] = NULL ;
    stats_blk_cb->num_sect-- ;
  }
  return (CS_OK) ;
}

/*********************************************************************
 * Internal functions definitions
 *********************************************************************/

/*********************************************************************
 * Internal function to read the hardware counter
 *********************************************************************/
CS_INLINE cs_uint64  arn5_stats_get_hw_cntr(volatile cs_reg * reg_tbl,
                        cs_uint8 num_regs)
{
  cs_uint8      i ;
  cs_uint64     cntr = 0 ;

  /* The order of read is from MSB to LSB */
  if (!reg_tbl) {
    return (0) ;
  }

  for (i = 0; i < num_regs; i++) {
    cs_uint16  reg_val = CS_REG_READ( &reg_tbl[i] ) ;
    cntr += ( ((cs_uint64) reg_val) << (16 * ((num_regs -1) - i)) ) ;
  }
  return (cntr) ;
}


/********************************************************************
 * Internal function to clear the hardware counter
 ********************************************************************/
CS_INLINE void  arn5_stats_clear_hw_cntr(volatile cs_reg * reg_tbl,
                        cs_uint8 num_regs, cs_boolean read_clear)
{
  cs_uint16     cntr = 0, i ;

  if (!reg_tbl) {
    return ;
  }
  /* The order of read is from MSB to LSB */
  if (read_clear) {
    for (i = 0; i < num_regs; i++) {
      cntr = CS_REG_READ( &reg_tbl[i] ) ;
    }
  }
  else { /* write to clear */
    /* The order of write is from LSB to MSB(reverse of read order) */
    for (i = 0; i < num_regs; i++) {
      CS_REG_WRITE( &reg_tbl[(num_regs - 1) - i], 0 ) ;
    }
  }
}


/********************************************************************
 * This function is STATS internal function. This function gets the
 * hardware counters for the STATS unit data ( RX and TX ) counters.
 ********************************************************************/
static
cs_status arn5_stats_update_unit_data(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 stride_id,
                        cs_uint32 stride_size,
                        const arn5_stats_info_t * p_unit_info,
                        arn5_stats_data_t * p_unit_data,
                        arn5_stats_work_area_t * p_work_area,
                        cs_dir_t dir)
{
  cs_uint16             sl_num = 0 ;
  ARN5_t                * pDev ;
  cs_uint64             counter_val = 0, tmp ;
  volatile cs_reg       * regaddr = NULL ;

  if (dp == ARN5_DEVICE) {
    pDev = ARN5_DEV_ID_TO_DEV_BASE(dpid) ;
  }
  else { /* ARN5_PORT */
    pDev = ARN5_PORT_ID_TO_DEV_BASE(dpid);
    sl_num = stride_id ;
  }

  /* If Rx is valid, read the Rx hw counter and increment */
  if ( (ARN5_STATS_UNIT_IS_RX_VALID(p_unit_info->flags)) &&
       ((dir == CS_RX) || (dir == CS_RX_AND_TX)) ) {
    regaddr = (volatile cs_reg*)
      ARN5_STATS_GET_RX_REG_ADDR(pDev, sl_num, stride_size, p_unit_info) ;

    if (p_unit_info->flags & ARN5_STATS_FLAG_PROTECT_RX) {
      if (dp == ARN5_DEVICE) {
        ARN5_DEV_LOCK (dpid, mod_id) ;
      }
      else {
        ARN5_PORT_LOCK (dpid, mod_id) ;
      }
    }
    counter_val = arn5_stats_get_hw_cntr(regaddr, p_unit_info->num_regs) ;
    if (p_unit_info->flags & ARN5_STATS_FLAG_PROTECT_RX) {
      if (dp == ARN5_DEVICE) {
        ARN5_DEV_UNLOCK (dpid, mod_id) ;
      }
      else {
        ARN5_PORT_UNLOCK (dpid, mod_id) ;
      }
    }

    if (p_unit_info->flags & ARN5_STATS_FLAG_NO_ACCUM) {
      p_unit_data->rx = counter_val ;
    }
    else {
      if (p_unit_info->flags & ARN5_STATS_FLAG_RX_READ_CLR) {
        p_unit_data->rx += counter_val ;
      }
      else if (p_unit_info->flags & ARN5_STATS_FLAG_NO_CLR_RX) {
        cs_uint8  shift = 64 - (p_unit_info->num_regs * 16) ;
        tmp = counter_val - p_work_area->prev_data.rx ;
        p_unit_data->rx += (tmp << shift) >> shift ;
        p_work_area->prev_data.rx = counter_val ;
      }
      else {
        tmp = 0 ;
        tmp = ((~tmp) << (p_unit_info->num_regs * 16)) &
          p_unit_data->rx ;
        p_unit_data->rx = (counter_val | tmp) ;
      }
    }
  }

  if ((ARN5_STATS_UNIT_IS_TX_VALID(p_unit_info->flags)) &&
      ((dir == CS_TX) || (dir == CS_RX_AND_TX))) {
    regaddr = (volatile cs_reg*)
      ARN5_STATS_GET_TX_REG_ADDR(pDev, sl_num, stride_size, p_unit_info) ;

    if (p_unit_info->flags & ARN5_STATS_FLAG_PROTECT_TX) {
      if (dp == ARN5_DEVICE) {
        ARN5_DEV_LOCK (dpid, mod_id) ;
      }
      else {
        ARN5_PORT_LOCK (dpid, mod_id) ;
      }
    }
    counter_val = arn5_stats_get_hw_cntr(regaddr, p_unit_info->num_regs) ;
    if (p_unit_info->flags & ARN5_STATS_FLAG_PROTECT_TX) {
      if (dp == ARN5_DEVICE) {
        ARN5_DEV_UNLOCK (dpid, mod_id) ;
      }
      else {
        ARN5_PORT_UNLOCK (dpid, mod_id) ;
      }
    }

    if (p_unit_info->flags & ARN5_STATS_FLAG_NO_ACCUM) {
      p_unit_data->tx = counter_val ;
    }
    else {
      if (p_unit_info->flags & ARN5_STATS_FLAG_TX_READ_CLR) {
        p_unit_data->tx += counter_val ;
      }
      else if (p_unit_info->flags & ARN5_STATS_FLAG_NO_CLR_TX) {
        cs_uint8  shift = 64 - (p_unit_info->num_regs * 16) ;
        tmp = counter_val - p_work_area->prev_data.tx ;
        p_unit_data->tx += (tmp << shift) >> shift ;
        p_work_area->prev_data.tx = counter_val ;
      }
      else {
        tmp = 0 ;
        tmp = ((~tmp) << (p_unit_info->num_regs * 16)) &
              p_unit_data->tx ;
        p_unit_data->tx = (counter_val | tmp) ;
      }
    }
  }

  return (CS_OK) ;
}


/*********************************************************************
 * This function is STATS internal function. This function clears the
 * hardware counters for the STATS unit data ( RX and TX ) counters.
 *********************************************************************/
static cs_status  arn5_stats_clear_unit_data(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 stride_id,
                        cs_uint32 stride_size,
                        const arn5_stats_info_t * p_unit_info,
                        arn5_stats_data_t * p_unit_data,
                        arn5_stats_work_area_t * p_work_area,
                        cs_dir_t dir)
{
  ARN5_t            * pDev ;
  volatile cs_reg   * regaddr ;
  cs_boolean        read_clr = FALSE ;

  if (dp == ARN5_DEVICE) {
    pDev = ARN5_DEV_ID_TO_DEV_BASE(dpid) ;
  }
  else { /* ARN5_PORT */
    pDev = ARN5_PORT_ID_TO_DEV_BASE(dpid);
  }

  /* If Rx is valid, read the RX hw counter and increment */
  if ((ARN5_STATS_UNIT_IS_RX_VALID(p_unit_info->flags)) &&
      ((dir == CS_RX) || (dir == CS_RX_AND_TX))) {
    regaddr = (volatile cs_reg*)
      ARN5_STATS_GET_RX_REG_ADDR(pDev, stride_id, stride_size, p_unit_info) ;

    if (p_unit_info->flags & ARN5_STATS_FLAG_NO_CLR_RX) {
      /* just to make sure prev_data is up to date at time of clear */
      /* since this is an offset to actual count                    */
      arn5_stats_update_unit_data(dpid, dp, mod_id, stride_id, stride_size,
                                 p_unit_info,
                                 p_unit_data,
                                 p_work_area,
                                 CS_RX) ;
    }
    else {
      read_clr = ( (p_unit_info->flags & ARN5_STATS_FLAG_RX_READ_CLR) ?
                   TRUE : FALSE ) ;
      arn5_stats_clear_hw_cntr(regaddr, p_unit_info->num_regs, read_clr) ;
    }
    p_unit_data->rx = 0 ;
  }

  if (ARN5_STATS_UNIT_IS_TX_VALID(p_unit_info->flags) &&
      ((dir == CS_TX) || (dir == CS_RX_AND_TX))) {
    regaddr = (volatile cs_reg*)
      ARN5_STATS_GET_TX_REG_ADDR(pDev, stride_id, stride_size, p_unit_info) ;

    if (p_unit_info->flags & ARN5_STATS_FLAG_NO_CLR_TX) {
      arn5_stats_update_unit_data(dpid, dp, mod_id, stride_id, stride_size,
                                 p_unit_info,
                                 p_unit_data,
                                 p_work_area,
                                 CS_TX) ;
    }
    else {
      read_clr = ( (p_unit_info->flags & ARN5_STATS_FLAG_TX_READ_CLR) ?
                   TRUE : FALSE ) ;
      arn5_stats_clear_hw_cntr(regaddr, p_unit_info->num_regs, read_clr) ;
    }
    p_unit_data->tx = 0 ;
  }

  return (CS_OK) ;
}


/*********************************************************************
 * This function performs get/print and clear operation on the stats
 * data unit.
 *********************************************************************/
cs_status arn5_stats_unit_cmn_op(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        arn5_stats_sect_cb_t * sect_cb,
                        arn5_stats_op_type_t op,
                        cs_dir_t dir,
                        cs_boolean sect_context)
{
  const arn5_stats_info_t    * p_unit_info ;
  arn5_stats_data_t          * p_unit_data ;
  arn5_stats_work_area_t     * p_work_area = NULL ;

  if (unit_id >= sect_cb->num_entries) {
    CS_PRINT("%s() ERROR: unit_id(%d) > num_entries(%d)\n",
             __FUNCTION__, unit_id, sect_cb->num_entries) ;
    return (CS_ERROR) ;
  }

  /* If Stats is disabled for this unit , return , dont do anything */
  if ((*(sect_cb->enbl_tbl + unit_id)) == FALSE) return (CS_ERROR) ;

  p_unit_info = sect_cb->info_tbl + unit_id ;

  /* Handle special case if special handling flags are set */
  if (arn5_stats_is_spl_hndlr_valid(p_unit_info, op)) {
    return arn5_stats_spl_handler(dpid, dp, mod_id, sect_id,
                                 unit_id, op, dir, sect_context) ;
  }

  p_unit_data = sect_cb->pStats + unit_id ;
  if (sect_cb->work_area_tbl != NULL) {
    p_work_area = &sect_cb->work_area_tbl[unit_id] ;
  }

  switch (op) {
    case ARN5_STATS_PRINT :
      if (ARN5_STATS_UNIT_IS_TIE_VALID(p_unit_info->flags)) {
        if (arn5_get_sect_print_flag() == 1) {
          if (ARN5_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
            /* update RX pkts & bytes first */
            if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
            }
            /* update TX pkts & bytes secondly */
            if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
            }
          }
        }
        else {
          if (ARN5_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
            /* update RX pkts & bytes first */
            if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
            }
            /* update TX pkts & bytes secondly */
            if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
            }
          }
          else {
            /* update RX pkts & bytes first */
            if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
            }
            /* update TX pkts & bytes secondly */
            if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
            }
          }
        }
      }
      else {
        arn5_stats_update_unit_data(dpid, dp, mod_id,
                                   sect_cb->stride_id,
                                   sect_cb->stride_size,
                                   p_unit_info,
                                   p_unit_data,
                                   p_work_area,
                                   dir) ;
     }

      p_unit_info = sect_cb->info_tbl + unit_id ;
      p_unit_data = sect_cb->pStats + unit_id ;

      arn5_stats_print_unit_cntr(p_unit_info->name, p_unit_info->flags,
                                p_unit_data) ;
      break ;

    case ARN5_STATS_GET :
      if (ARN5_STATS_UNIT_IS_TIE_VALID(p_unit_info->flags)) {
        if (arn5_get_sect_print_flag() == 1) {
          if (ARN5_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
                /* update RX pkts & bytes first */
                if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
            }
            /* update TX pkts & bytes secondly */
            if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
            }
          }
        } else {
          if (ARN5_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
                /* update RX pkts & bytes first */
                if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
            }
            /* update TX pkts & bytes secondly */
            if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
            }
          } else {
            /* update RX pkts & bytes first */
            if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1 ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_RX) ;
            }
            /* update TX pkts & bytes secondly */
            if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              arn5_stats_update_unit_data(dpid, dp, mod_id,
                                         sect_cb->stride_id,
                                         sect_cb->stride_size,
                                         p_unit_info,
                                         p_unit_data,
                                         p_work_area,
                                         CS_TX) ;
            }
          }
        }
      } else {
        arn5_stats_update_unit_data(dpid, dp, mod_id,
                                   sect_cb->stride_id,
                                   sect_cb->stride_size,
                                   p_unit_info,
                                   p_unit_data,
                                   p_work_area,
                                   dir) ;
      }
    break ;

    case ARN5_STATS_CLEAR :
      if ((arn5_get_sect_print_flag() == 1)
          || (!ARN5_STATS_UNIT_IS_TIE_VALID(p_unit_info->flags)) ) {
        arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                  sect_cb->stride_id,
                                  sect_cb->stride_size,
                                  p_unit_info,
                                  p_unit_data,
                                  p_work_area,
                                  dir) ;
      }
      else {
        if (ARN5_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
          /* clear RX pkts & bytes first */
          if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_RX) ;
            p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
            p_unit_data = sect_cb->pStats + unit_id + 1 ;
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_RX) ;
          }
          /* clear TX pkts & bytes secondly */
          if ((dir == CS_TX) || (dir == CS_RX_AND_TX)) {
            p_unit_info = sect_cb->info_tbl + unit_id ;
            p_unit_data = sect_cb->pStats + unit_id ;
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_TX) ;
            p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
            p_unit_data = sect_cb->pStats + unit_id + 1 ;
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_TX) ;
          }
        }
        else {
          /* clear RX pkts & bytes first */
          if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
            p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
            p_unit_data = sect_cb->pStats + unit_id - 1 ;
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_RX) ;
            p_unit_info = sect_cb->info_tbl + unit_id ;
            p_unit_data = sect_cb->pStats + unit_id ;
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_RX) ;
          }
          /* clear TX pkts & bytes secondly */
          if ((dir == CS_RX) || (dir == CS_RX_AND_TX)) {
            p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
            p_unit_data = sect_cb->pStats + unit_id - 1;
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_TX) ;
            p_unit_info = sect_cb->info_tbl + unit_id ;
            p_unit_data = sect_cb->pStats + unit_id ;
            arn5_stats_clear_unit_data(dpid, dp, mod_id,
                                      sect_cb->stride_id,
                                      sect_cb->stride_size,
                                      p_unit_info,
                                      p_unit_data,
                                      p_work_area,
                                      CS_TX) ;
          }
        }
      }
      break ;
  }

  return (CS_OK) ;
}


/**********************************************************************
 * This is Internal function for STATS only use. This function handles
 * GET/PRINT and CLEAR requests at the Section level.
 **********************************************************************/
cs_status arn5_stats_sect_cmn_op(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        arn5_stats_sect_cb_t * stats_sect_cb,
                        arn5_stats_op_type_t op,
                        cs_dir_t dir)
{
  cs_uint32   ii ;

  arn5_set_sect_print_flag(1) ;

  for (ii = 0; ii < stats_sect_cb->num_entries; ii++) {
    arn5_stats_unit_cmn_op(dpid, dp, mod_id, sect_id, ii,
                          stats_sect_cb, op, dir, TRUE) ;
  }

  return (CS_OK) ;
}


/*********************************************************************
 * Check if any special handling is required based on the flags set for
 * the unit data.
 *********************************************************************/
static
cs_boolean arn5_stats_is_spl_hndlr_valid(const arn5_stats_info_t * pinfo,
                        arn5_stats_op_type_t op)
{
  cs_uint32 val = 0 ;

  switch(op) {
    case ARN5_STATS_GET :
        val = (pinfo->flags & ARN5_STATS_FLAG_SPL_GET) ;
        break ;
    case ARN5_STATS_PRINT :
        val = (pinfo->flags & ARN5_STATS_FLAG_SPL_PRINT) ;
        break ;
    case ARN5_STATS_CLEAR :
        val = (pinfo->flags & ARN5_STATS_FLAG_SPL_CLEAR) ;
        break ;
  }

  return ((val) ? TRUE : FALSE) ;
}


/*********************************************************************
 * Call the Special handler.
 *********************************************************************/
static
cs_status arn5_stats_spl_handler(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id,
                        cs_uint16 unit_id,
                        arn5_stats_op_type_t op,
                        cs_dir_t dir,
                        cs_boolean sect_context)
{
  arn5_stats_blk_cb_t * blk_cb = arn5_stats_get_blk_cb(dpid, dp, mod_id) ;

  if (!blk_cb) {
    return (CS_ERROR) ;
  }

  switch (op) {
    case ARN5_STATS_GET :
      if (blk_cb->get_func) {
        blk_cb->get_func(dpid, sect_id, unit_id, dir, sect_context) ;
      }
      break ;
    case ARN5_STATS_PRINT :
      if (blk_cb->print_func) {
        blk_cb->print_func(dpid, sect_id, unit_id, dir, sect_context) ;
      }
      break ;
    case ARN5_STATS_CLEAR :
      if (blk_cb->clear_func) {
        blk_cb->clear_func(dpid, sect_id, unit_id, dir, sect_context) ;
      }
      break ;
  }
  return (CS_OK) ;
}


/*********************************************************************
 * GET POINTER TO STATS BLOCK CB
 *********************************************************************/
arn5_stats_blk_cb_t * arn5_stats_get_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id)
{
  if (dp == ARN5_DEVICE) {
    arn5_dev_stats_cb_t  * p = NULL ;

    p = ARN5_DEV_GET_STATS_CB( (cs_uint8)dpid ) ;
    if (p) {
      switch (mod_id) {
        case ARN5_ID_HOST_IF : return (p->host_if) ;
        default :
          return (NULL) ; /* error */
      }
    }
  }
  else { /* ARN5_PORT */
    arn5_port_stats_cb_t * p = NULL ;

    p = ARN5_PORT_GET_STATS_CB(dpid) ;
    if (p) {
      switch (mod_id) {
        case ARN5_ID_HOST_IF : return (p->host_if) ;
        case ARN5_ID_FRAMER  : return (p->framer) ;
        case ARN5_ID_LIF     : return (p->lif) ;
        case ARN5_ID_ETH     : return (p->eth) ;
        case ARN5_ID_PPRBS   : return (p->pprbs) ;
        default :
          return (NULL) ;  /* error */
      }
    }
  }

  return (NULL) ;
}


/********************************************************************
 * SET THE BLOCK CB POINTER IN PORT STATS CB
 ********************************************************************/
static
cs_status arn5_stats_set_blk_cb(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        arn5_module_id_t mod_id,
                        arn5_stats_blk_cb_t * blk_cb)
{
  if (dp == ARN5_DEVICE) {
    arn5_dev_stats_cb_t  * p = NULL ;

    p = ARN5_DEV_GET_STATS_CB( (cs_uint8)dpid ) ;
    if (p) {
      switch (mod_id) {
        case ARN5_ID_HOST_IF : (p->host_if = blk_cb) ; break ;
        default : return (CS_ERROR) ;
      }
    }
  }
  else {  /* ARN5_PORT */
    arn5_port_stats_cb_t * p = NULL ;

    p = ARN5_PORT_GET_STATS_CB(dpid) ;
    switch (mod_id) {
      case ARN5_ID_HOST_IF : p->host_if = blk_cb ; break ;
      case ARN5_ID_FRAMER  : p->framer  = blk_cb ; break ;
      case ARN5_ID_LIF     : p->lif     = blk_cb ; break ;
      case ARN5_ID_ETH     : p->eth     = blk_cb ; break ;
      case ARN5_ID_PPRBS   : p->pprbs   = blk_cb ; break ;
      default : return (CS_ERROR) ;
    }
  }
  return (CS_OK) ;
}


/*********************************************************************
 * PRINT BLOCK HEADER
 *********************************************************************/
void arn5_stats_print_blk_hdr(cs_uint16 dpid,
                        arn5_dev_or_port_t dp,
                        cs_int8 * blk_name)
{
  if (dp == ARN5_DEVICE) {
    if ( arn5_drvr_is_esc_code_enbld() ) {
      CS_PRINT("\033[4m\033[1m") ; /* underline + bold */
      CS_PRINT("\n\t\t\tDev-%d \t %s STATISTICS\n", dpid, blk_name) ;
      CS_PRINT("\033[m") ; /* put back to normal printing */
    }

    else {
      CS_PRINT("= = = = = = = = = = = = = = = = = = = = = = = = = = = =\n") ;
      CS_PRINT("Dev-%d \t %s STATISTICS\n",dpid, blk_name) ;
      CS_PRINT("= = = = = = = = = = = = = = = = = = = = = = = = = = = =\n") ;
    }
  }

  else { /* ARN5_PORT */
    if ( arn5_drvr_is_esc_code_enbld() ) {
      CS_PRINT("\033[4m\033[1m") ; /* underline + bold */
      CS_PRINT("\n\t\t\tPort-0x%x \t %s STATISTICS\n", dpid, blk_name) ;
      CS_PRINT("\033[m") ; /* put back to normal printing */
    }

    else {
      CS_PRINT("= = = = = = = = = = = = = = = = = = = = = = = = = = = =\n") ;
      CS_PRINT("Port-0x%x \t %s STATISTICS\n", dpid, blk_name) ;
      CS_PRINT("= = = = = = = = = = = = = = = = = = = = = = = = = = = =\n") ;
    }
  }
  return ;
}


/************************************************************************
 * PRINT SECTION HEADER
 ***********************************************************************/
void arn5_stats_print_sect_hdr(cs_uint16 dpid, arn5_dev_or_port_t dp,
                        cs_int8 * name)
{
  cs_uint8  i ;
  cs_int8   myName[] = "--------------------------------------------------" ;

  for (i = 0 ; i < sizeof(myName); i++) {
    if (name[i] != 0) {
      myName[i] = name[i] ;
    }
    else {
      myName[i] = ' ' ;
      break ;
    }
  }

  if (dp == ARN5_DEVICE) {
      CS_PRINT("--- Dev-%d %-50s------ RX --------------- TX --------\n",
               dpid, myName, "", "") ;
  }

  else { /* ARN5_PORT */
      CS_PRINT("--- Port-%d %-50s----- RX --------------- TX --------\n",
               dpid, myName, "", "") ;
  }

  return ;
}


void arn5_stats_print_unit_cntr(const cs_uint8 * name, cs_uint32 flags,
                               arn5_stats_data_t * p_data)
{
  cs_uint64 rx, tx;

  rx = p_data->rx ;
  tx = p_data->tx ;

  if ( arn5_drvr_is_stats_in_hex_fmt() ) {
    arn5_stats_print_unit_cntr_in_hex(name, flags, rx, tx) ;
  }
  else {
    arn5_stats_print_unit_cntr_in_dec(name, flags, rx, tx) ;
  }
}


static
void arn5_stats_print_unit_cntr_in_hex(const cs_uint8 * name, cs_uint32 flags,
                        cs_uint64 rx, cs_uint64 tx)
{
  cs_uint32 urx, lrx, utx, ltx;

  urx = ARN5_STATS_GET_UPPER_32BIT_HEX(rx) ;
  lrx = ARN5_STATS_GET_LOWER_32BIT_HEX(rx) ;
  utx = ARN5_STATS_GET_UPPER_32BIT_HEX(tx) ;
  ltx = ARN5_STATS_GET_LOWER_32BIT_HEX(tx) ;

  if (ARN5_STATS_UNIT_IS_RX_VALID(flags) &&
      ARN5_STATS_UNIT_IS_TX_VALID(flags)) { /* Rx & Tx */
    if (arn5_drvr_is_mask_zero_cntr() == TRUE) {
      if (tx|rx) {
        if ((urx>0)&&(utx>0)) {
          CS_PRINT("\t%-40s :  %8x%08xH %8x%08xH\n", name,
                   urx, lrx, utx, ltx) ;
        } else if (urx>0) {
          CS_PRINT("\t%-40s :  %8x%08xH %16xH\n", name,
                   urx, lrx, ltx) ;
        } else if (utx>0) {
          CS_PRINT("\t%-40s :  %16xH %8x%08xH\n", name,
                   lrx, utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %16xH %16xH\n", name,
                   lrx, ltx) ;
        }
      }
    } else {
      if ((urx>0)&&(utx>0)) {
        CS_PRINT("\t%-40s :  %8x%08xH %8x%08xH\n", name,
                 urx, lrx, utx, ltx) ;
      } else if (urx>0) {
        CS_PRINT("\t%-40s :  %8x%08xH %16xH\n", name,
                 urx, lrx, ltx) ;
      } else if (utx>0) {
        CS_PRINT("\t%-40s :  %16xH %8x%08xH\n", name,
                 lrx, utx, ltx) ;
      } else {
        CS_PRINT("\t%-40s :  %16xH %16xH\n", name,
                 lrx, ltx) ;
      }
    }
  }

  else if (ARN5_STATS_UNIT_IS_RX_VALID(flags)){ /* Rx only */
    if (arn5_drvr_is_mask_zero_cntr() == TRUE) {
      if (rx) {
        if (urx>0) {
          CS_PRINT("\t%-40s :  %8x%08xH %18s\n", name,
                   urx, lrx, "N/A") ;
        } else {
          CS_PRINT("\t%-40s :  %16xH %18s\n", name,
                   lrx, "N/A") ;
        }
      }
    } else {
      if (urx>0) {
        CS_PRINT("\t%-40s :  %8x%08xH %18s\n", name,
                 urx, lrx, "N/A") ;
      } else {
        CS_PRINT("\t%-40s :  %16xH %18s\n", name,
                 lrx, "N/A") ;
      }
    }
  }

  else if (ARN5_STATS_UNIT_IS_TX_VALID(flags)){ /* Tx only */
    if (arn5_drvr_is_mask_zero_cntr() == TRUE) {
      if (tx) {
        if (utx>0) {
          CS_PRINT("\t%-40s :  %18s %8x%08xH\n", name, "N/A",
                   utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %18s %16xH\n", name, "N/A",
                   ltx) ;
        }
      }
    } else {
      if (utx>0) {
        CS_PRINT("\t%-40s :  %18s %8x%08xH\n", name, "N/A",
                 utx, ltx) ;
      } else {
        CS_PRINT("\t%-40s :  %18s %16xH\n", name, "N/A",
                 ltx) ;
      }
    }
  }
}


static
void arn5_stats_print_unit_cntr_in_dec(const cs_uint8 * name, cs_uint32 flags,
                        cs_uint64 rx, cs_uint64 tx)
{
  cs_uint32 urx, lrx, utx, ltx;

  urx = ARN5_STATS_GET_BILLIONS_DEC(rx) ;
  lrx = ARN5_STATS_GET_BILLIONS_MOD_DEC(rx) ;
  utx = ARN5_STATS_GET_BILLIONS_DEC(tx) ;
  ltx = ARN5_STATS_GET_BILLIONS_MOD_DEC(tx) ;

  if (ARN5_STATS_UNIT_IS_RX_VALID(flags) &&
      ARN5_STATS_UNIT_IS_TX_VALID(flags)) { /* Rx & Tx */
    if (arn5_drvr_is_mask_zero_cntr() == TRUE) {
      if (tx|rx) {
        if ((urx>0)&&(utx>0)) {
          CS_PRINT("\t%-40s :  %9u%09u %9u%09u\n", name,
                   urx, lrx, utx, ltx) ;
        } else if (urx>0) {
          CS_PRINT("\t%-40s :  %9u%09u %18u\n", name,
                   urx, lrx, ltx) ;
        } else if (utx>0) {
          CS_PRINT("\t%-40s :  %18u %9u%09u\n", name,
                   lrx, utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %18u %18u\n", name,
                   lrx, ltx) ;
        }
      }
    } else {
      if ((urx>0)&&(utx>0)) {
        CS_PRINT("\t%-40s :  %9u%09u %9u%09u\n", name,
                 urx, lrx, utx, ltx) ;
      } else if (urx>0) {
        CS_PRINT("\t%-40s :  %9u%09u %18u\n", name,
                 urx, lrx, ltx) ;
      } else if (utx>0) {
        CS_PRINT("\t%-40s :  %18u %9u%09u\n", name,
                 lrx, utx, ltx) ;
      } else {
        CS_PRINT("\t%-40s :  %18u %18u\n", name,
                 lrx, ltx) ;
      }
    }
  }

  else if (ARN5_STATS_UNIT_IS_RX_VALID(flags)){ /* Rx only */
    if (arn5_drvr_is_mask_zero_cntr() == TRUE) {
      if (rx) {
        if (urx>0) {
          CS_PRINT("\t%-40s :  %9u%09u %18s\n", name,
                   urx, lrx, "N/A") ;
        } else {
          CS_PRINT("\t%-40s :  %18u %18s\n", name,
                   lrx, "N/A") ;
        }
      }
    } else {
      if (urx>0) {
        CS_PRINT("\t%-40s :  %9u%09u %18s\n", name,
                 urx, lrx, "N/A") ;
      } else {
        CS_PRINT("\t%-40s :  %18u %18s\n", name,
                 lrx, "N/A") ;
      }
    }
  }
  else if (ARN5_STATS_UNIT_IS_TX_VALID(flags)){ /* Tx only */
    if (arn5_drvr_is_mask_zero_cntr() == TRUE) {
      if (tx) {
        if (utx>0) {
          CS_PRINT("\t%-40s :  %18s %9u%09u\n", name, "N/A",
                   utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %18s %18u\n", name, "N/A",
                   ltx) ;
        }
      }
    } else {
      if (utx>0) {
        CS_PRINT("\t%-40s :  %18s %9u%09u\n", name, "N/A",
                 utx, ltx) ;
      } else {
        CS_PRINT("\t%-40s :  %18s %18u\n", name, "N/A",
                 ltx) ;
      }
    }
  }
}



#if 0 /***  FIXME ****/

/*********************************************************************/
/* $_hdr_start  UPDATE STATS FOR UNIT ON COUNTER IRQS                */
/* CATEGORY   : Device/Port                                          */
/* ACCESS     : PRIVATE                                              */
/* BLOCK      : General                                              */
/* CHIP       : Arsenal5                                             */
cs_status arn5_stats_update_unit_ctr(cs_uint16 dpid,
                                    arn5_dev_or_port_t dp,
                                    arn5_module_id_t mod_id,
                                    cs_uint16 sect_id,
                                    cs_uint16 unit_id,
                                    cs_dir_t dir, cs_uint16 ctl)
/* INPUTS     : o Device/Port Id                                     */
/*              o Module ID                                          */
/*              o Section ID                                         */
/* OUTPUTS    : ----                                                 */
/* RETURNS    : CS_OK or CS_ERROR                                    */
/* DESCRIPTION:                                                      */
/* Update the Stats counter on receipt of interrupt related to cntr  */
/* overflow or rollover.                                             */
/* $_hdr_end                                                         */
/*********************************************************************/
{
  cs_uint32               error = 0 ;
  arn5_stats_sect_cb_t     * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t      * stats_blk_cb ;
  const arn5_stats_info_t  * info_tbl = NULL ;
  arn5_stats_data_t        * p_unit_data ;
  cs_uint64               val = 0, tmp ;
  volatile cs_reg         * regaddr = NULL ;
  cs_uint16               sl_num ;
  ARN5_t                   * pDev ;
  cs_boolean              read_clr = FALSE ;

  if(ARN5_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = arn5_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "%s not registered for Statistics\n",
                   ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (unit_id >= stats_sect_cb->num_entries) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  info_tbl = stats_sect_cb->info_tbl ;
  p_unit_data = stats_sect_cb->pStats + unit_id ;

  switch (ctl) {
    case ARN5_STATS_CTR_OVRFLOW :
    break ;

    case ARN5_STATS_CTR_ROLLOVR :
      tmp = 1 ;
      val = (tmp << (info_tbl->num_regs * 16)) ;
      tmp = 0 ;
      if (dir == CS_TX) {
        p_unit_data->tx &= ((~tmp) << (info_tbl->num_regs * 16)) ;
        p_unit_data->tx += val ;
      }
      else {
        p_unit_data->rx &= ((~tmp) << (info_tbl->num_regs * 16)) ;
        p_unit_data->rx += val ;
      }
    break ;

    case ARN5_STATS_CTR_SATURATE :
    break ;

    case ARN5_STATS_CTR_MSB_0_TO_1 :
      pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id);
      sl_num = ARN5_PORT_ID_TO_PORT_NUM(port_id);

      if ((ARN5_STATS_UNIT_IS_RX_VALID(info_tbl->flags)) &&
          (dir == CS_RX)) {

        /* If this is not a slice register, then sl_num better be 0 */
        if ( !(ARN5_IS_SLICE_REG(info_tbl->rx_reg_ofst)) ) {
          sl_num = 0 ;
        }

        regaddr = (volatile cs_reg*)
                    ARN5_STATS_GET_RX_REG_ADDR(pDev, sl_num,
                                              stats_sect_cb->stride_id,
                                              info_tbl) ;
        read_clr = (info_tbl->flags & ARN5_STATS_FLAG_RX_READ_CLR) ?
                      TRUE : FALSE ;
        val = arn5_stats_get_hw_cntr(regaddr, info_tbl->num_regs) ;
        arn5_stats_clear_hw_cntr(regaddr, info_tbl->num_regs, read_clr) ;
        p_unit_data->rx += val ;
      }
      else {
        /* If this is not a slice register, then sl_num better be 0 */
        if ( !(ARN5_IS_SLICE_REG(info_tbl->tx_reg_ofst)) ) {
          sl_num = 0 ;
        }

        regaddr = (volatile cs_reg*)
                    ARN5_STATS_GET_TX_REG_ADDR(pDev, sl_num,
                                              stats_sect_cb->stride_id,
                                              info_tbl) ;
        read_clr = (info_tbl->flags & ARN5_STATS_FLAG_TX_READ_CLR) ?
                      TRUE : FALSE ;
        val = arn5_stats_get_hw_cntr(regaddr, info_tbl->num_regs) ;
        arn5_stats_clear_hw_cntr(regaddr, info_tbl->num_regs, read_clr) ;
        p_unit_data->tx += val ;
      }
    break ;
  }

  return (CS_OK) ;
}

#endif /***  FIXME ****/


/****************************************
 * Set Section print flag               *
 ****************************************/
void arn5_set_sect_print_flag(cs_uint32 val)
{
  _arn5_sect_print_flag = val ;
}

/****************************************
 * Get Section print flag               *
 ****************************************/
cs_uint32 arn5_get_sect_print_flag(void)
{
  return (_arn5_sect_print_flag) ;
}

