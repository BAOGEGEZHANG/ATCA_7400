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
**  FILE        : hyphy20g_cpb.c
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
#include "hyphy20g_cpb.h"
#include "hyphy20g_gfp.h"
#include "hyphy20g_lsci.h"
#include "hyphy20g_hsci.h"
#include "hyphy20g_util_acb.h"
#include "hyphy20g_util_scbs.h"
/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 utilHyPhy20gSchdInit(struct file *fileHndl, UINT4 schdIntf, UINT4 schdDir,
                          UINT4 length, BOOLEAN enable);
INT4 utilHyPhy20gSchdAdd(struct file *fileHndl, sHYPHY20G_SCHD_CTXT *pSchd,
                         void *pChnl);
INT4 cpbHyPhy20gCpbInitCtxt(struct file *fileHndl);
INT4 cpbHyPhy20gCpbDestroySchd(sHYPHY20G_SCHD_CTXT *pSchd);
INT4 cpbHyPhy20gCpbReadBlockPtr(struct file *fileHndl,
                                 UINT4         block,
                                 UINT4        *ptr);
INT4 cpbHyPhy20gCpbWriteBlockPtr(struct file *fileHndl,
                                  UINT4         block,
                                  UINT4         ptr);
INT4 cpbHyPhy20gCpbBuildFifo(struct file *fileHndl, UINT2 fifoId,
                            UINT2 numBlocks, UINT2 primRdChnl);
INT4 cpbHyPhy20gCpbFreeFifo(struct file *fileHndl,
                            UINT2          fifoId);
INT4 cpbHyPhy20gCpbInitWrPtr(struct file *fileHndl,
                             UINT2          chnlId);
INT4 cpbHyPhy20gCpbResetWrPtr(struct file *fileHndl,
                             UINT2          chnlId);
INT4 cpbHyPhy20gCpbEnableWrPtr(struct file *fileHndl,
                               UINT2           chnlId,
                               BOOLEAN         enableFlg);
INT4 cpbHyPhy20gCpbInitRdPtr(struct file *fileHndl,
                             UINT2          wrChnlId,
                             UINT2          rdChnlId);
INT4 cpbHyPhy20gCpbResetRdPtr(struct file *fileHndl,
                             UINT2          rdChnlId);
INT4 cpbHyPhy20gCpbEnableRdPtr(struct file *fileHndl,
                                 UINT2           chnlId,
                                 BOOLEAN         enableFlg);
INT4 cpbHyPhy20gCpbConfigWrPort(struct file *fileHndl, UINT2 chnlId);
INT4 cpbHyPhy20gCpbInitWrPort(struct file *fileHndl, UINT2 chnlId);
INT4 cpbHyPhy20gCpbResetWrPort(struct file *fileHndl, UINT2 chnlId);
INT4 cpbHyPhy20gCpbEnableWrPort(struct file *fileHndl,
                                UINT2           chnlId,
                                BOOLEAN         enableFlg);
INT4 cpbHyPhy20gCpbConfigRdPort(struct file *fileHndl, UINT2 chnlId);
INT4 cpbHyPhy20gCpbInitRdPort(struct file *fileHndl, UINT2 chnlId);
INT4 cpbHyPhy20gCpbResetRdPort(struct file *fileHndl, UINT2 chnlId);
INT4 cpbHyPhy20gCpbEnableRdPort(struct file *fileHndl,
                                  UINT2           chnlId,
                                  BOOLEAN         enableFlg);
INT4 cpbHyPhy20gCpbGetPortWidth(struct file *fileHndl,
                                UINT2          traffic,
                                UINT1          *pWidth);
INT4 cpbHyPhy20gCPBSchdUpdate(struct file  *fileHndl, UINT4 intfGrpMsk,
                              UINT4 rdSchFirst, UINT4 updateQDR);
INT4 cpbHyPhy20gCPBGenerateQdrSchd( struct file  *fileHndl,
                                   sHYPHY20G_SCHD_CTXT *pSchd,
                                    UINT4 *pCalData);
INT4 cpbHyPhy20gCPBGenerateIntfGrpSchd( struct file  *fileHndl,
                                        UINT4 scope,
                                        sHYPHY20G_SCHD_CTXT *pSchd,
                                        UINT4 *pCalData);
INT4 cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd( struct file  *fileHndl,
                                                     UINT4 scope, UINT4 mode,
                                                     sHYPHY20G_SCHD_CTXT *pSchd,
                                                     UINT4 *pCalData,
                                                     UINT4 *pNumRows);
INT4 cpbHyPhy20gCpbQdrSchdCmp(struct file *fileHndl, sHYPHY20G_SCHD_CTXT *pSchd,
                              UINT4 *pCalData, UINT4 *pDiff);
INT4 cpbHyPhy20gQdrScaleDataToCal(UINT4 *pRateData, UINT4 *pChnlData,
                                  UINT4 *pLinArray, UINT4 calEntries);
INT4 cpbHyPhy20gPutQdrArrayInOrder(UINT4 *pRawData, UINT4 *pRawChnl,
                                   UINT4 *pSortData, UINT4 *pSortChnl);
INT4 cpbHyPhy20gCPBInitQdrSchd( struct file  *fileHndl, UINT4 length );
INT4 cpbHyPhy20gCPBResetQdrSchd( struct file  *fileHndl);
INT4 cpbHyPhy20gCPBInitSchd( struct file  *fileHndl,
                                   UINT4        schd,
                                   UINT4        length);
INT4 cpbHyPhy20gCPBResetSchd( struct file  *fileHndl, UINT4 schd);
INT4 cpbHyPhy20gCPBInitIntfGrpCtxt( struct file  *fileHndl,
                                    UINT4         intfGrp);
INT4 cpbHyPhy20gCPBResetIntfGrpCtxt( struct file  *fileHndl, UINT4 intfGrp);
INT4 cpbHyPhy20gCPBUpdateCal( struct file  *fileHndl,
                             sHYPHY20G_SCHD_CTXT *pSchd,
                              UINT4 *pCalData);
INT4 cpbHyPhy20gCPBWriteCalOffLinePage( struct file  *fileHndl,
                                       sHYPHY20G_SCHD_CTXT *pSchd,
                                        UINT4 *pCalData);
INT4 cpbHyPhy20gCPBSwitchActiveCalPage( struct file  *fileHndl,
                                       sHYPHY20G_SCHD_CTXT *pSchd);
INT4 cpbHyPhy20gCPBWriteSchdCfg( struct file  *fileHndl,
                                sHYPHY20G_SCHD_CTXT *pSchd,
                                 UINT4         reset,
                                 UINT4         schdMode,
                                 UINT4         workPgSel);
INT4 cpbHyPhy20gCPBWriteSchdCtxt( struct file  *fileHndl,
                                 sHYPHY20G_SCHD_CTXT *pSchd,
                                  UINT4         startAddr,
                                  UINT4         endAddr);
INT4 cpbHyPhy20gCPBReadSchdCtxt( struct file  *fileHndl,
                                 sHYPHY20G_SCHD_CTXT *pSchd,
                                  UINT4         *pStartAddr,
                                  UINT4         *pEndAddr);
INT4 cpbHyPhy20gCPBReadSchdCfg( struct file  *fileHndl,
                                sHYPHY20G_SCHD_CTXT *pSchd,
                                 UINT4         *pReset,
                                 UINT4         *pSchdMode,
                                 UINT4         *pWorkPgSel);
INT4 cpbHyPhy20gCPBQuadReadSchdCal( struct file  *fileHndl,
                                sHYPHY20G_SCHD_CTXT *pSchd,
                                 UINT4         row,
                                 UINT4        *pEntry1,
                                 UINT4        *pEntry2,
                                 UINT4        *pEntry3,
                                 UINT4        *pEntry4);
INT4 cpbHyPhy20gCPBQuadWriteSchdCal( struct file  *fileHndl,
                                sHYPHY20G_SCHD_CTXT *pSchd,
                                 UINT4         row,
                                 UINT4         entry1,
                                 UINT4         entry2,
                                 UINT4         entry3,
                                 UINT4         entry4);
INT4 cpbHyPhy20gCpbFifoStatusReset(struct file  *fileHndl,
                                    UINT4       chnlId);
INT4 cpbHyPhy20gCpbToggleChnlRstGo(struct file *fileHndl);
INT4 cpbHyPhy20gCpbPifWrPortTypePresent(struct file *fileHndl, UINT4 chnlId,
                                        UINT4 typePresent);
INT4 cpbHyPhy20gCpbChkChnlORdValidState(struct file *fileHndl, UINT4 chnlId,
                                        UINT4 *valid);
INT4 cpbHyPhy20gCpbLineIntfLowPwrMode(struct file *fileHndl, UINT4 chnlId,
                                      UINT4 lowPwr);
INT4 cpbHyPhy20gCpbIntfInitPorts(struct file *fileHndl, UINT4 intfGrp);

INT4 hyPhy20gCpbSelectRAM(struct file *fileHndl, UINT1 ramType, UINT4 blkSize);

INT4 cpbHyPhy20gCpbWrPortFifoReset(struct file *fileHndl, UINT2 chnlId,
                                   BOOLEAN resetFlg);
INT4 cpbHyPhy20gCpbRdPortFifoReset(struct file *fileHndl, UINT2 chnlId,
                                   BOOLEAN resetFlg);
INT4 cpbHyPhy20gCpbQdrPhyInit(struct file *fileHndl, UINT4 enable);
INT4 cpbHyPhy20gCpbBlockReset(struct file *fileHndl, UINT4 block, UINT4 reset);
INT4 cpbHyPhy20gCpbBlockLowPower(struct file *fileHndl, UINT4 block,
                                 UINT4 lowPower);
INT4 cpbHyPhy20gCpbCheckWrChnlCtxtState(struct file *fileHndl, UINT4 wrChnlId,
                                        UINT4 state);
INT4 cpbHyPhy20gCpbCheckWrChnlCtxtValid(struct file *fileHndl, UINT4 wrChnlId,
                                        UINT4 validState);
INT4 cpbHyPhy20gCpbCheckRdChnlCtxtState(struct file *fileHndl, UINT4 rdChnlId,
                                        UINT4 state);
INT4 cpbHyPhy20gCpbCheckRdChnlCtxtValid(struct file *fileHndl, UINT4 rdChnlId,
                                        UINT4 validState);
INT4 cpbHyPhy20gCPBSchdRateAwareUpdate(struct file  *fileHndl, UINT4 intfGrpMsk,
                                       UINT4 rdSchFirst, UINT4 updateQDR);
INT4 cpbHyPhy20gCPBGenerateQdrRateAwareSchd(struct file  *fileHndl,
                                            sHYPHY20G_SCHD_CTXT *pSchd,
                                            UINT4 *pCalData);
INT4 cpbHyPhy20gCpbGetQdrCalData(struct file  *fileHndl, UINT4 dir,
                                 UINT4 *pLinearCal, UINT4 *pLineEntries,
                                 UINT4 *pGfpAEntries, UINT4 *pGfpBEntries,
                                 UINT4 *pPifEntries);
INT4 cpbHyPhy20gCpbGetQdrCalDataRevA(struct file  *fileHndl, UINT4 dir,
                                     UINT4 *pLinearCal, UINT4 *pLineEntries,
                                     UINT4 *pGfpAEntries, UINT4 *pGfpBEntries,
                                     UINT4 *pPifEntries);
INT4 cpbHyPhy20gCpbGetQdrCalDataStress(struct file  *fileHndl, UINT4 dir,
                                       UINT4 *pLinearCal, UINT4 *pLineEntries,
                                       UINT4 *pGfpAEntries, UINT4 *pGfpBEntries,
                                       UINT4 *pPifEntries);
INT4 cpbHyPhy20gCpbGetQdrIntfAvailRate(struct file  *fileHndl, UINT4 dir,
                                       UINT4 scale, UINT4 *pLineAvailRate,
                                       UINT4 *pGfpAAvailRate,
                                       UINT4 *pGfpBAvailRate, UINT4 *pPifAvailRate);
INT4 cpbHyPhy20gCpbUpdateIntfCtxt(struct file *fileHndl);
INT4 cpbHyPhy20gCpbWrChnlProvEval(struct file *fileHndl, UINT4 chnlId,
                                  UINT4 chnlRate);
INT4 cpbHyPhy20gCpbRdChnlProvEval(struct file *fileHndl, UINT4 chnlId,
                                  UINT4 chnlRate);
INT4 cpbHyPhy20gCpbIntfInitEval(struct file *fileHndl, UINT4 intf);
INT4 cpbHyPhy20gCpbLineSchdChnlRateScaleCalc(struct file  *fileHndl, UINT4 dir,
                                             UINT4 availBw, UINT4 numRows,
                                             UINT4 dataType, UINT4 pktSize,
                                             UINT4 chnlRate,
                                             UINT4 *pMinEntries);
INT4 cpbHyPhy20gCpbLineSchdChnlRateScale(struct file  *fileHndl, UINT4 dir,
                                         UINT4 chnlNum, UINT4 chnlRate,
                                         UINT4 availBw, UINT4 numRows,
                                         UINT4 *pMinEntries);
INT4 cpbHyPhy20gCpbFifoAddBlk(struct file *fileHndl, UINT4 fifoId);
INT4 cpbHyPhy20gCpbFifoDelBlk(struct file *fileHndl, UINT4 fifoId);
INT4 cpbHyPhy20gCpbSyncSWRamBlkLinkList(struct file *fileHndl, UINT4 fifoId);
INT4 cpbHyPhy20gCpbCrc32TableInit(struct file *fileHndl, UINT4 crc32Table[256]);
INT4 cpbHyPhy20gCpbCalcPauseFrmCrc32(struct file *fileHndl,
                                     UINT4 gfpSABase[2], UINT4 crc32Table[256],
                                     UINT4 *pCrc32);
INT4 cpbHyPhy20gCpbZone3DiscardCfg(struct file *fileHndl, UINT4 chnlId,
                                   UINT4 zone3Discard);
INT4 cpbHyPhy20gScaleNumToLargest32BitNum (struct file  *fileHndl, UINT4 num,
                                           UINT4 *pVal, UINT4 *pMult);
INT4 cpbHyPhy20gScaleMultToLargest32BitNum (struct file  *fileHndl, UINT4 num,
                                            UINT4 multNum, UINT4 multNumMult,
                                            UINT4 *pVal, UINT4 *pMult);
INT4 cpbHyPhy20gScaleMultTwoToLargest32BitNum (struct file  *fileHndl, UINT4 num0,
                                               UINT4 multNum0, UINT4 num1,
                                               UINT4 multNum1,
                                               UINT4 *pVal, UINT4 *pMult);
INT4 cpbHyPhy20gRoundedDivide32BitNum (struct file  *fileHndl, UINT4 numerator,
                                       UINT4 denominator, UINT4 *pVal);
INT4 cpbHyPhy20gCeilingDivide32BitNum (struct file  *fileHndl, UINT4 numerator,
                                       UINT4 denominator, UINT4 *pVal);
INT4 cpbHyPhy20gFloorDivide32BitNum (struct file  *fileHndl, UINT4 numerator,
                                     UINT4 denominator, UINT4 *pVal);
INT4 cpbHyPhy20gCPBSchdRateAwareCreateIntermediate(UINT4 startRows,
                                                   UINT4 *pStartData, 
                                                   UINT4 finalRows, 
                                                   UINT4 *pFinalData, 
                                                   UINT4 *pGoodData,
                                                   UINT4 *pStatus);
INT4 cpbHyPhy20gCPBSchdRateAwareGetCalDataAddr(UINT4 firstRow, UINT4 lastRow,
                                            UINT4 chnl, UINT4 *pCalData,
                                            UINT4 *pOffset);
INT4 cpbHyPhy20gCpbChnlDestinedToOtnmaChk(struct file  *fileHndl, UINT4 cpbId, 
                                          UINT4 rdChnlId);                                            
PRIVATE INT4 cpbHyPhy20gCpbAuditRamFifoCtxt(struct file *fileHndl);
PRIVATE INT4 cpbHyPhy20gCpbAuditSchdCtxt(struct file *fileHndl);
PRIVATE INT4 cpbHyPhy20gCpbAuditIntfCtxt(struct file *fileHndl);
PRIVATE INT4 cpbHyPhy20gCpbAuditWrChnlCtxt(struct file *fileHndl);
PRIVATE INT4 cpbHyPhy20gCpbAuditRdChnlCtxt(struct file *fileHndl);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gCpbSetIntQdrBlkSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function is used for internal device testing.  It
**                  operates using global variables that would not be supported
**                  if the driver has been modified to support multiple devices.
**                 
**                  This function overrides the default BLOCK_SIZE value of
**                  d'20 when using internal QDR RAM.
**
**                  To use a non-default BLOCK_SIZE value with internal QDR
**                  RAM, this function must be called BEFORE
**                  hyPhy20gCpbInit.
**
**  INPUTS:         *fileHndl - base address
**                  blkSize   - legal range 0 to 19
**                              0 - default value d'20 is used
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbSetIntQdrBlkSize(struct file *fileHndl, UINT4 blkSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 devId, revId, status;
    INT4 result;

	/* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (blkSize > 19) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt->gData.intQdrBlkSize = blkSize;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbSetIntQdrBlkSize */

/*******************************************************************************
**
**  hyPhy20gCpbInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes CPB subsystem.  
**
**                  If external QDR memory is selected, the QDR PHY will be
**                  initialized, else the QDR PHY will be placed in reset and
**                  low power mode.
**
**                  Alternatively, this function can be called to disable the
**                  CPB subsystem.
**
**                  QDR scheduler length is defaulted to 15
**
**  INPUTS:         *fileHndl - base address
**                  ramType   - 0 - internal memory
**                              15 to 20 - number of address lines used by
**                                         external QDR memory. For example a
**                                         72M device requires 19 address lines.
**                            - invalid if enable = 0
**                  enable    - 1 - enable CPB subsystem
**                              0 - disable CPB subsystem
**
**  NOTE:           The specification of ramType is hardware dependent.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbInit(struct file *fileHndl, UINT4 ramType, UINT4 enable)
{
    UINT4 devId, revId, status;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 qdrSchdLength = HYPHY20G_CPB_QDR_SCHD_MAX_NUM_ROW;
    UINT4 blkSize = 0;
    UINT4 i = 0, powerVal = 1;
    UINT4 lineRdReqTimeout = 0x14; /* internal ram value, d'20 */
    UINT4 gfpRdReqTimeout = 0x14;  /* internal ram value, d'20 */
    UINT4 piRdReqTimeout = 0x14;   /* internal ram value, d'20 */
    INT4 result;

    /* argument checking */
    if (((ramType > 0) && (ramType < 15)) || (ramType > 20) || (enable > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    if (enable==1){

        /* enable operations */
        /* test to ensure memory not already initialized */
        if(pDevCtxt->pCpbCtxt != NULL){
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        /* reset top level subsystem and place into low power mode */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                     HYPHY20G_TOP_BIT_SYS_SW_RST_3_CPB_RST_OFF,
                                     1);
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                              HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_CPB_LOWPWR_OFF, 1);
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                     HYPHY20G_TOP_BIT_SYS_SW_RST_3_CPB_RST_OFF,
                                     0);
        if(result) return result;

        /* reset block level and place into low power mode */
        result = cpbHyPhy20gCpbBlockReset(fileHndl, 0, 1);
        if(result) return result;
        result = cpbHyPhy20gCpbBlockLowPower(fileHndl, 0, 1);
        if(result) return result;

        /* remove cpb block low power */
        result = cpbHyPhy20gCpbBlockLowPower(fileHndl, 1, 0);
        if(result) return result;

        /* remove block soft reset */
        result = cpbHyPhy20gCpbBlockReset(fileHndl, 1, 0);
        if(result) return result;

        /* remove top level low power */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                              HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_CPB_LOWPWR_OFF, 0);
        if(result) return result;

        /* remove QDR scheduler reset */
        result = cpbHyPhy20gCpbBlockReset(fileHndl, 2, 0);
        if(result) return result;

        /* allocate and initialize memory used by CPB subsystem */
        result = hyPhy20gCpbCreateCtxt(fileHndl);
        if(result) return result;

        /* evaluation of compatibility between SW driver and device */
        result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
        if(result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl,
                              HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG,
                              HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_QDR_NUM_NOP_MSK,
                              0x4);
        if(result) return result;

        if(ramType != 0x0){
            /* rev B requirement - external ram value */
            lineRdReqTimeout = 0x22; /* d'34 */
            gfpRdReqTimeout = 0x20;  /* d'32 */
            piRdReqTimeout = 0x22;   /* d'34 */

            /* configure QDR PHY */
            result = cpbHyPhy20gCpbQdrPhyInit(fileHndl, 1);
            if(result) return result;

            /* calculate block size */
            if (ramType == 20) {
                blkSize = 0x3FF;
            } else {
                powerVal = 1;
                for (i = 1; i <= ramType; i++) {
                    powerVal = powerVal * 2;
                }
                blkSize = powerVal / 1024;
            }
            result = hyPhy20gCpbSelectRAM(fileHndl, 1, blkSize);
            if(result) return result;
        } else {
            /* disable QDR */
            result = cpbHyPhy20gCpbQdrPhyInit(fileHndl, 0);
            if(result) return result;

            result = hyPhy20gCpbSelectRAM(fileHndl, 0, blkSize);
            if(result) return result;
        }

        /* set LINE_RD_REQ_TIMEOUT */
        result = hyPhy20gRegFieldWrite(fileHndl,
                       HYPHY20G_CPB_CPB_REG_LN_MISC_CFG,
                       HYPHY20G_CPB_CPB_BIT_LN_MISC_CFG_LINE_RD_REQ_TIMEOUT_MSK,
                       lineRdReqTimeout);
        if(result) return result;

        /* set GFP_RD_REQ_TIMEOUT */
        result = hyPhy20gRegFieldWrite(fileHndl,
                       HYPHY20G_CPB_CPB_REG_GFP_MISC_CFG,
                       HYPHY20G_CPB_CPB_BIT_GFP_MISC_CFG_GFP_RD_REQ_TIMEOUT_MSK,
                       gfpRdReqTimeout);
        if(result) return result;

        /* set PI_RD_REQ_TIMEOUT */
        result = hyPhy20gRegFieldWrite(fileHndl,
                     HYPHY20G_CPB_CPB_REG_PAC_IF_MISC_CFG,
                     HYPHY20G_CPB_CPB_BIT_PAC_IF_MISC_CFG_PI_RD_REQ_TIMEOUT_MSK,
                     piRdReqTimeout);
        if(result) return result;

        /* Initialize QDR Scheduler */
        result = cpbHyPhy20gCPBInitQdrSchd(fileHndl, qdrSchdLength);
        if(result) return result;

        /* set the QDR_SCED_EN bit to 1 */
        result = sysHyPhy20gBitWrite(fileHndl,
                              HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG,
                              HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_QDR_SCED_EN_OFF,
                              1);
        if(result) return result;

        /* for Rev C and Rev D devices unconditionally set LENGTH_DROP_MODE to 1 */
        if(revId == HYPHY20G_REV_ID_C || revId == HYPHY20G_REV_ID_D){
            result = hyPhy20gRegFieldWrite(fileHndl, 
                              HYPHY20G_CPB_CPB_REG_CNT_UDA, 
                              HYPHY20G_CPB_CPB_BIT_CNT_UDA_LENGTH_DROP_MODE_MSK,
                              1);
            if(result) return result;
        } 
    } else {
        /* disable operations */
        /* test to ensure memory is already initialized */
        if(pDevCtxt->pCpbCtxt == NULL){
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        /* set the QDR_SCED_EN bit to 0 */
        result = sysHyPhy20gBitWrite(fileHndl,
                              HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG,
                              HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_QDR_SCED_EN_OFF,
                              0);
        if(result) return result;
        /* Reset QDR Scheduler */
        result = cpbHyPhy20gCPBResetQdrSchd(fileHndl);
        if(result) return result;

        /* reset block level */
        result = cpbHyPhy20gCpbBlockReset(fileHndl, 0, 1);
        if(result) return result;

        /* place block level in low power mode */
        result = cpbHyPhy20gCpbBlockLowPower(fileHndl, 0, 1);
        if(result) return result;

        /* free memory used by CPB subsystem */
        result = hyPhy20gCpbDestroyCtxt(fileHndl);
        if(result) return result;

        /* disable QDR */
        result = cpbHyPhy20gCpbQdrPhyInit(fileHndl, 0);
        if(result) return result;

        /* reset top level subsystem and place in low power */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                     HYPHY20G_TOP_BIT_SYS_SW_RST_3_CPB_RST_OFF,
                                     1);
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                              HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_CPB_LOWPWR_OFF, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbInit */

/*******************************************************************************
**
**  hyPhy20gCpbIntfInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs initialization required for the
**                  operation of channels within an interface group.  The
**                  interface groups are Client, PIF, GFP A and GFP B.
**
**                  Function will set up the read scheduler and the write
**                  scheduler for the interface group.  The PIF has not write
**                  scheduler.
**
**                  Alternatively, this function can be called to disable an
**                  interface group. When disabling an interface group, please
**                  ensure all channels within the interface group are
**                  disabled and deprovisioned.
**
**
**  INPUTS:         *fileHndl - base address
**                  intfGrp   - 0 - Line
**                              1 - GFP A
**                              2 - GFP B
**                              3 - PIF
**                  rdSchdLength - Read Scheduler Length
**                                  - Legal range 1 to 16 for Line, GFP A and
**                                    GFP B schedulers
**                                  - Legal range 1 to 64 for PIF scheduler
**                                  - Value not processed when enable = 0
**                  wrSchdLength - Write Scheduler Length
**                                  - Legal range 1 to 32 for Line
**                                  - Legal range 1 to 64 GFP A and GFP B
**                                    schedulers
**                                  - Invalid for PIF scheduler value not
**                                    processed
**                                  - Value not processed when enable = 0
**                  enable    - 1 - enable CPB subsystem
**                              0 - disable CPB subsystem
**
**  NOTE:           The specification of ramType is hardware dependent.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbIntfInit(struct file *fileHndl, UINT4 intfGrp,
                         UINT4 rdSchdLength, UINT4 wrSchdLength,
                         UINT4 enable)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    UINT4 lowPwrOff = 0;
    UINT4 rdRstOff = 0, wrRstOff = 0, rdSchdNum = 0, wrSchdNum = 0;
    UINT4 schdMode = HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE;
    UINT4 intfRdPortStart, intfRdPortEnd;
    UINT4 intfWrPortStart, intfWrPortEnd;
    UINT4 i;
    INT4 result;

    /* argument checking */
    if ((rdSchdLength < 1) || (wrSchdLength < 1) ||
        ((rdSchdLength > 16)&&(intfGrp == 0)&&(enable==1)) ||
        ((rdSchdLength > 16)&&(intfGrp == 1)&&(enable==1)) ||
        ((rdSchdLength > 16)&&(intfGrp == 2)&&(enable==1)) ||
        ((rdSchdLength > 64)&&(intfGrp == 3)&&(enable==1)) ||
        ((wrSchdLength > 32)&&(intfGrp == 0)&&(enable==1)) ||
        ((wrSchdLength > 64)&&(intfGrp == 1)&&(enable==1)) ||
        ((wrSchdLength > 64)&&(intfGrp == 2)&&(enable==1)) ||
        (intfGrp > 3) || (enable > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    pCpbCtxt = pDevCtxt->pCpbCtxt;

    /* confirm scheduler context is valid */
    if(pCpbCtxt->pSchdCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    pSchd = pCpbCtxt->pSchdCtxt;

    /* indentify parameters for the respective intf group scheduler(s) */
    switch(intfGrp){
        case HYPHY20G_CPB_LINE_INTF_GRP:
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_LINE_RD_SCED_RST_OFF;
            wrRstOff = HYPHY20G_CPB_CPB_BIT_CFG_LINE_WR_SCED_RST_OFF;
            /* scheduler instances */
            rdSchdNum = HYPHY20G_CPB_LINE_RD_SCHD;
            wrSchdNum = HYPHY20G_CPB_LINE_WR_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_LINE_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_GFPA_INTF_GRP:
            /* low power mask */
            lowPwrOff = HYPHY20G_CPB_CPB_BIT_LOW_PWR_GFPA_LOWPWR_OFF;
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPA_RD_SCED_RST_OFF;
            wrRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPA_WR_SCED_RST_OFF;
            /* scheduler instances */
            rdSchdNum = HYPHY20G_CPB_GFPA_RD_SCHD;
            wrSchdNum = HYPHY20G_CPB_GFPA_WR_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_GFPB_INTF_GRP:
            /* low power mask */
            lowPwrOff = HYPHY20G_CPB_CPB_BIT_LOW_PWR_GFPB_LOWPWR_OFF;
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPB_RD_SCED_RST_OFF;
            wrRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPB_WR_SCED_RST_OFF;
            /* scheduler instances */
            rdSchdNum = HYPHY20G_CPB_GFPB_RD_SCHD;
            wrSchdNum = HYPHY20G_CPB_GFPB_WR_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_PIF_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_PIF_INTF_GRP:
            /* low power mask */
            lowPwrOff = HYPHY20G_CPB_CPB_BIT_LOW_PWR_PI_LOWPWR_OFF;
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_PI_RD_SCED_RST_OFF;
            /* scheduler instance */
            rdSchdNum = HYPHY20G_CPB_PIF_RD_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_PIF_PORT_OFFSET;
            intfRdPortEnd = HYPHY20G_CPB_NUM_RD_PORT;
            intfWrPortEnd = HYPHY20G_CPB_NUM_WR_PORT;
            break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check that all write channels within interface are disabled and
       deprovisioned by checking context for HYPHY20G_CPB_CHNL_START and
       HYPHY20G_INVALID_VAL_16BIT states */
    for (i = intfWrPortStart; i < intfWrPortEnd; i++) {
        DBG_PRINT("DEBUG_JC: write ctxt chnl_start check\n");
        result = cpbHyPhy20gCpbCheckWrChnlCtxtState(fileHndl, i,
                                                   HYPHY20G_CPB_CHNL_START);
        if(result) return result;
        DBG_PRINT("DEBUG_JC: write ctxt invalid check\n");
        result = cpbHyPhy20gCpbCheckWrChnlCtxtValid(fileHndl, i,
                                                HYPHY20G_INVALID_VAL_16BIT);
        if(result) return result;
    }

    /* check that all read channels within interface are disabled and
       deprovisioned by checking context for HYPHY20G_CPB_CHNL_START and
       HYPHY20G_INVALID_VAL_16BIT states */
    for (i = intfRdPortStart; i < intfRdPortEnd; i++) {
        DBG_PRINT("DEBUG_JC: read ctxt chnl_start check\n");
        result = cpbHyPhy20gCpbCheckRdChnlCtxtState(fileHndl, i,
                                                   HYPHY20G_CPB_CHNL_START);
        if(result) return result;
        DBG_PRINT("DEBUG_JC: read ctxt invalid check\n");
        result = cpbHyPhy20gCpbCheckRdChnlCtxtValid(fileHndl, i,
                                                HYPHY20G_INVALID_VAL_16BIT);
        if(result) return result;
    }

    if(enable==1){
        /* bring interface out of low power mode */
        if (intfGrp != HYPHY20G_CPB_LINE_INTF_GRP) {
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_LOW_PWR,
                                         lowPwrOff, 0);
            if(result) return result;
        }

        /* reset and initialize read scheduler */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                     rdRstOff, 1);
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                     rdRstOff, 0);
        if(result) return result;

        pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                        rdSchdNum);
        result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd,
                                          (eHYPHY20G_SCBS_SCHD_ID)rdSchdNum,
                                          rdSchdLength, schdMode);
        if(result) return result;

        /* reset and initialize write scheduler */
        if(intfGrp != 3){
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                         wrRstOff, 1);
            if(result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                         wrRstOff, 0);
            if(result) return result;

            pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                            wrSchdNum);
            result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd,
                                              (eHYPHY20G_SCBS_SCHD_ID)wrSchdNum,
                                              wrSchdLength, schdMode);
            if(result) return result;
        }

        /* initialize read and write ports */
        result = cpbHyPhy20gCpbIntfInitPorts(fileHndl, intfGrp);
        if (result) return result;

    } else {
        /* destroy read scheduler context and put in reset */
        pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                        rdSchdNum);
        result = utilHyPhy20gScbsResetSchd(fileHndl, pSchd);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                     rdRstOff, 1);
        if(result) return result;

        /* destroy write scheduler context and put in reset */
        if(intfGrp != 3){
            pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                            wrSchdNum);
            result = utilHyPhy20gScbsResetSchd(fileHndl, pSchd);
            if(result) return result;

            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                         wrRstOff, 1);
            if(result) return result;
        }

        /* put interface into low power mode */
        if (intfGrp != HYPHY20G_CPB_LINE_INTF_GRP) {
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_LOW_PWR,
                                         lowPwrOff, 1);
            if(result) return result;
        }

    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbIntfInit */

/*******************************************************************************
**
**  hyPhy20gCpbChnlProv
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Channel Provision function. To configure a channel, firstly
**                  build a fifo, and then init a write channel as well as
**                  associate it with the fifo, lastly init read channel(s) as
**                  well as associate it to the write channel.
**
**                  This function takes in the UNITLESS chnlRate argument.
**                  chnlRate is used to communicate the RELATIVE RATE of
**                  channels in a user defined granularity to the scheduler
**                  calendar weighting algorithm.  For example, Channel A
**                  requiring 100Mbps bandwidth may be assigned chnlRate = 100.
**                  Channel B requiring 1Gbps bandwidth, should then be
**                  assigned chnlRate = 1000 since its bandwidth is 10 times
**                  that of Channel A.  For the case of equal bandwidth
**                  channels, chnlRate should be assigned the same value across
**                  these channels.
**
**                  The following should be noted for chnlRate usage:
**
**                  Scheduler weighting algorithm Operational Background:
**                  1) The algorithm will popluate all calendar entries with
**                     channel numbers.
**                   - If there is only one channel all entries will contain
**                     that channel number.
**                   - A channel must occupy at least one calendar entry
**                   - If there are more than one channel of the same rate, some
**                     channels may consume more calendar entries.
**                  2) The algorithm will use chnlRate to determine the relative
**                     bandwidth of all channels to allocate calendar entries.
**                  3) The algorithm will not test to relate the chnlRate with a
**                     true data rate.
**
**                  The resolution of any interface scheduler is a function of:
**                  1) The total bandwidth provided to the interface by the QDR
**                     scheduler
**                  2) The total bandwidth being transferred across the
**                     interface
**                  3) The number of entries configured for the scheduler
**
**                  For example, the interface capacity of GFP A in cases where
**                  multiple channels may be transferred across the interface is
**                  the payload rate of STS-192 or the payload rate of an OPU2.
**                  Consider this 10G.
**
**                  The GFP A scheduler has a maximum of 16 rows for a total of
**                  64 entries.
**
**                  At 10G maximum capacity, a single calendar entry would use
**                  1/64 of all requests or 1/64 * 10G  = .15625 G bandwidth
**                  rate of requests for data.
**
**                  "scheduler entry data request rate" =
**                     "Maximum interface capacity / number of calendar entries"
**
**                  The potential bandwidth of each channel within an
**                  interface can be determined:
**
**                  "potential bandwidth" = "scheduler entry data request rate"
**                   * "number of scheduler entries"
**
**                  Relating the calendar usage of entries with the potential
**                  data rate is left to the higher level calling software.
**
**                  This function takes in the zonex argument.  Zone units are
**                  bursts (48 bytes).  The total number of 48 byte bursts equal
**                  the number of blocks assigned to the FIFO times BLOCK_SIZE
**                  (ie. numBlocks * BLOCK_SIZE).
**
**                  If flow control is enabled, the difference between ZONE2 and
**                  ZONE3 must be large enough to "catch" all in-transit data
**                  and data that will be transmitted before the flow control
**                  message takes effect at the far end.
**
**                  The zone3Discard argument controls packet discard behavior.
**                  Packet discard when threshold reaches ZONE3 should be
**                  disabled (zone3Discard = 1) for channels carrying CBR
**                  streams.
**
**                  When packet discard is enabled (zone3Discard = 0), ZONE3
**                  recommended settings are:
**
**                      LINE ZONE3 = numBlocks * BLOCK_SIZE - 9
**                      GFP ZONE3  = numBlocks * BLOCK_SIZE - 17
**                      PIF ZONE3  = numBlocks * BLOCK_SIZE - 4
**
**
**  INPUTS:         fileHndl    - device handle
**                  numBlocks   - Number of blocks in RAM to assign to the FIFO
**                  wrChnlId    - The absolute write channel address
**                                Legal range 0 to 293
**                  wrChnlPortWidth- The write channel port width
**                                   0 - non-8b10b (32-bit)
**                                   1 - 8b10b (40-bit)
**                  primRdChnlId- The absolute read channel address  of the
**                                primary read channel.
**                                Legal range 0 to 297
**                  primRdChnlPortWidth- The primary read channel port width
**                                       0 - non-8b10b (32-bit)
**                                       1 - 8b10b (40-bit)
**                  chnlRate    - 1 - 12000 in units of relative rate
**                  zone1       - FIFO ZONE1 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than ZONE2
**                  zone2       - FIFO ZONE2 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than ZONE3
**                  zone3       - FIFO_ZONE3 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than
**                                numBlocks * BLOCK_SIZE.
**                                Packets will be dropped when FIFO level
**                                reaches ZONE 3.
**                  pifSysIfType - Format of data to and from PIF
**                                 Valid for wrChnlId and primRdChnlId from
**                                 147 to 293 otherwise set to 0.
**                                 0 - Interlaken transfers with TYPE field
**                                 1 - Interlaken transfers without TYPE field
**                                 2 - eXAUI transfers with TYPE/DUMMY field
**                                 3 - eXAUI transfers without TYPE/DUMMY field
**                                 4 - Standard XAUI transfers without
**                                     TYPE/DUMMY field
**                                 Note: For eXAUI mode pifSysIfType must be set
**                                       the same for all channels.
**                  zone3Discard - 0 - Packet received when channel FIFO in
**                                     ZONE3 will be discarded.
**                                 1 - Packet discarding disabled.  CPB FIFO
**                                     overflows can potentially occur.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_INVALID_WRCHNL_CTXT
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_CHNL_FIFO
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlProv(struct file *fileHndl, UINT4 numBlocks, UINT4 wrChnlId,
                         UINT4 wrChnlPortWidth, UINT4 primRdChnlId,
                         UINT4 primRdChnlPortWidth, UINT4 chnlRate, UINT4 zone1,
                         UINT4 zone2, UINT4 zone3, UINT4 pifSysIfType,
                         UINT4 zone3Discard)
{
    UINT4 blkSize, fifoId, typePresent, rdChnlValid, wrChnlValid;
    UINT4 pifRdChnlId, off, addr;
    INT4 result;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_CPB_RAM_CTXT *pRam;
    sHYPHY20G_CTXT *pDevCtxt;

DBG_PRINT("%s, %s(%u, %u, %u), %d\n",__FILE__, __FUNCTION__, (unsigned int)numBlocks, (unsigned int)wrChnlId, (unsigned int)primRdChnlId, __LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pRam = &(pDevCtxt->pCpbCtxt->ram);
    blkSize = (UINT4)pRam->blkSize;

    /* argument checking */
    if( (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
        (wrChnlPortWidth > 1) ||
        (primRdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) ||
        (primRdChnlPortWidth > 1) ||
        (chnlRate < 1)||(chnlRate > 12000)||
        (zone1 > zone2)||(zone2 > zone3)||(zone3 > (numBlocks*blkSize)) ||
        (zone3Discard > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* argument checking conditional on PIF I/F type */
    if ((wrChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         wrChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL)) ||
        (primRdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         primRdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType > 4) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
        if (pifSysIfType != 0) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    fifoId = wrChnlId;

    pFifo = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + primRdChnlId);

    /* Confirm fifo, write and primary read channel states */
    if( (pFifo->state != HYPHY20G_CPB_CHNL_START) ||
        (pWrChnl->state != HYPHY20G_CPB_CHNL_START) ||
        (pRdChnl->state != HYPHY20G_CPB_CHNL_START) ) {
        return HYPHY20G_ERR_INVALID_STATE;
    }

    HYPHY20G_ASSERT((pWrChnl->pRdChnlList == NULL && pWrChnl->rdChnlCnt == 0),
                    HYPHY20G_ERR_INVALID_WRCHNL_CTXT,
                    "%s", "pWrChnl context is invalid.");

    /* update FIFO zone context */
    pFifo->zone1 = zone1;
    pFifo->zone2 = zone2;
    pFifo->zone3 = zone3;

    /* Build a FIFO */
    result = cpbHyPhy20gCpbBuildFifo(fileHndl, (UINT2)fifoId, (UINT2)numBlocks,
                                     (UINT2)primRdChnlId);
    if (result) return result;

    /* Bring high or low speed line interface out of low power mode.
       Only check for line interface channels 0-17 */
    if (wrChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
        result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl, wrChnlId,
                                                    &wrChnlValid);
        if (result) return result;
    } else {
        wrChnlValid = 1;
    }
    if (primRdChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
        result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl, primRdChnlId,
                                                    &rdChnlValid);
        if (result) return result;
    } else {
        rdChnlValid = 1;
    }
    if ((wrChnlId == primRdChnlId) && (wrChnlValid == 0) && (rdChnlValid == 0)) {
        result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, wrChnlId, 0);
        if (result) return result;
    } else {
        if (wrChnlValid == 0) {
            result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, wrChnlId, 0);
            if (result) return result;
        }
        if (rdChnlValid == 0) {
            result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, primRdChnlId, 0);
            if (result) return result;
        }
    }

    /* Update port width before configuring client port reader/writer */
    pWrChnl->portWidth = wrChnlPortWidth;
    pRdChnl->portWidth = primRdChnlPortWidth;

    /* Build a write channel */
    result = cpbHyPhy20gCpbConfigWrPort(fileHndl, (UINT2)wrChnlId);
    if (result) return result;

    /* Configure PI_WR_TYPE_PRESENT as a function of interface type */
    if ((wrChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET) &&
        (wrChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET +
                    HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType == 0 || pifSysIfType == 2) {
            typePresent = 1;
        } else if (pifSysIfType == 1 || pifSysIfType == 3 || pifSysIfType == 4) {
            typePresent = 0;
        } else {
            /* Should never get here */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        result = cpbHyPhy20gCpbPifWrPortTypePresent(fileHndl,
                        (wrChnlId - HYPHY20G_CPB_PIF_PORT_OFFSET), typePresent);
        if (result) return result;
    }

    /* Build a primary read channel */
    result = cpbHyPhy20gCpbConfigRdPort(fileHndl, primRdChnlId);
    if (result) return result;

    /* Configure PI_RD_OUTPUT_LENGTH as a function of interface type */
    if ((primRdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET) &&
        (primRdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET +
                         HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType == 0 || pifSysIfType == 2 || pifSysIfType == 3) {
            typePresent = 1;
        } else {
            typePresent = 0;
        }
        /* Set PI_RD_OUTPUT_LENGTH bit */
        pifRdChnlId = primRdChnlId - HYPHY20G_CPB_PIF_PORT_OFFSET;
        off = HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_OUTPUT_LENGTH_OFF;
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(pifRdChnlId);
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, typePresent);
        if(result) return result;
    }

    /* Configure ZONE3 discard */
    result = cpbHyPhy20gCpbZone3DiscardCfg(fileHndl, wrChnlId, zone3Discard);
    if (result) return result;

    /* Firstly allocate memory for the read channel number list */
    pWrChnl->pRdChnlList = (UINT2 *)sysHyPhy20gMemAlloc(sizeof(UINT2));
    if (pWrChnl->pRdChnlList == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* Update FIFO contexts */
    pFifo->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    /* Update read channel context */
    pRdChnl->valid = HYPHY20G_VALID_SIGN;
    pRdChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;
    pRdChnl->rate = chnlRate;
    pRdChnl->index = 0;
    pRdChnl->primaryFlag = HYPHY20G_CPB_PRIMARY_RD_CHNL;

    /* Update write channel context */
    pWrChnl->valid = HYPHY20G_VALID_SIGN;
    pWrChnl->fifo = fifoId;
    pWrChnl->rate = chnlRate;
    pWrChnl->rdChnlCnt = 1;
    pWrChnl->rdChnlListLen = 1;
    (pWrChnl->pRdChnlList)[pRdChnl->index] = primRdChnlId;
    pWrChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlProv */


/*******************************************************************************
**
**  hyPhy20gCpbChnlUnProv
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Channel Unprovision function. If this channel is in the
**                  operational states, firstly disable this channel. Free the
**                  associated FIFO, reset the write channel, and reset each
**                  read channel.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - The write channel for channel provision
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_INVALID_WRCHNL_CTXT
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlUnProv(struct file *fileHndl, UINT4 wrChnlId)
{
    INT4 result;
    UINT4 i, skipWrChnl, rdChnlValid, wrChnlValid;
    UINT2 numRdChnls, rdChnlId;
    UINT2 *pRdChnls;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    if (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    HYPHY20G_ASSERT((pWrChnl->state != HYPHY20G_CPB_CHNL_START),
                    HYPHY20G_ERR_INVALID_STATE,
                    "%s", "Invalid write channel state.");
    HYPHY20G_ASSERT((pWrChnl->pRdChnlList != NULL),
                     HYPHY20G_ERR_NULL_PTR_PASSED,
                     "%s", "pWrChnl->pRdChnlList is NULL.");
    if(pWrChnl->rdChnlCnt < 1 ||
       pWrChnl->rdChnlCnt > HYPHY20G_CPB_NUM_RD_PORT) {
        return HYPHY20G_ERR_INVALID_WRCHNL_CTXT;
    }

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* If write channel is in the operational state,
       disable this channel at first */
    if  (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL) {
        result = hyPhy20gCpbChnlDisable(fileHndl, wrChnlId);
        if (result) return result;
    }

    /* Free the FIFO */
    result = cpbHyPhy20gCpbFreeFifo(fileHndl, pWrChnl->fifo);
    if (result) return result;

    /* Reset the write channel */
    result = cpbHyPhy20gCpbResetWrPort(fileHndl, wrChnlId);
    if (result) return result;

    result = cpbHyPhy20gCpbResetWrPtr(fileHndl, wrChnlId);
    if (result) return result;

    /* Reset all read channels in the list */
    for (i = 0; i<numRdChnls; i++) {
        pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        result = cpbHyPhy20gCpbResetRdPort(fileHndl, pRdChnls[i]);
        if (result) return result;

        result = cpbHyPhy20gCpbResetRdPtr(fileHndl, pRdChnls[i]);
        if (result) return result;

        /* update read channel context */
        pRdChnl->rate = 0;
        pRdChnl->portWidth = HYPHY20G_INVALID_VAL_16BIT;
        pRdChnl->primaryFlag = 0;
        pRdChnl->valid = HYPHY20G_INVALID_VAL_16BIT;
        pRdChnl->index = HYPHY20G_INVALID_VAL_16BIT;
        pRdChnl->state = HYPHY20G_CPB_CHNL_START;
    }

    /* Reset FIFO status level */
    result = cpbHyPhy20gCpbFifoStatusReset(fileHndl, wrChnlId);
    if (result) return result;

    /* At last, update write channel context */
    pWrChnl->rate = 0;
    pWrChnl->portWidth = HYPHY20G_INVALID_VAL_16BIT;
    pWrChnl->valid = HYPHY20G_INVALID_VAL_16BIT;
    pWrChnl->rdChnlCnt = 0;
    pWrChnl->rdChnlListLen = 0;
    pWrChnl->state = HYPHY20G_CPB_CHNL_START;

    /* If necessary, put high or low speed line interface into low power mode */
    skipWrChnl = 0;
    for (i = 0; i<numRdChnls; i++) {
        pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);
        rdChnlId = pRdChnl->portId;

        /* only check for line interface channels 0-17 */
        if (rdChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
            result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl,
                                                 (UINT4)rdChnlId, &rdChnlValid);
            if (result) return result;
            if (rdChnlValid == 0) {
                result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, rdChnlId,
                                                          1);
                if (result) return result;
                if ((UINT4)rdChnlId == wrChnlId) {
                    skipWrChnl = 1;
                }
            }
        }
    }
    if (skipWrChnl == 0) {
        if (wrChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
            result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl,
                                                        wrChnlId, &wrChnlValid);
            if (result) return result;
            if (wrChnlValid == 0) {
                result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, wrChnlId,
                                                          1);
                if (result) return result;
            }
        }
    }
    sysHyPhy20gMemFree(pWrChnl->pRdChnlList);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlUnProv */

/*******************************************************************************
**
**  hyPhy20gCpbNonPrimRdChnlProv
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Associate a non primary read channel with a write channel.
**                  This function MUST be used after hyPhy20gCpbChnlProv called,
**                  and MUST be used before the target channel is enabled by the
**                  hyPhy20gCpbChnlEnable function.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel for channel provision
**                                Legal range 0 to 293
**                  rdChnlId    - channel ID of a non primary read channel
**                                Legal range 0 to 296
**                  rdChnlPortWidth- The read channel port width
**                                   0 - non-8b10b (32-bit)
**                                   1 - 8b10b (40-bit)
**                  pifSysIfType - Format of data to and from PIF
**                                 Valid for rdChnlId from 147 to 293, otherwise
**                                 set to 0.
**                                 0 - Interlaken transfers with TYPE field
**                                 1 - Interlaken transfers without TYPE field
**                                 2 - XAUI transfers with TYPE/DUMMY field
**                                 3 - XAUI transers without TYPE/DUMMY field
**                                 4 - Standard XAUI transfers without
**                                     TYPE/DUMMY field
**                                 Note: For eXAUI mode pifSysIfType must be set
**                                       the same for all channels.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   The read channel id list in the write channel context
**                  The non primary read channel context
**                  Client port and read pointer register of this non primary
**                  read channel are configured.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbNonPrimRdChnlProv(struct file *fileHndl, UINT4 wrChnlId,
                                  UINT4 rdChnlId, UINT4 rdChnlPortWidth,
                                  UINT4 pifSysIfType)
{
    UINT4 rdChnlValid;
    UINT4 pifRdChnlId, off, addr, typePresent;
    UINT4 i;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Confirm valid fifoId, wrChnlId and numRdChnls have been specified */
    if( (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
        (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) ||
        (rdChnlPortWidth > 1) ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* argument checking conditional on PIF I/F type */
    if ((wrChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         wrChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL)) ||
        (rdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         rdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType > 4) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
        if (pifSysIfType != 0) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Bring high or low speed line interface out of low power mode.
       Only check line interface channels 0-17 */
    if (rdChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
        result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl, rdChnlId,
                                                    &rdChnlValid);
        if (result) return result;
        if (rdChnlValid == 0) {
            result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, rdChnlId, 0);
            if (result) return result;
        }
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* Confirm write and non-primary read channel states */
    if(pWrChnl->valid != HYPHY20G_VALID_SIGN ||
       pRdChnl->state != HYPHY20G_CPB_CHNL_START)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    HYPHY20G_ASSERT((pWrChnl->pRdChnlList != NULL),
                     HYPHY20G_ERR_NULL_PTR_PASSED,
                     "%s", "pWrChnl->pRdChnlList is NULL.");

    /* Update port width before configuring the client port reader */
    pRdChnl->portWidth = rdChnlPortWidth;
    pRdChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;
    pRdChnl->primaryFlag = HYPHY20G_CPB_NON_PRIMARY_RD_CHNL;

    /* Build a non-primary read channel */
    result = cpbHyPhy20gCpbConfigRdPort(fileHndl, rdChnlId);
    if (result) return result;

    /* Configure PI_RD_OUTPUT_LENGTH as a function of interface type */
    if ((rdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET) &&
        (rdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET +
                    HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType == 0 || pifSysIfType == 2 || pifSysIfType == 3) {
            typePresent = 1;
        } else {
            typePresent = 0;
        }
        /* Set PI_RD_OUTPUT_LENGTH bit */
        pifRdChnlId = rdChnlId - HYPHY20G_CPB_PIF_PORT_OFFSET;
        off = HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_OUTPUT_LENGTH_OFF;
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(pifRdChnlId);
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, typePresent);
        if(result) return result;
    }

    /* Increase the length of read channel ID list if it is necessary,
       and update channel context */
    if (pWrChnl->rdChnlCnt + 1 > pWrChnl->rdChnlListLen) {
        UINT2 *p;
        UINT2 len;
        len = pWrChnl->rdChnlListLen + HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN;
        p = (UINT2 *)sysHyPhy20gMemAlloc(len*sizeof(UINT2));

        if (p == NULL) {
            pRdChnl->state = HYPHY20G_CPB_CHNL_START;
            return HYPHY20G_ERR_MEM_ALLOC;
        }

        sysHyPhy20gMemCpy(p, pWrChnl->pRdChnlList, (pWrChnl->rdChnlCnt)*sizeof(UINT2));
        sysHyPhy20gMemFree(pWrChnl->pRdChnlList);
        pWrChnl->pRdChnlList = p;
        pWrChnl->rdChnlListLen += HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN;
    }
    pRdChnl->index = pWrChnl->rdChnlCnt++;
    (pWrChnl->pRdChnlList)[pRdChnl->index] = rdChnlId;
    pRdChnl->rate = pWrChnl->rate;
    pRdChnl->valid = HYPHY20G_VALID_SIGN;

    /* DEBUG */
    for (i = 0; i < (pWrChnl->rdChnlCnt); i++) {
        DBG_PRINT("DEBUG_JC: rdChnlList[%lu] = %u\n", i, (pWrChnl->pRdChnlList)[i]);
    }
    /* END DEBUG */

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbNonPrimRdChnlProv */

/*******************************************************************************
**
**  hyPhy20gCpbNonPrimRdChnlUnProv
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Remove the association of a non primary read channel from
**                  a write channel.
**                  This function MUST be used after the channel provision and
**                  MUST be used for the target channel is in disable state,
**                  like not enabled by hyPhy20gCpbChnlEnable function or
**                  disabled by hyPhy20gCpbChnlDisable function.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel for channel provision
**                                Legal range 0 to 293
**                  rdChnlId    - channel ID of a non primary read channel
**                                Legal range 0 to 296
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_RDCHNL_CTXT
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbNonPrimRdChnlUnProv(struct file *fileHndl, UINT4 wrChnlId,
                                    UINT4 rdChnlId)
{
    UINT4 i, rdChnlValid;
    INT4 result;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl, *pTmpRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Confirm valid fifoId, wrChnlId and numRdChnls have been specified */
    if( (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
        (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* this condition may need adjustment */
    if ((pRdChnl->primaryFlag != HYPHY20G_CPB_NON_PRIMARY_RD_CHNL) ||
        (pRdChnl->index == 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_RDCHNL_CTXT\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_RDCHNL_CTXT;
    }

    /* Confirm non-primary read channel states */
    if(pRdChnl->state != HYPHY20G_CPB_CHNL_EQUIPPED) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* Reset client point reader and read pointer */
    result = cpbHyPhy20gCpbResetRdPort(fileHndl, rdChnlId);
    if (result) return result;

    result = cpbHyPhy20gCpbResetRdPtr(fileHndl, rdChnlId);
    if (result) return result;
    pRdChnl->state = HYPHY20G_CPB_CHNL_START;

    /* update channel context */
    for (i = pRdChnl->index; i < pWrChnl->rdChnlCnt; i++) {
        (pWrChnl->pRdChnlList)[i] = (pWrChnl->pRdChnlList)[i+1];
        pTmpRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                      (pDevCtxt->pCpbCtxt->pRdChnl + (pWrChnl->pRdChnlList)[i]);
        pTmpRdChnl->index = i;
    }
    pWrChnl->rdChnlCnt--;
    pRdChnl->rate = 0;
    pRdChnl->portWidth = HYPHY20G_INVALID_VAL_16BIT;
    pRdChnl->primaryFlag = 0;
    pRdChnl->valid = HYPHY20G_INVALID_VAL_16BIT;
    pRdChnl->index = HYPHY20G_INVALID_VAL_16BIT;
    pRdChnl->state = HYPHY20G_CPB_CHNL_START;

    /* Lazy decrease the length of read channel ID list */
    if (pWrChnl->rdChnlListLen > HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN) {
        if (pWrChnl->rdChnlCnt <
            (pWrChnl->rdChnlListLen - HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN)/2) {
            UINT2 *p;
            UINT2 len;
            len = pWrChnl->rdChnlListLen - HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN;
            p = (UINT2 *)sysHyPhy20gMemAlloc(len*sizeof(UINT2));

            if (p == NULL) {
                return HYPHY20G_ERR_MEM_ALLOC;
            }

            sysHyPhy20gMemCpy(p, pWrChnl->pRdChnlList, len*sizeof(UINT2));
            sysHyPhy20gMemFree(pWrChnl->pRdChnlList);
            pWrChnl->pRdChnlList = p;
            pWrChnl->rdChnlListLen -= HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN;
        }
    }

    /* If necessary, put high or low speed line interface into low power.
       Only check for line interface channels 0-17 */
    if (rdChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
        result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl, rdChnlId,
                                                    &rdChnlValid);
        if (result){
            return result;
        }
        if (rdChnlValid == 0) {
            result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl,
                                                      rdChnlId, 1);
            if (result){
                return result;
            }
        }
    }

    /* DEBUG */
    for (i = 0; i < (pWrChnl->rdChnlCnt); i++) {
        DBG_PRINT("DEBUG_JC: rdChnlList[%lu] = %u\n", i, (pWrChnl->pRdChnlList)[i]);
    }
    /* END DEBUG */

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbNonPrimRdChnlUnProv */



/*******************************************************************************
**
**  hyPhy20gCpbChnlEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable a specific channel by turning on client port readers,
**                  read pointers, write pointer and client port writer.
**
**                  It further adds the channels into the relevant interface
**                  group and updates the QDR scheduler.
**
**                  All primary and non-primary readers associated with the
**                  write channel will be enabled.
**
**                  In order to ensure proper reset of resources associated
**                  with a channel once a channel is disabled 
**                  (with hyPhy20gCpbChnlDisable), 10 msec must pass before
**                  this API is called for the same channel.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID for channel enable
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlEnable(struct file *fileHndl, UINT4 wrChnlId)
{
    INT4 result;
    UINT4 i;
    UINT2 numRdChnls;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnls;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if(wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    /* Confirm write channel states */
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_EQUIPPED) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* Initialize pointers */
    result = cpbHyPhy20gCpbInitWrPtr(fileHndl, wrChnlId);
    if (result) return result;

    /* Determine primary read channel associated with write channel */
    for (i = 0; i<numRdChnls; i++) {
        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        result = cpbHyPhy20gCpbInitRdPtr(fileHndl, wrChnlId, pRdChnls[i]);
        if (result) return result;
    }


    /* Reset FIFO status level */
    result = cpbHyPhy20gCpbFifoStatusReset(fileHndl, wrChnlId);
    if (result) return result;

    /* Confirm read channel states */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;

        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);
        if(p->state != HYPHY20G_CPB_CHNL_EQUIPPED) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }


    /* Enable read channel pointers */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* Enable the read channel Pointer */
        result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnls[i], 1);
        if (result) return result;

        /* Make sure read state is in operational before calling
           cpbHyPhy20gCPBSchdUpdate */
        p->state = HYPHY20G_CPB_CHNL_OPERATIONAL;

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            intfGrpMsk |= 0x20;
        } else {
            intfGrpMsk |= 0x40;
        }
    }


    /* Enable the write channel Pointer */
    result = cpbHyPhy20gCpbEnableWrPtr(fileHndl, wrChnlId, 1);
    if (result) return result;

    /* update the mask reflecting the interface group this channel is
       associated with */
    if(pWrChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x1;
    }else if(pWrChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x4;
    }else if(pWrChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x10;
    }
    /* Make sure write state is in operational before calling
           cpbHyPhy20gCPBSchdUpdate */
    pWrChnl->state = HYPHY20G_CPB_CHNL_OPERATIONAL;


    /* Update interface group and QDR schedulers for the added channel(s) */
    DBG_PRINT("%s, %s, %d, intfGrpMsk = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)intfGrpMsk);
    result = cpbHyPhy20gCPBSchdUpdate(fileHndl, intfGrpMsk, 1, 1);
    if(result) return result;

    /* Enable the read fifo */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* Enable the read port fifo */
        result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl, pRdChnls[i], 0);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;
    }

    /* Enable the write port fifo */
    result = cpbHyPhy20gCpbWrPortFifoReset(fileHndl, wrChnlId, 0);
    if (result) return result;

    result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
    if (result) return result;
        
    /* Check if traffic is going to OTNMA, ensure FIFO_CENTER_EN is disabled
       if the read channel is non-primary */
    for (i = 0; i < numRdChnls; i++) {
        if (pRdChnls[i] > 18 && pRdChnls[i] < 147) {
            /* call the below private function only if the reader is mapped
               to GFP slices */
            result = cpbHyPhy20gCpbChnlDestinedToOtnmaChk(fileHndl, wrChnlId, 
                                                          pRdChnls[i]);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlEnable */

/*******************************************************************************
**
**  hyPhy20gCpbChnlDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Disable a specific channel by turning off the client port
**                  writer, write pointer, client port readers, and read
**                  pointers.
**
**                  It also logically removes the channel from inteface
**                  group scheduler and reevaluates QDR scheduler.
**
**                  All primary and non-primary readers associated with the
**                  write channel will be disabled.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID for channel disable
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_WRCHNL_CTXT
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlDisable(struct file *fileHndl, UINT4 wrChnlId)
{
    UINT4 i, size;
    UINT4 intfGrpMsk = 0;
    UINT2 numRdChnls;
    UINT2 *pRdChnls, *pEquipNonPrimRdChnls;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnl;
    INT4 result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Confirm valid fifoId, wrChnlId and numRdChnls have been specified */
    if (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    HYPHY20G_ASSERT((pWrChnl->pRdChnlList != NULL),
                     HYPHY20G_ERR_NULL_PTR_PASSED,
                     "%s", "pWrChnl->pRdChnlList is NULL.");
    if(pWrChnl->rdChnlCnt < 1 ||
       pWrChnl->rdChnlCnt > HYPHY20G_CPB_NUM_RD_PORT) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_WRCHNL_CTXT\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_WRCHNL_CTXT;
    }

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* Confirm write channel state */
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* Confirm read channel states */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;

        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        if(p->state != HYPHY20G_CPB_CHNL_EQUIPPED &&
           p->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_STATE;
        }
    }

    result = cpbHyPhy20gCpbWrPortFifoReset(fileHndl, wrChnlId, 1);
    if(result) return result;

    result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
    if(result) return result;

    /* Make sure write state is EQUIPPED before calling
       cpbHyPhy20gCPBSchdUpdate */
    pWrChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    /* update the mask reflecting the interface group this channel is
       associated with */
    DBG_PRINT("wrChnlId = %u\n", pWrChnl->portId);
    if(pWrChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        DBG_PRINT("wrChnlId intfGrpMsk |= 0x1\n");
        intfGrpMsk |= 0x1;
    }else if(pWrChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        DBG_PRINT("wrChnlId intfGrpMsk |= 0x4\n");
        intfGrpMsk |= 0x4;
    }else if(pWrChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        DBG_PRINT("wrChnlId intfGrpMsk |= 0x10\n");
        intfGrpMsk |= 0x10;
    }

    /* allocate memory to store current state of all read channels associated
       with the write channel */
    size = sizeof(UINT2)*numRdChnls;
    pEquipNonPrimRdChnls = (UINT2 *)sysHyPhy20gMemAlloc(size);
    if (pEquipNonPrimRdChnls == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* initialize entries to zero */
    for (i = 0; i<numRdChnls; i++) {
        pEquipNonPrimRdChnls[i] = 0;
    }

    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* Make sure read state is EQUIPPED before calling
           cpbHyPhy20gCPBSchdUpdate.  Non-primary readers may already be
           in EQUIPPED state. */
        if(p->state == HYPHY20G_CPB_CHNL_OPERATIONAL) {
            p->state = HYPHY20G_CPB_CHNL_EQUIPPED;
        }else if(p->primaryFlag == HYPHY20G_CPB_NON_PRIMARY_RD_CHNL &&
            p->state ==HYPHY20G_CPB_CHNL_EQUIPPED) {
            pEquipNonPrimRdChnls[i] = 1;
        }else{
            sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_STATE;
        }

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x2\n");
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x8\n");
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x20\n");
            intfGrpMsk |= 0x20;
        } else {
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x40\n");
            intfGrpMsk |= 0x40;
        }
    }

    /* update interface group and QDR schedulers for the added channel(s) */
    result = cpbHyPhy20gCPBSchdUpdate(fileHndl, intfGrpMsk, 0, 1);
    if(result) {
        sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
        return result;
    }

    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        if(pEquipNonPrimRdChnls[i] == 0) {
            /* Disable the read port */
            result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl, pRdChnls[i], 1);
            if(result) {
                sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
                return result;
            }

            result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
            if(result) {
                sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
                return result;
            }
        }
    }

    /* Disable the write channel Pointer */
    result = cpbHyPhy20gCpbEnableWrPtr(fileHndl, wrChnlId, 0);
    if(result) {
        sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
        return result;
    }

    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        if(pEquipNonPrimRdChnls[i] == 0) {
            /* Disable the read channel Pointer */
            result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnls[i], 0);
            if (result) {
                sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
                return result;
            }
        }
    }

    sysHyPhy20gMemFree(pEquipNonPrimRdChnls);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlDisable */

/*******************************************************************************
**
**  hyPhy20gCpbNonPrimRdChnlEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable a specific non-primary read channel by turning on
**                  client port reader and read pointer.  It also adds the read
**                  channel into the relevant interface group and updates the
**                  QDR scheduler.
**
**                  Before the non-primary read channel can be enabled:
**                      1) the associated write channel must first be enabled
**                         with hyPhy20gCpbChnlEnable
**                      2) non-primary read channel must be provisioned with
**                         hyPhy20gCpbNonPrimRdChnlProv
**
**  INPUTS:         fileHndl  - device handle
**                  wrChnlId  - the write channel ID
**                              Legal range 0 to 293
**                  rdChnlId  - the read channel ID for channel enable
**                              Legal range 0 to 297
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbNonPrimRdChnlEnable(struct file *fileHndl, UINT4 wrChnlId,
                                    UINT4 rdChnlId)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    UINT2 rdChnlIndex;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnlList;
    UINT4 lineIntf;
    INT4 result;

    /* retrieve device context and verify validity */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if (pDevCtxt->pCpbCtxt == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* argument checking */
    if((wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
       (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm write channel is in operational state */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *) (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG;
    }

    /* confirm read channel is a non-primary channel and is in the equipped
       state */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);
    rdChnlIndex = pRdChnl->index;
    pRdChnlList = pWrChnl->pRdChnlList;
    if (pRdChnl->primaryFlag == HYPHY20G_CPB_PRIMARY_RD_CHNL ||
        pRdChnl->state != HYPHY20G_CPB_CHNL_EQUIPPED) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG;
    }

    /* determine if read channel is from line interface */
    lineIntf = (rdChnlId < HYPHY20G_CPB_GFPA_PORT_OFFSET) ? 1 : 0;

    /* initialize read pointer */
    result = cpbHyPhy20gCpbInitRdPtr(fileHndl, wrChnlId,
                                     pRdChnlList[rdChnlIndex]);
    if(result) return result;

    /* enable read pointer */
    result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnlList[rdChnlIndex], 1);
    if(result) return result;

    if (lineIntf == 1) {
        /* if read channel is from line interface, enable read port fifo BEFORE
           adding channel to scheduler */
        result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl,
                                               pRdChnlList[rdChnlIndex], 0);
        if(result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if(result) return result;
    }

    /* make sure read state is in operational before calling
       cpbHyPhy20gCPBSchdUpdate */
    pRdChnl->state = HYPHY20G_CPB_CHNL_OPERATIONAL;

    /* update the mask reflecting the interface group this channel is
       associated with */
    if(pRdChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x2;
    }else if(pRdChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x8;
    }else if(pRdChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x20;
    } else {
        intfGrpMsk |= 0x40;
    }

    /* update interface group and QDR schedulers for the added channel(s) */
    result = cpbHyPhy20gCPBSchdUpdate(fileHndl, intfGrpMsk, 1, 1);
    if(result) return result;

    if (lineIntf != 1) {
        /* if read channel is not from line interface, enable read port fifo
           AFTER adding channel to scheduler */
        result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl,
                                               pRdChnlList[rdChnlIndex], 0);
        if(result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if(result) return result;
    }

    /* Check if traffic is going to OTNMA, ensure FIFO_CENTER_EN is disabled
       if the read channel is non-primary */
    if (rdChnlId > 18 && rdChnlId < 147) {
        /* call the following API only if the read channel is mapped to
           GFP slices */
        result = cpbHyPhy20gCpbChnlDestinedToOtnmaChk(fileHndl, wrChnlId, 
                                                      rdChnlId);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbNonPrimRdChnlEnable */

/*******************************************************************************
**
**  hyPhy20gCpbNonPrimRdChnlDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Disable a specific non-primary read channel by turning off
**                  the client port reader and read pointer.  It also removes
**                  the channel from the relevant interface group and updates
**                  the QDR scheduler.
**
**                  The associated write channel must be in operational state
**                  (enabled with hyPhy20gCpbChnlEnable) before this function
**                  can be called.
**
**  INPUTS:         fileHndl  - device handle
**                  wrChnlId  - the write channel ID
**                              Legal range 0 to 293
**                  rdChnlId  - the read channel ID for channel enable
**                              Legal range 0 to 297
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbNonPrimRdChnlDisable(struct file *fileHndl, UINT4 wrChnlId,
                                     UINT4 rdChnlId)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    UINT2 rdChnlIndex;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnlList;
    INT4 result;

    /* retrieve device context and verify validity */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if (pDevCtxt->pCpbCtxt == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* argument checking */
    if((wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
       (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm write channel is in operational state */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *) (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG;
    }

    /* confirm read channel is a non-primary channel and is in the operational
       state */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);
    rdChnlIndex = pRdChnl->index;
    pRdChnlList = pWrChnl->pRdChnlList;
    if (pRdChnl->primaryFlag == HYPHY20G_CPB_PRIMARY_RD_CHNL ||
        pRdChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG;
    }

    /* make sure read state is EQUIPPED before calling
       cpbHyPhy20gCPBSchdUpdate */
    pRdChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    /* update the mask reflecting the interface group this channel is
       associated with */
    if(pRdChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x2;
    }else if(pRdChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x8;
    }else if(pRdChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x20;
    } else {
        intfGrpMsk |= 0x40;
    }

    /* update interface group and QDR schedulers  */
    result = cpbHyPhy20gCPBSchdUpdate(fileHndl, intfGrpMsk, 0, 1);
    if(result) return result;

    /* Disable the read port */
    result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl, pRdChnlList[rdChnlIndex],
                                           1);
    if(result) return result;

    result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
    if(result) return result;

    /* Disable the read channel Pointer */
    result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnlList[rdChnlIndex], 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbNonPrimRdChnlDisable */

/*******************************************************************************
**
**  hyPhy20gCpbPifLnkZnConfig
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configures the fill levels of the Packet Interface Write
**                  Communal FIFO.
**
**  INPUTS:         *fileHndl   - base address
**                  zone1       - FIFO ZONE1 setting
**                                Legal range 0 to 2 ^ 11 (in units of 16 byte
**                                blocks)
**                  zone2       - FIFO ZONE2 setting
**                                Legal range 0 to 2 ^ 11 (in units of 16 byte
**                                blocks)
**                  zone3       - FIFO ZONE3 setting
**                                Legal range 0 to 2 ^ 11 (in units of 16 byte
**                                blocks)
**
**  NOTE:           The specification of ramType is hardware dependent.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbPifLnkZnConfig(struct file *fileHndl, UINT4 zone1, UINT4 zone2,
                               UINT4 zone3)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 addr;
    UINT4 val;

    DBG_PRINT("%s, %s(%u, %u, %u), %d\n",__FILE__, __FUNCTION__, (unsigned int)zone1, (unsigned int)zone2, (unsigned int)zone3, __LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* argument checking */
    if ((zone1 > zone2) || (zone2 > zone3) || (zone3 > 0x7FF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Packet Interface Link Zone Configuration A */
    addr = HYPHY20G_CPB_CPB_REG_PAC_IF_LNK_ZN_CFG_A;
    val = 0;

    val = mHYPHY20G_SET_FIELD4(val, zone1,
    HYPHY20G_CPB_CPB_BIT_PAC_IF_LNK_ZN_CFG_A_PI_WR_LINK_ZONE1_MSK,
    HYPHY20G_CPB_CPB_BIT_PAC_IF_LNK_ZN_CFG_A_PI_WR_LINK_ZONE1_OFF);

    val = mHYPHY20G_SET_FIELD4(val, zone2,
    HYPHY20G_CPB_CPB_BIT_PAC_IF_LNK_ZN_CFG_A_PI_WR_LINK_ZONE2_MSK,
    HYPHY20G_CPB_CPB_BIT_PAC_IF_LNK_ZN_CFG_A_PI_WR_LINK_ZONE2_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* Packet Interface Link Zone Configuration B */
    addr = HYPHY20G_CPB_CPB_REG_PAC_IF_LNK_ZN_CFG_B;
    val = 0;
    val = mHYPHY20G_SET_FIELD4(val, zone3,
    HYPHY20G_CPB_CPB_BIT_PAC_IF_LNK_ZN_CFG_B_PI_WR_LINK_ZONE3_MSK,
    HYPHY20G_CPB_CPB_BIT_PAC_IF_LNK_ZN_CFG_B_PI_WR_LINK_ZONE3_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbPifLnkZnConfig */

/*******************************************************************************
**
**  hyPhy20gCpbChnlRateChng
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Modifies the channel rate associated with a write channel
**                  and any associated read channels.  Optionally interface
**                  group schedulers with or without QDR scheduler may be
**                  updated to reflect the new rate data.
**
**                  For a channel to have the rate modified, the channel
**                  must have been provisioned (internal driver channel state of
**                  HYPHY20G_CPB_CHNL_EQUIPPED or HYPHY20G_CPB_CHNL_OPERATIONAL)
**                  An unprovisioned channel may not have the rate modified
**                  (internal driver channel state of HYPHY20G_CPB_CHNL_START).
**
**                  An unprovisioned write channel will return an error.  An
**                  unprovisioned read channel will not have rate updated and
**                  will not be reported.
**                  
**                  This function has no effect when the CPB scheduler is 
**                  running, so channel rate can't dynamically be changed. 
**                  In order this function make any effect, the respective CPB 
**                  channel should be disabled and then enabled.
**
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID for channel enable
**                                Legal range 0 to 293
**                  chnlRate    - Unitless channel rate to apply to
**                                specified wrChnlId
**                                Legal Range:
**                                1 to 12000
**                                (refer to hyPhy20gCpbChnlProv for details)
**                  upIntfSchd  - Control to regenerate interface group
**                                schedulers
**                                1 - regenerate and apply interface group
**                                    schedulers
**                                0 - do not modify interface group
**                                    schedulers
**                  upQdrSchd   - Control to regenerate QDR scheduler
**                                1 - regenerate and apply QDR scheduler
**                                    update
**                                0 - do not modify QDR scheduler
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlRateChng(struct file *fileHndl, UINT4 wrChnlId,
                             UINT4 chnlRate, UINT4 upIntfSchd, UINT4 upQdrSchd)
{
    INT4 result;
    UINT4 i;
    UINT2 numRdChnls;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnls;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if ((wrChnlId >= HYPHY20G_CPB_NUM_CHNL) || (chnlRate > 12000) ||
        (chnlRate < 1) || (upIntfSchd > 1) || (upQdrSchd > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if(wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    /* Confirm write channel state - HYPHY20G_CPB_CHNL_EQUIPPED or
       HYPHY20G_CPB_CHNL_OPERATIONAL states are required to apply
       the update of rate. */
    if (pWrChnl->state == HYPHY20G_CPB_CHNL_START) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }
    /* Update write channel rate data */
    DBG_PRINT("%s, %s, %d, wrChnl %d initial wrChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)wrChnlId, (unsigned int)pWrChnl->rate);
    pWrChnl->rate = chnlRate;
    DBG_PRINT("%s, %s, %d, wrChnl %d final wrChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)wrChnlId, (unsigned int)pWrChnl->rate);

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* Test read channel state and update if HYPHY20G_CPB_CHNL_EQUIPPED or
       HYPHY20G_CPB_CHNL_OPERATIONAL */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;

        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);
        DBG_PRINT("%s, %s, %d, rdChnl %d initial rdChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRdChnls[i], (unsigned int)p->rate);
        if(p->state != HYPHY20G_CPB_CHNL_START) {
            /* Update read channel rate data */
            p->rate = chnlRate;
        }
        DBG_PRINT("%s, %s, %d, rdChnl %d final rdChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRdChnls[i], (unsigned int)p->rate);

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            intfGrpMsk |= 0x20;
        } else {
            intfGrpMsk |= 0x40;
        }
    }

     /* update the mask reflecting the interface group this channel is
       associated with */
    if(pWrChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x1;
    }else if(pWrChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x4;
    }else if(pWrChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x10;
    }

    /* Update interface group and QDR schedulers for the added channel(s) */
    if (upIntfSchd) {
        DBG_PRINT("%s, %s, %d, intfGrpMsk = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)intfGrpMsk);
        result = cpbHyPhy20gCPBSchdUpdate(fileHndl, intfGrpMsk, 1, upQdrSchd);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlRateChng */

/*******************************************************************************
**
**  hyPhy20gCpbRdChnlRateChng
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Modifies the channel rate associated with a read channel
**
**                  For a channel to have the rate modified, the channel
**                  must have been provisioned (internal driver channel state of
**                  HYPHY20G_CPB_CHNL_EQUIPPED or HYPHY20G_CPB_CHNL_OPERATIONAL)
**                  An unprovisioned channel may not have the rate modified
**                  (internal driver channel state of HYPHY20G_CPB_CHNL_START).
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID
**                                Legal range 0 to 293
**                  rdChnlId    - channel ID of a read channel
**                                Legal range 0 to 296
**                  chnlRate    - Unitless channel rate to apply to
**                                specified rdChnlId
**                                Legal Range:
**                                1 to 12000
**                                (refer to hyPhy20gCpbChnlProv for details)
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbRdChnlRateChng(struct file *fileHndl, UINT4 wrChnlId,
                               UINT4 rdChnlId, UINT4 chnlRate)
{
    INT4 result;
    UINT4 i;
    UINT2 numRdChnls;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnls;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if ((wrChnlId >= HYPHY20G_CPB_NUM_CHNL) ||
        (rdChnlId >= HYPHY20G_CPB_NUM_RD_PORT) || (chnlRate > 12000) ||
        (chnlRate < 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* Confirm write channel state - HYPHY20G_CPB_CHNL_EQUIPPED or
       HYPHY20G_CPB_CHNL_OPERATIONAL states are required to apply
       the update of rate. */
    if (pWrChnl->state == HYPHY20G_CPB_CHNL_START ||
        pRdChnl->state == HYPHY20G_CPB_CHNL_START) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }
    HYPHY20G_ASSERT((pWrChnl->pRdChnlList != NULL),
                     HYPHY20G_ERR_NULL_PTR_PASSED,
                     "%s", "pWrChnl->pRdChnlList is NULL.");

    /* Update read channel rate data */
    pRdChnl->rate = chnlRate;

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* Test read channel state and update if HYPHY20G_CPB_CHNL_EQUIPPED or
       HYPHY20G_CPB_CHNL_OPERATIONAL */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;

        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            intfGrpMsk |= 0x20;
        } else {
            intfGrpMsk |= 0x40;
        }
    }

    /* Update interface group and QDR schedulers */
    result = cpbHyPhy20gCPBSchdUpdate(fileHndl, intfGrpMsk, 1, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbRdChnlRateChng */

/*******************************************************************************
**
**  hyPhy20gCpbIntfRateAwareInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs initialization required for the
**                  operation of channels within an interface group.  The
**                  interface groups are Client, PIF, GFP A and GFP B.
**
**                  Function will set up the read scheduler and the write
**                  scheduler for the interface group.  The PIF has no write
**                  scheduler.
**
**                  Alternatively, this function can be called to disable an
**                  interface group. When disabling an interface group, please
**                  ensure all channels within the interface group are
**                  disabled and deprovisioned.
**
**                  For Line Read and Write interface schedulers, the
**                  maximum scheduler size must be specified.
**
**                  To enable PIF interface when operating in Interlaken packet 
**                  mode in order to support 2 x 10G POS traffic the SCBS needs
**                  special configuration.  This is selected by calling with
**                  enable argument set to 2.  For all other PIF scenarios
**                  call with enable argument set to 1.
**
**
**  INPUTS:         *fileHndl - base address
**                  intfGrp   - 0 - Line
**                              1 - GFP A
**                              2 - GFP B
**                              3 - PIF
**                  rdSchdLength - Read Scheduler Length
**                                  - Legal range 16 for Line, GFP A and
**                                    GFP B schedulers
**                                  - Legal range 1 to 64 for PIF scheduler
**                                  - Value not processed when enable = 0
**                  wrSchdLength - Write Scheduler Length
**                                  - Legal range 32 for Line
**                                  - Legal range 1 to 64 GFP A and GFP B
**                                    schedulers
**                                  - Invalid for PIF scheduler value not
**                                    processed
**                                  - Value not processed when enable = 0
**                  enable    - 2 - enable CPB subsystem interface for PIF
**                                  Interlaken packet mode 2 x 10G POS
**                                  operation.  Valid for intf set to 3.
**                              1 - enable CPB subsystem Interface
**                              0 - disable CPB subsystem Interface
**
**  NOTE:           The specification of ramType is hardware dependent.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbIntfRateAwareInit(struct file *fileHndl, UINT4 intfGrp,
                                  UINT4 rdSchdLength, UINT4 wrSchdLength,
                                  UINT4 enable)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    sHYPHY20G_INTF_GRP_CTXT *pIntfGrp;
    sHYPHY20G_SCHD_CTXT *pSchd;
    UINT4 lowPwrOff = 0;
    UINT4 rdRstOff = 0, wrRstOff = 0, rdSchdNum = 0, wrSchdNum = 0;
    UINT4 schdMode = HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE;
    UINT4 intfRdPortStart, intfRdPortEnd;
    UINT4 intfWrPortStart, intfWrPortEnd;
    UINT4 i;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((rdSchdLength < 1) || (wrSchdLength < 1) ||
        ((rdSchdLength != 16)&&(intfGrp == 0)&&(enable==1)) ||
        ((rdSchdLength > 16)&&(intfGrp == 1)&&(enable==1)) ||
        ((rdSchdLength > 16)&&(intfGrp == 2)&&(enable==1)) ||
        ((rdSchdLength > 64)&&(intfGrp == 3)&&(enable!=0)) ||
        ((wrSchdLength != 32)&&(intfGrp == 0)&&(enable==1)) ||
        ((wrSchdLength > 64)&&(intfGrp == 1)&&(enable==1)) ||
        ((wrSchdLength > 64)&&(intfGrp == 2)&&(enable==1)) ||
        (intfGrp > 3) || (enable > 2) ||
        (intfGrp != 3 && enable == 2)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    pCpbCtxt = pDevCtxt->pCpbCtxt;

    /* confirm scheduler context is valid */
    if(pCpbCtxt->pSchdCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    pSchd = pCpbCtxt->pSchdCtxt;
    pIntfGrp = pCpbCtxt->pIfGrpCtxt+intfGrp;

    /* For Interlaken Packet 2 x 10G POS special config
       specify SCHD_MODE and update enable for usage by
       rest of function */
    if(enable == 2){
        schdMode = HYPHY20G_SCBS_SCHD_BONUS_MODE_ENABLE;
        enable = 1;
    }

    /* Evaluate if adding an interface will prevent existing
       provisioned and enabled channels from supporting
       requested bandwidth */
    if (enable == 1){
        result = cpbHyPhy20gCpbIntfInitEval(fileHndl, intfGrp);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
    }

    /* indentify parameters for the respective intf group scheduler(s) */
    switch(intfGrp){
        case HYPHY20G_CPB_LINE_INTF_GRP:
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_LINE_RD_SCED_RST_OFF;
            wrRstOff = HYPHY20G_CPB_CPB_BIT_CFG_LINE_WR_SCED_RST_OFF;
            /* scheduler instances */
            rdSchdNum = HYPHY20G_CPB_LINE_RD_SCHD;
            wrSchdNum = HYPHY20G_CPB_LINE_WR_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_LINE_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_GFPA_INTF_GRP:
            /* low power mask */
            lowPwrOff = HYPHY20G_CPB_CPB_BIT_LOW_PWR_GFPA_LOWPWR_OFF;
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPA_RD_SCED_RST_OFF;
            wrRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPA_WR_SCED_RST_OFF;
            /* scheduler instances */
            rdSchdNum = HYPHY20G_CPB_GFPA_RD_SCHD;
            wrSchdNum = HYPHY20G_CPB_GFPA_WR_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_GFPB_INTF_GRP:
            /* low power mask */
            lowPwrOff = HYPHY20G_CPB_CPB_BIT_LOW_PWR_GFPB_LOWPWR_OFF;
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPB_RD_SCED_RST_OFF;
            wrRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPB_WR_SCED_RST_OFF;
            /* scheduler instances */
            rdSchdNum = HYPHY20G_CPB_GFPB_RD_SCHD;
            wrSchdNum = HYPHY20G_CPB_GFPB_WR_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_PIF_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_PIF_INTF_GRP:
            /* low power mask */
            lowPwrOff = HYPHY20G_CPB_CPB_BIT_LOW_PWR_PI_LOWPWR_OFF;
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_PI_RD_SCED_RST_OFF;
            /* scheduler instance */
            rdSchdNum = HYPHY20G_CPB_PIF_RD_SCHD;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_PIF_PORT_OFFSET;
            intfRdPortEnd = HYPHY20G_CPB_NUM_RD_PORT;
            intfWrPortEnd = HYPHY20G_CPB_NUM_WR_PORT;
            break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if(enable==1){
        /* bring interface out of low power mode */
        if (intfGrp != HYPHY20G_CPB_LINE_INTF_GRP) {
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_LOW_PWR,
                                         lowPwrOff, 0);
            if(result) return result;
        }

        /* reset and initialize read scheduler */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                     rdRstOff, 1);
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                     rdRstOff, 0);
        if(result) return result;

        pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                        rdSchdNum);
        result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd,
                                          (eHYPHY20G_SCBS_SCHD_ID)rdSchdNum,
                                          rdSchdLength, schdMode);
        if(result) return result;

        /* reset and initialize write scheduler */
        if(intfGrp != 3){
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                         wrRstOff, 1);
            if(result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                         wrRstOff, 0);
            if(result) return result;

            pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                            wrSchdNum);
            result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd,
                                              (eHYPHY20G_SCBS_SCHD_ID)wrSchdNum,
                                              wrSchdLength, schdMode);
            if(result) return result;
        }

        /* initialize read and write ports */
        result = cpbHyPhy20gCpbIntfInitPorts(fileHndl, intfGrp);
        if (result) return result;

        /* enable the new interface group in context */
        pIntfGrp->enable = HYPHY20G_CPB_INTF_ENABLED;


    } else {
        /* check that all write channels within interface are disabled and
           deprovisioned by checking context for HYPHY20G_CPB_CHNL_START and
           HYPHY20G_INVALID_VAL_16BIT states */
        for (i = intfWrPortStart; i < intfWrPortEnd; i++) {
            result = cpbHyPhy20gCpbCheckWrChnlCtxtState(fileHndl, i,
                                                       HYPHY20G_CPB_CHNL_START);
            if(result) return result;
            result = cpbHyPhy20gCpbCheckWrChnlCtxtValid(fileHndl, i,
                                                    HYPHY20G_INVALID_VAL_16BIT);
            if(result) return result;
        }

        /* check that all read channels within interface are disabled and
           deprovisioned by checking context for HYPHY20G_CPB_CHNL_START and
           HYPHY20G_INVALID_VAL_16BIT states */
        for (i = intfRdPortStart; i < intfRdPortEnd; i++) {
            result = cpbHyPhy20gCpbCheckRdChnlCtxtState(fileHndl, i,
                                                       HYPHY20G_CPB_CHNL_START);
            if(result) return result;
            result = cpbHyPhy20gCpbCheckRdChnlCtxtValid(fileHndl, i,
                                                    HYPHY20G_INVALID_VAL_16BIT);
            if(result) return result;
        }

        /* destroy read scheduler context and put in reset */
        pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                        rdSchdNum);
        result = utilHyPhy20gScbsResetSchd(fileHndl, pSchd);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                     rdRstOff, 1);
        if(result) return result;

        /* destroy write scheduler context and put in reset */
        if(intfGrp != 3){
            pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                            wrSchdNum);
            result = utilHyPhy20gScbsResetSchd(fileHndl, pSchd);
            if(result) return result;

            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                         wrRstOff, 1);
            if(result) return result;
        }

        /* put interface into low power mode */
        if (intfGrp != HYPHY20G_CPB_LINE_INTF_GRP) {
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_LOW_PWR,
                                         lowPwrOff, 1);
            if(result) return result;
        }

        /* disable the removed interface group in context */
        pIntfGrp->enable = HYPHY20G_CPB_INTF_DISABLED;
    }

    /* Update interface group context for all interfaces to reflect the
       available rate available to each interface */
    result = cpbHyPhy20gCpbUpdateIntfCtxt(fileHndl);
    if(result) return result;

    DBG_PRINT("Exiting %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbIntfRateAwareInit */

/*******************************************************************************
**
**  hyPhy20gCpbChnlRateAwareProv
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Channel Provision function. To configure a channel, firstly
**                  build a fifo, and then init a write channel as well as
**                  associate it with the fifo, lastly init read channel(s) as
**                  well as associate it to the write channel.
**
**                  This function takes in the chnlRate argument in units of
**                  Mbps.  For example, Channel A requiring 100Mbps bandwidth
**                  should be assigned chnlRate = 100.  Channel B requiring
**                  1Gbps bandwidth, should be assigned chnlRate = 1000.
**
**                  The following should be noted for chnlRate usage:
**
**                  Scheduler weighting algorithm Operational Background:
**                  1) The algorithm will popluate all calendar entries with
**                     channel numbers.
**                   - If there is only one channel all entries will contain
**                     that channel number.
**                   - A channel must occupy at least one calendar entry
**                   - If there are more than one channel of the same rate, some
**                     channels may consume more calendar entries.
**                  2) The algorithm will use chnlRate to determine the relative
**                     bandwidth of all channels to allocate calendar entries.
**                  3) The algorithm will not test to relate the chnlRate with a
**                     true data rate.
**
**                  The resolution of any interface scheduler is a function of:
**                  1) The total bandwidth provided to the interface by the QDR
**                     scheduler
**                  2) The total bandwidth being transferred across the
**                     interface
**                  3) The number of entries configured for the scheduler
**
**                  For example, the interface capacity of GFP A in cases where
**                  multiple channels may be transferred across the interface is
**                  the payload rate of STS-192 or the payload rate of an OPU2.
**                  Consider this 10G.
**
**                  The GFP A scheduler has a maximum of 16 rows for a total of
**                  64 entries.
**
**                  At 10G maximum capacity, a single calendar entry would use
**                  1/64 of all requests or 1/64 * 10G  = .15625 G bandwidth
**                  rate of requests for data.
**
**                  "scheduler entry data request rate" =
**                     "Maximum interface capacity / number of calendar entries"
**
**                  The potential bandwidth of each channel within an
**                  interface can be determined:
**
**                  "potential bandwidth" = "scheduler entry data request rate"
**                   * "number of scheduler entries"
**
**                  Relating the calendar usage of entries with the potential
**                  data rate is left to the higher level calling software.
**
**                  This function takes in the zonex argument.  Zone units are
**                  bursts (48 bytes).  The total number of 48 byte bursts equal
**                  the number of blocks assigned to the FIFO times BLOCK_SIZE
**                  (ie. numBlocks * BLOCK_SIZE).  Refer to hyPhy20gCpbChnlProv
**                  for recommended ZONE3 settings.
**
**                  If flow control is enabled, the difference between ZONE2 and
**                  ZONE3 must be large enough to "catch" all in-transit data
**                  and data that will be transmitted before the flow control
**                  message takes effect at the far end.
**
**                  The zone3Discard argument controls packet discard behavior.
**                  Packet discard when threshold reaches ZONE3 should be
**                  disabled (zone3Discard = 1) for channels carrying CBR
**                  streams.
**
**                  When packet discard is enabled (zone3Discard = 0), ZONE3
**                  recommended settings are:
**
**                      LINE ZONE3 = numBlocks * BLOCK_SIZE - 9
**                      GFP ZONE3  = numBlocks * BLOCK_SIZE - 17
**                      PIF ZONE3  = numBlocks * BLOCK_SIZE - 4
**
**
**  INPUTS:         fileHndl    - device handle
**                  numBlocks   - Number of blocks in RAM to assign to the FIFO
**                  wrChnlId    - The absolute write channel address
**                                Legal range 0 to 293
**                  wrChnlPortWidth- The write channel port width
**                                   0 - non-8b10b (32-bit)
**                                   1 - 8b10b (40-bit)
**                  primRdChnlId- The absolute read channel address  of the
**                                primary read channel.
**                                Legal range 0 to 297
**                  primRdChnlPortWidth- The primary read channel port width
**                                       0 - non-8b10b (32-bit)
**                                       1 - 8b10b (40-bit)
**                  chnlRate    - Desired channel rate in Mbps
**                                Legal range 1 to 12000.
**                  zone1       - FIFO ZONE1 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than ZONE2
**                  zone2       - FIFO ZONE2 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than ZONE3
**                  zone3       - FIFO_ZONE3 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than
**                                numBlocks * BLOCK_SIZE.
**                  pifSysIfType - Format of data to and from PIF
**                                 Valid for wrChnlId and primRdChnlId from
**                                 147 to 293 otherwise set to 0.
**                                 0 - Interlaken transfers with TYPE field
**                                 1 - Interlaken transfers without TYPE field
**                                 2 - eXAUI transfers with TYPE/DUMMY field
**                                 3 - eXAUI transfers without TYPE/DUMMY field
**                                 4 - Standard XAUI transfers without
**                                     TYPE/DUMMY field
**                                 Note: For eXAUI mode pifSysIfType must be set
**                                       the same for all channels.
**                  zone3Discard - 0 - Packet received when channel FIFO in
**                                     ZONE3 will be discarded.
**                                 1 - Packet discarding disabled.  CPB FIFO
**                                     overflows can potentially occur.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_INVALID_WRCHNL_CTXT
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_CHNL_FIFO
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlRateAwareProv(struct file *fileHndl, UINT4 numBlocks,
                                  UINT4 wrChnlId, UINT4 wrChnlPortWidth,
                                  UINT4 primRdChnlId, UINT4 primRdChnlPortWidth,
                                  UINT4 chnlRate, UINT4 zone1, UINT4 zone2,
                                  UINT4 zone3, UINT4 pifSysIfType,
                                  UINT4 zone3Discard)
{
    UINT4 blkSize, fifoId, typePresent, rdChnlValid, wrChnlValid;
    UINT4 pifRdChnlId, off, addr;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_CPB_RAM_CTXT *pRam;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

DBG_PRINT("%s, %s(%u, %u, %u), %d\n",__FILE__, __FUNCTION__, (unsigned int)numBlocks, (unsigned int)wrChnlId, (unsigned int)primRdChnlId, __LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pRam = &(pDevCtxt->pCpbCtxt->ram);
    blkSize = (UINT4)pRam->blkSize;

    /* argument checking */
    if( (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
        (wrChnlPortWidth > 1) ||
        (primRdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) ||
        (primRdChnlPortWidth > 1) ||
        (chnlRate < 1)||(chnlRate > 12000)||
        (zone1 > zone2)||(zone2 > zone3)||(zone3 > (numBlocks*blkSize)) ||
        (zone3Discard > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* argument checking conditional on PIF I/F type */
    if ((wrChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         wrChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL)) ||
        (primRdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         primRdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType > 4) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
        if (pifSysIfType != 0) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    fifoId = wrChnlId;

    pFifo = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + primRdChnlId);

    /* Confirm fifo, write and primary read channel states */
    if( (pFifo->state != HYPHY20G_CPB_CHNL_START) ||
        (pWrChnl->state != HYPHY20G_CPB_CHNL_START) ||
        (pRdChnl->state != HYPHY20G_CPB_CHNL_START) ) {
        return HYPHY20G_ERR_INVALID_STATE;
    }

    HYPHY20G_ASSERT((pWrChnl->pRdChnlList == NULL && pWrChnl->rdChnlCnt == 0),
                    HYPHY20G_ERR_INVALID_WRCHNL_CTXT,
                    "%s", "pWrChnl context is invalid.");

    /* Evaluate if scheduling resources available to allow channel to be
       provisioned */
    /* write channel */
    result = cpbHyPhy20gCpbWrChnlProvEval(fileHndl, wrChnlId, chnlRate);
    if(result) return result;

    /* read channel */
    result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, primRdChnlId, chnlRate);
    if(result) return result;

    /* update FIFO zone context */
    pFifo->zone1 = zone1;
    pFifo->zone2 = zone2;
    pFifo->zone3 = zone3;

    /* Build a FIFO */
    result = cpbHyPhy20gCpbBuildFifo(fileHndl, (UINT2)fifoId, (UINT2)numBlocks,
                                     (UINT2)primRdChnlId);
    if (result) return result;

    /* Bring high or low speed line interface out of low power mode.
       Only check for line interface channels 0-17 */
    if (wrChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
        result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl, wrChnlId,
                                                    &wrChnlValid);
        if (result) return result;
    } else {
        wrChnlValid = 1;
    }
    if (primRdChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
        result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl, primRdChnlId,
                                                    &rdChnlValid);
        if (result) return result;
    } else {
        rdChnlValid = 1;
    }
    if ((wrChnlId == primRdChnlId) && (wrChnlValid == 0) && (rdChnlValid == 0)) {
        result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, wrChnlId, 0);
        if (result) return result;
    } else {
        if (wrChnlValid == 0) {
            result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, wrChnlId, 0);
            if (result) return result;
        }
        if (rdChnlValid == 0) {
            result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, primRdChnlId, 0);
            if (result) return result;
        }
    }

    /* Update port width before configuring client port reader/writer */
    pWrChnl->portWidth = wrChnlPortWidth;
    pRdChnl->portWidth = primRdChnlPortWidth;

    /* Build a write channel */
    result = cpbHyPhy20gCpbConfigWrPort(fileHndl, (UINT2)wrChnlId);
    if (result) return result;

    /* Configure PI_WR_TYPE_PRESENT as a function of interface type */
    if ((wrChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET) &&
        (wrChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET +
                    HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType == 0 || pifSysIfType == 2) {
            typePresent = 1;
        } else if (pifSysIfType == 1 || pifSysIfType == 3 || pifSysIfType == 4) {
            typePresent = 0;
        } else {
            /* Should never get here */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        result = cpbHyPhy20gCpbPifWrPortTypePresent(fileHndl,
                        (wrChnlId - HYPHY20G_CPB_PIF_PORT_OFFSET), typePresent);
        if (result) return result;
    }

    /* Build a primary read channel */
    result = cpbHyPhy20gCpbConfigRdPort(fileHndl, primRdChnlId);
    if (result) return result;

    /* Configure PI_RD_OUTPUT_LENGTH as a function of interface type */
    if ((primRdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET) &&
        (primRdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET +
                         HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType == 0 || pifSysIfType == 2 || pifSysIfType == 3) {
            typePresent = 1;
        } else {
            typePresent = 0;
        }
        /* Set PI_RD_OUTPUT_LENGTH bit */
        pifRdChnlId = primRdChnlId - HYPHY20G_CPB_PIF_PORT_OFFSET;
        off = HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_OUTPUT_LENGTH_OFF;
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(pifRdChnlId);
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, typePresent);
        if(result) return result;
    }

    /* Configure ZONE3 discard */
    result = cpbHyPhy20gCpbZone3DiscardCfg(fileHndl, wrChnlId, zone3Discard);
    if (result) return result;

    /* Firstly allocate memory for the read channel number list */
    pWrChnl->pRdChnlList = (UINT2 *)sysHyPhy20gMemAlloc(sizeof(UINT2));
    if (pWrChnl->pRdChnlList == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* Update FIFO contexts */
    pFifo->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    /* Update read channel context */
    pRdChnl->valid = HYPHY20G_VALID_SIGN;
    pRdChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;
    pRdChnl->rate = chnlRate;
    pRdChnl->index = 0;
    pRdChnl->primaryFlag = HYPHY20G_CPB_PRIMARY_RD_CHNL;

    /* Update write channel context */
    pWrChnl->valid = HYPHY20G_VALID_SIGN;
    pWrChnl->fifo = fifoId;
    pWrChnl->rate = chnlRate;
    pWrChnl->rdChnlCnt = 1;
    pWrChnl->rdChnlListLen = 1;
    (pWrChnl->pRdChnlList)[pRdChnl->index] = primRdChnlId;
    pWrChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlRateAwareProv */

/*******************************************************************************
**
**  hyPhy20gCpbChnlRateAwareEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable a specific channel by turning on client port readers,
**                  read pointers, write pointer and client port writer.
**
**                  It further adds the channels into the relevant interface
**                  group and updates the QDR scheduler.
**
**                  All primary and non-primary readers associated with the
**                  write channel will be enabled.
**
**                  In order to ensure proper reset of resources associated
**                  with a channel once a channel is disabled 
**                  (with hyPhy20gCpbChnlRateAwareDisable), 10 msec must pass 
**                  before this API is called for the same channel.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID for channel enable
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlRateAwareEnable(struct file *fileHndl, UINT4 wrChnlId)
{
    INT4 result;
    UINT4 i;
    UINT2 numRdChnls;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnls;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if(wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    /* Confirm write channel states */
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_EQUIPPED) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* Initialize pointers */
    result = cpbHyPhy20gCpbInitWrPtr(fileHndl, wrChnlId);
    if (result) return result;

    /* Determine primary read channel associated with write channel */
    for (i = 0; i<numRdChnls; i++) {
        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        result = cpbHyPhy20gCpbInitRdPtr(fileHndl, wrChnlId, pRdChnls[i]);
        if (result) return result;
    }


    /* Reset FIFO status level */
    result = cpbHyPhy20gCpbFifoStatusReset(fileHndl, wrChnlId);
    if (result) return result;

    /* Confirm read channel states */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;

        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);
        if(p->state != HYPHY20G_CPB_CHNL_EQUIPPED) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }


    /* Enable read channel pointers */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* Enable the read channel Pointer */
        result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnls[i], 1);
        if (result) return result;

        /* Make sure read state is in operational before calling
           cpbHyPhy20gCPBSchdUpdate */
        p->state = HYPHY20G_CPB_CHNL_OPERATIONAL;

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            intfGrpMsk |= 0x20;
        } else {
            intfGrpMsk |= 0x40;
        }
    }


    /* Enable the write channel Pointer */
    result = cpbHyPhy20gCpbEnableWrPtr(fileHndl, wrChnlId, 1);
    if (result) return result;

    /* update the mask reflecting the interface group this channel is
       associated with */
    if(pWrChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x1;
    }else if(pWrChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x4;
    }else if(pWrChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x10;
    }
    /* Make sure write state is in operational before calling
           cpbHyPhy20gCPBSchdUpdate */
    pWrChnl->state = HYPHY20G_CPB_CHNL_OPERATIONAL;

    /* Update interface group and QDR schedulers for the added channel(s) */
    DBG_PRINT("%s, %s, %d, intfGrpMsk = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)intfGrpMsk);
    result = cpbHyPhy20gCPBSchdRateAwareUpdate(fileHndl, intfGrpMsk, 1, 1);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* Enable the read fifo */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* Enable the read port fifo */
        result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl, pRdChnls[i], 0);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;
    }

    /* Enable the write port fifo */
    result = cpbHyPhy20gCpbWrPortFifoReset(fileHndl, wrChnlId, 0);
    if (result) return result;

    result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
    if (result) return result;
        
    /* Check if traffic is going to OTNMA, ensure FIFO_CENTER_EN is disabled
       if the read channel is non-primary */
    for (i = 0; i < numRdChnls; i++) {
        if (pRdChnls[i] > 18 && pRdChnls[i] < 147) {
            /* call the following API only if the reader is mapped to 
               GFP slices */
            result = cpbHyPhy20gCpbChnlDestinedToOtnmaChk(fileHndl, wrChnlId, 
                                                          pRdChnls[i]);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlRateAwareEnable */

/*******************************************************************************
**
**  hyPhy20gCpbChnlRateAwareRateChng
**  ___________________________________________________________________________
**
**  THIS NEEDS TO BE UPDATED FOR RATE AWARE OPERATION
**  IT WILL NOT TEST IF THE RATE ADJUSTMENT IS GOING TO VIOLATE
**  AVAILABLE QDR BANDWIDTH!!!!!
**
**  DESCRIPTION:    Modifies the channel rate associated with a write channel
**                  and any associated read channels.  Optionally interface
**                  group schedulers with or without QDR scheduler may be
**                  updated to reflect the new rate data.
**
**                  For a channel to have the rate modified, the channel
**                  must have been provisioned (internal driver channel state of
**                  HYPHY20G_CPB_CHNL_EQUIPPED or HYPHY20G_CPB_CHNL_OPERATIONAL)
**                  An unprovisioned channel may not have the rate modified
**                  (internal driver channel state of HYPHY20G_CPB_CHNL_START).
**
**                  An unprovisioned write channel will return an error.  An
**                  unprovisioned read channel will not have rate updated and
**                  will not be reported.
**
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID for channel enable
**                                Legal range 0 to 293
**                  chnlRate    - Unitless channel rate to apply to
**                                specified wrChnlId
**                                Legal Range:
**                                1 to 12000
**                                (refer to hyPhy20gCpbChnlProv for details)
**                  upIntfSchd  - Control to regenerate interface group
**                                schedulers
**                                1 - regenerate and apply interface group
**                                    schedulers
**                                0 - do not modify interface group
**                                    schedulers
**                  upQdrSchd   - Control to regenerate QDR scheduler
**                                1 - regenerate and apply QDR scheduler
**                                    update
**                                0 - do not modify QDR scheduler
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlRateAwareRateChng(struct file *fileHndl, UINT4 wrChnlId,
                                      UINT4 chnlRate, UINT4 upIntfSchd,
                                      UINT4 upQdrSchd)
{
    UINT4 i;
    UINT2 numRdChnls;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnls;
    UINT2 *pRdChnlsOrig;
    UINT4 size, rdLineDone, rdGfpaDone, rdGfpbDone, rdPifDone;
    sHYPHY20G_RD_CHNL_CTXT  *p;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    /* argument checking */
    if ((wrChnlId >= HYPHY20G_CPB_NUM_CHNL) || (chnlRate > 12000) ||
        (chnlRate < 1) || (upIntfSchd > 1) || (upQdrSchd > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if(wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    /* Confirm write channel state - HYPHY20G_CPB_CHNL_EQUIPPED or
       HYPHY20G_CPB_CHNL_OPERATIONAL states are required to apply
       the update of rate. */
    if (pWrChnl->state == HYPHY20G_CPB_CHNL_START) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* evaluate if scheduling resources are present to make change to rate
       for write channel */
    result = cpbHyPhy20gCpbWrChnlProvEval(fileHndl, wrChnlId, chnlRate);
    if(result) return result;

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;


    /* test for illegal pRdChnls[i] data and return error if present */
    for (i = 0; i<numRdChnls; i++) {
        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* allocate temporary storage for contents of pRdChnlList */
    size = sizeof(UINT2)*numRdChnls;
    pRdChnlsOrig = (UINT2 *)sysHyPhy20gMemAlloc(size);
    if (pRdChnlsOrig == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* Test read channel state and update if HYPHY20G_CPB_CHNL_EQUIPPED or
       HYPHY20G_CPB_CHNL_OPERATIONAL */
    for (i = 0; i<numRdChnls; i++) {
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        DBG_PRINT("%s, %s, %d, rdChnl %d initial rdChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRdChnls[i], (unsigned int)p->rate);
        /* temporarily store read channel rate */
        pRdChnlsOrig[i] = p->rate;

        if(p->state != HYPHY20G_CPB_CHNL_START) {
            /* Update read channel rate data */
            p->rate = chnlRate;
        }
        DBG_PRINT("%s, %s, %d, rdChnl %d final rdChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRdChnls[i], (unsigned int)p->rate);

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            intfGrpMsk |= 0x20;
        } else {
            intfGrpMsk |= 0x40;
        }
    }

    /* test each read interface with channels present to see if scheduler
       resources are available to support the change */
    rdLineDone = 0;
    rdGfpaDone = 0;
    rdGfpbDone = 0;
    rdPifDone = 0;

    for(i=0; i < numRdChnls; i++){
        /* identify interface associated with channel */
        if ((pRdChnls[i] < HYPHY20G_CPB_GFPA_PORT_OFFSET)&&(rdLineDone == 0)) {
            result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
            if(result){
                i = numRdChnls;
            }
            rdLineDone = 1;
        } else if ((pRdChnls[i] < HYPHY20G_CPB_GFPB_PORT_OFFSET)&&(rdGfpaDone == 0)) {
            result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
            if(result){
                i = numRdChnls;
            }
            rdGfpaDone = 1;
        } else if ((pRdChnls[i] < HYPHY20G_CPB_PIF_PORT_OFFSET)&&(rdGfpbDone == 0)) {
            result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
            if(result){
                i = numRdChnls;
            }
            rdGfpbDone = 1;
        } else {
            if((rdPifDone == 0)&&(pRdChnls[i] < HYPHY20G_CPB_NUM_WR_PORT)){
                result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
                if(result){
                    i = numRdChnls;
                }
                rdPifDone = 1;
            }
        }

    }

    /* return any error generated in evaluating scheduler availability */
    if(result){
        /* return channel rate to original */
        for(i=0; i < numRdChnls; i++){
            p = (sHYPHY20G_RD_CHNL_CTXT *)
                        (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);
            /* temporarily store read channel rate */
            p->rate = pRdChnlsOrig[i];
        }
        /* deallocate memory */
        sysHyPhy20gMemFree(pRdChnlsOrig);
        /* return error code */
        return result;
    }

    /* Update write channel rate data */
    DBG_PRINT("%s, %s, %d, wrChnl %d initial wrChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)wrChnlId, (unsigned int)pWrChnl->rate);
    pWrChnl->rate = chnlRate;
    DBG_PRINT("%s, %s, %d, wrChnl %d final wrChnl->rate %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)wrChnlId, (unsigned int)pWrChnl->rate);

    /* update the mask reflecting the interface group this channel is
       associated with */
    if(pWrChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x1;
    }else if(pWrChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x4;
    }else if(pWrChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x10;
    }

    /* Update interface group and QDR schedulers for the added channel(s) */
    if (upIntfSchd) {
        DBG_PRINT("%s, %s, %d, intfGrpMsk = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)intfGrpMsk);
        result = cpbHyPhy20gCPBSchdRateAwareUpdate(fileHndl, intfGrpMsk, 1,
                                                   upQdrSchd);
        if(result){
            sysHyPhy20gMemFree(pRdChnlsOrig);
            return result;
        }
    }

    sysHyPhy20gMemFree(pRdChnlsOrig);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlRateAwareRateChng */

/*******************************************************************************
**
**  hyPhy20gCpbChnlRateAwareDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Disable a specific channel by turning off the client port
**                  writer, write pointer, client port readers, and read
**                  pointers.
**
**                  It also logically removes the channel from inteface
**                  group scheduler and reevaluates QDR scheduler.
**
**                  All primary and non-primary readers associated with the
**                  write channel will be disabled.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID for channel disable
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_WRCHNL_CTXT
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbChnlRateAwareDisable(struct file *fileHndl, UINT4 wrChnlId)
{
    UINT4 i, size;
    UINT4 intfGrpMsk = 0;
    UINT2 numRdChnls;
    UINT2 *pRdChnls, *pEquipNonPrimRdChnls;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    INT4 result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Confirm valid fifoId, wrChnlId and numRdChnls have been specified */
    if (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    HYPHY20G_ASSERT((pWrChnl->pRdChnlList != NULL),
                     HYPHY20G_ERR_NULL_PTR_PASSED,
                     "%s", "pWrChnl->pRdChnlList is NULL.");
    if(pWrChnl->rdChnlCnt < 1 ||
       pWrChnl->rdChnlCnt > HYPHY20G_CPB_NUM_RD_PORT) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_WRCHNL_CTXT\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_WRCHNL_CTXT;
    }

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* Confirm write channel state */
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* Confirm read channel states */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;

        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        if(p->state != HYPHY20G_CPB_CHNL_EQUIPPED &&
           p->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_STATE;
        }
    }

    result = cpbHyPhy20gCpbWrPortFifoReset(fileHndl, wrChnlId, 1);
    if(result) return result;

    result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
    if(result) return result;

    /* Make sure write state is EQUIPPED before calling
       cpbHyPhy20gCPBSchdUpdate */
    pWrChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    /* update the mask reflecting the interface group this channel is
       associated with */
    DBG_PRINT("wrChnlId = %u\n", pWrChnl->portId);
    if(pWrChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        DBG_PRINT("wrChnlId intfGrpMsk |= 0x1\n");
        intfGrpMsk |= 0x1;
    }else if(pWrChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        DBG_PRINT("wrChnlId intfGrpMsk |= 0x4\n");
        intfGrpMsk |= 0x4;
    }else if(pWrChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        DBG_PRINT("wrChnlId intfGrpMsk |= 0x10\n");
        intfGrpMsk |= 0x10;
    }

    /* allocate memory to store current state of all read channels associated
       with the write channel */
    size = sizeof(UINT2)*numRdChnls;
    pEquipNonPrimRdChnls = (UINT2 *)sysHyPhy20gMemAlloc(size);
    if (pEquipNonPrimRdChnls == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* initialize entries to zero */
    for (i = 0; i<numRdChnls; i++) {
        pEquipNonPrimRdChnls[i] = 0;
    }

    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* Make sure read state is EQUIPPED before calling
           cpbHyPhy20gCPBSchdUpdate.  Non-primary readers may already be
           in EQUIPPED state. */
        if(p->state == HYPHY20G_CPB_CHNL_OPERATIONAL) {
            p->state = HYPHY20G_CPB_CHNL_EQUIPPED;
        }else if(p->primaryFlag == HYPHY20G_CPB_NON_PRIMARY_RD_CHNL &&
                 p->state ==HYPHY20G_CPB_CHNL_EQUIPPED) {
            pEquipNonPrimRdChnls[i] = 1;
        }else{
            sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_STATE;
        }

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x2\n");
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x8\n");
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x20\n");
            intfGrpMsk |= 0x20;
        } else {
            DBG_PRINT("rdChnlId intfGrpMsk |= 0x40\n");
            intfGrpMsk |= 0x40;
        }
    }


    /* update interface group and QDR schedulers for the added channel(s) */
    DBG_PRINT("%s, %s, %d, intfGrpMsk = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)intfGrpMsk);
    result = cpbHyPhy20gCPBSchdRateAwareUpdate(fileHndl, intfGrpMsk, 0, 1);
    if(result) {
        sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
        return result;
    }


    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        if(pEquipNonPrimRdChnls[i] == 0) {
            /* Disable the read port */
            result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl, pRdChnls[i], 1);
            if(result) {
                sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
                return result;
            }

            result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
            if(result) {
                sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
                return result;
            }
        }
    }

    /* Disable the write channel Pointer */
    result = cpbHyPhy20gCpbEnableWrPtr(fileHndl, wrChnlId, 0);
    if(result) {
        sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
        return result;
    }

    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        if(pEquipNonPrimRdChnls[i] == 0) {
            /* Disable the read channel Pointer */
            result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnls[i], 0);
            if (result) {
                sysHyPhy20gMemFree(pEquipNonPrimRdChnls);
                return result;
            }
        }
    }

    sysHyPhy20gMemFree(pEquipNonPrimRdChnls);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbChnlRateAwareDisable */

/*******************************************************************************
**
**  hyPhy20gCpbNonPrimRdChnlRateAwareProv
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Associate a non primary read channel with a write channel.
**                  This function MUST be used after calling
**                  hyPhy20gCpbChnlRateAwareProv, and MUST be used before the
**                  target channel is enabled by the
**                  hyPhy20gCpbChnlRateAwareEnable function.
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel for channel provision
**                                Legal range 0 to 293
**                  rdChnlId    - channel ID of a non primary read channel
**                                Legal range 0 to 296
**                  rdChnlPortWidth- The read channel port width
**                                   0 - non-8b10b (32-bit)
**                                   1 - 8b10b (40-bit)
**                  pifSysIfType - Format of data to and from PIF
**                                 Valid for rdChnlId from 147 to 293, otherwise
**                                 set to 0.
**                                 0 - Interlaken transfers with TYPE field
**                                 1 - Interlaken transfers without TYPE field
**                                 2 - XAUI transfers with TYPE/DUMMY field
**                                 3 - XAUI transers without TYPE/DUMMY field
**                                 4 - Standard XAUI transfers without
**                                     TYPE/DUMMY field
**                                 Note: For eXAUI mode pifSysIfType must be set
**                                       the same for all channels.
**
**  OUTPUTS:        NONE
**
**  SIDE_EFFECTS:   The read channel id list in the write channel context
**                  The non primary read channel context
**                  Client port and read pointer register of this non primary
**                  read channel are configured.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbNonPrimRdChnlRateAwareProv(struct file *fileHndl,
                                           UINT4 wrChnlId, UINT4 rdChnlId,
                                           UINT4 rdChnlPortWidth,
                                           UINT4 pifSysIfType)
{
    UINT4 rdChnlValid, rdChnlRate;
    UINT4 pifRdChnlId, off, addr, typePresent;
    UINT4 i;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Confirm valid fifoId, wrChnlId and numRdChnls have been specified */
    if( (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
        (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) ||
        (rdChnlPortWidth > 1) ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* argument checking conditional on PIF I/F type */
    if ((wrChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         wrChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL)) ||
        (rdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET &&
         rdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET + HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType > 4) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
        if (pifSysIfType != 0) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* Confirm write and non-primary read channel states */
    if(pWrChnl->valid != HYPHY20G_VALID_SIGN ||
       pRdChnl->state != HYPHY20G_CPB_CHNL_START)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }

    HYPHY20G_ASSERT((pWrChnl->pRdChnlList != NULL),
                     HYPHY20G_ERR_NULL_PTR_PASSED,
                     "%s", "pWrChnl->pRdChnlList is NULL.");

    /* Evaluate if scheduling resources available to allow channel to be
       provisioned */
    /* read channel */
    rdChnlRate = pWrChnl->rate;
    result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, rdChnlId, rdChnlRate);
    if(result) return result;

    /* Bring high or low speed line interface out of low power mode.
       Only check line interface channels 0-17 */
    if (rdChnlId < (HYPHY20G_CPB_LINE_MAX_NUM_CHNL - 1)) {
        result = cpbHyPhy20gCpbChkChnlORdValidState(fileHndl, rdChnlId,
                                                    &rdChnlValid);
        if (result) return result;
        if (rdChnlValid == 0) {
            result = cpbHyPhy20gCpbLineIntfLowPwrMode(fileHndl, rdChnlId, 0);
            if (result) return result;
        }
    }

    /* Update port width before configuring the client port reader */
    pRdChnl->portWidth = rdChnlPortWidth;
    pRdChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;
    pRdChnl->primaryFlag = HYPHY20G_CPB_NON_PRIMARY_RD_CHNL;

    /* Build a non-primary read channel */
    result = cpbHyPhy20gCpbConfigRdPort(fileHndl, rdChnlId);
    if (result) return result;

    /* Configure PI_RD_OUTPUT_LENGTH as a function of interface type */
    if ((rdChnlId >= HYPHY20G_CPB_PIF_PORT_OFFSET) &&
        (rdChnlId < (HYPHY20G_CPB_PIF_PORT_OFFSET +
                    HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL))) {
        if (pifSysIfType == 0 || pifSysIfType == 2 || pifSysIfType == 3) {
            typePresent = 1;
        } else {
            typePresent = 0;
        }
        /* Set PI_RD_OUTPUT_LENGTH bit */
        pifRdChnlId = rdChnlId - HYPHY20G_CPB_PIF_PORT_OFFSET;
        off = HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_OUTPUT_LENGTH_OFF;
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(pifRdChnlId);
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, typePresent);
        if(result) return result;
    }

    /* Increase the length of read channel ID list if it is necessary,
       and update channel context */
    if (pWrChnl->rdChnlCnt + 1 > pWrChnl->rdChnlListLen) {
        UINT2 *p;
        UINT2 len;
        len = pWrChnl->rdChnlListLen + HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN;
        p = (UINT2 *)sysHyPhy20gMemAlloc(len*sizeof(UINT2));

        if (p == NULL) {
            pRdChnl->state = HYPHY20G_CPB_CHNL_START;
            return HYPHY20G_ERR_MEM_ALLOC;
        }

        sysHyPhy20gMemCpy(p, pWrChnl->pRdChnlList, (pWrChnl->rdChnlCnt)*sizeof(UINT2));
        sysHyPhy20gMemFree(pWrChnl->pRdChnlList);
        pWrChnl->pRdChnlList = p;
        pWrChnl->rdChnlListLen += HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN;
    }
    pRdChnl->index = pWrChnl->rdChnlCnt++;
    (pWrChnl->pRdChnlList)[pRdChnl->index] = rdChnlId;
    pRdChnl->rate = pWrChnl->rate;
    pRdChnl->valid = HYPHY20G_VALID_SIGN;

    /* DEBUG */
    for (i = 0; i < (pWrChnl->rdChnlCnt); i++) {
        DBG_PRINT("DEBUG_JC: rdChnlList[%lu] = %u\n", i, (pWrChnl->pRdChnlList)[i]);
    }
    /* END DEBUG */

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbNonPrimRdChnlRateAwareProv */

/*******************************************************************************
**
**  hyPhy20gCpbNonPrimRdChnlRateAwareEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable a specific non-primary read channel by turning on
**                  client port reader and read pointer.  It also adds the read
**                  channel into the relevant interface group and updates the
**                  QDR scheduler.
**
**                  Before the non-primary read channel can be enabled:
**                      1) the associated write channel must first be enabled
**                         with hyPhy20gCpbChnlRateAwareEnable
**                      2) non-primary read channel must be provisioned with
**                         hyPhy20gCpbNonPrimRdChnlRateAwareProv
**
**  INPUTS:         fileHndl  - device handle
**                  wrChnlId  - the write channel ID
**                              Legal range 0 to 293
**                  rdChnlId  - the read channel ID for channel enable
**                              Legal range 0 to 297
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbNonPrimRdChnlRateAwareEnable(struct file *fileHndl,
                                             UINT4 wrChnlId, UINT4 rdChnlId)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    UINT2 rdChnlIndex;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnlList;
    UINT4 lineIntf;
    INT4 result;

    /* retrieve device context and verify validity */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if (pDevCtxt->pCpbCtxt == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* argument checking */
    if((wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
       (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm write channel is in operational state */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *) (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG;
    }

    /* confirm read channel is a non-primary channel and is in the equipped
       state */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);
    rdChnlIndex = pRdChnl->index;
    pRdChnlList = pWrChnl->pRdChnlList;
    if (pRdChnl->primaryFlag == HYPHY20G_CPB_PRIMARY_RD_CHNL ||
        pRdChnl->state != HYPHY20G_CPB_CHNL_EQUIPPED) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG;
    }

    /* determine if read channel is from line interface */
    lineIntf = (rdChnlId < HYPHY20G_CPB_GFPA_PORT_OFFSET) ? 1 : 0;

    /* initialize read pointer */
    result = cpbHyPhy20gCpbInitRdPtr(fileHndl, wrChnlId,
                                     pRdChnlList[rdChnlIndex]);
    if(result) return result;

    /* enable read pointer */
    result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnlList[rdChnlIndex], 1);
    if(result) return result;

    if (lineIntf == 1) {
        /* if read channel is from line interface, enable read port fifo BEFORE
           adding channel to scheduler */
        result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl,
                                               pRdChnlList[rdChnlIndex], 0);
        if(result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if(result) return result;
    }

    /* make sure read state is in operational before calling
       cpbHyPhy20gCPBSchdUpdate */
    pRdChnl->state = HYPHY20G_CPB_CHNL_OPERATIONAL;

    /* update the mask reflecting the interface group this channel is
       associated with */
    if(pRdChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x2;
    }else if(pRdChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x8;
    }else if(pRdChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x20;
    } else {
        intfGrpMsk |= 0x40;
    }

    /* update interface group and QDR schedulers for the added channel(s) */
    result = cpbHyPhy20gCPBSchdRateAwareUpdate(fileHndl, intfGrpMsk, 1, 1);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    if (lineIntf != 1) {
        /* if read channel is not from line interface, enable read port fifo
           AFTER adding channel to scheduler */
        result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl,
                                               pRdChnlList[rdChnlIndex], 0);
        if(result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if(result) return result;
    }
    
     /* Check if traffic is going to OTNMA, ensure FIFO_CENTER_EN is disabled
       if the read channel is non-primary */
    if (rdChnlId > 18 && rdChnlId < 147) {
        /* call the following API only if the reader is mapped to GFP slices */
        result = cpbHyPhy20gCpbChnlDestinedToOtnmaChk(fileHndl, wrChnlId, 
                                                      rdChnlId);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbNonPrimRdChnlRateAwareEnable */

/*******************************************************************************
**
**  hyPhy20gCpbNonPrimRdChnlRateAwareDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Disable a specific non-primary read channel by turning off
**                  the client port reader and read pointer.  It also removes
**                  the channel from the relevant interface group and updates
**                  the QDR scheduler.
**
**                  The associated write channel must be in operational state
**                  (enabled with hyPhy20gCpbChnlEnable) before this function
**                  can be called.
**
**  INPUTS:         fileHndl  - device handle
**                  wrChnlId  - the write channel ID
**                              Legal range 0 to 293
**                  rdChnlId  - the read channel ID for channel enable
**                              Legal range 0 to 297
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbNonPrimRdChnlRateAwareDisable(struct file *fileHndl,
                                              UINT4 wrChnlId, UINT4 rdChnlId)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    UINT2 rdChnlIndex;
    UINT4 intfGrpMsk = 0;
    UINT2 *pRdChnlList;
    INT4 result;

    /* retrieve device context and verify validity */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if (pDevCtxt->pCpbCtxt == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* argument checking */
    if((wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) ||
       (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm write channel is in operational state */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *) (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    if (pWrChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG;
    }

    /* confirm read channel is a non-primary channel and is in the operational
       state */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);
    rdChnlIndex = pRdChnl->index;
    pRdChnlList = pWrChnl->pRdChnlList;
    if (pRdChnl->primaryFlag == HYPHY20G_CPB_PRIMARY_RD_CHNL ||
        pRdChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG;
    }

    /* make sure read state is EQUIPPED before calling
       cpbHyPhy20gCPBSchdUpdate */
    pRdChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;

    /* update the mask reflecting the interface group this channel is
       associated with */
    if(pRdChnl->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
        intfGrpMsk |= 0x2;
    }else if(pRdChnl->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
        intfGrpMsk |= 0x8;
    }else if(pRdChnl->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
        intfGrpMsk |= 0x20;
    } else {
        intfGrpMsk |= 0x40;
    }

    /* update interface group and QDR schedulers  */
    result = cpbHyPhy20gCPBSchdRateAwareUpdate(fileHndl, intfGrpMsk, 0, 1);
    if(result) return result;

    /* Disable the read port */
    result = cpbHyPhy20gCpbRdPortFifoReset(fileHndl, pRdChnlList[rdChnlIndex],
                                           1);
    if(result) return result;

    result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
    if(result) return result;

    /* Disable the read channel Pointer */
    result = cpbHyPhy20gCpbEnableRdPtr(fileHndl, pRdChnlList[rdChnlIndex], 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbNonPrimRdChnlRateAwareDisable */

/*******************************************************************************
**
**  hyPhy20gCpbRdChnlRateAwareRateChng
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Modifies the channel rate associated with a read channel
**
**                  For a channel to have the rate modified, the channel
**                  must have been provisioned (internal driver channel state of
**                  HYPHY20G_CPB_CHNL_EQUIPPED or HYPHY20G_CPB_CHNL_OPERATIONAL)
**                  An unprovisioned channel may not have the rate modified
**                  (internal driver channel state of HYPHY20G_CPB_CHNL_START).
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - the write channel ID
**                                Legal range 0 to 293
**                  rdChnlId    - channel ID of a read channel
**                                Legal range 0 to 296
**                  chnlRate    - Unitless channel rate to apply to
**                                specified rdChnlId
**                                Legal Range:
**                                1 to 12000
**                                (refer to hyPhy20gCpbChnlRateAwareProv for
**                                 details)
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**
*******************************************************************************/
INT4 hyPhy20gCpbRdChnlRateAwareRateChng(struct file *fileHndl, UINT4 wrChnlId,
                                        UINT4 rdChnlId, UINT4 chnlRate)
{
    UINT2 *pRdChnls;
    UINT2 numRdChnls;
    UINT4 i, rdChnlOrig, rdLineDone, rdGfpaDone, rdGfpbDone, rdPifDone;
    UINT4 intfGrpMsk = 0;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result = HYPHY20G_SUCCESS;

    /* argument checking */
    if ((wrChnlId >= HYPHY20G_CPB_NUM_CHNL) ||
        (rdChnlId >= HYPHY20G_CPB_NUM_RD_PORT) || (chnlRate > 12000) ||
        (chnlRate < 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* Confirm write channel state - HYPHY20G_CPB_CHNL_EQUIPPED or
       HYPHY20G_CPB_CHNL_OPERATIONAL states are required to apply
       the update of rate. */
    if (pWrChnl->state == HYPHY20G_CPB_CHNL_START ||
        pRdChnl->state == HYPHY20G_CPB_CHNL_START) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_STATE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_STATE;
    }
    HYPHY20G_ASSERT((pWrChnl->pRdChnlList != NULL),
                     HYPHY20G_ERR_NULL_PTR_PASSED,
                     "%s", "pWrChnl->pRdChnlList is NULL.");

    /* temporarily store original read channel rate */
    rdChnlOrig = pRdChnl->rate;

    /* Update read channel rate data */
    pRdChnl->rate = chnlRate;

    numRdChnls = pWrChnl->rdChnlCnt;
    pRdChnls = pWrChnl->pRdChnlList;

    /* Update interface group mask */
    for (i = 0; i<numRdChnls; i++) {
        sHYPHY20G_RD_CHNL_CTXT  *p;

        if(pRdChnls[i] > HYPHY20G_CPB_NUM_RD_PORT - 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        p = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + pRdChnls[i]);

        /* update the mask reflecting the interface group this channel is
           associated with */
        if(p->portId < HYPHY20G_CPB_GFPA_PORT_OFFSET){
            intfGrpMsk |= 0x2;
        }else if(p->portId < HYPHY20G_CPB_GFPB_PORT_OFFSET){
            intfGrpMsk |= 0x8;
        }else if(p->portId < HYPHY20G_CPB_PIF_PORT_OFFSET){
            intfGrpMsk |= 0x20;
        } else {
            intfGrpMsk |= 0x40;
        }
    }

    /* test each read interface with channels present to see if scheduler
       resources are available to support the change */
    rdLineDone = 0;
    rdGfpaDone = 0;
    rdGfpbDone = 0;
    rdPifDone = 0;

    for(i=0; i < numRdChnls; i++){
        /* identify interface associated with channel */
        if ((pRdChnls[i] < HYPHY20G_CPB_GFPA_PORT_OFFSET)&&(rdLineDone == 0)) {
            result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
            if(result){
                i = numRdChnls;
            }
            rdLineDone = 1;
        } else if ((pRdChnls[i] < HYPHY20G_CPB_GFPB_PORT_OFFSET)&&(rdGfpaDone == 0)) {
            result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
            if(result){
                i = numRdChnls;
            }
            rdGfpaDone = 1;
        } else if ((pRdChnls[i] < HYPHY20G_CPB_PIF_PORT_OFFSET)&&(rdGfpbDone == 0)) {
            result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
            if(result){
                i = numRdChnls;
            }
            rdGfpbDone = 1;
        } else {
            if((rdPifDone == 0)&&(pRdChnls[i] < HYPHY20G_CPB_NUM_WR_PORT)){
                result = cpbHyPhy20gCpbRdChnlProvEval(fileHndl, pRdChnls[i], chnlRate);
                if(result){
                    i = numRdChnls;
                }
                rdPifDone = 1;
            }
        }

    }

    /* return any error generated in evaluating scheduler availability */
    if(result){
        /* return channel rate to original */
        pRdChnl->rate = rdChnlOrig;
        /* return error code */
        return result;
    }

    /* Update interface group and QDR schedulers */
    result = cpbHyPhy20gCPBSchdRateAwareUpdate(fileHndl, intfGrpMsk, 1, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbRdChnlRateAwareRateChng */

/*******************************************************************************
**
**  hyPhy20gCpbFifoResize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function dynamically resizes the circular FIFO for the
**                  specified write channel.
**
**                  Prior to calling this function, the channel must have been
**                  enabled with hyPhy20gCpbChnlEnable to bring it into
**                  operational state.
**
**                  For failure modes HYPHY20G_ERR_ADD_BLOCK and
**                  HYPHY20G_ERR_DEL_BLOCK, these may occur if there is no
**                  incoming data flow to the CPB fifo.  If this occurs, ensure
**                  that upstream data path is not disabled, and repeat
**                  add/delete till success.
**
**                  This function takes in the zonex argument.  Zone units are
**                  bursts (48 bytes).  The total number of 48 byte bursts equal
**                  the number of blocks assigned to the FIFO times BLOCK_SIZE
**                  (ie. numBlocks * BLOCK_SIZE).  Refer to hyPhy20gCpbChnlProv
**                  for recommended ZONE3 settings.  Note that zonex will only
**                  be applied if the addition/deletion of a block is
**                  successful.
**
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - The absolute write channel address
**                                Legal range 0 to 293
**                  addDel      - add or delete a block from FIFO
**                                0 - delete a block
**                                1 - add a block
**                  zone1       - FIFO ZONE1 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than ZONE2
**                  zone2       - FIFO ZONE2 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than ZONE3
**                  zone3       - FIFO_ZONE3 setting (refer to DESCRIPTION)
**                                Must be set such that it is less than
**                                FIFO size times BLOCK_SIZE.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_CHNL_FIFO
**                  HYPHY20G_ERR_REQ_BUSY
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_ADD_BLOCK
**                  HYPHY20G_ERR_DEL_BLOCK
**
*******************************************************************************/
INT4 hyPhy20gCpbFifoResize(struct file *fileHndl, UINT4 wrChnlId, UINT4 addDel,
                           UINT4 zone1, UINT4 zone2, UINT4 zone3)
{
    sHYPHY20G_CTXT          *pDevCtxt;
    sHYPHY20G_CPB_RAM_CTXT  *pRAM;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;

    UINT4 fifoId, numBlks, blkSize;
    UINT4 data[3];
    UINT4 retry, retryCnt;
    UINT4 maxRetryCnt = 20;
    UINT4 newNumBlks;
    INT4  result = HYPHY20G_SUCCESS;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context and CPB context are valid */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pCpbCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    /* argument check */
    if ((wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) || (addDel > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check wrChnlId is in operational state */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    if ((pWrChnl->state != HYPHY20G_CPB_CHNL_OPERATIONAL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    fifoId = wrChnlId;
    pFifo = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);
    pRAM = &(pDevCtxt->pCpbCtxt->ram);

    blkSize = (UINT4)pRAM->blkSize;
    numBlks = (UINT4)pFifo->numBlks;

    newNumBlks = (addDel == 0) ? (numBlks-1) : (numBlks+1);

    /* check zonex */
    if ((zone1 > zone2)||(zone2 > zone3)||(zone3 > (newNumBlks*blkSize))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    retry = 1;
    retryCnt = 0;
    while (retry == 1 && retryCnt < maxRetryCnt)
    {
        if (addDel) {
            /* Add block */
            result = cpbHyPhy20gCpbFifoAddBlk(fileHndl, fifoId);
        } else {
            /* Delete block */
            result = cpbHyPhy20gCpbFifoDelBlk(fileHndl, fifoId);
        }
        if(result) {
            retry = 1;
            retryCnt++;
        } else {
            retry = 0;
        }
    }
    if(result) return result;

    /* Configure FIFO_MAX_DEPTH bits */
    result = cpbHyPhy20gFifoDepthIndRead(fileHndl, fifoId,
                                         &data[0], &data[1],&data[2]);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&data[0],
               HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_FIFO_MAX_DEPTH_MSK,
               (pFifo->numBlks * pRAM->blkSize));
    if (result) return result;

    /* Configure ZONE1, ZONE2 and ZONE3 */
    result = hyPhy20gVarFieldWrite(&data[1],
               HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE1_MSK, zone1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&data[1],
               HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_1_MSK, zone2);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&data[2],
               HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_2_MSK,
               (zone2 >> 12));
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&data[2],
               HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE3_MSK, zone3);
    if (result) return result;

    result = cpbHyPhy20gFifoDepthIndWrite(fileHndl, fifoId,
                                          data[0], data[1], data[2]);
    if (result) return result;

    pFifo->zone1 = zone1;
    pFifo->zone2 = zone2;
    pFifo->zone3 = zone3;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbFifoResize */

/*******************************************************************************
**
**  hyPhy20gCpbGfpPauseInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes and configures global CPB-GFP
**                  parameters.  GFP_SA_BASE is configured in this function.
**
**                  To properly configure the source address of Far End pause
**                  frames, hyPhy20gCpbGfpChnlPauseSrcAddrCfg must be called.
**
**  INPUTS:         fileHndl     - device handle
**                  gfpSABase[2] - The common component of source address (SA)
**                                 inserted in Far End pause frames.
**                                 If MAC address is aa:bb:cc:dd:ee:ff
**                                   gfpSABase[1] = aa:bb
**                                   gfpSABase[0] = cc:dd:ee:ff
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gCpbGfpPauseInit(struct file *fileHndl, UINT4 gfpSABase[2])
{
    UINT4 reg, msk;
    UINT4 crc32;
    UINT4 *pCrc32Table;
    INT4 result;

    /* argument check */
    if (gfpSABase[0] > 0xFFFFFFFF || gfpSABase[1] > 0xFFFF)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = HYPHY20G_CPB_CPB_REG_GFP_FAR_END_PAUSE_BAS_SRC_MAC_ADR_1;
    msk = HYPHY20G_CPB_CPB_BIT_GFP_FAR_END_PAUSE_BAS_SRC_MAC_ADR_1_GFP_SA_BASE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, gfpSABase[0]);
    if(result) return result;

    reg = HYPHY20G_CPB_CPB_REG_GFP_FAR_END_PAUSE_BAS_SRC_MAC_ADR_2;
    msk = HYPHY20G_CPB_CPB_BIT_GFP_FAR_END_PAUSE_BAS_SRC_MAC_ADR_2_GFP_SA_BASE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, gfpSABase[1]);
    if(result) return result;

    /* allocate memory for CRC32 table */
    pCrc32Table = (UINT4 *)sysHyPhy20gMemAlloc(sizeof(UINT4) * 256);
    if (pCrc32Table == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* initialize CRC32 table */
    result = cpbHyPhy20gCpbCrc32TableInit(fileHndl, pCrc32Table);
    if(result) {
        sysHyPhy20gMemFree(pCrc32Table);
        return result;
    }

    /* calculate FCS */
    result = cpbHyPhy20gCpbCalcPauseFrmCrc32(fileHndl, gfpSABase, pCrc32Table,
                                             &crc32);
    if(result) {
        sysHyPhy20gMemFree(pCrc32Table);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pCrc32Table);

    /* write crc32 */
    reg = HYPHY20G_CPB_CPB_REG_GFP_FAR_END_PAUSE_FCS;
    msk = HYPHY20G_CPB_CPB_BIT_GFP_FAR_END_PAUSE_FCS_GFP_FCS_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, crc32);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbGfpPauseInit */


/*******************************************************************************
**
**  hyPhy20gCpbGfpIntfPauseInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes and configures the Tx portion
**                  of a CPB-GFP slice.
**
**  INPUTS:         fileHndl    - device handle
**                  slice       - 0 - Slice 'A'
**                              - 1 - Slice 'B'
**                  mode        - Interface GFP is transported over
**                                0 - SONET/SDH
**                                1 - OTN
**                  pauseTime   - Length of time to stop sending traffic when
**                                a pause command is received from the Far End
**                                device.  Units in 0.5ms.
**                                Default 0x47 (35.5ms)
**                  repeatTime  - Length of time between transmission of pause
**                                frames to the Far End device.  Units in 0.5ms.
**                                Default 0x40 (32ms)
**                  startZone   - FIFO zone level that will trigger transmission
**                                of pause frames to the Far End device.
**                                Default b'10
**                  endZone     - FIFO zone level that will trigger transmission
**                                of an unpause frame to the Far End device.
**                                Default b'00
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbGfpIntfPauseInit(struct file *fileHndl, UINT4 slice,
                                 UINT4 mode, UINT4 pauseTime, UINT4 repeatTime,
                                 UINT4 startZone, UINT4 endZone)
{
    UINT4 preScalerReg, preScalerMsk, preScalerVal;
    UINT4 cfgReg, neTimeMsk, feRptMsk, feStrtZnMsk, feEndZnMsk;
    UINT4 val;
    INT4 result;

    /* argument check */
    if (slice > 1 || mode > 1 || pauseTime > 0x7F || repeatTime > 0x7F ||
        startZone > 0x3 || endZone > 0x3)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
    case 0:
        preScalerReg = HYPHY20G_CPB_CPB_REG_GFPA_PAUSE_PRESCALER;
        preScalerMsk = HYPHY20G_CPB_CPB_BIT_GFPA_PAUSE_PRESCALER_GFPA_PRESCALER_MSK;

        cfgReg = HYPHY20G_CPB_CPB_REG_GFPA_PAUSE_CFG;
        neTimeMsk = HYPHY20G_CPB_CPB_BIT_GFPA_PAUSE_CFG_GFPA_NE_TIME_MSK;
        feRptMsk = HYPHY20G_CPB_CPB_BIT_GFPA_PAUSE_CFG_GFPA_FE_REPEAT_MSK;
        feStrtZnMsk = HYPHY20G_CPB_CPB_BIT_GFPA_PAUSE_CFG_GFPA_FE_START_ZONE_MSK;
        feEndZnMsk = HYPHY20G_CPB_CPB_BIT_GFPA_PAUSE_CFG_GFPA_FE_END_ZONE_MSK;
        break;
    case 1:
        preScalerReg = HYPHY20G_CPB_CPB_REG_GFPB_PAUSE_PRESCALER;
        preScalerMsk = HYPHY20G_CPB_CPB_BIT_GFPB_PAUSE_PRESCALER_GFPB_PRESCALER_MSK;

        cfgReg = HYPHY20G_CPB_CPB_REG_GFPB_PAUSE_CFG;
        neTimeMsk = HYPHY20G_CPB_CPB_BIT_GFPB_PAUSE_CFG_GFPB_NE_TIME_MSK;
        feRptMsk = HYPHY20G_CPB_CPB_BIT_GFPB_PAUSE_CFG_GFPB_FE_REPEAT_MSK;
        feStrtZnMsk = HYPHY20G_CPB_CPB_BIT_GFPB_PAUSE_CFG_GFPB_FE_START_ZONE_MSK;
        feEndZnMsk = HYPHY20G_CPB_CPB_BIT_GFPB_PAUSE_CFG_GFPB_FE_END_ZONE_MSK;
        break;
    default:
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Sonet/SDH = 0x25F7F */
    /* OTN = 0x28DA2 */
    preScalerVal = (mode == 0) ? 0x25F7F : 0x28DA2;


    result = hyPhy20gRegFieldWrite(fileHndl, preScalerReg, preScalerMsk,
                                   preScalerVal);
    if(result) return result;


    result = sysHyPhy20gRead(fileHndl, cfgReg, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val, neTimeMsk, pauseTime);
    if(result) return result;
    result = hyPhy20gVarFieldWrite(&val, feRptMsk, repeatTime);
    if(result) return result;
    result = hyPhy20gVarFieldWrite(&val, feStrtZnMsk, startZone);
    if(result) return result;
    result = hyPhy20gVarFieldWrite(&val, feEndZnMsk, endZone);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, cfgReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbGfpIntfPauseInit */

/*******************************************************************************
**
**  hyPhy20gCpbGfpChnlPauseSrcAddrCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Source MAC Address (SA) in Far
**                  End pause frames and is only applicable to CPB channels
**                  mapped to GFP in GFP-F mode.  GFP_SA_OFFSET is configured
**                  in this function.
**
**                  The final source address, SA[47:0], is GFP_SA_BASE[47:24]
**                  concatenated with GFP_SA_BASE[23:0] + GFPA_SA_OFFSET.
**                  hyPhy20gCpbGfpPauseInit configures GFP_SA_BASE.
**
**  INPUTS:         fileHndl    - device handle
**                  slice       - 0 - Slice 'A'
**                              - 1 - Slice 'B'
**                  chnlId      - GFP channel for pause frame configuration.
**                                Legal range 0 to 63.
**                  gfpSaOffset - Per channel component of SA inserted
**                                in Far End pause frames.
**                                Legal range 0 to 0x7F.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbGfpChnlPauseSrcAddrCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 chnlId, UINT4 gfpSaOffset)
{
    UINT4 saOffsetMsk;
    UINT4 reg, val;
    INT4 result;

    /* argument checking */
    if (slice > 1 || chnlId > 63 || gfpSaOffset > 0x7F)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId, slice);
    saOffsetMsk = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_SA_OFFSET_MSK;

    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val, saOffsetMsk, gfpSaOffset);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbGfpChnlPauseSrcAddrCfg */

/*******************************************************************************
**
**  hyPhy20gCpbGfpChnlFarEndPauseEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables the auto generation of
**                  Pause Frames to the Far End device and is only applicable
**                  to CPB channels mapped to GFP in GFP-F mode.
**
**                  Far End Pause generation must be ENABLED BEFORE the channel
**                  is enabled with hyPhy20gCpbChnlEnable, and it must be
**                  DISABLED BEFORE channel is disabled with
**                  hyPhy20gCpbChnlDisable.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - Slice 'A'
**                            - 1 - Slice 'B'
**                  chnlId    - GFP channel
**                              Legal range 0 to 63
**                  enable    - 0 - disable
**                              1 - enable
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbGfpChnlFarEndPauseEnable(struct file *fileHndl, UINT4 slice,
                                         UINT4 chnlId, UINT4 enable)
{
    UINT4 pauseEnReg[2], regIdx, chnlBitOff, pauseEnMsk, pauseEnVal;
    UINT4 reg, val;
    INT4 result;

    /* argument checking */
    if (slice > 1 || chnlId > 63 || enable > 1)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            pauseEnReg[0] = HYPHY20G_CPB_CPB_REG_GFPA_FAR_END_PAUSE_EN_1;
            pauseEnReg[1] = HYPHY20G_CPB_CPB_REG_GFPA_FAR_END_PAUSE_EN_2;
            break;
        case 1:
            pauseEnReg[0] = HYPHY20G_CPB_CPB_REG_GFPB_FAR_END_PAUSE_EN_1;
            pauseEnReg[1] = HYPHY20G_CPB_CPB_REG_GFPB_FAR_END_PAUSE_EN_2;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    regIdx = (chnlId < 32) ? 0 : 1;
    chnlBitOff = chnlId - (32*(chnlId/32));

    reg = pauseEnReg[regIdx];
    pauseEnMsk = HYPHY20G_CPB_CPB_BIT_GFPA_FAR_END_PAUSE_EN_1_GFPA_FE_ENABLE_MSK;

    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if(result) return result;

    if (enable == 1) {
        pauseEnVal = val | (0x1 << chnlBitOff);
    } else {
        pauseEnVal = val & ~(0x1 << chnlBitOff);
    }

    result = hyPhy20gVarFieldWrite(&val, pauseEnMsk, pauseEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbGfpChnlFarEndPauseEnable */

/*******************************************************************************
**
**  hyPhy20gCpbGfpChnlNearEndPauseEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables the pausing of the
**                  outgoing GFP channel based on commands received by the
**                  upstream GFP receiver and is only applicable to CPB
**                  channels mapped to GFP in GFP-F mode.
**
**                  Near End Pause generation must be ENABLED BEFORE the channel
**                  is enabled with hyPhy20gCpbChnlEnable, and it must be
**                  DISABLED BEFORE channel is disabled with
**                  hyPhy20gCpbChnlDisable.
**
**  INPUTS:         fileHndl    - device handle
**                  slice     - 0 - Slice 'A'
**                            - 1 - Slice 'B'
**                  chnlId    - GFP channel
**                              Legal range 0 to 63
**                  enable    - 0 - disable
**                              1 - enable
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbGfpChnlNearEndPauseEnable(struct file *fileHndl, UINT4 slice,
                                          UINT4 chnlId, UINT4 enable)
{
    UINT4 pauseEnReg[2], regIdx, chnlBitOff, pauseEnMsk, pauseEnVal;
    UINT4 reg, val;
    INT4 result;

    /* argument checking */
    if (slice > 1 || chnlId > 63 || enable > 1)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            pauseEnReg[0] = HYPHY20G_CPB_CPB_REG_GFPA_NEAR_END_PAUSE_EN_1;
            pauseEnReg[1] = HYPHY20G_CPB_CPB_REG_GFPA_NEAR_END_PAUSE_EN_2;
            break;
        case 1:
            pauseEnReg[0] = HYPHY20G_CPB_CPB_REG_GFPB_NEAR_END_PAUSE_EN_1;
            pauseEnReg[1] = HYPHY20G_CPB_CPB_REG_GFPB_NEAR_END_PAUSE_EN_2;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    regIdx = (chnlId < 32) ? 0 : 1;
    chnlBitOff = chnlId - (32*(chnlId/32));

    reg = pauseEnReg[regIdx];
    pauseEnMsk = HYPHY20G_CPB_CPB_BIT_GFPA_NEAR_END_PAUSE_EN_1_GFPA_NE_ENABLE_MSK;

    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if(result) return result;

    if (enable == 1) {
        pauseEnVal = val | (0x1 << chnlBitOff);
    } else {
        pauseEnVal = val & ~(0x1 << chnlBitOff);
    }

    result = hyPhy20gVarFieldWrite(&val, pauseEnMsk, pauseEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbGfpChnlNearEndPauseEnable */

/*******************************************************************************
**
**  hyPhy20gCpbSetQdrStressMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function is used for internal device testing.  It
**                 operates using global variables that would not be supported
**                 if the driver has been modified to support multiple devices.
**                 
**                 This function controls the contents generated for CPB QDR
**                 scheduler entries.
**
**                 The standard mode will use as many
**                 entries as possible maximizing the entries assigned to each
**                 interface for the combination of interfaces enabled.
**
**                 The stress optimized mode will limit the entries assigned to
**                 each interface.  GFP will use 6 entries and PIF/Line will
**                 will use 9 entries.  NOTE: Stress optimized mode QDR contents
**                 are not met if all CPB interfaces are enabled.
**
**  INPUTS:        *fileHndl   - base address
**                 mode        - 0 Standard
**                               1 Stress Optimized
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbSetQdrStressMode (struct file *fileHndl, UINT4 mode)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 devId, revId, status;
	
	/* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if(mode > 1)
        return HYPHY20G_ERR_INVALID_ARG;

    if(mode) {
        pDevCtxt->gData.hyphy20gQdrStressMode = 1;
    } else {
        pDevCtxt->gData.hyphy20gQdrStressMode = 0;
    }

    DBG_PRINT("%s, %s, %d, QDR Set hyphy20gQdrStressMode;  \
                hyphy20gQdrStressDist = %u, hyphy20gQdrStressMode = %u\n", 
                __FILE__, __FUNCTION__, __LINE__, 
                (unsigned int)pDevCtxt->gData.hyphy20gQdrStressDist, 
                (unsigned int)pDevCtxt->gData.hyphy20gQdrStressMode);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbSetQdrStressMode */

/*******************************************************************************
**
**  hyPhy20gCpbSetQdrStressFourIntfDist
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function is used for internal device testing.  It
**                 operates using global variables that would not be supported
**                 if the driver has been modified to support multiple devices.
**                 
**                 This function controls how many entries are assigned to read
**                 interfaces when the QDR is configured for 'Stress Optimized'
**                 mode (see hyPhy20gCpbSetQdrStressMode).  This is only
**                 applicable when all four CPB interfaces have been enabled.
**
**                 GFP scheduers will always require 6 entries each.  Four
**                 entries are reserved for NULL.  The other 14 entries are
**                 weighed between Line and PIF interfaces based on the 'dist'
**                 argument.
**
**
**  INPUTS:        *fileHndl   - base address
**                 dist        - distribution type.  Specifies the number
**                               of entries assigned to Line and PIF read
**                               interfaces.
**                               0 - 9/4 (Line/PIF)
**                               1 - 8/6
**                               2 - 7/7
**                               3 - 6/8
**                               4 - 4/9
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbSetQdrStressFourIntfDist (struct file *fileHndl, UINT4 dist)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 devId, revId, status;
	
	/* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if(dist > 4)
        return HYPHY20G_ERR_INVALID_ARG;

    pDevCtxt->gData.hyphy20gQdrStressDist = dist;
    DBG_PRINT("%s, %s, %d, QDR Set hyphy20gQdrStressDist; \
                hyphy20gQdrStressDist = %u, hyphy20gQdrStressMode = %u\n", 
                __FILE__, __FUNCTION__, __LINE__, 
                (unsigned int)pDevCtxt->gData.hyphy20gQdrStressDist, 
                (unsigned int)pDevCtxt->gData.hyphy20gQdrStressMode);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbSetQdrStressFourIntfDist */

/******************************************************************************* 
** 
**  hyPhy20gCpbSetRdPortThresh 
**  ___________________________________________________________________________ 
** 
**  DESCRIPTION:    This function configures the threshold value of high speed 
**                  or low speed client port reader, which is specified 
**                  by channel ID. 
** 
**                  The threshold value is determined by client side traffic 
**                  mode, CBR/GFP-T or non-CBR (i.e. MAC terminated) traffic mode. 
** 
**                  For CBR/GFP-T traffic, this function must be called after CPB 
**                  channel provision APIs, such as: 
**                      hyPhy20gCpbChnlProv 
**                      hyPhy20gCpbNonPrimRdChnlProv 
**                      hyPhy20gCpbNonPrimRdChnlRateAwareProv 
** 
** 
**  INPUTS:         fileHndl    - device handle 
**                  chnlId      - The channel ID of the client port reader. 
**                                Legal range 0 to 17 
**                  cbrMode     - 1 - client traffic is CBR or GFP-T 
**                                0 - client traffic is not CBR, i.e. MAC terminated 
**                                Legal range 0 to 1
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbSetRdPortThresh(struct file *fileHndl, UINT4 chnlId, 
                                UINT4 cbrMode)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    UINT4 thresh;

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1 || cbrMode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    thresh = (cbrMode == 1) ? 0x1 : 0x14;

    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, thresh,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_THRESH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, thresh,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_OUT_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_OUT_THRESH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbSetRdPortThresh */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gCpbCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  the CPB subsystem.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function cpbHyPhy20gCpbMemDestroy recovers this memory and
**                  set the device level reference to NULL.
**
**  INPUTS:         deviceHndl      - (pointer to) HyPHY 20G Context
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbCreateCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT  *pCpbCtxt;
    UINT4 size;
    INT4 result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pCpbCtxt is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pCpbCtxt != NULL) {
        hyPhy20gCpbDestroyCtxt(fileHndl);
    }

    /* allocate memory for and associate with pDevCtxt */
    size = sizeof(sHYPHY20G_CPB_CTXT);

    pDevCtxt->pCpbCtxt = (sHYPHY20G_CPB_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pCpbCtxt == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    DBG_PRINT("CPB Context memory allocated:addr=0x%x size=%ld\n",
              (unsigned int)pDevCtxt->pCpbCtxt, size);

    pCpbCtxt = pDevCtxt->pCpbCtxt;

    pCpbCtxt->pFifo = pCpbCtxt->fifoCtxt;
    pCpbCtxt->pIfGrpCtxt = pCpbCtxt->ifGrpCtxt;
    pCpbCtxt->pSchdCtxt = pCpbCtxt->schdCtxt;
    pCpbCtxt->pWrChnl = pCpbCtxt->wrChnl;
    pCpbCtxt->pRdChnl = pCpbCtxt->rdChnl;

    /* initialize memories */
    result = cpbHyPhy20gCpbInitCtxt(fileHndl);
    if (result){
        sysHyPhy20gMemFree(pDevCtxt->pCpbCtxt);
        return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbCreateCtxt */

/*******************************************************************************
**
**  hyPhy20gCpbDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the CPB subsystem.
**
**                  This function should be called when TBD
**
**
**
**                  This is a PRIVATE function.  This function should not be
**                  called by itself otherwise depending on device state, a
**                  memory leak may occur.
**
**  INPUTS:         fileHndl        - device handle
**                  *cpbCtxt        - pointer to CPB context structure
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbDestroyCtxt(struct file *fileHndl)
{
    sHYPHY20G_CPB_CTXT      *pCpbCtxt;
    sHYPHY20G_SCHD_CTXT     *pSchd;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    UINT4 i;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;

    if(pCpbCtxt == NULL){
        /* Nothing to destroy */
    	return HYPHY20G_SUCCESS;
    }

    /* destroy scheduler structures */
    pSchd = pCpbCtxt->pSchdCtxt;
    for (i = 0; i < HYPHY20G_CPB_NUM_SCHD; i++){
        utilHyPhy20gScbsDestroySchd(pSchd);
        pSchd++;
    }

    /* destroy write channel structures */
    pWrChnl = pCpbCtxt->pWrChnl;
    for (i = 0; i < HYPHY20G_CPB_NUM_WR_PORT; i++){
        sysHyPhy20gMemFree(pWrChnl->pRdChnlList);
        pWrChnl++;
    }

    sysHyPhy20gMemFree(pDevCtxt->pCpbCtxt);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbDestroyCtxt */


/*******************************************************************************
**
**  hyPhy20gCpbSelectRAM
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Select Internal RAM or QDR RAM.
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  ramType     - 1 - QDR RAM
**                                0 - Internal RAM
**                  blkSize     - number of 48 byte bursts contained in each
**                                block referenced in the block link list
**                              - only valid if ramType is 1, ignored otherwise
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   blkSize is modified based on the attribute of RAM
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbSelectRAM(struct file *fileHndl, UINT1 ramType, UINT4 blkSize)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    sHYPHY20G_CPB_RAM_CTXT *pRAM;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((ramType > 1) ||
        ((ramType == 1) && (blkSize > 0x3FF))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt->pCpbCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt->pCpbCtxt is NULL");

    pRAM = &(pDevCtxt->pCpbCtxt->ram);

    addr = HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    if (ramType == 0) {
        val |= HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_INTERNAL_RAM_EN_MSK;
    } else {
        val &= ~HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_INTERNAL_RAM_EN_MSK;
    }

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    pRAM->ramType = ramType;


    addr = HYPHY20G_CPB_CPB_REG_BLK_CFG;
    if (ramType == 0) {
/* emulation vs real device specific configurations */
#ifdef HWACCESSMODE_USB
        val = (0x8 << HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_OFF) &
                HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_MSK;

#else /* HWACCESSMODE_USB */
        if (pDevCtxt->gData.intQdrBlkSize == 0) {
            val = (HYPHY20G_INTERNAL_RAM_BLOCK_SIZE
                    << HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_OFF) &
                    HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_MSK;
        } else {
            val = (pDevCtxt->gData.intQdrBlkSize << HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_OFF)
                  & HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_MSK;
        }

#endif /* HWACCESSMODE_USB */
        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;
#ifdef HWACCESSMODE_USB
        pRAM->blkSize = 8;
#else /* HWACCESSMODE_USB */
        if (pDevCtxt->gData.intQdrBlkSize == 0) {
            pRAM->blkSize = HYPHY20G_INTERNAL_RAM_BLOCK_SIZE;
        } else {
            pRAM->blkSize = pDevCtxt->gData.intQdrBlkSize;
        }
#endif /* HWACCESSMODE_USB */
    } else {
        /* Caculate block size based on QDR RAM information */
        val = (blkSize & HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_MSK) <<
                HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_OFF;
        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;
        pRAM->blkSize = blkSize;

    }
DBG_PRINT("pRAM->blkSize = %u\n",(unsigned int)pRAM->blkSize);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbSelectRAM */


/*******************************************************************************
**
**  cpbHyPhy20gCpbInitCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Intializes memory required to operate the CPB subsystem.
**
**                  This function should be called when the memory is first
**                  alocated and prior to performing any channel specific
**                  configuration in the CPB.
**
**                  This is a PRIVATE function.  This function should not be
**                  called by itself otherwise depending on device state, a
**                  memory leak may occur.
**
**  INPUTS:         fileHndl        - device handle
**                  *cpbCtxt        - pointer to CPB context structure
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbInitCtxt(struct file *fileHndl)
{

    sHYPHY20G_CPB_CTXT      *pCpbCtxt;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_INTF_GRP_CTXT *pIntfGrp;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    UINT2 block;
    UINT4 i, portOffset, calEntryRate;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pCpbCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;

    /* initialize cpb top level context */
    pCpbCtxt->valid = HYPHY20G_VALID_SIGN;

    /* initialize cpb maxSchd/maxWrChnl */
    pCpbCtxt->maxSchd = HYPHY20G_CPB_NUM_SCHD;
    pCpbCtxt->maxWrChnl = HYPHY20G_CPB_NUM_WR_PORT;

    /* intialize ram structure */
    pCpbCtxt->ram.valid = HYPHY20G_VALID_SIGN;
    pCpbCtxt->ram.ramType = 0;
    pCpbCtxt->ram.blkSize = 0;
    pCpbCtxt->ram.minFifoSize = 1;

    pCpbCtxt->ram.maxFifoSize = HYPHY20G_CPB_NUM_BLOCK;
    pCpbCtxt->ram.numFreeBlk = HYPHY20G_CPB_NUM_BLOCK;

    /* Initialize calEntryRate structure element
       values assumes:
       - 90% utilization of available memory
       bandwidth is user data and not overhead.
       - QDR Scheduler has 15 rows available for interface
       group entries (30 entries per direction).

       76.8 Gpbs / 30 * 0.9 = 2.304 Gbps or 2304 Mbps

       76800 Mpbs / 30 = HYPHY20G_CPB_QDR_RAW_SCHD_ENTRY_RATE
       90% scaling factor = HYPHY20G_CPB_QDR_SCHD_ENTRTY_SCALE_FACTOR
    */

    calEntryRate = HYPHY20G_CPB_QDR_RAW_SCHD_ENTRY_RATE *
                   HYPHY20G_CPB_QDR_SCHD_ENTRTY_SCALE_PERCENT;
    calEntryRate = calEntryRate/100 + ((calEntryRate%100 > 49) ? 1 : 0);

    pCpbCtxt->ram.calEntryRate = calEntryRate;

    /* Initialize block pointer data structures */
    for (block = 0; block < pCpbCtxt->ram.numFreeBlk-1; block++) {
        pCpbCtxt->ram.blockPtr[block] = block + 1;
    }
    pCpbCtxt->ram.firstFreeBlk  = 0;

    pFifo = pCpbCtxt->pFifo;
    for (i = 0; i < HYPHY20G_CPB_NUM_FIFO; i++){
        pFifo->fifoId = i;
        pFifo->numBlks = 0;
        pFifo->firstBlk = HYPHY20G_INVALID_VAL_16BIT;
        pFifo->inUseFlag = HYPHY20G_FALSE;
        pFifo++;
    }

    /* initialize interface group structures */
    pIntfGrp = pCpbCtxt->pIfGrpCtxt;
    for (i = 0; i < HYPHY20G_CPB_NUM_IF_GRP; i++){
        pIntfGrp->valid = HYPHY20G_INVALID_VAL_16BIT;
        pIntfGrp->enable = HYPHY20G_CPB_INTF_DISABLED;
        pIntfGrp->intfGrp = i;
        pIntfGrp++;
    }

    /* initialize write channel structures */
    pWrChnl = pCpbCtxt->pWrChnl;
    for (i = 0; i < HYPHY20G_CPB_NUM_WR_PORT; i++){
        if ( i < HYPHY20G_CPB_GFPA_PORT_OFFSET ){
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
        } else if ( i < HYPHY20G_CPB_GFPB_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
        } else if ( i < HYPHY20G_CPB_PIF_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
        } else {
            portOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
        }
        pWrChnl->chnlId = i - portOffset;
        pWrChnl->portId = i;
        pWrChnl->rate = 0;
        pWrChnl->portWidth = HYPHY20G_INVALID_VAL_16BIT;
        pWrChnl->valid = HYPHY20G_INVALID_VAL_16BIT;
        pWrChnl->rdChnlCnt = 0;
        pWrChnl->rdChnlListLen = 0;
        pWrChnl->pRdChnlList = NULL;
        pWrChnl->state = HYPHY20G_CPB_CHNL_START;
        pWrChnl++;
    }

    /* initialize read channel structures */
    pRdChnl = pCpbCtxt->pRdChnl;
    for (i = 0; i < HYPHY20G_CPB_NUM_RD_PORT; i++){
        if ( i < HYPHY20G_CPB_GFPA_PORT_OFFSET ){
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
        } else if ( i < HYPHY20G_CPB_GFPB_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
        } else if ( i < HYPHY20G_CPB_PIF_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
        } else {
            portOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
        }
        pRdChnl->chnlId = i - portOffset;
        pRdChnl->portId = i;
        pRdChnl->rate = 0;
        pRdChnl->portWidth = HYPHY20G_INVALID_VAL_16BIT;
        pRdChnl->primaryFlag = HYPHY20G_CPB_PRIMARY_RD_CHNL;
        pRdChnl->valid = HYPHY20G_INVALID_VAL_16BIT;
        pRdChnl->index = HYPHY20G_INVALID_VAL_16BIT;
        pRdChnl->state = HYPHY20G_CPB_CHNL_START;
        pRdChnl++;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbInitCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbReadBlockPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reads the CPB block pointer for the specified block.
**
**  INPUTS:         fileHndl    - device handle
**                  block       - Block to retrieve pointer
**                  ptr         - Pointer to the value return back
**
**  OUTPUTS:        ptr         - Pointer to the value return back
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbReadBlockPtr(struct file *fileHndl, UINT4 block, UINT4 *ptr)
{
    UINT4 blockAddr;
    UINT4 val;
    INT4 result;

    /* retrieve block address */
    blockAddr = HYPHY20G_CPB_CPB_REG_BLK_LNK_LIST_OFFSET(block);

    /* read block address */
    result = sysHyPhy20gRead(fileHndl, blockAddr, &val);
    if (result) return result;

    /* mask out NXT_BLK[9:0] field and associate with pointer */
    *ptr = (val & HYPHY20G_CPB_CPB_BIT_BLK_LNK_LIST_NXT_BLK_MSK) >>
            HYPHY20G_CPB_CPB_BIT_BLK_LNK_LIST_NXT_BLK_OFF;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbReadBlockPtr */

/*******************************************************************************
**
**  cpbHyPhy20gCpbWriteBlockPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Writes the CPB block pointer for the block.
**
**  INPUTS:         fileHndl    - device handle
**                  block       - Block to write
**                  ptr         - Pointer to write
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbWriteBlockPtr(struct file *fileHndl, UINT4 block, UINT4 ptr)
{
    UINT4 blockAddr;
    UINT4 val;
    INT4 result;

    /* normalize NXT_BLK[9:0] field to register bit location */
    val = (ptr << HYPHY20G_CPB_CPB_BIT_BLK_LNK_LIST_NXT_BLK_OFF) &
           HYPHY20G_CPB_CPB_BIT_BLK_LNK_LIST_NXT_BLK_MSK;

    /* retrieve block address */
    blockAddr = HYPHY20G_CPB_CPB_REG_BLK_LNK_LIST_OFFSET(block);

    /* write block pointer */
    result = sysHyPhy20gWrite(fileHndl, blockAddr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
}


/*******************************************************************************
**
**  cpbHyPhy20gCpbBuildFifo
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and initializes a circular FIFO.
**
**  INPUTS:         fileHndl    - device handle
**                  fifoId      - The FIFO to build up
**                                Legal range 0 to 293
**                  numBlocks   - Number of blocks to allocate in the list
**                  primRdChnl  - Primary read channel number
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   CPB block link list registers are modified.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_CHNL_FIFO
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbBuildFifo(struct file *fileHndl, UINT2 fifoId,
                             UINT2 numBlocks, UINT2 primRdChnl)
{
    UINT4 block;
    UINT4 newblock;
    INT4 i;
    INT4 result;
    UINT4 data[3];
    sHYPHY20G_CPB_RAM_CTXT *pRAM;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_CTXT *pDevCtxt;

    DBG_PRINT("%s, %s(%u, %u), %d\n",__FILE__, __FUNCTION__, fifoId, numBlocks ,__LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Confirm a valid fifoId has been specified */
    if(fifoId > HYPHY20G_CPB_NUM_FIFO - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm CPB context pointer is valid before proceduing */
    HYPHY20G_ASSERT((pDevCtxt->pCpbCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt->pCpbCtxt is NULL");

    pRAM = &(pDevCtxt->pCpbCtxt->ram);
    pFifo = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);
    DBG_PRINT("%s, %s, %d, %u, %u, %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRAM->numFreeBlk, (unsigned int)pRAM->minFifoSize, (unsigned int)pRAM->maxFifoSize);
    if ((numBlocks > pRAM->numFreeBlk) ||
        (numBlocks < pRAM->minFifoSize) || (numBlocks > pRAM->maxFifoSize)) {
        return HYPHY20G_ERR_CHNL_FIFO;
    }

    /* Get the first free CPB block */
    pFifo->firstBlk = block = pRAM->firstFreeBlk;

    /* Find the last block */
    if (numBlocks == 1) {
        newblock = block;
    } else {
        i = 1;
        do {
            newblock = pRAM->blockPtr[block];
            block = newblock;
            i++;
        } while (i < numBlocks);
    }

    /* Set the first free one to the block following the last */
    pRAM->firstFreeBlk = pRAM->blockPtr[newblock];

    /* Complete the circle */
    pRAM->blockPtr[block] = pFifo->firstBlk;

    /* Write CPB block pointer RAM */
    block = pFifo->firstBlk;
    for (i = 0; i < numBlocks; i++) {
        result = cpbHyPhy20gCpbWriteBlockPtr(fileHndl, block,
                                                pRAM->blockPtr[block]);
        if (result != HYPHY20G_SUCCESS) {
            return result;
        }
        block = pRAM->blockPtr[block];
    }

    /* Update FIFO and memory context */
    pFifo->numBlks = numBlocks;
    pFifo->inUseFlag = HYPHY20G_TRUE;
    pRAM->numFreeBlk -= numBlocks;
    pRAM->numFifo++;

    /* Update FIFO_MAX_DEPTH bits */
    result = cpbHyPhy20gFifoDepthIndRead(fileHndl, fifoId,
                                        &data[0], &data[1],&data[2]);
    if (result) return result;

    data[0] = mHYPHY20G_SET_FIELD4(data[0], (pFifo->numBlks * pRAM->blkSize),
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_FIFO_MAX_DEPTH_MSK,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_FIFO_MAX_DEPTH_OFF);

    data[0] = mHYPHY20G_SET_FIELD4(data[0], primRdChnl,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_RD_CH_MSK,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_RD_CH_OFF);

    /* program ZONE1, ZONE2 and ZONE3 value */
    data[1] = mHYPHY20G_SET_FIELD4(data[1], (pFifo->zone1),
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE1_MSK,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE1_OFF);
    data[1] = mHYPHY20G_SET_FIELD4(data[1], (pFifo->zone2),
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_1_MSK,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_1_OFF);
    data[2] = mHYPHY20G_SET_FIELD4(data[2], ((pFifo->zone2)>>12),
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_2_MSK,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_2_OFF);
    data[2] = mHYPHY20G_SET_FIELD4(data[2], (pFifo->zone3),
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE3_MSK,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE3_OFF);

    result = cpbHyPhy20gFifoDepthIndWrite(fileHndl, fifoId,
                                        data[0], data[1], data[2]);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbBuildFifo */

/*******************************************************************************
**
**  cpbHyPhy20gCpbFreeFifo
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Delete a circular FIFO from software context memory. Re-join
**                  the list of FIFO blokcs to the beginning of the free list.
**                  Don't need to program the blocks' register.
**
**  INPUTS:         fileHndl    - device handle
**                  fifoId      - specific a circular FIFO to free
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   CPB block link list registers are modified.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbFreeFifo(struct file *fileHndl, UINT2 fifoId)
{
    INT4 result;
    UINT4 block;
    UINT4 nextBlock;
    UINT4 data[3];
    sHYPHY20G_CPB_RAM_CTXT *pRAM;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Confirm a valid fifoId has been specified */
    if(fifoId > HYPHY20G_CPB_NUM_FIFO - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    pRAM = &(pDevCtxt->pCpbCtxt->ram);
    pFifo = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);

    /* start by finding the last block in the list */
    block = pFifo->firstBlk;
    do {
        nextBlock = pRAM->blockPtr[block];

        if (nextBlock != pFifo->firstBlk) {
            block = nextBlock;
        }

    } while (nextBlock != pFifo->firstBlk);

DBG_PRINT("%s, %s, %d, pRAM->numFreeBlk_1 = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRAM->numFreeBlk);

    /* Re-join the blocks of the circular FIFO to the beginning of
       the free block list */
    /* Now 'block' is set to the last block in the circular FIFO, link the
       tail of the circular FIFO to the head of free block list */
    pRAM->blockPtr[block] = pRAM->firstFreeBlk;
    /* Adjust the head of free block list to the head of the circular FIFO */
    pRAM->firstFreeBlk = pFifo->firstBlk;


    pRAM->numFreeBlk += pFifo->numBlks;
    pRAM->numFifo--;

DBG_PRINT("%s, %s, %d, pRAM->numFreeBlk_2 = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRAM->numFreeBlk);

    /* update FIFO_INDIRECT_CONFIG register */
    /* Update FIFO_MAX_DEPTH bits */
    result = cpbHyPhy20gFifoDepthIndRead(fileHndl, fifoId,
                                        &data[0], &data[1],&data[2]);
    if (result) return result;

    data[0] = mHYPHY20G_SET_FIELD4(data[0], 0,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_FIFO_MAX_DEPTH_MSK,
            HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_FIFO_MAX_DEPTH_OFF);

    result = cpbHyPhy20gFifoDepthIndWrite(fileHndl, fifoId,
                                        data[0], data[1], data[2]);
    if (result) return result;

    /* Free the circular FIFO context memory */
    pFifo->numBlks = 0;
    pFifo->firstBlk = HYPHY20G_INVALID_VAL_16BIT;
    pFifo->inUseFlag = HYPHY20G_FALSE;
    pFifo->zone1 = 0;
    pFifo->zone2 = 0;
    pFifo->zone3 = 0;
    pFifo->state = HYPHY20G_CPB_CHNL_START;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbFreeFifo */


/*******************************************************************************
**
**  cpbHyPhy20gCpbInitWrPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Init a Write Pointer context memory, and associate the
**                  write channel with a specific fifo
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel of this write pointer context
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbInitWrPtr(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 data[2];
    UINT2 fifoId;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* fifoId is the same as write channel id */
    fifoId = chnlId;

    pFifo = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);

    /* recover pointer to write channel context */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);

    result = cpbHyPhy20gWrPtrCtxtIndRead(fileHndl, chnlId, &data[0], &data[1]);
    if(result) return result;

    /* 5.6.6.2.2	Configure Writer Pointer Context Memory */
    /* 1.	Set WRCHAN_EN to 0 */
    data[1] &= (~HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WRCHAN_EN_MSK);

    /* 2.	Set CURR_BLK_SOP [9:0] and CURR_BLK_WR [9:0] to the First Block
       addresses in the circular FIFO */
    data[0] = mHYPHY20G_SET_FIELD4(data[0], pFifo->firstBlk,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_CURR_BLK_WR_MSK,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_CURR_BLK_WR_OFF);

    data[0] = mHYPHY20G_SET_FIELD4(data[0], pFifo->firstBlk,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_CURR_BLK_SOP_MSK,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_CURR_BLK_SOP_OFF);

    /* 3.	Set WR_PTR [9:0] to b'0000000000 */
    data[1] &=
    (~HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_SOP_PTR_MSK);

    /* 4.	Set SOP_PTR [9:0] to b'0000000000 */
    data[1] &=
    (~HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WR_PTR_MSK);

    result = cpbHyPhy20gWrPtrCtxtIndWrite(fileHndl, chnlId, data[0], data[1]);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbInitWrPtr */


/*******************************************************************************
**
**  cpbHyPhy20gCpbResetWrPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset a Write Pointer context memory
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel of this write pointer context
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbResetWrPtr(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 data[2];

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    data[0] = data[1] = 0;

    result = cpbHyPhy20gWrPtrCtxtIndWrite(fileHndl,
                                        chnlId, data[0], data[1]);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbResetWrPtr */

/*******************************************************************************
**
**  cpbHyPhy20gCpbEnableWrPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable/Disable the write access for the specific
**                  write channel
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel of this write pointer context
**                                to be enabled/disable
**                                Legal range 0 to 293
**                  enableFlg   - 0 - disable this channel
**                                1 - enable this channel
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbEnableWrPtr(struct file *fileHndl, UINT2 chnlId,
                               BOOLEAN enableFlg)
{
    INT4 result;
    UINT4 data[2];
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) || (enableFlg > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to write channel context */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);
    result = cpbHyPhy20gWrPtrCtxtIndRead(fileHndl,
                                        chnlId, &data[0], &data[1]);

    if (result) return result;

    if (enableFlg == 1) {
        /* Enable the write channel and write accesses will
           result in the pointers incrementing. */
        data[1] |= mHYPHY20G_FORM_FIELD4(enableFlg,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WRCHAN_EN_MSK,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WRCHAN_EN_OFF);
    } else {
        /* Disable the write channel */
        data[1] &= mHYPHY20G_FORM_FIELD4(enableFlg,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WRCHAN_EN_MSK,
    HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WRCHAN_EN_OFF);
    }
    result = cpbHyPhy20gWrPtrCtxtIndWrite(fileHndl,
                                        chnlId, data[0], data[1]);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbEnableWrPtr */


/*******************************************************************************
**
**  cpbHyPhy20gCpbInitRdPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Init a Read Pointer context memory, and associate the
**                  read channel with a specific write channel
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  wrChnlId    - The write channel to associate with
**                                Legal range 0 to 293
**                  rdChnlId    - The channel of this read pointer context
**                                Legal range 0 to 296
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbInitRdPtr(struct file *fileHndl, UINT2 wrChnlId,
                             UINT2 rdChnlId)
{
    INT4 result;
    UINT4 data[2];
    UINT2 currBlk;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) ||
        (wrChnlId > HYPHY20G_CPB_NUM_WR_PORT - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if write channl context is valid */

    /* Check if fifo is the same one associated with the write channl */

    /* recover pointer to read/write channel context */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    /* recover current block in write pointer
       which this read channel associated with */
    result = cpbHyPhy20gWrPtrCtxtIndRead(fileHndl, wrChnlId, &data[0],
                                         &data[1]);
    if (result) return result;

    currBlk = (UINT2)mHYPHY20G_GET_FIELD4(data[0],
HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_CURR_BLK_SOP_MSK,
HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_CURR_BLK_SOP_OFF);

    result = cpbHyPhy20gRdPtrCtxtIndRead(fileHndl, rdChnlId, &data[0],
                                         &data[1]);
    if (result) return result;

    /* 5.6.6.3.2	Reader Pointer Context Memory Initialization */
    data[1] = mHYPHY20G_SET_FIELD4(data[1], 0,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_OFF);

    data[0] = mHYPHY20G_SET_FIELD4(data[0], currBlk,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_CURR_BLK_RD_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_CURR_BLK_RD_OFF);

    data[0] = mHYPHY20G_SET_FIELD4(data[0], wrChnlId,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_WR_CH_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_WR_CH_OFF);

    /* master reader is defined by setting the PRIMARY bit */
    if (pRdChnl->primaryFlag == HYPHY20G_CPB_PRIMARY_RD_CHNL) {
        /* This reader channel is choosen as master reader */
        data[0] = mHYPHY20G_SET_FIELD4(data[0], 1,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_PRIMARY_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_PRIMARY_OFF);
    } else {
        data[0] = mHYPHY20G_SET_FIELD4(data[0], 0,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_PRIMARY_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_PRIMARY_OFF);
    }

    data[1] = mHYPHY20G_SET_FIELD4(data[1], 0,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RD_PTR_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RD_PTR_OFF);

    data[1] = mHYPHY20G_SET_FIELD4(data[1], 0,
 HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RD_PTR_VALID_MSK,
 HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RD_PTR_VALID_OFF);

    data[1] = mHYPHY20G_SET_FIELD4(data[1], 0,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDPTR_AT_EOP_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDPTR_AT_EOP_OFF);

    result = cpbHyPhy20gRdPtrCtxtIndWrite(fileHndl, rdChnlId, data[0], data[1]);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbInitRdPtr */

/*******************************************************************************
**
**  cpbHyPhy20gCpbResetRdPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset a Read Pointer context memory
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  rdChnlId    - The channel of this read pointer context
**                                Legal range 0 to 296
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbResetRdPtr(struct file *fileHndl, UINT2 rdChnlId)
{
    INT4 result;
    UINT4 data[2];

    /* argument checking */
    if (rdChnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    data[0] = data[1] = 0;

    result = cpbHyPhy20gRdPtrCtxtIndWrite(fileHndl,
                                        rdChnlId, data[0], data[1]);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbResetRdPtr */


/*******************************************************************************
**
**  cpbHyPhy20gCpbEnableRdPtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable/Disable the read access for the specific
**                  read channel
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel of this read pointer context
**                                to be enabled/disable
**                                Legal range 0 to 296
**                  enableFlg   - 0 - disable this channel
**                                1 - enable this channel
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbEnableRdPtr(struct file *fileHndl, UINT2 chnlId,
                               BOOLEAN enableFlg)
{
    INT4 result;
    UINT4 data[2];
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) || (enableFlg > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to read channel context */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + chnlId);


    result = cpbHyPhy20gRdPtrCtxtIndRead(fileHndl,
                                        chnlId, &data[0], &data[1]);

    if (result) return result;

    if (enableFlg == 1) {
        /* Enable the read channel and read accesses will
           result in the pointers incrementing. */
        data[1] |= mHYPHY20G_FORM_FIELD4(enableFlg,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_OFF);
    } else {
        /* Disable the read channel */
        data[1] &= mHYPHY20G_FORM_FIELD4(enableFlg,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_MSK,
    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_OFF);
    }

    result = cpbHyPhy20gRdPtrCtxtIndWrite(fileHndl,
                                        chnlId, data[0], data[1]);
    if (result) return result;
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbEnableRdPtr */



/*******************************************************************************
**
**  cpbHyPhy20gCpbConfigWrPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Config a client port writer which is specified by channel
**                  ID.
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port writer belongs to
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbConfigWrPort(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    UINT1 portWidth;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to write channel context */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);

    portWidth = pWrChnl->portWidth;

    /* 5.6.6.2.1	Initialize the Client Port Writer */
    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, portWidth,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, portWidth,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId == 18){
       /* MPIF port initialization */
    } else if ((chnlId >= 19 && chnlId <= 82) ||
               (chnlId >= 83 && chnlId <= 146)) {
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82) {
            /* GFP A*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);
        } else {
            /* GFP B*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);
        }

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, portWidth,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_WIDTH_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_WIDTH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_WR_PORT - 1) {
        /* Packet Interface Write Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_WR_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        /* PI_WR_DISCARD_DIS field setting */
        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_DISCARD_DIS_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_DISCARD_DIS_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbConfigWrPort */


/*******************************************************************************
**
**  cpbHyPhy20gCpbInitWrPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Init a client port writer which is specified by channel ID.
**                  This includes enabling the write port, and placing the
**                  write FIFOs into reset.
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port writer belongs to
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbInitWrPort(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    UINT4 msk;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to write channel context */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);

    /* 5.6.6.2.1	Initialize the Client Port Writer */
    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_PORT_IN_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_PORT_IN_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_IN_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_IN_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_WR_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_WR_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if (chnlId == 18) {
        /* MPIF port initialization */
        addr = HYPHY20G_CPB_CPB_REG_MPRO_CLNT_PRT_CFG;
        msk = HYPHY20G_CPB_CPB_BIT_MPRO_CLNT_PRT_CFG_MICRO_WR_PORT_FIFO_RST_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if ((chnlId >= 19 && chnlId <= 82) ||
               (chnlId >= 83 && chnlId <= 146)) {
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82) {
            /* GFP A*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);
        } else {
            /* GFP B*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);
        }

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_WR_PORT - 1) {
        /* Packet Interface Write Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_WR_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_PORT_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_PORT_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbInitWrPort */


/*******************************************************************************
**
**  cpbHyPhy20gCpbResetWrPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset a client port writer which is specified by channel ID
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port writer belongs to
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   Disable write client port and reset the FIFO
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbResetWrPort(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlId <= 15) {
        /* LOW SPEED PORT RESET */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_DISCARD_DIS_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_DISCARD_DIS_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT RESET */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_WR_DISCARD_DIS_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_WR_DISCARD_DIS_OFF);
		
        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId == 18) {
       /* MPIF port initialization */
    } else if ((chnlId >= 19 && chnlId <= 82) ||
               (chnlId >= 83 && chnlId <= 146)) {
        /* GFP PORT RESET */
        if (chnlId >= 19 && chnlId <= 82) {
            /* GFP A*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);
        } else {
            /* GFP B*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);
        }

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_DISCARD_DIS_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_DISCARD_DIS_OFF);
		
        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_WR_PORT - 1) {
        /* Packet Interface Write Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_WR_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_DISCARD_DIS_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_DISCARD_DIS_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbResetWrPort */


/*******************************************************************************
**
**  cpbHyPhy20gCpbEnableWrPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable or disable the write client port, specificed by the
**                  chnlId.
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port writer belongs to
**                                Legal range 0 to 293
**                  enableFlg   - 0 - disable this channel
**                                1 - enable this channel
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   Write client port enable bit will be set by enableFlg
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbEnableWrPort(struct file *fileHndl, UINT2 chnlId,
                                BOOLEAN enableFlg)
{
    INT4 result;
    UINT4 addr = 0;
    UINT4 val;
    UINT4 enable_bit = 0;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) || (enableFlg > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to write channel context */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);


    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_PORT_IN_EN_MSK;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_IN_EN_MSK;

    } else if (chnlId == 18){
       /* MPIF port initialization */
    } else if ((chnlId >= 19 && chnlId <= 82) || (chnlId >= 83 && chnlId <= 146) ){
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82)
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);/* GFP A*/
        else
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);/* GFP B*/

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_EN_MSK;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, enable_bit = %d, enableFlg = %d, chnlId = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)enableFlg, (unsigned int)chnlId);
        /* Packet Interface Read Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_WR_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_PORT_EN_MSK;

        DBG_PRINT("%s, %s, %d, enable_bit = %d, enableFlg = %d, val = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)enableFlg, (unsigned int)val);
    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (enableFlg == 1) {
        /* Enable the write port and pass data to
           subsequent block when it is valid*/
        val |= enable_bit;
    } else {
        /* Disable the write port and no data is input to the staging buffer */
        val &= ~enable_bit;
    }
    if (chnlId != 18) {
        DBG_PRINT("%s, %s, %d, enable_bit = %d, enableFlg = %d, val = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)enableFlg, (unsigned int)val);
        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbEnableWrPort */


/*******************************************************************************
**
**  cpbHyPhy20gCpbWrPortFifoReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable or disable the write client port, specificed by the
**                  chnlId.
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port writer belongs to
**                                Legal range 0 to 293
**                  resetFlg   - 0 - disable this channel
**                               1 - enable this channel
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   Write client port enable bit will be set by resetFlg
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbWrPortFifoReset(struct file *fileHndl, UINT2 chnlId,
                                   BOOLEAN resetFlg)
{
    INT4 result;
    UINT4 addr = 0;
    UINT4 val;
    UINT4 enable_bit = 0;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) || (resetFlg > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to write channel context */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);


    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_PORT_FIFO_RST_MSK;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_WR_PORT_FIFO_RST_MSK;

    } else if (chnlId == 18){
        /* MPIF port initialization */
        addr = HYPHY20G_CPB_CPB_REG_MPRO_CLNT_PRT_CFG;
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_MPRO_CLNT_PRT_CFG_MICRO_WR_PORT_FIFO_RST_MSK;

    } else if ((chnlId >= 19 && chnlId <= 82) || (chnlId >= 83 && chnlId <= 146) ){
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82)
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);/* GFP A*/
        else
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);/* GFP B*/

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_FIFO_RST_MSK;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, enable_bit = %d, resetFlg = %d, chnlId = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)resetFlg, (unsigned int)chnlId);
        /* Packet Interface Read Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_WR_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_PORT_FIFO_RST_MSK;

        DBG_PRINT("%s, %s, %d, enable_bit = %d, resetFlg = %d, val = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)resetFlg, (unsigned int)val);
    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (resetFlg == 1) {
        /* Enable the write port and pass data to
           subsequent block when it is valid*/
        val |= enable_bit;
    } else {
        /* Disable the write port and no data is input to the staging buffer */
        val &= ~enable_bit;
    }

    DBG_PRINT("%s, %s, %d, enable_bit = %d, resetFlg = %d, val = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)resetFlg, (unsigned int)val);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbWrPortFifoReset */


/*******************************************************************************
**
**  cpbHyPhy20gCpbConfigRdPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Config a client port reader which is specified by channel
**                  ID.
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port reader belongs to
**                                Legal range 0 to 296
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbConfigRdPort(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    UINT4 thresh, dummy, schdMode;
    UINT1 portWidth;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to read channel context */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + chnlId);

    portWidth = pRdChnl->portWidth;

    /* 5.6.6.3.1	Client Port Reader Initialization */
    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        thresh = 0x14;

        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, portWidth,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_OFF);

        val = mHYPHY20G_SET_FIELD4(val, thresh,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_THRESH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        thresh = 0x14;

        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, portWidth,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_OFF);

        val = mHYPHY20G_SET_FIELD4(val, thresh,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_OUT_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_OUT_THRESH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId == 18){
       /* MPIF port initialization */
    } else if ((chnlId >= 19 && chnlId <= 82) ||
               (chnlId >= 83 && chnlId <= 146)) {
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82) {
            /* GFP A*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);
        } else {
            /* GFP B*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);
        }

        thresh = 0x1A;

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, portWidth,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_WIDTH_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_WIDTH_OFF);

        /* configure GFP_RD_OUTPUT_LENGTH field basic setting as function
           of portWidth, GFP subsystem API will in turn update setting as
           required for specific applications */
        if (portWidth == 1) {
            /* 40-bits */
            val = mHYPHY20G_SET_FIELD4(val, 0,
                    HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK,
                    HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_OFF);
        } else {
            /* 32-bits */
            val = mHYPHY20G_SET_FIELD4(val, 1,
                    HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK,
                    HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_OFF);
        }

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_RD_PORT - 1) {
        /* Packet Interface Read Port Configuration */
        pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt + HYPHY20G_CPB_PIF_RD_SCHD);
        result = utilHyPhy20gScbsReadSchdCfg(fileHndl, pSchd, &dummy,
                                             &schdMode, &dummy);
        if(result) return result;       
        thresh = (schdMode == 0)? 0x3:0x8;

        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, thresh,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_THRESH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbConfigRdPort */


/*******************************************************************************
**
**  cpbHyPhy20gCpbInitRdPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Init a client port reader which is specified by channel ID.
**                  This includes enabling the read port, and placing the read
**                  FIFOs into reset.
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port reader belongs to
**                                Legal range 0 to 296
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbInitRdPort(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to read channel context */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + chnlId);

    /* 5.6.6.3.1	Client Port Reader Initialization */
    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_PORT_OUT_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_PORT_OUT_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_OUT_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_OUT_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_RD_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_RD_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if (chnlId == 18) {
        /* MPIF port initialization */
        addr = HYPHY20G_CPB_CPB_REG_MPRO_CLNT_PRT_CFG;
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_MPRO_CLNT_PRT_CFG_MICRO_RD_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_MPRO_CLNT_PRT_CFG_MICRO_RD_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if ((chnlId >= 19 && chnlId <= 82) ||
               (chnlId >= 83 && chnlId <= 146)) {
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82) {
            /* GFP A*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);
        } else {
            /* GFP B*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);
        }

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_RD_PORT - 1) {
        /* Packet Interface Read Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_PORT_EN_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_PORT_EN_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_PORT_FIFO_RST_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_PORT_FIFO_RST_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

        result = cpbHyPhy20gCpbToggleChnlRstGo(fileHndl);
        if (result) return result;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbInitRdPort */


/*******************************************************************************
**
**  cpbHyPhy20gCpbResetRdPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset a client port reader which is specified by channel ID
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port reader belongs to
**                                Legal range 0 to 296
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbResetRdPort(struct file *fileHndl, UINT2 chnlId)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;

    /* argument checking */
    if (chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* 5.6.6.3.1	Client Port Reader Initialization */
    if (chnlId <= 15) {
        /* LOW SPEED PORT RESET */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_THRESH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT RESET */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_OUT_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_OUT_THRESH_OFF);
		
        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId == 18) {
       /* MPIF port initialization */
    } else if ((chnlId >= 19 && chnlId <= 82) ||
               (chnlId >= 83 && chnlId <= 146)) {
        /* GFP PORT RESET */
        if (chnlId >= 19 && chnlId <= 82) {
            /* GFP A*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);
        } else {
            /* GFP B*/
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);
        }

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK,
                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_RD_PORT - 1) {
        /* Packet Interface Read Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_OUTPUT_LENGTH_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_OUTPUT_LENGTH_OFF);

        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_THRESH_MSK,
                HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_THRESH_OFF);

        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbResetRdPort */


/*******************************************************************************
**
**  cpbHyPhy20gCpbEnableRdPort
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable/Disable a client port reader which is specified
**                  by channel ID
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port reader belongs to
**                                Legal range 0 to 293
**                  enableFlg   - 0 - disable this channel
**                                1 - enable this channel
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbEnableRdPort(struct file *fileHndl, UINT2 chnlId,
                                BOOLEAN enableFlg)
{
    INT4 result;
    UINT4 addr = 0;
    UINT4 val;
    UINT4 enable_bit = 0;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) || (enableFlg > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_PORT_OUT_EN_MSK;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_OUT_EN_MSK;

    } else if (chnlId == 18){
       /* MPIF port initialization */
    } else if ((chnlId >= 19 && chnlId <= 82) || (chnlId >= 83 && chnlId <= 146) ){
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82)
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);/* GFP A*/
        else
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);/* GFP B*/

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_EN_MSK;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_RD_PORT - 1) {
        /* Packet Interface Read Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_PORT_EN_MSK;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (enableFlg == 1) {
        /* Enable the read port and output data when data is available */
        val |= enable_bit;
    } else {
        /* Disable the read port and no data is output */
        val &= ~enable_bit;
    }

    if (chnlId != 18) {
        DBG_PRINT("%s, %s, %d, enable_bit = %d, enableFlg = %d, val = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)enableFlg, (unsigned int)val);
        result = sysHyPhy20gWrite(fileHndl, addr, val);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbEnableRdPort */



/*******************************************************************************
**
**  cpbHyPhy20gCpbRdPortFifoReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable/Disable a client port reader which is specified
**                  by channel ID
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port reader belongs to
**                                Legal range 0 to 293
**                  resetFlg   - 0 - disable this channel
**                                1 - enable this channel
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbRdPortFifoReset(struct file *fileHndl, UINT2 chnlId,
                                   BOOLEAN resetFlg)
{
    INT4 result;
    UINT4 addr = 0;
    UINT4 val;
    UINT4 enable_bit = 0;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) || (resetFlg > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_RD_PORT_FIFO_RST_MSK;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_RD_PORT_FIFO_RST_MSK;

    } else if (chnlId == 18){
        /* MPIF port initialization */
        addr = HYPHY20G_CPB_CPB_REG_MPRO_CLNT_PRT_CFG;
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_MPRO_CLNT_PRT_CFG_MICRO_RD_PORT_FIFO_RST_MSK;
    } else if ((chnlId >= 19 && chnlId <= 82) || (chnlId >= 83 && chnlId <= 146) ){
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82)
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);/* GFP A*/
        else
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);/* GFP B*/

        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_FIFO_RST_MSK;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_RD_PORT - 1) {
        /* Packet Interface Read Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_RD_PRT_CFG_OFFSET(chnlId - 147);
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;

        enable_bit = HYPHY20G_CPB_CPB_BIT_PAC_IF_RD_PRT_CFG_PI_RD_PORT_FIFO_RST_MSK;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (resetFlg == 1) {
        /* Enable the read port and output data when data is available */
        val |= enable_bit;
    } else {
        /* Disable the read port and no data is output */
        val &= ~enable_bit;
    }

    DBG_PRINT("%s, %s, %d, enable_bit = %d, resetFlg = %d, val = %d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)enable_bit, (unsigned int)resetFlg, (unsigned int)val);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbRdPortFifoReset */



/*******************************************************************************
**
**  cpbHyPhy20gCpbGetPortWidth
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         fileHndl    - device handle
**                  traffic     - specific the type of traffic
**                  pWidth      - A pointer to port width value
**
**  OUTPUTS:        pWidth      - A pointer to port width value
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetPortWidth(struct file *fileHndl, UINT2 traffic,
                                UINT1 *pWidth)
{
    if( (traffic > TRAFFIC_TYPES_MAX_VALUE) || (pWidth == NULL) ) {
        /* dbg code */
        DBG_PRINT("traffic=%d pWidth=%d\n", (unsigned int)traffic, (unsigned int)pWidth);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (traffic) {

    case LS_FCCBR_FC_8B10B:
    case LS_FCCBR_ESCON_8B10B:
    case LS_FCCBR_CBR_8B10B:
    case LS_FEGE_MAC_TEMINATION:
    case LS_FEGE_GE_GFPT_MAPPING:
    case HS_FCCBR_FC8G_8B10B:
    case HS_FCCBR_FC10G_64B66B:
    case HS_FCCBR_10GELAN_GSUP43_71:
    case HS_FCCBR_10GELAN_GSUP43_72:
    case HS_FCCBR_STS192:
        *pWidth = 40;
        break;

    case LS_FCCBR_STS3:
    case LS_FCCBR_STS12:
    case LS_FCCBR_STS48:
    case LS_FCCBR_CBR_NON8B10B:
    case HS_10GE_MAC_TEMINATION:
    case HS_10GE_10GELAN_GSUP43_73:
        *pWidth = 32;
        break;
    default:
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbGetPortWidth */


/* FUNCTIONS TO UPDATE SCHEDULERS BASED ON CHANNEL DATA CONTEXT */
/*******************************************************************************
**
**  cpbHyPhy20gCPBSchdUpdate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Updates specified interface group schedulers and the QDR
**                  scheduler using read and write channel state context.
**
**  INPUTS:         fileHndl  - device handle
**                  intfGrpMsk - bit mask of interface group(s) to be evaluated.
**                               bit offsets for respective schedulers within
**                               mask:
**                              0 - CPB Line Interface Group Write Scheduler
**                              1 - CPB Line Interface Group Read Scheduler
**                              2 - CPB GFP A Interface Group Write Scheduler
**                              3 - CPB GFP A Interface Group Read Scheduler
**                              4 - CPB GFP B Interface Group Write Scheduler
**                              5 - CPB GFP B Interface Group Read Scheduler
**                              6 - CPB PIF Interface Group Read Scheduler
**                              Note: Only one Write Scheduler may be selected
**                                    Within the intfGrpMsk
**                  rdSchFirst - determines if read or write scheduler will be
**                              configured first:
**                                  1 - configure read scheduler first,
**                                      then configure write scheduler
**                                  0 - configure write scheduler first,
**                                      then configure read scheduler
**                  updateQDR  - Option to update QDR scheduler.
**                               Set to 1 to update QDR scheduler.
**                               Set to 0 to not update QDR scheduler.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBSchdUpdate(struct file  *fileHndl, UINT4 intfGrpMsk,
                              UINT4 rdSchFirst, UINT4 updateQDR)
{

    UINT4 wrSchdNum, size;
    UINT4 *pCalData = NULL;
    UINT4 different = 0;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((intfGrpMsk >= 0x100) || ((intfGrpMsk & 0x15)==0x5) ||
        ((intfGrpMsk & 0x15)==0x11) || ((intfGrpMsk & 0x15)==0x14) ||
        ((intfGrpMsk & 0x15)==0x15)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((rdSchFirst > 1) || (updateQDR > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    DBG_PRINT("%s, %s, %d, Got to intfGrpMsk = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)intfGrpMsk);

    /* allocate memory to pCalData used for holding scheduler contents */
    /* use PIF Scheduler context as this is the largest scheduler */
    size = sizeof(UINT4)*(HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW*
                          HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW);

    pCalData = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pCalData == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    if (rdSchFirst) {
        /* First update read interface group scheduler(s) */
        if(intfGrpMsk & 0x2){
            DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x2\n",__FILE__, __FUNCTION__, __LINE__);
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_LINE_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x8){
            DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x8\n",__FILE__, __FUNCTION__, __LINE__);
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPA_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x20){
            DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x20\n",__FILE__, __FUNCTION__, __LINE__);
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPB_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x40){
             DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x40\n",__FILE__, __FUNCTION__, __LINE__);
             pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_PIF_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }

        /* Then update write interface group scheduler */
        DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
        /* reference write scheduler context */
        if(intfGrpMsk & 0x1){
            wrSchdNum = HYPHY20G_CPB_LINE_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x4){
            wrSchdNum = HYPHY20G_CPB_GFPA_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x10){
            wrSchdNum = HYPHY20G_CPB_GFPB_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }

    } else {
        /* First update write interface group scheduler */
        DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
        /* reference write scheduler context */
        if(intfGrpMsk & 0x1){
            wrSchdNum = HYPHY20G_CPB_LINE_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x4){
            wrSchdNum = HYPHY20G_CPB_GFPA_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x10){
            wrSchdNum = HYPHY20G_CPB_GFPB_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }

            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }


        /* Then update read interface group scheduler */
        if(intfGrpMsk & 0x2){
            DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x2\n",__FILE__, __FUNCTION__, __LINE__);
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_LINE_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x8){
            DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x8\n",__FILE__, __FUNCTION__, __LINE__);
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPA_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x20){
            DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x20\n",__FILE__, __FUNCTION__, __LINE__);
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPB_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        if(intfGrpMsk & 0x40){
            DBG_PRINT("%s, %s, %d, Got to intfGrpMsk && 0x40\n",__FILE__, __FUNCTION__, __LINE__);
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_PIF_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
        DBG_PRINT("%s, %s, %d, Got to here4444\n",__FILE__, __FUNCTION__, __LINE__);
    }


    if (updateQDR == 1) {
        /* update QDR scheduler */
        pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_QDR_SCHD;

        DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
        /* generate QDR scheduler content */
        result = cpbHyPhy20gCPBGenerateQdrSchd(fileHndl, pSchd, pCalData);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            return result;
        }
        /* compare scheduler data with current online scheduler data */
        result = cpbHyPhy20gCpbQdrSchdCmp(fileHndl, pSchd, pCalData, &different);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            return result;
        }

        if(different){
            DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update QDR scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                return result;
            }
        }
    }

    sysHyPhy20gMemFree(pCalData);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBSchdUpdate */

/*******************************************************************************
**
**  cpbHyPhy20gCPBGenerateQdrSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates interface group context, generates the scheduler
**                  calendar memory based on a best effort even distribution
**                  adhering to the requirement that no channel number be
**                  processed closer than 4 calendar entries apart.
**
**                  As part of process evaluates PIF write channel requirements
**                  (as the PIF write interface does not have a scheduler).
**
**
**  INPUTS:         fileHndl  - device handle
**                 *pSchd     - pointer to QDR scheduler context
**                 *pCalData  - pointer to formatted QDR scheduler calendar data
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   This function updates the totalChnlRate parameter of the
**                  pSchd structure.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBGenerateQdrSchd( struct file  *fileHndl,
                                    sHYPHY20G_SCHD_CTXT *pSchd, UINT4 *pCalData)
{
    sHYPHY20G_SCHD_CTXT *pIfGrpSchd;
    sHYPHY20G_INTF_GRP_CTXT *pIfGrpCtxt;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnl;

    UINT4 wrIfRate[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 rdIfRate[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 wrIfChnl[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 rdIfChnl[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 rawWrIfRate[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 rawRdIfRate[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 rawWrIfChnl[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 rawRdIfChnl[HYPHY20G_CPB_NUM_INTF_GRP];
    UINT4 *pWrIfRate, *pWrIfChnl, *pRawWrIfRate, *pRawWrIfChnl;
    UINT4 *pRdIfRate, *pRdIfChnl, *pRawRdIfRate, *pRawRdIfChnl;
    UINT4 qdrTotalWrRate = 0, qdrTotalRdRate = 0;
    UINT4 i, j, size;
    UINT4 chnlOffset;
    UINT4 revId, intRamEn;
    INT4 result;

    UINT4 wrCalEntries =
                       (pSchd->numRows) * HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW/2;
    UINT4 rdCalEntries =
                       (pSchd->numRows) * HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW/2;
    UINT4 *pLinWrArray, *pLinRdArray, *pFormattedCalData;

    /* verify pointer to global cpb data */
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* alocate memory for local use by function */
    size = sizeof(UINT4)*(wrCalEntries) +
           sizeof(UINT4)*(rdCalEntries);

    pLinWrArray = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pLinWrArray == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pLinRdArray = (UINT4 *)(pLinWrArray + wrCalEntries);
    pFormattedCalData = pCalData;

    /* reference to the first of the interface group schedulers */
    pIfGrpSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt);

    /* reference to the first of the context for interface groups */
    pIfGrpCtxt = (sHYPHY20G_INTF_GRP_CTXT *)(pDevCtxt->pCpbCtxt->pIfGrpCtxt);

    /* reference to the PIF write channel ports */
    chnlOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pWrChnl +
                                         chnlOffset);

    /* get device revision */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
    if(result){
        sysHyPhy20gMemFree(pLinWrArray);
        return result;
    }

    /* get QDR RAM mode */
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG,
             HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_INTERNAL_RAM_EN_OFF, &intRamEn);
    if(result){
        sysHyPhy20gMemFree(pLinWrArray);
        return result;
    }

    /* reference local arrays to pointers */
    pWrIfRate = wrIfRate;
    pWrIfChnl = wrIfChnl;
    pRawWrIfRate = rawWrIfRate;
    pRawWrIfChnl = rawWrIfChnl;

    pRdIfRate = rdIfRate;
    pRdIfChnl = rdIfChnl;
    pRawRdIfRate = rawRdIfRate;
    pRawRdIfChnl = rawRdIfChnl;

    /* retrieve interface group rate data */

    for(i=0; i < HYPHY20G_CPB_NUM_INTF_GRP; i++){
        rawWrIfChnl[i] = 2*i;
        rawRdIfChnl[i] = 2*i+1;
        if (i < HYPHY20G_CPB_PIF_INTF_GRP){
            rawWrIfRate[i] = pIfGrpSchd->totalChnlRate;
            pIfGrpCtxt->totalWrRate = rawWrIfRate[i];
            pIfGrpSchd++;
            rawRdIfRate[i] = pIfGrpSchd->totalChnlRate;
            pIfGrpCtxt->totalRdRate = rawRdIfRate[i];
            pIfGrpSchd++;
        } else {
            rawWrIfRate[i] = 0;
            for (j=0; j < HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL; j++) {
               if((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
                  (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                    rawWrIfRate[i] += pWrChnl->rate;
                }
                pWrChnl++;
            }
            pIfGrpCtxt->totalWrRate = rawWrIfRate[i];
            rawRdIfRate[i] = pIfGrpSchd->totalChnlRate;
            pIfGrpCtxt->totalRdRate = rawRdIfRate[i];
        }
        qdrTotalWrRate += rawWrIfRate[i];
        qdrTotalRdRate += rawRdIfRate[i];
        pIfGrpCtxt++;
DBG_PRINT("%s, %s, %d, rawWrIfRate(%d)=%d, qdrTotalWrRate=%d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)rawWrIfRate[i], (unsigned int)qdrTotalWrRate);
DBG_PRINT("%s, %s, %d, rawRdIfRate(%d)=%d, qdrTotalRdRate=%d\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)rawRdIfRate[i], (unsigned int)qdrTotalRdRate);
    }

    /* put interface groups in order from lowest to highest rate */
    result = cpbHyPhy20gPutQdrArrayInOrder(pRawWrIfRate, pRawWrIfChnl,
                                           pWrIfRate, pWrIfChnl);
    if(result){
        sysHyPhy20gMemFree(pLinWrArray);
        return result;
    }

    result = cpbHyPhy20gPutQdrArrayInOrder(pRawRdIfRate, pRawRdIfChnl,
                                           pRdIfRate, pRdIfChnl);
    if(result){
        sysHyPhy20gMemFree(pLinWrArray);
        return result;
    }

    /* distribute channels to linear write calendar */
    result = cpbHyPhy20gQdrScaleDataToCal(pWrIfRate, pWrIfChnl, pLinWrArray,
                                          wrCalEntries);
    if(result){
        sysHyPhy20gMemFree(pLinWrArray);
        return result;
    }

    /* distribute channels to linear read calendar */
    result = cpbHyPhy20gQdrScaleDataToCal(pRdIfRate, pRdIfChnl, pLinRdArray,
                                          rdCalEntries);
    if(result){
        sysHyPhy20gMemFree(pLinWrArray);
        return result;
    }

    /* generate scheduler calendar as used by device */

    /* even address contain write interface group data */
    j = 0;
    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)) =
             *(pLinWrArray+j);
        j++;
    }

    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW) + 2) =
             *(pLinWrArray+j);
        j++;
    }

    /* odd address contain read interface group data */
    j = 0;
    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW) + 1) =
             *(pLinRdArray+j);
        j++;
    }

    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW) + 3) =
             *(pLinRdArray+j);
        j++;
    }

    sysHyPhy20gMemFree(pLinWrArray);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBGenerateQdrSchd */

/*******************************************************************************
**
**  cpbHyPhy20gCPBGenerateIntfGrpSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the channel context for all channels used in
**                  an interface group scheduler, generates the scheduler
**                  calendar memory based on a best effort even distribution.
**
**                  Scheduling ensures a minimum of 1 entry per channel. Extra
**                  entries are roughly distributed between entries based on
**                  channel bandwidth.
**
**
**  INPUTS:         fileHndl  - device handle
**                  scope     - scope of channels to be associated with calendar
**                              generation
**                              1 - Provisioned and Enabled Channels
**                              0 - Enabled Channels Only
**                 *pSchd     - pointer to interface group scheduler context
**                              for one of the following schedulers
**                               - CPB Line Interface Group Read Scheduler
**                               - CPB Line Interface Group Read Scheduler
**                               - CPB GFP A Interface Group Read Scheduler
**                               - CPB GFP A Interface Group Read Scheduler
**                               - CPB GFP B Interface Group Read Scheduler
**                               - CPB GFP B Interface Group Read Scheduler
**                               - CPB PIF Interface Group Read Scheduler
**                 *pCalData  - pointer to formatted scheduler calendar data
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   This function updates the totalChnlRate parameter of the
**                  pSchd structure.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBGenerateIntfGrpSchd( struct file  *fileHndl,
                                        UINT4 scope,
                                        sHYPHY20G_SCHD_CTXT *pSchd,
                                        UINT4 *pCalData)
{
    sHYPHY20G_RD_CHNL_CTXT *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnl;
    UINT4 chnlOffset, chnlDir;
    UINT4 totalChnlRate = 0, numCalChnl = 0;
    UINT4 lowestChnlRate, maxChnlRate;
    UINT4 chnlToCalMultFactor;
    UINT4 workingRate, workingEntries, chnlRate, scaleEntries;
    UINT4 numCalEntries, populateChnl, populateRate;
    UINT4 calSpace, calSingles;
    UINT4 i, j, k, size;
    UINT4 lowestIndex = 0, lowestRate;
    /* alloate memory for local use by function */
    UINT4 *pChnlNum, *pRawChnlNum;
    UINT4 *pChnlRate, *pRawChnlRate;
    UINT4 *pLinearCal;
    UINT4 *pValid;
    UINT4 *pEmptyLoc, numEmptyLoc, newNumEmptyLoc;
    UINT4 emptyLocOffset, linearCaloffset;
    UINT4 sum, tmp, swap;
    UINT4 chnl0, chnl1, chnlNumData;
    UINT4 halfNumCalEntries, updatedNumCalChnl, splitLowRate;

    /* verify pointer to global cpb data */
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* argument checking */
    if(scope > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* When a channel has a bandwidth > 50%, the channel is split into two
       entries.  One entry represents 50% of the channel's bandwidth; the other
       represents the remainder.  Therefore, we need (pSchd->maxChnl)+1),
       instead of (pSchd->maxChnl), worth of memory for pChnlNum and pChnlRate.
    */

    size = sizeof(UINT4)*((pSchd->maxChnl)+1) +
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->maxChnl)+1) +
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->numRows)*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)+
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->numRows)*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW);


    pChnlNum = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pChnlNum == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pRawChnlNum  = (UINT4 *)(pChnlNum + ((pSchd->maxChnl)+1));
    pChnlRate    = (UINT4 *)(pRawChnlNum + (pSchd->maxChnl));
    pRawChnlRate = (UINT4 *)(pChnlRate + ((pSchd->maxChnl)+1));
    pLinearCal   = (UINT4 *)(pRawChnlRate + (pSchd->maxChnl));
    pValid       = (UINT4 *)(pLinearCal + ((pSchd->numRows)*
                             HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW));
    pEmptyLoc    = (UINT4 *)(pValid + (pSchd->maxChnl));

    /* initialize channel data pointers */
    pRdChnl = NULL;
    pWrChnl = NULL;

    /**************************************************************************/
    /*  EXTRACT CHANNEL RATE DATA FOR LOCAL USE BY FUNCTION                   */
    /**************************************************************************/

    chnlOffset = pSchd->portOffset;
    chnlDir = pSchd->intfGrpDir;
    /* extract channel context for making calendar population decisions */
    if (chnlDir){
        /* Interface Group Contains Read Channels/Ports */
        pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl +
                                             chnlOffset);
        j = 0;
        lowestChnlRate = 0xFFFFFFFF;
        maxChnlRate = 0;
        for (i=0; i < pSchd->maxChnl; i++) {
             if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
               ((pRdChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)||
               ((pRdChnl->state == HYPHY20G_CPB_CHNL_EQUIPPED)&&(scope == 1)))){
                *(pRawChnlNum + j) = pRdChnl->chnlId;
                *(pRawChnlRate + j) = pRdChnl->rate;
                totalChnlRate += *(pRawChnlRate+j);
                if (lowestChnlRate > *(pRawChnlRate + j)){
                    lowestChnlRate = *(pRawChnlRate + j);
                }
                if (maxChnlRate < *(pRawChnlRate + j)) {
                    maxChnlRate = *(pRawChnlRate + j);
                }
                numCalChnl++;
                j++;
            }
            pRdChnl++;
        }
    } else {
        /* Interface Group Contains Write Channels/Ports */
        pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pWrChnl +
                                             chnlOffset);
        j = 0;
        lowestChnlRate = 0xFFFFFFFF;
        maxChnlRate = 0;
        for (i=0; i < pSchd->maxChnl; i++) {
             if(((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
               (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL))||
               ((pWrChnl->state == HYPHY20G_CPB_CHNL_EQUIPPED)&&(scope == 1))) {
                *(pRawChnlNum + j) = pWrChnl->chnlId;
                *(pRawChnlRate + j) = pWrChnl->rate;
                totalChnlRate += *(pRawChnlRate + j);
                if (lowestChnlRate > *(pRawChnlRate + j)){
                    lowestChnlRate = *(pRawChnlRate + j);
                }
                if (maxChnlRate < *(pRawChnlRate + j)){
                    maxChnlRate = *(pRawChnlRate + j);
                }
                numCalChnl++;
                j++;
            }
            pWrChnl++;
        }
    }

    /* Update scheduler structure with context gathered from individual
       channels */
    pSchd->totalChnlRate = totalChnlRate;

    /**************************************************************************/
    /*  PROVIDE DATA FOR SIMPLEST POPULATION CASE                             */
    /**************************************************************************/

    /* if totalChnlRate = 0, the entire scheduler must be populated with
       NULL addresses */
    if(totalChnlRate == 0){
        for (i=0; i < pSchd->numRows; i++) {
            for (j=0; j < HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; j++){
                *(pCalData + (HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW*i) + j) =
                    HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /**************************************************************************/
    /*  PUT CHANNEL RATE DATA IN ORDER FROM LOWEST TO HIGHEST RATE            */
    /**************************************************************************/

    /* sort chnlNum and chnlRate so that they are ordered from lowest to
       highest chnlRate */
    for(i=0; i < pSchd->maxChnl; i++){
        if(i < numCalChnl){
            *(pValid + i) = 1;
        } else {
            *(pValid + i) = 0;
        }
    }
    lowestRate = 0xFFFFFFFF;
    for(j=0; j < numCalChnl; j++) {
        /* find lowest valid in set */
        for(i=0; i < numCalChnl; i++) {
            if((*(pRawChnlRate + i) < lowestRate)&& *(pValid + i)){
                lowestRate = *(pRawChnlRate + i);
                lowestIndex = i;
            }
        }
        *(pChnlRate + j) = *(pRawChnlRate + lowestIndex);
        *(pChnlNum + j) = *(pRawChnlNum + lowestIndex);
        *(pValid + lowestIndex) = 0;
        lowestRate = 0xFFFFFFFF;
    }

    /* retrieve scheduler context to determine size of scheduler calendar
       needing to be populated */
    numCalEntries = HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW * (pSchd->numRows);

    /**************************************************************************/
    /*     DEBUG PRINT INITIAL DATA                                           */
    /**************************************************************************/
    DBG_PRINT("%s, %s, %d, Initial Rate Data: with numCalEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries);
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
            DBG_PRINT("%s, %s, %d, chnl = %u, rate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pChnlRate + i));
        }
    }

    /**************************************************************************/
    /*  END DEBUG PRINT INITIAL DATA                                          */
    /**************************************************************************/

    /* provide scheduler contents for the Interlaken Packet Mode 2 x 10G POS
       mode to ensure calendar populated in a bonus mode manner */
    if(pSchd->schdId == HYPHY20G_CPB_PIF_RD_SCHD && pSchd->schdMode == HYPHY20G_SCBS_SCHD_BONUS_MODE_ENABLE){
        DBG_PRINT("%s, %s, %d, Interlaken Packet 2 x 10G POS setup\n",__FILE__, __FUNCTION__, __LINE__);
        switch(numCalChnl){
            case 0: /* no channels */
                chnl0 = HYPHY20G_CPB_SCHD_NULL_CHNL;
                chnl1 = HYPHY20G_CPB_SCHD_NULL_CHNL;
                break;
            case 1: /* one channel */
                chnl0 = *(pChnlNum + 0);
                chnl1 = HYPHY20G_CPB_SCHD_NULL_CHNL;
                break;
            case 2: /* two channels */
                chnl0 = *(pChnlNum + 0);
                chnl1 = *(pChnlNum + 1);
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_TOO_MANY_CHANNEL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_TOO_MANY_CHANNEL;
        }
        /* apply to scheduler entry format */
        for (i=0; i < pSchd->numRows; i++) {
            for (j=0; j < HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; j++){
                chnlNumData = ( j==3 || j==0 ) ? chnl0:chnl1;
                *(pCalData + (HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW*i) + j) =
                    chnlNumData;
            }
        }
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }


    DBG_PRINT("%s, %s, %d, Got to here: numCalEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries);
    if (numCalChnl > numCalEntries){
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_ERR_SCHD_CAL_SIZE;
    }

    /**************************************************************************/
    /*  SCALE CALENDAR ENTRIES                                                */
    /**************************************************************************/

	/* ensure totalChnlRate > numCalEntries */
    if (totalChnlRate < numCalEntries){

        chnlToCalMultFactor = (numCalEntries/totalChnlRate)+1;

        for (i=0; i < numCalChnl; i++){

            *(pChnlRate + i) *= chnlToCalMultFactor;

        }
        totalChnlRate *= chnlToCalMultFactor;
    }

    /* scale required calendar entries so that they can be contained in
       the number of available calendar entries ensuring that at least one
       entry is present for a channel */
    workingRate = totalChnlRate;
    workingEntries = numCalEntries;
    for(i=0; i < numCalChnl; i++){
        DBG_PRINT("%s, %s, %d, chnl = %u, workingRate = %u, workingEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)workingRate, (unsigned int)workingEntries);
        if(i == (numCalChnl-1)){
            /* ensure last channel consumes remainder of calendar entries */
            *(pChnlRate+i) = workingEntries;
        } else {
            chnlRate = *(pChnlRate+i);

            /* check for failing conditions */
            if ((workingRate == 0) ||
                ((chnlRate*workingEntries) > (0xFFFFFFFF/100))){
                sysHyPhy20gMemFree(pChnlNum);
                return HYPHY20G_ERR_SCHD_ALGO_FAIL;
            }

            scaleEntries = (chnlRate*workingEntries*100)/workingRate;

            if ((scaleEntries%100) > 49){
                DBG_PRINT("%s, %d, round up\n",__FUNCTION__, __LINE__);
                scaleEntries = (scaleEntries/100) + 1;
            } else {
                DBG_PRINT("%s, %d, round down\n",__FUNCTION__, __LINE__);
                scaleEntries = scaleEntries/100;
            }

            /* ensure a minimum of 1 entry */
            if(scaleEntries == 0){
                scaleEntries = 1;
            }

            /* update results */
            workingRate -= *(pChnlRate+i);
            workingEntries -= scaleEntries;
            *(pChnlRate+i) = scaleEntries;
        }
    }

    /* Need to ensure the above always populates the number of entries for
       the calendar, the sum must be equal to the total number of entries or we
       will not be able to populate to the calendar correctly */

    /**************************************************************************/
    /*  EVENLY DISTRIBUTE CALENDAR ENTRIES                                    */
    /**************************************************************************/

    /* populate the calendar so that entries are roughly evenly distributed */
    /* start with the highest bandwidth channels */
    for(i=0; i < numCalEntries; i++){
        *(pLinearCal + i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
        *(pEmptyLoc + i) = i;
    }
    numEmptyLoc = newNumEmptyLoc = numCalEntries;

    /* Test for entry greater than 50% calendar length and split into two
       entries if necessary.  This is where we might need the
       (pSchd->maxChnl)+1) entry.
    */
    halfNumCalEntries = numCalEntries/2;
    updatedNumCalChnl = numCalChnl;
    for (i = 0; i < numCalChnl; i++) {
        if (*(pChnlRate + i) > halfNumCalEntries) {
            /* split into two entries */
            splitLowRate = *(pChnlRate + i) - halfNumCalEntries;
            *(pChnlRate + updatedNumCalChnl) = splitLowRate;
            *(pChnlNum + updatedNumCalChnl) = *(pChnlNum + i);
            updatedNumCalChnl++;

            *(pChnlRate + i) = halfNumCalEntries;
        }
    }
    numCalChnl = updatedNumCalChnl;

    /* order entries from high-to-low based on channel rate */
    do {
        swap = 0;
        for(i=0; i < numCalChnl-1; i++) {
            j = i + 1;
            if (*(pChnlRate + j) > *(pChnlRate + i)) {
                /* swap */
                tmp = *(pChnlRate + i);
                *(pChnlRate + i) = *(pChnlRate + j);
                *(pChnlRate + j) = tmp;

                tmp = *(pChnlNum + i);
                *(pChnlNum + i) = *(pChnlNum + j);
                *(pChnlNum + j) = tmp;

                swap = 1;
            }
        }
    } while (swap);



    /**************************************************************************/
    /*  DEBUG PRINT ORDERED SCALED DATA                                       */
    /**************************************************************************/
    DBG_PRINT("%s, %s, %d, Ordered Final Rate Data: with numCalEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries);
    sum = 0;
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
    /* this prints excess information */
            DBG_PRINT("%s, %s, %d, chnl = %u, rate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pChnlRate + i));
            sum = sum + *(pChnlRate + i);
        }
    }
    DBG_PRINT("%s, %s, %d, Final Rate Sum = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)sum);
    /**************************************************************************/
    /*  END DEBUG PRINT ORDERED SCALED DATA                                   */
    /**************************************************************************/

    /* STRICT POPULATION ALGO */
    for (i=0; i < numCalChnl; i++) {
        DBG_PRINT("DEBUG_JC 1: numEmptyLoc = %lu, newNumEmptyLoc = %lu\n", numEmptyLoc, newNumEmptyLoc);

        populateRate = *(pChnlRate + i);
        populateChnl = *(pChnlNum + i);
        /* identify the required number of calendar entries between each
           occurance of this channel number */
        calSpace = numEmptyLoc/populateRate;
        calSingles = numEmptyLoc%populateRate;
        if (calSpace == 0) {
            DBG_PRINT("DEBUG_JC 1a: calSpace forced to 1\n");
            calSpace = 1;
        }

        DBG_PRINT("DEBUG_JC 2: LINE %d\n", __LINE__);
        emptyLocOffset = 0;
        for (j=0; j < populateRate; j++) {
            /* fetch location of empty entry, alternating between start and
               end of list */
            if(j==0) {
                emptyLocOffset = 0;
            } else {
                if(calSingles){
                    emptyLocOffset += (calSpace+1);
                    calSingles--;
                } else {
                    emptyLocOffset += calSpace;
                }
            }
            linearCaloffset = *(pEmptyLoc + emptyLocOffset);

            /*pPopulate calendar with channel number */
           *(pLinearCal + linearCaloffset) = populateChnl;

           /* remove empty location from list */
           *(pEmptyLoc + emptyLocOffset) = 0xFFFF;
           newNumEmptyLoc--;
        }

        DBG_PRINT("DEBUG_JC 3: LINE %d\n", __LINE__);
        /* push all empty locations to front of list while maintaining order */
        do {
            swap = 0;
            for(j=0; j < numCalEntries-1; j++) {
                k = j + 1;
                if (*(pEmptyLoc + k) < *(pEmptyLoc + j)) {
                    /* swap */
                    tmp = *(pEmptyLoc + j);
                    *(pEmptyLoc + j) = *(pEmptyLoc + k);
                    *(pEmptyLoc + k) = tmp;

                    swap = 1;
                }
            }
        } while (swap);

        numEmptyLoc = newNumEmptyLoc;
    }

    DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);

    /**************************************************************************/
    /*  FORMAT CALENDAR ENTRY DATA TO ROWS OF SCBS                            */
    /**************************************************************************/

    /* apply the linear calendar data to the device scheduler calendar
       format */
    j = 0;
    for(i = 0; i < pSchd->numRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 1)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    for(i=0; i < pSchd->numRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 2)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 3)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    /* free allocated memory used by local function */
    sysHyPhy20gMemFree(pChnlNum);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBGenerateIntfGrpSchd */

/*******************************************************************************
**
**  cpbHyPhy20gCpbQdrSchdCmp
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Compares contents of two schedulers and if not identical
**                  sets 'different' to 1.
**
**  INPUTS:         fileHndl  - device handle
**                  *pSchd    - pointer to scheduler containing the online
**                              calendar data to compare new calendar data
**                              against
**                  *pCalData - poiter to new calendar data
**                  *pDiff    - pointer to parameter to be updated by function
**                              different = 1 online and new calendar do not
**                              match
**                              different = 0 online and new calendar match
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbQdrSchdCmp(struct file *fileHndl, sHYPHY20G_SCHD_CTXT *pSchd,
                              UINT4 *pCalData, UINT4 *pDiff)
{
    sHYPHY20G_SCHD_GRP_CTXT *pScbsGrpCtxt;
    UINT4 entries, i, length;

    pScbsGrpCtxt = pSchd->pScbsGrpCtxt;
    length = pScbsGrpCtxt->grpNumRows;

    entries = HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW * length;

    *pDiff = 0;
    i = 0;

    while((*pDiff == 0)&&(i < entries)){
        if((*(pSchd->pOnLineCalMem+i)) != (*(pCalData+i))){
            *pDiff = 1;
        }
        i++;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbQdrSchdCmp */

/* FUNCTION USED TO ASSIST IN GENERATING SCHEDULER CONTENTS */
/*******************************************************************************
**
**  cpbHyPhy20gQdrScaleDataToCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Function to take the raw data for one direction of data
**                  travel through QDR scheduler and adapt the data to the
**                  number of available entries in the QDR scheduler calendar.
**
**                  The adaption will ensure that the same channel value is
**                  not in adjacent entries.  For GFPA and GFPB write interface,
**                  calendar entries may not be closer than 8 entries apart.
**                  This function applies this rule to both GFPA and GFPB read
**                  and write interfaces.
**
**                  The QDR scheduler will be populated with entries for each
**                  used interface group.  GFPA and GFPB interface group entries
**                  will be populated relative to GFPA and GFPB data rate.
**
**                  This is a PRIVATE function.
**
**  INPUTS:        *pRateData - pointer to an array of rate data
**                 *pChnlData - pointer to an array of channel number data
**                 *pLinArray - pointer to a linear list of the scheduler
**                              content data generated by this function
**                  calEntries- number of entries in the scheduler calendar that
**                              are used for data flow in a single direction
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gQdrScaleDataToCal(UINT4 *pRateData, UINT4 *pChnlData,
                                  UINT4 *pLinArray, UINT4 calEntries)
{

    /* QDR scheduler contains interleaved schedulers for QDR read and
       QDR write ports.

       This calendar population assumes that the scheduler uses 15 rows for
       interface group data.

       Requirement 1:

       At the QDR scheduler level, no channel may appear closer than 4 calendar
       entries appart.  When considering this scheduler as two seperate read and
       write ports each having their own linear scheduler no entry may appear
       closer than 2 calendar entries appart (no calendar entry may be the same
       in adjoining entries).

       Requirement 2:

       There is an additional requirement on GFP A and GFP B Wr I/F that the
       scheduler entries be no closer than 8 entries apart (4 entries apart when
       we consider the unidirectional case).

       Requirement 3:

       Another requirement is that these closeness relationship rules must be
       maintained between a set of calendar entries intending to be applied and
       the presently operating calendar data.

       Requirement 1 limits a PIF or LINE I/F channel to occupy a maximum of
       1/2 the port calendar entries.

       Requirement 3 generates the need to provide deterministic locations to
       calendar entries for each interface such that Requirement 1 and 2 are
       not broken when calendar is updated.

       Requirement 2 limits GFP A or GFP B to 7 entries.  As the total
       QDR bandwidth is approx 76.8 Gbps and there are 30 calendar entries for
       each direction of data flow (read and write to QDR), each calendar entry
       represents ~ 2.5Gbps of QDR throughput.  GFP ports support a maximum of
       FC-1200 rate signal, or in the range of 12 Gpbs.  As such with 2.5 Gpbs
       increments, either GFP port would require a maximum of 5 calendar entries
       to achieve 12 Gbps of availability to the interface.

       In order to generate deterministic interface locations in the calendar,
       the following policy is applied (When considering a single direction):

       1) PIF entires will always be in even calendar locations
       2) LINE I/F entries will always be in odd calendar locations
       3) GFP A entries will always be isolated to entries 0, 5, 10, 15, 20, 25
       4) GFP B entries will always be isolated to entries 1, 6, 11, 16, 21, 26

       A pending calendar data set will initially be populate with the NULL
       channel (0xFF).

       If PIF or LINE I/F are present the 50% of entries
       allocated to each will be populated with the respective interface
       number.

       When the GFP interfaces are in use, six calendar entries will always
       be assigned to each interface regardless of its rate.  Six entries are
       applied because the scheduler must provide the maximum VCAT/LCAS
       interface bandwidth to the CPB as this is the rate that CPB must
       both source and sink data from GFP subsystem.
    */
    UINT4 gfpARate = 0, gfpBRate = 0, gfpAChnl = 0, gfpBChnl = 0;
    UINT4 i, j;

    /* initially populate unidirectional calendar with NULL address */
    for(i=0; i < calEntries; i++){
        *(pLinArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
    }

    /* populate LINE I/F and PIF entries to calendar and determine rate of
       GFP interfaces */
    for(i=0; i < HYPHY20G_CPB_NUM_INTF_GRP; i++){
        switch((*(pChnlData+i))){
            /* is there a way to make an OR for two cases if the logic in case is repeated */
            case LINE_WR_INTF_GRP:
                for(j=0; j < (calEntries/2); j++){
                    if (*(pRateData+i) != 0) {
                        *(pLinArray+(j*2)) = *(pChnlData+i);
                    } else {
                        *(pLinArray+(j*2)) = HYPHY20G_CPB_SCHD_NULL_CHNL;
                    }
                }
                break;
            case LINE_RD_INTF_GRP:
                for(j=0; j < (calEntries/2); j++){
                    if (*(pRateData+i) != 0) {
                        *(pLinArray+(j*2)) = *(pChnlData+i);
                    } else {
                        *(pLinArray+(j*2)) = HYPHY20G_CPB_SCHD_NULL_CHNL;
                    }
                }
                break;
            case GFP_A_WR_INTF_GRP:
                gfpARate = *(pRateData+i);
                if (gfpARate != 0) {
                    gfpAChnl = *(pChnlData+i);
                } else {
                    gfpAChnl = HYPHY20G_CPB_SCHD_NULL_CHNL;
                }
                break;
            case GRP_A_RD_INTF_GRP:
                gfpARate = *(pRateData+i);
                if (gfpARate != 0) {
                    gfpAChnl = *(pChnlData+i);
                } else {
                    gfpAChnl = HYPHY20G_CPB_SCHD_NULL_CHNL;
                }
                break;
            case GFP_B_WR_INTF_GRP:
                gfpBRate = *(pRateData+i);
                if (gfpBRate != 0) {
                    gfpBChnl = *(pChnlData+i);
                } else {
                    gfpBChnl = HYPHY20G_CPB_SCHD_NULL_CHNL;
                }
                break;
            case GRP_B_RD_INTF_GRP:
                gfpBRate = *(pRateData+i);
                if (gfpBRate != 0) {
                    gfpBChnl = *(pChnlData+i);
                } else {
                    gfpBChnl = HYPHY20G_CPB_SCHD_NULL_CHNL;
                }
                break;
            case PIF_WR_INTF_GRP:
                for(j=0; j < (calEntries/2); j++){
                    if (*(pRateData+i) != 0) {
                        *(pLinArray+(j*2)+1) = *(pChnlData+i);
                    } else {
                        *(pLinArray+(j*2)+1) = HYPHY20G_CPB_SCHD_NULL_CHNL;
                    }
                }
                break;
            case PIF_RD_INTF_GRP:
                for(j=0; j < (calEntries/2); j++){
                    if (*(pRateData+i) != 0) {
                        *(pLinArray+(j*2)+1) = *(pChnlData+i);
                    } else {
                        *(pLinArray+(j*2)+1) = HYPHY20G_CPB_SCHD_NULL_CHNL;
                    }
                }
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }

    /* overwrite required calendar entries for GFP A */
    /* assumes rate data is 1Gbps per token */
    if(gfpARate != 0){
        /* populate first entry */
        *(pLinArray+0) = gfpAChnl;
        *(pLinArray+5) = gfpAChnl;
        *(pLinArray+10) = gfpAChnl;
        *(pLinArray+15) = gfpAChnl;
        *(pLinArray+20) = gfpAChnl;
        *(pLinArray+25) = gfpAChnl;
    }

    /* overwrite required calendar entries for GFP B */
    /* assumes rate data is 1Gbps per token */
    if(gfpBRate != 0){
        /* populate first entry */
        *(pLinArray+1) = gfpBChnl;
        *(pLinArray+6) = gfpBChnl;
        *(pLinArray+11) = gfpBChnl;
        *(pLinArray+16) = gfpBChnl;
        *(pLinArray+21) = gfpBChnl;
        *(pLinArray+26) = gfpBChnl;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gQdrScaleDataToCal */


/*******************************************************************************
**
**  cpbHyPhy20gPutQdrArrayInOrder
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Data manipultion function to sort four array elements in
**                  order from smallest to highest.
**
**                  This is a PRIVATE function.
**
**  INPUTS:        *pRawData -  pointer to a 4 element array of raw data to
**                 *pRawChnl -  pointer to a 4 element array of meta data
**                              (channel number) associated with pRawData
**                 *pSortData - pointer to a 4 element array of the data from
**                              pRawData put in order form lowest to highest
**                 *pSortChnl - pointer to a 4 element array of meta data
**                              (channel number) associated with pSortData
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gPutQdrArrayInOrder(UINT4 *pRawData, UINT4 *pRawChnl,
                                   UINT4 *pSortData, UINT4 *pSortChnl)
{
    UINT4 lowestRate = 0xFFFFFFFF;
    UINT4 valid[4], i, j, lowestIndex = 0;

    valid[0] = 1;
    valid[1] = 1;
    valid[2] = 1;
    valid[3] = 1;
    for(j=0; j < HYPHY20G_CPB_NUM_INTF_GRP; j++) {
        /* find lowest valid in set */
        for(i=0; i < HYPHY20G_CPB_NUM_INTF_GRP; i++) {
            if((*(pRawData+i) < lowestRate)&& valid[i]){
                lowestRate = *(pRawData+i);
                lowestIndex = i;
            }
        }
        *(pSortData+j) = *(pRawData+lowestIndex);
        *(pSortChnl+j) = *(pRawChnl+lowestIndex);
        valid[lowestIndex] = 0;
        lowestRate = 0xFFFFFFFF;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gPutQdrArrayInOrder */

/* FUNCTIONS TO INITIALIZE AND RESET SCHEDULERS */
/*******************************************************************************
**
**  cpbHyPhy20gCPBInitQdrSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes the QDR Scheduler and memory used by software
**                  to operate the scheduler.
**
**  INPUTS:         fileHndl  - device handle
**                  length    - number of 4 entry scheduler addresses
**                            - Legal range for QDR Scheduler
**                                1 to 15
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBInitQdrSchd( struct file  *fileHndl, UINT4 length )
{

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    UINT4 schdMode = HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE;
    INT4 result;
    UINT4 i;

    /* argument checking */
    if ((length < 1)||(length > HYPHY20G_CPB_QDR_SCHD_MAX_NUM_ROW)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    pCpbCtxt = pDevCtxt->pCpbCtxt;

    /* confirm scheduler context is valid */
    if(pCpbCtxt->pSchdCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                                HYPHY20G_CPB_QDR_SCHD);

    /* initialize the QDR scheduler and calendar memory */
    result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd, HYPHY20G_CPB_QDR_SCHD,
                                      length, schdMode);
    if (result) return result;

    /* initialize memory used for operation of QDR scheduler */
    for (i=0; i < HYPHY20G_CPB_NUM_INTF_GRP; i++){
        result = cpbHyPhy20gCPBInitIntfGrpCtxt(fileHndl, i);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBInitQdrSchd */

/*******************************************************************************
**
**  cpbHyPhy20gCPBResetQdrSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Resets QDR Scheduler and frees memory used by software
**                  to operate the scheduler.
**
**  INPUTS:         fileHndl  - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBResetQdrSchd( struct file  *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    INT4 result;
    UINT4 i;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* recover pointer to scheduler context */
    pSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt +
                                    HYPHY20G_CPB_QDR_SCHD);

    /* initialize the QDR scheduler and calendar memory */
    result = utilHyPhy20gScbsResetSchd(fileHndl, pSchd);
    if (result) return result;

    /* initialize memory used for operation of QDR scheduler */
    for (i=0; i < HYPHY20G_CPB_NUM_INTF_GRP; i++){
        result = cpbHyPhy20gCPBResetIntfGrpCtxt(fileHndl, i);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBResetQdrSchd */

/*******************************************************************************
**
**  cpbHyPhy20gCPBInitIntfGrpCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes the context for one of the interface groups
**                  contained in the CPB subsystem.
**
**                  This is a PRIVATE function
**
**  INPUTS:         fileHndl  - device handle
**                  intfGrp   - interface group scheduler to intialize
**                              0 - CPB CPB Line Interface Group
**                              1 - CPB GFP A Interface Group
**                              2 - CPB GFP B Interface Group
**                              3 - CPB PIF Interface Group
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBInitIntfGrpCtxt( struct file  *fileHndl, UINT4 intfGrp)
{

    UINT4 maxWrChnl, maxRdChnl;
    UINT4 wrRate = 0, rdRate = 0;
    sHYPHY20G_INTF_GRP_CTXT *pIntfGrp;
    sHYPHY20G_CTXT *pDevCtxt;


    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((intfGrp >= HYPHY20G_CPB_NUM_INTF_GRP)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    switch(intfGrp){
        case HYPHY20G_CPB_LINE_INTF_GRP:
            maxWrChnl = HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL;
            maxRdChnl = HYPHY20G_CPB_LINE_RD_SCHD_MAX_CHNL;
            break;
        case HYPHY20G_CPB_GFPA_INTF_GRP:
            maxWrChnl = HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL;
            maxRdChnl = HYPHY20G_CPB_GFPA_RD_SCHD_MAX_CHNL;
            break;
        case HYPHY20G_CPB_GFPB_INTF_GRP:
            maxWrChnl = HYPHY20G_CPB_GFPB_WR_SCHD_MAX_CHNL;
            maxRdChnl = HYPHY20G_CPB_GFPB_RD_SCHD_MAX_CHNL;
            break;
        case HYPHY20G_CPB_PIF_INTF_GRP:
            maxWrChnl = HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL;
            maxRdChnl = HYPHY20G_CPB_PIF_RD_SCHD_MAX_CHNL;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to scheduler context */
    pIntfGrp = (sHYPHY20G_INTF_GRP_CTXT *)(pDevCtxt->pCpbCtxt->pIfGrpCtxt
                                           + intfGrp);

    pIntfGrp->valid = HYPHY20G_VALID_SIGN;
    pIntfGrp->enable = HYPHY20G_CPB_INTF_DISABLED;
    pIntfGrp->intfGrp = intfGrp;
    pIntfGrp->totalWrRate = wrRate;
    pIntfGrp->totalRdRate = rdRate;
    pIntfGrp->numWrChnls = maxWrChnl;
    pIntfGrp->numRdChnls = maxRdChnl;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBInitIntfGrpCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCPBResetIntfGrpCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Resets the context used by an interface group
**
**  INPUTS:         fileHndl  - device handle
**                  intfGrp   - interface group scheduler to intialize
**                              0 - CPB CPB Line Interface Group
**                              1 - CPB GFP A Interface Group
**                              2 - CPB GFP B Interface Group
**                              3 - CPB PIF Interface Group
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBResetIntfGrpCtxt( struct file  *fileHndl, UINT4 intfGrp)
{

    sHYPHY20G_INTF_GRP_CTXT *pIntfGrp;
    sHYPHY20G_CTXT *pDevCtxt;


    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((intfGrp >= HYPHY20G_CPB_NUM_INTF_GRP)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to scheduler context */
    pIntfGrp = (sHYPHY20G_INTF_GRP_CTXT *)(pDevCtxt->pCpbCtxt->pIfGrpCtxt
                                           + intfGrp);

    pIntfGrp->valid = HYPHY20G_INVALID_VAL_16BIT;
    pIntfGrp->enable = HYPHY20G_CPB_INTF_DISABLED;
    pIntfGrp->intfGrp = intfGrp;
    pIntfGrp->totalWrRate = 0;
    pIntfGrp->totalRdRate = 0;
    pIntfGrp->numWrChnls = 0;
    pIntfGrp->numRdChnls = 0;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBResetIntfGrpCtxt */

/* FUNCTIONS TO UPDATE SCHEDULER CALENDAR MEMORY WITH USER DATA */


/*******************************************************************************
**
**  cpbHyPhy20gCpbFifoStatusReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Resets the FIFO channel to guarantee FIFO depth is
**                  consistent with the read and write pointers.
**
**  INPUTS:         fileHndl  - device handle
**                  chnlId    - FIFO channel
**                              Legal range: 0 - 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbFifoStatusReset(struct file *fileHndl, UINT4 chnlId)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 addr;
    UINT4 val = 0;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* Confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Check input arguments */
    if (chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Poll register bit to make sure bit is cleared */
    addr = HYPHY20G_CPB_CPB_REG_FIFO_STAT_RST;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                    HYPHY20G_CPB_CPB_BIT_FIFO_STAT_RST_FIFO_CLEAR_MSK, 0, 1);
    if (result) return result;

    /* Construct register value */
    val = mHYPHY20G_SET_FIELD4(val, chnlId,
            HYPHY20G_CPB_CPB_BIT_FIFO_STAT_RST_FIFO_CLEAR_ADDR_MSK,
            HYPHY20G_CPB_CPB_BIT_FIFO_STAT_RST_FIFO_CLEAR_ADDR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_CPB_CPB_BIT_FIFO_STAT_RST_FIFO_CLEAR_MSK,
            HYPHY20G_CPB_CPB_BIT_FIFO_STAT_RST_FIFO_CLEAR_OFF);

    /* Initiate FIFO clearing */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* Poll register bit */
    result = sysHyPhy20gPollBit(fileHndl, addr,
                    HYPHY20G_CPB_CPB_BIT_FIFO_STAT_RST_FIFO_CLEAR_MSK, 0, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbFifoStatusReset */


/*******************************************************************************
**
**  cpbHyPhy20gCpbToggleChnlRstGo
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets and unsets the CHAN_RESET_GO bit so that all channel
**                  reset states are captured and implemented.
**
**  INPUTS:         fileHndl  - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbToggleChnlRstGo(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 addr;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* Confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Poll register bit to make sure bit is cleared */
    addr = HYPHY20G_CPB_CPB_REG_CHNL_RST;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                    HYPHY20G_CPB_CPB_BIT_CHNL_RST_CHAN_RESET_GO_MSK, 0, 1);
    if (result) return result;

    /* Set bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_CPB_CPB_BIT_CHNL_RST_CHAN_RESET_GO_OFF, 1);
    if (result) return result;

    /* Wait at least 400ns */
    result = sysHyPhy20gUsDelay(1);
    if(result) return result;

    /* Clear bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_CPB_CPB_BIT_CHNL_RST_CHAN_RESET_GO_OFF, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbToggleChnlRstGo */


/*******************************************************************************
**
**  cpbHyPhy20gCpbPifWrPortTypePresent
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets and unsets the packet interface write port type
**                  present bit.
**
**  INPUTS:         fileHndl  - device handle
**                  chnlId    - pif wr channel
**                              range: 0 to 146
**                typePresent - determines if packet has a 4-byte header with
**                              TYPE field
**                              range: 0 to 1
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbPifWrPortTypePresent(struct file *fileHndl, UINT4 chnlId,
                                        UINT4 typePresent)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 addr, val;

DBG_PRINT("%s, %s, %d, chnlId=%u, type=%u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)chnlId, (unsigned int)typePresent);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* Confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Check input arguments */
    if (chnlId > HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (typePresent > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Packet Interface Write Port Configuration */
    addr = HYPHY20G_CPB_CPB_REG_PAC_IF_WR_PRT_CFG_OFFSET(chnlId);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* Configure PI_WR_TYPE_PRESENT field */
    val = mHYPHY20G_SET_FIELD4(val, typePresent,
            HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_TYPE_PRESENT_MSK,
            HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_TYPE_PRESENT_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbPifWrPortTypePresent */


/*******************************************************************************
**
**  cpbHyPhy20gCpbChkChnlORdValidState
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function determines if the specified channel ID
**                  read and write context is in HYPHY20G_VALID_SIGN state and
**                  returns the OR'd value of both states.
**
**  INPUTS:         fileHndl  - device handle
**                  chnlId    - channel ID
**                              legal range 0 to 297
**                  valid     - channel valid state
**                              legal values 0 and 1
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbChkChnlORdValidState(struct file *fileHndl, UINT4 chnlId,
                                        UINT4 *valid)
{
    sHYPHY20G_RD_CHNL_CTXT *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 wrValid, rdValid;

DBG_PRINT("%s, %s, %d, chnlId=%u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)chnlId);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* Confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Check input arguments */
    if (chnlId > HYPHY20G_CPB_NUM_RD_PORT - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlId < HYPHY20G_CPB_NUM_WR_PORT) {
        pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                     (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);
        if (pWrChnl->valid == HYPHY20G_VALID_SIGN) {
            wrValid = 1;
        } else {
            wrValid = 0;
        }
    } else {
        /* In this case, write channel ID is out of range but that's ok,
           since the final value is OR'd between wrValid and rdValid */
        wrValid = 0;
    }

    if (chnlId < HYPHY20G_CPB_NUM_RD_PORT) {
        pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                     (pDevCtxt->pCpbCtxt->pRdChnl + chnlId);
        if (pRdChnl->valid == HYPHY20G_VALID_SIGN) {
            rdValid = 1;
        } else {
            rdValid = 0;
        }
    } else {
        /* should never get here */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    *valid = wrValid | rdValid;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbChkChnlORdValidState */


/*******************************************************************************
**
**  cpbHyPhy20gCpbLineIntfLowPwrMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the low power mode of the
**                  high and low speed clients of the line interface.
**
**  INPUTS:         fileHndl  - device handle
**                  chnlId    - channel ID
**                              legal range 0 to 18
**                  lowPwr    - low power state
**                              legal values 0 and 1
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbLineIntfLowPwrMode(struct file *fileHndl, UINT4 chnlId,
                                      UINT4 lowPwr)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 lowPwrBitOff;
    INT4 result;

DBG_PRINT("%s, %s, %d, chnlId=%u, lowPwr=%u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)chnlId, (unsigned int)lowPwr);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* Confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Check input arguments */
    if ((chnlId > HYPHY20G_CPB_LINE_MAX_NUM_CHNL) || (lowPwr > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlId == 18) {
        /* MPIF - do nothing */
    } else {
        lowPwrBitOff = chnlId + 1;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_LOW_PWR,
                                     lowPwrBitOff, lowPwr);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbLineIntfLowPwrMode */


/*******************************************************************************
**
**  cpbHyPhy20gCpbIntfInitPorts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs initialization required for the
**                  operation of channels within an interface group.  The
**                  interface groups are Client, PIF, GFP A and GFP B.
**
**                  Function will set up the read scheduler and the write
**                  scheduler for the interface group.  The PIF has not write
**                  scheduler.
**
**                  Alternatively, this function can be called to disable an
**                  interface group.
**
**  INPUTS:         *fileHndl - base address
**                  intfGrp   - 0 - Line
**                              1 - GFP A
**                              2 - GFP B
**                              3 - PIF
**
**  NOTE:           The specification of ramType is hardware dependent.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbIntfInitPorts(struct file *fileHndl, UINT4 intfGrp)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    UINT2 i, wrStartChnl, wrEndChnl, rdStartChnl, rdEndChnl;
    INT4 result;

    /* argument checking */
    if (intfGrp > 3){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    pCpbCtxt = pDevCtxt->pCpbCtxt;

    /* confirm scheduler context is valid */
    if(pCpbCtxt->pSchdCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* indentify parameters for the respective intf group scheduler(s) */
    switch(intfGrp){
        case HYPHY20G_CPB_LINE_INTF_GRP:
            wrStartChnl = rdStartChnl = HYPHY20G_CPB_LINE_PORT_OFFSET;
            wrEndChnl = rdEndChnl = HYPHY20G_CPB_GFPA_PORT_OFFSET - 1;
            break;
        case HYPHY20G_CPB_GFPA_INTF_GRP:
            wrStartChnl = rdStartChnl = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            wrEndChnl = rdEndChnl = HYPHY20G_CPB_GFPB_PORT_OFFSET - 1;
            break;
        case HYPHY20G_CPB_GFPB_INTF_GRP:
            wrStartChnl = rdStartChnl = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            wrEndChnl = rdEndChnl = HYPHY20G_CPB_PIF_PORT_OFFSET - 1;
            break;
        case HYPHY20G_CPB_PIF_INTF_GRP:
            wrStartChnl = rdStartChnl = HYPHY20G_CPB_PIF_PORT_OFFSET;
            wrEndChnl = HYPHY20G_CPB_NUM_WR_PORT - 1;
            rdEndChnl = HYPHY20G_CPB_NUM_RD_PORT - 1;
            break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Initialize write ports */
    for (i = wrStartChnl; i <= wrEndChnl; i++) {
        result = cpbHyPhy20gCpbInitWrPort(fileHndl, i);
        if (result) return result;
    }

    /* Initialize read ports */
    for (i = rdStartChnl; i <= rdEndChnl; i++) {
        result = cpbHyPhy20gCpbInitRdPort(fileHndl, i);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbIntfInitPorts */


/*******************************************************************************
**
**  cpbHyPhy20gCpbQdrPhyInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes the QDR PHY.
**
**  INPUTS:         fileHndl    - device handle
**                  enable      - 1 - enable QDR PHY
**                                0 - disable QDR PHY
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbQdrPhyInit(struct file *fileHndl, UINT4 enable)
{
    UINT4 phasePol0, phase0, phasePol1, phase1, phasePol2, phase2;
    UINT4 phaseCtlReg0, phaseCtlReg1, phaseCtlReg2;
    UINT4 phasePolMsk, phaseMsk;

    UINT4 kCpfr0, dCpfr0, cqCpfr0, cqbCpfr0, caCpfr;
    UINT4 pdlCoaReg0;
    UINT4 kCpfr1, dCpfr1, cqCpfr1, cqbCpfr1;
    UINT4 pdlCoaReg1;
    UINT4 kCpfr2, dCpfr2, cqCpfr2, cqbCpfr2;
    UINT4 pdlCoaReg2;
    UINT4 kCpfrMsk, dCpfrMsk, cqCpfrMsk, cqbCpfrMsk, caCpfrMsk;

    UINT4 perThMax0, perThMin0;
    UINT4 pdlPvtCmpReg0;
    UINT4 perThMax1, perThMin1;
    UINT4 pdlPvtCmpReg1;
    UINT4 perThMax2, perThMin2;
    UINT4 pdlPvtCmpReg2;
    UINT4 perThMaxMsk, perThMinMsk;

    UINT4 cqDlyMax0, cqDlyMin0;
    UINT4 cqPvtAdjReg0;
    UINT4 cqDlyMax1, cqDlyMin1;
    UINT4 cqPvtAdjReg1;
    UINT4 cqDlyMax2, cqDlyMin2;
    UINT4 cqPvtAdjReg2;
    UINT4 cqDlyMaxMsk, cqDlyMinMsk;

    UINT4 cqbDlyMax0, cqbDlyMin0;
    UINT4 cqbPvtAdjReg0;
    UINT4 cqbDlyMax1, cqbDlyMin1;
    UINT4 cqbPvtAdjReg1;
    UINT4 cqbDlyMax2, cqbDlyMin2;
    UINT4 cqbPvtAdjReg2;
    UINT4 cqbDlyMaxMsk, cqbDlyMinMsk;

    UINT4 dDlyMax0, dDlyMin0;
    UINT4 dPvtAdjReg0;
    UINT4 dDlyMax1, dDlyMin1;
    UINT4 dPvtAdjReg1;
    UINT4 dDlyMax2, dDlyMin2;
    UINT4 dPvtAdjReg2;
    UINT4 dDlyMaxMsk, dDlyMinMsk;

    UINT4 kDlyMax0, kDlyMin0;
    UINT4 kPvtAdjReg0;
    UINT4 kDlyMax1, kDlyMin1;
    UINT4 kPvtAdjReg1;
    UINT4 kDlyMax2, kDlyMin2;
    UINT4 kPvtAdjReg2;
    UINT4 kDlyMaxMsk, kDlyMinMsk;

    UINT4 caDlyMax, caDlyMin;
    UINT4 caPvtAdjReg;
    UINT4 caDlyMaxMsk, caDlyMinMsk;

    UINT4 clkPerManUpOff;

    UINT4 halfRdCycLat, rddataEnLat, odtMode, odtEn;
    UINT4 qdrPhyCfgReg;
    UINT4 halfRdCycLatMsk, rddataEnLatMsk, odtModeMsk, odtEnMsk;
    UINT4 ddllSwRstMsk0, ddllSwRstMsk1, ddllSwRstMsk2, softRstMsk;

    UINT4 dPipeSel, caPipeSel, kPipeSel;
    UINT4 pdlPvtCmpCfgReg;
    UINT4 dPipeSelMsk, caPipeSelMsk, kPipeSelMsk;

    UINT4 padPvtMult, caPvtMult, clkPvtMult;
    UINT4 padPvtCmpCfgReg2;
    UINT4 padPvtMultMsk, caPvtMultMsk, clkPvtMultMsk;

    UINT4 regData, i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    phasePol0 = 0;
    phase0    = 0x1F;
    phasePol1 = 0;
    phase1    = 0x1F;
    phasePol2 = 0;
    phase2    = 0x1F;

    kCpfr0   = 0x2;
    dCpfr0   = 0;
    cqCpfr0  = 0x2;
    cqbCpfr0 = 0x2;
    kCpfr1   = 0x2;
    dCpfr1   = 0;
    cqCpfr1  = 0x2;
    cqbCpfr1 = 0x2;
    kCpfr2   = 0x3;
    dCpfr2   = 0x1;
    cqCpfr2  = 0x2;
    cqbCpfr2 = 0x2;
    caCpfr   = 0x2;

    perThMax0 = 0x82;
    perThMin0 = 0x32;
    perThMax1 = 0x82;
    perThMin1 = 0x32;
    perThMax2 = 0x82;
    perThMin2 = 0x32;

    kDlyMax0   = 0x6;
    kDlyMin0   = 0x2;
    dDlyMax0   = 0x0;
    dDlyMin0   = 0x0;
    cqDlyMax0  = 0x6;
    cqDlyMin0  = 0x2;
    cqbDlyMax0 = 0x1;
    cqbDlyMin0 = 0x1;

    kDlyMax1   = 0x10;
    kDlyMin1   = 0x6;
    dDlyMax1   = 0x12;
    dDlyMin1   = 0x7;
    cqDlyMax1  = 0x6;
    cqDlyMin1  = 0x2;
    cqbDlyMax1 = 0x1;
    cqbDlyMin1 = 0x1;

    kDlyMax2   = 0xFB;
    kDlyMin2   = 0xFE;
    dDlyMax2   = 0xF9;
    dDlyMin2   = 0xFD;
    cqDlyMax2  = 0x1;
    cqDlyMin2  = 0x1;
    cqbDlyMax2 = 0x3;
    cqbDlyMin2 = 0x1;

    caDlyMax = 0xE;
    caDlyMin = 0x5;

    halfRdCycLat = 0x1;
    rddataEnLat  = 0x1;
    dPipeSel  = 0x0;
    caPipeSel = 0x1;
    kPipeSel  = 0x7;

    odtMode = 0x3;
    odtEn   = 0x1;
    padPvtMult = 0x7;
    caPvtMult  = 0x4;
    clkPvtMult = 0x7;

    phaseCtlReg0 = HYPHY20G_QDR_PHY_DDR_DDLL5_TSB_SLICE_REG_OFFSET(
                                   HYPHY20G_QDR_PHY_DDR_DDLL5_REG_PHASE_CTL, 0);
    phaseCtlReg1 = HYPHY20G_QDR_PHY_DDR_DDLL5_TSB_SLICE_REG_OFFSET(
                                   HYPHY20G_QDR_PHY_DDR_DDLL5_REG_PHASE_CTL, 1);
    phaseCtlReg2 = HYPHY20G_QDR_PHY_DDR_DDLL5_TSB_SLICE_REG_OFFSET(
                                   HYPHY20G_QDR_PHY_DDR_DDLL5_REG_PHASE_CTL, 2);
    phasePolMsk  = HYPHY20G_QDR_PHY_DDR_DDLL5_BIT_PHASE_CTL_PHASEPOL_MSK;
    phaseMsk     = HYPHY20G_QDR_PHY_DDR_DDLL5_BIT_PHASE_CTL_PHASE_MSK;

    pdlCoaReg0 = HYPHY20G_QDR_PHY_TOP_REG_PDL_COA_OFFSET_0;
    pdlCoaReg1 = HYPHY20G_QDR_PHY_TOP_REG_PDL_COA_OFFSET_1;
    pdlCoaReg2 = HYPHY20G_QDR_PHY_TOP_REG_PDL_COA_OFFSET_2;
    kCpfrMsk   = HYPHY20G_QDR_PHY_TOP_BIT_PDL_COA_OFFSET_0_K_CPFR_0_MSK;
    dCpfrMsk   = HYPHY20G_QDR_PHY_TOP_BIT_PDL_COA_OFFSET_0_D_CPFR_0_MSK;
    cqCpfrMsk  = HYPHY20G_QDR_PHY_TOP_BIT_PDL_COA_OFFSET_0_CQ_CPFR_0_MSK;
    cqbCpfrMsk = HYPHY20G_QDR_PHY_TOP_BIT_PDL_COA_OFFSET_0_CQB_CPFR_0_MSK;
    caCpfrMsk  = HYPHY20G_QDR_PHY_TOP_BIT_PDL_COA_OFFSET_0_CA_CPFR_MSK;

    pdlPvtCmpReg0 = HYPHY20G_QDR_PHY_TOP_REG_CLK0_PDL_PVT_CMP_TH;
    pdlPvtCmpReg1 = HYPHY20G_QDR_PHY_TOP_REG_CLK1_PDL_PVT_CMP_TH;
    pdlPvtCmpReg2 = HYPHY20G_QDR_PHY_TOP_REG_CLK2_PDL_PVT_CMP_TH;
    perThMaxMsk =
            HYPHY20G_QDR_PHY_TOP_BIT_CLK0_PDL_PVT_CMP_TH_CLK0_PERIOD_TH_MAX_MSK;
    perThMinMsk =
            HYPHY20G_QDR_PHY_TOP_BIT_CLK0_PDL_PVT_CMP_TH_CLK0_PERIOD_TH_MIN_MSK;

    cqPvtAdjReg0 = HYPHY20G_QDR_PHY_TOP_REG_CQ0_PVT_ADJ_OFFSET_CFG;
    cqPvtAdjReg1 = HYPHY20G_QDR_PHY_TOP_REG_CQ1_PVT_ADJ_OFFSET_CFG;
    cqPvtAdjReg2 = HYPHY20G_QDR_PHY_TOP_REG_CQ2_PVT_ADJ_OFFSET_CFG;
    cqDlyMaxMsk =
            HYPHY20G_QDR_PHY_TOP_BIT_CQ0_PVT_ADJ_OFFSET_CFG_CQ0_DLY_OFF_MAX_MSK;
    cqDlyMinMsk =
            HYPHY20G_QDR_PHY_TOP_BIT_CQ0_PVT_ADJ_OFFSET_CFG_CQ0_DLY_OFF_MIN_MSK;

    cqbPvtAdjReg0 = HYPHY20G_QDR_PHY_TOP_REG_CQB0_PVT_ADJ_OFFSET_CFG;
    cqbPvtAdjReg1 = HYPHY20G_QDR_PHY_TOP_REG_CQB1_PVT_ADJ_OFFSET_CFG;
    cqbPvtAdjReg2 = HYPHY20G_QDR_PHY_TOP_REG_CQB2_PVT_ADJ_OFFSET_CFG;
    cqbDlyMaxMsk =
          HYPHY20G_QDR_PHY_TOP_BIT_CQB0_PVT_ADJ_OFFSET_CFG_CQB0_DLY_OFF_MAX_MSK;
    cqbDlyMinMsk =
          HYPHY20G_QDR_PHY_TOP_BIT_CQB0_PVT_ADJ_OFFSET_CFG_CQB0_DLY_OFF_MIN_MSK;

    dPvtAdjReg0 = HYPHY20G_QDR_PHY_TOP_REG_D0_PVT_ADJ_OFFSET_CFG;
    dPvtAdjReg1 = HYPHY20G_QDR_PHY_TOP_REG_D1_PVT_ADJ_OFFSET_CFG;;
    dPvtAdjReg2 = HYPHY20G_QDR_PHY_TOP_REG_D2_PVT_ADJ_OFFSET_CFG;;
    dDlyMaxMsk =
              HYPHY20G_QDR_PHY_TOP_BIT_D0_PVT_ADJ_OFFSET_CFG_D0_DLY_OFF_MAX_MSK;
    dDlyMinMsk =
              HYPHY20G_QDR_PHY_TOP_BIT_D0_PVT_ADJ_OFFSET_CFG_D0_DLY_OFF_MIN_MSK;

    kPvtAdjReg0 = HYPHY20G_QDR_PHY_TOP_REG_K0_PVT_ADJ_OFFSET_CFG;
    kPvtAdjReg1 = HYPHY20G_QDR_PHY_TOP_REG_K1_PVT_ADJ_OFFSET_CFG;
    kPvtAdjReg2 = HYPHY20G_QDR_PHY_TOP_REG_K2_PVT_ADJ_OFFSET_CFG;
    kDlyMaxMsk =
              HYPHY20G_QDR_PHY_TOP_BIT_K0_PVT_ADJ_OFFSET_CFG_K0_DLY_OFF_MAX_MSK;
    kDlyMinMsk =
              HYPHY20G_QDR_PHY_TOP_BIT_K0_PVT_ADJ_OFFSET_CFG_K0_DLY_OFF_MIN_MSK;

    caPvtAdjReg = HYPHY20G_QDR_PHY_TOP_REG_CA_PVT_ADJ_OFFSET_CFG;
    caDlyMaxMsk =
              HYPHY20G_QDR_PHY_TOP_BIT_CA_PVT_ADJ_OFFSET_CFG_CA_DLY_OFF_MAX_MSK;
    caDlyMinMsk =
              HYPHY20G_QDR_PHY_TOP_BIT_CA_PVT_ADJ_OFFSET_CFG_CA_DLY_OFF_MIN_MSK;

    clkPerManUpOff =
                 HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_CLK_PERIOD_MAN_UPDATE_OFF;

    qdrPhyCfgReg = HYPHY20G_QDR_PHY_TOP_REG_QDR_PHY_CFG;
    halfRdCycLatMsk =
                   HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_HALF_RD_CYC_LATENCY_MSK;
    rddataEnLatMsk = HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_RDDATA_EN_LATENCY_MSK;
    odtModeMsk    = HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_ODT_MODE_MSK;
    odtEnMsk      = HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_ODT_EN_MSK;
    ddllSwRstMsk0 = HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_DDLL0_SW_RST_MSK;
    ddllSwRstMsk1 = HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_DDLL1_SW_RST_MSK;
    ddllSwRstMsk2 = HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_DDLL2_SW_RST_MSK;
    softRstMsk    = HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_SOFT_RST_MSK;

    pdlPvtCmpCfgReg = HYPHY20G_QDR_PHY_TOP_REG_PAD_PVT_CMP_CFG;
    dPipeSelMsk  = HYPHY20G_QDR_PHY_TOP_BIT_PAD_PVT_CMP_CFG_D_PIPELINE_SEL_MSK;
    caPipeSelMsk = HYPHY20G_QDR_PHY_TOP_BIT_PAD_PVT_CMP_CFG_CA_PIPELINE_SEL_MSK;
    kPipeSelMsk  = HYPHY20G_QDR_PHY_TOP_BIT_PAD_PVT_CMP_CFG_K_PIPELINE_SEL_MSK;

    padPvtCmpCfgReg2 = HYPHY20G_QDR_PHY_TOP_REG_PAD_PVT_CMP_CFG_2;
    padPvtMultMsk = HYPHY20G_QDR_PHY_TOP_BIT_PAD_PVT_CMP_CFG_2_PAD_PVT_MULT_MSK;
    caPvtMultMsk  = HYPHY20G_QDR_PHY_TOP_BIT_PAD_PVT_CMP_CFG_2_CA_PVT_MULT_MSK;
    clkPvtMultMsk = HYPHY20G_QDR_PHY_TOP_BIT_PAD_PVT_CMP_CFG_2_CLK_PVT_MULT_MSK;

    if (enable == 1) {
        /* remove top level reset and low power */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                          HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_QDR_PHY_LOWPWR_OFF, 0);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                              HYPHY20G_TOP_BIT_SYS_SW_RST_3_QDR_PHY_RST_OFF, 0);
        if(result) return result;

        /* poll QDR PHY_READY bit to determine if it is ready */
        result = sysHyPhy20gPollBit(fileHndl,
                         HYPHY20G_QDR_PHY_TOP_REG_QDR_PHY_CFG,
                         HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                         HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                         HYPHY20G_DELAY_20USEC);
        if(result) return result;

        /* remove subsystem reset */
        result = sysHyPhy20gRead(fileHndl, qdrPhyCfgReg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, ddllSwRstMsk0, 0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, ddllSwRstMsk1, 0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, ddllSwRstMsk2, 0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, softRstMsk, 0);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, qdrPhyCfgReg, regData);
        if(result) return result;


        /* configure Phase Control Reg 0 */
        result = sysHyPhy20gRead(fileHndl, phaseCtlReg0, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, phasePolMsk, phasePol0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, phaseMsk, phase0);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, phaseCtlReg0, regData);
        if(result) return result;

        /* configure Phase Control Reg 1 */
        result = sysHyPhy20gRead(fileHndl, phaseCtlReg1, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, phasePolMsk, phasePol1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, phaseMsk, phase1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, phaseCtlReg1, regData);
        if(result) return result;

        /* configure Phase Control Reg 2 */
        result = sysHyPhy20gRead(fileHndl, phaseCtlReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, phasePolMsk, phasePol2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, phaseMsk, phase2);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, phaseCtlReg2, regData);
        if(result) return result;


        /* wait for DDLLs to update */
        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
        if(result) return result;


        /* configure PDL Coarse Offset Reg - Lane 0 */
        result = sysHyPhy20gRead(fileHndl, pdlCoaReg0, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kCpfrMsk, kCpfr0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dCpfrMsk, dCpfr0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqCpfrMsk, cqCpfr0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbCpfrMsk, cqbCpfr0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, caCpfrMsk, caCpfr);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, pdlCoaReg0, regData);
        if(result) return result;

        /* configure PDL Coarse Offset Reg - Lane 1 */
        result = sysHyPhy20gRead(fileHndl, pdlCoaReg1, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kCpfrMsk, kCpfr1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dCpfrMsk, dCpfr1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqCpfrMsk, cqCpfr1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbCpfrMsk, cqbCpfr1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, pdlCoaReg1, regData);
        if(result) return result;

        /* configure PDL Coarse Offset Reg - Lane 2 */
        result = sysHyPhy20gRead(fileHndl, pdlCoaReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kCpfrMsk, kCpfr2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dCpfrMsk, dCpfr2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqCpfrMsk, cqCpfr2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbCpfrMsk, cqbCpfr2);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, pdlCoaReg2, regData);
        if(result) return result;

        /* configure CLK0 PDL PVT Compensation Thresholds */
        result = sysHyPhy20gRead(fileHndl, pdlPvtCmpReg0, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, perThMaxMsk, perThMax0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, perThMinMsk, perThMin0);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, pdlPvtCmpReg0, regData);
        if(result) return result;

        /* configure CLK1 PDL PVT Compensation Thresholds */
        result = sysHyPhy20gRead(fileHndl, pdlPvtCmpReg1, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, perThMaxMsk, perThMax1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, perThMinMsk, perThMin1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, pdlPvtCmpReg1, regData);
        if(result) return result;

        /* configure CLK2 PDL PVT Compensation Thresholds */
        result = sysHyPhy20gRead(fileHndl, pdlPvtCmpReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, perThMaxMsk, perThMax2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, perThMinMsk, perThMin2);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, pdlPvtCmpReg2, regData);
        if(result) return result;

        /* configure CQ0 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, cqPvtAdjReg0, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqDlyMaxMsk, cqDlyMax0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqDlyMinMsk, cqDlyMin0);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, cqPvtAdjReg0, regData);
        if(result) return result;

        /* configure CQ1 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, cqPvtAdjReg1, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqDlyMaxMsk, cqDlyMax1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqDlyMinMsk, cqDlyMin1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, cqPvtAdjReg1, regData);
        if(result) return result;

        /* configure CQ2 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, cqPvtAdjReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqDlyMaxMsk, cqDlyMax2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqDlyMinMsk, cqDlyMin2);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, cqPvtAdjReg2, regData);
        if(result) return result;

        /* configure CQB0 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, cqbPvtAdjReg0, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbDlyMaxMsk, cqbDlyMax0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbDlyMinMsk, cqbDlyMin0);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, cqbPvtAdjReg0, regData);
        if(result) return result;

        /* configure CQB1 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, cqbPvtAdjReg1, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbDlyMaxMsk, cqbDlyMax1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbDlyMinMsk, cqbDlyMin1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, cqbPvtAdjReg1, regData);
        if(result) return result;

        /* configure CQB2 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, cqbPvtAdjReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbDlyMaxMsk, cqbDlyMax2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cqbDlyMinMsk, cqbDlyMin2);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, cqbPvtAdjReg2, regData);
        if(result) return result;

        /* configure D0 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, dPvtAdjReg0, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dDlyMaxMsk, dDlyMax0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dDlyMinMsk, dDlyMin0);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, dPvtAdjReg0, regData);
        if(result) return result;

        /* configure D1 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, dPvtAdjReg1, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dDlyMaxMsk, dDlyMax1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dDlyMinMsk, dDlyMin1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, dPvtAdjReg1, regData);
        if(result) return result;

        /* configure D2 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, dPvtAdjReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dDlyMaxMsk, dDlyMax2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dDlyMinMsk, dDlyMin2);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, dPvtAdjReg2, regData);
        if(result) return result;

        /* configure K0 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, kPvtAdjReg0, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kDlyMaxMsk, kDlyMax0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kDlyMinMsk, kDlyMin0);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, kPvtAdjReg0, regData);
        if(result) return result;

        /* configure K1 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, kPvtAdjReg1, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kDlyMaxMsk, kDlyMax1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kDlyMinMsk, kDlyMin1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, kPvtAdjReg1, regData);
        if(result) return result;

        /* configure K2 PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, kPvtAdjReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kDlyMaxMsk, kDlyMax2);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kDlyMinMsk, kDlyMin2);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, kPvtAdjReg2, regData);
        if(result) return result;

        /* configure CA PVT Adjustment Offset */
        result = sysHyPhy20gRead(fileHndl, caPvtAdjReg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, caDlyMaxMsk, caDlyMax);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, caDlyMinMsk, caDlyMin);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, caPvtAdjReg, regData);
        if(result) return result;

        /* step PDLs to their final values */
        for (i = 0; i < 128; i++) {
            result = sysHyPhy20gBitWrite(fileHndl, qdrPhyCfgReg,
                                         clkPerManUpOff, 0);
            if(result) return result;

            result = sysHyPhy20gBitWrite(fileHndl, qdrPhyCfgReg,
                                         clkPerManUpOff, 1);
            if(result) return result;

            result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
            if(result) return result;
        }

        /* configure QDR PHY Reg  */
        result = sysHyPhy20gRead(fileHndl, qdrPhyCfgReg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, halfRdCycLatMsk, halfRdCycLat);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, rddataEnLatMsk, rddataEnLat);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, qdrPhyCfgReg, regData);
        if(result) return result;

        /* configure PDL PVT Compensation */
        result = sysHyPhy20gRead(fileHndl, pdlPvtCmpCfgReg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dPipeSelMsk, dPipeSel);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, caPipeSelMsk, caPipeSel);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, kPipeSelMsk, kPipeSel);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, pdlPvtCmpCfgReg, regData);
        if(result) return result;

        /* configure QDR PHY Reg  */
        result = sysHyPhy20gRead(fileHndl, qdrPhyCfgReg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, odtModeMsk, odtMode);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, odtEnMsk, odtEn);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, qdrPhyCfgReg, regData);
        if(result) return result;

        /* configure PAD PVT Compensation Reg 2 */
        result = sysHyPhy20gRead(fileHndl, padPvtCmpCfgReg2, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, padPvtMultMsk, padPvtMult);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, caPvtMultMsk, caPvtMult);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, clkPvtMultMsk, clkPvtMult);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, padPvtCmpCfgReg2, regData);
        if(result) return result;

        /* wait */
        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_20USEC);
        if(result) return result;

    } else {
        /* place subsystem in reset */
        result = sysHyPhy20gRead(fileHndl, qdrPhyCfgReg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, ddllSwRstMsk0, 1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, ddllSwRstMsk1, 1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, ddllSwRstMsk2, 1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, softRstMsk, 1);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, qdrPhyCfgReg, regData);
        if(result) return result;

        /* place QDR PHY in reset and low power */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                              HYPHY20G_TOP_BIT_SYS_SW_RST_3_QDR_PHY_RST_OFF, 1);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                          HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_QDR_PHY_LOWPWR_OFF, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbQdrPhyInit */


/*******************************************************************************
**
**  cpbHyPhy20gCpbBlockReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configures the reset bit of the specified block in the CFG
**                  register (0x4928000).
**
**  INPUTS:         fileHndl  - device handle
**                  block     - 0 - all blocks (1 to 9)
**                              1 - CPB SOFT_RST
**                              2 - QDR_SCED_RST
**                              3 - PI_RD_SCED_RST
**                              4 - GFPB_RD_SCED_RST
**                              5 - GFPB_WR_SCED_RST
**                              6 - GFPA_RD_SCED_RST
**                              7 - GFPA_WR_SCED_RST
**                              8 - LINE_RD_SCED_RST
**                              9 - LINE_WR_SCED_RST
**                  reset     - reset bit value (0 or 1)
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbBlockReset(struct file *fileHndl, UINT4 block, UINT4 reset)
{
    UINT4 i;
    UINT4 strtBlk;
    UINT4 endBlk;
    UINT4 msk;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (block < 0 || block > 9 ||
        reset < 0 || reset > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (block == 0) {
        strtBlk = 1;
        endBlk = 9;
    } else {
        strtBlk = endBlk = block;
    }

    result = sysHyPhy20gRead(fileHndl, HYPHY20G_CPB_CPB_REG_CFG, &val);
    if(result) return result;

    for (i = strtBlk; i <= endBlk; i++) {
        switch (i) {
            case 1: /* SOFT_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_SOFT_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 2: /* QDR_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_QDR_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 3: /* PI_RD_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_PI_RD_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 4: /* GFPB_RD_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_GFPB_RD_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 5: /* GFPB_WR_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_GFPB_WR_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 6: /* GFPA_RD_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_GFPA_RD_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 7: /* GFPA_WR_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_GFPA_WR_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 8: /* LINE_RD_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_LINE_RD_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            case 9: /* LINE_WR_SCED_RST */
                msk = HYPHY20G_CPB_CPB_BIT_CFG_LINE_WR_SCED_RST_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, reset);
                if(result) return result;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    result = sysHyPhy20gWrite(fileHndl, HYPHY20G_CPB_CPB_REG_CFG, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbBlockReset */

/*******************************************************************************
**
**  cpbHyPhy20gCpbBlockLowPower
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configures the lower power bit of the specified block in
**                  the LOW_POWER register (0x49288BC).
**
**  INPUTS:         fileHndl - device handle
**                  block    - 0  - all blocks (1 to 22)
**                             1  - CPB_LOWPWR
**                             2  - GFPA_LOWPWR
**                             3  - GFPB_LOWPWR
**                             4  - PI_LOWPWR
**                             5  - HS_LOWPWR_0
**                             6  - HS_LOWPWR_1
**                             7  - LS_LOWPWR_0
**                             8  - LS_LOWPWR_1
**                             9  - LS_LOWPWR_2
**                             10 - LS_LOWPWR_3
**                             11 - LS_LOWPWR_4
**                             12 - LS_LOWPWR_5
**                             13 - LS_LOWPWR_6
**                             14 - LS_LOWPWR_7
**                             15 - LS_LOWPWR_8
**                             16 - LS_LOWPWR_9
**                             17 - LS_LOWPWR_10
**                             18 - LS_LOWPWR_11
**                             19 - LS_LOWPWR_12
**                             20 - LS_LOWPWR_13
**                             21 - LS_LOWPWR_14
**                             22 - LS_LOWPWR_15
**                  lowPower - low power bit value (0 or 1)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbBlockLowPower(struct file *fileHndl, UINT4 block,
                                 UINT4 lowPower)
{
    UINT4 i;
    UINT4 strtBlk;
    UINT4 endBlk;
    UINT4 msk;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (block < 0 || block > 22 ||
        lowPower < 0 || lowPower > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (block == 0) {
        strtBlk = 1;
        endBlk = 22;
    } else {
        strtBlk = endBlk = block;
    }

    result = sysHyPhy20gRead(fileHndl, HYPHY20G_CPB_CPB_REG_LOW_PWR, &val);
    if(result) return result;

    for (i = strtBlk; i <= endBlk; i++) {
        switch (i) {
            case 1: /* CPB_LOWPWR */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_CPB_LOWPWR_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 2: /* GFPA_LOWPWR */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_GFPA_LOWPWR_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 3: /* GFPB_LOWPWR */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_GFPB_LOWPWR_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 4: /* PI_LOWPWR */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_PI_LOWPWR_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 5: /* HS_LOWPWR_0 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_HS_LOWPWR_0_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 6: /* HS_LOWPWR_1 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_HS_LOWPWR_1_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 7: /* LS_LOWPWR_0 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_0_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 8: /* LS_LOWPWR_1 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_1_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 9: /* LS_LOWPWR_2 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_2_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 10: /* LS_LOWPWR_3 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_3_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 11: /* LS_LOWPWR_4 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_4_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 12: /* LS_LOWPWR_5 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_5_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 13: /* LS_LOWPWR_6 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_6_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 14: /* LS_LOWPWR_7 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_7_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 15: /* LS_LOWPWR_8 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_8_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 16: /* LS_LOWPWR_9 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_9_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 17: /* LS_LOWPWR_10 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_10_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 18: /* LS_LOWPWR_11 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_11_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 19: /* LS_LOWPWR_12 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_12_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 20: /* LS_LOWPWR_13 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_13_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 21: /* LS_LOWPWR_14 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_14_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            case 22: /* LS_LOWPWR_15 */
                msk = HYPHY20G_CPB_CPB_BIT_LOW_PWR_LS_LOWPWR_15_MSK;
                result = hyPhy20gVarFieldWrite (&val, msk, lowPower);
                if(result) return result;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    result = sysHyPhy20gWrite(fileHndl, HYPHY20G_CPB_CPB_REG_LOW_PWR, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbBlockLowPower */

/*******************************************************************************
**
**  cpbHyPhy20gCpbCheckWrChnlCtxtState
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Compares the write channel context state against the
**                  specified state (ie. pWrChnl->state).
**
**  INPUTS:         fileHndl - device handle
**                  wrChnlId - write channel number
**                  state    - expected state
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbCheckWrChnlCtxtState(struct file *fileHndl, UINT4 wrChnlId,
                                        UINT4 state)
{
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (wrChnlId < 0 || wrChnlId >= HYPHY20G_CPB_NUM_WR_PORT) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    /* confirm write channel state */
    if (pWrChnl->state != state) {
        return HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbCheckWrChnlCtxtState */

/*******************************************************************************
**
**  cpbHyPhy20gCpbCheckWrChnlCtxtValid
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Compares the write channel context valid state against
**                  the specified state (ie. pWrChnl->valid).
**
**  INPUTS:         fileHndl   - device handle
**                  wrChnlId   - write channel number
**                  validState - expected valid state
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbCheckWrChnlCtxtValid(struct file *fileHndl, UINT4 wrChnlId,
                                        UINT4 validState)
{
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (wrChnlId < 0 || wrChnlId >= HYPHY20G_CPB_NUM_WR_PORT) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);

    /* confirm write channel state */
    if (pWrChnl->valid != validState) {
        return HYPHY20G_ERR_INVALID_WRCHNL_PRECONFIG;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbCheckWrChnlCtxtValid */

/*******************************************************************************
**
**  cpbHyPhy20gCpbCheckRdChnlCtxtState
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Compares the read channel context state against the
**                  specified state (ie. pRdChnl->state).
**
**  INPUTS:         fileHndl - device handle
**                  rdChnlId - read channel number
**                  state    - expected state
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbCheckRdChnlCtxtState(struct file *fileHndl, UINT4 rdChnlId,
                                        UINT4 state)
{
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (rdChnlId < 0 || rdChnlId >= HYPHY20G_CPB_NUM_RD_PORT) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* confirm read channel state */
    if (pRdChnl->state != state) {
        return HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbCheckRdChnlCtxtState */

/*******************************************************************************
**
**  cpbHyPhy20gCpbCheckRdChnlCtxtValid
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Compares the read channel context valid state against
**                  the specified state (ie. pRdChnl->valid).
**
**  INPUTS:         fileHndl   - device handle
**                  rdChnlId   - read channel number
**                  validState - expected valid state
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbCheckRdChnlCtxtValid(struct file *fileHndl, UINT4 rdChnlId,
                                        UINT4 validState)
{
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (rdChnlId < 0 || rdChnlId >= HYPHY20G_CPB_NUM_RD_PORT) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* confirm read channel state */
    if (pRdChnl->valid != validState) {
        return HYPHY20G_ERR_INVALID_RDCHNL_PRECONFIG;
    }

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbCheckRdChnlCtxtValid */

/*******************************************************************************
**
**  cpbHyPhy20gCPBSchdRateAwareUpdate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Updates specified interface group schedulers and the QDR
**                  scheduler using read and write channel state context.
**
**  INPUTS:         fileHndl  - device handle
**                  intfGrpMsk - bit mask of interface group(s) to be evaluated.
**                               bit offsets for respective schedulers within
**                               mask:
**                              0 - CPB Line Interface Group Write Scheduler
**                              1 - CPB Line Interface Group Read Scheduler
**                              2 - CPB GFP A Interface Group Write Scheduler
**                              3 - CPB GFP A Interface Group Read Scheduler
**                              4 - CPB GFP B Interface Group Write Scheduler
**                              5 - CPB GFP B Interface Group Read Scheduler
**                              6 - CPB PIF Interface Group Read Scheduler
**                              Note: Only one Write Scheduler may be selected
**                                    Within the intfGrpMsk
**                  rdSchFirst - determines if read or write scheduler will be
**                              configured first:
**                                  1 - configure read scheduler first,
**                                      then configure write scheduler
**                                  0 - configure write scheduler first,
**                                      then configure read scheduler
**                  updateQDR  - Option to update QDR scheduler.
**                               Set to 1 to update QDR scheduler.
**                               Set to 0 to not update QDR scheduler.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBSchdRateAwareUpdate(struct file  *fileHndl, UINT4 intfGrpMsk,
                                       UINT4 rdSchFirst, UINT4 updateQDR)
{

    UINT4 wrSchdNum, size, numRows, startAddr, endAddr, numRowsUsed;
    UINT4 *pCalData = NULL;
    UINT4 *pInterData = NULL;
    UINT4 different = 0, needInter = 0;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if ((intfGrpMsk >= 0x100) || ((intfGrpMsk & 0x15)==0x5) ||
        ((intfGrpMsk & 0x15)==0x11) || ((intfGrpMsk & 0x15)==0x14) ||
        ((intfGrpMsk & 0x15)==0x15)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((rdSchFirst > 1) || (updateQDR > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* allocate memory to pCalData used for holding scheduler contents */
    /* use PIF Scheduler context as this is the largest scheduler */
    size = sizeof(UINT4)*((HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW*
                           HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)+
                          (HYPHY20G_CPB_LINE_WR_SCHD_MAX_NUM_ROW*
                           HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW));

    pCalData = (UINT4 *) sysHyPhy20gMemAlloc(size);
    pInterData = (UINT4 *) (pCalData + (HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW));
    if(pCalData == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_MEM_ALLOC;
    }

    if (rdSchFirst) {
        /* First update read interface group scheduler(s) */
        if(intfGrpMsk & 0x2){
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_LINE_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 0, 0,
                                                                    pSchd, pCalData,
                                                                    &numRows);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler number or rows and content */

            /* test to determine number of rows used by current scheduler */
            result = utilHyPhy20gScbsReadSchdCtxt(fileHndl, pSchd, 0, &startAddr,
                                                  &endAddr);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            numRowsUsed = endAddr - startAddr + 1;
            /* if number of scheduler rows changes apply intermediate scheduler as part of
               resizing number of rows */
            if(numRowsUsed != numRows){
                /* retrieve intermediate scheduler */
                DBG_PRINT("%s, %s, %d, RESIZE ROWS RETRIEVE INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                result = cpbHyPhy20gCPBSchdRateAwareCreateIntermediate(numRowsUsed, pSchd->pOnLineCalMem, 
                                                                       numRows, pCalData, 
                                                                       pInterData, &needInter);
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }               
                /* apply intermediate scheduler if the current scheduler does not meet 
                   entry requirements for beginning and final END_ADDRESS settings */
                if(needInter){
                    DBG_PRINT("%s, %s, %d, RESIZE ROWS APPLY INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                    result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pInterData);
                    if(result){
                        sysHyPhy20gMemFree(pCalData);
                        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                        return result;
                    }
                }
                /* update END_ADDRESS used by scheduler */                
                DBG_PRINT("%s, %s, %d, RESIZE ROWS UPDATE END_ADDRESS\n",__FILE__, __FUNCTION__, __LINE__);
                result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, 0, startAddr,
                                                       (startAddr + numRows - 1));
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x8){
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPA_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x20){
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPB_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x40){
             pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_PIF_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }

        /* Then update write interface group scheduler */
        /* reference write scheduler context */
        if(intfGrpMsk & 0x1){
            wrSchdNum = HYPHY20G_CPB_LINE_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 0, 0,
                                                                    pSchd, pCalData,
                                                                    &numRows);
            DBG_PRINT("%s, %s, %d, numRows =  %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numRows);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler number or rows and content */

            /* test to determine number of rows used by current scheduler */
            result = utilHyPhy20gScbsReadSchdCtxt(fileHndl, pSchd, 0, &startAddr,
                                                  &endAddr);
            DBG_PRINT("%s, %s, %d, startAddr = %u endAddr = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)startAddr, (unsigned int)endAddr);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            numRowsUsed = endAddr - startAddr + 1;
            /* if number of scheduler rows changes apply intermediate scheduler as part of
               resizing number of rows */
            if(numRowsUsed != numRows){
                /* retrieve intermediate scheduler */
                DBG_PRINT("%s, %s, %d, RESIZE ROWS RETRIEVE INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                result = cpbHyPhy20gCPBSchdRateAwareCreateIntermediate(numRowsUsed, pSchd->pOnLineCalMem, 
                                                                       numRows, pCalData, 
                                                                       pInterData, &needInter);
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }               
                /* apply intermediate scheduler if the current scheduler does not meet 
                   entry requirements for beginning and final END_ADDRESS settings */
                if(needInter){
                    DBG_PRINT("%s, %s, %d, RESIZE ROWS APPLY INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                    result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pInterData);
                    if(result){
                        sysHyPhy20gMemFree(pCalData);
                        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                        return result;
                    }
                }
                /* update END_ADDRESS used by scheduler */                
                DBG_PRINT("%s, %s, %d, RESIZE ROWS UPDATE END_ADDRESS\n",__FILE__, __FUNCTION__, __LINE__);
                result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, 0, startAddr,
                                                       (startAddr + numRows - 1));
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }

        if(intfGrpMsk & 0x4){
            wrSchdNum = HYPHY20G_CPB_GFPA_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x10){
            wrSchdNum = HYPHY20G_CPB_GFPB_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }

    } else {
        /* First update write interface group scheduler */
        /* reference write scheduler context */
        if(intfGrpMsk & 0x1){
            wrSchdNum = HYPHY20G_CPB_LINE_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 0, 0,
                                                                    pSchd, pCalData,
                                                                    &numRows);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler number or rows and content */

            /* test to determine number of rows used by current scheduler */
            result = utilHyPhy20gScbsReadSchdCtxt(fileHndl, pSchd, 0, &startAddr,
                                                  &endAddr);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            numRowsUsed = endAddr - startAddr + 1;
            /* if number of scheduler rows changes apply intermediate scheduler as part of
               resizing number of rows */
            if(numRowsUsed != numRows){
                /* retrieve intermediate scheduler */
                DBG_PRINT("%s, %s, %d, RESIZE ROWS RETRIEVE INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                result = cpbHyPhy20gCPBSchdRateAwareCreateIntermediate(numRowsUsed, pSchd->pOnLineCalMem, 
                                                                       numRows, pCalData, 
                                                                       pInterData, &needInter);
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }               
                /* apply intermediate scheduler if the current scheduler does not meet 
                   entry requirements for beginning and final END_ADDRESS settings */
                if(needInter){
                    DBG_PRINT("%s, %s, %d, RESIZE ROWS APPLY INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                    result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pInterData);
                    if(result){
                        sysHyPhy20gMemFree(pCalData);
                        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                        return result;
                    }
                }
                /* update END_ADDRESS used by scheduler */                
                DBG_PRINT("%s, %s, %d, RESIZE ROWS UPDATE END_ADDRESS\n",__FILE__, __FUNCTION__, __LINE__);
                result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, 0, startAddr,
                                                       (startAddr + numRows - 1));
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x4){
            wrSchdNum = HYPHY20G_CPB_GFPA_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x10){
            wrSchdNum = HYPHY20G_CPB_GFPB_WR_SCHD;
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+wrSchdNum;

            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }


        /* Then update read interface group scheduler */
        if(intfGrpMsk & 0x2){
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_LINE_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 0, 0,
                                                                    pSchd, pCalData,
                                                                    &numRows);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler number or rows and content */

            /* test to determine number of rows used by current scheduler */
            result = utilHyPhy20gScbsReadSchdCtxt(fileHndl, pSchd, 0, &startAddr,
                                                  &endAddr);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            numRowsUsed = endAddr - startAddr + 1;
            /* if number of scheduler rows changes apply intermediate scheduler as part of
               resizing number of rows */
            if(numRowsUsed != numRows){
                /* retrieve intermediate scheduler */
                DBG_PRINT("%s, %s, %d, RESIZE ROWS RETRIEVE INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                result = cpbHyPhy20gCPBSchdRateAwareCreateIntermediate(numRowsUsed, pSchd->pOnLineCalMem, 
                                                                       numRows, pCalData, 
                                                                       pInterData, &needInter);
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }               
                /* apply intermediate scheduler if the current scheduler does not meet 
                   entry requirements for beginning and final END_ADDRESS settings */
                if(needInter){
                    DBG_PRINT("%s, %s, %d, RESIZE ROWS APPLY INTERMEDIATE SCHEDULER\n",__FILE__, __FUNCTION__, __LINE__);
                    result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pInterData);
                    if(result){
                        sysHyPhy20gMemFree(pCalData);
                        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                        return result;
                    }
                }
                /* update END_ADDRESS used by scheduler */                
                DBG_PRINT("%s, %s, %d, RESIZE ROWS UPDATE END_ADDRESS\n",__FILE__, __FUNCTION__, __LINE__);
                result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, 0, startAddr,
                                                       (startAddr + numRows - 1));
                if(result){
                    sysHyPhy20gMemFree(pCalData);
                    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                    return result;
                }
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x8){
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPA_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x20){
            pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_GFPB_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
        if(intfGrpMsk & 0x40){
             pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_PIF_RD_SCHD;
            /* get scheduler content data */
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 0, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
            /* update scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
    }


    if (updateQDR == 1) {
        /* update QDR scheduler */
        pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+HYPHY20G_CPB_QDR_SCHD;

    DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
        /* generate QDR scheduler content */
        result = cpbHyPhy20gCPBGenerateQdrRateAwareSchd(fileHndl, pSchd, pCalData);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
        /* compare scheduler data with current online scheduler data */
        result = cpbHyPhy20gCpbQdrSchdCmp(fileHndl, pSchd, pCalData, &different);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }

        if(different){
    DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);
            /* update QDR scheduler with content data */
            result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
            if(result){
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }
        }
    }

    sysHyPhy20gMemFree(pCalData);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBSchdRateAwareUpdate */

/*******************************************************************************
**
**  cpbHyPhy20gCPBGenerateQdrRateAwareSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates interface group context, generates the scheduler
**                  calendar memory based on a best effort even distribution
**                  adhering to the requirement that no channel number be
**                  processed closer than 4 calendar entries apart.
**
**                  As part of process evaluates PIF write channel requirements
**                  (as the PIF write interface does not have a scheduler).
**
**
**  INPUTS:         fileHndl  - device handle
**                 *pSchd     - pointer to QDR scheduler context
**                 *pCalData  - pointer to formatted QDR scheduler calendar data
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   This function updates the totalChnlRate parameter of the
**                  pSchd structure.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBGenerateQdrRateAwareSchd(struct file  *fileHndl,
                                            sHYPHY20G_SCHD_CTXT *pSchd,
                                            UINT4 *pCalData)
{
    sHYPHY20G_SCHD_CTXT *pIfGrpSchd;
    sHYPHY20G_INTF_GRP_CTXT *pIfGrpCtxt;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnl;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnl;
    UINT4 i, size, revId;
    UINT4 lineWrChnlEnabled = 0, gfpaWrChnlEnabled = 0, gfpbWrChnlEnabled = 0;
    UINT4 pifWrChnlEnabled = 0, lineRdChnlEnabled = 0, gfpaRdChnlEnabled = 0;
    UINT4 gfpbRdChnlEnabled = 0, pifRdChnlEnabled = 0;
    INT4 result = 0;
    INT4 j = 0;
    UINT4 wrCalEntries =
                       (pSchd->numRows) * HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW/2;
    UINT4 rdCalEntries =
                       (pSchd->numRows) * HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW/2;
    UINT4 *pLinWrArray, *pLinRdArray, *pFormattedCalData;
    UINT4 numWrLineEntries, numWrGfpAEntries, numWrGfpBEntries;
    UINT4 numWrPifEntries, numRdLineEntries, numRdGfpAEntries;
    UINT4 numRdGfpBEntries, numRdPifEntries;

    /* verify pointer to global cpb data */
    sHYPHY20G_CTXT *pDevCtxt;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* alocate memory for local use by function */
    size = sizeof(UINT4)*(wrCalEntries) +
           sizeof(UINT4)*(rdCalEntries);

    pLinWrArray = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pLinWrArray == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pLinRdArray = (UINT4 *)(pLinWrArray + wrCalEntries);
    pFormattedCalData = pCalData;

    /* reference to the first of the interface group schedulers */
    pIfGrpSchd = (sHYPHY20G_SCHD_CTXT *)(pDevCtxt->pCpbCtxt->pSchdCtxt);

    /* reference to the first of the context for interface groups */
    pIfGrpCtxt = (sHYPHY20G_INTF_GRP_CTXT *)(pDevCtxt->pCpbCtxt->pIfGrpCtxt);

    /* reference to the PIF write channel ports */
    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pWrChnl);
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl);

    if(pDevCtxt->gData.hyphy20gQdrStressMode) {
        /* call Stress Optimized QDR Config */
        /* retrieve linear write calendar entries for enabled QDR interfaces */
        result = cpbHyPhy20gCpbGetQdrCalDataStress(fileHndl, 0, pLinWrArray,
                                                   &numWrLineEntries, &numWrGfpAEntries,
                                                   &numWrGfpBEntries, &numWrPifEntries);
        if(result){
            sysHyPhy20gMemFree(pLinWrArray);
            return result;
        }

        /* retrieve linear read calendar entries for enabled QDR interfaces */
        result = cpbHyPhy20gCpbGetQdrCalDataStress(fileHndl, 1, pLinRdArray,
                                                   &numRdLineEntries, &numRdGfpAEntries,
                                                   &numRdGfpBEntries, &numRdPifEntries);
        if(result){
            sysHyPhy20gMemFree(pLinWrArray);
            return result;
        }
    } else {
       /* retrieve revision ID */
        result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
        if(result) return result;
        /* call Standard QDR Config */
        /* retrieve linear write calendar entries for enabled QDR interfaces */
        result = cpbHyPhy20gCpbGetQdrCalData(fileHndl, 0, pLinWrArray,
                                             &numWrLineEntries, &numWrGfpAEntries,
                                             &numWrGfpBEntries, &numWrPifEntries);
        if(result){
            sysHyPhy20gMemFree(pLinWrArray);
            return result;
        }

        /* retrieve linear read calendar entries for enabled QDR interfaces */
        result = cpbHyPhy20gCpbGetQdrCalData(fileHndl, 1, pLinRdArray,
                                             &numRdLineEntries, &numRdGfpAEntries,
                                             &numRdGfpBEntries, &numRdPifEntries);
        if(result){
            sysHyPhy20gMemFree(pLinWrArray);
            return result;
        }
    }

    /* identify which interfaces present in QDR scheduler have operational
       channels */
    for(i=HYPHY20G_CPB_LINE_PORT_OFFSET; i < HYPHY20G_CPB_NUM_WR_PORT; i++){
        if(i < HYPHY20G_CPB_GFPA_PORT_OFFSET && numWrLineEntries != 0){
           if((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
              (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                lineWrChnlEnabled = 1;
           }
        } else if(i < HYPHY20G_CPB_GFPB_PORT_OFFSET && numWrGfpAEntries != 0){
           if((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
              (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                gfpaWrChnlEnabled = 1;
            }
        } else if(i < HYPHY20G_CPB_PIF_PORT_OFFSET && numWrGfpBEntries != 0){
            if((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
              (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                gfpbWrChnlEnabled = 1;
            }
        } else {
            if(numWrPifEntries != 0){
                if((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
                  (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                    pifWrChnlEnabled = 1;
                }
            }
        }
        pWrChnl++;
    }

    for(i=HYPHY20G_CPB_LINE_PORT_OFFSET; i < HYPHY20G_CPB_NUM_RD_PORT; i++){
        if(i < HYPHY20G_CPB_GFPA_PORT_OFFSET && numRdLineEntries != 0){
           if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
              (pRdChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                lineRdChnlEnabled = 1;
           }
        } else if(i < HYPHY20G_CPB_GFPB_PORT_OFFSET && numRdGfpAEntries != 0){
           if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
              (pRdChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                gfpaRdChnlEnabled = 1;
            }
        } else if(i < HYPHY20G_CPB_PIF_PORT_OFFSET && numRdGfpBEntries != 0){
            if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
              (pRdChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                gfpbRdChnlEnabled = 1;
            }
        } else {
            if(numRdPifEntries != 0){
                if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
                  (pRdChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)){
                    pifRdChnlEnabled = 1;
                }
            }
        }
        pRdChnl++;
    }

    DBG_PRINT("lineWrChnlEnabled = %d gfpaWrChnlEnabled = %d gfpbWrChnlEnabled = %d pifWrChnlEnabled = %d\n",(unsigned int)lineWrChnlEnabled, (unsigned int)gfpaWrChnlEnabled, (unsigned int)gfpbWrChnlEnabled, (unsigned int)pifWrChnlEnabled);
    DBG_PRINT("lineRdChnlEnabled = %d gfpaRdChnlEnabled = %d gfpbRdChnlEnabled = %d pifRdChnlEnabled = %d\n",(unsigned int)lineRdChnlEnabled, (unsigned int)gfpaRdChnlEnabled, (unsigned int)gfpbRdChnlEnabled, (unsigned int)pifRdChnlEnabled);

    /* if no operational
       channels in interface, overwrite cases of this channel in linear
       calendar with NULL entry (0xFF) - this operation is conditional
       on the presence of the entry type in the scheduler */
    /* line interface */
    for(i=0; i < wrCalEntries; i++){
        if(numWrLineEntries != 0 && lineWrChnlEnabled == 0){
            if(*(pLinWrArray+i)==LINE_WR_INTF_GRP){
                *(pLinWrArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
        if(numWrGfpAEntries != 0 && gfpaWrChnlEnabled == 0){
            if(*(pLinWrArray+i)==GFP_A_WR_INTF_GRP){
                *(pLinWrArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
        if(numWrGfpBEntries != 0 && gfpbWrChnlEnabled == 0){
            if(*(pLinWrArray+i)==GFP_B_WR_INTF_GRP){
                *(pLinWrArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
        if(numWrPifEntries != 0 && pifWrChnlEnabled == 0){
            if(*(pLinWrArray+i)==PIF_WR_INTF_GRP){
                *(pLinWrArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
    }
    for(i=0; i < rdCalEntries; i++){
        if(numRdLineEntries != 0 && lineRdChnlEnabled == 0){
            if(*(pLinRdArray+i)==LINE_RD_INTF_GRP){
                *(pLinRdArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
        if(numRdGfpAEntries != 0 && gfpaRdChnlEnabled == 0){
            if(*(pLinRdArray+i)==GRP_A_RD_INTF_GRP){
                *(pLinRdArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
        if(numRdGfpBEntries != 0 && gfpbRdChnlEnabled == 0){
            if(*(pLinRdArray+i)==GRP_B_RD_INTF_GRP){
                *(pLinRdArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
        if(numRdPifEntries != 0 && pifRdChnlEnabled == 0){
            if(*(pLinRdArray+i)==PIF_RD_INTF_GRP){
                *(pLinRdArray+i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
            }
        }
    }

    /* debug print the write and read linear arrays */
    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
    for(i=0; i < 30; i++){
        DBG_PRINT("pLinWrArray+%d = %d\n",(unsigned int)i, (unsigned int)*(pLinWrArray+i));

    }
    for(i=0; i < 30; i++){
        DBG_PRINT("pLinRdArray+%d = %d\n",(unsigned int)i, (unsigned int)*(pLinRdArray+i));

    }

    /* generate scheduler calendar as used by device from two linear arrays */

    /* even address contain write interface group data */
    j = 0;
    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)) =
             *(pLinWrArray+j);
        j++;
    }

    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW) + 2) =
             *(pLinWrArray+j);
        j++;
    }

    /* odd address contain read interface group data */
    j = 0;
    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW) + 1) =
             *(pLinRdArray+j);
        j++;
    }

    for(i=0; i<pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW) + 3) =
             *(pLinRdArray+j);
        j++;
    }

    sysHyPhy20gMemFree(pLinWrArray);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBGenerateQdrRateAwareSchd */




/*******************************************************************************
**
**  cpbHyPhy20gCpbGetQdrCalData
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function returns a 30 element linear list of QDR
**                  data for one direction of travel. The contents are
**                  generated as a function of enabled interfaces.
**
**  INPUTS:         fileHndl          - device handle
**                  dir               - Entry values generated relative to:
**                                      0 - Write interfaces
**                                      1 - Read interfaces
**                  *pLinearCal[30]   - 30 element array of QDR entry data
**                  *pLineEntries     - Number of Line Entries
**                  *pGfpAEntries     - Number of GFPA Entries
**                  *pGfpBEntries     - Number of GFPB Entries
**                  *pPifEntries      - Number of PIF Entries
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetQdrCalData(struct file  *fileHndl, UINT4 dir,
                                 UINT4 *pLinearCal, UINT4 *pLineEntries,
                                 UINT4 *pGfpAEntries, UINT4 *pGfpBEntries,
                                 UINT4 *pPifEntries)
{
    UINT4 valid, enable, dummy, enMsk = 0, i;
    UINT4 line, gfpa, gfpb, pifI;
    UINT4 numLine = 0, numGfpA = 0, numGfpB = 0, numPifI = 0;
    UINT4 data[30];
    UINT4 intRamEn;
    INT4 result;

    /* argument checking */
    if(dir > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Initialize scheduler entry values as a function of RD or WR interface */
    if (dir) {
        line = LINE_RD_INTF_GRP;
        gfpa = GRP_A_RD_INTF_GRP;
        gfpb = GRP_B_RD_INTF_GRP;
        pifI = PIF_RD_INTF_GRP;
    } else {
        line = LINE_WR_INTF_GRP;
        gfpa = GFP_A_WR_INTF_GRP;
        gfpb = GFP_B_WR_INTF_GRP;
        pifI = PIF_WR_INTF_GRP;
    }

    /* retrieve enabled interfaces */
    /*
         0 - CPB CPB Line Interface Group
         1 - CPB GFP A Interface Group
         2 - CPB GFP B Interface Group
         3 - CPB PIF Interface Group
    */
    for (i=0; i < 4; i++) {
        result = cpbHyPhy20gCpbGetIntfCtxt(fileHndl, i, &valid, &dummy, &enable,
                                           &dummy, &dummy, &dummy, &dummy,
                                           &dummy, &dummy);
        if(result) return result;

        if(valid == HYPHY20G_VALID_SIGN && enable == HYPHY20G_CPB_INTF_ENABLED) {
            enMsk |= (1 << i);
        }
    }

    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG,
             HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_INTERNAL_RAM_EN_OFF, &intRamEn);
    if(result) return result;

    /* retrieve QDR entries for specified set of interfaces */
    switch(enMsk){
        case 0: /* No interfaces */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = 0xFF; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = 0xFF; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = 0xFF; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = 0xFF; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = 0xFF;
            break;
        case 0x1: /* Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = line; data[25] = 0xFF; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = 0xFF; data[20] = line;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = line;
            data[15] = 0xFF; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = 0xFF; data[10] = line; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = line; data[5]  = 0xFF; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = 0xFF; data[0]  = line;
            break;
        case 0x2: /* GFP A */ 
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = gfpa; data[26] = 0xFF; data[25] = 0xFF; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = 0xFF; data[20] = gfpa;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = gfpa; data[16] = 0xFF;
            data[15] = 0xFF; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = gfpa; data[6]  = 0xFF; data[5]  = 0xFF; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0x3: /* GFP A / Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = gfpa; data[26] = line; data[25] = 0xFF; data[24] = line;
            data[23] = gfpa; data[22] = line; data[21] = 0xFF; data[20] = line;
            data[19] = 0xFF; data[18] = line; data[17] = gfpa; data[16] = line;
            data[15] = 0xFF; data[14] = line; data[13] = gfpa; data[12] = line;
            data[11] = 0xFF; data[10] = line; data[9]  = 0xFF; data[8]  = line;
            data[7]  = gfpa; data[6]  = line; data[5]  = 0xFF; data[4]  = line;
            data[3]  = gfpa; data[2]  = line; data[1]  = 0xFF; data[0]  = line;
            break;
        case 0x4: /* GFP B */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = gfpb; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = gfpb; data[21] = 0xFF; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = gfpb; data[14] = 0xFF; data[13] = 0xFF; data[12] = gfpb;
            data[11] = 0xFF; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = gfpb; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = gfpb; data[1]  = 0xFF; data[0]  = 0xFF;
            break;
        case 0x5: /* GFP B / Line */
                                              data[29] = gfpb; data[28] = line;
            data[27] = 0xFF; data[26] = line; data[25] = gfpb; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = 0xFF; data[20] = line;
            data[19] = gfpb; data[18] = line; data[17] = 0xFF; data[16] = line;
            data[15] = gfpb; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = 0xFF; data[10] = line; data[9]  = gfpb; data[8]  = line;
            data[7]  = 0xFF; data[6]  = line; data[5]  = gfpb; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = 0xFF; data[0]  = line;
            break;
        case 0x6: /* GFP B / GFP A */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = gfpa; data[26] = 0xFF; data[25] = gfpb; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = gfpb; data[21] = 0xFF; data[20] = gfpa;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = gfpa; data[16] = 0xFF;
            data[15] = gfpb; data[14] = 0xFF; data[13] = 0xFF; data[12] = gfpb;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = gfpa; data[6]  = 0xFF; data[5]  = gfpb; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = gfpb; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0x7: /* GFP B / GFP A / Line */
                                              data[29] = gfpb; data[28] = line;
            data[27] = gfpa; data[26] = line; data[25] = gfpb; data[24] = line;
            data[23] = gfpa; data[22] = line; data[21] = 0xFF; data[20] = line;
            data[19] = gfpb; data[18] = line; data[17] = gfpa; data[16] = line;
            data[15] = gfpb; data[14] = line; data[13] = gfpa; data[12] = line;
            data[11] = 0xFF; data[10] = line; data[9]  = gfpb; data[8]  = line;
            data[7]  = gfpa; data[6]  = line; data[5]  = gfpb; data[4]  = line;
            data[3]  = gfpa; data[2]  = line; data[1]  = 0xFF; data[0]  = line;
            break;
        case 0x8: /* PIF */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = 0xFF; data[25] = pifI; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = pifI; data[20] = 0xFF;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = 0xFF;
            data[15] = pifI; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = pifI; data[10] = 0xFF; data[9]  = pifI; data[8]  = 0xFF;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = pifI; data[4]  = 0xFF;
            data[3]  = pifI; data[2]  = 0xFF; data[1]  = pifI; data[0]  = 0xFF;
            break;
        case 0x9: /* PIF / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = line; data[25] = pifI; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = pifI; data[20] = line;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = line;
            data[15] = pifI; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = pifI; data[10] = line; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = line; data[5]  = pifI; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = pifI; data[0]  = line;
            break;
        case 0xA: /* PIF / GFP A */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = 0xFF; data[25] = pifI; data[24] = gfpa;
            data[23] = pifI; data[22] = 0xFF; data[21] = pifI; data[20] = gfpa;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = 0xFF;
            data[15] = pifI; data[14] = gfpa; data[13] = pifI; data[12] = 0xFF;
            data[11] = pifI; data[10] = gfpa; data[9]  = pifI; data[8]  = 0xFF;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = pifI; data[4]  = gfpa;
            data[3]  = pifI; data[2]  = 0xFF; data[1]  = pifI; data[0]  = gfpa;
            break;
        case 0xB: /* PIF / GFP A / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = gfpa; data[26] = line; data[25] = pifI; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = pifI; data[20] = gfpa;
            data[19] = pifI; data[18] = line; data[17] = gfpa; data[16] = line;
            data[15] = pifI; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = pifI; data[10] = gfpa; data[9]  = pifI; data[8]  = line;
            data[7]  = gfpa; data[6]  = line; data[5]  = pifI; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = pifI; data[0]  = gfpa;
            break;
        case 0xC: /* PIF / GFP B */
                                              data[29] = pifI; data[28] = gfpb;
            data[27] = pifI; data[26] = 0xFF; data[25] = pifI; data[24] = 0xFF;
            data[23] = pifI; data[22] = gfpb; data[21] = pifI; data[20] = 0xFF;
            data[19] = pifI; data[18] = gfpb; data[17] = pifI; data[16] = 0xFF;
            data[15] = pifI; data[14] = 0xFF; data[13] = pifI; data[12] = gfpb;
            data[11] = pifI; data[10] = 0xFF; data[9]  = pifI; data[8]  = gfpb;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = pifI; data[4]  = 0xFF;
            data[3]  = pifI; data[2]  = gfpb; data[1]  = pifI; data[0]  = 0xFF;
            break;
        case 0xD: /* PIF / GFP B / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = line; data[25] = gfpb; data[24] = line;
            data[23] = pifI; data[22] = gfpb; data[21] = pifI; data[20] = line;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = line;
            data[15] = gfpb; data[14] = line; data[13] = pifI; data[12] = gfpb;
            data[11] = pifI; data[10] = line; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = line; data[5]  = gfpb; data[4]  = line;
            data[3]  = pifI; data[2]  = gfpb; data[1]  = pifI; data[0]  = line;
            break;
        case 0xE: /* PIF / GFP B / GFP A */
                                              data[29] = pifI; data[28] = gfpb;
            data[27] = pifI; data[26] = 0xFF; data[25] = pifI; data[24] = gfpa;
            data[23] = pifI; data[22] = gfpb; data[21] = pifI; data[20] = gfpa;
            data[19] = pifI; data[18] = gfpb; data[17] = pifI; data[16] = 0xFF;
            data[15] = pifI; data[14] = gfpa; data[13] = pifI; data[12] = gfpb;
            data[11] = pifI; data[10] = gfpa; data[9]  = pifI; data[8]  = gfpb;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = pifI; data[4]  = gfpa;
            data[3]  = pifI; data[2]  = gfpb; data[1]  = pifI; data[0]  = gfpa;
            break;
        case 0xF: /* PIF / GFP B / GFP A / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = gfpa; data[26] = line; data[25] = gfpb; data[24] = line;
            data[23] = pifI; data[22] = gfpb; data[21] = pifI; data[20] = gfpa;
            data[19] = pifI; data[18] = line; data[17] = gfpa; data[16] = line;
            data[15] = gfpb; data[14] = line; data[13] = pifI; data[12] = gfpb;
            data[11] = pifI; data[10] = gfpa; data[9]  = pifI; data[8]  = line;
            data[7]  = gfpa; data[6]  = line; data[5]  = gfpb; data[4]  = line;
            data[3]  = pifI; data[2]  = gfpb; data[1]  = pifI; data[0]  = gfpa;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;

    }

    /* generate number of entries per interface */
    for (i = 0; i < 30; i++) {
        switch(data[i]){
            case LINE_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numLine++;
                break;
            case LINE_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numLine++;
                break;
            case GFP_A_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpA++;
                break;
            case GRP_A_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpA++;
                break;
            case GFP_B_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpB++;
                break;
            case GRP_B_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpB++;
                break;
            case PIF_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numPifI++;
                break;
            case PIF_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numPifI++;
                break;
            case HYPHY20G_CPB_SCHD_NULL_CHNL:
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    for(i=0; i < 30; i++){
        *(pLinearCal+i) = data[i];
        DBG_PRINT("data[%d] = %d, *(pLinearCal+%d) = %d\n",(unsigned int)i, (unsigned int)data[i], (unsigned int)i, (unsigned int)(*(pLinearCal+i)));
    }

    pLinearCal = data;
    *pLineEntries = numLine;
    *pGfpAEntries = numGfpA;
    *pGfpBEntries = numGfpB;
    *pPifEntries = numPifI;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbGetQdrCalData */

/*******************************************************************************
**
**  cpbHyPhy20gCpbGetQdrCalDataRevA
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function returns a 30 element linear list of QDR
**                  data for one direction of travel. The contents are
**                  generated as a function of enabled interfaces.
**
**                  This function ensures entry spacing is compliant with Rev A
**                  device requirements.
**
**  INPUTS:         fileHndl          - device handle
**                  dir               - Entry values generated relative to:
**                                      0 - Write interfaces
**                                      1 - Read interfaces
**                  *pLinearCal[30]   - 30 element array of QDR entry data
**                  *pLineEntries     - Number of Line Entries
**                  *pGfpAEntries     - Number of GFPA Entries
**                  *pGfpBEntries     - Number of GFPB Entries
**                  *pPifEntries      - Number of PIF Entries
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetQdrCalDataRevA(struct file  *fileHndl, UINT4 dir,
                                 UINT4 *pLinearCal, UINT4 *pLineEntries,
                                 UINT4 *pGfpAEntries, UINT4 *pGfpBEntries,
                                 UINT4 *pPifEntries)
{
    UINT4 valid, enable, dummy, enMsk = 0, i;
    UINT4 line, gfpa, gfpb, pifI;
    UINT4 numLine = 0, numGfpA = 0, numGfpB = 0, numPifI = 0;
    UINT4 data[30];
    UINT4 intRamEn, revId;
    INT4 result;

    /* argument checking */
    if(dir > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Initialize scheduler entry values as a function of RD or WR interface */
    if (dir) {
        line = LINE_RD_INTF_GRP;
        gfpa = GRP_A_RD_INTF_GRP;
        gfpb = GRP_B_RD_INTF_GRP;
        pifI = PIF_RD_INTF_GRP;
    } else {
        line = LINE_WR_INTF_GRP;
        gfpa = GFP_A_WR_INTF_GRP;
        gfpb = GFP_B_WR_INTF_GRP;
        pifI = PIF_WR_INTF_GRP;
    }

    /* retrieve enabled interfaces */
    /*
         0 - CPB CPB Line Interface Group
         1 - CPB GFP A Interface Group
         2 - CPB GFP B Interface Group
         3 - CPB PIF Interface Group
    */
    for (i=0; i < 4; i++) {
        result = cpbHyPhy20gCpbGetIntfCtxt(fileHndl, i, &valid, &dummy, &enable,
                                           &dummy, &dummy, &dummy, &dummy,
                                           &dummy, &dummy);
        if(result) return result;

        if(valid == HYPHY20G_VALID_SIGN && enable == HYPHY20G_CPB_INTF_ENABLED) {
            enMsk |= (1 << i);
        }
    }

    /* extract device revision and external QDR usage to identify if Rev A
       external QDR workaround needs to be applied to scheduler
       contents */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
    if(result) return result;

    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG,
             HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_INTERNAL_RAM_EN_OFF, &intRamEn);
    if(result) return result;

    /* retrieve QDR entries for specified set of interfaces */
    switch(enMsk){
        case 0: /* No interfaces */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = 0xFF; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = 0xFF; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = 0xFF; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = 0xFF; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = 0xFF;
            break;
        case 0x1: /* Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = line; data[25] = 0xFF; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = 0xFF; data[20] = line;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = line;
            data[15] = 0xFF; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = 0xFF; data[10] = line; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = line; data[5]  = 0xFF; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = 0xFF; data[0]  = line;
            break;
        case 0x2: /* GFP A */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = gfpa; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = 0xFF; data[20] = gfpa;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = gfpa; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = gfpa; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0x3: /* GFP A / Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = line; data[25] = gfpa; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = 0xFF; data[20] = gfpa;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = line;
            data[15] = gfpa; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = line; data[5]  = gfpa; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0x4: /* GFP B */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = gfpb; data[25] = 0xFF; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = gfpb; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = gfpb;
            data[15] = 0xFF; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = gfpb; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = 0xFF; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = 0xFF;
            break;
        case 0x5: /* GFP B / Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = gfpb; data[25] = 0xFF; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = gfpb; data[20] = line;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = gfpb;
            data[15] = 0xFF; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = gfpb; data[10] = line; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = 0xFF; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = gfpb; data[0]  = line;
            break;
        case 0x6: /* GFP B / GFP A */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = gfpb; data[25] = gfpa; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = gfpb; data[20] = gfpa;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = gfpb;
            data[15] = gfpa; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = gfpb; data[10] = gfpa; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = gfpa; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = gfpa;
            break;
        case 0x7: /* GFP B / GFP A / Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = gfpb; data[25] = gfpa; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = gfpb; data[20] = gfpa;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = gfpb;
            data[15] = gfpa; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = gfpb; data[10] = gfpa; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = gfpa; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = gfpb; data[0]  = gfpa;
            break;
        case 0x8: /* PIF */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = 0xFF; data[25] = pifI; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = pifI; data[20] = 0xFF;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = 0xFF;
            data[15] = pifI; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = pifI; data[10] = 0xFF; data[9]  = pifI; data[8]  = 0xFF;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = pifI; data[4]  = 0xFF;
            data[3]  = pifI; data[2]  = 0xFF; data[1]  = pifI; data[0]  = 0xFF;
            break;
        case 0x9: /* PIF / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = line; data[25] = pifI; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = pifI; data[20] = line;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = line;
            data[15] = pifI; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = pifI; data[10] = line; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = line; data[5]  = pifI; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = pifI; data[0]  = line;
            break;
        case 0xA: /* PIF / GFP A */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = 0xFF; data[25] = gfpa; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = pifI; data[20] = gfpa;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = 0xFF;
            data[15] = gfpa; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = pifI; data[10] = gfpa; data[9]  = pifI; data[8]  = 0xFF;
            data[07] = pifI; data[6]  = 0xFF; data[5]  = gfpa; data[4]  = 0xFF;
            data[03] = pifI; data[2]  = 0xFF; data[1]  = pifI; data[0]  = gfpa;
            break;
        case 0xB: /* PIF / GFP A / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = line; data[25] = gfpa; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = pifI; data[20] = gfpa;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = line;
            data[15] = gfpa; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = pifI; data[10] = gfpa; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = line; data[5]  = gfpa; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = pifI; data[0]  = gfpa;
            break;
        case 0xC: /* PIF / GFP B */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = gfpb; data[25] = pifI; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = gfpb; data[20] = 0xFF;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = gfpb;
            data[15] = pifI; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = gfpb; data[10] = 0xFF; data[9]  = pifI; data[8]  = 0xFF;
            data[07] = pifI; data[6]  = gfpb; data[5]  = pifI; data[4]  = 0xFF;
            data[03] = pifI; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = 0xFF;
            break;
        case 0xD: /* PIF / GFP B / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = gfpb; data[25] = pifI; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = gfpb; data[20] = line;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = gfpb;
            data[15] = pifI; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = gfpb; data[10] = line; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = gfpb; data[5]  = pifI; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = gfpb; data[0]  = line;
            break;
        case 0xE: /* PIF / GFP B / GFP A */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = gfpb; data[25] = gfpa; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = gfpb; data[20] = gfpa;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = gfpb;
            data[15] = gfpa; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = gfpb; data[10] = gfpa; data[9]  = pifI; data[8]  = 0xFF;
            data[7]  = pifI; data[6]  = gfpb; data[5]  = gfpa; data[4]  = 0xFF;
            data[3]  = pifI; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = gfpa;
            break;
        case 0xF: /* PIF / GFP B / GFP A / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = gfpb; data[25] = gfpa; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = gfpb; data[20] = gfpa;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = gfpb;
            data[15] = gfpa; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = gfpb; data[10] = gfpa; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = gfpb; data[5]  = gfpa; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = gfpb; data[0]  = gfpa;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;

    }

    /* generate number of entries per interface */
    for (i = 0; i < 30; i++) {
        switch(data[i]){
            case LINE_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numLine++;
                break;
            case LINE_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numLine++;
                break;
            case GFP_A_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpA++;
                break;
            case GRP_A_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpA++;
                break;
            case GFP_B_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpB++;
                break;
            case GRP_B_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpB++;
                break;
            case PIF_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numPifI++;
                break;
            case PIF_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numPifI++;
                break;
            case HYPHY20G_CPB_SCHD_NULL_CHNL:
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    for(i=0; i < 30; i++){
        *(pLinearCal+i) = data[i];
        DBG_PRINT("data[%d] = %d, *(pLinearCal+%d) = %d\n",(unsigned int)i, (unsigned int)data[i], (unsigned int)i, (unsigned int)(*(pLinearCal+i)));
    }

    pLinearCal = data;
    *pLineEntries = numLine;
    *pGfpAEntries = numGfpA;
    *pGfpBEntries = numGfpB;
    *pPifEntries = numPifI;



    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbGetQdrCalDataRevA */

/*******************************************************************************
**
**  cpbHyPhy20gCpbGetQdrCalDataStress
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Stress Test variation of QDR calendar generation to
**                  provice QDR calendar minimum entries available to each
**                  interface by Rev B are used.  Min entries GFP A/B 6, Line
**                  and PIF 9.  With these minimum number of entries the BW
**                  requirement must be met.
**
**                  This function returns a 30 element linear list of QDR
**                  data for one direction of travel. The contents are
**                  generated as a function of enabled interfaces.
**
**  INPUTS:         fileHndl          - device handle
**                  dir               - Entry values generated relative to:
**                                      0 - Write interfaces
**                                      1 - Read interfaces
**                  *pLinearCal[30]   - 30 element array of QDR entry data
**                  *pLineEntries     - Number of Line Entries
**                  *pGfpAEntries     - Number of GFPA Entries
**                  *pGfpBEntries     - Number of GFPB Entries
**                  *pPifEntries      - Number of PIF Entries
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetQdrCalDataStress(struct file  *fileHndl, UINT4 dir,
                                 UINT4 *pLinearCal, UINT4 *pLineEntries,
                                 UINT4 *pGfpAEntries, UINT4 *pGfpBEntries,
                                 UINT4 *pPifEntries)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 valid, enable, dummy, enMsk = 0, i;
    UINT4 line, gfpa, gfpb, pifI;
    UINT4 numLine = 0, numGfpA = 0, numGfpB = 0, numPifI = 0;
    UINT4 data[30];
    UINT4 intRamEn, revId;
    INT4 result;

    /* argument checking */
    if(dir > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

	/* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
    DBG_PRINT("%s, %s, %d, QDR Distribution  \
                hyphy20gQdrStressDist = %u, hyphy20gQdrStressMode = %u\n", 
                __FILE__, __FUNCTION__, __LINE__, 
                (unsigned int)pDevCtxt->gData.hyphy20gQdrStressDist, 
                (unsigned int)pDevCtxt->gData.hyphy20gQdrStressMode);

    /* Initialize scheduler entry values as a function of RD or WR interface */
    if (dir) {
        line = LINE_RD_INTF_GRP;
        gfpa = GRP_A_RD_INTF_GRP;
        gfpb = GRP_B_RD_INTF_GRP;
        pifI = PIF_RD_INTF_GRP;
    } else {
        line = LINE_WR_INTF_GRP;
        gfpa = GFP_A_WR_INTF_GRP;
        gfpb = GFP_B_WR_INTF_GRP;
        pifI = PIF_WR_INTF_GRP;
    }

    /* retrieve enabled interfaces */
    /*
         0 - CPB CPB Line Interface Group
         1 - CPB GFP A Interface Group
         2 - CPB GFP B Interface Group
         3 - CPB PIF Interface Group
    */
    for (i=0; i < 4; i++) {
        result = cpbHyPhy20gCpbGetIntfCtxt(fileHndl, i, &valid, &dummy, &enable,
                                           &dummy, &dummy, &dummy, &dummy,
                                           &dummy, &dummy);
        if(result) return result;

        if(valid == HYPHY20G_VALID_SIGN && enable == HYPHY20G_CPB_INTF_ENABLED) {
            enMsk |= (1 << i);
        }
    }

    /* extract device revision and external QDR usage to identify if Rev A
       external QDR workaround needs to be applied to scheduler
       contents */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
    if(result) return result;

    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG,
             HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_INTERNAL_RAM_EN_OFF, &intRamEn);
    if(result) return result;

    /* retrieve QDR entries for specified set of interfaces */
    switch(enMsk){
        case 0: /* No interfaces */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = 0xFF; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = 0xFF; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = 0xFF; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = 0xFF; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = 0xFF;
            break;
        case 0x1: /* Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = 0xFF; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = 0xFF; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = 0xFF; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = 0xFF; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = 0xFF; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = 0xFF; data[0]  = 0xFF;
            break;
        case 0x2: /* GFP A */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = gfpa; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = 0xFF; data[20] = gfpa;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = gfpa; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = gfpa; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0x3: /* GFP A / Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = 0xFF; data[25] = gfpa; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = 0xFF; data[20] = gfpa;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = 0xFF;
            data[15] = gfpa; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = 0xFF; data[5]  = gfpa; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0x4: /* GFP B */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = gfpb; data[25] = 0xFF; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = gfpb; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = gfpb;
            data[15] = 0xFF; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = gfpb; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = 0xFF; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = 0xFF;
            break;
        case 0x5: /* GFP B / Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = gfpb; data[25] = 0xFF; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = gfpb; data[20] = 0xFF;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = gfpb;
            data[15] = 0xFF; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = gfpb; data[10] = 0xFF; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = 0xFF; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = gfpb; data[0]  = 0xFF;
            break;
        case 0x6: /* GFP B / GFP A */
                                              data[29] = 0xFF; data[28] = 0xFF;
            data[27] = 0xFF; data[26] = gfpb; data[25] = gfpa; data[24] = 0xFF;
            data[23] = 0xFF; data[22] = 0xFF; data[21] = gfpb; data[20] = gfpa;
            data[19] = 0xFF; data[18] = 0xFF; data[17] = 0xFF; data[16] = gfpb;
            data[15] = gfpa; data[14] = 0xFF; data[13] = 0xFF; data[12] = 0xFF;
            data[11] = gfpb; data[10] = gfpa; data[9]  = 0xFF; data[8]  = 0xFF;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = gfpa; data[4]  = 0xFF;
            data[3]  = 0xFF; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = gfpa;
            break;
        case 0x7: /* GFP B / GFP A / Line */
                                              data[29] = 0xFF; data[28] = line;
            data[27] = 0xFF; data[26] = gfpb; data[25] = gfpa; data[24] = line;
            data[23] = 0xFF; data[22] = line; data[21] = gfpb; data[20] = gfpa;
            data[19] = 0xFF; data[18] = line; data[17] = 0xFF; data[16] = gfpb;
            data[15] = gfpa; data[14] = line; data[13] = 0xFF; data[12] = line;
            data[11] = gfpb; data[10] = gfpa; data[9]  = 0xFF; data[8]  = line;
            data[7]  = 0xFF; data[6]  = gfpb; data[5]  = gfpa; data[4]  = line;
            data[3]  = 0xFF; data[2]  = line; data[1]  = gfpb; data[0]  = gfpa;
            break;
        case 0x8: /* PIF */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = 0xFF; data[25] = 0xFF; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = 0xFF; data[20] = 0xFF;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = 0xFF;
            data[15] = 0xFF; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = 0xFF; data[9]  = pifI; data[8]  = 0xFF;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = 0xFF; data[4]  = 0xFF;
            data[3]  = pifI; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = 0xFF;
            break;
        case 0x9: /* PIF / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = 0xFF; data[25] = 0xFF; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = 0xFF; data[20] = 0xFF;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = 0xFF;
            data[15] = 0xFF; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = 0xFF; data[10] = 0xFF; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = 0xFF; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = 0xFF; data[0]  = 0xFF;
            break;
        case 0xA: /* PIF / GFP A */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = 0xFF; data[25] = gfpa; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = 0xFF; data[20] = gfpa;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = 0xFF;
            data[15] = gfpa; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = pifI; data[8]  = 0xFF;
            data[07] = pifI; data[6]  = 0xFF; data[5]  = gfpa; data[4]  = 0xFF;
            data[03] = pifI; data[2]  = 0xFF; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0xB: /* PIF / GFP A / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = 0xFF; data[25] = gfpa; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = 0xFF; data[20] = gfpa;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = 0xFF;
            data[15] = gfpa; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = 0xFF; data[10] = gfpa; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = 0xFF; data[5]  = gfpa; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = 0xFF; data[0]  = gfpa;
            break;
        case 0xC: /* PIF / GFP B */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = gfpb; data[25] = pifI; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = gfpb; data[20] = 0xFF;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = gfpb;
            data[15] = pifI; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = gfpb; data[10] = 0xFF; data[9]  = pifI; data[8]  = 0xFF;
            data[07] = pifI; data[6]  = gfpb; data[5]  = pifI; data[4]  = 0xFF;
            data[03] = pifI; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = 0xFF;
            break;
        case 0xD: /* PIF / GFP B / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = gfpb; data[25] = 0xFF; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = gfpb; data[20] = 0xFF;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = gfpb;
            data[15] = 0xFF; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = gfpb; data[10] = 0xFF; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = gfpb; data[5]  = 0xFF; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = gfpb; data[0]  = 0xFF;
            break;
        case 0xE: /* PIF / GFP B / GFP A */
                                              data[29] = pifI; data[28] = 0xFF;
            data[27] = pifI; data[26] = gfpb; data[25] = gfpa; data[24] = 0xFF;
            data[23] = pifI; data[22] = 0xFF; data[21] = gfpb; data[20] = gfpa;
            data[19] = pifI; data[18] = 0xFF; data[17] = pifI; data[16] = gfpb;
            data[15] = gfpa; data[14] = 0xFF; data[13] = pifI; data[12] = 0xFF;
            data[11] = gfpb; data[10] = gfpa; data[9]  = pifI; data[8]  = 0xFF;
            data[7]  = pifI; data[6]  = gfpb; data[5]  = gfpa; data[4]  = 0xFF;
            data[3]  = pifI; data[2]  = 0xFF; data[1]  = gfpb; data[0]  = gfpa;
            break;
        case 0xF: /* PIF / GFP B / GFP A / Line */
                                              data[29] = pifI; data[28] = line;
            data[27] = pifI; data[26] = gfpb; data[25] = gfpa; data[24] = line;
            data[23] = pifI; data[22] = line; data[21] = gfpb; data[20] = gfpa;
            data[19] = pifI; data[18] = line; data[17] = pifI; data[16] = gfpb;
            data[15] = gfpa; data[14] = line; data[13] = pifI; data[12] = line;
            data[11] = gfpb; data[10] = gfpa; data[9]  = pifI; data[8]  = line;
            data[7]  = pifI; data[6]  = gfpb; data[5]  = gfpa; data[4]  = line;
            data[3]  = pifI; data[2]  = line; data[1]  = gfpb; data[0]  = gfpa;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;

    }

    /* generate number of entries per interface */
    for (i = 0; i < 30; i++) {
        switch(data[i]){
            case LINE_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numLine++;
                break;
            case LINE_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numLine++;
                break;
            case GFP_A_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpA++;
                break;
            case GRP_A_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpA++;
                break;
            case GFP_B_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpB++;
                break;
            case GRP_B_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numGfpB++;
                break;
            case PIF_WR_INTF_GRP:
                if(dir==1){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numPifI++;
                break;
            case PIF_RD_INTF_GRP:
                if(dir==0){
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                numPifI++;
                break;
            case HYPHY20G_CPB_SCHD_NULL_CHNL:
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    for(i=0; i < 30; i++){
        *(pLinearCal+i) = data[i];
        DBG_PRINT("data[%d] = %d, *(pLinearCal+%d) = %d\n",(unsigned int)i, (unsigned int)data[i], (unsigned int)i, (unsigned int)(*(pLinearCal+i)));
    }

    pLinearCal = data;
    *pLineEntries = numLine;
    *pGfpAEntries = numGfpA;
    *pGfpBEntries = numGfpB;
    *pPifEntries = numPifI;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbGetQdrCalDataStress */

/*******************************************************************************
**
**  cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the channel context for all channels used in
**                  line read or write interface group schedulers.  Optionally
**                  generates the scheduler calendar memory based on a best
**                  effort even distribution.  Ensures sufficient scheduler
**                  entries assigned per channel to guarantee minimum bandwidth
**                  requirements met.
**
**
**  INPUTS:         fileHndl  - device handle
**                  scope     - scope of channels to be associated with calendar
**                              generation
**                              0 - Enabled Channels Only
**                              1 - Provisioned and Enabled Channels
**                  mode      - evaluation mode
**                              0 - generate scheduler
**                              1 - evaluate ability to add to scheduler
**                              When mode is set to 1 *pCalData and
**                              *pNumRows are not updated by the code, however
**                              proper size elements must be provided.
**                 *pSchd     - pointer to interface group scheduler context
**                              for one of the following schedulers
**                               - CPB Line Interface Group Read Scheduler
**                               - CPB Line Interface Group Read Scheduler
**                 *pCalData  - pointer to formatted scheduler calendar data
**                 *pNumRows  - pointer to number of rows required by the
**                              scheduler
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   This function updates the totalChnlRate parameter of the
**                  pSchd structure.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd( struct file  *fileHndl,
                                                     UINT4 scope, UINT4 mode,
                                                     sHYPHY20G_SCHD_CTXT *pSchd,
                                                     UINT4 *pCalData,
                                                     UINT4 *pNumRows)
{
    sHYPHY20G_RD_CHNL_CTXT *pRdChnl;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnl;
    UINT4 chnlOffset, chnlDir;
    UINT4 totalChnlRate = 0, numCalChnl = 0;
    UINT4 lowestChnlRate, maxChnlRate;
    UINT4 workingEntries, chnlRate;
    UINT4 numCalEntries, populateChnl, populateRate, minEntries;
    UINT4 numEntriesReqd, assignEntries, numEntriesStole = 0, maxCalRows;
    UINT4 numCalRows, findNumCalRows, totalMinEntries, chnlNum, numEntriesAvail;
    UINT4 calSpace, calSingles;
    UINT4 i, j, k, size;
    UINT4 lowestIndex = 0, lowestRate;
    UINT4 emptyLocOffset, linearCaloffset;
    UINT4 sum, tmp, swap;
    UINT4 dummy, availRate;
    UINT4 halfNumCalEntries, updatedNumCalChnl, splitLowRate;
    UINT4 numEmptyLoc, newNumEmptyLoc;
    /* alloate memory for local use by function */
    UINT4 *pChnlNum, *pRawChnlNum;
    UINT4 *pChnlRate, *pRawChnlRate;
    UINT4 *pLinearCal;
    UINT4 *pValid;
    UINT4 *pEmptyLoc;
    UINT4 *pMinEntries;
    UINT4 *pAssignEntries;

    INT4 result;
    /* verify pointer to global cpb data */
    sHYPHY20G_CTXT *pDevCtxt;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    /* argument checking */
    if((scope > 1)||(pSchd->schdId != HYPHY20G_CPB_LINE_WR_SCHD &&
        pSchd->schdId != HYPHY20G_CPB_LINE_RD_SCHD)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* When a channel has a bandwidth > 50%, the channel is split into two
       entries.  One entry represents 50% of the channel's bandwidth; the other
       represents the remainder.  Therefore, we need (pSchd->maxChnl)+1),
       instead of (pSchd->maxChnl), worth of memory for pChnlNum and
       pAssignEntries.
    */

    size = sizeof(UINT4)*((pSchd->maxChnl)+1) +
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->maxChnl)) +
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->numRows)*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)+
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->numRows)*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)+
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->maxChnl)+1);


    pChnlNum = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pChnlNum == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pRawChnlNum  = (UINT4 *)(pChnlNum + ((pSchd->maxChnl)+1));
    pChnlRate    = (UINT4 *)(pRawChnlNum + (pSchd->maxChnl));
    pRawChnlRate = (UINT4 *)(pChnlRate + ((pSchd->maxChnl)));
    pLinearCal   = (UINT4 *)(pRawChnlRate + (pSchd->maxChnl));
    pValid       = (UINT4 *)(pLinearCal + ((pSchd->numRows)*
                             HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW));
    pEmptyLoc    = (UINT4 *)(pValid + (pSchd->maxChnl));
    pMinEntries  = (UINT4 *)(pEmptyLoc + ((pSchd->numRows)*
                             HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW));
    pAssignEntries = (UINT4 *)(pMinEntries + (pSchd->maxChnl));

    /* initialize channel data pointers */
    pRdChnl = NULL;
    pWrChnl = NULL;

    /**************************************************************************/
    /*  EXTRACT CHANNEL RATE DATA FOR LOCAL USE BY FUNCTION                   */
    /**************************************************************************/

    chnlOffset = pSchd->portOffset;
    chnlDir = pSchd->intfGrpDir;
    /* extract channel context for making calendar population decisions */
    if (chnlDir){
        /* Interface Group Contains Read Channels/Ports */
        pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl +
                                             chnlOffset);
        j = 0;
        lowestChnlRate = 0xFFFFFFFF;
        maxChnlRate = 0;
        for (i=0; i < pSchd->maxChnl; i++) {
             if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
               ((pRdChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL)||
               ((pRdChnl->state == HYPHY20G_CPB_CHNL_EQUIPPED)&&(scope == 1)))){
                *(pRawChnlNum + j) = pRdChnl->chnlId;
                *(pRawChnlRate + j) = pRdChnl->rate;
                totalChnlRate += *(pRawChnlRate+j);
                if (lowestChnlRate > *(pRawChnlRate + j)){
                    lowestChnlRate = *(pRawChnlRate + j);
                }
                if (maxChnlRate < *(pRawChnlRate + j)) {
                    maxChnlRate = *(pRawChnlRate + j);
                }
                numCalChnl++;
                j++;
            }
            pRdChnl++;
        }
    } else {
        /* Interface Group Contains Write Channels/Ports */
        pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pWrChnl +
                                             chnlOffset);
        j = 0;
        lowestChnlRate = 0xFFFFFFFF;
        maxChnlRate = 0;
        for (i=0; i < pSchd->maxChnl; i++) {
             if(((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
               (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL))||
               ((pWrChnl->state == HYPHY20G_CPB_CHNL_EQUIPPED)&&(scope == 1))) {
                *(pRawChnlNum + j) = pWrChnl->chnlId;
                *(pRawChnlRate + j) = pWrChnl->rate;
                totalChnlRate += *(pRawChnlRate + j);
                if (lowestChnlRate > *(pRawChnlRate + j)){
                    lowestChnlRate = *(pRawChnlRate + j);
                }
                if (maxChnlRate < *(pRawChnlRate + j)){
                    maxChnlRate = *(pRawChnlRate + j);
                }
                numCalChnl++;
                j++;
            }
            pWrChnl++;
        }
    }

    /**************************************************************************/
    /*  DETERMINE NUMBER OF SCBS ROWS FOR INTERFACE                           */
    /**************************************************************************/
    maxCalRows = (chnlDir!=0) ? HYPHY20G_CPB_LINE_RD_SCHD_MAX_NUM_ROW:HYPHY20G_CPB_LINE_WR_SCHD_MAX_NUM_ROW;

    /**************************************************************************/
    /*  GENERATE RETURNED SCHEDULER WILL ALL NULL ENTRIES                     */
    /**************************************************************************/
    for (i=0; i < maxCalRows; i++) {
        for (j=0; j < HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; j++){
            *(pCalData + (HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW*i) + j) =
                HYPHY20G_CPB_SCHD_NULL_CHNL;
        }
    }

    /**************************************************************************/
    /*  RETURN DATA FOR SCHEDULER WITH 0 CHANNELS REQUIRING BANDWIDTH         */
    /**************************************************************************/
    if(numCalChnl==0){
        *pNumRows = maxCalRows;
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /**************************************************************************/
    /*  RETRIEVE AVAILABLE BANDWIDTH FOR INTERFACE                            */
    /**************************************************************************/

    /* retrieve bandwidth required by interface */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, chnlDir, 0,
                                               &availRate, &dummy, &dummy,
                                               &dummy);
    if(result){
        sysHyPhy20gMemFree(pChnlNum);
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* Update scheduler structure with context gathered from individual
       channels */
    pSchd->totalChnlRate = totalChnlRate;

    /**************************************************************************/
    /*  PUT CHANNEL RATE DATA IN ORDER FROM LOWEST TO HIGHEST RATE            */
    /**************************************************************************/

    /* sort chnlNum and chnlRate so that they are ordered from lowest to
       highest chnlRate */
    for(i=0; i < pSchd->maxChnl; i++){
        if(i < numCalChnl){
            *(pValid + i) = 1;
        } else {
            *(pValid + i) = 0;
        }
    }
    lowestRate = 0xFFFFFFFF;
    for(j=0; j < numCalChnl; j++) {
        /* find lowest valid in set */
        for(i=0; i < numCalChnl; i++) {
            if((*(pRawChnlRate + i) < lowestRate)&& *(pValid + i)){
                lowestRate = *(pRawChnlRate + i);
                lowestIndex = i;
            }
        }
        *(pChnlRate + j) = *(pRawChnlRate + lowestIndex);
        *(pChnlNum + j) = *(pRawChnlNum + lowestIndex);
        *(pValid + lowestIndex) = 0;
        lowestRate = 0xFFFFFFFF;
    }


    /**************************************************************************/
    /*  DETERMINE NUMBER OF SCBS ROWS TO SUPPORT SCHEDULER BW REQUIREMENTS    */
    /*  STORE THE MINIMUM ENTRIES PER CHNL AND SUM OF MIN ENTRIES             */
    /**************************************************************************/

    /* determine the number of calendar entries starting at maximum possible */
    numCalRows = maxCalRows;
    findNumCalRows = 1;
    totalMinEntries = 0;

    while(findNumCalRows){
        /* check for failing conditions */
        if (numCalRows == 0){
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
        }
        /* calculate rate per interface calendar entry */
        numCalEntries = numCalRows * HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW;


        /* determine the minimum entries required to meet channel rate */
        for(j=0; j < numCalChnl; j++) {
            chnlRate = *(pChnlRate + j);
            chnlNum = *(pChnlNum +j);
            result = cpbHyPhy20gCpbLineSchdChnlRateScale(fileHndl, chnlDir,
                                                         chnlNum, chnlRate,
                                                         availRate, numCalRows,
                                                         &minEntries);
            if(result){
                sysHyPhy20gMemFree(pChnlNum);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            /* store to array */
            *(pMinEntries + j) = minEntries;
            /* store accumulate entries required by calendar */
            totalMinEntries += minEntries;
        }


        /* determine if totalMinEntries <= numCalEntries */
        if(totalMinEntries <= numCalEntries){
            /* if true exit */
            findNumCalRows = 0;
        } else {
            /* if false reduce numCalEntries by 4 and reset totalMinEntries */
            numCalRows--;
            totalMinEntries = 0;
            if((numCalRows*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW < numCalChnl)||(numCalRows == 0)){
                sysHyPhy20gMemFree(pChnlNum);
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_BW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_BW;
            }
        }
    }

    /* Error out on unexpected exception case that prevents further operation
       of code */
    numCalEntries = numCalRows * HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW;
    if (numCalChnl > numCalEntries){
        sysHyPhy20gMemFree(pChnlNum);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_CAL_SIZE\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_CAL_SIZE;
    }

    *pNumRows = numCalRows;

    /**************************************************************************/
    /*  EXIT API IF ONLY EVALUATING THE ABILITY FOR THE CHNLRATE TO BE        */
    /*  ACCEPTED                                                              */
    /**************************************************************************/
    if(mode == 1){
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /******************************************************************************/
    /*  DEBUG PRINT INITIAL DATA                                                  */
    /******************************************************************************/
    DBG_PRINT("%s, %s, %d, Initial Rate Data: with numCalEntries = %u numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries, (unsigned int)numCalChnl);
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
            DBG_PRINT("%s, %s, %d, chnl = %u, rate = %u, minEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pChnlRate + i), (unsigned int)*(pMinEntries + i));
        }
    }

    /******************************************************************************/
    /*  END DEBUG PRINT INITIAL DATA                                              */
    /******************************************************************************/


    /**************************************************************************/
    /*  SCALE CALENDAR ENTRIES                                                */
    /**************************************************************************/
    /* scale required calendar entries relative to the sum of minimum required
       entries - increase the number of entries for channels to ensure that
       scheduler does not have empty entries - ensure that minEntries per
       channel is maintained */
    workingEntries = numCalEntries;
    for(i=0; i < numCalChnl; i++){
        minEntries = *(pMinEntries + i);
        /* scale entries rounding up */

        if(totalMinEntries == 0){
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
        }
        assignEntries = minEntries*numCalEntries/totalMinEntries +
                        (minEntries*numCalEntries%totalMinEntries != 0 ? 1:0 );
        /* for the last channel ensure that round up of assignEntries does not
           prevent completion of subsequent code */
        if(numCalChnl - 1 == i){
            DBG_PRINT("%s, %s, %d, Last Channel Assign Balance of Entries\n",__FILE__, __FUNCTION__, __LINE__);
            assignEntries = workingEntries;
        }

        if(assignEntries < minEntries){
            DBG_PRINT("%s, %s, %d, Increase Entries to meet minimum required\n",__FILE__, __FUNCTION__, __LINE__);
            assignEntries = minEntries;
        }
        if(assignEntries > workingEntries){
            DBG_PRINT("%s, %s, %d, Require entry steal\n",__FILE__, __FUNCTION__, __LINE__);
            /* steal entries from previously scaled channels */
            numEntriesReqd = assignEntries - workingEntries;
            numEntriesStole = 0;
            DBG_PRINT("%s, %s, %d, chnl = %u, Needs entries numEntriesStole = %u, numEntriesReqd = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)numEntriesStole, (unsigned int)numEntriesReqd);
            for(j=0; j < i; j++){
                DBG_PRINT("%s, %s, %d, steal entry for loop\n",__FILE__, __FUNCTION__, __LINE__);
                if(numEntriesReqd > numEntriesStole){
                    DBG_PRINT("%s, %s, %d,  if(numEntriesReqd < numEntriesStole\n)",__FILE__, __FUNCTION__, __LINE__);
                    if(*(pAssignEntries + j) > *(pMinEntries + j)){
                        numEntriesAvail = *(pAssignEntries + j) - *(pMinEntries + j);
                        if (numEntriesAvail){
                            if(numEntriesReqd > numEntriesAvail + numEntriesStole){
                                /* steal all entries */
                                DBG_PRINT("%s, %s, %d, Steal All Entries\n",__FILE__, __FUNCTION__, __LINE__);
                                *(pAssignEntries+j) = *(pMinEntries + j);
                                numEntriesStole += numEntriesAvail;
                            } else {
                                /* steal balance of entries needed */
                                DBG_PRINT("%s, %s, %d, steel entries needed\n",__FILE__, __FUNCTION__, __LINE__);
                                *(pAssignEntries+j) -= (numEntriesReqd-numEntriesStole);
                                numEntriesStole += (numEntriesReqd-numEntriesStole);
                            }
                        }
                    }
                }
            }
            if(numEntriesStole < numEntriesReqd){
                sysHyPhy20gMemFree(pChnlNum);
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_BW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_BW;
            }
            workingEntries += numEntriesStole;
        }
        DBG_PRINT("%s, %s, %d, chnl = %u, numEntriesStole = %u, workingEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)numEntriesStole, (unsigned int)workingEntries);
        workingEntries -= assignEntries;
        *(pAssignEntries + i) = assignEntries;
        DBG_PRINT("%s, %s, %d, chnl = %u, *(pAssignEntries + i) = %u, workingEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pAssignEntries + i), (unsigned int)workingEntries);

    }

    /******************************************************************************/
    /*  DEBUG PRINT SCALED DATA                                                  */
    /******************************************************************************/
    DBG_PRINT("%s, %s, %d, Scaled Rate Data: with numCalEntries = %u numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries, (unsigned int)numCalChnl);
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
            DBG_PRINT("%s, %s, %d, chnl = %u, *(pAssignEntries + i) = %u, minEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pAssignEntries + i), (unsigned int)*(pMinEntries + i));
        }
    }

    /******************************************************************************/
    /*  END DEBUG PRINT SCALED DATA                                              */
    /******************************************************************************/


    /**************************************************************************/
    /*  EVENLY DISTRIBUTE CALENDAR ENTRIES                                    */
    /**************************************************************************/

    /* initially populate the full calendar with NULL channel */
    for(i=0; i < numCalEntries; i++){
        *(pLinearCal + i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
        *(pEmptyLoc + i) = i;
    }
    numEmptyLoc = newNumEmptyLoc = numCalEntries;

    /* test for entry greater than 50% calendar length and split into two
       entries */
    halfNumCalEntries = numCalEntries/2;
    updatedNumCalChnl = numCalChnl; /* this index requires the extra element
                                       when memory allocated for pChnlNum
                                       and pAssignEntries */

    for (i = 0; i < numCalChnl; i++) {
        if (*(pAssignEntries + i) > halfNumCalEntries) {
            /* split into two entries */
            splitLowRate = *(pAssignEntries + i) - halfNumCalEntries;
            *(pAssignEntries + updatedNumCalChnl) = splitLowRate;
            *(pChnlNum + updatedNumCalChnl) = *(pChnlNum + i);
            updatedNumCalChnl++;

            *(pAssignEntries + i) = halfNumCalEntries;
        }
    }
    DBG_PRINT("%s, %s, %d, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalChnl);
    numCalChnl = updatedNumCalChnl;
    DBG_PRINT("%s, %s, %d, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalChnl);

    /* order entries from high-to-low based on channel rate  */
    do {
        swap = 0;
        for(i=0; i < numCalChnl-1; i++) {
            j = i + 1;
            if (*(pAssignEntries + j) > *(pAssignEntries + i)) {
                /* swap */
                tmp = *(pAssignEntries + i);
                *(pAssignEntries + i) = *(pAssignEntries + j);
                *(pAssignEntries + j) = tmp;

                tmp = *(pChnlNum + i);
                *(pChnlNum + i) = *(pChnlNum + j);
                *(pChnlNum + j) = tmp;

                swap = 1;
            }
        }
    } while (swap);

    /******************************************************************************/
    /*  DEBUG PRINT ORDERED SCALED DATA                                           */
    /******************************************************************************/
    DBG_PRINT("%s, %s, %d, Ordered Final Rate Data: with numCalEntries = %u, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries, (unsigned int)numCalChnl);
    sum = 0;
    for(i=0; i < numCalChnl; i++){
        if(*(pAssignEntries + i) != 0){
        /* this prints excess information */
        DBG_PRINT("%s, %s, %d, chnl = %u, assignedEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pAssignEntries + i));
            sum = sum + *(pAssignEntries + i);
        }
    }
    DBG_PRINT("%s, %s, %d, Final Rate Sum = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)sum);
    /******************************************************************************/
    /*  END DEBUG PRINT ORDERED SCALED DATA                                       */
    /******************************************************************************/

    /* STRICT POPULATION ALGO */
    for (i=0; i < numCalChnl; i++) {
        DBG_PRINT("DEBUG_JC 1: numEmptyLoc = %lu, newNumEmptyLoc = %lu\n", numEmptyLoc, newNumEmptyLoc);

        populateRate = *(pAssignEntries + i);
        populateChnl = *(pChnlNum + i);
        /* identify the required number of calendar entries between each
           occurance of this channel number */
        if(populateRate == 0){
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
        }
        calSpace = numEmptyLoc/populateRate;
        calSingles = numEmptyLoc%populateRate;
        if (calSpace == 0) {
            DBG_PRINT("DEBUG_JC 1a: calSpace forced to 1\n");
            calSpace = 1;
        }

        DBG_PRINT("DEBUG_JC 2: LINE %d\n", __LINE__);
        emptyLocOffset = 0;
        for (j=0; j < populateRate; j++) {
            /* fetch location of empty entry, alternating between start and
               end of list */
            if(j==0) {
                emptyLocOffset = 0;
            } else {
                if(calSingles){
                    emptyLocOffset += (calSpace+1);
                    calSingles--;
                } else {
                    emptyLocOffset += calSpace;
                }
            }
            linearCaloffset = *(pEmptyLoc + emptyLocOffset);

            /*pPopulate calendar with channel number */
           *(pLinearCal + linearCaloffset) = populateChnl;

           /* remove empty location from list */
           *(pEmptyLoc + emptyLocOffset) = 0xFFFF;
           newNumEmptyLoc--;
        }

        DBG_PRINT("DEBUG_JC 3: LINE %d\n", __LINE__);
        /* push all empty locations to front of list while maintaining order */
        do {
            swap = 0;
            for(j=0; j < numCalEntries-1; j++) {
                k = j + 1;
                if (*(pEmptyLoc + k) < *(pEmptyLoc + j)) {
                    /* swap */
                    tmp = *(pEmptyLoc + j);
                    *(pEmptyLoc + j) = *(pEmptyLoc + k);
                    *(pEmptyLoc + k) = tmp;

                    swap = 1;
                }
            }
        } while (swap);

        numEmptyLoc = newNumEmptyLoc;
    }

    DBG_PRINT("%s, %s, %d, Format Calendar Entry Data to Rows of SCBS\n",__FILE__, __FUNCTION__, __LINE__);

    /**************************************************************************/
    /*  FORMAT CALENDAR ENTRY DATA TO ROWS OF SCBS                            */
    /**************************************************************************/

    /* apply the linear calendar data to the device scheduler calendar
       format */
    /* Note: balance of rows previously populated with NULL address */
    j = 0;
    for(i = 0; i < numCalRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 1)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    for(i=0; i < numCalRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 2)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 3)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    /* free allocated memory used by local function */
    sysHyPhy20gMemFree(pChnlNum);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd */

/*******************************************************************************
**
**  cpbHyPhy20gCpbGetQdrIntfAvailRate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the available QDR bandwidth
**                  available to each interface.
**
**                  It uses the ram context to identify the rate per QDR entry
**                  and the interface context to determine which interfaces
**                  are enabled.
**
**  INPUTS:         fileHndl        - device handle
**                  dir             - Entry values generated relative to:
**                                    0 - Write interfaces
**                                    1 - Read interfaces
**                  scale           - 0 - Use raw QDR entry bandwidth
**                                    1 - Use scaled QDR entry bandwidth
**                  *pLineAvailRate - Rate in Mbps for Line Entries
**                  *pGfpAAvailRate - Rate in Mbps for GFPA Entries
**                  *pGfpBAvailRate - Rate in Mbps for GFPB Entries
**                  *pPifAvailRate  - Rate in Mbps for PIF Entries
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetQdrIntfAvailRate(struct file  *fileHndl, UINT4 dir,
                                       UINT4 scale, UINT4 *pLineAvailRate,
                                       UINT4 *pGfpAAvailRate,
                                       UINT4 *pGfpBAvailRate, UINT4 *pPifAvailRate)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 valid;
    UINT4 qdrEntryRate, numLine, numGfpA, numGfpB, numPif, dummy, dummyArray[30];
    UINT4 revId;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Argument checking */
    if(dir > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve rate of QDR entry */
    result = cpbHyPhy20gCpbGetQdrRamCtxt(fileHndl, &valid, &dummy, &dummy,
                                         &dummy, &dummy, &dummy, &dummy,
                                         &qdrEntryRate);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    if(scale == 0){
        qdrEntryRate = HYPHY20G_CPB_QDR_RAW_SCHD_ENTRY_RATE;
    }

    if (valid != HYPHY20G_VALID_SIGN){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if(pDevCtxt->gData.hyphy20gQdrStressMode) {
        /* call Stress Optimized QDR Config */
        /* retrieve maximum number of entries assigned to each interface */
        result = cpbHyPhy20gCpbGetQdrCalDataStress(fileHndl, dir, dummyArray, &numLine,
                                                   &numGfpA, &numGfpB, &numPif);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
    } else {
       /* retrieve revision ID */
        result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
        if(result) return result;
        /* call Standard QDR Config */
        /* retrieve maximum number of entries assigned to each interface */
        result = cpbHyPhy20gCpbGetQdrCalData(fileHndl, dir, dummyArray, &numLine,
                                             &numGfpA, &numGfpB, &numPif);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
    }

    /* calculate available rates */
    *pLineAvailRate = qdrEntryRate * numLine;
    *pGfpAAvailRate = qdrEntryRate * numGfpA;
    *pGfpBAvailRate = qdrEntryRate * numGfpB;
    *pPifAvailRate = qdrEntryRate * numPif;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbGetQdrIntfAvailRate */

/*******************************************************************************
**
**  cpbHyPhy20gCpbUpdateIntfCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Updates interface context related to total available
**                  bandwidth for read and write schedulers based on
**                  which interfaces are enabled in interface context.
**
**                  For Line and GFP the available interface rate is the raw
**                  interface rate.  For PIf the interface rate is the scaled
**                  interface rate.
**
**  INPUTS:         fileHndl    - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbUpdateIntfCtxt(struct file *fileHndl)
{
    /* variable declartion */
    sHYPHY20G_INTF_GRP_CTXT *pLineIntf;
    sHYPHY20G_INTF_GRP_CTXT *pGfpAIntf;
    sHYPHY20G_INTF_GRP_CTXT *pGfpBIntf;
    sHYPHY20G_INTF_GRP_CTXT *pPifIntf;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 lineWrAvailRate, gfpaWrAvailRate, gfpbWrAvailRate, pifWrAvailRate;
    UINT4 lineRdAvailRate, gfpaRdAvailRate, gfpbRdAvailRate, pifRdAvailRate;
    UINT4 dummy;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* retrieve pointers to interface group context */
    pLineIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+HYPHY20G_CPB_LINE_INTF_GRP;
    DBG_PRINT("Assigned pLineIntf Pointer %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
    pGfpAIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+HYPHY20G_CPB_GFPA_INTF_GRP;
    pGfpBIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+HYPHY20G_CPB_GFPB_INTF_GRP;
    pPifIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+HYPHY20G_CPB_PIF_INTF_GRP;
    DBG_PRINT("Assigned Remaining Pointers %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* retrieve raw available rate for line and gfp write schedulers */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 0, 0,
                                               &lineWrAvailRate,
                                               &gfpaWrAvailRate,
                                               &gfpbWrAvailRate,
                                               &dummy);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* retrieve raw available rate for line and gfp read schedulers */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 1, 0,
                                               &lineRdAvailRate,
                                               &gfpaRdAvailRate,
                                               &gfpbRdAvailRate,
                                               &dummy);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* retrieve scaled available rate for PIF write schedulers */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 0, 1,
                                               &dummy,
                                               &dummy,
                                               &dummy,
                                               &pifWrAvailRate);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* retrieve scaled available rate for PIF read schedulers */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 1, 1,
                                               &dummy,
                                               &dummy,
                                               &dummy,
                                               &pifRdAvailRate);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* update interface context with new available interface rates */
    pLineIntf->availWrRate = lineWrAvailRate;
    DBG_PRINT("Updating availWrRate data %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
    pGfpAIntf->availWrRate = gfpaWrAvailRate;
    pGfpBIntf->availWrRate = gfpbWrAvailRate;
    pPifIntf->availWrRate = pifWrAvailRate;
    pLineIntf->availRdRate = lineRdAvailRate;
    pGfpAIntf->availRdRate = gfpaRdAvailRate;
    pGfpBIntf->availRdRate = gfpbRdAvailRate;
    pPifIntf->availRdRate = pifRdAvailRate;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbUpdateIntfCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbWrChnlProvEval
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the ability to add a write channel to an interface
**                  group.
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - channel number
**                  chnlRate    - channel rate
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbWrChnlProvEval(struct file *fileHndl, UINT4 chnlId,
                                  UINT4 chnlRate)
{
    /* variable declartion */
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 *pCalData = NULL;
    UINT4 intf, size;
    UINT4 validTemp, stateTemp, rateTemp;
    UINT4 i, totalChnlRate = 0, dummy, *wrAvailRate = 0;
    UINT4 gfpaWrAvailRate, gfpbWrAvailRate, pifWrAvailRate;
    UINT4 wrPortOffset = 0, wrMaxChnl = 0;
    UINT4 numRows;
    INT4 result;

    /* argument checking */
    if((chnlId >= HYPHY20G_CPB_NUM_WR_PORT)||(chnlRate < 1)||
       (chnlRate > 12000)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* extract total raw bandwidth for GFP A and GFP B interface */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 0, 0,
                                               &dummy,
                                               &gfpaWrAvailRate,
                                               &gfpbWrAvailRate,
                                               &dummy);
    if(result) return result;
    /* extract total scaled bandwidth for PIF interface */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 0, 1,
                                               &dummy,
                                               &dummy,
                                               &dummy,
                                               &pifWrAvailRate);
    if(result) return result;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);

    /* store channel context during scheduler evaluation */
    validTemp = pWrChnl->valid;
    stateTemp = pWrChnl->state;
    rateTemp = pWrChnl->rate;

    /* temporarily update channel context for scheduler evaluation */
    pWrChnl->valid = HYPHY20G_VALID_SIGN;
    pWrChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;
    pWrChnl->rate = chnlRate;

    /* identify interface associated with channel */
    if (chnlId < HYPHY20G_CPB_GFPA_PORT_OFFSET) {
        intf = HYPHY20G_CPB_LINE_WR_SCHD;
    } else if (chnlId < HYPHY20G_CPB_GFPB_PORT_OFFSET) {
        /* GFP WR interface populates SCBS to ensure
           a minimum of 1 entry per channel.

           For this interfaces channel will be rejected
           if QDR BW dedicated to interface is exceeded by the raw
           channel rate of channels within interface.*/
        intf = HYPHY20G_CPB_GFPA_WR_SCHD;
        wrAvailRate = &gfpaWrAvailRate;
        wrPortOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
        wrMaxChnl = HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL;
        DBG_PRINT("%s, %s, %d, gfpaWrAvailRate = %u, wrPortOffset = %u, wrMaxChnl = %u\n",__FILE__, __FUNCTION__, __LINE__,
                 (unsigned int)gfpaWrAvailRate, (unsigned int)wrPortOffset, (unsigned int)wrMaxChnl);
    } else if (chnlId < HYPHY20G_CPB_PIF_PORT_OFFSET) {
        /* GFP WR interface populates SCBS to ensure
           a minimum of 1 entry per channel.

           For this interfaces channel will be rejected
           if QDR BW dedicated to interface is exceeded by the raw
           channel rate of channels within interface.*/
        intf = HYPHY20G_CPB_GFPB_WR_SCHD;
        wrAvailRate = &gfpbWrAvailRate;
        wrPortOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
        wrMaxChnl = HYPHY20G_CPB_GFPB_WR_SCHD_MAX_CHNL;
        DBG_PRINT("%s, %s, %d, gfpbWrAvailRate = %u, wrPortOffset = %u, wrMaxChnl = %u\n",__FILE__, __FUNCTION__, __LINE__,
                 (unsigned int)gfpbWrAvailRate, (unsigned int)wrPortOffset, (unsigned int)wrMaxChnl);
    } else {
        /* if PIF_WR_INTF_GRP there is no interface scheduler */
        /* retrieve total bandwidth of channels and compare with
           available PIF QDR interface rate */
        intf = PIF_WR_INTF_GRP;
        wrAvailRate = &pifWrAvailRate;
        wrPortOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
        wrMaxChnl = HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL;
        DBG_PRINT("%s, %s, %d, pifWrAvailRate = %u, wrPortOffset = %u, wrMaxChnl = %u\n",__FILE__, __FUNCTION__, __LINE__,
                 (unsigned int)pifWrAvailRate, (unsigned int)wrPortOffset, (unsigned int)wrMaxChnl);
    }

    if(intf != HYPHY20G_CPB_LINE_WR_SCHD){     /* if GFP or PIF case...*/
        /* Extract chnlRate required for PROVISIONED/ENABLED channels */
        pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pWrChnl +
                                             wrPortOffset);

        for (i=0; i < wrMaxChnl; i++) {
             if(((pWrChnl->valid == HYPHY20G_VALID_SIGN)&&
               (pWrChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL))||
               (pWrChnl->state == HYPHY20G_CPB_CHNL_EQUIPPED)) {
                totalChnlRate += pWrChnl->rate;
                DBG_PRINT("%s, %s, %d, i = %u, pRdChnl->rate = %u, totalChnlRate = %u\n",__FILE__, __FUNCTION__, __LINE__, 
                         (unsigned int)i, (unsigned int)pWrChnl->rate, (unsigned int)totalChnlRate);
            }
            pWrChnl++;
        }

        /* undo temporary channel context change */
        pWrChnl = (sHYPHY20G_WR_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pWrChnl + chnlId);
        pWrChnl->valid = validTemp;
        pWrChnl->state = stateTemp;
        pWrChnl->rate = rateTemp;

        DBG_PRINT("%s, %s, %d, totalChnlRate = %u, *wrAvailRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)totalChnlRate, (unsigned int)*wrAvailRate);

        if(totalChnlRate > *wrAvailRate){
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_BW\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_SCHD_BW;
        }
        return HYPHY20G_SUCCESS;
    } /* end of GFP/PIF case */

    /* evaluate if channel addition will impact line wr SCBS
       requirements */

    /* allocate memory to pCalData used for holding scheduler contents */
    /* use PIF Scheduler context size as this is the largest scheduler */
    size = sizeof(UINT4)*(HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW*
                          HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW);

    pCalData = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pCalData == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__); return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* retrieve pointer to scheduler context */
    pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+intf;

    /* evaluate if interface scheduler can or cannot be generated */
    result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 1, 1,
                                                            pSchd, pCalData,
                                                            &numRows);
    /* undo temporary channel context change */
    pWrChnl->valid = validTemp;
    pWrChnl->state = stateTemp;
    pWrChnl->rate = rateTemp;

    /* deallocate memory */
    sysHyPhy20gMemFree(pCalData);

    /* return any unsuccessful result of applying the channel to
       interface scheduler */
    DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbWrChnlProvEval */

/*******************************************************************************
**
**  cpbHyPhy20gCpbRdChnlProvEval
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the ability to add a read channel to an interface
**                  group.
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - channel number
**                  chnlRate    - channel rate
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbRdChnlProvEval(struct file *fileHndl, UINT4 chnlId,
                                  UINT4 chnlRate)
{
    /* variable declartion */
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 *pCalData = NULL;
    UINT4 intf, size;
    UINT4 validTemp, stateTemp, rateTemp, numRows;
    UINT4 i, rdPortOffset = 0, rdMaxChnl = 0, dummy, *rdAvailRate = 0;
    UINT4 totalChnlRate = 0, gfpaRdAvailRate, gfpbRdAvailRate, pifRdAvailRate;
    INT4 result;

    /* argument checking */
    if((chnlId >= HYPHY20G_CPB_NUM_RD_PORT)||(chnlRate < 1)||
       (chnlRate > 12000)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
   /* retrieve available interface rates */
    /* extract total raw bandwidth for GFP A and GFP B interface */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 1, 0,
                                               &dummy,
                                               &gfpaRdAvailRate,
                                               &gfpbRdAvailRate,
                                               &dummy);
    if(result) return result;
    /* extract total scaled bandwidth for PIF interface */
    result = cpbHyPhy20gCpbGetQdrIntfAvailRate(fileHndl, 1, 1,
                                               &dummy,
                                               &dummy,
                                               &dummy,
                                               &pifRdAvailRate);
    if(result) return result;


    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                (pDevCtxt->pCpbCtxt->pRdChnl + chnlId);

    /* store channel context during scheduler evaluation */
    validTemp = pRdChnl->valid;
    stateTemp = pRdChnl->state;
    rateTemp = pRdChnl->rate;

    /* temporarily update channel context for scheduler evaluation */
    pRdChnl->valid = HYPHY20G_VALID_SIGN;
    pRdChnl->state = HYPHY20G_CPB_CHNL_EQUIPPED;
    pRdChnl->rate = chnlRate;


    /* identify interface associated with channel */
    if (chnlId < HYPHY20G_CPB_GFPA_PORT_OFFSET) {
        intf = HYPHY20G_CPB_LINE_RD_SCHD;
    } else if (chnlId < HYPHY20G_CPB_GFPB_PORT_OFFSET) {
        intf = HYPHY20G_CPB_GFPA_RD_SCHD;
        rdAvailRate = &gfpaRdAvailRate;
        rdPortOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
        rdMaxChnl = HYPHY20G_CPB_GFPA_RD_SCHD_MAX_CHNL;
        DBG_PRINT("%s, %s, %d, gfpaRdAvailRate = %u, rdPortOffset = %u, rdMaxChnl = %u\n",__FILE__, __FUNCTION__, __LINE__,
                 (unsigned int)gfpaRdAvailRate, (unsigned int)rdPortOffset, (unsigned int)rdMaxChnl);
    } else if (chnlId < HYPHY20G_CPB_PIF_PORT_OFFSET) {
        intf = HYPHY20G_CPB_GFPB_RD_SCHD;
        rdAvailRate = &gfpbRdAvailRate;
        rdPortOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
        rdMaxChnl = HYPHY20G_CPB_GFPB_RD_SCHD_MAX_CHNL;
        DBG_PRINT("%s, %s, %d, gfpbRdAvailRate = %u, rdPortOffset = %u, rdMaxChnl = %u\n",__FILE__, __FUNCTION__, __LINE__,
                 (unsigned int)gfpbRdAvailRate, (unsigned int)rdPortOffset, (unsigned int)rdMaxChnl);
    } else {
        intf = HYPHY20G_CPB_PIF_RD_SCHD;
        rdAvailRate = &pifRdAvailRate;
        rdPortOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
        rdMaxChnl = HYPHY20G_CPB_PIF_RD_SCHD_MAX_CHNL;
        DBG_PRINT("%s, %s, %d, pifRdAvailRate = %u, rdPortOffset = %u, rdMaxChnl = %u\n",__FILE__, __FUNCTION__, __LINE__,
                 (unsigned int)pifRdAvailRate, (unsigned int)rdPortOffset, (unsigned int)rdMaxChnl);

    }



    if(intf != HYPHY20G_CPB_LINE_RD_SCHD) {
        /* Extract chnlRate required for PROVISIONED/ENABLED channels */
        pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl +
                                             rdPortOffset);

        for (i=0; i < rdMaxChnl; i++) {
             if(((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
               (pRdChnl->state == HYPHY20G_CPB_CHNL_OPERATIONAL))||
               (pRdChnl->state == HYPHY20G_CPB_CHNL_EQUIPPED)) {
                totalChnlRate += pRdChnl->rate;
                DBG_PRINT("%s, %s, %d, i = %u, pRdChnl->rate = %u, totalChnlRate = %u\n",__FILE__, __FUNCTION__, __LINE__, 
                         (unsigned int)i, (unsigned int)pRdChnl->rate, (unsigned int)totalChnlRate);
            }
            pRdChnl++;
        }

        /* undo temporary channel context change */
        pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + chnlId);
        pRdChnl->valid = validTemp;
        pRdChnl->state = stateTemp;
        pRdChnl->rate = rateTemp;

        DBG_PRINT("%s, %s, %d, totalChnlRate = %u, *rdAvailRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)totalChnlRate, (unsigned int)*rdAvailRate);

        if(totalChnlRate > *rdAvailRate){
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_BW\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_SCHD_BW;
        }

        return HYPHY20G_SUCCESS;
    }


    /* allocate memory to pCalData used for holding scheduler contents */
    /* use PIF Scheduler context as this is the largest scheduler */
    size = sizeof(UINT4)*(HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW*
                          HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW);

    pCalData = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pCalData == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__); return HYPHY20G_ERR_MEM_ALLOC;
    }


    /* retrieve pointer to scheduler context */
    pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+intf;

    /* evaluate if interface scheduler can or cannot be generated */
    result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 1, 1,
                                                            pSchd, pCalData,
                                                            &numRows);
    /* undo temporary channel context change */
    pRdChnl->valid = validTemp;
    pRdChnl->state = stateTemp;
    pRdChnl->rate = rateTemp;

    /* deallocate memory */
    sysHyPhy20gMemFree(pCalData);

    /* return any unsuccessful result of applying the read channel to
       interface scheduler */
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbRdChnlProvEval */

/*******************************************************************************
**
**  cpbHyPhy20gCpbIntfInitEval
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the ability to add the specified interface to the
**                  CPB based on the availability of scheduler resources.
**
**                  An interface will not be added if the addition of the
**                  interface takes QDR entries away from another interface
**                  such that the other interface will not have enough
**                  bandwidth to support provisioned and enabled channels.
**
**  INPUTS:         fileHndl    - device handle
**                  intf        - interface to be added
**                                0 - Line
**                                1 - GFP A
**                                2 - GFP B
**                                3 - PIF
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbIntfInitEval(struct file *fileHndl, UINT4 intf)
{
    /* variable declartion */
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_INTF_GRP_CTXT *pLineIntf;
    sHYPHY20G_INTF_GRP_CTXT *pGfpAIntf;
    sHYPHY20G_INTF_GRP_CTXT *pGfpBIntf;
    sHYPHY20G_INTF_GRP_CTXT *pPifIntf;
    sHYPHY20G_INTF_GRP_CTXT *pAddIntf;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 *pCalData = NULL;
    UINT4 size, i, numRows;
    UINT4 enblLineStore;
    UINT4 enblGfpaStore, enblGfpbStore, enblPifStore;
    INT4 result;

    /* argument checking */
    if(intf >= HYPHY20G_CPB_NUM_IF_GRP){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* allocate memory to pCalData used for holding scheduler contents */
    /* use PIF Scheduler context as this is the largest scheduler */
    size = sizeof(UINT4)*(HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW*
                          HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW);

    pCalData = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pCalData == NULL) {
        DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* retrieve pointers to interface group context */
    pLineIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+0;
    pGfpAIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+1;
    pGfpBIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+2;
    pPifIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+3;
    pAddIntf = pDevCtxt->pCpbCtxt->pIfGrpCtxt+intf;

    /* capture enable state for interfaces */
    enblLineStore = pLineIntf->enable;
    enblGfpaStore = pGfpAIntf->enable;
    enblGfpbStore = pGfpBIntf->enable;
    enblPifStore = pPifIntf->enable;

    /* temporarily update interface context for scheduler evaluation */
    pAddIntf->enable = HYPHY20G_CPB_INTF_ENABLED;

    /* temporarily update interface context with new available interface
       rates */
    result = cpbHyPhy20gCpbUpdateIntfCtxt(fileHndl);
    if(result){
        /* return interface state to initial value */
        pLineIntf->enable = enblLineStore;
        pGfpAIntf->enable = enblGfpaStore;
        pGfpBIntf->enable = enblGfpbStore;
        pPifIntf->enable  = enblPifStore;
        /* deallocate memory */
        sysHyPhy20gMemFree(pCalData);
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* evaluate each interface scheduler to determine if any
       interface break  */
    for(i=0; i < (HYPHY20G_CPB_NUM_SCHD - 1); i++){
        /* identify interface associated with channel */

        /* retrieve pointer to scheduler context */
        pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+i;

        /* evaluate if interface scheduler can or cannot be generated */
        if (i == HYPHY20G_CPB_LINE_WR_SCHD || i == HYPHY20G_CPB_LINE_RD_SCHD){
            result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 1, 1, pSchd, pCalData, &numRows);
        } else {
            result = cpbHyPhy20gCPBGenerateIntfGrpSchd(fileHndl, 1, pSchd, pCalData);
        }

        if (result){
            /* return interface state to initial value */
            pLineIntf->enable = enblLineStore;
            pGfpAIntf->enable = enblGfpaStore;
            pGfpBIntf->enable = enblGfpbStore;
            pPifIntf->enable  = enblPifStore;

            /* restore interface context related to available rate */
            result = cpbHyPhy20gCpbUpdateIntfCtxt(fileHndl);
            if(result){
                /* deallocate memory */
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            /* deallocate memory */
            sysHyPhy20gMemFree(pCalData);

            /* return error to calling API */
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
    }

    /* if successful to this point regenerate lineRd and lineWr to ensure
       minimum number of SCBS entries per channel for new scheduler is
       satisfied */
    for(i=0; i < 2; i++){
        /* retrieve pointer to scheduler context */
        pSchd = pDevCtxt->pCpbCtxt->pSchdCtxt+i;

        /* update interface scheduler */
        result = cpbHyPhy20gCPBGenerateLineIntfGrpRateAwareSchd(fileHndl, 0, 0, pSchd, pCalData, &numRows);

        if (result){
            /* return interface state to initial value */
            pLineIntf->enable = enblLineStore;
            pGfpAIntf->enable = enblGfpaStore;
            pGfpBIntf->enable = enblGfpbStore;
            pPifIntf->enable  = enblPifStore;

            /* restore interface context related to available rate */
            result = cpbHyPhy20gCpbUpdateIntfCtxt(fileHndl);
            if(result){
                /* deallocate memory */
                sysHyPhy20gMemFree(pCalData);
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
                return result;
            }

            /* deallocate memory */
            sysHyPhy20gMemFree(pCalData);

            /* return error to calling API */
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
    } /* end update lineRd and lineWr SCBS for smaller QDR bandwidth */

    /* return interface state to initial value */
    pLineIntf->enable = enblLineStore;
    pGfpAIntf->enable = enblGfpaStore;
    pGfpBIntf->enable = enblGfpbStore;
    pPifIntf->enable  = enblPifStore ;

    /* restore interface context related to available rate */
    result = cpbHyPhy20gCpbUpdateIntfCtxt(fileHndl);
    if(result){
        /* deallocate memory */
        sysHyPhy20gMemFree(pCalData);
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* deallocate memory */
    sysHyPhy20gMemFree(pCalData);

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbIntfInitEval */

/*******************************************************************************
**
**  cpbHyPhy20gCpbGetQdrEntryWeight
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Retrieves the rate SW Driver applies to each QDR calendar
**                  entry
**
**  INPUTS:         fileHndl    - device handle
**                  *pValid     - chnl state stored in context
**                  *pQdrRate   - rate in Mbps applied to each QDR entry
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetQdrEntryWeight(struct file *fileHndl, UINT4 *pValid,
                                     UINT4 *pQdrRate)
{
    UINT4 valid, qdrRate, dummy;
    INT4 result;

    result = cpbHyPhy20gCpbGetQdrRamCtxt(fileHndl, &valid, &dummy, &dummy,
                                         &dummy, &dummy, &dummy, &dummy,
                                         &qdrRate);
    if(result) return result;

    *pValid = valid;
    *pQdrRate = qdrRate;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbGetQdrEntryWeight */

/*******************************************************************************
**
**  cpbHyPhy20gCpbSetQdrEntryWeight
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets the rate that is associated with a QDR calendar entry.
**
**                  This function will update the context that stores the
**                  QDR rate per entry.
**
**                  This function may only be called after hyPhy20bCpbInit.
**
**                  This function must be used with caution.  If changed to a
**                  smaller number, the currently populated calendar may become
**                  invalid.  This is an unrecoverable condition.
**
**                  For the rate change to take effect the
**
**  INPUTS:         fileHndl    - device handle
**                  qdrRate     - rate in Mbps applied to each QDR entry
**                                Legal range 1 to 2560
**                                2560 = 7680 Mbps / 30 entries
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbSetQdrEntryWeight(struct file *fileHndl, UINT4 qdrRate)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    /* argument checking */
    if ((qdrRate < 1)||(qdrRate > 2560)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pDevCtxt->pCpbCtxt->ram.calEntryRate = qdrRate;

    /* update total available rate for each interface in cpb context */
    result = cpbHyPhy20gCpbUpdateIntfCtxt(fileHndl);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbSetQdrEntryWeight */



/*******************************************************************************
**
**  cpbHyPhy20gCpbGetIntfCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Retrieves context for a CPB interface.  Retrieves contents
**                  of sHYPHY20G_INTF_GRP_CTXT.
**
**  INPUTS:         fileHndl      - device handle
**                  intfGrp       - interface group scheduler to intialize
**                                  0 - CPB CPB Line Interface Group
**                                  1 - CPB GFP A Interface Group
**                                  2 - CPB GFP B Interface Group
**                                  3 - CPB PIF Interface Group
**                  *pValid       Valid state of context
**                                0x5420 (valid)
**                                Others (invalid)
**                  *pIntfGrpNum  Interface Group ID
**                  *pEnable      enabled state of interface group
**                                  0 - disabled
**                                  1 - enabled
**                  *pNumWrChnls  total number of write channels
**                  *pTotalWrRate aggregate bandwidth of all write
**                                channels associated with interface
**                                group
**                  *pAvailWrRate aggregate bandwidth available to
**                                all write channels associated with
**                                interface group
**                  *pNumRdChnls  total number of read channels
**                  *pTotalRdRate aggregate bandwidth of all read
**                                channels associated with interface
**                                group
**                  *pAvailRdRate aggregate bandwidth available to
**                                all read channels associated with
**                                interface group
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetIntfCtxt(struct file  *fileHndl, UINT4 intfGrp,
                               UINT4 *pValid, UINT4 *pIntfGrpNum,
                               UINT4 *pEnable, UINT4 *pNumWrChnls,
                               UINT4 *pTotalWrRate, UINT4 *pAvailWrRate,
                               UINT4 *pNumRdChnls, UINT4 *pTotalRdRate,
                               UINT4 *pAvailRdRate)
{
    /* variable declaration */
    sHYPHY20G_INTF_GRP_CTXT *pIntfGrp;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if(intfGrp >= HYPHY20G_CPB_NUM_IF_GRP){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* argument checking */
    if ((intfGrp >= HYPHY20G_CPB_NUM_INTF_GRP)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover pointer to scheduler context */
    pIntfGrp = (sHYPHY20G_INTF_GRP_CTXT *)(pDevCtxt->pCpbCtxt->pIfGrpCtxt
                                           + intfGrp);

    *pValid = pIntfGrp->valid;
    *pEnable = pIntfGrp->enable;
    *pIntfGrpNum = pIntfGrp->intfGrp;
    *pNumWrChnls = pIntfGrp->numWrChnls;
    *pTotalWrRate = pIntfGrp->totalWrRate;
    *pAvailWrRate = pIntfGrp->availWrRate;
    *pNumRdChnls = pIntfGrp->numRdChnls;
    *pTotalRdRate = pIntfGrp->totalRdRate;
    *pAvailRdRate = pIntfGrp->availRdRate;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbGetIntfCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbGetQdrRamCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Retrieves context CPB QDR.  Retrieves contents of
**                  sHYPHY20G_CPB_RAM_CTXT excluding firstFreeBlk and
**                  blockPtr[1024]
**
**  INPUTS:         fileHndl  - device handle
**                 *pValid
**                 *pRamType        0 - QDR RAM or 1 - interal RAM
**                 *pBlkSize        block size in unitss of 1 brust
**                 *pMinFifoSize    Minimum FIFO size in units of 1 block
**                 *pMaxFifoSize    Maximum FIFO size in units of 1 block
**                 *pNumFreeBlk     Number of free CPB FIFO blocks
**                 *pNumFifo        Number of created FIFOs
**                 *pCalEntryRate   Rate of QDR calendar entry in Mbps
**                                  Used for evaluating available
**                                  bandwidth when initializing interfaces,
**                                  adding channels, or modifying channel
**                                  rate in CPB
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbGetQdrRamCtxt( struct file *fileHndl, UINT4 *pValid,
                                  UINT4 *pRamType, UINT4 *pBlkSize,
                                  UINT4 *pMinFifoSize, UINT4 *pMaxFifoSize,
                                  UINT4 *pNumFreeBlk, UINT4 *pNumFifo,
                                  UINT4 *pCalEntryRate)
{
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* recover pointer to scheduler context */
    pCpbCtxt = pDevCtxt->pCpbCtxt;

    *pValid = pCpbCtxt->ram.valid;
    *pRamType = pCpbCtxt->ram.ramType;
    *pBlkSize = pCpbCtxt->ram.blkSize;
    *pMinFifoSize = pCpbCtxt->ram.minFifoSize;
    *pMaxFifoSize = pCpbCtxt->ram.maxFifoSize;
    *pNumFreeBlk = pCpbCtxt->ram.numFreeBlk;
    *pNumFifo = pCpbCtxt->ram.numFifo;
    *pCalEntryRate = pCpbCtxt->ram.calEntryRate;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbGetQdrRamCtxt */


/*******************************************************************************
**
**  cpbHyPhy20gCpbLineSchdChnlRateScaleCalc
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function calculates the minimum number of interface
**                  scheduler entries required to satisfy the requested chnlRate
**                  in Mbps.
**
**                  For MAC terminated applications, the worst case packet size
**                  of 69 will be used.  This is the most extreme demand on QDR.
**
**  INPUTS:         fileHndl        - device handle
**                  dir             - Entry values generated relative to:
**                                    0 - Write interfaces
**                                    1 - Read interfaces
**                  availBw         - Available Bandwidth provided to interface
**                                    scheduler by QDR memory
**                  numRows         - Number of rows in interface scheduler
**                  dataType        - 0 - MAC Terminated
**                                    1 - CBR
**                  pktSize         - CBR packet size in bytes.  Value not
**                                    processed for dataType == 0
**                  chnlRate        - desired chnlRate for interface in
**                                    Mbps.  Legal range 1 to 12000
**                  *pMinEntries    - pointer to storage for minimum number
**                                    of interface scheduler entries required
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbLineSchdChnlRateScaleCalc(struct file  *fileHndl, UINT4 dir,
                                             UINT4 availBw, UINT4 numRows,
                                             UINT4 dataType, UINT4 pktSize,
                                             UINT4 chnlRate,
                                             UINT4 *pMinEntries)
{
    /* Variable Declaration */
    UINT4 qdrSize, macUtil, qdrWords ,inclLengthBurst ,rdOSval ,wrOSval;
    UINT4 macWrOSval ,macRdOSval, entryBw, ceilingNum;
    UINT4 qdrSizeScaled, qdrSizeMult, adjChnlRateScaled, adjChnlRateMult;
    UINT4 oSval, inclLengthBurstMult, pktSizeScaled, pktSizeMult;
    UINT4 oSvalMult, macRdOSvalMult, macWrOSvalMult;
    UINT4 inclLengthBurst12, inclLengthBurst12Mult;
    UINT4 remainder;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
    DBG_PRINT("Inputs availBw = %u, numRows = %u, dataType = %u, chnlRate = %u\n",
              (unsigned int)availBw, (unsigned int)numRows, (unsigned int)dataType, (unsigned int)chnlRate);
    DBG_PRINT("Inputs dir = %u, pktSize = %u\n",
              (unsigned int)dir, (unsigned int)pktSize);

    /* Argument checking */
    if((dir > 1)||(dataType > 1)||(chnlRate < 1)||(chnlRate > 12000)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* use worst case packet size for MAC Terminated clients */
    if (dataType == 0) {
        if (dir == 0) {
            pktSize = 177;
        } else {
            pktSize = 69;
        }
    }

    /***************************************************************************
    ** Calculate variables in the function                                    **
    ***************************************************************************/
    /* Entry Bandwidth */
    if(numRows == 0){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
    }
    entryBw = availBw/(numRows * HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW);
    DBG_PRINT("entryBw = %u\n", (unsigned int)entryBw);

    /* QDR Words, which is the minimum number of qdr words required,
       12 bytes/qdr word */
    qdrWords = ((pktSize + 4)%12 != 0 ? 1:0 ) + (pktSize + 4)/12;
    DBG_PRINT("qdrWords = %u\n", (unsigned int)qdrWords);

    /* QDR Size, that is the number of bytes in qdrWords*/
    qdrSize = 12 * qdrWords;
    DBG_PRINT("qdrSize = %u\n", (unsigned int)qdrSize);
    result = cpbHyPhy20gScaleNumToLargest32BitNum(fileHndl, qdrSize, &qdrSizeScaled, &qdrSizeMult);
    if(result) return result;
    DBG_PRINT("qdrSizeScaled = %u, qdrSizeMult = %u\n", (unsigned int)qdrSizeScaled, (unsigned int)qdrSizeMult);

    /* MAC Utilization, macUtil mac utilization multiplied by pktSizeMult */
    if(pktSize == 0){ /* test to ensure divide by 0 does not occur */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
    }
    result = cpbHyPhy20gScaleNumToLargest32BitNum(fileHndl, pktSize, &pktSizeScaled, &pktSizeMult);
    if(result) return result;
    result = cpbHyPhy20gRoundedDivide32BitNum(fileHndl, pktSizeScaled, (pktSize + 5 + 7 + 1), &macUtil); /* rounded division */
    if(result) return result;
    DBG_PRINT("pktSize = %u, pktSizeScaled = %u, pktSizeMult = %u, macUtil = %u\n", (unsigned int)pktSize, (unsigned int)pktSizeScaled, (unsigned int)pktSizeMult, (unsigned int)macUtil);

    /* inclkLengthBurst Incl Length Burst times inclLengthBurstMult */
    if (qdrWords < 6) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
    } else if (qdrWords == 6){ /* inclLengthBurst = qdrWords + 1; */
        inclLengthBurst = qdrWords + 1;
        inclLengthBurstMult = 1;
        DBG_PRINT("inclLengthBurst = %u, inclLengthBurstMult = %u\n", (unsigned int)inclLengthBurst, (unsigned int)inclLengthBurstMult);
    } else if (qdrWords < 17) {
                         /* inclLengthBurst = qdrWords + (1 + 3*(qdrWords-6)/11); */
        /* inclLengthBurst = qdrWords+4*(0.25 + 0.75*(qdrWords-6)/11); */
        /* values
           qdrWords inclLengthBurst
           7         8.27272727
           8         9.54545454
           9        10.81818181
           10       12.09090909
           11       13.36363636
           12       14.63636363
           13       15.90909090
           14       17.18181818
           15       18.45454545
           16       19.72727272
        */
        inclLengthBurst = 700000000 + 127272727*(qdrWords - 6);
        inclLengthBurstMult = 100000000;
        DBG_PRINT("inclLengthBurst = %u, inclLengthBurstMult = %u\n", (unsigned int)inclLengthBurst, (unsigned int)inclLengthBurstMult);
    } else { /* inclLengthBurst = qdrWords + 4; */
        inclLengthBurst = qdrWords + 4;
        inclLengthBurstMult = 1;
        DBG_PRINT("inclLengthBurst = %u, inclLengthBurstMult = %u\n", (unsigned int)inclLengthBurst, (unsigned int)inclLengthBurstMult);
    }

    /* Read Over Speed, rdOSval rd overspeed value multiplied by qdrSizeMult */
    if(pktSize == 0){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
    }
    result = cpbHyPhy20gRoundedDivide32BitNum(fileHndl, qdrSizeScaled, pktSize, &rdOSval); /* rounded division */
    if(result) return result;
    DBG_PRINT("qdrSizeMult = %u, rdOSval = %u\n", (unsigned int)qdrSizeMult, (unsigned int)rdOSval);

    /* calculate inclLengthBurst * 12 with multiple inclLengthBurstMult */
    result = cpbHyPhy20gScaleMultToLargest32BitNum(fileHndl, 12, inclLengthBurst, inclLengthBurstMult, &inclLengthBurst12, &inclLengthBurst12Mult);
    if(result) return result;
    DBG_PRINT("inclLengthBurst12 = %u, inclLengthBurst12Mult = %u\n", (unsigned int)inclLengthBurst12, (unsigned int)inclLengthBurst12Mult);

    /* increase inclLengthBurst12 in anticipation for being used as the numerator in division */
    if (inclLengthBurstMult == 1  && inclLengthBurst12Mult == 1) {
        DBG_PRINT("inclLengthBurst12 = %u, inclLengthBurst12Mult = %u\n", (unsigned int)inclLengthBurst12, (unsigned int)inclLengthBurst12Mult);
        result = cpbHyPhy20gScaleNumToLargest32BitNum(fileHndl, inclLengthBurst12, &inclLengthBurst12, &inclLengthBurst12Mult);
        if(result) return result;
        DBG_PRINT("inclLengthBurst12 = %u, inclLengthBurst12Mult = %u\n", (unsigned int)inclLengthBurst12, (unsigned int)inclLengthBurst12Mult);
    }

    /* Write Over Speed, wrOSval wr overspeed value multiplied by inclLengthBurstMult */
    result = cpbHyPhy20gRoundedDivide32BitNum(fileHndl, inclLengthBurst12, pktSize, &wrOSval); /* rounded division */
    if(result) return result;
    DBG_PRINT("wrOSval = %u, inclLengthBurst12Mult = %u\n", (unsigned int)wrOSval, (unsigned int)inclLengthBurst12Mult);

    /* MAC Read Over Speed, macRdOSval Mac Read Over Speed percent multiplied by macRdOSvalMult */
    macRdOSval = rdOSval * macUtil;
    result = cpbHyPhy20gScaleMultTwoToLargest32BitNum(fileHndl, rdOSval, qdrSizeMult, macUtil, pktSizeMult, &macRdOSval, &macRdOSvalMult);
    if (result) return result;
    DBG_PRINT("macRdOSval = %u macRdOSvalMult = %u\n", (unsigned int)macRdOSval, (unsigned int)macRdOSvalMult);

    /* Mac Write Over Speed percent, 100 timer larger than the actual value */
    result = cpbHyPhy20gScaleMultTwoToLargest32BitNum(fileHndl, wrOSval, inclLengthBurst12Mult, macUtil, pktSizeMult, &macWrOSval, &macWrOSvalMult);
    if (result) return result;
    DBG_PRINT("macWrOSval = %u, macWrOSvalMult = %u\n", (unsigned int)macWrOSval, (unsigned int)macWrOSvalMult);


    /***************************************************************************
    ** Determine pMinEntries value with different parameters                  **
    ***************************************************************************/
    /* CBR Write use wrOSval and inclLengthBurst12Mult */
    /* MAC REad use macWrOSval and macWrOSvalMult    */
    /* CBR Read use rdOSval and qdrSizeMult          */
    /* MAC Read use macRdOSval and macRdOSvalMult    */
    if(entryBw == 0){ /* test to prevent divide by 0 error */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
    }

    if( dir == 0 && dataType == 1 ) { /* CBR Write interface */
        oSval = wrOSval;
        oSvalMult = inclLengthBurst12Mult;
        DBG_PRINT("CBR Write Inteface oSval = %u, oSvalMult = %u\n", (unsigned int)oSval, (unsigned int)oSvalMult);
    } else if ( dir == 0 && dataType == 0) { /* MAC Write Interface */
        oSval = macWrOSval;
        oSvalMult = macWrOSvalMult;
        DBG_PRINT("MAC Write Inteface oSval = %u, oSvalMult = %u\n", (unsigned int)oSval, (unsigned int)oSvalMult);
    } else if ( dir == 1 && dataType == 1) { /* CBR Read Interface */
        oSval = rdOSval;
        oSvalMult = qdrSizeMult;
        DBG_PRINT("CBR Read Inteface oSval = %u, oSvalMult = %u\n", (unsigned int)oSval, (unsigned int)oSvalMult);
    } else if ( dir == 1 && dataType == 0) { /* MAC Read Interface */
        oSval = macRdOSval;
        oSvalMult = macRdOSvalMult;
        DBG_PRINT("MAC Read Inteface oSval = %u, oSvalMult = %u\n", (unsigned int)oSval, (unsigned int)oSvalMult);
    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = cpbHyPhy20gScaleMultToLargest32BitNum(fileHndl, chnlRate, oSval, oSvalMult, &adjChnlRateScaled, &adjChnlRateMult);
    if(result) return result;
    DBG_PRINT("adjChnlRateScaled = %u, adjChnlRateMult = %u\n", (unsigned int)adjChnlRateScaled, (unsigned int)adjChnlRateMult);

    ceilingNum = (adjChnlRateScaled / entryBw) + (adjChnlRateScaled % entryBw != 0 ? 1:0);
    DBG_PRINT("ceilingNum = %u\n", (unsigned int)ceilingNum);
    /* provide 5% margin on the bandwidth utilization of the final
       calendar entry */
    remainder = ceilingNum % adjChnlRateMult;
    DBG_PRINT("remainder = %u\n", (unsigned int)remainder);
    ceilingNum = (ceilingNum / adjChnlRateMult) + (ceilingNum % adjChnlRateMult != 0 ? 1:0);
    DBG_PRINT("ceilingNum = %u\n", (unsigned int)ceilingNum);


    /* In anticipation of determining the % occupancy of the partially utilized
       SCBS calendar entry, scale numbers so comparison can be made
       where:
         0xFFFFFFFF/100 > 42949672
         0xFFFFFFFF/95 > 45210182
    */
    while (remainder > 42949672 || adjChnlRateMult > 45210182) {
        remainder /= 10;
        adjChnlRateMult /= 10;
    } /* end while */
    DBG_PRINT("remainder = %u, adjChnlRateMult = %u\n", (unsigned int)remainder, (unsigned int)adjChnlRateMult);

    /* if the required bandwidth consumes 95% of the final SCBS entry 
       assign an additional calendar entry */
    if ((remainder * 100) > (adjChnlRateMult * 95)) {
        ceilingNum++;
    }

    /***************************************************************************
    ** Pass the value to the pointer                                          **
    ***************************************************************************/
	*pMinEntries = ceilingNum;
	DBG_PRINT("%s, %s, %d, *pMinEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*pMinEntries);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbLineSchdChnlRateScaleCalc */

/*******************************************************************************
**
**  cpbHyPhy20gCpbLineSchdChnlRateScale
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function takes in the desired Mbps throughput of an
**                  LSCI/HSCI channel and scales it to accomodate QDR interface
**                  efficiency for the traffic type and CPB packet size.  It
**                  generates the minimum number of interface scheduler entries
**                  required to meet the requested chnlRate bandwidth.
**
**
**  INPUTS:         fileHndl        - device handle
**                  dir             - Entry values generated relative to:
**                                    0 - Write interfaces
**                                    1 - Read interfaces
**                  chnlNum         - 0 - 15 LSCI interface
**                                    16 - 17 HSCI interface
**                  chnlRate        - desired chnlRate for interface in
**                                    Mbps.  Legal range 1 to 12000
**                  availBw         - Raw QDR Bandwidth available to interface
**                                    scheduler.
**                  numRows         - Number of SCBS rows in interface
**                                    scheduler
**                  *pMinEntries    - Minimum interface group scheduler entries
**                                    to guarantee bandwidth requested by
**                                    chnlRate
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbLineSchdChnlRateScale(struct file  *fileHndl, UINT4 dir,
                                         UINT4 chnlNum, UINT4 chnlRate,
                                         UINT4 availBw, UINT4 numRows,
                                         UINT4 *pMinEntries)
{
    UINT4 cpbDataType, monPktSz, mkrPktSz, mkrPktSzVar, mkrPktPeriod;
    UINT4 pktSize, pMinEntriesCalc;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);


    DBG_PRINT("Inputs dir = %u, chnlNum = %u, chnlRate = %u, availBw = %u, numRows = %u\n",
              (unsigned int)dir, (unsigned int)chnlNum, (unsigned int)chnlRate, (unsigned int)availBw, (unsigned int)numRows);

    /* Argument checking */
    if((dir > 1)||(chnlNum > 17)||(chnlRate < 1)||(chnlRate > 12000)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlNum < 16){
        /* retrieve LSCI/HSCI CPB payload type (packet vs CBR) */
        result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, chnlNum, &cpbDataType);
        if(result) return result;
        /*  LSCI cpbDataType definition
            0 - Disable the LSCI slice
            1 - MAC Terminated (FE/GE, SGMII FE/GE)
            2 - GE GFP-T (Transparent GE)
            3 - FC 4G or lower rate (including FICON)
            4 - ESCON
            5 - Low-Speed Byte-aligned CBR (STS-48/12/3
                asynchronous transparent)
            6 - Other Low-Speed Byte-aligned CBR
            7 - Low-Speed 8b10b encoded CBR with TX_RD_INV_EN
            8 - Low-Speed 8b10b encoded CBR with TX_RD_INV_DIS
        */
        if((cpbDataType < 1)||(cpbDataType > LSCI_TRAFFIC_MAX)){
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        if(cpbDataType != 1){
            result = utilHyPhy20gSfpAcbGetPmmPktData(fileHndl, chnlNum, 8,
                                                     &monPktSz, &mkrPktSz,
                                                     &mkrPktSzVar, &mkrPktPeriod);
            if(result) return result;
            if (dir == 1){
                /* Read Direction (data to HSCI/LSCI) */
                pktSize = monPktSz;
            } else {
                /* Write Direction (data from HSCI/LSCI)*/
                pktSize = mkrPktSz;
            }
            if((pktSize == 0)||(pktSize > 511)){
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            /* scale packet size by 1.25 for 8b10b CBR applications */
            if((cpbDataType == 2)||(cpbDataType == 3)||(cpbDataType == 4)||
               (cpbDataType == 7)||(cpbDataType == 8)){
                pktSize = (pktSize*125)/100 + ((pktSize*125)%100 > 49 ? 1:0);
            }
        } else {
            pktSize = 69;
        }
        /* update cpbDataType for use by calculation API */
        cpbDataType = (cpbDataType==1)? 0:1;
    } else {
        result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, chnlNum-16, &cpbDataType);
        if(result) return result;
        /*  HSCI cpbDataType definition
            1 - 10GE MAC Terminated (including Gsup43
                Clause 7.1 and 7.2 10GE MAC terminated)
            2 - Gsup43 Clause 7.3
            3 - FC 8G and subrate
            4 - High Speed CBR 16/32 bit mode (including
                Gsup43 Clause 7.1 and 7.2 10GE transparent,
                STS-192 asynchronous transparent)
            5 - High Speed CBR 20/40 bit mode
        */
        /* if CBR retrieve the Maker Or Monitor Packet Size Data */
        if((cpbDataType < 1)||(cpbDataType > 5)){
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        if(cpbDataType != 1){
            result = utilHyPhy20gXfiAcbGetPmmPktData(fileHndl,
                                                     (chnlNum - 16 + 2),
                                                     &monPktSz, &mkrPktSz,
                                                     &mkrPktSzVar, &mkrPktPeriod);
            if(result) return result;
            if (dir == 1){
                /* Read Direction (data to HSCI/LSCI) */
                pktSize = monPktSz;
            } else {
                /* Write Direction (data from HSCI/LSCI)*/
                pktSize = mkrPktSz;
            }

            if((pktSize == 0)||(pktSize > 511)){
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            /* scale packet size by 1.25 for 8b10b CBR applications */
            if(cpbDataType == 3 || cpbDataType == 5){
                pktSize = (pktSize*125)/100 + ((pktSize*125)%100 > 49 ? 1:0);
            }
        } else {
            pktSize = 69;
        }
        /* update cpbDataType for use by calculation API */
        cpbDataType = (cpbDataType==1)? 0:1;
    }

    /* call sub routine to get the speed adjusted bandwidth */
    result = cpbHyPhy20gCpbLineSchdChnlRateScaleCalc(fileHndl, dir, availBw,
                                                     numRows, cpbDataType,
                                                     pktSize, chnlRate,
                                                     &pMinEntriesCalc);
    if(result){
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /* pass the value to the pointer */
    *pMinEntries = pMinEntriesCalc;
    DBG_PRINT("%s, %s, %d, *pMinEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*pMinEntries);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbLineSchdChnlRateScale */

/*******************************************************************************
**
**  cpbHyPhy20gCpbFifoAddBlk
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Adds a block to the specified FIFO and update FIFO's SW
**                  context.
**
**  INPUTS:         fileHndl    - device handle
**                  fifoId      - the FIFO to add a block to
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_CHNL_FIFO
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_REQ_BUSY
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_ADD_BLOCK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbFifoAddBlk(struct file *fileHndl, UINT4 fifoId)
{
    sHYPHY20G_CTXT          *pDevCtxt;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_CPB_RAM_CTXT  *pRAM;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;

    UINT2 wrChnlId, primRdChnlId;
    UINT2 ramNumFreeBlks, ramFirstFreeBlk, ramNewBlk;
    UINT2 fifoNumBlks, fifoFirstBlk;
    UINT4 reg, msk, val;
    INT4  result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (fifoId > (HYPHY20G_CPB_NUM_FIFO - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context and CPB context are valid */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pCpbCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    pRAM     = &(pDevCtxt->pCpbCtxt->ram);
    pFifo    = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);

    /* Retrieve RAM and FIFO info from SW context */
    ramNumFreeBlks  = pRAM->numFreeBlk;
    ramFirstFreeBlk = pRAM->firstFreeBlk;
    ramNewBlk       = pRAM->blockPtr[ramFirstFreeBlk];
    fifoNumBlks     = pFifo->numBlks;
    fifoFirstBlk    = pFifo->firstBlk;

    if (ramNumFreeBlks < 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_CHNL_FIFO\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_CHNL_FIFO;
    }

    /* Retrieve primary read channel from SW context */
    wrChnlId = fifoId;
    pWrChnl  = (sHYPHY20G_WR_CHNL_CTXT *)
                 (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    if (pWrChnl->rdChnlCnt < 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    primRdChnlId = (pWrChnl->pRdChnlList)[0];


    /* Check DELBLK_REQ_BUSY bit is cleared before attempting to add block */
    reg = HYPHY20G_CPB_CPB_REG_DEL_BLK;
    msk = HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_REQ_BUSY_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;
    if (val != 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_REQ_BUSY\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_REQ_BUSY;
    }

    /* Check ADDBLK_REQ_BUSY bit is cleared before attempting to add block */
    reg = HYPHY20G_CPB_CPB_REG_ADD_BLK;
    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_REQ_BUSY_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;
    if (val != 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_REQ_BUSY\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_REQ_BUSY;
    }

    /* Add a block */
    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if (result) return result;

    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_BLOCK_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, ramFirstFreeBlk);
    if (result) return result;

    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_RD_CH_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, primRdChnlId);
    if (result) return result;

    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_WR_CH_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, wrChnlId);
    if (result) return result;

    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_REQ_BUSY_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if (result) return result;

    /* Poll ADDBLK_REQ_BUSY */
    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_REQ_BUSY_MSK;
    result = sysHyPhy20gPollBit(fileHndl, reg, msk, 0, 1000);
    if (result) return result;

    /* Check result of block addition */
    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_RESULT_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;

    if ((val & 0x2) != 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_ADD_BLOCK\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_ADD_BLOCK;
    }

DBG_PRINT("DEBUG_JC_1a: fifoFirstBlk = %u, fifoNumBlks = %u\n", pFifo->firstBlk, pFifo->numBlks);
DBG_PRINT("DEBUG_JC_1b: ramFirstFreeBlk = %u, ramNumFreeBlk = %u\n", pRAM->firstFreeBlk, pRAM->numFreeBlk);

    /* Update SW context */

    /* Update FIFO block list context */
    pFifo->numBlks = fifoNumBlks+1;

    /* Update RAM block list context */
    pRAM->firstFreeBlk = ramNewBlk;
    pRAM->numFreeBlk = ramNumFreeBlks-1;

DBG_PRINT("DEBUG_JC_2a: fifoFirstBlk = %u, fifoNumBlks = %u\n", pFifo->firstBlk, pFifo->numBlks);
DBG_PRINT("DEBUG_JC_2b: ramFirstFreeBlk = %u, ramNumFreeBlk = %u\n", pRAM->firstFreeBlk, pRAM->numFreeBlk);

    /* Synchronize SW context block link list with HW for the specified FIFO */
    result = cpbHyPhy20gCpbSyncSWRamBlkLinkList(fileHndl, fifoId);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbFifoAddBlk */

/*******************************************************************************
**
**  cpbHyPhy20gCpbFifoDelBlk
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Delete a block from the specified FIFO and update FIFO's SW
**                  context.
**
**  INPUTS:         fileHndl    - device handle
**                  fifoId      - the FIFO to remove a block from
**                                Legal range 0 to 293
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_CHNL_FIFO
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_REQ_BUSY
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_DEL_BLOCK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbFifoDelBlk(struct file *fileHndl, UINT4 fifoId)
{
    sHYPHY20G_CTXT          *pDevCtxt;
    sHYPHY20G_CPB_CTXT      *pCpbCtxt;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_CPB_RAM_CTXT  *pRAM;
    sHYPHY20G_WR_CHNL_CTXT  *pWrChnl;

    UINT2 wrChnlId;
    UINT2 primRdChnlId;
    UINT2 ramNumFreeBlks;
    UINT2 fifoNumBlks;
    UINT4 reg, msk, val, delBlk;
    INT4  result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (fifoId > (HYPHY20G_CPB_NUM_FIFO - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context and CPB context are valid */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pCpbCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;
    pRAM     = &(pDevCtxt->pCpbCtxt->ram);
    pFifo    = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);

    /* Retrieve RAM and FIFO info from SW context */
    ramNumFreeBlks  = pRAM->numFreeBlk;
    fifoNumBlks     = pFifo->numBlks;
    if (fifoNumBlks < 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_CHNL_FIFO\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_CHNL_FIFO;
    }

    /* Retrieve primary read channel from SW context */
    wrChnlId = fifoId;
    pWrChnl  = (sHYPHY20G_WR_CHNL_CTXT *)
                 (pDevCtxt->pCpbCtxt->pWrChnl + wrChnlId);
    if (pWrChnl->rdChnlCnt < 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    primRdChnlId = (pWrChnl->pRdChnlList)[0];

    /* Check ADDBLK_REQ_BUSY bit is cleared before attempting to delete block */
    reg = HYPHY20G_CPB_CPB_REG_ADD_BLK;
    msk = HYPHY20G_CPB_CPB_BIT_ADD_BLK_ADDBLK_REQ_BUSY_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;
    if (val != 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_REQ_BUSY\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_REQ_BUSY;
    }

    /* Check DELBLK_REQ_BUSY bit is cleared before attempting to delete block */
    reg = HYPHY20G_CPB_CPB_REG_DEL_BLK;
    msk = HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_REQ_BUSY_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;
    if (val != 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_REQ_BUSY\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_REQ_BUSY;
    }

    /* Delete a block */
    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if (result) return result;

    msk = HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_RD_CH_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, primRdChnlId);
    if (result) return result;

    msk = HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_WR_CH_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, wrChnlId);
    if (result) return result;

    msk = HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_REQ_BUSY_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if (result) return result;

    /* Poll DELBLK_REQ_BUSY */
    msk = HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_REQ_BUSY_MSK;
    result = sysHyPhy20gPollBit(fileHndl, reg, msk, 0, 1000);
    if (result) return result;

    /* Check result of block deletion */
    msk = HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_RESULT_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;

    if ((val & 0x4) != 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEL_BLOCK\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEL_BLOCK;
    }

    /* Retrieve deleted block number */
    result = hyPhy20gRegFieldRead(fileHndl, reg,
               HYPHY20G_CPB_CPB_BIT_DEL_BLK_DELBLK_BLOCK_MSK, &delBlk);
    if (result) return result;

DBG_PRINT("DEBUG_JC_1a: fifoFirstBlk = %u, fifoNumBlks = %u\n", pFifo->firstBlk, pFifo->numBlks);
DBG_PRINT("DEBUG_JC_1b: ramFirstFreeBlk = %u, ramNumFreeBlk = %u\n", pRAM->firstFreeBlk, pRAM->numFreeBlk);

    /* Update SW context */

    /* Update FIFO block list context */
    if (delBlk == pFifo->firstBlk) {
        pFifo->firstBlk = pRAM->blockPtr[delBlk];
    }
    pFifo->numBlks = fifoNumBlks-1;

    /* Add deleted block to beginning of free block list */
    pRAM->blockPtr[delBlk] = pRAM->firstFreeBlk;
    pRAM->firstFreeBlk     = (UINT2)delBlk;
    pRAM->numFreeBlk       = ramNumFreeBlks+1;

DBG_PRINT("DEBUG_JC_2a: fifoFirstBlk = %u, fifoNumBlks = %u\n", pFifo->firstBlk, pFifo->numBlks);
DBG_PRINT("DEBUG_JC_2b: ramFirstFreeBlk = %u, ramNumFreeBlk = %u\n", pRAM->firstFreeBlk, pRAM->numFreeBlk);

    /* Synchronize SW context block link list with HW for the specified FIFO */
    result = cpbHyPhy20gCpbSyncSWRamBlkLinkList(fileHndl, fifoId);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbFifoDelBlk */

/*******************************************************************************
**
**  cpbHyPhy20gCpbSyncSWRamBlkLinkList
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Synchronize SW RAM context to HW block link list.  This is
**                  useful after performing dynamic fifo resizing.
**
**  INPUTS:         fileHndl    - device handle
**                  fifoId      - fifo ID
**                                Legal range 0 to 1023
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbSyncSWRamBlkLinkList(struct file *fileHndl, UINT4 fifoId)
{
    sHYPHY20G_CTXT          *pDevCtxt;
    sHYPHY20G_CPB_CTXT      *pCpbCtxt;
    sHYPHY20G_CPB_FIFO_CTXT *pFifo;
    sHYPHY20G_CPB_RAM_CTXT  *pRAM;

    UINT2 fifoNumBlks, fifoFirstBlk, blk;
    UINT4 reg, nxtBlk, i;
    INT4  result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (fifoId > (HYPHY20G_CPB_NUM_FIFO - 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context and CPB context are valid */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pCpbCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;
    pRAM     = &(pDevCtxt->pCpbCtxt->ram);
    pFifo    = (sHYPHY20G_CPB_FIFO_CTXT *)(pDevCtxt->pCpbCtxt->pFifo + fifoId);

    fifoNumBlks  = pFifo->numBlks;
    fifoFirstBlk = pFifo->firstBlk;


/* DEBUG PRINT */
    DBG_PRINT("DEBUG: pRAM PRINT BEFORE\n");
    for (i = 0; i < 25; i++) {
        DBG_PRINT("DEBUG: pRAM->blockPtr[%u] = %u\n", (unsigned int)i, pRAM->blockPtr[i]);
    }

    blk = fifoFirstBlk;
    nxtBlk = pRAM->blockPtr[blk];
    DBG_PRINT("DEBUG_JC: fifoFirstBlk = %u, fifoNumBlks = %u\n", fifoFirstBlk, fifoNumBlks);
    DBG_PRINT("DEBUG_JC: SW context BEFORE synchronization\n");
    for (i = 0; i < fifoNumBlks; i++) {
        DBG_PRINT("DEBUG: pRAM->blockPtr[%u] = %lu\n", blk, nxtBlk);

        blk = (UINT2)nxtBlk;
        nxtBlk = pRAM->blockPtr[blk];
    }
/* END DEBUG PRINT */


    blk = fifoFirstBlk;
    reg = HYPHY20G_CPB_CPB_REG_BLK_LNK_LIST_OFFSET(blk);
    for (i = 0; i < fifoNumBlks; i++) {
        result = hyPhy20gRegFieldRead(fileHndl, reg,
                   HYPHY20G_CPB_CPB_BIT_BLK_LNK_LIST_NXT_BLK_MSK, &nxtBlk);
        if (result) return result;

        pRAM->blockPtr[blk] = (UINT2)nxtBlk;

        blk = (UINT2)nxtBlk;
        reg = HYPHY20G_CPB_CPB_REG_BLK_LNK_LIST_OFFSET(blk);
    }


/* DEBUG PRINT */
    DBG_PRINT("DEBUG: pRAM PRINT AFTER\n");
    for (i = 0; i < 25; i++) {
        DBG_PRINT("DEBUG: pRAM->blockPtr[%u] = %u\n", (unsigned int)i, pRAM->blockPtr[i]);
    }

    blk = fifoFirstBlk;
    nxtBlk = pRAM->blockPtr[blk];
    DBG_PRINT("DEBUG_JC: SW context AFTER synchronization\n");
    for (i = 0; i < fifoNumBlks; i++) {
        DBG_PRINT("DEBUG: pRAM->blockPtr[%u] = %lu\n", blk, nxtBlk);

        blk = (UINT2)nxtBlk;
        nxtBlk = pRAM->blockPtr[blk];
    }
/* END DEBUG PRINT */


    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbSyncSWRamBlkLinkList */

/*******************************************************************************
**
**  cpbHyPhy20gCpbCrc32TableInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes the CRC32 table.
**
**  INPUTS:         fileHndl        - device handle
**                  crc32Table[256] - CRC32 table
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbCrc32TableInit(struct file *fileHndl, UINT4 crc32Table[256])
{
    UINT4 i, j;
    UINT4 iRemainder, temp1, temp2;
    UINT4 poly;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    poly = 0x04C11DB7;

    for (i = 0; i < 256; i++) {

        temp1 = i;
        temp2 = 0;

        for (j = 8; j > 0; j--) {
            if ((temp1 & 0x1) == 0x1) {
                temp2 = (temp2 | (0x1 << (j-1)));
            }
            temp1 = temp1 >> 1;
        }

        /* initialize the remainder */
        iRemainder = temp2 << 24;

        /* Long division */
        for (j = 8; j > 0; j--) {
            if ((iRemainder & 0x80000000) == 0x80000000) {
                iRemainder = iRemainder << 1;
                iRemainder = iRemainder ^ poly;
            } else {
                iRemainder = iRemainder << 1;
            }
        }

        /* reflect remainder */
        temp1 = 0;

        for (j = 32; j > 0; j--) {
            if ((iRemainder & 0x1) == 0x1) {
                temp1 = temp1 | (0x1 << (j-1));
            }
            iRemainder = iRemainder >> 1;
        }


        crc32Table[i] = temp1;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbCrc32TableInit */

/*******************************************************************************
**
**  cpbHyPhy20gCpbCalcPauseFrmCrc32
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculates the CRC-32 for a pause frame.
**                  cpbHyPhy20gCpbCrc32TableInit must be called before this
**                  function.
**
**  INPUTS:         fileHndl        - device handle
**                  gfpSABase[2]    - The common component of source address
**                                    (SA) inserted in Far End pause frames.
**                                    If MAC address is aa:bb:cc:dd:ee:ff
**                                      gfpSABase[1] = aa:bb
**                                      gfpSABase[0] = cc:dd:ee:ff
**                  crc32Table[256] - CRC32 table
**                  *pCrc32         - Pointer to storage for calculated CRC-32
**                                    value.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbCalcPauseFrmCrc32(struct file *fileHndl,
                                     UINT4 gfpSABase[2], UINT4 crc32Table[256],
                                     UINT4 *pCrc32)
{
    UINT1 *pByteArr;
    UINT4 iCrc32, iInvCrc32;
    UINT4 iTableIndex;
    UINT4 i;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (gfpSABase[0] > 0xFFFFFFFF || gfpSABase[1] > 0xFFFF)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* allocate memory */
    pByteArr = (UINT1 *)sysHyPhy20gMemAlloc(sizeof(UINT1) * 60);
    if (pByteArr == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* destination mac address:
       global multicast address = 0x0180C2000001 */
    pByteArr[0] = 0x01;
    pByteArr[1] = 0x80;
    pByteArr[2] = 0xC2;
    pByteArr[3] = 0x00;
    pByteArr[4] = 0x00;
    pByteArr[5] = 0x01;

    /* source mac address: gfpSABase */
    pByteArr[6]  = (gfpSABase[1] >> 8)  & 0xFF;
    pByteArr[7]  = (gfpSABase[1] >> 0)  & 0xFF;
    pByteArr[8]  = (gfpSABase[0] >> 24) & 0xFF;
    pByteArr[9]  = (gfpSABase[0] >> 16) & 0xFF;
    pByteArr[10] = (gfpSABase[0] >> 8)  & 0xFF;
    pByteArr[11] = (gfpSABase[0] >> 0)  & 0xFF;

    /* length/type = 0x8808 */
    pByteArr[12] = 0x88;
    pByteArr[13] = 0x08;

    /* mac control opcode:
       pause = 0x0001 */
    pByteArr[14] = 0x00;
    pByteArr[15] = 0x01;

    /* mac control parameter:
       pause duration = 0xFFFF */
    pByteArr[16] = 0xFF;
    pByteArr[17] = 0xFF;

    /* zeroes */
    for (i = 18; i < 60; i++) {
        pByteArr[i] = 0;
    }

    iCrc32 = 0xFFFFFFFF;

    for (i = 0; i < 60; i++) {
        iTableIndex = (iCrc32  ^ pByteArr[i]) & 0xFF;
        iCrc32 = ((iCrc32 >> 8) & 0xFFFFFF) ^ crc32Table[iTableIndex];
    }

    iInvCrc32 = ~iCrc32;

    *pCrc32 = (((iInvCrc32 >> 0) & 0xFF)  << 24) |
              (((iInvCrc32 >> 8) & 0xFF)  << 16) |
              (((iInvCrc32 >> 16) & 0xFF) << 8)  |
              (((iInvCrc32 >> 24) & 0xFF) << 0);

    /* free memory */
    sysHyPhy20gMemFree(pByteArr);
    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbCalcPauseFrmCrc32 */

/*******************************************************************************
**
**  cpbHyPhy20gCpbZone3DiscardCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Enable or disable the ZONE3 packet discard behavior on the
**                  write client port, specificed by the chnlId.
**
**
**  INPUTS:         fileHndl    - device handle
**                  chnlId      - The channel this client port writer belongs to
**                                Legal range 0 to 293
**                 zone3Discard - 0 - Packet received when channel FIFO in
**                                    ZONE3 will be discarded.
**                                1 - Packet discarding disabled.  CPB FIFO
**                                    overflows can potentially occur.
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbZone3DiscardCfg(struct file *fileHndl, UINT4 chnlId,
                                   UINT4 zone3Discard)
{
    UINT4 addr = 0;
    UINT4 wrOvrEnMsk = 0;
    INT4 result;

    /* argument checking */
    if ((chnlId > HYPHY20G_CPB_NUM_WR_PORT - 1) || (zone3Discard > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlId <= 15) {
        /* LOW SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(chnlId);
        wrOvrEnMsk = HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_WR_OVR_EN_MSK;

    } else if (chnlId >= 16 && chnlId <= 17) {
        /* HIGH SPEED PORT CONFIGURATION */
        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(chnlId - 16);
        wrOvrEnMsk = HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_WR_OVR_EN_MSK;

    } else if (chnlId == 18){
       /* MPIF port initialization */

    } else if ((chnlId >= 19 && chnlId <= 82) || (chnlId >= 83 && chnlId <= 146) ){
        /* GFP PORT CONFIGURATION */
        if (chnlId >= 19 && chnlId <= 82) {
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-19, 0);/* GFP A*/
        } else {
            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(chnlId-83, 1);/* GFP B*/
        }
        wrOvrEnMsk = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_OVR_EN_MSK;

    } else if (chnlId >= 147 && chnlId <= HYPHY20G_CPB_NUM_WR_PORT - 1) {
        /* Packet Interface Read Port Configuration */
        addr = HYPHY20G_CPB_CPB_REG_PAC_IF_WR_PRT_CFG_OFFSET(chnlId - 147);
        wrOvrEnMsk = HYPHY20G_CPB_CPB_BIT_PAC_IF_WR_PRT_CFG_PI_WR_OVR_EN_MSK;

    } else {
        /* should never run into this place */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (chnlId != 18) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, wrOvrEnMsk,
                                       zone3Discard);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbZone3DiscardCfg */

/*******************************************************************************
**
**  cpbHyPhy20gScaleNumToLargest32BitNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Multiplies a 32 bit integer by a multiple of 10 till the
**                  integer is the largest possible value that can be stored in
**                  a 32 bit value.
**
**
**  INPUTS:         fileHndl    - device handle
**                  num         - number to be multiplied by a multiple of 10
**                  *pVal       - pointer storage to multiple * num
**                  *pMult      - pointer storage to multiple
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gScaleNumToLargest32BitNum(struct file  *fileHndl, UINT4 num,
                                           UINT4 *pVal, UINT4 *pMult)
{
    /* multiplies a 32 bit integer by a multiple of 10 till the
       integer is the largest possible value that can be stored in a
       32 bit value */
    UINT4 mult;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("Inputs num = %u\n",
              (unsigned int)num);

    mult = 1000000000; /* 1G */

    while ( (0xFFFFFFFF/num) < mult ){
        DBG_PRINT("Inside while loop num = %u, mult = %u\n",
                  (unsigned int)num, (unsigned int)mult);
        mult /= 10;
        if (mult == 1) break;
    }

    *pVal = mult * num;
    *pMult = mult;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gScaleNumToLargest32BitNum */

/*******************************************************************************
**
**  cpbHyPhy20gScaleMultToLargest32BitNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Should the product of num and multNum exceed a 32 bit value
**                  this function provides the largest 32 bit number num and
**                  multNum can multiple to by modifying the base 10 value of
**                  the provided scaling. If we remove the original scaling and
**                  the multiplication still requires greater than a 32 bit
**                  number return an error.
**
**
**  INPUTS:         fileHndl    - device handle
**                  num         - number to be multiplied by a multiple of 10
**                  multNum     - multiple
**                  multNumMult -
**                  *pVal       - pointer storage to multiple * num
**                  *pMult      - pointer storage to multiple
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gScaleMultToLargest32BitNum(struct file  *fileHndl, UINT4 num,
                                            UINT4 multNum, UINT4 multNumMult,
                                            UINT4 *pVal, UINT4 *pMult)
{
    /* should the product of num and multNum exceed a 32 bit value this
       function
       provides the largest 32 bit number num and multNum can multiple to by
       modifying the base 10 value of the provided scaling
       If we remove the original scaling and the multiplication
       still requires greater than a 32 bit number return an
       error */

    UINT4 mult, divisor = 1, newMultNum;
    INT4 result;

    mult = multNumMult;
    newMultNum = multNum;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("Inputs num = %u, multNum = %u, multNumMult = %u\n",
              (unsigned int)num, (unsigned int)multNum, (unsigned int)multNumMult);

    while ( 0xFFFFFFFF/num < newMultNum ){
        DBG_PRINT("Inside while loop num = %u, newMultNum = %u\n",
                  (unsigned int)num, (unsigned int)newMultNum);
        if (mult == 1) return HYPHY20G_ERR_VAR_OVERFLOW;

        mult /= 10;
        divisor *= 10;
        result = cpbHyPhy20gRoundedDivide32BitNum (fileHndl, multNum,
                                                   divisor, &newMultNum);
        if(result) return result;
        if (mult == 1) break;
    }

    if (0xFFFFFFFF/num < newMultNum) return HYPHY20G_ERR_VAR_OVERFLOW;

    *pVal = newMultNum * num;
    *pMult = mult;
    DBG_PRINT("Outputs *pVal = %u, *pMult = %u\n", (unsigned int)*pVal, (unsigned int)*pMult);

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gScaleMultToLargest32BitNum */

/*******************************************************************************
**
**  cpbHyPhy20gScaleMultTwoToLargest32BitNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Should the product of num0 and num1 with multNum1 exceed a
**                  32 bit value this function provides the largest 32 bit
**                  number num0 and num1 can multiple to by
**                  modifying the base 10 value of the provided scaling
**                  If we remove the original scaling and the multiplication
**                  still requires greater than a 32 bit number return an
**                  error upon completion of this operation, it ensures that
**                  the scaling multiplier will not have overflowed.
**
**  INPUTS:         fileHndl     - device handle
**                  num0         - number to be multiplied by a multiple of 10
**                  multNum0     - multiple
**                  num1         -
**                  multNum1     -
**                  *pVal        - pointer storage to multiple * num
**                  *pMult       - pointer storage to multiple
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gScaleMultTwoToLargest32BitNum(struct file  *fileHndl, UINT4 num0,
                                               UINT4 multNum0, UINT4 num1,
                                               UINT4 multNum1,
                                               UINT4 *pVal, UINT4 *pMult)
{
    /* should the product of num0 and num1 with multNum1 exceed a 32 bit value this
       function
       provides the largest 32 bit number num0 and num1 can multiple to by
       modifying the base 10 value of the provided scaling
       If we remove the original scaling and the multiplication
       still requires greater than a 32 bit number return an
       error

       upon completion of this operation, it ensures that the scaling
       multiplier will not have overflowed */

    UINT4 num0Num1, num0Num1Mult, newNum0Num1, divisor = 1;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("Inputs num0 = %u, multNum0 = %u, num1 = %u, multNum1 = %u\n",
              (unsigned int)num0, (unsigned int)multNum0, (unsigned int)num1, (unsigned int)multNum1);

    result = cpbHyPhy20gScaleMultToLargest32BitNum(fileHndl, num0, num1, multNum1, &num0Num1, &num0Num1Mult);
    if(result) return result;

    newNum0Num1 = num0Num1;

    /* need to ensure num0Num1Mult * multNum0 do not overflow 32 bits */
    while(0xffffffff/num0Num1Mult < multNum0){
        multNum0 /= 10;
        divisor *= 10;
        result = cpbHyPhy20gRoundedDivide32BitNum (fileHndl, num0Num1,
                                                   divisor, &newNum0Num1);
        if (result) return result;
        if (multNum0 == 1) break;
    }

    if (0xFFFFFFFF/num0Num1Mult < multNum0) return HYPHY20G_ERR_VAR_OVERFLOW;

    *pVal = newNum0Num1;
    *pMult = multNum0 * num0Num1Mult;
    DBG_PRINT("Outputs *pVal = %u, *pMult = %u\n", (unsigned int)*pVal, (unsigned int)*pMult);

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gScaleMultTwoToLargest32BitNum */

/*******************************************************************************
**
**  cpbHyPhy20gRoundedDivide32BitNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This divides numerator by denominator and rounds the result.
**
**  INPUTS:         fileHndl     - device handle
**                  numerator    - numerator
**                  denominator  - denominator
**                  *pVal        - pointer storage to numerator/denominator
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gRoundedDivide32BitNum(struct file  *fileHndl, UINT4 numerator,
                                       UINT4 denominator, UINT4 *pVal)
{
    /* this divides numerator by denominator and rounds the result */

    if (denominator == 0){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (denominator % 2 == 0 ) { /* even denominator */
        *pVal = numerator/denominator + (numerator%denominator >= denominator/2 ? 1:0);
    } else { /* odd denominator */
        *pVal = numerator/denominator + (numerator%denominator > denominator/2 ? 1:0);
    }

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gRoundedDivide32BitNum */

/*******************************************************************************
**
**  cpbHyPhy20gCeilingDivide32BitNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This divides numerator by denominator and rounds the result.
**
**  INPUTS:         fileHndl     - device handle
**                  numerator    - numerator
**                  denominator  - denominator
**                  *pVal        - pointer storage to numerator/denominator
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCeilingDivide32BitNum(struct file  *fileHndl, UINT4 numerator,
                                       UINT4 denominator, UINT4 *pVal)
{
    /* this divides numerator by denominator and rounds the result */

    if (denominator == 0){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    *pVal = numerator/denominator + (numerator%denominator != 0 ? 1:0);

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCeilingDivide32BitNum */

/*******************************************************************************
**
**  cpbHyPhy20gFloorDivide32BitNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This divides numerator by denominator and rounds the result.
**
**  INPUTS:         fileHndl     - device handle
**                  numerator    - numerator
**                  denominator  - denominator
**                  *pVal        - pointer storage to numerator/denominator
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gFloorDivide32BitNum(struct file  *fileHndl, UINT4 numerator,
                                     UINT4 denominator, UINT4 *pVal)
{
    /* this divides numerator by denominator and rounds the result */

    if (denominator == 0){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    *pVal = numerator/denominator;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gFloorDivide32BitNum */

/*******************************************************************************
**
**  cpbHyPhy20gCPBSchdRateAwareCreateIntermediate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function is used to generate the contents of an 
**                  intermediate scheduler that will allow Line Rd/Wr SCBS
**                  to have the number of rows used by the schduler change.
**
**                  The function will return scheduler contents that will 
**                  meet entry requirements for operational channels within
**                  the scheduler with either the start or final number of
**                  rows assigned to the scheduler.
**
**                  The returned scheduler data will span the largest number
**                  or rows provided by startRows and finalRows arguments.
**                  
**
**  INPUTS:         startRows     - Number of scheduler rows in initial scheduler
**                  *pStartData   - Pointer to the operational scheduler contents
**                  finalRows     - Number of scheduler rows in final scheduler
**                  *pFinalData   - Pointer to the data that is to be applied to
**                                  the scheduler.
**                  *pGoodData    - Pointer to data
**                                  that meets entry requirements for lengths
**                                  of startRows and endRows
**                  *pStatus      - Status of starting data to satisfy entry
**                                  requirements for both number of rows:
**                                  0 - Compatible.  The provided *pStartData
**                                      meets entry requirements for rows
**                                      spanned by startRows and finalRows
**                                  1 - Incompatible.  The provided *pStartData
**                                      fails entry requirements for rows
**                                      spanned by startRows and finalRows
**                                  Note functionality of *pStatus not 
**                                  implemented.
**                                
**                               
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBSchdRateAwareCreateIntermediate(UINT4 startRows,
                                                   UINT4 *pStartData, 
                                                   UINT4 finalRows, 
                                                   UINT4 *pFinalData, 
                                                   UINT4 *pGoodData,
                                                   UINT4 *pStatus)
{

    /* Following variables assume RD/WR have same channel ranges */
    UINT1 numLongEntry[HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL];
    UINT1 numShortEntry[HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL];
    UINT1 numExcessEntry[HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL];
    UINT1 opEntryState[HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL];  /* operational channel entry state */

    UINT4 offset, swapOffset;
    UINT4 *pLongRowData, *pShortRowData;
    UINT4 numShortRows, numLongRows;
    UINT4 longData, shortData, swapChnl;
    UINT4 i, j;
    INT4 result;

    /* confirm pointers passed in are valid before processing */
    HYPHY20G_ASSERT((pStartData != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    HYPHY20G_ASSERT((pFinalData != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    HYPHY20G_ASSERT((pGoodData != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* argument checking */
    if (startRows > HYPHY20G_CPB_LINE_WR_SCHD_MAX_NUM_ROW || 
        finalRows > HYPHY20G_CPB_LINE_WR_SCHD_MAX_NUM_ROW){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Assumptions in the implemenation of this function:
        - Rows will only shrink if adding a channel
        - Rows will only grow if removing a channel

       Because of the above, the longer scheduler will always
       contain the set of operational channels.

    */
    numShortRows = (finalRows>startRows) ? startRows:finalRows;
    numLongRows = (finalRows>startRows) ? finalRows:startRows;
    pLongRowData = (finalRows>startRows) ? pFinalData:pStartData;
    pShortRowData = (finalRows>startRows) ? pStartData:pFinalData;    

    /* initialize array data */
    for (i=0; i<HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL; i++){
        numShortEntry[i] = 0;
        numLongEntry[i] = 0;
        numExcessEntry[i] = 0;
    }

    /* determine number of instances of each channel in shorter calendar */
    for (i=0; i<numShortRows; i++){
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            longData = *(pLongRowData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
            shortData = *(pShortRowData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
            if (shortData < HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL) numShortEntry[shortData]++;
            if (longData < HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL) numLongEntry[longData]++;
        }
    }
    /* determine number or entries in the excess rows of longer calendar */
    for (i = numShortRows; i < numLongRows; i++){
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            longData = *(pLongRowData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
            if (longData < HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL) numExcessEntry[longData]++;
        }
    }

    /* transfer long calendar data to good calendar data */
    for (i = 0; i < numLongRows; i++){
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            offset = i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j; 
            *(pGoodData + offset) = *(pLongRowData + offset);
        }
    }

    /* per channel categorize the relationship of entries between long and short calendars */
    for (i=0; i<HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL; i++){
        if(numShortEntry[i] < numLongEntry[i]){
            /* channel is over subscribed */
            opEntryState[i] = HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OVER;
        } else if(numShortEntry[i] > numLongEntry[i]){
            /* if channel does exist we need to indicate it is
               undersubscribed provided there is an excess row 
               opportunity for it to be swapped with an over subscribed
               channel.  This occurs because the minEntries of the 
               schedulers being compared may be padded by the 
               code to generate the scheduler contents from minEntries.
               If there is not an opportunity to swap this indicates
               the number of entries in the smaller scheduler were
               padded.  As scheduler shrinking the QDR bandwidth per
               entry is greater, so if we use the same number of entries
               as larger scheduler we must still meet the minEntries for
               the channel as it's rate has not changed.  */            
            
            /* channel is over subscribed or channel does not
               exist in long scheduler */
            if(numExcessEntry[i] == 0){
                /* if channel does not exist or we do not have entries available
                   in excess rows we do not need to modify for it */
                opEntryState[i] = HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OKAY;
            } else if(numExcessEntry[i] + numLongEntry[i] < numShortEntry[i]){
                /* if we do not have enough entries to meet numShortEntry,
                   reduce numShortEntry so that we can pull in all numExcessEntry
                   for the channel to the short calendar range */
                numShortEntry[i] = numExcessEntry[i] + numLongEntry[i];
                opEntryState[i] = HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_UNDER;
                // we trick it by reducing the numShortEntry so the future code executes okay
            } else {
                opEntryState[i] = HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_UNDER;
            }
        } else {
            /* channel meets required number of entries in the smaller
               number of rows */
            opEntryState[i] = HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OKAY;
        }
    }

    for (i=0; i<HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL; i++){
        while(opEntryState[i] == HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_UNDER){
            /* confirm the undersubscribed channel has an available entry in the
               excess rows */
            if(numExcessEntry[i] == 0){
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_EVALUATING_SCHD\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_EVALUATING_SCHD;
            }
            /* identify an oversubscried channel within the short number of scheduler rows */
            swapChnl = 0xFF;
            for (j=0; j<HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL; j++){
                if(opEntryState[j]==HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OVER){
                    swapChnl = j;
                    j = HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL; /* force exit for loop */
                }
            }
            if(swapChnl == 0xFF){
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_EVALUATING_SCHD\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_EVALUATING_SCHD;
            }
            numLongEntry[swapChnl]--;
            numExcessEntry[swapChnl]++;
            if(numLongEntry[swapChnl] == numShortEntry[swapChnl]){
                opEntryState[swapChnl] = HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OKAY;
            }
            /* overwrite the undersubscribed channel number in excess rows */
            result = cpbHyPhy20gCPBSchdRateAwareGetCalDataAddr((numShortRows-1), (numLongRows-1), i, pGoodData, &swapOffset);
            if(result) return result;
            *(pGoodData + swapOffset) = swapChnl;

            /* overwrite the oversubscribed channel number in the short range of rows */
            result = cpbHyPhy20gCPBSchdRateAwareGetCalDataAddr(0, (numLongRows-1), swapChnl, pGoodData, &swapOffset);
            if(result) return result;            
            *(pGoodData + swapOffset) = i;

            numLongEntry[i]++;
            numExcessEntry[i]--;
            if(numLongEntry[i] == numShortEntry[i]){
                opEntryState[i] = HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OKAY;
            }            
        } /* end while (opEntryState... */
    } /* end for (i=0... */

    /* unconditionally force *pStatus to report new scheduler required.  Potential location to
       provide future code that will not update the device SCBS if there is no requirement to */
    *pStatus = 1;

    return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCPBSchdRateAwareCreateIntermediate */


/*******************************************************************************
**
**  cpbHyPhy20gCPBSchdRateAwareGetCalDataAddr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function searches the passed in array containing
**                  calendar data and overwrites the first instance of the
**                  specified value with a new value.
**
**  DESCRIPTION:    This function searches the passed in array containing
**                  calendar data and provides the offset for the first 
**                  occurance of the specified channel number.  The search
**                  spans the range specified with firstRow and lastRow
**                  arguments.
**
**  INPUTS:         firstRow      - First row of passed in scheduler data to
**                                  be analyzed for the oldChnl number                                 
**                  lastRow       - Last row of scheduler to be analyzed, if
**                                  'oldChnl' not found an error returned
**                  chnl          - Channel number to be searched for
**                  *pCalData     - Pointer to calendar data to be searched
**                  *pOffset      - Pointer to offset of the first encountered
**                                  instance of chnl.
**                                
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCPBSchdRateAwareGetCalDataAddr(UINT4 firstRow, UINT4 lastRow,
                                            UINT4 chnl, UINT4 *pCalData,
                                            UINT4 *pOffset)
{
    UINT4 i, j, offset, inspectChnl, notSuccess = 1;

    DBG_PRINT("%s, %s, %d, firstRow = %u, lastRow = %u, chnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)firstRow, (unsigned int)lastRow, (unsigned int)chnl);        

    *pOffset = 0xFFFF;

    /* transfer long calendar data to good calendar data */
    for (i = firstRow; i < lastRow+1; i++){
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            offset = i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j; 
            inspectChnl = *(pCalData + offset);
            if(chnl == inspectChnl){
                DBG_PRINT("%s, %s, %d, FOUND chnl = %u, offset = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)chnl, (unsigned int)offset);        
                *pOffset = offset;
                j = HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW;
                i = lastRow+1;
                notSuccess = 0;
            }
        }
    }

    if(notSuccess){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_EVALUATING_SCHD\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_EVALUATING_SCHD;
    }
    return HYPHY20G_SUCCESS;
    
} /* cpbHyPhy20gCPBSchdRateAwareGetCalDataAddr */

/*******************************************************************************
**
**  cpbHyPhy20gCpbChnlDestinedToOtnmaChk
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks if the CPB channel to be enabled
**                  is destined to OTNMA. 
**
**                  If the CPB channel is destined to OTNMA, FIFO_CENTER_EN bit
**                  in OTNMA will be set as a function of primaryFlag.
**
**  INPUTS:         fileHndl     - device handle
**                  wrChnlId  - the write channel ID
**                              Legal range 0 to 293
**                  rdChnlId  - the read channel ID for channel enable
**                              Legal range 0 to 297
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbChnlDestinedToOtnmaChk(struct file *fileHndl, UINT4 wrChnlId, 
                                          UINT4 rdChnlId)
{ 
    /* variable declaration */
    UINT4 regAddr, off, mask;
    UINT4 otnMode, slice = 0;
    UINT4 otnmaReg[64] = {0}, sliceEn;
    UINT4 i, j, t;
    UINT4 cpbId;
    INT4 result;
    sHYPHY20G_CTXT          *pDevCtxt;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)
                    (pDevCtxt->pCpbCtxt->pRdChnl + rdChnlId);

    /* 1. Check if datapath is in 1 x ODU2 vs. 4 x ODU1 */
    if (rdChnlId > 17 && rdChnlId < 83) { 
        /* GFP A Slice */
        slice = 0;
    } else if (rdChnlId > 82 && rdChnlId < 147) {
        /* GFP B Slice */
        slice = 1;
    }
    
    DBG_PRINT("%s, %s, %d, Slice to be checked = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);
    
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    off = HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, off, &otnMode);
    if (result) return result;
        
    DBG_PRINT("%s, %s, %d, OTN Mode = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)otnMode);    
    
    if (otnMode == 0) { /* 4 x OTU1 mode */
        /* 2. Check SLICE_EN */
        /* check SLICE_EN */
        for (j = 0; j < 4; j++) {
            regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(j);
            regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            off = HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF;
            result = sysHyPhy20gBitRead(fileHndl, regAddr, off, &sliceEn);
            if (result) return result;   
                 
            if (sliceEn == 0) {
                /* Do nothing. Traffic does not to go OTNMA */  
                DBG_PRINT("%s, %s, %d, Unexpected return\n",__FILE__, __FUNCTION__, __LINE__);
                //return HYPHY20G_SUCCESS;
            } else if (sliceEn == 1) { /* OTNMA slice is enabled */
                /* 3. Check to see if the CPB channel to be enabled matches 
                      the CPB channel related to this OTNMA slice */
                for (t = 0; t < 64; t++) {
                    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(t);
                    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                    mask = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_CPB_CHN_ID_VCAT_GRP_ID_MSK;
                    result = hyPhy20gRegFieldRead(fileHndl, regAddr, mask, 
                                                  &otnmaReg[t]);
                    if (result) return result;
                }
            }
        }
    } else { /* 1 x OTU2 mode */
        /* Only check OTNMA slice '0' */
        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        off = HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, off, &sliceEn);
        if (result) return result;        
        
        if (sliceEn == 0) {
            /* Do nothing. Traffic does not to go OTNMA */
            return HYPHY20G_SUCCESS;
        } else if (sliceEn == 1) { /* OTNMA slice is enabled */
            /* 3. Check to see if the CPB channel to be enabled matches the
              CPB channel related to this OTNMA slice */
            for (t = 0; t < 64; t++) {
                regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(t);
                regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                mask = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_CPB_CHN_ID_VCAT_GRP_ID_MSK;
                result = hyPhy20gRegFieldRead(fileHndl, regAddr, mask, 
                                              &otnmaReg[t]);
                if (result) return result;
            }
        }
    }
    
    /* Set FIFO_CENTER_EN as a function of primaryFlag */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_OTN_MAP_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    mask = HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_FIFO_CENTER_EN_MSK;
    off = HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_FIFO_CENTER_EN_OFF;
    
    if (slice == 0) {
        cpbId = rdChnlId - 19; 
        DBG_PRINT("%s, %s, %d, cpbId = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)cpbId);
    } else {
        cpbId = rdChnlId - 83; 
        DBG_PRINT("%s, %s, %d, cpbId = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)cpbId); 
    }
    
    for (i = 0; i < 64; i++) {
        if (cpbId == otnmaReg[i]) {   
            DBG_PRINT("%s, %s, %d, CPB ID matches, cpbId = %u, otnmaReg[%u] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)cpbId, (unsigned int)i, (unsigned int)otnmaReg[i]);
            /* Traffic is destined to OTNMA. 
               Check if the channel is primary or non-primary */
            if (pRdChnl->primaryFlag == HYPHY20G_CPB_NON_PRIMARY_RD_CHNL) { 
                /* non-primary channel */
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, off, 0);
                if (result) return result;
                DBG_PRINT("%s, %s, %d, pRdChnl->primaryFlag = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRdChnl->primaryFlag);
            } 
        } 
    }
    
    /* Wait for 10 ms delay and reset FIFO_CENTER_EN back to 1 */  
    result = sysHyPhy20gMsDelay(10);
    if (result) return result;
        
    /* set FIFO_CENTER_EN bit back to default which is 1 for normal operation */
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, off, 1);                                                                  
    if (result) return result;                                                                                                
    DBG_PRINT("%s, %s, %d, pRdChnl->primaryFlag = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)pRdChnl->primaryFlag);   
    
    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbChnlDestinedToOtnmaChk */

/*******************************************************************************
**
**  cpbHyPhy20gCpbGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of CPB context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of CPB context
**
*******************************************************************************/
UINT4 cpbHyPhy20gCpbGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    UINT4 size = 0, i = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;
    
	/* include the ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();
	
    if (pCpbCtxt != NULL) {
    	/* calculate the first level structure size of CPB subsystem */
	    size += sizeof(sHYPHY20G_CPB_CTXT);


	    /* calculate the second level size,
	    schd[0~8].pScbsGrpCtxt,
	    schd[0~8].pOnLineCalMem,
	    schd[0~8].pOffLineCalMem,
	    */
	    for (i = 0; i < pCpbCtxt->maxSchd; i++) {
	        size += utilHyPhy20gScbsGetSchdCtxtSize(&(pCpbCtxt->pSchdCtxt[i]));
	    }


	    /* calculate the second level size,
	    wrChnl[0~293].pRdChnlList,
	    */
	    for (i = 0; i < pCpbCtxt->maxWrChnl; i++) {
	        /* include ctxt buffer header */
	        size += utilHyPhy20gGetCtxtHdrSize();
	        size += (pCpbCtxt->pWrChnl[i].rdChnlCnt) * sizeof(UINT2);;
	    }
	}

    return size;
}/* cpbHyPhy20gCpbGetCtxtSize */

/*******************************************************************************
**
**  cpbHyPhy20gCpbExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the CPB context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for CPB context extraction
**
**  OUTPUTS:        pExtractedSize -the context size(in byte) has been extracted
**                  into the buffer
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    INT4 result;
    UINT4 rtLen;
    UINT4 offset = 0, len = 0, i = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pExtractedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pExtractedSize is NULL.");

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;
	
    /* insert ctxt buffer header */
    len = (pCpbCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
			(sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_CPB_CTXT));
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                    HYPHY_CPB_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);
    if (pCpbCtxt != NULL) {
	    /* extract the first level context of CPB subsystem */
	    /* copy the payload into the buffer */
	    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                    (void *)pCpbCtxt, sizeof(sHYPHY20G_CPB_CTXT));
	    offset += sizeof(sHYPHY20G_CPB_CTXT);


	    /* extract the second level context,
	    schd[0~8].pScbsGrpCtxt,
	    schd[0~8].pOnLineCalMem,
	    schd[0~8].pOffLineCalMem,
	    */
	    for (i = 0; i < pCpbCtxt->maxSchd; i++) {
	        result = utilHyPhy20gScbsExtractSchdCtxt(&(pCpbCtxt->pSchdCtxt[i]), 
	                                    ((UINT1 *)pCtxtBuf + offset), &len);
	        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
	                        "%s", "utilHyPhy20gScbsExtractSchdCtxt failed.");
	        offset += len;
	    }


	    /* extract the second level context,
	    wrChnl[0~293].pRdChnlList,
	    */
	    for (i = 0; i < pCpbCtxt->maxWrChnl; i++ ) {
	        /* insert ctxt buffer header */
	        len = sizeof(sHYPHY20G_CTXT_HDR) +  
	            (pCpbCtxt->pWrChnl[i].rdChnlCnt) * sizeof(UINT2);
	        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
	                                HYPHY_CPB_WRCHNL_RDCHNLLIST_CTXT_ID, i);
	        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
	            "%s", "pCtxtBuf is NULL.");  
	        offset += sizeof(sHYPHY20G_CTXT_HDR);

	        /* copy the payload into the buffer */
	        len = (pCpbCtxt->pWrChnl[i].rdChnlCnt) * sizeof(UINT2);
	        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                        (void *)(pCpbCtxt->pWrChnl[i].pRdChnlList), len);
	        offset += len;
	    }
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;

    return HYPHY20G_SUCCESS;
}/* cpbHyPhy20gCpbExtractCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the CPB context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored CPB context for restoration
**
**  OUTPUTS:        pParsedSize -the data size(in byte) has been parsed 
**                  in the buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CPB_CTXT  *pCpbCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    sHYPHY20G_CPB_FIFO_CTXT *pTmpFifo;
    sHYPHY20G_INTF_GRP_CTXT *pTmpIntfGrp;
    sHYPHY20G_SCHD_CTXT     *pTmpSchd;
    sHYPHY20G_WR_CHNL_CTXT  *pTmpWrChnl;
    sHYPHY20G_RD_CHNL_CTXT  *pTmpRdChnl;
    INT4 result;
    UINT4 offset = 0, len = 0, i = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the HYPHY_CPB_CTXT_ID header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_CPB_CTXT_ID, HYPHY_NO_INDEX);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_CPB_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    /* check if the payload is valid */
    if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
        offset = sizeof(sHYPHY20G_CTXT_HDR);
    } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
        /* create new CPB context memory */
        result = hyPhy20gCpbCreateCtxt(fileHndl);
        if (result) {
            DBG_PRINT("%s, %s, %d, hyPhy20gCpbCreateCtxt failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return result;
        }

        /* retrieve device context with pre-created CPB context */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
        
        pCpbCtxt = pDevCtxt->pCpbCtxt;

        /* restore the first level context for CPB subsystem */

        /* backup the useful pointer values */
        pTmpFifo = pCpbCtxt->pFifo;
        pTmpIntfGrp = pCpbCtxt->pIfGrpCtxt;
        pTmpSchd = pCpbCtxt->pSchdCtxt;
        pTmpWrChnl = pCpbCtxt->pWrChnl;
        pTmpRdChnl = pCpbCtxt->pRdChnl;

        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        /* payload len */
        len = pCtxtBufHdr->len - 
                sizeof(sHYPHY20G_CTXT_HDR);
        
        sysHyPhy20gMemCpy((void *)(pCpbCtxt), 
                        ((UINT1 *)pCtxtBuf + offset), len);
        offset += len;

        /* finish restore the first level context for CPB subsystem */
        pCpbCtxt->pFifo = pTmpFifo;
        pCpbCtxt->pIfGrpCtxt = pTmpIntfGrp;
        pCpbCtxt->pSchdCtxt = pTmpSchd;
        pCpbCtxt->pWrChnl = pTmpWrChnl;
        pCpbCtxt->pRdChnl = pTmpRdChnl;

        /* restore the second level context,
        schd[0~8].pScbsGrpCtxt,
        schd[0~8].pOnLineCalMem,
        schd[0~8].pOffLineCalMem,
        */
        for (i = 0; i < pCpbCtxt->maxSchd; i++) {
            result = utilHyPhy20gScbsRestoreSchdCtxt(&(pCpbCtxt->pSchdCtxt[i]), 
                                            ((UINT1 *)pCtxtBuf + offset), &len);
            if (result) {
                DBG_PRINT("%s, %s, %d, utilHyPhy20gScbsRestoreSchdCtxt failed\n", 
                            __FILE__, __FUNCTION__, __LINE__);
                hyPhy20gCpbDestroyCtxt(fileHndl);
                *pParsedSize = 0;
                return result;
            }
            offset += len;
        }

        /* restore the second level context,
        wrChnl[0~293].pRdChnlList,
        */
        for (i = 0; i < pCpbCtxt->maxWrChnl; i++ ) {
            /* try to retrieve the ctxt header in the pre-stored context buffer */
            pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
            if (pCtxtBufHdr->id != HYPHY_CPB_WRCHNL_RDCHNLLIST_CTXT_ID) {
                *pParsedSize = 0;
                hyPhy20gCpbDestroyCtxt(fileHndl);
                return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
            }
            HYPHY20G_ASSERT((pCtxtBufHdr->len >= sizeof(sHYPHY20G_CTXT_HDR)), 
                            HYPHY20G_ERR_DEV_CTXT_CORRUPTED,
                            "%s", "HYPHY20G_ERR_DEV_CTXT_CORRUPTED.");

            /* offset pCtxtBuf to figure out the payload position */
            offset += sizeof(sHYPHY20G_CTXT_HDR);

            /* payload len */
            len = pCtxtBufHdr->len - 
                    sizeof(sHYPHY20G_CTXT_HDR);
            if (len > 0) {
                pCpbCtxt->pWrChnl[i].pRdChnlList = (UINT2 *)sysHyPhy20gMemAlloc(len);
                if (pCpbCtxt->pWrChnl[i].pRdChnlList == NULL) {
                    *pParsedSize = 0;
                    hyPhy20gCpbDestroyCtxt(fileHndl);
                    return HYPHY20G_ERR_MEM_ALLOC;
                }
                sysHyPhy20gMemCpy((void *)(pCpbCtxt->pWrChnl[i].pRdChnlList), 
                                ((UINT1 *)pCtxtBuf + offset), len);
            }
            /* offset to next ctxt buffer header */
            offset += len;
        }
    } else {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        *pParsedSize = 0;
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pParsedSize = offset;
    return HYPHY20G_SUCCESS;
}/* cpbHyPhy20gCpbRestoreCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbAuditRamFifoCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit Ram and Fifo members in CPB subCtxt
**                  These two data members have some dependent 
**                  relationship, so it is better to audit them together.
**                  
**                  PRIVATE function called by cpbHyPhy20gCpbAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 cpbHyPhy20gCpbAuditRamFifoCtxt(struct file *fileHndl)
{  
    UINT4 addr, blockIdx, msk, blkSize=0, data[3], dataTmp, block, blockPtr=0;
    UINT2 fifoIdx, numFifo=0,numFreeBlk=0, numBlks=0;
    INT4 result;
    UINT4 ramType;
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_CPB_CTXT *pCpbCtxt = NULL;
    sHYPHY20G_CPB_RAM_CTXT *pCpbRamCtxt = NULL;
    sHYPHY20G_CPB_FIFO_CTXT *pCpbFifoCtxt=NULL;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnlCtxt=NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    pCpbCtxt = pDevCtxt->pCpbCtxt;
    if (pCpbCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt==NULL in cpbHyPhy20gCpbAuditRamFifoCtxt.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }
            
    pCpbRamCtxt = &(pCpbCtxt->ram);

    pCpbFifoCtxt = pCpbCtxt->pFifo;

    pWrChnlCtxt = pCpbCtxt->pWrChnl;
    
    if ((pCpbFifoCtxt==NULL)||(pWrChnlCtxt==NULL))
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt->pFifo|pWrChnl==NULL.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* audit rule according to cpbHyPhy20gCpbInitCtxt and hyPhy20gCpbSelectRAM function */
    if ((pCpbRamCtxt->valid!=HYPHY20G_VALID_SIGN) ||(pCpbRamCtxt->minFifoSize!=1) 
    	||(pCpbRamCtxt->maxFifoSize!=HYPHY20G_CPB_NUM_BLOCK)||(pCpbRamCtxt->ramType>1)
        ||((pCpbRamCtxt->ramType == 1) && (pCpbRamCtxt->blkSize > 0x3FF))
    	||((pCpbRamCtxt->calEntryRate<1)||(pCpbRamCtxt->calEntryRate>2560))
        ||(pCpbRamCtxt->numFifo>HYPHY20G_CPB_NUM_FIFO))
    {
        DBG_PRINT("%s, %s, %d, pCpbRamCtxt members are out of valid range.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* audit ramType and blkSize according to hyPhy20gCpbSelectRAM function*/
    /*retrieve ramType field as the inversion of INTERNAL_RAM_EN field */
    addr = HYPHY20G_CPB_CPB_REG_QDR_CTLR_CFG;	
    msk = HYPHY20G_CPB_CPB_BIT_QDR_CTLR_CFG_INTERNAL_RAM_EN_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &ramType);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "hyPhy20gRegFieldRead fail."); 

    ramType = (ramType == 0)? 1:0;
    if(pCpbRamCtxt->ramType!=(UINT1)(ramType)) 
    {
        DBG_PRINT("%s, %s, %d, pCpbRamCtxt->ramType mismatch INTERNAL_RAM_EN reg.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    addr = HYPHY20G_CPB_CPB_REG_BLK_CFG;
    msk = HYPHY20G_CPB_CPB_BIT_BLK_CFG_BLOCK_SIZE_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &blkSize);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "hyPhy20gRegFieldRead fail."); 

    if(pCpbRamCtxt->blkSize!=(UINT2)(blkSize)) 
    {
        DBG_PRINT("%s, %s, %d, pCpbRamCtxt->blkSize(%u) mismatch BLOCK_SIZE reg(%u).\n",
            __FILE__, __FUNCTION__, __LINE__, pCpbRamCtxt->blkSize, (UINT2)(blkSize));
        return HYPHY20G_FAILURE;
    }
    /*end audit ramType and blkSize*/

    numFreeBlk = pCpbRamCtxt->numFreeBlk;
    numFifo =0;

    /* audit pCpbRamCtxt/pCpbFifoCtxt/pWrChnlCtxt dependent logic */
    for (fifoIdx=0; fifoIdx<HYPHY20G_CPB_NUM_FIFO; fifoIdx++)
    {
        /* the rule from function cpbHyPhy20gCpbInitCtxt */
    	if (pCpbFifoCtxt->fifoId != fifoIdx) 
        {
            DBG_PRINT("%s, %s, %d, pCpbFifoCtxt->fifoId value wrong.\n",
                __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        /*the rule from function hyPhy20gCpbChnlProv and cpbHyPhy20gCpbBuildFifo*/
        if (pCpbFifoCtxt->inUseFlag==HYPHY20G_TRUE)
        {
            if ((pWrChnlCtxt->valid!=HYPHY20G_VALID_SIGN)||(pWrChnlCtxt->fifo!=pCpbFifoCtxt->fifoId)||(pCpbFifoCtxt->state!=HYPHY20G_CPB_CHNL_EQUIPPED))
            {
                DBG_PRINT("%s, %s, %d, %uth pCpbFifoCtxt wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, fifoIdx);
                return HYPHY20G_FAILURE;
            }
            else
            {
                /* check whether the ram blocks in pCpbFifoCtxt is circled according to cpbHyPhy20gCpbBuildFifo */
                block = pCpbFifoCtxt->firstBlk;

                /* Find the last block */
                if (pCpbFifoCtxt->numBlks > 1) {
                    blockIdx = 1;
                    do {
                        block = pCpbRamCtxt->blockPtr[block];
                        blockIdx++;
                    } while (blockIdx < pCpbFifoCtxt->numBlks);
                }
                
                /* check whether it is the circle FIFO*/
                if (pCpbRamCtxt->blockPtr[block] != pCpbFifoCtxt->firstBlk)
                {
                    DBG_PRINT("%s, %s, %d, the %uth pCpbFifoCtxt ram blocks are not circled.\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)(fifoIdx));
                    return HYPHY20G_FAILURE;
                }

                /* check CPB block pointed RAM according to cpbHyPhy20gCpbWriteBlockPtr 
                   and cpbHyPhy20gCpbSyncSWRamBlkLinkList*/
                block = pCpbFifoCtxt->firstBlk;
                for (blockIdx = 0; blockIdx < pCpbFifoCtxt->numBlks; blockIdx++) 
                {
                    result = cpbHyPhy20gCpbReadBlockPtr(fileHndl, block,&blockPtr);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "cpbHyPhy20gCpbReadBlockPtr fail."); 

                    if (pCpbRamCtxt->blockPtr[block]!=(UINT2)blockPtr)
                    {
                        DBG_PRINT("%s, %s, %d, the %uth pCpbFifoCtxt %uth blockPtr value(%u) mismatch with the register val(%u).\n",
                            __FILE__, __FUNCTION__, __LINE__, fifoIdx, (unsigned int)blockIdx,
                            pCpbRamCtxt->blockPtr[block], (unsigned short)blockPtr);
                        return HYPHY20G_FAILURE;
                    }

                    block = pCpbRamCtxt->blockPtr[block];
                }

                /* audit pFifo->zone1-3 values according to cpbHyPhy20gCpbBuildFifo */
                result = cpbHyPhy20gFifoDepthIndRead(fileHndl, (UINT4)fifoIdx,
                                            &data[0], &data[1],&data[2]);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "cpbHyPhy20gFifoDepthIndRead fail."); 

                
                dataTmp = (UINT4)((pCpbFifoCtxt->numBlks) * (pCpbRamCtxt->blkSize));
                if (dataTmp!= mHYPHY20G_GET_FIELD4(data[0], 
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_FIFO_MAX_DEPTH_MSK,
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_FIFO_MAX_DEPTH_OFF))
                {
                    DBG_PRINT("%s, %s, %d, the %uth pCpbFifoCtxt FIFO_MAX_DEPTH mismatch with (pFifo->numBlks)*(pRAM->blkSize).\n",
                        __FILE__, __FUNCTION__, __LINE__, fifoIdx);
                    return HYPHY20G_FAILURE;
                }

                dataTmp = (UINT4)((pWrChnlCtxt->pRdChnlList)[0]);
                if (dataTmp!= mHYPHY20G_GET_FIELD4(data[0],
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_RD_CH_MSK,
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_RD_CH_OFF))
                {
                    DBG_PRINT("%s, %s, %d, the %uth pCpbFifoCtxt (pWrChnlCtxt->pRdChnlList)[0] mismatch IBIT_FIFO_CFG_RD_CH reg.\n",
                        __FILE__, __FUNCTION__, __LINE__, fifoIdx);
                    return HYPHY20G_FAILURE;
                }
                
                if (pCpbFifoCtxt->zone1 != mHYPHY20G_GET_FIELD4(data[1],
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE1_MSK,
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE1_OFF))
                {
                    DBG_PRINT("%s, %s, %d, the %uth pCpbFifoCtxt->zone1 value mismatch IBIT_FIFO_CFG_ZONE1 reg.\n",
                        __FILE__, __FUNCTION__, __LINE__, fifoIdx);
                    return HYPHY20G_FAILURE;
                }
                    
                if (pCpbFifoCtxt->zone2 != 
                    (mHYPHY20G_GET_FIELD4(data[1],
                     HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_1_MSK,
                     HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_1_OFF)|
                     (mHYPHY20G_GET_FIELD4(data[2],
                      HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_2_MSK,
                      HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE2_2_OFF)<<12)
                    )
                   )
                {
                    DBG_PRINT("%s, %s, %d, the %uth pCpbFifoCtxt->zone2 value mismatch IBIT_FIFO_CFG_ZONE2 reg.\n",
                        __FILE__, __FUNCTION__, __LINE__, fifoIdx);
                    return HYPHY20G_FAILURE;
                }

                if (pCpbFifoCtxt->zone3 != 
                    mHYPHY20G_GET_FIELD4(data[2],
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE3_MSK,
                    HYPHY20G_CPB_CPB_FIFO_IND_CFG_IBIT_FIFO_CFG_ZONE3_OFF))
                {
                    DBG_PRINT("%s, %s, %d, the %uth pCpbFifoCtxt->zone3 value mismatch IBIT_FIFO_CFG_ZONE3 reg.\n",
                        __FILE__, __FUNCTION__, __LINE__, fifoIdx);
                    return HYPHY20G_FAILURE;
                }
            }
            
    		numFifo++;
    		numBlks = numBlks + pCpbFifoCtxt->numBlks;
                
        }
        pCpbFifoCtxt++;
        pWrChnlCtxt++;
    }

    if (pCpbRamCtxt->numFifo!=numFifo)
    {
        DBG_PRINT("%s, %s, %d, pCpbRamCtxt->numFifo(%u) mismatch with created FIFO number(%u).\n",
            __FILE__, __FUNCTION__, __LINE__, pCpbRamCtxt->numFifo, numFifo);
        return HYPHY20G_FAILURE;
    }
    if ((numBlks>HYPHY20G_CPB_NUM_BLOCK)||(numFreeBlk!=(HYPHY20G_CPB_NUM_BLOCK-numBlks)))
    {
        DBG_PRINT("%s, %s, %d, numBlks(%u) or numFreeBlk(%u) value is wrong.\n",
            __FILE__, __FUNCTION__, __LINE__, numBlks, numFreeBlk);
        return HYPHY20G_FAILURE;
    }

	return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbAuditRamFifoCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbAuditIntfCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit IntfGrp data in CPB subCtxt.
**
**                  PRIVATE function called by cpbHyPhy20gCpbAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE 
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 cpbHyPhy20gCpbAuditIntfCtxt(struct file *fileHndl)
{
    UINT4 i, rdRstOff, rdRstVal;
    UINT4 intfRdPortStart, intfRdPortEnd, intfWrPortStart, intfWrPortEnd;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_CPB_CTXT *pCpbCtxt = NULL;
    sHYPHY20G_INTF_GRP_CTXT *pIntfGrp = NULL;
    rdRstOff=rdRstVal=0;
    intfRdPortStart=intfRdPortEnd=intfWrPortStart=intfWrPortEnd=0;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    pCpbCtxt = pDevCtxt->pCpbCtxt;
    if (pCpbCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt==NULL in cpbHyPhy20gCpbAuditIntfCtxt.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* pointer to context for first SCBS in CPB */
    pIntfGrp = (sHYPHY20G_INTF_GRP_CTXT*)pCpbCtxt->pIfGrpCtxt;

    if(pIntfGrp == NULL)
    {
        DBG_PRINT("%s, %s, %d, pIntfGrp == NULL in cpbHyPhy20gCpbAuditIntfCtxt.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* audit sHYPHY20G_INTF_CTXT structure elements according to hyPhy20gCpbIntfRateAwareInit function*/
    for (i=0; i < HYPHY20G_CPB_NUM_IF_GRP; i++)
    {
        if(pIntfGrp->intfGrp != i)
        {
            DBG_PRINT("%s, %s, %d, %uth pIntfGrp->intfGrp value wrong.\n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
            return HYPHY20G_FAILURE;
        }

        /* retrieve setting and register addr for scheduler */
        switch(i) {
        case HYPHY20G_CPB_LINE_INTF_GRP:
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_LINE_RD_SCED_RST_OFF;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_LINE_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_GFPA_INTF_GRP:
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPA_RD_SCED_RST_OFF;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_GFPA_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_GFPB_INTF_GRP:
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_GFPB_RD_SCED_RST_OFF;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_GFPB_PORT_OFFSET;
            intfRdPortEnd = intfWrPortEnd = HYPHY20G_CPB_PIF_PORT_OFFSET;
            break;
        case HYPHY20G_CPB_PIF_INTF_GRP:
            /* reset masks */
            rdRstOff = HYPHY20G_CPB_CPB_BIT_CFG_PI_RD_SCED_RST_OFF;
            /* port offsets */
            intfRdPortStart = intfWrPortStart = HYPHY20G_CPB_PIF_PORT_OFFSET;
            intfRdPortEnd = HYPHY20G_CPB_NUM_RD_PORT;
            intfWrPortEnd = HYPHY20G_CPB_NUM_WR_PORT;
            break;
        default:
            /* redundant to initial argument checking, should never happen */
            HYPHY20G_ASSERT((i<=HYPHY20G_CPB_PIF_INTF_GRP), HYPHY20G_FAILURE,
                            "%s", "i value is out of range.");    
        } /* end switch */

        result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_CPB_CPB_REG_CFG,
                                     rdRstOff, &rdRstVal);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        if(rdRstVal == 1)
        {/*the IntfGrp should be disabled */
            if (pIntfGrp->enable != HYPHY20G_CPB_INTF_DISABLED)
            {
                DBG_PRINT("%s, %s, %d, %uth pIntfGrp->enable value wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
                return HYPHY20G_FAILURE;
            }
            else 
            {
                /* increment to next SCBS block in CPB */
                pIntfGrp++;
                continue;
            }
        }

        if (pIntfGrp->enable == HYPHY20G_CPB_INTF_ENABLED)
        {
            /* test numWrChnls */
            if(pIntfGrp->numWrChnls != (intfWrPortEnd-intfWrPortStart)) 
            {
                DBG_PRINT("%s, %s, %d, %uth pIntfGrp->numWrChnls value wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
                return HYPHY20G_FAILURE;
            }

            /* test numRdChnls */
            if(pIntfGrp->numRdChnls != (intfRdPortEnd-intfRdPortStart)) 
            {
                DBG_PRINT("%s, %s, %d, %uth pIntfGrp->numRdChnls value wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
                return HYPHY20G_FAILURE;
            }
        }
        /* increment to next SCBS block in CPB */
        pIntfGrp++;        

    } /* end while */

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbAuditIntfCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbAuditSchdCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit SCHD data in the CPB subCtxt. 
**
**                  PRIVATE function called by cpbHyPhy20gCpbAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 cpbHyPhy20gCpbAuditSchdCtxt(struct file *fileHndl)
{     
    UINT4 i; 
    INT4 result=0;
    
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_CPB_CTXT *pCpbCtxt = NULL;
    sHYPHY20G_SCHD_CTXT *pSchdCtxt = NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    pCpbCtxt = pDevCtxt->pCpbCtxt;
    if (pCpbCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt==NULL in cpbHyPhy20gCpbAuditSchdCtxt.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* pointer to context for first SCBS in CPB */
    pSchdCtxt = (sHYPHY20G_SCHD_CTXT*)pCpbCtxt->pSchdCtxt;

    for (i=0; i< HYPHY20G_CPB_NUM_SCHD; i++)
    {
        result = utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pSchdCtxt);
        if (result != HYPHY20G_SUCCESS)
        {
            DBG_PRINT("%s, %s, %d, %uth SCHD utilHyPhy20gScbsAuditSchdCtxt failed.\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
            return result;
        }
        /* increment to next SCBS block in CPB */
        pSchdCtxt++;
        DBG_PRINT("%s, %s, %d, %uth SCHD utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pSchdCtxt) SUCCESS in cpbHyPhy20gCpbAuditSchdCtxt\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
    }    
    
	return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbAuditSchdCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbAuditWrChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit WR RD CHNL data in CPB subCtxt 
**
**                  PRIVATE function called by cpbHyPhy20gCpbAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**
*******************************************************************************/
PRIVATE INT4 cpbHyPhy20gCpbAuditWrChnlCtxt(struct file *fileHndl)
{     
    UINT4 i,portOffset=0, enableFlg, addr, portWidth=0; 
    UINT2 wrChnlIdx,rdChnlIdx;
    UINT4 data[4]={0}, dataTmp;
    INT4 result=0;
    UINT4 val=0;
    
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_CPB_CTXT *pCpbCtxt = NULL;
    sHYPHY20G_WR_CHNL_CTXT *pWrChnlCtxt = NULL;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt = NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    pCpbCtxt = pDevCtxt->pCpbCtxt;
    if (pCpbCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt==NULL in cpbHyPhy20gCpbAuditWrChnlCtxt.\n",
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* pointer to context for first SCBS in CPB */
    pWrChnlCtxt = pCpbCtxt->pWrChnl;
    pRdChnlCtxt = pCpbCtxt->pRdChnl;

    if ((pWrChnlCtxt==NULL)||(pRdChnlCtxt==NULL))
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt->pWrChnl|pRdChnl==NULL\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }
    
    for (wrChnlIdx = 0; wrChnlIdx < HYPHY20G_CPB_NUM_WR_PORT; wrChnlIdx++)
    {
        if ( wrChnlIdx< HYPHY20G_CPB_GFPA_PORT_OFFSET ){
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
        } else if ( wrChnlIdx < HYPHY20G_CPB_GFPB_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
        } else if ( wrChnlIdx < HYPHY20G_CPB_PIF_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
        } else {
            portOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
        }
        
        if ((pWrChnlCtxt->chnlId == (wrChnlIdx - portOffset))&&(pWrChnlCtxt->portId == wrChnlIdx)
            &&(pWrChnlCtxt->rdChnlCnt<= pWrChnlCtxt->rdChnlListLen))
        {
            if (((pWrChnlCtxt->state == HYPHY20G_CPB_CHNL_START)&&(pWrChnlCtxt->valid==HYPHY20G_VALID_SIGN))||
                ((pWrChnlCtxt->state > HYPHY20G_CPB_CHNL_START)&&(pWrChnlCtxt->valid==HYPHY20G_INVALID_VAL_16BIT)))
            {
                DBG_PRINT("%s, %s, %d, %uth pWrChnlCtxt state value mismatched with valid value.\n",
                            __FILE__, __FUNCTION__, __LINE__, wrChnlIdx);
                return HYPHY20G_FAILURE;
            }
            /*
            ** audit the HYPHY20G_CPB_CHNL_START state according to cpbHyPhy20gCpbInitCtxt function.
            */
            if (pWrChnlCtxt->state == HYPHY20G_CPB_CHNL_START)
            {
                if ((pWrChnlCtxt->rate != 0)||(pWrChnlCtxt->portWidth != HYPHY20G_INVALID_VAL_16BIT)||
                    (pWrChnlCtxt->valid != HYPHY20G_INVALID_VAL_16BIT)||(pWrChnlCtxt->rdChnlCnt != 0)||
                    (pWrChnlCtxt->rdChnlListLen != 0)||(pWrChnlCtxt->pRdChnlList != NULL))
                {
                    DBG_PRINT("%s, %s, %d, %uth pWrChnlCtxt members value wrong in CHNL_START state.\n",
                        __FILE__, __FUNCTION__, __LINE__, wrChnlIdx);
                    return HYPHY20G_FAILURE;
                }
            }
            else
            {
                /*the attribute range general audit (hyPhy20gCpbChnlProv function)  */
                if( (pWrChnlCtxt->portWidth> 1) ||
                    (pWrChnlCtxt->rate < 1)||(pWrChnlCtxt->rate > 12000)||
                    (pWrChnlCtxt->fifo != wrChnlIdx)
                  )
                {
                    DBG_PRINT("%s, %s, %d, %uth pWrChnlCtxt members value wrong in NON CHNL_START state.\n",
                        __FILE__, __FUNCTION__, __LINE__, wrChnlIdx);
                    return HYPHY20G_FAILURE;
                }
                /* audit portWidth according to 
                hyPhy20gCpbChnlProv/hyPhy20gCpbChnlRateAwareProv/cpbHyPhy20gCpbConfigWrPort functions*/
                if (wrChnlIdx <= HYPHY20G_CPB_LINE_LS_MAX_PORT) 
                {
                    /* LOW SPEED PORT CONFIGURATION */
                    addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(wrChnlIdx);
                    result = sysHyPhy20gRead(fileHndl, addr, &val);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "sysHyPhy20gRead fail."); 

                    portWidth = mHYPHY20G_GET_FIELD4(val,
                            HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_MSK,
                            HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_OFF);

                } 
                else if (wrChnlIdx >= HYPHY20G_CPB_LINE_HS_MIN_PORT&& wrChnlIdx <= HYPHY20G_CPB_LINE_HS_MAX_PORT) 
                {
                    /* HIGH SPEED PORT CONFIGURATION */
                    addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(wrChnlIdx - HYPHY20G_CPB_LINE_HS_MIN_PORT);
                    result = sysHyPhy20gRead(fileHndl, addr, &val);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "sysHyPhy20gRead fail."); 

                    portWidth = mHYPHY20G_GET_FIELD4(val,
                            HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_MSK,
                            HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_OFF);

                } 
                else if (wrChnlIdx >= HYPHY20G_CPB_GFPA_MIN_PORT&& wrChnlIdx <= HYPHY20G_CPB_GFPB_MAX_PORT) 
                {
                    /* GFP PORT CONFIGURATION */
                    if (wrChnlIdx >= HYPHY20G_CPB_GFPA_MIN_PORT && wrChnlIdx <= HYPHY20G_CPB_GFPA_MAX_PORT) {
                        /* GFP A*/
                        addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(wrChnlIdx-HYPHY20G_CPB_GFPA_MIN_PORT, 0);
                    } else {
                        /* GFP B*/
                        addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(wrChnlIdx-HYPHY20G_CPB_GFPB_MIN_PORT, 1);
                    }

                    result = sysHyPhy20gRead(fileHndl, addr, &val);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "sysHyPhy20gRead fail."); 

                    portWidth = mHYPHY20G_GET_FIELD4(val,
                            HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_WIDTH_MSK,
                            HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_WR_PORT_WIDTH_OFF);
                    
                } 
                else
                {
                    /*Other wrChnl don't need check the portWidth value. */
                }
                
                if(((wrChnlIdx <= HYPHY20G_CPB_LINE_LS_MAX_PORT)||
                    (wrChnlIdx >= HYPHY20G_CPB_LINE_HS_MIN_PORT&& wrChnlIdx <= HYPHY20G_CPB_LINE_HS_MAX_PORT)||
                    (wrChnlIdx >= HYPHY20G_CPB_GFPA_MIN_PORT&& wrChnlIdx <= HYPHY20G_CPB_GFPB_MAX_PORT))&&
                    (portWidth != pWrChnlCtxt->portWidth))
                {
                    DBG_PRINT("%s, %s, %d, %uth pWrChnlCtxt->portWidth(%u) != WR_PORT_WIDTH(%u).\n",
                        __FILE__, __FUNCTION__, __LINE__, wrChnlIdx,
                        pWrChnlCtxt->portWidth, (unsigned short)portWidth);
                    return HYPHY20G_FAILURE;
                }
                    

                /*audit operational state according to 
                hyPhy20gCpbChnlEnable/hyPhy20gCpbChnlRateAwareEnable/cpbHyPhy20gCpbEnableWrPtr functions */

                result = cpbHyPhy20gWrPtrCtxtIndRead(fileHndl,
                                                    (UINT4)wrChnlIdx, &data[0], &data[1]);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "cpbHyPhy20gWrPtrCtxtIndRead fail."); 

                enableFlg = mHYPHY20G_GET_FIELD4(data[1], 
                HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WRCHAN_EN_MSK, 
                HYPHY20G_CPB_CPB_WR_PTR_IND_CFG_IBIT_WR_PTR_CTXT_CFG_WRCHAN_EN_OFF);   
                    
                if (pWrChnlCtxt->state == HYPHY20G_CPB_CHNL_OPERATIONAL)
                {
                    if (enableFlg == 0) 
                    {
                        DBG_PRINT("%s, %s, %d, %uth pWrChnlCtxt->state OPERATIONAL when WRCHAN_EN is disabled.\n",
                            __FILE__, __FUNCTION__, __LINE__, wrChnlIdx);
                        return HYPHY20G_FAILURE;
                    }
                }
                else
                {
                    if (enableFlg == 1) 
                    {
                        DBG_PRINT("%s, %s, %d, %uth pWrChnlCtxt->state NON_OPERATIONAL when WRCHAN_EN is enabled.\n",
                            __FILE__, __FUNCTION__, __LINE__, wrChnlIdx);
                        return HYPHY20G_FAILURE;
                    }
                }

                /* audit the rdchnl list and the rdchnl member operational state*/
                for (i=0;i<pWrChnlCtxt->rdChnlCnt; i++)
                {
                    rdChnlIdx=(pWrChnlCtxt->pRdChnlList)[i];
                    if (rdChnlIdx > (HYPHY20G_CPB_NUM_RD_PORT - 1))
                    {
                        DBG_PRINT("%s, %s, %d, %uth pWrChnlCtxt->(pWrChnlCtxt->pRdChnlList)[%u] > (HYPHY20G_CPB_NUM_RD_PORT - 1).\n",
                            __FILE__, __FUNCTION__, __LINE__, wrChnlIdx, (unsigned int)i );
                        return HYPHY20G_FAILURE;
                    }
                    
                    pRdChnlCtxt=pCpbCtxt->pRdChnl+rdChnlIdx;
                    if (pRdChnlCtxt->valid!=HYPHY20G_VALID_SIGN)
                    {
                        DBG_PRINT("%s, %s, %d, %uth RdChnl valid!=HYPHY20G_VALID_SIGN.\n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx);
                        return HYPHY20G_FAILURE;
                    }

                    if (pRdChnlCtxt->index!=i)
                    {
                        DBG_PRINT("%s, %s, %d, %uth RdChnl index wrong.\n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx);
                        return HYPHY20G_FAILURE;
                    }

                    if (pRdChnlCtxt->rate != pWrChnlCtxt->rate)
                    {
                        DBG_PRINT("%s, %s, %d, %uth RdChnl rate mismatch with %uth WrChnl rate.\n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx, wrChnlIdx );
                        return HYPHY20G_FAILURE;
                    }

                    if (((i==0)&&(pRdChnlCtxt->primaryFlag!=HYPHY20G_CPB_PRIMARY_RD_CHNL))||
                        ((i>0)&&(pRdChnlCtxt->primaryFlag!=HYPHY20G_CPB_NON_PRIMARY_RD_CHNL)))
                    {
                        DBG_PRINT("%s, %s, %d, %uth RdChnl primaryFlag value is wrong. \n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx);
                        return HYPHY20G_FAILURE;
                    }

                    /* check the pWrChnlCtxt and it's pRdChnlCtxt primary member state consistent*/
                    if ((pRdChnlCtxt->primaryFlag==HYPHY20G_CPB_PRIMARY_RD_CHNL)&&
                        (pRdChnlCtxt->state != pWrChnlCtxt->state))
                    {
                        DBG_PRINT("%s, %s, %d, %uth RdChnl(primary) state mismatch with %uth WrChnl state.\n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx, wrChnlIdx );
                        return HYPHY20G_FAILURE;
                    }

                    /* Audit Check according to cpbHyPhy20gCpbInitRdPtr/cpbHyPhy20gCpbEnableRdPtr */
                    result = cpbHyPhy20gRdPtrCtxtIndRead(fileHndl, rdChnlIdx, &data[0],
                                                         &data[1]);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "cpbHyPhy20gRdPtrCtxtIndRead fail."); 

                    dataTmp = mHYPHY20G_GET_FIELD4(data[0],
                    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_WR_CH_MSK,
                    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_WR_CH_OFF);

                    if (wrChnlIdx != (UINT2)dataTmp)
                    {
                        DBG_PRINT("%s, %s, %d, %uth pRdChnlCtxt wrong because wrChnlIdx(%u) mismatch WR_CH reg(%u).\n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx, 
                            wrChnlIdx, (UINT2)dataTmp);
                        return HYPHY20G_FAILURE;
                    }
                   
                    dataTmp = mHYPHY20G_GET_FIELD4(data[0],
                    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_PRIMARY_MSK,
                    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_PRIMARY_OFF);

                    if (((pRdChnlCtxt->primaryFlag==HYPHY20G_CPB_PRIMARY_RD_CHNL)&&(dataTmp != 1))||
                        ((pRdChnlCtxt->primaryFlag!=HYPHY20G_CPB_PRIMARY_RD_CHNL)&&(dataTmp != 0)))
                    {
                        DBG_PRINT("%s, %s, %d, %uth pRdChnlCtxt wrong because primaryFlag and RD PRIMARY reg mismatch.\n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx);
                        return HYPHY20G_FAILURE;
                    }

                    enableFlg = mHYPHY20G_GET_FIELD4(data[1],
                    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_MSK,
                    HYPHY20G_CPB_CPB_RD_PTR_IND_CFG_IBIT_RD_PTR_CTXT_CFG_RDCHAN_EN_OFF);
                    if (pRdChnlCtxt->state == HYPHY20G_CPB_CHNL_OPERATIONAL)
                    {
                        if (enableFlg == 0) 
                        {
                            DBG_PRINT("%s, %s, %d, %uth pRdChnlCtxt->state OPERATIONAL when RDCHAN_EN is disabled.\n",
                                __FILE__, __FUNCTION__, __LINE__, rdChnlIdx);
                            return HYPHY20G_FAILURE;
                        }
                    }
                    else
                    {
                        if (enableFlg == 1) 
                        {
                            DBG_PRINT("%s, %s, %d, %uth pRdChnlCtxt->state NON_OPERATIONAL when RDCHAN_EN is enabled.\n",
                                __FILE__, __FUNCTION__, __LINE__, rdChnlIdx);
                            return HYPHY20G_FAILURE;
                        }
                    }
                   
                    /* audit portWidth according to 
                    hyPhy20gCpbChnlProv/hyPhy20gCpbChnlRateAwareProv/cpbHyPhy20gCpbConfigRdPort functions*/
                    if (rdChnlIdx <= HYPHY20G_CPB_LINE_LS_MAX_PORT) 
                    {
                        /* LOW SPEED PORT CONFIGURATION */
                        addr = HYPHY20G_CPB_CPB_REG_LOW_SPD_PRT_CFG_OFFSET(rdChnlIdx);
                        result = sysHyPhy20gRead(fileHndl, addr, &val);
                        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                        "%s", "sysHyPhy20gRead fail."); 

                        portWidth = mHYPHY20G_GET_FIELD4(val,
                                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_MSK,
                                HYPHY20G_CPB_CPB_BIT_LOW_SPD_PRT_CFG_LS_PORT_WIDTH_OFF);

                    } 
                    else if (rdChnlIdx >= HYPHY20G_CPB_LINE_HS_MIN_PORT&& rdChnlIdx <= HYPHY20G_CPB_LINE_HS_MAX_PORT) 
                    {
                        /* HIGH SPEED PORT CONFIGURATION */
                        addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(wrChnlIdx - HYPHY20G_CPB_LINE_HS_MIN_PORT);
                        result = sysHyPhy20gRead(fileHndl, addr, &val);
                        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                        "%s", "sysHyPhy20gRead fail."); 

                        portWidth = mHYPHY20G_GET_FIELD4(val,
                                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_MSK,
                                HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_PORT_WIDTH_OFF);

                    } 
                    else if (rdChnlIdx >= HYPHY20G_CPB_GFPA_MIN_PORT&& rdChnlIdx <= HYPHY20G_CPB_GFPB_MAX_PORT) 
                    {
                        /* GFP PORT CONFIGURATION */
                        if (rdChnlIdx >= HYPHY20G_CPB_GFPA_MIN_PORT && rdChnlIdx <= HYPHY20G_CPB_GFPA_MAX_PORT) {
                            /* GFP A*/
                            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(rdChnlIdx-HYPHY20G_CPB_GFPA_MIN_PORT, 0);
                        } else {
                            /* GFP B*/
                            addr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(rdChnlIdx-HYPHY20G_CPB_GFPB_MIN_PORT, 1);
                        }

                        result = sysHyPhy20gRead(fileHndl, addr, &val);
                        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                        "%s", "sysHyPhy20gRead fail."); 

                        portWidth = mHYPHY20G_GET_FIELD4(val,
                                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_WIDTH_MSK,
                                HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_PORT_WIDTH_OFF);
                        
                    } 
                    else
                    {
                        /*Other rdChnl don't need check the portWidth value. */
                    }
                    
                    if(((rdChnlIdx <= HYPHY20G_CPB_LINE_LS_MAX_PORT)||
                        (rdChnlIdx >= HYPHY20G_CPB_LINE_HS_MIN_PORT&& wrChnlIdx <= HYPHY20G_CPB_LINE_HS_MAX_PORT)||
                        (rdChnlIdx >= HYPHY20G_CPB_GFPA_MIN_PORT&& rdChnlIdx <= HYPHY20G_CPB_GFPB_MAX_PORT))&&
                        (portWidth != pRdChnlCtxt->portWidth))
                    {
                        DBG_PRINT("%s, %s, %d, %uth pRdChnlCtxt->portWidth(%u) != RD_PORT_WIDTH(%u).\n",
                            __FILE__, __FUNCTION__, __LINE__, rdChnlIdx, pRdChnlCtxt->portWidth, (unsigned short)portWidth );
                        return HYPHY20G_FAILURE;
                    }

                }
            }

            /* audit state  */
            pWrChnlCtxt++;
        }
        else
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
    }
	return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbAuditWrChnlCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbAuditRdChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit RD CHNL data in CPB subCtxt 
**
**                  PRIVATE function called by cpbHyPhy20gCpbAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 cpbHyPhy20gCpbAuditRdChnlCtxt(struct file *fileHndl)
{
    UINT4 portOffset=0; 
    UINT2 rdChnlIdx;
    
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_CPB_CTXT *pCpbCtxt = NULL;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt = NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    pCpbCtxt = pDevCtxt->pCpbCtxt;
    if (pCpbCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt==NULL in cpbHyPhy20gCpbAuditRdChnlCtxt.\n",
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* pointer to context for first SCBS in CPB */
    pRdChnlCtxt = pCpbCtxt->pRdChnl;

    if (pRdChnlCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt->pRdChnl==NULL\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    pRdChnlCtxt = pCpbCtxt->pRdChnl;

    for (rdChnlIdx= 0; rdChnlIdx < HYPHY20G_CPB_NUM_RD_PORT; rdChnlIdx++)
    {
        if ( rdChnlIdx < HYPHY20G_CPB_GFPA_PORT_OFFSET ){
            portOffset = HYPHY20G_CPB_LINE_PORT_OFFSET;
        } else if ( rdChnlIdx < HYPHY20G_CPB_GFPB_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET;
        } else if ( rdChnlIdx < HYPHY20G_CPB_PIF_PORT_OFFSET ) {
            portOffset = HYPHY20G_CPB_GFPB_PORT_OFFSET;
        } else {
            portOffset = HYPHY20G_CPB_PIF_PORT_OFFSET;
        }
        if ((pRdChnlCtxt->chnlId == (rdChnlIdx - portOffset))&&(pRdChnlCtxt->portId == rdChnlIdx))
        {
            if (((pRdChnlCtxt->state == HYPHY20G_CPB_CHNL_START)&&(pRdChnlCtxt->valid!=HYPHY20G_INVALID_VAL_16BIT))||
                ((pRdChnlCtxt->state > HYPHY20G_CPB_CHNL_START)&&(pRdChnlCtxt->valid==HYPHY20G_INVALID_VAL_16BIT)))
            {
                DBG_PRINT("%s, %s, %d, pRdChnlCtxt state value mismatched with valid value.\n",
                            __FILE__, __FUNCTION__, __LINE__);
            }
            /*
            ** audit the HYPHY20G_CPB_CHNL_START state according to cpbHyPhy20gCpbInitCtxt function.
            */
            if (pRdChnlCtxt->state == HYPHY20G_CPB_CHNL_START)
            {
                if ((pRdChnlCtxt->rate != 0)||(pRdChnlCtxt->portWidth != HYPHY20G_INVALID_VAL_16BIT)||
                    (pRdChnlCtxt->valid != HYPHY20G_INVALID_VAL_16BIT)||(pRdChnlCtxt->primaryFlag!= 0)||
                    (pRdChnlCtxt->index != HYPHY20G_INVALID_VAL_16BIT))
                {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
            }
            else
            {
                /*the attribute range general audit  */
                if( (pRdChnlCtxt->portWidth> 1) ||
                    (pRdChnlCtxt->rate < 1)||(pRdChnlCtxt->rate > 12000)
                  )
                {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }

                /* audit the portWidth, already done in wrChnl audit */
                
                /* audit the index and primaryFlag, already done in wrChnl audit  */

                /* audit the state, already done in wrChnl audit */

                /* audit the gfpGrpId according to the hyPhy20gGfpTxChAdd and hyPhy20gGfpRateAwareTxChAdd
                in hyphy20g_gfp.c, 0-63, 0xFF*/
                if ((rdChnlIdx>=HYPHY20G_CPB_GFPA_MIN_PORT)&&(rdChnlIdx<=HYPHY20G_CPB_GFPB_MAX_PORT))
                {
                    if ((pRdChnlCtxt->gfpGrpId!=0xFF)&&(pRdChnlCtxt->gfpGrpId>=HYPHY20G_GFP_TX_SCHD_MAX_GRP))
                    {
                        DBG_PRINT("%s, %s, %d, pRdChnlCtxt->gfpGrpId value wrong.\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_FAILURE;
                    }
                }
            }
            pRdChnlCtxt++;
        }
        else
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
    }
    
	return HYPHY20G_SUCCESS;

}/* cpbHyPhy20gCpbAuditRdChnlCtxt */

/*******************************************************************************
**
**  cpbHyPhy20gCpbAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit CPB subCtxt.
**
**                  PRIVATE function called by the hyPhy20gAuditCtxt
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gCpbAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_CPB_CTXT *pCpbCtxt = NULL;
    INT4 result;
    UINT4 lowPwrBit,rstBit;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    pCpbCtxt = pDevCtxt->pCpbCtxt;

    /* check whether CPD module is disabled*/
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                          HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_CPB_LOWPWR_OFF, &lowPwrBit);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gBitRead fail."); 
    
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                          HYPHY20G_TOP_BIT_SYS_SW_RST_3_CPB_RST_OFF, &rstBit);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gBitRead fail."); 

    /* CPB module is disabled */
    if (lowPwrBit == 1)
    {
        if (pCpbCtxt!=NULL)
        {
            DBG_PRINT("%s, %s, %d, pCpbCtxt!=NULL when CPB_LOWPWR == 1\n",
                __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        else
            return HYPHY20G_SUCCESS;
    }

    if (rstBit == 1)
    {
        if (pCpbCtxt!=NULL)
        {
            DBG_PRINT("%s, %s, %d, pCpbCtxt!=NULL when CPB_RST == 1\n",
                __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        else
            return HYPHY20G_SUCCESS;
    }

    /*CPB module is enabled*/
    if (pCpbCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, pCpbCtxt==NULL when CPB module enabled\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    if ((pCpbCtxt->pFifo == NULL)||(pCpbCtxt->pIfGrpCtxt == NULL)||
        (pCpbCtxt->pSchdCtxt == NULL)||(pCpbCtxt->pWrChnl == NULL)||
        (pCpbCtxt->pRdChnl == NULL)||(pCpbCtxt->valid != HYPHY20G_VALID_SIGN))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }


    /* audit ram and *pFifo */
    result = cpbHyPhy20gCpbAuditRamFifoCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, cpbHyPhy20gCpbAuditRamFifoCtxt(fileHndl) SUCCESS in cpbHyPhy20gCpbAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pIfGrpCtxt */
    result = cpbHyPhy20gCpbAuditIntfCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, cpbHyPhy20gCpbAuditIntfCtxt(fileHndl) SUCCESS in cpbHyPhy20gCpbAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pSchdCtxt */
    result = cpbHyPhy20gCpbAuditSchdCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, cpbHyPhy20gCpbAuditSchdCtxt(fileHndl) SUCCESS in cpbHyPhy20gCpbAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pWrChnl */
    result = cpbHyPhy20gCpbAuditWrChnlCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, cpbHyPhy20gCpbAuditWrChnlCtxt(fileHndl) SUCCESS in cpbHyPhy20gCpbAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pRdChnl */
    result = cpbHyPhy20gCpbAuditRdChnlCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, cpbHyPhy20gCpbAuditRdChnlCtxt(fileHndl) SUCCESS in cpbHyPhy20gCpbAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);
    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbAuditCtxt */

/* end of file */

