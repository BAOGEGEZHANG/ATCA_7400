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
**  FILE        : hyphy20g_opi_counters.c
**
**  $Date: 2013-01-09 11:55:47 +0800 (Wed, 09 Jan 2013) $
**
**  $Revision: 30239 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_opi_counters.h"

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
**  hyPhy20gCbriChnlCntCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the channel to monitor for per
**                channel statistics.  This function must only be called once
**                for a channel and the channel number used with 
**                hyPhy20gCbriGetChnlCounts in order for counter accesses to be
**                valid.
**
**                This function will call hyPhy20gCbriGetChnlCounts to clear
**                stale counter data that would represent the previous channel
**                monitored and the new channel presently being monitored.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - CBRI interface to configure
**                                0 - CBRI 0
**                                1 - CBRI 1
**                 dir          - Direction of travel associated with counters:
**                                0 - Transmit Channel Counters
**                                1 - Receive Channel Counters
**                 monChnl      - CBRI channel to be monitored
**                                0 to 33
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriChnlCntCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                            UINT4 monChnl)
{
    UINT4 addr, msk, val;
    UINT4 data[2];
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 1 || dir > 1 || monChnl > 33) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }    

    addr = HYPHY20G_CBRI_CORE_REG_CNT_ACC_OFFSET(dir);
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Set CMD to b'000 */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_CMD_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0);
    if(result) return result; 

    /* Set TYPE to b'0000 */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0);
    if(result) return result; 

    /* Set ADDR to user specified channel */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_ADDR_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, monChnl);
    if(result) return result; 

    /* write COUNTER ACCESS register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* access counter to clear stale data */
    result = hyPhy20gCbriGetChnlCounts(fileHndl, slice, dir, monChnl, data, 
                                       data, data, data, data, data, data, 
                                       data, data, data);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriChnlCntCfg */

/*******************************************************************************
**
**  hyPhy20gCbriGetChnlCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function retrieves per channel counter statistics for
**                a transmit or receive channel.
**
**                The channel being monitored needs to have been initialized
**                with hyPhy20gCbriChnlCntCfg for counter accesses to be valid.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - CBRI interface to configure
**                                0 - CBRI 0
**                                1 - CBRI 1
**                 dir          - Direction of travel associated with counters:
**                                0 - Transmit Channel Counters
**                                1 - Receive Channel Counters
**                 monChnl      - Channel for counter access.  Must be set to 
**                                the same monChnl as initialized with
**                                hyPhy20gCbriChnlCntCfg.  Legal range:
**                                0 - 33
**                 *pByteCnt[2] - pointer to byte count data.
**                                pByteCnt[0] bit 31:0
**                                pByteCnt[1] bit 55:32
**                 *pPktCnt[2]  - pointer to packet count data.
**                                pPktCnt[0] bit 31:0
**                                pPktCnt[1] bit 47:32
**                 *pErrCnt     - pointer to error count data.
**                 *pBMaxCnt    - pointer to burst max count data.
**                 *pBMinCnt    - pointer to burst min count data.
**                 *pByteStat   - Byte counter status
**                                bit 1 RD_BOVER
**                                bit 0 RD_BHALF
**                 *pPktStat    - Packet counter status
**                                bit 1 RD_POVER
**                                bit 0 RD_PHALF
**                 *pErrStat    - Error counter status
**                                bit 1 RD_EOVER
**                                bit 0 RD_EHALF
**                 *pBMaxStat   - Burst Max counter status
**                                  bit 1 RD_EOVER
**                                  bit 0 RD_EHALF
**                 *pBMinStat   - Burst Min counter status
**                                  bit 1 RD_EOVER
**                                  bit 0 RD_EHALF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriGetChnlCounts(struct file *fileHndl, UINT4 slice, UINT4 dir, 
                               UINT4 monChnl, UINT4 *pByteCnt, UINT4 *pPktCnt, 
                               UINT4 *pErrCnt, UINT4 *pBMaxCnt, UINT4 *pBMinCnt,
                               UINT4 *pByteStat, UINT4 *pPktStat, 
                               UINT4 *pErrStat, UINT4 *pBMaxStat, 
                               UINT4 *pBMinStat)
{
    /* varible declaration */
    UINT4 addr, msk, val;
    UINT4 pcCntData;
    UINT4 byteCntData;
    UINT4 errCntData;
    UINT4 bCntHiAddr, bCntLoAddr;
    UINT4 pCntHiAddr, pCntLoAddr;
    UINT4 eCntAddr, burstCfgAddr;
    UINT4 hiMsk, loMsk;
    UINT4 maxDisMsk, minDisMsk;
    UINT4 overMsk, halfMsk;
    UINT4 byteCntHi, byteCntLo;
    UINT4 pcCntHi, pcCntLo;
    UINT4 errCnt, eMsk;
    UINT4 bMinCntData, bMaxCntData;
    UINT4 bMinCnt, bMaxCnt; 
    UINT4 bOverStat, bHalfStat;
    UINT4 pOverStat, pHalfStat;
    UINT4 eOverStat, eHalfStat;
    UINT4 bMaxOverStat, bMaxHalfStat;
    UINT4 bMinOverStat, bMinHalfStat;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 1 || dir > 1 || monChnl > 33) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }    
        
    addr = HYPHY20G_CBRI_CORE_REG_CNT_ACC_OFFSET(dir);
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Set CMD to b'011 */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_CMD_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x3);
    if(result) return result; 

    /* Set TYPE to b'0000 */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0);
    if(result) return result; 

    /* Set ADDR to user specified channel */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_ADDR_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, monChnl);
    if(result) return result; 

    /* write COUNTER ACCESS register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* Extract Counter Data and Overflow Status */
    /* Byte Counter Read */
    bCntHiAddr = HYPHY20G_CBRI_CORE_REG_BYT_CNT_HI_RD_OFFSET(dir);
    bCntHiAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(bCntHiAddr, slice);
    hiMsk = HYPHY20G_CBRI_CORE_BIT_BYT_CNT_HI_RD_BC_RDATA_MSK;
    overMsk = HYPHY20G_CBRI_CORE_BIT_BYT_CNT_HI_RD_RD_BOVER_MSK;
    halfMsk = HYPHY20G_CBRI_CORE_BIT_BYT_CNT_HI_RD_RD_BHALF_MSK;
    
    result = sysHyPhy20gRead(fileHndl, bCntHiAddr, &byteCntData);
    if (result) return result; 
    
    result = hyPhy20gVarFieldRead(byteCntData, hiMsk, &byteCntHi);
    if (result) return result;
    
    /* Extract status of Byte Counter */   
    result = hyPhy20gVarFieldRead(byteCntData, overMsk, &bOverStat);
    if (result) return result;
    
    result = hyPhy20gVarFieldRead(byteCntData, halfMsk, &bHalfStat);
    if (result) return result;
    
    /* Extract lower bits of the read data */     
    bCntLoAddr = HYPHY20G_CBRI_CORE_REG_BYT_CNT_LOW_RD_OFFSET(dir);
    bCntLoAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(bCntLoAddr, slice);
    loMsk = HYPHY20G_CBRI_CORE_BIT_BYT_CNT_LOW_RD_BC_RDATA_MSK;
    
    result = sysHyPhy20gRead(fileHndl, bCntLoAddr, &byteCntData);
    if (result) return result;
           
    result = hyPhy20gVarFieldRead(byteCntData, loMsk, &byteCntLo);
    if (result) return result;
    
    /* Packet Counter Read */ 
    pCntHiAddr = HYPHY20G_CBRI_CORE_REG_PAC_CNT_HI_RD_OFFSET(dir); 
    pCntHiAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(pCntHiAddr, slice);
    hiMsk = HYPHY20G_CBRI_CORE_BIT_PAC_CNT_HI_RD_PC_RDATA_MSK;
    overMsk = HYPHY20G_CBRI_CORE_BIT_PAC_CNT_HI_RD_RD_POVER_MSK;
    halfMsk = HYPHY20G_CBRI_CORE_BIT_PAC_CNT_HI_RD_RD_PHALF_MSK;
     
    result = sysHyPhy20gRead(fileHndl, pCntHiAddr, &pcCntData);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(pcCntData, hiMsk, &pcCntHi);
    if (result) return result;
    
    /* Extract status information for Packet Counter */    
    result = hyPhy20gVarFieldRead(pcCntData, overMsk, &pOverStat);
    if (result) return result;
    
    result = hyPhy20gVarFieldRead(pcCntData, halfMsk, &pHalfStat);
    if (result) return result;
    
    /* Read from lower bits of read data */           
    pCntLoAddr = HYPHY20G_CBRI_CORE_REG_PAC_CNT_LOW_RD_OFFSET(dir);
    pCntLoAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(pCntLoAddr, slice);
    loMsk = HYPHY20G_CBRI_CORE_BIT_PAC_CNT_LOW_RD_PC_RDATA_MSK;
        
    result = sysHyPhy20gRead(fileHndl, pCntLoAddr, &pcCntData);
    if (result) return result;
           
    result = hyPhy20gVarFieldRead(pcCntData, loMsk, &pcCntLo);
    if (result) return result;
     
    /* Error Counter Read */
    eCntAddr = HYPHY20G_CBRI_CORE_REG_ERR_CNT_LOW_RD_OFFSET(dir);
    eCntAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(eCntAddr, slice);
    eMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_LOW_RD_EC_RDATA_MSK;  
    overMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_HI_RD_RD_EOVER_MSK;
    halfMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_HI_RD_RD_EHALF_MSK;
    
    result = sysHyPhy20gRead(fileHndl, eCntAddr, &errCntData);
    if (result) return result; 
        
    result = hyPhy20gVarFieldRead(errCntData, msk, &errCnt);
    if (result) return result;
    
    result = hyPhy20gVarFieldRead(errCntData, overMsk, &eOverStat);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(errCntData, halfMsk, &eHalfStat);
    if (result) return result;
        
    /* Extract BMax_Cnt and BMin_Cnt counter data */
    burstCfgAddr = HYPHY20G_CBRI_CORE_REG_BRST_CFG_OFFSET(dir);
    burstCfgAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(burstCfgAddr, slice);
    maxDisMsk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_MAX_DIS_MSK;
    minDisMsk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_MIN_DIS_MSK;
    
    result = sysHyPhy20gRead(fileHndl, burstCfgAddr, &val);
    if (result) return result;
        
    if (dir == 0) {
        /* M = Tx, set this field to 1 */
        result = hyPhy20gVarFieldWrite(&val, maxDisMsk, 1);
        if (result) return result;
        
        result = hyPhy20gVarFieldWrite(&val, minDisMsk, 1);
        if (result) return result;
    } else if (dir == 1) {
        result = hyPhy20gVarFieldWrite(&val, maxDisMsk, 0);
        if (result) return result;
        
        result = hyPhy20gVarFieldWrite(&val, minDisMsk, 0);
        if (result) return result;
    }
    
    result = sysHyPhy20gWrite(fileHndl, burstCfgAddr, val);
    if (result) return result;
        
    /* Set TYPE to b'0011 to read BMAX_CNT data */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x3);
    if(result) return result; 

    /* write COUNTER ACCESS register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;
        
    /* Read Error counter for BMAX_CNT */
    result = sysHyPhy20gRead(fileHndl, eCntAddr, &bMaxCntData);
    if (result) return result; 
      
    result = hyPhy20gVarFieldRead(bMaxCntData, eMsk, &bMaxCnt);
    if (result) return result;
    
    result = hyPhy20gVarFieldRead(bMaxCntData, overMsk, &bMaxOverStat);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(bMaxCntData, halfMsk, &bMaxHalfStat);
    if (result) return result;
        
    /* Set TYPE to b'0100 to read BMIN_CNT data */
    //msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x4);
    if(result) return result; 
        
    /* Read Error counter for BMIN_CNT */
    result = sysHyPhy20gRead(fileHndl, eCntAddr, &bMinCntData);
    if (result) return result; 
        
    result = hyPhy20gVarFieldRead(bMinCntData, eMsk, &bMinCnt);
    if (result) return result;
    
    result = hyPhy20gVarFieldRead(bMinCntData, overMsk, &bMinOverStat);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(bMinCntData, halfMsk, &bMinHalfStat);
    if (result) return result;
        
    /* Return values */
    /* Issue 291970 */
    pByteCnt[0] = byteCntLo;
    pByteCnt[1] = byteCntHi;
    pPktCnt[0] = pcCntLo;
    pPktCnt[1] = pcCntHi;

    *pErrCnt = errCnt;
    
    *pBMaxCnt = bMaxCnt;
    *pBMinCnt = bMinCnt; 
    
    *pByteStat = (bOverStat << 1) + bHalfStat;
    *pPktStat = (pOverStat << 1) + pHalfStat;
    *pErrStat = (eOverStat << 1) + eHalfStat;
    
    *pBMaxStat = (bMaxOverStat << 1) + bMaxHalfStat;
    *pBMinStat = (bMinOverStat << 1) + bMinHalfStat;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriGetChnlCounts */

/*******************************************************************************
**
**  hyPhy20gCbriGetRxIntfCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function retrieves receive interface counters
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - CBRI interface to configure
**                                0 - CBRI 0
**                                1 - CBRI 1
**                 *pIbFcErrCnt - pointer to IB_FC_ERR count
**                 *pCrc24Cnt   - pointer to CRC24_CNT count
**                 *pIbFcStat   - pointer to IB_FC_ERR counter status
**                                bit 1 RD_EOVER
**                                bit 0 RD_EHALF
**                 *pCrc24Stat  - pointer to CRC24 counter status
**                                bit 1 RD_EOVER
**                                bit 0 RD_EHALF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriGetRxIntfCounts(struct file *fileHndl, UINT4 slice, 
                                 UINT4 *pIbFcErrCnt, UINT4 *pCrc24Cnt,
                                 UINT4 *pIbFcStat, UINT4 *pCrc24Stat)
{
    /* variable declaration */
    UINT4 addr, val, msk;
    UINT4 errRdAddr;
    UINT4 ibFcCntData;
    UINT4 crc24CntData;
    UINT4 statAddr;
    UINT4 errDataMsk;
    UINT4 overMsk, halfMsk;
    UINT4 counterData, statData;
    UINT4 ibFcOverStat, ibFcHalfStat;
    UINT4 crc24OverStat, crc24HalfStat;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
           
    addr = HYPHY20G_CBRI_CORE_REG_CNT_ACC_OFFSET(1);
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Set CMD to b'011 */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_CMD_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x3);
    if(result) return result; 

    /* Set TYPE to b'0001 for IB_FC_ERR count */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 1);
    if(result) return result; 
        
    /* Set ADDR to 0x0 for both IB_FC_ERR_CNT and CRC24_CNT */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_ADDR_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x0);
    if(result) return result; 
    
    /* Write to COUNTER ACCESS register */    
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;
    
    /* Extract counter data */     
    errRdAddr = HYPHY20G_CBRI_CORE_REG_ERR_CNT_LOW_RD_OFFSET(1); 
    errRdAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(errRdAddr, slice);
    DBG_PRINT("errRdAddr = 0x%lx\n", errRdAddr);
    
    errDataMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_LOW_RD_EC_RDATA_MSK;
    
    result = sysHyPhy20gRead(fileHndl, errRdAddr, &counterData);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(counterData, errDataMsk, &ibFcCntData);
    if (result) return result;
    
    /* Extract status information for IB_FC_ERR_CNT */
    statAddr = HYPHY20G_CBRI_CORE_REG_ERR_CNT_HI_RD_OFFSET(1);
    statAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(statAddr, slice);
    DBG_PRINT("statAddr = 0x%lx\n", statAddr);

    overMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_HI_RD_RD_EOVER_MSK;
    halfMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_HI_RD_RD_EHALF_MSK;
    
    result = sysHyPhy20gRead(fileHndl, statAddr, &statData);   
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(statData, overMsk, &ibFcOverStat);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(statData, halfMsk, &ibFcHalfStat);
    if (result) return result;
        
    /* Set TYPE to b'0010 for CRC24_CNT */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 2);
    if(result) return result; 
    
    DBG_PRINT("crc24Addr = 0x%lx\n", addr);    
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
        
    /* Read CRC24_CNT */
    DBG_PRINT("crc24Addr = 0x%lx\n", errRdAddr);
    result = sysHyPhy20gRead(fileHndl, errRdAddr, &counterData);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(counterData, errDataMsk, &crc24CntData);
    if (result) return result;
        
    /* Extract status information for CRC24_CNT */  
    result = sysHyPhy20gRead(fileHndl, statAddr, &statData);   
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(statData, overMsk, &crc24OverStat);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(statData, halfMsk, &crc24HalfStat);
    if (result) return result;
        
    *pIbFcErrCnt = ibFcCntData;
    *pCrc24Cnt = crc24CntData;
    *pIbFcStat = (ibFcOverStat << 1) + ibFcHalfStat; 
    *pCrc24Stat = (crc24OverStat << 1) + crc24HalfStat;
       
    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriGetRxIntfCounts */


/*******************************************************************************
**
**  hyPhy20gCbriGetSerdesLaneCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function retrieves per lane counter statistics for
**                one of the 8 supported SERDES lanes.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - CBRI interface to configure
**                                0 - CBRI 0
**                                1 - CBRI 1
**                 lane         - Serdes Lane:
**                                0 to 7
**                 counter      - Counter to extract:
**                                0101 - BURST_CNT
**                                0110 - ALIGN_CNT
**                                0111 - ALIGN_FAIL_CNT
**                                1000 - WRD_SYNC_CNT
**                                1001 - CDR_CNT
**                                1010 - CRC32_CNT_FILL
**                                1011 - BADCTL_CNT
**                 *pCntVal     - Retrieved Counter value
**                 *pCntStat    - pointer to Cunter status
**                                bit 1 RD_EOVER
**                                bit 0 RD_EHALF
**                                  
**               
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriGetSerdesLaneCounts(struct file *fileHndl, UINT4 slice, 
                                     UINT4 lane, UINT4 counter, UINT4 *pCntVal, 
                                     UINT4 *pCntStat)
{
    /* variable declaration */
    UINT4 addr, val, msk;
    UINT4 cntAddr;
    UINT4 statAddr;
    UINT4 cntData, statData;
    UINT4 counterData;
    UINT4 cntStatHi, cntStatLo;
    UINT4 hiMsk, loMsk;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 1) || (lane > 7) || (counter > 11)) {
         DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    addr = HYPHY20G_CBRI_CORE_REG_CNT_ACC_OFFSET(1);
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Set CMD to b'011 */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_CMD_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x3);
    if(result) return result; 
    
    /* Set TYPE to user specified counter */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, counter);
    if(result) return result; 
       
    /* Set ADDR to user specified serdes lane */
    msk = HYPHY20G_CBRI_CORE_BIT_CNT_ACC_ACC_ADDR_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, lane);
    if(result) return result; 
        
    /* Write to COUNTER ACCESS register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    /* Extract counter */
    cntAddr = HYPHY20G_CBRI_CORE_REG_ERR_CNT_LOW_RD_OFFSET(1);
    cntAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(cntAddr, slice);
    msk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_LOW_RD_EC_RDATA_MSK;
    
    result = sysHyPhy20gRead(fileHndl, cntAddr, &cntData);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(cntData, msk, &counterData);
    if (result) return result; 
        
    /* Extract status information */
    statAddr = HYPHY20G_CBRI_CORE_REG_ERR_CNT_HI_RD_OFFSET(1);
    statAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(statAddr, slice);
    hiMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_HI_RD_RD_EOVER_MSK;
    loMsk = HYPHY20G_CBRI_CORE_BIT_ERR_CNT_HI_RD_RD_EHALF_MSK;
    
    result = sysHyPhy20gRead(fileHndl, statAddr, &statData);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(statData, hiMsk, &cntStatHi);
    if (result) return result;
        
    result = hyPhy20gVarFieldRead(statData, loMsk, &cntStatLo);
    if (result) return result;
        
    *pCntVal = counterData;
    *pCntStat = (cntStatHi << 1) + cntStatLo;
           
    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriGetSerdesLaneCounts */

/*******************************************************************************
**
**  hyPhy20gCbriRxBurstMinCntCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables generation of BMIN_CNT
**                  counter events and the assertion of BMIN_ERR_I in the
**                  presence of received bursts smaller than BURSTMIN or
**                  BURSTSHORT.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - CBRI interface to configure
**                                0 - CBRI 0
**                                1 - CBRI 1
**                  enable    - 1 - enable generation of counter events
**                              0 - disable generation of counter events
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriRxBurstMinCntCfg(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    UINT4 msk, addr, val;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 1) || (enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_CBRI_CORE_REG_BRST_CFG_OFFSET(1);
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_MIN_DIS_MSK;

    /* SET MIN_DIS to enable or disable event generation */
    val = (enable == 0) ? 1:0;
    result = hyPhy20gRegFieldWrite (fileHndl, addr, msk, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCbriRxBurstMinCntCfg */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
