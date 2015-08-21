/******************************************************************************/
/**  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        : hyphy20g_version.h
**
**  $Date:  $
**
**  $Revision:  $
**
**  DESCRIPTION : Contains driver version code definitions
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_VERSION_H
#define _HYPHY20G_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

/* please update the codes according to the new release version, */ 
/* when doing a new release */

#define MAJ_VER  0x07 /* driver major version */
#define MIN_VER  0x07 /* driver minor version */
#define RELEASE_VER  0x00 /* internal build number */

#define DRIVER_VERSION(a,b,c)           (((a) << 16) + ((b) << 8) + (c))

/* HyPHY Driver Version Code */
#define HYPHY20G_DRIVER_VERSION_CODE    DRIVER_VERSION(MAJ_VER,MIN_VER,RELEASE_VER)  

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_VERSION_H */
/* end of file */
