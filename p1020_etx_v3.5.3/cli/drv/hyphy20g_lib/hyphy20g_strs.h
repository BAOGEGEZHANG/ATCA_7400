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
**  MODULE      : HyPHY 20G Data Structure Code
**
**  FILE        : hyphy20g_strs.h
**
**  $Date: 2011-08-25 15:11:05 +0800 (Thu, 25 Aug 2011) $
**
**  $Revision: 14116 $
**
**  DESCRIPTION : Contains the definitions of the data structure used by the
**                HyPHY 20G Driver.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_STRS_H
#define _HYPHY20G_STRS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g_typs.h"

#include "hyphy20g_cpb.h"


/*----------------------------------------------------------------------------*/
/* MODULE AND DEVICE MANAGEMENT STRUCTURES                                    */
/*----------------------------------------------------------------------------*/
/* These structures are used to initialize the driver and individual devices  */
/*----------------------------------------------------------------------------*/
#define HYPHY20G_ENABLE                                     1
#define HYPHY20G_DISABLE                                    0
#define HYPHY20G_TX_DIRECTION                               0
#define HYPHY20G_RX_DIRECTION                               1

typedef enum {
    HYPHY20G_CPB = 0x0000 << 16, /* Central Packet Interface */
    HYPHY20G_PIF = 0x0001 << 16, /* Packet Interfacee */
    HYPHY20G_OPI = 0x0002 << 16, /* OPI */
    HYPHY20G_GFP = 0x0003 << 16, /* GFP */
    HYPHY20G_TBD = 0x0004 << 16  /*  */
} eHYPHY20G_SS_ID;


/******************************************************************************/
/* DRIVER'S INTERNAL STRUCTURES                                               */
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/* HYPHY 20G SCBS RESOURCE MANAGEMENT STRUCTURES                              */
/*----------------------------------------------------------------------------*/
/* These definations, structures are used to maintain SCBS resource status    */
/*----------------------------------------------------------------------------*/
#define HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW                4
#define HYPHY20G_SCBS_SCHD_BONUS_MODE_ENABLE                0
#define HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE               1
#define HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER                 0xFF

/* transfer to hyphy20g_cpb.h */
#define HYPHY20G_CPB_NUM_FIFO                           	294
#define HYPHY20G_CPB_NUM_SCHD                           	8
#define HYPHY20G_CPB_NUM_IF_GRP                         	4
#define HYPHY20G_CPB_NUM_WR_PORT                        	294
#define HYPHY20G_CPB_NUM_RD_PORT                        	297

//transfer to hyphy20g_gfp.h
#define HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW                    127
#define HYPHY20G_GFP_TX_SCHD_MAX_CHNL                       64
#define HYPHY20G_GFP_TX_SCHD_MAX_GRP                        64
#define HYPHY20G_GFP_SCHD_MAX_GRP                           64

//transfer to hyphy20g_opi.h
#define HYPHY20G_OPI_SCHD_MAX_NUM_ROW                       2
#define HYPHY20G_OPI_SCHD_MAX_CHNL                          8
#define HYPHY20G_OPI_SCHD_MAX_GRP                           1
#define HYPHY20G_OPI_CBRI1_CHNL_OFFSET                      18
#define HYPHY20G_OPI_CBRI2_CHNL_OFFSET                      26

typedef enum {
    HYPHY20G_CBRI_NO_RATE   = 0, /* 0 - no data */
    HYPHY20G_CBRI_ODU1_RATE = 1, /* 1 - ODU1 */
    HYPHY20G_CBRI_ODU2_RATE = 2, /* 2 - ODU2(2e/2f/1e/1f) */
} eHYPHY20G_CBRI_CHNL_RATE;



//transfer to hyphy20g_pif.h etc
#define HYPHY20G_PIF_SCHD_MAX_GRP                           1

#define HYPHY20G_SCBS_SCHD_MAX_GRP_NUMBER                   64

#define HYPHY20G_SCBS_SCHD_START_ADDR_OFF                   0
#define HYPHY20G_SCBS_SCHD_START_ADDR_MSK                   (0x7F << HYPHY20G_SCBS_SCHD_START_ADDR_OFF)
#define HYPHY20G_SCBS_SCHD_END_ADDR_OFF                     9
#define HYPHY20G_SCBS_SCHD_END_ADDR_MSK                     (0x7F << HYPHY20G_SCBS_SCHD_END_ADDR_OFF)

#define HYPHY20G_SCBS_SCED_BIT_CFG_SOFT_RESET_OFF           31
#define HYPHY20G_SCBS_SCED_BIT_CFG_SOFT_RESET_MSK           (0x0001 << 31)
#define HYPHY20G_SCBS_SCED_BIT_CFG_UNUSED_OFF               3
#define HYPHY20G_SCBS_SCED_BIT_CFG_UNUSED_MSK               (0xFFFFFFF << 3)
#define HYPHY20G_SCBS_SCED_BIT_CFG_SCHD_MODE_OFF            2
#define HYPHY20G_SCBS_SCED_BIT_CFG_SCHD_MODE_MSK            (0x0001 << 2)
#define HYPHY20G_SCBS_SCED_BIT_CFG_WORK_PAGE_SEL_OFF        1
#define HYPHY20G_SCBS_SCED_BIT_CFG_WORK_PAGE_SEL_MSK        (0x0001 << 1)

#define HYPHY20G_SCBS_SCHD_NO_PORT_OFFSET                   0
#define HYPHY20G_SCBS_SCHD_NO_INTF_GRP_DIR                  0

typedef enum {
    HYPHY20G_CPB_LINE_WR_SCHD = HYPHY20G_CPB | 0x0000, /* CPB Line Interface Group Write Scheduler */
    HYPHY20G_CPB_LINE_RD_SCHD = HYPHY20G_CPB | 0x0001, /* CPB Line Interface Group Read Scheduler */
    HYPHY20G_CPB_GFPA_WR_SCHD = HYPHY20G_CPB | 0x0002, /* CPB GFP A Interface Group Write Scheduler */
    HYPHY20G_CPB_GFPA_RD_SCHD = HYPHY20G_CPB | 0x0003, /* CPB GFP A Interface Group ReadScheduler */
    HYPHY20G_CPB_GFPB_WR_SCHD = HYPHY20G_CPB | 0x0004, /* CPB GFP B Interface Group Write Scheduler */
    HYPHY20G_CPB_GFPB_RD_SCHD = HYPHY20G_CPB | 0x0005, /* CPB GFP B Interface Group Read Scheduler */
    HYPHY20G_CPB_PIF_RD_SCHD  = HYPHY20G_CPB | 0x0006, /* CPB PIF Interface Group Read Scheduler */
    HYPHY20G_CPB_QDR_SCHD     = HYPHY20G_CPB | 0x0007, /* CPB QDR Scheduler */

    HYPHY20G_PIF_TX_SCHD_0    = HYPHY20G_PIF | 0x0000, /* PIF Tx Scheduler 0 */
    HYPHY20G_PIF_TX_SCHD_1    = HYPHY20G_PIF | 0x0001, /* PIF Tx Scheduler 1 */

    HYPHY20G_GFP_TX_SCHD_0    = HYPHY20G_GFP | 0x0000, /* GFP Tx Scheduler 0 */
    HYPHY20G_GFP_TX_SCHD_1    = HYPHY20G_GFP | 0x0001, /* GFP Tx Scheduler 1 */

    HYPHY20G_OPI_SCHD_0       = HYPHY20G_OPI | 0x0000, /* OPI Scheduler 0 */
    HYPHY20G_OPI_SCHD_1       = HYPHY20G_OPI | 0x0001, /* OPI Scheduler 1 */

    HYPHY20G_TBD_SCHD         = 0xffffffff
} eHYPHY20G_SCBS_SCHD_ID;

typedef struct _shyphy20g_schd_grp_ctxt
{
    UINT4                   startAddr;  /* scheduler start address */
    UINT4                   endAddr;    /* scheduler end address */
    UINT4                   grpNumRows; /* number of rows in SCBS group
                                           endAddr - startAddr number of four
                                           entry calendar rows */
    /* GB TBD: does Mike have something to add for GFP specific like the
       gfpGrpId? */
} sHYPHY20G_SCHD_GRP_CTXT;

typedef struct _shyphy20g_schd_ctxt
{
    UINT4                   valid;
    eHYPHY20G_SCBS_SCHD_ID  schdId;      /* scheduler ID */
    UINT4                   numGrp;      /* number of groups. it will be 1 for
                                            all schedulers with exception of
                                            GFP A and GFP B.*/
    sHYPHY20G_SCHD_GRP_CTXT *pScbsGrpCtxt;  /* pointer to array of numGrp group
                                            context records */
    UINT4                   sizeScbsGrpCtxt;/* the size of memory space 
											pScbsGrpCtxt point to */
    UINT4                   schdMode;    /* scheduling algorithm mode */
    /* debug lines, add these back so original CPB code can compile */
/*
    UINT4                   startAddr;
    UINT4                   endAddr;
*/
    /* end debug lines */
    UINT4                   numRows;     /* total number of rows available to
                                            the scheduler */
    UINT4                   numChnl;     /* number of channels contained
                                            within the interface group */
    UINT4                   maxChnl;
    UINT4                   chnlOffset;  /* Base channel number used within
                                            data of scheduler */
    UINT4                   portOffset;  /* Base channel number relative to all
                                            channel ports of the CPB
                                            subsystem */
    UINT4                   intfGrpDir;  /* Direction of data flow through
                                            interface group for CPB
                                            0 - write interface group
                                            1 - read interface group */
    UINT4                   totalChnlRate; /* total rate of scheduler elements */
    UINT4                   *pOnLineCalMem;  /* Online calendar data in memory,
                                                should point to a
                                                4*numRows*sizeof(UINT4) memory*/
    UINT4                   sizeOnLineCalMem;/* the size of memory space 
												pOnLineCalMem point to */
    UINT4                   *pOffLineCalMem; /* Offline calendar data in memory,
                                                should point to a
                                                4*numRows*sizeof(UINT4) memory*/
    UINT4                   sizeOffLineCalMem;/* the size of memory space 
												pOffLineCalMem point to */
} sHYPHY20G_SCHD_CTXT;


/*----------------------------------------------------------------------------*/
/* HYPHY 20G INTERLAKEN RESOURCE MANAGEMENT STRUCTURES                        */
/*----------------------------------------------------------------------------*/
/* These definations, structures are used to maintain INTERLAKEN status       */
/*----------------------------------------------------------------------------*/
typedef enum {
    HYPHY20G_INTLKN_PIF_INTLKN = 2, /* PIF Interlaken block */
    HYPHY20G_INTLKN_OPI_CBRI0  = 0, /* OPI CBRI 0 block */
    HYPHY20G_INTLKN_OPI_CBRI1  = 1  /* OPI CBRI 1 block */
} eHYPHY20G_INTLKN_ID;

/* INTERLAKEN block 'Major Bidirectional States' */
typedef enum {
    HYPHY20G_INTLKN_STATE_INVALID     = 0, /* Invalid 'Major Bidirectional States' */
    HYPHY20G_INTLKN_STATE_INRESET     = 1, /* 'Major Bidirectional States' of 1 */
    HYPHY20G_INTLKN_STATE_STANDBY     = 2, /* 'Major Bidirectional States' of 2 */
    HYPHY20G_INTLKN_STATE_OPERATIONAL = 3  /* 'Major Bidirectional States' of 3 */
} eHYPHY20G_INTLKN_MBDIR_STATE;


/*----------------------------------------------------------------------------*/
/* HYPHY 20G FC_8G RESOURCE MANAGEMENT STRUCTURES                             */
/*----------------------------------------------------------------------------*/
/* These definations, structures are used to maintain FC_8G resource status   */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* CPB RESOURCE MANAGEMENT STRUCTURES                                         */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain CPB resource status                  */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_cpb_ram_ctxt
{
    UINT4                   valid;
    UINT1                   ramType;     /* 0 - QDR RAM or 1 - interal RAM */
    UINT2                   blkSize;     /* block size in unitss of 1 brust */
    UINT2                   minFifoSize; /* Minimum FIFO size in units of 1 block*/
    UINT2                   maxFifoSize; /* Maximum FIFO size in units of 1 block*/

    UINT2                   numFreeBlk;   /* Number of free CPB FIFO blocks */
    UINT2                   firstFreeBlk; /* First free FIFO block in CPB RAM */
                                      /* copy of CPB block pointer RAM */
//    UINT2                   blockPtr[HYPHY20G_CPB_NUM_BLOCK];
    UINT2                   blockPtr[1024];

    UINT2                   numFifo;    /* Number of created FIFOs */
    UINT4                   calEntryRate; /* Rate of QDR calendar entry in Mbps
                                             Used for evaluating available
                                             bandwidth when initializing interfaces,
                                             adding channels, or modifying channel
                                             rate in CPB */
} sHYPHY20G_CPB_RAM_CTXT;

typedef struct _shyphy20g_cpb_fifo_ctxt
{
    UINT4                   valid;
    UINT2                   fifoId;   /* FIFO ID */
    UINT2                   numBlks;  /* Number of blocks in this FIFO */
    UINT2                   firstBlk; /* First block in this FIFO */
    UINT2                   portId;   /* channel id, port numbering */
    UINT4                   zone1;    /* FIFO ZONE1 setting */
    UINT4                   zone2;    /* FIFO ZONE2 setting */
    UINT4                   zone3;    /* FIFO ZONE3 setting */
//    eHYPHY20G_CPB_CHNL_STATE    state;      /* channel state */
    UINT4    state;      /* channel state */
    BOOLEAN                 inUseFlag; /* 0-available 1-used */
} sHYPHY20G_CPB_FIFO_CTXT;

typedef struct _shyphy20g_wr_chnl_ctxt
{
    UINT4                   valid;
    UINT2                   chnlId;    /* channel id, channel numbering */
    UINT2                   portId;    /* client port id, port numbering */
    UINT4                   rate;      /* channel rate in units of 1G */
    UINT2                   portWidth; /* port width */
    UINT2                   fifo;      /* FIFO ID */
    UINT2                   rdChnlListLen;  /* Max Non-Primary channel number allowed */
    UINT2                   rdChnlCnt; /* Non-Primary channel counter */
    UINT2                   *pRdChnlList; /* Array of read channel ID */
    UINT4    state;      /* channel state */
    BOOLEAN                 inUseFlag;    /* 0-unused 1-used */
} sHYPHY20G_WR_CHNL_CTXT;

typedef struct _shyphy20g_rd_chnl_ctxt
{
    UINT2                   valid;
    UINT2                   chnlId;         /* channel id, channel numbering */
    UINT2                   portId;         /* client port id, port numbering */
    UINT4                   rate;           /* channel rate in units of 1G */
    UINT2                   portWidth;      /* port width */
    BOOLEAN                 primaryFlag;    /* For reand channel, 0-non primary
                                                channel 1-primary channel */
    UINT2                   index;
    UINT4    state;      /* channel state */
    UINT4                   gfpGrpId;       /* for GFP A and GFP B interface
                                               group channels this field is used
                                               by GFP configuration functions
                                               to identify the CPBID associated
                                               with a VCAT group.
                                               0 to 63 VCAT Group ID
                                               0xFF No Associated Group
                                            */
    BOOLEAN                 inUseFlag;      /* 0-unused 1-used */
} sHYPHY20G_RD_CHNL_CTXT;

typedef struct _shyphy20g_intf_grp_ctxt
{
    UINT4                    valid;
    UINT4   intfGrp;     /* Interface Group ID */
    UINT4                    enable;      /* enabled state of interface group
                                             0 - disabled
                                             1 - enabled
                                          */
    UINT4                    numWrChnls;  /* total number of write channels */
    UINT4                    totalWrRate; /* aggregate bandwidth of all write
                                             channels associated with interface
                                             group */
    UINT4                    availWrRate; /* aggregate bandwidth available to
                                             all write channels associated with
                                             interface group */
    UINT4                    numRdChnls;  /* total number of read channels */
    UINT4                    totalRdRate; /* aggregate bandwidth of all read
                                            channels associated with interface
                                            group */
    UINT4                    availRdRate; /* aggregate bandwidth available to
                                             all read channels associated with
                                             interface group */
} sHYPHY20G_INTF_GRP_CTXT;

typedef struct _shyphy20g_cpb_ctxt
{
    UINT4                       valid;
    UINT4                       maxSchd;    /* the maximal schd num */
    UINT4                       maxWrChnl;  /* the maximal wrChnl */
    sHYPHY20G_CPB_RAM_CTXT      ram;         /* Internal or QDR RAM instance */
    sHYPHY20G_CPB_FIFO_CTXT     *pFifo;      /* point to fifoCtxt array */
    sHYPHY20G_INTF_GRP_CTXT     *pIfGrpCtxt; /* point to ifGrpCtxt array */
    sHYPHY20G_SCHD_CTXT         *pSchdCtxt;  /* point to schdCtxt array */
    sHYPHY20G_WR_CHNL_CTXT      *pWrChnl;    /* point to wrChnl array */
    sHYPHY20G_RD_CHNL_CTXT      *pRdChnl;    /* point to rdChnl array  */
    sHYPHY20G_CPB_FIFO_CTXT     fifoCtxt[HYPHY20G_CPB_NUM_FIFO];    /* Array of CPB FIFOs */
    sHYPHY20G_INTF_GRP_CTXT     ifGrpCtxt[HYPHY20G_CPB_NUM_IF_GRP]; /* Array of context for interface groups */
    sHYPHY20G_SCHD_CTXT         schdCtxt[HYPHY20G_CPB_NUM_SCHD];    /* Array of CPB Schedulers */
    sHYPHY20G_WR_CHNL_CTXT      wrChnl[HYPHY20G_CPB_NUM_WR_PORT];   /* Array of CPB chnls */
    sHYPHY20G_RD_CHNL_CTXT      rdChnl[HYPHY20G_CPB_NUM_RD_PORT];   /* Array of CPB chnls */
} sHYPHY20G_CPB_CTXT;




/*----------------------------------------------------------------------------*/
/* HSCI and LSCI RESOURCE MANAGEMENT STRUCTURES                               */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_lsci_ctxt
{
    UINT4                       valid;
    UINT4                       traffic;        /* software context to store
                                                  major operational mode
                                                  provided by LSCI, refer to:
                                                  eHYPHY20G_LSCI_TRAFFIC_TYPES
                                                  in hyphy20g_lsci.h
                                                */
} sHYPHY20G_LSCI_CTXT;

typedef struct _shyphy20g_hsci_ctxt
{
    UINT4                       valid;
    UINT4                       traffic;        /* software context to store
                                                  major operational mode
                                                  provided by HSCI, refer to:
                                                  eHYPHY20G_HSCI_TRAFFIC_TYPES
                                                  in hyphy20g_hsci.h
                                                */
} sHYPHY20G_HSCI_CTXT;

/*----------------------------------------------------------------------------*/
/* GFP RESOURCE MANAGEMENT STRUCTURES                                         */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain GFP resource status                  */
/*----------------------------------------------------------------------------*/

typedef enum {
    HYPHY20G_GFP_UNUSED = 0, /* GFP channel unconfigured */
    HYPHY20G_GFP_T_AA = 1, /* GFP-T AA mode */
    HYPHY20G_GFP_T_FA = 2, /* GFP-T FA mode */
    HYPHY20G_GFP_F_PKT = 3, /* GFP-F Packet mode */
    HYPHY20G_GFP_F_PHASE = 4, /* GFP-F Phase mode */
    HYPHY20G_GFP_OTHER = 5, /* Configured mode without specific definition */
} eHYPHY20G_GFP_CHNL_MODE;

typedef struct _shyphy20g_gfp_chnl_ctxt
{
    UINT4                       valid;
    eHYPHY20G_GFP_CHNL_MODE     mode;         /* GFP Channel Mode of operation*/
    UINT4                       numSb;        /* GFP-T modes - number of super
                                                 blocks */
    UINT4                       cpbPktLen;    /* Packet Length in CPB:
                                                 - GFP-F Packet mode field not
                                                 currently used, could set to
                                                 minimum packet length
                                                 GFP-T and GFP-F phase mode use
                                                 nominal PMM packet size */
    UINT4                       gfpEncapLen;  /* Number of bytes added by GFP:
                                                 Headers + Extension Header +
                                                 FCS */
} sHYPHY20G_GFP_CHNL_CTXT;

typedef struct _shyphy20g_gfp_scbs_row_ctxt
{
    UINT4                    grpId        ;  /* group that this row belongs to */
} sHYPHY20G_GFP_SCBS_ROW_CTXT;

typedef struct _shyphy20g_gfp_slice_ctxt
{
    UINT4                       valid;
    UINT4                       maxGfpChnl;   /* the maximal gfpChnl */
    sHYPHY20G_SCHD_CTXT         schd;         /* SCBS context structure */
    sHYPHY20G_GFP_SCBS_ROW_CTXT pGfpRowCtxt[128];/* GFP SCBS Row Context */
    sHYPHY20G_GFP_CHNL_CTXT     *pGfpChnlCtxt[64]; /* GFP Channel Context */
} sHYPHY20G_GFP_SLICE_CTXT;

/*----------------------------------------------------------------------------*/
/* PIF RESOURCE MANAGEMENT STRUCTURES                                         */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain PIF resource status                  */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_pif_fc_cal
{
    UINT4                       calLen;     /* active calendar length */
    UINT4                       onlineCalMem[256];  /* calendar memory
                                                       - online page */
    UINT4                       offlineCalMem[256]; /* calendar memory
                                                       - offline page */
} sHYPHY20G_PIF_FC_CAL;

typedef struct _shyphy20g_pif_tx_chnl_ctxt
{
    UINT4                       valid;
    UINT4                       chnlId;     /* channel id, channel numbering */
    UINT4                       rate0;      /* channel rate in units of 1G */
    UINT4                       rate1;      /* channel rate in units of 1G */
    UINT4                       state;      /* channel state */
    UINT4                       intf;       /* channel state */
} sHYPHY20G_PIF_TX_CHNL_CTXT;

typedef struct _shyphy20g_pif_ctxt
{
    UINT4                       valid;
    UINT4                       mode;               /* major interface mode */
    sHYPHY20G_PIF_TX_CHNL_CTXT  txChnlCtxt[150];    /* Tx channel context */
    /* Note that pFcRxCal must be first of other pointers,
    using it as a separator to calculate the size of above variables */
    sHYPHY20G_PIF_FC_CAL        *pFcRxCal;
    sHYPHY20G_PIF_FC_CAL        *pFcTxCal;
    sHYPHY20G_SCHD_CTXT         *pTxSchdCtxt0;  /* PIF TX Scheduler 0 */
    sHYPHY20G_SCHD_CTXT         *pTxSchdCtxt1;  /* PIF TX Scheduler 1 */
} sHYPHY20G_PIF_CTXT;


/*----------------------------------------------------------------------------*/
/* OPI RESOURCE MANAGEMENT STRUCTURES                                         */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain OPI resource status                  */
/*----------------------------------------------------------------------------*/


typedef struct _shyphy20g_cbri_rd_chnl_ctxt
{
    UINT4                   valid;
    UINT4                   rate;           /* channel rate:
                                                   0 - no data
                                                   1 - ODU1
                                                   2 - ODU2(2e/2f/1e/1f) */
} sHYPHY20G_CBRI_RD_CHNL_CTXT;

typedef struct _shyphy20g_cbri_ctxt
{
    UINT4                       valid;
    UINT4                       maxChnl;       /* the maximal Chnl */
    UINT4                       baseChnl;      /* base CBRB channel number of
                                                  CBRI interface */
    /* Note that pSchdCtxt must be first of other pointers,
    using it as a separator to calculate the size of above variables */
    sHYPHY20G_SCHD_CTXT         *pSchdCtxt;  /* CBRI TX Scheduler Context */
    sHYPHY20G_CBRI_RD_CHNL_CTXT *pChnlCtxt;  /* Array TX Scheduler 0 chnl data */
} sHYPHY20G_CBRI_CTXT;

/*----------------------------------------------------------------------------*/
/* TSE RESOURCE MANAGEMENT STRUCTURES                                         */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain TSE resource status                  */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_tse_tslot_ctxt
{
    UINT4                   iPort;     /* input port */
    UINT4                   iTs;       /* input time slot */
    BOOLEAN                 update;    /* update offline page
                                          0 - no update needed
                                          1 - update required */
} sHYPHY20G_TSE_TSLOT_CTXT;

typedef struct _shyphy20g_tse_port_ctxt
{
    sHYPHY20G_TSE_TSLOT_CTXT      *pTseTslotCtxt[48];
} sHYPHY20G_TSE_PORT_CTXT;

typedef struct _shyphy20g_tse_ctxt
{
    sHYPHY20G_TSE_PORT_CTXT      *pTsePortCtxt[40];
} sHYPHY20G_TSE_CTXT;

/*----------------------------------------------------------------------------*/
/* VCAT RESOURCE MANAGEMENT STRUCTURES                                        */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain VCAT resource status                 */
/*----------------------------------------------------------------------------*/
typedef enum {
    HYPHY20G_VCAT_GRP_UNUSED = 0, /* Group currently Unused */
    HYPHY20G_VCAT_GRP_STS1 = 1, /* Group consists of STS1 members */
    HYPHY20G_VCAT_GRP_STS3c  = 2, /* Group consists of STS3c members */
    HYPHY20G_VCAT_GRP_STS12 = 3,  /* Group consists of 1 STS12c */
    HYPHY20G_VCAT_GRP_STS24 = 4,  /* Group consists of 1 STS24c */
    HYPHY20G_VCAT_GRP_STS48 = 5,  /* Group consists of 1 STS48c */
    HYPHY20G_VCAT_GRP_STS192 = 6,  /* Group consists of 1 STS192c */
    HYPHY20G_VCAT_GRP_TS = 7,  /* Group consists of TribSlot members */
    HYPHY20G_VCAT_GRP_ODU1 = 8,  /* Group consists of ODU1 members */
    HYPHY20G_VCAT_GRP_ODU2 = 9,  /* Group consists of OTU2 member */
} eHYPHY20G_VCAT_GRP_TYPE;

typedef struct _shyphy20g_vcat_member_ctxt
{
    UINT4                   stsNum;    /* member number */
} sHYPHY20G_VCAT_MEMBER_CTXT;

typedef struct _shyphy20g_vcat_sts1_ctxt
{
    sHYPHY20G_VCAT_MEMBER_CTXT      *pVcatMemberList[192];
} sHYPHY20G_VCAT_STS1_CTXT;

typedef struct _shyphy20g_vcat_sts3c_ctxt
{
    sHYPHY20G_VCAT_MEMBER_CTXT      *pVcatMemberList[64];
} sHYPHY20G_VCAT_STS3c_CTXT;

typedef struct _shyphy20g_vcatA_ctxt
{
    sHYPHY20G_VCAT_STS1_CTXT      *pVcatSts1TxCtxt[64];
    sHYPHY20G_VCAT_STS1_CTXT      *pVcatSts1RxCtxt[64];
    sHYPHY20G_VCAT_STS3c_CTXT     *pVcatSts3cTxCtxt[64];
    sHYPHY20G_VCAT_STS3c_CTXT     *pVcatSts3cRxCtxt[64];
    UINT4                          sizeMemberCtxt;  /* the size of memory space 
                                                    above pVcatMemberLists point to */
    UINT4                          vcatGrpSpeedCtxt[64];
    eHYPHY20G_VCAT_GRP_TYPE        vcatGrpTypeCtxt[64];
} sHYPHY20G_VCATA_CTXT;

typedef struct _shyphy20g_vcatB_ctxt
{
    UINT4                               vcatGrpSpeedCtxt[64];
    eHYPHY20G_VCAT_GRP_TYPE             vcatGrpTypeCtxt[64];
} sHYPHY20G_VCATB_CTXT;

/*----------------------------------------------------------------------------*/
/* SERDES RESOURCE MANAGEMENT STRUCTURES                                      */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain SERDES resource status               */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_serdes_adc_cal_ctxt
{
    UINT4               valid;
    UINT4               calMode;        /* calibration mode */
    UINT4               calReq;         /* calibration request */
    UINT4               calOk;          /* successful calibration flag */
    UINT4               pisoEnb;        /* stored value of PISO_ENB */
    UINT4               adsmIntDiv;     /* stored value of ADSM_INT_DIV */
    UINT4              *pOffset;        /* offset correction code */
    UINT4               sizeOffset;     /* the size of memory space 
										pOffset point to */
} sHYPHY20G_SERDES_ADC_CAL_CTXT;

typedef struct _shyphy20g_serdes_mode_sel_ctxt
{
    UINT4               valid;
    UINT4               calMode;        /* calibration mode */
    UINT4               mapType;        /* mapping type: linear or non-linear */
    UINT4               accumRef0;      /* stored value ACCUM_REF0 */
} sHYPHY20G_SERDES_MODE_SEL_CTXT;

typedef struct _shyphy20g_serdes_eq_ctxt
{
    UINT4               valid;
    UINT4               calMode;       /* calibration mode */
    UINT4               eqTap1;        /* stored value for EQ_TAP_1 */
    UINT4               eqTap2;        /* stored value for EQ_TAP_2 */
    UINT4               eqTap3;        /* stored value for EQ_TAP_3 */
    UINT4               timingEqTap;   /* stored value for TIMING_EQ_TAP */
} sHYPHY20G_SERDES_EQ_CTXT;

typedef struct _shyphy20g_serdes_ctxt
{
    sHYPHY20G_SERDES_ADC_CAL_CTXT       adcCalCtxt;
    sHYPHY20G_SERDES_MODE_SEL_CTXT      modeSelCtxt;
    sHYPHY20G_SERDES_EQ_CTXT            eqCtxt;
} sHYPHY20G_SERDES_CTXT;

/*----------------------------------------------------------------------------*/
/* HOSTMSG RESOURCE MANAGEMENT STRUCTURES                                     */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain Host Message resource status         */
/*----------------------------------------------------------------------------*/

typedef struct _shyphy20g_hostmsg_ctxt
{
    UINT4                       valid;
    UINT4                       firmwareVer; /* firmware version */
    UINT4                       *pHostMsgSeqCount; /* host message sequence
                                                      count */
} sHYPHY20G_HOSTMSG_CTXT;

/*----------------------------------------------------------------------------*/
/* OTN OH RAM RESOURCE MANAGEMENT STRUCTURES                                  */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain OTN OH RAM resource status           */
/*----------------------------------------------------------------------------*/

typedef struct _shyphy20g_otn_oh_ram_ctxt
{
    UINT4                   otu1RamSize; /* Maximum OTU1 RAM size */
    UINT4                   otu2RamSize; /* Maximum OTU2 RAM size */
} sHYPHY20G_OTN_OH_RAM_CTXT;

/*----------------------------------------------------------------------------*/
/* ODUKPP OH RAM RESOURCE MANAGEMENT STRUCTURES                               */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain ODUKPP OH RAM resource status           */
/*----------------------------------------------------------------------------*/

typedef struct _shyphy20g_odukpp_oh_ram_ctxt
{
    UINT4                   odukppOtu1RamSize; /* Maximum OTU1 RAM size */
    UINT4                   odukppOtu2RamSize; /* Maximum OTU2 RAM size */
} sHYPHY20G_ODUKPP_OH_RAM_CTXT;


#ifndef PMC_HW_CODE
/*----------------------------------------------------------------------------*/
/* RTOS TIMER RESOURCE MANAGEMENT STRUCTURES                                  */
/*----------------------------------------------------------------------------*/
/* These structures are used to maintain rtos timer resource status           */
/*----------------------------------------------------------------------------*/

typedef struct _shyphy20g_rtos_timer_ctxt
{
    UINT4                       valid;
    UINT4                       var0;
    UINT4                       var1;
    UINT4                       var2;
    UINT4                       var3;
    UINT4                       var4;
    TIMEVAL                     startTime_0; /* start time */
    TIMEVAL                     startTime_1; /* start time */
    TIMEVAL                     startTime_2; /* start time */
    TIMEVAL                     startTime_3; /* start time */
    TIMEVAL                     startTime_4; /* start time */
    TIMEVAL                     endTime_0;   /* end time */
    TIMEVAL                     endTime_1;   /* end time */
    TIMEVAL                     endTime_2;   /* end time */
    TIMEVAL                     endTime_3;   /* end time */
    TIMEVAL                     endTime_4;   /* end time */
} sHYPHY20G_RTOS_TIMER_CTXT;
#endif /* PMC_HW_CODE */

/*----------------------------------------------------------------------------*/
/* sHYPHY20G_GLOBAL_DATA_CTXT: global variables used in the driver            */
/*----------------------------------------------------------------------------*/
/* [CAUTION]: add global variables here, not into any *.c files               */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_global_data_ctxt
{
	/* These two members must be first and in order */
    UINT4           hyphy20gCheckRev;
    UINT4           forceRevId;
    
    UINT4           hyphy20gQdrStressMode;
    UINT4           hyphy20gQdrStressDist;
    UINT4           intQdrBlkSize;
    UINT4           hostMsgFwOp;
    UINT4           hyphy20gTraceWrite;
    UINT4           hyphy20gCbrbPacketSizeMode;
} sHYPHY20G_GLOBAL_DATA_CTXT;

/*----------------------------------------------------------------------------*/
/* sHYPHY20G_CTXT: HyPHY 20G Device Context                                   */
/*----------------------------------------------------------------------------*/
/* contains configuration data about the Device level code and pointers to    */
/* configuration data about Device level sub-blocks                           */
/*----------------------------------------------------------------------------*/
/* [CAUTION]: if the context structure is modified in a future version,       */
/* the modifier should also update the code to verify the                     */
/* backwards-compatibility for context extraction/restoration                 */
/* in hyPhy20gExtractCtxt/hyPhy20gRestoreCtxt.                                */
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_ctxt
{
	/* These five members must be first and in order */
    UINT4            valid; /*Indicates that this structure has been initialized*/
    UINT4            version; /* driver version */
    UINT2            realDeviceId;  /* Real device ID                         */
    UINT2            realRevId;     /* Real revision ID of the device         */
    sHYPHY20G_GLOBAL_DATA_CTXT gData; /* global variables used in the driver  */

    INT4             errDevice;     /* Global error var for device calls      */
    UINT2            mimicDeviceID; /* Mimic device ID                        */
    UINT2            mimicRevId;    /* Mimic revision ID of the device        */
    eHYPHY20G_DEV_STATE stateDevice; /* Device State; can be one of the
                                        following: START, PRESENT */
    void             *baseAddr;   /* Base address of the Device               */

    /* Pointers to each subsystem of the device */
    /* Note that pCpbCtxt must be first of other subsystems pointers,
    using it as a separator to calculate the size of above variables */
    sHYPHY20G_CPB_CTXT          *pCpbCtxt;
    sHYPHY20G_LSCI_CTXT         *pLsciCtxt[16];
    sHYPHY20G_HSCI_CTXT         *pHsciCtxt[2];
    sHYPHY20G_PIF_CTXT          *pPifCtxt;
    sHYPHY20G_GFP_SLICE_CTXT    *pGfpACtxt;
    sHYPHY20G_GFP_SLICE_CTXT    *pGfpBCtxt;
    sHYPHY20G_CBRI_CTXT         *pCbriCtxt[2];
    sHYPHY20G_TSE_CTXT          *pTseCtxt;
    sHYPHY20G_VCATA_CTXT        *pVcatACtxt;
    sHYPHY20G_VCATB_CTXT        *pVcatBCtxt;
    sHYPHY20G_SERDES_CTXT       *pSfpSerdesCtxt[16];
    sHYPHY20G_SERDES_CTXT       *pXfiSerdesCtxt[2];
    sHYPHY20G_SERDES_CTXT       *pBackplaneSerdesCtxt[32];
    sHYPHY20G_SERDES_CTXT       *pPifSerdesCtxt[8];
    sHYPHY20G_HOSTMSG_CTXT      *pHostMsgCtxt;   
    sHYPHY20G_OTN_OH_RAM_CTXT   *pOtnOhRamCtxt;
    sHYPHY20G_ODUKPP_OH_RAM_CTXT   *pOdukppOhRamCtxt;
#ifndef PMC_HW_CODE
    sHYPHY20G_RTOS_TIMER_CTXT   *pRtosTimerCtxt;
#endif /* PMC_HW_CODE */
#ifdef HYPHY20G_API_REENTRANT
    UINT4       sem; /* semaphore id, should be re-created after warm restart, 
                        and not overwritten by the RestoreContext() function. */
#endif
} sHYPHY20G_CTXT;

/*----------------------------------------------------------------------------*/
/* sHYPHY20G_CFG_DUMP_REQUEST_TYPE: HyPHY 20G Configuration Dump Request Type.*/
/*----------------------------------------------------------------------------*/
typedef struct _shyphy20g_cfg_dump_request_type
{
    UINT4 length;               /*the requested length, unit 4 bytes */
    UINT4 startAddr;            /*startAddr of the register block, 4 bytes alignment*/
    INT4 rtCode;                /*the dump request return value */
}sHYPHY20G_CFG_DUMP_REQUEST_TYPE;


#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_STRS_H */
/* end of file */
