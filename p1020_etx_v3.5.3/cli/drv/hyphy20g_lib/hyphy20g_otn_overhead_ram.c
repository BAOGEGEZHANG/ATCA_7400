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
**  FILE        : hyphy20g_otn_overhead_ram.c
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
#include "hyphy20g_otn_overhead_ram.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hyPhy20gOtnOhRamCreateCtxt(struct file *fileHndl);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gOtnOhRamCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the format of the OTN OH RAM.
**                  
**                  FAS, MFAS, JC, BIP, and NJO cannot be inserted via OH RAM.
**
**                  For symmetrical configurations, both Extraction and
**                  Insertion OH RAMs can be initalized with one call to this
**                  function.
**
**                  For asymmetrical configuration, the function may be called
**                  twice to apply unique configurations for Extraction and
**                  Insertion OH RAMs.
**
**                  For INS OH bytes INS_APS_PCC1 to INS_APS_PCC4 
**                  hyPhy20gOtnOhRamApsPccCfg must be called for additional
**                  insertion configuration.
**
**                  Based on the number of bytes selected the function in turn
**                  configures *_NUM_WORDS_PER_FRAME and *_NUM_FRM_PER_RAM.
**
**                  'maxRamSize' is used to configure *_NUM_FRM_PER_RAM to 
**                  provide the maximum number of frames per RAM.
**                  (maxRamSize x (*_NUM_WORDS_PER_FRAME + 1)) should not exceed 
**                  320 for OTU1 framer and 1280 for OTU2 framer.   
**                  
**                  hyPhy20gOtu2DeviceInit must be called before this function is
**                  called.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1                          
**                 ohRam        - OH RAM to be configured
**                                0 - EXTRACTION OH RAM
**                                1 - INSERTION OH RAM
**                                2 - EXTRACTION AND INSERTION OH RAM
**                 maxRamSize   - Maximum RAM size 
**                                0 - Use maximum available RAM size
**                                    320 will be used for OTU1 Framer and
**                                    1280 will be used for OTU2 Framer.
**                                1 - 320 for OTU1 Framer 
**                                1 - 1280 for OTU2 Framer
**                                Refer to DESCRIPTION.   
**                 cfgMsk0      - Ext/Ins mask for OH bytes 0 to 31
**                                Set a bit to 1 to format OH RAM for inclusion
**                                of OH byte.
**                                Set a bit to 0 to format OH RAM for exclusion
**                                of OH byte.
**                                Valid values: 0x00000000 - 0x7FFFFFFF
**                                bit 31   : RESERVED
**                                bit 30   : FAS1      
**                                bit 29   : FAS2      
**                                bit 28   : FAS3      
**                                bit 27   : FAS4      
**                                bit 26   : FAS5      
**                                bit 25   : FAS6      
**                                bit 24   : SM1       
**                                bit 23   : SM2       
**                                bit 22   : SM3       
**                                bit 21   : GCC0_1    
**                                bit 20   : GCC0_2    
**                                bit 19   : RES1      
**                                bit 18   : RES2      
**                                bit 17   : VCOH1     
**                                bit 16   : JC1       
**                                bit 15   : RES3      
**                                bit 14   : RES4      
**                                bit 13   : RES5      
**                                bit 12   : TCM_ACT   
**                                bit 11   : TCM6_1    
**                                bit 10   : TCM6_2    
**                                bit 9    : TCM6_3    
**                                bit 8    : TCM5_1    
**                                bit 7    : TCM5_2    
**                                bit 6    : TCM5_3    
**                                bit 5    : TCM4_1    
**                                bit 4    : TCM4_2    
**                                bit 3    : TCM4_3    
**                                bit 2    : FTFL      
**                                bit 1    : VCOH2     
**                                bit 0    : JC2       
**                 cfgMsk1      - Ext/Ins mask for OH bytes 32 to 63
**                                Set a bit to 1 to format OH RAM for inclusion
**                                of OH byte.
**                                Set a bit to 0 to format OH RAM for exclusion
**                                of OH byte.
**                                Valid values: 0x00000000 - 0xFFFFFFFF
**                                bit 31   : TCM3_1         
**                                bit 30   : TCM3_2      
**                                bit 29   : TCM3_3      
**                                bit 28   : TCM2_1      
**                                bit 27   : TCM2_2      
**                                bit 26   : TCM2_3      
**                                bit 25   : TCM1_1      
**                                bit 24   : TCM1_2      
**                                bit 23   : TCM1_3      
**                                bit 22   : PM1_1       
**                                bit 21   : PM1_2       
**                                bit 20   : PM1_3       
**                                bit 19   : EXP1        
**                                bit 18   : EXP2        
**                                bit 17   : VCOH3       
**                                bit 16   : JC3         
**                                bit 15   : GCC1_1      
**                                bit 14   : GCC1_2      
**                                bit 13   : GCC2_1      
**                                bit 12   : GCC2_2      
**                                bit 11   : APS_PCC1    
**                                bit 10   : APS_PCC2    
**                                bit 9    : APS_PCC3    
**                                bit 8    : APS_PCC4    
**                                bit 7    : RES6        
**                                bit 6    : RES7        
**                                bit 5    : RES8        
**                                bit 4    : RES9        
**                                bit 3    : RES10       
**                                bit 2    : RES11       
**                                bit 1    : PSI         
**                                bit 0    : NJO         
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnOhRamCfg(struct file *fileHndl, UINT4 slice, UINT4 ohRam,
                         UINT4 maxRamSize, UINT4 cfgMsk0, UINT4 cfgMsk1)
{ 
    /* variable declaration */
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_OTN_OH_RAM_CTXT *pOtnOhRamCtxt;
    UINT4 regAddr;
    UINT4 txRegAddr, rxRegAddr;
    UINT4 regData = 0; 
    UINT4 mskInsAddr0, mskInsAddr1;
    UINT4 mskExtAddr0, mskExtAddr1;
    UINT4 hex0, hex1;
    UINT4 msk31 = 0;
    UINT4 msk = 0;
    UINT4 frMsk = 0;
    UINT4 cntByteMsk0 = 0;
    UINT4 cntByteMsk1 = 0;
    UINT4 numWords = 0;
    UINT4 numFr = 0;
    UINT4 i = 0;
    UINT4 rxFrRegAddr, rxMsk = 0, rxFrMsk = 0;
    UINT4 ohRamRecfgAddr, extRecfgOff, insRecfgOff;
    UINT4 sliceOff; 
    INT4 result;
    UINT4 otnOtu2LwrPwr;
    
    /* argument checking */
    if ((slice > 9)||(ohRam > 2)|| (slice < 8 && maxRamSize > 320) ||
        (slice > 7 && maxRamSize > 1280) || (cfgMsk0 > 0x7fffffff) ||
        (cfgMsk1 > 0xffffffff)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");
                    
    /* Initialize pOtnOhRamCtxt */
    pOtnOhRamCtxt = pDevCtxt->pOtnOhRamCtxt;    
    
    /* check OTN_OTU2_LOWPWR to make sure the OTN OTU2 subsystem is out of lowPwr */
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_OTU2_LOWPWR_OFF,
                             &otnOtu2LwrPwr);
    if(result) return result;

    if (otnOtu2LwrPwr==1 || pOtnOhRamCtxt == NULL) 
    {
        DBG_PRINT("%s, %s, %d, hyPhy20gOtu2DeviceInit must be called before this function.\n",
            __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    
    /* Determine register and masks for OHRAM_RECFG */
    ohRamRecfgAddr = HYPHY20G_OTU2_TOP_REG_OHRAM_RECONFIG;
    if (slice > 7) {
        extRecfgOff = HYPHY20G_OTU2_TOP_BIT_OHRAM_RECONFIG_OHRAM_EXT_RECFG_OFF + (slice - 8);
        insRecfgOff = HYPHY20G_OTU2_TOP_BIT_OHRAM_RECONFIG_OHRAM_INS_RECFG_OFF + (slice - 8);
    } else {
        extRecfgOff = HYPHY20G_OTU2_TOP_BIT_OHRAM_RECONFIG_OHRAM_EXT_RECFG_OFF + (slice + 2);
        insRecfgOff = HYPHY20G_OTU2_TOP_BIT_OHRAM_RECONFIG_OHRAM_INS_RECFG_OFF + (slice + 2);
    }
    
    /* Determine registers for Insertion and Extraction */
    sliceOff = (slice > 7)? (slice-8):(slice+2);
        
    mskInsAddr0 = HYPHY20G_OTU2_TOP_REG_OTUK_TX_OH_INS_MASK0_OFFSET(sliceOff);
    mskInsAddr1 = HYPHY20G_OTU2_TOP_REG_OTUK_TX_OH_INS_MSK1_OFFSET(sliceOff);
    DBG_PRINT("mskInsAddr0 = 0x%lx\n", mskInsAddr0);
    DBG_PRINT("mskInsAddr1 = 0x%lx\n", mskInsAddr1);
    
    mskExtAddr0 = HYPHY20G_OTU2_TOP_REG_OTUK_RX_OH_EXT_MASK0_OFFSET(sliceOff);
    mskExtAddr1 = HYPHY20G_OTU2_TOP_REG_OTUK_RX_OH_EXT_MSK1_OFFSET(sliceOff); 
    DBG_PRINT("mskExtAddr0 = 0x%lx\n", mskExtAddr0);
    DBG_PRINT("mskExtAddr1 = 0x%lx\n", mskExtAddr1);
    
    /* Mask for bit31 in Mask0 */
    msk31 = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MASK0_INS_MFAS_MSK;
    
    /* Calculate the number of bytes selected */ 
    hex0 = cfgMsk0;
    hex1 = cfgMsk1; 
    DBG_PRINT("hex0 = 0x%lx\n", hex0);
    DBG_PRINT("hex1 = 0x%lx\n", hex1);
    
    for (i = 0; i < 32; i++) {
        if (((hex0 & (1 << i)) >> i) == 1) {
            DBG_PRINT("[%li] has its bit set\n", i);
            cntByteMsk0++;
            DBG_PRINT("cntByteMsk0 = %li\n", cntByteMsk0);
        }
    }
    
    for (i = 0; i < 32; i++) {
        if (((hex1 & (1 << i))>> i) == 1) {
            DBG_PRINT("[%li] has its bit set\n", i);
            cntByteMsk1++;
            DBG_PRINT("cntByteMsk1 = %li\n", cntByteMsk1);
        }
    }
    
    DBG_PRINT("Byte mask0 = %li and Byte mask1 = %li\n", cntByteMsk0, cntByteMsk1);
   
    /* Calculate number of words per frame */
    numWords = ((cntByteMsk0 + cntByteMsk1)/4) + 1;
    DBG_PRINT("1: numWords = %li\n", numWords);
    
    /* Configure the maximum RAM size */
    if (maxRamSize == 0) { /* Use maximum availabe RAM size */
        /* Calculate number of frames per ram */
        numFr = (slice < 8)? ((320 / numWords) - 1):((1280 / numWords) - 1);
        DBG_PRINT("numFr = %li\n", numFr);
    } else { /* Use user specified maximum RAM size */
        /* Check if maximum RAM size is not violated */
        if (((slice < 8) && ((numWords * maxRamSize) > 320)) ||
           ((slice > 7) && ((numWords * maxRamSize) > 1280))) {
           return HYPHY20G_ERR_INVALID_ARG;
        } 
        numFr = maxRamSize - 1;
    }   
     
    /* Store the user specified maximum RAM size to a context */
    if (slice < 8) { /* Configure OTU1 RAM Size */
        pOtnOhRamCtxt->otu1RamSize = numFr;
        DBG_PRINT("pOtnOhRamCtxt->otu1RamSize = %u\n",(unsigned int)pOtnOhRamCtxt->otu1RamSize);
    } else { /* Configure OTU2 RAM size */
        pOtnOhRamCtxt->otu2RamSize = numFr;
        DBG_PRINT("pOtnOhRamCtxt->otu2RamSize = %u\n",(unsigned int)pOtnOhRamCtxt->otu2RamSize);      
    }        
           
    /* Set *_NUM_WORDS_PER_FRM0/1 */
    numWords = numWords - 1;
    DBG_PRINT("2: numWords = %li\n", numWords);
        
    /* Determine masks for OH Insert Number of Words per Frame and Number of 
       Frames per RAM*/
    switch (slice) {
        case 0: /* OTU1 Framer 0 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM0_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES0_OTU1_INS_NUM_FRM_PER_RAM0_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM0_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES0_OTU1_EXT_NUM_FRM_PER_RAM0_MSK;
            break;
        case 1: /* OTU1 Framer 1 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM1_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES0_OTU1_INS_NUM_FRM_PER_RAM1_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM1_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES0_OTU1_EXT_NUM_FRM_PER_RAM1_MSK;
            break;
        case 2: /* OTU1 Framer 2 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM2_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES1_OTU1_INS_NUM_FRM_PER_RAM2_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM2_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES1_OTU1_EXT_NUM_FRM_PER_RAM2_MSK;
            break;
        case 3: /* OTU1 Framer 3 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM3_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES1_OTU1_INS_NUM_FRM_PER_RAM3_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM3_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES1_OTU1_EXT_NUM_FRM_PER_RAM3_MSK;
            break;
        case 4: /* OTU1 Framer 4 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM4_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES2_OTU1_INS_NUM_FRM_PER_RAM4_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM4_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES2_OTU1_EXT_NUM_FRM_PER_RAM4_MSK;
            break;
        case 5: /* OTU1 Framer 5 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM5_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES2_OTU1_INS_NUM_FRM_PER_RAM5_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM5_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES2_OTU1_EXT_NUM_FRM_PER_RAM5_MSK;
            break;
        case 6: /* OTU1 Framer 6 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM6_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES3_OTU1_INS_NUM_FRM_PER_RAM6_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM6_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES3_OTU1_EXT_NUM_FRM_PER_RAM6_MSK;
            break;
        case 7: /* OTU1 Framer 7 */
            msk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_WORDS_OTU1_INS_NUM_WORDS_PER_FRM7_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_TX_OH_INS_NUM_OF_FRAMES3_OTU1_INS_NUM_FRM_PER_RAM7_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_WORDS_OTU1_EXT_NUM_WORDS_PER_FRM7_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_OTU1_RX_OH_EXT_NUM_OF_FRAMES3_OTU1_EXT_NUM_FRM_PER_RAM7_MSK;
            break;
        case 8: /* OTU2 Framer 0 */
            msk = HYPHY20G_OTU2_TOP_BIT_TX_OH_INS_NUM_OF_WORDS_OTU2_INS_NUM_WORDS_PER_FRM0_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_TX_OH_INS_NUM_OF_FRAMES_OTU2_INS_NUM_FRM_PER_RAM0_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_RX_OH_EXT_NUM_OF_WORDS_OTU2_EXT_NUM_WORDS_PER_FRM0_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_RX_OH_EXT_NUM_OF_FRAMES_OTU2_EXT_NUM_FRM_PER_RAM0_MSK;
            break;
        case 9: /* OTU2 Framer 1 */
            msk = HYPHY20G_OTU2_TOP_BIT_TX_OH_INS_NUM_OF_WORDS_OTU2_INS_NUM_WORDS_PER_FRM1_MSK;
            frMsk = HYPHY20G_OTU2_TOP_BIT_TX_OH_INS_NUM_OF_FRAMES_OTU2_INS_NUM_FRM_PER_RAM1_MSK;
            
            rxMsk = HYPHY20G_OTU2_TOP_BIT_RX_OH_EXT_NUM_OF_WORDS_OTU2_EXT_NUM_WORDS_PER_FRM1_MSK;
            rxFrMsk = HYPHY20G_OTU2_TOP_BIT_RX_OH_EXT_NUM_OF_FRAMES_OTU2_EXT_NUM_FRM_PER_RAM1_MSK;
            break;
    }
    
    /* Determine registers and masks for TX/RX OH Insert Number of Words 
       per Frame */
    if (slice < 8) {
        txRegAddr = HYPHY20G_OTU2_TOP_REG_OTU1_TX_OH_INS_NUM_OF_WORDS;
        rxRegAddr = HYPHY20G_OTU2_TOP_REG_OTU1_RX_OH_EXT_NUM_OF_WORDS;
    } else {
        txRegAddr = HYPHY20G_OTU2_TOP_REG_TX_OH_INS_NUM_OF_WORDS;
        rxRegAddr = HYPHY20G_OTU2_TOP_REG_RX_OH_EXT_NUM_OF_WORDS;
    }
    
    //DBG_PRINT("regAddr = 0x%lx\n", regAddr);
     
    /* Determine registers and masks for OH Insert Number of Frames per RAM */
    if ((slice == 0) || (slice == 1))  {
        regAddr = HYPHY20G_OTU2_TOP_REG_OTU1_TX_OH_INS_NUM_OF_FRAMES0;
        rxFrRegAddr = HYPHY20G_OTU2_TOP_REG_OTU1_RX_OH_EXT_NUM_OF_FRAMES0;
    } else if ((slice == 2) || (slice == 3)) {
        regAddr = HYPHY20G_OTU2_TOP_REG_OTU1_TX_OH_INS_NUM_OF_FRAMES1;
        rxFrRegAddr = HYPHY20G_OTU2_TOP_REG_OTU1_RX_OH_EXT_NUM_OF_FRAMES1;
    } else if ((slice == 4) || (slice == 5)) {
        regAddr = HYPHY20G_OTU2_TOP_REG_OTU1_TX_OH_INS_NUM_OF_FRAMES2;
        rxFrRegAddr = HYPHY20G_OTU2_TOP_REG_OTU1_RX_OH_EXT_NUM_OF_FRAMES2;
    } else if ((slice == 6) || (slice == 7)) {
        regAddr = HYPHY20G_OTU2_TOP_REG_OTU1_TX_OH_INS_NUM_OF_FRAMES3;
        rxFrRegAddr = HYPHY20G_OTU2_TOP_REG_OTU1_RX_OH_EXT_NUM_OF_FRAMES3;
    } else {
        regAddr = HYPHY20G_OTU2_TOP_REG_TX_OH_INS_NUM_OF_FRAMES;
        rxFrRegAddr = HYPHY20G_OTU2_TOP_REG_RX_OH_EXT_NUM_OF_FRAMES;
    }
    
    DBG_PRINT("regAddr numFr = 0x%lx\n", regAddr);
    
    switch(ohRam) {

        /* Extraction */
        case(0):
            
            /* Set OHRAM_EXT_RECFG = 1 before changing RAM configuration */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, extRecfgOff, 1);
            if (result) return result;
                
            /* Write Extraction Mask0 */ 
            result = sysHyPhy20gWrite(fileHndl, mskExtAddr0, cfgMsk0);
            if (result) return result;
            
            /* Write Extraction Mask1 */   
            result = sysHyPhy20gWrite(fileHndl, mskExtAddr1, cfgMsk1);
            if (result) return result;
           
            /* Write EXT_NUM_WORDS */ 
            result = hyPhy20gRegFieldWrite(fileHndl, rxRegAddr, rxMsk, 
                                           numWords);
            if (result) return result;
            
            /* Write EXT_NUM_OF_FRAMES */ 
            result = hyPhy20gRegFieldWrite(fileHndl, rxFrRegAddr, rxFrMsk, 
                                           numFr);    
            if (result) return result;
                
            /* Set OHRAM_EXT_RECFG = 0 for normal operation */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, extRecfgOff, 0);
            if (result) return result;
            
            break;
            
        /* Insertion */
        case(1):
            
            /* Set OHRAM_INS_RECFG = 1 before changing OH RAM configuration */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, insRecfgOff, 1);
            if (result) return result;
                
            /* When writing to Mask0 register, set bit 31 to 1 */ 
            regData = (cfgMsk0 | msk31);
                  
            result = sysHyPhy20gWrite(fileHndl, mskInsAddr0, regData);
            if (result) return result;
            
            /* Write Insertion Mask1 */
            result = sysHyPhy20gWrite(fileHndl, mskInsAddr1, cfgMsk1);
            if (result) return result;
            
            /* Write INS_NUM_WORDS */ 
            result = hyPhy20gRegFieldWrite(fileHndl, txRegAddr, msk, numWords);
            if (result) return result;
                
            /* Write INS_NUM_OF_FRAMES */
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, frMsk, numFr);    
            if (result) return result;
                
            /* Set OHRAM_INS_RECFG = 0 for normal operation */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, insRecfgOff, 0);
            if (result) return result;
                  
            break;
        
        /* Extration and Insertion */         
        case(2):
            
            /* Set OHRAM_EXT_RECFG = 1 before changing RAM configuration */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, extRecfgOff, 1);
            if (result) return result;
            
            /* Set OHRAM_INS_RECFG = 1 before changning OH RAM configuration */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, insRecfgOff, 1);
            if (result) return result;
          
            /* When writing to Mask0 register, set bit 31 to 1 */ 
            result = hyPhy20gRegFieldWrite(fileHndl, mskExtAddr0, msk31, 1);
            if (result) return result;
            
            result = sysHyPhy20gWrite(fileHndl, mskExtAddr0, cfgMsk0);
            if (result) return result;
             
            /* When writing to Mask0 register, set bit 31 to 1 */ 
            regData = (cfgMsk0 | msk31);
            
            result = sysHyPhy20gWrite(fileHndl, mskInsAddr0, regData);
            if (result) return result;
            
            /* Write Extraction Mask1 */     
            result = sysHyPhy20gWrite(fileHndl, mskExtAddr1, cfgMsk1);
            if (result) return result;
            
            /* Write Insertion Mask1 */    
            result = sysHyPhy20gWrite(fileHndl, mskInsAddr1, cfgMsk1);
            if (result) return result;
            
            /* Write INS_NUM_WORDS */
            result = hyPhy20gRegFieldWrite(fileHndl, txRegAddr, msk, numWords);
            if (result) return result;
                
            /* Write INS_NUM_OF_FRAMES */
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, frMsk, numFr);    
            if (result) return result;
            
            /* Write EXT_NUM_WORDS */ 
            result = hyPhy20gRegFieldWrite(fileHndl, rxRegAddr, rxMsk, 
                                           numWords);
            if (result) return result;
            
            /* Write EXT_NUM_OF_FRAMES */ 
            result = hyPhy20gRegFieldWrite(fileHndl, rxFrRegAddr, rxFrMsk, 
                                           numFr);    
            if (result) return result;
               
            /* Set OHRAM_EXT_RECFG = 0 for normal operation */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, extRecfgOff, 0);
            if (result) return result;
                
            /* Set OHRAM_INS_RECFG = 0 for normal operation */
            result = sysHyPhy20gBitWrite(fileHndl, ohRamRecfgAddr, insRecfgOff, 0);
            if (result) return result;
                
            break;          
    }   
  
    return HYPHY20G_SUCCESS;
} /* hyPhy20gOtnOhRamCfg */


/*******************************************************************************
**
**  hyPhy20gOtnOhRamApsPccCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures which multiframes of the APS/PCC
**                  bytes will be inserted from the OTN OH RAM.
**
**                  This function is dependent on hyPhy20gOtnOhRamCfg being
**                  called to configure OTN Insertion OH RAM with APS_PCC1 to
**                  APS_PCC4 set to 1.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 mfMsk        - Mask of multiframes to have APS_PCC byte
**                                insertion.
**                                Set to 1 to enable insertion from OH RAM
**                                for the multiframe
**                                Set to 0 to disable insertion from OH RAM
**                                for the multiframe
**                                bit 7 - frame 7 of the multiframe
**                                bit 6 - frame 6 of the multiframe
**                                bit 5 - frame 5 of the multiframe
**                                bit 4 - frame 4 of the multiframe
**                                bit 3 - frame 3 of the multiframe
**                                bit 2 - frame 2 of the multiframe
**                                bit 1 - frame 1 of the multiframe
**                                bit 0 - frame 0 of the multiframe
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnOhRamApsPccCfg(struct file *fileHndl, UINT4 slice, UINT4 mfMsk)
{
    /* variable declaration */
    UINT4 regAddr, sliceOff;
    UINT4 ctrlReg0, ctrlReg1, ctrlReg2;
    UINT4 apsPccMsk[4];
    UINT4 apsPccOff[4];
    UINT4 otu1Msk[8];
    UINT4 otu2Msk[2];
    UINT4 insMsk1Data;
    //UINT4 i = 0;
    INT4 result;
    
     /* argument checking */
    if ((slice > 9)||(mfMsk > 0xff)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Ensure APS_PCC1 through APS_PCC4 are set to 1 */ 
    sliceOff = (slice > 7)? (slice - 8):(slice + 2);
    regAddr = HYPHY20G_OTU2_TOP_REG_OTUK_TX_OH_INS_MSK1_OFFSET(sliceOff);
            
    apsPccMsk[0] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC1_MSK;
    apsPccMsk[1] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC2_MSK;
    apsPccMsk[2] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC3_MSK;
    apsPccMsk[3] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC4_MSK;
    
    apsPccOff[0] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC1_OFF;
    apsPccOff[1] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC2_OFF;
    apsPccOff[2] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC3_OFF;
    apsPccOff[3] = HYPHY20G_OTU2_TOP_BIT_OTUK_TX_OH_INS_MSK1_INS_APS_PCC4_OFF;
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &insMsk1Data); 
    if (result) return result; 
    
    /* Determine registers and masks for APS/PCC MFAS Insert Control */
    ctrlReg0 = HYPHY20G_OTU2_TOP_REG_OTU1_APS_PCC_MFAS_INS_CONTROL0;
    ctrlReg1 = HYPHY20G_OTU2_TOP_REG_OTU1_APS_PCC_MFAS_INS_CONTROL1;
    ctrlReg2 = HYPHY20G_OTU2_TOP_REG_APS_PCC_MFAS_INS_CTL;                  
    
    otu1Msk[0] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL0_OTU1_APS_PCC_INS_MFAS0_MSK;
    otu1Msk[1] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL0_OTU1_APS_PCC_INS_MFAS1_MSK;
    otu1Msk[2] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL0_OTU1_APS_PCC_INS_MFAS2_MSK;
    otu1Msk[3] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL0_OTU1_APS_PCC_INS_MFAS3_MSK;
    otu1Msk[4] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL1_OTU1_APS_PCC_INS_MFAS4_MSK;
    otu1Msk[5] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL1_OTU1_APS_PCC_INS_MFAS5_MSK;
    otu1Msk[6] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL1_OTU1_APS_PCC_INS_MFAS6_MSK;
    otu1Msk[7] = HYPHY20G_OTU2_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CONTROL1_OTU1_APS_PCC_INS_MFAS7_MSK;
    
    otu2Msk[0] = HYPHY20G_OTU2_TOP_BIT_APS_PCC_MFAS_INS_CTL_OTU2_APS_PCC_INS_MFAS0_MSK;
    otu2Msk[1] = HYPHY20G_OTU2_TOP_BIT_APS_PCC_MFAS_INS_CTL_OTU2_APS_PCC_INS_MFAS1_MSK;
    
    if (slice < 4) {
        result = hyPhy20gRegFieldWrite(fileHndl, ctrlReg0, otu1Msk[slice], 
                                       mfMsk);
        if (result) return result;    
    } else if ((slice > 3) && (slice < 8)) {
        result = hyPhy20gRegFieldWrite(fileHndl, ctrlReg1, otu1Msk[slice], 
                                       mfMsk);
        if (result) return result;    
    } else {
        result = hyPhy20gRegFieldWrite(fileHndl, ctrlReg2, otu2Msk[slice-8], 
                                       mfMsk);
        if (result) return result;
    }    
     
    return HYPHY20G_SUCCESS;
} /* hyPhy20gOtnOhRamApsPccCfg */

/*******************************************************************************
**
**  hyPhy20gOtnOhRamInsEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the insertion of overhead bytes
**                  at an OTUk, ODUk or OPUk granularity for an OTU Framer.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 otukEnbl     - Enable OTUk OH byte insertion
**                                0 - Disabled
**                                1 - Enabled
**                 odukEnbl     - Enable ODUk OH byte insertion
**                                0 - Disabled
**                                1 - Enabled
**                 opukEnbl     - Enable OPUk OH byte insertion
**                                0 - Disabled
**                                1 - Enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnOhRamInsEnable(struct file *fileHndl, UINT4 slice, 
                               UINT4 otukEnbl, UINT4 odukEnbl, UINT4 opukEnbl)
{
    /* variable declaration */
    UINT4 otu1Addr, otu2Addr;
    UINT4 odu1Addr, odu2Addr;
    UINT4 opu1Addr, opu2Addr;
    UINT4 otu1Msk, otu2Msk;
    UINT4 odu1Msk, odu2Msk;
    UINT4 opu1Msk, opu2Msk;
    INT4 result;
    
    /* argument checking */
    if ((slice > 9)||(otukEnbl > 1) || (odukEnbl > 1) || (opukEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Determine registers and masks */
    otu1Addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL;
    otu1Addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(otu1Addr, slice);
    odu1Addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_ODU_CTL;
    odu1Addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(odu1Addr, slice);
    opu1Addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_OPU_CTL;
    opu1Addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(opu1Addr, slice);
    
    otu2Addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
    otu2Addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(otu2Addr, slice-8); 
    odu2Addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_ODU_CTL;                    
    odu2Addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(odu2Addr, slice-8); 
    opu2Addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_CTL;
    opu2Addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(opu2Addr, slice-8); 
    
    otu1Msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_OTU_PORT_INSERT_MSK;
    odu1Msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_ODU_CTL_ODU_PORT_INSERT_MSK;
    opu1Msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_OPU_CTL_OPU_PORT_INSERT_MSK;
    
    otu2Msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_OTU_PORT_INSERT_MSK;
    odu2Msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_ODU_CTL_ODU_PORT_INSERT_MSK;
    opu2Msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_OPU_CTL_OPU_PORT_INSERT_MSK;
    
    /* Modify the registers with *Enbl mask values */
    if (slice < 8) {
        /* OTU1 Framer */
        result = hyPhy20gRegFieldWrite(fileHndl, otu1Addr, otu1Msk, otukEnbl);
        if (result) return result;
        
        result = hyPhy20gRegFieldWrite(fileHndl, odu1Addr, odu1Msk, odukEnbl);
        if (result) return result;
        
        result = hyPhy20gRegFieldWrite(fileHndl, opu1Addr, opu1Msk, opukEnbl);
        if (result) return result;    
    } else if (slice > 7) {
        /* OTU2 Framer */
        result = hyPhy20gRegFieldWrite(fileHndl, otu2Addr, otu2Msk, otukEnbl);
        if (result) return result;
        
        result = hyPhy20gRegFieldWrite(fileHndl, odu2Addr, odu2Msk, odukEnbl);
        if (result) return result;
        
        result = hyPhy20gRegFieldWrite(fileHndl, opu2Addr, opu2Msk, opukEnbl);
        if (result) return result;
     }
        
   
    return HYPHY20G_SUCCESS;
} /* hyPhy20gOtnOhRamInsEnable */


/*******************************************************************************
**
**  hyPhy20gOtnOhRamGetPointers
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the OTN OH RAM insertion and
**                  extraction pointers.
**
**                  It performs a burst access to retrieve all pointer data
**                  in a single PCIe transaction.
**
**  INPUTS:       *fileHndl      - base address
**                *extRamPtr[10] - OTN OH Extraction RAM pointers
**                                 index 0 to 7 OTU1 Framer 0 to 7
**                                 index 8 to 9 OTU2 Framer 0 to 1
**                *insRamPtr[10] - OTN OH Insertion RAM pointers
**                                 index 0 to 7 OTU1 Framer 0 to 7
**                                 index 8 to 9 OTU2 Framer 0 to 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnOhRamGetPointers(struct file *fileHndl, UINT4 *extRamPtr, 
                                 UINT4 *insRamPtr)
{
    /* variable declaration */
    UINT4 regAddr1, regAddr2;
    UINT4 ptrStatData1, ptrStatData2;
    UINT4 extRamPtrData[10], insRamPtrData[10];
    UINT4 otu1ExtMsk, otu1InsMsk;
    UINT4 otu2ExtMsk, otu2InsMsk;
    UINT4 i = 0;
    INT4 result;
    
    /* Determine masks */
    otu1ExtMsk = HYPHY20G_OTN_OH_BIT_OTU1_RAM_PTR_STAT_OTU1_EXT_RAM_PTR_MSK;
    otu1InsMsk = HYPHY20G_OTN_OH_BIT_OTU1_RAM_PTR_STAT_OTU1_INS_RAM_PTR_MSK;
    
    otu2ExtMsk = HYPHY20G_OTN_OH_BIT_OTU2_RAM_PTR_STAT_OTU2_EXT_RAM_PTR_MSK;
    otu2InsMsk = HYPHY20G_OTN_OH_BIT_OTU2_RAM_PTR_STAT_OTU2_INS_RAM_PTR_MSK;
    
    for (i = 0; i < 8; i++) {
        regAddr1 = HYPHY20G_OTN_OH_REG_OTU1_RAM_PTR_STAT_OFFSET(i); 
        result = sysHyPhy20gRead(fileHndl, regAddr1, &ptrStatData1);
        if (result) return result;
            
        result = hyPhy20gVarFieldRead(ptrStatData1, otu1ExtMsk, &extRamPtrData[i]);
        if (result) return result;
            
        result = hyPhy20gVarFieldRead(ptrStatData1, otu1InsMsk, &insRamPtrData[i]);
        if (result) return result;
    }
    
    for (i = 0; i < 2; i++) {
        regAddr2 = HYPHY20G_OTN_OH_REG_OTU2_RAM_PTR_STAT_OFFSET(i);
        result = sysHyPhy20gRead(fileHndl, regAddr2, &ptrStatData2);
        if (result) return result;
            
        result = hyPhy20gVarFieldRead(ptrStatData2, otu2ExtMsk, &extRamPtrData[i+8]);
        if (result) return result;
            
        result = hyPhy20gVarFieldRead(ptrStatData2, otu2InsMsk, &insRamPtrData[i+8]);
        if (result) return result;
    }

    for (i = 0; i < 10; i++) {  
        *extRamPtr = extRamPtrData[i];
        extRamPtr++;
        
        *insRamPtr = insRamPtrData[i];
        insRamPtr++;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gOtnOhRamGetPointers */


/*******************************************************************************
**
**  hyPhy20gOtnOhRamBurstRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a 128 byte burst read access to
**                  overhead RAM.  In cases where the starting access address
**                  is less than 128 bytes from the end of the RAM for the
**                  framer, the function will perform more than one device
**                  access.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 addr         - RAM row number to burst read from
**                                0 - 319 for OTU1 Framer Instances
**                                0 - 1279 for OTU2 Framer Instances
**                                Note: Maximum RAM row number should equal to  
**                                      'maxRamSize' - 1 specified in 
**                                      hyPhy20gOtnOhRamCfg.
**                *pData[32]    - Pointer to an array for read data storage.
**                                Consists of 32 - 4 byte RAM rows.
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnOhRamBurstRead(struct file *fileHndl, UINT4 slice, 
                               UINT4 addr, UINT4 *pData)
{
    /* variable declaration */
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_OTN_OH_RAM_CTXT *pOtnOhRamCtxt;
    UINT4 regAddr = 0;
    UINT4 startAddr = 0;
    UINT4 tmpData[32];
    UINT4 pRamSize;
    UINT4 szToEnd = 0;
    UINT4 readRest = 1;
    UINT4 i = 0;
    UINT4 j = 0;
    INT4 result;
    
    /* argument checking */
    if ((slice > 9) || ((slice < 8) && (addr > 319)) || 
        ((slice > 7) && (addr > 1279))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the OTN OH RAM context is valid */
    if(pDevCtxt->pOtnOhRamCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
 
    pOtnOhRamCtxt = pDevCtxt->pOtnOhRamCtxt;
    
    pRamSize = (slice < 8)? pOtnOhRamCtxt->otu1RamSize:pOtnOhRamCtxt->otu2RamSize;
    
    DBG_PRINT("pRamSize = %li\n", pRamSize);
    
    /* Check if the user specified starting address is valid */
    if (addr > pRamSize) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Read from OTU2 Extract RAM 0 at 0x2004000 + 0x4*N(N=0:1279) */
    switch (slice) {
        case 0: /* OTU1 Framer 0 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM0_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM0_OFFSET(addr);
            break;
        case 1: /* OTU1 Framer 1 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM1_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM1_OFFSET(addr);
            break;
        case 2: /* OTU1 Framer 2 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM2_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM2_OFFSET(addr);
            break;
        case 3: /* OTU1 Framer 3 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM3_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM3_OFFSET(addr);
            break;
        case 4: /* OTU1 Framer 4 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM4_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM4_OFFSET(addr);
            break;
        case 5: /* OTU1 Framer 5 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM5_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM5_OFFSET(addr);
            break;
        case 6: /* OTU1 Framer 6 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM6_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM6_OFFSET(addr);
            break;
        case 7: /* OTU1 Framer 7 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM7_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_EXT_RAM7_OFFSET(addr);
            break;
        case 8: /* OTU2 Framer 0 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU2_EXT_RAM0_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU2_EXT_RAM0_OFFSET(addr);
            break;
        case 9: /* OTU2 Framer 1 */
            startAddr = HYPHY20G_OTN_OH_REG_OTU2_EXT_RAM1_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU2_EXT_RAM1_OFFSET(addr);
            break;
    }
    
    /* Calculate number of reads until the end of the RAM */
    szToEnd = pRamSize - addr;
    
    DBG_PRINT("szToEnd = %li\n", szToEnd);
    
    /* Calculate number of reads after wrap around */
    readRest = 32 - szToEnd;
    DBG_PRINT("readRest = %li\n", readRest); 
    
    /* if the starting access addres is less than 128 bytes from the end of 
       the RAM for the framer */
    if (szToEnd < 32) {
        
        /* more than one device read */ 
        result = sysHyPhy20gPcieBurstRead(fileHndl, regAddr, szToEnd, tmpData);
        if (result) {
            return result;
        }
        /* Copy wrapped around data */      
        for (i = 0; i < szToEnd; i++) {
            pData[i] = tmpData[i];  
        }
        
        /* Wrap around and read from the beginning of the RAM */
        result = sysHyPhy20gPcieBurstRead(fileHndl, startAddr, readRest,
                                          tmpData);
        if (result) {
            return result; 
        }
        
        /* Copy wrapped around data */      
        for (j = 0; j < readRest; j++) {
            pData[j+szToEnd] = tmpData[j];  
        }
        
    }
    /* one device access for 128 bytes burst read */
    else {
        /* read once */
        result = sysHyPhy20gPcieBurstRead(fileHndl, regAddr, 32, tmpData);
        if (result) {
            return result;
        }
		
		/* Copy data to return */ 
		for (i = 0; i < 32; i++) {
			 pData[i] = tmpData[i];
		}	
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOtnOhRamBurstRead */
  

/*******************************************************************************
**
**  hyPhy20gOtnOhRamBurstWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a 128 byte burst write access to
**                  overhead RAM.  In cases where the starting access address
**                  is less than 128 bytes(32 rows) from the end of the RAM 
**                  for the framer, the function will perform more than one 
**                  device access.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 addr         - RAM row number to burst write to
**                                0 - 319 for OTU1 Framer Instance
**                                0 - 1279 for OTU2 Framer Instance
**                                Note: Maximum RAM row number should equal to
**                                      'maxRamSize' - 1 as specified in
**                                      hyPhy20gOtnOhRamCfg.
**                 data[32]     - Data to be written to OH RAM
**                                Consists of 32 - 4 byte RAM rows.
**                                Note: 32 32-bit values in the format of 
**                                      0xaabbccdd are required where the left- 
**                                      most byte of the first word of each   
**                                      frame must be MFAS value in incremental
**                                      order from 00 to FF.   
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnOhRamBurstWrite(struct file *fileHndl, UINT4 slice, 
                                UINT4 addr, UINT4 data[32])
{    
    /* variable declaration */ 
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_OTN_OH_RAM_CTXT *pOtnOhRamCtxt; 
    UINT4 pRamSize;              
    UINT4 startAddr = 0;
    UINT4 regAddr = 0; 
    UINT4 szToEnd = 0;
    UINT4 writeRest = 0;
    INT4 result;
    
    /* argument checking */
    if ((slice > 9) || ((slice < 8) && (addr > 319)) ||
        ((slice > 7) && (addr > 1279))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the OTN OH RAM context is valid */
    if(pDevCtxt->pOtnOhRamCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
 
    pOtnOhRamCtxt = pDevCtxt->pOtnOhRamCtxt;
    
    pRamSize = (slice < 8)? pOtnOhRamCtxt->otu1RamSize:pOtnOhRamCtxt->otu2RamSize;
    
    DBG_PRINT("pRamSize = %li\n", pRamSize);
    
    /* Check if the user specified starting address is valid */
    if (addr > pRamSize - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Determine register for each RAM */ 
    switch (slice) {
        case 0:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM0_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM0_OFFSET(addr);
            break;
        case 1:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM1_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM1_OFFSET(addr);
            break;
        case 2:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM2_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM2_OFFSET(addr);
            break;
        case 3:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM3_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM3_OFFSET(addr);
            break;
        case 4:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM4_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM4_OFFSET(addr);
            break;
        case 5:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM5_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM5_OFFSET(addr);
            break;
        case 6:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM6_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM6_OFFSET(addr);
            break;
        case 7:
            startAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM7_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU1_INS_RAM7_OFFSET(addr);
            break;
        case 8:
            startAddr = HYPHY20G_OTN_OH_REG_OTU2_INS_RAM0_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU2_INS_RAM0_OFFSET(addr);
            break;
        case 9:
            startAddr = HYPHY20G_OTN_OH_REG_OTU2_INS_RAM1_OFFSET(0);
            regAddr = HYPHY20G_OTN_OH_REG_OTU2_INS_RAM1_OFFSET(addr);
            break;
    }
    
    /* Calculate number of writes until the end of the RAM */
    szToEnd = pRamSize - addr;
    DBG_PRINT("szToEnd = %li\n", szToEnd);
    
    /* Calculate number of writes after wrap around */
    writeRest = 32 - szToEnd;
    DBG_PRINT("writeRest = %li\n", writeRest);
    
    /* if the starting access addres is less than 128 bytes from the end of 
       the RAM for the framer */
    if (szToEnd < 32) {
  
        /* more than one device write */ 
        result = sysHyPhy20gPcieBurstWrite(fileHndl, regAddr, szToEnd, data);
        if (result) {
            return result;
        }
        
        result = sysHyPhy20gPcieBurstWrite(fileHndl, startAddr, writeRest, 
                                           &data[szToEnd]);
        if (result) {
            return result;  
        }
    }
    /* one device access for 128 bytes burst read */
    else {
        /* read once */
        result = sysHyPhy20gPcieBurstWrite(fileHndl, regAddr, 32, data);
        if (result) {
            return result;
        }
    }      
            
    return HYPHY20G_SUCCESS;
} /* hyPhy20gOtnOhRamBurstWrite */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gOtnOhRamCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  the OTN OH RAM.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function hyPhy20gOtnOhRamDestroyCtxt recovers this memory 
**                  and set the device level reference to NULL.
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
INT4 hyPhy20gOtnOhRamCreateCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_OTN_OH_RAM_CTXT  *pOtnOhRamCtxt;
    UINT4 size;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pOtnOhRamCtxt is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pOtnOhRamCtxt != NULL) {
        hyPhy20gOtnOhRamDestroyCtxt(fileHndl);
    }

    /* allocate memory for and associate with pDevCtxt */
    size = sizeof(sHYPHY20G_OTN_OH_RAM_CTXT);

    pDevCtxt->pOtnOhRamCtxt = (sHYPHY20G_OTN_OH_RAM_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pOtnOhRamCtxt == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    DBG_PRINT("OTN OH RAM Context memory allocated:addr=0x%x size=%ld\n",
              (unsigned int)pDevCtxt->pOtnOhRamCtxt, size);

    pOtnOhRamCtxt = pDevCtxt->pOtnOhRamCtxt;

    /* initialize memories */
    pOtnOhRamCtxt->otu1RamSize = 0;
    pOtnOhRamCtxt->otu2RamSize = 0;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtnOhRamCreateCtxt */

/*******************************************************************************
**
**  hyPhy20gOtnOhRamDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the OTN OH RAM.
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
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnOhRamDestroyCtxt(struct file *fileHndl)
{
    sHYPHY20G_OTN_OH_RAM_CTXT      *pOtnOhRamCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pOtnOhRamCtxt = pDevCtxt->pOtnOhRamCtxt;

    if(pOtnOhRamCtxt == NULL){
        /* Nothing to destroy */
        DBG_PRINT("%s, %s, %d, returning success as already NULL\n",__FILE__, __FUNCTION__, __LINE__);
    	return HYPHY20G_SUCCESS;
    }

    sysHyPhy20gMemFree(pDevCtxt->pOtnOhRamCtxt);
    DBG_PRINT("%s, %s, %d, returning success after free\n",__FILE__, __FUNCTION__, __LINE__);
    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtnOhRamDestroyCtxt */

/*******************************************************************************
**
**  otnOhRamhyPhy20gOtnOhRamGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of OtnOhRam context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of OtnOhRam context
**
*******************************************************************************/
UINT4 otnOhRamhyPhy20gOtnOhRamGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 size = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* include ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();

    /* calculate context size */
    if (pDevCtxt->pOtnOhRamCtxt != NULL) {
        size += sizeof(sHYPHY20G_OTN_OH_RAM_CTXT);
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* otnOhRamhyPhy20gOtnOhRamGetCtxtSize */

/*******************************************************************************
**
**  otnOhRamhyPhy20gOtnOhRamExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the OtnOhRam context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for OtnOhRam context 
**                  extraction
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
INT4 otnOhRamhyPhy20gOtnOhRamExtractCtxt(struct file *fileHndl, 
                                    void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_OTN_OH_RAM_CTXT *pOtnOhRamCtxt;
    UINT4 rtLen;
    UINT4 offset = 0, len = 0;

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

    pOtnOhRamCtxt = pDevCtxt->pOtnOhRamCtxt;

    DBG_PRINT("%s, %s, %d, header offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* insert ctxt buffer header */
    len = (pOtnOhRamCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
            (sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_OTN_OH_RAM_CTXT));
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_OTN_OH_RAM_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);
    
    if (pOtnOhRamCtxt != NULL) {
        /* copy the context into the buffer */
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                        (void *)pOtnOhRamCtxt, sizeof(sHYPHY20G_OTN_OH_RAM_CTXT));
        offset += sizeof(sHYPHY20G_OTN_OH_RAM_CTXT);
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;

    return HYPHY20G_SUCCESS;
}/* otnOhRamhyPhy20gOtnOhRamExtractCtxt */

/*******************************************************************************
**
**  otnOhRamhyPhy20gOtnOhRamRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the OtnOhRam context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored OtnOhRam context for restoration
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
INT4 otnOhRamhyPhy20gOtnOhRamRestoreCtxt(struct file *fileHndl, 
                                     void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_OTN_OH_RAM_CTXT *pOtnOhRamCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the OtnOhRam header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_OTN_OH_RAM_CTXT_ID, HYPHY_NO_INDEX);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_OTN_OH_RAM_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;
    
    DBG_PRINT("%s, %s, %d, 1st offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, len);

    /* check if the payload is valid */
    if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
        offset = sizeof(sHYPHY20G_CTXT_HDR);
    } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
        /* create OtnOhRam context */
        result = hyPhy20gOtnOhRamCreateCtxt(fileHndl);
        if (result) {
            DBG_PRINT("%s, %s, %d, hyPhy20gOtnOhRamCreateCtxt failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return result;
        }

        /* retrieve device context */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pDevCtxt is NULL.");

        pOtnOhRamCtxt = pDevCtxt->pOtnOhRamCtxt;

        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        
        /* payload len */
        len = pCtxtBufHdr->len - 
                sizeof(sHYPHY20G_CTXT_HDR);
        
        sysHyPhy20gMemCpy((void *)(pOtnOhRamCtxt), 
                        ((UINT1 *)pCtxtBuf + offset), len);
        offset += len;
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
}/* otnOhRamhyPhy20gOtnOhRamRestoreCtxt */

/********************************************************************************
**
**  otnOhRamHyPhy20gOtnOhRamAuditCtxt 
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function audit the OTN OverHead Ram subCTXT.
**
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 otnOhRamHyPhy20gOtnOhRamAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_OTN_OH_RAM_CTXT *pOtnOhRamCtxt;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    /* the below check rule mainly come from hyPhy20gOtnOhRamCfg */
    pOtnOhRamCtxt=pDevCtxt->pOtnOhRamCtxt;
    if (pOtnOhRamCtxt!=NULL)
    {
        if ((pOtnOhRamCtxt->otu1RamSize > 320)||
            (pOtnOhRamCtxt->otu2RamSize > 1280))
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
    }
    
    return HYPHY20G_SUCCESS;
    
}/* otnOhRamHyPhy20gOtnOhRamAuditCtxt */

/* end of file */
