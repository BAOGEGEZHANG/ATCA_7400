/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_stats.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"
#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_stats.h"


/************************************************************************
 * External function declaration
 ***********************************************************************/
extern cs_boolean m2r_drvr_is_stats_in_hex_fmt(void) ;
extern cs_boolean m2r_drvr_is_mask_zero_cntr(void) ;

/************************************************************************
 * Internal function declaration
 ***********************************************************************/
static
cs_boolean m2r_stats_is_spl_hndlr_valid(const m2r_stats_info_t * p_unit_info,
                                        m2r_stats_op_type_t op) ;
static
cs_status m2r_stats_spl_handler(cs_uint16 port_id,
                                m2r_module_id_t mod_id,
                                cs_uint16 sect_id,
                                cs_uint16 unit_id,
                                m2r_stats_op_type_t op) ;
static
cs_status m2r_stats_update_unit_data(cs_uint16 port_id,
                                     cs_uint16 stride_id,
                                     const m2r_stats_info_t * p_unit_info,
                                     m2r_stats_data_t * p_unit_data,
                                     m2r_dir_t dir) ;
static
cs_status m2r_stats_clear_unit_data(cs_uint16 port_id,
                                    cs_uint16 stride_id,
                                    const m2r_stats_info_t * p_unit_info,
                                    m2r_stats_data_t * p_unit_data,
                                    m2r_dir_t dir) ;
static
cs_status m2r_stats_unit_cmn_op(cs_uint16 port_id,
                                m2r_module_id_t mod_id,
                                cs_uint16 sect_id,
                                cs_uint16 unit_id,
                                m2r_stats_sect_cb_t * sect_cb,
                                m2r_stats_op_type_t op,
                                m2r_dir_t dir) ;
static
cs_status m2r_stats_sect_cmn_op(cs_uint16 port_id,
                                m2r_module_id_t mod_id,
                                cs_uint16 sect_id,
                                m2r_stats_sect_cb_t * stats_sect_cb,
                                m2r_stats_op_type_t op,
                                m2r_dir_t dir) ;

static void m2r_stats_print_blk_hdr(cs_uint16 port_id, cs_int8 * blk_name) ;

static void m2r_stats_print_sect_hdr(cs_uint16 port_id, cs_int8 * name) ;

static void m2r_stats_print_unit_cntr(const m2r_stats_info_t * p_info,
                                      m2r_stats_data_t * p_data) ;
static m2r_stats_blk_cb_t * m2r_stats_get_blk_cb(cs_uint16 port_id,
                                                 m2r_module_id_t mod_id) ;

static cs_status m2r_stats_set_blk_cb(cs_uint16 port_id,
                                      m2r_module_id_t mod_id,
                                      m2r_stats_blk_cb_t * blk_cb) ;

/************************************************************************
 * Macro definitions
 ***********************************************************************/
#define M2R_STATS_IS_BLK_REGISTERED(PORTID, MODID)      \
           ((m2r_stats_get_blk_cb((PORTID), (MODID))) ? TRUE : FALSE)

/************************************************************************
 * Static variable
 ***********************************************************************/
/* Adam on 10/28/03 -> use to distinguish the context of getting stats */
static int _m2r_sect_print_flag = 0;

/************************************************************************/
/* $rtn_hdr_start  PRINT PORT STATISTICS                                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_print_stats(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the Port Statistics.                                          */
/* It prints the Statistics of all the blocks that are registered and   */
/* are active in the current mode of MILAN.                             */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  CS_PRINT("\n") ;
  if ( m2r_drvr_is_esc_code_enbld() ) {
    CS_PRINT("\033[7m Port-%d  STATISTICS :\033[m\n",
             port_id) ;
  }          
  else {     
    CS_PRINT(       " Port-%d  STATISTICS :\n",
                    port_id) ; 
  }                 
  
  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_PPRBS)) {
    m2r_print_blk_stats(port_id, M2R_ID_PPRBS) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_FRAMER)) {
    m2r_print_blk_stats(port_id, M2R_ID_FRAMER) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_RMAC)) {
    m2r_print_blk_stats(port_id, M2R_ID_RMAC) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_LIF)) {
    m2r_print_blk_stats(port_id, M2R_ID_LIF) ;
  }
  
  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_SPI42)) {
    m2r_print_blk_stats(port_id, M2R_ID_SPI42) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_ETH)) {
    m2r_print_blk_stats(port_id, M2R_ID_ETH) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_XGXS)) {
    m2r_print_blk_stats(port_id, M2R_ID_XGXS) ;
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  PRINT BLOCK STATS                                    */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_print_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the block's statistics.                                       */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0, ii ;
  m2r_stats_blk_cb_t      * blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  blk_cb = m2r_stats_get_blk_cb(port_id, mod_id) ;
  if ( blk_cb == NULL ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if (blk_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s\n", blk_cb->blk_name) ;
    return (CS_ERROR) ;
  }

  m2r_stats_print_blk_hdr(port_id, blk_cb->blk_name) ;
  
  for (ii = 0; ii < M2R_STATS_MAX_SECT; ii++) {
    /* Print Section stats if valid and are enabled */
    if ((blk_cb->p_sect_cb[ii]) &&
        (blk_cb->p_sect_cb[ii]->enabled)) {
      m2r_print_sect_stats(port_id, mod_id, ii) ;
    }
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  PRINT SECTION STATS                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_print_sect_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                               cs_uint16 sect_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the Stats for the user specified section.                     */
/* Stats within a block are organised as a collection of section stats. */
/* Each section is identified by mod_id and section_id combo.           */
/* Each block has number of sections for example Framer block has       */
/* SONET Section Stats section, SONET Line stats section.               */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * sect_cb = NULL ;
  m2r_stats_blk_cb_t      * blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((sect_cb = blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  if (sect_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s section %s\n",
             blk_cb->blk_name, sect_cb->sect_name) ;
    return (CS_ERROR) ;
  }


  m2r_stats_print_sect_hdr(port_id, sect_cb->sect_name) ;
  
  m2r_stats_sect_cmn_op(port_id, mod_id, sect_id,
                        sect_cb, M2R_STATS_PRINT, M2R_RX_AND_TX) ;
  
  /*
  m2r_stats_sect_cmn_op(port_id, mod_id, sect_id, sect_cb, M2R_STATS_PRINT, M2R_RX_AND_TX);
  m2r_stats_sect_cmn_op(port_id, mod_id, sect_id, sect_cb, M2R_STATS_CLEAR,M2R_RX_AND_TX);
  */
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  PRINT UNIT STATS                                     */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_print_unit_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                               cs_uint16 sect_id, cs_uint16 unit_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the Stats for the user specified unit.                        */
/* Statistics for the port are organised as a collection of block       */
/* stats. Each block has number of section stats identified by section  */
/* id. Each section then has unit stats. Unit stat is the atomic entity */
/* of the stats and carry RX and TX stats. Each unit is uniquely        */
/* identified by block_id, section_id and unit_id combo.                */
/* Refer to the individual header files for the section ID and unit_id  */
/* defines.                                                             */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;
  m2r_stats_data_t        * p_unit_data ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
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

  /* Adam on 10/28/03 -> */
  _m2r_sect_print_flag = 0;
  
  return m2r_stats_unit_cmn_op(port_id, mod_id, sect_id, unit_id,
                        stats_sect_cb, M2R_STATS_PRINT, M2R_RX_AND_TX) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR PORT STATS                                     */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_stats(cs_uint16 port_id)
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
  return m2r_clear_dir_stats(port_id, M2R_RX_AND_TX) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR PORT STATS FOR THE DIRECTION ( RX/TX/BOTH)     */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_dir_stats(cs_uint16 port_id, m2r_dir_t dir)
/* INPUTS     : o Port Id                                               */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clear the Port RX/TX or Both stats. It clears the hardware registers */
/* and also the the internal stats structure which accumulate the stats.*/
/* It clears the stats and hw register for the direction specified.     */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_PPRBS)) {
    m2r_clear_blk_dir_stats(port_id, M2R_ID_PPRBS, dir) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_FRAMER)) {
    m2r_clear_blk_dir_stats(port_id, M2R_ID_FRAMER, dir) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_RMAC)) {
    m2r_clear_blk_dir_stats(port_id, M2R_ID_RMAC, dir) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_LIF)) {
    m2r_clear_blk_dir_stats(port_id, M2R_ID_LIF, dir) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_SPI42)) {
    m2r_clear_blk_dir_stats(port_id, M2R_ID_SPI42, dir) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_ETH)) {
    m2r_clear_blk_dir_stats(port_id, M2R_ID_ETH, dir) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_XGXS)) {
    m2r_clear_blk_dir_stats(port_id, M2R_ID_XGXS, dir) ;
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR BLOCK STATS                                    */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the stats for the block.                                      */
/* This API clears both the hardware counters and internal stats block  */
/* stats structure.                                                     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return m2r_clear_blk_dir_stats(port_id, mod_id, M2R_RX_AND_TX) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR BLOCK STATS FOR DIRECTION (RX/TX or BOTH)      */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_blk_dir_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                  m2r_dir_t dir)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the stats for the block.                                      */
/* It clears the stats and hw register for the direction specified.     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0, ii ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id) ;
  if ( stats_blk_cb == NULL ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < M2R_STATS_MAX_SECT; ii++) {
    if ((stats_blk_cb->p_sect_cb[ii]) &&
        (stats_blk_cb->p_sect_cb[ii]->enabled)) {
      m2r_clear_sect_dir_stats(port_id, mod_id, ii, dir) ;
    }
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR SECTION STATS                                  */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_sect_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                               cs_uint16 sect_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the section in user specified block's section.  */
/* Refer to the individual header files for the section ID defines.     */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return m2r_clear_sect_dir_stats(port_id, mod_id, sect_id, M2R_RX_AND_TX) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR SECTION STATS FOR RX/TX OR BOTH                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_sect_dir_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                               cs_uint16 sect_id, m2r_dir_t dir)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the section in user specified block's section.  */
/* It clears the stats and hw register for the direction specified.     */
/* Refer to the individual header files for the section ID defines.     */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (stats_sect_cb->enabled == FALSE) {
    CS_PRINT("STATISTICS DISABLED FOR %s section %s\n",
             stats_blk_cb->blk_name, stats_sect_cb->sect_name) ;
    return (CS_ERROR) ;
  }

  m2r_stats_sect_cmn_op(port_id, mod_id, sect_id,
                        stats_sect_cb, M2R_STATS_CLEAR, dir) ;

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR STATS UNIT                                     */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_unit_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
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
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  return m2r_clear_unit_dir_stats(port_id, mod_id,
                                  sect_id, unit_id, M2R_RX_AND_TX) ;
}

/************************************************************************/
/* $rtn_hdr_start  CLEAR STATS UNIT FOR RX OR TX OR BOTH                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_clear_unit_dir_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                                   cs_uint16 sect_id, cs_uint16 unit_id,
                                   m2r_dir_t dir)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Clears the Stats for the unit in user specified block's section.     */
/* It clears the stats and hw register for the direction specified.     */
/* Refer to the individual header files for the section ID and unit Id  */
/* defines.                                                             */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (*(stats_sect_cb->enbl_tbl + unit_id) == FALSE) {
    CS_PRINT("STATS DISABLED FOR %s : SECT_ID : %d UNIT_ID : %d\n",
             stats_blk_cb->blk_name, sect_id, unit_id) ;
    return (CS_ERROR) ;
  }

  /* Adam on 10/28/03 -> */
  _m2r_sect_print_flag = 0;
  
  return m2r_stats_unit_cmn_op(port_id, mod_id, sect_id, unit_id,
                        stats_sect_cb, M2R_STATS_CLEAR, M2R_RX_AND_TX) ;

}

/************************************************************************/
/* $rtn_hdr_start  GET PORT STATS                                       */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_get_stats(cs_uint16 port_id, m2r_port_stats_t * pStats)
/* INPUTS     : o Port Id                                               */
/*              o Pointer to user allocated m2r_port_stats_t struct     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Get the Stats for user specified Port.                               */
/* User should allocate at least as many bytes as size of               */
/* m2r_port_stats_t structure and pass the pointer to that as pStats    */
/* argument to the function.                                            */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_PPRBS)) {
    m2r_get_blk_stats(port_id, M2R_ID_PPRBS, &pStats->pprbs,
                      sizeof(m2r_pprbs_stats_t)) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_FRAMER)) {
    m2r_get_blk_stats(port_id, M2R_ID_FRAMER, &pStats->framer,
                      sizeof(m2r_framer_stats_t)) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_RMAC)) {
    m2r_get_blk_stats(port_id, M2R_ID_RMAC, &pStats->rmac,
                      sizeof(m2r_rmac_stats_t)) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_LIF)) {
    m2r_get_blk_stats(port_id, M2R_ID_LIF, &pStats->lif,
                      sizeof(m2r_lif_stats_t)) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_SPI42)) {
    m2r_get_blk_stats(port_id, M2R_ID_SPI42, &pStats->spi42,
                      sizeof(m2r_spi42_stats_t)) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_ETH)) {
    m2r_get_blk_stats(port_id, M2R_ID_ETH, &pStats->eth,
                      sizeof(m2r_eth_stats_t)) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_XGXS)) {
    m2r_get_blk_stats(port_id, M2R_ID_XGXS, &pStats->xgxs,
                      sizeof(m2r_xgxs_stats_t)) ;
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  GET BLOCK STATS                                      */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_get_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
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
/* size of the m2r_framer_stats_t structure. The len field specifies    */
/* the length of the user allocated bytes.                              */
/* Refer to the individual header files for definition of the block     */
/* stats structures definitions.                                        */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/*                                                                      */
/* The [len] parameter is the length of the user allocated memory where */
/* block stats will be copied.                                          */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0, ii ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;
  cs_uint16                 blk_stats_len ;
  void                    * p_blk_stats ;
  m2r_port_stats_t        * p_port_stats ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", stats_blk_cb->blk_name) ;
    return (CS_ERROR) ;
  }

  p_port_stats = M2R_GET_STATS_PTR(port_id) ;

  switch (mod_id) {
    case M2R_ID_SPI42  :
      p_blk_stats = (void*) &p_port_stats->spi42 ;
      blk_stats_len = sizeof(m2r_spi42_stats_t) ;
      break ;
    case M2R_ID_PPRBS  :
      p_blk_stats = (void*) &p_port_stats->pprbs;
      blk_stats_len = sizeof(m2r_pprbs_stats_t) ;
      break ;
    case M2R_ID_FRAMER :
      p_blk_stats = (void*) &p_port_stats->framer;
      blk_stats_len = sizeof(m2r_framer_stats_t) ;
      break ;
    case M2R_ID_RMAC   :
      p_blk_stats = (void*) &p_port_stats->rmac;
      blk_stats_len = sizeof(m2r_rmac_stats_t) ;
      break ;
    case M2R_ID_LIF    :
      p_blk_stats = (void*) &p_port_stats->lif;
      blk_stats_len = sizeof(m2r_lif_stats_t) ;
      break ;
    case M2R_ID_ETH   :
      p_blk_stats = (void*) &p_port_stats->eth;
      blk_stats_len = sizeof(m2r_eth_stats_t) ;
      break ;
    case M2R_ID_XGXS   :
      p_blk_stats = (void*) &p_port_stats->xgxs;
      blk_stats_len = sizeof(m2r_xgxs_stats_t) ;
      break ;
    case M2R_ID_MPIF:
    case M2R_ID_ALL_BLOCKS:
    default :
      return (CS_ERROR) ;
  }

  if (len < blk_stats_len) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_SIZE,
                  "len s/b %d\n", blk_stats_len) ;
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < M2R_STATS_MAX_SECT; ii++) {
    /* Print Section stats if valid and are enabled */
    if ((stats_blk_cb->p_sect_cb[ii]) &&
        (stats_blk_cb->p_sect_cb[ii]->enabled)) {
      m2r_stats_sect_cmn_op(port_id, mod_id, ii,
                            stats_blk_cb->p_sect_cb[ii],
                            M2R_STATS_GET, M2R_RX_AND_TX) ;
    }
  }

  CS_MEMCPY(pStats, p_blk_stats, len) ;

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  GET SECTION STATS                                    */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_get_sect_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                             cs_uint16 sect_id, void * pStats, cs_uint16 len)
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
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/*                                                                      */
/* The [len] parameter is the length of the user allocated memory where */
/* section stats will be copied.                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;
  m2r_stats_data_t        * p_unit_data ;
  cs_uint16                 sect_stats_len ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  sect_stats_len = (sizeof(m2r_stats_data_t) * stats_sect_cb->num_entries) ;

  if ( len < sect_stats_len ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_SIZE,
                  "len s/b %d\n", sect_stats_len) ;
    return (CS_ERROR) ;
  }

  p_unit_data = stats_sect_cb->pStats ;
  m2r_stats_sect_cmn_op(port_id, mod_id, sect_id,
                        stats_sect_cb, M2R_STATS_GET, M2R_RX_AND_TX) ;

  CS_MEMCPY(pStats, p_unit_data, len) ;

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  GET UNIT STATS                                       */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_get_unit_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                             cs_uint16 sect_id, cs_uint16 unit_id,
                             m2r_stats_data_t * pUnit_Stats)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/*              o Pointer to user allocated m2r_stats_data_t structure  */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Get unit stats for the user specified unit with specified block's    */
/* section. unit stats carry TX and RX stats and for the given stats    */
/* Both RX and RX or onlt RX or only TX might be valid. In case only    */
/* one direction stats are valid, the other counter will be Zero.       */
/* Refer to the individual header files for the section ID and unit id  */
/* defines.                                                             */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;
  m2r_stats_data_t        * p_unit_data ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (pUnit_Stats == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  if (unit_id >= stats_sect_cb->num_entries) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  /* Adam on 10/28/03 -> */
  _m2r_sect_print_flag = 0;
  
  m2r_stats_unit_cmn_op(port_id, mod_id, sect_id, unit_id,
                        stats_sect_cb, M2R_STATS_GET, M2R_RX_AND_TX) ;

  p_unit_data = stats_sect_cb->pStats + unit_id ;
  CS_MEMCPY(pUnit_Stats, p_unit_data, sizeof(m2r_stats_data_t)) ;

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE PORT STATISTICS                       */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_ctl_stats(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)                       */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the Port Statistics.                                  */
/* By Default Stats is enabled for Port after port config.              */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_PPRBS)) {
    m2r_ctl_blk_stats(port_id, M2R_ID_PPRBS, ctl) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_FRAMER)) {
    m2r_ctl_blk_stats(port_id, M2R_ID_FRAMER, ctl) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_RMAC)) {
    m2r_ctl_blk_stats(port_id, M2R_ID_RMAC, ctl) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_LIF)) {
    m2r_ctl_blk_stats(port_id, M2R_ID_LIF, ctl) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_SPI42)) {
    m2r_ctl_blk_stats(port_id, M2R_ID_SPI42, ctl) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_ETH)) {
    m2r_ctl_blk_stats(port_id, M2R_ID_ETH, ctl) ;
  }

  if (M2R_STATS_IS_BLK_REGISTERED(port_id, M2R_ID_XGXS)) {
    m2r_ctl_blk_stats(port_id, M2R_ID_XGXS, ctl) ;
  }

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE BLOCK STATS                           */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_ctl_blk_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                            m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)                       */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the stats for the block.                              */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id) ;
  if ( stats_blk_cb == NULL ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }
  stats_blk_cb->enabled = (ctl == M2R_ENABLE) ? TRUE : FALSE ;

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE SECTION STATS                         */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_ctl_sect_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                               cs_uint16 sect_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)                       */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the Stats for the section in user specified block.    */
/* Refer to the individual header files for the section ID defines.     */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics (%d)\n", sect_id) ;
    return (CS_ERROR) ;
  }

  stats_sect_cb->enabled = (ctl == M2R_ENABLE) ? TRUE : FALSE ;
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE STATS UNIT                            */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_ctl_unit_stats(cs_uint16 port_id, m2r_module_id_t mod_id,
                      cs_uint16 sect_id, cs_uint16 unit_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/*              o Unit ID                                               */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)                       */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the Stats for the section in user specified block.    */
/* Refer to the individual header files for the section ID and unit ID  */
/* defines. Unit ID is the atmoc unit of stats, it consist of the RX    */
/* and TX side stats.                                                   */
/*                                                                      */
/* The [mod_id] parameter is specified as one of the following:         */
/* FOR MILAN2 :                                                         */
/* M2R_ID_SPI42  or M2R_ID_PPRBS  or M2R_ID_FRAMER or                   */
/* M2R_ID_RMAC   or M2R_ID_XGXS   or M2R_ID_LIF    or M2R_ID_ETH        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (unit_id >= stats_sect_cb->num_entries) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_ID_OUT_OF_RANGE,
           "Unit Stat specified out of Range (%d)\n", unit_id) ;
    return (CS_ERROR) ;
  }

  *(stats_sect_cb->enbl_tbl + unit_id) = (ctl == M2R_ENABLE) ? TRUE : FALSE ;

  return (CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start  LOAD PORT STATS                                      */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PUBLIC                                                  */
/* BLOCK      : General                                                 */
/* CHIP       : Milan2                                                  */
cs_status m2r_load_stats(cs_uint16 port_id, m2r_port_stats_t * pStats)
/* INPUTS     : o Port Id                                               */
/*              o Pointer to user allocated m2r_port_stats_t struct     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Load the user provided statistics counters into the driver.          */
/* Normally, after cold initialization the driver statistic counters are*/
/* cleared.                                                             */
/* This API is provided so that the software has the flexibility in     */
/* order to have the driver maintain counters from a non-zero starting  */
/* point - for eg., in warm start, the application could save the       */
/* driver statistics, do warm reboot and call this API to reload it.    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_port_stats_t          * p_drvrStats = NULL ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (pStats == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_NULL_PTR, NULL ) ;
    return (CS_ERROR) ;
  }

  p_drvrStats = M2R_GET_STATS_PTR(port_id) ;
  CS_MEMCPY( p_drvrStats, pStats, sizeof(m2r_port_stats_t) ) ;
  return (CS_OK) ;
}


/************************************************************************
 * PRIVATE APIs definitions
 ************************************************************************/
/************************************************************************/
/*                    REGISTER STATS BLOCK CB                           */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
m2r_stats_blk_cb_t * m2r_stats_register_blk_cb(cs_uint16 port_id,
                                           m2r_module_id_t mod_id,
                                           cs_boolean ctl_enabled,
                                           m2r_stats_get_func pGet_func,
                                           m2r_stats_print_func pPrint_func,
                                           m2r_stats_clear_func pClear_func)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o TRUE ( Stats Enabled ) or FALSE                       */
/*              o Pointer to special handler for GET                    */
/*              o Pointer to special handler for PRINT                  */
/*              o Pointer to special handler for CLEAR                  */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Register the Block CB. This is an internal API to be invoked by      */
/* individual modules ( Framer, LIF , etc ) to register thier block CB. */
/* This API will do tha allocation of memory for the block CB and       */
/* if sanity checks pass, returns the pointer to m2r_stats_blk_cb_t     */
/* back to the called. The individual blocks can the go on adding thier */
/* sections to the block_cb.                                            */
/*                                                                      */
/* the [mod_id] parameter can be logical mask of the following:         */
/* for MILAN2 :                                                         */
/* M2R_ID_SPI42  OR M2R_ID_PPRBS  OR M2R_ID_FRAMER OR M2R_ID_ETH OR     */
/* M2R_ID_RMAC   OR M2R_ID_XGXS OR M2R_ID_LIF                           */
/************************************************************************/
{
  cs_uint16            ii ;
  m2r_stats_blk_cb_t * stats_blk_cb =
                            m2r_stats_get_blk_cb(port_id, mod_id) ;

  /* If block CB is NULL, Allocate it */
  if ( stats_blk_cb == NULL) {
    /* Allocate memory for STATS block CB */
    if ((stats_blk_cb = (m2r_stats_blk_cb_t *)
                    CS_MALLOC(sizeof(m2r_stats_blk_cb_t))) == NULL) {
      CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED, NULL ) ;
      return (NULL) ;
    }
    /* Clear out the pointer table to STATS section CB */
    for(ii = 0; ii < M2R_STATS_MAX_SECT; ii++) {
      stats_blk_cb->p_sect_cb[ii] = NULL ;
    }
  }

  stats_blk_cb->blk_name = M2R_GET_MOD_NAME(mod_id) ;
  stats_blk_cb->enabled = ctl_enabled ;
  stats_blk_cb->num_sect = 0 ;
  stats_blk_cb->get_func = pGet_func ;
  stats_blk_cb->print_func = pPrint_func ;
  stats_blk_cb->clear_func = pClear_func ;
  m2r_stats_set_blk_cb(port_id, mod_id, stats_blk_cb) ;
  return (stats_blk_cb) ;
}

/************************************************************************/
/*                 UNREGISTER STATS BLOCK CB                            */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_stats_deregister_blk_cb(cs_uint16 port_id,
                                      m2r_module_id_t mod_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Unregister the Block CB. This is an internal API to be invoked       */
/* to deregister the block CB. This API shall free any memory allocated */
/* for the block CB and section CBs. This should be called ideally      */
/* everytime before the Port init to clean up any resources ( memory )  */
/* allocated from last time.                                            */
/*                                                                      */
/* the [mod_id] parameter can be logical mask of the following:         */
/* for MILAN2 :                                                         */
/* M2R_ID_SPI42  OR M2R_ID_PPRBS  OR M2R_ID_FRAMER OR M2R_ID_ETH OR     */
/* M2R_ID_RMAC   OR M2R_ID_XGXS OR M2R_ID_LIF                           */
/************************************************************************/
{
  cs_uint16            ii ;
  m2r_stats_blk_cb_t * stats_blk_cb =
                            m2r_stats_get_blk_cb(port_id, mod_id) ;

  if (stats_blk_cb == NULL) {
    return (CS_ERROR) ;
  }

  for (ii = 0; ii < M2R_STATS_MAX_SECT; ii++) {
    m2r_stats_remove_sect_tbl_entry(port_id, mod_id, ii) ;
  }

  CS_FREE(stats_blk_cb) ;
  m2r_stats_set_blk_cb(port_id, mod_id, NULL) ;

  return (CS_OK) ;
}

/************************************************************************/
/*                 CLEAN STATS BLOCK CB                                 */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_stats_clean_all(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Unregister the All Block CB. This is an internal API to be invoked   */
/* to deregister the block CB. This API shall free any memory allocated */
/* for the block CB and section CBs. This should be called ideally      */
/* everytime before the Port init to clean up any resources ( memory )  */
/* allocated from last time.                                            */
/************************************************************************/
{

  m2r_stats_deregister_blk_cb(port_id, M2R_ID_FRAMER) ;

  m2r_stats_deregister_blk_cb(port_id, M2R_ID_RMAC) ;

  m2r_stats_deregister_blk_cb(port_id, M2R_ID_LIF) ;

  m2r_stats_deregister_blk_cb(port_id, M2R_ID_PPRBS) ;

  m2r_stats_deregister_blk_cb(port_id, M2R_ID_SPI42) ;

  m2r_stats_deregister_blk_cb(port_id, M2R_ID_ETH) ;

  m2r_stats_deregister_blk_cb(port_id, M2R_ID_XGXS) ;

  return (CS_OK) ;
}

/************************************************************************/
/*                 ADD SECTION CB TO BLOCK CB                           */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_stats_add_sect_tbl_entry(cs_uint16 port_id,
                                       m2r_module_id_t mod_id,
                                       cs_int8 * sect_name,
                                       cs_uint16 sect_id,
                                       cs_uint8  stride_id,
                                       cs_boolean ctl_enabled,
                                       cs_uint16 num_child,
                                       m2r_stats_data_t * p_data,
                                       const m2r_stats_info_t * p_info_tbl)
/* INPUTS     : o Port Id                                               */
/*              o Section Name                                          */
/*              o Section ID                                            */
/*              o Number of counter blocks in this section              */
/*              o Pointer to Section stats data structure               */
/*              o Pointer to the Section Info Table                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Add Section CB to block CB. Individual modules should call this API  */
/* to register thier Section tables. Section ID identifies the section  */
/* stats.                                                               */
/* The [sect_id] is the section ID of the section stats. Æach Module    */
/* should register thier section stats by calling this API for each     */
/* section they wish to regsiter.                                       */
/* The [stride_id] identifies the stride, for example in framer it      */
/* identifies each stream.                                              */
/* The [ctl_enabled] is to enable/disable statistics gathering/printing */
/* for this section table. If [ctl_enabled] is FALSE, no statistics are */
/* accumulated for this section.                                        */
/* The [num_child] is the number of STAT unit data entries in this      */
/* section.                                                             */
/* The [p_data] is pointer to user allocated data structure where the   */
/* statistics will be accumulated.                                      */
/* The [p_info_tbl] is pointer to section info table. This table        */
/* should be an array of stats unit info entries and contain info about */
/* the register offset , flags which dictate how the registers will be  */
/* read/cleared and other info like if special handling is required to  */
/* get the stats for this unit.                                         */
/************************************************************************/
{
  cs_uint16             ii ;
  m2r_stats_sect_cb_t * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t  * stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id) ;

  if ((stats_blk_cb == NULL) || (sect_id >= M2R_STATS_MAX_SECT)) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL, NULL ) ;
    return (CS_ERROR) ;
  }

  /* Get the Pointer to Section CB entry, if its NULL,
   * Allocate a new entry.
   */
  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;
  if (stats_sect_cb == NULL) {
    if ((stats_sect_cb = (m2r_stats_sect_cb_t *)
                    CS_MALLOC(sizeof(m2r_stats_sect_cb_t))) == NULL) {
      CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED, NULL ) ;
      goto RTN_CLEANUP;
    }
    if ((stats_sect_cb->enbl_tbl = (cs_boolean *)
                CS_MALLOC(num_child * sizeof(cs_boolean))) == NULL ) {
      CS_HNDL_ERROR( port_id, EM2R_PORT_MALLOC_FAILED, NULL ) ;
      goto RTN_CLEANUP;
    }
    stats_blk_cb->p_sect_cb[sect_id] = stats_sect_cb ;
    for (ii = 0; ii < num_child; ii++) {
      *(stats_sect_cb->enbl_tbl + ii) = TRUE ; /* Enable All by default */
    }
  }

  stats_sect_cb->sect_name = sect_name ;
  stats_sect_cb->enabled = ctl_enabled ;
  stats_sect_cb->stride_id = stride_id ;
  stats_sect_cb->num_entries = num_child ;
  stats_sect_cb->pStats = p_data ;
  stats_sect_cb->info_tbl = p_info_tbl ;
  stats_blk_cb->num_sect++ ;
  return (CS_OK) ;

RTN_CLEANUP:
  if (stats_sect_cb) {
    if (stats_sect_cb->enbl_tbl) {
      CS_FREE(stats_sect_cb->enbl_tbl) ;
    }
    CS_FREE(stats_sect_cb) ;
  }
  return (CS_ERROR) ;
}

/************************************************************************/
/*                 REMOVE SECTION CB FROM BLOCK CB                      */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_stats_remove_sect_tbl_entry(cs_uint16 port_id,
                                          m2r_module_id_t mod_id,
                                          cs_uint16 sect_id)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Removes the section table from the stats block CB and frees up any   */
/* memory allocated for the section CB.                                 */
/************************************************************************/
{
  m2r_stats_sect_cb_t * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t * stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id) ;

  if ((stats_blk_cb == NULL) || (sect_id >= M2R_STATS_MAX_SECT)) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL, NULL ) ;
    return (CS_ERROR) ;
  }

  stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id] ;
  if (stats_sect_cb) {
    CS_FREE(stats_sect_cb->enbl_tbl) ;
    CS_FREE(stats_sect_cb) ;
    stats_blk_cb->p_sect_cb[sect_id] = NULL ;
    stats_blk_cb->num_sect-- ;
  }
  return (CS_OK) ;
}


/************************************************************************
 * Internal functions definitions
 ************************************************************************/

/**************************************************************************
 * Internal function to read the hardware counter
 *************************************************************************/
CS_INLINE cs_uint64 m2r_stats_get_hw_cntr(volatile cs_reg * reg_tbl,
                                          cs_uint8 num_regs)
{
  cs_uint8      i ;
  cs_uint64     cntr = 0 ;

  /* The order of read is from MSB to LSB */
  if (!reg_tbl) {
    return (0) ;
  }

  for (i = 0; i < num_regs; i++) {
    cs_uint16  reg_val = M2R_REG_READ( &reg_tbl[i] ) ;
    cntr += ( ((cs_uint64) reg_val) << (16 * ((num_regs -1) - i)) ) ;
  }
  return (cntr) ;
}

/**************************************************************************
 * Internal function to clear the hardware counter
 *************************************************************************/
CS_INLINE void  m2r_stats_clear_hw_cntr(volatile cs_reg * reg_tbl,
                              cs_uint8 num_regs, cs_boolean read_clear)
{
  cs_uint16     cntr = 0, i ;

  if (!reg_tbl) {
    return ;
  }
  /* The order of read is from MSB to LSB */
  if (read_clear) {
    for (i = 0; i < num_regs; i++) {
      cntr = M2R_REG_READ( &reg_tbl[i] ) ;
    }
  }
  else { /* write to clear */
    /* The order of write is from LSB to MSB(reverse of read order) */
    for (i = 0; i < num_regs; i++) {
      M2R_REG_WRITE( &reg_tbl[(num_regs - 1) - i], 0 ) ;
    }
  }
}

/************************************************************************
 * This function is STATS internal function. This function gets the 
 * hardware counters for the STATS unit data ( RX and TX ) counters.
 ***********************************************************************/
static
cs_status m2r_stats_update_unit_data(cs_uint16 port_id,
                                     cs_uint16 stride_id,
                                     const m2r_stats_info_t * p_unit_info,
                                     m2r_stats_data_t * p_unit_data,
                                     m2r_dir_t dir)
{
  cs_uint16             sl_num ;
  M2R_t                 * pDev ;
  cs_uint64           counter_val = 0, tmp ;
  volatile cs_reg   * regaddr = NULL ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);
  
  /* IF RX UNIT is Valid, read the RX counter and increment the RX member */
  if ((M2R_STATS_UNIT_IS_RX_VALID(p_unit_info->flags)) &&
      ((dir == M2R_RX) || (dir == M2R_RX_AND_TX))) {

    /* If this is not a slice register, then sl_num better be 0 */
    if ( !(M2R_IS_SLICE_REG(p_unit_info->rx_reg_ofst)) ) {
      sl_num = 0 ;
    }

    regaddr = (volatile cs_reg*)
               M2R_STATS_GET_RX_REG_ADDR(pDev, sl_num, stride_id, p_unit_info) ;

    counter_val = m2r_stats_get_hw_cntr(regaddr, p_unit_info->num_regs) ;

    if (p_unit_info->flags & M2R_STATS_FLAG_NO_ACCUM) {
      p_unit_data->rx = counter_val ;
    }
    else {
      if (p_unit_info->flags & M2R_STATS_FLAG_RX_READ_CLR) {
        p_unit_data->rx += counter_val ;
      }
      else {
        tmp = 0 ;
        tmp = ((~tmp) << (p_unit_info->num_regs * 16)) & p_unit_data->rx ;
        p_unit_data->rx = (counter_val | tmp) ;
      }
    }
  }

  if ((M2R_STATS_UNIT_IS_TX_VALID(p_unit_info->flags)) &&
      ((dir == M2R_TX) || (dir == M2R_RX_AND_TX))) {

    /* If this is not a slice register, then sl_num better be 0 */
    if ( !(M2R_IS_SLICE_REG(p_unit_info->tx_reg_ofst)) ) {
      sl_num = 0 ;
    }

    regaddr = (volatile cs_reg*)
               M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, stride_id, p_unit_info) ;

    counter_val = m2r_stats_get_hw_cntr(regaddr, p_unit_info->num_regs) ;

    if (p_unit_info->flags & M2R_STATS_FLAG_NO_ACCUM) {
      p_unit_data->tx = counter_val ;
    }
    else {
      if (p_unit_info->flags & M2R_STATS_FLAG_TX_READ_CLR) {
        p_unit_data->tx += counter_val ;
      }
      else {
        tmp = 0 ;
        tmp = ((~tmp) << (p_unit_info->num_regs * 16)) & p_unit_data->tx ;
        p_unit_data->tx = (counter_val | tmp) ;
      }
    }
  }
  return (CS_OK) ;
}

/************************************************************************
 * This function is STATS internal function. This function clears the
 * hardware counters for the STATS unit data ( RX and TX ) counters.
 ***********************************************************************/
static cs_status m2r_stats_clear_unit_data(cs_uint16 port_id,
                                     cs_uint16 stride_id,
                                     const m2r_stats_info_t * p_unit_info,
                                     m2r_stats_data_t * p_unit_data,
                                     m2r_dir_t dir)
{
  cs_uint16             sl_num ;
  M2R_t                 * pDev ;

  volatile cs_reg   * regaddr ;
  cs_boolean          read_clr = FALSE ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);
  
  /* IF RX UNIT is Valid, read the RX counter and increment the RX member */
  if ((M2R_STATS_UNIT_IS_RX_VALID(p_unit_info->flags)) &&
      ((dir == M2R_RX) || (dir == M2R_RX_AND_TX))) {

    /* If this is not a slice register, then sl_num better be 0 */
    if ( !(M2R_IS_SLICE_REG(p_unit_info->rx_reg_ofst)) ) {
      sl_num = 0 ;
    }

    /* Get the Address of the RX Register */
    regaddr = (volatile cs_reg*)
               M2R_STATS_GET_RX_REG_ADDR(pDev, sl_num, stride_id, p_unit_info) ;

    read_clr = (p_unit_info->flags & M2R_STATS_FLAG_RX_READ_CLR) ?
                  TRUE : FALSE ;

    m2r_stats_clear_hw_cntr(regaddr, p_unit_info->num_regs, read_clr) ;

    p_unit_data->rx = 0 ;
  }

  if (M2R_STATS_UNIT_IS_TX_VALID(p_unit_info->flags) &&
      ((dir == M2R_TX) || (dir == M2R_RX_AND_TX))) {

    /* If this is not a slice register, then sl_num better be 0 */
    if ( !(M2R_IS_SLICE_REG(p_unit_info->tx_reg_ofst)) ) {
      sl_num = 0 ;
    }

    /* Get the Address of the RX Register */
    regaddr = (volatile cs_reg*)
               M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, stride_id, p_unit_info) ;

    read_clr = (p_unit_info->flags & M2R_STATS_FLAG_TX_READ_CLR) ?
                  TRUE : FALSE ;

    m2r_stats_clear_hw_cntr(regaddr, p_unit_info->num_regs, read_clr) ;

    p_unit_data->tx = 0 ;
  }
  return (CS_OK) ;
}

/************************************************************************
 * This function performs get/print and clear operation on the stats
 * data unit.
 ***********************************************************************/
static cs_status m2r_stats_unit_cmn_op(cs_uint16 port_id,
                                       m2r_module_id_t mod_id,
                                       cs_uint16 sect_id,
                                       cs_uint16 unit_id,
                                       m2r_stats_sect_cb_t * sect_cb,
                                       m2r_stats_op_type_t op,
                                       m2r_dir_t dir)
{
  const m2r_stats_info_t    * p_unit_info ;
  m2r_stats_data_t          * p_unit_data ;

  if (unit_id >= sect_cb->num_entries) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  /* If Stats is disabled for this unit , return , dont do anything */
  if ((*(sect_cb->enbl_tbl + unit_id)) == FALSE) return (CS_ERROR) ;

  p_unit_info = sect_cb->info_tbl + unit_id ;

  /* Handle special case if special handling flags are set */
  if (m2r_stats_is_spl_hndlr_valid(p_unit_info, op)) {
    return m2r_stats_spl_handler(port_id, mod_id, sect_id,
                                 unit_id, op) ;
  }

  p_unit_data = sect_cb->pStats + unit_id ;

  switch (op) {
    case M2R_STATS_PRINT :
      /* Adam on 10/28/03 -> add check if this unit data has been updated */
      if (M2R_STATS_UNIT_IS_TIE_VALID(p_unit_info->flags)) {
        if (_m2r_sect_print_flag == 1) {
          /* Adam on 10/28/03 -> during section process, 
                   do not get unit with Last tie */
          if (M2R_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
          	/* update RX pkts & bytes first */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
            }
          	/* update TX pkts & bytes secondly */
          	if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
            }
          }
        } else {
          if (M2R_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
          	/* update RX pkts & bytes first */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
            }
          	/* update TX pkts & bytes secondly */
          	if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
            }
          } else {
          	/* update RX pkts & bytes first */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
            }
          	/* update TX pkts & bytes secondly */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
            }
          }
    	}
      } else {
        m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                   p_unit_info, p_unit_data, dir) ;
      }

      p_unit_info = sect_cb->info_tbl + unit_id ;
      p_unit_data = sect_cb->pStats + unit_id ;
      
      m2r_stats_print_unit_cntr(p_unit_info, p_unit_data) ;
    break ;

    case M2R_STATS_GET :
      /* Adam on 10/28/03 -> add check if this unit data has been updated */
      if (M2R_STATS_UNIT_IS_TIE_VALID(p_unit_info->flags)) {
        if (_m2r_sect_print_flag == 1) {
          /* Adam on 10/28/03 -> during section process, 
                   do not get unit with Last tie */
          if (M2R_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
          	/* update RX pkts & bytes first */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
            }
          	/* update TX pkts & bytes secondly */
          	if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
            }
          }
        } else {
          if (M2R_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
          	/* update RX pkts & bytes first */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
            }
          	/* update TX pkts & bytes secondly */
          	if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
              p_unit_data = sect_cb->pStats + unit_id + 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
            }
          } else {
          	/* update RX pkts & bytes first */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1 ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_RX) ;
            }
          	/* update TX pkts & bytes secondly */
          	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
              p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
              p_unit_data = sect_cb->pStats + unit_id - 1;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
              p_unit_info = sect_cb->info_tbl + unit_id ;
              p_unit_data = sect_cb->pStats + unit_id ;
              m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                         p_unit_info, p_unit_data, M2R_TX) ;
            }
          }
    	}
      } else {
        m2r_stats_update_unit_data(port_id, sect_cb->stride_id,
                                   p_unit_info, p_unit_data, dir) ;
      }
    break ;

    case M2R_STATS_CLEAR :
      /* Adam Chen on 10/28/03 -> add check if this unit data has been updated */
      if ((_m2r_sect_print_flag == 1)
          || (!M2R_STATS_UNIT_IS_TIE_VALID(p_unit_info->flags)) ) {
        m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                  p_unit_info, p_unit_data, dir) ;
      } else {

        if (M2R_STATS_UNIT_IS_NEXT_TIE(p_unit_info->flags)) {
        	/* clear RX pkts & bytes first */
        	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_RX) ;
            p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
            p_unit_data = sect_cb->pStats + unit_id + 1 ;
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_RX) ;
          }
        	/* clear TX pkts & bytes secondly */
        	if ((dir == M2R_TX) || (dir == M2R_RX_AND_TX)) {
            p_unit_info = sect_cb->info_tbl + unit_id ;
            p_unit_data = sect_cb->pStats + unit_id ;
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_TX) ;
            p_unit_info = sect_cb->info_tbl + unit_id + 1 ;
            p_unit_data = sect_cb->pStats + unit_id + 1 ;
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_TX) ;
          }
        } else {
        	/* clear RX pkts & bytes first */
        	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
            p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
            p_unit_data = sect_cb->pStats + unit_id - 1 ;
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_RX) ;
            p_unit_info = sect_cb->info_tbl + unit_id ;
            p_unit_data = sect_cb->pStats + unit_id ;
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_RX) ;
          }
        	/* clear TX pkts & bytes secondly */
        	if ((dir == M2R_RX) || (dir == M2R_RX_AND_TX)) {
            p_unit_info = sect_cb->info_tbl + unit_id - 1 ;
            p_unit_data = sect_cb->pStats + unit_id - 1;
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_TX) ;
            p_unit_info = sect_cb->info_tbl + unit_id ;
            p_unit_data = sect_cb->pStats + unit_id ;
            m2r_stats_clear_unit_data(port_id, sect_cb->stride_id,
                                      p_unit_info, p_unit_data, M2R_TX) ;
          }
        }
      	
      }
    break ;
  }
  return (CS_OK) ;
}

/**************************************************************************
 * This is Internal function for STATS only use. This function handles
 * GET/PRINT and CLEAR requests at the Section level.
 **************************************************************************/
static cs_status m2r_stats_sect_cmn_op(cs_uint16 port_id,
                                       m2r_module_id_t mod_id,
                                       cs_uint16 sect_id,
                                       m2r_stats_sect_cb_t * stats_sect_cb,
                                       m2r_stats_op_type_t op,
                                       m2r_dir_t dir)
{
  cs_uint32   ii ;

  /* Adam on 10/28/03 -> */
  _m2r_sect_print_flag = 1;
  
  for (ii = 0; ii < stats_sect_cb->num_entries; ii++) {
    m2r_stats_unit_cmn_op(port_id, mod_id, sect_id, ii,
                          stats_sect_cb, op, dir) ;
  }

  return (CS_OK) ;
}

/************************************************************************
 * Check if any special handling is required based on the flags set for
 * the unit data.
 ***********************************************************************/
static
cs_boolean m2r_stats_is_spl_hndlr_valid(const m2r_stats_info_t * p_unit_info,
                                        m2r_stats_op_type_t op)
{
  cs_uint32 val = 0 ;

  switch(op) {
    case M2R_STATS_GET :
        val = (p_unit_info->flags & M2R_STATS_FLAG_SPL_GET) ;
        break ;
    case M2R_STATS_PRINT :
        val = (p_unit_info->flags & M2R_STATS_FLAG_SPL_PRINT) ;
        break ;
    case M2R_STATS_CLEAR :
        val = (p_unit_info->flags & M2R_STATS_FLAG_SPL_CLEAR) ;
        break ;
  }

  return ((val) ? TRUE : FALSE) ;
}

/************************************************************************
 * Call the Special handler.
 ***********************************************************************/
static cs_status m2r_stats_spl_handler(cs_uint16 port_id,
                                       m2r_module_id_t mod_id,
                                       cs_uint16 sect_id,
                                       cs_uint16 unit_id,
                                       m2r_stats_op_type_t op)
{
  m2r_stats_blk_cb_t * blk_cb = m2r_stats_get_blk_cb(port_id, mod_id) ;

  if (!blk_cb) {
    return (CS_ERROR) ;
  }

  switch (op) {
    case M2R_STATS_GET :
      if (blk_cb->get_func) {
        blk_cb->get_func(port_id, sect_id, unit_id) ;
      }
      break ;
    case M2R_STATS_PRINT :
      if (blk_cb->print_func) {
        blk_cb->print_func(port_id, sect_id, unit_id) ;
      }
      break ;
    case M2R_STATS_CLEAR :
      if (blk_cb->clear_func) {
        blk_cb->clear_func(port_id, sect_id, unit_id) ;
      }
      break ;
  }
  return (CS_OK) ;
}

/************************************************************************
 * GET POINTER TO STATS BLOCK CB
 ***********************************************************************/
static m2r_stats_blk_cb_t * m2r_stats_get_blk_cb(cs_uint16 port_id,
                                                 m2r_module_id_t mod_id)
{
  m2r_port_stats_cb_t * port_stats_cb ;

  /* Get the PORT STATS CB */
  port_stats_cb = M2R_GET_STATS_CB(port_id) ;

  if ( port_stats_cb == NULL ) {
    return (NULL) ;
  }

  switch (mod_id) {
    case M2R_ID_SPI42 : return( port_stats_cb->spi42 ) ;
    case M2R_ID_PPRBS : return( port_stats_cb->pprbs ) ;
    case M2R_ID_FRAMER: return( port_stats_cb->framer ) ;
    case M2R_ID_RMAC  : return( port_stats_cb->rmac ) ;
    case M2R_ID_LIF   : return( port_stats_cb->lif  ) ;
    case M2R_ID_ETH   : return( port_stats_cb->eth ) ;
    case M2R_ID_XGXS  : return( port_stats_cb->xgxs ) ;
    case M2R_ID_ALL_BLOCKS:
    default :
        return (NULL) ;
  }
  return (NULL) ;
}

/************************************************************************
 * SET THE BLOCK CB POINTER IN PORT STATS CB
 ***********************************************************************/
static cs_status m2r_stats_set_blk_cb(cs_uint16 port_id,
                                      m2r_module_id_t mod_id,
                                      m2r_stats_blk_cb_t * blk_cb)
{
  m2r_port_stats_cb_t * port_stats_cb ;

  /* Get the PORT STATS CB */
  port_stats_cb = M2R_GET_STATS_CB(port_id) ;

  if ( port_stats_cb == NULL ) {
    return (CS_ERROR) ;
  }

  switch (mod_id) {
    case M2R_ID_SPI42 : ( port_stats_cb->spi42 = blk_cb) ; break ;
    case M2R_ID_PPRBS : ( port_stats_cb->pprbs = blk_cb) ; break ;
    case M2R_ID_FRAMER: ( port_stats_cb->framer = blk_cb); break ;
    case M2R_ID_RMAC  : ( port_stats_cb->rmac = blk_cb) ;  break ;
    case M2R_ID_LIF   : ( port_stats_cb->lif  = blk_cb) ;  break ;
    case M2R_ID_ETH   : ( port_stats_cb->eth = blk_cb) ;   break ;
    case M2R_ID_XGXS  : ( port_stats_cb->xgxs = blk_cb) ; break ;
    default : return (CS_ERROR) ;
  }
  return (CS_OK) ;
}

/************************************************************************
 * PRINT BLOCK HEADER
 ***********************************************************************/
static void m2r_stats_print_blk_hdr(cs_uint16 port_id, cs_int8 * blk_name)
{
  if ( m2r_drvr_is_esc_code_enbld() ) {
     CS_PRINT("\033[4m\033[1m") ; /* underline + bold */
     CS_PRINT("\n\t\t\tPort-%d \t %s STATISTICS\n", port_id, blk_name) ;
     CS_PRINT("\033[m") ; /* put back to normal printing */
  }

  else {
    CS_PRINT("= = = = = = = = = = = = = = = = = = = = = = = = = = = =\n") ;
    CS_PRINT("Port-%d \t%s STATISTICS\n", port_id, blk_name) ;
    CS_PRINT("= = = = = = = = = = = = = = = = = = = = = = = = = = = =\n") ;
  }
  return ;
}

/************************************************************************
 * PRINT SECTION HEADER
 ***********************************************************************/
static void m2r_stats_print_sect_hdr(cs_uint16 port_id, cs_int8 * name)
{
  if ( m2r_drvr_is_esc_code_enbld() ) {
     CS_PRINT("\033[4m") ; /* underline */
     CS_PRINT("Port-%d %-44s %-16sRX %-16sTX%-1s\n", 
              port_id, name, "", "", "") ;
     CS_PRINT("\033[m") ; /* back to normal printing */
  }
 
  else {
     CS_PRINT("=============================================") ;
     CS_PRINT("=============================================\n") ;
     CS_PRINT("Port-%d %-44s %-16sRX %-16sTX\n", 
              port_id, name, "", "") ;
     CS_PRINT("=============================================") ;
     CS_PRINT("=============================================\n") ;
  }

  return ;
}

/************************************************************************
 * PRINT LEAF STATS
 ***********************************************************************/
static void m2r_stats_print_unit_cntr(const m2r_stats_info_t * p_info,
                                      m2r_stats_data_t * p_data)
{
  cs_uint64 rx, tx;
  cs_uint32 urx, lrx, utx, ltx;

  rx = p_data->rx ;
  tx = p_data->tx ;

  if (m2r_drvr_is_stats_in_hex_fmt() == TRUE ) {

    urx = M2R_STATS_GET_UPPER_32BIT_HEX(rx) ;
    lrx = M2R_STATS_GET_LOWER_32BIT_HEX(rx) ;
    utx = M2R_STATS_GET_UPPER_32BIT_HEX(tx) ;
    ltx = M2R_STATS_GET_LOWER_32BIT_HEX(tx) ;

    if (M2R_STATS_UNIT_IS_RX_VALID(p_info->flags) &&
        M2R_STATS_UNIT_IS_TX_VALID(p_info->flags)) { /* RX and TX Both valid */
      if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
      	if (tx|rx) {
          if ((urx>0)&&(utx>0)) {
      	    CS_PRINT("\t%-40s :  %8x%08xH %8x%08xH\n", p_info->name, 
                     urx, lrx, utx, ltx) ;
          } else if (urx>0) {
            CS_PRINT("\t%-40s :  %8x%08xH %16xH\n", p_info->name,    
                     urx, lrx, ltx) ;
          } else if (utx>0) {
            CS_PRINT("\t%-40s :  %16xH %8x%08xH\n", p_info->name,  
                     lrx, utx, ltx) ;
          } else {
            CS_PRINT("\t%-40s :  %16xH %16xH\n", p_info->name,  
                     lrx, ltx) ;
          }
      	}
      } else {
        if ((urx>0)&&(utx>0)) {
          CS_PRINT("\t%-40s :  %8x%08xH %8x%08xH\n", p_info->name,  
                   urx, lrx, utx, ltx) ;
        } else if (urx>0) {
          CS_PRINT("\t%-40s :  %8x%08xH %16xH\n", p_info->name,   
                   urx, lrx, ltx) ;
        } else if (utx>0) {
          CS_PRINT("\t%-40s :  %16xH %8x%08xH\n", p_info->name,   
                   lrx, utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %16xH %16xH\n", p_info->name,      
                   lrx, ltx) ;
        }
      }
    }
    else if (M2R_STATS_UNIT_IS_RX_VALID(p_info->flags)){ /* Only RX Valid */
      if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
      	if (rx) {
          if (urx>0) {
	     	
            CS_PRINT("\t%-40s :  %8x%08xH %18s\n", p_info->name,
                     urx, lrx, "N/A") ;
            /*
            CS_PRINT(PROMPT1("%-40s"), p_info->name);
	     	CS_PRINT("%8x%08xH\n", urx, lrx);	 
	    */
          } else {
            
            CS_PRINT("\t%-40s :  %16xH %18s\n", p_info->name,
                     lrx, "N/A") ;
           /*
            CS_PRINT(PROMPT1("%-40s"), p_info->name);
	     	CS_PRINT("%16xH\n", lrx);
	   */	
          }
      	}
      } else {
        if (urx>0) {
	   		
          CS_PRINT("\t%-40s :  %8x%08xH %18s\n", p_info->name,
                   urx, lrx, "N/A") ;
          /*
            CS_PRINT(PROMPT1("%-40s"), p_info->name);
	     	CS_PRINT("%8x%08xH\n", urx, lrx);
	 */    
        } else {
          
          CS_PRINT("\t%-40s :  %16xH %18s\n", p_info->name,
                   lrx, "N/A") ;
          /*
            CS_PRINT(PROMPT1("%-40s"), p_info->name);
	     	CS_PRINT("%16xH\n", lrx);
	  */          
        }
      }
    }
    else if (M2R_STATS_UNIT_IS_TX_VALID(p_info->flags)){ /* Only TX Valid */
      		
      if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
      	if (tx) {
          if (utx>0) {
      	    CS_PRINT("\t%-40s :  %18s %8x%08xH\n", p_info->name, "N/A", 
                     utx, ltx) ; 
          } else {
            CS_PRINT("\t%-40s :  %18s %16xH\n", p_info->name, "N/A",  
                     ltx) ;
          }
      	}
      } else {
        if (utx>0) {
          CS_PRINT("\t%-40s :  %18s %8x%08xH\n", p_info->name, "N/A",  
                   utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %18s %16xH\n", p_info->name, "N/A",   
                   ltx) ;
        }
      }
      	
    }
  }
  else {
    
    urx = M2R_STATS_GET_BILLIONS_DEC(rx) ;
    lrx = M2R_STATS_GET_BILLIONS_MOD_DEC(rx) ;
    utx = M2R_STATS_GET_BILLIONS_DEC(tx) ;
    ltx = M2R_STATS_GET_BILLIONS_MOD_DEC(tx) ;

    if (M2R_STATS_UNIT_IS_RX_VALID(p_info->flags) &&
        M2R_STATS_UNIT_IS_TX_VALID(p_info->flags)) { 
      if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
      	if (tx|rx) {
          if ((urx>0)&&(utx>0)) {
            CS_PRINT("\t%-40s :  %9u%09u %9u%09u\n", p_info->name,
                     urx, lrx, utx, ltx) ;
          } else if (urx>0) {
            CS_PRINT("\t%-40s :  %9u%09u %18u\n", p_info->name,
                     urx, lrx, ltx) ;
          } else if (utx>0) {
            CS_PRINT("\t%-40s :  %18u %9u%09u\n", p_info->name,
                     lrx, utx, ltx) ;
          } else {
            CS_PRINT("\t%-40s :  %18u %18u\n", p_info->name,
                     lrx, ltx) ;
          }
      	}
      } else {
        if ((urx>0)&&(utx>0)) {
          CS_PRINT("\t%-40s :  %9u%09u %9u%09u\n", p_info->name,
                   urx, lrx, utx, ltx) ;
        } else if (urx>0) {
          CS_PRINT("\t%-40s :  %9u%09u %18u\n", p_info->name,
                   urx, lrx, ltx) ;
        } else if (utx>0) {
          CS_PRINT("\t%-40s :  %18u %9u%09u\n", p_info->name,
                   lrx, utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %18u %18u\n", p_info->name,
                   lrx, ltx) ;
        }
      }
    }
    else if (M2R_STATS_UNIT_IS_RX_VALID(p_info->flags)){ 
      if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
      	if (rx) {
          if (urx>0) {
      	    CS_PRINT("\t%-40s :  %9u%09u %18s\n", p_info->name, 
                     urx, lrx, "N/A") ;
          } else {
            CS_PRINT("\t%-40s :  %18u %18s\n", p_info->name,
                     lrx, "N/A") ;
          }
      	}
      } else {
        if (urx>0) {
          CS_PRINT("\t%-40s :  %9u%09u %18s\n", p_info->name,
                   urx, lrx, "N/A") ;
        } else {
          CS_PRINT("\t%-40s :  %18u %18s\n", p_info->name,
                   lrx, "N/A") ;
        }
      }
    }
    else if (M2R_STATS_UNIT_IS_TX_VALID(p_info->flags)){ 
      if (m2r_drvr_is_mask_zero_cntr() == TRUE) {
      	if (tx) {
          if (utx>0) {
      	    CS_PRINT("\t%-40s :  %18s %9u%09u\n", p_info->name, "N/A", 
                     utx, ltx) ;
          } else {
            CS_PRINT("\t%-40s :  %18s %18u\n", p_info->name, "N/A",
                     ltx) ;
          }
      	}
      } else {
        if (utx>0) {
          CS_PRINT("\t%-40s :  %18s %9u%09u\n", p_info->name, "N/A",
                   utx, ltx) ;
        } else {
          CS_PRINT("\t%-40s :  %18s %18u\n", p_info->name, "N/A",
                   ltx) ;
        }
      }
    } 
  } 
}

/************************************************************************/
/* $rtn_hdr_start  UPDATE STATS FOR UNIT ON COUNTER IRQS                */
/* CATEGORY   : Port                                                    */
/* ACCESS     : PRIVATE                                                 */
/* BLOCK      : General                                                 */
/* CHIP       : BOTH                                                    */
cs_status m2r_stats_update_unit_ctr(cs_uint16 port_id,
                                    m2r_module_id_t mod_id,
                                    cs_uint16 sect_id,
                                    cs_uint16 unit_id,
                                    m2r_dir_t dir, cs_uint16 ctl)
/* INPUTS     : o Port Id                                               */
/*              o Module ID                                             */
/*              o Section ID                                            */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Update the Stats counter on receipt of interrupt related to counter  */
/* overflow or rollover.                                                */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32                 error = 0 ;
  m2r_stats_sect_cb_t     * stats_sect_cb = NULL ;
  m2r_stats_blk_cb_t      * stats_blk_cb ;
  const m2r_stats_info_t  * info_tbl = NULL ;
  m2r_stats_data_t        * p_unit_data ;
  cs_uint64                 val = 0, tmp ;
  volatile cs_reg         * regaddr = NULL ;
  cs_uint16               sl_num ;
  M2R_t                   * pDev ;
  cs_boolean          read_clr = FALSE ;

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ":port_id : %d\n", port_id) ;
    return (CS_ERROR) ;
  }

  if (sect_id >= M2R_STATS_MAX_SECT) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  if ((stats_blk_cb = m2r_stats_get_blk_cb(port_id, mod_id)) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "%s not registered for Statistics\n", M2R_GET_MOD_NAME(mod_id)) ;
    return (CS_ERROR) ;
  }

  if ((stats_sect_cb = stats_blk_cb->p_sect_cb[sect_id]) == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_TBL_NULL,
           "Section not registered for Statistics\n") ;
    return (CS_ERROR) ;
  }

  if (unit_id >= stats_sect_cb->num_entries) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_RANGE_ERR, NULL ) ;
    return (CS_ERROR) ;
  }

  info_tbl = stats_sect_cb->info_tbl ;
  p_unit_data = stats_sect_cb->pStats + unit_id ;

  switch (ctl) {
    case M2R_STATS_CTR_OVRFLOW :
    break ;

    case M2R_STATS_CTR_ROLLOVR :
      tmp = 1 ;
      val = (tmp << (info_tbl->num_regs * 16)) ;
      tmp = 0 ;
      if (dir == M2R_TX) {
        p_unit_data->tx &= ((~tmp) << (info_tbl->num_regs * 16)) ;
        p_unit_data->tx += val ;
      }
      else {
        p_unit_data->rx &= ((~tmp) << (info_tbl->num_regs * 16)) ;
        p_unit_data->rx += val ;
      }
    break ;

    case M2R_STATS_CTR_SATURATE :
    break ;

    case M2R_STATS_CTR_MSB_0_TO_1 :
      pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
      sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

      if ((M2R_STATS_UNIT_IS_RX_VALID(info_tbl->flags)) &&
          (dir == M2R_RX)) {

        /* If this is not a slice register, then sl_num better be 0 */
        if ( !(M2R_IS_SLICE_REG(info_tbl->rx_reg_ofst)) ) {
          sl_num = 0 ;
        }

        regaddr = (volatile cs_reg*)
                    M2R_STATS_GET_RX_REG_ADDR(pDev, sl_num, stats_sect_cb->stride_id,
                                              info_tbl) ;
        read_clr = (info_tbl->flags & M2R_STATS_FLAG_RX_READ_CLR) ?
                      TRUE : FALSE ;
        val = m2r_stats_get_hw_cntr(regaddr, info_tbl->num_regs) ;
        m2r_stats_clear_hw_cntr(regaddr, info_tbl->num_regs, read_clr) ;
        p_unit_data->rx += val ;
      }
      else {
        /* If this is not a slice register, then sl_num better be 0 */
        if ( !(M2R_IS_SLICE_REG(info_tbl->tx_reg_ofst)) ) {
          sl_num = 0 ;
        }

        regaddr = (volatile cs_reg*)
                    M2R_STATS_GET_TX_REG_ADDR(pDev, sl_num, stats_sect_cb->stride_id,
                                              info_tbl) ;
        read_clr = (info_tbl->flags & M2R_STATS_FLAG_TX_READ_CLR) ?
                      TRUE : FALSE ;
        val = m2r_stats_get_hw_cntr(regaddr, info_tbl->num_regs) ;
        m2r_stats_clear_hw_cntr(regaddr, info_tbl->num_regs, read_clr) ;
        p_unit_data->tx += val ;
      }
    break ;
  }

  return (CS_OK) ;
}


