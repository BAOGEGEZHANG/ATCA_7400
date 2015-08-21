/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_mpif.c
 * Code related to MPIF block
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_registers.h"
#include "arn5_error.h"
#include "arn5_cb.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_mpif_api.h"
#include "arn5_ver.h"


/* Forward declaration */
cs_status  arn5_check_mbist_status(cs_uint8 dev_id) ;
cs_status  arn5_check_sbist_status(cs_uint8 dev_id) ;

/****************************************************************/
/* $rtn_hdr_start  GET CHIP'S JTAG ID                           */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_uint32  arn5_get_chip_jtag_id(cs_uint8 dev_id)
/* INPUTS     : o Device Id                                     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : 32-bit JTAG Id (or 0 on error)                  */
/* DESCRIPTION:                                                 */
/* Returns the 32-bit JTAG Id of the chip.                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint32     chip_id ;
  cs_uint32     err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (0) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  chip_id = ( CS_REG_READ(&pDev->MicroIF.CHIPIdLsb.wrd) |
          CS_REG_READ(&pDev->MicroIF.CHIPIdMsb.wrd) << 16 ) ;
  return(chip_id) ;
}


/****************************************************************/
/* $rtn_hdr_start  PERFORM CHIP'S BUILT-IN-SELF-TEST(BIST)      */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_do_bist(cs_uint8 dev_id, cs_boolean mbist,
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
  ARN5_t                * pDev ;
  ARN5_MPIF_BISTCONTROL_t  ctl ;
  cs_uint32             err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  CS_PRINT("******************************************************\n") ;
  CS_PRINT("NOTE: BIST is intended for Cortina INTERNAL USE only!\n") ;
  CS_PRINT("******************************************************\n") ;
  CS_PRINT("Doing BIST and soft repair(as needed)...\n") ;

  ctl.wrd = 0 ;
  if (mbist) ctl.bf.START_FIFO_BIST = 1 ;
  if (sbist) ctl.bf.START_PROD_BIST = 1 ;
  CS_REG_WRITE(&pDev->MicroIF.BISTCONTROL.wrd, ctl.wrd) ;

  if ( arn5_wait_for_bist_done(dev_id, mbist, sbist, 50) != CS_OK ) {
    return (CS_ERROR) ;
  }

  if (arn5_check_bist_status(dev_id, mbist, sbist) != CS_OK) {
    return (CS_ERROR) ;
  }

  CS_PRINT("\nBIST completed successfully!\n") ;
  return (CS_OK) ;
}


/************************************************
 * Poll for BIST completion                     *
 ************************************************/
cs_status  arn5_wait_for_bist_done(cs_uint8 dev_id, cs_boolean mbist,
                        cs_boolean sbist, cs_uint32 msec_timeout)
{
  cs_uint32  msec_delay = 2, msec_elapsed = 0 ;
  cs_boolean done = FALSE ;

  while ( (done = arn5_is_bist_done(dev_id, mbist, sbist)) == FALSE ) {
    if (msec_elapsed >= msec_timeout) break ;
    ARN5_MDELAY(dev_id, msec_delay) ;
    msec_elapsed += msec_delay ;
  }

  if (!done) {
    CS_HNDL_ERROR(dev_id, EARN5_BIST_NOT_DONE, NULL) ;
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
/* CHIP       : Arsenal5                                        */
cs_boolean  arn5_is_bist_done(cs_uint8 dev_id, cs_boolean mbist,
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
  ARN5_t                * pDev ;
  ARN5_MPIF_BISTCONTROL_t  ctl ;
  cs_uint32             err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ctl.wrd = CS_REG_READ(&pDev->MicroIF.BISTCONTROL.wrd) ;
  if (mbist) {
    if (ctl.bf.START_FIFO_BIST) return(FALSE) ;
  }
  if (sbist) {
    if ( (ctl.bf.START_PROD_BIST) || !(ctl.bf.REPAIR_DONE) ) {
      return (FALSE) ;
    }
  }

  return (TRUE) ;
}


/****************************************************************/
/* $rtn_hdr_start  CHECK FOR BIST STATUS                        */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_check_bist_status(cs_uint8 dev_id, cs_boolean mbist,
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
  ARN5_t                * pDev ;
  ARN5_MPIF_BISTCONTROL_t  ctl ;
  cs_uint32             err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ctl.wrd = CS_REG_READ(&pDev->MicroIF.BISTCONTROL.wrd) ;
  if (mbist) {
    if (ctl.bf.START_FIFO_BIST) {
      CS_HNDL_ERROR(dev_id, EARN5_BIST_NOT_DONE, ": MBIST\n") ;
      return (CS_ERROR) ;
    }
    if (arn5_check_mbist_status(dev_id) != CS_OK) {
      return (CS_ERROR) ;
    }
  }

  if (sbist) {
    if (ctl.bf.START_PROD_BIST) {
      CS_HNDL_ERROR(dev_id, EARN5_BIST_NOT_DONE, ":SBIST\n") ;
      return (CS_ERROR) ;
    }
    if ( !(ctl.bf.REPAIR_DONE) ) {
      CS_HNDL_ERROR(dev_id, EARN5_BIST_NOT_DONE,
            ": SBIST Self Repair\n") ;
      return (CS_ERROR) ;
    }
    if (arn5_check_sbist_status(dev_id) != CS_OK) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}

#define ARN5_GOOD_BIST_STATUS0   0xAAAA
#define ARN5_GOOD_BIST_STATUS1   0xAAAA
#define ARN5_GOOD_BIST_STATUS3   0xAAAA
#define ARN5_GOOD_BIST_STATUS4   0x0002

/***********************
 * Check MBIST status  *
 ***********************/
cs_status  arn5_check_mbist_status(cs_uint8 dev_id)
{
  ARN5_t                * pDev ;
  ARN5_MPIF_BISTSTATUS0_t status0 ;
  ARN5_MPIF_BISTSTATUS1_t status1 ;
  ARN5_MPIF_BISTSTATUS3_t status3 ;
  ARN5_MPIF_BISTSTATUS4_t status4 ;
  cs_status             ret = CS_OK ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  status0.wrd = CS_REG_READ(&pDev->MicroIF.BISTSTATUS0.wrd) ;
  if (status0.wrd != ARN5_GOOD_BIST_STATUS0) {
    CS_HNDL_ERROR(dev_id, EARN5_MBIST_BAD_STATUS,
          ": Status-0= 0x%x (expected= 0x%x)\n",
          status0.wrd, ARN5_GOOD_BIST_STATUS0) ;
    if (status0.bf.FRAMER_RX_0 != 2) {
      CS_PRINT("\tMBIST Error: Framer-0 Rx memory\n") ;
    }
    if (status0.bf.FRAMER_RX_1 != 2) {
      CS_PRINT("\tMBIST Error: Framer-1 Rx memory\n") ;
    }
    if (status0.bf.FRAMER_RX_2 != 2) {
      CS_PRINT("\tMBIST Error: Framer-2 Rx memory\n") ;
    }
    if (status0.bf.FRAMER_RX_3 != 2) {
      CS_PRINT("\tMBIST Error: Framer-3 Rx memory\n") ;
    }
    if (status0.bf.FRAMER_RX_4 != 2) {
      CS_PRINT("\tMBIST Error: Framer-4 Rx memory\n") ;
    }
    if (status0.bf.FRAMER_RX_5 != 2) {
      CS_PRINT("\tMBIST Error: Framer-5 Rx memory\n") ;
    }
    if (status0.bf.FRAMER_RX_6 != 2) {
      CS_PRINT("\tMBIST Error: Framer-6 Rx memory\n") ;
    }
    if (status0.bf.FRAMER_RX_7 != 2) {
      CS_PRINT("\tMBIST Error: Framer-7 Rx memory\n") ;
    }
    CS_PRINT("\n") ;
    ret = CS_ERROR ;
  }

  status1.wrd = CS_REG_READ(&pDev->MicroIF.BISTSTATUS1.wrd) ;
  if (status1.wrd != ARN5_GOOD_BIST_STATUS1) {
    CS_HNDL_ERROR(dev_id, EARN5_MBIST_BAD_STATUS,
          ": Status-1= 0x%x (expected= 0x%x)\n",
          status1.wrd, ARN5_GOOD_BIST_STATUS1) ;
    if (status1.bf.FRAMER_TX_0 != 2) {
      CS_PRINT("\tMBIST Error: Framer-0 Tx memory\n") ;
    }
    if (status1.bf.FRAMER_TX_1 != 2) {
      CS_PRINT("\tMBIST Error: Framer-1 Tx memory\n") ;
    }
    if (status1.bf.FRAMER_TX_2 != 2) {
      CS_PRINT("\tMBIST Error: Framer-2 Tx memory\n") ;
    }
    if (status1.bf.FRAMER_TX_3 != 2) {
      CS_PRINT("\tMBIST Error: Framer-3 Tx memory\n") ;
    }
    if (status1.bf.FRAMER_TX_4 != 2) {
      CS_PRINT("\tMBIST Error: Framer-4 Tx memory\n") ;
    }
    if (status1.bf.FRAMER_TX_5 != 2) {
      CS_PRINT("\tMBIST Error: Framer-5 Tx memory\n") ;
    }
    if (status1.bf.FRAMER_TX_6 != 2) {
      CS_PRINT("\tMBIST Error: Framer-6 Tx memory\n") ;
    }
    if (status1.bf.FRAMER_TX_7 != 2) {
      CS_PRINT("\tMBIST Error: Framer-7 Tx memory\n") ;
    }
    CS_PRINT("\n") ;
    ret = CS_ERROR ;
  }

  status3.wrd = CS_REG_READ(&pDev->MicroIF.BISTSTATUS3.wrd) ;
  if (status3.wrd != ARN5_GOOD_BIST_STATUS3) {
    CS_HNDL_ERROR(dev_id, EARN5_MBIST_BAD_STATUS,
          ": Status-3= 0x%x (expected= 0x%x)\n",
          status3.wrd, ARN5_GOOD_BIST_STATUS3) ;
    if (status3.bf.GE_TX_0 != 2) {
      CS_PRINT("\tMBIST Error: GigE-0 Tx memory\n") ;
    }
    if (status3.bf.GE_TX_1 != 2) {
      CS_PRINT("\tMBIST Error: GigE-1 Tx memory\n") ;
    }
    if (status3.bf.GE_TX_2 != 2) {
      CS_PRINT("\tMBIST Error: GigE-2 Tx memory\n") ;
    }
    if (status3.bf.GE_TX_3 != 2) {
      CS_PRINT("\tMBIST Error: GigE-3 Tx memory\n") ;
    }
    if (status3.bf.GE_TX_4 != 2) {
      CS_PRINT("\tMBIST Error: GigE-4 Tx memory\n") ;
    }
    if (status3.bf.GE_TX_5 != 2) {
      CS_PRINT("\tMBIST Error: GigE-5 Tx memory\n") ;
    }
    if (status3.bf.GE_TX_6 != 2) {
      CS_PRINT("\tMBIST Error: GigE-6 Tx memory\n") ;
    }
    if (status3.bf.GE_TX_7 != 2) {
      CS_PRINT("\tMBIST Error: GigE-7 Tx memory\n") ;
    }
    CS_PRINT("\n") ;
    ret = CS_ERROR ;
  }

  status4.wrd = CS_REG_READ(&pDev->MicroIF.BISTSTATUS4.wrd) ;
  if (status4.wrd != ARN5_GOOD_BIST_STATUS4) {
    CS_HNDL_ERROR(dev_id, EARN5_MBIST_BAD_STATUS,
          ": Status-4= 0x%x (expected= 0x%x)\n",
          status4.wrd, ARN5_GOOD_BIST_STATUS4) ;
    if (status4.bf.BRIDGE_RX != 2) {
      CS_PRINT("\tMBIST Error: Bridge Rx memory\n") ;
    }
    CS_PRINT("\n") ;
    ret = CS_ERROR ;
  }

  return (ret) ;
}


#define ARN5_GOOD_BIST_STATUS5   0x8000
/***********************
 * Check SBIST status  *
 ***********************/
cs_status  arn5_check_sbist_status(cs_uint8 dev_id)
{
  ARN5_t                * pDev ;
  ARN5_MPIF_BISTSTATUS5_t status5 ;

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  status5.wrd = CS_REG_READ(&pDev->MicroIF.BISTSTATUS5.wrd) ;
  if (status5.wrd != ARN5_GOOD_BIST_STATUS5) {
    CS_HNDL_ERROR(dev_id, EARN5_SBIST_BAD_STATUS,
          ": Status-5= 0x%x (expected= 0x%x)\n",
          status5.wrd, ARN5_GOOD_BIST_STATUS5) ;
    if (status5.bf.STAR_BIST_BRIDGE) {
      CS_PRINT("\tSBIST Error: Bridge memory\n") ;
    }
    CS_PRINT("\n") ;
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DEVICE HARD RESET                            */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_dev_hard_reset(cs_uint8 dev_id, arn5_module_id_t mid,
                        cs_reset_action_t act)
/* INPUTS     : o Device Id                                     */
/*              o Module Id                                     */
/*              o Reset control                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Hard reset control of the device(chip).                      */
/*                                                              */
/* The [mid] parameter indicates the block selection and is     */
/* one of the following:                                        */
/* ARN5_ID_HOST_IF   or   ARN5_ID_FRAMER   or    ARN5_ID_LIF    */
/* ARN5_ID_ETH       or   ARN5_ID_PPRBS    or    (ARN5_ID_MPIF) */
/* ARN5_ID_ALL_BLOCKS.                                          */
/* All slices of the specified block(s) will be reset, wherever */
/* the slice concept is applicable.                             */
/*                                                              */
/* The [act] parameter specifies what kind of reset to be done  */
/* and is one of the following:                                 */
/* CS_RESET_DEASSERT, CS_RESET_ASSERT, or                       */
/* CS_RESET_TOGGLE.                                             */
/* The CS_RESET_TOGGLE option will assert reset, hold the       */
/* reset for a while and then de-assert the reset bit(s).       */
/*                                                              */
/* NOTE: The MPIF block technically CANNOT be hard-reset by the */
/* chip. But some SW clean-up (e.g. put GPIO to default config, */
/* power down all slices, etc.) will be done when MPIF block is */
/* specified included in the [mid] parameter.                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                  * pDev ;
  ARN5_MPIF_GlobalReset0_t rst0 ;
  ARN5_MPIF_GlobalReset1_t rst1 ;
  ARN5_MPIF_GlobalReset2_t rst2 ;
  cs_uint16               glbl_enbl_reg, chan_enbl_reg , i;
  cs_uint16               bits_0 = 0, bits_1 = 0, bits_2 = 0 ;
  cs_uint16               glbl_enbl_bits = 0, chan_enbl_bits = 0 ;
  cs_uint32               err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if (mid & ~ARN5_ID_ALL_BLOCKS) {
    CS_HNDL_ERROR(dev_id, EARN5_INVALID_HRESET_SEL, ": mid= 0x%x\n",
                  mid) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  /*
   * Disable the block interrupts also as MPIF block cannot
   * be reset.
   */
  if (act == CS_RESET_ASSERT || act == CS_RESET_TOGGLE) {
    if (mid & ARN5_ID_HOST_IF) glbl_enbl_bits |= 0x0100 ;
    if (mid & ARN5_ID_MPIF)    glbl_enbl_bits |= 0x0600 ;
    if (mid & ARN5_ID_ETH)     chan_enbl_bits |= 0x0008 ;
    if (mid & ARN5_ID_PPRBS)   chan_enbl_bits |= 0x0004 ;
    if (mid & ARN5_ID_LIF)     chan_enbl_bits |= 0x0002 ;
    if (mid & ARN5_ID_FRAMER)  chan_enbl_bits |= 0x0001 ;

    if (mid & ARN5_ID_MPIF) {
      /*
       * Do some SW "reset" to reset some MPIF registers to *
       * the default values.
       */
      CS_REG_WRITE(&pDev->MicroIF.ChannelMode.wrd,
        ARN5_MPIF_ChannelMode_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.SONETInterface.wrd,
        ARN5_MPIF_SONETInterface_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIODirection0.wrd,
         ARN5_MPIF_GPIODirection0_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIODirection1.wrd,
         ARN5_MPIF_GPIODirection1_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIOAlarmControl.wrd,
         ARN5_MPIF_GPIOAlarmControl_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIOFmroControl.wrd,
         ARN5_MPIF_GPIOFmroControl_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIOOutput0.wrd,
         ARN5_MPIF_GPIOOutput0_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIOOutput1.wrd,
         ARN5_MPIF_GPIOOutput1_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIORMask0.wrd,
         ARN5_MPIF_GPIORMask0_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIORMask1.wrd,
         ARN5_MPIF_GPIORMask1_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIOFMask0.wrd,
         ARN5_MPIF_GPIOFMask0_dft) ;
      CS_REG_WRITE(&pDev->MicroIF.GPIOFMask1.wrd,
         ARN5_MPIF_GPIOFMask1_dft) ;

      for (i = 0 ; i < ARN5_MAX_NUM_PORTS; i++) {
        if (g_ARN5_ver[i] == 0) {
          arn5_slice_ctl_pwr_down(dev_id, i, CS_ENABLE) ;
        }
      }
    }
  }

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;

  if (glbl_enbl_bits) {
    glbl_enbl_reg = CS_REG_READ(&pDev->MicroIF.GlobalIntEnable.wrd) ;
    glbl_enbl_reg &= ~glbl_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.GlobalIntEnable.wrd,
         glbl_enbl_reg) ;
  }

  if (chan_enbl_bits) {
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel0IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel0IntEnable.wrd,
         chan_enbl_reg) ;
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel1IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel1IntEnable.wrd,
         chan_enbl_reg) ;
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel2IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel2IntEnable.wrd,
         chan_enbl_reg) ;
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel3IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel3IntEnable.wrd,
         chan_enbl_reg) ;
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel4IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel4IntEnable.wrd,
         chan_enbl_reg) ;
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel5IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel5IntEnable.wrd,
         chan_enbl_reg) ;
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel6IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel6IntEnable.wrd,
         chan_enbl_reg) ;
    chan_enbl_reg = CS_REG_READ(&pDev->MicroIF.Channel7IntEnable.wrd) ;
    chan_enbl_reg &= ~chan_enbl_bits ;
    CS_REG_WRITE(&pDev->MicroIF.Channel7IntEnable.wrd,
         chan_enbl_reg) ;
  }

  if (mid & ARN5_ID_LIF)     bits_0 |= 0xff00 ; /* all slices */
  if (mid & ARN5_ID_FRAMER)  bits_0 |= 0x00ff ;
  if (mid & ARN5_ID_HOST_IF) bits_1 |= 0x0100 ;
  if (mid & ARN5_ID_ETH)     bits_1 |= 0x00ff ;
  if (mid & ARN5_ID_PPRBS)   bits_2 |= 0x00ff ;

  if (bits_0) {
    rst0.wrd = CS_REG_READ(&pDev->MicroIF.GlobalReset0.wrd) ;
  }

  if (bits_1) {
    rst1.wrd = CS_REG_READ(&pDev->MicroIF.GlobalReset1.wrd) ;
  }

  if (bits_2) {
    rst2.wrd = CS_REG_READ(&pDev->MicroIF.GlobalReset2.wrd) ;
  }

  switch (act) {
    case CS_RESET_DEASSERT:
      if (bits_0) {
        rst0.wrd &= ~bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd &= ~bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd &= ~bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }
      break ;

    case CS_RESET_ASSERT :
      if (bits_0) {
        rst0.wrd |= bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd |= bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd |= bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }
      break ;

    case CS_RESET_TOGGLE :
      if (bits_0) {
        rst0.wrd |= bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd |= bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd |= bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }

      ARN5_MDELAY(dev_id, ARN5_RESET_MSEC_DELAY) ;

      if (bits_0) {
        rst0.wrd &= ~bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd &= ~bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd &= ~bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }
      break ;

    default :
      break ;
  }

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;

  if (act != CS_RESET_ASSERT) {
    ARN5_MDELAY (dev_id, ARN5_RESET_MSEC_DELAY) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  PORT HARD RESET                              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_port_hard_reset(cs_uint16 port_id,
                        arn5_module_id_t mid,
                        cs_reset_action_t act)
/* INPUTS     : o Device Id                                     */
/*              o Module Id                                     */
/*              o Reset control                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Hard reset control of the slce.                              */
/*                                                              */
/* The [mid] parameter indicates the block selection and is     */
/* one of the following:                                        */
/* ARN5_ID_FRAMER, ARN5_ID_LIF, ARN5_ID_ETH, ARN5_ID_PPRBS or   */
/* ARN5_ID_FRAMER | ARN5_ID_LIF | ARN5_ID_ETH | ARN5_ID_PPRBS   */
/*                                                              */
/* The [act] parameter specifies what kind of reset to be done  */
/* and is one of the following:                                 */
/* CS_RESET_DEASSERT, CS_RESET_ASSERT, or                       */
/* CS_RESET_TOGGLE.                                             */
/* The CS_RESET_TOGGLE option will assert reset, hold the       */
/* reset for a while and then de-assert the reset bit(s).       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32    err_code ;
  cs_uint8     dev_id ;
  cs_uint16    sl_num ;

  dev_id = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  if ( !(ARN5_IS_PORT_VALID(port_id, &err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return (CS_ERROR) ;
  }

  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  if ( arn5_slice_hard_reset(dev_id, sl_num, mid, act) != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SLICE POWER DOWN                             */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_slice_ctl_pwr_down(cs_uint8 dev_id, cs_uint8 slice_num,
                        cs_ctl_t ctl)
/* INPUTS     : o Device Id                                     */
/*              o Slice number [0..7]                           */
/*              o CS_ENABLE(power down) or CS_DISABLE(power up) */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Power down a slice.                                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                * pDev ;
  ARN5_MPIF_PowerdownControl_t pdreg ;
  cs_uint32             err_code ;
  cs_uint16             bit ;
  arn5_dev_cb_t         * pDevCb ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if (slice_num > 7) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        " : slice_num(%d) s/b < 8\n", slice_num) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  bit = (ctl == CS_ENABLE) ? 1 : 0 ;
  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;
  pdreg.wrd = CS_REG_READ(&pDev->MicroIF.PowerdownControl.wrd) ;
  switch (slice_num) {
    case 0 : pdreg.bf.PwrDwn_0 = bit ;  break ;
    case 1 : pdreg.bf.PwrDwn_1 = bit ;  break ;
    case 2 : pdreg.bf.PwrDwn_2 = bit ;  break ;
    case 3 : pdreg.bf.PwrDwn_3 = bit ;  break ;
    case 4 : pdreg.bf.PwrDwn_4 = bit ;  break ;
    case 5 : pdreg.bf.PwrDwn_5 = bit ;  break ;
    case 6 : pdreg.bf.PwrDwn_6 = bit ;  break ;
    case 7 : pdreg.bf.PwrDwn_7 = bit ;  break ;
  }

  CS_REG_WRITE(&pDev->MicroIF.PowerdownControl.wrd, pdreg.wrd) ;
  pDevCb = ARN5_DEV_ID_TO_DEVCB_PTR(dev_id) ;
  pDevCb->slice_enbl = pdreg.wrd & 0xff ;

  /* allow at least 100 us for the slice to come up */
  if ((ctl != CS_ENABLE) && (!CS_IN_SIM_ENV())) {
    ARN5_MDELAY (dev_id, 1) ;
  }

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SLICE POWER DOWN QUERY                       */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_boolean  arn5_is_slice_pwr_down(cs_uint8 dev_id,
                        cs_uint8 slice_num)
/* INPUTS     : o Device Id                                     */
/*              o Slice number [0..7]                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : TRUE (powered down) or FALSE                    */
/* DESCRIPTION:                                                 */
/* Get a slice's power down status                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                * pDev ;
  volatile cs_reg       pdCtl ;
  cs_uint32             err_code ;
  cs_boolean            ret = FALSE ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(ret) ;
  }

  if (slice_num > 7) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        " : slice_num(%d) s/b < 8\n", slice_num) ;
    return(ret) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  pdCtl = CS_REG_READ(&pDev->MicroIF.PowerdownControl.wrd) ;
  ret = ((pdCtl & (1 << slice_num)) != 0) ? TRUE  : FALSE ;

  return (ret) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONFIGURE DIAG(FMRO) BUS                     */
/* CATEGORY   : Device                                          */
/* ACCESS     : private                                         */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status arn5_mpif_cfg_gpio_diag_bus(cs_uint8 dev_id,
                        arn5_diag_bus_t diag_bus)
/* INPUTS     : o Device Id                                     */
/*              o Diag(FMRO) bus selection                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This configures the diagnostic(FMRO) bus to be output on the */
/* GPIO pins.                                                   */
/*                                                              */
/* The [diag_bus] parameter is specified to be one of the       */
/* following -                                                  */
/* ARN5_DIAG_BUS_NONE ,               ARN5_DIAG_BUS_BRIDE ,     */
/* ARN5_DIAG_BUS_FRAMER_0 ,           ARN5_DIAG_BUS_FRAMER_1 ,  */
/* ARN5_DIAG_BUS_FRAMER_2 ,           ARN5_DIAG_BUS_FRAMER_3 ,  */
/* ARN5_DIAG_BUS_FRAMER_4 ,           ARN5_DIAG_BUS_FRAMER_5 ,  */
/* ARN5_DIAG_BUS_FRAMER_6 ,           ARN5_DIAG_BUS_FRAMER_7 ,  */
/* ARN5_DIAG_BUS_GIGE_0 ,             ARN5_DIAG_BUS_GIGE_1 ,    */
/* ARN5_DIAG_BUS_GIGE_2 ,             ARN5_DIAG_BUS_GIGE_3 ,    */
/* ARN5_DIAG_BUS_GIGE_4 ,             ARN5_DIAG_BUS_GIGE_5 ,    */
/* ARN5_DIAG_BUS_GIGE_6 ,             ARN5_DIAG_BUS_GIGE_7      */
/*                                                              */
/* These GPIO pins are shared between general-purpose I/O(GPIO) */
/* framer alarm/GigE status and diagnostic functionalities.     */
/*                                                              */
/* The API, arn5_mpif_cfg_gpio_io() configures the pins to be   */
/* output or input gpio pins.                                   */
/* The API, arn5_mpif_cfg_gpio_alm_status() may enable the pins */
/* GPIO[0..7] as framer alarm/GigE status outputs.              */
/* The API, arn5_mpif_cfg_gpio_debug_bus() will overwrite both  */
/* conf.                                                        */
/*                                                              */
/* This API is provided for lab debug only!                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t               * pDev ;
  ARN5_MPIF_GPIOFmroControl_t   fmroCtl ;
  cs_uint32            err_code ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;
  fmroCtl.wrd = CS_REG_READ(&pDev->MicroIF.GPIOFmroControl.wrd) ;

  switch (diag_bus) {
    default :
    case ARN5_DIAG_BUS_NONE:
      fmroCtl.bf.FMRO_E = 0 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_0:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr0 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_1:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr1 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_2:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr2 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_3:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr3 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_4:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr4 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_5:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr5 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_6:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr6 ;
      break ;

    case ARN5_DIAG_BUS_FRAMER_7:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Fr7 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_0:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge0 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_1:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge1 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_2:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge2 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_3:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge3 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_4:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge4 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_5:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge5 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_6:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge6 ;
      break ;

    case ARN5_DIAG_BUS_GIGE_7:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_FrGe ;
      fmroCtl.bf.FMRO_FRSEL  = ARN5_FMRO_FRSEL_Ge7 ;
      break ;

    case ARN5_DIAG_BUS_BRIDGE:
      fmroCtl.bf.FMRO_E      = 1 ;
      fmroCtl.bf.FMRO_SEL    = ARN5_FMRO_SEL_Br;
      break ;
  }

  CS_REG_WRITE(&pDev->MicroIF.GPIOFmroControl.wrd, fmroCtl.wrd) ;

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONFIGURE ALAMR/STATUS ON GPIO PINS          */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_mpif_cfg_gpio_alm_status(cs_uint8 dev_id,
                        cs_uint8 alm_status_map,
                        cs_uint8 mask)
/* INPUTS     : o Device Id                                     */
/*              o 8-bit Enable Map (1=alarm/status, 0=gpio)     */
/*              o 8-bit Mask (0=don't change, 1=change config)  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This configures the Framer alarm(s) or GigE status on to     */
/* GPIO[0..7] pins.  One bit per corresponding Framer/GigE slice*/
/*                                                              */
/* These GPIO pins are shared between general-purpose I/O(GPIO) */
/* framer alarm/GigE status and diagnostic functionalities.     */
/*                                                              */
/* The API, arn5_mpif_cfg_gpio_io() configures the GPIO pins to */
/* be output or input gpio pins.  Make sure all interested pins */
/* in these two configurations are mutually exclusive.          */
/*                                                              */
/* The API, arn5_mpif_cfg_gpio_debug_bus() will overwrite this  */
/* conf.                                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint32     err_code ;
  cs_uint16     almCtl, output0 ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;

  almCtl = CS_REG_READ(&pDev->MicroIF.GPIOAlarmControl.wrd) ;
  output0 = CS_REG_READ(&pDev->MicroIF.GPIOOutput0.wrd) ;
  almCtl = (almCtl & ~((cs_uint16)mask)) | (alm_status_map & mask) ;
  output0 = output0 | (alm_status_map & mask) ;

  CS_REG_WRITE(&pDev->MicroIF.GPIOAlarmControl.wrd, almCtl) ;
  CS_REG_WRITE(&pDev->MicroIF.GPIOOutput0.wrd, output0) ;

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONFIGURE GPIO PINS                          */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_mpif_cfg_gpio_io(cs_uint8 dev_id, cs_uint16 io_map,
                        cs_uint16 mask)
/* INPUTS     : o Device Id                                     */
/*              o 16-bit I/O map( 1=output, 0=input)            */
/*              o 16-bit Mask (0=don't change, 1=change config) */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This configures the GPIO pins on the chip. There are 16 of   */
/* them and they can be configured to be either input or output */
/* pins.                                                        */
/*                                                              */
/* The API, arn5_mpif_cfg_gpio_alm_status() may enable the pins */
/* GPIO[0..7] as framer alarm/GigE status outputs. Make sure all*/
/* interested pins in these two configurations are mutually     */
/* exclusive.                                                   */
/*                                                              */
/* The API, arn5_mpif_cfg_debug_bus() will overwrite this conf. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint32     err_code ;
  cs_uint16     direction, myMask, myMap ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;

  direction = CS_REG_READ(&pDev->MicroIF.GPIODirection0.wrd) ;
  myMask = mask & 0x00ff ;
  myMap = (io_map & 0x00ff) & myMask ;
  direction = (direction & ~myMask) | myMap ;
  CS_REG_WRITE(&pDev->MicroIF.GPIODirection0.wrd, direction) ;

  direction = CS_REG_READ(&pDev->MicroIF.GPIODirection1.wrd) ;
  myMask = (mask & 0xff00) >> 8 ;
  myMap = ((io_map & 0xff00) >> 8) & myMask ;
  direction = (direction & ~myMask) | myMap ;
  CS_REG_WRITE(&pDev->MicroIF.GPIODirection1.wrd, direction) ;

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  READ GPIO INPUTS                             */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_uint16  arn5_mpif_read_gpio_input(cs_uint8 dev_id, cs_uint16 mask)
/* INPUTS     : o Device Id                                     */
/*              o Bit-mask (0 - mask off, 1 - input pin)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : 16-bit gpio sample(input) data                  */
/* DESCRIPTION:                                                 */
/* This reads(samples) the GPIO input pins. The [mask] parameter*/
/* specifies which input pins to be read from (the others are   */
/* masked out and the value returned has 0's for those bits.    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint32     err_code ;
  cs_uint16     data ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(0) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  data = CS_REG_READ(&pDev->MicroIF.GPIOSample0.wrd) & 0x00ff ;
  data = data |
         ((CS_REG_READ(&pDev->MicroIF.GPIOSample1.wrd) & 0x00ff)
           << 8) ;
  return ( data & mask ) ;
}


/****************************************************************/
/* $rtn_hdr_start  WRITE GPIO OUTPUTS                           */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_mpif_write_gpio_output(cs_uint8 dev_id,
                        cs_uint16 data, cs_uint16 mask)
/* INPUTS     : o Device Id                                     */
/*              o Data to be output on the pins                 */
/*              o Bit-mask (0 - don't change, 1 - change output)*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Writes a 1 or 0 to the GPIO output pin. The data is bit-map  */
/* corresponding to the 16 GPIO pins.                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint32     err_code ;
  cs_uint16     output, myMask, myMap ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;

  output = CS_REG_READ(&pDev->MicroIF.GPIOOutput0.wrd) ;
  myMask = mask & 0x00ff ;
  myMap = (data & 0x00ff) & myMask ;
  output = (output & ~myMask) | myMap ;
  CS_REG_WRITE(&pDev->MicroIF.GPIOOutput0.wrd, output) ;

  output = CS_REG_READ(&pDev->MicroIF.GPIOOutput1.wrd) ;
  myMask = (mask & 0xff00) >> 8 ;
  myMap = ((data & 0xff00) >> 8) & myMask ;
  output = (output & ~myMask) | myMap ;
  CS_REG_WRITE(&pDev->MicroIF.GPIOOutput1.wrd, output) ;

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  CONFIGURE SMH PORT                           */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : General                                         */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_mpif_cfg_smh_port(cs_uint8 dev_id,
                        arn5_smh_port_t smh_port,
                        cs_uint8 slice_num,
                        arn5_smh_data_type_sel_t type,
                        cs_dir_t dir)
/* INPUTS     : o Device Id                                     */
/*              o SMH port, ARN5_SMH_PORT_0 or ARN5_SMH_PORT_1  */
/*              o Slice where overhead bytes is to or from      */
/*              o Data Type                                     */
/*              o CS_RX , CS_TX or CS_RX_AND_TX                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure Serial Multiplexed overHead port (only two ports to*/
/* output to or input from).                                    */
/*                                                              */
/* [type] :  ARN5_SMH_DATA_TYPE_NONE (disable the port),        */
/*           ARN5_SMH_DATA_TYPE_TOH or ARN5_SMH_DATA_TYPE_DCC   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                 * pDev ;
  cs_uint32              err_code ;
  ARN5_MPIF_SMHControl0_t ctl ;  /* same ctl data struct for port 1 */
  volatile cs_reg        * pReg ;

  if ( !(ARN5_IS_DEVICE_VALID(dev_id, &err_code)) ) {
    CS_HNDL_ERROR(dev_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if (slice_num > 7) {
    CS_HNDL_ERROR(dev_id, EARN5_DEV_INVALID_USER_ARG,
        " : slice_num(%d) s/b < 8\n", slice_num) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;

  if (smh_port == ARN5_SMH_PORT_0) {
    pReg = &pDev->MicroIF.SMHControl0.wrd ;
  }
  else {
    pReg = &pDev->MicroIF.SMHControl1.wrd ;
  }

  ctl.wrd = CS_REG_READ(pReg) ;

  if ((dir == CS_RX) || (dir == CS_RX_AND_TX))
  {
    switch (type) {
      default :
      case ARN5_SMH_DATA_TYPE_NONE :
        ctl.bf.RxEn = 0 ;
        break;

      case ARN5_SMH_DATA_TYPE_TOH :
        ctl.bf.RxEn = 1 ;
        ctl.bf.RxType = 0 ;
        break ;

      case ARN5_SMH_DATA_TYPE_DCC :
        ctl.bf.RxEn = 1 ;
        ctl.bf.RxType = 1 ;
        break ;
    }
    ctl.bf.RxChID = slice_num ;
  }

  if ((dir == CS_TX) || (dir == CS_RX_AND_TX))
  {
    switch (type) {
      default :
      case ARN5_SMH_DATA_TYPE_NONE :
        ctl.bf.TxEn = 0 ;
        break;

      case ARN5_SMH_DATA_TYPE_TOH :
        ctl.bf.TxEn = 1 ;
        ctl.bf.TxType = 0 ;
        break ;

      case ARN5_SMH_DATA_TYPE_DCC :
        ctl.bf.TxEn = 1 ;
        ctl.bf.TxType = 1 ;
        break ;
    }
    ctl.bf.TxChID = slice_num ;
  }

  CS_REG_WRITE(pReg, ctl.wrd) ;

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;
  return(CS_OK) ;
}


/*****************************************************************
 *****************************************************************
 **                  Internal functions and utilities           **
 *****************************************************************
 *****************************************************************/

/****************************************************************/
/* Set the line rate for a port.                                */
/*                                                              */
/* [line_rate] :                                                */
/*   ARN5_PORT_RATE_OC48C ,   ARN5_PORT_RATE_OC12C              */
/*   ARN5_PORT_RATE_OC3C  or  ARN5_PORT_RATE_GIGE               */
/****************************************************************/
cs_status  arn5_mpif_set_line_rate(cs_uint16 port_id,
                        arn5_port_line_rate_t line_rate)
{
  ARN5_t        * pDev ;
  cs_uint32     err_code ;
  cs_uint8      devId, slNum ;
  cs_uint16     chMode ;
  cs_status     ret = CS_OK ;

  if ( !ARN5_IS_PORT_VALID(port_id, &err_code) ) {
    CS_HNDL_ERROR(port_id, err_code, " :port_id= %d",
                  port_id) ;
    return(CS_ERROR) ;
  }

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  slNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;

  switch (line_rate) {
    case ARN5_PORT_RATE_OC12C :
      if ((g_ARN5_ver[slNum] & 0x4) == 0) {
        CS_HNDL_ERROR(devId, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as OC12C.\n", slNum) ;
        ret = CS_ERROR ;
      }
      break ;

    case ARN5_PORT_RATE_OC3C :
      if ((g_ARN5_ver[slNum] & 0x2) == 0) {
        CS_HNDL_ERROR(devId, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as OC3C.\n", slNum) ;
        ret = CS_ERROR ;
      }
      break ;

    case ARN5_PORT_RATE_GIGE :
      if ((g_ARN5_ver[slNum] & 0x1) == 0) {
        CS_HNDL_ERROR(devId, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as GigE.\n", slNum) ;
        ret = CS_ERROR ;
      }
      break ;

    default :
      if ((g_ARN5_ver[slNum] & 0x8) == 0) {
        CS_HNDL_ERROR(devId, EARN5_DEV_CAPACITY_EXCEEDED ,
          "\nThe slice %d can't be configured as OC48C.\n", slNum) ;
        ret = CS_ERROR ;
      }
      break ;
  }

  if (ret != CS_OK) return (CS_ERROR) ;

  ARN5_DEV_LOCK(devId, ARN5_ID_MPIF) ;

  chMode = CS_REG_READ(&pDev->MicroIF.ChannelMode.wrd) ;
  if (line_rate != ARN5_PORT_RATE_GIGE) {
     cs_uint16 sonetInf, mask ;

     chMode &= ~((cs_uint16)(1 << slNum)) ;

     sonetInf = CS_REG_READ(&pDev->MicroIF.SONETInterface.wrd) ;
     mask = 0x3 ;
     mask <<= slNum * 2 ;
     sonetInf = sonetInf & (~mask) ;
     if (line_rate == ARN5_PORT_RATE_OC48C) {
        sonetInf |= ((cs_uint16)ARN5_Ch0SIType_OC48) << (slNum * 2) ;
     }
     else if (line_rate == ARN5_PORT_RATE_OC12C) {
        sonetInf |= ((cs_uint16)ARN5_Ch0SIType_OC12) << (slNum * 2) ;
     }
     else {
        sonetInf |= ((cs_uint16)ARN5_Ch0SIType_OC3) << (slNum * 2) ;
     }
     CS_REG_WRITE(&pDev->MicroIF.SONETInterface.wrd, sonetInf) ;
  }
  else {
    chMode |= (cs_uint16)(1 << slNum) ;
  }

  CS_REG_WRITE(&pDev->MicroIF.ChannelMode.wrd, chMode) ;

  ARN5_DEV_UNLOCK(devId, ARN5_ID_MPIF) ;
  return(CS_OK) ;
}


/****************************************************
 * Hard reset the specified slice                   *
 ****************************************************/
cs_status  arn5_slice_hard_reset(cs_uint8 dev_id, cs_uint8 slice_num,
                        arn5_module_id_t mid,
                        cs_reset_action_t act)
{
  ARN5_t        * pDev ;
  ARN5_MPIF_GlobalReset0_t  rst0 ;
  ARN5_MPIF_GlobalReset1_t  rst1 ;
  ARN5_MPIF_GlobalReset2_t  rst2 ;
  volatile cs_reg           * pReg ;
  cs_uint16     chan_enbl_bits = 0 ;
  cs_uint16     chan_enbl;
  cs_uint16     bits_0 = 0, bits_1 = 0, bits_2 = 0;

  if ( !(mid &
        (ARN5_ID_LIF | ARN5_ID_PPRBS | ARN5_ID_ETH | ARN5_ID_FRAMER)) ) {
    CS_HNDL_ERROR(dev_id, EARN5_INVALID_HRESET_SEL, ": mid= 0x%x\n",
          mid) ;
    return(CS_ERROR) ;
  }
  pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  if ((act == CS_RESET_ASSERT) || (act == CS_RESET_TOGGLE)) {
    if (mid & ARN5_ID_ETH)     chan_enbl_bits |= 0x0008 ;
    if (mid & ARN5_ID_PPRBS)   chan_enbl_bits |= 0x0004 ;
    if (mid & ARN5_ID_LIF)     chan_enbl_bits |= 0x0002 ;
    if (mid & ARN5_ID_FRAMER)  chan_enbl_bits |= 0x0001 ;
  }

  ARN5_DEV_LOCK(dev_id, ARN5_ID_MPIF) ;

  if (chan_enbl_bits) {
    pReg = (cs_reg *)&pDev->MicroIF.Channel0IntEnable.wrd + (slice_num * 2) ;
    chan_enbl = CS_REG_READ(pReg) ;
    chan_enbl &= ~chan_enbl_bits ;
    CS_REG_WRITE(pReg, chan_enbl) ;
  }

  if (mid & ARN5_ID_LIF)     bits_0 |= 1 << (slice_num + 8);
  if (mid & ARN5_ID_FRAMER)  bits_0 |= 1 << slice_num ;
  if (mid & ARN5_ID_ETH)     bits_1 |= 1 << slice_num ;
  if (mid & ARN5_ID_PPRBS)   bits_2 |= 1 << slice_num ;

  if (bits_0) {
    rst0.wrd = CS_REG_READ(&pDev->MicroIF.GlobalReset0.wrd) ;
  }

  if (bits_1) {
    rst1.wrd = CS_REG_READ(&pDev->MicroIF.GlobalReset1.wrd) ;
  }

  if (bits_2) {
    rst2.wrd = CS_REG_READ(&pDev->MicroIF.GlobalReset2.wrd) ;
  }

  switch (act) {
    case CS_RESET_DEASSERT:
      if (bits_0) {
        rst0.wrd &= ~bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd &= ~bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd &= ~bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }
      break ;

    case CS_RESET_ASSERT :
      if (bits_0) {
        rst0.wrd |= bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd |= bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd |= bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }
      break ;

    case CS_RESET_TOGGLE :
      if (bits_0) {
        rst0.wrd |= bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd |= bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd |= bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }

      ARN5_MDELAY(dev_id, ARN5_RESET_MSEC_DELAY) ;

      if (bits_0) {
        rst0.wrd &= ~bits_0 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset0.wrd, rst0.wrd) ;
      }
      if (bits_1) {
        rst1.wrd &= ~bits_1 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset1.wrd, rst1.wrd) ;
      }
      if (bits_2) {
        rst2.wrd &= ~bits_2 ;
        CS_REG_WRITE(&pDev->MicroIF.GlobalReset2.wrd, rst2.wrd) ;
      }
      break ;

    default :
      break ;
  }

  ARN5_DEV_UNLOCK(dev_id, ARN5_ID_MPIF) ;

  if (act != CS_RESET_ASSERT) {
    ARN5_MDELAY (dev_id, ARN5_RESET_MSEC_DELAY) ;
  }

  return (CS_OK) ;
}


/****************************************************
 * Check if the port is in gige mode                *
 ****************************************************/
cs_boolean  arn5_mpif_is_in_gige_mode(cs_uint16 port_id)
{
  cs_uint16     slNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  ARN5_t        * pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  cs_uint16     chMode, mask ;

  chMode = CS_REG_READ(&pDev->MicroIF.ChannelMode.wrd) ;
  mask = ((cs_uint16) 1) << slNum ;
  return ((chMode & mask) != 0) ;
}


/****************************************************
 * Check if the port is in OC48 mode                *
 ****************************************************/
cs_boolean  arn5_mpif_is_in_oc48_mode(cs_uint16 port_id)
{
  cs_uint16     slNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  ARN5_t        * pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  cs_uint16     chMode, sonetInf, mask ;

  chMode = CS_REG_READ(&pDev->MicroIF.ChannelMode.wrd) ;
  mask = 1 << slNum ;
  if ((chMode & mask) != 0) return FALSE ;

  sonetInf = CS_REG_READ(&pDev->MicroIF.SONETInterface.wrd) ;
  mask = 3 << (slNum * 2) ;
  return (((sonetInf & mask) >> (slNum * 2)) == ARN5_Ch0SIType_OC48) ;
}


/****************************************************
 * Check if the port is in OC12 mode                *
 ****************************************************/
cs_boolean  arn5_mpif_is_in_oc12_mode(cs_uint16 port_id)
{
  cs_uint16     slNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  ARN5_t        * pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  cs_uint16     chMode, sonetInf, mask ;

  chMode = CS_REG_READ(&pDev->MicroIF.ChannelMode.wrd) ;
  mask = 1 << slNum ;
  if ((chMode & mask) != 0) return FALSE ;

  sonetInf = CS_REG_READ(&pDev->MicroIF.SONETInterface.wrd) ;
  mask = 3 << (slNum * 2) ;
  return (((sonetInf & mask) >> (slNum * 2)) == ARN5_Ch0SIType_OC12) ;
}


/****************************************************
 * Check if the port is in OC3 mode                 *
 ****************************************************/
cs_boolean  arn5_mpif_is_in_oc3_mode(cs_uint16 port_id)
{
  cs_uint16     slNum = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  ARN5_t        * pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  cs_uint16     chMode, sonetInf, mask ;

  chMode = CS_REG_READ(&pDev->MicroIF.ChannelMode.wrd) ;
  mask = 1 << slNum ;
  if ((chMode & mask) != 0) return FALSE ;

  sonetInf = CS_REG_READ(&pDev->MicroIF.SONETInterface.wrd) ;
  mask = 3 << (slNum * 2) ;
  return (((sonetInf & mask) >> (slNum * 2)) == ARN5_Ch0SIType_OC3) ;
}


