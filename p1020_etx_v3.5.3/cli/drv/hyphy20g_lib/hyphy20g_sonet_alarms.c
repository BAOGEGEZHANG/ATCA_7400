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
**  FILE        : hyphy20g_sonet_alarms.c
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
#include "hyphy20g_sonet_alarms.h"

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
**  hyPhy20gSonetTohRxCfgInsAisL
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables added Line AIS insertion 
**                  (RLAISINS) alarm into the ingress data stream within the 
**                  SONET subsystem. RLAISINS alarm is selectively enabled based
**                  on Section Rx defects (OOF, LOS, LOF, STIU, STIM.) 
**                  The indication associated defect will be ORed into the 
**                  RLAISINS alarm. 
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15 
**                  aisType     - AIS-L type inserted into downstream payload
**                                0 - AIS is inserted in the line overhead 
**                                    bytes and in the payload bytes of the main
**                                    output stream
**                                1 - AIS is only inserted in bits 6, 7 and 8 
**                                    of the main output stream K2 byte
**                  lofEnbl     - 0 - LOF defect will not be ORed with other 
**                                    defect indications, and will not 
**                                    contribute to the RLAISINS alarm.
**                                1 - LOF defect will be ORed with other defect
**                                    indications, and contribute to the
**                                    RLAISINS alarm. 
**                  losEnbl     - 0 - LOS defect will not be ORed with other
**                                    defect indications, and will not 
**                                    contribute to the RLAISINS alarm.
**                                1 - LOS defect will be ORed with other defect
**                                    indications, and contribute to the
**                                    RLAISINS alarm. 
**                  sTimEnbl    - 0 - TIM-S defect will not be ORed with other
**                                    defect indications, and will not 
**                                    contribute to the RLAISINS alarm.  
**                                1 - TIM-S defect will be ORed with other
**                                    defect indications, and contribute to the
**                                    RLAISINS alarm.
**                  sTiuEnbl    - 0 - TIU-S defect will not be ORed with other
**                                    defect indications, and will not 
**                                    contribute to the RLAISINS alarm.
**                                1 - TIU-S defect will be ORed with other 
**                                    defect indications, and contribute to the
**                                    RLAISINS alarm.
**                  oofEnbl     - 0 - OOF defect will not be ORed with other
**                                    defect indications, and will not 
**                                    contribute to the RLAISINS alarm.
**                                1 - OOF defect will be ORed with other 
**                                    defect indications, and contribute to the
**                                    RLAISINS alarm.
**                                 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxCfgInsAisL(struct file *fileHndl, UINT4 slice, 
                                  UINT4 aisType, UINT4 lofEnbl, UINT4 losEnbl, 
                                  UINT4 sTimEnbl, UINT4 sTiuEnbl, 
                                  UINT4 oofEnbl)
{
    /* Variable declaration */ 
    UINT4 regAddr;
    UINT4 msk;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 15) || (aisType > 1) || (lofEnbl > 1) || (losEnbl > 1) || 
        (sTimEnbl > 1) || (sTiuEnbl > 1)||(oofEnbl >1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
   
    /* Set K2AIS in SONET:RRMP 0x494009C + 0x01000*A(A=0:15) 
       AIS Enable register */
    regAddr = HYPHY20G_SONET_RRMP_REG_AIS_EN; 
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    msk = HYPHY20G_SONET_RRMP_BIT_AIS_EN_K2AIS_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, aisType);
    if (result) return result;  
        
    /* Read SONET:SARC 0x4940410 + 0x01000*A(A=0:15) Receive Section RLAISINS
       and RPAIS Enable register */ 
    regAddr = HYPHY20G_SONET_SARC_REG_RX_SEC_RLAISINS_AND_RPAIS_EN;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Write user supplied lofEnbl to LOFEN */ 
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_LOFEN_MSK; 
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lofEnbl);
    if (result) return result; 
        
    /* Write user supplied losEnbl to LOFEN */ 
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_LOSEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, losEnbl); 
    if (result) return result;
        
    /* Write user supplied sTimEnbl to STIMEN */ 
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_STIMEN_MSK; 
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sTimEnbl); 
    if (result) return result;
    
    /* Write user supplied sTiuEnbl to STIUEN */ 
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_STIUEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sTiuEnbl); 
    if (result) return result;
        
    /* Write user supplied offEnbl to OOFEN */ 
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_OOFEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, oofEnbl); 
    if (result) return result;    
    
    /* Read SONET:RRMP 0x494009C + 0x01000*A(A=0:15) AIS Enable register */
    regAddr = HYPHY20G_SONET_RRMP_REG_AIS_EN; 
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* RLAISEN set as a function *Enbl set to 1 if any are set to 1 else set 
       to 0 to enable line AIS insertion in main outgoing data stream */
    msk = HYPHY20G_SONET_RRMP_BIT_AIS_EN_RLAISEN_MSK;
  
    if (lofEnbl || losEnbl || sTimEnbl || sTiuEnbl) {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
        if (result) return result; 
    } else {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0);
        if (result) return result; 
    }
         
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxCfgInsAisL */



/*******************************************************************************
**
**  hyPhy20gSonetTohRxCfgInsAisP
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables added RPAIS alarm into the
**                  ingress data stream within the SONET subsystem. RPAIS alarm 
**                  is selectively enabled based on Line Rx defects (AIS-L,
**                  RDI-L,APSBF, SD-L, and SF-L). 
**                  The indication associated defect will be ORed into the 
**                  RPAIS alarm. This function controls all paths contained 
**                  within the SONET client.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15 
**                  sfEnbl      - 0 - SF-L defect will not be ORed with other 
**                                    defect indications, and will not 
**                                    contribute to the RPAIS alarm.
**                                1 - SF-L defect will be ORed with other 
**                                    defect indications, and contribute to the
**                                    RPAIS alarm.
**                  sdEnbl      - 0 - SD-L defect will not be ORed with other 
**                                    defect indications, and will not 
**                                    contribute to the RPAIS alarm.
**                                1 - SD-L defect will be ORed with other 
**                                    defect indications, and contribute to the
**                                    RPAIS alarm.
**                  apsBfEnbl   - 0 - APSBF defect will not be ORed with other 
**                                    defect indications, and will not 
**                                    contribute to the RPAIS alarm.
**                                1 - APSBF defect will be ORed with other 
**                                    defect indications, and contribute to the
**                                    RPAIS alarm.
**                  lRdiEnbl    - 0 - RDI-L defect will not be ORed with other 
**                                    defect indications, and will not 
**                                    contribute to the RPAIS alarm.
**                                1 - RDI-L defect will be ORed with other 
**                                    defect indications, and contribute to the
**                                    RPAIS alarm.
**                  lAisEnbl    - 0 - AIS-L defect will not be ORed with other 
**                                    defect indications, and will not 
**                                    contribute to the RPAIS alarm.
**                                1 - AIS-L defect will be ORed with other 
**                                    defect indications, and contribute to the
**                                    RPAIS alarm.
**                                 
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxCfgInsAisP(struct file *fileHndl, UINT4 slice, 
                                  UINT4 sfEnbl, UINT4 sdEnbl, UINT4 apsBfEnbl, 
                                  UINT4 lRdiEnbl, UINT4 lAisEnbl)
{
    /* variable declaration */ 
    UINT4 regAddr;
    UINT4 msk;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 15) || (sfEnbl > 1) || (sdEnbl > 1) || (apsBfEnbl > 1) ||
        (lRdiEnbl > 1) || (lAisEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    /* Read from SONET:SARC 0x4940410 + 0x01000*A(A=0:15)
       Receive Section RLAISINS and RPAIS Enable register */ 
    regAddr = HYPHY20G_SONET_SARC_REG_RX_SEC_RLAISINS_AND_RPAIS_EN;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Write user supplied sfEnbl to SFBEREN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_SFBEREN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sfEnbl);
    if (result) return result;
        
    /* Write user supplied sdEnbl to SDBEREN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_SDBEREN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sdEnbl);
    if (result) return result; 
    
    /* Write user supplied apsBfEnbl to APSBFEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_APSBFEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, apsBfEnbl);
    if (result) return result;
        
    /* Write user supplied lRdiEnbl to LRDIEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_LRDIEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lRdiEnbl);
    if (result) return result;
        
    /* Write user supplied lAisEnbl to LAISEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_RLAISINS_AND_RPAIS_EN_LAISEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lAisEnbl);
    if (result) return result;     
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxCfgInsAisP */

/*******************************************************************************
**
**  hyPhy20gSonetPohRxCfgInsAisP
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables automatic Path AIS, 
**                  (AIS-P) into the ingress data stream within the SONET
**                  subsystem, based on incoming Rx path alarms.  This 
**                  function controls 1 path within the SONET client.  For 
**                  concatenated payloads, configure the first STS1 of the
**                  concatenated payload. 
**
**                  Consequential action generation may be confirmed by checking
**                  the RSVCA indirect AIS_V bit.
**                  
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15 
**                  sts12       - 0 to 3 
**                  sts3        - 0 to 3 
**                  sts1        - 0 to 2
**                  lineEnbl    - Enable PAIS in response to line alarms as
**                                configured in hyPhy20gSonetTohRxCfgInsAisL
**                                0 - Disabled
**                                1 - Enabled
**                  sectEnbl    - Enable PAIS in response to section alarms
**                                as configured in hyPhy20gSonetTohRxCfgInsAisP
**                                0 - Disabled
**                                1 - Enabled
**                  sfEnbl      - 0 - SF-P will not cause AIS-P to overwrite 
**                                    data stream 
**                                1 - SF-P will cause AIS-P to overwrite data
**                                    stream
**                  sdEnbl      - 0 - SD-P will not cause AIS-P to overwrite 
**                                    data stream
**                                1 - SD-P will cause AIS-P to overwrite data
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
INT4 hyPhy20gSonetPohRxCfgInsAisP(struct file *fileHndl, UINT4 slice, 
                                  UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                                  UINT4 lineEnbl, UINT4 sectEnbl,
                                  UINT4 sfEnbl, UINT4 sdEnbl,
                                  UINT4 pTimEnbl, UINT4 pTiuEnbl,
                                  UINT4 pErdiEnbl, UINT4 pRdiEnbl,
                                  UINT4 pPdiEnbl, UINT4 pUneqEnbl,
                                  UINT4 pPlmEnbl, UINT4 pPluEnbl,
                                  UINT4 pAisPtrEnbl, UINT4 pLopEnbl)
{
    /* Variable declaration */ 
    UINT4 sonetSarcRegData;
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 15) || (sts12 > 3) || (sts3 > 3) || ( sts1 > 2) || (sfEnbl > 1)
        || (sdEnbl > 1) || (pTimEnbl > 1) || (pTiuEnbl > 1) || (pErdiEnbl > 1)
        || (pRdiEnbl > 1) || (pPdiEnbl > 1) || (pUneqEnbl > 1) || (pPlmEnbl > 1)
        || (pPluEnbl > 1) || (pAisPtrEnbl > 1) || (pLopEnbl > 1)
        || (lineEnbl > 1) || (sectEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    sts3Off = 4 * sts12 + sts3; 
    
    /* Read from SONET:SARC indirect Receive Path RPAISINS Enable 0x8 */
    iAddr = 0x8;
    result = sonetHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1, 
                                      &sonetSarcRegData);
    if (result) return result;
    
    /* Set PAIS_RPAISEN as a function of sectEnbl b'00 or b'11 */
    msk = HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPALM_RPAISINS_EN_DAT_2_PAIS_RPAISEN_MSK;
    if (sectEnbl == 1) {
        result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, 0x3);
        if (result) return result; 
    } else if (sectEnbl == 0) {
        result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, 0x0);
        if (result) return result; 
    }
    
    /* Write to PAIS_RPAISEN */
    result = sonetHyPhy20gSARCIndWrite(fileHndl, slice, iAddr, sts3Off, sts1, 
                                       sonetSarcRegData);
    if (result) return result; 
    
    /* Read from SONET:SARC indirect Receive Path RPAISINS Enable 0x7 */
    iAddr = 0x7;
    result = sonetHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1, 
                                      &sonetSarcRegData);
    if (result) return result;
        
    /* Set RLAISINSEN as a function of lineEnbl b'00 or b'11 */
    msk = 
   HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_RLAISINSEN_MSK;
    if (lineEnbl == 1) {
        result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, 0x3);
        if (result) return result;
    } else if (lineEnbl == 0) {
        result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, 0x0);
        if (result) return result;
    } 
        
    /* Modify bit values with user supplied data */ 
    msk = 
        HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PSDEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, sdEnbl);
    if (result) return result;
        
    msk = 
        HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PSFEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, sfEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PTIMEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pTimEnbl);
    if (result) return result;
        
    msk = 
       HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PTIUEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pTiuEnbl);
    if (result) return result;
        
    msk = 
      HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PERDIEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pErdiEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PRDIEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pRdiEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PPDIEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pPdiEnbl);
    if (result) return result;
    
    msk = 
      HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PUNEQEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pUneqEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PPLMEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pPlmEnbl);
    if (result) return result;
    
    msk = 
       HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PPLUEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pPluEnbl);
    if (result) return result;
    
    msk = 
    HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PAISPTREN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pAisPtrEnbl);
    if (result) return result;
    
    msk = 
     HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_RPAISINS_EN_DAT_1_PLOPTREN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pLopEnbl);
    if (result) return result;   
        
    /* Write to SONET:SARC indirect Receive Path RPAISINS Enable 0x7 */
    result = sonetHyPhy20gSARCIndWrite(fileHndl, slice, iAddr, sts3Off, sts1, 
                                       sonetSarcRegData);
    if (result) return result;
           
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxCfgInsAisP */


/*******************************************************************************
**
**  hyPhy20gSonetTohTxCfgInsRdiL
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables automatic Line RDI, 
**                  (RDI-L) into the transmit data stream within the SONET
**                  subsystem, based on incoming Rx section/line alarms. 
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15 
**                  rdiDuration - 0 - RDI-L asserted a minimum of 12 frames.
**                                1 - RDI-L asserted a minimum of 22 frames.
**                  sfEnbl      - 0 - SF-L will not cause RDI-L to be
**                                    transmitted 
**                                1 - SF-L will cause RDI-L to be transmitted
**                  sdEnbl      - 0 - SD-L will not cause RDI-L to be
**                                    transmitted 
**                                1 - SD-L will cause RDI-L to be transmitted
**                  sTimEnbl    - 0 - TIM-S will not cause RDI-L to be
**                                    transmitted    
**                                1 - TIM-S will cause RDI-L to be transmitted
**                  sTiuEnbl    - 0 - TIU-S will not cause RDI-L to be
**                                    transmitted 
**                                1 - TIU-S will cause RDI-L to be transmitted
**                  lRdiEnbl    - 0 - RDI-L will not cause RDI-L to be
**                                    transmitted 
**                                1 - RDI-L will cause RDI-L to be transmitted
**                  lAisEnbl    - 0 - AIS-L will not cause RDI-L to be
**                                    transmitted 
**                                1 - AIS-L will cause RDI-L to be transmitted
**                  losEnbl     - 0 - LOS will not cause RDI-L to be
**                                    transmitted 
**                                1 - LOS will cause RDI-L to be transmitted
**                  lofEnbl     - 0 - LOF will not cause RDI-L to be
**                                    transmitted 
**                                1 - LOF will cause RDI-L to be transmitted
**                                 
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohTxCfgInsRdiL(struct file *fileHndl, UINT4 slice, 
                                  UINT4 rdiDuration, UINT4 sfEnbl, 
                                  UINT4 sdEnbl, UINT4 sTimEnbl,
                                  UINT4 sTiuEnbl, UINT4 lRdiEnbl,
                                  UINT4 lAisEnbl, UINT4 losEnbl,
                                  UINT4 lofEnbl)
{
    /* Variable declaration */
    UINT4 regAddr;
    UINT4 msk;
    UINT4 lRdi22 = 0;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 15) || (rdiDuration > 1) || (sfEnbl > 1) || (sdEnbl > 1) ||
        (sTimEnbl > 1) || (sTiuEnbl > 1) || (lRdiEnbl > 1) || (lAisEnbl > 1) ||
        (losEnbl > 1) || (lofEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Set minimum duration for RDI-L assertion */
    switch(rdiDuration)
    {
        /* RDI-L asserted min of 12 frames */
        case 0:
            /* Set LRDI22 to 0 at 0x4940408 + 0x01000*A(A=0:15) 
               Receive configuration */
            lRdi22 = 0;
            
            break;
        /* RDI-L asserted min of 22 frames */
        case 1:
            /* Set LRDI22 to 1 at 0x4940408 + 0x01000*A(A=0:15)
               Receive configuration */
            lRdi22 = 1;
            
            break;
    }  
    
    /* Read from SONET:SARC 0x4940408 + 0x01000*A(A=0:15) Receive 
       configuration */
    regAddr = HYPHY20G_SONET_SARC_REG_RX_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Write to LRDI22 */
    msk = HYPHY20G_SONET_SARC_BIT_RX_CFG_LRDI22_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lRdi22);
    if (result) return result;
        
    /* Read from SONET:SARC 0x4940414 + 0x01000*A(A=0:15) Receive Section 
       TLRDIINS Enable */
    regAddr = HYPHY20G_SONET_SARC_REG_RX_SEC_TLRDIINS_EN;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Write user supplied sfEnbl to SFBEREN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_SFBEREN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sfEnbl); 
    if (result) return result;
        
    /* Write user supplied sdEnbl to SDBEREN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_SDBEREN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sdEnbl); 
    if (result) return result;
        
    /* Write user supplied sTimEnbl to STIMEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_STIMEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sTimEnbl); 
    if (result) return result;
        
    /* Write user supplied sTiuEnbl to STIUEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_STIUEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, sTiuEnbl); 
    if (result) return result;
        
    /* Write user supplied lRdiEnbl to LRDIEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_LRDIEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lRdiEnbl); 
    if (result) return result;
     
    /* Write user supplied lAisEnbl to LAISEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_LAISEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lAisEnbl); 
    if (result) return result;
        
    /* Write user supplied losEnbl to LOSEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_LOSEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, losEnbl); 
    if (result) return result;
        
    /* Write user supplied lofEnbl to LOFEN */
    msk = HYPHY20G_SONET_SARC_BIT_RX_SEC_TLRDIINS_EN_LOFEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lofEnbl); 
    if (result) return result;
                           
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohTxCfgInsRdiL */


/*******************************************************************************
**
**  hyPhy20gSonetPohTxCfgInsAisP
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables automatic Path AIS, 
**                  (AIS-P) into the transmit data stream within the SONET
**                  subsystem, based on incoming Rx path alarms.  This 
**                  function controls 1 path within the SONET client.  For 
**                  concatenated payloads, configure the first STS1 of the
**                  concatenated payload. 
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15 
**                  sts12        - 0 to 3 
**                  sts3         - 0 to 3 
**                  sts1         - 0 to 2
**                  pAisPtrCfg   - 0 - the pointer or any of the concatenated 
**                                     pointers is in AIS state. 
**                                 1 - the pointer is in AIS state. 
**                                 2 - the pointer and all the concatenated 
**                                     pointers are in AIS state.
**                  pAisPtrEnbl  - 0 - AISPTR-P will not cause AIS-P to be
**                                    transmitted.
**                                 1 - AISPTR-P will cause AIS-P to be
**                                    transmitted.
**                  pLopEnbl     - 0 - LOP-P will not cause AIS-P to be
**                                    transmitted.
**                                 1 - LOP-P will cause AIS-P to be
**                                    transmitted.
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
INT4 hyPhy20gSonetPohTxCfgInsAisP(struct file *fileHndl, UINT4 slice, 
                                  UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                                  UINT4 pAisPtrCfg, UINT4 pAisPtrEnbl, 
                                  UINT4 pLopEnbl)
{
    /* Variable declaration */
    UINT4 sonetSarcRegData;
    UINT4 iAddr, regAddr;
    UINT4 msk;
    UINT4 sts3Off;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 15) || (sts12 > 3) || (sts3 > 3) || (sts1 > 2) || 
        (pAisPtrEnbl > 1) || (pLopEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    sts3Off = 4 * sts12 + sts3;
    
    /* Direct read from SONET:TSVCA 0x49406A8 + 0x01000*A(A=0:15) */
    regAddr = HYPHY20G_SONET_TSVCA_REG_DEC_INT_EN_AND_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Unconditionally set RELAY_AISEN = 1 to propagate AIS-P */
    msk = HYPHY20G_SONET_TSVCA_BIT_DEC_INT_EN_AND_CFG_RELAY_AISEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0x1);
    if (result) return result;
    
    /* Configure TPAISPTRCFG[1:0] in SONET:SARC 0x494041C + 0x01000*A(A=0:15) */
    regAddr = HYPHY20G_SONET_SARC_REG_TX_PATH_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    msk = HYPHY20G_SONET_SARC_BIT_TX_PATH_CFG_TPAISPTRCFG_MSK;
    
    if (pAisPtrCfg == 0) {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0x1);
        if (result) return result;
    } else if (pAisPtrCfg == 1) {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0x0);
        if (result) return result;
    } else if (pAisPtrCfg == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0x2);
        if (result) return result;
    }
    
    /* Read from SONET:SARC indirect Transmit Path Configuration */ 
    iAddr = 0x9;
    result = sonetHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1, 
                                      &sonetSarcRegData);
    if (result) return result;
    
    /* Modify TPAISPTREN and TPLOPTREN with pAisPtrEnbl and pLopEnbl */
    msk = 
     HYPHY20G_SONET_SARC_SARC48_IND_IBIT_TX_PATH_TPAISINS_EN_DAT_TPAISPTREN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pAisPtrEnbl);
    if (result) return result;
        
    msk = 
      HYPHY20G_SONET_SARC_SARC48_IND_IBIT_TX_PATH_TPAISINS_EN_DAT_TPLOPTREN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, pLopEnbl);
    if (result) return result;
    
    /* Write to TPAISPTREN and TPLOPTREN */ 
    result = sonetHyPhy20gSARCIndWrite(fileHndl, slice, iAddr, sts3Off, sts1, 
                                       sonetSarcRegData);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
    
} /* hyPhy20gSonetPohTxCfgInsAisP */

/*******************************************************************************
**
**  hyPhy20gSonetPohTxCfgRdiReiP
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables automatic Path
**                  RDI and REI generation into the transmit data stream on a
**                  per path basis within the SONET subsystem
**
**                  Prior to calling this function the device must have been
**                  globally configured for RDI vs ERDI mode of operation with
**                  hyPhy20gSonetPohCfgRdiModeP.
**
**                  If configured for ERDI mode of operation, TIM-P and TIU-P
**                  events can be configured to contribute to the ERDI value
**                  generated in the transmit stream.
**                  
**                  REI may be configured to report a count of BIP-8 errors or
**                  a block indication if any BIP-8 errors occurred in the
**                  multiframe.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15 
**                  sts12       - 0 to 3 
**                  sts3        - 0 to 3 
**                  sts1        - 0 to 2
**                  erdiMode    - Contribution of TIM-P and TIU-P to ERDI
**                                generation
**                                0 - TIM-P and TIU-P do not contribute to ERDI
**                                1 - TIM-P and TIU-P do contribute to ERDI
**                  reiMode     - REI counting vs block mode configuration
**                                0 - REI provides a count of BIP-8 errors from 
**                                    0 to 8
**                                1 - REI reports 0 if no BIP-8 errors or 1 if
**                                    one or more BIP-8 errors.
**                  rdiErdiEnbl  - Enables insertion of RDI/ERDI in the G1 byte
**                                 based on receive alarms from the SARC block
**                                 (TPERDIINS)
**                                0 - Disable insertion of RDI/ERDI based on  
**                                    receive path alarms 
**                                1 - Enable insertion of RDI/ERDI based on 
**                                    recieve path alarms
**                                    NOTE: G1 byte is generated as per the 
**                                    "Path overhead byte source priority" table
**                                    in the device hardward specification.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohTxCfgRdiReiP(struct file *fileHndl, UINT4 slice, 
                                  UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                                  UINT4 erdiMode, UINT4 reiMode, 
                                  UINT4 rdiErdiEnbl)
{
    /* Variable declaration */
    UINT4 iAddr;
    UINT4 msk;
    UINT4 g1InsDisVal = 0;
    UINT4 sonetSarcRegData, sonetThppRegData0, sonetThppRegData1;
    UINT4 sts3Off;
    INT4 result;    

    /* Argument checking */
    if ((slice > 15) || (sts12 > 3) || (sts3 > 3) || (sts1 > 2) ||
        (erdiMode > 1) || (reiMode > 1) || (rdiErdiEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;   
    }
    
    sts3Off = 4 * sts12 + sts3;
      
    /* Configure PTEN in SONET:SARC indirect 0xE Receive Path ERDI Enable */ 
    iAddr = 0xe;
    msk = HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_ERDI_REI_EN_DAT_PTEN_MSK;
    result = sonetHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1, 
                                      &sonetSarcRegData);
    if (result) return result;
    
    /* Configure PTEN for erdiMode */
    result = hyPhy20gVarFieldWrite(&sonetSarcRegData, msk, erdiMode);
    if (result) return result;
        
    result = sonetHyPhy20gSARCIndWrite(fileHndl, slice, iAddr, sts3Off, sts1, 
                                       sonetSarcRegData);
    if (result) return result;
    
    /* Configure PREIEBLK in SONET:THPP indirect register 0x0 Control
       for counting or blocking mode for BIP-8 reporting */
    iAddr = 0x0;
    result = sonetHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1, 
                                      &sonetThppRegData0); 
    if (result) return result;

    msk = HYPHY20G_SONET_THPP_THPP48_IND_IBIT_THPP_CTL_PREIEBLK_MSK;
    result = hyPhy20gVarFieldWrite(&sonetThppRegData0, msk, reiMode);
    if (result) return result;
  
    result = sonetHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1, 
                                       sonetThppRegData0);
    if (result) return result;
    
    /* Configure G1INSDIS in SONET:THPP indirect register 0x1 
       Priority Configuration to enable or disable RDI/ERDI and REI insertion */
    iAddr = 0x1;
    result = sonetHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1, 
                                      &sonetThppRegData1);
    if (result) return result;

    if (rdiErdiEnbl == 1){
        g1InsDisVal = 0;
    } else if (rdiErdiEnbl == 0) {
        g1InsDisVal = 1;
    }

    msk = 
  HYPHY20G_SONET_THPP_THPP48_IND_IBIT_THPP_PRIORITY_CFG_G1INSDIS_MSK;
    result = hyPhy20gVarFieldWrite(&sonetThppRegData1, msk, g1InsDisVal);
    if (result) return result;
    
    result = sonetHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1, 
                                       sonetThppRegData1);
    
    if (result) return result;

    return HYPHY20G_SUCCESS;
    
} /* hyPhy20gSonetPohTxCfgRdiReiP */

/*******************************************************************************
**
**  hyPhy20gSonetTohTxCfgInsReiL
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables automatic Line REI, 
**                  (REI-L) insertion into the transmit data stream within 
**                  the SONET subsystem, based on incoming REI-L events. 
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15 
**                  lReiEnbl    - 0 - REI will not cause REI-L to be transmitted
**                                1 - REI will cause REI-L to be transmitted
**                                 
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohTxCfgInsReiL(struct file *fileHndl, UINT4 slice, 
                                  UINT4 lReiEnbl)
{                                            
    UINT4 regAddr; 
    UINT4 msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (lReiEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;   
    }
        
    /* Set LREIEN at SONET:TRMP 0x4940480 + 0x01000*A(A=0:15) Configuration */
    regAddr = HYPHY20G_SONET_TRMP_REG_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    msk = HYPHY20G_SONET_TRMP_BIT_CFG_LREIEN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, lReiEnbl);                     
    if (result) return result;

    return HYPHY20G_SUCCESS;  
} /* hyPhy20gSonetTohTxCfgInsReiL */

/*******************************************************************************
**
**  hyPhy20gSonetPohCfgRdiModeP
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function configures a slice within the SONET subsystem
**                  to operate in RDI or ERDI mode of operation.
**
**                  This function disables RDI/ERDI generation on all paths.
**
**                  When ERDI mode is configured, SARC_H is configured to
**                  disable ERDI insertion in response to TIM and TIU events.
**
**                  This function disables transfer of RDI/ERDI and REI data
**                  from being reported from receive to transmit paths.
**
**                  The function hyPhy20gSonetPohTxCfgRdiReiP can be called 
**                  after this function to configure RDI/ERDI and REI generation
**                  per path in response to receive events.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15 
**                  rdiMode     - RDI vs ERDI configuration
**                                0 - ERDI
**                                1 - RDI
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohCfgRdiModeP(struct file *fileHndl, UINT4 slice, 
                                 UINT4 rdiMode)
{ 
    /* Variable declaration */ 
    UINT4 iAddr, regAddr;
    UINT4 msk;
    UINT4 sonetSarcRegData, sonetThppRegData;
    UINT4 sts12Num, sts3Num, sts1Num;
    UINT4 sts3Off;
    UINT4 ptenMsk; 
    INT4 result;    
    
    /* Argument checking */ 
    if ((slice > 15) || (rdiMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;   
    }
    
    /* for each path set PTEN = 0 at SONET:SARC at 0x4940400 + 0x01000*A(A=0:15)
       indirect register 0xE Receive Path ERDI Enable and set G1INSDIS = 1 */ 
    iAddr = 0xe;
    ptenMsk = 
           HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_ERDI_REI_EN_DAT_PTEN_MSK;
    
    for (sts12Num = 0; sts12Num <= 3; sts12Num ++) {        
      for (sts3Num = 0; sts3Num <= 3; sts3Num++) {
            for (sts1Num = 0; sts1Num <= 2; sts1Num++) {
                sts3Off = 4 * sts12Num + sts3Num;
                
                result = sonetHyPhy20gSARCIndRead(fileHndl, slice, iAddr, 
                                                  sts3Off, sts1Num, 
                                                  &sonetSarcRegData); 
                if (result) return result;
    
                result = hyPhy20gVarFieldWrite(&sonetSarcRegData, ptenMsk, 0);
                if (result) return result;
        
                result = sonetHyPhy20gSARCIndWrite(fileHndl, slice, iAddr, 
                                                   sts3Off, sts1Num, 
                                                   sonetSarcRegData);
                if (result) return result;

                /* Read SONET:THPP at 0x49405C0 + 0x01000*A(A=0:15) indirect
                   register Priority Configuration 0x1 */  
                iAddr = 0x1;
                result = sonetHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, 
                                                  sts3Off, sts1Num, 
                                                  &sonetThppRegData);
                if (result) return result;
            
                /* set G1INSDIS to 1 for the path */
                msk = 
  HYPHY20G_SONET_THPP_THPP48_IND_IBIT_THPP_PRIORITY_CFG_G1INSDIS_MSK;
                result = hyPhy20gVarFieldWrite(&sonetThppRegData, msk, 1); 
                if (result) return result; 
   
                result = sonetHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, 
                                                   sts3Off, sts1Num, 
                                                   sonetThppRegData);
                if (result) return result;    
           }        
        }
    }
     
    /* Configure RDIEN bit in SONET:SARC 0x4940418 + 0x01000*A(A=0:15) */
    regAddr = HYPHY20G_SONET_SARC_REG_RX_PATH_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    msk = HYPHY20G_SONET_SARC_BIT_RX_PATH_CFG_RDIEN_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, rdiMode);
    if (result) return result;
        
    /* Unconditionally set PERDI22 = 1 in SONET:SARC 
       0x4940418 + 0x01000*A(A=0:15) Receive Path Configuration */ 
    msk = HYPHY20G_SONET_SARC_BIT_RX_PATH_CFG_PERDI22_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    if (result) return result; 

    /* Unconditionally set PERDIPRIO = 1 in SONET:SARC 
       0x4940408 + 0x01000*A(A=0:15) Receive Configuration */
    regAddr = HYPHY20G_SONET_SARC_REG_RX_CFG;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    msk = HYPHY20G_SONET_SARC_BIT_RX_CFG_PERDIPRIO_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohCfgRdiModeP */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
