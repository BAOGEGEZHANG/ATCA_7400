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
**  FILE        : hyphy20g_opi_features.c
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
#include "hyphy20g_opi_features.h"

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
**  hyPhy20gCbrbFailSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the events that contribute to
**                fail indication being communicated to downstream OTUn or
**                ODUKPP subsystems.
**
**
**  INPUTS:       *fileHndl     - base address
**                 port         - ODUk mini framer instance
**                                0 to 7   - OTU1 Framer 0 to 7
**                                8 to 9   - OTU2 Framer 0 to 1
**                                10       - OPI Port 10 associated with OTU2 or
**                                           OTU1 ODUKPP Framer
**                                11 to 13 - OPI Port 11 to 13 associated with
**                                           OTU1 ODUKPP Framer
**                                14       - OPI Port 14 associated with OTU2 or
**                                           OTU1 ODUKPP Framer
**                                15 to 17 - OPI Port 15 to 17 associated with
**                                           OTU1 ODUKPP Framer
**                               Write channel related mini framers:
**                               18 to 21 VCAT_LCAT interface 0 data towards
**                                        CBRB, OTU1 #1 to #4
**                               22 to 25 VCAT_LCAT interface 1 data towards
**                                        CBRB, OTU1 #1 to #4
**                 oofEnbl      - OOF event enable
**                                0 - Do not trigger fail signal on OOF event
**                                1 - Trigger fail signal on OOF event
**                 aisEnbl      - AIS event enable
**                                0 - Do not trigger fail signal on AIS event
**                                1 - Trigger fail signal on AIS event
**                 ptmEnbl      - PTM event enable
**                                0 - Do not trigger fail signal on PTM event
**                                1 - Trigger fail signal on PTM event
**                 vcPtmEnbl    - VC PTM event enable
**                                0 - Do not trigger fail signal on VC PTM event
**                                1 - Trigger fail signal on VC PTM event
**                 ociEnbl      - OCI event enable
**                                0 - Do not trigger fail signal on OCI event
**                                1 - Trigger fail signal on OCI event
**                 soakEnbl     - SOAK OOF persistance enable
**                                0 - Do not trigger fail signal when OOF
**                                    condition has persisted for SOAK_OOF_TIMER
**                                    number of frames
**                                1 - Trigger fail signal when OOF condition has
**                                    persisted for SOAK_OOF_TIMER number of
**                                    frames
**                 lckEnbl      - LCK event enable
**                                0 - Do not trigger fail signal on LCK event
**                                1 - Trigger fail signal on LCK event
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbFailSrcCfg(struct file *fileHndl, UINT4 port, UINT4 oofEnbl,
                            UINT4 aisEnbl, UINT4 ptmEnbl, UINT4 vcPtmEnbl,
                            UINT4 ociEnbl, UINT4 soakEnbl, UINT4 lckEnbl)
{

    UINT4 addr, msk, val;
    INT4 result;

    /* argument checking */
    if((port > 25)||(oofEnbl > 1)||(aisEnbl > 1)||(ptmEnbl > 1)||
       (vcPtmEnbl > 1)||(ociEnbl > 1)||(soakEnbl > 1)||(lckEnbl > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_OPI_TOP_REG_ALM_CFG_OFFSET(port);

    /* Configure Register Fields */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* OOF_PORT_ENABLE */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_OOF_PORT_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, oofEnbl);
    if(result) return result;

    /* AIS_PORT_ENABLE */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_AIS_PORT_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, aisEnbl);
    if(result) return result;

    /* PTM_PORT_ENABLE */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_PTM_PORT_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, ptmEnbl);
    if(result) return result;

    /* VC_PTM_PORT_ENABLE */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_VC_PTM_PORT_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, vcPtmEnbl);
    if(result) return result;

    /* OCI_PORT_ENABLE */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_OCI_PORT_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, ociEnbl);
    if(result) return result;

    /* SOAK_OOF_FAIL_PORT_ENABLE */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_SOAK_OOF_FAIL_PORT_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, soakEnbl);
    if(result) return result;

    /* LCK_PORT_ENABLE */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_LCK_PORT_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, lckEnbl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbFailSrcCfg */


/*******************************************************************************
**
**  hyPhy20gCbrbFailProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the criteria for declaring
**                SOAK_OOF, PT mismatch and VC PT mismatch fail events.
**
**
**  INPUTS:       *fileHndl     - base address
**                 port         - ODUk mini framer instance
**                                0 to 7   - OTU1 Framer 0 to 7
**                                8 to 9   - OTU2 Framer 0 to 1
**                                10       - OPI Port 10 associated with OTU2 or
**                                           OTU1 ODUKPP Framer
**                                11 to 13 - OPI Port 11 to 13 associated with
**                                           OTU1 ODUKPP Framer
**                                14       - OPI Port 14 associated with OTU2 or
**                                           OTU1 ODUKPP Framer
**                                15 to 17 - OPI Port 15 to 17 associated with
**                                           OTU1 ODUKPP Framer
**                               Write channel related mini framers:
**                               18 to 21 VCAT_LCAT interface 0 data towards
**                                        CBRB, OTU1 #1 to #4
**                               22 to 25 VCAT_LCAT interface 1 data towards
**                                        CBRB, OTU1 #1 to #4
**                 ptExp        - Expected PT message.  Legal range 0 to 0xFF
**                 vcPtExp      - Expected VC PT message.  Legal range 0 to 0xFF
**                 soakOffTmr   - Number of OOF frames before declaring SOAK OFF
**                                event.
**                                Default value 0xF9
**
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbFailProcCfg(struct file *fileHndl, UINT4 port,
                             UINT4 ptExp, UINT4 vcPtExp, UINT4 soakOofTmr)
{
    UINT4 addr, msk, val;
    INT4 result;

    /* argument checking */
    if((port > 25)||(ptExp > 0xFF)||(vcPtExp > 0xFF)||(soakOofTmr > 0xFF)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_OPI_TOP_REG_ALM_CFG_OFFSET(port);

    /* Configure Register Fields */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* PT_EXPECTED */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_PT_EXPECTED_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, ptExp);
    if(result) return result;

    /* VC_PT_EXPECTED */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_VC_PT_EXPECTED_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, vcPtExp);
    if(result) return result;

    /* SOAK_OOF_TIMER */
    msk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_SOAK_OOF_TIMER_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, soakOofTmr);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbFailProcCfg */

/*******************************************************************************
**
**  hyPhy20gCbrbMiniFrmrGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve the status for one of the ODUk
**                  mini framers associated with OTN and ODUKPP interfaces.
**
**                  Status bit mapping for bits 25:0
**                  0 to 7   - OTU1 Framer 0 to 7
**                  8 to 9   - OTU2 Framer 0 to 1
**                  10       - OPI Port 10 associated with OTU2 or
**                             OTU1 ODUKPP Framer
**                  11 to 13 - OPI Port 11 to 13 associated with
**                             OTU1 ODUKPP Framer
**                  14       - OPI Port 14 associated with OTU2 or
**                             OTU1 ODUKPP Framer
**                  15 to 17 - OPI Port 15 to 17 associated with
**                             OTU1 ODUKPP Framer
**                  18       - VCAT_LCAS interface 0, OTU2 or
**                             OTU1 #1 Inbound
**                  19 to 21 - VCAT_LCAS interface 0, OTU1 #2 to
**                             #4 Inbound
**                  22       - VCAT_LCAS interface 1, OTU2 or
**                             OTU1 #1 Inbound
**                  23 to 25 - VCAT_LCAS interface 1, OTU1 #2 to
**                                           #4 Inbound
**
**  INPUTS:       *fileHndl     - base address
**                *pOofStat     - Pointer to storage for OOF_STAT
**                                bit 31-26 : RESERVED
**                                bit 25-0  : OOF_CHG_V
**                *pAisStat     - Pointer to storage for AIS_STAT
**                                bit 31-26 : RESERVED
**                                bit 25-0  : AIS_CHG_V
**                *pLckStat     - Pointer to storage for LCK_STAT
**                                bit 31-26 : RESERVED
**                                bit 25-0  : LCK_CHG_V
**                *pOciStat     - Pointer to storage for OCI_STAT
**                                bit 31-26 : RESERVED
**                                bit 25-0  : OCI_CHG_V
**                *pPtmStat     - Pointer to storage for PTM_STAT
**                                bit 31-26 : RESERVED
**                                bit 25-0  : PTM_CHG_V
**                *pVcPtmStat   - Pointer to storage for VC_PTM_STAT
**                                bit 31-26 : RESERVED
**                                bit 25-0  : VC_PTM_CHG_V
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbMiniFrmrGetStatus(struct file *fileHndl, UINT4 *pOofStat,
                                   UINT4 *pAisStat, UINT4 *pLckStat,
                                   UINT4 *pOciStat, UINT4 *pPtmStat,
                                   UINT4 *pVcPtmStat)
{
    UINT4 addr, val;
    INT4 result;

    /* retrieve OOF status */
    addr = HYPHY20G_OPI_TOP_REG_OOF_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pOofStat = val;

    /* retrieve AIS status */
    addr = HYPHY20G_OPI_TOP_REG_AIS_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pAisStat = val;

    /* retrieve LCK status */
    addr = HYPHY20G_OPI_TOP_REG_LCK_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pLckStat = val;

    /* retrieve OCI status */
    addr = HYPHY20G_OPI_TOP_REG_OCI_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pOciStat = val;

    /* retrieve PTM status */
    addr = HYPHY20G_OPI_TOP_REG_PTM_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pPtmStat = val;

    /* retrieve VC_PTM status */
    addr = HYPHY20G_OPI_TOP_REG_VC_PTM_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pVcPtmStat = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbMiniFrmrGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbrbPktMonGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the packet monitor status.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits for systems that implement
**                  WCIMODE, otherwise interrupts are cleared on read.
**
**                  MON_*_*_*_I[17:0] bits correspond to device resources
**                  as follows:
**
**                  n = 0 to 7 OTU1 Framers 0 to 7
**                  n = 8 to 9 OTU2 Framers 0 to 1
**                  n = 10 to 13 ODUKPP (1 port ODU2 mode, 4 ports ODU1 mode)
**                  n = 14 to 17 ODUKPP (1 port ODU2 mode, 4 ports ODU1 mode)
**
**  INPUTS:       *fileHndl    - base address
**                intClear     - 1 - all *_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                             - 0 - all *_I interrupt bits accessed by this
**                               function will retain their values if WCIMODE is
**                               used.
**                *pStatus0    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MON_DP_OVERFLOW_INT_I[17:0]
**                *pStatus1    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MON_SOP_ERR_INT_I[17:0]
**                *pStatus2    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MON_EOP_ERR_INT_I[17:0]
**                *pStatus3    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MON_CSI_CHG_I[17:0]
**                *pStatus4    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MON_PKT1_LOS_I[17:0]
**                *pStatus5    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MON_PKT2_LOS_I[17:0]
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbPktMonGetStatus(struct file *fileHndl, UINT4 intClear,
                                 UINT4 *pStatus0, UINT4 *pStatus1,
                                 UINT4 *pStatus2, UINT4 *pStatus3, 
                                 UINT4 *pStatus4, UINT4 *pStatus5)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    /* MON_DP_OVERFLOW_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MON_INT_1;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus0 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* MON_SOP_ERR_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MON_INT_2;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus1 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* MON_EOP_ERR_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MON_INT_3;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus2 = val;
    
    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }
    
    /* MON_CSI_CHG_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MON_INT_4;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus3 = val;
    
    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }
    
    /* MON_PKT1_LOS_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MON_INT_5;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus4 = val;
    
    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }
    
    /* MON_PKT2_LOS_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MON_INT_6;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus5 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbPktMonGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbrbPktMonGetRxCsiValue
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the CSI value received by a 
**                  packet monitor.  This function is only valid for monitors
**                  operating in timestamp mode.
**
**  INPUTS:       *fileHndl    - base address
**                 pmm         - PMM Monitor to access
**                               0 - 7 OTU1 Framer 0 to 7
**                               8 - 9 OTU2 Framer 0 to 1
**                               10 - 13 VCAT Interface 0 ports 0 to 3
**                               14 - 17 VCAT Interface 1 ports 0 to 3
**                *pMonCsiOut  - Pointer to MON_CSI_OUT value
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbPktMonGetRxCsiValue(struct file *fileHndl, UINT4 pmm,
                                     UINT4 *pMonCsiOut)
{
    UINT4 addr, msk, val;
    INT4 result;

    /* argument checking */
    if (pmm > 17) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve MON_CSI_OUT */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_3_OFFSET(pmm);
    msk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_3_MON_CSI_OUT_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &val);
    if (result) return result;
    *pMonCsiOut = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbPktMonGetRxCsiValue */

/*******************************************************************************
**
**  hyPhy20gCbrbPktMkrGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the packet maker status.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits for systems that implement
**                  WCIMODE, otherwise interrupts are cleared on read.
**
**                  MKR_*_*_INT_I[17:0] bits correspond to device resources
**                  as follows:
**
**                  n = 0 to 7 OTU1 Framers 0 to 7
**                  n = 8 to 9 OTU2 Framers 0 to 1
**                  n = 10 to 13 VCAT#0 (1 port ODU2 mode, 4 ports ODU1 mode)
**                  n = 14 to 17 VCAT#1 (1 port ODU2 mode, 4 ports ODU1 mode)
**
**  INPUTS:       *fileHndl    - base address
**                intClear     - 1 - all *_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                             - 0 - all *_I interrupt bits accessed by this
**                               function will retain their values if WCIMODE is
**                               used.
**                *pStatus0    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MKR_DP_OVERFLOW_INT_I[17:0]
**                *pStatus1    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MKR_DSM_OVERFLOW_INT_I[17:0]
**                *pStatus2    - Pointer to storage for status bits
**                               bit 31-18: RESERVED
**                               bit 17-0 : MKR_DSM_UNDERFLOW_INT_I[17:0]
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbPktMkrGetStatus(struct file *fileHndl, UINT4 intClear,
                                 UINT4 *pStatus0, UINT4 *pStatus1,
                                 UINT4 *pStatus2)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    /* MKR_DP_OVERFLOW_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MKR_INT_1;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus0 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* MKR_DSM_OVERFLOW_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MKR_INT_2;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus1 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* MKR_DSM_UNDERFLOW_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_PKT_MKR_INT_3;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus2 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbPktMkrGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbrbFcGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the transition status of interlaken
**                  flow control.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits for systems that implement
**                  WCIMODE, otherwise interrupts are cleared on read.
**
**                  FC_STATUS_n_INT_I[17:0] bits correspond to device resources
**                  as follows:
**
**                  n = 0 to 7 CBRI#1 flow control ports 0 to 7
**                  n = 8 CBRI#1 link level flow control
**                  n = 9 to 16 CBRI#2 flow control ports 0 to 7
**                  n = 17 CBRI#2 link level flow control
**
**  INPUTS:       *fileHndl    - base address
**                intClear     - 1 - all *_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                             - 0 - all *_I interrupt bits accessed by this
**                               function will retain their values if WCIMODE is
**                               used.
**                *pStatus0    - Pointer to storage for event transition
**                               bit 31-18: RESERVED
**                               bit 17-0 : FC_STATUS_INT_I[17:0]
**                *pStatus1    - Pointer to storage for event state
**                               bit 31-18: RESERVED
**                               bit 17-0 : FC_STATUS_INT_V[17:0]
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbFcGetStatus(struct file *fileHndl, UINT4 intClear,
                             UINT4 *pStatus0, UINT4 *pStatus1)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    /* FC_STATUS_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_INTLKN_FC_INT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus0 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* FC_STATUS_FALL_INT_I[17:0] */
    addr = HYPHY20G_OPI_TOP_REG_INTLKN_FC_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus1 = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbFcGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbrbFifoGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve CBRB FIFO overflow status.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits for systems that implement
**                  WCIMODE, otherwise interrupts are cleared on read.
**
**                  FC_STATUS_n_INT_I[17:0] bits correspond to device resources
**                  as follows:
**
**                  n = 0 to 7 CBRI#1 flow control ports 0 to 7
**                  n = 8 CBRI#1 link level flow control
**                  n = 9 to 16 CBRI#2 flow control ports 0 to 7
**                  n = 17 CBRI#2 link level flow control
**
**  INPUTS:       *fileHndl    - base address
**                intClear     - 1 - all *_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                             - 0 - all *_I interrupt bits accessed by this
**                               function will retain their values if WCIMODE is
**                               used.
**                *pStatus0    - Pointer to storage for status bits
**                               bit 31-0 : FIFO_OVF_INT_I[31:0]
**                *pStatus1    - Pointer to storage for status bits
**                               bit 31-2 : RESERVED
**                               bit 1-0  : FIFO_OVF_INT_I[33:32]
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbFifoGetStatus(struct file *fileHndl, UINT4 intClear,
                               UINT4 *pStatus0, UINT4 *pStatus1)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    /* FIFO_OVF_INT_I[31:0] */
    addr = HYPHY20G_OPI_CBRB_REG_FIFO_OF_INT_1;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus0 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* FIFO_OVF_INT_I[33:32] */
    addr = HYPHY20G_OPI_CBRB_REG_FIFO_OF_INT_2;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatus1 = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbFifoGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbriShimGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve the status for one of the CBRI
**                  Interlaken instances at the Shim level.
**
**  INPUTS:       *fileHndl    - base address
**                 slice       - CBRI Interlaken Slice instance
**                               0 - CBRI0
**                               1 - CBRI1
**                *pStatusBits - Pointer to storage for status bits
**                               bit 31-7 : RESERVED
**                               bit 6    : PB_FIFO_2B_ECC_V
**                               bit 5    : PB_FIFO_1B_ECC_V
**                               bit 4    : RX_SOP_MISSING_V
**                               bit 3    : RX_EOP_MISSING_V
**                               bit 2    : PB_FIFO_OVERFLOW_V
**                               bit 1    : TX_SOP_MISSING_V
**                               bit 0    : TX_EOP_MISSING_V
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriShimGetStatus(struct file *fileHndl, UINT4 slice,
                               UINT4 *pStatusBits)
{
    UINT4 addr, val;
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

    addr = HYPHY20G_CBRI_SHIM_REG_INT_VAL;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice) ;

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatusBits= val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriShimGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbriCoreRxIntGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve the top level interrupt status for
**                  one of the CBRI Interlaken Core instances for the receive
**                  direction.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**  INPUTS:       *fileHndl    - base address
**                 slice       - CBRI Interlaken Slice instance
**                               0 - CBRI0
**                               1 - CBRI1
**                intClear     - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                             - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                *pStatusBits - Pointer to storage for interrupt bits
**                               bit 31   : CTLBIT_ERR_I
**                               bit 29   : MF_ERR_I
**                               bit 28   : BADCTL_ERR_I
**                               bit 27   : SCRAM_ERR_I
**                               bit 26   : CRC32_ERR_I
**                               bit 24   : WRD_SYNC_CHANGE_I
**                               bit 23   : ALIGN_CHANGE_I
**                               bit 22   : ALIGN_ERR_I
**                               bit 21   : BURST_ERR_I
**                               bit 20   : BMIN_ERR_I
**                               bit 19   : BMAX_ERR_I
**                               bit 18   : CRC24_ERR_I
**                               bit 17   : IB_FC_ERR_I
**                               bit 16   : RX_STATUS_MSG_CHANGE_I
**                               bit 13   : BADCTL_CNT_FILL_I
**                               bit 10   : CRC32_CNT_FILL_I
**                               bit 8    : WRD_SYNC_CNT_FILL_I
**                               bit 7    : ALIGN_FAIL_CNT_FILL_I
**                               bit 6    : ALIGN_CNT_FILL_I
**                               bit 5    : BURST_CNT_FILL_I
**                               bit 4    : BMIN_CNT_FILL_I
**                               bit 3    : BMAX_CNT_FILL_I
**                               bit 2    : CRC24_CNT_FILL_I
**                               bit 1    : RX_FC_CNT_FILL_I
**                               bit 0    : RX_CHAN_CNT_FILL_I
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriCoreRxIntGetStatus(struct file *fileHndl, UINT4 slice,
                                    UINT4 intClear, UINT4 *pStatusBits)
{
    UINT4 devId, revId, status;
    UINT4 addr, val;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 1)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* select RX_INT register */
    addr = HYPHY20G_CBRI_CORE_REG_RX_INT;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice) ;

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatusBits= val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriCoreRxIntGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbriCoreTxIntGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve the top level interrupt status for
**                  one of the CBRI Interlaken Core instances for the transmit
**                  direction.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - CBRI Interlaken Slice instance
**                                0 - CBRI0
**                                1 - CBRI1
**                intClear      - 1 - all *INT_I interrupt bits accessed by this
**                                function will be cleared by writing them to
**                                '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                                function will retain their values.
**                *pStatusBits  - Pointer to storage for interrupt bits
**                                bit 31-17: RESERVED
**                                bit 16   : TX_STATUS_MSG_CHANGE_I
**                                bit 15-2 : RESERVED
**                                bit 1    : TX_FC_CNT_FILL_I
**                                bit 0    : TX_CHAN_CNT_FILL_I
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriCoreTxIntGetStatus(struct file *fileHndl, UINT4 slice,
                                    UINT4 intClear, UINT4 *pStatusBits)
{
    UINT4 devId, revId, status;
    UINT4 addr, val;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 1)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* select RX_INT register */
    addr = HYPHY20G_CBRI_CORE_REG_TX_INT;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice) ;

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatusBits= val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriCoreTxIntGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbriCoreTxCounterGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve the status for transmit counters from
**                  the CBRI Interlaken Core.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - CBRI Interlaken Slice instance
**                                0 - CBRI0
**                                1 - CBRI1
**                intClear      - 1 - all status bits accessed by this
**                                function will be cleared by writing them to
**                                '1'
**                              - 0 - all status bits accessed by this
**                                function will retain their values.
**                *pCntFill     - Pointer to storage for counter fill status:
**                                bit 31   : BOVER
**                                bit 30   : POVER
**                                bit 29   : EOVER
**                                bit 28   : BHALF
**                                bit 27   : PHALF
**                                bit 26   : EHALF
**                                bit 25-0 : RESERVED
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriCoreTxCounterGetStatus(struct file *fileHndl, UINT4 slice,
                                        UINT4 intClear, UINT4 *pStatusBits)
{
    UINT4 addr, val;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 1)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve counter fill status for Tx direction (index = 0) */
    addr = HYPHY20G_CBRI_CORE_REG_CNT_FILL_OFFSET(0);
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pStatusBits= val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriCoreTxCounterGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbriCoreRxCounterGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve the status for receive counters from
**                  the CBRI Interlaken Core.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl      - base address
**                 slice         - CBRI Interlaken Slice instance
**                                 0 - CBRI0
**                                 1 - CBRI1
**                intClear       - 1 - all status bits accessed by this
**                                 function will be cleared by writing them to
**                                 '1'
**                               - 0 - all status bits accessed by this
**                                 function will retain their values.
**                *pCntFill      - Pointer to storage for counter fill status:
**                                 bit 31   : BOVER
**                                 bit 30   : POVER
**                                 bit 29   : EOVER
**                                 bit 28   : BHALF
**                                 bit 27   : PHALF
**                                 bit 26   : EHALF
**                                 bit 25-0 : RESERVED
**                *pCrc32CntFill - Rx Lane CRC Error Count over half full status
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Counter over half full
**                                 0 - Counter upto half full
**                *pWordCntFill  - Rx Lane Word Sync Error Count over half full
**                                 status
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Counter over half full
**                                 0 - Counter upto half full
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriCoreRxCounterGetStatus(struct file *fileHndl, UINT4 slice, 
                                        UINT4 intClear, UINT4 *pCntFill, 
                                        UINT4 *pCrc32CntFill, 
                                        UINT4 *pWordCntFill)
{
    UINT4 addr, val;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 1)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve counter fill status for Rx direction (index = 1) */
    addr = HYPHY20G_CBRI_CORE_REG_CNT_FILL_OFFSET(1);
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCntFill = val;

    /* RX_CRC32_CNT_FILL_LANE */
    addr = HYPHY20G_CBRI_CORE_REG_CRC32_CNT_FILL;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCrc32CntFill = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_WRD_SYNC_CNT_FILL_LANE */
    addr = HYPHY20G_CBRI_CORE_REG_RX_WD_SYNC_CNT_FILL;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pWordCntFill = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriCoreRxCounterGetStatus */

/*******************************************************************************
**
**  hyPhy20gCbriCoreRxLaneGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieve the status for receive lanes from
**                  the CBRI Interlaken Core.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**  INPUTS:       *fileHndl      - base address
**                 slice         - CBRI Interlaken Slice instance
**                                 0 - CBRI0
**                                 1 - CBRI1
**                intClear       - 1 - all status bits accessed by this
**                                 function will be cleared by writing them to
**                                 '1'
**                               - 0 - all status bits accessed by this
**                                 function will retain their values.
**                *pMfErr        - Pointer to storage for per lane metaframe
**                                 errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pCtlWrdErr    - Pointer to storage for per lane bad control
**                                 word errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pScrbStateErr - Pointer to storage for per lane scrambler
**                                 state errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pCrc32Err     - Pointer to storage for per lane CRC 32
**                                 errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pWrdSyncChng  - Pointer to storate for per lane word
**                                 synchronization changes
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Change of word synchronization occurred
**                                 0 - No change occurred
**                *pWrdSyncStat  - Pointer to storage for per lane word
**                                 synchronization status
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Word synchronization present
**                                 0 - Word synchronization not present
**                *pAlignStatus  - Pointer to storage for interface alignment
**                                 status.  One bit:
**                                 1 - Alignment Good
**                                 0 - Alignment Bad
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriCoreRxLaneGetStatus(struct file *fileHndl, UINT4 slice, 
                                     UINT4 intClear, UINT4 *pMfErr, 
                                     UINT4 *pCtlWrdErr, UINT4 *pScrbStateErr, 
                                     UINT4 *pCrc32Err,
                                     UINT4 *pWrdSyncChng, UINT4 *pWrdSyncStat, 
                                     UINT4 *pAlignStatus)
{
    UINT4 addr, val;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 1)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* RX_MF_ERR */
    addr = HYPHY20G_CBRI_CORE_REG_RX_MFRM_ERR;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pMfErr = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_BADCTL_ERR */
    addr = HYPHY20G_CBRI_CORE_REG_RX_BAD_CTL_WD_ERR;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCtlWrdErr = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_SCRAM_ERR */
    addr = HYPHY20G_CBRI_CORE_REG_RX_SCR_STAT_ERR;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pScrbStateErr = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_CRC32_ERR */
    addr = HYPHY20G_CBRI_CORE_REG_RX_CRC_ERR;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCrc32Err = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_WRD_SYNC_CHG */
    addr = HYPHY20G_CBRI_CORE_REG_RX_WD_SYNC_CHG;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pWrdSyncChng = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_WRD_SYNC_LANE */
    addr = HYPHY20G_CBRI_CORE_REG_RX_WD_SYNC;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pWrdSyncStat = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_ALIGN_GOOD */
    addr = HYPHY20G_CBRI_CORE_REG_RX_ALGN_STAT_STAT;
    addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pAlignStatus= val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriCoreRxLaneGetStatus */

/*******************************************************************************
**                                                            
**  hyPhy20gCbriCoreDiagLpbk
**  ___________________________________________________________________________ 
**                                                            
**  DESCRIPTION:    This function enable or disable CBRI Interlaken Core 
**                  diagnostic loopback.  Data from Tx is looped back to Rx.
**
**                  The block must be in in 'Major Bidirectional State' of 2 in
**                  order for fields accessed by this API to be modified.
**
**                  Use this API with hyPhy20gCbriIntfEnable as follows:
**                  1. call hyPhy20gCbriIntfEnable with enable set to 0
**                  2. call hyPhy20gCbriCoreDiagLpbk with desired loopback 
**                     setting
**                  3. call hyPhy20gCbriIntfEnable with enable set to 1
**                                                            
**                                                            
**  INPUTS:       *fileHndl     - base address                
**                 slice        - CBRI Interlaken Slice instance
**                                0 - CBRI0
**                                1 - CBRI1
**                 enable       - Looback enable
**                                1 - Loopback enabled
**                                0 - Loopback disabled
**                                                            
**  OUTPUTS:        None.                                     
**                                                            
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriCoreDiagLpbk(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 txRegAddr;
    UINT4 rxRegAddr;
    UINT4 msk;
    UINT4 dataLbMsk;
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
    
    /* Configure transmit control register for loopback feature 
       at 0x495A018 + 0x00800*A(A=0:1) TRANSMIT CONTROL: TX_CTRL */
    regAddr = HYPHY20G_CBRI_CORE_REG_TX_CTL;
    regAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    msk = HYPHY20G_CBRI_CORE_BIT_TX_CTL_TX_DATA_OUT_KILL_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, enable);
    if (result) return result;
        
    /* Enable or disable loopback feature by configuring 
       0x495A010 + 0x00800*A(A=0:1) + 0x200*M(M=0:1) CONFIGURATION: CFG */
    txRegAddr = HYPHY20G_CBRI_CORE_REG_CFG_OFFSET(0);
    txRegAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(txRegAddr, slice); 
    rxRegAddr = HYPHY20G_CBRI_CORE_REG_CFG_OFFSET(1);
    rxRegAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(rxRegAddr, slice);
    
    dataLbMsk = HYPHY20G_CBRI_CORE_BIT_CFG_DATA_TX_TO_RX_LB_MSK;
        
    result = hyPhy20gRegFieldWrite(fileHndl, txRegAddr, dataLbMsk, enable);
    if (result) return result;
        
    result = hyPhy20gRegFieldWrite(fileHndl, rxRegAddr, dataLbMsk, enable);
    if (result) return result;
    
    /* Enable Core Internal Loopback at Shim 0x495A408 + 0x00800*A(A=0:1) */
    regAddr = HYPHY20G_CBRI_SHIM_REG_CORE_LP_BACK;
    regAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(regAddr, slice); 
    msk = HYPHY20G_CBRI_SHIM_BIT_CORE_LP_BACK_INTERNAL_LOOP_BACK_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, enable);
    if (result) return result;
       
    return HYPHY20G_SUCCESS;
} /* hyPhy20gCbriCoreDiagLpbk */

/*******************************************************************************
**
**  hyPhy20gCbriSerdesTestCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the Interlaken Serdes test generator
**                or detector.  This feature allows one of 6 PRBS signals or a 
**                fixed programmable pattern to be generated or detected.  
**
**                For programmable pattern generation, up to 3 - 32 bit patterns  
**                may be generated or detected.  The programmable pattern is  
**                formed as follows:
**
**                Pattern A * Repetition A, followed by Pattern B * Repetition B 
**                followed by Pattern C * Repetition C.
**
**                When Repetion N(A, B or C) is set to 0, Pattern N is not
**                included in the expected pattern stream.
**
**                This function must be called when the CBRI instance is in 
**                Major Bidirection State == 1.  It takes effect when the CBRI
**                instance is transitioned to Major Bidirection State == 2.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - CBRI interface to configure
**                                0 - CBRI 0
**                                1 - CBRI 1
**                 dir          - Direction of travel associated with counters:
**                                0 - Transmit PRBS Generator
**                                1 - Receive PRBS Detector
**                 mode         - Test pattern generation mode
**                                0 - Disabled
**                                1 - PRBS
**                                2 - Programmable Pattern
**                 prbs         - For mode == 1, PRBS type:
**                                0 - PRBS31
**                                1 - PRBS23
**                                2 - PRBS7
**                                3 - PRBS9
**                                4 - PRBS11
**                                5 - PRBS15
**                                6 - PRBS29
**                 patternA     - For mode == 2 32 bit pattern to be repeated  
**                                repA times
**                                0 - 6 (refer to prbs argument for definition)
**                 patternB     - For mode == 2 32 bit pattern to be repeated  
**                                repB times
**                                0 - 6 (refer to prbs argument for definition)
**                 patternC     - For mode == 2 32 bit pattern to be repeated  
**                                repC times
**                                0 - 6 (refer to prbs argument for definition)
**                 repA         - For mode == 2 number of time to repeat 
**                                patternA.  If not to be repeated set to 0.
**                                0 - 0xFF
**                 repB         - For mode == 2 number of time to repeat 
**                                patternB.  If not to be repeated set to 0.
**                                0 - 0xFF
**                 repC         - For mode == 2 number of time to repeat 
**                                patternC.  If not to be repeated set to 0.
**                                0 - 0xFF

**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriSerdesTestCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                               UINT4 mode, UINT4 prbs, UINT4 patternA, 
                               UINT4 patternB, UINT4 patternC, UINT4 repA, 
                               UINT4 repB, UINT4 repC)
{

    UINT4 regData;
    UINT4 prbsVal;
    UINT4 progEnVal;
    UINT4 prbsEnVal;

    UINT4 ctlReg = HYPHY20G_CBRI_CORE_REG_SERDES_TST_CTL_OFFSET(dir);
    UINT4 repCMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_CTL_REPC_MSK;
    UINT4 repBMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_CTL_REPB_MSK;
    UINT4 repAMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_CTL_REPA_MSK;
    UINT4 progEnMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_CTL_PROG_EN_MSK;
    UINT4 prbsEnMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_CTL_PRBS_EN_MSK;

    UINT4 patAReg = HYPHY20G_CBRI_CORE_REG_SERDES_TST_PAT_OFFSET(dir, 0);
    UINT4 patBReg = HYPHY20G_CBRI_CORE_REG_SERDES_TST_PAT_OFFSET(dir, 1);
    UINT4 patCReg = HYPHY20G_CBRI_CORE_REG_SERDES_TST_PAT_OFFSET(dir, 2);
    UINT4 patMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_PAT_TST_PTRN_VALUE_MSK;

    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    /* Note patternA, patternB, patternC not checked in args as legal range
       full range of UINT4 */
    if (slice > 1 || dir > 1 || mode > 2 || prbs > 6 || repA > 0xFF || 
        repB > 0xFF || repC > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register offsets */
    patAReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(patAReg, slice);
    patBReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(patBReg, slice);
    patCReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(patCReg, slice);
    
    /* read SERDES TEST CONTROL register */
    result = sysHyPhy20gRead(fileHndl, ctlReg, &regData);
    if(result) return result;

    if(mode == 2){
        /* set REPA, REPB and REPC */
        result = hyPhy20gVarFieldWrite (&regData, repAMsk, repA);
        if(result) return result;   

        result = hyPhy20gVarFieldWrite (&regData, repBMsk, repB);
        if(result) return result; 

        result = hyPhy20gVarFieldWrite (&regData, repCMsk, repC);
        if(result) return result; 

        /* set TST_PTRN_VALUE for Pattern A, B and C */
        result = hyPhy20gRegFieldWrite(fileHndl, patAReg, patMsk, patternA);
        if(result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, patBReg, patMsk, patternB);
        if(result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, patCReg, patMsk, patternC);
        if(result) return result;
    } else if(mode == 1) {
        /* set TST_PTRN_VALUE for PRBS pattern */
        switch (prbs) {
            case 0: /* 0 - PRBS32 */           
                prbsVal = 0x90000001;
                break;            
            case 1: /* 1 - PRBS23 */ 
                prbsVal = 0x00840001; 
                break;  
            case 2: /* 2 - PRBS7 */
                prbsVal = 0x000000C1; 
                break;  
            case 3: /* 3 - PRBS9 */             
                prbsVal = 0x00000221; 
                break;            
            case 4: /* 4 - PRBS11 */ 
                prbsVal = 0x00000A01; 
                break;  
            case 5: /* 5 - PRBS15 */
                prbsVal = 0x0000C001; 
                break;  
            case 6: /* 6 - PRBS29 */
                prbsVal = 0x28000001; 
                break; 
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }

        result = hyPhy20gRegFieldWrite(fileHndl, patAReg, patMsk, prbsVal);
        if(result) return result;
    }

    switch (mode) {
        case 0: /* 0 - Disabled */            
            progEnVal = 0;
            prbsEnVal = 0; 
            break;            
        case 1: /* 1 - PRBS */ 
            progEnVal = 0;
            prbsEnVal = 1; 
            break;  
        case 2: /* 2 - Programmable Pattern */
            progEnVal = 1;
            prbsEnVal = 0; 
            break;  
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Set PRBS_EN and PROG_EN fields */
    result = hyPhy20gVarFieldWrite (&regData, progEnMsk, progEnVal);
    if(result) return result; 

    result = hyPhy20gVarFieldWrite (&regData, prbsEnMsk, prbsEnVal);
    if(result) return result; 

    /* write SERDES TEST CONTROL register */
    result = sysHyPhy20gWrite(fileHndl, ctlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriSerdesTestCfg */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
