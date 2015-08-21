/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 * File Name  		: hyphy20g_wrapper.c
 * Description		: wrapper file for hyphy20g drivers
 *******************************************************************************/
#include <linux/pci.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif

#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h> 

#include "hyphy20g.h"
#include "hyphy20g_pcie.h"
#include "hyphy20g_dcsu.h"
#include "hyphy20g_sonet.h"
#include "hyphy20g_api.h"



#define DRV_FIRMWARE_FILE		"/etc/sysconfig/firmware.mem"
#define DRV_CONF_FILE			"/etc/sysconfig/qnf_mc20p.conf"


// change the default settings here
static int mc20p_lineType 			= 0;  		// 10GE LAN
static int mc20p_sonetScram 		= 1;  		// SONET SCRAMBLE
static int mc20p_payloadScram 	= 0; 		// PAYLOAD SCRAMBLE
static int mc20p_fcsMode 			= 1;  		// CRC32
static int mc20p_minPkt 			= 64;		// MIN Packet Len
static int mc20p_maxPkt 			= 1518;		// MAX Packet Len


// external functions
extern INT4 hyPhy20gPifSerdesDiagLpbk(struct file *fileHndl, UINT4 sliceMsk, 
	UINT4 enblMsk);
extern INT4 hyPhy20gGfpHdlcPrependCfg(struct file *fileHndl, UINT4 slice, 
	UINT4 cpbId, UINT4 hdrLength, UINT4 hdrData[4], UINT4 enable);
extern INT4 hyPhy20gSonetPohPslPdiCfg(struct file *fileHndl, UINT4 slice, 
	UINT4 sts12, UINT4 sts3, UINT4 sts1, UINT4 pslValue, UINT4 pdiRangeEn, UINT4 pdiVal);
extern INT4 hyPhy20gGfpRxPmonGrpFrmCntrs(struct file *fileHndl, UINT4 slice, UINT4 grpId, 
	UINT4 *rxByteCntMsb, UINT4 *rxByteCntLsb, UINT4 *rxCtrlFrmCnt, 
	UINT4 *rxFrmCnt, UINT4 *rxIdleFrmCnt);


#define DA(MSG)		printk MSG

/*******************************************************************************
 First step init function
*******************************************************************************/
INT4 initHyphy20gGlobal(struct file *sHandle)
{
    int i;
    char            	firmware[256] = DRV_FIRMWARE_FILE;
    
    if (hyPhy20gPcieInit(sHandle, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[framer_open]: hyPhy20PcieInit failed!\n"));
        return -EFAULT;
    }
    
    for (i = 0; i < 5; i++)
    {
     	if (hyPhy20gDcsuInit(sHandle, i) != HYPHY20G_SUCCESS)
     	{
            DA((KERN_ERR "[hyPhy_open]: hyPhy20gDcsuInit failed!\n"));
            return -EFAULT;
     	}
    }
    
    if (hyPhy20gCreateCtxt(sHandle) != HYPHY20G_SUCCESS)
    {
      	DA((KERN_ERR "[hyPhy_open]: hyPhy20gCreateCtxt failed!\n"));
        return -EFAULT;
    }
    
    // fread & jumpAddr is for rev.4.2
    if (hyPhy20gFwDownload(sHandle, (STRING *)firmware, 0xA0000400) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[hyPhy_open]: hyPhy20gFwDownload failed!\n"));
        return -EFAULT;
    }
    
    if (hyPhy20gHostMsgInit(sHandle, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[hyPhy_open]: hyPhy20gHostMsgInit failed!\n"));
        return -EFAULT;
    }
    
    // 2013.04.15 add this for dev.07.07
    if (hyPhy20gHostMsgFwOp(sHandle, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[hyPhy_open]: hyPhy20gHostMsgFwOp failed!\n"));
        return -EFAULT;
    }
    
    for (i = 0; i < 32; i++) // put unused resources into lowest energy state
    {
        if (hyPhy20gBackplaneSerdesInit(sHandle, i, 2) != HYPHY20G_SUCCESS)
        {
            DA((KERN_ERR "[hyPhy_open]: hyPhy20gBackplaneSerdesInit failed!\n"));
            return -EFAULT;
        }
    }
    
    for (i = 0; i < 16; i++) // put unused resources into lowest energy state
    {
        if (hyPhy20gSfpSerdesInit(sHandle, i, 2) != HYPHY20G_SUCCESS)
        {
            DA((KERN_ERR "[hyPhy_open]: hyPhy20gSfpSerdesInit failed!\n"));
            return -EFAULT;
        }
    }

    return HYPHY20G_SUCCESS;	
}


static INT4 PifXAUIConf(struct file *sHandle, int optLoopback)
{
    int i;
    
    // standard xaui 
    if(hyPhy20gPifInit(sHandle, 8, 2, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifInit failed!\n"));
        return -EFAULT;
    }
    
    // According to SW driver manual, the following functions should be AFTER hyPhy20gPifInit
    for (i=0; i < 8; i++)
    {
        if(hyPhy20gPifSerdesInit(sHandle, i, 1) != HYPHY20G_SUCCESS)
        {
            DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifSerdesInit failed!\n"));
            return -EFAULT;
        }
        
        if(hyPhy20gPifRateCfg(sHandle, i, 1, 0, 0, 1) != HYPHY20G_SUCCESS)
        {
            DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifRateCfg failed!\n"));
            return -EFAULT;
        }
    }
    
    // dir = 0
    if(hyPhy20gPifFlowCtrlInit(sHandle, 0, 2, 2, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifFlowCtrlInit failed!\n"));
        return -EFAULT;
    }            
    // dir = 1
    if(hyPhy20gPifFlowCtrlInit(sHandle, 1, 2, 2, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifFlowCtrlInit failed!\n"));
        return -EFAULT;
    }            
    
    
    if(hyPhy20gPifXauiInit(sHandle, 1, 4, 2, 128, 128) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifXauiInit failed!\n"));
        return -EFAULT;
    }            
    
    for (i=0; i < 2; i++)
    {
        if(hyPhy20gPifStdXauiCpbChnlCfg(sHandle, i, i+148) != HYPHY20G_SUCCESS)
        {
            DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifStdXauiCpbChnlCfg failed!\n"));
            return -EFAULT;
        }
    }
    
    if(hyPhy20gPifIntfEnable(sHandle, 1, 0x1F) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifIntfEnable failed!\n"));
        return -EFAULT;
    }
    
    // calling the following function when you want to do XAUI loopback
    if (optLoopback)
    {
    	if(hyPhy20gPifSerdesDiagLpbk(sHandle, 0xFF, 0xFF) != HYPHY20G_SUCCESS)
    	{
           DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifSerdesDiagLpbk failed!\n"));
           return -EFAULT;
    	}	
    }
    
    //hyPhy20gPifChnlFcMap();
    
    // Enabling Data Paths
    if(hyPhy20gPifTxChnlEnable(sHandle, 1, 1, 1, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifTxChnlEnable failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gPifTxChnlEnable(sHandle, 2, 1, 2, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[PifXAUIConf]: hyPhy20gPifTxChnlEnable failed!\n"));
        return -EFAULT;
    }
    
    
    return HYPHY20G_SUCCESS;
}


static INT4 DataPathEnable(struct file *sHandle, INT4 lineType, INT4 cpblp)
{
		INT4 type = 4;
		int  chan, pifChan, lineChan;
		
		// it's for 10GE line interface
		if (lineType == 0)
		{
				// no loopback
				if (cpblp == 0)
				{
						for (chan = 0; chan < 2; chan++)
						{
								pifChan = chan + 147 + 1;
								lineChan = chan + 16;
								
								if(hyPhy20gCpbChnlProv(sHandle, 128, lineChan, 0, pifChan, 0, 1, 640, 1280, 1920, 0, 0) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
        						return -EFAULT;
    						}
    						
          			if(hyPhy20gCpbChnlEnable(sHandle, lineChan) != HYPHY20G_SUCCESS)
          			{
              			DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
              			return -EFAULT;
          			}
          			
          			if(hyPhy20gCpbChnlProv(sHandle, 128, pifChan, 0, lineChan, 0, 1, 640, 1280, 1920, type, 0) != HYPHY20G_SUCCESS)
          			{
              			DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
              			return -EFAULT;
          			}
          			
          			if(hyPhy20gCpbChnlEnable(sHandle, pifChan) != HYPHY20G_SUCCESS)
          			{
              			DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
              			return -EFAULT;
          			}
						} // for (chan = 0; chan < 2; chan++)
				} // if (cpblp == 0)
				else
				{
						for (chan = 0; chan < 2; chan++)
						{
								pifChan = chan + 147 + 1;
								lineChan = chan + 16;
								
								if(hyPhy20gCpbChnlProv(sHandle, 128, lineChan, 0, lineChan, 0, 1, 640, 1280, 1920, 0, 0) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
        						return -EFAULT;
    						}
    						
    						if(hyPhy20gCpbChnlEnable(sHandle, lineChan) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
        						return -EFAULT;
    						}
    						
    						if(hyPhy20gCpbChnlProv(sHandle, 128, pifChan, 0, pifChan, 0, 1, 640, 1280, 1920, type, 0) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
        						return -EFAULT;
    						}
    						
    						if(hyPhy20gCpbChnlEnable(sHandle, pifChan) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
        						return -EFAULT;
    						}
						} // for (chan = 0; chan < 2; chan++)
				}
		}
		else if (lineType == 2)
		{
				// no loopback
				if (cpblp == 0)
				{
						for (chan = 0; chan < 2; chan++)
						{
								pifChan = chan + 147 + 1;
								lineChan = (chan == 0) ? 19 : 83;
					
								if(hyPhy20gCpbChnlProv(sHandle, 64, lineChan, 0, pifChan, 0, 1, 320, 640, 960, 0, 0) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
        						return -EFAULT;
    						}
    						
								if(hyPhy20gCpbChnlEnable(sHandle, lineChan) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
        						return -EFAULT;
    						}
								
    						if(hyPhy20gCpbChnlProv(sHandle, 64, pifChan, 0, lineChan, 0, 1, 320, 640, 960, type, 0) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
        						return -EFAULT;
    						}
								
								if(hyPhy20gCpbChnlEnable(sHandle, pifChan) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
        						return -EFAULT;
    						}
#if 0				
    						if(hyPhy20gWrite(sHandle, 0x4928344 + (pifChan - 147) * 4, ((3 << 1) + 1)) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gWrite failed!\n"));
        						return -EFAULT;
    						}
#endif
						} // for (chan = 0; chan < 2; chan++)
				}
				else
				{
						for (chan = 0; chan < 2; chan++)
						{
								pifChan = chan + 147 + 1;
								lineChan = (chan == 0) ? 19 : 83;
								
								if(hyPhy20gCpbChnlProv(sHandle, 64, lineChan, 0, lineChan, 0, 1, 320, 640, 960, 0, 0) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
        						return -EFAULT;
    						}
    				
    						if(hyPhy20gCpbChnlEnable(sHandle, lineChan) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
        						return -EFAULT;
    						}
    						
    						if(hyPhy20gCpbChnlProv(sHandle, 64, pifChan, 0, pifChan, 0, 1, 320, 640, 960, type, 0) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlProv failed!\n"));
        						return -EFAULT;
    						}
    						
    						if(hyPhy20gCpbChnlEnable(sHandle, pifChan) != HYPHY20G_SUCCESS)
    						{
        						DA((KERN_ERR "[DataPathEnable]: hyPhy20gCpbChnlEnable failed!\n"));
        						return -EFAULT;
    						} // if (hyPhy20gCpb
						} // for (chan = 0; chan < 2; chan++)
				}
		}
		
		
		return HYPHY20G_SUCCESS;	
}


/******************************************************************************* 	
lineType: 	
			0 - 10GELAN
			1 - 10GEWAN
			2 - 10GPOS
			
	cpblp:	 
			0 - no loopback inside cpb
			1 - loopback inside cpb
********************************************************************************/	
static INT4 initHyphy20g(struct file *sHandle, INT4 lineType, INT4 cpblp, INT4 xauilp)
{
    // Init each module
    if(hyPhy20gEnetComDeviceInit(sHandle, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[initMeta20g]: hyPhy20gEnetComDeviceInit failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gTseInit(sHandle, 0, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[initMeta20g]: hyPhy20gTseInit failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gCpbInit(sHandle, 0, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[initMeta20g]: hyPhy20gCpbInit failed!\n"));
        return -EFAULT;
    }            
    
    if (lineType == 0)
    {
    	// line to cpb
    	if(hyPhy20gCpbIntfInit(sHandle, 0, 16, 32, 1) != HYPHY20G_SUCCESS)
    	{
           DA((KERN_ERR "[initMeta20g]: hyPhy20gCpbIntfInit failed!\n"));
           return -EFAULT;
    	}
    }
    else if (lineType == 2)
    {
    	// line to cpb GFP A
    	if(hyPhy20gCpbIntfInit(sHandle, 1, 16, 32, 1) != HYPHY20G_SUCCESS)
    	{
           DA((KERN_ERR "[initMeta20g]: hyPhy20gCpbIntfRateAwareInit failed!\n"));
           return -EFAULT;
    	}
	
    	// line to cpb GFP B
    	if(hyPhy20gCpbIntfInit(sHandle, 2, 16, 32, 1) != HYPHY20G_SUCCESS)
    	{
           DA((KERN_ERR "[initMeta20g]: hyPhy20gCpbIntfRateAwareInit failed!\n"));
           return -EFAULT;
    	}
    }
    
    // pif to cpb
    if(hyPhy20gCpbIntfInit(sHandle, 3, 16, 32, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[initMeta20g]: hyPhy20gCpbIntfInit failed!\n"));
        return -EFAULT;
    }            
    
    // WCI_MODE = 0 read clear mode
    if(hyPhy20gWrite(sHandle, 0x49630A0, 0x0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[initMeta20g]: hyPhy20gWrite failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gWrite(sHandle, 0x49630A4, 0x0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[initMeta20g]: hyPhy20gWrite failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gWrite(sHandle, 0x49630A8, 0x0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[initMeta20g]: hyPhy20gWrite failed!\n"));
        return -EFAULT;
    }
    
    PifXAUIConf(sHandle, xauilp);
    
    DataPathEnable(sHandle, lineType, cpblp);
    
    
    return HYPHY20G_SUCCESS;
}

// According to SW driver manual, the following sequence is correct
static INT4 Client10GELANConf(struct file *sHandle, INT4 xfislice, INT4 hscislice)
{
    //Step 1. Line Interface
    if(hyPhy20gXfiSerdesInit(sHandle, xfislice, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gXfiSerdesInit failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gXfiRateCfg(sHandle, xfislice, 2, 0, 0, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gXfiRateCfg failed!\n"));
        return -EFAULT;
    }
    
    //Step 2. Cross Point (line interface to hsci)
    if(hyPhy20gSonetLnXfiSfiDataPathCfg(sHandle, xfislice, hscislice, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gSonetLnXfiSfiDataPathCfg failed!\n"));
        return -EFAULT;
    }
    
    //Step 3. Client Configuration (HSCI)
    if(hyPhy20gHsciInit(sHandle, hscislice, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gHsciInit failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gEnetComHsciInit(sHandle, hscislice, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gEnetComHsciInit failed!\n"));
        return -EFAULT;
    }
    
    // crcAppend, PadEn	
    if(hyPhy20gHsci10GeLanTxxgCfg(sHandle, hscislice, 0xc, 1, 1, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gEnetComHsciInit failed!\n"));
        return -EFAULT;
    }
    
    // oversizeResp = 0, crcStrip = 0, shortPktEn = 0
    if(hyPhy20gHsci10GeLanRxxgCfg(sHandle, hscislice, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0x4, 0, 0, 0, 0, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gEnetComHsciInit failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gHsci10GeLanSetFrameSizes(sHandle, hscislice, mc20p_maxPkt, mc20p_minPkt, mc20p_maxPkt) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gEnetComHsciInit failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gHsciDataPathEnable(sHandle, hscislice, 0, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gHsciDataPathEnable failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gHsciDataPathEnable(sHandle, hscislice, 1, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GELANConf]: hyPhy20gHsciDataPathEnable failed!\n"));
        return -EFAULT;
    }
    
    
    return HYPHY20G_SUCCESS;
}

static INT4 Client10GEWANConf(struct file *sHandle, INT4 xfislice, INT4 hscislice)
{
    // right now, not support WAN mode
    
    return HYPHY20G_SUCCESS;
}

static INT4 gfpConfig(struct file *sHandle, INT4 xfislice)
{
    UINT4 hdrData[4];
    
    if(hyPhy20gGfpInit(sHandle, xfislice, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpInit failed!\n"));
        return -EFAULT;
    }
    
    // encaps, fcs, 
    if(hyPhy20gGfpHdlcGrpChInitRx(sHandle, xfislice, 1, 0, 0, 1, mc20p_fcsMode, 1, 0, 0, 1, mc20p_payloadScram) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpHdlcGrpChInitRx failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gGfpHdlcGrpChInitTx(sHandle, xfislice, 1, 0, 0, 1, 1, 0, 0, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpHdlcGrpChInitTx failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gGfpSonetSysClkCfg(sHandle, xfislice, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpSonetSysClkCfg failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gGfpGenericFilterCfg(sHandle, xfislice, 0, 0, 0xf, 0, 0x0, 0, 0, 0, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpGenericFilterCfg failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gGfpGenericFilterCfg(sHandle, xfislice, 0, 1, 0xf, 0, 0x0, 0, 0, 0, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpGenericFilterCfg failed!\n"));
        return -EFAULT;
    }
	
    hdrData[0] = hdrData[1] = hdrData[2] = hdrData[3] = 0;
    if(hyPhy20gGfpHdlcPrependCfg(sHandle, xfislice, 0, 4, hdrData, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpHdlcPrependCfg failed!\n"));
        return -EFAULT;
    }
    
    // CPB GFP port configuration
    if(hyPhy20gWrite(sHandle, (0x49285D8 + xfislice * 0x100), 0x0000087d) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gWrite failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gWrite(sHandle, (0x4921590 + xfislice * 0x800), 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gWrite failed!\n"));
        return -EFAULT;
    }	
    
    if(hyPhy20gGfpRxChAdd(sHandle, xfislice, 0, 0, 0, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpRxChAdd failed!\n"));
        return -EFAULT;
    }
    
    if(hyPhy20gGfpTxChAdd(sHandle, xfislice, 0, 0, 4) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[gfpConfig]: hyPhy20gGfpTxChAdd failed!\n"));
        return -EFAULT;
    }
    
    
    return HYPHY20G_SUCCESS;
}


static INT4 Client10GPOSConf(struct file *sHandle, INT4 xfislice, INT4 sonetslicegrp)
{
    int i;
    UINT4 block;
    UINT4 reg_data;
    
    gfpConfig(sHandle, xfislice);
    
    //Line Interface
    if(hyPhy20gXfiSerdesInit(sHandle, xfislice, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gXfiSerdesInit failed!\n"));
        return -EFAULT;
    }
    // OC192 rate
    if(hyPhy20gXfiRateCfg(sHandle, xfislice, 1, 0, 0, 1) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gXfiRateCfg failed!\n"));
        return -EFAULT;
    }
    
    //Transport Service Configuration
    for ( i = 0 ; i < 4; i++)
    {
       	// four slices for one sts192
    		if(hyPhy20gSonetInit(sHandle, sonetslicegrp*4 + i, 1) != HYPHY20G_SUCCESS)
    		{
       	 		DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gSonetInit failed!\n"));
        		return -EFAULT;
    		}
    	hyPhy20gRead(sHandle, (0x4940054 + (sonetslicegrp*4 + i)*0x1000), &reg_data);
    	if (mc20p_sonetScram == 1)
    		reg_data &= 0xffffffcf; // bit 5, bit 4 is TDS, RDDS
    	else
    		reg_data |= 0x30;
    	hyPhy20gWrite(sHandle, (0x4940054 + (sonetslicegrp*4 + i)*0x1000), reg_data);
    }
    
    //Cross Point (line interface to sonet, first time)
    block = sonetslicegrp + 2;
    if(hyPhy20gSonetLnXfiSfiDataPathCfg(sHandle, xfislice, block, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gSonetLnXfiSfiDataPathCfg failed!\n"));
        return -EFAULT;
    }
    
    //Configure SONET/SDH	
    if(hyPhy20gSonetSts192FrmrCfg(sHandle, sonetslicegrp, 4) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gSonetSts192FrmrCfg failed!\n"));
        return -EFAULT;
    }
    
    // additional function, not in the flow chart, totally unkown!!!
    if(hyPhy20gSonetPohPslPdiCfg(sHandle, sonetslicegrp*4, 0, 0, 0, 0x16, 0, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gSonetPohPslPdiCfg failed!\n"));
        return -EFAULT;
    }
    
    // optionally, process OC192 in WIS WAN, maybe not needed	
    if(hyPhy20gSonetLnSonetSysDataPathCfg(sHandle, sonetslicegrp, sonetslicegrp, 1, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gSonetLnSonetSysDataPathCfg failed!\n"));
        return -EFAULT;
    }
    
    //Cross Point (line interface to sonet, second time)
    block = sonetslicegrp + 2;
    if(hyPhy20gSonetLnXfiSfiDataPathCfg(sHandle, xfislice, block, 0) != HYPHY20G_SUCCESS)
    {
        DA((KERN_ERR "[Client10GPOSConf]: hyPhy20gSonetLnXfiSfiDataPathCfg failed!\n"));
        return -EFAULT;
    }
    
    
    return HYPHY20G_SUCCESS;
}


/*******************************************************************************
	mode: 	
			0 - 10GELAN
			1 - 10GEWAN
			2 - 10GPOS
*******************************************************************************/
INT4 clientProv(struct file *sHandle, INT4 slice, INT4 mode)
{
	  if (mode == 0)
		    Client10GELANConf(sHandle, slice, slice);
	  else if (mode == 1)
		    Client10GEWANConf(sHandle, slice, slice);
	  else if (mode == 2)
		    Client10GPOSConf(sHandle, slice, slice);
		
		
	  return HYPHY20G_SUCCESS;
}

int hyphy20g_init(struct file *sHandle, int optLoopback)
{
	char             	drvconf[256] = DRV_CONF_FILE;
	struct file      *conf_file = NULL;
    	
    	char              	cfgbuf[512];
    	int			cfglen = 0;	
    	mm_segment_t      old_fs;
	
	
	// first of all get the configuration for pos
  	conf_file = filp_open(drvconf, O_RDONLY, 0);
	if (IS_ERR(conf_file))
	{
	 	DA((KERN_ERR "Error reading qnf_mc20p.conf!\n"));
	}
	else
	{
	 	old_fs = get_fs();
		set_fs(KERNEL_DS);
		cfglen = conf_file->f_op->read(conf_file, cfgbuf, 1024, &conf_file->f_pos);
		cfgbuf[cfglen] = '\0';
		set_fs(old_fs);
	}
	
	if (conf_file != NULL)
	 	filp_close(conf_file, NULL);
	
	sscanf(cfgbuf, "%u, %u, %u, %u, %u, %u", &mc20p_lineType, &mc20p_sonetScram, &mc20p_payloadScram, &mc20p_fcsMode, &mc20p_minPkt, &mc20p_maxPkt);
	
	initHyphy20gGlobal(sHandle);
	
	initHyphy20g(sHandle, mc20p_lineType, 0, optLoopback);
	
	clientProv(sHandle, 0, mc20p_lineType);
	clientProv(sHandle, 1, mc20p_lineType);
  
  return 0;
}
		
int hyphy20g_read(struct file *sHandle, UINT4 offset, UINT4 *pvalue)
{
    sysHyPhy20gRead(sHandle, offset, pvalue);    
		return 0;
}

int hyphy20g_write(struct file *sHandle, UINT4 offset, UINT4 value)
{
    sysHyPhy20gWrite(sHandle, offset, value);    
    return 0;
}
