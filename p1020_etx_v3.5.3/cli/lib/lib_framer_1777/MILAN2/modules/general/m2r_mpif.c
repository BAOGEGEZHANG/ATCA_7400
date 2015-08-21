/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_milan2_mpif.c
 * Code related to Milan2's MPIF block
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"
#include "cs_rtos.h"
#include "m2r_error.h"

#include "m2r_cb.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_mpif.h"


/* Forward declaration */
cs_status  m2r_check_mbist_status(cs_uint16 dev_id) ;
cs_status  m2r_check_sbist_status(cs_uint16 dev_id) ;


/****************************************************************/
/* $rtn_hdr_start  GET CHIP'S JTAG ID                           */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Milan2                                          */
cs_uint32  m2r_get_chip_jtag_id(cs_uint16 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : 32-bit JTAG Id (or 0 on error)                  */
/* DESCRIPTION:                                                 */
/* Returns the 32-bit JTAG Id of the chip.                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t	       		* pDev ;
  cs_uint32		chip_id ;
  cs_uint32		err_code ;

  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (0) ;         
  }

  pDev = M2R_DEV_ID_TO_DEV_BASE(dev_id) ;
  
  chip_id = ( M2R_REG_READ(&pDev->MicroIF.CHIPIdLsb.wrd) |
	      M2R_REG_READ(&pDev->MicroIF.CHIPIdMsb.wrd) << 16 ) ;
  
  return(chip_id) ;
}


/****************************************************************/
/* $rtn_hdr_start  PERFORM CHIP'S BUILT-IN-SELF-TEST(BIST)      */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */ 
/* BLOCK      : General                                         */
/* CHIP	      : Milan2                                          */
cs_status m2r_do_bist(cs_uint16 dev_id, cs_boolean mbist, 
		      cs_boolean sbist) 
/* INPUTS     : o Device Id                                     */
/*              o MBIST(or FIFO bist) enable                    */
/*              o PROD(or PROD bist) enable                     */ 
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This API is intended for Cortina INTERNAL use only!          */
/* Runs BIST for the specified memories and does self-repair    */ 
/* as needed.                                                   */
/* MBIST or FIFO BIST is the self-test for non-reparable        */           
/* memories.                                                    */
/* SBIST or PROD BIST is the self-test for reparable memories   */ 
/* and it will do a self-repair as needed.                      */
/* On chip pin reset, the hw will AUTOMATICALLY run SBIST       */
/* and do the self-repair. This API is provided only if it is   */
/* needed to run the BIST's at a later time for debug purposes. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t	       		* pDev ;
  MPIF_BISTCONTROL_t	ctl ;
  cs_uint32		err_code ;
  
  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pDev = M2R_DEV_ID_TO_DEV_BASE(dev_id) ;
  CS_PRINT("******************************************************\n") ;
  CS_PRINT("NOTE: BIST is intended for Cortina INTERNAL USE only!\n") ;
  CS_PRINT("******************************************************\n") ;
  CS_PRINT("Doing BIST and soft repair...\n") ;

  ctl.wrd = 0 ;
  if (mbist) {
    ctl.bf.START_FIFO_BIST = 1 ;
  }

  if (sbist) {
    ctl.bf.START_PROD_BIST = 1 ;
  }

  M2R_REG_WRITE(&pDev->MicroIF.BISTCONTROL.wrd, ctl.wrd) ;

  if ( m2r_wait_for_bist_done(dev_id, mbist, sbist, 50) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_check_bist_status(dev_id, mbist, sbist) == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  CS_PRINT("BIST completed successfully!\n") ;
  return (CS_OK) ;
}



cs_status  m2r_wait_for_bist_done(cs_uint16 dev_id, cs_boolean mbist,
				  cs_boolean sbist, cs_uint32 msec_timeout)
{
  cs_uint32    	msec_delay = 2, msec_elapsed = 0 ;
  cs_boolean   	done = FALSE ;

  while ( (done = m2r_is_bist_done(dev_id, mbist, sbist)) == FALSE ) {
    if (msec_elapsed >= msec_timeout) break ;
    CS_MDELAY(msec_delay) ;
    msec_elapsed += msec_delay ;
  }

  if (!done) {
    CS_HNDL_ERROR(dev_id, EM2R_BIST_NOT_DONE, NULL) ;
    CS_PRINT("\n*************************************************\n") ;
    CS_PRINT("ERROR: TIMED OUT WAITING FOR BIST DONE(%s %s)\n",
	     (mbist) ? "MBist" : "",
	     (sbist) ? "SBist" : "") ;
    CS_PRINT("\n*************************************************\n") ;
    return(CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CHECK FOR BIST DONE STATUS                   */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */ 
/* BLOCK      : General                                         */
/* CHIP	      : Milan2                                          */
cs_boolean  m2r_is_bist_done(cs_uint16 dev_id, cs_boolean mbist,
			     cs_boolean sbist) 
/* INPUTS     : o Device Id                                     */
/*              o TRUE for checking MBIST completion            */
/*              o TRUE for checking SBIST completion            */ 
/* OUTPUTS    : ----                                            */
/* RETURNS    : TRUE(if BIST done) or FALSE(if BIST not done)   */
/* DESCRIPTION:                                                 */
/* This API is intended for Cortina INTERNAL use only!          */
/* On chip pin reset, the hw will AUTOMATICALLY run STAR        */
/* and do the self-repair. This API is provided only if it is   */
/* needed to run the BIST's at a later time for debug purposes. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t	       		* pDev ;
  MPIF_BISTCONTROL_t	ctl ;
  cs_uint32		err_code ;

  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pDev = M2R_DEV_ID_TO_DEV_BASE(dev_id) ;

  ctl.wrd = M2R_REG_READ(&pDev->MicroIF.BISTCONTROL.wrd) ;
  if (mbist) {
    if (ctl.bf.START_FIFO_BIST) {
      return(FALSE) ;
    }
  }
  if (sbist) {
    if ( (ctl.bf.START_PROD_BIST) || !(ctl.bf.REPAIR_DONE) ) {
      return(FALSE) ;
    }
  }
      
  return(TRUE) ;
}


/****************************************************************/
/* $rtn_hdr_start  CHECK FOR BIST STATUS                        */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Milan2                                          */
cs_status  m2r_check_bist_status(cs_uint16 dev_id, cs_boolean mbist,
				 cs_boolean sbist)
/* INPUTS     : o Device Id                                     */
/*              o TRUE for checking MBIST completion            */
/*              o TRUE for checking SBIST completion            */ 
/* OUTPUTS    : ----                                            */
/* RETURNS    : TRUE or FALSE                                   */
/* DESCRIPTION:                                                 */
/* Check MBIST and SBIST status.                                */
/* On chip pin reset, the hw will AUTOMATICALLY run STAR BIST   */
/* (SBIST) and do the self-repair.                              */
/* NOTE: Running of mbist is intended for Cortina internal use  */ 
/* only! As such, it is recommended that the customer checks    */
/* only the status of SBIST if at all needed.                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t	       		* pDev ;
  MPIF_BISTCONTROL_t	ctl ;
  cs_uint32		err_code ;

  if ( !(M2R_IS_DEVICE_VALID(dev_id, err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pDev = M2R_DEV_ID_TO_DEV_BASE(dev_id) ;

  ctl.wrd = M2R_REG_READ(&pDev->MicroIF.BISTCONTROL.wrd) ;
  if (mbist) {
    if (ctl.bf.START_FIFO_BIST) {
      CS_HNDL_ERROR(dev_id, EM2R_BIST_NOT_DONE, ": MBIST\n") ;
      return (CS_ERROR) ;
    }      
    if ( m2r_check_mbist_status(dev_id) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (sbist) {
    if (ctl.bf.START_PROD_BIST) {
      CS_HNDL_ERROR(dev_id, EM2R_BIST_NOT_DONE, ": SBIST\n") ;
      return (CS_ERROR) ;
    }
    if ( !(ctl.bf.REPAIR_DONE) ) {
      CS_HNDL_ERROR(dev_id, EM2R_BIST_NOT_DONE, ": SBIST-Self Repair\n") ;
      return (CS_ERROR) ;
    }
    if ( m2r_check_sbist_status(dev_id) == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


#define M2R_GOOD_BIST_STATUS0	0xAAAA
#define M2R_GOOD_BIST_STATUS1	0xAAAA
#define M2R_GOOD_BIST_STATUS2	0x8000
#define M2R_GOOD_BIST_STATUS3   0x0000
/***********************
 * Check MBIST status  *
 ***********************/
cs_status  m2r_check_mbist_status(cs_uint16 dev_id) 
{
  M2R_t	       		* pDev ;
  cs_uint32		st0, st1, st2, st3 ;
  cs_status		ret = CS_OK ;

  pDev = M2R_DEV_ID_TO_DEV_BASE(dev_id) ;
  
  st0 = M2R_REG_READ(&pDev->MicroIF.BISTSTATUS0.wrd) ;
  st1 = M2R_REG_READ(&pDev->MicroIF.BISTSTATUS1.wrd) ;
  st2 = M2R_REG_READ(&pDev->MicroIF.BISTSTATUS2.wrd) ;
  st3 = M2R_REG_READ(&pDev->MicroIF.BISTSTATUS3.wrd) ;

  if (st0 != M2R_GOOD_BIST_STATUS0) {
    CS_HNDL_ERROR(dev_id, EM2R_MBIST_BAD_STATUS, 
		  ": Status-0= 0x%x (expected= 0x%x)\n",
		  st0, M2R_GOOD_BIST_STATUS0) ;
    ret = CS_ERROR ;
  }

  if (st1 != M2R_GOOD_BIST_STATUS1) {
    CS_HNDL_ERROR(dev_id, EM2R_MBIST_BAD_STATUS,
		  ": Status-1= 0x%x (expected= 0x%x)\n",
		  st1, M2R_GOOD_BIST_STATUS1) ;
    ret = CS_ERROR ;
  }

  if (st2 != M2R_GOOD_BIST_STATUS2) {
    CS_HNDL_ERROR(dev_id, EM2R_MBIST_BAD_STATUS,
		  ": Status-2= 0x%x (expected= 0x%x)\n",
		  st2, M2R_GOOD_BIST_STATUS2) ;
    ret = CS_ERROR ;
  }

  if (st3 != M2R_GOOD_BIST_STATUS3) {
    CS_HNDL_ERROR(dev_id, EM2R_MBIST_BAD_STATUS,
		  ": Status-3= 0x%x (expected= 0x%x)\n",
		  st3, M2R_GOOD_BIST_STATUS3) ;
    ret = CS_ERROR ;
  }

  return (ret) ;
}

#define M2R_GOOD_BIST_STATUS4	0x8000
/***********************
 * Check SBIST status  *
 ***********************/
cs_status  m2r_check_sbist_status(cs_uint16 dev_id) 
{
  M2R_t	       		* pDev ;
  cs_uint32		st4 ;

  pDev = M2R_DEV_ID_TO_DEV_BASE(dev_id) ;
  
  st4 = M2R_REG_READ(&pDev->MicroIF.BISTSTATUS4.wrd) ;
  
  if (st4 != M2R_GOOD_BIST_STATUS4) {
    CS_HNDL_ERROR(dev_id, EM2R_SBIST_BAD_STATUS,
		  ": Status-4= 0x%x (expected= 0x%x)\n",
		  st4, M2R_GOOD_BIST_STATUS4) ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  PORT HARD RESET                              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */ 
/* BLOCK      : General                                         */
/* CHIP	      : Milan2                                          */
cs_status  m2r_port_hard_reset(cs_uint16 port_id, m2r_module_id_t mid,
			       m2r_reset_action_t act) 
/* INPUTS     : o Port Id                                       */	
/*              o Module Id                                     */	
/*              o Reset control                                 */	
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Hard reset control of the port.                              */ 
/*                                                              */
/* The [mid] parameter indicates the block selection and is     */ 
/* one of the following:                                        */ 
/* M2R_ID_SPI42,                      M2R_ID_PPRBS              */
/* M2R_ID_FRAMER,                     M2R_ID_RMAC               */
/* M2R_ID_XGXS,                       M2R_ID_LIF                */
/* M2R_ID_XETH,     or                M2R_ID_ALL_BLOCKS         */	
/*                                                              */
/* The [act] parameter specifies what kind of reset to be done  */ 
/* and is one of the following:                                 */
/* M2R_RESET_DEASSERT, M2R_RESET_ASSERT, or                     */
/* M2R_RESET_TOGGLE.                                            */
/* The M2R_RESET_TOGGLE option will assert reset, hold the      */
/* reset for a while and then de-assert the reset bit(s).       */
/*                                                              */
/* NOTE: The MPIF block CANNOT be hard-reset by software - it   */
/* gets reset only on power-on!                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t	       		* pDev ;
  MPIF_GlobalReset_t    rst, sel ;
  cs_uint32		err_code ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if (mid & ~M2R_ID_ALL_BLOCKS) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_HRESET_SEL, ": mid= 0x%x\n",
		  mid) ;
    return(CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  M2R_PORT_LOCK( port_id, M2R_ID_MPIF ) ;
  
  /*
   * Since MPIF cannot be reset, disable the associated block-level
   * interrupts also in the global int enable register.
   */
  if ( m2r_port_disable_blk_irq(port_id, M2R_ID_ALL_BLOCKS)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }
  
  sel.wrd = 0 ;
  if (mid & M2R_ID_SPI42)  sel.bf.SPI4P2HReset  = 1 ;
  if (mid & M2R_ID_PPRBS)  sel.bf.PPRBSHReset   = 1 ;
  if (mid & M2R_ID_FRAMER) sel.bf.FramerHReset  = 1 ;  
  if (mid & M2R_ID_RMAC)   sel.bf.RingMacHReset = 1 ;
  if (mid & M2R_ID_XGXS)   sel.bf.XgxsHReset    = 1 ;
  if (mid & M2R_ID_LIF)    sel.bf.LifHReset     = 1 ;
  if (mid & M2R_ID_ETH)    sel.bf.XETHHReset    = 1 ;

  rst.wrd = M2R_REG_READ(&pDev->MicroIF.GlobalReset.wrd) ;
  switch (act) {
    case M2R_RESET_DEASSERT : 
      /* clear the hard reset bit(s) */
      rst.wrd &= ~(sel.wrd) ;
      M2R_REG_WRITE(&pDev->MicroIF.GlobalReset.wrd, rst.wrd) ;
      break ;

    case M2R_RESET_ASSERT :
      /* set the hard reset bit(s) */
      rst.wrd |= sel.wrd ;
      M2R_REG_WRITE(&pDev->MicroIF.GlobalReset.wrd, rst.wrd) ;
      break ;

    case M2R_RESET_TOGGLE :
      /* set, wait for a few msecs, and clear hard reset bit(s) */
      rst.wrd |= sel.wrd ;
      M2R_REG_WRITE(&pDev->MicroIF.GlobalReset.wrd, rst.wrd) ;

#ifndef RELEASE_PLATFORM
      if (IN_SIM_ENV()) {
        m2r_sim_hreset_delay( M2R_PORT_ID_TO_DEV_ID(port_id), 
	                      M2R_RESET_MSEC_DELAY) ;
      } else {
        CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
      }
#else 
      CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
#endif

      rst.wrd &= ~sel.wrd ;
      M2R_REG_WRITE(&pDev->MicroIF.GlobalReset.wrd, rst.wrd) ;
      break ;
  }

#ifndef RELEASE_PLATFORM
  if (IN_SIM_ENV()) {
    m2r_sim_hreset_delay( M2R_PORT_ID_TO_DEV_ID(port_id),
                          M2R_RESET_MSEC_DELAY) ;
  } else {
    CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
  }
#else
  CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
#endif
  
  M2R_PORT_UNLOCK( port_id, M2R_ID_MPIF ) ;
  
  return (CS_OK) ;
}


#define CS_PALERMO_JTAG_ID     0x300073E5
/****************************************************
 * Enable/Disable MAC bypass in MPIF mode register  *
 ****************************************************/
cs_status  m2r_mpif_ctl_bypass_mode(cs_uint16 port_id, m2r_ctl_t ctl)
{
  M2R_t         * pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  MPIF_Mode_t	mdreg ;

  if ( (m2r_get_chip_jtag_id(port_id) == CS_PALERMO_JTAG_ID) &&
       (ctl != CS_ENABLE) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ":only FRAMER mode(MAC bypass) supported on Palermo!\n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK( port_id, M2R_ID_MPIF ) ;
  mdreg.wrd = M2R_REG_READ(&pDev->MicroIF.Mode.wrd) ;
  mdreg.bf.RingMac_Bypass_Ovride = 1 ;	/* not valid for Palermo */
  mdreg.bf.RingMac_Bypass = ( (ctl == M2R_ENABLE) ? 1 : 0 ) ;
  M2R_REG_WRITE(&pDev->MicroIF.Mode.wrd, mdreg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_MPIF ) ;
  return (CS_OK) ;
}


/****************************************************
 * Get port MAC mode from MPIF global mode register *
 ****************************************************/
cs_boolean m2r_mpif_is_in_mac_bypass(cs_uint16 port_id) 
{
  M2R_t         * pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  MPIF_Mode_t	mdreg ;

  mdreg.wrd = M2R_REG_READ(&pDev->MicroIF.Mode.wrd) ;

  if (mdreg.bf.RingMac_Bypass_Ovride) {
    if (mdreg.bf.RingMac_Bypass) {
      return (TRUE) ;
    } else {
      return (FALSE) ; 
    }
  }

  if (mdreg.bf.FRMD) {
    return (TRUE) ;
  }

  return (FALSE) ;
}

/*******************************************************
 * Enable/Disable ethernet mode in MPIF mode register  *
 ******************************************************/
cs_status  m2r_mpif_ctl_eth_mode(cs_uint16 port_id, m2r_ctl_t ctl)
{
  M2R_t         * pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  MPIF_Mode_t	mdreg ;

  M2R_PORT_LOCK( port_id, M2R_ID_MPIF ) ;

  mdreg.wrd = M2R_REG_READ(&pDev->MicroIF.Mode.wrd) ;

  mdreg.bf.XGEMD_Ovride = 1 ;
  mdreg.bf.XGEMD_Ovride_val = ( (ctl == M2R_ENABLE) ? 1 : 0 ) ;
  M2R_REG_WRITE(&pDev->MicroIF.Mode.wrd, mdreg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_MPIF ) ;
  return (CS_OK) ;
}

/****************************************************
 * Get port XETH mode from MPIF global mode register *
 ****************************************************/
cs_boolean m2r_mpif_is_in_eth_mode(cs_uint16 port_id) 
{
  M2R_t         * pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  MPIF_Mode_t	mdreg ;

  mdreg.wrd = M2R_REG_READ(&pDev->MicroIF.Mode.wrd) ;

  if (mdreg.bf.XGEMD_Ovride) {
    if (mdreg.bf.XGEMD_Ovride_val) {
      return (TRUE) ;
    } else {
      return (FALSE) ;
    }
  }

  if (mdreg.bf.XGEMD) {
    return (TRUE) ;
  }

  return (FALSE) ;
}

/****************************************************
 * Enable/Disable oc48 mode  in MPIF mode register  *
 ****************************************************/
cs_status  m2r_mpif_ctl_oc48_mode(cs_uint16 port_id, m2r_ctl_t ctl)
{
  M2R_t         * pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  MPIF_Mode_t	mdreg ;

  M2R_PORT_LOCK( port_id, M2R_ID_MPIF ) ;

  mdreg.wrd = M2R_REG_READ(&pDev->MicroIF.Mode.wrd) ;

  mdreg.bf.OC48MD_Ovride = 1 ;
  mdreg.bf.OC48MD_Ovride_val = ( (ctl == M2R_ENABLE) ? 1 : 0 ) ;
  M2R_REG_WRITE(&pDev->MicroIF.Mode.wrd, mdreg.wrd) ;

  M2R_PORT_UNLOCK( port_id, M2R_ID_MPIF ) ;
  return (CS_OK) ;
}

/****************************************************
 * Get port XETH mode from MPIF global mode register *
 ****************************************************/
cs_boolean m2r_mpif_is_in_oc48_mode(cs_uint16 port_id) 
{
  M2R_t         * pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  MPIF_Mode_t	mdreg ;

  mdreg.wrd = M2R_REG_READ(&pDev->MicroIF.Mode.wrd) ;

  if (mdreg.bf.OC48MD_Ovride) {
    if (mdreg.bf.OC48MD_Ovride_val) {
      return (TRUE) ;
    } else {
      return (FALSE) ;
    }
  }

  if (mdreg.bf.OC48MD) {
    return (TRUE) ;
  }

  return (FALSE) ;
}



