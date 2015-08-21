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
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        :  hyPhy20g_util_ctxhdr.h
**
**  $Date: 
**
**  $Revision: 
**
**  DESCRIPTION :  This file contains all the definitions & declarations
**                 that are required by context buffer header for extraction.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_UTIL_CTXHDR_H
#define _HYPHY20G_UTIL_CTXHDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pmc_mid.h"

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/

/* encode the module id to subCtxt id */
#define PMC_CREATE_UNIQUE_ID(mid, id)               ((mid << 16) + id) 

/* top level sHYPHY20G_CTXT id */
#define HYPHY_CTXT_ID                               PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY, 0x0)

/* common scheduler subCtxt ID */
#define HYPHY_SCHD_CTXT_ID                          PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_SCHD, 0x0)
#define HYPHY_SCHD_SCBS_GRP_CTXT_ID                 PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_SCHD, 0x1)
#define HYPHY_SCHD_ONLINE_CAL_CTXT_ID               PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_SCHD, 0x2)
#define HYPHY_SCHD_OFFLINE_CAL_CTXT_ID              PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_SCHD, 0x3)

/* CPB subCtxt id */
#define HYPHY_CPB_CTXT_ID                           PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_CPB, 0x0)
#define HYPHY_CPB_WRCHNL_RDCHNLLIST_CTXT_ID         PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_CPB, 0x1)

/* LSCI subCtxt id */
#define HYPHY_LSCI_CTXT_ID                          PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_LSCI, 0x0)

/* HSCI subCtxt id */
#define HYPHY_HSCI_CTXT_ID                          PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_HSCI, 0x0)

/* PIF subCtxt id */
#define HYPHY_PIF_CTXT_ID                           PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_PIF, 0x0)
#define HYPHY_PIF_FC_CAL_CTXT_ID                    PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_PIF, 0x1)

/* GFP subCtxt id */
#define HYPHY_GFP_CTXT_ID                           PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_GFP, 0x0)
#define HYPHY_GFP_CHNL_CTXT_ID                      PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_GFP, 0x1)

/* CBRI subCtxt id */
#define HYPHY_CBRI_CTXT_ID                          PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_CBRI, 0x0)
#define HYPHY_CBRI_RD_CHNL_CTXT_ID                  PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_CBRI, 0x1)

/* TSE subCtxt id */
#define HYPHY_TSE_CTXT_ID                           PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_TSE, 0x0)

/* VCAT subCtxt id */
#define HYPHY_VCATA_CTXT_ID                         PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_VCAT, 0x0)
#define HYPHY_VCATA_MEMBER_CTXT_ID                  PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_VCAT, 0x1)
#define HYPHY_VCATB_CTXT_ID                         PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_VCAT, 0x2)

/* SERDES subCtxt id */
#define HYPHY_SERDES_CTXT_ID                        PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_SERDES, 0x0)
#define HYPHY_SERDES_OFFSET_CTXT_ID                 PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_SERDES, 0x1)

/* HostMsg subCtxt id */
#define HYPHY_HOSTMSG_CTXT_ID                       PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_HOSTMSG, 0x0)
#define HYPHY_HOSTMSG_SEQ_COUNT_CTXT_ID             PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_HOSTMSG, 0x1)

/* OTN OH Ram subCtxt id */
#define HYPHY_OTN_OH_RAM_CTXT_ID                    PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_OTN_OH_RAM, 0x0)

/* Odukpp OH Ram subCtxt id */
#define HYPHY_ODUKPP_OH_RAM_CTXT_ID                 PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_ODUKPP_OH_RAM, 0x0)

/* RTOS timer subCtxt id */
#define HYPHY_RTOS_CTXT_ID                          PMC_CREATE_UNIQUE_ID(PMC_MID_HYPHY_RTOS, 0x0)


#define HYPHY_NO_INDEX                              0

/******************************************************************************/
/* DRIVER'S INTERNAL STRUCTURES                                               */
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/* HYPHY 20G CTXT BUFFER HEADER STRUCTURES                                    */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_ctxt_hdr
{
    UINT4               len;    /* len includes the header size 
                                12 bytes + 
                                the structure size that is attached. 
                                Noted that:
                                A len=0 means end of data;
                                A len=12 means just a header;
                                and the next header is currentHdrOffset+len;
                                Also:
                                A len=12 means no valid ctxt data, and skip to do restore;
                                A len>12 means there are valid ctxt data to do restore.
                                */
    UINT4               id;     /* the context id */                                
    UINT4               index;  /* index into the context array,
                                such as 
                                cpb[0],
                                lsci[0~15],
                                cbri[0~1],
                                serdes[0~57],
                                */
} sHYPHY20G_CTXT_HDR;

/******************************************************************************/
/* FUNCTION PROTOTYPES                                                        */
/******************************************************************************/

/* calculate the context buffer header size */
UINT4 utilHyPhy20gGetCtxtHdrSize(void);

/* insert the context header into the buffer */
UINT4 utilHyPhy20gInsertCtxtHdr(void *pCtxtBuf, UINT4 len, 
                                UINT4 id, UINT4 index);

/* search the special context header in the buffer via ctxtId & index */
sHYPHY20G_CTXT_HDR *utilHyPhy20gSearchCtxtHdr(void *pCtxtBuf, UINT4 ctxtId, UINT4 index);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_UTIL_CTXHDR_H */
/* end of file */
