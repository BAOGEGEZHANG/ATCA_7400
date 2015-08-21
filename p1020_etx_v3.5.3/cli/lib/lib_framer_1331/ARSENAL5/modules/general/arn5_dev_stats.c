/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_dev_stats.c
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



/***********************************************************************
 ***********************************************************************
 *********************    DEVICE  API's       **************************
 ***********************************************************************
 ***********************************************************************/

/************************************************************************/
/* $rtn_hdr_start  PRINT DEVICE STATISTICS                              */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_print_stats(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints statistics for the device.                                    */
/* It prints the Statistics of all the blocks that are registered and   */
/* are active in the current mode.                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     error = 0 ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  CS_PRINT("\n") ;
  if ( arn5_drvr_is_esc_code_enbld() ) {
    CS_PRINT("\033[7m Device-%d  STATISTICS :\033[m\n",
             dev_id) ;
  }
  else {
    CS_PRINT(" Device-%d  STATISTICS :\n", dev_id) ;
  }

  if ( ARN5_STATS_IS_BLK_REGISTERED((cs_uint16)dev_id, ARN5_DEVICE,
                                  ARN5_ID_HOST_IF) ) {
    arn5_dev_print_blk_stats(dev_id, ARN5_ID_HOST_IF) ;
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  PRINT DEVICE BLOCK STATS                             */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_print_blk_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the block's statistics for the device.                        */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0, ii ;
  arn5_stats_blk_cb_t   * blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,  mod_id) ;
  if ( blk_cb == NULL ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
                  "%s not registered for Statistics\n",
                  ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if (blk_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s\n", blk_cb->blk_name) ;
    return (CS_ERROR) ;
  }

  arn5_stats_print_blk_hdr((cs_uint16)dev_id, ARN5_DEVICE,
                          blk_cb->blk_name) ;

  for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    /* Print Section stats if valid and are enabled */
    if ((blk_cb->p_sect_cb[ii]) &&
        (blk_cb->p_sect_cb[ii]->enabled)) {
      arn5_dev_print_sect_stats(dev_id, mod_id, ii) ;
    }
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  PRINT DEVICE SECTION STATS                           */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_print_sect_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the Stats for the user specified section of the device.       */
/* Stats within a block are organised as a collection of section stats. */
/* Each section is identified by mod_id and section_id combo.           */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * sect_cb = NULL ;
  arn5_stats_blk_cb_t   * blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL) ;
    return (CS_ERROR) ;
  }

  blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE, mod_id) ;
  if (blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((sect_cb = blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  if (sect_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s section %s\n",
             blk_cb->blk_name, sect_cb->sect_name) ;
    return (CS_ERROR) ;
  }

  arn5_stats_print_sect_hdr((cs_uint16)dev_id, ARN5_DEVICE,
                           sect_cb->sect_name) ;

  arn5_stats_sect_cmn_op((cs_uint16)dev_id, ARN5_DEVICE, mod_id, sect_id,
                        sect_cb, ARN5_STATS_PRINT, CS_RX_AND_TX) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  PRINT DEVICE UNIT STATS                              */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_print_unit_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the Stats for the user specified unit for the device.         */
/* Statistics for the device are organised as a collection of block     */
/* stats. Each block has number of section stats identified by section  */
/* id. Each section then has unit stats. Unit stat is the atomic entity */
/* of the stats and carry RX and TX stats. Each unit is uniquely        */
/* identified by block_id, section_id and unit_id combo.                */
/* Refer to the individual header files for the section ID and unit_id  */
/* defines.                                                             */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;
  arn5_stats_data_t     * p_unit_data ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, "\n") ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  if ( stats_sect_cb->enabled == FALSE ) {
    CS_PRINT("STATISTICS DISABLED FOR %s section %s\n",
             stats_blk_cb->blk_name, stats_sect_cb->sect_name) ;
    return (CS_ERROR) ;
  }

  p_unit_data = stats_sect_cb->pStats + unit_id ;
  if (*(stats_sect_cb->enbl_tbl + unit_id) == FALSE) {
    CS_PRINT("STATS DISABLED FOR %s : SECT_ID : %d UNIT_ID : %d\n",
             stats_blk_cb->blk_name, sect_id, unit_id) ;
    return (CS_ERROR) ;
  }

  arn5_set_sect_print_flag(0) ;

  return arn5_stats_unit_cmn_op((cs_uint16)dev_id, ARN5_DEVICE,
                               mod_id, sect_id, unit_id,
                               stats_sect_cb, ARN5_STATS_PRINT,
                               CS_RX_AND_TX,
                               FALSE) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE STATS                                   */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_stats(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clear the Device stats. It clears the hardware registers and also the*/
/* the internal stats structure which accumulate the stats for both     */
/* RX and TX direction.                                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_dev_clear_dir_stats(dev_id, CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE STATS FOR THE DIRECTION (RX/TX/BOTH)    */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_dir_stats(cs_uint8 dev_id, cs_dir_t dir)
/* INPUTS     : o Device Id                                             */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clear the RX/TX or Both stats. It clears the hardware registers      */
/* and also the the internal stats structure which accumulate the stats.*/
/* It clears the stats and hw register for the direction specified.     */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     error = 0 ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, "\n") ;
    return (CS_ERROR) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED((cs_uint16)dev_id, ARN5_DEVICE,
                                  ARN5_ID_HOST_IF)) {
    arn5_dev_clear_blk_dir_stats(dev_id, ARN5_ID_HOST_IF, dir) ;
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE BLOCK STATS                             */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_blk_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id)
/* INPUTS     : o Device Id                                             */
/*              o Module Id                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the stats for the block.                                      */
/* This API clears both the hardware counters and internal stats block  */
/* stats structure.                                                     */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_dev_clear_blk_dir_stats(dev_id, mod_id, CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE BLOCK STATS FOR DIRECTION(RX/TX or BOTH)*/
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_blk_dir_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id, cs_dir_t dir)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the stats for the block.                                      */
/* It clears the stats and hw register for the direction specified.     */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0, ii ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, "\n") ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if ( stats_blk_cb == NULL ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    if ((stats_blk_cb->p_sect_cb[ii]) &&
        (stats_blk_cb->p_sect_cb[ii]->enabled)) {
      arn5_dev_clear_sect_dir_stats(dev_id, mod_id, ii, dir) ;
    }
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE SECTION STATS                           */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_sect_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the section in user specified block's section.  */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_dev_clear_sect_dir_stats(dev_id, mod_id, sect_id,
                                      CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE SECTION STATS FOR RX/TX OR BOTH         */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_sect_dir_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_dir_t dir)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the section in user specified block's section.  */
/* It clears the stats and hw register for the direction specified.     */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, "\n") ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (stats_sect_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s section %s\n",
             stats_blk_cb->blk_name, stats_sect_cb->sect_name) ;
    return (CS_ERROR) ;
  }

  arn5_stats_sect_cmn_op((cs_uint16)dev_id, ARN5_DEVICE, mod_id,
                        sect_id, stats_sect_cb,
                        ARN5_STATS_CLEAR, dir) ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE STATS UNIT                              */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_unit_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the unit in user specified block's section.     */
/* It clears the stats structure and also the hardware counters.        */
/* Refer to the individual header files for the section ID and unit Id  */
/* defines.                                                             */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_dev_clear_unit_dir_stats(dev_id, mod_id,
                        sect_id, unit_id, CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR DEVICE STATS UNIT FOR RX OR TX OR BOTH         */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_clear_unit_dir_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id,
                        cs_uint16 unit_id, cs_dir_t dir)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the unit in user specified block's section.     */
/* It clears the stats and hw register for the direction specified.     */
/* Refer to the individual header files for the section ID and unit Id  */
/* defines.                                                             */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id,
                                      ARN5_DEVICE, mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (*(stats_sect_cb->enbl_tbl + unit_id) == FALSE) {
    CS_PRINT("STATS DISABLED FOR %s : SECT_ID : %d UNIT_ID : %d\n",
             stats_blk_cb->blk_name, sect_id, unit_id) ;
    return (CS_ERROR) ;
  }

  arn5_set_sect_print_flag(0) ;

  return arn5_stats_unit_cmn_op((cs_uint16)dev_id, ARN5_DEVICE,
                               mod_id, sect_id, unit_id,
                               stats_sect_cb, ARN5_STATS_CLEAR,
                               CS_RX_AND_TX, FALSE) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET DEVICE STATS                                     */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_get_stats(cs_uint8 dev_id, arn5_dev_stats_t * pStats)
/* INPUTS     : o Device Id                                             */
/*              o Pointer to user allocated arn5_dev_stats_t struct     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Get the Stats for user specified Device.                             */
/* User should allocate at least as many bytes as size of               */
/* arn5_dev_stats_t structure and pass the pointer to that as pStats    */
/* argument to the function.                                            */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     error = 0 ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED((cs_uint16)dev_id, ARN5_DEVICE,
                                  ARN5_ID_HOST_IF)) {
    arn5_dev_get_blk_stats(dev_id, ARN5_ID_HOST_IF, &pStats->host_if,
                          sizeof(arn5_host_if_dev_stats_t)) ;
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET DEVICE BLOCK STATS                               */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_get_blk_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        void * pStats, cs_uint16 len)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Pointer to user allocated structure                   */
/*              o Size of the user allocated structure                  */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Gets the Stats for the section in user specified block.              */
/* User is responsible for allocating the memory where the block        */
/* stats will be copied back and pass the pointer to that as pStats     */
/* argument to the API. The size of the memory to be allocated is the   */
/* size of the blocks stats structure. Block's stats structure can be   */
/* found in individual blocks's header file. The len field specifies    */
/* the length of the user allocated bytes.                              */
/* Refer to the individual header files for definition of the block     */
/* stats structures definitions.                                        */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/*                                                                      */
/* The [len] parameter is the length of the user allocated memory where */
/* block stats will be copied.                                          */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0, ii ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;
  cs_uint16             blk_stats_len ;
  void                  * p_blk_stats ;
  arn5_dev_stats_t      * p_dev_stats ;

  if(ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "%s not registered for Statistics\n", stats_blk_cb->blk_name) ;
    return (CS_ERROR) ;
  }

  p_dev_stats = ARN5_DEV_GET_STATS_PTR(dev_id) ;

  switch (mod_id) {
    case ARN5_ID_HOST_IF  :
      p_blk_stats = (void*) &p_dev_stats->host_if ;
      blk_stats_len = sizeof(arn5_host_if_dev_stats_t) ;
      break ;

    default :
      return (CS_ERROR) ;
  }

  if (len < blk_stats_len) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_SIZE,
                  "len s/b %d\n", blk_stats_len) ;
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    /* Print Section stats if valid and are enabled */
    if ((stats_blk_cb->p_sect_cb[ii]) &&
        (stats_blk_cb->p_sect_cb[ii]->enabled)) {
      arn5_stats_sect_cmn_op((cs_uint16)dev_id, ARN5_DEVICE,
                            mod_id, ii,
                            stats_blk_cb->p_sect_cb[ii],
                            ARN5_STATS_GET, CS_RX_AND_TX) ;
    }
  }

  CS_MEMCPY(pStats, p_blk_stats, len) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET DEVICE SECTION STATS                             */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_get_sect_stats(cs_uint8 dev_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id,
                        void * pStats, cs_uint16 len)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Pointer to user allocated structure                   */
/*              o Length of the user allocated structure                */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Gets the Stats for the section in user specified block.              */
/* User should allocate memory of alteast as many bytes as the size of  */
/* section stats structure and pass its pointer as pStats aregument to  */
/* the API. If the size of the memory is less than the size of the      */
/* section stats structure, API will return CS_ERROR.                   */
/* Refer to the individual header files for the section stats struct    */
/* definitions and for section ID defines.                              */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/*                                                                      */
/* The [len] parameter is the length of the user allocated memory where */
/* section stats will be copied.                                        */
/*                                                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;
  arn5_stats_data_t     * p_unit_data ;
  cs_uint16             sect_stats_len ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, ":dev_id : %d\n", dev_id) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_NULL_PTR, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
                  "%s not registered for Statistics\n",
                  ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  sect_stats_len = (sizeof(arn5_stats_data_t) * stats_sect_cb->num_entries) ;

  if ( len < sect_stats_len ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_SIZE,
                  "len s/b %d\n", sect_stats_len) ;
    return (CS_ERROR) ;
  }

  p_unit_data = stats_sect_cb->pStats ;
  arn5_stats_sect_cmn_op((cs_uint16)dev_id, ARN5_DEVICE,
                        mod_id, sect_id,
                        stats_sect_cb, ARN5_STATS_GET, CS_RX_AND_TX) ;

  CS_MEMCPY(pStats, p_unit_data, len) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET DEVICE UNIT STATS                                */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_get_unit_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id,
                        arn5_stats_data_t * pUnit_Stats)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/*              o Pointer to user allocated arn5_stats_data_t structure */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Get unit stats for the user specified unit with specified block's    */
/* section. unit stats carry TX and RX stats and for the given stats    */
/* Both RX and RX or onlt RX or only TX might be valid. In case only    */
/* one direction stats are valid, the other counter will be Zero.       */
/* Refer to the individual header files for the section ID and unit id  */
/* defines.                                                             */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;
  arn5_stats_data_t     * p_unit_data ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (pUnit_Stats == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_NULL_PTR, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  if (unit_id >= stats_sect_cb->num_entries) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  arn5_set_sect_print_flag(0) ;

  arn5_stats_unit_cmn_op((cs_uint16)dev_id, ARN5_DEVICE,
                        mod_id, sect_id, unit_id,
                        stats_sect_cb, ARN5_STATS_GET, CS_RX_AND_TX,
                        FALSE) ;

  p_unit_data = stats_sect_cb->pStats + unit_id ;
  CS_MEMCPY(pUnit_Stats, p_unit_data, sizeof(arn5_stats_data_t)) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE DEVICE BLOCK STATS                    */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_ctl_blk_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                                cs_ctl_t ctl)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the stats for the Device block.                       */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if ( stats_blk_cb == NULL ) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
                  "%s not registered for Statistics\n",
                  ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb->enabled = (ctl == CS_ENABLE) ? TRUE : FALSE ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE DEVICE SECTION STATS                  */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_ctl_sect_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_ctl_t ctl)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the Stats for the section in user specified block.    */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
                  "%s not registered for Statistics\n",
                  ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  stats_sect_cb->enabled = (ctl == CS_ENABLE) ? TRUE : FALSE ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE DEVICE STATS UNIT                     */
/* CATEGORY   : Device                                                  */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_dev_ctl_unit_stats(cs_uint8 dev_id, arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id, cs_ctl_t ctl)
/* INPUTS     : o Device Id                                             */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the Stats for the section in user specified block.    */
/* Refer to the individual header files for the section ID and unit ID  */
/* defines. Unit ID is the atmoc unit of stats, it consist of the RX    */
/* and TX side stats.                                                   */
/*                                                                      */
/* The [mod_id] parameter is specified as  ARN5_ID_HOST_IF only         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t  * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t   * stats_blk_cb ;

  if (ARN5_IS_DEVICE_VALID(dev_id, &error) != TRUE) {
    CS_HNDL_ERROR(dev_id, error, "\n", dev_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_RANGE_ERR, NULL) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb((cs_uint16)dev_id, ARN5_DEVICE,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
                  "%s not registered for Statistics\n",
                  ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (unit_id >= stats_sect_cb->num_entries) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_ID_OUT_OF_RANGE,
           "Unit Stat specified out of Range (%d)\n", unit_id) ;
    return (CS_ERROR) ;
  }

  *(stats_sect_cb->enbl_tbl + unit_id) = ((ctl == CS_ENABLE) ?
                                          TRUE : FALSE) ;
  return (CS_OK) ;
}


