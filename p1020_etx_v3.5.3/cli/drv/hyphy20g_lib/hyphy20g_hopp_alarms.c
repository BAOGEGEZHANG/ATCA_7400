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
**  FILE        : hyphy20g_hopp_alarms.c 
**
**  $Date: 2011-01-30 17:12:19 +0800 (Sun, 30 Jan 2011) $
**
**  $Revision: 10693 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/


/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hopp_alarms.h"

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
**  hyPhy20gHoppPohRxCfgInsAisP
**  ___________________________________________________________________________ 
**
**
**  DESCRIPTION:    This function enables or disables automatic Path AIS, 
**                  (AIS-P) reporting to the VCAT subsystem from the HOPP
**                  subsystem.  For concatenated payloads, configure the first 
**                  STS1 of the concatenated payload. 
**
**                  In order for the VCAT subsystem to overwrite the data stream
**                  with all 1s, the API hyPhy20gVcatLcasSonetTopCfg and 
**                  hyPhy20gVcatLcasSonetLinkSts192Cfg should be
**                  called to initialize the VCAT subsystem.                   
**                  
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 3 
**                  sts12       - 0 to 3 
**                  sts3        - 0 to 3 
**                  sts1        - 0 to 2
**                  sdEnbl      - 0 - SD-P will not cause AIS-P to overwrite 
**                                    data stream
**                                1 - SD-P will cause AIS-P to overwrite data
**                                    stream
**                  sfEnbl      - 0 - SF-P will not cause AIS-P to overwrite 
**                                    data stream 
**                                1 - SF-P will cause AIS-P to overwrite data
**                                    stream
**                  pTimEnbl    - 0 - TIM-P will not cause AIS-P to overwrite
**                                    data stream  
**                                1 - TIM-P will cause AIS-P to overwrite data
**                                    stream
**                  pTiuEnbl    - 0 - TIU-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - TIU-P will cause AIS-P to overwrite data
**                                    stream
**                  pErdiEnbl   - 0 - ERDI-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - ERDI-P will cause AIS-P to overwrite data
**                                    stream
**                  pRdiEnbl    - 0 - RDI-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - RDI-P will cause AIS-P to overwrite data
**                                    stream
**                  pPdiEnbl    - 0 - PDI-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - PDI-P will cause AIS-P to overwrite data
**                                    stream
**                  pUneqEnbl   - 0 - UNEQ-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - UNEQ-P will cause AIS-P to overwrite data
**                                    stream
**                  pPlmEnbl    - 0 - PLM-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - PLM-P will cause AIS-P to overwrite data
**                                    stream
**                  pPluEnbl    - 0 - PLU-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - PLU-P will cause AIS-P to overwrite data
**                                    stream
**                  pAisPtrEnbl - 0 - AISPTR-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - AISPTR-P will cause AIS-P to overwrite 
**                                    data stream
**                  pLopEnbl    - 0 - LOP-P will not cause AIS-P to overwrite
**                                    data stream
**                                1 - LOP-P will cause AIS-P to overwrite data
**                                    stream
**                                 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohRxCfgInsAisP(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                                 UINT4 sdEnbl, UINT4 sfEnbl,
                                 UINT4 pTimEnbl, UINT4 pTiuEnbl,
                                 UINT4 pErdiEnbl, UINT4 pRdiEnbl,
                                 UINT4 pPdiEnbl, UINT4 pUneqEnbl,
                                 UINT4 pPlmEnbl, UINT4 pPluEnbl,
                                 UINT4 pAisPtrEnbl, UINT4 pLopEnbl)
{
    /* Variable declaration */ 
    UINT4 hoppSarcRegData;
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 3) || (sts12 > 3) || (sts3 > 3) || ( sts1 > 2) || (sdEnbl > 1)
        || (sfEnbl > 1) || (pTimEnbl > 1) || (pTiuEnbl > 1) || (pErdiEnbl > 1)
        || (pRdiEnbl > 1) || (pPdiEnbl > 1) || (pUneqEnbl > 1) || (pPlmEnbl > 1)
        || (pPluEnbl > 1) || (pAisPtrEnbl > 1) || (pLopEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    sts3Off = 4 * sts12 + sts3; 
        
    /* Read from HOPP:SARC indirect Receive Path RPAISINS Enable 0x7 */
    iAddr = 0x7;
    result = hoppHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1, 
                                     &hoppSarcRegData);
    if (result) return result;
        
    /* Modify bit values with user supplied data */ 
    msk = 
        HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PSDEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, sdEnbl);
    if (result) return result;
        
    msk = 
        HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PSFEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, sfEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PTIMEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pTimEnbl);
    if (result) return result;
        
    msk = 
       HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PTIUEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pTiuEnbl);
    if (result) return result;
        
    msk = 
      HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PERDIEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pErdiEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PRDIEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pRdiEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PPDIEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pPdiEnbl);
    if (result) return result;
    
    msk = 
      HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PUNEQEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pUneqEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PPLMEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pPlmEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PPLUEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pPluEnbl);
    if (result) return result;
    
    msk = 
    HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PAISPTREN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pAisPtrEnbl);
    if (result) return result;
    
    msk = 
     HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PLOPTREN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, pLopEnbl);
    if (result) return result;   
        
    /* Write to HOPP:SARC indirect Receive Path RPAISINS Enable 0x7 */
    result = hoppHyPhy20gSARCIndWrite(fileHndl, slice, iAddr, sts3Off, sts1, 
                                      hoppSarcRegData);
    if (result) return result;
           
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohRxCfgInsAisP */



/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
