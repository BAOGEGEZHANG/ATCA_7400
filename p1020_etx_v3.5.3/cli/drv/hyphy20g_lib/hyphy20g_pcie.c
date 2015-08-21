/******************************************************************************/
/**  COPYRIGHT (C) 2009 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        : hyphy20g_pcie.c
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
#include "hyphy20g_pcie.h"

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
**  hyPhy20gPcieInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the PCIe configuration space
**                  in order for host process to have ability to access
**                  HyPHY 20G device register space.
**
**                  No BAR configuration is performed.  This assumes that
**                  HyPHY 20G register addresses do not have any offset from
**                  values presented in register description.
**
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - 0 - PCIe access to device register space
**                                   disabled
**                               1 - PCIe access to device register space
**                                   enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_TARGET
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPcieInit(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    UINT4 devIdReg = 0x0; /* Device ID and Vendor ID Register */
    UINT4 devIdVal = 0x5400; /* DEVICE_ID value */
    UINT4 devIdMsk = 0xffff0000;
    UINT4 devIdOff = 16;
    UINT4 vendorIdVal = 0x11f8; /* VENDOR_ID value */
    UINT4 vendorIdMsk = 0xffff;
    UINT4 vendorIdOff = 0;
    UINT4 statCmdReg = 0x4; /* PCIe Status/Command Register */
    UINT4 barAddrReg = 0x10;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if(enable > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }


    /* set MEM_SPACE_EN and BM_EN based on the enable parameter value */
    result = sysHyPhy20gPcieCfgRead(fileHndl, statCmdReg, &regData);
    if(result) {
	printk("1");
    	return result;
    }

    if(enable){
        regData |= 0x6;
    } else {
        regData &= ~(0x6);
    }

    result = sysHyPhy20gPcieCfgWrite(fileHndl, statCmdReg, regData);
    if(result) {
        printk("2");
        return result;
    }

    /* confirm device is HyPHY 20G */
    result = sysHyPhy20gPcieCfgRead(fileHndl, devIdReg, &regData);
    if(result) {
	printk("3");
        return result;
    }

    if(((regData & vendorIdMsk)>>vendorIdOff)!= vendorIdVal)
    {
	printk("4, vendorIdVal is: 0x%x, regData is: 0x%x\n", vendorIdVal, regData); 
    	return HYPHY20G_ERR_INVALID_TARGET;
    }	
    if(((regData & devIdMsk)>>devIdOff)!= devIdVal)
    {    
	printk("5");
	return HYPHY20G_ERR_INVALID_TARGET;
    }
    	
    /* following are used in point to point connection case */
    /* Set BAR to 0 */
    /* Basic implemention for point to point connection */
    result = sysHyPhy20gPcieCfgWrite(fileHndl, barAddrReg, 0xFFFFFFFF);
    if(result) return result;

    result = sysHyPhy20gPcieCfgRead(fileHndl, barAddrReg, &regData);
    if(result) return result;

    if (regData != 0xF0000000) {
        return HYPHY20G_ERR_INVALID_TARGET;
    }

    result = sysHyPhy20gPcieCfgWrite(fileHndl, barAddrReg, 0x00000000);
    if(result) return result;

    /* perform a test access to confirm ability to access device registers */
    result = sysHyPhy20gWrite(fileHndl, 0x4000420, 0x12345678);
    if (result) {
	printk("7");
	return result;
    }	

    result = sysHyPhy20gRead(fileHndl, 0x4000420, &regData);
    if (result) {
	printk("8");
	return result;
    }
    if (regData != 0x12345678) {
	//printk("9, regData is: 0x%08x", regData);
       return HYPHY20G_ERR_DEV_ACCESS;
    }


#if 0
    sysHyPhy20gRead(fileHndl, 0x4000000, &regData);
    printk("0x4000000: 0x%08x\n", regData);
    
    sysHyPhy20gRead(fileHndl, 0x4000010, &regData);
    printk("0x4000010: 0x%08x\n", regData);
    
    sysHyPhy20gRead(fileHndl, 0x496a10c, &regData);
    printk("0x496a10c: 0x%08x\n", regData);
#endif


    return HYPHY20G_SUCCESS;

} //hyPhy20gPcieInit

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
