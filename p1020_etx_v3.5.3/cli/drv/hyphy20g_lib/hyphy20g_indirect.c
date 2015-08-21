/******************************************************************************/
/**  COPYRIGHT (C) 2000-2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.           **/
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
**  MODULE      : Hyphy20G Configuration Code
**
**  FILE        : hyphy20g_indirect.c
**
**  $LastChangedDate: 2011-11-08 10:46:38 +0800 (Tue, 08 Nov 2011) $
**
**  $Revision: 16843 $
**
**  DESCRIPTION : Contains the Hyphy20g indirect register access 
**                functions.
**
**  NOTES       :
**
*******************************************************************************/
/******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_indirect.h"
#include "hyphy20g_api.h"

/* VCAT */
/* PIF */
/* CPB */
/* GFP */

/******************************************************************************/
/* PRIVATE TYPE and MACRO DEFINITION                                          */
/******************************************************************************/
/* the sHYPHY_IND_MEM_DATA_TYPE is defined to store the Addr range 
   for each memSel*/
typedef struct{
    UINT4 memSel;
    UINT4 minAddr;
    UINT4 maxAddr;
}sHYPHY_IND_MEM_DATA_TYPE;

#define HYPHY20G_VCAT_RVCPO_MEM_IND_WIDTH 5
#define HYPHY20G_VCAT_RVCPO_MEM_IND_ENTRY_NUM 13

#define HYPHY20G_VCAT_RVCPO_INT_IND_WIDTH 1
#define HYPHY20G_VCAT_RVCPO_INT_IND_ENTRY_NUM 2

#define HYPHY20G_VCAT_TVCPO_MEM_IND_WIDTH 2
#define HYPHY20G_VCAT_TVCPO_MEM_IND_ENTRY_NUM 6

#define HYPHY20G_VCAT_TVCPO_INT_IND_WIDTH 1

#define HYPHY20G_PIF_RX_CAL_PROG_IND_WIDTH 1

#define HYPHY20G_PIF_TX_CAL_PROG_IND_WIDTH 1

#define HYPHY20G_CPB_WR_PTR_CTXT_IND_WIDTH 2

#define HYPHY20G_CPB_RD_PTR_CTXT_IND_WIDTH 2

#define HYPHY20G_CPB_FIFO_DEPTH_IND_WIDTH 3

#define HYPHY20G_GFPF_TXDP_CHAN_IND_WIDTH 3

#define HYPHY20G_GFPF_TXDP_EXT_HDR_IND_WIDTH 1

#define HYPHY20G_GFPF_TXDP_PMON_IND_WIDTH 6

#define HYPHY20G_GFPF_RXDP_CHAN_IND_WIDTH 2 

#define HYPHY20G_GFPT_RX_GFPT_CHAN_IND_WIDTH 1

#define HYPHY20G_GFPT_RX_GFPT_PMON_IND_WIDTH 4

#define HYPHY20G_GFPT_TX_GFPT_REQ_PROC_CHAN_IND_WIDTH 2

#define HYPHY20G_GFPT_TX_GFPT_CHAN_IND_WIDTH 2

#define HYPHY20G_GFPT_TX_GFPT_PMON_IND_WIDTH 4

#define HYPHY20G_VCAT_TVCP_MEM_IND_WIDTH 8
#define HYPHY20G_VCAT_TVCP_MEM_IND_ENTRY_NUM 7

#define HYPHY20G_VCAT_TVCP_INT_IND_WIDTH 1

#define HYPHY20G_VCAT_RVCP_MEM_IND_WIDTH 8
#define HYPHY20G_VCAT_RVCP_MEM_IND_ENTRY_NUM 14

#define HYPHY20G_VCAT_RVCP_INT_IND_WIDTH 1
#define HYPHY20G_VCAT_RVCP_INT_IND_ENTRY_NUM 2

#define HYPHY20G_TSE_PRGM_IND_WIDTH 1

#define HYPHY20G_TSE_SHPI_IND_WIDTH 1

#define HYPHY20G_TSE_PCTL_MEM_IND_WIDTH 1

#define HYPHY20G_DMI_SHPI_IND_WIDTH 1

#define HYPHY20G_DMI_STFA_IND_WIDTH 1

#define HYPHY20G_SONET_SBER_SECT_IND_WIDTH 1

#define HYPHY20G_SONET_RTTP_SECT_IND_WIDTH 1

#define HYPHY20G_SONET_RSHPI_IND_WIDTH 1

#define HYPHY20G_SONET_RHAP_IND_WIDTH 1

#define HYPHY20G_SONET_RTTP_PATH_IND_WIDTH 1

#define HYPHY20G_SONET_RSVCA_IND_WIDTH 1

#define HYPHY20G_SONET_SBER_PATH_IND_WIDTH 1

#define HYPHY20G_SONET_SARC_IND_WIDTH 1

#define HYPHY20G_SONET_TTTP_SECT_IND_WIDTH 1

#define HYPHY20G_SONET_THPP_IND_WIDTH 1

#define HYPHY20G_SONET_TTTP_PATH_IND_WIDTH 1

#define HYPHY20G_SONET_TSVCA_IND_WIDTH 1

#define HYPHY20G_SONET_RTERB_IND_WIDTH 1

#define HYPHY20G_SONET_TRERB_IND_WIDTH 1

#define HYPHY20G_SONET_TSHPI_IND_WIDTH 1

#define HYPHY20G_HOPP_RSHPI_IND_WIDTH 1

#define HYPHY20G_HOPP_RHAP_IND_WIDTH 1

#define HYPHY20G_HOPP_RTTP_PATH_IND_WIDTH 1

#define HYPHY20G_HOPP_SBER_PATH_IND_WIDTH 1

#define HYPHY20G_HOPP_SARC_IND_WIDTH 1

#define HYPHY20G_HOPP_THPP_IND_WIDTH 1

#define HYPHY20G_HOPP_TTERB_IND_WIDTH 1

#define HYPHY20G_HOPP_RRERB_IND_WIDTH 1

#define HYPHY20G_PIF_INTLNK_CAL_WIDTH 1

#define HYPHY20G_CBRI_INTLNK_CAL_WIDTH 1

/** private data **/

/* the vcatHyPhy20gRVCPOMemInd defined to store the Addr range 
   for each memSel of RVCPO MTSB Indirect Memory data*/
PRIVATE sHYPHY_IND_MEM_DATA_TYPE 
vcatHyPhy20gRVCPOMemInd[HYPHY20G_VCAT_RVCPO_MEM_IND_ENTRY_NUM] = {
    {0x0,0,0x3},
    {0x1,0,0x3},
    {0x2,0,0x7},
    {0x3,0,0x7F},
    {0x4,0,0x3},
    {0x5,0,0x7F},
    {0x6,0,0x3},
    {0x7,0,0x7},
    {0x8,0,0x7},
    {0x9,0,0x3},
    {0xA,0,0x3},
    {0xB,0,0x3},
    {0xC,0,0x3},
};

/* the vcatHyPhy20gRVCPOIntInd defined to store the Addr range 
   for each memSel of RVCPO MTSB Indirect Interrupt Data*/
PRIVATE sHYPHY_IND_MEM_DATA_TYPE 
vcatHyPhy20gRVCPOIntInd[HYPHY20G_VCAT_RVCPO_INT_IND_ENTRY_NUM] = {
    {0x0,0,0x3},
    {0x1,0,0x3},
};

/* the vcatHyPhy20gTVCPOMemInd defined to store the Addr range 
   for each memSel of TVCPO MTSB Indirect Memory Data*/
PRIVATE sHYPHY_IND_MEM_DATA_TYPE
vcatHyPhy20gTVCPOMemInd[HYPHY20G_VCAT_TVCPO_MEM_IND_ENTRY_NUM] = {
    {0x0,0,0x3},
    {0x1,0,0x3},
    {0x2,0,0xB},
    {0x3,0,0x3},
    {0x4,0,0x3},
    {0x5,0,0x7},        
};

/* the vcatHyPhy20gTVCPMemInd defined to store the Addr range 
   for each memSel of TVCP MTSB Indirect Memory Data*/
PRIVATE sHYPHY_IND_MEM_DATA_TYPE
vcatHyPhy20gTVCPMemInd[HYPHY20G_VCAT_TVCP_MEM_IND_ENTRY_NUM] = {
    {0x0,0,0xBF},
    {0x1,0,0x5FF},
    {0x2,0,0xBF},
    {0x3,0,0xBF},
    {0x4,0,0x100},
    {0x5,0,0x17F},
    {0x6,0,0x7F},
};

/* the vcatHyPhy20gRVCPMemInd defined to store the Addr range 
   for each memSel of RVCP MTSB Indirect Memory Data*/
PRIVATE sHYPHY_IND_MEM_DATA_TYPE
vcatHyPhy20gRVCPMemInd[HYPHY20G_VCAT_RVCP_MEM_IND_ENTRY_NUM] = {
    {0x0,0,0xBF},
    {0x1,0,0x3F},
    {0x2,0,0x17F},
    {0x4,0,0x2FFF},
    {0x5,0,0xBF},
    {0x6,0,0x2FFF},
    {0x7,0,0xBF},
    {0x8,0,0x80},
    {0x9,0,0x80},
    {0xA,0,0x80},
    {0xB,0,0x3F},
    {0xC,0,0x3F},
    {0xD,0,0xFE},
    {0xE,0,0xBF},
};

/* the vcatHyPhy20gRVCPIntInd defined to store the Addr range 
   for each memSel of RVCP MTSB Indirect Interrupt Data*/
PRIVATE sHYPHY_IND_MEM_DATA_TYPE 
vcatHyPhy20gRVCPIntInd[HYPHY20G_VCAT_RVCP_INT_IND_ENTRY_NUM] = {
    {0x0,0,0xBF},
    {0x1,0,0x3F},
};

/******************************************************************************/
/* PUBLIC FUNCTIONS (called by API or User)                                   */
/******************************************************************************/

/*******************************************************************************
**
**  vcatHyPhy20gRVCPOMemIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 4 bytes of RVCPO MTSB Indirect Memory Data.
**
**  INPUTS:        *fileHndl  - base address
**                 slice      - valid values are:
**                              0x0, 0x1
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : Group Configuration Table 
**                              0x2 : DM Link Context Table 
**                              0x3 : Link Control Packet DCB Table
**                              0x4 : Link Delay Eval Status Table
**                              0x5 : Link DCB CRC-16 Table 
**                              0x6 : Link Statistics Table
**                              0x7 : DM Group Context Table 0 
**                              0x8 : DM Group Context Table 1 
**                              0x9 : DM Group Context Table 2 
**                              0xA : NE Group MST Buffer Tbale 
**                              0xB : OTN Control Packet Extraction
**                                    Context Table  
**                              0xC : DCB CRC16 Read Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              -----------------------------
**                              -  memSel  | memAddr range  -
**                              -   0x0    |   0x0 ~ 0x3    -
**                              -   0x1    |   0x0 ~ 0x3    -
**                              -   0x2    |   0x0 ~ 0x7    -
**                              -   0x3    |   0x0 ~ 0x7F   -
**                              -   0x4    |   0x0 ~ 0x3    -
**                              -   0x5    |   0x0 ~ 0x7F   -
**                              -   0x6    |   0x0 ~ 0x3    -
**                              -   0x7    |   0x0 ~ 0x7    -
**                              -   0x8    |   0x0 ~ 0x7    -
**                              -   0x9    |   0x0 ~ 0x3    -
**                              -   0xA    |   0x0 ~ 0x3    -
**                              -   0xB    |   0x0 ~ 0x3    -
**                              -   0xC    |   0x0 ~ 0x3    -
**                              -----------------------------
**                 *idata0    - pointer to storage for data reg 0 (bits 31:0)
**                 *idata1    - pointer to storage for data reg 1 (bits 63:32)
**                 *idata2    - pointer to storage for data reg 2 (bits 95:64)
**                 *idata3    - pointer to storage for data reg 3 (bits 127:96)
**                 *idata4    - pointer to storage for data reg 4 (bits 161:128)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPOMemIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memSel, UINT4 memAddr, UINT4 *idata0,
                                 UINT4 *idata1, UINT4 *idata2, UINT4 *idata3,
                                 UINT4 *idata4)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;   
    UINT4 indMemAddrReg; 
    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    UINT4 indMemDataReg2;
    UINT4 indMemDataReg3;
    UINT4 indMemDataReg4;
    UINT4 indMemData0;
    UINT4 indMemData1;
    UINT4 indMemData2;
    UINT4 indMemData3;
    UINT4 indMemData4;
    
    INT4  result;
       
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
    
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    switch(memSel) {
        case 0x0:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x2:
            if (memAddr > 7) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x3:
            if (memAddr > 0x7F) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x4:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x5:
            if (memAddr > 0x7F) { 
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
            
        case 0x6:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
            
        case 0x7:
            if (memAddr > 7) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x8:
            if (memAddr > 7) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x9:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xA:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xB:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xC:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        default :
          /* MEM_SEL is out of valid range */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }; 

    /* variable declaration */         
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_CMD;
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(indMemCtrlReg,
                                                                  slice);
    indMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_ADR;
    indMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(indMemAddrReg,
                                                                  slice);    
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memAddr to read */
    result = hyPhy20gRegFieldWrite(fileHndl, indMemAddrReg, 
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_ADR_MEM_ADDR_MSK, 
                          memAddr);
    if(result) return result;

    /* Specify the memSel and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                           memSel);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                           1);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          1);
    if(result) return result;    
      
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_0;
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg0,
                                                                 slice);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_1;
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg1,
                                                                 slice);    
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_2;
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg2,
                                                                 slice);   
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_3;
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg3,
                                                                 slice);   
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_4;
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg4,
                                                                 slice);   
    
    result = sysHyPhy20gRead(fileHndl, indMemDataReg0, &indMemData0);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg1, &indMemData1);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg2, &indMemData2);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg3, &indMemData3);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg4, &indMemData4);
    if(result) return result;


    *idata0 = indMemData0;
    *idata1 = indMemData1;
    *idata2 = indMemData2;
    *idata3 = indMemData3;
    *idata4 = indMemData4;


    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPOMemIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gRVCPOMemIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gRVCPOMemInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPOMemIndDump (struct file *fileHndl, void *pDumpBuff, 
                                        UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 i,j,slice, dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;
        
    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_0) >> 9;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (i=0; i<HYPHY20G_VCAT_RVCPO_MEM_IND_ENTRY_NUM; i++)
    {
        for (j=vcatHyPhy20gRVCPOMemInd[i].minAddr; j<=vcatHyPhy20gRVCPOMemInd[i].maxAddr; j++)
        {
            result = vcatHyPhy20gRVCPOMemIndRead(fileHndl,slice, vcatHyPhy20gRVCPOMemInd[i].memSel, j, 
                pDumpVal, pDumpVal+1, pDumpVal+2, pDumpVal+3, pDumpVal+4);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_VCAT_RVCPO_MEM_IND_WIDTH;
            pDumpVal += HYPHY20G_VCAT_RVCPO_MEM_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_VCAT_RVCPO_MEM_IND_WIDTH*sizeof(UINT4));
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gRVCPOMemIndDump */



/*******************************************************************************
**
**  vcatHyPhy20gRVCPOMemIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 4 bytes of data to the RVCPO MTSB Indirect Memory
**                 registers.
**
**  INPUTS:        *fileHndl  - base address
**                 slice      - valid values are:
**                              0x0, 0x1
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : Group Configuration Table 
**                              0x2 : DM Link Context Table 
**                              0x3 : Link Control Packet DCB Table
**                              0x4 : Link Delay Eval Status Table
**                              0x5 : Link DCB CRC-16 Table 
**                              0x6 : Link Statistics Table
**                              0x7 : DM Group Context Table 0 
**                              0x8 : DM Group Context Table 1 
**                              0x9 : DM Group Context Table 2 
**                              0xA : NE Group MST Buffer Tbale 
**                              0xB : OTN Control Packet Extraction
**                                    Context Table  
**                              0xC : DCB CRC16 Read Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              -----------------------------
**                              -  memSel  | memAddr range  -
**                              -   0x0    |   0x0 ~ 0x3    -
**                              -   0x1    |   0x0 ~ 0x3    -
**                              -   0x2    |      ---       -
**                              -   0x3    |   0x0 ~ 0x7F   -
**                              -   0x4    |   0x0 ~ 0x3    -
**                              -   0x5    |   0x0 ~ 0x7F   -
**                              -   0x6    |   0x0 ~ 0x3    -
**                              -   0x7    |   0x0 ~ 0x7    -
**                              -   0x8    |   0x0 ~ 0x7    -
**                              -   0x9    |   0x0 ~ 0x3    -
**                              -   0xA    |   0x0 ~ 0x3    -
**                              -   0xB    |   0x0 ~ 0x3    -
**                              -   0xC    |   0x0 ~ 0x3    -
**                              -----------------------------
**                 idata0       - data for reg 0 (bits 31:0)
**                 idata1       - data for reg 1 (bits 63:32)
**                 idata2       - data for reg 2 (bits 95:64)
**                 idata3       - data for reg 3 (bits 127:96)
**                 idata4       - data for reg 4 (bits 159:128)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPOMemIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memSel, UINT4 memAddr, UINT4 idata0,
                                  UINT4 idata1, UINT4 idata2, UINT4 idata3,
                                  UINT4 idata4)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemAddrReg;

    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    UINT4 indMemDataReg2;
    UINT4 indMemDataReg3;
    UINT4 indMemDataReg4;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    switch(memSel) {
        case 0x0:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;      
        case 0x2:
            if (memAddr > 7) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;                   
        case 0x3:
            if (memAddr > 0x7F) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x4:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x5:
            if (memAddr > 0x7F) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x6:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x7:
            if (memAddr > 7) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x8:
            if (memAddr > 7) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x9:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xA:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xB:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xC:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        default :
          /* MEM_SEL is out of valid range */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    };

    /* variable declaration */ 
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_CMD;
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(indMemCtrlReg,
                                                                  slice);
    indMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_ADR;
    indMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(indMemAddrReg,
                                                                  slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Write indirect access data to indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_0;
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg0,
                                                                 slice);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_1;
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg1,
                                                                 slice);
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_2;
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg2,
                                                                 slice);
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_3;
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg3,
                                                                 slice);
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_RVCPO_REG_IND_MEM_DAT_4;
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg4,
                                                                 slice);


    result = sysHyPhy20gWrite(fileHndl, indMemDataReg0, idata0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg1, idata1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg2, idata2);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg3, idata3);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg4, idata4);
    if(result) return result;

    /* Specify the memAddr to write */
    result = hyPhy20gRegFieldWrite(fileHndl, indMemAddrReg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_ADR_MEM_ADDR_MSK,
                          memAddr);
    if(result) return result;
 
    /* Specify the memSel and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                           memSel);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                           0);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;   
    
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result; 

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPOMemIndWrite */


/*******************************************************************************
**
**  vcatHyPhy20gRVCPOIntIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 4 bytes of RVCPO MTSB Indirect Interrupt Data.
**
**  INPUTS:        *fileHndl  - base address
**                 slice      - valid values are:
**                              0x0, 0x1
**                 memSel     - valid values are:
**                              0x0 : Link Interrupt Status Table
**                              0x1 : Group Interrupt Status Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0x3     -
**                              -   0x1    |   0x0 ~ 0x3     -
**                              ------------------------------
**                 *idata0    - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPOIntIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memSel, UINT4 memAddr, UINT4 *idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemAddrReg;

    UINT4 intMemDataReg0;
    UINT4 intMemData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    if(memSel == 0) {
        if(memAddr > 3) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (memSel == 1) {
        if(memAddr > 3) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
       return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_MEM_CMD;
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(intMemCtrlReg,
                                                                  slice);
    intMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_MEM_ADR;
    intMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(intMemAddrReg,
                                                                  slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memAddr to read */
    result = hyPhy20gRegFieldWrite(fileHndl, intMemAddrReg, 
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_ADR_INT_MEM_ADDR_MSK, 
                      memAddr);
    if(result) return result;
    
    /* Specify the memSel and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if(result) return result;
        
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_SEL_MSK,
                       memSel);
    if(result) return result; 
       
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                      1);
    if(result) return result;   
    
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_RWB_MSK,
                       1);
    if(result) return result;


    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_MEM_DAT;
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 intMemDataReg0,
                                                                 slice);

    result = sysHyPhy20gRead(fileHndl, intMemDataReg0, &intMemData0);
    if(result) return result;
    
    *idata0 = intMemData0;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPOIntIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gRVCPOIntIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gRVCPOIntInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPOIntIndDump(struct file *fileHndl, void *pDumpBuff, 
                                        UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 i,j,slice, dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_MEM_DAT) >> 9;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (i=0; i<HYPHY20G_VCAT_RVCPO_INT_IND_ENTRY_NUM; i++)
    {
        for (j=vcatHyPhy20gRVCPOIntInd[i].minAddr; j<=vcatHyPhy20gRVCPOIntInd[i].maxAddr; j++)
        {
            result = vcatHyPhy20gRVCPOIntIndRead(fileHndl,slice, vcatHyPhy20gRVCPOIntInd[i].memSel, j, 
                                                    pDumpVal);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_VCAT_RVCPO_INT_IND_WIDTH;
            pDumpVal += HYPHY20G_VCAT_RVCPO_INT_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_VCAT_RVCPO_INT_IND_WIDTH*sizeof(UINT4));
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gRVCPOIntIndDump */



/*******************************************************************************
**
**  vcatHyPhy20gRVCPOIntIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 4 bytes of data to the RVCPO MTSB Indirect Interrupt
**                 registers.
**
**  INPUTS:        *fileHndl  - base address
**                 slice      - valid values are:
**                              0x0, 0x1
**                 memSel     - valid values are:
**                              0x0 : Link Interrupt Status Table
**                              0x1 : Group Interrupt Status Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0x3     -
**                              -   0x1    |   0x0 ~ 0x3     -
**                              ------------------------------
**                 idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPOIntIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memSel, UINT4 memAddr, UINT4 idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemAddrReg;

    UINT4 intMemDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */ 
    if(memSel == 0) {
        if(memAddr > 3) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (memSel == 1) {
        if(memAddr > 3) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
       return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_MEM_CMD;
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(intMemCtrlReg,
                                                                  slice);
    intMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_MEM_ADR;
    intMemAddrReg = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(intMemAddrReg,
                                                                  slice);

    /* Poll BUSY bit to ensure register is ready for indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
      
    /* Write indirect access data to indirect data registers */    
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_MEM_DAT;
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(
                                                                 intMemDataReg0,
                                                                 slice);

    result = sysHyPhy20gWrite(fileHndl, intMemDataReg0, idata0);
    if(result) return result;   
    
    /* Specify the memAddr to read */
    result = hyPhy20gRegFieldWrite(fileHndl, intMemAddrReg, 
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_ADR_INT_MEM_ADDR_MSK, 
                      memAddr);
    if(result) return result;

    /* Specify the memSel and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_SEL_MSK,
                       memSel);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_RWB_MSK,
                       0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                      1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if(result) return result;
    
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                      HYPHY20G_VCAT_LCAS_RVCPO_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result; 

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPOIntIndWrite */


/*******************************************************************************
**
**  vcatHyPhy20gTVCPOMemIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 4 bytes of TVCPO MTSB Indirect Memory Data.
**
**  INPUTS:        *fileHndl  - base address
**                 slice      - valid values are:
**                              0x0, 0x1
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : FE Sink MST Buffer
**                              0x2 : Group Context Table
**                              0x3 : LCAS State Table
**                              0x4 : OH Table
**                              0x5 : Link Control Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0x3     -
**                              -   0x1    |   0x0 ~ 0x3     - 
**                              -   0x2    |   0x0 ~ 0xB     - 
**                              -   0x3    |   0x0 ~ 0x3     - 
**                              -   0x4    |   0x0 ~ 0x3     - 
**                              -   0x5    |   0x0 ~ 0x7     - 
**                              ------------------------------
**                 *idata0    - pointer to storage for data reg 0 (bits 31:0)
**                 *idata1    - pointer to storage for data reg 1 (bits 63:32)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPOMemIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memSel, UINT4 memAddr,UINT4 *idata0,
                                 UINT4 *idata1)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    UINT4 indMemData0;
    UINT4 indMemData1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    switch(memSel) {
        case 0x0:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x2:
            if (memAddr > 0xB) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x3:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x4:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x5:
            if (memAddr > 7) { 
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
            
        default :
            /* MEM_SEL is out of valid range */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }; 

    /* variable declaration */ 
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_MEM_CMD;
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(indMemCtrlReg,
                                                                  slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_ADDR_MSK, 
                          memAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                           memSel);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                           1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_MEM_DAT_OFFSET(0);
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg0,
                                                                 slice);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_MEM_DAT_OFFSET(1);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg1,
                                                                 slice);
    
    result = sysHyPhy20gRead(fileHndl, indMemDataReg0, &indMemData0);
    if(result) return result;
      
    result = sysHyPhy20gRead(fileHndl, indMemDataReg1, &indMemData1);
    if(result) return result;
      
    *idata0 = indMemData0;
    *idata1 = indMemData1;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPOMemIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gTVCPOMemIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gTVCPOMemInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPOMemIndDump (struct file *fileHndl, void *pDumpBuff, 
                                        UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 i,j,slice, dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_MEM_DAT_OFFSET(0)) >> 9;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (i=0; i<HYPHY20G_VCAT_TVCPO_MEM_IND_ENTRY_NUM; i++)
    {
        for (j=vcatHyPhy20gTVCPOMemInd[i].minAddr; j<=vcatHyPhy20gTVCPOMemInd[i].maxAddr; j++)
        {
            result = vcatHyPhy20gTVCPOMemIndRead(fileHndl,slice, vcatHyPhy20gTVCPOMemInd[i].memSel, j, 
                                                    pDumpVal, pDumpVal+1);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_VCAT_TVCPO_MEM_IND_WIDTH;
            pDumpVal += HYPHY20G_VCAT_TVCPO_MEM_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_VCAT_TVCPO_MEM_IND_WIDTH*sizeof(UINT4));
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gTVCPOMemIndDump */



/*******************************************************************************
**
**  vcatHyPhy20gTVCPOMemIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 4 bytes of data to the TVCPO MTSB Indirect Memory
**                 registers.
**
**  INPUTS:        *fileHndl  - base address
**                 slice      - valid values are:
**                              0x0, 0x1
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : FE Sink MST Buffer
**                              0x2 : Group Context Table
**                              0x3 : LCAS State Table
**                              0x4 : OH Table
**                              0x5 : Link Control Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0x3     -
**                              -   0x1    |   0x0 ~ 0x3     -
**                              -   0x2    |   0x0 ~ 0xB     -
**                              -   0x3    |      ---        -
**                              -   0x4    |   0x0 ~ 0x3     -
**                              -   0x5    |      ---        -
**                              ------------------------------
**                 idata0     - data for reg 0 (bits 31:0)
**                 idata1     - data for reg 1 (bits 63:32)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPOMemIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memSel, UINT4 memAddr, UINT4 idata0,
                                  UINT4 idata1)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    switch(memSel) {
        case 0x0:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
            
        case 0x2:
            if (memAddr > 0xB) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x4:
            if (memAddr > 3) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        default :
          /* MEM_SEL is out of valid range  */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    };

    /* variable declaration */ 
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_MEM_CMD;
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(indMemCtrlReg,
                                                                  slice);
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Write indirect access data to indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_MEM_DAT_OFFSET(0);
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg0,
                                                                 slice);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_MEM_DAT_OFFSET(1);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(
                                                                 indMemDataReg1,
                                                                 slice);
    
    result = sysHyPhy20gWrite(fileHndl, indMemDataReg0, idata0);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, indMemDataReg1, idata1);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                           0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                           memSel);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_ADDR_MSK, 
                          memAddr);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                          HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                          0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPOMemIndWrite */


/*******************************************************************************
**
**  vcatHyPhy20gTVCPOIntIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 4 bytes of TVCPO MTSB Indirect Interrupt Data.
**
**  INPUTS:        *fileHndl  - base address
**                 slice      - valid values are:
**                              0x0, 0x1
**                 memAddr    - valid value range: 0x0 ~ 0x3
**                 *idata0    - pointer to storage for data reg 0 (bits 31:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPOIntIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memAddr, UINT4 *idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemDataReg0;
    UINT4 intMemData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    if(memAddr > 0x3) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_INT_MEM_CMD;
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(intMemCtrlReg,
                                                                  slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                  HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                  0, HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                   HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_RWB_MSK,
                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
              HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_ADDR_REG_MSK, 
              memAddr);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                  HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                  1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                  HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                  0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_INT_MEM_DAT;
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(
                                                                 intMemDataReg0,
                                                                 slice);

    result = sysHyPhy20gRead(fileHndl, intMemDataReg0, &intMemData0);
    if(result) return result;

    *idata0 = intMemData0;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPOIntIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gRVCPOIntIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gRVCPOIntInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPOIntIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice, dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;
    UINT4 memAddr;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_INT_MEM_DAT) >> 9;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (memAddr=0; memAddr<=3; memAddr++)
    {
        result = vcatHyPhy20gTVCPOIntIndRead(fileHndl,slice, memAddr, pDumpVal);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_VCAT_TVCPO_INT_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_VCAT_TVCPO_INT_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gTVCPOIntIndDump */

/*******************************************************************************
**
**  vcatHyPhy20gTVCPOIntIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 4 bytes of data to the TVCPO MTSB Indirect Interrupt
**                 registers.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - valid values are:
**                               0x0, 0x1
**                 memAddr     - valid value range: 0x0 ~ 0x3
**                 idata0      - data for reg 0 (bits 31:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPOIntIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memAddr, UINT4 idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x1)
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    if(memAddr > 0x3) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_INT_MEM_CMD;
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(intMemCtrlReg,
                                                                  slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                  HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                  0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
      
    /* Write indirect access data to indirect data registers */
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_REG_IND_INT_MEM_DAT;
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(
                                                                 intMemDataReg0,
                                                                 slice);
    result = sysHyPhy20gWrite(fileHndl, intMemDataReg0, idata0);
    if(result) return result;


    /* Initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                   HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_RWB_MSK,
                   0);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
              HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_ADDR_REG_MSK, 
              memAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                  HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                  1);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                  HYPHY20G_VCAT_LCAS_TVCPO_BIT_IND_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                  0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPOIntIndWrite */


/*******************************************************************************
**
**  pifHyPhy20gRXCalProgIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads Slot Ram bits to provide the Channel ID programmed in
**                  the selected Calendar Slot for the receive flow control.
**
**  INPUTS:         *fileHndl   - base address
**                  rxCalAddr   - [7:0] bits denote the calendar slot index that
**                                should be programmed.
**                                [8] bit denotes the page that should be
**                                selected for programming.
**                                valid values : 0x0 ~ 0x1FF
**                  *idata0     - pointer to storage for data reg 0 (bits 31:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gRXCalProgIndRead(struct file *fileHndl, UINT4 rxCalAddr,
                                 UINT4 *idata0)
{
    UINT4 rxCalCtrlReg;
    UINT4 rxCalCtrlData;
    UINT4 rxCalDataReg0;
    UINT4 rxCalData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid rxCalAddr has been specified */
    if(rxCalAddr > 511) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */
    rxCalCtrlReg = HYPHY20G_PIF_PIF_REG_RX_CLDR_PROG;    

    /* Poll ACCESS_REQ bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, rxCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl,rxCalCtrlReg, &rxCalCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rxCalCtrlData,
                               HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_RWB_MSK,
                               1);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rxCalCtrlData,
                              HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ADDR_MSK,
                              rxCalAddr);
    if (result) return result;
      
    result = hyPhy20gVarFieldWrite(&rxCalCtrlData,
                        HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ACCESS_REQ_MSK,
                        1);
    if (result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, rxCalCtrlReg, rxCalCtrlData);
    if (result) return result;
    
    /*Poll ACCESS_REQ bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, rxCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data registers */
    rxCalDataReg0 = HYPHY20G_PIF_PIF_REG_RX_CLDR_PROG_DAT;
    result = sysHyPhy20gRead(fileHndl, rxCalDataReg0, &rxCalData0);
    if (result) return result;

    *idata0 = rxCalData0;

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gRXCalProgIndRead */

/*******************************************************************************
**
**  pifHyPhy20gRXCalProgIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the pifHyPhy20gRXCalProgInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gRXCalProgIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 rxCalAddr,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (rxCalAddr=0; rxCalAddr<=0x1FF; rxCalAddr++)
    {
        result = pifHyPhy20gRXCalProgIndRead(fileHndl, rxCalAddr, pDumpVal);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_PIF_RX_CAL_PROG_IND_WIDTH;
        pDumpVal += HYPHY20G_PIF_RX_CAL_PROG_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_PIF_RX_CAL_PROG_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* pifHyPhy20gRXCalProgIndDump */


/*******************************************************************************
**
**  pifHyPhy20gRXCalProgIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes to the Slot Ram bits to set the Channel ID in the
**                  desired Calendar Slot for the receive flow control.
**
**  INPUTS:         *fileHndl   - base address
**                  rxCalAddr   - [7:0] bits denote the calendar slot index that
**                                should be programmed.
**                                [8] bit denotes the page that should be
**                                selected for programming.
**                                valid values : 0x0 ~ 0x1FF
**                  idata0      - data for reg 0 (bits 31:0)
**
**  NOTE:           Only offline pages should be selected for programming.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gRXCalProgIndWrite(struct file *fileHndl, UINT4 rxCalAddr,
                                  UINT4 idata0)
{
    UINT4 rxCalCtrlReg;
    UINT4 rxCalCtrlData;
    UINT4 rxCalDataReg0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid rxCalAddr has been specified */
    if(rxCalAddr > 511) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    rxCalCtrlReg = HYPHY20G_PIF_PIF_REG_RX_CLDR_PROG;    

    /* Poll ACCESS_REQ bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, rxCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Write indirect access data to indirect data registers */
    rxCalDataReg0 = HYPHY20G_PIF_PIF_REG_RX_CLDR_PROG_DAT;
    result = sysHyPhy20gWrite(fileHndl, rxCalDataReg0, idata0);
    if (result) return result;
    
    /*Initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl,rxCalCtrlReg, &rxCalCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rxCalCtrlData,
                               HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_RWB_MSK,
                               0);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rxCalCtrlData,
                              HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ADDR_MSK,
                              rxCalAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rxCalCtrlData,
                        HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ACCESS_REQ_MSK,
                        1);
    if (result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, rxCalCtrlReg, rxCalCtrlData);
    if (result) return result;
    
    /* Poll ACCESS_REQ bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, rxCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_RX_CLDR_PROG_RX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gRXCalProgIndWrite */


/*******************************************************************************
**
**  pifHyPhy20gTXCalProgIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads Channel Ram bits to provide the Channel ID programmed
**                  in the selected Calendar Slot for the transmit flow control.
**
**  INPUTS:         *fileHndl   - base address
**                  txCalAddr   - [7:0] bits denote the calendar slot index that
**                                should be programmed.
**                                [8] bit denotes the page that should be
**                                selected for programming.
**                                valid values : 0x0 ~ 0x1FF
**                  *idata0      - pointer to storage for data reg 0 (bits 31:0)
**
**  NOTE:           Only offline pages should be selected for programming.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gTXCalProgIndRead(struct file *fileHndl, UINT4 txCalAddr,
                                 UINT4 *idata0)
{
    UINT4 txCalCtrlReg;
    UINT4 txCalCtrlData;
    UINT4 txCalDataReg0;
    UINT4 txCalData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid txCalAddr has been specified */
    if(txCalAddr > 511) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */
    txCalCtrlReg = HYPHY20G_PIF_PIF_REG_TX_CLDR_PROG;    

    /* Poll ACCESS_REQ bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, txCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl,txCalCtrlReg, &txCalCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&txCalCtrlData,
                               HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_RWB_MSK,
                               1);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&txCalCtrlData,
                              HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ADDR_MSK,
                              txCalAddr);
    if (result) return result;
      
    result = hyPhy20gVarFieldWrite(&txCalCtrlData,
                        HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ACCESS_REQ_MSK,
                        1);
    if (result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, txCalCtrlReg, txCalCtrlData);
    if (result) return result;
    
    /*Poll ACCESS_REQ bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, txCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data registers */
    txCalDataReg0 = HYPHY20G_PIF_PIF_REG_TX_CLDR_PROG_DAT;
    result = sysHyPhy20gRead(fileHndl, txCalDataReg0, &txCalData0);
    if (result) return result;
      
    *idata0 = txCalData0;

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gTXCalProgIndRead */

/*******************************************************************************
**
**  pifHyPhy20gTXCalProgIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the pifHyPhy20gTXCalProgInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gTXCalProgIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 txCalAddr,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (txCalAddr=0; txCalAddr<=0x1FF; txCalAddr++)
    {
        result = pifHyPhy20gTXCalProgIndRead(fileHndl, txCalAddr, pDumpVal);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_PIF_TX_CAL_PROG_IND_WIDTH;
        pDumpVal += HYPHY20G_PIF_TX_CAL_PROG_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_PIF_TX_CAL_PROG_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* pifHyPhy20gTXCalProgIndDump */


/*******************************************************************************
**
**  pifHyPhy20gTXCalProgIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes to the Slot Ram bits to set the Channel ID in the
**                  desired Calendar Slot for the receive flow control.
**
**  INPUTS:         *fileHndl   - base address
**                  txCalAddr   - [7:0] bits denote the calendar slot index that
**                                should be programmed.
**                                [8] bit denotes the page that should be
**                                selected for programming.
**                                valid values : 0x0 ~ 0x1FF
**                  idata0      - data for reg 0 (bits 31:0)
**
**  NOTE:           Only offline pages should be selected for programming.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gTXCalProgIndWrite(struct file *fileHndl, UINT4 txCalAddr,
                                  UINT4 idata0)
{
    UINT4 txCalCtrlReg;
    UINT4 txCalCtrlData;
    UINT4 txCalDataReg0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid txCalAddr has been specified */
    if(txCalAddr > 511) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    txCalCtrlReg = HYPHY20G_PIF_PIF_REG_TX_CLDR_PROG;    

    /* Poll ACCESS_REQ bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, txCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Write indirect access data to indirect data registers */
    txCalDataReg0 = HYPHY20G_PIF_PIF_REG_TX_CLDR_PROG_DAT;
    result = sysHyPhy20gWrite(fileHndl, txCalDataReg0, idata0);
    if (result) return result;
    
    /*Initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl,txCalCtrlReg, &txCalCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&txCalCtrlData,
                               HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_RWB_MSK,
                               0);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&txCalCtrlData,
                              HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ADDR_MSK,
                              txCalAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&txCalCtrlData,
                        HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ACCESS_REQ_MSK,
                        1);
    if (result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, txCalCtrlReg, txCalCtrlData);
    if (result) return result;
    
    /* Poll ACCESS_REQ bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, txCalCtrlReg,
                        HYPHY20G_PIF_PIF_BIT_TX_CLDR_PROG_TX_CAL_ACCESS_REQ_MSK,
                        0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gTXCalProgIndWrite */


/*******************************************************************************
**
**  cpbHyPhy20gWrPtrCtxtIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads CPB Write Pointer Indirect Context memory for a 
**                  specified channel.
**
**  INPUTS:         *fileHndl   - base address
**                  wrPtrCfgAddr- CPB channel number to access
**                                Valid values: 0x0 ~ 0x125
**                  *idata0     - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1     - pointer to storage for data reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gWrPtrCtxtIndRead(struct file *fileHndl, UINT4 wrPtrCfgAddr,
                                 UINT4 *idata0, UINT4 *idata1)
{
    UINT4 wrPtrCtrlReg;
    UINT4 wrPtrCtrlData;
    UINT4 wrPtrDataReg0;
    UINT4 wrPtrDataReg1;
    UINT4 wrPtrData0;
    UINT4 wrPtrData1;

    INT4  result;

    /* Argument Checking */
    /* Confirm a valid fifoCfgAddr has been specified */
    if(wrPtrCfgAddr > 293) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    wrPtrCtrlReg = HYPHY20G_CPB_CPB_REG_WR_PTR_CTXT_IND_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, wrPtrCtrlReg,
          HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_REQ_BUSY_MSK,
          0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Specify the fifoCfgAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, wrPtrCtrlReg, &wrPtrCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&wrPtrCtrlData,
              HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_ADDR_MSK,
              wrPtrCfgAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&wrPtrCtrlData,
            HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_RD_WRB_MSK,
            1);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&wrPtrCtrlData,
          HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_REQ_BUSY_MSK,
          1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, wrPtrCtrlReg, wrPtrCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, wrPtrCtrlReg,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data registers */
    wrPtrDataReg0 = HYPHY20G_CPB_CPB_REG_WR_PTR_CTXT_IND_DAT_OFFSET(0);
    wrPtrDataReg1 = HYPHY20G_CPB_CPB_REG_WR_PTR_CTXT_IND_DAT_OFFSET(1);

    result = sysHyPhy20gRead(fileHndl, wrPtrDataReg0, &wrPtrData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, wrPtrDataReg1, &wrPtrData1);
    if (result) return result;

    *idata0 = wrPtrData0;
    *idata1 = wrPtrData1;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gWrPtrCtxtIndRead */

/*******************************************************************************
**
**  cpbHyPhy20gWrPtrCtxtIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the cpbHyPhy20gWrPtrCtxtInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gWrPtrCtxtIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 wrPtrCfgAddr,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (wrPtrCfgAddr=0; wrPtrCfgAddr<=0x125; wrPtrCfgAddr++)
    {
        result = cpbHyPhy20gWrPtrCtxtIndRead(fileHndl, wrPtrCfgAddr, pDumpVal, pDumpVal+1);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_CPB_WR_PTR_CTXT_IND_WIDTH;
        pDumpVal += HYPHY20G_CPB_WR_PTR_CTXT_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_CPB_WR_PTR_CTXT_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* cpbHyPhy20gWrPtrCtxtIndDump */


/*******************************************************************************
**
**  cpbHyPhy20gWrPtrCtxtIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes CPB Write Pointer Indirect Context memory for a 
**                  specified channel.
**
**  INPUTS:         *fileHndl   - base address
**                  wrPtrCfgAddr- CPB channel number to access
**                                Valid values: 0x0 ~ 0x125.
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gWrPtrCtxtIndWrite(struct file *fileHndl, UINT4 wrPtrCfgAddr,
                                  UINT4 idata0, UINT4 idata1)
{
    UINT4 wrPtrCtrlReg;
    UINT4 wrPtrCtrlData;
    UINT4 wrPtrDataReg0;
    UINT4 wrPtrDataReg1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid fifoCfgAddr has been specified */
    if(wrPtrCfgAddr > 293) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    wrPtrCtrlReg = HYPHY20G_CPB_CPB_REG_WR_PTR_CTXT_IND_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, wrPtrCtrlReg,
          HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_REQ_BUSY_MSK,
          0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    wrPtrDataReg0 = HYPHY20G_CPB_CPB_REG_WR_PTR_CTXT_IND_DAT_OFFSET(0);
    wrPtrDataReg1 = HYPHY20G_CPB_CPB_REG_WR_PTR_CTXT_IND_DAT_OFFSET(1);

    result = sysHyPhy20gWrite(fileHndl, wrPtrDataReg0, idata0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, wrPtrDataReg1, idata1);
    if (result) return result;

    /* Specify the fifoCfgAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, wrPtrCtrlReg, &wrPtrCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&wrPtrCtrlData,
              HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_ADDR_MSK,
              wrPtrCfgAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&wrPtrCtrlData,
            HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_RD_WRB_MSK,
            0);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&wrPtrCtrlData,
          HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_REQ_BUSY_MSK,
          1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, wrPtrCtrlReg, wrPtrCtrlData);
    if (result) return result;
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, wrPtrCtrlReg,
          HYPHY20G_CPB_CPB_BIT_WR_PTR_CTXT_IND_CTL_WR_PTR_INDIRECT_REQ_BUSY_MSK,
          0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gWrPtrCtxtIndWrite */

/*******************************************************************************
**
**  cpbHyPhy20gRdPtrCtxtIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads CPB Read Pointer Indirect Context memory for a 
**                  specified channel.
**
**  INPUTS:         *fileHndl   - base address
**                  rdPtrCfgAddr- CPB channel number to access
**                                Valid values: 0x0 ~ 0x128
**                  *idata0     - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1     - pointer to storage for data reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gRdPtrCtxtIndRead(struct file *fileHndl, UINT4 rdPtrCfgAddr,
                                 UINT4 *idata0, UINT4 *idata1)
{
    UINT4 rdPtrCtrlReg;
    UINT4 rdPtrCtrlData;
    UINT4 rdPtrDataReg0;
    UINT4 rdPtrDataReg1;
    UINT4 rdPtrData0;
    UINT4 rdPtrData1;

    INT4  result;

    /* Argument Checking */
    /* Confirm a valid fifoCfgAddr has been specified */
    if(rdPtrCfgAddr > 296) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    rdPtrCtrlReg = HYPHY20G_CPB_CPB_REG_RD_PTR_CTXT_IND_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, rdPtrCtrlReg,
          HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_REQ_BUSY_MSK,
          0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Specify the fifoCfgAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, rdPtrCtrlReg, &rdPtrCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rdPtrCtrlData,
              HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_ADDR_MSK,
              rdPtrCfgAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rdPtrCtrlData,
            HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_RD_WRB_MSK,
            1);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rdPtrCtrlData,
          HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_REQ_BUSY_MSK,
          1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, rdPtrCtrlReg, rdPtrCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, rdPtrCtrlReg,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data registers */
    rdPtrDataReg0 = HYPHY20G_CPB_CPB_REG_RD_PTR_CTXT_IND_DAT_OFFSET(0);
    rdPtrDataReg1 = HYPHY20G_CPB_CPB_REG_RD_PTR_CTXT_IND_DAT_OFFSET(1);

    result = sysHyPhy20gRead(fileHndl, rdPtrDataReg0, &rdPtrData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, rdPtrDataReg1, &rdPtrData1);
    if (result) return result;

    *idata0 = rdPtrData0;
    *idata1 = rdPtrData1;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gRdPtrCtxtIndRead */

/*******************************************************************************
**
**  cpbHyPhy20gRdPtrCtxtIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the cpbHyPhy20gRdPtrCtxtInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gRdPtrCtxtIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 rdPtrCfgAddr,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (rdPtrCfgAddr=0; rdPtrCfgAddr<=0x128; rdPtrCfgAddr++)
    {
        result = cpbHyPhy20gRdPtrCtxtIndRead(fileHndl, rdPtrCfgAddr, pDumpVal, pDumpVal+1);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_CPB_RD_PTR_CTXT_IND_WIDTH;
        pDumpVal += HYPHY20G_CPB_RD_PTR_CTXT_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_CPB_RD_PTR_CTXT_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* cpbHyPhy20gRdPtrCtxtIndDump */


/*******************************************************************************
**
**  cpbHyPhy20gRdPtrCtxtIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes CPB Read Pointer Indirect Context memory for a 
**                  specified channel.
**
**  INPUTS:         *fileHndl   - base address
**                  rdPtrCfgAddr- CPB channel number to access
**                                Valid values: 0x0 ~ 0x128
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gRdPtrCtxtIndWrite(struct file *fileHndl, UINT4 rdPtrCfgAddr,
                                  UINT4 idata0, UINT4 idata1)
{
    UINT4 rdPtrCtrlReg;
    UINT4 rdPtrCtrlData;
    UINT4 rdPtrDataReg0;
    UINT4 rdPtrDataReg1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid fifoCfgAddr has been specified */
    if(rdPtrCfgAddr > 296) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    rdPtrCtrlReg = HYPHY20G_CPB_CPB_REG_RD_PTR_CTXT_IND_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, rdPtrCtrlReg,
          HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_REQ_BUSY_MSK,
          0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    rdPtrDataReg0 = HYPHY20G_CPB_CPB_REG_RD_PTR_CTXT_IND_DAT_OFFSET(0);
    rdPtrDataReg1 = HYPHY20G_CPB_CPB_REG_RD_PTR_CTXT_IND_DAT_OFFSET(1);

    result = sysHyPhy20gWrite(fileHndl, rdPtrDataReg0, idata0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, rdPtrDataReg1, idata1);
    if (result) return result;

    /* Specify the fifoCfgAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, rdPtrCtrlReg, &rdPtrCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rdPtrCtrlData,
              HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_ADDR_MSK,
              rdPtrCfgAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rdPtrCtrlData,
            HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_RD_WRB_MSK,
            0);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&rdPtrCtrlData,
          HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_REQ_BUSY_MSK,
          1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, rdPtrCtrlReg, rdPtrCtrlData);
    if (result) return result;
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, rdPtrCtrlReg,
          HYPHY20G_CPB_CPB_BIT_RD_PTR_CTXT_IND_CTL_RD_PTR_INDIRECT_REQ_BUSY_MSK,
          0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gRdPtrCtxtIndWrite */


/*******************************************************************************
**
**  cpbHyPhy20gFifoDepthIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads CPB FIFO Depth Indirect memory for a specified 
**                  channel.
**
**  INPUTS:         *fileHndl   - base address
**                  fifoCfgAddr - CPB channel number to access
**                                Valid values: 0x0 ~ 0x125
**                  *idata0     - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1     - pointer to storage for data reg 1 (bits 63:32)
**                  *idata2     - pointer to storage for data reg 2 (bits 95:64)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gFifoDepthIndRead(struct file *fileHndl, UINT4 fifoCfgAddr,
                                 UINT4 *idata0, UINT4 *idata1, UINT4 *idata2)
{
    UINT4 fifoDpthCtrlReg;
    UINT4 fifoDpthCtrlData;
    UINT4 fifoDpthDataReg0;
    UINT4 fifoDpthDataReg1;
    UINT4 fifoDpthDataReg2;
    UINT4 fifoDpthData0;
    UINT4 fifoDpthData1;
    UINT4 fifoDpthData2;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid fifoCfgAddr has been specified */
    if(fifoCfgAddr > 293) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    fifoDpthCtrlReg = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, fifoDpthCtrlReg,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Specify the fifoCfgAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, fifoDpthCtrlReg, &fifoDpthCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&fifoDpthCtrlData,
           HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_ADDR_MSK,
           fifoCfgAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&fifoDpthCtrlData,
         HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_RD_WRB_MSK,
         1);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&fifoDpthCtrlData,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, fifoDpthCtrlReg, fifoDpthCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, fifoDpthCtrlReg,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data registers */
    fifoDpthDataReg0 = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_DAT_OFFSET(0);
    fifoDpthDataReg1 = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_DAT_OFFSET(1);
    fifoDpthDataReg2 = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_DAT_OFFSET(2);

    result = sysHyPhy20gRead(fileHndl, fifoDpthDataReg0, &fifoDpthData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, fifoDpthDataReg1, &fifoDpthData1);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, fifoDpthDataReg2, &fifoDpthData2);
    if (result) return result;

    *idata0 = fifoDpthData0;
    *idata1 = fifoDpthData1;
    *idata2 = fifoDpthData2;

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gFifoDepthIndRead */

/*******************************************************************************
**
**  cpbHyPhy20gFifoDepthIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the cpbHyPhy20gFifoDepthInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gFifoDepthIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 fifoCfgAddr,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (fifoCfgAddr = 0; fifoCfgAddr<= 0x125; fifoCfgAddr++)
    {
        result = cpbHyPhy20gFifoDepthIndRead(fileHndl, fifoCfgAddr, pDumpVal, 
                                                pDumpVal+1, pDumpVal+2);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_CPB_FIFO_DEPTH_IND_WIDTH;
        pDumpVal += HYPHY20G_CPB_FIFO_DEPTH_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_CPB_FIFO_DEPTH_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* cpbHyPhy20gFifoDepthIndDump */

/*******************************************************************************
**
**  cpbHyPhy20gFifoDepthIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes CPB FIFO Depth Indirect memory for a specified 
**                  channel.
**
**  INPUTS:         *fileHndl   - base address
**                  fifoCfgAddr - CPB channel number to access
**                                Valid values: 0x0 ~ 0x125
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**                  idata2      - data for reg 2 (bits 95:64)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cpbHyPhy20gFifoDepthIndWrite(struct file *fileHndl, UINT4 fifoCfgAddr,
                                  UINT4 idata0, UINT4 idata1, UINT4 idata2)
{
    UINT4 fifoDpthCtrlReg;
    UINT4 fifoDpthCtrlData;
    UINT4 fifoDpthDataReg0;
    UINT4 fifoDpthDataReg1;
    UINT4 fifoDpthDataReg2;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid fifoCfgAddr has been specified */
    if(fifoCfgAddr > 293) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    fifoDpthCtrlReg = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, fifoDpthCtrlReg,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    fifoDpthDataReg0 = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_DAT_OFFSET(0);
    fifoDpthDataReg1 = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_DAT_OFFSET(1);
    fifoDpthDataReg2 = HYPHY20G_CPB_CPB_REG_FIFO_DPTH_IND_DAT_OFFSET(2);

    result = sysHyPhy20gWrite(fileHndl, fifoDpthDataReg0, idata0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, fifoDpthDataReg1, idata1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, fifoDpthDataReg2, idata2);
    if (result) return result;

    /* Specify the fifoCfgAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, fifoDpthCtrlReg, &fifoDpthCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&fifoDpthCtrlData,
         HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_RD_WRB_MSK,
         0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&fifoDpthCtrlData,
           HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_ADDR_MSK,
           fifoCfgAddr);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&fifoDpthCtrlData,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, fifoDpthCtrlReg, fifoDpthCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, fifoDpthCtrlReg,
       HYPHY20G_CPB_CPB_BIT_FIFO_DPTH_IND_CTL_FIFO_CONFIG_INDIRECT_REQ_BUSY_MSK,
       0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gFifoDepthIndWrite */

/*******************************************************************************
**
**  gfpfHyPhy20gTXDPChanIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads GFP_FRM TXDP64 Indirect Channel Control and Status
**                  register which provides control and status bits to access 
**                  the per-channel indirect registers.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - valid values are:
**                                0x0, 0x1
**                  cfgChan     - GFP FRM channel number to access
**                                Valid values: 0x0 ~ 0x3F.
**                  *idata0     - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1     - pointer to storage for data reg 0 (bits 63:32)
**                  *idata2     - pointer to storage for data reg 0 (bits 95:64)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPChanIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 cfgChan, UINT4 *idata0, UINT4 *idata1,
                                 UINT4 *idata2)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    UINT4 indChanDataReg2;
    UINT4 indChanData0;
    UINT4 indChanData1;
    UINT4 indChanData2;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /*Specify the cfgChan and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_CHAN_MSK,
                   cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                    HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_RWB_MSK,
                    1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_BUSY_MSK,
                   1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data  registers */
    indChanDataReg0 = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_COM_CFG_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                             
    indChanDataReg1 = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_COM_CFG_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);

    indChanDataReg2 = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_COM_CFG_OFFSET(2);
    indChanDataReg2 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg2,
                                                             slice);
                                                             
    result = sysHyPhy20gRead(fileHndl, indChanDataReg0, &indChanData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indChanDataReg1, &indChanData1);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indChanDataReg2, &indChanData2);
    if (result) return result;

    *idata0 = indChanData0;
    *idata1 = indChanData1;
    *idata2 = indChanData2;

    return HYPHY20G_SUCCESS;
} /* gfpfHyPhy20gTXDPChanIndRead */

/*******************************************************************************
**
**  gfpfHyPhy20gTXDPChanIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfpfHyPhy20gTXDPChanInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPChanIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 cfgChan, slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_COM_CFG_OFFSET(0))>>11;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (cfgChan=0; cfgChan<=0x3F; cfgChan++)
    {
        result = gfpfHyPhy20gTXDPChanIndRead(fileHndl, slice, cfgChan, pDumpVal,
                                                pDumpVal+1, pDumpVal+2);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_GFPF_TXDP_CHAN_IND_WIDTH;
        pDumpVal += HYPHY20G_GFPF_TXDP_CHAN_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_GFPF_TXDP_CHAN_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfpfHyPhy20gTXDPChanIndDump */


/*******************************************************************************
**
**  gfpfHyPhy20gTXDPChanIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes GFP_FRM TXDP64 Indirect Channel Control and Status
**                  register which provides control and status bits to access 
**                  the per-channel indirect registers.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - valid values are:
**                                0x0, 0x1
**                  cfgChan     - GFP FRM channel number to access
**                                Valid values: 0x0 ~ 0x3F.
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**                  idata2      - data for reg 2 (bits 95:64)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPChanIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 cfgChan, UINT4 idata0, UINT4 idata1,
                                  UINT4 idata2)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    UINT4 indChanDataReg2;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    indChanDataReg0 = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_COM_CFG_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                             
    indChanDataReg1 = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_COM_CFG_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);

    indChanDataReg2 = HYPHY20G_GFP_FRM_TXDP_REG_IND_CHNL_COM_CFG_OFFSET(2);
    indChanDataReg2 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg2,
                                                             slice);

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg0, idata0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg1, idata1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg2, idata2);
    if (result) return result;

    /* Specify the cfgChan and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_CHAN_MSK,
                   cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                    HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_RWB_MSK,
                    0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_BUSY_MSK,
                   1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_TXDP_BIT_IND_CHNL_CTL_AND_STAT_CFG_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* gfpfHyPhy20gTXDPChanIndWrite */

/*******************************************************************************
**
**  gfpfHyPhy20gTXDPExtHdrIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads GFP_FRM Indirect Extension Header / CMF / Pause Memory 
**                Control bits to provide the control and status bits in order 
**                to access the per-channel Client Data Extension memory/Client
**                Management Frame, Ethernet Pause Frame and HDLC Control
**                Frame processing memory HDR_CFRM_DAT. 
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0, 0x1
**                Chan        - GFP FRM channel number to access
**                              Valid values: 0x0 ~ 0x3F.
**                cfrmPriority- This control bit indicates the priority of GFP
**                              Client Management Frames(CMF) or HDLC Control
**                              Frames.
**                              1: Indicates High Priorty Insertion, GFP CMF/
**                                 HDLC Control frames are inserted in data
**                                 stream at the end of current frame, even if
**                                 there is FDI data to be sent.
**                                 Note: cfrmPriority bit is always set to 1
**                                 during Pause Frame Processing. 
**                gfpCfrm     - This control bit indicates the type of reads
**                              from the GFP/HDLC Client Data extension memory
**                              or Client Management/Control Frame Memory.
**                              Valid values:
**                              3: access HDLC Control frame memory location.
**                              2: access HDLC Header memory locations  
**                              1: access GFP Client Management / Pause frame
**                                 memory location.
**                              0: access GFP Client Data Frame 
**                                 memory locations  
**                burstInit   - Access the Client Data Extension memory/Client
**                              Management Frame memory.
**                burstEnd    - This bit should be set to indiate the last
**                              byte of the burst of either Extension Header 
**                              of Client Management Frame. 
**                              The valid BURST_END[3:0] configuration: 
**                              ------------------------------------------------
**                              -BURST_END|      ExtenHd/CMF                   -
**                              -  0000   | HDR_CFRM_DAT[31:0] valid           -
**                              -  0001   | HDR_CFRM_DAT[7:0] last valid byte  -
**                              -  0010   | HDR_CFRM_DAT[15:8] last valid byte -
**                              -  0100   | HDR_CFRM_DAT[23:16] last valid byte-
**                              -  1000   | HDR_CFRM_DAT[31:24] last valid byte-
**                              - others  | Invalid                            -
**                              ------------------------------------------------
**                *idata0     - pointer to storage for data reg 0 (bits 31:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPExtHdrIndRead(struct file *fileHndl, UINT4 slice,
                                   UINT4 Chan, UINT4 cfrmPriority,
                                   UINT4 gfpCfrm, UINT4 burstInit,
                                   UINT4 burstEnd, UINT4 *idata0)
{
    UINT4 indExtHdrCtrlReg;
    UINT4 indExtHdrCtrlData;
    UINT4 indExtHdrDataReg0;
    UINT4 indExtHdrData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid Chan has been specified */
    if(Chan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* Confirm a valid burstEnd has been specified */
    if( burstEnd != 0x0 && burstEnd != 0x1 && burstEnd != 0x2 && burstEnd != 0x4
      && burstEnd != 0x8) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Variable declaration */
    indExtHdrCtrlReg = HYPHY20G_GFP_FRM_TXDP_REG_IND_EXTN_HDR_CMF_MEM_CTL;
    indExtHdrCtrlReg = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indExtHdrCtrlReg,
                                                              slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indExtHdrCtrlReg,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_BUSY_MSK,
               0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Specify the Chan, cfrmPriority, gfpCfrm, burstInit, burstEnd, and 
       initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indExtHdrCtrlReg, &indExtHdrCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
            HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_CHAN_ID_MSK,
            Chan);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
           HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_CFRM_PRIORITY_MSK,
           cfrmPriority);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
                HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_GFP_CFRM_MSK,
                gfpCfrm);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
              HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_BURST_INIT_MSK,
              burstInit);
    if (result) return result;
 
    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_BURST_END_MSK,
               burstEnd);
    if (result) return result;
 
    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
                HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_RWB_MSK,
                1);
    if (result) return result; 
 
    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_BUSY_MSK,
               1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indExtHdrCtrlReg, indExtHdrCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read. */
    result = sysHyPhy20gPollBit(fileHndl, indExtHdrCtrlReg,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_BUSY_MSK,
               0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Read indirect access data from indirect data register */
    indExtHdrDataReg0 = HYPHY20G_GFP_FRM_TXDP_REG_IND_EXTN_HDR_CMF_MEM_DAT;
    indExtHdrDataReg0 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(
                                                              indExtHdrDataReg0,
                                                              slice);
                                                             
    result = sysHyPhy20gRead(fileHndl, indExtHdrDataReg0, &indExtHdrData0);
    if (result) return result;

    *idata0 = indExtHdrData0;

    return HYPHY20G_SUCCESS;
} /* gfpfHyPhy20gTXDPExtHdrIndRead */

/*******************************************************************************
**
**  gfpfHyPhy20gTXDPExtHdrIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfpfHyPhy20gTXDPExtHdrInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPExtHdrIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[5];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_FRM_TXDP_REG_IND_EXTN_HDR_CMF_MEM_DAT)>>11;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /*loop the Chan from 0 to 63*/
    for (params[0]=0; params[0]<=0x3F; params[0]++)
    {
        /*loop the cfrmPriority from 0 to 1*/
        for (params[1]=0; params[1]<=1; params[1]++)
        {
            /*loop the gfpCfrm from 0 to 3*/
            for (params[2]=0; params[2]<=3; params[2]++)
            {
                /*loop the burstInit from 0 to 1*/
                for (params[3]=0; params[3]<=1; params[3]++)
                {
                    /*loop the burstEnd from 0 to 8, but jump over 3, 5-7*/
                    for (params[4]=0; params[4]<=8; params[4]++)
                    {
                        if ( (params[4]==3)||((params[4]>=5)&&(params[4]<=7)) )
                            continue;
                        result = gfpfHyPhy20gTXDPExtHdrIndRead(fileHndl, slice, params[0], params[1],
                                                        params[2], params[3], params[4], pDumpVal);
                        if(result) 
                        {
                            pCfgDumpReq->rtCode = result;
                            (*pValidDumpBuffSize) = dumpBuffSize;
                            return result;
                        }
                        pCfgDumpReq->length += HYPHY20G_GFPF_TXDP_EXT_HDR_IND_WIDTH;
                        pDumpVal += HYPHY20G_GFPF_TXDP_EXT_HDR_IND_WIDTH;
                        dumpBuffSize += (HYPHY20G_GFPF_TXDP_EXT_HDR_IND_WIDTH*sizeof(UINT4));                    
                    }
                }
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfpfHyPhy20gTXDPExtHdrIndDump */


/*******************************************************************************
**
**  gfpfHyPhy20gTXDPExtHdrIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes GFP_FRM Indirect Extension Header / CMF / Pause Memory 
**                Control bits to provide the control and status bits in order 
**                to access the per-channel Client Data Extension memory/Client
**                Management Frame, Ethernet Pause Frame and HDLC Control
**                Frame processing memory HDR_CFRM_DAT. 
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0, 0x1
**                Chan        - GFP FRM channel number to access
**                              Valid values: 0x0 ~ 0x3F.
**                cfrmPriority- This control bit indicates the priority of GFP
**                              Client Management Frames(CMF) or HDLC Control
**                              Frames.
**                              1: Indicates High Priorty Insertion, GFP CMF/
**                                 HDLC Control frames are inserted in data
**                                 stream at the end of current frame, even if
**                                 there is FDI data to be sent.
**                                 Note: cfrmPriority bit is always set to 1
**                                 during Pause Frame Processing. 
**                gfpCfrm     - This control bit indicates the type of writes
**                              to the GFP/HDLC Client Data extension memory
**                              or Client Management/Control Frame Memory.
**                              Valid values:
**                              3: access HDLC Control frame memory location.
**                              2: access HDLC Header memory locations  
**                              1: access GFP Client Management / Pause frame
**                                 memory location.
**                              0: access GFP Client Data Frame 
**                                 memory locations  
**                burstInit   - Access the Client Data Extension memory/Client
**                              Management Frame memory.
**                burstEnd    - This bit should be set to indiate the last
**                              byte of the burst of either Extension Header 
**                              of Client Management Frame. 
**                              The valid BURST_END[3:0] configuration: 
**                              ------------------------------------------------
**                              -BURST_END|      ExtenHd/CMF                   -
**                              -  0000   | HDR_CFRM_DAT[31:0] valid           -
**                              -  0001   | HDR_CFRM_DAT[7:0] last valid byte  -
**                              -  0010   | HDR_CFRM_DAT[15:8] last valid byte -
**                              -  0100   | HDR_CFRM_DAT[23:16] last valid byte-
**                              -  1000   | HDR_CFRM_DAT[31:24] last valid byte-
**                              - others  | Invalid                            -
**                              ------------------------------------------------
**                idata0      - data for reg 0 (bits 31:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPExtHdrIndWrite(struct file *fileHndl, UINT4 slice,
                                    UINT4 Chan, UINT4 cfrmPriority,
                                    UINT4 gfpCfrm, UINT4 burstInit,
                                    UINT4 burstEnd, UINT4 idata0)
{
    UINT4 indExtHdrCtrlReg;
    UINT4 indExtHdrCtrlData;
    UINT4 indExtHdrDataReg0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid Chan has been specified */
    if(Chan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* Confirm a valid burstEnd has been specified */
    if( burstEnd != 0x0 && burstEnd != 0x1 && burstEnd != 0x2 && burstEnd != 0x4
      && burstEnd != 0x8) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Variable declaration */
    indExtHdrCtrlReg = HYPHY20G_GFP_FRM_TXDP_REG_IND_EXTN_HDR_CMF_MEM_CTL;
    indExtHdrCtrlReg = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indExtHdrCtrlReg,
                                                              slice);
                                                              
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indExtHdrCtrlReg,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_BUSY_MSK,
               0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data register */
    indExtHdrDataReg0 = HYPHY20G_GFP_FRM_TXDP_REG_IND_EXTN_HDR_CMF_MEM_DAT;
    indExtHdrDataReg0 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(
                                                              indExtHdrDataReg0,
                                                              slice);
                                                              
    result = sysHyPhy20gWrite(fileHndl, indExtHdrDataReg0, idata0);
    if (result) return result;

    /* Specify the Chan, cfrmPriority, gfpCfrm, burstInit, burstEnd, and 
       initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indExtHdrCtrlReg, &indExtHdrCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
            HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_CHAN_ID_MSK,
            Chan);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
           HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_CFRM_PRIORITY_MSK,
           cfrmPriority);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
                HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_GFP_CFRM_MSK,
                gfpCfrm);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
              HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_BURST_INIT_MSK,
              burstInit);
    if (result) return result;
 
    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_BURST_END_MSK,
               burstEnd);
    if (result) return result;
 
    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
                HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_RWB_MSK,
                0);
    if (result) return result; 
 
    result = hyPhy20gVarFieldWrite(&indExtHdrCtrlData,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_BUSY_MSK,
               1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indExtHdrCtrlReg, indExtHdrCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indExtHdrCtrlReg,
               HYPHY20G_GFP_FRM_TXDP_BIT_IND_EXTN_HDR_CMF_MEM_CTL_EXTN_BUSY_MSK,
               0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* gfpfHyPhy20gTXDPExtHdrIndWrite */


/*******************************************************************************
**
**  gfpfHyPhy20gTXDPPerfMonIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads Indirect Performance Monitor bits to provide the
**                  per-channel performance monitoring registers. The parameter
**                  'trigger' may be set to 1 to initiate the transfer of PMON 
**                  data to indirect memory. It need only be called once for all
**                  channels and then individual channel data may be read by 
**                  calling the function with 'trigger' set to 0.

**  INPUTS:         *fileHndl - base address
**                  slice     - valid values are:
**                              0x0, 0x1
**                  cfgChan   - GFP_FRM channel number to access
**                              Valid values: 0x0 ~ 0x3F.
**                              cfgChan data is valid for trigger settings of 
**                              0 or 1
**                  trigger    - Indirect access type
**                             1 - initiate capture of PMON data for all 
**                                 channels
**                             0 - access captured data for cfgChan
**
**                  *idata0   - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1   - pointer to storage for data reg 1 (bits 63:32)
**                  *idata2   - pointer to storage for data reg 2 (bits 95:64)
**                  *idata3   - pointer to storage for data reg 3 (bits 127:96)
**                  *idata4   - pointer to storage for data reg 4 (bits 159:128)
**                  *idata5   - pointer to storage for data reg 5 (bits 191:160)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPPerfMonIndRead(struct file *fileHndl, UINT4 slice, 
                                    UINT4 cfgChan, UINT4 trigger, UINT4 *idata0, 
                                    UINT4 *idata1, UINT4 *idata2, UINT4 *idata3,
                                    UINT4 *idata4, UINT4 *idata5)
{
    UINT4 pmonCtrlReg;
    UINT4 pmonCtrlData;
    UINT4 pmonDataReg0;
    UINT4 pmonDataReg1;
    UINT4 pmonDataReg2;
    UINT4 pmonDataReg3;
    UINT4 pmonDataReg4;
    UINT4 pmonDataReg5;
    UINT4 pmonData0;
    UINT4 pmonData1;
    UINT4 pmonData2;
    UINT4 pmonData3;
    UINT4 pmonData4;
    UINT4 pmonData5;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Variable declaration */
    pmonCtrlReg = HYPHY20G_GFP_FRM_TXDP_REG_IND_PMON_CTL;
    pmonCtrlReg = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(pmonCtrlReg, slice);
    
                                                            
  /* Poll BUSY bit to ensure register is ready indirect access */
  result = sysHyPhy20gPollBit(fileHndl, pmonCtrlReg,
                       HYPHY20G_GFP_FRM_TXDP_BIT_IND_PMON_CTL_PMON_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
  if (result) return result;

  /* Initiate the indirect read access */
  result = sysHyPhy20gRead(fileHndl, pmonCtrlReg, &pmonCtrlData);
  if (result) return result;

  result = hyPhy20gVarFieldWrite(&pmonCtrlData,
                    HYPHY20G_GFP_FRM_TXDP_BIT_IND_PMON_CTL_PMON_CHAN_ID_MSK,
                    cfgChan);
  if (result) return result;

  /* if triggering capture set the BUSY bit otherwise do not */
    if (trigger == 1) {       
      result = hyPhy20gVarFieldWrite(&pmonCtrlData,
                           HYPHY20G_GFP_FRM_TXDP_BIT_IND_PMON_CTL_PMON_BUSY_MSK,
                           1);
      if (result) return result;  
    } else {
      result = hyPhy20gVarFieldWrite(&pmonCtrlData,
                           HYPHY20G_GFP_FRM_TXDP_BIT_IND_PMON_CTL_PMON_BUSY_MSK,
                           0);
      if (result) return result;        
    }
    
    result = sysHyPhy20gWrite(fileHndl, pmonCtrlReg, pmonCtrlData);
    if (result) return result;
       
    if (trigger == 1) {         
    /* If triggering capture Poll BUSY bit to ensure data is ready to be 
       read */
    result = sysHyPhy20gPollBit(fileHndl, pmonCtrlReg,
                         HYPHY20G_GFP_FRM_TXDP_BIT_IND_PMON_CTL_PMON_BUSY_MSK,
                         0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
  }   

  /* Read indirect access data from indirect data registers */
  pmonDataReg0 = HYPHY20G_GFP_FRM_TXDP_REG_PERF_MONING_STAT_DAT_OFFSET(0);
  pmonDataReg0 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(pmonDataReg0, slice); 
   
  pmonDataReg1 = HYPHY20G_GFP_FRM_TXDP_REG_PERF_MONING_STAT_DAT_OFFSET(1);
  pmonDataReg1 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(pmonDataReg1, slice);

  pmonDataReg2 = HYPHY20G_GFP_FRM_TXDP_REG_PERF_MONING_STAT_DAT_OFFSET(2);
  pmonDataReg2 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(pmonDataReg2, slice);

  pmonDataReg3 = HYPHY20G_GFP_FRM_TXDP_REG_PERF_MONING_STAT_DAT_OFFSET(3);
  pmonDataReg3 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(pmonDataReg3, slice);

  pmonDataReg4 = HYPHY20G_GFP_FRM_TXDP_REG_PERF_MONING_STAT_DAT_OFFSET(4);
  pmonDataReg4 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(pmonDataReg4, slice);

  pmonDataReg5 = HYPHY20G_GFP_FRM_TXDP_REG_PERF_MONING_STAT_DAT_OFFSET(5);
  pmonDataReg5 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(pmonDataReg5, slice);

  result = sysHyPhy20gRead(fileHndl, pmonDataReg0, &pmonData0);
  if (result) return result;

  result = sysHyPhy20gRead(fileHndl, pmonDataReg1, &pmonData1);
  if (result) return result;

  result = sysHyPhy20gRead(fileHndl, pmonDataReg2, &pmonData2);
  if (result) return result;

  result = sysHyPhy20gRead(fileHndl, pmonDataReg3, &pmonData3);
  if (result) return result;

  result = sysHyPhy20gRead(fileHndl, pmonDataReg4, &pmonData4);
  if (result) return result;

  result = sysHyPhy20gRead(fileHndl, pmonDataReg5, &pmonData5);
  if (result) return result;

  *idata0 = pmonData0;
  *idata1 = pmonData1;
  *idata2 = pmonData2;
  *idata3 = pmonData3;
  *idata4 = pmonData4;
  *idata5 = pmonData5;
      
    return HYPHY20G_SUCCESS;
} /* gfpfHyPhy20gTXDPPerfMonIndRead */

/*******************************************************************************
**
**  gfpfHyPhy20gTXDPPerfMonIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfpfHyPhy20gTXDPPerfMonInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gTXDPPerfMonIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[2];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_FRM_TXDP_REG_PERF_MONING_STAT_DAT_OFFSET(0))>>11;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the cfgChan from 0 to 0x3F */
    for (params[0]=0; params[0]<=0x3F; params[0]++)
    {
        /* loop the trigger from 0 to 1 */
        for (params[1]=0; params[1]<=1; params[1]++)
        {
            result = gfpfHyPhy20gTXDPPerfMonIndRead(fileHndl, slice, params[0], params[1],
                                                       pDumpVal, pDumpVal+1, pDumpVal+2, 
                                                       pDumpVal+3, pDumpVal+4, pDumpVal+5);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_GFPF_TXDP_PMON_IND_WIDTH;
            pDumpVal += HYPHY20G_GFPF_TXDP_PMON_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_GFPF_TXDP_PMON_IND_WIDTH*sizeof(UINT4));                    
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfpfHyPhy20gTXDPPerfMonIndDump */


/*******************************************************************************
**
**  gfpfHyPhy20gRXDPChanIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads GFP_FRM RXDP64 Indirect Channel Control and Status
**                  register which provides control and status bits to access 
**                  the per-channel indirect registers.
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - valid values are:
**                               0x0, 0x1
**                  pageAddr   - Defines the group of registers to which this
**                               indirect access is directed with respect to the
**                               following table:
** -----------------------------------------------------------------------------
** -   pageAddr   |     Register Group Accessed               |   VCID/CPBID   -
** -----------------------------------------------------------------------------
** -     0x00     | GFP / HDLC-Like Channel Configuration     |      VCID      -
** -     0x01     | GFP EXI Match and Extension Header Size   |      VCID      -
** -     0x02     | Generic Filters 0 and 1 Control           |      VCID      -
** -     0x03     | Generic Filter 0 Data                     |      VCID      -
** -     0x04     | Generic Filter 1 Data                     |      VCID      -
** -     0x05     | Frame Data FIFO Status and Reset          |      VCID      -
** -     0x06     | Frame Data FIFO Data                      |      VCID      -
** -     0x07     | Performance Monitoring Counters 0         |      VCID      -
** -     0x08     | Performance Monitoring Counters 1         |      VCID      -
** -     0x09     | Performance Monitoring Counters 2         |      VCID      -
** -     0x0A     | Performance Monitoring Counters 3         |      VCID      -
** -     0x0B     | Performance Monitoring Counters 4         |      VCID      -
** - 0xC  -> 0xF  | FDF Control debug                         |      VCID      -
** - 0x10 -> 0x1F | VCAT grp context memorydebug              |      VCID      -
** -     0x20     | CPBID Based Common Configuration Register |     CPBID      -
** -     0x21     | PTI / UPI Filter                          |     CPBID      -
** -     0x22     | Pause Register                            |     CPBID      -
** -     0x23     | Performance Monitoring Counters 5         |     CPBID      -
** -     0x24     | Performance Monitoring Counters 6         |     CPBID      -
** - 0x25 -> 0x29 | CPB group context memory debug            |     CPBID      -
** - 0x2A -> 0x3F | Reserved                                  |    Reserved    -
** -----------------------------------------------------------------------------
**                  cpbidAddr  - These bits define the VCID/CPBID ADDRESS to be 
**                               queried in the indirect access.
**                               For VCID and CPBID, valid values range from 0
**                               to 63. 
**                  *idata0    - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1    - pointer to storage for data reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gRXDPChanIndRead(struct file *fileHndl, UINT4 slice, 
                                 UINT4 pageAddr, UINT4 cpbidAddr, 
                                 UINT4 *idata0, UINT4 *idata1)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    UINT4 indChanData0;
    UINT4 indChanData1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid pageAddr has been specified */
    if( pageAddr > 0x29  || pageAddr < 0x0 ) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* Confirm a valid cpbidAddr has been specified */
    if( cpbidAddr > 0x3F ) {
      return HYPHY20G_ERR_INVALID_ARG;
    }


    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_FRM_RXDP_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);
                                                            
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_REQ_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_RXDP);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;
      
    /*Specify the pageAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
            HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_VCID_CPBID_ADDR_MSK,
            cpbidAddr);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                  HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_PAGE_ADDR_MSK,
                  pageAddr);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }
     
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                        HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_RWB_MSK,
                        1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                   HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_REQ_BUSY_MSK,
                   1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_REQ_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    /* Read indirect access data from indirect data  registers */
    indChanDataReg0 = HYPHY20G_GFP_FRM_RXDP_REG_IND_CTXT_MEM_DAT_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
    
    indChanDataReg1 = HYPHY20G_GFP_FRM_RXDP_REG_IND_CTXT_MEM_DAT_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);
    
    result = sysHyPhy20gRead(fileHndl, indChanDataReg0, &indChanData0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    result = sysHyPhy20gRead(fileHndl, indChanDataReg1, &indChanData1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    *idata0 = indChanData0;
    *idata1 = indChanData1;

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* gfpfHyPhy20gRXDPChanIndRead */

/*******************************************************************************
**
**  gfpfHyPhy20gRXDPChanIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfpfHyPhy20gRXDPChanInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gRXDPChanIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[2];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_FRM_RXDP_REG_IND_CTXT_MEM_DAT_OFFSET(0))>>11;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop pageAddr from 0 to 0x29 */
    for (params[0]=0; params[0]<=0x29; params[0]++)
    {
        /* loop cpbidAddr from 0 to 63 */
        for (params[1]=0; params[1]<=63; params[1]++)
        {
            result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, params[0], params[1],
                                                       pDumpVal, pDumpVal+1);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_GFPF_RXDP_CHAN_IND_WIDTH;
            pDumpVal += HYPHY20G_GFPF_RXDP_CHAN_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_GFPF_TXDP_PMON_IND_WIDTH*sizeof(UINT4));                    
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfpfHyPhy20gRXDPChanIndDump */


/*******************************************************************************
**
**  gfpfHyPhy20gRXDPChanIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes GFP_FRM RXDP64 Indirect Channel Control and Status
**                  register which provides control and status bits to access 
**                  the per-channel indirect registers.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - valid values are:
**                               0x0, 0x1
**                  pageAddr   - Defines the group of registers to which this
**                               indirect access is directed with respect to the
**                               following table:
** -----------------------------------------------------------------------------
** -   pageAddr   |     Register Group Accessed               |   VCID/CPBID   -
** -----------------------------------------------------------------------------
** -     0x00     | GFP / HDLC-Like Channel Configuration     |      VCID      -
** -     0x01     | GFP EXI Match and Extension Header Size   |      VCID      -
** -     0x02     | Generic Filters 0 and 1 Control           |      VCID      -
** -     0x03     | Generic Filter 0 Data                     |      VCID      -
** -     0x04     | Generic Filter 1 Data                     |      VCID      -
** -     0x05     | Frame Data FIFO Status and Reset          |      VCID      -
** -     0x06     | Frame Data FIFO Data                      |      VCID      -
** -     0x07     | Performance Monitoring Counters 0         |      VCID      -
** -     0x08     | Performance Monitoring Counters 1         |      VCID      -
** -     0x09     | Performance Monitoring Counters 2         |      VCID      -
** -     0x0A     | Performance Monitoring Counters 3         |      VCID      -
** -     0x0B     | Performance Monitoring Counters 4         |      VCID      -
** - 0xC  -> 0xF  | FDF Control debug                         |      VCID      -
** - 0x10 -> 0x1F | VCAT grp context memorydebug              |      VCID      -
** -     0x20     | CPBID Based Common Configuration Register |     CPBID      -
** -     0x21     | PTI / UPI Filter                          |     CPBID      -
** -     0x22     | Pause Register                            |     CPBID      -
** -     0x23     | Performance Monitoring Counters 5         |     CPBID      -
** -     0x24     | Performance Monitoring Counters 6         |     CPBID      -
** - 0x25 -> 0x29 | CPB group context memory debug            |     CPBID      -
** - 0x29 -> 0x3F | Reserved                                  |    Reserved    -
** -----------------------------------------------------------------------------
**                  cpbidAddr  - These bits define the VCID/CPBID ADDRESS to be 
**                               queried in the indirect access.
**                               For VCID and CPBID, valid values range from 0
**                               to 63.
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpfHyPhy20gRXDPChanIndWrite(struct file *fileHndl, UINT4 slice, 
                                  UINT4 pageAddr, UINT4 cpbidAddr, 
                                  UINT4 idata0, UINT4 idata1)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid pageAddr has been specified */
    if( pageAddr > 0x29  || pageAddr < 0x0 ) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* Confirm a valid cpbidAddr has been specified */
    if( cpbidAddr > 0x3F ) {
      return HYPHY20G_ERR_INVALID_ARG;
    }


    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_FRM_RXDP_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_REQ_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_RXDP);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;
      
    /* Write indirect access data to indirect data registers */
    indChanDataReg0 = HYPHY20G_GFP_FRM_RXDP_REG_IND_CTXT_MEM_DAT_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
    
    indChanDataReg1 = HYPHY20G_GFP_FRM_RXDP_REG_IND_CTXT_MEM_DAT_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg0, idata0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg1, idata1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    /* Specify the cfgChan and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
            HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_VCID_CPBID_ADDR_MSK,
            cpbidAddr);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                  HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_PAGE_ADDR_MSK,
                  pageAddr);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }
     
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                        HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_RWB_MSK,
                        0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
                   HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_REQ_BUSY_MSK,
                   1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }
    
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
                   HYPHY20G_GFP_FRM_RXDP_BIT_IND_CHNL_CTL_AND_STAT_REQ_BUSY_MSK,
                   0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
        return result;
    }

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RXDP);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* gfpfHyPhy20gRXDPChanIndWrite */


/*******************************************************************************
**
**  gfptHyPhy20gRXGFPTChanIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads GFP_TRA RXGFPT Indirect Channel Control and Status
**                  register which provides control and status bits to access 
**                  the per-channel indirect registers.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - valid values are:
**                                0x0, 0x1
**                  cfgChan     - GFP_TRA channel number to access
**                                Valid values: 0x0 ~ 0x3F.
**                  *idata0     - pointer to storage for data reg 0 (bits 31:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gRXGFPTChanIndRead(struct file *fileHndl, UINT4 slice, 
                                   UINT4 cfgChan, UINT4 *idata0)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_TRA_RXGFPT_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);
                                                            
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /*Specify the cfgChan and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_ADDR_MSK,
cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
 HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_RWB_MSK,
 1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_BUSY_MSK,
1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data  registers */
    indChanDataReg0 = HYPHY20G_GFP_TRA_RXGFPT_REG_IND_CHNL_COM_CFG;
    indChanDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                             
    result = sysHyPhy20gRead(fileHndl, indChanDataReg0, &indChanData0);
    if (result) return result;


    *idata0 = indChanData0;


    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gRXGFPTChanIndRead */

/*******************************************************************************
**
**  gfptHyPhy20gRXGFPTChanIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfptHyPhy20gRXGFPTChanInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gRXGFPTChanIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 cfgChan;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_TRA_RXGFPT_REG_IND_CHNL_COM_CFG)>>13;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (cfgChan=0; cfgChan<=0x3F; cfgChan++)
    {
        result = gfptHyPhy20gRXGFPTChanIndRead(fileHndl, slice, cfgChan, pDumpVal);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_GFPT_RX_GFPT_CHAN_IND_WIDTH;
        pDumpVal += HYPHY20G_GFPT_RX_GFPT_CHAN_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_GFPT_RX_GFPT_CHAN_IND_WIDTH*sizeof(UINT4));                    
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfptHyPhy20gRXGFPTChanIndDump */


/*******************************************************************************
**
**  gfptHyPhy20gRXGFPTChanIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes GFP_TRA RXGFPT Indirect Channel Control and Status
**                  register which provides control and status bits to access 
**                  the per-channel indirect registers.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - valid values are:
**                                0x0, 0x1
**                  cfgChan     - GFP_TRA channel number to access
**                                Valid values: 0x0 ~ 0x3F.
**                  idata0      - data for reg 0 (bits 31:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gRXGFPTChanIndWrite(struct file *fileHndl, UINT4 slice, 
                                    UINT4 cfgChan, UINT4 idata0)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_TRA_RXGFPT_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);
                                                            
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    indChanDataReg0 = HYPHY20G_GFP_TRA_RXGFPT_REG_IND_CHNL_COM_CFG;
    indChanDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                             
    result = sysHyPhy20gWrite(fileHndl, indChanDataReg0, idata0);
    if (result) return result;


    /* Specify the cfgChan and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_ADDR_MSK,
cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
 HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_RWB_MSK,
 0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_BUSY_MSK,
1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_CHNL_CTL_AND_STAT_CHAN_CONFIG_INDIRECT_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gRXGFPTChanIndWrite */


/*******************************************************************************
**
**  gfptHyPhy20gRXGFPTPerfMonIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads Indirect Performance Monitor bits to provide the
**                  per-channel performance monitoring registers. 
**
**  INPUTS:         *fileHndl - base address
**                  slice     - valid values are:
**                              0x0, 0x1
**                  cfgChan   - GFP_TRA channel number to access
**                              Valid values: 0x0 ~ 0x3F.
**                  *idata0   - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1   - pointer to storage for data reg 1 (bits 63:32)
**                  *idata2   - pointer to storage for data reg 2 (bits 95:64)
**                  *idata3   - pointer to storage for data reg 3 (bits 127:96)
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gRXGFPTPerfMonIndRead(struct file *fileHndl, UINT4 slice,
                                      UINT4 cfgChan, UINT4 *idata0,
                                      UINT4 *idata1, UINT4 *idata2,
                                      UINT4 *idata3)
{
    UINT4 pmonCtrlReg;
    UINT4 pmonCtrlData;
    UINT4 pmonDataReg0;
    UINT4 pmonDataReg1;
    UINT4 pmonDataReg2;
    UINT4 pmonDataReg3;
    UINT4 pmonData0;
    UINT4 pmonData1;
    UINT4 pmonData2;
    UINT4 pmonData3;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Variable declaration */
    pmonCtrlReg = HYPHY20G_GFP_TRA_RXGFPT_REG_IND_PMON_CTL;
    pmonCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonCtrlReg, slice);
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, pmonCtrlReg,
                         HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_PMON_CTL_PMON_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, pmonCtrlReg, &pmonCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&pmonCtrlData,
                      HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_PMON_CTL_PMON_CHAN_ID_MSK,
                      cfgChan);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&pmonCtrlData,
                         HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_PMON_CTL_PMON_BUSY_MSK,
                         1);
    if (result) return result;      
    
    result = sysHyPhy20gWrite(fileHndl, pmonCtrlReg, pmonCtrlData);
    if (result) return result;   
    
    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, pmonCtrlReg,
                         HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_PMON_CTL_PMON_BUSY_MSK,
                         0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Read indirect access data from indirect data registers */
    pmonDataReg0 = HYPHY20G_GFP_TRA_RXGFPT_REG_PERF_MONING_STAT_DAT_OFFSET(0);
    pmonDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg0, slice);
        
    pmonDataReg1 = HYPHY20G_GFP_TRA_RXGFPT_REG_PERF_MONING_STAT_DAT_OFFSET(1);
    pmonDataReg1 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg1, slice);
        
    pmonDataReg2 = HYPHY20G_GFP_TRA_RXGFPT_REG_PERF_MONING_STAT_DAT_OFFSET(2);
    pmonDataReg2 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg2, slice);
        
    pmonDataReg3 = HYPHY20G_GFP_TRA_RXGFPT_REG_PERF_MONING_STAT_DAT_OFFSET(3);
    pmonDataReg3 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg3, slice);
    
    result = sysHyPhy20gRead(fileHndl, pmonDataReg0, &pmonData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, pmonDataReg1, &pmonData1);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, pmonDataReg2, &pmonData2);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, pmonDataReg3, &pmonData3);
    if (result) return result;

    *idata0 = pmonData0;
    *idata1 = pmonData1;
    *idata2 = pmonData2;
    *idata3 = pmonData3;

    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gRXGFPTPerfMonIndRead */

/*******************************************************************************
**
**  gfptHyPhy20gRXGFPTPerfMonIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfptHyPhy20gRXGFPTPerfMonInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gRXGFPTPerfMonIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 cfgChan;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_TRA_RXGFPT_REG_PERF_MONING_STAT_DAT_OFFSET(0))>>13;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (cfgChan=0; cfgChan<=0x3F; cfgChan++)
    {
        result = gfptHyPhy20gRXGFPTPerfMonIndRead(fileHndl, slice, cfgChan, 
                                          pDumpVal, pDumpVal+1, pDumpVal+2, pDumpVal+3);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_GFPT_RX_GFPT_PMON_IND_WIDTH;
        pDumpVal += HYPHY20G_GFPT_RX_GFPT_PMON_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_GFPT_RX_GFPT_PMON_IND_WIDTH*sizeof(UINT4));                    
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfptHyPhy20gRXGFPTPerfMonIndDump */


/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTReqProChanIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads Indirect Request Processor Channel Control and Status
**                  bits to provide the per-channel performance monitoring 
**                  registers. 
**
**  INPUTS:         *fileHndl - base address
**                  slice     - valid values are:
**                              0x0, 0x1
**                  cfgChan   - GFP_TRA channel number to access
**                              Valid values: 0x0 ~ 0x3F.
**                  *idata0   - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1   - pointer to storage for data reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTReqProChanIndRead(struct file *fileHndl, UINT4 slice, 
                                         UINT4 cfgChan, UINT4 *idata0,
                                         UINT4 *idata1)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    UINT4 indChanData0;
    UINT4 indChanData1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */
    indChanCtrlReg = 
                 HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_REQ_PROC_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);                

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /*Specify the cfgChan and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_ADDR_MSK,
cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_RWB_MSK,
1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_REQ_BUSY_MSK,
1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data  registers */
    indChanDataReg0 =
                HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_REQ_PROC_CHNL_DAT_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                                             
    indChanDataReg1 =
                HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_REQ_PROC_CHNL_DAT_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);                

    result = sysHyPhy20gRead(fileHndl, indChanDataReg0, &indChanData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indChanDataReg1, &indChanData1);
    if (result) return result;

    *idata0 = indChanData0;
    *idata1 = indChanData1;

    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gTXGFPTReqProChanIndRead */

/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTReqProChanIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfptHyPhy20gTXGFPTReqProChanInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTReqProChanIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 cfgChan;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_REQ_PROC_CHNL_DAT_OFFSET(0))>>13;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (cfgChan=0; cfgChan<=0x3F; cfgChan++)
    {
        result = gfptHyPhy20gTXGFPTReqProChanIndRead(fileHndl, slice, cfgChan, 
                                                    pDumpVal, pDumpVal+1);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_GFPT_TX_GFPT_REQ_PROC_CHAN_IND_WIDTH;
        pDumpVal += HYPHY20G_GFPT_TX_GFPT_REQ_PROC_CHAN_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_GFPT_TX_GFPT_REQ_PROC_CHAN_IND_WIDTH*sizeof(UINT4));                    
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfptHyPhy20gTXGFPTReqProChanIndDump */


/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTReqProChanIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes Indirect Request Processor Channel Control and Status
**                  bits to provide the per-channel performance monitoring 
**                  registers. 
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - valid values are:
**                                0x0, 0x1
**                  cfgChan     - GFP_TRA channel number to access
**                                Valid values: 0x0 ~ 0x3F.
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTReqProChanIndWrite(struct file *fileHndl, UINT4 slice, 
                                          UINT4 cfgChan, UINT4 idata0,
                                          UINT4 idata1)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    indChanCtrlReg = 
                 HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_REQ_PROC_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice); 
                                                            
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    indChanDataReg0 =
                HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_REQ_PROC_CHNL_DAT_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                                             
    indChanDataReg1 =
                HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_REQ_PROC_CHNL_DAT_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg0, idata0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg1, idata1);
    if (result) return result;

    /* Specify the cfgChan and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_ADDR_MSK,
cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_RWB_MSK,
0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_REQ_BUSY_MSK,
1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_REQ_PROC_CHNL_CTL_AND_STAT_RP_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gTXGFPTReqProChanIndWrite */

/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTChanIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads Indirect Channel Control and Status bits to provide
**                  the per-channel performance monitoring registers. 
**
**  INPUTS:         *fileHndl - base address
**                  slice     - valid values are:
**                              0x0, 0x1
**                  cfgChan   - GFP_TRA channel number to access
**                              Valid values: 0x0 ~ 0x3F.
**                  *idata0   - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1   - pointer to storage for data reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTChanIndRead(struct file *fileHndl, UINT4 slice, 
                                   UINT4 cfgChan, UINT4 *idata0, UINT4 *idata1)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    UINT4 indChanData0;
    UINT4 indChanData1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice); 
                                                            
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /*Specify the cfgChan and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
   HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_ADDR_MSK,
   cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
    HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_RWB_MSK,
    1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_REQ_BUSY_MSK,
1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    /* Read indirect access data from indirect data  registers */
    indChanDataReg0 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_DAT_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                             
    indChanDataReg1 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_DAT_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);

    result = sysHyPhy20gRead(fileHndl, indChanDataReg0, &indChanData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indChanDataReg1, &indChanData1);
    if (result) return result;

    *idata0 = indChanData0;
    *idata1 = indChanData1;

    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gTXGFPTChanIndRead */

/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTChanIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfptHyPhy20gTXGFPTChanInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTChanIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 cfgChan;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_DAT_OFFSET(0))>>13;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (cfgChan=0; cfgChan<=0x3F; cfgChan++)
    {
        result = gfptHyPhy20gTXGFPTChanIndRead(fileHndl, slice, cfgChan, 
                                                    pDumpVal, pDumpVal+1);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_GFPT_TX_GFPT_CHAN_IND_WIDTH;
        pDumpVal += HYPHY20G_GFPT_TX_GFPT_CHAN_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_GFPT_TX_GFPT_CHAN_IND_WIDTH*sizeof(UINT4));                    
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfptHyPhy20gTXGFPTChanIndDump */


/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTChanIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes Indirect Channel Control and Status bits to provide
**                  the per-channel performance monitoring registers. 
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - valid values are:
**                                0x0, 0x1
**                  cfgChan     - GFP_TRA channel number to access
**                                Valid values: 0x0 ~ 0x3F.
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTChanIndWrite(struct file *fileHndl, UINT4 slice, 
                                    UINT4 cfgChan, UINT4 idata0, UINT4 idata1)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    indChanCtrlReg = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice); 
                                                            
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    indChanDataReg0 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_DAT_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                             
    indChanDataReg1 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_DAT_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg0, idata0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg1, idata1);
    if (result) return result;

    /* Specify the cfgChan and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
   HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_ADDR_MSK,
   cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
    HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_RWB_MSK,
    0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_REQ_BUSY_MSK,
1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_CTL_AND_STAT_CHAN_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gTXGFPTChanIndWrite */


/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTChanPauseIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes Indirect Channel Transmit Pause Control and Status 
**                  bits to provide the per-channel performance monitoring 
**                  registers. 
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - valid values are:
**                                0x0, 0x1
**                  cfgChan     - GFP_TRA channel number to access
**                                Valid values: 0x0 ~ 0x3F.
**                  idata0      - data for reg 0 (bits 31:0)
**                  idata1      - data for reg 1 (bits 63:32)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTChanPauseIndWrite(struct file *fileHndl, UINT4 slice, 
                                         UINT4 cfgChan, UINT4 idata0,
                                         UINT4 idata1)
{
    UINT4 indChanCtrlReg;
    UINT4 indChanCtrlData;
    UINT4 indChanDataReg0;
    UINT4 indChanDataReg1;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    indChanCtrlReg = 
                 HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_TX_PAUSE_CTL_AND_STAT;
    indChanCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanCtrlReg,
                                                            slice);   
                                                            
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_TX_PAUSE_CTL_AND_STAT_TX_PAUSE_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Write indirect access data to indirect data registers */
    indChanDataReg0 = 
                HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_TX_PAUSE_DAT_OFFSET(0);
    indChanDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg0,
                                                             slice);
                                                                             
    indChanDataReg1 = 
                HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_CHNL_TX_PAUSE_DAT_OFFSET(1);
    indChanDataReg1 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(indChanDataReg1,
                                                             slice);    

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg0, idata0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanDataReg1, idata1);
    if (result) return result;

    /* Specify the cfgChan and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indChanCtrlReg, &indChanCtrlData);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_TX_PAUSE_CTL_AND_STAT_TX_PAUSE_INDIRECT_ADDR_MSK,
cfgChan);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_TX_PAUSE_CTL_AND_STAT_TX_PAUSE_INDIRECT_RWB_MSK,
0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&indChanCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_TX_PAUSE_CTL_AND_STAT_TX_PAUSE_INDIRECT_REQ_BUSY_MSK,
1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indChanCtrlReg, indChanCtrlData);
    if (result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indChanCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_CHNL_TX_PAUSE_CTL_AND_STAT_TX_PAUSE_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gTXGFPTChanPauseIndWrite */


/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTPerfMonIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Reads Indirect Performance Monitor bits to provide the
**                  per-channel performance monitoring registers. 
**
**  INPUTS:         *fileHndl - base address
**                  slice     - valid values are:
**                              0x0, 0x1
**                  cfgChan   - GFP_TRA channel number to access
**                              Valid values: 0x0 ~ 0x3F.
**                  *idata0   - pointer to storage for data reg 0 (bits 31:0)
**                  *idata1   - pointer to storage for data reg 1 (bits 63:32)
**                  *idata2   - pointer to storage for data reg 2 (bits 95:64)
**                  *idata3   - pointer to storage for data reg 3 (bits 127:96)

**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTPerfMonIndRead(struct file *fileHndl, UINT4 slice,
                                      UINT4 cfgChan, UINT4 *idata0, 
                                      UINT4 *idata1, UINT4 *idata2, 
                                      UINT4 *idata3)
{
    UINT4 pmonCtrlReg;
    UINT4 pmonCtrlData;
    UINT4 pmonDataReg0;
    UINT4 pmonDataReg1;
    UINT4 pmonDataReg2;
    UINT4 pmonDataReg3;
    UINT4 pmonData0;
    UINT4 pmonData1;
    UINT4 pmonData2;
    UINT4 pmonData3;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid cfgChan has been specified */
    if(cfgChan > 63) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Variable declaration */
    pmonCtrlReg = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_PMON_CTL_AND_STAT;
    pmonCtrlReg = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonCtrlReg, slice);
       
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, pmonCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_PMON_CTL_AND_STAT_PMON_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, pmonCtrlReg, &pmonCtrlData);
    if (result) return result;
    
    result = hyPhy20gVarFieldWrite(&pmonCtrlData,
   HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_PMON_CTL_AND_STAT_PMON_INDIRECT_ADDR_MSK,
   cfgChan);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&pmonCtrlData,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_PMON_CTL_AND_STAT_PMON_INDIRECT_REQ_BUSY_MSK,
1);
    if (result) return result;      
    
    result = sysHyPhy20gWrite(fileHndl, pmonCtrlReg, pmonCtrlData);
    if (result) return result;   
    
    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, pmonCtrlReg,
HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_PMON_CTL_AND_STAT_PMON_INDIRECT_REQ_BUSY_MSK,
0, HYPHY20G_DELAY_20USEC);
    if (result) return result;

    /* Read indirect access data from indirect data registers */
    pmonDataReg0 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_PMON_DAT_OFFSET(0);
    pmonDataReg0 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg0, slice);    
    
    pmonDataReg1 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_PMON_DAT_OFFSET(1);
    pmonDataReg1 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg1, slice);
    
    pmonDataReg2 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_PMON_DAT_OFFSET(2);
    pmonDataReg2 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg2, slice);
    
    pmonDataReg3 = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_PMON_DAT_OFFSET(3);
    pmonDataReg3 = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(pmonDataReg3, slice);
    
    result = sysHyPhy20gRead(fileHndl, pmonDataReg0, &pmonData0);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, pmonDataReg1, &pmonData1);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, pmonDataReg2, &pmonData2);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, pmonDataReg3, &pmonData3);
    if (result) return result;

    *idata0 = pmonData0;
    *idata1 = pmonData1;
    *idata2 = pmonData2;
    *idata3 = pmonData3;

    return HYPHY20G_SUCCESS;
} /* gfptHyPhy20gTXGFPTPerfMonIndRead */

/*******************************************************************************
**
**  gfptHyPhy20gTXGFPTPerfMonIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the gfptHyPhy20gTXGFPTPerfMonInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfptHyPhy20gTXGFPTPerfMonIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 cfgChan;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_PMON_DAT_OFFSET(0))>>13;
    
    if (slice >1 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    for (cfgChan=0; cfgChan<=0x3F; cfgChan++)
    {
        result = gfptHyPhy20gTXGFPTPerfMonIndRead(fileHndl, slice, cfgChan, 
                                          pDumpVal, pDumpVal+1, pDumpVal+2, pDumpVal+3);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_GFPT_TX_GFPT_PMON_IND_WIDTH;
        pDumpVal += HYPHY20G_GFPT_TX_GFPT_PMON_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_GFPT_TX_GFPT_PMON_IND_WIDTH*sizeof(UINT4));                    
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* gfptHyPhy20gTXGFPTPerfMonIndDump */


/*******************************************************************************
**
**  vcatHyPhy20gTVCPMemIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 32 bytes of TVCP MTSB Indirect Memory Data.
**
**  INPUTS:        *fileHndl  - base address
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : FE Sink MST Buffer
**                              0x2 : Group Context Table
**                              0x3 : LCAS State Table
**                              0x4 : H4 Table
**                              0x5 : Link Control Table
**                              0x6 : Group Control Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0xBF    -
**                              -   0x1    |   0x0 ~ 0x5FF   -
**                              -   0x2    |   0x0 ~ 0xBF    -
**                              -   0x3    |   0x0 ~ 0xBF    -
**                              -   0x4    |   0x0 ~ 0x100   -
**                              -   0x5    |   0x0 ~ 0x17F   -
**                              -   0x6    |   0x0 ~ 0x7F    -
**                              ------------------------------
**                 *idata0    - pointer to storage for data reg 0 (bits 31:0)
**                 *idata1    - pointer to storage for data reg 1 (bits 63:32)
**                 *idata2    - pointer to storage for data reg 2 (bits 95:64)
**                 *idata3    - pointer to storage for data reg 3 (bits 127:96)
**                 *idata4    - pointer to storage for data reg 4 (bits 161:128)
**                 *idata5    - pointer to storage for data reg 5 (bits 193:160)
**                 *idata6    - pointer to storage for data reg 6 (bits 223:192)
**                 *idata7    - pointer to storage for data reg 7 (bits 255:224)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPMemIndRead(struct file *fileHndl, UINT4 memSel,
                                UINT4 memAddr, UINT4 *idata0, UINT4 *idata1,
                                UINT4 *idata2, UINT4 *idata3, UINT4 *idata4,
                                UINT4 *idata5, UINT4 *idata6, UINT4 *idata7)                              
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    UINT4 indMemDataReg2;
    UINT4 indMemDataReg3;
    UINT4 indMemDataReg4;
    UINT4 indMemDataReg5;
    UINT4 indMemDataReg6;
    UINT4 indMemDataReg7;
    UINT4 indMemData0;
    UINT4 indMemData1;
    UINT4 indMemData2;
    UINT4 indMemData3;
    UINT4 indMemData4;
    UINT4 indMemData5;
    UINT4 indMemData6;
    UINT4 indMemData7;
    INT4  result;
    
    /* Argument Checking */     
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    switch(memSel) {
        case 0x0:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 1535) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x2:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x3:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x4:
            if (memAddr > 256) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x5:
            if (memAddr > 383) { 
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;

        case 0x6:
            if (memAddr > 127) { 
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
            
        default :
            /* MEM_SEL is out of valid range */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }; 

    /* variable declaration */ 
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCP_REG_IND_MEM_CMD;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_ADDR_MSK, 
                           memAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                            memSel);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                            1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(0);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(1);
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(2);
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(3);
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(4);
    indMemDataReg5 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(5);
    indMemDataReg6 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(6);
    indMemDataReg7 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(7);
    
    result = sysHyPhy20gRead(fileHndl, indMemDataReg0, &indMemData0);
    if(result) return result;
      
    result = sysHyPhy20gRead(fileHndl, indMemDataReg1, &indMemData1);
    if(result) return result;
    
    result = sysHyPhy20gRead(fileHndl, indMemDataReg2, &indMemData2);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg3, &indMemData3);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg4, &indMemData4);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg5, &indMemData5);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg6, &indMemData6);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, indMemDataReg7, &indMemData7);
    if (result) return result;
      
    *idata0 = indMemData0;
    *idata1 = indMemData1;
    *idata2 = indMemData2;
    *idata3 = indMemData3;
    *idata4 = indMemData4;
    *idata5 = indMemData5;
    *idata6 = indMemData6;
    *idata7 = indMemData7;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPMemIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gTVCPMemIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gTVCPMemInd 
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPMemIndDump (struct file *fileHndl, void *pDumpBuff, 
                                        UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 i,j,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (i=0; i<HYPHY20G_VCAT_TVCP_MEM_IND_ENTRY_NUM; i++)
    {
        for (j=vcatHyPhy20gTVCPMemInd[i].minAddr; j<=vcatHyPhy20gTVCPMemInd[i].maxAddr; j++)
        {
            result = vcatHyPhy20gTVCPMemIndRead(fileHndl,vcatHyPhy20gTVCPMemInd[i].memSel, j, 
                                        pDumpVal, pDumpVal+1, pDumpVal+2, pDumpVal+3, 
                                        pDumpVal+4, pDumpVal+5, pDumpVal+6, pDumpVal+7);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_VCAT_TVCP_MEM_IND_WIDTH;
            pDumpVal += HYPHY20G_VCAT_TVCP_MEM_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_VCAT_TVCP_MEM_IND_WIDTH*sizeof(UINT4));
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gTVCPMemIndDump */


/*******************************************************************************
**
**  vcatHyPhy20gTVCPMemIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 32 bytes of data to the TVCP MTSB Indirect Memory
**                 registers.
**
**  INPUTS:        *fileHndl  - base address
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : FE Sink MST Buffer
**                              0x2 : Group Context Table
**                              0x3 : LCAS State Table
**                              0x4 : H4 Table
**                              0x5 : Link Control Table
**                              0x6 : Group Control Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0xBF    -
**                              -   0x1    |   0x0 ~ 0x5FF   -
**                              -   0x2    |   0x0 ~ 0xBF    -
**                              -   0x3    |      ---        -
**                              -   0x4    |   0x0 ~ 0x100   -
**                              -   0x5    |      ---        -
**                              -   0x6    |      ---        - 
**                              ------------------------------
**                 idata0     - data for reg 0 (bits 31:0)
**                 idata1     - data for reg 1 (bits 63:32)
**                 idata2     - data for reg 2 (bits 95:64)
**                 idata3     - data for reg 3 (bits 127:96)
**                 idata4     - data for reg 4 (bits 159:128)
**                 idata5     - data for reg 5 (bits 191:160)
**                 idata6     - data for reg 6 (bits 223:192)
**                 idata7     - data for reg 7 (bits 255:224)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPMemIndWrite(struct file *fileHndl, UINT4 memSel,
                                 UINT4 memAddr, UINT4 idata0, UINT4 idata1,
                                 UINT4 idata2, UINT4 idata3, UINT4 idata4,
                                 UINT4 idata5, UINT4 idata6, UINT4 idata7)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    UINT4 indMemDataReg2;
    UINT4 indMemDataReg3;
    UINT4 indMemDataReg4;
    UINT4 indMemDataReg5;
    UINT4 indMemDataReg6;
    UINT4 indMemDataReg7;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    switch(memSel) {
        case 0x0:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 1535) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x2:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x4:
            if (memAddr > 256) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;          
        default :
            /* MEM_SEL is out of valid range */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }; 

    /* variable declaration */ 
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCP_REG_IND_MEM_CMD;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Write indirect access data to indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(0);
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(1);
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(2);
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(3);
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(4);
    indMemDataReg5 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(5);
    indMemDataReg6 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(6);
    indMemDataReg7 = HYPHY20G_VCAT_LCAS_TVCP_REG_MEM_DAT_OFFSET(7);
    
    result = sysHyPhy20gWrite(fileHndl, indMemDataReg0, idata0);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, indMemDataReg1, idata1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg2, idata2);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg3, idata3);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg4, idata4);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg5, idata5);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg6, idata6);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg7, idata7);
    if (result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                            0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                            memSel);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_ADDR_MSK, 
                           memAddr);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_TVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPMemIndWrite */

/*******************************************************************************
**
**  vcatHyPhy20gTVCPIntIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 4 bytes of TVCP MTSB Indirect Interrupt Data.
**
**  INPUTS:        *fileHndl  - base address
**                 memAddr    - valid value range: 0 ~ 191
**                 *idata0    - pointer to storage for data reg 0 (bits 31:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPIntIndRead(struct file *fileHndl, UINT4 memAddr,
                                UINT4 *idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemDataReg0;
    UINT4 intMemData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm MEM_ADDR is valid */
    if(memAddr > 191) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCP_REG_INT_MEM_CMD;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                        HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_RWB_MSK,
                        1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                   HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_ADDR_REG_MSK, 
                   memAddr);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCP_REG_INT_MEM_DAT;

    result = sysHyPhy20gRead(fileHndl, intMemDataReg0, &intMemData0);
    if(result) return result;

    *idata0 = intMemData0;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPIntIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gTVCPIntIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gTVCPIntInd 
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPIntIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 memAddr,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (memAddr=0; memAddr<=191; memAddr++)
    {
        result = vcatHyPhy20gTVCPIntIndRead(fileHndl, memAddr, pDumpVal);
            
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }

        pCfgDumpReq->length += HYPHY20G_VCAT_TVCP_INT_IND_WIDTH;
        pDumpVal += HYPHY20G_VCAT_TVCP_INT_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_VCAT_TVCP_INT_IND_WIDTH*sizeof(UINT4));
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gTVCPIntIndDump */


/*******************************************************************************
**
**  vcatHyPhy20gTVCPIntIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Writes 4 bytes of data to the TVCP MTSB Indirect Interrupt
**                  registers.
**
**  INPUTS:         *fileHndl   - base address
**                  memAddr     - valid value range: 0 ~ 191
**                  idata0      - data for reg 0 (bits 31:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gTVCPIntIndWrite(struct file *fileHndl, UINT4 memAddr,
                                 UINT4 idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm MEM_ADDR is valid*/
    if(memAddr > 191) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_TVCP_REG_INT_MEM_CMD;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
      
    /* Write indirect access data to indirect data registers */
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_TVCP_REG_INT_MEM_DAT;
    
    result = sysHyPhy20gWrite(fileHndl, intMemDataReg0, idata0);
    if(result) return result;

    /* Initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                        HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_RWB_MSK,
                        0);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                   HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_ADDR_REG_MSK, 
                   memAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       1);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_TVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gTVCPIntIndWrite */


/*******************************************************************************
**
**  vcatHyPhy20gRVCPMemIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 32 bits of RVCP MTSB Indirect Memory Data.
**
**  INPUTS:        *fileHndl  - base address
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : Group Configuration Table 
**                              0x2 : DM Link HO Context Table 
**                              0x3 : RSVD
**                              0x4 : Link Control Packet DCB
**                              0x5 : Link Delay Eval Status Table
**                              0x6 : Link DCB CRC-16 Table 
**                              0x7 : Link Statistics Table
**                              0x8 : DM Group Context Table 0 
**                              0x9 : DM Group Context Table 1 
**                              0xA : DM Group Context Table 2 
**                              0xB : NE Group MST Buffer 
**                              0xC : NE Group MST Range Table
**                              0xD : SONET Control Packet Extraction Context
**                                    Table
**                              0xE : DCB CRC16 Read Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              -----------------------------
**                              -  memSel  | memAddr range  -
**                              -   0x0    |   0x0 ~ 0xBF   -
**                              -   0x1    |   0x0 ~ 0x3F   -
**                              -   0x2    |   0x0 ~ 0x17F  -
**                              -   0x3    |      ---       -
**                              -   0x4    |   0x0 ~ 0x2FFF -
**                              -   0x5    |   0x0 ~ 0xBF   - 
**                              -   0x6    |   0x0 ~ 0x2FFF - 
**                              -   0x7    |   0x0 ~ 0xBF   -
**                              -   0x8    |   0x0 ~ 0x80   -
**                              -   0x9    |   0x0 ~ 0x80   -
**                              -   0xA    |   0x0 ~ 0x80   -
**                              -   0xB    |   0x0 ~ 0x3F   -
**                              -   0xC    |   0x0 ~ 0x3F   -
**                              -   0xD    |   0x0 ~ 0xFE   -
**                              -   0xE    |   0x0 ~ 0xBF   -
**                              -----------------------------
**                 *idata0    - pointer to storage for data reg 0 (bits 31:0)
**                 *idata1    - pointer to storage for data reg 1 (bits 63:32)
**                 *idata2    - pointer to storage for data reg 2 (bits 95:64)
**                 *idata3    - pointer to storage for data reg 3 (bits 127:96)
**                 *idata4    - pointer to storage for data reg 4 (bits 161:128)
**                 *idata5    - pointer to storage for data reg 5 (bits 193:160)
**                 *idata6    - pointer to storage for data reg 6 (bits 223:192)
**                 *idata7    - pointer to storage for data reg 7 (bits 255:224)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPMemIndRead(struct file *fileHndl, UINT4 memSel, 
                                UINT4 memAddr, UINT4 *idata0, UINT4 *idata1,
                                UINT4 *idata2, UINT4 *idata3, UINT4 *idata4,
                                UINT4 *idata5, UINT4 *idata6, UINT4 *idata7)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemAddrReg;
    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    UINT4 indMemDataReg2;
    UINT4 indMemDataReg3;
    UINT4 indMemDataReg4;
    UINT4 indMemDataReg5;
    UINT4 indMemDataReg6;
    UINT4 indMemDataReg7;
    UINT4 indMemData0;
    UINT4 indMemData1;
    UINT4 indMemData2;
    UINT4 indMemData3;
    UINT4 indMemData4;
    UINT4 indMemData5;
    UINT4 indMemData6;
    UINT4 indMemData7;
    INT4  result;
       
    /* Argument Checking */
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    switch(memSel) {
        case 0x0:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 63) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x2:
            if (memAddr > 383) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x4:
            if (memAddr > 12287) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x5:
            if (memAddr > 191) { 
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;           
        case 0x6:
            if (memAddr > 12287) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;            
        case 0x7:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x8:
            if (memAddr > 128) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x9:
            if (memAddr > 128) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xA:
            if (memAddr > 128) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xB:
            if (memAddr > 63) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xC:
            if (memAddr > 63) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;            
        case 0xD:
            if (memAddr > 254) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xE:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;            
        default :
          /* MEM_SEL is out of valid range */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }; 

    /* variable declaration */         
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_CMD;
    indMemAddrReg = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_ADR;   
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;

    /* Specify the memAddr to read */
    result = hyPhy20gRegFieldWrite(fileHndl, indMemAddrReg, 
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_ADR_MEM_ADDR_MSK, 
                           memAddr);                       
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    /* Specify the memSel and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                            memSel);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                            1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }     
      
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    /* Read indirect access data from indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_0;
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_1;
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_2;
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_3;
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_4;
    indMemDataReg5 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_5;
    indMemDataReg6 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_6;
    indMemDataReg7 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_7;
    
    result = sysHyPhy20gRead(fileHndl, indMemDataReg0, &indMemData0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gRead(fileHndl, indMemDataReg1, &indMemData1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gRead(fileHndl, indMemDataReg2, &indMemData2);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gRead(fileHndl, indMemDataReg3, &indMemData3);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gRead(fileHndl, indMemDataReg4, &indMemData4);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 
      
    result = sysHyPhy20gRead(fileHndl, indMemDataReg5, &indMemData5);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gRead(fileHndl, indMemDataReg6, &indMemData6);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gRead(fileHndl, indMemDataReg7, &indMemData7);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    *idata0 = indMemData0;
    *idata1 = indMemData1;
    *idata2 = indMemData2;
    *idata3 = indMemData3;
    *idata4 = indMemData4;
    *idata5 = indMemData5;
    *idata6 = indMemData6;
    *idata7 = indMemData7;

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return result;   

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPMemIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gRVCPMemIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gRVCPMemInd 
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPMemIndIndDump (struct file *fileHndl, void *pDumpBuff, 
                                        UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 i,j,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (i=0; i<HYPHY20G_VCAT_RVCP_MEM_IND_ENTRY_NUM; i++)
    {
        for (j=vcatHyPhy20gRVCPMemInd[i].minAddr; j<=vcatHyPhy20gRVCPMemInd[i].maxAddr; j++)
        {
            result = vcatHyPhy20gRVCPMemIndRead(fileHndl,vcatHyPhy20gRVCPMemInd[i].memSel, j, 
                                        pDumpVal, pDumpVal+1, pDumpVal+2, pDumpVal+3, 
                                        pDumpVal+4, pDumpVal+5, pDumpVal+6, pDumpVal+7);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_VCAT_RVCP_MEM_IND_WIDTH;
            pDumpVal += HYPHY20G_VCAT_RVCP_MEM_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_VCAT_RVCP_MEM_IND_WIDTH*sizeof(UINT4));
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gRVCPMemIndIndDump */


/*******************************************************************************
**
**  vcatHyPhy20gRVCPMemIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 4 bytes of data to the RVCP MTSB Indirect Memory
**                 registers.
**
**  INPUTS:        *fileHndl  - base address
**                 memSel     - valid values are:
**                              0x0 : Link Configuration Table
**                              0x1 : Group Configuration Table 
**                              0x2 : DM Link HO Context Table 
**                              0x3 : RSVD
**                              0x4 : Link Control Packet DCB
**                              0x5 : Link Delay Eval Status Table
**                              0x6 : Link DCB CRC-16 Table 
**                              0x7 : Link Statistics Table
**                              0x8 : DM Group Context Table 0 
**                              0x9 : DM Group Context Table 1 
**                              0xA : DM Group Context Table 2 
**                              0xB : NE Group MST Buffer 
**                              0xC : NE Group MST Range Table
**                              0xD : SONET Control Packet Extraction Context
**                                    Table
**                              0xE : DCB CRC16 Read Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              -----------------------------
**                              -  memSel  | memAddr range  -
**                              -   0x0    |   0x0 ~ 0xBF   -
**                              -   0x1    |   0x0 ~ 0x3F   -
**                              -   0x2    |      ---       -
**                              -   0x3    |      ---       -
**                              -   0x4    |   0x0 ~ 0x2FFF -
**                              -   0x5    |   0x0 ~ 0xBF   -
**                              -   0x6    |   0x0 ~ 0x2FFF -
**                              -   0x7    |      ---       -
**                              -   0x8    |   0x0 ~ 0x80   -
**                              -   0x9    |   0x0 ~ 0x80   -
**                              -   0xA    |   0x0 ~ 0x80   -
**                              -   0xB    |   0x0 ~ 0x3F   -
**                              -   0xC    |   0x0 ~ 0x3F   -
**                              -   0xD    |      ---       -
**                              -   0xE    |   0x0 ~ 0xBF   -
**                              -----------------------------
**                 idata0     - data for reg 0 (bits 31:0)
**                 idata1     - data for reg 1 (bits 63:32)
**                 idata2     - data for reg 2 (bits 95:64)
**                 idata3     - data for reg 3 (bits 127:96)
**                 idata4     - data for reg 4 (bits 159:128)
**                 idata5     - data for reg 5 (bits 191:160)
**                 idata6     - data for reg 6 (bits 223:192)
**                 idata7     - data for reg 7 (bits 255:224)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPMemIndWrite(struct file *fileHndl, UINT4 memSel,
                                 UINT4 memAddr, UINT4 idata0, UINT4 idata1,
                                 UINT4 idata2, UINT4 idata3, UINT4 idata4,
                                 UINT4 idata5, UINT4 idata6, UINT4 idata7)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemAddrReg;
    UINT4 indMemDataReg0;
    UINT4 indMemDataReg1;
    UINT4 indMemDataReg2;
    UINT4 indMemDataReg3;
    UINT4 indMemDataReg4;
    UINT4 indMemDataReg5; 
    UINT4 indMemDataReg6; 
    UINT4 indMemDataReg7; 
    INT4  result;

    /* Argument Checking */
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */    
    switch(memSel) {
        case 0x0:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x1:
            if (memAddr > 63) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x4:
            if (memAddr > 12287) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x5:
            if (memAddr > 191) { 
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;           
        case 0x6:
            if (memAddr > 12287) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;            
        case 0x8:
            if (memAddr > 128) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0x9:
            if (memAddr > 128) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xA:
            if (memAddr > 128) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xB:
            if (memAddr > 63) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;
        case 0xC:
            if (memAddr > 63) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;            
        case 0xE:
            if (memAddr > 191) {
                return HYPHY20G_ERR_INVALID_ARG;
            }
            break;            
        default :
          /* MEM_SEL is out of valid range */
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }; 
    /* variable declaration */         
    indMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_CMD;
    indMemAddrReg = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_ADR;   
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;

    /* Write indirect access data to indirect data registers */
    indMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_0;
    indMemDataReg1 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_1;
    indMemDataReg2 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_2;
    indMemDataReg3 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_3;
    indMemDataReg4 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_4;
    indMemDataReg5 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_5;
    indMemDataReg6 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_6;
    indMemDataReg7 = HYPHY20G_VCAT_LCAS_RVCP_REG_IND_MEM_DAT_7;


    result = sysHyPhy20gWrite(fileHndl, indMemDataReg0, idata0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg1, idata1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg2, idata2);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg3, idata3);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg4, idata4);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 
    
    result = sysHyPhy20gWrite(fileHndl, indMemDataReg5, idata5);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg6, idata6);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gWrite(fileHndl, indMemDataReg7, idata7);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }  

    /* Specify the memAddr to write */
    result = hyPhy20gRegFieldWrite(fileHndl, indMemAddrReg,
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_ADR_MEM_ADDR_MSK,
                           memAddr);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 
 
    /* Specify the memSel and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_SEL_MSK,
                            memSel);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 
      
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                            HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_RWB_MSK,
                            0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }  
    
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                           HYPHY20G_VCAT_LCAS_RVCP_BIT_IND_MEM_CMD_MEM_BUSY_MSK,
                           0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPMemIndWrite */


/*******************************************************************************
**
**  vcatHyPhy20gRVCPIntIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 4 bytes of RVCP MTSB Indirect Interrupt Data.
**
**  INPUTS:        *fileHndl  - base address
**                 memSel     - valid values are:
**                              0x0 : Link Interrupt Status Table
**                              0x1 : Group Interrupt Status Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0xBF    -
**                              -   0x1    |   0x0 ~ 0x3F    -
**                              ------------------------------
**                 *idata0    - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPIntIndRead(struct file *fileHndl, UINT4 memSel,
                                UINT4 memAddr, UINT4 *idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemAddrReg;

    UINT4 intMemDataReg0;
    UINT4 intMemData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */
    if(memSel == 0) {
        if(memAddr > 191) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (memSel == 1) {
        if(memAddr > 63) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
       return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCP_REG_INT_MEM_CMD;
    intMemAddrReg = HYPHY20G_VCAT_LCAS_RVCP_REG_INT_MEM_ADR;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;

    /* Specify the memAddr to read */
    result = hyPhy20gRegFieldWrite(fileHndl, intMemAddrReg, 
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_ADR_INT_MEM_ADDR_MSK, 
                       memAddr);                   
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }
    
    /* Specify the memSel and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }
        
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                        HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_SEL_MSK,
                        memSel);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 
       
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }  
    
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                        HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_RWB_MSK,
                        1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }

    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }

    /* Read indirect access data from indirect data registers */
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCP_REG_INT_MEM_DAT;

    result = sysHyPhy20gRead(fileHndl, intMemDataReg0, &intMemData0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }
    
    *idata0 = intMemData0;

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPIntIndRead */

/*******************************************************************************
**
**  vcatHyPhy20gRVCPIntIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the vcatHyPhy20gRVCPIntInd 
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPIntIndDump(struct file *fileHndl, void *pDumpBuff, 
                                        UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 i,j, dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    for (i=0; i<HYPHY20G_VCAT_RVCP_INT_IND_ENTRY_NUM; i++)
    {
        for (j=vcatHyPhy20gRVCPIntInd[i].minAddr; j<=vcatHyPhy20gRVCPIntInd[i].maxAddr; j++)
        {
            result = vcatHyPhy20gRVCPIntIndRead(fileHndl,vcatHyPhy20gRVCPIntInd[i].memSel, j, 
                                                    pDumpVal);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_VCAT_RVCP_INT_IND_WIDTH;
            pDumpVal += HYPHY20G_VCAT_RVCP_INT_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_VCAT_RVCP_INT_IND_WIDTH*sizeof(UINT4));
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gRVCPIntIndDump */


/*******************************************************************************
**
**  vcatHyPhy20gRVCPIntIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 4 bytes of data to the RVCP MTSB Indirect Interrupt
**                 registers.
**
**  INPUTS:        *fileHndl  - base address
**                 memSel     - valid values are:
**                              0x0 : Link Interrupt Status Table
**                              0x1 : Group Interrupt Status Table
**                 memAddr    - the range of "memAddr" depend on the value
**                              of "memSel", valid values are:
**                              ------------------------------
**                              -  memSel  |  memAddr range  -
**                              -   0x0    |   0x0 ~ 0xBF    -
**                              -   0x1    |   0x0 ~ 0x3F    -
**                              ------------------------------
**                 idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gRVCPIntIndWrite(struct file *fileHndl, UINT4 memSel,
                                 UINT4 memAddr, UINT4 idata0)
{
    UINT4 intMemCtrlReg;
    UINT4 intMemCtrlData;
    UINT4 intMemAddrReg;
    UINT4 intMemDataReg0;
    INT4  result;
    
    /* Argument Checking */     
    /* Confirm MEM_ADDR is valid for the specified MEM_SEL */ 
    if(memSel == 0) {
        if(memAddr > 191) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (memSel == 1) {
        if(memAddr > 63) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
       return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    intMemCtrlReg = HYPHY20G_VCAT_LCAS_RVCP_REG_INT_MEM_CMD;
    intMemAddrReg = HYPHY20G_VCAT_LCAS_RVCP_REG_INT_MEM_ADR;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
      
    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;

    /* Write indirect access data to indirect data registers */    
    intMemDataReg0 = HYPHY20G_VCAT_LCAS_RVCP_REG_INT_MEM_DAT;

    result = sysHyPhy20gWrite(fileHndl, intMemDataReg0, idata0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }   
    
    /* Specify the memAddr to read */
    result = hyPhy20gRegFieldWrite(fileHndl, intMemAddrReg, 
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_ADR_INT_MEM_ADDR_MSK, 
                       memAddr);                   
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }

    /* Specify the memSel and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, intMemCtrlReg, &intMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                        HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_SEL_MSK,
                        memSel);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                        HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_RWB_MSK,
                        0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }

    result = hyPhy20gVarFieldWrite(&intMemCtrlData,
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }
      
    result = sysHyPhy20gWrite(fileHndl, intMemCtrlReg, intMemCtrlData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    }
    
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, intMemCtrlReg,
                       HYPHY20G_VCAT_LCAS_RVCP_BIT_INT_MEM_CMD_INT_MEM_BUSY_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
        return result;
    } 

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_RVCP192);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gRVCPIntIndWrite */

/*******************************************************************************
**
**  tseHyPhy20gPRGMIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads 2 bytes of PRGM_N Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The internal RAM page bits[3:0] select which
**                              page of the internal RAM is accessed by the 
**                              current indirect transfer based on the
**                              following values:
**                              0x0: STS-1 path configuration page
**                              0x1: PRBS[22:7] page
**                              0x2: PRBS[6:0] page
**                              0x3: B1/E1 value page
**                              0x4: Monitor error count page
**                              0x5: Received B1 and E1
**                              0x8: STS-1 path configuration page
**                              0x9: PRBS[22:7] page
**                              0xA: PRBS[6:0] page
**                              0xB: B1/E1 value page
**                path        - The path[3:0] bits select which time-multiplexed
**                              division is accessed by the current indirect
**                              transfer.
**                              0000     : Invalid STS-1 path
**                              0001-1100: STS-1 path #1 to STS-1 path #12
**                              1101-1111: Invalid STS-1 path
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gPRGMIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr, 
                            UINT4 path, UINT4 *idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x3) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( iAddr > 0xB || iAddr == 0x6 || iAddr == 0x7) 
        return HYPHY20G_ERR_INVALID_ARG;
   
    /* Confirm a path has been specified */
    if( path == 0x0 || path > 0xC) 
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_TSE_PRGM_N_REG_IND_ADR;
    indAddrReg = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_BUSY_MSK,
                                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                      HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_IADDR_MSK, 
                                      iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_PATH_MSK,
                                       path);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_RWB_MSK,
                                        1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_BUSY_MSK,
                                       1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_BUSY_MSK,
                                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_TSE_PRGM_N_REG_IND_DAT;
    indDataReg0 = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* tseHyPhy20gPRGMIndRead */

/*******************************************************************************
**
**  tseHyPhy20gPRGMIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the tseHyPhy20gPRGMInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gPRGMIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 slice,dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[2];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    slice = (pCfgDumpReq->startAddr - HYPHY20G_TSE_PRGM_N_REG_IND_DAT)>>6;
    if (slice >3 )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* loop iAddr from 0 to 0xB */
    for (params[0]=0; params[0]<=0xB; params[0]++)
    {
        /* jump over the invalid iAddr space */
        if( (params[0] == 0x6) || (params[0] == 0x7) )
            continue;
        /* loop path from 1 to 0xC */
        for (params[1]=1; params[1]<=0xC; params[1]++)
        {
            result = tseHyPhy20gPRGMIndRead(fileHndl, slice, params[0], params[1],
                                                       pDumpVal);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_TSE_PRGM_IND_WIDTH;
            pDumpVal += HYPHY20G_TSE_PRGM_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_TSE_PRGM_IND_WIDTH*sizeof(UINT4));                    
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* tseHyPhy20gPRGMIndDump */


/*******************************************************************************
**
**  tseHyPhy20gPRGMIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes 2 bytes of PRGM_N Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The internal RAM page bits[3:0] select which
**                              page of the internal RAM is accessed by the 
**                              current indirect transfer based on the
**                              following values:
**                              0x0: STS-1 path configuration page
**                              0x1: PRBS[22:7] page
**                              0x2: PRBS[6:0] page
**                              0x3: B1/E1 value page
**                              0x4: Monitor error count page
**                              0x5: Received B1 and E1
**                              0x8: STS-1 path configuration page
**                              0x9: PRBS[22:7] page
**                              0xA: PRBS[6:0] page
**                              0xB: B1/E1 value page
**                path        - The path[3:0] bits select which time-multiplexed
**                              division is accessed by the current indirect
**                              transfer.
**                              0000     : Invalid STS-1 path
**                              0001-1100: STS-1 path #1 to STS-1 path #12
**                              1101-1111: Invalid STS-1 path
**                idata0      - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gPRGMIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr, 
                             UINT4 path, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0x3) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( iAddr > 0xB || iAddr == 0x6 || iAddr == 0x7) 
        return HYPHY20G_ERR_INVALID_ARG;
   
    /* Confirm a path has been specified */
    if( path == 0x0 || path > 0xC) 
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_TSE_PRGM_N_REG_IND_ADR;
    indAddrReg = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_BUSY_MSK,
                                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_TSE_PRGM_N_REG_IND_DAT;
    indDataReg0 = HYPHY20G_TSE_PRGM_N_TSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                      HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_IADDR_MSK, 
                                      iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_PATH_MSK,
                                       path);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_RWB_MSK,
                                        0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_BUSY_MSK,
                                       1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                       HYPHY20G_TSE_PRGM_N_BIT_IND_ADR_BUSY_MSK,
                                       0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* tseHyPhy20gPRGMIndWrite */


/*******************************************************************************
**
**  tseHyPhy20gSHPIIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads 2 bytes of SHPI Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**                iAddr       - The indirect address bit[2:0] field controls 
**                              which address location is accessed by the 
**                              current indirect transfer. Valid values for
**                              iAddr are as defined in the following values:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                  sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                  sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                  *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gSHPIIndRead(struct file *fileHndl, UINT4 iAddr, UINT4 sts3,
                            UINT4 sts1, UINT4 *idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid sts3 has been specified */
    if (sts3 > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( iAddr > 0x6) 
        return HYPHY20G_ERR_INVALID_ARG;
   
    /* Confirm a sts1 has been specified */
    if( sts1 > 0x2) 
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_TSE_SHPI_REG_IND_ADR;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_TSE_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_SHPI_BIT_IND_ADR_IADDR_MSK, 
                                        iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_SHPI_BIT_IND_ADR_STS_1_MSK,
                                        sts1);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_SHPI_BIT_IND_ADR_STS_3_MSK,
                                        sts3);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                          HYPHY20G_TSE_SHPI_BIT_IND_ADR_RWB_MSK,
                                          1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                         HYPHY20G_TSE_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_TSE_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_TSE_SHPI_REG_IND_DAT;
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* tseHyPhy20gSHPIIndRead */

/*******************************************************************************
**
**  tseHyPhy20gSHPIIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the tseHyPhy20gSHPIInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gSHPIIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x6; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = tseHyPhy20gSHPIIndRead(fileHndl, params[0], params[1], params[2],
                                                    pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_TSE_SHPI_IND_WIDTH;
                pDumpVal += HYPHY20G_TSE_SHPI_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_TSE_SHPI_IND_WIDTH*sizeof(UINT4));   
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* tseHyPhy20gSHPIIndDump */



/*******************************************************************************
**
**  tseHyPhy20gSHPIIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes 2 bytes of SHPI Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**            iAddr       - The indirect address bit[2:0] field controls 
**                              which address location is accessed by the 
**                              current indirect transfer. Valid values for
**                              iAddr are as defined in the following values:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gSHPIIndWrite(struct file *fileHndl, UINT4 iAddr, UINT4 sts3,
                             UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid sts3 has been specified */
    if (sts3 > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( iAddr > 0x6) 
        return HYPHY20G_ERR_INVALID_ARG;
   
    /* Confirm a sts1 has been specified */
    if( sts1 > 0x2) 
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_TSE_SHPI_REG_IND_ADR;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_TSE_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_TSE_SHPI_REG_IND_DAT;

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_SHPI_BIT_IND_ADR_IADDR_MSK, 
                                        iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_SHPI_BIT_IND_ADR_STS_1_MSK,
                                        sts1);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_TSE_SHPI_BIT_IND_ADR_STS_3_MSK,
                                        sts3);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                          HYPHY20G_TSE_SHPI_BIT_IND_ADR_RWB_MSK,
                                          0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                         HYPHY20G_TSE_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_TSE_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* tseHyPhy20gSHPIIndWrite */


/*******************************************************************************
**
**  tseHyPhy20gPCTLCfgMemIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Reads 2 bytes of PCTL Indirect Configuration Memory Data.
**
**  INPUTS:        *fileHndl  - base address
**                 oPort      - Output port component of offline page address
**                              valid values are:
**                              0x0 ~ 0x27
**                 oTs        - Output Timeslot component of offline page 
**                              address. 
**                              valid values are:
**                              0x0 ~ 0x2F
**                 *idata0    - pointer to storage for data reg 0 (bits 13:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gPCTLCfgMemIndRead(struct file *fileHndl, UINT4 oPort, UINT4 oTs,
                                  UINT4 *idata0)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemDataReg0;
    UINT4 indMemData0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid oPort has been specified */
    if (oPort > 0x27) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid oTs has been specified */
    if (oTs > 0x2F) 
        return HYPHY20G_ERR_INVALID_ARG;
    
    /* variable declaration */ 
    indMemCtrlReg = HYPHY20G_TSE_PCTL_REG_IND_CFG_MEM_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                      HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_REQ_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                                HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_OPORT_MSK,
                                oPort);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                                  HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_OTS_MSK,
                                  oTs);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_RWB_MSK,
                           1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                      HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_REQ_BUSY_MSK,
                      1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                      HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_REQ_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indMemDataReg0 = HYPHY20G_TSE_PCTL_REG_IND_CFG_MEM_DAT;
  
    result = sysHyPhy20gRead(fileHndl, indMemDataReg0, &indMemData0);
    if(result) return result;   

    *idata0 = indMemData0;

    return HYPHY20G_SUCCESS;
} /* tseHyPhy20gPCTLCfgMemIndRead */

/*******************************************************************************
**
**  tseHyPhy20gPCTLCfgMemIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the tseHyPhy20gPCTLCfgMemInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gPCTLCfgMemIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[2];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    /* loop oPort from 0 to 0x27 */
    for (params[0]=0; params[0]<=0x27; params[0]++)
    {
        /* loop oTs from 0 to 0x2F */
        for (params[1]=0; params[1]<=0x2F; params[1]++)
        {
            result = tseHyPhy20gPCTLCfgMemIndRead(fileHndl, params[0], params[1], 
                                                pDumpVal);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_TSE_PCTL_MEM_IND_WIDTH;
            pDumpVal += HYPHY20G_TSE_PCTL_MEM_IND_WIDTH;
            dumpBuffSize += (HYPHY20G_TSE_PCTL_MEM_IND_WIDTH*sizeof(UINT4));   
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* tseHyPhy20gPCTLCfgMemIndDump */


/*******************************************************************************
**
**  tseHyPhy20gPCTLCfgMemIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:   Writes 2 bytes of PCTL Indirect Configuration Memory Data.
**
**  INPUTS:        *fileHndl  - base address
**                 oPort      - Output port component of offline page address
**                              valid values are:
**                              0x0 ~ 0x27
**                 oTs        - Output Timeslot component of offline page 
**                              address. 
**                              valid values are:
**                              0x0 ~ 0x2F
**                 idata0     - data for reg 0 (bits 13:0)
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gPCTLCfgMemIndWrite(struct file *fileHndl, UINT4 oPort,
                                   UINT4 oTs, UINT4 idata0)
{
    UINT4 indMemCtrlReg;
    UINT4 indMemCtrlData;
    UINT4 indMemDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid oPort has been specified */
    if (oPort > 0x27) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid oTs has been specified */
    if (oTs > 0x2F) 
        return HYPHY20G_ERR_INVALID_ARG;
    
    /* variable declaration */ 
    indMemCtrlReg = HYPHY20G_TSE_PCTL_REG_IND_CFG_MEM_CTL;

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                      HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_REQ_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Write indirect access data to indirect data registers */
    indMemDataReg0 = HYPHY20G_TSE_PCTL_REG_IND_CFG_MEM_DAT;
    
    result = sysHyPhy20gWrite(fileHndl, indMemDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indMemCtrlReg, &indMemCtrlData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                                HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_OPORT_MSK,
                                oPort);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                                  HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_OTS_MSK,
                                  oTs);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                           HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_RWB_MSK,
                           0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indMemCtrlData,
                      HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_REQ_BUSY_MSK,
                      1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indMemCtrlReg, indMemCtrlData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indMemCtrlReg,
                      HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_CTL_MEMORY_REQ_BUSY_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* tseHyPhy20gPCTLCfgMemIndWrite */

/*******************************************************************************
**
**  dmiHyPhy20gSHPIIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads 2 bytes of DMI_SHPI Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**                sliceA      - DMI slice, valid values are:
**                              0x0 ~ 0x4
**                sliceB      - SHPI slice, valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address bit[2:0] field controls
**                              which address location is accessed by the
**                              current indirect transfer.
**                              Valid values for iAddr are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Unused
**                sts3       -  These bits[3:0] select which STS-3/STM-1 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0xF: STS-3/STM-1 path #1 to #16
**                sts1       -  These bits[1:0] select which STS-1/STM-0 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0x2: STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dmiHyPhy20gSHPIIndRead(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB, 
                            UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if(sliceA > 0x4 ||
       sliceB > 0x3 ||
       iAddr > 0x6 ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_DMI_SHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indAddrReg, sliceA);
    indAddrReg = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(indAddrReg, sliceB);
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                         HYPHY20G_DMI_SHPI_BIT_IND_ADR_BUSY_MSK, 
                                         1);    
                                          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                          HYPHY20G_DMI_SHPI_BIT_IND_ADR_RWB_MSK, 
                                          1);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_SHPI_BIT_IND_ADR_IADDR_MSK,
                                        iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_SHPI_BIT_IND_ADR_STS_3_MSK,
                                        sts3);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_SHPI_BIT_IND_ADR_STS_1_MSK,
                                        sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_DMI_SHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indDataReg0, sliceA);
    indDataReg0 = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(indDataReg0, sliceB);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* dmiHyPhy20gSHPIIndRead */

/*******************************************************************************
**
**  dmiHyPhy20gSHPIIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the dmiHyPhy20gSHPIInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dmiHyPhy20gSHPIIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 sliceA, sliceB;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    sliceA = (pCfgDumpReq->startAddr - HYPHY20G_DMI_SHPI_REG_IND_DAT)>>11;
    sliceB = (pCfgDumpReq->startAddr - HYPHY20G_DMI_SHPI_REG_IND_DAT)>>6;
    if ( (sliceA > 0x4) || (sliceB > 0x3) )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x6; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = dmiHyPhy20gSHPIIndRead(fileHndl, sliceA, sliceB, params[0],
                                                   params[1], params[2], pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_DMI_SHPI_IND_WIDTH;
                pDumpVal += HYPHY20G_DMI_SHPI_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_DMI_SHPI_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* dmiHyPhy20gSHPIIndDump */


/*******************************************************************************
**
**  dmiHyPhy20gSHPIIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes 2 bytes of DMI_SHPI Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**                sliceA      - DMI slice, valid values are:
**                              0x0 ~ 0x4
**                sliceB      - SHPI slice, valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address bit[2:0] field controls
**                              which address location is accessed by the
**                              current indirect transfer.
**                              Valid values for iAddr are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Unused
**                sts3       -  These bits[3:0] select which STS-3/STM-1 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0xF: STS-3/STM-1 path #1 to #16
**                sts1       -  These bits[1:0] select which STS-1/STM-0 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0x2: STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dmiHyPhy20gSHPIIndWrite(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB, 
                             UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if(sliceA > 0x4 ||
       sliceB > 0x3 ||
       iAddr > 0x6 ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_DMI_SHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indAddrReg, sliceA);
    indAddrReg = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(indAddrReg, sliceB);
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_DMI_SHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indDataReg0, sliceA);
    indDataReg0 = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(indDataReg0, sliceB);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                         HYPHY20G_DMI_SHPI_BIT_IND_ADR_BUSY_MSK, 
                                         1);    
                                          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                          HYPHY20G_DMI_SHPI_BIT_IND_ADR_RWB_MSK, 
                                          0);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_SHPI_BIT_IND_ADR_IADDR_MSK,
                                        iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_SHPI_BIT_IND_ADR_STS_3_MSK,
                                        sts3);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_SHPI_BIT_IND_ADR_STS_1_MSK,
                                        sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_SHPI_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* dmiHyPhy20gSHPIIndWrite */

/*******************************************************************************
**
**  dmiHyPhy20gSTFAIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads 2 bytes of DMI_STFA Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**                sliceA      - DMI slice, valid values are:
**                              0x0 ~ 0x4
**                sliceB      - STFA slice, valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address bit[1:0] field controls
**                              which address location is accessed by the
**                              current indirect transfer.
**                              Valid values for iAddr are:
**                              0x0: STFA Status Bit
**                              0x1: STFA Interrupts Enable
**                              0x2: STFA Interrupts Event
**                              0x3: STFA Diagnostic and Configuration
**                sts3       -  These bits[3:0] select which STS-3/STM-1 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0xF: STS-3#1/STM-1#1 to STS-3#16/STM-1#16
**                sts1       -  These bits[1:0] select which STS-1/STM-0 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0x2: STS-1#1/STM-0#1 to STS-1#3/STM-0#3
**                *idata0     - pointer to storage for data reg 0 (bits 5:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dmiHyPhy20gSTFAIndRead(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB, 
                            UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if(sliceA > 0x4 ||
       sliceB > 0x3 ||
       iAddr > 0x3 ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_DMI_STFA_REG_IND_ADR;
    indAddrReg = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indAddrReg, sliceA);
    indAddrReg = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(indAddrReg, sliceB);
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_STFA_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                         HYPHY20G_DMI_STFA_BIT_IND_ADR_BUSY_MSK, 
                                         1);    
                                          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                          HYPHY20G_DMI_STFA_BIT_IND_ADR_RWB_MSK, 
                                          1);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_STFA_BIT_IND_ADR_IADDR_MSK,
                                        iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_STFA_BIT_IND_ADR_STS_3_MSK,
                                        sts3);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_STFA_BIT_IND_ADR_STS_1_MSK,
                                        sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_STFA_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_DMI_STFA_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indDataReg0, sliceA);
    indDataReg0 = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(indDataReg0, sliceB);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* dmiHyPhy20gSTFAIndRead */

/*******************************************************************************
**
**  dmiHyPhy20gSTFAIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the dmiHyPhy20gSTFAInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dmiHyPhy20gSTFAIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 sliceA, sliceB;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    sliceA = (pCfgDumpReq->startAddr - HYPHY20G_DMI_STFA_REG_IND_RD_WR_DAT)>>11;
    sliceB = (pCfgDumpReq->startAddr - HYPHY20G_DMI_STFA_REG_IND_RD_WR_DAT)>>6;
    if ( (sliceA > 0x4) || (sliceB > 0x3) )
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x3; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = dmiHyPhy20gSTFAIndRead(fileHndl, sliceA, sliceB, params[0],
                                                   params[1], params[2], pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_DMI_STFA_IND_WIDTH;
                pDumpVal += HYPHY20G_DMI_STFA_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_DMI_STFA_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* dmiHyPhy20gSTFAIndDump */


/*******************************************************************************
**
**  dmiHyPhy20gSTFAIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes 2 bytes of DMI_STFA Indirect Address Data.
**
**  INPUTS:       *fileHndl   - base address
**                sliceA      - DMI slice, valid values are:
**                              0x0 ~ 0x4
**                sliceB      - STFA slice, valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address bit[1:0] field controls
**                              which address location is accessed by the
**                              current indirect transfer.
**                              Valid values for iAddr are:
**                              0x0: STFA Status Bit
**                              0x1: STFA Interrupts Enable
**                              0x2: STFA Interrupts Event
**                              0x3: STFA Diagnostic and Configuration
**                sts3       -  These bits[3:0] select which STS-3/STM-1 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0xF: STS-3#1/STM-1#1 to STS-3#16/STM-1#16
**                sts1       -  These bits[1:0] select which STS-1/STM-0 path is
**                              accessed by the current indirect transfer.
**                              valid values are:
**                              0x0 ~ 0x2: STS-1#1/STM-0#1 to STS-1#3/STM-0#3
**                *idata0     - data for reg 0 (bits 5:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dmiHyPhy20gSTFAIndWrite(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB, 
                             UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if(sliceA > 0x4 ||
       sliceB > 0x3 ||
       iAddr > 0x3 ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_DMI_STFA_REG_IND_ADR;
    indAddrReg = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indAddrReg, sliceA);
    indAddrReg = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(indAddrReg, sliceB);
    
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_STFA_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_DMI_STFA_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(indDataReg0, sliceA);
    indDataReg0 = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(indDataReg0, sliceB);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                         HYPHY20G_DMI_STFA_BIT_IND_ADR_BUSY_MSK, 
                                         1);    
                                          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                          HYPHY20G_DMI_STFA_BIT_IND_ADR_RWB_MSK, 
                                          0);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_STFA_BIT_IND_ADR_IADDR_MSK,
                                        iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_STFA_BIT_IND_ADR_STS_3_MSK,
                                        sts3);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                        HYPHY20G_DMI_STFA_BIT_IND_ADR_STS_1_MSK,
                                        sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                         HYPHY20G_DMI_STFA_BIT_IND_ADR_BUSY_MSK,
                                         0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* dmiHyPhy20gSTFAIndWrite */


/*******************************************************************************
**
**  sonetHyPhy20gSBERSectIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the SBER_SECTION MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[4:0] bits field controls 
**                              which address location is accessed by the
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: SBER SD/SF BERM State
**                              0x01: SBER SD/SF BERM Interrupt Enable
**                              0x02: SBER SD/SF BERM Interrupt Status
**                              0x03: Reserved
**                              0x04: SD BERM Set Threshold
**                              0x05: SD BERM Clear Threshold
**                              0x06: SD Monitoring Enable
**                              0x07: SD BERM Set Integration Period[31:16]
**                              0x08: SD BERM Set Integration Period[15:0]
**                              0x09  ~ 0x0C: Reserved
**                              0x13: Reserved
**                              0x14: SF BERM Set Threshold
**                              0x15: SF BERM Clear threshold
**                              0x16: SF Monitoring Enable
**                              0x17: SF BERM Set Integration Period[31:16]
**                              0x18: SF BERM Set Integration Period[15:0]
**                              0x19 ~ 0x1C: Reserved
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSBERSectIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 *idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( (iAddr > 0x08 && iAddr < 0x13) || iAddr > 0x18 ) 
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_SBER_SECT_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Indirect accesses will be complete within 80 us of being launched. */
    result = sysHyPhy20gUsDelay(80);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_SBER_SECT_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gSBERSectIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gSBERSectIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gSBERSectInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSBERSectIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 iAddr;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_SBER_SECT_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (iAddr=0; iAddr<=0x18; iAddr++)
    {
        if ((iAddr > 0x08) && (iAddr < 0x13)) 
            continue;

        result = sonetHyPhy20gSBERSectIndRead(fileHndl, slice, iAddr, pDumpVal);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_SONET_SBER_SECT_IND_WIDTH;
        pDumpVal += HYPHY20G_SONET_SBER_SECT_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_SONET_SBER_SECT_IND_WIDTH*sizeof(UINT4));  
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gSBERSectIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gSBERSectIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the SBER_SECTION MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[4:0] bits field controls 
**                              which address location is accessed by the
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: SBER SD/SF BERM State
**                              0x01: SBER SD/SF BERM Interrupt Enable
**                              0x02: SBER SD/SF BERM Interrupt Status
**                              0x03: Reserved
**                              0x04: SD BERM Set Threshold
**                              0x05: SD BERM Clear Threshold
**                              0x06: SD Monitoring Enable
**                              0x07: SD BERM Set Integration Period[31:16]
**                              0x08: SD BERM Set Integration Period[15:0]
**                              0x09  ~ 0x0C: Reserved
**                              0x13: SF BERM BER Threshold
**                              0x14: SF BERM Set Threshold
**                              0x15: SF BERM Clear threshold
**                              0x16: SF Monitoring Enable
**                              0x17: SF BERM Set Integration Period[31:16]
**                              0x18: SF BERM Set Integration Period[15:0]
**                              0x19 ~ 0x1C: Reserved
**                 idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSBERSectIndWrite(struct file *fileHndl, UINT4 slice, 
                                   UINT4 iAddr, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( (iAddr > 0x08 && iAddr < 0x13) || iAddr > 0x18 ) 
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_SBER_SECT_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_SBER_SECT_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Indirect accesses will be complete within 80 us of being launched. */
    result = sysHyPhy20gUsDelay(80);
    if(result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gSBERSectIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gRTTPSectIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the RTTP_SECTION MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[7:0] bit field controls 
**                              which address location is accessed by the 
**                              current indirect transfer. 
**                              Valid values are:
**                              0x00: Traced Unstable Status; 
**                                    Trace Mismatch Status
**                              0x01: Trace Unstable Interrupt Enable;
**                                    Trace Mismatch Interrupt Enable
**                              0x02: RTTP-U Trace Unstable Interrupt Status;
**                                    RTTP-U Trace Mismatch Interrupt Status
**                              0x03: Configuration
**                              0x40: First Byte of the 1/16/64 byte captured
**                                    trace
**                              0x41 ~ 0x7F: Other Bytes of the 16/64 byte
**                                           captured trace
**                              0x80: First Byte of the 1/16/64 byte accepted
**                                    trace
**                              0x81 ~ 0xBF: Other Bytes of the 16/64 byte
**                                           accepted trace
**                              0xC0: First Byte of the 16/64 byte expected
**                                    trace
**                              0xC1-0xFF: Other Bytes of the 16/64 byte
**                                         expected trace
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTTPSectIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 *idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( (iAddr > 0x3 && iAddr < 0x40) || iAddr > 0xFF)
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RTTP_SECT_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RTTP_SECT_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRTTPSectIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gRTTPSectIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gRTTPSectInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTTPSectIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 iAddr;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_RTTP_SECT_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (iAddr=0; iAddr<=0xFF; iAddr++)
    {
        if( (iAddr > 0x3)&&(iAddr < 0x40) ) 
            continue;

        result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr, pDumpVal);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_SONET_RTTP_SECT_IND_WIDTH;
        pDumpVal += HYPHY20G_SONET_RTTP_SECT_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_SONET_RTTP_SECT_IND_WIDTH*sizeof(UINT4));  
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gRTTPSectIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gRTTPSectIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the RTTP_SECTION MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[7:0] bit field controls 
**                              which address location is accessed by the 
**                              current indirect transfer. 
**                              Valid values are:
**                              0x00: Traced Unstable Status; 
**                                    Trace Mismatch Status
**                              0x01: Trace Unstable Interrupt Enable;
**                                    Trace Mismatch Interrupt Enable
**                              0x02: RTTP-U Trace Unstable Interrupt Status;
**                                    RTTP-U Trace Mismatch Interrupt Status
**                              0x03: Configuration
**                              0x40: First Byte of the 1/16/64 byte captured
**                                    trace
**                              0x41 ~ 0x7F: Other Bytes of the 16/64 byte
**                                           captured trace
**                              0x80: First Byte of the 1/16/64 byte accepted
**                                    trace
**                              0x81 ~ 0xBF: Other Bytes of the 16/64 byte
**                                           accepted trace
**                              0xC0: First Byte of the 16/64 byte expected
**                                    trace
**                              0xC1-0xFF: Other Bytes of the 16/64 byte
**                                         expected trace
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTTPSectIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( (iAddr > 0x3 && iAddr < 0x40) || iAddr > 0xFF)
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RTTP_SECT_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RTTP_SECT_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRTTPSectIndWrite */


/*******************************************************************************
**
**  sonetHyPhy20gRSHPIIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the RSHPI MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRSHPIIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RSHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RSHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRSHPIIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gRSHPIIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gRSHPIInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRSHPIIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_RSHPI_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=6; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=2; params[2]++)
            {
                result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_RSHPI_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_RSHPI_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_RSHPI_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gRSHPIIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gRSHPIIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the RSHPI MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRSHPIIndWrite(struct file *fileHndl, UINT4 slice, 
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RSHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RSHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSHPI_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRSHPIIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gRHAPIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the RHAP MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: RHAP-U Error Monitor Status
**                              0x1: RHAP-U Error Monitor Interrupt Enable
**                              0x2: RHAP-U Error Monitor Interrupt Status
**                              0x3: RHAP-U Monitor Configuration
**                              0x4: RHAP-U Expected PSL and PDI
**                              0x5: RHAP-U Filtered PERDI Value
**                              0x6: RHAP-U Captured and Accepted PSL
**                              0x7: RHAP-U J1 and F2
**                              0x8: RHAP-U H4 and Z3
**                              0x9: RHAP-U Z4 and Z5
**                              0xA: RHAP-U Path BIP Error Counter
**                              0xB: RHAP-U Path REI Error Counter
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRHAPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0xB) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RHAP_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RHAP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRHAPIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gRHAPIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gRHAPInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRHAPIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_RHAP_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0xB; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=2; params[2]++)
            {
                result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_RHAP_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_RHAP_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_RHAP_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gRHAPIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gRHAPIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the RHAP MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: RHAP-U Error Monitor Status
**                              0x1: RHAP-U Error Monitor Interrupt Enable
**                              0x2: RHAP-U Error Monitor Interrupt Status
**                              0x3: RHAP-U Monitor Configuration
**                              0x4: RHAP-U Expected PSL and PDI
**                              0x5: RHAP-U Filtered PERDI Value
**                              0x6: RHAP-U Captured and Accepted PSL
**                              0x7: RHAP-U J1 and F2
**                              0x8: RHAP-U H4 and Z3
**                              0x9: RHAP-U Z4 and Z5
**                              0xA: RHAP-U Path BIP Error Counter
**                              0xB: RHAP-U Path REI Error Counter
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRHAPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0xB) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RHAP_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RHAP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RHAP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRHAPIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gRTTPPathIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the RTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[7:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Traced Unstable Status;
**                                    Trace Mismatch Status
**                              0x01: Trace Unstable Interrupt Enable;
**                                    Trace Mismatch Interrupt Enable
**                              0x02: RTTP-U Trace Unstable Interrupt Status;
**                                    RTTP-U Trace Mismatch Interrupt Status
**                              0x03: Configuration
**                              0x40: First Byte of the 1/16/64 byte captured
**                                    trace
**                              0x41-0x7F: Other Bytes of the 16/64 byte
**                                         captured trace
**                              0x80: First Byte of the 1/16/64 byte accepted
**                                    trace
**                              0x81-0xBF: Other Bytes of the 16/64 byte
**                                         accepted trace
**                              0xC0: First Byte of the 16/64 byte expected
**                                    trace
**                              0xC1-0xFF: Other Bytes of the 16/64 byte
**                                         expected trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if(slice > 0xF ||
       (iAddr > 0x3 && iAddr < 0x40) ||
       iAddr > 0xFF ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRTTPPathIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gRTTPPathIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gRTTPPathInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_RTTP_PATH_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0xFF; params[0]++)
    {
        /* jump over invalid iAddr from 4 to 0x39 */
        if ((params[0]>3)&&(params[0]<0x40))
            continue;
        
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=2; params[2]++)
            {
                result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_RTTP_PATH_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_RTTP_PATH_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_RTTP_PATH_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gRTTPPathIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gRTTPPathIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the RTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[7:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Traced Unstable Status;
**                                    Trace Mismatch Status
**                              0x01: Trace Unstable Interrupt Enable;
**                                    Trace Mismatch Interrupt Enable
**                              0x02: RTTP-U Trace Unstable Interrupt Status;
**                                    RTTP-U Trace Mismatch Interrupt Status
**                              0x03: Configuration
**                              0x40: First Byte of the 1/16/64 byte captured
**                                    trace
**                              0x41-0x7F: Other Bytes of the 16/64 byte
**                                         captured trace
**                              0x80: First Byte of the 1/16/64 byte accepted
**                                    trace
**                              0x81-0xBF: Other Bytes of the 16/64 byte
**                                         accepted trace
**                              0xC0: First Byte of the 16/64 byte expected
**                                    trace
**                              0xC1-0xFF: Other Bytes of the 16/64 byte
**                                         expected trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                   UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if(slice > 0xF ||
       (iAddr > 0x3 && iAddr < 0x40) ||
       iAddr > 0xFF ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRTTPPathIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gRSVCAIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the RSVCA MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SCVA-U Status
**                              0x1: SCVA-U Interrupts Enable
**                              0x2: SCVA-U Interrupts Event
**                              0x3: SVCA-U Outgoing Positive Justification
**                                   Performance Monitor
**                              0x4: SVCA-U Outgoing Negative Justification
**                                   Performance Monitor
**                              0x5: SVCA-U Diagnostic and Configuration
**                              0x6: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3#1/STM-1 #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1#1/STM-0#1 to STS-1#3/STM-0#3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRSVCAIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x5) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RSVCA_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RSVCA_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRSVCAIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gRSVCAIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gRSVCAInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRSVCAIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_RSVCA_REG_IND_RD_WR_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* loop the param iAddr */
    for (params[0]=0; params[0]<=5; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=2; params[2]++)
            {
                result = sonetHyPhy20gRSVCAIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_RSVCA_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_RSVCA_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_RSVCA_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gRSVCAIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gRSVCAIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the RSVCA MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SCVA-U Status
**                              0x1: SCVA-U Interrupts Enable
**                              0x2: SCVA-U Interrupts Event
**                              0x3: SVCA-U Outgoing Positive Justification
**                                   Performance Monitor
**                              0x4: SVCA-U Outgoing Negative Justification
**                                   Performance Monitor
**                              0x5: SVCA-U Diagnostic and Configuration
**                              0x6: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3#1/STM-1 #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1#1/STM-0#1 to STS-1#3/STM-0#3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRSVCAIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;

    /* Argument Checking */    
    if((slice > 0xF) ||
       (iAddr > 0x5) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RSVCA_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RSVCA_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RSVCA_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRSVCAIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gSBERPathIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the SBER_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[4:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: PBER SD/SF BERM State
**                              0x01: PBER SD/SF BERM Interrupt Enable
**                              0x02: PBER SD/SF BERM Interrupt Status
**                              0x03: SD BERM BER Threshold
**                              0x04: SD BERM Set Threshold
**                              0x05: SD BERM Clear Threshold
**                              0x06: SD Path Monitoring Enable
**                              0x07: SD BERM Set Integration Period 2
**                              0x08: SD BERM Set Integration Period 1
**                              0x09-0x0C: Reserved
**                              0x13: SF BERM BER Threshold
**                              0x14: SF BERM Set Threshold
**                              0x15: SF BERM Clear threshold
**                              0x16: SF Path Monitoring Enable
**                              0x17: SF BERM Set Integration Period 2
**                              0x18: SF BERM Set Integration Period 1
**                              0x19-0x1C: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to STS-3/STM-1#16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSBERPathIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x08 && iAddr < 0x13) ||
       (iAddr > 0x18) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_SBER_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Indirect accesses will be complete within 80 us of being launched. */
    result = sysHyPhy20gUsDelay(80);
    if(result) return result;
        
    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_SBER_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gSBERPathIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gSBERPathIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gSBERPathInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSBERPathIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_SBER_PATH_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x18; params[0]++)
    {
        /* jump over invalid iAddr from 9 to 0x12 */
        if ((params[0]>8)&&(params[0]<0x13))
            continue;
        
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=2; params[2]++)
            {
                result = sonetHyPhy20gSBERPathIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_SBER_PATH_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_SBER_PATH_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_SBER_PATH_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gSBERPathIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gSBERPathIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the SBER_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[4:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: PBER SD/SF BERM State
**                              0x01: PBER SD/SF BERM Interrupt Enable
**                              0x02: PBER SD/SF BERM Interrupt Status
**                              0x03: SD BERM BER Threshold
**                              0x04: SD BERM Set Threshold
**                              0x05: SD BERM Clear Threshold
**                              0x06: SD Path Monitoring Enable
**                              0x07: SD BERM Set Integration Period 2
**                              0x08: SD BERM Set Integration Period 1
**                              0x09-0x0C: Reserved
**                              0x13: SF BERM BER Threshold
**                              0x14: SF BERM Set Threshold
**                              0x15: SF BERM Clear threshold
**                              0x16: SF Path Monitoring Enable
**                              0x17: SF BERM Set Integration Period 2
**                              0x18: SF BERM Set Integration Period 1
**                              0x19-0x1C: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to STS-3/STM-1#16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSBERPathIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                   UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    if((slice > 0xF) ||
       (iAddr > 0x08 && iAddr < 0x13) ||
       (iAddr > 0x18) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_SBER_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);
        
    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_SBER_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Indirect accesses will be complete within 80 us of being launched. */
    result = sysHyPhy20gUsDelay(80);
    if(result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gSBERPathIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gSARCIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the SARC MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: SARC Receive Path LOP and AIS status
**                              0x01: SARC Receive Path LOP,
**                                    AIS and COMAPS interrupt enable
**                              0x02: SARC Receive Path LOP,
**                                    AIS and COMAPS interrupt status
**                              0x03: SARC Transmit Path LOP and AIS status
**                              0x04: SARC Transmit Path LOP 
**                                    and AIS interrupt enable
**                              0x05: SARC Receive Path LOP 
**                                    and AIS interrupt status
**                              0x06: Reserved
**                              0x07: SARC Receive Path RPAISINS enable
**                                    Indirect Data #1
**                              0x08: SARC Receive Path RPAISINS enable
**                                    Indirect Data #2
**                              0x09: SARC Transmit Path TPAISINS Enable
**                                    Indirect Data
**                              0x0A: SARC MAPS Software Path L-Code
**                                    Configuration
**                              0x0B: SARC MAPS Path Configuration
**                              0x0C: SARC MAPS Alarm L-code status
**                              0x0D: SARC MAPS Extracted L-code Status
**                              0x0E: SARC Receive Path ERDI Enable
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSARCIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr, 
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr == 0x06) ||
       (iAddr > 0x0E) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_SARC_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_SARC_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gSARCIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gSARCIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gSARCInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSARCIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 params[3];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_SARC_REG_IND_RD_WR_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0xE; params[0]++)
    {
        /* jump over the invalid iAddr 6*/
        if (params[0]==6)
            continue;
        
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=2; params[2]++)
            {
                result = sonetHyPhy20gSARCIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_SARC_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_SARC_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_SARC_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gSARCIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gSARCIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the SARC MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: SARC Receive Path LOP and AIS status
**                              0x01: SARC Receive Path LOP,
**                                    AIS and COMAPS interrupt enable
**                              0x02: SARC Receive Path LOP,
**                                    AIS and COMAPS interrupt status
**                              0x03: SARC Transmit Path LOP and AIS status
**                              0x04: SARC Transmit Path LOP 
**                                    and AIS interrupt enable
**                              0x05: SARC Receive Path LOP 
**                                    and AIS interrupt status
**                              0x06: Reserved
**                              0x07: SARC Receive Path RPAISINS enable
**                                    Indirect Data #1
**                              0x08: SARC Receive Path RPAISINS enable
**                                    Indirect Data #2
**                              0x09: SARC Transmit Path TPAISINS Enable
**                                    Indirect Data
**                              0x0A: SARC MAPS Software Path L-Code
**                                    Configuration
**                              0x0B: SARC MAPS Path Configuration
**                              0x0C: SARC MAPS Alarm L-code status
**                              0x0D: SARC MAPS Extracted L-code Status
**                              0x0E: SARC Receive Path ERDI Enable
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gSARCIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr == 0x06) ||
       (iAddr > 0x0E) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_SARC_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_SARC_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_SARC_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gSARCIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gTTTPSectIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the TTTP_SECTION MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[6:0] bit field controls 
**                              which address location is accessed by the 
**                              current indirect transfer. 
**                              Valid values are:
**                              0x00: Configuration
**                              0x40: First byte of the 1/16/64 byte trace
**                              0x41-0x7F: Other bytes of the 16/64 byte trace
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTTTPSectIndRead(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 *idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( (iAddr > 0x0 && iAddr < 0x40) || iAddr > 0x7F)
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TTTP_SECT_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
                   
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TTTP_SECT_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTTTPSectIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gTTTPSectIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gTTTPSectInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTTTPSectIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 iAddr;
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_TTTP_SECT_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (iAddr=0; iAddr<=0x7F; iAddr++)
    {
        if (iAddr > 0x0 && iAddr < 0x40) 
            continue;
        
        result = sonetHyPhy20gTTTPSectIndRead(fileHndl, slice, iAddr,
                                    pDumpVal);
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        pCfgDumpReq->length += HYPHY20G_SONET_TTTP_SECT_IND_WIDTH;
        pDumpVal += HYPHY20G_SONET_TTTP_SECT_IND_WIDTH;
        dumpBuffSize += (HYPHY20G_SONET_TTTP_SECT_IND_WIDTH*sizeof(UINT4));  
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gTTTPSectIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gTTTPSectIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the TTTP_SECTION MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[6:0] bit field controls 
**                              which address location is accessed by the 
**                              current indirect transfer. 
**                              Valid values are:
**                              0x00: Configuration
**                              0x40: First byte of the 1/16/64 byte trace
**                              0x41-0x7F: Other bytes of the 16/64 byte trace
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTTTPSectIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    /* Confirm a valid slice has been specified */
    if (slice > 0xF) 
      return HYPHY20G_ERR_INVALID_ARG;
      
    /* Confirm a valid iAddr has been specified */
    if( (iAddr > 0x0 && iAddr < 0x40) || iAddr > 0x7F)
        return HYPHY20G_ERR_INVALID_ARG;

    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TTTP_SECT_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TTTP_SECT_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;
    
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_SECT_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTTTPSectIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gTHPPIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the THPP MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: THPP Control Register
**                              0x01: THPP Priority Configuration Register
**                              0x02: THPP B3MASK Register
**                              0x03: THPP J1 and C2 Register
**                              0x04: THPP G1 and H4MASK Register
**                              0x05: THPP F2 and Z3 Register
**                              0x06: THPP Z4 and Z5 Register
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTHPPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr, 
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_THPP_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_THPP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTHPPIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gTHPPIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gTHPPInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTHPPIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_THPP_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x6; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = sonetHyPhy20gTHPPIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_THPP_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_THPP_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_TTTP_SECT_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gTHPPIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gTHPPIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the THPP MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: THPP Control Register
**                              0x01: THPP Priority Configuration Register
**                              0x02: THPP B3MASK Register
**                              0x03: THPP J1 and C2 Register
**                              0x04: THPP G1 and H4MASK Register
**                              0x05: THPP F2 and Z3 Register
**                              0x06: THPP Z4 and Z5 Register
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTHPPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_THPP_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_THPP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_THPP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTHPPIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gTTTPPathIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the TTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[6:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Configuration
**                              0x40: First byte of the 1/16/64 byte trace
**                              0x41-0x7F: Other bytes of the 16/64 byte trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if(slice > 0xF ||
       (iAddr > 0x00 && iAddr < 0x40) ||
       iAddr > 0x7F ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTTTPPathIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gTTTPPathIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gTTTPPathInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_TTTP_PATH_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x7F; params[0]++)
    {
        /* jump over invalid iAddr from 1 to 0x39 */
        if (params[0] > 0x0 && params[0] < 0x40) 
            continue;
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = sonetHyPhy20gTTTPPathIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_TTTP_PATH_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_TTTP_PATH_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_TTTP_PATH_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gTTTPPathIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gTTTPPathIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the TTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[6:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Configuration
**                              0x40: First byte of the 1/16/64 byte trace
**                              0x41-0x7F: Other bytes of the 16/64 byte trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                   UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if(slice > 0xF ||
       (iAddr > 0x00 && iAddr < 0x40) ||
       iAddr > 0x7F ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTTTPPathIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gTSVCAIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the TSVCA MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SVCA-U Status
**                              0x1: SVCA-U Interrupts Enable
**                              0x2: SVCA-U Interrupts Event
**                              0x3: SVCA-U Outgoing Positive Justification
**                                   Performance Monitor
**                              0x4: SVCA-U Outgoing Negative Justification
**                                   Performance Monitor
**                              0x5: SVCA-U Diagnostic and Configuration
**                              0x6: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTSVCAIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x05) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TSVCA_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TSVCA_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTSVCAIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gTSVCAIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gTSVCAInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTSVCAIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_TSVCA_REG_IND_RD_WR_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x5; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = sonetHyPhy20gTSVCAIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_TSVCA_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_TSVCA_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_TSVCA_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gTSVCAIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gTSVCAIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the TSVCA MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SVCA-U Status
**                              0x1: SVCA-U Interrupts Enable
**                              0x2: SVCA-U Interrupts Event
**                              0x3: SVCA-U Outgoing Positive Justification
**                                   Performance Monitor
**                              0x4: SVCA-U Outgoing Negative Justification
**                                   Performance Monitor
**                              0x5: SVCA-U Diagnostic and Configuration
**                              0x6: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTSVCAIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x05) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TSVCA_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TSVCA_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSVCA_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSVCA_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTSVCAIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gRTERBIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the RTERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[1:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: TERB R1i Message (LSB)
**                              0x1: TERB R1i Message (MSB)
**                              0x2: TERB R1i Control
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTERBIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x02) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RTERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RTERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRTERBIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gRTERBIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gRTERBInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTERBIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_RTERB_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x2; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = sonetHyPhy20gRTERBIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_RTERB_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_RTERB_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_RTERB_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gRTERBIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gRTERBIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the RTERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[1:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: TERB R1i Message (LSB)
**                              0x1: TERB R1i Message (MSB)
**                              0x2: TERB R1i Control
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gRTERBIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x02) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_RTERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_RTERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_RTERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_RTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gRTERBIndWrite */

/*******************************************************************************
**
**  sonetHyPhy20gTRERBIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the TRERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: Indirect Interrupt Enable/Configuration
**                              0x1: Monitoring Status
**                              0x2: Indirect Interrupt Status
**                              0x3: Expected CID (LSB)
**                              0x4: Expected CID (MSB)/Accepted ESM
**                              0x5: CRC Error Count
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTRERBIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x5) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TRERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TRERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTRERBIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gTRERBIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gTRERBInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTRERBIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_TRERB_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x5; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = sonetHyPhy20gTRERBIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_TRERB_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_TRERB_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_TRERB_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gTRERBIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gTRERBIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the TRERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: Indirect Interrupt Enable/Configuration
**                              0x1: Monitoring Status
**                              0x2: Indirect Interrupt Status
**                              0x3: Expected CID (LSB)
**                              0x4: Expected CID (MSB)/Accepted ESM
**                              0x5: CRC Error Count
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTRERBIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x5) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TRERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TRERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TRERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTRERBIndWrite */


/*******************************************************************************
**
**  sonetHyPhy20gTSHPIIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads SONET indirect bits for the TSHPI MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTSHPIIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TSHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TSHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTSHPIIndRead */

/*******************************************************************************
**
**  sonetHyPhy20gTSHPIIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the sonetHyPhy20gTSHPIInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTSHPIIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_SONET_TSHPI_REG_IND_DAT)>>12;
    if (slice > 0xF)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x6; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_SONET_TSHPI_IND_WIDTH;
                pDumpVal += HYPHY20G_SONET_TSHPI_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_SONET_TSHPI_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* sonetHyPhy20gTSHPIIndDump */


/*******************************************************************************
**
**  sonetHyPhy20gTSHPIIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the TSHPI MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetHyPhy20gTSHPIIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0xF) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_SONET_TSHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_SONET_TSHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_SONET_TSHPI_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_SONET_TSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetHyPhy20gTSHPIIndWrite */

/*******************************************************************************
**
**  hoppHyPhy20gRSHPIIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the RSHPI MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRSHPIIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x6) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RSHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RSHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRSHPIIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gRSHPIIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gRSHPIInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRSHPIIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_RSHPI_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x6; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gRSHPIIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_RSHPI_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_RSHPI_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_RSHPI_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gRSHPIIndDump */

/*******************************************************************************
**
**  hoppHyPhy20gRSHPIIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the RSHPI MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[2:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: SHPI Indirect Interrupt Values
**                              0x1: SHPI Indirect Interrupt Enables
**                              0x2: SHPI Indirect Interrupt Status
**                              0x3: SHPI Configuration
**                              0x4: SHPI Path Negative Justification Event
**                                   Counter
**                              0x5: SHPI Path Positive Justification Event
**                                   Counter
**                              0x6: SHPI Pointer Value
**                              0x7: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRSHPIIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x6) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RSHPI_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RSHPI_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RSHPI_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRSHPIIndWrite */

/*******************************************************************************
**
**  hoppHyPhy20gRHAPIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the RHAP MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: RHAP-U Error Monitor Status
**                              0x1: RHAP-U Error Monitor Interrupt Enable
**                              0x2: RHAP-U Error Monitor Interrupt Status
**                              0x3: RHAP-U Monitor Configuration
**                              0x4: RHAP-U Expected PSL and PDI
**                              0x5: RHAP-U Filtered PERDI Value
**                              0x6: RHAP-U Captured and Accepted PSL
**                              0x7: RHAP-U J1 and F2
**                              0x8: RHAP-U H4 and Z3
**                              0x9: RHAP-U Z4 and Z5
**                              0xA: RHAP-U Path BIP Error Counter
**                              0xB: RHAP-U Path REI Error Counter
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRHAPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                             UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0xB) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RHAP_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RHAP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRHAPIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gRHAPIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gRHAPInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRHAPIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_RHAP_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0xB; params[0]++)
    {
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_RHAP_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_RHAP_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_RHAP_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gRHAPIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gRHAPIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the RSHPI MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: RHAP-U Error Monitor Status
**                              0x1: RHAP-U Error Monitor Interrupt Enable
**                              0x2: RHAP-U Error Monitor Interrupt Status
**                              0x3: RHAP-U Monitor Configuration
**                              0x4: RHAP-U Expected PSL and PDI
**                              0x5: RHAP-U Filtered PERDI Value
**                              0x6: RHAP-U Captured and Accepted PSL
**                              0x7: RHAP-U J1 and F2
**                              0x8: RHAP-U H4 and Z3
**                              0x9: RHAP-U Z4 and Z5
**                              0xA: RHAP-U Path BIP Error Counter
**                              0xB: RHAP-U Path REI Error Counter
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRHAPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0xB) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RHAP_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RHAP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RHAP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RHAP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRHAPIndWrite */


/*******************************************************************************
**
**  hoppHyPhy20gRTTPPathIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the RTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[7:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Traced Unstable Status;
                                      Trace Mismatch Status
**                              0x01: Trace Unstable Interrupt Enable;
**                                    Trace Mismatch Interrupt Enable
**                              0x02: RTTP-U Trace Unstable Interrupt Status;
**                                    RTTP-U Trace Mismatch Interrupt Status
**                              0x03: Configuration
**                              0x40: First Byte of the 1/16/64 byte captured
**                                    trace
**                              0x41-0x7F: Other Bytes of the 16/64 byte
**                                         captured trace
**                              0x80: First Byte of the 1/16/64 byte accepted
**                                    trace
**                              0x81-0xBF: Other Bytes of the 16/64 byte
**                                         accepted trace
**                              0xC0: First Byte of the 16/64 byte expected
**                                    trace
**                              0xC1-0xFF: Other Bytes of the 16/64 byte
**                                         expected trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                 UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if(slice > 0x3 ||
       (iAddr > 0x3 && iAddr < 0x40) ||
       iAddr > 0xFF ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                  iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                  sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                  sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRTTPPathIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gRTTPPathIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gRTTPPathInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_RTTP_PATH_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0xFF; params[0]++)
    {
        /*jump over the invalid iAddr space */
        if (params[0] > 0x3 && params[0] < 0x40) 
            continue;
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_RTTP_PATH_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_RTTP_PATH_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_RTTP_PATH_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gRTTPPathIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gRTTPPathIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the RTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[7:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Traced Unstable Status;
                                      Trace Mismatch Status
**                              0x01: Trace Unstable Interrupt Enable;
**                                    Trace Mismatch Interrupt Enable
**                              0x02: RTTP-U Trace Unstable Interrupt Status;
**                                    RTTP-U Trace Mismatch Interrupt Status
**                              0x03: Configuration
**                              0x40: First Byte of the 1/16/64 byte captured
**                                    trace
**                              0x41-0x7F: Other Bytes of the 16/64 byte
**                                         captured trace
**                              0x80: First Byte of the 1/16/64 byte accepted
**                                    trace
**                              0x81-0xBF: Other Bytes of the 16/64 byte
**                                         accepted trace
**                              0xC0: First Byte of the 16/64 byte expected
**                                    trace
**                              0xC1-0xFF: Other Bytes of the 16/64 byte
**                                         expected trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if(slice > 0x3 ||
       (iAddr > 0x3 && iAddr < 0x40) ||
       iAddr > 0xFF ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                  iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                  sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                  sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRTTPPathIndWrite */

/*******************************************************************************
**
**  hoppHyPhy20gSBERPathIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the SBER_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[4:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: PBER SD/SF BERM State
**                              0x01: PBER SD/SF BERM Interrupt Enable
**                              0x02: PBER SD/SF BERM Interrupt Status
**                              0x03: SD BERM BER Threshold
**                              0x04: SD BERM Set Threshold
**                              0x05: SD BERM Clear Threshold
**                              0x06: SD Path Monitoring Enable
**                              0x07: SD BERM Set Integration Period 2
**                              0x08: SD BERM Set Integration Period 1
**                              0x09-0x0C: Reserved
**                              0x13: SF BERM BER Threshold
**                              0x14: SF BERM Set Threshold
**                              0x15: SF BERM Clear threshold
**                              0x16: SF Path Monitoring Enable
**                              0x17: SF BERM Set Integration Period 2
**                              0x18: SF BERM Set Integration Period 1
**                              0x19-0x1C: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to STS-3/STM-1#16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gSBERPathIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                 UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x08 && iAddr < 0x13) ||
       (iAddr > 0x18) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_SBER_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_IADDR_MSK, 
                                  iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_STS_3_MSK,
                                  sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_STS_1_MSK,
                                  sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Indirect accesses will be complete within 80 us of being launched. */
    result = sysHyPhy20gUsDelay(80);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_SBER_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gSBERPathIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gSBERPathIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gSBERPathInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gSBERPathIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_SBER_PATH_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x18; params[0]++)
    {
        /* jump over the invalid iAddr space */
        if ((params[0] > 0x08) && (params[0] < 0x13))
            continue;
       
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gSBERPathIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_SBER_PATH_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_SBER_PATH_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_SBER_PATH_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gSBERPathIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gSBERPathIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes SONET indirect bits for the SBER_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0xF
**                iAddr       - The indirect address[4:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: PBER SD/SF BERM State
**                              0x01: PBER SD/SF BERM Interrupt Enable
**                              0x02: PBER SD/SF BERM Interrupt Status
**                              0x03: SD BERM BER Threshold
**                              0x04: SD BERM Set Threshold
**                              0x05: SD BERM Clear Threshold
**                              0x06: SD Path Monitoring Enable
**                              0x07: SD BERM Set Integration Period 2
**                              0x08: SD BERM Set Integration Period 1
**                              0x09-0x0C: Reserved
**                              0x13: SF BERM BER Threshold
**                              0x14: SF BERM Set Threshold
**                              0x15: SF BERM Clear threshold
**                              0x16: SF Path Monitoring Enable
**                              0x17: SF BERM Set Integration Period 2
**                              0x18: SF BERM Set Integration Period 1
**                              0x19-0x1C: Reserved
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 #1 to STS-3/STM-1#16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 #1 to STS-1/STM-0 #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gSBERPathIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x08 && iAddr < 0x13) ||
       (iAddr > 0x18) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_SBER_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_SBER_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_IADDR_MSK, 
                                  iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_STS_3_MSK,
                                  sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_STS_1_MSK,
                                  sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Indirect accesses will be complete within 80 us of being launched. */
    result = sysHyPhy20gUsDelay(80);
    if(result) return result;

    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SBER_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gSBERPathIndWrite */


/*******************************************************************************
**
**  hoppHyPhy20gSARCIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the SARC MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: SARC Receive Path LOP and AIS status
**                              0x01: SARC Receive Path LOP, 
**                                    AIS and COMAPS interrupt enable
**                              0x02: SARC Receive Path LOP,
**                                    AIS and COMAPS interrupt status
**                              0x03: Reserved
**                              0x04: Reserved
**                              0x05: Reserved
**                              0x06: Reserved
**                              0x07: SARC Receive Path RPAISINS enable
**                                    Indirect Data #1
**                              0x08: SARC Receive Path RPAISINS enable
**                                    Indirect Data #2
**                              0x09: Reserved
**                              0x0A: Reserved
**                              0x0B: Reserved
**                              0x0C: Reserved
**                              0x0D: Reserved
**                              0x0E: SARC Receive Path ERDI Enable 
**                                    Indirect Data
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gSARCIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                             UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr == 0x06) ||
       (iAddr > 0x0E) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        DBG_PRINT("iAddr checking: SARCINDREAD\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_SARC_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_SARC_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gSARCIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gSARCIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gSARCInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gSARCIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_SARC_REG_IND_RD_WR_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0xE; params[0]++)
    {
        /* jump over the invalid iAddr space */
        if (params[0] == 6) continue;
        
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gSARCIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_SARC_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_SARC_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_SARC_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gSARCIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gSARCIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the SARC MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: SARC Receive Path LOP and AIS status
**                              0x01: SARC Receive Path LOP, 
**                                    AIS and COMAPS interrupt enable
**                              0x02: SARC Receive Path LOP,
**                                    AIS and COMAPS interrupt status
**                              0x03: Reserved
**                              0x04: Reserved
**                              0x05: Reserved
**                              0x06: Reserved
**                              0x07: SARC Receive Path RPAISINS enable
**                                    Indirect Data #1
**                              0x08: SARC Receive Path RPAISINS enable
**                                    Indirect Data #2
**                              0x09: Reserved
**                              0x0A: Reserved
**                              0x0B: Reserved
**                              0x0C: Reserved
**                              0x0D: Reserved
**                              0x0E: SARC Receive Path ERDI Enable 
**                                    Indirect Data
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gSARCIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr == 0x06) ||
       (iAddr > 0x0E) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_SARC_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_SARC_REG_IND_RD_WR_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_SARC_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_SARC_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gSARCIndWrite */


/*******************************************************************************
**
**  hoppHyPhy20gTHPPIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the THPP MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: THPP Control Register
**                              0x01: THPP Priority Configuration Register
**                              0x02: THPP B3MASK Register
**                              0x03: THPP J1 and C2 Register
**                              0x04: THPP G1 and H4MASK Register
**                              0x05: THPP F2 and Z3 Register
**                              0x06: THPP Z4 and Z5 Register
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTHPPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                             UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_THPP_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;
      
    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_RWB_MSK,
                                   1);                               
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_THPP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    } 

    *idata0 = indData0;

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gTHPPIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gTHPPIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gTHPPInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTHPPIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_THPP_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x6; params[0]++)
    {        
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gTHPPIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_THPP_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_THPP_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_THPP_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gTHPPIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gTHPPIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the THPP MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[3:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: THPP Control Register
**                              0x01: THPP Priority Configuration Register
**                              0x02: THPP B3MASK Register
**                              0x03: THPP J1 and C2 Register
**                              0x04: THPP G1 and H4MASK Register
**                              0x05: THPP F2 and Z3 Register
**                              0x06: THPP Z4 and Z5 Register
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTHPPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x06) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    

    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_THPP_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Aquire Mutex for this memory */
    result = sysHyPhy20gMutexAquire (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
    if (result) return HYPHY20G_ERR_MUTEX_AQUIRE;
      
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_THPP_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_RWB_MSK,
                                   0);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_STS_3_MSK,
                                   sts3);                               
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_THPP_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_THPP_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if (result) {
        /* Release Mutex when the operation fails */ 
        sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
        return result;
    }

    /* Release Mutex for this memory */
    result = sysHyPhy20gMutexRelease (fileHndl, HYPHY20G_MUTEX_HOPP_THPP);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gTHPPIndWrite */


/*******************************************************************************
**
**  hoppHyPhy20gTTTPPathIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the TTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[6:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Configuration
**                              0x40: First byte of the 1/16/64 byte trace
**                              0x41-0x7F: Other bytes of the 16/64 byte trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 path #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                 UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if(slice > 0x3 ||
       (iAddr > 0x00 && iAddr < 0x40) ||
       iAddr > 0x7F ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_TTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_TTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gTTTPPathIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gTTTPPathIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gTTTPPathInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_TTTP_PATH_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x7F; params[0]++)
    {        
        /* jump over the invalid iAddr space */
       if ((params[0] > 0x00) && (params[0] < 0x40))
            continue;

        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gTTTPPathIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_THPP_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_THPP_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_THPP_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gTTTPPathIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gTTTPPathIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the TTTP_PATH MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[6:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x00: Configuration
**                              0x40: First byte of the 1/16/64 byte trace
**                              0x41-0x7F: Other bytes of the 16/64 byte trace
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF : STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 : STS-1/STM-0 path #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if(slice > 0x3 ||
       (iAddr > 0x00 && iAddr < 0x40) ||
       iAddr > 0x7F ||
       sts3 > 0xF ||
       sts1 > 0x2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_TTTP_PATH_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_TTTP_PATH_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                  HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                  1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_IADDR_MSK, 
                                 iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_STS_3_MSK,
                                 sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                 HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_STS_1_MSK,
                                 sts1);
    if(result) return result;
      
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTTP_PATH_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gTTTPPathIndWrite */


/*******************************************************************************
**
**  hoppHyPhy20gTTERBIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the TTERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[1:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: TERB R1i Message (LSB)
**                              0x1: TERB R1i Message (MSB)
**                              0x2: TERB R1i Control
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTTERBIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x2) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_TTERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_TTERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gTTERBIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gTTERBIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gTTERBInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTTERBIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_TTERB_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x2; params[0]++)
    {        
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gTTERBIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_TTERB_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_TTERB_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_TTERB_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gTTERBIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gTTERBIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the TTERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[1:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: TERB R1i Message (LSB)
**                              0x1: TERB R1i Message (MSB)
**                              0x2: TERB R1i Control
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gTTERBIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x2) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_TTERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_TTERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_TTERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_TTERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gTTERBIndWrite */


/*******************************************************************************
**
**  hoppHyPhy20gRRERBIndRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Reads HOPP indirect bits for the RRERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[1:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: Indirect Interrupt Enable/Configuration
**                              0x1: Monitoring Status
**                              0x2: Indirect Interrupt Status
**                              0x3: Expected CID (LSB)
**                              0x4: Expected CID (MSB)/Accepted ESM
**                              0x5: CRC Error Count
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                *idata0     - pointer to storage for data reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRRERBIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0)
{ 
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    UINT4 indData0;
    INT4  result;

    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x5) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RRERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Specify the memSel, memAddr and initiate the indirect read access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_RWB_MSK,
                                   1);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;

    /* Poll BUSY bit to ensure data is ready to be read */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read indirect access data from indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RRERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);
    
    result = sysHyPhy20gRead(fileHndl, indDataReg0, &indData0);
    if(result) return result; 

    *idata0 = indData0;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRRERBIndRead */

/*******************************************************************************
**
**  hoppHyPhy20gRRERBIndDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the hoppHyPhy20gRRERBInd  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRRERBIndDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 slice;
    UINT4 *pDumpVal;
    UINT4 params[3];

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_HOPP_RRERB_REG_IND_DAT)>>12;
    if (slice > 0x3)
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop the param iAddr */
    for (params[0]=0; params[0]<=0x5; params[0]++)
    {        
        /* loop the param sts3 */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            /* loop the param sts1 */
            for (params[2]=0; params[2]<=0x2; params[2]++)
            {
                result = hoppHyPhy20gRRERBIndRead(fileHndl, slice, params[0],
                                            params[1],params[2],pDumpVal);
                if(result) 
                {
                    pCfgDumpReq->rtCode = result;
                    (*pValidDumpBuffSize) = dumpBuffSize;
                    return result;
                }
                pCfgDumpReq->length += HYPHY20G_HOPP_RRERB_IND_WIDTH;
                pDumpVal += HYPHY20G_HOPP_RRERB_IND_WIDTH;
                dumpBuffSize += (HYPHY20G_HOPP_RRERB_IND_WIDTH*sizeof(UINT4));  
            }
        }
    }
    
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* hoppHyPhy20gRRERBIndDump */


/*******************************************************************************
**
**  hoppHyPhy20gRRERBIndWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:  Writes HOPP indirect bits for the RRERB MTSB.
**
**  INPUTS:       *fileHndl   - base address
**                slice       - valid values are:
**                              0x0 ~ 0x3
**                iAddr       - The indirect address[1:0] bits field controls 
**                              which address location is accessed by the 
**                              current indirect transfer.
**                              Valid values are:
**                              0x0: Indirect Interrupt Enable/Configuration
**                              0x1: Monitoring Status
**                              0x2: Indirect Interrupt Status
**                              0x3: Expected CID (LSB)
**                              0x4: Expected CID (MSB)/Accepted ESM
**                              0x5: CRC Error Count
**                sts3        - The STS_3[3:0] bits select which STS-3/STM-1 
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0xF :STS-3/STM-1 path #1 to #16
**                sts1        - The STS_1[1:0] bits select which STS-1/STM-0
**                              path is accessed by the current indirect
**                              transfer.
**                              Valid values:
**                              0x0 - 0x2 :STS-1/STM-0 paths #1 to #3
**                idata0     - data for reg 0 (bits 15:0)
**
**  OUTPUTS:      None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hoppHyPhy20gRRERBIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0)
{
    UINT4 indAddrReg;
    UINT4 indAddrData;
    UINT4 indDataReg0;
    INT4  result;
    
    /* Argument Checking */
    if((slice > 0x3) ||
       (iAddr > 0x5) ||
       (sts3 > 0xF) ||
       (sts1 > 0x2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    indAddrReg = HYPHY20G_HOPP_RRERB_REG_IND_ADR;
    indAddrReg = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indAddrReg, slice);

    /* Poll BUSY bit to ensure register is ready indirect access */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;
    
    /* Write indirect access data to indirect data registers */
    indDataReg0 = HYPHY20G_HOPP_RRERB_REG_IND_DAT;
    indDataReg0 = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(indDataReg0, slice);

    result = sysHyPhy20gWrite(fileHndl, indDataReg0, idata0);
    if(result) return result;

    /* Specify the memSel,memAddr and initiate the indirect write access */
    result = sysHyPhy20gRead(fileHndl, indAddrReg, &indAddrData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_BUSY_MSK,
                                   1);
    if(result) return result;  
          
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_RWB_MSK,
                                   0);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_IADDR_MSK, 
                                   iAddr);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_STS_3_MSK,
                                   sts3);
    if(result) return result;
      
    result = hyPhy20gVarFieldWrite(&indAddrData,
                                   HYPHY20G_HOPP_RRERB_BIT_IND_ADR_STS_1_MSK,
                                   sts1);
    if(result) return result;
                 
    result = sysHyPhy20gWrite(fileHndl, indAddrReg, indAddrData);
    if(result) return result;  
 
    /* Poll BUSY bit to ensure indirect write access completes */
    result = sysHyPhy20gPollBit(fileHndl, indAddrReg,
                                HYPHY20G_HOPP_RRERB_BIT_IND_ADR_BUSY_MSK,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hoppHyPhy20gRRERBIndWrite */


/*******************************************************************************
**
**  pifHyPhy20gIntlnkCalRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Read flow control's programmable calender information
**                  from a specified programmable calendar location.
**  INPUTS:         *fileHndl - base address
**                  dir       - direction
**                               0 - TX (FC Tx from HyPHY 20G in BCW)
**                               1 - RX (FC HyPHY 20G Receives in BCW)
**                  cal_addr  - the location of programmable calendar to access
**                  *idata    - pointer to storage for data reg 1 (bits 7:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gIntlnkCalRead (struct file *fileHndl, UINT4 dir, UINT4 cal_addr,
                               UINT4 *data)
{
    UINT4 reg0; //0x495E0C8
    UINT4 reg1; //0x495E0D8
    UINT4 wrd1;
    UINT4 val;
    INT4 result;

    /* Argument Checking */
    if( (dir >0x1) || (cal_addr > 0xFF) ) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    reg0 = HYPHY20G_PIF_INTLKN_CORE_REG_PROG_CLDR_ACC_OFFSET(dir);
    reg1 = HYPHY20G_PIF_INTLKN_CORE_REG_PROG_CLDR_RD_DAT_OFFSET(dir);

    result = sysHyPhy20gRead(fileHndl, reg0, &val);
    if(result) return result;

    /* Write "cal_addr" to the control register "reg0" */
    result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_PIF_INTLKN_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_ADDR_MSK,
                    cal_addr);
    if(result) return result;

    /* Trigger a read operation */
    result = hyPhy20gVarFieldWrite(&val,
                     HYPHY20G_PIF_INTLKN_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_CMD_MSK,
                     0x1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg0, val);
    if(result) return result;

    /* Retrieve the data */
    result = sysHyPhy20gRead (fileHndl, reg1, &wrd1);
    if (result) return result;
    *data = wrd1; /* passing the content from &wrd1 */

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gIntlnkCalRead */

/*******************************************************************************
**
**  pifHyPhy20gIntlnkCalDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the pifHyPhy20gIntlnkCal  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gIntlnkCalDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[2];
    UINT4 *pDumpVal;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

    /* loop the dir param*/
    for (params[0]=0; params[0]<=1; params[0]++)
    {
        /* loop the cal_addr param */
        for (params[1]=0; params[1]<=0xFF; params[1]++)
        {
            result = pifHyPhy20gIntlnkCalRead(fileHndl, params[0], params[1], pDumpVal);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_PIF_INTLNK_CAL_WIDTH;
            pDumpVal += HYPHY20G_PIF_INTLNK_CAL_WIDTH;
            dumpBuffSize += HYPHY20G_PIF_INTLNK_CAL_WIDTH*sizeof(UINT4);  
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* pifHyPhy20gIntlnkCalDump */


/*******************************************************************************
**
**  pifHyPhy20gIntlnkCalWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Write flow control's programmable calender information
**                  from a specified programmable calendar location.
**  INPUTS:         *fileHndl - base address
**                  dir       - direction
**                                0 - TX
**                                1 - RX
**                  cal_addr  - the location of programmable calendar to access
**                  data      - data reg 1 (bits 7:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gIntlnkCalWrite (struct file *fileHndl, UINT4 dir,
                                UINT4 cal_addr, UINT4 data)
{
    UINT4 reg0; //0x495E0C8
    UINT4 reg1; //0x495E0D0
    UINT4 val;
    INT4 result;

    /* Argument Checking */
    if( (dir >0x1) || (cal_addr > 0xFF) || (data > 0x97) ) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    reg0 = HYPHY20G_PIF_INTLKN_CORE_REG_PROG_CLDR_ACC_OFFSET(dir);
    reg1 = HYPHY20G_PIF_INTLKN_CORE_REG_PROG_CLDR_WR_DAT_OFFSET(dir);

    /* Write data to reg1 */
    result = hyPhy20gRegFieldWrite (fileHndl, reg1,
                    HYPHY20G_PIF_INTLKN_CORE_BIT_PROG_CLDR_WR_DAT_CAL_WDATA_MSK,
                    data);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, reg0, &val);
    if(result) return result;

    /* Write "cal_addr" to the control register "reg0" */
    result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_PIF_INTLKN_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_ADDR_MSK,
                    cal_addr);
    if(result) return result;

    /* Trigger a write operation */
    result = hyPhy20gVarFieldWrite(&val,
                     HYPHY20G_PIF_INTLKN_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_CMD_MSK,
                     0x2);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg0, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gIntlnkCalWrite */


/*******************************************************************************
**
**  cbriHyphy20gIntlnkCalRead
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Read flow control's programmable calender information
**                  from a specified programmable calendar location.
**  INPUTS:         *fileHndl  - base address
**                  slice      - valid values are:
**                                 0x0, 0x1
**                  dir        - direction
**                                 0 - TX
**                                 1 - RX
**                  cal_addr   - the location of programmable calendar to access
**                  *idata     - pointer to storage for data reg 1 (bits 7:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cbriHyphy20gIntlnkCalRead (struct file *fileHndl, UINT4 slice, UINT4 dir,
                                UINT4 cal_addr, UINT4 *data)

{
    UINT4 reg0; //0x495A0C8 + 0x00800*A(A=0:1) + 0x200*M(M=0:1)
    UINT4 reg1; //0x495A0D8 + 0x00800*A(A=0:1) + 0x200*M(M=0:1)
    UINT4 wrd1;
    UINT4 val;
    INT4 result;

    /* Argument Checking */
    if( (slice > 1) || (dir > 0x1) || (cal_addr > 0xF) ) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    reg0 = HYPHY20G_CBRI_CORE_REG_PROG_CLDR_ACC_OFFSET(dir);
    reg0 = reg0 + 0x00800 * slice;
    reg1 = HYPHY20G_CBRI_CORE_REG_PROG_CLDR_RD_DAT_OFFSET(dir);
    reg1 = reg1 + 0x00800 * slice;

    result = sysHyPhy20gRead(fileHndl, reg0, &val);
    if(result) return result;

    /* Write "cal_addr" to the control register "reg0" */
    result = hyPhy20gVarFieldWrite(&val,
                          HYPHY20G_CBRI_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_ADDR_MSK,
                          cal_addr);
    if(result) return result;

    /* Trigger a read operation */
    result = hyPhy20gVarFieldWrite(&val,
                           HYPHY20G_CBRI_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_CMD_MSK,
                           0x1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg0, val);
    if(result) return result;

    /* Retrieve the data */
    result = sysHyPhy20gRead (fileHndl, reg1, &wrd1);
    if (result) return result;
    *data = wrd1; /* passing the content from &wrd1 */

    return HYPHY20G_SUCCESS;
} /* cbriHyphy20gIntlnkCalRead */

/*******************************************************************************
**
**  cbriHyphy20gIntlnkCalDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs the cbriHyphy20gIntlnkCal  
**                 register dump. 
**                 Before calling the function,the cfgDumpReq content should be
**                 copied to the pCfgDumpBuf from the pCfgDumpReqList, point   
**                 the pDumpBuff to this cfgDumpReq in the pCfgDumpBuf. 
**                 The function will store the registers value after the
**                 cfgDumpReq head and count how many bytes are stored in 
**                 the dump Buff.
**
**  INPUTS:        fileHndl   - device handler
**                 pDumpBuff  - base pointer to one cfgDumpReq in the pCfgDumpBuf
**
**  OUTPUTS:       pValidDumpBuffSize - return the size including the cfgDumpReq
**                 header and registers value, unit: 1BYTE.
**                 pDumpBuff - return pointer to one cfgDumpReq followed with 
**                 register dump data in the pCfgDumpBuf.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cbriHyphy20gIntlnkCalDump(struct file *fileHndl, void *pDumpBuff, 
                                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 params[2];
    UINT4 *pDumpVal;
    UINT4 slice;

    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    pCfgDumpReq->length = 0;

    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    slice = (pCfgDumpReq->startAddr - HYPHY20G_CBRI_CORE_REG_PROG_CLDR_RD_DAT_OFFSET(0) )>>11;
    if (slice > 1) return HYPHY20G_ERR_INVALID_ARG;

    /* loop the dir param*/
    for (params[0]=0; params[0]<=1; params[0]++)
    {
        /* loop the cal_addr param */
        for (params[1]=0; params[1]<=0xF; params[1]++)
        {
            result = cbriHyphy20gIntlnkCalRead(fileHndl, slice, params[0], params[1], pDumpVal);
            if(result) 
            {
                pCfgDumpReq->rtCode = result;
                (*pValidDumpBuffSize) = dumpBuffSize;
                return result;
            }
            pCfgDumpReq->length += HYPHY20G_CBRI_INTLNK_CAL_WIDTH;
            pDumpVal += HYPHY20G_CBRI_INTLNK_CAL_WIDTH;
            dumpBuffSize += HYPHY20G_CBRI_INTLNK_CAL_WIDTH*sizeof(UINT4);  
        }
    }

    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* cbriHyphy20gIntlnkCalDump */



/*******************************************************************************
**
**  cbriHyphy20gIntlnkCalWrite
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Write flow control's programmable calender information
**                  from a specified programmable calendar location.
**  INPUTS:         *fileHndl  - base address
**                  slice      - valid values are:
**                                 0x0, 0x1
**                  dir        - direction
**                                 0 - TX
**                                 1 - RX
**                  cal_addr  - the location of programmable calendar to access
**                              Legal range 0 to 9
**                  data      - data reg 1 (bits 7:0)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cbriHyphy20gIntlnkCalWrite (struct file *fileHndl, UINT4 slice, UINT4 dir,
                                 UINT4 cal_addr, UINT4 data)
{
    UINT4 reg0; //0x495A0C8 + 0x00800*A(A=0:1) + 0x200*M(M=0:1)
    UINT4 reg1; //0x495A0D0 + 0x00800*A(A=0:1) + 0x200*M(M=0:1):
    UINT4 val;
    INT4 result;

    /* Argument Checking */
    if( (slice > 0x1 ) || (dir > 0x1) || (cal_addr > 0xF) || (data > 0x9) ) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    reg0 = HYPHY20G_CBRI_CORE_REG_PROG_CLDR_ACC_OFFSET(dir);
    reg0 = reg0 + 0x00800 * slice;
    reg1 = HYPHY20G_CBRI_CORE_REG_PROG_CLDR_WR_DAT_OFFSET(dir);
    reg1 = reg1 + 0x00800 * slice;

    /* Write data to reg1 */
    result = hyPhy20gRegFieldWrite (fileHndl, reg1,
                          HYPHY20G_CBRI_CORE_BIT_PROG_CLDR_WR_DAT_CAL_WDATA_MSK,
                          data);
    if (result) return result;

    result = sysHyPhy20gRead(fileHndl, reg0, &val);
    if(result) return result;

    /* Write "cal_addr" to the control register "reg0" */
    result = hyPhy20gVarFieldWrite(&val,
                          HYPHY20G_CBRI_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_ADDR_MSK,
                          cal_addr);
    if(result) return result;

    /* Trigger a write operation */
    result = hyPhy20gVarFieldWrite(&val,
                           HYPHY20G_CBRI_CORE_BIT_PROG_CLDR_ACC_CAL_ACC_CMD_MSK,
                           0x2);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg0, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* cbriHyphy20gIntlnkCalWrite */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
