/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
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



/***********************************************************************
 ***********************************************************************
 *********************    PORT   API's        **************************
 ***********************************************************************
 ***********************************************************************/

/************************************************************************/
/* $rtn_hdr_start  PRINT PORT STATISTICS                                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_print_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints statistics for the port.                                      */
/* It prints the Statistics of all the blocks that are registered and   */
/* are active in the current mode.                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     error = 0 ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  CS_PRINT("\n") ;
  if ( arn5_drvr_is_esc_code_enbld() ) {
    CS_PRINT("\033[7m Port-%d  STATISTICS :\033[m\n",
             port_id) ;
  }
  else {
    CS_PRINT("Port-0x%x  STATISTICS :\n", port_id) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT, ARN5_ID_HOST_IF) ) {
    arn5_port_print_blk_stats(port_id, ARN5_ID_HOST_IF) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT, ARN5_ID_ETH) ) {
    arn5_port_print_blk_stats(port_id, ARN5_ID_ETH) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT, ARN5_ID_FRAMER) ) {
    arn5_port_print_blk_stats(port_id, ARN5_ID_FRAMER) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT, ARN5_ID_LIF) ) {
    arn5_port_print_blk_stats(port_id, ARN5_ID_LIF) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT, ARN5_ID_PPRBS)) {
    arn5_port_print_blk_stats(port_id, ARN5_ID_PPRBS) ;
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  PRINT PORT BLOCK STATS                               */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_print_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the block's statistics.                                       */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0, ii ;
  arn5_stats_blk_cb_t   * blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if ( blk_cb == NULL ) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
                   "%s not registered for Statistics\n",
                   ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if (blk_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s\n", blk_cb->blk_name) ;
    return (CS_ERROR) ;
  }

  arn5_stats_print_blk_hdr(port_id, ARN5_PORT,
                          blk_cb->blk_name) ;

  for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    /* Print Section stats if valid and are enabled */
    if ((blk_cb->p_sect_cb[ii]) &&
        (blk_cb->p_sect_cb[ii]->enabled)) {
      arn5_port_print_sect_stats(port_id, mod_id, ii) ;
    }
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  PRINT SECTION STATS                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_print_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the Stats for the user specified section of the port.         */
/* Stats within a block are organised as a collection of section stats. */
/* Each section is identified by mod_id and section_id combo.           */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32               error = 0 ;
  arn5_stats_sect_cb_t     * sect_cb = NULL ;
  arn5_stats_blk_cb_t      * blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if (blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((sect_cb = blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  if (sect_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s section %s\n",
             blk_cb->blk_name, sect_cb->sect_name) ;
    return (CS_ERROR) ;
  }

  arn5_stats_print_sect_hdr(port_id, ARN5_PORT,
                           sect_cb->sect_name) ;

  arn5_stats_sect_cmn_op(port_id, ARN5_PORT, mod_id, sect_id,
                        sect_cb, ARN5_STATS_PRINT, CS_RX_AND_TX) ;

 /* newlly added to clear stats */
  arn5_stats_sect_cmn_op(port_id, ARN5_PORT, mod_id, sect_id,
  						sect_cb, ARN5_STATS_CLEAR, CS_RX_AND_TX);


  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  PRINT UNIT STATS                                     */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_print_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the Stats for the user specified unit for the port.           */
/* Statistics for the port are organised as a collection of block       */
/* stats. Each block has number of section stats identified by section  */
/* id. Each section then has unit stats. Unit stat is the atomic entity */
/* of the stats and carry RX and TX stats. Each unit is uniquely        */
/* identified by block_id, section_id and unit_id combo.                */
/* Refer to the individual header files for the section ID and unit_id  */
/* defines.                                                             */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t   * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;
  arn5_stats_data_t      * p_unit_data ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
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

  return arn5_stats_unit_cmn_op(port_id, ARN5_PORT,
                               mod_id, sect_id, unit_id,
                               stats_sect_cb, ARN5_STATS_PRINT,
                               CS_RX_AND_TX,
                               FALSE) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR PORT STATS                                     */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clear the Port stats. It clears the hardware registers and also the  */
/* the internal stats structure which accumulate the stats for both     */
/* RX and TX direction for the whole port.                              */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_port_clear_dir_stats(port_id, CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR PORT STATS FOR THE DIRECTION (RX/TX/BOTH)      */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_dir_stats(cs_uint16 port_id, cs_dir_t dir)
/* INPUTS     : o Port Id                                               */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clear the Port RX/TX or Both stats. It clears the hardware registers */
/* and also the the internal stats structure which accumulate the stats.*/
/* It clears the stats and hw register for the direction specified.     */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32    error = 0 ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_HOST_IF)) {
    arn5_port_clear_blk_dir_stats(port_id, ARN5_ID_HOST_IF, dir) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_ETH)) {
    arn5_port_clear_blk_dir_stats(port_id, ARN5_ID_ETH, dir) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_FRAMER)) {
    arn5_port_clear_blk_dir_stats(port_id, ARN5_ID_FRAMER, dir) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_LIF)) {
    arn5_port_clear_blk_dir_stats(port_id, ARN5_ID_LIF, dir) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_PPRBS)) {
    arn5_port_clear_blk_dir_stats(port_id, ARN5_ID_PPRBS, dir) ;
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR PORT BLOCK STATS                               */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id)
/* INPUTS     : o Port Id                                               */
/*              o Module Id                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the stats for the block.                                      */
/* This API clears both the hardware counters and internal stats block  */
/* stats structure.                                                     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_port_clear_blk_dir_stats(port_id, mod_id, CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR BLOCK STATS FOR DIRECTION (RX/TX or Both)      */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_blk_dir_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_dir_t dir)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                        */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the stats for the block.                                      */
/* It clears the stats and hw register for the direction specified.     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0, ii ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if ( stats_blk_cb == NULL ) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    if ((stats_blk_cb->p_sect_cb[ii]) &&
        (stats_blk_cb->p_sect_cb[ii]->enabled)) {
      arn5_port_clear_sect_dir_stats(port_id, mod_id, ii, dir) ;
    }
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR SECTION STATS                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the section in user specified block's section.  */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_port_clear_sect_dir_stats(port_id, mod_id, sect_id,
                                       CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR SECTION STATS FOR RX/TX OR BOTH                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_sect_dir_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_dir_t dir)
/* INPUTS     : o Port Id                                               */
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
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t   * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (stats_sect_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s section %s\n",
             stats_blk_cb->blk_name, stats_sect_cb->sect_name) ;
    return (CS_ERROR) ;
  }

  arn5_stats_sect_cmn_op(port_id, ARN5_PORT, mod_id,
                        sect_id, stats_sect_cb,
                        ARN5_STATS_CLEAR, dir) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR PORT STATS UNIT                                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_uint16 unit_id)
/* INPUTS     : o Port Id                                               */
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
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return arn5_port_clear_unit_dir_stats(port_id, mod_id,
                                  sect_id, unit_id, CS_RX_AND_TX) ;
}


/************************************************************************/
/* $rtn_hdr_start  CLEAR STATS UNIT FOR RX OR TX OR BOTH                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_clear_unit_dir_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id,
                        cs_uint16 unit_id, cs_dir_t dir)
/* INPUTS     : o Port Id                                               */
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
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t   * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "%s not registered for Statistics\n", ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (*(stats_sect_cb->enbl_tbl + unit_id) == FALSE) {
    CS_PRINT("STATS DISABLED FOR %s : SECT_ID : %d UNIT_ID : %d\n",
             stats_blk_cb->blk_name, sect_id, unit_id) ;
    return (CS_ERROR) ;
  }

  arn5_set_sect_print_flag(0) ;

  return arn5_stats_unit_cmn_op(port_id, ARN5_PORT,
                               mod_id, sect_id, unit_id,
                               stats_sect_cb, ARN5_STATS_CLEAR,
                               CS_RX_AND_TX,
                               FALSE) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET PORT STATS                                       */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_get_stats(cs_uint16 port_id, arn5_port_stats_t * pStats)
/* INPUTS     : o Port Id                                               */
/*              o Pointer to user allocated arn5_port_stats_t struct    */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Get the Stats for user specified Port.                               */
/* User should allocate at least as many bytes as size of               */
/* arn5_port_stats_t structure and pass the pointer to that as pStats   */
/* argument to the function.                                            */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     error = 0 ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_HOST_IF)) {
    arn5_port_get_blk_stats(port_id, ARN5_ID_HOST_IF, &pStats->host_if,
                           sizeof(arn5_host_if_port_stats_t)) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_ETH)) {
    arn5_port_get_blk_stats(port_id, ARN5_ID_ETH, &pStats->eth,
                           sizeof(arn5_eth_port_stats_t)) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_FRAMER)) {
    arn5_port_get_blk_stats(port_id, ARN5_ID_FRAMER, &pStats->framer,
                           sizeof(arn5_framer_stats_t)) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_LIF)) {
    arn5_port_get_blk_stats(port_id, ARN5_ID_LIF, &pStats->lif,
                           sizeof(arn5_lif_stats_t)) ;
  }

  if (ARN5_STATS_IS_BLK_REGISTERED(port_id, ARN5_PORT,
                                  ARN5_ID_PPRBS)) {
    arn5_port_get_blk_stats(port_id, ARN5_ID_PPRBS, &pStats->pprbs,
                           sizeof(arn5_pprbs_port_stats_t)) ;
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET PORT BLOCK STATS                                 */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_get_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        void * pStats, cs_uint16 len)
/* INPUTS     : o Port Id                                               */
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
/* found in individual blocks's header file. For example to get the     */
/* stats for Framer, user should allocate alteast as many bytes as the  */
/* size of the arn5_framer_stats_t structure. The len field specifies   */
/* the length of the user allocated bytes.                              */
/* Refer to the individual header files for definition of the block     */
/* stats structures definitions.                                        */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/*                                                                      */
/* The [len] parameter is the length of the user allocated memory where */
/* block stats will be copied.                                          */
/*                                                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0, ii ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;
  cs_uint16             blk_stats_len ;
  void                  * p_blk_stats ;
  arn5_port_stats_t      * p_port_stats ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "%s not registered for Statistics\n", stats_blk_cb->blk_name) ;
    return (CS_ERROR) ;
  }

  p_port_stats = ARN5_PORT_GET_STATS_PTR(port_id) ;

  switch (mod_id) {
    case ARN5_ID_HOST_IF  :
      p_blk_stats = (void*) &p_port_stats->host_if ;
      blk_stats_len = sizeof(arn5_host_if_port_stats_t) ;
      break ;
    case ARN5_ID_ETH    :
      p_blk_stats = (void*) &p_port_stats->eth ;
      blk_stats_len = sizeof(arn5_eth_port_stats_t) ;
      break ;
    case ARN5_ID_FRAMER:
      p_blk_stats = (void*) &p_port_stats->framer;
      blk_stats_len = sizeof(arn5_framer_stats_t) ;
      break ;
    case ARN5_ID_LIF  :
      p_blk_stats = (void*) &p_port_stats->lif;
      blk_stats_len = sizeof(arn5_lif_stats_t) ;
      break ;
    case ARN5_ID_PPRBS  :
      p_blk_stats = (void*) &p_port_stats->pprbs;
      blk_stats_len = sizeof(arn5_pprbs_port_stats_t) ;
      break ;
    case ARN5_ID_MPIF:
    case ARN5_ID_ALL_BLOCKS:
    default :
      return (CS_ERROR) ;
  }

  if (len < blk_stats_len) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_SIZE,
                  "len s/b %d\n", blk_stats_len) ;
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    /* Print Section stats if valid and are enabled */
    if ((stats_blk_cb->p_sect_cb[ii]) &&
        (stats_blk_cb->p_sect_cb[ii]->enabled)) {
      arn5_stats_sect_cmn_op(port_id, ARN5_PORT,
                            mod_id, ii,
                            stats_blk_cb->p_sect_cb[ii],
                            ARN5_STATS_GET, CS_RX_AND_TX) ;
    }
  }

  CS_MEMCPY(pStats, p_blk_stats, len) ;

    for (ii = 0; ii < ARN5_STATS_MAX_SECT; ii++) {
    /* Print Section stats if valid and are enabled */
    if ((stats_blk_cb->p_sect_cb[ii]) &&
        (stats_blk_cb->p_sect_cb[ii]->enabled)) {
      arn5_stats_sect_cmn_op(port_id, ARN5_PORT,
                            mod_id, ii,
                            stats_blk_cb->p_sect_cb[ii],
                            ARN5_STATS_CLEAR, CS_RX_AND_TX) ;
    }
  }

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET SECTION STATS                                    */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_get_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id,
                        void * pStats, cs_uint16 len)
/* INPUTS     : o Port Id                                               */
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
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/*                                                                      */
/* The [len] parameter is the length of the user allocated memory where */
/* section stats will be copied.                                        */
/*                                                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t   * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;
  arn5_stats_data_t      * p_unit_data ;
  cs_uint16             sect_stats_len ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
                   "%s not registered for Statistics\n",
                   ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  sect_stats_len = (sizeof(arn5_stats_data_t) * stats_sect_cb->num_entries) ;

  if ( len < sect_stats_len ) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_SIZE,
                  "len s/b %d\n", sect_stats_len) ;
    return (CS_ERROR) ;
  }

  p_unit_data = stats_sect_cb->pStats ;
  arn5_stats_sect_cmn_op(port_id, ARN5_PORT,
                        mod_id, sect_id,
                        stats_sect_cb, ARN5_STATS_GET, CS_RX_AND_TX) ;

  CS_MEMCPY(pStats, p_unit_data, len) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  GET PORT UNIT STATS                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_get_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id,
                        cs_uint16 unit_id, arn5_stats_data_t * pUnit_Stats)
/* INPUTS     : o Port Id                                               */
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
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t   * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;
  arn5_stats_data_t      * p_unit_data ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (pUnit_Stats == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT,
                                      mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
                   "%s not registered for Statistics\n",
                   ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  if (unit_id >= stats_sect_cb->num_entries) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  arn5_set_sect_print_flag(0) ;

  arn5_stats_unit_cmn_op(port_id, ARN5_PORT,
                        mod_id, sect_id, unit_id,
                        stats_sect_cb, ARN5_STATS_GET, CS_RX_AND_TX,
                        FALSE) ;

  p_unit_data = stats_sect_cb->pStats + unit_id ;
  CS_MEMCPY(pUnit_Stats, p_unit_data, sizeof(arn5_stats_data_t)) ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PORT BLOCK STATS                      */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_ctl_blk_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the stats for the block.                              */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT,
                                      mod_id) ;
  if ( stats_blk_cb == NULL ) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
                   "%s not registered for Statistics\n",
                   ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb->enabled = (ctl == CS_ENABLE) ? TRUE : FALSE ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PORT SECTION STATS                    */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_ctl_sect_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint16 sect_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the Stats for the section in user specified block.    */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t   * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if (stats_blk_cb == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
                   "%s not registered for Statistics\n",
                   ARN5_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  stats_sect_cb->enabled = (ctl == CS_ENABLE) ? TRUE : FALSE ;
  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE STATS UNIT                            */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Arsenal5                                                */
cs_status arn5_port_ctl_unit_stats(cs_uint16 port_id,
                        arn5_module_id_t mod_id, cs_uint16 sect_id,
                        cs_uint16 unit_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
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
/* The [mod_id] parameter is specified as one of the following:         */
/* ARN5_ID_HOST_IF   or   ARN5_ID_ETH   or   ARN5_ID_FRAMER   or        */
/* ARN5_ID_LIF       or   ARN5_ID_PPRBS                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32             error = 0 ;
  arn5_stats_sect_cb_t   * stats_sect_cb = NULL ;
  arn5_stats_blk_cb_t    * stats_blk_cb ;

  if (ARN5_IS_PORT_VALID(port_id, &error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, NULL) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= ARN5_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EARN5_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = arn5_stats_get_blk_cb(port_id, ARN5_PORT, mod_id) ;
  if (stats_blk_cb == NULL) {
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
    CS_HNDL_ERROR( port_id, EARN5_PORT_ID_OUT_OF_RANGE,
           "Unit Stat specified out of Range (%d)\n", unit_id) ;
    return (CS_ERROR) ;
  }

  *(stats_sect_cb->enbl_tbl + unit_id) = ((ctl == CS_ENABLE) ?
                                          TRUE : FALSE) ;
  return (CS_OK) ;
}

