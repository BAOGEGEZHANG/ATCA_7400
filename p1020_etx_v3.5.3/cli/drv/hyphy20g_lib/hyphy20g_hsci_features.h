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
**  MODULE      : HyPHY 20G LSCI Subsystem Header File
**
**  FILE        :  hyphy20g_hsci.h
**
**  $Date: 2011-04-28 14:20:12 +0800 (Thu, 28 Apr 2011) $
**
**  $Revision: 11852 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_HSCI_FEATURES_H
#define _HYPHY20G_HSCI_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_pmm.h"
#include "hyphy20g_util_ecom.h"
#include "hyphy20g_hsci.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gHsciGsup43C73PrbsEnable(struct file *fileHndl, UINT4 slice,
                                     UINT4 enable);                                     

INT4 hyPhy20gHsciPattForce(struct file *fileHndl, UINT4 slice, UINT4 dir,
                           UINT4 mode);


INT4 hyPhy20gHsciByteAlignedCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 dir, UINT4 mode,
                                          UINT4 userChar);

INT4 hyPhy20gHsci8b10bCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 dir, UINT4 userChar[4],
                                    UINT4 userCharDisp);

INT4 hyPhy20gHsciFcUsrPattCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                              UINT4 userChar[4]);

INT4 hyPhy20gHsciMacGsup43BpEn(struct file *fileHndl, UINT4 slice,
                               UINT4 enable);

INT4 hyPhy20gHsci10GeTestPattInit(struct file *fileHndl, UINT4 slice,
                                  UINT4 enbl);

INT4 hyPhy20gHsci10GeR64b66bPattCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 mode);

INT4 hyPhy20gHsci10GeT64b66bPattCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 mode, UINT4 swaveLen);

INT4 hyPhy20gHsci10GeT64b66bSetSeed(struct file *fileHndl, UINT4 slice,
                                    UINT4 seedSel, UINT4 seedVal0,
                                    UINT4 seedVal1, UINT4 seedVal2,
                                    UINT4 seedVal3);

INT4 hyPhy20gHsci10GeT64b66bPattEnbl(struct file *fileHndl, UINT4 slice,
                                     UINT4 enbl);


/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HSCI_FEATURES_H */
/* end of file */
