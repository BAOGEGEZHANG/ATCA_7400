/******************************************************************************/
/**  COPYRIGHT (C) 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
/**--------------------------------------------------------------------------**/
/** This software embodies materials and concepts which are proprietary and  **/
/** confidential to PMC-Sierra, Inc.                                         **/
/** PMC-Sierra distributes this software to its customers pursuant to the    **/
/** terms and conditions of the Device Driver Software License Agreement     **/
/** contained in the text file software.lic that is distributed along with   **/
/** the device driver software. This software can only be utilized if all    **/
/** terms and conditions of the Device Driver Software License Agreement are **/
/** accepted. If there are any questions, concerns, or if the Device Driver  **/
/** Software License Agreement text file, software.lic, is missing please    **/
/** contact PMC-Sierra for assistance.                                       **/
/**--------------------------------------------------------------------------**/
/**                                                                          **/
/******************************************************************************/
/*******************************************************************************
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        : hyphy20g.h
**
**  $Date: 2011-03-02 18:08:37 +0800 (Wed, 02 Mar 2011) $
**
**  $Revision: 11174 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_H
#define _HYPHY20G_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g_version.h"
#include "hyphy20g_rtos.h"
#include "hyphy20g_typs.h"
#include "hyphy20g_err.h"
#include "hyphy20g_defs.h"
#include "hyphy20g_idefs.h"
#include "hyphy20g_util_ctxhdr.h"

/*
#include "hyphy20g_strs.h"
*/
#ifndef PMC_HW_CODE
#include    <linux/delay.h>
#include    <linux/sched.h>
#include    <linux/time.h>
#endif /* PMC_HW_CODE */
#include "hyphy20g_hw.h"

#define HYPHY20G_DELAY_5USEC		5
#define HYPHY20G_DELAY_10USEC		10
#define HYPHY20G_DELAY_20USEC		20
#define HYPHY20G_DELAY_1250USEC		1250
#define HYPHY20G_DELAY_100MSEC		100000
#define HYPHY20G_DELAY_1MINUTE		60000000


#define RWB_READ        1
#define RWB_WRITE       0

#define REQ_BUSY        1


#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_H */
/* end of file */
