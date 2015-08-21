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
**  FILE        :  hyphy20g_typs.h
**
**  $Date: 2011-11-08 10:46:38 +0800 (Tue, 08 Nov 2011) $
**
**  $Revision: 16843 $
**
**  DESCRIPTION :  Contains the variable type definitions used by the
**                 HyPHY 20G Configuration Code
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_TYPS_H
#define _HYPHY20G_TYPS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _PMC_TYPES_
#define _PMC_TYPES_

typedef unsigned char       UINT1;
typedef char                INT1;
typedef unsigned short      UINT2;
typedef short               INT2;
typedef unsigned long       UINT4;
typedef long                INT4;
typedef unsigned short      BOOLEAN;
typedef char                STRING;
#ifndef PMC_HW_CODE
typedef unsigned long long  UINT8;
typedef struct timeval      TIMEVAL;
#endif

#define HYPHY20G_TRUE                               0
#define HYPHY20G_FALSE                              1

#define PRIVATE             static

#endif /* _PMC_TYPES_ */

/******************************************************************************/
/* CENTRE PACKET BUFFER SPECIFIC CONSTANTS                                    */
/******************************************************************************/

/******************************************************************************/
/* CENTRE PACKET BUFFER ENUMERATED TYPES                                      */
/******************************************************************************/


/******************************************************************************/
/* COMMONLY USED CONSTANTS                                                    */
/******************************************************************************/
#define HYPHY20G_MANU_ID                            0x0000
#define HYPHY20G_PART_TYP_HYPHY20G                  0x8240
#define HYPHY20G_LATEST_REV_HYPHY20G                0x0001
#define HYPHY20G_HYPHY20G_DEVICE                    0x5420
#define HYPHY20G_META20G_DEVICE                     0x5422
#define HYPHY20G_HYPHY10G_DEVICE                    0x5426
#define HYPHY20G_REV_ID_A                           0x0000
#define HYPHY20G_REV_ID_B                           0x0001
#define HYPHY20G_REV_ID_C                           0x0002
#define HYPHY20G_REV_ID_D                           0x0003

#define HYPHY20G_MAX_DEVS                           1
#define HYPHY20G_MAX_INIT_PROFS                     1

#define HYPHY20G_VALID_VAL_32BIT                    0x0
#define HYPHY20G_INVALID_VAL_8BIT                   0xff
#define HYPHY20G_INVALID_VAL_16BIT                  0xffff
#define HYPHY20G_INVALID_VAL_32BIT                  0xffffffff
#define HYPHY20G_VALID_SIGN                         0x5420

#define HYPHY20G_FREE                               0
#define HYPHY20G_IN_USE                             1


/******************************************************************************/
/* COMMON DEFINITIONS                                                         */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* HYPHY20G_CBACK: Application Callbacks                                      */
/*----------------------------------------------------------------------------*/
/* This is the prototype for the application callbacks                        */
/*----------------------------------------------------------------------------*/
typedef void (*HYPHY20G_CBACK)(void *usrCtxt, void *pdpv);

/*----------------------------------------------------------------------------*/
/* HYPHY20G_CTXT_MGR: Context Management Methods                              */
/*----------------------------------------------------------------------------*/
/* This is the prototype for context management methods                       */
/*----------------------------------------------------------------------------*/
typedef UINT4 (*HYPHY20G_CTXT_MGR)(void *ctxt);

/******************************************************************************/
/* COMMON ENUMERATED TYPES                                                    */
/******************************************************************************/

/* Module State Definition */
typedef enum {
    HYPHY20G_MOD_START,
    HYPHY20G_MOD_IDLE,
    HYPHY20G_MOD_READY
} eHYPHY20G_MOD_STATE;

/* Device State Definition */
typedef enum {
    HYPHY20G_START,
    HYPHY20G_PRESENT
} eHYPHY20G_DEV_STATE;

/* Polling Mode Definition*/
typedef enum {
    HYPHY20G_ISR_MODE, /* Interrupt Driven Mode */
    HYPHY20G_POLL_MODE /* Polling Mode          */
} eHYPHY20G_ISR_MODE;

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_TYPS_H */
/* end of file */
