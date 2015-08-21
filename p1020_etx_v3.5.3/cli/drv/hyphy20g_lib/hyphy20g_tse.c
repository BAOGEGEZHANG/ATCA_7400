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
**  FILE        : hyphy20g_tse.c
**
**  $Date: 2011-11-24 14:57:54 +0800 (Thu, 24 Nov 2011) $
**
**  $Revision: 17388 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include    "hyphy20g_tse.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 tseHyPhy20gTseCreateCtxt(struct file *fileHndl);

INT4 tseHyPhy20gTseCreateTslotCtxt(struct file *fileHndl);

INT4 tseHyPhy20gTseDestroyTslotCtxt(struct file *fileHndl);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gTseInit
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function performs a device top level reset of the TSE
**                  subsystem and initializes the individual components within
**                  the TSE block.
**
**                  It leaves the PRGM & SHPI blocks in the reset state.  It 
**                  also leaves the 20 DPSF blocks in the reset state and does
**                  not drive any of the 40 output ports.  It also initializes
**                  the page controller to generate UNEQ-M from the Diagnostic
**                  port towards all outgoing STS-1 timeslots. 
**
**                  When this function is called with enable = 2 for 2xPOS Loop 
**                  Timing mode, TSE subsystem is put in low power. 
**
**  INPUTS:         *fileHndl   - base address
**                  mode        - 0 - Monolithic 100G TSE Switch
**                                1 - Split 2x50G TSE Switch
**                  enable      - 0 - TSE subsystem disabled 
**                                1 - TSE subsystem enabled
**                                2 - 2xPOS mode TSE subsystem in low power
**                                  
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTseInit(struct file *fileHndl, UINT4 mode, UINT4 enable) 
{
    UINT4 regAddr;
    UINT4 tseTxRxJ0Delay = 0;
    UINT4 dmiJ0Delay, vcatJ0Delay;
    UINT4 tsePctlIport;
    UINT4 i, j;
    UINT4 offset;
    UINT4 iter;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    /* argument checking */
    if ((enable > 2)|| (mode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;


    /* Reset TSE susbystem at the top level.  Configure TSE_RST */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;        

    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;        
    
    /* Configure Top Level Low Power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;        
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;        
    }

    if (enable == 1 || enable == 2) {
        /* Reset DPSF, PRGM, SHPI, PCTL subsystems within TSE */
        regAddr = HYPHY20G_TSE_TOP_REG_SW_RST;
    
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;        
            
        /* remove PCTL and DPSF reset */
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0x78C);
        if (result) return result;          
                            
        /* Configure J0 Delays within TSE */               
        vcatJ0Delay = HYPHY20G_TSE_RX_J0DLY - 1549;

        if (mode == 0) {
        /* 100G Mode */       
            /* DMI_J0 Delay */
            dmiJ0Delay = HYPHY20G_TSE_TX_J0DLY - 3;
        
            regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_OF_DMI;
            result = sysHyPhy20gWrite(fileHndl, regAddr, dmiJ0Delay);
            if (result) return result;
                
            /* TSE Rx_J0 Delay & Tx_J0 Delay */
            regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR;
            result = hyPhy20gVarFieldWrite(&tseTxRxJ0Delay, 
                                HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_TSE_TX_J0DLY_MSK, 
                                HYPHY20G_TSE_TX_J0DLY); 
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&tseTxRxJ0Delay, 
                                HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_TSE_RX_J0DLY_MSK, 
                                HYPHY20G_TSE_RX_J0DLY); 
            if (result) return result;
            result = sysHyPhy20gWrite(fileHndl, regAddr, tseTxRxJ0Delay);
            if (result) return result;
        } else {
        /* 2 x 50G Mode */
            /* TSE Rx_J0 Delay */
            regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR;
            result = hyPhy20gVarFieldWrite(&tseTxRxJ0Delay, 
                                HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_TSE_RX_J0DLY_MSK, 
                                HYPHY20G_TSE_RX_J0DLY); 
            if (result) return result;
            result = sysHyPhy20gWrite(fileHndl, regAddr, tseTxRxJ0Delay);
            if (result) return result;
        }

        /* VCAT_J0 Delay */
        regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_VCAT;
        result = sysHyPhy20gWrite(fileHndl, regAddr, vcatJ0Delay);
        if (result) return result;
            
            
        /* SONET_SLICE Rx_J0 Delay */
        /* SONET_SLICE Tx_J0 Delay */    
        /* Above 2 delay parameters should be configured as part of SONET LINE init */

        /* Enable DPSF configuration by setting DISABLE to '0' */
        regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_CFG;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_DISABLE_OFF, 
                                0);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, 
                    HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_DISABLE_J0_MSK, 0, 
                    HYPHY20G_DELAY_1250USEC);
        if (result) return result;    
            
        /* set CM_CLEAR to '0' for all 20 DPSF blocks */
        for (i = 0; i<HYPHY20G_TSE_MAX_NUMBER_DPSF_BLOCKS; i++) {
            regAddr = HYPHY20G_TSE_DPSF_N_REG_CFG;
            regAddr = HYPHY20G_TSE_DPSF_N_TSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0x103);
            if (result) return result;
        }
    
        /* Create TSE level context */
        result = tseHyPhy20gTseCreateCtxt(fileHndl);
        if (result) return result;     
                                                                                           
        /* Enable page controller to generate UNEQ-V towards all 40 output 
           ports on both the online and offline pages */
        for (iter = 0; iter < 2; iter++) {
            for (i = 0; i<HYPHY20G_TSE_MAX_NUMBER_DPSF_PORTS; i++) {
                 /* Set up IPORT settings */    
                 result = hyPhy20gVarFieldWrite(&tsePctlIport, 
                                 HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK, 
                                 HYPHY20G_TSE_UNEQ_MASTER_TIMESLOT); 
                 if (result) return result;
         
                 result = hyPhy20gVarFieldWrite(&tsePctlIport, 
                                 HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK, 
                                 HYPHY20G_TSE_DIAG_GENERATOR_PORT); 
                 if (result) return result;
                 for (j = 0; j<HYPHY20G_TSE_MAX_NUMBER_PCTL_TIMESLOTS; j++) {
                     /* Configure OPORT settings */
                     result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, i, j, tsePctlIport);
                     if (result) return result;
                 }
            }
            result = hyPhy20gTsePageSwitch(fileHndl, 
                                               HYPHY20G_TSE_SWITCH_BOTH_GROUPS);
            if (result) return result;
        }
        
        if (enable == 2) {                                                                          
            /* Configure Top Level Low Power bit */                                                  
            regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;                                                
            offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;                                                                     
                                                                                                     
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);                              
            if (result) return result;  
        }                                                                   
    } else {        
        /* Subsystem disabled. Destroy TSE level context */
        result = tseHyPhy20gTseDestroyCtxt(fileHndl);
        if (result) return result;                    
                    
    }               
    return HYPHY20G_SUCCESS;
                    
} /* hyPhy20gTseInit */
                    
                    
/*******************************************************************************
**                  
**  hyPhy20gTseSts192PortMapCfg
**  ___________________________________________________________________________
**                  
**  DESCRIPTION:    This function associates an STS-192 rate data source with
**                  an STS-192 rate destination port.  If a diagnostic source
**                  is specified all STS-1s at the destination port will receive
**                  the diagnostic signal.
**                  
**                  
**  INPUTS:         *fileHndl   - base address
**                  sts192Src   - STS-192 rate source port
**                                0 - SONET_SLICE 0 to 3
**                                1 - SONET_SLICE 4 to 7
**                                2 - SONET_SLICE 8 to 11
**                                3 - SONET_SLICE 12 to 15
**                                4 - HYPHY_HOPP slice 0 to 3
**                                5 - DMI Slice 0 port 0 to 3
**                                6 - DMI Slice 1 port 0 to 3
**                                7 - DMI Slice 2 port 0 to 3
**                                8 - DMI Slice 3 port 0 to 3
**                                9 - DMI Slice 4 port 0 to 3
**                                10 - Diag Pattern - AIS
**                                11 - Diag Pattern - AIS-L
**                                12 - Diag Pattern - UNEQ for all 192 STS-1s of
**                                     a channelized STS-192 signal
**                                13 - Diag Pattern - UNEQ for an STS-192c
**                                     concatenated signal
**                                14 - RESERVED
**                                15 - RESERVED
**                                16 - Diag Source 2 - PRGM
**                  
**                  sts192Dest  - STS-192 rate destination port
**                                0 - DMI Slice 0 port 0 to 3
**                                1 - DMI Slice 1 port 0 to 3
**                                2 - DMI Slice 2 port 0 to 3
**                                3 - DMI Slice 3 port 0 to 3
**                                4 - DMI Slice 4 port 0 to 3
**                                5 - SONET_SLICE 0 to 3
**                                6 - SONET_SLICE 4 to 7
**                                7 - SONET_SLICE 8 to 11
**                                8 - SONET_SLICE 12 to 15
**                                9 - HYPHY_HOPP slice 0 to 3
**                  
**  OUTPUTS:        None.
**                  
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTseSts192PortMapCfg(struct file *fileHndl, UINT4 sts192Src,
                                 UINT4 sts192Dest)
{                   
    UINT4 oPort, iPort;
    UINT4 diagSrc;  
    UINT4 i,j;      
    UINT4 iData = 0;
    UINT4 inPort;   
    UINT4 iTs = 0;  
    UINT4 inSlot;   
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 regAddr;  
    UINT4 offset;   
    INT4 result;    
                    
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_TSE_PORT_CTXT *pPortCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *pTslotCtxt;
                    
    /* argument checking */
    if ((sts192Src > 16)||(sts192Dest > 9)||(sts192Src == 14)||(sts192Src == 15)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }               
                    
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }               
                    
    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;
    HYPHY20G_ASSERT((pTseCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTseCtxt is NULL.");

    /* calculate value of iport and oport base addresses */
    oPort = sts192Dest * 4;
    if (sts192Src <=9 ) {
        iPort = sts192Src * 4;
        diagSrc = 0;
    } else {
        diagSrc = 1;
        if (sts192Src < 16) {
            iPort = HYPHY20G_TSE_DIAG_GENERATOR_PORT;
            iTs = sts192Src%10;
        } else {
            iPort = HYPHY20G_TSE_PRGM_GENERATOR_PORT;
        }
    }

    for (i = 0; i <4; i++) {
        /* configure the four 2.5G ports associated with the 10G interface */
        for (j = 0; j<HYPHY20G_TSE_MAX_NUMBER_PCTL_TIMESLOTS; j++) {
        /* Configure all 48 timeslots within each port */
            /* form a word for Input settings */
            if (diagSrc) {
                result = hyPhy20gVarFieldWrite(&iData,
                           HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                           iPort);
                if(result) return result;
                inPort = iPort;

                if (iPort == HYPHY20G_TSE_DIAG_GENERATOR_PORT) {
                    inSlot = iTs;
                    /* for concatenated payloads AIS-V or UNEQ ensure first
                       timeslot is a master */
                    if ((iTs == 3 || iTs == 5) && i == 0 && j == 0) {
                        inSlot--;
                    }
                    result = hyPhy20gVarFieldWrite(&iData,
                                HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                                inSlot);
                    if(result) return result;
                } else {
                    result = hyPhy20gVarFieldWrite(&iData,
                          HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                          j);
                    if(result) return result;
                    inSlot = j;
                }
            } else {
                result = hyPhy20gVarFieldWrite(&iData,
                           HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                           iPort+i);
                if(result) return result;
                inPort = iPort + i;

                result = hyPhy20gVarFieldWrite(&iData,
                              HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                              j);
                if(result) return result;
                inSlot = j;
            }
            /* call indirect access function */
            result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, oPort + i, j, iData);
            if(result) return result;
            /* update context information */
            pPortCtxt = (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[oPort + i]);
            pTslotCtxt = (sHYPHY20G_TSE_TSLOT_CTXT *)(pPortCtxt->pTseTslotCtxt[j]);
            pTslotCtxt->iPort = inPort;
            pTslotCtxt->iTs = inSlot;
            pTslotCtxt->update = 1;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTseSts192PortMapCfg */


/*******************************************************************************
**
**  hyPhy20gTseSts48PortMapCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function associates an STS-48 rate data source with
**                  an STS-48 rate destination.  The destination may be an
**                  STS-48 rate port or a component of an STS-192 rate port. If
**                  a diagnostic source is specified all STS-1s at the
**                  destination port will receive the diagnostic signal.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Src   - STS-48 rate source
**                                0  to  3 - SONET_SLICE 0 to 3
**                                4  to  7 - SONET_SLICE 4 to 7
**                                8  to 11 - SONET_SLICE 8 to 11
**                                12 to 15 - SONET_SLICE 12 to 15
**                                16 to 19 - HYPHY_HOPP slice 0 to 3
**                                20 to 23 - DMI Slice 0 port 0 to 3
**                                24 to 27 - DMI Slice 1 port 0 to 3
**                                28 to 31 - DMI Slice 2 port 0 to 3
**                                32 to 35 - DMI Slice 3 port 0 to 3
**                                36 to 39 - DMI Slice 4 port 0 to 3
**                                40 - Diag Pattern - AIS
**                                41 - Diag Pattern - AIS-L
**                                42 - Diag Pattern - UNEQ for all 48 STS-1s of
**                                     a channelized STS-48 signal
**                                43 - Diag Pattern - UNEQ for an STS-48c
**                                     concatenated signal
**                                44 - RESERVED
**                                45 - RESERVED
**                                46 - Diagnostic Source 2 - PRGM Port
**
**                  sts48Dest  - STS-48 rate destination
**                                0  to  3 - DMI Slice 0 port 0 to 3
**                                4  to  7 - DMI Slice 1 port 0 to 3
**                                8  to 11 - DMI Slice 2 port 0 to 3
**                                12 to 15 - DMI Slice 3 port 0 to 3
**                                16 to 19 - DMI Slice 4 port 0 to 3
**                                20 to 23 - SONET_SLICE 0 to 3
**                                24 to 27 - SONET_SLICE 4 to 7
**                                28 to 31 - SONET_SLICE 8 to 11
**                                32 to 35 - SONET_SLICE 12 to 15
**                                36 to 39 - HYPHY_HOPP slice 0 to 3
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTseSts48PortMapCfg(struct file *fileHndl, UINT4 sts48Src,
                                UINT4 sts48Dest)
{
    UINT4 j;
    UINT4 iData = 0;
    UINT4 diagSrc = 0;
    UINT4 iPort = 0;
    UINT4 iTs = 0;
    UINT4 inPort;
    UINT4 inSlot;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 regAddr;
    UINT4 offset;
    INT4 result;
    UINT4 devId, revId, status;
	
	sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_TSE_PORT_CTXT *pPortCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *pTslotCtxt;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    

    /* argument checking */
    if ((sts48Src > 46)||(sts48Dest > 39)||(sts48Src == 44)||(sts48Src == 45)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;
    HYPHY20G_ASSERT((pTseCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTseCtxt is NULL.");

    if (sts48Src > 39 ) {
        diagSrc = 1;
        if (sts48Src < 46) {
            iPort = HYPHY20G_TSE_DIAG_GENERATOR_PORT;
            iTs = sts48Src % HYPHY20G_TSE_DIAG_GENERATOR_PORT;
        } else {
            iPort = HYPHY20G_TSE_PRGM_GENERATOR_PORT;
        }
    }


    for (j = 0; j<HYPHY20G_TSE_MAX_NUMBER_PCTL_TIMESLOTS; j++) {
        /* Configure all 48 timeslots within this port */
        /* form a word for Input settings */
        if (diagSrc) {
            result = hyPhy20gVarFieldWrite(&iData,
                       HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                       iPort);
            if(result) return result;
            inPort = iPort;

            if (iPort == HYPHY20G_TSE_DIAG_GENERATOR_PORT) {
                inSlot = iTs;
                /* for concatenated payloads AIS-V or UNEQ ensure first
                   timeslot is a master */
                if ((iTs == 3 || iTs == 5) && j == 0) {
                    inSlot--;
                }
                result = hyPhy20gVarFieldWrite(&iData,
                          HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                          inSlot);
                if(result) return result;

            } else {
                result = hyPhy20gVarFieldWrite(&iData,
                          HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                          j);
                if(result) return result;
                inSlot = j;
            }
        } else {
            result = hyPhy20gVarFieldWrite(&iData,
                               HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                               sts48Src);
            if(result) return result;
            inPort = sts48Src;
            result = hyPhy20gVarFieldWrite(&iData,
                                  HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                                  j);
            if(result) return result;
            inSlot = j;
        }
        /* call indirect access function */
        result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, sts48Dest, j, iData);
        if(result) return result;
        /* update context information */
        pPortCtxt = (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[sts48Dest]);
        pTslotCtxt = (sHYPHY20G_TSE_TSLOT_CTXT *)(pPortCtxt->pTseTslotCtxt[j]);
        pTslotCtxt->iPort = inPort;
        pTslotCtxt->iTs = inSlot;
        pTslotCtxt->update = 1;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTseSts48PortMapCfg */


/*******************************************************************************
**
**  hyPhy20gTseSts24PortMapCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function associates an STS-24 rate data source with
**                  an STS-24 rate destination.  The destination may be
**                  a component of an STS-192, or STS-48. If
**                  a diagnostic source is specified all STS-1s at the
**                  destination port will receive the diagnostic signal.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Src   - STS-48 rate source
**                                0  to  3 - SONET_SLICE 0 to 3
**                                4  to  7 - SONET_SLICE 4 to 7
**                                8  to 11 - SONET_SLICE 8 to 11
**                                12 to 15 - SONET_SLICE 12 to 15
**                                16 to 19 - HYPHY_HOPP slice 0 to 3
**                                20 to 23 - DMI Slice 0 port 0 to 3
**                                24 to 27 - DMI Slice 1 port 0 to 3
**                                28 to 31 - DMI Slice 2 port 0 to 3
**                                32 to 35 - DMI Slice 3 port 0 to 3
**                                36 to 39 - DMI Slice 4 port 0 to 3
**                                40 - Diag Pattern - AIS
**                                41 - Diag Pattern - AIS-L
**                                42 - Diag Pattern - UNEQ for all 24 STS-1s of
**                                     a channelized STS-24 signal
**                                43 - Diag Pattern - UNEQ for an STS-24c
**                                     concatenated signal
**                                44 - RESERVED
**                                45 - RESERVED
**                                46 - Diagnostic Source 2 - PRGM Port
**
**                  sts24Src  - STS-24 channel within sts48Src
**                                0 to 1 - STS-24 channels 1-2
**
**                  sts48Dest  - STS-48 rate destination
**                                0  to  3 - DMI Slice 0 port 0 to 3
**                                4  to  7 - DMI Slice 1 port 0 to 3
**                                8  to 11 - DMI Slice 2 port 0 to 3
**                                12 to 15 - DMI Slice 3 port 0 to 3
**                                16 to 19 - DMI Slice 4 port 0 to 3
**                                20 to 23 - SONET_SLICE 0 to 3
**                                24 to 27 - SONET_SLICE 4 to 7
**                                28 to 31 - SONET_SLICE 8 to 11
**                                32 to 35 - SONET_SLICE 12 to 15
**                                36 to 39 - HYPHY_HOPP slice 0 to 3
**
**                  sts24Dest  - STS-24 channel within sts48Dest
**                                0 to 1 - STS-24 channels 1-2
**
**  COMMENTS:       For sts24 parameters:
**                  STS-24 ch1 = STS-1 tslots 1-8, 17-24, 33-40
**                  STS-24 ch2 = STS-1 tslots 9-16, 25-32, 41-48
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTseSts24PortMapCfg(struct file *fileHndl, UINT4 sts48Src,
                                UINT4 sts24Src, UINT4 sts48Dest, UINT4 sts24Dest)
{
    UINT4 i,j;
    UINT4 iData = 0;
    UINT4 diagSrc = 0;
    UINT4 iPort = 0;
    UINT4 iTs = 0;
    UINT4 sourceSlot;
    UINT4 destSlot;
    UINT4 inPort;
    UINT4 inSlot;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 regAddr;
    UINT4 offset;
    INT4 result;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_TSE_PORT_CTXT *pPortCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *pTslotCtxt;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);


    /* argument checking */
    if ((sts48Src > 46)||(sts24Src > 1)||(sts48Dest > 39)||(sts24Dest > 1)
        ||(sts48Src == 44)||(sts48Src == 45)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;
    HYPHY20G_ASSERT((pTseCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTseCtxt is NULL.");

    if (sts48Src > 39 ) {
        diagSrc = 1;
        if (sts48Src < 46) {
            iPort = HYPHY20G_TSE_DIAG_GENERATOR_PORT;
            iTs = sts48Src%HYPHY20G_TSE_DIAG_GENERATOR_PORT;
        } else {
            iPort = HYPHY20G_TSE_PRGM_GENERATOR_PORT;
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; j++) {
            /* configure 24 timeslots as 3 groups of 8 within 2.5G port
               associated with this STS48 signal  */

            sourceSlot = j + 16*i + 8*sts24Src;
            destSlot = j + 16*i + 8*sts24Dest;

            /* form a word for Input settings */
            if (diagSrc) {
                result = hyPhy20gVarFieldWrite(&iData,
                            HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                            iPort);
                if(result) return result;
                inPort = iPort;

                if (iPort == HYPHY20G_TSE_DIAG_GENERATOR_PORT) {
                    inSlot = iTs;
                    /* for concatenated payloads AIS-V or UNEQ ensure first
                       timeslot is a master */
                    if ((iTs == 3 || iTs == 5) && i == 0 && j == 0) {
                        inSlot--;
                    }
                    result = hyPhy20gVarFieldWrite(&iData,
                               HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                               inSlot);
                    if(result) return result;

                } else {
                    result = hyPhy20gVarFieldWrite(&iData,
                             HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                             sourceSlot);
                    if(result) return result;
                    inSlot = sourceSlot;
                }
            } else {
                result = hyPhy20gVarFieldWrite(&iData,
                                HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                                sts48Src);
                if(result) return result;
                inPort = sts48Src;

                result = hyPhy20gVarFieldWrite(&iData,
                                   HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                                   sourceSlot);
                if(result) return result;
                inSlot = sourceSlot;
            }
            /* call indirect access function */
            result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, sts48Dest, destSlot, iData);
            if(result) return result;
            /* update context information */
            pPortCtxt = (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[sts48Dest]);
            pTslotCtxt = (sHYPHY20G_TSE_TSLOT_CTXT *)(pPortCtxt->pTseTslotCtxt[destSlot]);
            pTslotCtxt->iPort = inPort;
            pTslotCtxt->iTs = inSlot;
            pTslotCtxt->update = 1;
        }
    }
      return HYPHY20G_SUCCESS;
} /*hyPhy20gTseSts24PortMapCfg*/

/*******************************************************************************
**
**  hyPhy20gTseSts12PortMapCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function associates an STS-12 rate data source with
**                  an STS-12 rate destination.  The destination may be an
**                  a component of an STS-192, STS-48 or STS-12 rate port. If
**                  a diagnostic source is specified all STS-1s at the
**                  destination port will receive the diagnostic signal.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Src   - STS-48 rate source
**                                0  to  3 - SONET_SLICE 0 to 3
**                                4  to  7 - SONET_SLICE 4 to 7
**                                8  to 11 - SONET_SLICE 8 to 11
**                                12 to 15 - SONET_SLICE 12 to 15
**                                16 to 19 - HYPHY_HOPP slice 0 to 3
**                                20 to 23 - DMI Slice 0 port 0 to 3
**                                24 to 27 - DMI Slice 1 port 0 to 3
**                                28 to 31 - DMI Slice 2 port 0 to 3
**                                32 to 35 - DMI Slice 3 port 0 to 3
**                                36 to 39 - DMI Slice 4 port 0 to 3
**                                40 - Diag Pattern - AIS
**                                41 - Diag Pattern - AIS-L
**                                42 - Diag Pattern - UNEQ for all 12 STS-1s of
**                                     a channelized STS-12 signal
**                                43 - Diag Pattern - UNEQ for an STS-12c
**                                     concatenated signal
**                                44 - RESERVED
**                                45 - RESERVED
**                                46 - Diagnostic Source 2 - PRGM Port
**
**                  sts12Src  - STS-12 channel within sts48Src
**                                0 to 3 - STS-12 channels 1-4
**
**                  sts48Dest  - STS-48 rate destination
**                                0  to  3 - DMI Slice 0 port 0 to 3
**                                4  to  7 - DMI Slice 1 port 0 to 3
**                                8  to 11 - DMI Slice 2 port 0 to 3
**                                12 to 15 - DMI Slice 3 port 0 to 3
**                                16 to 19 - DMI Slice 4 port 0 to 3
**                                20 to 23 - SONET_SLICE 0 to 3
**                                24 to 27 - SONET_SLICE 4 to 7
**                                28 to 31 - SONET_SLICE 8 to 11
**                                32 to 35 - SONET_SLICE 12 to 15
**                                36 to 39 - HYPHY_HOPP slice 0 to 3
**
**                  sts12Dest  - STS-12 channel within sts48Dest
**                                0 to 3 - STS-12 channels 1-4
**
**  COMMENTS:       For sts12 parameters:
**                  STS-12 ch1 = STS-1 tslots 1,2,3,4,17,18,19,20,33,34,35,36
**                  STS-12 ch2 = STS-1 tlots 5,6,7,8,21,22,23,24,37,38,39,40
**                  STS-12 ch3 = STS-1 tslots 9,10,11,12,25,26,27,28,41,42,43,44
**                  STS-12 ch4 = STS-1 tslots 13,14,15,16,29,30,31,32,45,46,47,48
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTseSts12PortMapCfg(struct file *fileHndl, UINT4 sts48Src,
                                UINT4 sts12Src, UINT4 sts48Dest, UINT4 sts12Dest)
{
    UINT4 i,j;
    UINT4 iData = 0;
    UINT4 diagSrc = 0;
    UINT4 iPort = 0;
    UINT4 iTs = 0;
    UINT4 sourceSlot;
    UINT4 destSlot;
    UINT4 inPort;
    UINT4 inSlot;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 regAddr;
    UINT4 offset;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_TSE_PORT_CTXT *pPortCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *pTslotCtxt;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Src > 46)||(sts12Src > 3)||(sts48Dest > 39)||(sts12Dest > 3)
        ||(sts48Src == 44)||(sts48Src == 45)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;
    HYPHY20G_ASSERT((pTseCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTseCtxt is NULL.");

    if (sts48Src > 39 ) {
        diagSrc = 1;
        if (sts48Src < 46) {
            iPort = HYPHY20G_TSE_DIAG_GENERATOR_PORT;
            iTs = sts48Src%HYPHY20G_TSE_DIAG_GENERATOR_PORT;
        } else {
            iPort = HYPHY20G_TSE_PRGM_GENERATOR_PORT;
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            /* configure 12 timeslots as 3 groups of 4 within 2.5G port
               associated with this STS48 signal  */

            sourceSlot = j + 16*i + 4*sts12Src;
            destSlot = j + 16*i + 4*sts12Dest;

            /* form a word for Input settings */
            if (diagSrc) {
                result = hyPhy20gVarFieldWrite(&iData,
                            HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                            iPort);
                if(result) return result;
                inPort = iPort;

                if (iPort == HYPHY20G_TSE_DIAG_GENERATOR_PORT) {
                    inSlot = iTs;
                    /* for concatenated payloads AIS-V or UNEQ ensure first
                       timeslot is a master */
                    if ((iTs == 3 || iTs == 5) && i == 0 && j == 0) {
                        inSlot--;
                    }
                    result = hyPhy20gVarFieldWrite(&iData,
                               HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                               inSlot);
                    if(result) return result;

                } else {
                    result = hyPhy20gVarFieldWrite(&iData,
                             HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                             sourceSlot);
                    if(result) return result;
                    inSlot = sourceSlot;
                }
            } else {
                result = hyPhy20gVarFieldWrite(&iData,
                                HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                                sts48Src);
                if(result) return result;
                inPort = sts48Src;

                result = hyPhy20gVarFieldWrite(&iData,
                                   HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                                   sourceSlot);
                if(result) return result;
                inSlot = sourceSlot;
            }
            /* call indirect access function */
            result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, sts48Dest, destSlot, iData);
            if(result) return result;
            /* update context information */
            pPortCtxt = (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[sts48Dest]);
            pTslotCtxt = (sHYPHY20G_TSE_TSLOT_CTXT *)(pPortCtxt->pTseTslotCtxt[destSlot]);
            pTslotCtxt->iPort = inPort;
            pTslotCtxt->iTs = inSlot;
            pTslotCtxt->update = 1;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTseSts12PortMapCfg */



/*******************************************************************************
**
**  hyPhy20gTseSts3PortMapCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function associates an STS-3 rate data source with
**                  an STS-3 rate destination.  The destination may be an
**                  a component of an STS-192, STS-48 or STS-12 rate port. If
**                  a diagnostic source is specified all STS-1s at the
**                  destination port will receive the diagnostic signal.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Src   - STS-48 rate source
**                                0  to  3 - SONET_SLICE 0 to 3
**                                4  to  7 - SONET_SLICE 4 to 7
**                                8  to 11 - SONET_SLICE 8 to 11
**                                12 to 15 - SONET_SLICE 12 to 15
**                                16 to 19 - HYPHY_HOPP slice 0 to 3
**                                20 to 23 - DMI Slice 0 port 0 to 3
**                                24 to 27 - DMI Slice 1 port 0 to 3
**                                28 to 31 - DMI Slice 2 port 0 to 3
**                                32 to 35 - DMI Slice 3 port 0 to 3
**                                36 to 39 - DMI Slice 4 port 0 to 3
**                                40 - Diag Pattern - AIS
**                                41 - Diag Pattern - AIS-L
**                                42 - Diag Pattern - UNEQ for all 3 STS-1s of
**                                     a channelized STS-3 signal
**                                43 - Diag Pattern - UNEQ for an STS-3c
**                                     concatenated signal
**                                44 - RESERVED
**                                45 - RESERVED
**                                46 - PRGM Port
**
**                  sts12Src  - STS-12 channel within sts48Src
**                                0 to 3 - STS-12 channels 1-4
**
**                  sts3Src   - STS-3 channel within sts12Src
**                                0 to 3 - STS-3 channels 1-4
**
**                  sts48Dest  - STS-48 rate destination
**                                0  to  3 - DMI Slice 0 port 0 to 3
**                                4  to  7 - DMI Slice 1 port 0 to 3
**                                8  to 11 - DMI Slice 2 port 0 to 3
**                                12 to 15 - DMI Slice 3 port 0 to 3
**                                16 to 19 - DMI Slice 4 port 0 to 3
**                                20 to 23 - SONET_SLICE 0 to 3
**                                24 to 27 - SONET_SLICE 4 to 7
**                                28 to 31 - SONET_SLICE 8 to 11
**                                32 to 35 - SONET_SLICE 12 to 15
**                                36 to 39 - HYPHY_HOPP slice 0 to 3
**
**                  sts12Dest  - STS-12 channel within sts48Dest
**                                0 to 3 - STS-12 channels 1-4
**
**                  sts3Dest   - STS-3 channel within sts12Dest
**                                0 to 3 - STS-3 channels 1-4
**
**  COMMENTS:       For sts12/sts3 parameters:
**                  STS-12 ch1 / sts3 ch1 = STS-1 tslots 1,17,33
**                  STS-12 ch1 / sts3 ch2 = STS-1 tslots 2,18,34
**                  STS-12 ch1 / sts3 ch3 = STS-1 tslots 3,19,35
**                  STS-12 ch1 / sts3 ch4 = STS-1 tslots 4,20,36
**                  STS-12 ch2 / sts3 ch1 = STS-1 tslots 5,21,37
**                  STS-12 ch2 / sts3 ch2 = STS-1 tslots 6,22,38
**                  STS-12 ch2 / sts3 ch3 = STS-1 tslots 7,23,39
**                  STS-12 ch2 / sts3 ch4 = STS-1 tslots 8,24,40
**                  STS-12 ch3 / sts3 ch1 = STS-1 tslots 9,25,41
**                  STS-12 ch3 / sts3 ch2 = STS-1 tslots 10,26,42
**                  STS-12 ch3 / sts3 ch3 = STS-1 tslots 11,27,43
**                  STS-12 ch3 / sts3 ch4 = STS-1 tslots 12,28,44
**                  STS-12 ch4 / sts3 ch1 = STS-1 tslots 13,29,45
**                  STS-12 ch4 / sts3 ch2 = STS-1 tslots 14,30,46
**                  STS-12 ch4 / sts3 ch3 = STS-1 tslots 15,31,47
**                  STS-12 ch4 / sts3 ch4 = STS-1 tslots 16,32,48
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTseSts3PortMapCfg(struct file *fileHndl, UINT4 sts48Src,
                                UINT4 sts12Src, UINT4 sts3Src,
                                UINT4 sts48Dest, UINT4 sts12Dest, UINT4 sts3Dest)
{
    UINT4 j;
    UINT4 iData = 0;
    UINT4 iPort = 0;
    UINT4 iTs = 0;
    UINT4 diagSrc = 0;
    UINT4 sourceSlot;
    UINT4 destSlot;
    UINT4 inPort;
    UINT4 inSlot;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 regAddr;
    UINT4 offset;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_TSE_PORT_CTXT *pPortCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *pTslotCtxt;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Src > 46)||(sts12Src > 3)||(sts3Src > 3)||(sts48Dest > 39)
        ||(sts12Dest > 3)||(sts3Dest > 3)||(sts48Src == 44)||(sts48Src == 45)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;
    HYPHY20G_ASSERT((pTseCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTseCtxt is NULL.");

    if (sts48Src > 39 ) {
        diagSrc = 1;
        if (sts48Src < 46) {
            iPort = HYPHY20G_TSE_DIAG_GENERATOR_PORT;
            iTs = sts48Src%HYPHY20G_TSE_DIAG_GENERATOR_PORT;
        } else {
            iPort = HYPHY20G_TSE_PRGM_GENERATOR_PORT;
        }
    }

    for (j = 0; j < 3; j++) {
        /* configure 3 timeslots within 2.5G port associated
          with this STS48 signal */

        sourceSlot = 16*j + 4*sts12Src + sts3Src;
        destSlot = 16*j + 4*sts12Dest + sts3Dest;

        /* form a word for Input settings */
        if (diagSrc) {
            result = hyPhy20gVarFieldWrite(&iData,
                       HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                       iPort);
            if(result) return result;
            inPort = iPort;

            if (iPort == HYPHY20G_TSE_DIAG_GENERATOR_PORT) {
                inSlot = iTs;
                /* for concatenated payloads AIS-V or UNEQ ensure first
                   timeslot is a master */
                if ((iTs == 3 || iTs == 5) && j == 0) {
                    inSlot--;
                }
                result = hyPhy20gVarFieldWrite(&iData,
                          HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                          inSlot);
                if(result) return result;

            } else {
                result = hyPhy20gVarFieldWrite(&iData,
                          HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                          sourceSlot);
                if(result) return result;
                inSlot = sourceSlot;
            }
        } else {
            result = hyPhy20gVarFieldWrite(&iData,
                             HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                              sts48Src);
            if(result) return result;
            inPort = sts48Src;

            result = hyPhy20gVarFieldWrite(&iData,
                                  HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                                  sourceSlot);
            if(result) return result;
            inSlot = sourceSlot;
        }
        /* call indirect access function */
        result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, sts48Dest, destSlot, iData);
        if(result) return result;
        /* update context information */
        pPortCtxt = (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[sts48Dest]);
        pTslotCtxt = (sHYPHY20G_TSE_TSLOT_CTXT *)(pPortCtxt->pTseTslotCtxt[destSlot]);
        pTslotCtxt->iPort = inPort;
        pTslotCtxt->iTs = inSlot;
        pTslotCtxt->update = 1;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTseSts3PortMapCfg */



/*******************************************************************************
**
**  hyPhy20gTseSts1PortMapCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function associates an STS-1 rate data source with
**                  an STS-1 rate destination.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Src   - STS-48 rate source
**                                0  to  3 - SONET_SLICE 0 to 3
**                                4  to  7 - SONET_SLICE 4 to 7
**                                8  to 11 - SONET_SLICE 8 to 11
**                                12 to 15 - SONET_SLICE 12 to 15
**                                16 to 19 - HYPHY_HOPP slice 0 to 3
**                                20 to 23 - DMI Slice 0 port 0 to 3
**                                24 to 27 - DMI Slice 1 port 0 to 3
**                                28 to 31 - DMI Slice 2 port 0 to 3
**                                32 to 35 - DMI Slice 3 port 0 to 3
**                                36 to 39 - DMI Slice 4 port 0 to 3
**                                40 - Diag Pattern - AIS
**                                41 - Diag Pattern - AIS-L
**                                42 - Diag Pattern - UNEQ-M
**                                43 - Diag Pattern - UNEQ-S
**                                44 - RESERVED
**                                45 - RESERVED
**                                46 - Diagnostic Source 2 - PRGM Port
**
**                  sts12Src  - STS-12 channel within sts48Src
**                                0 to 3 - STS-12 channels 1-4
**
**                  sts3Src   - STS-3 channel within sts12Src
**                                0 to 3 - STS-3 channels 1-4
**
**                  sts1Src   - STS-1 channel within sts3Src
**                                0 to 2 - STS-1 channels 1-3
**
**                  sts48Dest  - STS-48 rate destination
**                                0  to  3 - DMI Slice 0 port 0 to 3
**                                4  to  7 - DMI Slice 1 port 0 to 3
**                                8  to 11 - DMI Slice 2 port 0 to 3
**                                12 to 15 - DMI Slice 3 port 0 to 3
**                                16 to 19 - DMI Slice 4 port 0 to 3
**                                20 to 23 - SONET_SLICE 0 to 3
**                                24 to 27 - SONET_SLICE 4 to 7
**                                28 to 31 - SONET_SLICE 8 to 11
**                                32 to 35 - SONET_SLICE 12 to 15
**                                36 to 39 - HYPHY_HOPP slice 0 to 3
**
**                  sts12Dest  - STS-12 channel within sts48Dest
**                                0 to 3 - STS-12 channels 1-4
**
**                  sts3Dest   - STS-3 channel within sts12Dest
**                                0 to 3 - STS-3 channels 1-4
**
**                  sts1Dest   - STS-1 channel within sts3Dest
**                                0 to 2 - STS-1 channels 1-3
**
**  COMMENTS:       For sts12/sts3/sts1 parameters:
**                  STS-12 ch1 / sts3 ch1 / sts1 ch 1,2,3 = STS-1 tslots 1,17,33
**                  STS-12 ch1 / sts3 ch2 / sts1 ch 1,2,3 = STS-1 tslots 2,18,34
**                  STS-12 ch1 / sts3 ch3 / sts1 ch 1,2,3 = STS-1 tslots 3,19,35
**                  STS-12 ch1 / sts3 ch4 / sts1 ch 1,2,3 = STS-1 tslots 4,20,36
**                  STS-12 ch2 / sts3 ch1 / sts1 ch 1,2,3 = STS-1 tslots 5,21,37
**                  STS-12 ch2 / sts3 ch2 / sts1 ch 1,2,3 = STS-1 tslots 6,22,38
**                  STS-12 ch2 / sts3 ch3 / sts1 ch 1,2,3 = STS-1 tslots 7,23,39
**                  STS-12 ch2 / sts3 ch4 / sts1 ch 1,2,3 = STS-1 tslots 8,24,40
**                  STS-12 ch3 / sts3 ch1 / sts1 ch 1,2,3 = STS-1 tslots 9,25,41
**                  STS-12 ch3 / sts3 ch2 / sts1 ch 1,2,3 = STS-1 tslots 10,26,42
**                  STS-12 ch3 / sts3 ch3 / sts1 ch 1,2,3 = STS-1 tslots 11,27,43
**                  STS-12 ch3 / sts3 ch4 / sts1 ch 1,2,3 = STS-1 tslots 12,28,44
**                  STS-12 ch4 / sts3 ch1 / sts1 ch 1,2,3 = STS-1 tslots 13,29,45
**                  STS-12 ch4 / sts3 ch2 / sts1 ch 1,2,3 = STS-1 tslots 14,30,46
**                  STS-12 ch4 / sts3 ch3 / sts1 ch 1,2,3 = STS-1 tslots 15,31,47
**                  STS-12 ch4 / sts3 ch4 / sts1 ch 1,2,3 = STS-1 tslots 16,32,48
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTseSts1PortMapCfg(struct file *fileHndl, UINT4 sts48Src,
                               UINT4 sts12Src, UINT4 sts3Src, UINT4 sts1Src,
                               UINT4 sts48Dest, UINT4 sts12Dest, UINT4 sts3Dest,
                               UINT4 sts1Dest)
{
    UINT4 iData = 0;
    UINT4 diagSrc = 0;
    UINT4 iPort = 0;
    UINT4 iTs = 0;
    UINT4 sourceSlot;
    UINT4 destSlot;
    UINT4 inPort;
    UINT4 inSlot;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 regAddr;
    UINT4 offset;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_TSE_PORT_CTXT *pPortCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *pTslotCtxt;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Src > 46)||(sts12Src > 3)||(sts3Src > 3)||(sts1Src > 2)
        ||(sts48Dest > 39)||(sts12Dest > 3)||(sts3Dest > 3)||(sts1Dest > 2)
        ||(sts48Src == 44)||(sts48Src == 45)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;
    HYPHY20G_ASSERT((pTseCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTseCtxt is NULL.");

    if (sts48Src > 39 ) {
        diagSrc = 1;
        if (sts48Src < 46) {
            iPort = HYPHY20G_TSE_DIAG_GENERATOR_PORT;
            iTs = sts48Src%HYPHY20G_TSE_DIAG_GENERATOR_PORT;
        } else {
            iPort = HYPHY20G_TSE_PRGM_GENERATOR_PORT;
        }
    }

    /* configure 1 timeslot within 2.5G port associated
       with this STS48 signal */
    /* form a word for Input settings */
    sourceSlot = 16*sts1Src + 4*sts12Src + sts3Src;
    destSlot = 16*sts1Dest + 4*sts12Dest + sts3Dest;

    if (diagSrc) {
        result = hyPhy20gVarFieldWrite(&iData,
                   HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                   iPort);
        if(result) return result;
        inPort = iPort;

        if (iPort == HYPHY20G_TSE_DIAG_GENERATOR_PORT) {
            result = hyPhy20gVarFieldWrite(&iData,
                        HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                        iTs);
            if(result) return result;
            inSlot = iTs;

        } else {
            result = hyPhy20gVarFieldWrite(&iData,
                      HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                      sourceSlot);
            if(result) return result;
            inSlot = sourceSlot;
        }
    } else {
        result = hyPhy20gVarFieldWrite(&iData,
                       HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                       sts48Src);
        if(result) return result;
        inPort = sts48Src;

        result = hyPhy20gVarFieldWrite(&iData,
                          HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                          sourceSlot);
        if(result) return result;
        inSlot = sourceSlot;
    }
    /* call indirect access function */
    result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, sts48Dest, destSlot, iData);
    if(result) return result;
    /* update context information */
    pPortCtxt = (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[sts48Dest]);
    pTslotCtxt = (sHYPHY20G_TSE_TSLOT_CTXT *)(pPortCtxt->pTseTslotCtxt[destSlot]);
    pTslotCtxt->iPort = inPort;
    pTslotCtxt->iTs = inSlot;
    pTslotCtxt->update = 1;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTseSts1PortMapCfg */

/*******************************************************************************
**
**  hyPhy20gTsePageSwitch
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function switches the online and offline switch
**                  context for TSE switching group 0, group 1 or both.
**
**                  Note: This functionality may also be implemented in systems
**                        using the external TSE_CMPS[1:0] device pins.
**
**                        These mechanisms should be used mutually exclusively.
**                        However this function may be used during system
**                        initialization till this control is passed over to
**                        TSE_CMPS[1:0] device pins.
**
**  INPUTS:         *fileHndl  - base address
**                  group      - Specifies the group or groups that will be
**                               switched:
**                               0 - group 0
**                               1 - group 1
**                               2 - group 0 and group 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePageSwitch(struct file *fileHndl, UINT4 group)
{
    UINT4 regAddr;
    UINT4 cmpGrp0 = 0;
    UINT4 cmpGrp1 = 0;
    UINT4 startPort = 0;
    UINT4 stopPort = 0;
    UINT4 i, j;
    UINT4 iData = 0;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 offset;
    INT4 result;
	UINT4 TmpReg;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_TSE_PORT_CTXT *pPortCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *pTslotCtxt;


    /* argument checking */
    if (group > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to TSE level context information*/
    pTseCtxt = pDevCtxt->pTseCtxt;
    HYPHY20G_ASSERT((pTseCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTseCtxt is NULL.");

    regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP0_OFF,
                        &cmpGrp0);
    if (result) return result;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP1_OFF,
                        &cmpGrp1);
    if (result) return result;

    /* Bit 2: the WCIMODE_TSE bit: 1, write-clear; 0, read-clear*/
    regAddr = HYPHY20G_PGMRCLK_REG_WCIMODE_CTL_3;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, 
		        HYPHY20G_PGMRCLK_BIT_WCIMODE_CTL_3_WCI_MODE_CTRL_OFF+2, &TmpReg);
    if (result) return result;

    if(TmpReg != 0){
	    /* For group 0. Write 1 to clear the page change status bit */
	    if(((group+1)&1) == 1) {
	        regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
	        result = sysHyPhy20gBitWrite(fileHndl, regAddr, 
				        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP0_CHG_IC_OFF,1);
	        if (result) return result;
	    }

		/* For group 1.   */
		if(((group+1)&2) == 2){
	        regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
	        result = sysHyPhy20gBitWrite(fileHndl, regAddr, 
				        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP1_CHG_IC_OFF,1);
	        if (result) return result;
		}
    } else { /* read-clear bit*/
        regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
        result = sysHyPhy20gRead(fileHndl, regAddr,&TmpReg);
        if (result) return result;
	}

    switch (group) {
        case 0:
            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP0_CHG_IC_OFF,1);
            if (result) return result;

            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_CFG;
            if (cmpGrp0) {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP0_OFF,
                                0);
                if (result) return result;
            } else {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP0_OFF,
                                1);
                if (result) return result;
            }

            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gPollBit(fileHndl, regAddr,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP0_CHG_IC_MSK,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP0_CHG_IC_MSK,
                        HYPHY20G_DELAY_1250USEC);
            if (result) return result;
            startPort = 0;
            stopPort = 19;
            break;

        case 1:
            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP1_CHG_IC_OFF,1);
            if (result) return result;
            
            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_CFG;
            if (cmpGrp1) {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP1_OFF,
                                0);
                if (result) return result;
            } else {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP1_OFF,
                                1);
                if (result) return result;
            }

            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gPollBit(fileHndl, regAddr,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP1_CHG_IC_MSK,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP1_CHG_IC_MSK,
                        HYPHY20G_DELAY_1250USEC);
            if (result) return result;
            startPort = 20;
            stopPort = 39;
            break;

        case 2:
            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP0_CHG_IC_OFF,1);
            if (result) return result;
            
            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_CFG;
            if (cmpGrp0) {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP0_OFF,
                                0);
                if (result) return result;
            } else {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP0_OFF,
                                1);
                if (result) return result;
            }

            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gPollBit(fileHndl, regAddr,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP0_CHG_IC_MSK,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP0_CHG_IC_MSK,
                        HYPHY20G_DELAY_1250USEC);
            if (result) return result;

            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP1_CHG_IC_OFF,1);
            if (result) return result;
            
            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_CFG;
            if (cmpGrp1) {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP1_OFF,
                                0);
                if (result) return result;
            } else {
                result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_TSE_PCTL_BIT_PG_CTLR_CFG_CMP_GRP1_OFF,
                                1);
                if (result) return result;
            }

            regAddr = HYPHY20G_TSE_PCTL_REG_PG_CTLR_INT;
            result = sysHyPhy20gPollBit(fileHndl, regAddr,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP1_CHG_IC_MSK,
                        HYPHY20G_TSE_PCTL_BIT_PG_CTLR_INT_CMP_GRP1_CHG_IC_MSK,
                        HYPHY20G_DELAY_1250USEC);
            if (result) return result;
            startPort = 0;
            stopPort = 39;
            break;
    }

    /* update offline page to be in sync with online page */
    for (i = startPort; i<=stopPort; i++) {
        pPortCtxt = (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[i]);
        for (j = 0; j<HYPHY20G_TSE_MAX_NUM_TSLOTS; j++) {
            pTslotCtxt = (sHYPHY20G_TSE_TSLOT_CTXT *)(pPortCtxt->pTseTslotCtxt[j]);
            if (pTslotCtxt->update == 1) {
                iData = 0;
                result = hyPhy20gVarFieldWrite(&iData,
                             HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_IPORT_MSK,
                             pTslotCtxt->iPort);
                if(result) return result;

                result = hyPhy20gVarFieldWrite(&iData,
                                HYPHY20G_TSE_PCTL_BIT_IND_CFG_MEM_DAT_ITS_MSK,
                                pTslotCtxt->iTs);
                if(result) return result;

                /* call indirect access function */
                result = tseHyPhy20gPCTLCfgMemIndWrite(fileHndl, i, j, iData);
                if(result) return result;
                pTslotCtxt->update = 0;
            }
        }
    }
  return HYPHY20G_SUCCESS;

} /*hyPhy20gTsePageSwitch*/

/*******************************************************************************
**
**  hyPhy20gTse2x50SetJ0Delays
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the J0 delay offsets within the TSE
**                  for operation in 2x50G switching mode.
**
**                  Note: This function should not be called until all active DMI
**                  and ESSI links have been configured and hyPhy20gEssiReAlign
**                  has been called for all active ESSI.
**
**
**  INPUTS:         *fileHndl  - base address
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTse2x50SetJ0Delays(struct file *fileHndl)
{
    UINT4 regAddr;
    UINT4 minDly = 0xFFFF;
    UINT4 maxDly = 0;
    UINT4 bypassMatrix = 0;
    UINT4 bypass = 0;
    UINT4 protect = 0;
    UINT4 protectMatrix = 0;
    UINT4 tseTxRxJ0Dly = 0;
    UINT4 delayToUse;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 mask;
    UINT4 offset;
    UINT4 essiJ0DlyVal = 0;
    UINT4 essiJ0Dly;
    UINT4 oofEssi;
    UINT4 i;
    INT4 result;

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Compile list of which DMI links are in bypass mode */
    for (i=0; i<4; i++) {
        regAddr = HYPHY20G_DMI_DMI_REG_LNK_BYP;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, i);
        result = sysHyPhy20gRead(fileHndl, regAddr, &bypass);
        if (result) return result;

        bypassMatrix = bypassMatrix + (bypass << 4*i);
    }

    /* Compile list of which DMI links are in working or protect mode */
    for (i=0; i<4; i++) {
        regAddr = HYPHY20G_DMI_DMI_REG_WK_PROT;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, i);
        result = sysHyPhy20gRead(fileHndl, regAddr, &protect);
        if (result) return result;

        protectMatrix = protectMatrix + (protect << 4*i);
    }

    for (i=0; i<16; i++) {
        mask = 0x1 << i;
        if (mHYPHY20G_GET_FIELD4(bypassMatrix, mask, i) == 1) {
            /* Link is in Bypass mode - Include in calculations */
            if (mHYPHY20G_GET_FIELD4(protectMatrix, mask, i) == 1) {
                /* Working link is being used.  Check framing status */
                regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
                regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, 2*(i/8));
                regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, i/8);
                offset = HYPHY20G_ESSI_RSDM_BIT_STAT_OOF_ESSI_V_OFF;
                result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &oofEssi);
                if (result) return result;
                if (oofEssi == 0) {
                    /* Link is in frame.  Check ESSI_J0DLY value in TSE */
                    regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_ESSI_OFFSET(i);
                    result = sysHyPhy20gRead(fileHndl, regAddr, &essiJ0Dly);
                    if (result) return result;
                    mask = HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_ESSI_ESSI_J0DLY_MSK;
                    offset = HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_ESSI_ESSI_J0DLY_OFF;
                    essiJ0DlyVal = mHYPHY20G_GET_FIELD4(essiJ0Dly, mask, offset);

                }
            } else {
                /* Protect link is being used.  Check framing status */
                regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
                regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, (2*(i/8)) + 1);
                regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, i/8);
                offset = HYPHY20G_ESSI_RSDM_BIT_STAT_OOF_ESSI_V_OFF;
                result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &oofEssi);
                if (result) return result;
                if (oofEssi == 0) {
                    /* Link is in frame.  Check ESSI_J0DLY value in TSE */
                    regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_ESSI_OFFSET(16 + i);
                    result = sysHyPhy20gRead(fileHndl, regAddr, &essiJ0Dly);
                    if (result) return result;
                    mask = HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_ESSI_ESSI_J0DLY_MSK;
                    offset = HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_ESSI_ESSI_J0DLY_OFF;
                    essiJ0DlyVal = mHYPHY20G_GET_FIELD4(essiJ0Dly, mask, offset);
                }
            }

            /* Check for smallest or largest delay */
            if (essiJ0DlyVal >  maxDly) {
                maxDly = essiJ0DlyVal;
            }
            if (essiJ0DlyVal < minDly) {
                minDly = essiJ0DlyVal;
            }
        }
    }

    delayToUse = (maxDly + minDly) / 2;

    for (i=0; i<16; i++) {
        mask = 0x1 << i;
        if (mHYPHY20G_GET_FIELD4(bypassMatrix, mask, i) == 1) {
            /* Configure ESSI_J0DLY value in TSE */
            regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_ESSI_OFFSET(i);
            result = sysHyPhy20gWrite(fileHndl, regAddr, delayToUse);
            if (result) return result;
            regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_ESSI_OFFSET(i+16);
            result = sysHyPhy20gWrite(fileHndl, regAddr, delayToUse);
            if (result) return result;
        }
    }

    regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR;
    result = sysHyPhy20gRead(fileHndl, regAddr, &tseTxRxJ0Dly);
    result = hyPhy20gVarFieldWrite(&tseTxRxJ0Dly,
             HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_TSE_TX_J0DLY_MSK,
             delayToUse+10);
    if (result) return result;
    result = sysHyPhy20gWrite(fileHndl, regAddr, tseTxRxJ0Dly);
    if (result) return result;

  return HYPHY20G_SUCCESS;

} /* hyPhy20gTse2x50SetJ0Delays */
/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/*******************************************************************************
**
**  tseHyPhy20gTseCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates memory for tracking TSE page memory context.
**
**  INPUTS:         fileHndl        - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gTseCreateCtxt(struct file *fileHndl)
{
    UINT4 size;
    UINT4 i;
    INT4 result;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* allocate memory to TSE level context information */
    /* if structure already exists destroy previous to prevent memory
       leak */
    if (pDevCtxt->pTseCtxt != NULL){
        DBG_PRINT("Existing TSE Context memory detected, destroy it before allocation.\n");
        tseHyPhy20gTseDestroyCtxt(fileHndl);
    }

    /* allocate memory for TSE Top Level Context*/
    size = sizeof(sHYPHY20G_TSE_CTXT);
    pDevCtxt->pTseCtxt = (sHYPHY20G_TSE_CTXT *)sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pTseCtxt == NULL) {
        DBG_PRINT("file %s, line %d, - HYPHY20G_ERR_MEM_ALLOC, size=%ld\n",
                  __FILE__, __LINE__, (long int)size);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    DBG_PRINT("pDevCtxt->pTseCtxt Context memory allocated:addr=0x%x size=%ld\n",
                  (unsigned int)pDevCtxt->pTseCtxt, size); 
    
    pTseCtxt = pDevCtxt->pTseCtxt;
    /* allocate memory for each pointer array within the data structure
    ** memory organized:
    **    sHYPHY20G_TSE_CTXT *pTseCtxt
    **          sHYPHY20G_TSE_PORT_CTXT      *pTsePortCtxt[40]
    **                  sHYPHY20G_TSE_TSLOT_CTXT      *pTseTslotCtxt[48];
    **/
    size = HYPHY20G_TSE_MAX_NUM_OPORTS * (sizeof (sHYPHY20G_TSE_PORT_CTXT));
    pTseCtxt->pTsePortCtxt[0] = 
                           (sHYPHY20G_TSE_PORT_CTXT *)sysHyPhy20gMemAlloc(size);
    
    if (pTseCtxt->pTsePortCtxt[0]==NULL) {
    /* deallocate memory allocated up to this point in function */
        DBG_PRINT("file %s, line %d, - HYPHY20G_ERR_MEM_ALLOC, size=%ld\n",
                  __FILE__, __LINE__, (long int)size);
        sysHyPhy20gMemFree(pDevCtxt->pTseCtxt);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    DBG_PRINT("memory allocated for pTseCtxt->pTsePortCtxt[0] :addr=0x%x size=%ld\n",
                  (unsigned int)pTseCtxt->pTsePortCtxt[0], size);
    
    /* Assign addr to pointer array pTsePortCtxt[i] */
    for (i = 1; i<HYPHY20G_TSE_MAX_NUM_OPORTS; i++) {
        pTseCtxt->pTsePortCtxt[i] = 
        (sHYPHY20G_TSE_PORT_CTXT *)(pTseCtxt->pTsePortCtxt[i-1] + 1);
    }
    DBG_PRINT("Assigned pTseCtxt->pTsePortCtxt[39] to :addr=0x%x size=%ld\n",
              (unsigned int)pTseCtxt->pTsePortCtxt[39], (long int)sizeof(sHYPHY20G_TSE_PORT_CTXT));
    
    /* Allocate the TSE Timeslot Memory Context */
    result = tseHyPhy20gTseCreateTslotCtxt(fileHndl);
    if (result) {
        sysHyPhy20gMemFree(pDevCtxt->pTseCtxt->pTsePortCtxt[0]);
        sysHyPhy20gMemFree(pDevCtxt->pTseCtxt);
        return result;
    }     
  
    return HYPHY20G_SUCCESS;

} /* tseHyPhy20gTseCreateCtxt */


/*******************************************************************************
**
**  tseHyPhy20gTseCreateTslotCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and initializes the TSE Timeslot Memory Context 
**                  for the online and offline page memories.
**
**  INPUTS:         fileHndl  - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gTseCreateTslotCtxt(struct file *fileHndl)
{
    UINT4 size;
    UINT4 i, j;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseFullCtxt;
    sHYPHY20G_TSE_TSLOT_CTXT *p;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);

    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pTseFullCtxt = pDevCtxt->pTseCtxt;
    /* confirm all pointers that reference memory allocated by this function are 
       not NULL */
    for (i=0; i<HYPHY20G_TSE_MAX_NUM_OPORTS; i++) {
        if (pTseFullCtxt->pTsePortCtxt[i] == NULL) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }
    /* Allocate memory for sHYPHY20G_TSE_TSLOT_CTXT storage to 
       pTsePortCtxt[0]->pTseTslotCtxt[0] */
    size = HYPHY20G_TSE_MAX_NUM_OPORTS * HYPHY20G_TSE_MAX_NUM_TSLOTS * 
           sizeof(sHYPHY20G_TSE_TSLOT_CTXT);
    pTseFullCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0] = 
                          (sHYPHY20G_TSE_TSLOT_CTXT *)sysHyPhy20gMemAlloc(size);
    
    if (pTseFullCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0] == NULL) {
        DBG_PRINT("file %s, line %d, - HYPHY20G_ERR_MEM_ALLOC, size=%ld\n",__FILE__, __LINE__, (long int)size);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    DBG_PRINT("pTsePortCtxt[0]->pTseTslotCtxt[0] Context memory allocated:addr=0x%x size=%ld\n",
              (unsigned int)pTseFullCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0], size);
    
    /* Assign addr to pointer array, and initialize memory for Tse Timeslot Memory */
    p = pTseFullCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0];
   
    for(i=0; i<HYPHY20G_TSE_MAX_NUM_OPORTS; i++) {
        for (j=0; j<HYPHY20G_TSE_MAX_NUM_TSLOTS; j++) {
            pTseFullCtxt->pTsePortCtxt[i]->pTseTslotCtxt[j] = p;
            p->iPort = HYPHY20G_TSE_NULL_IPORT;
            p->iTs = HYPHY20G_TSE_NULL_ITS;
            p->update = 0;
            p++;
        }
    }
    DBG_PRINT("Assigned pTseFullCtxt->pTsePortCtxt[39]->pTseTslotCtxt[47] to :addr=0x%x size=%ld\n",
              (unsigned int)pTseFullCtxt->pTsePortCtxt[39]->pTseTslotCtxt[47], (long int)sizeof(sHYPHY20G_TSE_TSLOT_CTXT));
    
    return HYPHY20G_SUCCESS;

} /* tseHyPhy20gTseCreateTslotCtxt */

/*******************************************************************************
**
**  tseHyPhy20gTseDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the TSE subsystem.  Deallocates memory
**                  used for storing this information.
**
**
**  INPUTS:         fileHndl        - device handle
**                  *pTseCtxt       - pointer to Tse Context
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gTseDestroyCtxt(struct file *fileHndl)
{
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pTseCtxt = pDevCtxt->pTseCtxt;

    if(pTseCtxt == NULL){
      /* Nothing to destroy */
      return HYPHY20G_SUCCESS;
    }
    
    /* deallocate memory associated with Tse Timeslot Memory */
    tseHyPhy20gTseDestroyTslotCtxt (fileHndl);
    
    /* deallocate memory associated with TSE structure */
    DBG_PRINT("pDevCtxt->pTseCtxt->pTsePortCtxt[0] will be deallocated :addr=0x%x\n", (unsigned int)pDevCtxt->pTseCtxt->pTsePortCtxt[0]);
    sysHyPhy20gMemFree(pDevCtxt->pTseCtxt->pTsePortCtxt[0]);
    DBG_PRINT("pDevCtxt->pTseCtxt->pTsePortCtxt[0] , Deallocated!\n");
    
    DBG_PRINT("pDevCtxt->pTseCtxt will be deallocated :addr=0x%x\n", (unsigned int)pDevCtxt->pTseCtxt);
    sysHyPhy20gMemFree(pDevCtxt->pTseCtxt);
    DBG_PRINT("pDevCtxt->pTseCtxt , Deallocated!\n");

    return HYPHY20G_SUCCESS;

} /* tseHyPhy20gTseDestroyCtxt */

/********************************************************************************
**
**  tseHyPhy20gTseDestroyTslotCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset Port context of the Tse Timeslot Memory .  Deallocates
**                  memory used for storing this information.
**
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 tseHyPhy20gTseDestroyTslotCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    
    DBG_PRINT("pDevCtxt->pTseCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0] will be deallocated :addr=0x%x\n",(unsigned int) pDevCtxt->pTseCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0]);
    sysHyPhy20gMemFree (pDevCtxt->pTseCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0]);
    DBG_PRINT("pDevCtxt->pTseCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0] , Deallocated!\n");
 
    return HYPHY20G_SUCCESS;
    
} /* tseHyPhy20gTseDestroyTslotCtxt */

/*******************************************************************************
**
**  tseHyPhy20gTseGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of TSE context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of TSE context
**
*******************************************************************************/
UINT4 tseHyPhy20gTseGetCtxtSize(struct file *fileHndl)
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

    if (pDevCtxt->pTseCtxt != NULL) {
        /* calculate the TSE context size */
        /* do not include the first/second level pointers values */
        /* calculate the total sHYPHY20G_TSE_TSLOT_CTXT size */
        size += HYPHY20G_TSE_MAX_NUM_OPORTS * 
                HYPHY20G_TSE_MAX_NUM_TSLOTS * 
                sizeof(sHYPHY20G_TSE_TSLOT_CTXT);
    }
    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* tseHyPhy20gTseGetCtxtSize */

/*******************************************************************************
**
**  tseHyPhy20gTseExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the TSE context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for TSE context extraction
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
INT4 tseHyPhy20gTseExtractCtxt(struct file *fileHndl, 
                            void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    UINT4 rtLen;
    UINT4 offset = 0, payloadLen = 0, len = 0;

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

    pTseCtxt = pDevCtxt->pTseCtxt;

    DBG_PRINT("%s, %s, %d, header offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);
    
    payloadLen = HYPHY20G_TSE_MAX_NUM_OPORTS * 
                HYPHY20G_TSE_MAX_NUM_TSLOTS * 
                sizeof(sHYPHY20G_TSE_TSLOT_CTXT);
    
    len = (pTseCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
            (sizeof(sHYPHY20G_CTXT_HDR) + payloadLen);

    /* insert ctxt buffer header */
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_TSE_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);
    
    if (pTseCtxt != NULL) {
        /* copy the context into the buffer */
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                (void *)(pTseCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0]), payloadLen);    
        offset += payloadLen;
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;

    return HYPHY20G_SUCCESS;
}/* tseHyPhy20gTseExtractCtxt */

/*******************************************************************************
**
**  tseHyPhy20gTseRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the TSE context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored TSE context for restoration
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
INT4 tseHyPhy20gTseRestoreCtxt(struct file *fileHndl, 
                            void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_TSE_CTXT *pTseCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the tse header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_TSE_CTXT_ID, HYPHY_NO_INDEX);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_TSE_CTXT_ID hdr\n", 
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
        /* create TSE context */
        result = tseHyPhy20gTseCreateCtxt(fileHndl);
        if (result) {
            DBG_PRINT("%s, %s, %d, tseHyPhy20gTseCreateCtxt failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return result;
        }

        /* retrieve device context */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pDevCtxt is NULL.");

        pTseCtxt = pDevCtxt->pTseCtxt;

        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        
        /* payload len */
        /* do not include the first/second level pointers values */
        /* restore the total sHYPHY20G_TSE_TSLOT_CTXT */
        len = HYPHY20G_TSE_MAX_NUM_OPORTS * 
              HYPHY20G_TSE_MAX_NUM_TSLOTS * 
              sizeof(sHYPHY20G_TSE_TSLOT_CTXT);
        sysHyPhy20gMemCpy((void *)(pTseCtxt->pTsePortCtxt[0]->pTseTslotCtxt[0]), 
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
}/* tseHyPhy20gTseRestoreCtxt */


/********************************************************************************
**
**  tseHyPhy20gTseAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function audit the TSE subCTXT.
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
INT4 tseHyPhy20gTseAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result, i, j;
    UINT4 regAddr, offset, rstBit, lowPwrBit;
    sHYPHY20G_TSE_CTXT *pTseFullCtxt;
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt == NULL."); 

    /* The lowPwrBit reflect whether TSE module is enabled or not, 1: disable/lowpwr mode|0: enable */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_TSE_LOWPWR_OFF;
    
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &lowPwrBit);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gBitRead fail."); 
    
    if (lowPwrBit == 1)
    {
        return HYPHY20G_SUCCESS;
    }
    
    /* The TSE_RST reflect whether TSE module is enabled or not, 1: disable|0: enable */

    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_TSE_RST_OFF;
    
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rstBit);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gBitRead fail."); 

    if (rstBit == 1)
    {
        if (pDevCtxt->pTseCtxt!=NULL)
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        else
            return HYPHY20G_SUCCESS;
    }
    
    if (pDevCtxt->pTseCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }
    
    pTseFullCtxt = pDevCtxt->pTseCtxt;
    for(i=0; i<HYPHY20G_TSE_MAX_NUM_OPORTS; i++) 
    {
        if (pTseFullCtxt->pTsePortCtxt[i]==NULL)
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        for (j=0; j<HYPHY20G_TSE_MAX_NUM_TSLOTS; j++) 
        {
            if (pTseFullCtxt->pTsePortCtxt[i]->pTseTslotCtxt[j]==NULL)
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
        }
    }        
    
    return HYPHY20G_SUCCESS;
    
} /* tseHyPhy20gTseAuditCtxt */

/* end of file */
