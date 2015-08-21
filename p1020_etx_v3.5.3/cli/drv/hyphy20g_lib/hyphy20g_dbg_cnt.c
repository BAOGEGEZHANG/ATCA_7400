
#include "hyphy20g.h"
#include "hyphy20g_pcie.h"
#include "hyphy20g_dcsu.h"
#include "hyphy20g_sonet.h"
#include "hyphy20g_api.h"



#define PMC_COUNTER_IDENTIFIER               \
{                                            \
	"RX_FRAMES_OK                          ",  \
	"RX_OCTETS_OK                          ",  \
	"RX_FRAMES                             ",  \
	"RX_OCTETS                             ",  \
	"RX_UNICAST_FRAMES_OK                  ",  \
	"RX_BROADCAST_FRAMES_OK                ",  \
	"RX_MULTICAST_FRAMES_OK                ",  \
	"RX_TAGGED_FRAMES_OK                   ",  \
	"RX_MAC_PAUSE_CONTROL_FRAMES_OK        ",  \
	"RX_MAC_CONTROL_FRAMES_OK              ",  \
	"RX_FCS_ERROR                          ",  \
	"RESERVED                              ",  \
	"RX_SYMBOL_ERROR                       ",  \
	"RX_LAYER1_ERROR                       ",  \
	"RX_INRANGE_LENGTH_ERROR               ",  \
	"RX_LONG_LENGTH_ERROR                  ",  \
	"RX_LONG_LENGTH_CRC_ERROR              ",  \
	"RX_SHORT_LENGTH_ERROR                 ",  \
	"RX_SHORT_LENGTH_CRC_ERROR             ",  \
	"RX_FRAMES_64_OCTETS                   ",  \
	"RX_FRAMES_65_TO_127_OCTETS            ",  \
	"RX_FRAMES_128_TO_255_OCTETS           ",  \
	"RX_FRAMES_256_TO_511_OCTETS           ",  \
	"RX_FRAMES_512_TO_1023_OCTETS          ",  \
	"RX_FRAMES_1024_TO_1518_OCTETS         ",  \
	"RX_FRAMES_1519_TO_MAXSIZE             ",  \
	"RX_STATION_ADDRESS_FILTERED           ",  \
	"RX_VARIABLE                           ",  \
	"RX_FRAMES_1519_TO_1522_OCTETS         ",  \
	"RX_FRAMES_1523_TO_1548_OCTETS         ",  \
	"RX_FRAMES_1549_TO_2000_OCTETS         ",  \
	"RX_FRAMES_2001_TO_2500_OCTETS         ",  \
	"RX_FRAMES_2501_TO_MAXSIZE             ",  \
	"TX_FRAMES_OK                          ",  \
	"TX_OCTETS_OK                          ",  \
	"TX_OCTETS                             ",  \
	"TX_TAGGED_FRAMES_OK                   ",  \
	"TX_MAC_PAUSE_CONTROL_FRAMES_OK        ",  \
	"TX_FCS_ERROR                          ",  \
	"TX_SHORT_LENGTH_ERROR                 ",  \
	"TX_LONG_LENGTH_ERROR                  ",  \
	"TX_SYSTEM_ERROR                       ",  \
	"TX_MAC_ERROR                          ",  \
	"TX_FRAMES_1523_TO_MAXSIZE             ",  \
	"TX_MAC_CONTROLFRAMES_OK               ",  \
	"TX_UNICAST_FRAMES_OK                  ",  \
	"TX_BROADCAST_FRAMES_OK                ",  \
	"TX_MULTICAST_FRAMES_OK                ",  \
	"TX_UNICAST_FRAMES_ATTEMPTED           ",  \
	"TX_BROADCAST_FRAMES_ATTEMPTED         ",  \
	"TX_MULTICAST_FRAMES_ATTEMPTED         ",  \
	"TX_FRAMES_64_OCTETS                   ",  \
	"TX_FRAMES_65_TO_127_OCTETS            ",  \
	"TX_FRAMES_128_TO_255_OCTETS           ",  \
	"TX_FRAMES_256_TO_511_OCTETS           ",  \
	"TX_FRAMES_512_TO_1023_OCTETS          ",  \
	"TX_FRAMES_1024_TO_1518_OCTETS         ",  \
	"TX_FRAMES_1519_TO_MAXSIZE             ",  \
	"TX_VARIABLE                           ",  \
	"RX_FRAMES_1523_TO_MAXSIZE             ",  \
	"TX_FRAMES_1519_TO_1522_OCTETS         ",  \
	"TX_FRAMES_1523_TO_1548_OCTETS         ",  \
	"TX_FRAMES_1549_TO_2000_OCTETS         ",  \
	"TX_FRAMES_2001_TO_2500_OCTETS         ",  \
	"TX_FRAMES_2501_TO_MAXSIZE             ",  \
}

extern INT4 hyPhy20gHsciEthStatGetCounts(struct file *fileHndl, UINT4 slice, 
                                  UINT4 counter, UINT4 *pCount0,
                                  UINT4 *pCount1, UINT4 *pCount2);
extern INT4 hyPhy20gHsciEthStatTrigCounts(struct file *fileHndl, UINT4 slice);

/******************************************************************************
Function: pmc_eth_get_status
Abstract: hsci port send and receive packet count.
Input:    none
Output:   none
******************************************************************************/
INT4 pmc_eth_get_status(struct file *fileHandle)
{
    INT4 loop;
    INT4  RetVal = HYPHY20G_SUCCESS;
    UINT4 slice;
    UINT4 uCount0 = 0;
    UINT4 uCount1 = 0;
    UINT4 uCount2 = 0;
    unsigned long long PMCcounter = 0;
    char *pcounter[] = PMC_COUNTER_IDENTIFIER;
    
    
    slice = 0;  /* 0 :first port,   1: second port */
    RetVal = hyPhy20gHsciEthStatTrigCounts(fileHandle, slice);
    if (RetVal)
      return RetVal;
    
    printk("\r\nhyPhy20gHsciEthStatGetCounts slice %d", (int)slice);
    for (loop = 0; loop < 65; loop++)
    {
      PMCcounter = 0;
      uCount0 = uCount1 = uCount2 = 0;
      RetVal = hyPhy20gHsciEthStatGetCounts(fileHandle, slice, loop, &uCount0, &uCount1, &uCount2);
      PMCcounter = ((unsigned long long)uCount2<<32)|((unsigned long long)uCount1<<16)|uCount0;
      if (RetVal)
    		return RetVal;
    	
      if (PMCcounter)
      {
        printk("\r\n %s:  %lld",pcounter[loop],PMCcounter);
      }
    }
    printk("\r\n");
    
    slice = 1;
    RetVal = hyPhy20gHsciEthStatTrigCounts(fileHandle, slice);
    if (RetVal)
    	return RetVal;
    
    printk("\r\nhyPhy20gHsciEthStatGetCounts slice %d", (int)slice);
    for (loop = 0; loop < 65; loop++)
    {
      PMCcounter = 0;
      uCount0 = uCount1 = uCount2 = 0;
      RetVal = hyPhy20gHsciEthStatGetCounts(fileHandle, slice, loop, &uCount0, &uCount1, &uCount2);
      PMCcounter = ((unsigned long long)uCount2<<32)|((unsigned long long)uCount1<<16)|uCount0;
      if (RetVal)
    		return RetVal;
      
      if (PMCcounter)
      {
        printk("\r\n %s:  %lld",pcounter[loop],PMCcounter);
      }
    }
    printk("\r\n");
    
    return RetVal;

}
