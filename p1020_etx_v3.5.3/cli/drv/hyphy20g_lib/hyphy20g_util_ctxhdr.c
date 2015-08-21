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
**
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        : hyphy20g_util_ctxhdr.c
**
**  $Date: 
**
**  $Revision: 
**
**  DESCRIPTION : Contains the HyPHY 20G utility functions.
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g.h"

/******************************************************************************/
/* PRIVATE FUNCTIONS - CTXT header Operations called from APIs                */
/******************************************************************************/

/*******************************************************************************
**
**  utilHyPhy20gGetCtxtHdrSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the maximal size of context header.
**
**  INPUTS:         None
**
**  OUTPUTS:        None
**
**  RETURNS:        The maximal size of context header
**
*******************************************************************************/
UINT4 utilHyPhy20gGetCtxtHdrSize(void)
{
    return (sizeof(sHYPHY20G_CTXT_HDR));
} /* utilHyPhy20gGetCtxtHdrSize */

/*******************************************************************************
**
**  utilHyPhy20gInsertCtxtHdr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Insert the context header into pre-allocated buffer.
**
**
**  INPUTS:         pCtxtBuf - pre-allocated buffer for driver context
**                  extraction
**                  len - include the header size and the structure 
**                  size that is attached.
**                  id - context id
**                  index - index of the context array
**
**  OUTPUTS:        None
**
**  RETURNS:        The length has been inserted to the buffer
**
*******************************************************************************/
UINT4 utilHyPhy20gInsertCtxtHdr(void *pCtxtBuf, UINT4 len, 
                                UINT4 id, UINT4 index)
{
    sHYPHY20G_CTXT_HDR ctxtBufHdr;
    UINT4 hdrLen = 0;

    HYPHY20G_ASSERT((pCtxtBuf != NULL), hdrLen,
                    "%s", "pCtxtBuf is NULL.");

    ctxtBufHdr.len = len;
    ctxtBufHdr.id = id;
    ctxtBufHdr.index = index;

    hdrLen = sizeof(ctxtBufHdr);
    sysHyPhy20gMemCpy(pCtxtBuf, (void *)&ctxtBufHdr, hdrLen);

    return hdrLen;
} /* utilHyPhy20gInsertCtxtHdr */

/*******************************************************************************
**
**  utilHyPhy20gSearchCtxtHdr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Search the special context header position in the buffer.
**
**
**  INPUTS:         pCtxtBuf - pre-stored context buffer
**                  ctxtId - context id to be searched in the buffer
**                  index - index of the context array to be searched
**
**  OUTPUTS:        None
**
**  RETURNS:        The special header position in the buffer, NULL on failure
**
*******************************************************************************/
sHYPHY20G_CTXT_HDR *utilHyPhy20gSearchCtxtHdr(void *pCtxtBuf, UINT4 ctxtId, UINT4 index)
{
    sHYPHY20G_CTXT_HDR *pTmpHdr;
    
    HYPHY20G_ASSERT((pCtxtBuf != NULL), NULL,
                    "%s", "pCtxtBuf is NULL.");

    /* retrieve the ctxt header in the pre-stored context buffer */
    pTmpHdr = (sHYPHY20G_CTXT_HDR *)pCtxtBuf;

    while (pTmpHdr != NULL && 
        pTmpHdr->len > 0) {
        if (pTmpHdr->id == ctxtId && 
            pTmpHdr->index == index) {
            return pTmpHdr;
        }
		/* move to next ctxt buffer header */
        pTmpHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pTmpHdr + pTmpHdr->len);
    }

    return NULL;
} /* utilHyPhy20gSearchCtxtHdr */

/* end of file */
