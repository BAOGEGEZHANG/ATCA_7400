/******************************************************************************/
/**  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  MODULE      : PMC MID Configuration Code
**
**  FILE        :  pmc_mid.h
**
**  $Date: 
**
**  $Revision: 
**
**  DESCRIPTION :  This file contains general Module ID definitions.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _PMC_MID_H
#define _PMC_MID_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/

/* general Module ID's for logging, tagging, error codes */
#define PMC_MID_BASE                                (0x1000)
#define PMC_MID_HYPHY                               (PMC_MID_BASE + 0x0)
#define PMC_MID_HYPHY_CPB                           (PMC_MID_BASE + 0x01)
#define PMC_MID_HYPHY_LSCI                          (PMC_MID_BASE + 0x02)
#define PMC_MID_HYPHY_HSCI                          (PMC_MID_BASE + 0x03)
#define PMC_MID_HYPHY_PIF                           (PMC_MID_BASE + 0x04)
#define PMC_MID_HYPHY_GFP                           (PMC_MID_BASE + 0x05)
#define PMC_MID_HYPHY_CBRI                          (PMC_MID_BASE + 0x06)
#define PMC_MID_HYPHY_TSE                           (PMC_MID_BASE + 0x07)
#define PMC_MID_HYPHY_VCAT                          (PMC_MID_BASE + 0x08)
#define PMC_MID_HYPHY_SERDES                        (PMC_MID_BASE + 0x09)
#define PMC_MID_HYPHY_HOSTMSG                       (PMC_MID_BASE + 0x0A)
#define PMC_MID_HYPHY_OTN_OH_RAM                    (PMC_MID_BASE + 0x0B)
#define PMC_MID_HYPHY_ODUKPP_OH_RAM                 (PMC_MID_BASE + 0x0C)
#define PMC_MID_HYPHY_RTOS                          (PMC_MID_BASE + 0x0D)

/* common Module ID */
#define PMC_MID_HYPHY_SCHD                          (PMC_MID_BASE + 0x0E)

#ifdef __cplusplus
}
#endif

#endif /* _PMC_MID_H */
/* end of file */
