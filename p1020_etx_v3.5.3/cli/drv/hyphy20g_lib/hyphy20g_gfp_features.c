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
**
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        : hyphy20g_gfp_features.c
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_gfp_features.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gGfpHdlcPrependCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Specifies the HDLC header prepend to use for a
**                  channel configured for HDLC processing.   This procedure
**                  should only be called once the GFP subsystem has been
**                  initialized, and the HDLC channel being configured as also
**                  been initialized.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              hdrLength   - Specifies the length of HDLC prepend to use
**                          - 1 - 4  - Header prepend will be inserted with
**                            1 - 4 bytes of data.
**              hdrData[4]  - Data to be inserted in HDLC header prepend.  For
**                            header lengths of less than 4 bytes, hdrData[n]
**                            can be any value.  i.e. if hdrLength = 2, then
**                            hdrData[3,4] will be ignored.
**              enable      - 0 - HDLC prepend will NOT be inserted
**                          - 1 - HDLC prepend will be inserted.
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpHdlcPrependCfg(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                               UINT4 hdrLength, UINT4 hdrData[4], UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 extHdrData = 0;
    UINT4 burstEnd;
    UINT4 data0, data1, data2;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(hdrLength > 4)||(hdrLength < 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure HDLC Prepend Parameters */
    if (enable == 1) {
        /* Configure  HDLC Prepend Contents */
        extHdrData  =  (hdrData[0]<<24) + (hdrData[1]<<16) + (hdrData[2]<<8) +hdrData[3];
        burstEnd = 0x10 >> hdrLength;
        gfpfHyPhy20gTXDPExtHdrIndWrite(fileHndl, slice, cpbId,
                                       0, 2, 1, burstEnd, extHdrData);

        /* Enable HDLC Prepend Insertion */
        result = gfpfHyPhy20gTXDPChanIndRead(fileHndl, slice, cpbId,
                                                        &data0, &data1, &data2);
        if (result) return result;
                                                        
        result = hyPhy20gVarFieldWrite(&data2,
            HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_EHDR_EN_MSK,
            0x1);
        if (result) return result;

        result = gfpfHyPhy20gTXDPChanIndWrite(fileHndl, slice, cpbId,
                                                        data0, data1, data2);
        if (result) return result;
    } else {
        /* Disable HDLC Prepend Insertion */
        result = gfpfHyPhy20gTXDPChanIndRead(fileHndl, slice, cpbId,
                                                    &data0, &data1, &data2);
        if (result) return result;
            
        result = hyPhy20gVarFieldWrite(&data2,
            HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_EHDR_EN_MSK,
            0x0);
        if (result) return result;

        result = gfpfHyPhy20gTXDPChanIndWrite(fileHndl, slice, cpbId,
                                                    data0, data1, data2);
        if (result) return result;
    }
    return HYPHY20G_SUCCESS;

} /* hyPhy20gGfpHdlcPrependCfg */

/*******************************************************************************
**
**  hyPhy20gGfpHdlcCtrlFrmIns
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the HDLC control frame to be inserted.  This
**                  procedure should only be called once the GFP subsystem has
**                  been initialized, and the HDLC channel being configured has
**                  also been initialized.
**
**                  For all normal operations of HDLC type of traffic,
**                  'cmfPrio' must always be set to 1 to use high priority.   
**                  
**
**  INPUTS:     *fileHndl       - base address
**              slice           - 0 - Slice 'A'
**                              - 1 - Slice 'B'
**              cpbId           - CPBID of the channel being configured
**                              - valid values are 0x0 - 0x3F (0-63)
**              ctrlFrmPrio     - Specifies the priority of HDLC control frame
**                              - 0 - HDLC control frame is inserted when there
**                                is no data arriving from TXGFPT
**                              - 1 - HDLC control frame is inserted at end of
**                                current frame, even if there is data arriving
**                                from TXGFPT
**              ctrlFrmLength   - Specifies the byte length of HDLC control frame
**                              - Valid values are 0x4 - 0x64 (4 - 100)
**              ctrlFrmData[100]  - Data to be inserted in HDLC control frame.
**                                For control frames of less than 100 bytes,
**                                ctrlFrmData[n] can be any value.  i.e. if
**                                ctrlFrmLength = 60, then ctrlFrmData[61-100] 
**                                will be ignored.
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gGfpHdlcCtrlFrmIns(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                               UINT4 ctrlFrmPrio, UINT4 ctrlFrmLength,
                               UINT4 ctrlFrmData[100])
{
    INT4 result;
    UINT4 regAddr;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 ctrlData = 0;
    UINT4 burstInit = 0;
    UINT4 burstEnd = 0;
    UINT4 accesses = 0;
    UINT4 extraAccess;
    UINT4 i, j, x;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(ctrlFrmLength > 100)||(ctrlFrmLength < 4)||(ctrlFrmPrio > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure HDLC Control Frame Parameters */
    extraAccess = ctrlFrmLength % 4;
    if (extraAccess) {
        accesses = (ctrlFrmLength/4) + 1;
    } else {
        accesses = (ctrlFrmLength/4);
    }

    for (i = 0; i<accesses; i++) {
        j = (4*i);
        ctrlData  =  (ctrlFrmData[j]<<24) + (ctrlFrmData[j+1]<<16) +
                   (ctrlFrmData[j+2]<<8) +ctrlFrmData[j+3];

          if (i==0) {
              burstInit = 1;
          } else {
              burstInit = 0;
          }

          if (i==(accesses -1)) {
              if ((ctrlFrmLength % 4) == 0) {
                  burstEnd = 1;
              } else {
                  x = ctrlFrmLength % 4;
                  burstEnd = 0x10 >> x;
              }
          } else {
              burstEnd = 0;
          }

          result = gfpfHyPhy20gTXDPExtHdrIndWrite(fileHndl, slice, cpbId,
                                                  ctrlFrmPrio, 3, burstInit,
                                                  burstEnd, ctrlData);
          if (result) return result;

    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpHDLCCtrlFrmIns */

/*******************************************************************************
**
**  hyPhy20gGfpFDataExtHdrCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Specifies what configuration of TYPE header and extension
**                  header to use for Client Data frames on a channel configured
**                  for GFP-F processing.  This procedure should only be called
**                  once the GFP subsystem has been initialized, and the GFP-F
**                  channel being configured has also been initialized.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              cid         - Specifies the CID to use within the extension
**                            header.  This field is only used when 'enable'=1
**                          - Valid values are 0x0 - 0xFF (0 - 255)
**              hdrVersion  - Specifies that particular version of Type Header /
**                            Extention header combination to use
**                          - 0 - Version 0 (Linear Frame)
**                          - 1 - Version 1 (TBD)
**                          - 2 - Version 2 (TBD)
**              enable      - 0 - Extension header will NOT be inserted
**                          - 1 - Extension header will be inserted.
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpFDataExtHdrCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 cpbId, UINT4 cid, UINT4 hdrVersion,
                               UINT4 enable)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 extHdrData = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 data0 = 0;
    UINT4 data1 = 0;
    UINT4 data2 = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(cid > 0xFF)||(hdrVersion > 2)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure Extension Header Parameters */
    if (enable == 1) {
        if (hdrVersion == 0) {
            /* Configure Linear Extension Header Contents */
            mask = 0xFF << 24;
            result = hyPhy20gVarFieldWrite(&extHdrData, mask, cid);
            if (result) return result;
            gfpfHyPhy20gTXDPExtHdrIndWrite(fileHndl, slice, cpbId,
                                                    0, 0, 1, 0x4, extHdrData);

            /* Set EXI field to 0x1 */
            result = gfpfHyPhy20gTXDPChanIndRead(fileHndl, slice, cpbId,
                                                        &data0, &data1, &data2);
            if (result) return result;
            offset = 24;
            mask = 0xF << offset;
            result = hyPhy20gVarFieldWrite(&data1, mask, 0x1);
            if (result) return result;

            /* Enable Ext Header Insertion */
            result = hyPhy20gVarFieldWrite(&data2,
                HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_EHDR_EN_MSK,
                                                0x1);
            if (result) return result;

            result = gfpfHyPhy20gTXDPChanIndWrite(fileHndl, slice, cpbId,
                                                        data0, data1, data2);
            if (result) return result;
        }
    } else {
        /* Set EXI field to 0x0 */
        result = gfpfHyPhy20gTXDPChanIndRead(fileHndl, slice, cpbId,
                                                    &data0, &data1, &data2);
        if (result) return result;
        offset = 24;
        mask = 0xF << offset;
        result = hyPhy20gVarFieldWrite(&data1, mask, 0x0);
        if (result) return result;

        /* Disable Ext Header Insertion */
        result = hyPhy20gVarFieldWrite(&data2,
            HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_EHDR_EN_MSK,
                                            0x0);
        if (result) return result;

        result = gfpfHyPhy20gTXDPChanIndWrite(fileHndl, slice, cpbId,
                                                    data0, data1, data2);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpFDataExtHdrCfg */


/*******************************************************************************
**
**  hyPhy20gGfpCmfIns
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the GFP CMF frame to be inserted.  This
**                  procedure should only be called once the GFP subsystem has
**                  been initialized, and the GFP channel being configured has
**                  also been initialized.
**      
**                  For all normal operations of CMF and Pause frames of GFP
**                  traffic, 'cmfPrio' must always be set to 1 to use high
**                  priority.
**
**  INPUTS:     *fileHndl       - base address
**              slice           - 0 - Slice 'A'
**                              - 1 - Slice 'B'
**              cpbId           - CPBID of the channel being configured
**                              - valid values are 0x0 - 0x3F (0-63)
**              cmfPrio         - Specifies the priority of CMF insertion
**                              - 0 - CMF is inserted when there
**                                is no data arriving from TXGFPT
**                              - 1 - CMF is inserted at end of current frame,
**                                even if there is data arriving from TXGFPT
**              cmfLength       - Specifies the byte length of GFP CMF
**                              - Valid values are 0x4 - 0x64 (4 - 100)
**              cmFData[100]    - Data to be inserted in GFP CMF.
**                                For control frames of less than 100 bytes,
**                                cmFData[n] can be any value.  i.e. if
**                                cmfLength = 60, then cmFData[61-100] will be
**                                ignored.
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gGfpCmfIns(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                       UINT4 cmfPrio, UINT4 cmfLength, UINT4 cmFData[100])
{
    INT4 result;
    UINT4 regAddr;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 burstInit = 0;
    UINT4 burstEnd = 0;
    UINT4 accesses = 0;
    UINT4 cmFrameData = 0;
    UINT4 extraAccess;
    UINT4 i, j, x;
    UINT4 cpbChOffset;
    UINT4 chIdOffset;
    UINT4 groupId;
    UINT4 chIdMask;
    UINT4 procChanId;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(cmfLength > 100)||(cmfLength < 4)||(cmfPrio > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Grab pointer to CPB Rd Channel Context */
    cpbChOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET
                + slice*HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL + cpbId;
    pRdChnlCtxt = (sHYPHY20G_RD_CHNL_CTXT *)
                                   (pDevCtxt->pCpbCtxt->pRdChnl + cpbChOffset);

    /* Check to make sure channel is already part of a group */
    if (pRdChnlCtxt->gfpGrpId == HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check to make sure channel has been provisioned in CPB */
    if (pRdChnlCtxt->state == HYPHY20G_CPB_CHNL_START) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Get group Id for this channel */
    groupId = pRdChnlCtxt->gfpGrpId;

    /* Update PROC_CHAN_ID with the current channel for the group */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_PROC_CHNL_ID_OFFSET(groupId/4);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &procChanId);
    if (result) return result;

    chIdOffset = 8*(groupId%4);
    chIdMask = 0x3F << chIdOffset;
    result = hyPhy20gVarFieldWrite(&procChanId, chIdMask, cpbId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, procChanId);
    if (result) return result;

    /* Configure CMF Parameters */
    extraAccess = cmfLength % 4;
    if (extraAccess) {
        accesses = (cmfLength/4) + 1;
    } else {
        accesses = (cmfLength/4);
    }

    for (i = 0; i<accesses; i++) {
        j = (4*i);
        cmFrameData  =  (cmFData[j]<<24) + (cmFData[j+1]<<16) +
                   (cmFData[j+2]<<8) +cmFData[j+3];

          if (i==0) {
              burstInit = 1;
          } else {
              burstInit = 0;
          }

          if (i==(accesses -1)) {
              if ((cmfLength % 4) == 0) {
                  burstEnd = 1;
              } else {
                  x = cmfLength % 4;
                  burstEnd = 0x10 >> x;
              }
          } else {
              burstEnd = 0;
          }

          result = gfpfHyPhy20gTXDPExtHdrIndWrite(fileHndl, slice, cpbId,
                                                  cmfPrio, 1, burstInit,
                                                  burstEnd, cmFrameData);
          if (result) return result;

  }
    return HYPHY20G_SUCCESS;

} /* hyPhy20gGfpCmfIns */


/*******************************************************************************
**
**  hyPhy20gGfpRxFdfFrameRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts up to 1 packet from the Frame Data
**                  FIFO.
**
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  cpbId               - cpbId of the channel being queried
**                                      - valid values are 0x0 - 0x3F (0-63)
**                  *fdfNumBytes        - pointer to number of bytes within the
**                                        packet.
**                  *fdfErr             - pointer to FDF Error status.  If this
**                                        bit is a '1', only a partial packet
**                                        was received, and should be discarded.
**                  *fdfData[400]       - pointer to up to 400 bytes of data
**                                        from FDF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxFdfFrameRead(struct file *fileHndl, UINT4 slice,
                               UINT4 cpbId, UINT4 *fdfNumBytes,
                               UINT4 *fdfErr, UINT4 *fdfData)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 currBytes, fdfError, dataAv, eof;
    UINT4 data0, data1;
    UINT4 lastReadSt = 0;
    UINT4 lastReadData = 0;
    UINT4 reads = 0;
    UINT4 fullReads = 0;
    UINT4 remainder = 0;
    UINT4 byte[8];
    UINT4 idx = 0;
    UINT4 i, j, m;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize fdfData[400] */
    for (i = 0; i<400; i++) {
        fdfData[i] = 0x0;
    }

    /* Capture FDF Control data from RXDP */
    while (lastReadSt == 0) {
        result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x5, cpbId,
                                                &data0, &data1);
        if (result) return result;

        mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_NUMBYTES_MSK;
        offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_NUMBYTES_OFF;
        currBytes = mHYPHY20G_GET_FIELD4(data0, mask, offset);

        mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_ERR_AV_MSK;
        offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_ERR_AV_OFF;
        fdfError = mHYPHY20G_GET_FIELD4(data0, mask, offset);

        mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_DATA_AV_MSK;
        offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_DATA_AV_OFF;
        dataAv = mHYPHY20G_GET_FIELD4(data0, mask, offset);

        mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_EOF_AV_MSK;
        offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00101_IBIT_FRM_DAT_FIFO_STAT_RST_FDF_EOF_AV_OFF;
        eof = mHYPHY20G_GET_FIELD4(data0, mask, offset);

        *fdfErr = fdfError;

        result = sysHyPhy20gWrite(fileHndl, 0x4000440, currBytes);
        if (result) return result;

        if (dataAv == 0) {
            *fdfNumBytes = 0;
            return HYPHY20G_SUCCESS;
        }

        if (eof) {
          lastReadSt = 1;
        }

        fullReads = currBytes/8;
        remainder = currBytes%8;
        lastReadData = 0;

        if (remainder) {
            reads = fullReads + 1;
        } else {
            reads = fullReads;
        }

        for (j = 1; j<= reads; j++) {

            if (j==reads) {
                lastReadData = 1;
            }

            /* Capture FDF data from RXDP */
            result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x6, cpbId,
                                                    &data0, &data1);
            if (result) return result;

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE0_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE0_OFF;
            byte[0] = mHYPHY20G_GET_FIELD4(data0, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE1_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE1_OFF;
            byte[1] = mHYPHY20G_GET_FIELD4(data0, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE2_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE2_OFF;
            byte[2] = mHYPHY20G_GET_FIELD4(data0, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE3_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE3_OFF;
            byte[3] = mHYPHY20G_GET_FIELD4(data0, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE4_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE4_OFF;
            byte[4] = mHYPHY20G_GET_FIELD4(data1, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE5_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE5_OFF;
            byte[5] = mHYPHY20G_GET_FIELD4(data1, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE6_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE6_OFF;
            byte[6] = mHYPHY20G_GET_FIELD4(data1, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE7_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00110_IBIT_FRM_DAT_FIFO_DAT_FDF_BYTE7_OFF;
            byte[7] = mHYPHY20G_GET_FIELD4(data1, mask, offset);

            if ((lastReadData==0)||(remainder == 0)) {
                for (m = 0; m< 8; m++) {
                    fdfData[idx] = byte[m];
                    idx++;
                    *fdfNumBytes = idx;
                }
            } else {
                for (m = 0; m< remainder; m++) {
                    fdfData[idx] = byte[m];
                    idx++;
                    *fdfNumBytes = idx;
                }
            }
        }
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxFdfFrameRead */

/*******************************************************************************
**
**  hyPhy20gGfpRxGetFailureId
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function provides the GID and CID for GFP frames that
**                  have exhibited some sort of lookup failure.
**
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  failureType         - Specifies which type of failure status
**                                        to query.
**                                      - 0 - CPBID Lookup Failure
**                                      - 1 - EXI Filter Failure
**                                      - 2 - PTI/UPI Filter Failure
**                  clear               - 0 - Do not clear failure status
**                                      - 1 - Clear failure status
**                  *gid                - pointer to GID for the failed packet.
**                  *cid                - pointer to CID for the failed packet.
**                  *type               - pointer to TYPE header for the failed
**                                        packet.  (Only applies when
**                                        failureType > 0)
**                  *valid              - pointer to REG_FREE bit - when this bit
**                                        is '0' it indicates that new data is
**                                        being provided.  When this bit is '1' it
**                                        indicates data from a previous failure is
**                                        being provided.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxGetFailureId(struct file *fileHndl, UINT4 slice,
                               UINT4 failureType, UINT4 clear,
                               UINT4 *gid, UINT4 *cid, UINT4 *type,
                               UINT4 *valid)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 cpbidLutSt, exiLutSt, typLutSt;

    /* argument checking */
    if ((slice > 1)||(failureType > 2)||(clear > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    switch (failureType) {
        case 0:
            regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_LKUP_FAIL_STAT;
            regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gRead(fileHndl, regAddr, &cpbidLutSt);
            if (result) return result;

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_FAIL_STAT_CPBIC_LUT_FAIL_CID_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_FAIL_STAT_CPBIC_LUT_FAIL_CID_OFF;
            *cid =  mHYPHY20G_GET_FIELD4(cpbidLutSt, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_FAIL_STAT_CPBID_LUT_FAIL_VCID_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_FAIL_STAT_CPBID_LUT_FAIL_VCID_OFF;
            *gid =  mHYPHY20G_GET_FIELD4(cpbidLutSt, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_FAIL_STAT_CPBID_LUT_STATUS_REG_FREE_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_FAIL_STAT_CPBID_LUT_STATUS_REG_FREE_OFF;
            *valid =  mHYPHY20G_GET_FIELD4(cpbidLutSt, mask, offset);

            *type = 0x0;

            if (clear) {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
                if (result) return result;
            }
            break;
        case 1:
            regAddr = HYPHY20G_GFP_FRM_RXDP_REG_EXTN_TYP_FLT_FAIL_STAT;
            regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gRead(fileHndl, regAddr, &exiLutSt);
            if (result) return result;

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_CID_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_CID_OFF;
            *cid =  mHYPHY20G_GET_FIELD4(exiLutSt, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_VCID_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_VCID_OFF;
            *gid =  mHYPHY20G_GET_FIELD4(exiLutSt, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_TYPEHDR_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_TYPEHDR_OFF;
            *type =   mHYPHY20G_GET_FIELD4(exiLutSt, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_STATUS_REG_FREE_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_EXTN_TYP_FLT_FAIL_STAT_EXT_FAIL_STATUS_REG_FREE_OFF;
            *valid =  mHYPHY20G_GET_FIELD4(exiLutSt, mask, offset);


            if (clear) {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
                if (result) return result;
            }
            break;
        case 2:
            regAddr = HYPHY20G_GFP_FRM_RXDP_REG_TYP_FLT_FAIL_STAT;
            regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gRead(fileHndl, regAddr, &typLutSt);
            if (result) return result;

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_CID_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_CID_OFF;
            *cid =  mHYPHY20G_GET_FIELD4(typLutSt, mask, offset);
            mask = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_CPBID_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_CPBID_OFF;
            *gid =  mHYPHY20G_GET_FIELD4(typLutSt, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_TYPEHDR_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_TYPEHDR_OFF;
            *type =   mHYPHY20G_GET_FIELD4(typLutSt, mask, offset);

            mask = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_STATUS_REG_FREE_MSK;
            offset = HYPHY20G_GFP_FRM_RXDP_BIT_TYP_FLT_FAIL_STAT_TYPE_FAIL_STATUS_REG_FREE_OFF;
            *valid =  mHYPHY20G_GET_FIELD4(typLutSt, mask, offset);


            if (clear) {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
                if (result) return result;
            }
            break;
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxGetFailureId */

/*******************************************************************************
**
**  hyPhy20gGfpRxPauseCmfSliceCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the CMF and Pause frame processing parameters
**                  within the RXDP64 on a per-subsystem basis.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              csfStThresh - Specifies the number of CSF frames that must be
**                            received by the RXDP64 before raising the
**                            CSF_ST_AV_I interrupt.
**                          - Valid values 0 - 63 - (0x0 - 0x3F)
**              macDaMsb    - 16-bit MSB of MAC Destination Address
**              macDaLsb    - 32-bit LSB of MAC Destination Address
**                          - Pause time will only be extracted for pause frames
**                            whose destination address match macDaMSB/macDaLSB
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxPauseCmfSliceCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 csfStThresh, UINT4 macDaMsb,
                                   UINT4 macDaLsb)
{
    INT4 result;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 rxdpCfg;
    UINT4 regAddr;

    /* argument checking */
    if ((slice > 1)||(csfStThresh > 0x3F)||(macDaMsb > 0xFFFF)||(macDaLsb > 0xFFFFFFFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure CSF_ST_I_THRSHLD */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CFG;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &rxdpCfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpCfg,
        HYPHY20G_GFP_FRM_RXDP_BIT_CFG_CSF_ST_I_TRSHLD_MSK,
        csfStThresh);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, rxdpCfg);
    if (result) return result;

    /* Configure MAC_DA_LSB */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_MAC_DEST_ADR_LSB;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gWrite(fileHndl, regAddr, macDaLsb);
    if (result) return result;

    /* Configure MAC_DA_MSB */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_MAC_DEST_ADR_MSB;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gWrite(fileHndl, regAddr, macDaMsb);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxPauseCmfSliceCfg */


/*******************************************************************************
**
**  hyPhy20gGfpRxPauseCmfChnlCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the CMF and Pause frame processing parameters
**                  within the RXDP64 on a per-CPBID basis.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - cpbId of the channel being queried
**                          - valid values are 0x0 - 0x3F (0-63)
**              csfDropEn   - 0 - CSF Frames will not be dropped by RXDP64
**                          - 1 - CSF Frames will be dropped by RXDP64
**              csfMaxUpi   - Specifies the upper bound for UPI value to be
**                            contained within CSF frames.
**                          - Valid values 1-254 (0x1 - 0xFE)
**              pauseUpi    - Pause frame detector will only parse frames with
**                            this UPI.  Valid values: 1 - 254 (0x1 - 0xFE)
**              pausePti    - Pause frame detector will only parse frames with
**                            this PTI.  Valid values: 0 or 4 (0x0 or 0x4)
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxPauseCmfChnlCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 cpbId, UINT4 csfDropEn, UINT4 csfMaxUpi,
                                UINT4 pauseUpi, UINT4 pausePti)
{
    INT4 result;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 regAddr;
    UINT4 lsbMaxUpi;
    UINT4 msbMaxUpi;
    UINT4 rxdpPageAddr32Word0;
    UINT4 rxdpPageAddr32Word1;

    /* Configure Indirect Variable settings */
    /* RxDP64 indirect setting initialization */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_CSF_DROP_EN = csfDropEn;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_UPI = pauseUpi;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_PTI = pausePti;

    lsbMaxUpi = mHYPHY20G_GET_FIELD4(csfMaxUpi, 0x7F, 0);
    msbMaxUpi = mHYPHY20G_GET_FIELD4(csfMaxUpi, 0x80, 7);


    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(csfDropEn > 0x1)||(csfMaxUpi > 0xFE)||(csfMaxUpi < 1)||(pauseUpi > 0xFE)||(pauseUpi < 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* argument checking */
    if ((pausePti != 0)&&(pausePti != 4)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure PAGE_ADDRESS_10000 indirect registers */
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x20, cpbId, &rxdpPageAddr32Word0,
                                    &rxdpPageAddr32Word1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_CSF_DROP_EN_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_CSF_DROP_EN);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_CSF_UPI_MAX_VAL_2_MSK,
        msbMaxUpi);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_CSF_UPI_MAX_VAL_1_MSK,
        lsbMaxUpi);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PAUSE_UPI_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_UPI);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PAUSE_PTI_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_PTI);
    if (result) return result;

    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x20, cpbId, rxdpPageAddr32Word0,
                                    rxdpPageAddr32Word1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxPauseCmfChnlCfg */
