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
**  FILE        : hyphy20g_api.c
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
#include "hyphy20g_api.h"
#include "hyphy20g_indirect.h"
#include "hyphy20g_hw.h"

/******************************************************************************/
/* PRIVATE CONFIGURATION DUMP TYPE DEFINITIONS                                */
/******************************************************************************/
#define REG_DUMP_END_MARK 0xFFFFFFFF

/* the HYPHY20G chip MIN and MAX register address */
#define HYPHY20G_REG_MIN_ADDR                   0x1000000
#define HYPHY20G_REG_MAX_ADDR                   0xE000FFC

/*register config dump Function*/
typedef INT4 (*CFG_DUMP_FUNC_TYPE)(struct file *, void *, UINT4 *);

/*register configuration dump callback structure*/
typedef struct {
    UINT4 startAddr;            /*startAddr of the register block, 4 bytes alignment*/
    UINT4 blockLen;             /*the block length, unit 4 bytes */
    CFG_DUMP_FUNC_TYPE  cfg_dump_func;  /*config dump function*/
} sHYPHY_CFG_DUMP_ENTRY_TYPE; 

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/* hyphyCfgDumpEntry used to map the register dump functions to 
 * each specific register block.
 */
PRIVATE sHYPHY_CFG_DUMP_ENTRY_TYPE hyphyCfgDumpEntry[]= {
    {0x49208C8, 0x618, vcatHyPhy20gRVCPOMemIndDump},
    {0x4920AC8, 0x618, vcatHyPhy20gRVCPOMemIndDump},
    {0x49208E4, 8, vcatHyPhy20gRVCPOIntIndDump},
    {0x4920AE4, 8, vcatHyPhy20gRVCPOIntIndDump},
    {0x4920548, 0x48, vcatHyPhy20gTVCPOMemIndDump},
    {0x4920748, 0x48, vcatHyPhy20gTVCPOMemIndDump},
    {0x4920558, 4, vcatHyPhy20gTVCPOIntIndDump},
    {0x4920758, 4, vcatHyPhy20gTVCPOIntIndDump},
    {0x495E840, 0x200, pifHyPhy20gRXCalProgIndDump},
    {0x495E860, 0x200, pifHyPhy20gTXCalProgIndDump},
    {0x4928820, 0x24C, cpbHyPhy20gWrPtrCtxtIndDump},
    {0x492882C, 0x252, cpbHyPhy20gRdPtrCtxtIndDump},
    {0x4928838, 0x372, cpbHyPhy20gFifoDepthIndDump},
    {0x4921208, 0xC0, gfpfHyPhy20gTXDPChanIndDump},
    {0x4921A08, 0xC0, gfpfHyPhy20gTXDPChanIndDump},
    {0x4921218, 0x1400, gfpfHyPhy20gTXDPExtHdrIndDump},       
    {0x4921A18, 0x1400, gfpfHyPhy20gTXDPExtHdrIndDump},
    {0x49212DC, 0x300, gfpfHyPhy20gTXDPPerfMonIndDump},       
    {0x4921ADC, 0x300, gfpfHyPhy20gTXDPPerfMonIndDump},
    {0x4921408, 0x1500, gfpfHyPhy20gRXDPChanIndDump},                 
    {0x4921C08, 0x1500, gfpfHyPhy20gRXDPChanIndDump},
    {0x4924C08, 0x40, gfptHyPhy20gRXGFPTChanIndDump},                 
    {0x4926C08, 0x40, gfptHyPhy20gRXGFPTChanIndDump},
    {0x4924C54, 0x100, gfptHyPhy20gRXGFPTPerfMonIndDump},                 
    {0x4926C54, 0x100, gfptHyPhy20gRXGFPTPerfMonIndDump},
    {0x4925008, 0x80, gfptHyPhy20gTXGFPTReqProChanIndDump},                 
    {0x4927008, 0x80, gfptHyPhy20gTXGFPTReqProChanIndDump},
    {0x4925014, 0x80, gfptHyPhy20gTXGFPTChanIndDump},  
    {0x4927014, 0x80, gfptHyPhy20gTXGFPTChanIndDump},
    {0x492505C, 0x100, gfptHyPhy20gTXGFPTPerfMonIndDump},                        
    {0x492705C, 0x100, gfptHyPhy20gTXGFPTPerfMonIndDump},
    {0x4920140, 0x5A08, vcatHyPhy20gTVCPMemIndDump},
    {0x4920168, 0xC0, vcatHyPhy20gTVCPIntIndDump},
    {0x492026C, 0x33E10, vcatHyPhy20gRVCPMemIndIndDump},
    {0x4920294, 0x100, vcatHyPhy20gRVCPIntIndDump},
    {0x4932504, 0x78, tseHyPhy20gPRGMIndDump},                        
    {0x4932544, 0x78, tseHyPhy20gPRGMIndDump},                        
    {0x4932584, 0x78, tseHyPhy20gPRGMIndDump},                        
    {0x49325C4, 0x78, tseHyPhy20gPRGMIndDump},
    {0x4932604, 0x150, tseHyPhy20gSHPIIndDump},
    {0x4932644, 0x780, tseHyPhy20gPCTLCfgMemIndDump},
    {0x4934104, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4934144, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4934184, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x49341C4, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4934904, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4934944, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4934984, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x49349C4, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4935104, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4935144, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4935184, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x49351C4, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4935904, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4935944, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4935984, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x49359C4, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4936104, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4936144, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4936184, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x49361C4, 0x150, dmiHyPhy20gSHPIIndDump},
    {0x4934304, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4934344, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4934384, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x49343C4, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4934B04, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4934B44, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4934B84, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4934BC4, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4935304, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4935344, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4935384, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x49353C4, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4935B04, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4935B44, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4935B84, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4935BC4, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4936304, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4936344, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4936384, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x49363C4, 0xC0, dmiHyPhy20gSTFAIndDump},
    {0x4940104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4941104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4942104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4943104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4944104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4945104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4946104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4947104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4948104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4949104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x494A104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x494B104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x494C104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x494D104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x494E104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x494F104, 0xF, sonetHyPhy20gSBERSectIndDump},
    {0x4940184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4941184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4942184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4943184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4944184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4945184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4946184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4947184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4948184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x4949184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x494A184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x494B184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x494C184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x494D184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x494E184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x494F184, 0xC4, sonetHyPhy20gRTTPSectIndDump},
    {0x49401C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49411C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49421C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49431C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49441C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49451C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49461C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49471C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49481C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x49491C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x494A1C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x494B1C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x494C1C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x494D1C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x494E1C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x494F1C4, 0x150, sonetHyPhy20gRSHPIIndDump},
    {0x4940204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4941204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4942204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4943204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4944204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4945204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4946204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4947204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4948204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4949204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x494A204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x494B204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x494C204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x494D204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x494E204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x494F204, 0x240, sonetHyPhy20gRHAPIndDump},
    {0x4940244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4941244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4942244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4943244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4944244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4945244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4946244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4947244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4948244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4949244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x494A244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x494B244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x494C244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x494D244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x494E244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x494F244, 0x24C0, sonetHyPhy20gRTTPPathIndDump},
    {0x4940284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4941284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4942284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4943284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4944284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4945284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4946284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4947284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4948284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4949284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x494A284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x494B284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x494C284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x494D284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x494E284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x494F284, 0x120, sonetHyPhy20gRSVCAIndDump},
    {0x4940344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4941344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4942344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4943344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4944344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4945344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4946344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4947344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4948344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4949344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x494A344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x494B344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x494C344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x494D344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x494E344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x494F344, 0x2D0, sonetHyPhy20gSBERPathIndDump},
    {0x4940404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4941404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4942404, 0x2A0, sonetHyPhy20gSARCIndDump},       
    {0x4943404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4944404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4945404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4946404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4947404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4948404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4949404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x494A404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x494B404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x494C404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x494D404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x494E404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x494F404, 0x2A0, sonetHyPhy20gSARCIndDump},
    {0x4940584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4941584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4942584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4943584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4944584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4945584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4946584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4947584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4948584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x4949584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x494A584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x494B584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x494C584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x494D584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x494E584, 0x41, sonetHyPhy20gTTTPSectIndDump},
    {0x494F584, 0x41, sonetHyPhy20gTTTPSectIndDump},	
    {0x49405C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x49415C4, 0x150, sonetHyPhy20gTHPPIndDump},  
    {0x49425C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x49435C4, 0x150, sonetHyPhy20gTHPPIndDump},  
    {0x49445C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x49455C4, 0x150, sonetHyPhy20gTHPPIndDump},  
    {0x49465C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x49475C4, 0x150, sonetHyPhy20gTHPPIndDump},  
    {0x49485C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x49495C4, 0x150, sonetHyPhy20gTHPPIndDump},  
    {0x494A5C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x494B5C4, 0x150, sonetHyPhy20gTHPPIndDump},  
    {0x494C5C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x494D5C4, 0x150, sonetHyPhy20gTHPPIndDump},  
    {0x494E5C4, 0x150, sonetHyPhy20gTHPPIndDump},
    {0x494F5C4, 0x150, sonetHyPhy20gTHPPIndDump}, 
    {0x4940644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4941644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4942644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4943644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4944644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4945644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4946644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4947644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4948644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4949644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x494A644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x494B644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x494C644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x494D644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x494E644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x494F644, 0xC30, sonetHyPhy20gTTTPPathIndDump},
    {0x4940684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4941684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4942684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4943684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4944684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4945684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4946684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4947684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4948684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4949684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x494A684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x494B684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x494C684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x494D684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x494E684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x494F684, 0x120, sonetHyPhy20gTSVCAIndDump},
    {0x4940804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4941804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4942804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4943804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4944804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4945804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4946804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4947804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4948804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4949804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x494A804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x494B804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x494C804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x494D804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x494E804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x494F804, 0x90, sonetHyPhy20gRTERBIndDump},
    {0x4940844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4941844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4942844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4943844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4944844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4945844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4946844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4947844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4948844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4949844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x494A844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x494B844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x494C844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x494D844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x494E844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x494F844, 0x120, sonetHyPhy20gTRERBIndDump},
    {0x4940884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4941884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4942884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4943884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4944884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4945884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4946884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4947884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4948884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x4949884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x494A884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x494B884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x494C884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x494D884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x494E884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x494F884, 0x150, sonetHyPhy20gTSHPIIndDump},
    {0x49541C4, 0x150, hoppHyPhy20gRSHPIIndDump},
    {0x49551C4, 0x150, hoppHyPhy20gRSHPIIndDump},
    {0x49561C4, 0x150, hoppHyPhy20gRSHPIIndDump},
    {0x49571C4, 0x150, hoppHyPhy20gRSHPIIndDump},
    {0x4954204, 0x240, hoppHyPhy20gRHAPIndDump},
    {0x4955204, 0x240, hoppHyPhy20gRHAPIndDump},
    {0x4956204, 0x240, hoppHyPhy20gRHAPIndDump},
    {0x4957204, 0x240, hoppHyPhy20gRHAPIndDump},
    {0x4954244, 0x24C0, hoppHyPhy20gRTTPPathIndDump},
    {0x4955244, 0x24C0, hoppHyPhy20gRTTPPathIndDump},
    {0x4956244, 0x24C0, hoppHyPhy20gRTTPPathIndDump},
    {0x4957244, 0x24C0, hoppHyPhy20gRTTPPathIndDump},
    {0x4954344, 0x2D0, hoppHyPhy20gSBERPathIndDump},
    {0x4955344, 0x2D0, hoppHyPhy20gSBERPathIndDump},
    {0x4956344, 0x2D0, hoppHyPhy20gSBERPathIndDump},
    {0x4957344, 0x2D0, hoppHyPhy20gSBERPathIndDump},
    {0x4954404, 0x2A0, hoppHyPhy20gSARCIndDump},
    {0x4955404, 0x2A0, hoppHyPhy20gSARCIndDump},
    {0x4956404, 0x2A0, hoppHyPhy20gSARCIndDump},
    {0x4957404, 0x2A0, hoppHyPhy20gSARCIndDump},
    {0x59545C4, 0x150, hoppHyPhy20gTHPPIndDump},
    {0x59555C4, 0x150, hoppHyPhy20gTHPPIndDump},
    {0x59565C4, 0x150, hoppHyPhy20gTHPPIndDump},
    {0x59575C4, 0x150, hoppHyPhy20gTHPPIndDump},
    {0x4954644, 0xC30, hoppHyPhy20gTTTPPathIndDump},
    {0x4955644, 0xC30, hoppHyPhy20gTTTPPathIndDump},
    {0x4956644, 0xC30, hoppHyPhy20gTTTPPathIndDump},
    {0x4957644, 0xC30, hoppHyPhy20gTTTPPathIndDump},
    {0x4954804, 0x90, hoppHyPhy20gTTERBIndDump},
    {0x4955804, 0x90, hoppHyPhy20gTTERBIndDump},
    {0x4956804, 0x90, hoppHyPhy20gTTERBIndDump},
    {0x4957804, 0x90, hoppHyPhy20gTTERBIndDump},
    {0x4954844, 0x120, hoppHyPhy20gRRERBIndDump},
    {0x4955844, 0x120, hoppHyPhy20gRRERBIndDump},
    {0x4956844, 0x120, hoppHyPhy20gRRERBIndDump},
    {0x4957844, 0x120, hoppHyPhy20gRRERBIndDump},
    {0x495E0D8, 0x200,pifHyPhy20gIntlnkCalDump},
    {0x495A0D8, 0x20,cbriHyphy20gIntlnkCalDump},
    {0x495A8D8, 0x20,cbriHyphy20gIntlnkCalDump},
};

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
PRIVATE sHYPHY_CFG_DUMP_ENTRY_TYPE *utilHyPhy20gConfigDumpSearchEntry(UINT4 addr);
PRIVATE INT4 untilHyPhy20gDump(struct file *fileHndl, void *pDumpBuff,UINT4 *pValidDumpBuffSize);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Verifies the presence of a new device in the hardware, then
**                  locate memory for the HyPHY 20G Context.
**                  The device handle should be used to identify the device on
**                  which the operation is to be performed. If baseAddr point
**                  to a invalid device, the error code HYPHY20G_ERR_INVALID_DEV
**                  will return.
**
**  INPUTS:         fileHndl  - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   Changes the DEVICE state to HYPHY20G_PRESENT
**
**  RETURNS:        HYPHY20G_ERR_CTXT_ALREADY_VALID
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCreateCtxt(struct file *fileHndl)
{
    UINT2            realDeviceId = 0, realRev = 0, mimicRevId = 0, mimicDeviceID = 0;
    UINT4            value, i;
    UINT4            msk, off;
    sHYPHY20G_CTXT  *pDevCtxt;
    INT4             result;


    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* if pDevCtxt is valid,  don't modify it */
    if(pDevCtxt->valid == HYPHY20G_VALID_SIGN) {
    	return HYPHY20G_ERR_CTXT_ALREADY_VALID;
    }

    result = sysHyPhy20gRead (fileHndl, HYPHY20G_TOP_REG_DEV_ID, &value);
    if (result) return result;

    /* get part number of device */
    msk = HYPHY20G_TOP_BIT_DEV_ID_DEVID_MSK;
    off = HYPHY20G_TOP_BIT_DEV_ID_DEVID_OFF;
    realDeviceId  = (UINT2)((value & msk) >> off);
    mimicDeviceID = (UINT2)((value & msk) >> off);
    
    /* get revision number of device */
    msk = HYPHY20G_TOP_BIT_DEV_ID_REV_CODE_MSK;
    off = HYPHY20G_TOP_BIT_DEV_ID_REV_CODE_OFF;

    realRev = (UINT2)((value & msk) >> off);
	
    if (pDevCtxt->gData.forceRevId) {
        /* force revision ID to revB (0x1) or revC (0x2) */
        mimicRevId = (UINT2)pDevCtxt->gData.forceRevId;
    } else {
        /* use hardware revision ID */
        mimicRevId = (UINT2)((value & msk) >> off);
    }

    /* evaluation of compatibility between SW driver and device */
    if(pDevCtxt->gData.hyphy20gCheckRev){
            if(!(((realDeviceId == HYPHY20G_HYPHY20G_DEVICE)   ||
                (realDeviceId == HYPHY20G_HYPHY10G_DEVICE) ||
                (realDeviceId == HYPHY20G_META20G_DEVICE))  &&
                (mimicRevId == HYPHY20G_REV_ID_B || mimicRevId == HYPHY20G_REV_ID_C || mimicRevId == HYPHY20G_REV_ID_D))){
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INCOMPATIBLE_DEVICE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INCOMPATIBLE_DEVICE;
            }
    }

    /* Initialize the relevant HyPHY 20G Context fields */
    pDevCtxt->version = HYPHY20G_DRIVER_VERSION_CODE;

    /* save device ID and revision information */
    pDevCtxt->realDeviceId = realDeviceId;
    pDevCtxt->realRevId    = realRev;

    /* Set mimic device ID and revision information */
    pDevCtxt->mimicDeviceID = mimicDeviceID;
    pDevCtxt->mimicRevId    = mimicRevId;

    /* set device state to PRESENT and make context valid */
    pDevCtxt->stateDevice = HYPHY20G_PRESENT;

    /* initialize the base address of device to NULL */
    pDevCtxt->baseAddr = NULL;

    /* initialize subsystem context pointers to NULL */
    pDevCtxt->pCpbCtxt = NULL;
    pDevCtxt->pGfpACtxt = NULL;
    pDevCtxt->pGfpBCtxt = NULL;
    pDevCtxt->pCbriCtxt[0] = NULL;
    pDevCtxt->pCbriCtxt[1] = NULL;
    pDevCtxt->pTseCtxt = NULL;
    pDevCtxt->pPifCtxt = NULL;
    pDevCtxt->pVcatACtxt = NULL;
    pDevCtxt->pVcatBCtxt = NULL;
    for (i = 0; i < 16; i++) {
        pDevCtxt->pSfpSerdesCtxt[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        pDevCtxt->pXfiSerdesCtxt[i] = NULL;
    }
    for (i = 0; i < 32; i++) {
        pDevCtxt->pBackplaneSerdesCtxt[i] = NULL;
    }
    for (i = 0; i < 8; i++) {
        pDevCtxt->pPifSerdesCtxt[i] = NULL;
    }
    
    for (i = 0; i < 16; i++) {
        pDevCtxt->pLsciCtxt[i] = NULL;
    }
    
    for (i = 0; i < 2; i++) {
        pDevCtxt->pHsciCtxt[i] = NULL;
    }
    
    pDevCtxt->pHostMsgCtxt = NULL;
    pDevCtxt->pOtnOhRamCtxt = NULL;
    pDevCtxt->pOdukppOhRamCtxt = NULL;

#ifndef PMC_HW_CODE
    pDevCtxt->pRtosTimerCtxt = NULL;
#endif /* PMC_HW_CODE */

#ifdef HYPHY20G_API_REENTRANT
    result = sysHyPhy20gAtomicCreate(&(pDevCtxt->sem));
    if (result) return result;
#endif

    pDevCtxt->valid       = HYPHY20G_VALID_SIGN;
    return HYPHY20G_SUCCESS;

} /* hyPhy20gCreateCtxt */

/*******************************************************************************
**
**  hyPhy20gDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function is used to destroy the the HyPHY 20G Context.
**                  This function iterate the pointer to every subsystem in
**                  the device context, and free memory which is dynamic
**                  allocated as well as set the pointer to NULL.
**
**  INPUTS:         fileHndl  - base address
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_DEV
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**
*******************************************************************************/
INT4 hyPhy20gDestroyCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt;
    sHYPHY20G_CBRI_CTXT *pCbriCtxt;
    UINT4 i;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* validate the device handle */
    if(pDevCtxt->valid != HYPHY20G_VALID_SIGN)
        return HYPHY20G_ERR_INVALID_DEV;

    /* destroy CPB context if present */
    if(pDevCtxt->pCpbCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling hyPhy20gCpbDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        hyPhy20gCpbDestroyCtxt(fileHndl);
    }

    /* destroy GFP A and B context if present */
    pGfpSliceCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)pDevCtxt->pGfpACtxt;
    if(pGfpSliceCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling gfpHyPhy20gGfpDestroySliceCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        gfpHyPhy20gGfpDestroySliceCtxt(fileHndl, 0);
    }

    pGfpSliceCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)pDevCtxt->pGfpBCtxt;
    if(pGfpSliceCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling gfpHyPhy20gGfpDestroySliceCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        gfpHyPhy20gGfpDestroySliceCtxt(fileHndl, 1);
    }

   /* destroy OPI CBRI Context if present */
    for(i=0; i < 2; i++){
        pCbriCtxt = (sHYPHY20G_CBRI_CTXT *)pDevCtxt->pCbriCtxt[i];
        if(pCbriCtxt != NULL) {
            DBG_PRINT("%s, %s, %d, calling hyPhy20gCbriDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
            hyPhy20gCbriDestroyCtxt(fileHndl, i);
        }
    }

    /* destroy TSE Context if present */
    if(pDevCtxt->pTseCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling tseHyPhy20gTseDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        tseHyPhy20gTseDestroyCtxt(fileHndl);
    }

    /* destroy PIF context if present */
    DBG_PRINT("%s, %s, %d, calling pifHyPhy20gPifDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
    pifHyPhy20gPifDestroyCtxt(fileHndl, 1);

    /* destroy LSCI context if present */
    for (i = 0; i < 16; i++) {
        DBG_PRINT("%s, %s, %d, calling hyPhy20gLsciDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        hyPhy20gLsciDestroyCtxt(fileHndl, i);
    }

    /* destroy HSCI context if present */
    for (i = 0; i < 2; i++) {
        DBG_PRINT("%s, %s, %d, calling hsciHyPhy20gHsciDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        hsciHyPhy20gHsciDestroyCtxt(fileHndl, i);
    }

    /* destroy VCAT A Context if present */
    if(pDevCtxt->pVcatACtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling vcatHyPhy20gVcatDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        vcatHyPhy20gVcatDestroyCtxt(fileHndl, 0);
    }

    /* destroy VCAT B Context if present */
    if(pDevCtxt->pVcatBCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling vcatHyPhy20gVcatDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        vcatHyPhy20gVcatDestroyCtxt(fileHndl, 1);
    }

    /* destroy SERDES context if present */
    /* SFP serdes */
    for (i = 0; i < 16; i++) {
        DBG_PRINT("%s, %s, %d, calling serdesHyPhy20gSerdesDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        serdesHyPhy20gSerdesDestroyCtxt(fileHndl, HYPHY20G_SERDES_SFP_INTF, i);
    }
    /* XFI serdes */
    for (i = 0; i < 2; i++) {
        DBG_PRINT("%s, %s, %d, calling serdesHyPhy20gSerdesDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        serdesHyPhy20gSerdesDestroyCtxt(fileHndl, HYPHY20G_SERDES_XFI_INTF, i);
    }
    /* Backplane serdes */
    for (i = 0; i < 32; i++) {
        DBG_PRINT("%s, %s, %d, calling serdesHyPhy20gSerdesDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        serdesHyPhy20gSerdesDestroyCtxt(fileHndl, HYPHY20G_SERDES_BACKPLANE_INTF,
                                        i);
    }
    /* PIF serdes */
    for (i = 0; i < 8; i++) {
        DBG_PRINT("%s, %s, %d, calling serdesHyPhy20gSerdesDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        serdesHyPhy20gSerdesDestroyCtxt(fileHndl, HYPHY20G_SERDES_PIF_INTF, i);
    }

    /* destroy Host Message Context if present */
    if(pDevCtxt->pHostMsgCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling hyPhy20gHostMsgDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        hyPhy20gHostMsgDestroyCtxt(fileHndl);
    }
    
    /* destroy OTN OH RAM Context if present */
    if (pDevCtxt->pOtnOhRamCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling hyPhy20gOtnOhRamDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        hyPhy20gOtnOhRamDestroyCtxt(fileHndl);
    }  
    
    /* destroy ODUKPP OH RAM Context if present */
    if (pDevCtxt->pOdukppOhRamCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling hyPhy20gOdukppOhRamDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        hyPhy20gOdukppOhRamDestroyCtxt(fileHndl);
    } 

#ifndef PMC_HW_CODE
    /* destroy timer context if present */
    if(pDevCtxt->pRtosTimerCtxt != NULL) {
        DBG_PRINT("%s, %s, %d, calling sysHyPhy20gTimerDestroyCtxt\n",__FILE__, __FUNCTION__, __LINE__);
        sysHyPhy20gTimerDestroyCtxt(fileHndl);
    }
#endif /* PMC_HW_CODE */

#ifdef HYPHY20G_API_REENTRANT
    sysHyPhy20gAtomicDelete(pDevCtxt->sem);
#endif

    /* invalid device context */
    pDevCtxt->valid = HYPHY20G_INVALID_VAL_32BIT;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gDestroyCtxt */

/*******************************************************************************
**
**  hyPhy20gRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function reads a register of the HyPHY 20G device.
**
**
**  INPUTS:         *fileHndl - base address
**                  regNum    - register number
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gRead(struct file *fileHndl, UINT4 regNum, UINT4 *pvalue)
{
    INT4 result;
    
    result = sysHyPhy20gRead(fileHndl, regNum, pvalue);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gRead */


/*******************************************************************************
**
**  hyPhy20gWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function writes a register of the HyPHY 20G device.
**
**
**  INPUTS:         *fileHndl - base address
**                  regNum    - register number
**                  value     - value to be written
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gWrite(struct file *fileHndl, UINT4 regNum, UINT4 value)
{
    INT4 result;

    result = sysHyPhy20gWrite(fileHndl, regNum, value);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gWrite */


/*******************************************************************************
**
**  hyPhy20gPcieCfgRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function reads a PCIe configuration space register
**                  in the HyPHY 20G device.
**
**  INPUTS:         *fileHndl - base address
**                  regNum    - register number
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPcieCfgRead(struct file *fileHndl, UINT4 regNum, UINT4 *pvalue)
{
    INT4 result;

    result = sysHyPhy20gPcieCfgRead(fileHndl, regNum, pvalue);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPcieCfgRead */


/*******************************************************************************
**
**  hyPhy20gPcieCfgWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function writes a PCIe configuration space register
**                  in the HyPHY 20G device.
**
**  INPUTS:         *fileHndl - base address
**                  regNum    - register number
**                  value     - value to be written
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPcieCfgWrite(struct file *fileHndl, UINT4 regNum, UINT4 value)
{
    INT4 result;

    result = sysHyPhy20gPcieCfgWrite(fileHndl, regNum, value);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPcieCfgWrite */


/*******************************************************************************
**
**  hyPhy20gPcieBurstRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a PCIe burst read of device registers
**                  in the HyPHY 20G.
**
**                  This function doesn't support to read multiple words over 
**                  4KB boundaries.
**
**  INPUTS:         *fileHndl - base address
**                  startAddr - device address where burst read will start
**                  length    - burst length in 32-bit words (1-32)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPcieBurstRead(struct file *fileHndl, UINT4 startAddr,
                           UINT4 length, UINT4 *dataList)
{
    INT4 result;

    result = sysHyPhy20gPcieBurstRead(fileHndl, startAddr, length, dataList);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPcieBurstRead */


/*******************************************************************************
**
**  hyPhy20gPcieBurstWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a PCIe burst write to device
**                  registers in the HyPHY 20G device.
**
**                  This function doesn't support to write multiple words over 
**                  4KB boundaries.
**
**  INPUTS:         *fileHndl - base address
**                  startAddr - device address where burst read will start
**                  length    - burst length in 32-bit words (1-32)
**                  dataList  - list of data to write
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPcieBurstWrite(struct file *fileHndl, UINT4 startAddr,
                            UINT4 length, UINT4 *dataList)
{
    INT4 result;

    result = sysHyPhy20gPcieBurstWrite(fileHndl, startAddr, length, dataList);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPcieBurstWrite */


/*******************************************************************************
**
**  hyPhy20gFwDownload
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads a binary file, writes its contents to
**                 SPRAM and auto-launch MIPS.
**
**  INPUTS:         *fileHndl  - base address
**                  *fread     - path to binary file
**                  jumpAddr   - vector address, for MIPS to start execution at
**                               (Please refer to FW release notes, PMC-2091033,
**                                associated with this release for jumpAddr
**                                value)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_FW_INVALID_BOOT_STATE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gFwDownload(struct file *fileHndl, STRING *fread, UINT4 jumpAddr)
{
    INT4 result;

    result = sysHyPhy20gFwDownload(fileHndl, fread, jumpAddr);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gFwDownload */


/*******************************************************************************
**
**  hyPhy20gFwReload
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reloads the firmware without the need to
**                 reset the device first by performing a soft reset on the
**                 MIPS core.
**
**                 Prior to using this function to reload new FW, ensure
**                 previous version of FW is operational using
**                 hyPhy20gFwDownload.
**
**  INPUTS:         *fileHndl  - base address
**                  *fread     - path to binary file
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gFwReload(struct file *fileHndl, STRING *fread)
{
    INT4 result;

    result = sysHyPhy20gFwReload(fileHndl, fread);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gFwReload */

/*******************************************************************************
**
**  hyPhy20gCheckRevEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables revision checking in
**                  hyPhy20gCreateCtxt.  By default, revision checking is
**                  enabled and this function need not be called.  If revision
**                  checking needs to be enabled, call this function before
**                  hyPhy20gCreateCtxt.
**
**                  This function uses global variable to maintain revision
**                  checking state.  Should driver be modified to support 
**                  multiple devices this global variable would apply this
**                  setting to all devices unless modifications to the 
**                  implementation are made.
**
**  INPUTS:         *fileHndl  - base address
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCheckRevEnable(struct file *fileHndl)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pDevCtxt->gData.hyphy20gCheckRev = 1;
    pDevCtxt->gData.forceRevId = 0;
    return HYPHY20G_SUCCESS;
} /* hyPhy20gCheckRevEnable */


/*******************************************************************************
**
**  hyPhy20gCheckRevDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function disables revision checking in
**                  hyPhy20gCreateCtxt.  By default, revision checking is
**                  enabled.  Call this function before hyPhy20gCreateCtxt
**                  to disable revision checking.
**
**                  This function uses global variable to maintain revision
**                  checking state.  Should driver be modified to support 
**                  multiple devices this global variable would apply this
**                  setting to all devices unless modifications to the 
**                  implementation are made.
**
**  INPUTS:         *fileHndl  - base address
**                  revId      - revision ID to be forced
**                               0 - Rev A
**                               1 - Rev B
**                               2 - Rev C
**                               3 - Rev D
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCheckRevDisable(struct file *fileHndl, UINT4 revId)
{
    sHYPHY20G_CTXT  *pDevCtxt;
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if(revId > 2){
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    pDevCtxt->gData.hyphy20gCheckRev = 0;
    pDevCtxt->gData.forceRevId = revId;
    return HYPHY20G_SUCCESS;
} /* hyPhy20gCheckRevDisable */

/*******************************************************************************
**
**  hyPhy20gGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Accurately calculate the size of driver context.
**
**  INPUTS:         fileHndl  - base address
**
**  OUTPUTS:        None
**
**  RETURNS:        The size of driver context
**
*******************************************************************************/
UINT4 hyPhy20gGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 offset = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), offset,
                    "%s", "pDevCtxt is NULL.");

    /* no valid driver context */
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        DBG_PRINT("%s, %s, %d, no valid driver context\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return offset;
    }

    /* place the version info at the first field inside the context buffer, 
    so that any driver version could easily find it */
    offset += sizeof(pDevCtxt->version);    

    DBG_PRINT("%s, %s, %d, top level offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the top level context size */
    /* include the context buffer header size */
    offset += utilHyPhy20gGetCtxtHdrSize();
    offset += sizeof(sHYPHY20G_CTXT);

  
    DBG_PRINT("%s, %s, %d, cpb offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of CPB subsystem */
    offset += cpbHyPhy20gCpbGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, lsci offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);
    
    /* calculate the context size of LSCI subsystem */
    offset += lsciHyPhy20gLsciGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, hsci offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of HSCI subsystem */
    offset += hsciHyPhy20gHsciGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, pif offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of PIF subsystem */
    offset += pifHyPhy20gPifGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, gfp offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of GFP subsystem */
    offset += gfpHyPhy20gGfpGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, cbrb offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of CBRI subsystem */
    offset += cbriHyPhy20gCbriGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, tse offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of TSE subsystem */
    offset += tseHyPhy20gTseGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, vcat offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of VCAT subsystem */
    offset += vcatHyPhy20gVcatGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, serdes offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of SERDES subsystem */
    offset += serdesHyPhy20gSerdesGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, hostmsg offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of HostMsg subsystem */
    offset += hostMsgHyPhy20gHostMsgGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, OtnOhRam offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of OtnOhRam subsystem */
    offset += otnOhRamhyPhy20gOtnOhRamGetCtxtSize(fileHndl);

    DBG_PRINT("%s, %s, %d, OdukppOhRam offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of OdukppOhRam subsystem */
    offset += odukppOhRamhyPhy20gOdukppOhRamGetCtxtSize(fileHndl);

#ifndef PMC_HW_CODE
    DBG_PRINT("%s, %s, %d, timer offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* calculate the context size of RTOS Timer subsystem */
    offset += sysHyPhy20gTimerGetCtxtSize(fileHndl);
#endif

    /* include end header flag in the buffer */
    offset += sizeof(UINT4);

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    return offset;
} /* hyPhy20gGetCtxtSize */

/*******************************************************************************
**
**  hyPhy20gExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the driver context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for driver context 
**                  extraction
**                  ctxtBufSize - pre-allocated buffer size
**
**  OUTPUTS:        pExtractedSize -the context size(in byte) has been extracted
**                  into the buffer
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gExtractCtxt(struct file *fileHndl, void *pCtxtBuf, 
                        UINT4 ctxtBufSize, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 rtLen;
    UINT4 *p4;
    UINT4 offset = 0, len = 0;

    if (pCtxtBuf == NULL || 
        pExtractedSize == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* no valid driver context for extraction */
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        DBG_PRINT("%s, %s, %d, no valid driver context for extraction\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        *pExtractedSize = 0;
        return HYPHY20G_SUCCESS;
    }

    /* the pre-allocated buffer does not have 
    sufficient memory for context extraction */
    if (ctxtBufSize < hyPhy20gGetCtxtSize(fileHndl)) {
        DBG_PRINT("%s, %s, %d, input buffer too small, at least 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, hyPhy20gGetCtxtSize(fileHndl));
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* be-zero the context buffer for extraction */
    sysHyPhy20gMemSet(pCtxtBuf, 0, ctxtBufSize);

    /* place the version info at the first field inside the context buffer, 
    so that any driver version could easily find it */
    p4 = (UINT4 *)((UINT1 *)pCtxtBuf + offset);
    *p4 = pDevCtxt->version;
    offset += sizeof(*p4);

    DBG_PRINT("%s, %s, %d, top level offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract the top level structure data */
    /* insert ctxt buffer header */
    len = sizeof(sHYPHY20G_CTXT_HDR) +
            sizeof(sHYPHY20G_CTXT);
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    /* copy the payload into the buffer */
    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                    (void *)pDevCtxt, sizeof(sHYPHY20G_CTXT));
    offset += sizeof(sHYPHY20G_CTXT);

    DBG_PRINT("%s, %s, %d, cpb offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract CPB context */
    result = cpbHyPhy20gCpbExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "cpbHyPhy20gCpbExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, lsci offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);
    
    /* extract LSCI context */
    result = lsciHyPhy20gLsciExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "lsciHyPhy20gLsciExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, hsci offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract HSCI context */
    result = hsciHyPhy20gHsciExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "hsciHyPhy20gHsciExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, pif offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract PIF context */
    result = pifHyPhy20gPifExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "pifHyPhy20gPifExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, gfp offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract GFP context */
    result = gfpHyPhy20gGfpExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "gfpHyPhy20gGfpExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, cbrb offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract CBRI context */
    result = cbriHyPhy20gCbriExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "cbriHyPhy20gCbriExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, tse offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract TSE context */
    result = tseHyPhy20gTseExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "tseHyPhy20gTseExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, vcat offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract VCAT context */
    result = vcatHyPhy20gVcatExtractCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "vcatHyPhy20gVcatExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, serdes offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract SERDES context */
    result = serdesHyPhy20gSerdesExtractCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "serdesHyPhy20gSerdesExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, hostmsg offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract HostMsg context */
    result = hostMsgHyPhy20gHostMsgExtractCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "hostMsgHyPhy20gHostMsgExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, OtnOhRam offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract OtnOhRam context */
    result = otnOhRamhyPhy20gOtnOhRamExtractCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "otnOhRamhyPhy20gOtnOhRamExtractCtxt failed.");
    offset += len;

    DBG_PRINT("%s, %s, %d, OdukppOhRam offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract OdukppOhRam context */
    result = odukppOhRamhyPhy20gOdukppOhRamExtractCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "odukppOhRamhyPhy20gOdukppOhRamExtractCtxt failed.");
    offset += len;

#ifndef PMC_HW_CODE    
    DBG_PRINT("%s, %s, %d, timer offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract RTOS Timer context */
    result = sysHyPhy20gTimerExtractCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "sysHyPhy20gTimerExtractCtxt failed.");
    offset += len;
#endif

    /* insert end header flag in the buffer */
    p4 = (UINT4 *)((UINT1 *)pCtxtBuf + offset);
    *p4 = 0;
    offset += sizeof(*p4);

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;
    return HYPHY20G_SUCCESS;
} /* hyPhy20gExtractCtxt */

/*******************************************************************************
**
**  hyPhy20gRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the driver context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored context data for restoration
**
**  OUTPUTS:        pParsedSize -the data size(in byte) has been parsed 
**                  in the buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_CTXT_ALREADY_VALID
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gRestoreCtxt(struct file *fileHndl, 
                        void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 *p4;
    UINT2 *p2;
    UINT4 preStoredVersion;
    UINT4 preStoredDeviceId, preStoredRevId, realDeviceId, realRevId, status;
    UINT4 offset = 0, len = 0;

    if (pCtxtBuf == NULL || 
        pParsedSize == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }
    
    /*--------------------------------------------------------------------*/
    /* [1st Step]: verify the driver context valid or not                 */
    /*--------------------------------------------------------------------*/

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* if pDevCtxt is valid, not to do context restoration */
    if(pDevCtxt->valid == HYPHY20G_VALID_SIGN) {
        DBG_PRINT("%s, %s, %d, driver context is already valid\n", 
                    __FILE__, __FUNCTION__, __LINE__);
    	return HYPHY20G_ERR_CTXT_ALREADY_VALID;
    }

    /*--------------------------------------------------------------------*/
    /* [2nd Step]: verify the version code of pre-stored data             */
    /*--------------------------------------------------------------------*/
    /* retrieve the version code of the pre-stored context */
    p4 = (UINT4 *)pCtxtBuf;
    preStoredVersion = *p4;
    
    /* 07.04.00 is the first version to support warm restart */
    if (preStoredVersion < DRIVER_VERSION(7, 4, 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED;
    }

    /* check pre-stored context version vs current driver version */
    /* not support forward-compatible */
    if (preStoredVersion > HYPHY20G_DRIVER_VERSION_CODE){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED;
    }

    /* for backwards-compatible */
    if (preStoredVersion < HYPHY20G_DRIVER_VERSION_CODE){
        /* for the current running driver, if there are some new structure elements 
        that are not recoverable from the pre-stored context data of previous drivers,
        we should return HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED */
        
        /* [TBD]: if the context structure is modified in a future version,
        the modifier should also update the code here to verify backwards-compatibility
        of context restoration */

        /*return HYPHY20G_ERR_DEV_CTXT_VERSION_UNRECOGNIZED;*/
    }

    /*--------------------------------------------------------------------*/
    /* [3rd Step ]: verify the top level ctxt header in pre-stored data   */
    /*--------------------------------------------------------------------*/
    /* try to retrieve the HYPHY_HYPHY_CTXT_ID header */
    pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + sizeof(pDevCtxt->version));
    if (pCtxtBufHdr->id != HYPHY_CTXT_ID) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    /* check if the data is corrupted */
    if (pCtxtBufHdr->len <= sizeof(sHYPHY20G_CTXT_HDR)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    /*--------------------------------------------------------------------*/
    /* [4th Step]: verify the deviceId and revisionId of pre-stored data  */
    /*--------------------------------------------------------------------*/
    /* retrieve the pre-stored device id and revision id */
    p4 = (UINT4 *)((UINT1 *)pCtxtBuf + sizeof(sHYPHY20G_CTXT_HDR));
    p4++;
    p4++;
    p2 = (UINT2 *)p4;
    preStoredDeviceId = (UINT4)(*p2);
    p2++;
    preStoredRevId = (UINT4)(*p2);

    DBG_PRINT("%s, %s, %d, preStoredDeviceId 0x%lx preStoredRevId 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, preStoredDeviceId, preStoredRevId);

    /* retrieve these global data before call hyPhy20gCreateCtxt */
    p4++;
    pDevCtxt->gData.hyphy20gCheckRev = *p4;
    p4++;
    pDevCtxt->gData.forceRevId = *p4;
    /* create driver context */
    result = hyPhy20gCreateCtxt(fileHndl);
    if (result) {
        DBG_PRINT("%s, %s, %d, hyPhy20gCreateCtxt failed\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return result;
    }

    /* retrieve the real device id and revision id */
    result = hyPhy20gCheckRealRev(fileHndl, &realDeviceId, &realRevId, &status);
    if(result != HYPHY20G_SUCCESS ||
        status == 0) {
        DBG_PRINT("%s, %s, %d, hyPhy20gCheckRealRev failed! DevId 0x%lx RevId 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, realDeviceId, realRevId);
        goto RESTORE_CTXT_FAILED;
    }

    /* check the real decice id and revision id vs pre-stored ids */
    if (realDeviceId != preStoredDeviceId || 
        realRevId != preStoredRevId) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INCOMPATIBLE_DEVICE preDevId 0x%lx preRevId 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, preStoredDeviceId, preStoredRevId);
        result = HYPHY20G_ERR_INCOMPATIBLE_DEVICE;
        goto RESTORE_CTXT_FAILED;
    }

    /*----------------------------------------------------------------------------*/
    /* [5th Step]: try to re-apply the pre-stored data into the new driver memory */
    /*----------------------------------------------------------------------------*/

    DBG_PRINT("%s, %s, %d, top level offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore the first level context data */
    /* not copy the unuseful pointer value from the pre-stored data */
    len = offsetof(sHYPHY20G_CTXT, pCpbCtxt);
    sysHyPhy20gMemCpy((void *)pDevCtxt, 
                    ((UINT1 *)pCtxtBuf + sizeof(sHYPHY20G_CTXT_HDR)), len);

    /* finish restore the top level structure in the data */
    offset += pCtxtBufHdr->len;
    pDevCtxt->version = HYPHY20G_DRIVER_VERSION_CODE;

    DBG_PRINT("%s, %s, %d, cpb offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset); 

    /* restore CPB context */
    result = cpbHyPhy20gCpbRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, lsci offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore LSCI context */
    result = lsciHyPhy20gLsciRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, hsci offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore HSCI context */
    result = hsciHyPhy20gHsciRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, pif offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore PIF context */
    result = pifHyPhy20gPifRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, gfp offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore GFP context */
    result = gfpHyPhy20gGfpRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, cbrb offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore CBRI context */
    result = cbriHyPhy20gCbriRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, tse offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore TSE context */ /* pure payload */
    result = tseHyPhy20gTseRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, vcat offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore VCAT context */ /* pure payload */
    result = vcatHyPhy20gVcatRestoreCtxt(fileHndl, 
                        ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, serdes offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore SERDES context */
    result = serdesHyPhy20gSerdesRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, hostmsg offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore HostMsg context */
    result = hostMsgHyPhy20gHostMsgRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, OtnOhRam offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore OtnOhRam context */
    result = otnOhRamhyPhy20gOtnOhRamRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

    DBG_PRINT("%s, %s, %d, OdukppOhRam offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* restore OdukppOhRam context */
    result = odukppOhRamhyPhy20gOdukppOhRamRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;

#ifndef PMC_HW_CODE
    DBG_PRINT("%s, %s, %d, timer offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);
    /* restore RTOS Timer context */
    result = sysHyPhy20gTimerRestoreCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    if (result) {
        goto RESTORE_CTXT_FAILED;
    }
    offset += len;
#endif

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pParsedSize = offset;
    return HYPHY20G_SUCCESS;

RESTORE_CTXT_FAILED:
    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);
    hyPhy20gDestroyCtxt(fileHndl);
    *pParsedSize = 0;
    return result;    
} /* hyPhy20gRestoreCtxt */

/*******************************************************************************
**
**  hyPhy20gAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This Public function audit SW Driver Device context.
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
INT4 hyPhy20gAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    INT4 result;
    UINT4 value, msk, off;
    UINT2 realDeviceId, realRevId, mimicDeviceID, mimicRevId;  
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN)
    {
        DBG_PRINT("%s, %s, %d, hyPhy20gCreateCtxt is not done yet.\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* Audit the pDevCtxt global data members according to hyPhy20gCreateCtxt
    */
    if ((pDevCtxt->version != HYPHY20G_DRIVER_VERSION_CODE)||
        (pDevCtxt->stateDevice != HYPHY20G_PRESENT)
        )
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* audit the gData according to hyPhy20gCheckRevDisable/hyPhy20gCheckRevEnable/hyPhy20gSetTraceWrite/
    hyPhy20gCbrbPktSizeTypeCfg/hyPhy20gCpbSetQdrStressMode/hyPhy20gCpbSetQdrStressFourIntfDist
    hyPhy20gCpbSetIntQdrBlkSize/hyPhy20gHostMsgFwOp/hyPhy20gCpbSetIntQdrBlkSize functions */
    if ((pDevCtxt->gData.forceRevId > 2)||(pDevCtxt->gData.hyphy20gCheckRev >1)||
        (pDevCtxt->gData.hyphy20gQdrStressMode >1)||(pDevCtxt->gData.hyphy20gQdrStressDist>4)||
        (pDevCtxt->gData.intQdrBlkSize>19)||(pDevCtxt->gData.hostMsgFwOp>1)||
        (pDevCtxt->gData.hyphy20gTraceWrite>1)||(pDevCtxt->gData.hyphy20gCbrbPacketSizeMode>2)
        )
    {
        DBG_PRINT("%s, %s, %d, pDevCtxt->gData members value wrong.\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    result = sysHyPhy20gRead(fileHndl, HYPHY20G_TOP_REG_DEV_ID, &value);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gRead fail."); 

    /* get part number of device */
    msk = HYPHY20G_TOP_BIT_DEV_ID_DEVID_MSK;
    off = HYPHY20G_TOP_BIT_DEV_ID_DEVID_OFF;
    realDeviceId  = (UINT2)((value & msk) >> off);
    mimicDeviceID = (UINT2)((value & msk) >> off);

    /* get revision number of device */
    msk = HYPHY20G_TOP_BIT_DEV_ID_REV_CODE_MSK;
    off = HYPHY20G_TOP_BIT_DEV_ID_REV_CODE_OFF;

    realRevId = (UINT2)((value & msk) >> off);
	
    if (pDevCtxt->gData.forceRevId) {
        /* force revision ID to revB (0x1) or revC (0x2) */
        mimicRevId = (UINT2)pDevCtxt->gData.forceRevId;
    } else {
        /* use hardware revision ID */
        mimicRevId = (UINT2)((value & msk) >> off);
    }

    if ((pDevCtxt->realDeviceId!=realDeviceId)||(pDevCtxt->realRevId!=realRevId)||
        (pDevCtxt->mimicDeviceID!=mimicDeviceID)||(pDevCtxt->mimicRevId!=mimicRevId))
    {
        DBG_PRINT("%s, %s, %d, pDevCtxt->realDeviceId|realRevId|mimicDeviceID|mimicRevId mismatched with the current HYPHY device.\n",
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }
        
    /* evaluation of compatibility between SW driver and device */
    if(pDevCtxt->gData.hyphy20gCheckRev){
            if(!(((realDeviceId == HYPHY20G_HYPHY20G_DEVICE)   ||
                (realDeviceId == HYPHY20G_HYPHY10G_DEVICE) ||
                (realDeviceId == HYPHY20G_META20G_DEVICE))  &&
                (mimicRevId == HYPHY20G_REV_ID_B || mimicRevId == HYPHY20G_REV_ID_C || mimicRevId == HYPHY20G_REV_ID_D)))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INCOMPATIBLE_DEVICE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
    }

    /* audit *pCpbCtxt */
    result = cpbHyPhy20gCpbAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, cpbHyPhy20gCpbAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);
    
    /* audit *pLsciCtxt */
    result = lsciHyPhy20gLsciAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, lsciHyPhy20gLsciAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pHsciCtxt */
    result = hsciHyPhy20gHsciAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, hsciHyPhy20gHsciAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pPifCtxt */
    result = pifHyPhy20gPifAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, pifHyPhy20gPifAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);
    
    /* audit *pGfpACtxt and *pGfpBCtxt */
    result = gfpHyPhy20gGfpAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return HYPHY20G_FAILURE;
    }
    DBG_PRINT("%s, %s, %d, gfpHyPhy20gGfpAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pCbriCtxt */
    result = cbriHyPhy20gCbriAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, cbriHyPhy20gCbriAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pTseCtxt */
    result = tseHyPhy20gTseAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return HYPHY20G_FAILURE;
    }
    DBG_PRINT("%s, %s, %d, tseHyPhy20gTseAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pVcatACtxt and *pVcatBCtxt */
    result = vcatHyPhy20gVcatAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, vcatHyPhy20gVcatAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pSfpSerdesCtxt */
    result = sfpSerdesHyPhy20gSfpSerdesAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return HYPHY20G_FAILURE;
    }
    DBG_PRINT("%s, %s, %d, sfpSerdesHyPhy20gSfpSerdesAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pXfiSerdesCtxt */
    result = xfiSerdesHyPhy20gXfiSerdesAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, xfiSerdesHyPhy20gXfiSerdesAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pBackplaneSerdesCtxt */
    result = bkpSerdesHyPhy20gBkpSerdesAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return HYPHY20G_FAILURE;
    }
    DBG_PRINT("%s, %s, %d, bkpSerdesHyPhy20gBkpSerdesAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pPifSerdesCtxt */
    result = pifSerdesHyPhy20gPifSerdesAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, pifSerdesHyPhy20gPifSerdesAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pHostMsgCtxt */
    result = hostMsgHyPhy20gHostMsgAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, hostMsgHyPhy20gHostMsgAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pOtnOhRamCtxt */
    result = otnOhRamHyPhy20gOtnOhRamAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, otnOhRamHyPhy20gOtnOhRamAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pOdukppOhRamCtxt */
    result = odukppOhRamHyPhy20gOdukppOhRamAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, odukppOhRamHyPhy20gOdukppOhRamAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

#ifndef PMC_HW_CODE
    /* audit *pRtosTimerCtxt */
    result = rtosTimerHyPhy20gRtosTimerAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, rtosTimerHyPhy20gRtosTimerAuditCtxt SUCCESS\n",__FILE__, __FUNCTION__, __LINE__);

#endif

    return HYPHY20G_SUCCESS;
} /* cpbHyPhy20gCpbAuditCtxt */

/*******************************************************************************
**
**  hyPhy20gConfigDumpSizeGet
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Get the dumpBuff size needed to store the register config
**                  according to the CfgDumpReqList. The CfgDumpReqList will  
**                  contain several CfgDumpReq, req1, req2,req3....Each one  
**                  will get back one register block dump. The last reqn 
**                  set 0xFFFFFFFF in length field, it is just used as end 
**                  flag, will not get back any dump data.
**
**                  The dumpBuff will contain those CfgDumpReq and according
**                  register block dump, req1,req1RegBlockDump, 
**                  req2, req2RegBlockDump..., reqn.
**
**  INPUTS:         fileHndl  - device handle
**                  pCfgDumpReqList  - pointer to the configuration dump 
**                  request list.
**
**  OUTPUTS:        None
**
**  RETURNS:        dumpBuffSize
**                  =0, means the dump request is wrong.
**                  >0, return the dumpBuff size.
**
*******************************************************************************/
UINT4 hyPhy20gConfigDumpSizeGet(struct file *fileHndl, 
                                sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReqList)
{
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq;
    UINT4 size = 0;

    /* check the inputs */
    if (pCfgDumpReqList == NULL ) {
        DBG_PRINT("%s, %s, %d, input Null arg\n",__FILE__, __FUNCTION__, __LINE__);
        return 0;
    }

    /* include DeviceId register value at first field of the buffer */
    size += sizeof(UINT4);

    /* assume that the request list has the correct format */
    pCfgDumpReq = pCfgDumpReqList;
    while (pCfgDumpReq->length != REG_DUMP_END_MARK) {
        /* include the request header size */
        size += sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);

        /* the input request length should be correct for 
        both direct/indirect register access */
        size += pCfgDumpReq->length * sizeof(UINT4);

        /* move to next request header */
        pCfgDumpReq++;
    } /* end of while */

    /* include the end flag size */
    size += sizeof(UINT4);

    DBG_PRINT("%s, %s, %d, size 0x%lx\n",
        __FILE__, __FUNCTION__, __LINE__, size); 
    
    return size;    
} /* hyPhy20gConfigDumpSizeGet */

/*******************************************************************************
**
**  hyPhy20gConfigDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Get the dumpBuff content 
**                  according to the CfgDumpReqList. The CfgDumpReqList will  
**                  contain several CfgDumpReq, req1, req2,req3....Each one  
**                  will get back one register block dump. The last reqn 
**                  set 0xFFFFFFFF in length field, it is just used as end 
**                  flag, will not get back any dump data.
**
**                  The dumpBuff should first contain devId and revId, then 
**                  those CfgDumpReq and according register block dump, req1,
**                  req1RegBlockDump, req2, req2RegBlockDump..., reqn....
**                  the register config dump should jump to next register  
**                  block when some registers read error happened in the  
**                  middle of one continuous register block, go on dumping.
**                  
**                  The return value can only log the first dump error.
**                  
**                  *pValidDumpBuffSize is used to count the actual valid 
**                  BYTE.
**                  So if *pValidDumpBuffSize>0, there are some valid registers
**                  data in *pCfgDumpBuf to be stored.
**
**  INPUTS:         fileHndl  - device handle
**                  pCfgDumpReqList  - pointer to the configuration dump 
**                  request list.
**                  pCfgDumpBuf - pointer to the configuration dump buff.
**
**  OUTPUTS:        pValidDumpBuffSize - return the valid Dump Buff size.
**                  pCfgDumpBuf - return the configuration dump buff with a list of  
**                  dumpReq head and register dump data.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gConfigDump(struct file *fileHndl, 
                        sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReqList, 
                        void *pCfgDumpBuf, UINT4 *pValidDumpBuffSize)
{
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pTmpCfgDumpReq;
    sHYPHY_CFG_DUMP_ENTRY_TYPE *pCfgDumpEntry;
    INT4 result = HYPHY20G_SUCCESS, rtVal = HYPHY20G_SUCCESS;
    UINT4 offset = 0, dumpSize = 0;
    UINT4 endFlag = REG_DUMP_END_MARK;
    UINT4 value = 0;

    /* check the inputs */
    if (pCfgDumpReqList == NULL ||
        pCfgDumpBuf == NULL || 
        pValidDumpBuffSize == NULL) {
        DBG_PRINT("%s, %s, %d, input NULL arg\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* store DeviceId register value at first field of the buffer */
    result = sysHyPhy20gRead (fileHndl, HYPHY20G_TOP_REG_DEV_ID, &value);
    if (result) {
        DBG_PRINT("%s, %s, %d, can not dump DeviceId register\n", 
            __FILE__, __FUNCTION__, __LINE__);
        value = 0;
    }
    sysHyPhy20gMemCpy(((UINT1 *)pCfgDumpBuf + offset), 
                    &value, sizeof(value));
    offset += sizeof(value);

    /* assume that the request list has the correct format */
    pCfgDumpReq = pCfgDumpReqList;
    while (pCfgDumpReq != NULL &&
        pCfgDumpReq->length != REG_DUMP_END_MARK) {
        /* copy the pCfgDumpReq header into the buffer */
        sysHyPhy20gMemCpy(((UINT1 *)pCfgDumpBuf + offset), 
                        (void *)pCfgDumpReq, sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
        
        /* backup pCfgDumpReq->rtCode to be updated according Dump() return value */
        pTmpCfgDumpReq = (sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)((UINT1 *)pCfgDumpBuf + offset);

        /* search startAddr in hyphyCfgDumpEntry */
        pCfgDumpEntry = utilHyPhy20gConfigDumpSearchEntry(pCfgDumpReq->startAddr);
        /* no such entry, perform direct register access */
        if (pCfgDumpEntry == NULL) {
            DBG_PRINT("%s, %s, %d, can not find the dump entry for addr 0x%lx\n",
                __FILE__, __FUNCTION__, __LINE__, pCfgDumpReq->startAddr);

            /* callback dump function */
            result = untilHyPhy20gDump(fileHndl, ((UINT1 *)pCfgDumpBuf + offset), 
                                        &dumpSize);
        } else {
            /* perform indirect register access */
            if (pCfgDumpEntry->cfg_dump_func != NULL) {
                DBG_PRINT("%s, %s, %d, got the indirect entry for addr 0x%lx\n",
                    __FILE__, __FUNCTION__, __LINE__, pCfgDumpReq->startAddr);

                /* callback dump function */
                result = pCfgDumpEntry->cfg_dump_func(fileHndl, 
                                        ((UINT1 *)pCfgDumpBuf + offset), &dumpSize);
            } else {
                DBG_PRINT("%s, %s, %d, got the indirect entry for addr 0x%lx, but callback NULL\n",
                    __FILE__, __FUNCTION__, __LINE__, pCfgDumpReq->startAddr);
                result = HYPHY20G_FAILURE;
                pTmpCfgDumpReq->length = 0;
                pTmpCfgDumpReq->rtCode = result;
                dumpSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
            }
        }

        /* only log the first dump error for return */
        if (result != HYPHY20G_SUCCESS && 
            rtVal == HYPHY20G_SUCCESS) {
            rtVal = result;
        }
        
        /* offset dump buffer */
        offset += dumpSize;

        DBG_PRINT("%s, %s, %d, pCfgDumpReq 0x%p 0x%p len 0x%lx addr 0x%lx offset 0x%lx\n",
            __FILE__, __FUNCTION__, __LINE__, 
            pCfgDumpReq, &pCfgDumpReq, 
            pCfgDumpReq->length, pCfgDumpReq->startAddr, offset);

        /* move to the next request header */
        pCfgDumpReq++;
    } /* end of while */

    /* add end flag in the dump buffer */
    sysHyPhy20gMemCpy(((UINT1 *)pCfgDumpBuf + offset), 
                    &endFlag, sizeof(endFlag));
    offset += sizeof(endFlag);
    
    DBG_PRINT("%s, %s, %d, offset 0x%lx\n",
        __FILE__, __FUNCTION__, __LINE__, offset);

    *pValidDumpBuffSize = offset;
    return rtVal;
} /* hyPhy20gConfigDump */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  utilHyPhy20gConfigDumpSearchEntry
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Search the config dump entry via startAddr of 
**                  the register block.
**
**
**  INPUTS:         addr - the startAddr of the register block
**
**  OUTPUTS:        None
**
**  RETURNS:        The special config dump entry, NULL on failure
**
*******************************************************************************/
sHYPHY_CFG_DUMP_ENTRY_TYPE *utilHyPhy20gConfigDumpSearchEntry(UINT4 addr)
{
    UINT4 i;
    sHYPHY_CFG_DUMP_ENTRY_TYPE *pCfgDumpEntry;
    UINT4 cfgDumpEntryLen;

    cfgDumpEntryLen = sizeof(hyphyCfgDumpEntry)/sizeof(sHYPHY_CFG_DUMP_ENTRY_TYPE);

    for (i = 0; i < cfgDumpEntryLen; i++) {
        pCfgDumpEntry = (sHYPHY_CFG_DUMP_ENTRY_TYPE *)&(hyphyCfgDumpEntry[i]);
        
        /* search the config dump entry via addr */
        if (pCfgDumpEntry->startAddr == addr) {
            return pCfgDumpEntry;
        }
    }
    return NULL;
}/* utilHyPhy20gConfigDumpSearchEntry */

/*******************************************************************************
**
**  untilHyPhy20gDump
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function performs a direct register dump. Before calling
**                 the function, the cfgDumpReq content should be copied to the   
**                 pCfgDumpBuf from the pCfgDumpReqList, point the pDumpBuff to  
**                 this cfgDumpReq in the pCfgDumpBuf. The function will store 
**                 the registers value after the cfgDumpReq head and count how 
**                 many bytes are stored in the dump Buff.
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
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 untilHyPhy20gDump(struct file *fileHndl, void *pDumpBuff, 
                    UINT4 *pValidDumpBuffSize)
{
    INT4 result;
    UINT4 i, dumpBuffSize=0;
    sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReq=(sHYPHY20G_CFG_DUMP_REQUEST_TYPE *)pDumpBuff;
    UINT4 *pDumpVal;

    /* check the input param */
    if ((pDumpBuff==NULL)||(pValidDumpBuffSize==NULL)) return HYPHY20G_ERR_NULL_PTR_PASSED;
    
    /* init pDumpVal to buff data after the cfgDumpReq Header */
    pDumpVal = (UINT4 *)((UINT1 *)pDumpBuff + sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE));
    /* init the dumpBuffSize to cfgDumpReq header size */
    dumpBuffSize = sizeof(sHYPHY20G_CFG_DUMP_REQUEST_TYPE);
    
    /* check the direct registers dumpReq address space is within the valid address range of HYPHY*/
    if ((pCfgDumpReq->startAddr < HYPHY20G_REG_MIN_ADDR)||(pCfgDumpReq->startAddr > HYPHY20G_REG_MAX_ADDR)
        ||((pCfgDumpReq->startAddr + pCfgDumpReq->length)> HYPHY20G_REG_MAX_ADDR))
    {
        pCfgDumpReq->rtCode = HYPHY20G_ERR_INVALID_ARG;
        pCfgDumpReq->length = 0;
        (*pValidDumpBuffSize) = dumpBuffSize;
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* loop to read all registers requested in pCfgDumpReq */
    for (i=0; i<pCfgDumpReq->length; i++)
    {
        result = sysHyPhy20gRead(fileHndl, pCfgDumpReq->startAddr+i*sizeof(UINT4), pDumpVal+i);
        /*if read error happen, update the rtCode, successful register read length and 
          the successfull buffer size*/
        if(result) 
        {
            pCfgDumpReq->rtCode = result;
            pCfgDumpReq->length = i;
            (*pValidDumpBuffSize) = dumpBuffSize;
            return result;
        }
        dumpBuffSize += sizeof(UINT4);
    }
    
    /* set the completed length to *pValidDumpBuffSize */
    (*pValidDumpBuffSize) = dumpBuffSize;
    return HYPHY20G_SUCCESS;
}/* untilHyPhy20gDump */

/* end of file */
