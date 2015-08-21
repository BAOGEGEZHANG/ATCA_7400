/******************************************************************************/
/**  COPYRIGHT (C) 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
/**--------------------------------------------------------------------------**/
/** This software embodies materials and concepts which are proprietary and  **/
/** confidential to PMC-Sierra, Inc. PMC-Sierra distributes this software    **/
/** to its customers pursuant to the terms and conditions of the Software    **/
/** License Agreement contained in the text file software.lic that is        **/
/** distributed along with the software. This software can only be utilized  **/
/** if all terms and conditions of the Software License Agreement are        **/
/** accepted. If there are any questions, concerns, or if the                **/
/** Software License Agreement text file, software.lic, is missing please    **/
/** contact PMC-Sierra for assistance.                                       **/
/**--------------------------------------------------------------------------**/
/**                                                                          **/
/******************************************************************************/
/*******************************************************************************
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        :  hyphy20g_err.h
**
**  $Date: 2011-02-12 03:57:34 +0800 (Sat, 12 Feb 2011) $
**
**  $Revision: 10864 $
**
**  DESCRIPTION :  Defines all the error codes returned by the HyPHY 20G driver
**
**  NOTES       :  The user should modify HYPHY20G_ERR_BASE so that the error
**                 code values do not overlap those of any other module
**
*******************************************************************************/


#ifndef _HYPHY20G_ERR_H
#define _HYPHY20G_ERR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"

/* modify this so that HyPHY 20G error codes do not overlap other error codes */
#define HYPHY20G_ERR_BASE    (-600)

/* do NOT modify anything below this line !!! */
#define HYPHY20G_SUCCESS     0

#define HYPHY20G_FAILURE                                (HYPHY20G_ERR_BASE - 00)
#define HYPHY20G_ERR_MEM_ALLOC                          (HYPHY20G_ERR_BASE - 01)
#define HYPHY20G_ERR_INVALID_ARG                        (HYPHY20G_ERR_BASE - 02)
#define HYPHY20G_ERR_NULL_PTR_PASSED                    (HYPHY20G_ERR_BASE - 03)
#define HYPHY20G_ERR_CTXT_ALREADY_VALID                 (HYPHY20G_ERR_BASE - 04)
#define HYPHY20G_ERR_NOT_VALID_DATA                     (HYPHY20G_ERR_BASE - 05)

#define HYPHY20G_ERR_INVALID_MODULE_STATE               (HYPHY20G_ERR_BASE - 10)
#define HYPHY20G_ERR_INVALID_MIV                        (HYPHY20G_ERR_BASE - 11)
#define HYPHY20G_ERR_PROFILES_FULL                      (HYPHY20G_ERR_BASE - 12)
#define HYPHY20G_ERR_INVALID_PROFILE                    (HYPHY20G_ERR_BASE - 13)
#define HYPHY20G_ERR_INVALID_PROFILE_NUM                (HYPHY20G_ERR_BASE - 14)
#define HYPHY20G_ERR_INT_INSTALL                        (HYPHY20G_ERR_BASE - 15)
#define HYPHY20G_ERR_BUF_START                          (HYPHY20G_ERR_BASE - 16)
#define HYPHY20G_ERR_MODULE_NOT_INIT                    (HYPHY20G_ERR_BASE - 17)

#define HYPHY20G_ERR_INVALID_DEVICE_STATE               (HYPHY20G_ERR_BASE - 20)
#define HYPHY20G_ERR_DEVS_FULL                          (HYPHY20G_ERR_BASE - 21)
#define HYPHY20G_ERR_DEV_ALREADY_ADDED                  (HYPHY20G_ERR_BASE - 22)
#define HYPHY20G_ERR_INVALID_DEV                        (HYPHY20G_ERR_BASE - 23)
#define HYPHY20G_ERR_INVALID_DIV                        (HYPHY20G_ERR_BASE - 24)
#define HYPHY20G_ERR_INVALID_MODE                       (HYPHY20G_ERR_BASE - 25)

#define HYPHY20G_ERR_INVALID_REG                        (HYPHY20G_ERR_BASE - 30)
#define HYPHY20G_ERR_POLL_TIMEOUT                       (HYPHY20G_ERR_BASE - 31)
#define HYPHY20G_ERR_MUTEX_AQUIRE                       (HYPHY20G_ERR_BASE - 32)
#define HYPHY20G_ERR_REQ_BUSY                           (HYPHY20G_ERR_BASE - 33)

#define HYPHY20G_ERR_DEV_ACCESS                         (HYPHY20G_ERR_BASE - 40)
#define HYPHY20G_ERR_HW_FAULT                           (HYPHY20G_ERR_BASE - 42)
#define HYPHY20G_ERR_INVALID_PRECONFIG                  (HYPHY20G_ERR_BASE - 43)
#define HYPHY20G_ERR_NON_TERMINATION                    (HYPHY20G_ERR_BASE - 44)
#define HYPHY20G_ERR_TIU                                (HYPHY20G_ERR_BASE - 45)
#define HYPHY20G_ERR_INVALID_TARGET                     (HYPHY20G_ERR_BASE - 46)
#define HYPHY20G_ERR_CSU_LOCK                           (HYPHY20G_ERR_BASE - 50)
#define HYPHY20G_ERR_TIP_TIMEOUT                        (HYPHY20G_ERR_BASE - 51)
#define HYPHY20G_ERR_PMON_FAULT                         (HYPHY20G_ERR_BASE - 52)
#define HYPHY20G_ERR_ZERO_DIVIDE                        (HYPHY20G_ERR_BASE - 53)
#define HYPHY20G_ERR_VAR_OVERFLOW                       (HYPHY20G_ERR_BASE - 54)

/* Driver Context Restoration Error Code */
#define HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED      (HYPHY20G_ERR_BASE - 60)
#define HYPHY20G_ERR_DEV_CTXT_CORRUPTED                 (HYPHY20G_ERR_BASE - 61)

/* FW Error Code */
#define HYPHY20G_ERR_FW_INVALID_BOOT_STATE             (HYPHY20G_ERR_BASE - 101)
#define HYPHY20G_ERR_FW_BOOT_FAIL                      (HYPHY20G_ERR_BASE - 102)

/* HOSTMSG Error Code */
#define HYPHY20G_ERR_STATUS_CHECK_MISMATCH             (HYPHY20G_ERR_BASE - 201)
#define HYPHY20G_ERR_INBOUND_MAILBOX_BUSY              (HYPHY20G_ERR_BASE - 202)
#define HYPHY20G_ERR_SEQNUM_MISMATCH                   (HYPHY20G_ERR_BASE - 203)
#define HYPHY20G_ERR_RSP_CFG                           (HYPHY20G_ERR_BASE - 204)
#define HYPHY20G_ERR_RSP_REQ                           (HYPHY20G_ERR_BASE - 205)
#define HYPHY20G_ERR_RSP_NACK                          (HYPHY20G_ERR_BASE - 206)
#define HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT     (HYPHY20G_ERR_BASE - 207)

/* VCAT Subsystem Error Code */
#define HYPHY20G_DDR_BIST_DATA_FAILURE                (HYPHY20G_ERR_BASE - 301)
#define HYPHY20G_DDR_BIST_ADDR_FAILURE                (HYPHY20G_ERR_BASE - 302)
#define HYPHY20G_DDR_BIST_ADDR_AND_DATA_FAILURE       (HYPHY20G_ERR_BASE - 303)
#define HYPHY20G_DDR_BIST_TIMEOUT                     (HYPHY20G_ERR_BASE - 304)

/* CPB Subsystem Error Code */
#define HYPHY20G_ERR_CHNL_FIFO                         (HYPHY20G_ERR_BASE - 501)
#define HYPHY20G_ERR_NO_PRIMARY_RD                     (HYPHY20G_ERR_BASE - 502)
#define HYPHY20G_ERR_INVALID_STATE                     (HYPHY20G_ERR_BASE - 503)
#define HYPHY20G_ERR_INVALID_WRCHNL_CTXT               (HYPHY20G_ERR_BASE - 504)
#define HYPHY20G_ERR_INVALID_RDCHNL_CTXT               (HYPHY20G_ERR_BASE - 505)
#define HYPHY20G_ERR_EVALUATING_SCHD                   (HYPHY20G_ERR_BASE - 506)
#define HYPHY20G_ERR_SCHD_CAL_SIZE                     (HYPHY20G_ERR_BASE - 507)
#define HYPHY20G_ERR_QDR_TEST_FAILURE                  (HYPHY20G_ERR_BASE - 508)
#define HYPHY20G_ERR_SCHD_BW                           (HYPHY20G_ERR_BASE - 509)
#define HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG          (HYPHY20G_ERR_BASE - 510)
#define HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG          (HYPHY20G_ERR_BASE - 511)
#define HYPHY20G_ERR_SCHD_ALGO_FAIL                    (HYPHY20G_ERR_BASE - 512)
#define HYPHY20G_ERR_ADD_BLOCK                         (HYPHY20G_ERR_BASE - 513)
#define HYPHY20G_ERR_DEL_BLOCK                         (HYPHY20G_ERR_BASE - 514)
#define HYPHY20G_ERR_INVALID_GFP_CHNL_MODES            (HYPHY20G_ERR_BASE - 550)
#define HYPHY20G_ERR_TOO_MANY_CHANNEL                  (HYPHY20G_ERR_BASE - 551)

/* SW Error Code */
#define HYPHY20G_ERR_NO_SW_CONTENT                     (HYPHY20G_ERR_BASE - 601)

/* TSE Subsystem Error Code */
#define HYPHY20G_ERR_INVALID_MASTER                    (HYPHY20G_ERR_BASE - 701)

/* SERDES Error Code */
#define HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK         (HYPHY20G_ERR_BASE - 801)
#define HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED     (HYPHY20G_ERR_BASE - 802)
#define HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS        (HYPHY20G_ERR_BASE - 803)
#define HYPHY20G_ERR_SERDES_ADSM_LOCK                  (HYPHY20G_ERR_BASE - 804)
#define HYPHY20G_ERR_SERDES_CSU_INACTIVE               (HYPHY20G_ERR_BASE - 805)
#define HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL       (HYPHY20G_ERR_BASE - 806)
#define HYPHY20G_ERR_SERDES_ADC_INVALID_EQ             (HYPHY20G_ERR_BASE - 807)
#define HYPHY20G_ERR_SERDES_ADC_FACTORY_LOAD           (HYPHY20G_ERR_BASE - 808)

/* COMPATIBLITY Error Code */
#define HYPHY20G_ERR_INCOMPATIBLE_DEVICE               (HYPHY20G_ERR_BASE - 901)

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ERR_H */
/* end of file */
