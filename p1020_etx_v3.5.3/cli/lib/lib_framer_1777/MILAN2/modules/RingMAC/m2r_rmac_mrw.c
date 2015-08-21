/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_mrw.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_rmac.h"
#include "m2r_rmac_priv.h"
#include "m2r_rmac_mrw.h"

/****************************************************************
 * Internal function declarations
 ****************************************************************/
static
cs_status  m2r_write_to_rewrite_idx_sram(cs_uint16 port_id,
                                         cs_uint16 entry_num,
                                         m2r_rmac_rewrite_entry_t * p) ;

static
cs_status  m2r_read_from_rewrite_idx_sram(cs_uint16 port_id,
                                          cs_uint16 entry_num,
                                          m2r_rmac_rewrite_entry_t * p) ;

static
cs_status m2r_rmac_set_hdr_field(cs_uint16 port_id,
                                 M2R_t * pDev,
                                 cs_uint16 sl_num,
                                 cs_uint16 index,
                                 m2r_rmac_rpr_hdr_field_t field,
                                 cs_uint8 field_val) ;

/****************************************************************/
/* $rtn_hdr_start ENABLE/DISABLE RINGMAC REWRITE MODE           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_ctl_rewrite(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable the Mac re-write Mode. When re-write mode is  */
/* is enabled, packets coming from the HOST are expected to be  */
/* psuedo RPR Mode. The psuedo RPR packet header contains info  */
/* fields which are indexed into re-write SRAM memory and other */
/* registers to get the RPR header field values.                */
/* encoded-DA field is indexed into the DA SRAM to get 48 bit   */
/* DA MAC address. Priority index is indexed into MAC_OFFSET0-3 */
/* registers to get COS field offset and then indexed into SRAM */
/* to get the 3 bit priority (FE and SC) fields for RPR header. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16         dev_num, sl_num ;
  M2R_t           * pDev ;
  DR_MICRO_IF_CR1_t cr1 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  cr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
  cr1.bf.MAC_REWRITE = (ctl == M2R_ENABLE) ?
                MAC_REWRITE_MACRW_PSRP : MAC_REWRITE_MACRW_OFF ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start ADD MAC ADDR, TTL FILEDS TO REWRITE TABLE     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_add_entry_da(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 mac5_4,
                                       cs_uint16 mac3_2,
                                       cs_uint16 mac1_0,
                                       cs_uint8 ttl)
/* INPUTS     : o Port Id                                       */
/*              o SRAM Location                                 */
/*              o DA MAC Address byte 5 and 6                   */
/*              o DA MAC Address byte 3 and 4                   */
/*              o DA MAC Address byte 1 and 2                   */
/*              o TTL Value                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Add a MAC rewrite entry (MAC address and TTL fields) into    */
/* SRAM for pseudo SRP or RPR mode.                             */
/* This RAM is used only when re-write is selected/enabled.     */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_rewrite_reset_entry() API.                          */
/*                                                              */
/* NOTE:                                                        */
/* Although, MAC address(48-bits) and Priority field(3-bits)    */
/* share the same index SRAM entry, conceptually they are       */
/* orthogonal!  This SRAM is looked up twice -                  */
/* First time, with the encoded DA to get the DA MAC address,   */
/* and second time, with (priority index(2-bits),               */
/* class-of-service(8-bits)) to get priority(3-bit) field.      */
/* Use m2r_rmac_rewrite_add_entry_priority() to add only        */
/* priority field.                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_rewrite_entry_t     sram_entry ;
  cs_status                    status = CS_OK ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, sram_index, 1,
			    M2R_RMAC_REWRITE_SRAM_SIZE,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  if (m2r_rmac_rewrite_get_entry(port_id, sram_index, 1, &sram_entry)
        != 1) {
    return CS_ERROR ;
  }

  sram_entry.da_addr.byte5 = mac5_4 >> 8 ;
  sram_entry.da_addr.byte4 = mac5_4 & 0xff ;
  sram_entry.da_addr.byte3 = mac3_2 >> 8 ;
  sram_entry.da_addr.byte2 = mac3_2 & 0xff ;
  sram_entry.da_addr.byte1 = mac1_0 >> 8 ;
  sram_entry.da_addr.byte0 = mac1_0 & 0xff ;

  sram_entry.ttl  = ttl ;

  status = m2r_write_to_rewrite_idx_sram(port_id, sram_index, &sram_entry) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start ADD PRIORITY FILED TO REWRITE TABLE           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_add_entry_priority(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint8 priority)
/* INPUTS     : o Port Id                                       */
/*              o SRAM Location                                 */
/*              o Priority ( FE and SC, 0...7)                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Add a MAC rewrite entry (priority field only) into SRAM for  */
/* pseudo SRP or RPR mode.                                      */
/* This RAM is used only when re-write is selected/enabled.     */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_rewrite_reset_entry() API.                          */
/*                                                              */
/* NOTE:                                                        */
/* Although, MAC address(48-bits) and Priority field(3-bits)    */
/* share the same index SRAM entry, conceptually they are       */
/* orthogonal!  This SRAM is looked up twice -                  */
/* First time, with the encoded DA to get the DA MAC address,   */
/* and second time, with (priority index(2-bits),               */
/* class-of-service(8-bits)) to get priority(3-bit) field.      */
/* Use m2r_rmac_rewrite_add_entry_da() to add MAC DA and TTL    */
/* fields.                                                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_rewrite_entry_t     sram_entry ;
  cs_status                    status = CS_OK ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, sram_index, 1,
			    M2R_RMAC_REWRITE_SRAM_SIZE,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  if (priority > 7) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
        ": priority(%d) s/b 0...7\n", priority) ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  if (m2r_rmac_rewrite_get_entry(port_id, sram_index, 1, &sram_entry)
        != 1) {
    return CS_ERROR ;
  }

  sram_entry.prio = priority ;

  status = m2r_write_to_rewrite_idx_sram(port_id, sram_index, &sram_entry) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start ADD MAC REWRITE DA SRAM ENTRY                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_add_entry(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 mac5_4,
                                       cs_uint16 mac3_2,
                                       cs_uint16 mac1_0,
                                       cs_uint8 priority,
                                       cs_uint8 ttl)
/* INPUTS     : o Port Id                                       */
/*              o SRAM Location                                 */
/*              o DA MAC Address byte 5 and 6                   */
/*              o DA MAC Address byte 3 and 4                   */
/*              o DA MAC Address byte 1 and 2                   */
/*              o Priority ( FE and SC, 0...7)                  */
/*              o TTL Value                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Add a MAC rewrite entry into SRAM for pseudo SRP or RPR mode.*/
/* This RAM is used only when re-write is selected/enabled.     */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_rewrite_reset_entry() API.                          */
/*                                                              */
/* NOTE:                                                        */
/* Although, MAC address(48-bits) and Priority field(3-bits)    */
/* share the same index SRAM entry, conceptually they are       */
/* orthogonal!  This SRAM is looked up twice -                  */
/* First time, with the encoded DA to get the DA MAC address,   */
/* and second time, with (priority index(2-bits),               */
/* class-of-service(8-bits)) to get priority(3-bit) field.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  m2r_rmac_rewrite_entry_t     sram_entry ;
  cs_status                    status = CS_OK ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, sram_index, 1,
			    M2R_RMAC_REWRITE_SRAM_SIZE,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  if (priority > 7) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
        ": priority(%d) s/b 0...7\n", priority) ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  sram_entry.da_addr.byte5 = mac5_4 >> 8 ;
  sram_entry.da_addr.byte4 = mac5_4 & 0xff ;
  sram_entry.da_addr.byte3 = mac3_2 >> 8 ;
  sram_entry.da_addr.byte2 = mac3_2 & 0xff ;
  sram_entry.da_addr.byte1 = mac1_0 >> 8 ;
  sram_entry.da_addr.byte0 = mac1_0 & 0xff ;

  sram_entry.prio = priority ;
  sram_entry.ttl  = ttl ;

  status = m2r_write_to_rewrite_idx_sram(port_id, sram_index, &sram_entry) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (status) ;
}


/****************************************************************/
/* $rtn_hdr_start ADD MAC REWRITE DA SRAM ENTRIES               */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_add_entries(cs_uint16 port_id,
                                       cs_uint16 start_idx,
                                       cs_uint16 num,
                                    m2r_rmac_rewrite_entry_t * tbl)
/* INPUTS     : o Port Id                                       */
/*              o Start Index in SRAM                           */
/*              o Number of entries to be added                 */
/*              o Pointer to array of entries                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/*                                                              */
/* Add MAC rewrite entries into SRAM for pseudo SRP or RPR mode.*/
/* This RAM is used only when re-write is selected/enabled.     */
/* User is responsible for providing pointer to array of        */
/* m2r_rmac_rewrite_entry_t.                                    */
/* Entries can be selectively invalidated by calling            */
/* m2r_rmac_rewrite_reset_entry() API.                          */
/* [start_idx] specifies the index into re-write SRAM from      */
/* where the entries need to be added and [num] specifies       */
/* number of entries to be added. User should make sure that    */
/* index of entries doesn't fall out of the SRAM index range.   */
/*                                                              */
/* NOTE:                                                        */
/* Although, MAC address(48-bits) and Priority field(3-bits)    */
/* share the same index SRAM entry, conceptually they are       */
/* orthogonal!  This SRAM is looked up twice -                  */
/* First time, with the encoded DA to get the DA MAC address,   */
/* and second time, with (priority index(2-bits),               */
/* class-of-service(8-bits)) to get priority(3-bit) field.      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32                    ii ;
  cs_status                    status = CS_OK ;

  if (tbl == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_NULL_PTR, NULL) ;
    return (CS_ERROR) ;
  }

  if ( M2R_TBL_BOUNDS_CHECK(port_id, start_idx, num,
			    M2R_RMAC_REWRITE_SRAM_SIZE,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;


  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  for ( ii = 0; ii < num; ii++) {
    if ((status = m2r_write_to_rewrite_idx_sram(port_id, (start_idx + ii), (tbl + ii)))
            == CS_ERROR) {
      break ;
    }
  }
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (status) ;
}

/****************************************************************/
/* $rtn_hdr_start  READ MAC REWRITE INDEX SRAM ENTRIES          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint16 m2r_rmac_rewrite_get_entry(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 num,
                                       m2r_rmac_rewrite_entry_t * tbl)
/* INPUTS     : o Port Id                                       */
/*              o Start Index in SRAM                           */
/*              o Number of entries to be added                 */
/*              o Pointer to array of entries                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Number of entries read into the array           */
/* DESCRIPTION:                                                 */
/* Read the Index SRAM entries into the user supplied array.    */
/* User is responsible for allocating the array and passing the */
/* Array pointer to the function.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32                    ii = 0;

  if (tbl == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_NULL_PTR, NULL) ;
    return (0) ;
  }

  if ( M2R_TBL_BOUNDS_CHECK(port_id, sram_index, num,
			    M2R_RMAC_REWRITE_SRAM_SIZE,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  for ( ii = 0; ii < num; ii++) {
    if (m2r_read_from_rewrite_idx_sram(port_id,
               (ii + sram_index), (tbl + ii)) == CS_ERROR) {
      break ;
    }
  }
  return (ii) ;
}

/****************************************************************/
/* $rtn_hdr_start  DUMP MAC REWRITE INDEX SRAM ENTRIES          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint16 m2r_rmac_rewrite_dump_entry(cs_uint16 port_id,
                                       cs_uint16 sram_index,
                                       cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/*              o Start Index in SRAM                           */
/*              o Number of entries to be added                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Number of entries read into the array           */
/* DESCRIPTION:                                                 */
/* Read the Index SRAM entries into the user supplied array.    */
/* User is responsible for allocating the array and passing the */
/* Array pointer to the function.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32                    ii = 0;
  m2r_rmac_rewrite_entry_t     tbl ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, sram_index, num,
			    M2R_RMAC_REWRITE_SRAM_SIZE,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  for ( ii = 0; ii < num; ii++) {
    if (m2r_read_from_rewrite_idx_sram(port_id,
               (ii + sram_index), &tbl) == CS_ERROR) {
      break ;
    }

    CS_PRINT("SRAM Index %d => DA (%x:%x:%x:%x:%x:%x) Prio (%d) TTL (%x)\n",
              ii + sram_index, tbl.da_addr.byte5, tbl.da_addr.byte4,
              tbl.da_addr.byte3, tbl.da_addr.byte2, tbl.da_addr.byte1,
              tbl.da_addr.byte0, tbl.prio, tbl.ttl) ;
  }
  return (ii) ;
}

/****************************************************************/
/* $rtn_hdr_start CLEAR MAC REWRITE SRAM ENTRIES                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_reset_entries(cs_uint16 port_id,
                                         cs_uint16 sram_index,
                                         cs_uint16 num)
/* INPUTS     : o Port Id                                       */
/*              o Start Index of the entry to be purged         */
/*              o Number of entries to be purged                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Clears the MAC re-write SRAM entries.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  m2r_rmac_rewrite_entry_t     sram_entry ;
  cs_uint32                    ii ;

  if ( M2R_TBL_BOUNDS_CHECK(port_id, sram_index, num,
			    M2R_RMAC_REWRITE_SRAM_SIZE,
			    __FUNCTION__, __LINE__)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;


  CS_MEMSET( (void *)&sram_entry, 0,
	     sizeof(m2r_rmac_rewrite_entry_t) ) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  for (ii = 0; ii < num; ii++) {
    m2r_write_to_rewrite_idx_sram(port_id, (sram_index + ii),
                                  &sram_entry) ;
  }
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE PROTOCOL FIELD FOR MAC RE-WRITE      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_protocol(cs_uint16 port_id,
                                        cs_uint8  index,
                                        cs_uint16 protocol)
/* INPUTS     : o Port Id                                       */
/*              o Index of the protocol register [0-3]          */
/*              o Protocol field value                          */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the Protocol Field that is inserted as part of the data */
/* packet header in the MAC Header Re-write block.              */
/* [index] is the protocol index that is carried in psuedo-RPR  */
/* packet's protocol_index field.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  switch (index) {
    case 0 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_PROTOCOL0.wrd, protocol) ;
      break ;
    case 1 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_PROTOCOL1.wrd, protocol) ;
      break ;
    case 2 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_PROTOCOL2.wrd, protocol) ;
      break ;
    case 3 :
      M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_PROTOCOL3.wrd, protocol) ;
      break ;
    default :
      CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR,
                    "Invalid index, valid range [0-3]\n") ;
  }
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start SET THE RPR HDR FIELD VALUE TO BE WRITTEN     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_rpr_hdr_field(cs_uint16 port_id,
                                        cs_uint16 index,
                                        m2r_rmac_rpr_hdr_field_t field,
                                        cs_uint8  field_val)
/* INPUTS     : o Port Id                                       */
/*              o Index value [0 - 3]                           */
/*              o RPR header field Type                         */
/*              o Field value                                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the RPR Header field value to written to the incoming   */
/* psuedo RPR packet. The value field_val is written to the     */
/* RPR packet.                                                  */
/*                                                              */
/* The [field] can be one of the followings :                   */
/* M2R_RMAC_RPR_FIELD_WE or M2R_RMAC_RPR_FIELD_STRICT or        */
/* M2R_RMAC_RPR_FIELD_WS                                        */
/*                                                              */
/* Note : User should also call                                 */
/* m2r_rmac_ctl_rpr_hdr_field_rewrite() API to enable/disable   */
/* re-writing of the RPR header fields.                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (index >= 4) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NUM_INVALID, NULL) ;
    return ( CS_ERROR ) ;
  }

  if (field & M2R_RMAC_RPR_FIELD_WE) {
    m2r_rmac_set_hdr_field(port_id, pDev, sl_num,
                           index, M2R_RMAC_RPR_FIELD_WE,
                           field_val) ;
  }

  if (field & M2R_RMAC_RPR_FIELD_STRICT) {
    m2r_rmac_set_hdr_field(port_id, pDev, sl_num,
                           index, M2R_RMAC_RPR_FIELD_STRICT,
                           field_val) ;
  }

  if (field & M2R_RMAC_RPR_FIELD_WS) {
    m2r_rmac_set_hdr_field(port_id, pDev, sl_num,
                           index, M2R_RMAC_RPR_FIELD_WS, field_val) ;
  }
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE RE-WRITE OF RPR/SRP FIELDS    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_ctl_rpr_hdr_field_rewrite(cs_uint16 port_id,
                    m2r_rmac_rpr_hdr_field_t field, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o RPR header field Type                         */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable re-write of RPR header fields.                */
/* The [field] can be one of the followings :                   */
/* M2R_RMAC_RPR_FIELD_WE or M2R_RMAC_RPR_FIELD_STRICT or        */
/* M2R_RMAC_RPR_FIELD_WS or M2R_RMAC_RPR_FIELD_SA               */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  if ( (field & M2R_RMAC_RPR_FIELD_WE) ||
       (field & M2R_RMAC_RPR_FIELD_STRICT) ) {
    DR_MAC_HEAD_MACWR_t          macwr ;

    macwr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACWR.wrd) ;
    if (field & M2R_RMAC_RPR_FIELD_WE) {
      macwr.bf.WE = (ctl == M2R_ENABLE) ? 1 : 0 ;
    }
    if (field & M2R_RMAC_RPR_FIELD_STRICT) {
      macwr.bf.STRICT_MODE = (ctl == M2R_ENABLE) ? 1 : 0 ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MACWR.wrd, macwr.wrd) ;
  }

  if ( (field & M2R_RMAC_RPR_FIELD_WS) ||
       (field & M2R_RMAC_RPR_FIELD_SA) ) {
    DR_MAC_HEAD_MAC_MODE_t       macmode ;
    macmode.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd) ;

    if (field & M2R_RMAC_RPR_FIELD_WS) {
      macmode.bf.WS = (ctl == M2R_ENABLE) ? 1 : 0 ;
    }
    if (field & M2R_RMAC_RPR_FIELD_SA) {
      macmode.bf.REW_DRONLY = (ctl == M2R_ENABLE) ? 1 : 0 ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd, macmode.wrd) ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start SET THE IPDA VALUE IN MAC_OFFSET REGISTER     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_ipda(cs_uint16 port_id,
                              cs_uint16 index, cs_uint8  ipda_val)
/* INPUTS     : o Port Id                                       */
/*              o index value [0-3]                             */
/*              o IPDA [0-63]                                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the IPDA value in the MAC-OFFSET register.              */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  volatile cs_reg            * regaddr ;
  DR_MAC_HEAD_MAC_OFFSET0_t    hdr_rw ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if (index >= 4) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_TBL_NUM_INVALID, NULL) ;
    return ( CS_ERROR ) ;
  }

  if ((ipda_val > 0x3f) || (ipda_val <= 3)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR, NULL) ;
    return ( CS_ERROR ) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  regaddr = &pDev->slice[sl_num].RingMAC.MAC_OFFSET0.wrd + index ;
  hdr_rw.wrd = M2R_REG_READ(regaddr) ;
  hdr_rw.bf.IPDA = (ipda_val & 0x3f) ;
  M2R_REG_WRITE(regaddr, hdr_rw.wrd) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start SET THE COS FIELD OFFSET IN THE INCOMING PKT  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_cos_offset(cs_uint16 port_id,
                             cs_uint16 index, cs_uint8  offset_val)
/* INPUTS     : o Port Id                                       */
/*              o index value [0-3]                             */
/*              o COS field offset value [0-63]                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the offset of the COS byte in the incoming packet.      */
/* COS byte from this offset is taken and is then indexed       */
/* into SRAM to get to the final priority field thats           */
/* inserted in priority field of the RPR packet.                */
/* The priority field is composed of FE ( Fairness eligible )   */
/* field and SC ( Service category field ).                     */
/* The index value is in the range 0 to 3. Incoming psuedo RPR  */
/* packet will have this index in priority_index field.         */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  volatile cs_reg            * regaddr ;
  DR_MAC_HEAD_MAC_OFFSET0_t    hdr_rw ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  if ((index >= 4) || (offset_val > 0x3f)) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_RANGE_ERR, NULL) ;
    return ( CS_ERROR ) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  regaddr = &pDev->slice[sl_num].RingMAC.MAC_OFFSET0.wrd + index ;
  hdr_rw.wrd = M2R_REG_READ(regaddr) ;
  hdr_rw.bf.COS = (offset_val & 0x3f) ;
  M2R_REG_WRITE(regaddr, hdr_rw.wrd) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE SOURCE MAC ADDRESS                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_sa_mac(cs_uint16 port_id,
                                      cs_uint16 sa_mac_4_5,
                                      cs_uint16 sa_mac_2_3,
                                      cs_uint16 sa_mac_0_1,
                                      cs_boolean rewrite)
/* INPUTS     : o Port Id                                       */
/*              o Mac address byte 5 and 6                      */
/*              o Mac address byte 3 and 4                      */
/*              o Mac address byte 1 and 2                      */
/*              o TRUE (re-write MAC SA always)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the source Mac address to be inserted in the psudo RPR   */
/* packet. If [rewrite] is TRUE, irrespective of the rewrite    */
/* mode, MAC SA will always be re-written. To re-write MAC      */
/* SA only when in psuedo RPR Mode,                             */
/* m2r_rmac_ctl_rpr_hdr_field_rewrite() API should be used      */
/* to control re-writing of MAC SA.                             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  DR_MAC_HEAD_MAC_MODE_t       macmode ;
  DR_MICRO_IF_CR1_t            cr1 ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_SA2.wrd, sa_mac_4_5) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_SA1.wrd, sa_mac_2_3) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_SA0.wrd, sa_mac_0_1) ;

  macmode.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd) ;
  macmode.bf.REW_SA = (rewrite) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd, macmode.wrd) ;

  cr1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.CR1_.wrd) ;
  cr1.bf.MAC_SA_PKT = (rewrite) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.CR1_.wrd, cr1.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start SET THE TTL VALUE TO BE WRITEN TO RPR PKT     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_ttl(cs_uint16 port_id,
                             cs_uint32 ttl_type, cs_uint8 ttl_val)
/* INPUTS     : o Port Id                                       */
/*              o TTL field Type                                */
/*              o TTL value                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the TTL field values to be written to RPR pkt for       */
/* non-flooded pkts, uni-directional flooded pkt and bi-dir     */
/* flooded pkts.                                                */
/*                                                              */
/* The [ttl_type] can be logical OR mask of the following:      */
/* M2R_RMAC_NO_FLOOD_TTL  OR M2R_RMAC_NO_FLOOD_TTL_BASE   OR    */
/* M2R_RMAC_UNI_FLOOD_TTL OR M2R_RMAC_UNI_FLOOD_TTL_BASE  OR    */
/* M2R_RMAC_BI_FLOOD_TTL  OR M2R_RMAC_BI_FLOOD_TTL_BASE   or    */
/* M2R_RMAC_TTL_TYPE_ALL                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  if ( (ttl_type & M2R_RMAC_NO_FLOOD_TTL) ||
       (ttl_type & M2R_RMAC_NO_FLOOD_TTL_BASE) ) {
    DR_MAC_HEAD_MACTTL0_t macttl0 ;
    macttl0.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACTTL0.wrd) ;
    if (ttl_type & M2R_RMAC_NO_FLOOD_TTL){
      macttl0.bf.TTL = ttl_val ;
    }
    if (ttl_type & M2R_RMAC_NO_FLOOD_TTL_BASE){
      macttl0.bf.TTLBASE = ttl_val ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MACTTL0.wrd, macttl0.wrd) ;
  }

  if ( (ttl_type & M2R_RMAC_UNI_FLOOD_TTL) ||
       (ttl_type & M2R_RMAC_BI_FLOOD_TTL) ) {
    DR_MAC_HEAD_MACTTL1_t macttl1 ;
    macttl1.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACTTL1.wrd) ;
    if (ttl_type & M2R_RMAC_UNI_FLOOD_TTL) {
      macttl1.bf.TTLUNI = ttl_val ;
    }
    if (ttl_type & M2R_RMAC_BI_FLOOD_TTL) {
      macttl1.bf.TTLBI = ttl_val ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MACTTL1.wrd, macttl1.wrd) ;
  }

  if ( (ttl_type & M2R_RMAC_UNI_FLOOD_TTL_BASE) ||
       (ttl_type & M2R_RMAC_BI_FLOOD_TTL_BASE) ) {
    DR_MAC_HEAD_MACTTL2_t macttl2 ;
    macttl2.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACTTL2.wrd) ;
    if (ttl_type & M2R_RMAC_UNI_FLOOD_TTL_BASE) {
      macttl2.bf.TTLUNIBASE = ttl_val ;
    }
    if (ttl_type & M2R_RMAC_BI_FLOOD_TTL_BASE) {
      macttl2.bf.TTLBIBASE = ttl_val ;
    }
    M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MACTTL2.wrd, macttl2.wrd) ;
  }

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE TTL INSERTION MODE                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_ctl_ttl_mode(cs_uint16 port_id,
                                 cs_uint32 ttl_type,
                                 m2r_rmac_rpr_hdr_ttl_mode_t mode)
/* INPUTS     : o Port Id                                       */
/*              o TTL field Type                                */
/*              o TTL MODE                                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the TTL Mode for Data and control RPR packets.          */
/*                                                              */
/* The [ttl_type] can be logical OR mask of the following:      */
/* M2R_RMAC_NO_FLOOD_TTL  OR M2R_RMAC_NO_FLOOD_TTL_BASE   OR    */
/* M2R_RMAC_UNI_FLOOD_TTL OR M2R_RMAC_UNI_FLOOD_TTL_BASE  OR    */
/* M2R_RMAC_BI_FLOOD_TTL  OR M2R_RMAC_BI_FLOOD_TTL_BASE   or    */
/* M2R_RMAC_TTL_TYPE_ALL                                        */
/*                                                              */
/* The [mode] can be one of the followings:                     */
/* M2R_RMAC_RPR_TTL_NO_CHANGE or M2R_RMAC_RPR_TTL_DA_SRAM or    */
/* M2R_RMAC_RPR_TTL_REG or M2R_RMAC_RPR_TTL_BASE                */
/* Note : M2R_RMAC_RPR_TTL_DA_SRAM is Not valid for flooded     */
/* uni-directional and bi-directional Packets.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  DR_MAC_HEAD_MACWR_t          macwr ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  macwr.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACWR.wrd) ;

  if( ttl_type & M2R_RMAC_NO_FLOOD_TTL) {
    macwr.bf.TTL = (mode & 0x03) ;
  }
  if ( ttl_type & M2R_RMAC_NO_FLOOD_TTL_BASE) {
    macwr.bf.TTLBASE = (mode & 0x003) ;
  }
  if ( ttl_type & M2R_RMAC_UNI_FLOOD_TTL) {
    macwr.bf.TTLUNI = (mode & 0x03) ;
  }
  if ( ttl_type & M2R_RMAC_UNI_FLOOD_TTL_BASE) {
    macwr.bf.TTLUNIBASE = (mode & 0x03) ;
  }
  if ( ttl_type & M2R_RMAC_BI_FLOOD_TTL) {
    macwr.bf.TTLBI = (mode & 0x03) ;
  }
  if ( ttl_type & M2R_RMAC_BI_FLOOD_TTL_BASE) {
    macwr.bf.TTLBIBASE = (mode & 0x03) ;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MACWR.wrd, macwr.wrd) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET THE PT/MODE FIELD OF RPR/SRP PKT         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_pkt_type(cs_uint16 port_id,
                                        cs_uint8  pktType)
/* INPUTS     : o Port Id                                       */
/*              o Packet Type                                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the MODE value that is inserted as part of the SRP      */
/* Header in Pseudo-SRP mode.                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  DR_MAC_HEAD_MAC_MODE_t       macmode ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  macmode.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd) ;
  if (M2R_GET_MAC_MODE(port_id) == M2R_PORT_IN_SRP_MODE) {
    macmode.bf.MAC_MODE = (pktType & 0x07) ;
  }
  else {
    macmode.bf.PT = (pktType & 0x03) ;
  }
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MAC_MODE.wrd, macmode.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE TSN VALUE                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_set_tsn(cs_uint16 port_id, cs_uint8  tsn)
/* INPUTS     : o Port Id                                       */
/*              o TSN VALUE                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the expected TSN Value in the IDLE packets coming from   */
/* the HOST .                                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  DR_MAC_HEAD_MACTSN_t         mactsn ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  mactsn.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACTSN.wrd) ;
  mactsn.bf.TSN = tsn  ;
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MACTSN.wrd, mactsn.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SET THE TSN VALUE                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_uint8 m2r_rmac_rewrite_get_tsn(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : FLUSHED TSN value from the IDLE Packet          */
/* DESCRIPTION:                                                 */
/* Return the TSN value read from the IDLE packet.              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  return (M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACTSN.wrd) & 0x00ff) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE CHECKING OF TSN IN IDLE PKTS  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : RMAC                                            */
/* CHIP       : Both                                            */
cs_status m2r_rmac_rewrite_ctl_tsn_chk(cs_uint16 port_id,
                                       m2r_rmac_idle_pkt_ctl_t op,
                                       m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Control option                                */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable various operations on IDLE packets.           */
/*                                                              */
/* The [op] parameter can be one of the followings:             */
/* M2R_RMAC_DROP_TSN_MISMATCH or M2R_RMAC_UPDATE_TSN or         */
/* M2R_RMAC_STRIP_ALL                                           */
/*                                                              */
/* M2R_RMAC_DROP_TSN_MISMATCH option is used to drop the IDLE   */
/* packets coming from the HOST that dont have the matching     */
/* TSN as expected. To Set the expected TSN, user should use    */
/* m2r_rmac_rewrite_set_tsn() API.                              */
/* M2R_RMAC_UPDATE_TSN option is used to update the TSN value   */
/* in RMAC register by reading it from the TSN in IDLE pkt.     */
/* User should use m2r_rmac_get_flushed_tsn() API to read TSN.  */
/* M2R_RMAC_STRIP_ALL option is used to strip all the incoming  */
/* IDLE packets in mac re-write block.                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                    dev_num, sl_num ;
  M2R_t                      * pDev ;
  DR_MAC_HEAD_MACTSN_t         mactsn ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;
  M2R_GET_PORT_PARAMS(port_id, &dev_num, &sl_num, &pDev) ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;

  mactsn.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MACTSN.wrd) ;
  switch (op) {
    case M2R_RMAC_DROP_TSN_MISMATCH :
      mactsn.bf.ENABLE = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
    case M2R_RMAC_UPDATE_TSN :
      mactsn.bf.IDLEUPDATE = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
    case M2R_RMAC_STRIP_IDLE :
      mactsn.bf.IDLESTRIP = (ctl == M2R_ENABLE) ? 1 : 0 ;
      break ;
  }
  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MACTSN.wrd, mactsn.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}

/*********************************************************************
 *            WRITE TO RE-WRITE INDEX SRAM
 *********************************************************************/
static
cs_status  m2r_write_to_rewrite_idx_sram(cs_uint16 port_id,
                                         cs_uint16 entry_num,
                                         m2r_rmac_rewrite_entry_t * p)
{
  m2r_rmac_gram_t       gram ;

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  gram.GRAM_D0.wrd = ( p->da_addr.byte0 | (p->da_addr.byte1 << 8) ) ;
  gram.GRAM_D1.wrd = ( p->da_addr.byte2 | (p->da_addr.byte3 << 8) ) ;
  gram.GRAM_D2.wrd = ( p->da_addr.byte4 | (p->da_addr.byte5 << 8) ) ;
  gram.GRAM_D3.wrd = ( p->prio | (p->ttl << 3) ) ;

  gram.GRAM_CR.bf.SEL    = SEL_MACREW_IDX_RAM ;

  gram.GRAM_CR.bf.GRWN   = GRWN_WRITE ;
  gram.GRAM_CR.bf.GRAM_A = entry_num ;

  return ( m2r_rmac_write_gram_regs(port_id, &gram) ) ;
}

/*********************************************************************
 *            READ INDEX SRAM ENTRY
 *********************************************************************/
static
cs_status  m2r_read_from_rewrite_idx_sram(cs_uint16 port_id,
                                          cs_uint16 entry_num,
                                          m2r_rmac_rewrite_entry_t * p)
{
  m2r_rmac_gram_t       gram ;

  CS_MEMSET( (void *)&gram, 0, sizeof(m2r_rmac_gram_t) ) ;

  gram.GRAM_CR.bf.SEL    = SEL_MACREW_IDX_RAM ;
  gram.GRAM_CR.bf.GRWN   = GRWN_READ ;
  gram.GRAM_CR.bf.GRAM_A = entry_num ;

  if (m2r_rmac_read_gram_regs(port_id, &gram) == CS_ERROR) {
    return (CS_ERROR) ;
  }

  p->da_addr.byte0 = (gram.GRAM_D0.wrd & 0x00ff) ;
  p->da_addr.byte1 = ((gram.GRAM_D0.wrd >> 8 ) & 0x00ff) ;
  p->da_addr.byte2 = (gram.GRAM_D1.wrd & 0x00ff) ;
  p->da_addr.byte3 = ((gram.GRAM_D1.wrd >> 8 ) & 0x00ff) ;
  p->da_addr.byte4 = (gram.GRAM_D2.wrd & 0x00ff) ;
  p->da_addr.byte5 = ((gram.GRAM_D2.wrd >> 8 ) & 0x00ff) ;
  p->prio = (gram.GRAM_D3.wrd & 0x0007) ;
  p->ttl  =  ((gram.GRAM_D3.wrd >> 3) & 0x00ff) ;

  return (CS_OK) ;
}

/*********************************************************************
 *            WRITE TO RE-WRITE INDEX SRAM
 *********************************************************************/
static
cs_status m2r_rmac_set_hdr_field(cs_uint16 port_id,
                                 M2R_t * pDev,
                                 cs_uint16 sl_num,
                                 cs_uint16 index,
                                 m2r_rmac_rpr_hdr_field_t field,
                                 cs_uint8 field_val)
{
  volatile cs_reg            * regaddr ;
  DR_MAC_HEAD_MAC_OFFSET0_t    hdr_rw ;

  M2R_PORT_LOCK( port_id, M2R_ID_RMAC ) ;
  regaddr = &pDev->slice[sl_num].RingMAC.MAC_OFFSET0.wrd + index ;
  hdr_rw.wrd = M2R_REG_READ(regaddr) ;

  if( field == M2R_RMAC_RPR_FIELD_WE) {
    hdr_rw.bf.WE = field_val ;
  }

  if( field == M2R_RMAC_RPR_FIELD_STRICT) {
    hdr_rw.bf.STRICT_MODE = field_val ;
  }

  if( field == M2R_RMAC_RPR_FIELD_WS) {
    hdr_rw.bf.WS = field_val ;
  }

  M2R_REG_WRITE(regaddr, hdr_rw.wrd) ;
  M2R_PORT_UNLOCK( port_id, M2R_ID_RMAC ) ;
  return (CS_OK) ;
}


