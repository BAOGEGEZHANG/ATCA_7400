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
**  FILE        : hyphy20g_vcat.c
**
**  $Date: 2013-01-17 19:35:54 +0800 (Thu, 17 Jan 2013) $
**
**  $Revision: 30563 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include    "hyphy20g_vcat.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 vcatHyPhy20gVcatDdrCtrlrInit(struct file *fileHndl, UINT4 ddrResMode);

INT4 vcatHyPhy20gVcatDdrPhyInit(struct file *fileHndl);

INT4 vcatHyPhy20gVcatIndMemInit(struct file *fileHndl);

INT4 vcatHyPhy20gVcatCreateCtxt(struct file *fileHndl, UINT4 slice);

INT4 vcatHyPhy20gVcatCreateMemberCtxt(struct file *fileHndl);

INT4 vcatHyPhy20gVcatDestroyMemberCtxt(struct file *fileHndl);

INT4 vcatHyPhy20gVcatAdjustSts1SequenceInfo(struct file *fileHndl,
                                    sHYPHY20G_VCATA_CTXT *pVcatCtxt,
                                    UINT4 direction, UINT4 groupId,
                                    UINT4 sts1Num);

INT4 vcatHyPhy20gVcatAdjustSts3cSequenceInfo(struct file *fileHndl,
                                    sHYPHY20G_VCATA_CTXT *pVcatCtxt,
                                    UINT4 direction, UINT4 groupId,
                                    UINT4 sts3cNum);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gVcatLcasAInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes datapath 'A' of the VCAT/LCAS subsystem for
**                  operation in either SONET or OTN mode.  Datapath
**                  initialization is done by enabling the relevant subsystem
**                  blocks and removing any relevant Software Resets.
**
**                  This function also removes top level reset & low power
**                  if 'enable' = 1, and will apply top level low power
**                  mode if 'enable' = 0 and subsystem 'B' is also disabled.
**
**                  Blocks affected by this procedure are : VCAT Top Level,
**                  TVPCO_0, RVCPO_0, TVCP192_LCAS, RVCP192_LCAS
**
**  INPUTS:         *fileHndl   - base address
**                  mode        - Specifies the desired operating mode for the
**                                subsystem.
**                              - 0 - SONET mode
**                              - 1 - OTN mode
**                  enable      - 0 - Disable Subsystem
**                              - 1 - Enable Subsystem
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasAInit(struct file *fileHndl, UINT4 mode, UINT4 enable)
{

    sHYPHY20G_CTXT *pDevCtxt;

    INT4 result;
    UINT4 slice = 0;
    UINT4 offset = 0;
    UINT4 regAddr = 0;
    UINT4 vcatTiming0 = 0;
    UINT4 vcatRst, vcatLwPwr, rvcpo1Rst, tvcpo1Rst;
    UINT4 tsGrpCfg = 0;
    UINT4 i, j;


    /* argument checking */
    if ((mode > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Retrieve top level reset status */
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                 &vcatRst);
    if(result) return result;

    /* Retrieve top level low power status */
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                                 HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_VCAT_LOWPWR_OFF,
                                 &vcatLwPwr);
    if(result) return result;

    /* Retrieve SubsystemB Reset Status */
    result = sysHyPhy20gBitRead(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCPO_1_RST_OFF,
                                &rvcpo1Rst);
    if (result) return result;

    result = sysHyPhy20gBitRead(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCPO_1_RST_OFF,
                                &tvcpo1Rst);
    if (result) return result;

    if (enable) {
        /* Clear Top level Reset & Low Power Status */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                 0);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                                 HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_VCAT_LOWPWR_OFF,
                                 0);
        if(result) return result;

    } else {
        if ((rvcpo1Rst) && (tvcpo1Rst)) {
            /* If subsystem B is already disabled, reset & issue low power mode
               for VCAT subsystem */
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                     HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                     1);
            if(result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                     HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                     0);
            if(result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                                     HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_VCAT_LOWPWR_OFF,
                                     1);
            if(result) return result;
        }
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Place DDR PHY in reset & Low Power */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_3;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_3_DDR_PHY_RST_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_3_DDR_PHY_RST_D_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_3;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_DDR_PHY_LOWPWR_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* Place TVCP192/RVCP192/TVCPO_0/RVCPO_0/DDR Controller in reset */
    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCP192_RST_OFF,
                                1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCP192_RST_OFF,
                                1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCPO_0_RST_OFF,
                                1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCPO_0_RST_OFF,
                                1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_DSDC_RST_OFF,
                                1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_CFG,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SCH_EN_OFF,
                                0);
    if (result) return result;


    if (enable==1){
        /* Create VCAT level context */
        result = vcatHyPhy20gVcatCreateCtxt(fileHndl, 0);
        if (result) return result;

        /* enable subsystem */
        /* update timing parameters */
        regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_TM_PAR_0;
        result = sysHyPhy20gRead(fileHndl, regAddr, &vcatTiming0);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&vcatTiming0,
                            HYPHY20G_VCAT_LCAS_TOP_BIT_TM_PAR_0_ACK_DLY_MSK,
                            0x1C);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, vcatTiming0);
        if (result) return result;


        if (mode==0){
            /* SONET operation */

            result = sysHyPhy20gBitWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_TOP_REG_CFG,
                                    HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                                    1);
            if (result) return result;


            /* Remove TVCP192/RVCP192 subsystem resets */

            result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCP192_RST_OFF,
                                0);
            if (result) return result;


            result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCP192_RST_OFF,
                                0);
            if (result) return result;


            /* Set GRP_INHIBIT to '1' for all groups in RVCP192 */
            regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_GRP_INH_OFFSET(0);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
            if (result) return result;
            regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_GRP_INH_OFFSET(1);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
            if (result) return result;

        } else {
        /* OTN operation */
            result = sysHyPhy20gBitWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_TOP_REG_CFG,
                                    HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                                    0);
            if (result) return result;

            /* Remove TVCPO_0 / RVCPO_0 subsystem resets */
            result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCPO_0_RST_OFF,
                                0);
            if (result) return result;

            result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCPO_0_RST_OFF,
                                0);
            if (result) return result;

            /* Perform SW reset of RVCPO_0 subsystem */
            result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_SW_RESET_OFF,
                                1);
            if (result) return result;

            result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_SW_RESET_OFF,
                                0);
            if (result) return result;

            /* Enable TVCPO_0/RVCPO_0 subsystems */
            result = sysHyPhy20gBitWrite(fileHndl,
                            HYPHY20G_VCAT_LCAS_TVCPO_REG_CTL,
                            HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_TVCPO_ENABLE_OFF,
                            1);
            if (result) return result;

            result = sysHyPhy20gBitWrite(fileHndl,
                            HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG,
                            HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_RVCPO_ENABLE_OFF,
                            1);
            if (result) return result;

            /* Set GRP_INHIBIT to '1' for all groups in RVCPO_0 */
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0;
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
            if (result) return result;

            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_1;
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
            if (result) return result;

            /* Disable all TribSlots in TVCPO_0 by setting GROUP_ID to 0x3F
               and REQ_SRC_SEL to '11' */

            for (j = 0; j<4; j++) {
                for (i = 0; i<HYPHY20G_VCAT_NUM_TRIBSLOTS; i++) {
                    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_GRP_15_CFG_OFFSET(i);
                    regAddr = regAddr + 0x40*j;
                    result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                                HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_REQ_SRC_SEL_MSK,
                                0x3);
                    if (result) return result;
                    result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                                HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_MSK,
                                0x3F);
                    if (result) return result;
                    result = sysHyPhy20gWrite(fileHndl, regAddr, tsGrpCfg);
                    if (result) return result;
                }
            }

            /* Set all TribSlots in TVCPO_0 to a unique FIFO ID */

            for (i = 0; i<4; i++) {
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_TS_CONFIG0;
                regAddr = regAddr + 4*i;
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x76543210);
                if (result) return result;
            }
            for (i = 0; i<4; i++) {
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_TS_CONFIG1;
                regAddr = regAddr + 4*i;
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFEDCBA98);
                if (result) return result;
            }

            /* Disable all TribSlots in RVCPO_0 by setting GROUP_ID to 0x3F
               and SINK_SEL to '11' */

            tsGrpCfg = 0;

            for (j = 0; j<4; j++) {
                for (i = 0; i<HYPHY20G_VCAT_NUM_TRIBSLOTS/2; i++) {
                    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG + 4*i;
                    regAddr = regAddr + 0x20*j;
                    result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_SINK_SEL_MSK,
                                0x3);
                    if (result) return result;
                    result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_GRP_ID_MSK,
                                0x3F);
                    if (result) return result;
                    result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_SINK_SEL_MSK,
                                0x3);
                    if (result) return result;
                    result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_GRP_ID_MSK,
                                0x3F);
                    if (result) return result;
                    result = sysHyPhy20gWrite(fileHndl, regAddr, tsGrpCfg);
                    if (result) return result;
                }
            }

            /* Set all TribSlots in RVCPO_0 to a unique FIFO ID */

            for (i = 0; i<4; i++) {
                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_TRIB_SLOT_CFG;
                regAddr = regAddr + 8*i;
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFEDCBA98);
                if (result) return result;
                regAddr = regAddr + 4;
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x76543210);
                if (result) return result;
            }

        }
    } else {

        /* Subsystem disabled. Destroy VCAT level context */
        result = vcatHyPhy20gVcatDestroyCtxt(fileHndl, 0);
        if (result) return result;
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasAInit */


/*******************************************************************************
**
**  hyPhy20gVcatLcasBInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes datapath 'B' of the VCAT/LCAS subsystem for
**                  operation in OTN mode.  Datapath initialization is done by
**                  enabling the relevant subsystem blocks and removing any
**                  relevant Software Resets.
**
**                  This function also removes top level reset & low power
**                  if 'enable' = 1, and will apply top level low power
**                  mode if 'enable' = 0 and subsystem 'B' is also disabled.
**
**                  Blocks affected by this procedure are : VCAT Top Level,
**                  TVPCO_1, RVCPO_1.
**
**  INPUTS:         *fileHndl   - base address
**                  enable      - 0 - Disable Subsystem
**                              - 1 - Enable Subsystem
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasBInit(struct file *fileHndl, UINT4 enable)
{
    UINT4 slice = 1;
    UINT4 regAddr = 0;
    UINT4 vcatTiming0 = 0;
    UINT4 vcatRst, vcatLwPwr, rvcp192Rst, tvcp192Rst, rvcp0Rst, tvcp0Rst;
    UINT4 tsGrpCfg = 0;
    UINT4 i, j;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Retrieve top level reset status */
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                 &vcatRst);
    if(result) return result;

    /* Retrieve top level low power status */
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                                 HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_VCAT_LOWPWR_OFF,
                                 &vcatLwPwr);
    if(result) return result;

    /* Retrieve SubsystemA Reset Status */
    result = sysHyPhy20gBitRead(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCP192_RST_OFF,
                                &rvcp192Rst);
    if (result) return result;

    result = sysHyPhy20gBitRead(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCP192_RST_OFF,
                                &tvcp192Rst);
    if (result) return result;

    result = sysHyPhy20gBitRead(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCPO_0_RST_OFF,
                                &rvcp0Rst);
    if (result) return result;

    result = sysHyPhy20gBitRead(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCPO_0_RST_OFF,
                                &tvcp0Rst);
    if (result) return result;

    if (enable) {
        /* Create VCAT level context */
        result = vcatHyPhy20gVcatCreateCtxt(fileHndl, 1);
        if (result) return result;

        /* Clear Top level Reset & Low Power Status */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                 0);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                                 HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_VCAT_LOWPWR_OFF,
                                 0);
        if(result) return result;

    } else {
        if ((rvcp192Rst) && (tvcp192Rst) && (rvcp0Rst) && (tvcp0Rst)) {
            /* If subsystem A is already disabled, reset & issue low power mode
               for VCAT subsystem */
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                     HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                     1);
            if(result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                     HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                                     0);
            if(result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                                     HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_VCAT_LOWPWR_OFF,
                                     1);
            if(result) return result;
        }
    }

    /* Place TVCPO_1 / RVCPO_1 in reset */
    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCPO_1_RST_OFF,
                                1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCPO_1_RST_OFF,
                                1);
    if (result) return result;

    if (enable==1){
        /* enable subsystem */
        /* update timing parameters */
        regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_TM_PAR_0;
        result = sysHyPhy20gRead(fileHndl, regAddr, &vcatTiming0);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&vcatTiming0,
                            HYPHY20G_VCAT_LCAS_TOP_BIT_TM_PAR_0_ACK_DLY_MSK,
                            0x1C);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, vcatTiming0);
        if (result) return result;

        /* Take TVCPO_1 / RVCPO_1 out of reset */
        result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_RVCPO_1_RST_OFF,
                                0);
        if (result) return result;

        result = sysHyPhy20gBitWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                                HYPHY20G_VCAT_LCAS_TOP_BIT_RST_TVCPO_1_RST_OFF,
                                0);
        if (result) return result;

        /* Perform SW Reset of RVCPO_1 */
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_SW_RESET_OFF,
                                1);
        if (result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_SW_RESET_OFF,
                                0);
        if (result) return result;

        /* Enable TVCPO_1/RVCPO_1 subsystems */
        regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
        regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                            HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_TVCPO_ENABLE_OFF,
                            1);
        if (result) return result;

        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                            HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_RVCPO_ENABLE_OFF,
                            1);
        if (result) return result;

        /* Set GRP_INHIBIT to '1' for all groups in RVCPO_1 */
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;

        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_1;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;

        /* Disable all TribSlots in TVCPO_1 by setting GROUP_ID to 0x3F
           and REQ_SRC_SEL to '11' */

        for (j = 0; j<4; j++) {
            for (i = 0; i<HYPHY20G_VCAT_NUM_TRIBSLOTS; i++) {
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_GRP_15_CFG_OFFSET(i);
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
                regAddr = regAddr + 0x40*j;
                result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                            HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_REQ_SRC_SEL_MSK,
                            0x3);
                if (result) return result;
                result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                            HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_MSK,
                            0x3F);
                if (result) return result;
                result = sysHyPhy20gWrite(fileHndl, regAddr, tsGrpCfg);
          }
      }

      /* Set all TribSlots in TVCPO_1 to a unique FIFO ID */

      for (i = 0; i<4; i++) {
          regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_TS_CONFIG0;
          regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
          regAddr = regAddr + 4*i;
          result = sysHyPhy20gWrite(fileHndl, regAddr, 0x76543210);
          if (result) return result;
      }

      for (i = 0; i<4; i++) {
          regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_TS_CONFIG1;
          regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
          regAddr = regAddr + 4*i;
          result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFEDCBA98);
          if (result) return result;
      }

      /* Disable all TribSlots in RVCPO_1 by setting GROUP_ID to 0x3F
         and SINK_SEL to '11' */

      tsGrpCfg = 0;

      for (j = 0; j<4; j++) {
          for (i = 0; i<HYPHY20G_VCAT_NUM_TRIBSLOTS/2; i++) {
              regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG + 4*i;
              regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
              regAddr = regAddr + 0x20*j;
              result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_SINK_SEL_MSK,
                          0x3);
              if (result) return result;
              result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_GRP_ID_MSK,
                          0x3F);
              if (result) return result;
              result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_SINK_SEL_MSK,
                          0x3);
              if (result) return result;
              result = hyPhy20gVarFieldWrite(&tsGrpCfg,
                          HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_GRP_ID_MSK,
                          0x3F);
              if (result) return result;
              result = sysHyPhy20gWrite(fileHndl, regAddr, tsGrpCfg);
              if (result) return result;
          }
      }

      /* Set all TribSlots in RVCPO_1 to a unique FIFO ID */

      for (i = 0; i<4; i++) {
          regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_TRIB_SLOT_CFG;
          regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
          regAddr = regAddr + 8*i;
          result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFEDCBA98);
          if (result) return result;
          regAddr = regAddr + 4;
          result = sysHyPhy20gWrite(fileHndl, regAddr, 0x76543210);
          if (result) return result;
      }
    } else {
        /* Subsystem disabled. Destroy VCAT level context */
        result = vcatHyPhy20gVcatDestroyCtxt(fileHndl, 1);
        if (result) return result;
    }
 return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasBInit */




/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnModeCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Selects between operation in 4xOTU1 mode vs 1xOTU2 mode for
**                  one of the two slices within the VCAT/LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for OTN operating mode.
**
**                  This function also initializes the VCAT_DATA_MODE_SELn field
**                  in ODUKPP subsystem.  The ODUKPP subsystem must have been
**                  taken out of lowpwr/rst state prior to calling this function
**                  with hyPhy20gOdukppDeviceInit.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - Selects which slice to configure
**                              - 0 - Slice 'A' (TVCPO_0, RVCPO_0)
**                              - 1 - Slice 'B' (TVCPO_1, RVCPO_1)
**                  mode        - Select between OTU2 or 4xOTU1 operation
**                              - 0 - OTU2 mode
**                              - 1 - 4xOTU1 mode
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnModeCfg(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    UINT4 offset = 0;
    UINT4 mask = 0;
    UINT4 otnModeValue = 0;
    UINT4 tvcpoControlValue = 0;
    UINT4 regAddr = 0;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if ((mode > 1)||(slice > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Set top level OTN_MODE[1:0] field */
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_OTN_MODE_OFF + slice;
    mask = 1 << offset;

    result = sysHyPhy20gRead(fileHndl, HYPHY20G_VCAT_LCAS_TOP_REG_CFG,
                            &otnModeValue);
    if (result) return result;

    if (mode==0) {
        /* OTU2 operation - clear bit */
        otnModeValue &= ~mask;
    } else {
        /* 4xOTU1 operation  - set bit */
        otnModeValue |= mask;
    }
    result = sysHyPhy20gWrite(fileHndl, HYPHY20G_VCAT_LCAS_TOP_REG_CFG,
                            otnModeValue);
    if (result) return result;


    /* Configure TVCPO/RVCPO OTU2/OTU1 settings */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &tvcpoControlValue);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                            HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_DATA_DELAY_MSK,
                            0x1E);
    if (result) return result;

    if (mode) {
        result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_MSK,
                        0);
        if (result) return result;
    } else  {
        result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_MSK,
                        1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, tvcpoControlValue);
    if (result) return result;


    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                            HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                            mode);
    if (result) return result;

    /* configure the VCAT_DATA_MODE_SELn field of the ODUKPP subsystem
       for the required VCAT slice */
    regAddr = HYPHY20G_ODUKPP_TOP_REG_SYS_SIDE_DAT_SEL;
    switch (slice) {
        case 0:
            mask =
               HYPHY20G_ODUKPP_TOP_BIT_SYS_SIDE_DAT_SEL_VCAT_DATA_MODE_SEL0_MSK;
            break;
        case 1:
            mask =
               HYPHY20G_ODUKPP_TOP_BIT_SYS_SIDE_DAT_SEL_VCAT_DATA_MODE_SEL1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    /* set to 0 for 4 x ODU1 mode, set to 1 or 2 for ODU2 mode (as a function
       of the selected slice).  The ODU2 mode setting is temporary, ODUKPP
       subsystem API will select the appropriate slice */
    val = (mode == 1) ? 0:1;
    val = (val == 1) ? (slice + 1):0;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasOtnModeCfg */


/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnOtu2TransGrpCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures an OTU2 within the VCAT/LCAS subsystem for
**                  transparently mapped data.  Transparently mapped data is
**                  defined as data that is processed by the OTNMA/OTNMD blocks
**                  within the GFP Subsystem.  This can also be thought of as
**                  data that is NOT GFP-mapped.
**
**                  This function should only be called once the associated
**                  slice within VCAT/LCAS subsystem has been initialized and
**                  configured for OTU2 operation.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - Selects which slice to configure
**                              - 0 - Slice 'A' (TVCPO_0, RVCPO_0)
**                              - 1 - Slice 'B' (TVCPO_1, RVCPO_1)
**                  grpId       - Indicates the GID group number to use
**                              - valid values are 0x0 - 0x3F (0-63)
**                  mapping     - 0 - Gsup43 7.1 mapping
**                              - 1 - STS-192c --> OTU2 mapping
**                              - 2 - Other
**                  master      - Select whether TVCPO will act as OTU2 master
**                              - 0 - TVCPO is OTU2 slave
**                              - 1 - TVCPO is OTU2 master
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnOtu2TransGrpCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 grpId, UINT4 mapping, UINT4 master)
{
    UINT4 otu2Value = 0;
    UINT4 regAddr = 0;
    UINT4 tvcpoControlValue = 0;
    UINT4 rvcpoSlice0Config = 0;
    UINT4 mask, offset;
    UINT4 idCode, rev, status;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt;

    /* argument checking */
    if ((slice > 1)||(grpId > 63)||(mapping > 2)||(master > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gCheckRev(fileHndl, &idCode, &rev, &status);
    if (result) return result;

    /* pre-config checking */
    /* check OTU2_OTU1_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check OTU_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

   /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    if (slice == 0) {
        pVcatACtxt = pDevCtxt->pVcatACtxt;
        HYPHY20G_ASSERT((pVcatACtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        pVcatACtxt->vcatGrpSpeedCtxt[grpId] = 192;
        /* Update group type context */
        pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU2;
    } else {
        pVcatBCtxt = pDevCtxt->pVcatBCtxt;
        HYPHY20G_ASSERT((pVcatBCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        pVcatBCtxt->vcatGrpSpeedCtxt[grpId] = 192;
        /* Update group type context */
        pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU2;
    }

    /* Configure TVCPO parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &tvcpoControlValue);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                            HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_GROUPID_MSK,
                            grpId);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_REQ_SRC_SEL_MSK,
                        0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_DATA_SRC_MSK,
                        1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_MASTER_SLAVE_MSK,
                        master);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, tvcpoControlValue);
    if (result) return result;

    /* Configure RVCPO parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rvcpoSlice0Config);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rvcpoSlice0Config,
            HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_SINK_SEL_MSK,
            0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rvcpoSlice0Config,
            HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_DATA_SINK_MSK,
            1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rvcpoSlice0Config,
            HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_GRP_ID_MSK,
            grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, rvcpoSlice0Config);
    if (result) return result;

    /* Configure OPU2_FIXED_STUFF_EN */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OPU2_FIXED_STUFF_EN_OFF;
    if (mapping == 2) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
    }

    /* Set RVCPO UPDATE field */
    if (rev == HYPHY20G_REV_ID_B || rev == HYPHY20G_REV_ID_C || rev == HYPHY20G_REV_ID_D) {
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_OFF;
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                            HYPHY20G_DELAY_1250USEC);
        if (result) return result;
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasOtnOtu2TransGrpCfg */

/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnOtu2GfpGrpCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures an OTU2 within the VCAT/LCAS subsystem for
**                  GFP mapped data.  GFP mapped data may be GFP-T or GFP-F,
**                  and will interface to the VCAT/LCAS susbsystem via the
**                  TXDP64 / RXDP64 blocks within the GFP subsystem.  In this
**                  mode, TVCPO will act as the OTU2 timing master.
**
**                  This function should only be called once the associated
**                  slice within VCAT/LCAS subsystem has been initialized and
**                  configured for OTU2 operation.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - Selects which slice to configure
**                              - 0 - Slice 'A' (TVCPO_0, RVCPO_0)
**                              - 1 - Slice 'B' (TVCPO_1, RVCPO_1)
**                  grpId       - Indicates the GID group number to use
**                              - valid values are 0x0 - 0x3F (0-63)
**                  lan10Ge     - Indicate if mapping is GFP-F based 10GE Lan
**                                (Gsup43 7.3 mapping)
**                              - 0 - Any other GFP mapping
**                              - 1 - GFP-F mapping of 10GE LAN
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnOtu2GfpGrpCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 grpId, UINT4 lan10Ge)
{
    UINT4 otu2Value = 0;
    UINT4 regAddr = 0;
    UINT4 tvcpoControlValue = 0;
    UINT4 rvcpoSlice0Config = 0;
    UINT4 mask, offset;
    UINT4 idCode, rev, status;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt;

    /* argument checking */
    if ((slice > 1)||(grpId > 63)||(lan10Ge > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gCheckRev(fileHndl, &idCode, &rev, &status);
    if (result) return result;

    /* pre-config checking */
    /* check OTU2_OTU1_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check OTU_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    if (slice == 0) {
        pVcatACtxt = pDevCtxt->pVcatACtxt;
        HYPHY20G_ASSERT((pVcatACtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        pVcatACtxt->vcatGrpSpeedCtxt[grpId] = 192;
        /* Update group type context */
        pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU2;
    } else {
        pVcatBCtxt = pDevCtxt->pVcatBCtxt;
        HYPHY20G_ASSERT((pVcatBCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        pVcatBCtxt->vcatGrpSpeedCtxt[grpId] = 192;
        /* Update group type context */
        pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU2;
    }

    /* Configure TVCPO parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &tvcpoControlValue);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                            HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_GROUPID_MSK,
                            grpId);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_REQ_SRC_SEL_MSK,
                        1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_DATA_SRC_MSK,
                        0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_GFP_10GE_LAN_MSK,
                        lan10Ge);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&tvcpoControlValue,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_MASTER_SLAVE_MSK,
                        1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, tvcpoControlValue);
    if (result) return result;

    /* Configure RVCPO parameters */
    /* Configure OPU2_FIXED_STUFF_EN to 0 */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OPU2_FIXED_STUFF_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    if (lan10Ge) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                    HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_DATA_MAPPING_OFF,
                    0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                    HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_DATA_MAPPING_OFF,
                    1);
        if (result) return result;
    }

    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &rvcpoSlice0Config);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rvcpoSlice0Config,
            HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_SINK_SEL_MSK,
            1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rvcpoSlice0Config,
            HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_DATA_SINK_MSK,
            0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rvcpoSlice0Config,
            HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_GRP_ID_MSK,
            grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, rvcpoSlice0Config);
    if (result) return result;

    /* Set RVCPO UPDATE field */
    if (rev == HYPHY20G_REV_ID_B || rev == HYPHY20G_REV_ID_C || rev == HYPHY20G_REV_ID_D) {
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_OFF;
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                            HYPHY20G_DELAY_1250USEC);
        if (result) return result;
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasOtnOtu2GfpGrpCfg */

/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnOtu1LinkMode
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Selects which mapping mode will be used for a slice
**                  within the TVCPO/RVCPO when operating in 4xOTU1 mode.
**
**                  This function should only be called once the associated
**                  slice within VCAT/LCAS subsystem has been initialized and
**                  configured for 4xOTU1 operation.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - Selects which slice to configure
**                              - 0 - slice 'A' (TVCPO_0, RVCPO_0)
**                              - 1 - slice 'B' (TVCPO_1, RVCPO_1)
**                  otu1Num     - Selects which OTU1 within the
**                                TVCPO/RVCPO to configure
**                              - valid values are 0-3
**                  master      - Selects whether the OTU1 within the TVCPO
**                                will operate as master or slave.
**                              - 0 - TVCPO is OTU1 slave
**                              - 1 - TVCPO is OTU1 master
**                  mode        - Specifies which mapping mode will be used for
**                                the OTU1 being configured
**                              - 0 - TS Mode
**                              - 1 - ODU1 Mode
**                              - 2 - VCAT Mode
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnOtu1LinkMode(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 master, UINT4 mode)
{
    UINT4 otu1ModeValue = 0;
    UINT4 regAddr = 0;
    UINT4 otu2Value = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(master > 1)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check OTU2_OTU1_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check OTU_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


   /* Set TVCPO OTU1 SLICEn_MODE and MASTER_SLAVE fields */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    result = hyPhy20gVarFieldWrite(&otu1ModeValue, mask, mode);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_MASTER_SLAVE_OFF + otu1Num;
    mask = 1 << offset;

    result = hyPhy20gVarFieldWrite(&otu1ModeValue, mask, master);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otu1ModeValue);
    if (result) return result;


  /* Set RVCPO OTU1 SLICEn_MODE fields */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_MD;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_MD_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    result = hyPhy20gVarFieldWrite(&otu1ModeValue, mask, mode);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otu1ModeValue);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasOtnOtu1LinkMode */


/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnOtu1TsModeGrpCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures a 'grouping' of one or more Tribslots within the
**                  16-slot OPTM structure of an OTU1.  A 'grouping' may consist
**                  of a single TS1, or up to 15 concatenated TS-1s.  When grouping
**                  all 16 Tribslots into 1 'grouping' (i.e. TS1-16c), it is
**                  recommended to use 'ODU1' mode instead of 'TS' mode.
**
**                  This function can be called several times until all 16
**                  available tribslots have been allocated.  This function
**                  should only be called once the associated slice within the
**                  VCAT/LCAS subsystem has been initialized, configured for
**                  4xOTU1 operation and configured for OTU1 TS mode.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice        - Selects which slice to configure
**                              - 0 - path 'A' (TVCPO_0, RVCPO_0)
**                              - 1 - path 'B' (TVCPO_1, RVCPO_1)
**                  otu1Num     - Selects which OTU1 within the TVCPO/RVCPO to
**                                configure
**                              - valid values are 0-3
**                  grpId       - Indicates the GID group number assigned to the
**                                'grouping'.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  grpSize     - Selects the size of the grouping to use in terms
**                                TS1-Xc, where X is grpSize
**                              - valid values are 0x1 - 0xF (1-15)
**                  startSlot   - Selects which of the 16 tribslots to use as a
**                                'starting' position for the grouping.
**                              - valid values are 0-15 (with exceptions).
**                  dataSrc     - Selects the source for data for this grouping.
**                              - 0 - Transparently mapped data (using OTNMA /
**                                OTNMD)
**                              - 1 - GFP Mapped data (using TXDP64/RXDP64)
**                  tsEnable    - 0 - disables the TS1-Xc
**                              - 1 - enables the TS1-Xc
**                  lnkEnable   - 0 - disables the OTU1 link
**                              - 1 - enables the OTU1 link - Use this only after
**                                    all desired tribslots have been configured.
**
**  COMMENTS:       grpId of 0x3F is not permitted if dataSrc is '0'.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnOtu1TsModeGrpCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 grpId, UINT4 grpSize,
                                 UINT4 startSlot, UINT4 dataSrc, UINT4 tsEnable,
                                 UINT4 lnkEnable)
{
    UINT4 otu2Value = 0;
    UINT4 otu1ModeValue = 0;
    UINT4 groupIdValue = 0;
    UINT4 groupParams = 0;
    UINT4 regAddr = 0;
    UINT4 fifoCaddr = 0;
    UINT4 endSlot = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 enableSlice = 0;
    UINT4 i = 0;
    UINT4 newSpeed = 0;
    UINT4 currentSpeed = 0;
    UINT4 idCode, rev, status;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt;

    endSlot = startSlot + grpSize;

    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(grpId > 63)||(grpSize > 15)||(startSlot > 15)||(grpSize < 1)||(dataSrc > 1)||(lnkEnable > 1)||(tsEnable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((grpId == 0x3F) && (dataSrc == 0)) {
      DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (endSlot > 16) {
      DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gCheckRev(fileHndl, &idCode, &rev, &status);
    if (result) return result;

    /* pre-config checking */
    /* check OTU2_OTU1_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check SLICEn_MODE in TVCPO */

    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    if (mHYPHY20G_GET_FIELD4(otu1ModeValue, mask, offset) != HYPHY20G_VCAT_SLICE_MODE_TS_MODE)
        return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check OTU_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check SLICEn_MODE in RVCPO */

    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_MD;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_MD_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    if (mHYPHY20G_GET_FIELD4(otu1ModeValue, mask, offset) != HYPHY20G_VCAT_SLICE_MODE_TS_MODE)
        return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    if (slice == 0) {
        pVcatACtxt = pDevCtxt->pVcatACtxt;
        HYPHY20G_ASSERT((pVcatACtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        currentSpeed = pVcatACtxt->vcatGrpSpeedCtxt[grpId];
        if (tsEnable) {
            newSpeed = currentSpeed + 3*grpSize;
        } else {
              newSpeed = currentSpeed - 3*grpSize;
        }
        if (newSpeed > 192) return HYPHY20G_ERR_INVALID_PRECONFIG;
        pVcatACtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;
        /* Update group type context */
        if (newSpeed > 0)
        {
            pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_TS;
        }
        else
        {
            pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_UNUSED;
        }

    } else {
        pVcatBCtxt = pDevCtxt->pVcatBCtxt;
        HYPHY20G_ASSERT((pVcatBCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        currentSpeed = pVcatBCtxt->vcatGrpSpeedCtxt[grpId];
        if (tsEnable) {
            newSpeed = currentSpeed + 3*grpSize;
        } else {
              newSpeed = currentSpeed - 3*grpSize;
        }
        if (newSpeed > 192) return HYPHY20G_ERR_INVALID_PRECONFIG;
        pVcatBCtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;
        /* Update group type context */
        if (newSpeed > 0)
        {
            pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_TS;
        }
        else
        {
            pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_UNUSED;
        }

    }

    /* Configure TVCPO FIFO group IDs*/
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_TS_CONFIG0;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    if (startSlot < 8) {
        regAddr = regAddr + 4*otu1Num;
    } else {
        regAddr = regAddr + 4*otu1Num + 0x10;
    }

    result = sysHyPhy20gRead(fileHndl, regAddr, &groupIdValue);
    if (result) return result;

    /* Case where FIFO GroupIDs to be configured reside in 2 separate
       registers */
    if ((startSlot < 8) && (endSlot > 8)) {

        for (i = startSlot; i<8; i++) {
            offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_TS_CONFIG0_SLICE0_TS1_GROUP_OFF + 4*i;
            mask = 0xF << offset;
            if (tsEnable) {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, startSlot);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i);
                if (result) return result;
            }
        }
        result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
        if (result) return result;

        regAddr = regAddr + 0x10;
        result = sysHyPhy20gRead(fileHndl, regAddr, &groupIdValue);
        if (result) return result;

        for (i = 0; i<endSlot-8; i++) {
            offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_TS_CONFIG0_SLICE0_TS1_GROUP_OFF + 4*i;
            mask = 0xF << offset;
            if (tsEnable) {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, startSlot);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i+8);
                if (result) return result;
            }
        }
        result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
        if (result) return result;
    }

    /* Case where FIFO GroupIDs to be configured reside in 1 register */
    else {
        for (i = startSlot; i<endSlot; i++) {
            offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_TS_CONFIG0_SLICE0_TS1_GROUP_OFF + (4*i)%32;
            mask = 0xF << offset;
            if (tsEnable) {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, startSlot);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i);
                if (result) return result;
            }
        }
        result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
        if (result) return result;
    }

    /* Configure TVCPO TS Mode Group parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_GRP_15_CFG_OFFSET(startSlot);
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = regAddr + 0x40*otu1Num;

    /* Determine FIFO_CADDR to use */
    fifoCaddr = 12*endSlot-1;

    for (i = 0; i<grpSize; i++) {
        result = sysHyPhy20gRead(fileHndl, regAddr+4*i, &groupParams);
        if (result) return result;

        /* Configure GRP_ENABLE_SLICE */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_GRP_ENABLE_SLICE0_MSK;
        result = hyPhy20gVarFieldWrite(&groupParams, mask, tsEnable);
        if (result) return result;

        /* Configure REQ_SRC_SEL */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_REQ_SRC_SEL_MSK;
        if (tsEnable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, dataSrc);
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3);
        }
        if (result) return result;

        /* Configure DATA_SRC */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_DATA_SRC_MSK;
        if (dataSrc) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 1);
            if (result) return result;
        }

        /* Configure FIFO_CADDR */

        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_FIFO_CADDR_MSK;
        if (tsEnable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, fifoCaddr);
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x0);
        }
        if (result) return result;

        /* Configure GROUP_ID */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_MSK;
        if (tsEnable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, grpId);
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3F);
        }
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr+4*i, groupParams);
        if (result) return result;
    }

    /* Set TVCPO UPDATE field */
    if (rev == HYPHY20G_REV_ID_B || rev == HYPHY20G_REV_ID_C || rev == HYPHY20G_REV_ID_D) {
        regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_CTL;
        regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_UPDATE_OFF;
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_UPDATE_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                            HYPHY20G_DELAY_1250USEC);
        if (result) return result;
    }

   /* Configure RVCPO FIFO group IDs*/
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_TRIB_SLOT_CFG_1;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    if (startSlot < 8) {
        regAddr = regAddr + 8*otu1Num;
    } else {
        regAddr = regAddr + 8*otu1Num - 4;
    }

    result = sysHyPhy20gRead(fileHndl, regAddr, &groupIdValue);
    if (result) return result;

    /* Case where FIFO GroupIDs to be configured reside in 2 separate
       registers */
    if ((startSlot < 8) && (endSlot > 8)) {

        for (i = startSlot; i<8; i++) {
            offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_TRIB_SLOT_CFG_1_SLICE0_TS1_GRP_OFF + 4*i;
            mask = 0xF << offset;
            if (tsEnable) {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, startSlot);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i);
                if (result) return result;
            }
        }
        result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
        if (result) return result;

        regAddr = regAddr - 4;
        result = sysHyPhy20gRead(fileHndl, regAddr, &groupIdValue);
        if (result) return result;

        for (i = 0; i<endSlot-8; i++) {
            offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_TRIB_SLOT_CFG_SLICE0_TS9_GRP_OFF + 4*i;
            mask = 0xF << offset;
            if (tsEnable) {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, startSlot);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i+8);
                if (result) return result;
            }
        }
        result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
        if (result) return result;
    }

    /* Case where FIFO GroupIDs to be configured reside in 1 register */
    else {
        for (i = startSlot; i<endSlot; i++) {
            offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_TRIB_SLOT_CFG_1_SLICE0_TS1_GRP_OFF + (4*i)%32;
            mask = 0xF << offset;
            if (tsEnable) {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, startSlot);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i);
                if (result) return result;
            }
        }
        result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
        if (result) return result;
    }

    /* Configure RVCPO TS Mode Group parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = regAddr + 0x20*otu1Num;

    /* Determine FIFO_CADDR to use */
    fifoCaddr = 8*endSlot-1;

    for (i = startSlot; i<endSlot; i++) {
        result = sysHyPhy20gRead(fileHndl, regAddr+4*(i/2), &groupParams);
        if (result) return result;

        /* Configure SINK_SEL */
        if (i%2 == 0) {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_SINK_SEL_MSK;
        } else {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_SINK_SEL_MSK;
        }
        if (tsEnable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, dataSrc);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3);
            if (result) return result;
        }

        /* Configure DATA_SRC */
        if (i%2 == 0) {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_DATA_SINK_MSK;
        } else {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_DATA_SINK_MSK;
        }

        if (dataSrc) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 1);
            if (result) return result;
        }

        /* Configure FIFO_CADDR */
        if (i%2 == 0) {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_FIFO_CADDR_MSK;
        } else {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_FIFO_CADDR_MSK;
        }
        if (tsEnable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, fifoCaddr);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x0);
            if (result) return result;
        }

        /* Configure GROUP_ID */
        if (i%2 == 0) {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_GRP_ID_MSK;
        } else {
            mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_GRP_ID_MSK;
        }
        if (tsEnable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, grpId);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3F);
            if (result) return result;
        }
        result = sysHyPhy20gWrite(fileHndl, regAddr+4*(i/2), groupParams);
        if (result) return result;

    }

    /* Configure GRP_ENABLE_SLICE */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ENB_1 + 0x4*(otu1Num/2);
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_GRP_ENB_1_GRP_ENABLE_SLICE0_OFF + 16*(otu1Num%2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &enableSlice);
    if (result) return result;
    for (i = (offset + startSlot); i<(offset + endSlot); i++) {
        mask = 0x1 << i;
        result = hyPhy20gVarFieldWrite(&enableSlice, mask, tsEnable);
        if (result) return result;
   }
    result = sysHyPhy20gWrite(fileHndl, regAddr, enableSlice);
    if (result) return result;

    /* Set RVCPO UPDATE field */
    if (rev == HYPHY20G_REV_ID_B || rev == HYPHY20G_REV_ID_C || rev == HYPHY20G_REV_ID_D) {
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_OFF;
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                            HYPHY20G_DELAY_1250USEC);
        if (result) return result;
    }

    /* Enable/Disable TVCPO slice for this OTU1 */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE_ENABLE_OFF + otu1Num;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, lnkEnable);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasOtnOtu1TsModeGrpCfg */

/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnOtu1Odu1ModeGrpCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures a TS1-16c 'grouping' using all 16 Tribslots
**                  within the 16-slot OPTM structure of an OTU1.
**
**                  This function should only be called once the associated
**                  slice within the VCAT/LCAS subsystem has been initialized,
**                  configured for 4xOTU1 operation and configured for OTU1
**                  ODU1 mode.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - Selects which data path to configure
**                              - 0 - path 'A' (TVCPO_0, RVCPO_0)
**                              - 1 - path 'B' (TVCPO_1, RVCPO_1)
**                  otu1Num     - Selects which OTU1 within the TVCPO/RVCPO to
**                                configure
**                              - valid values are 0-3
**                  grpId       - Indicates the GID group number assigned to the
**                                'grouping'.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  dataSrc     - Selects the source for data for this grouping.
**                              - 0 - Transparently mapped data (using OTNMA /
**                                OTNMD)
**                              - 1 - GFP Mapped data (using TXDP64/RXDP64)
**                  enable      - 0 - disables the OTU1 link
**                              - 1 - enables the OTU1 link
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnOtu1Odu1ModeGrpCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 grpId, UINT4 dataSrc,
                                 UINT4 enable)
{
    UINT4 otu2Value = 0;
    UINT4 otu1ModeValue = 0;
    UINT4 groupParams = 0;
    UINT4 groupIdValue = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 enableSlice = 0;
    UINT4 i = 0;
    UINT4 newSpeed = 0;
    UINT4 currentSpeed = 0;
    UINT4 devId, revId, status;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt;


    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(grpId > 63)||(dataSrc > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;
            
    /* pre-config checking */
    /* check OTU2_OTU1_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check SLICEn_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    if (mHYPHY20G_GET_FIELD4(otu1ModeValue, mask, offset) != HYPHY20G_VCAT_SLICE_MODE_ODU1_MODE)
        return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check OTU_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check SLICEn_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_MD;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_MD_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    if (mHYPHY20G_GET_FIELD4(otu1ModeValue, mask, offset) != HYPHY20G_VCAT_SLICE_MODE_ODU1_MODE)
        return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    if (slice == 0) {
        pVcatACtxt = pDevCtxt->pVcatACtxt;
        HYPHY20G_ASSERT((pVcatACtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        currentSpeed = pVcatACtxt->vcatGrpSpeedCtxt[grpId];
        if (enable) {
            newSpeed = currentSpeed + 48;
        } else {
              newSpeed = currentSpeed - 48;
        }
        if (newSpeed > 192) return HYPHY20G_ERR_INVALID_PRECONFIG;
        pVcatACtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;
        /* Update group type context */
        if (newSpeed > 0)
        {
            pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU1;
        }
        else
        {
            pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_UNUSED;
        }

    } else {
        pVcatBCtxt = pDevCtxt->pVcatBCtxt;
        HYPHY20G_ASSERT((pVcatBCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        currentSpeed = pVcatBCtxt->vcatGrpSpeedCtxt[grpId];
        if (enable) {
            newSpeed = currentSpeed + 48;

        } else {
              newSpeed = currentSpeed - 48;
        }
        if (newSpeed > 192) return HYPHY20G_ERR_INVALID_PRECONFIG;
        pVcatBCtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;
        /* Update group type context */
        if (newSpeed > 0)
        {
            pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU1;
        }
        else
        {
            pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_UNUSED;
        }

    }

    /* Disable TVCPO slice for this OTU1 before configuring */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE_ENABLE_OFF + otu1Num;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);

    /* Configure TVCPO FIFO group IDs to 0 for all 16 slots*/
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_TS_CONFIG0;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = regAddr + 4*otu1Num;

    groupIdValue = 0;

    for (i = 0; i<8; i++) {
        offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_TS_CONFIG0_SLICE0_TS1_GROUP_OFF + 4*i;
        mask = 0xF << offset;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i);
            if (result) return result;
        }
    }
    result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
    if (result) return result;

    regAddr = regAddr + 0x10;
    groupIdValue = 0;

    for (i = 0; i<8; i++) {
        offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_TS_CONFIG1_SLICE0_TS9_GROUP_OFF + 4*i;
        mask = 0xF << offset;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i+8);
            if (result) return result;
        }
    }
    result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
    if (result) return result;

    /* Configure TVCPO TS Mode Group parameters for all 16 slots*/
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_GRP_15_CFG_OFFSET(0);
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = regAddr + 0x40*otu1Num;

    for (i = 0; i<16; i++) {
        groupParams = 0;

        /* Configure GRP_ENABLE_SLICE */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_GRP_ENABLE_SLICE0_MSK;
        result = hyPhy20gVarFieldWrite(&groupParams, mask, enable);
        if (result) return result;

        /* Configure REQ_SRC_SEL */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_REQ_SRC_SEL_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, dataSrc);
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3);
        }
            if (result) return result;

        /* Configure DATA_SRC */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_DATA_SRC_MSK;
        if (dataSrc) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 1);
            if (result) return result;
        }

        /* Configure FIFO_CADDR */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_FIFO_CADDR_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0xBF);
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x0);
        }
        if (result) return result;

        /* Configure GROUP_ID */
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, grpId);
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3F);
        }
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr+4*i, groupParams);
        if (result) return result;
    }

    /* Set TVCPO UPDATE field */
    if (revId == HYPHY20G_REV_ID_B || revId == HYPHY20G_REV_ID_C || revId == HYPHY20G_REV_ID_D) {
        regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_CTL;
        regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_UPDATE_OFF;
        mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_UPDATE_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                           HYPHY20G_DELAY_1250USEC);
       if (result) return result;
    }

   /* Configure RVCPO FIFO group IDs to 0 for all 16 slots*/
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_TRIB_SLOT_CFG_1;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = regAddr + 8*otu1Num;

    groupIdValue = 0;

    for (i = 0; i<8; i++) {
        offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_TRIB_SLOT_CFG_1_SLICE0_TS1_GRP_OFF + 4*i;
        mask = 0xF << offset;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i);
            if (result) return result;
        }
    }
    result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
    if (result) return result;

    regAddr = regAddr - 4;
    groupIdValue = 0;

    for (i = 0; i<8; i++) {
        offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_TRIB_SLOT_CFG_SLICE0_TS9_GRP_OFF + 4*i;
        mask = 0xF << offset;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupIdValue, mask, i+8);
            if (result) return result;
        }
    }
    result = sysHyPhy20gWrite(fileHndl, regAddr, groupIdValue);
    if (result) return result;

    /* Configure RVCPO TS Mode Group parameters  for all 16 slots */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = regAddr + 0x20*otu1Num;

    for (i = 0; i<8; i++) {
        groupParams = 0;

        /* Configure SINK_SEL_0 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_SINK_SEL_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, dataSrc);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3);
            if (result) return result;
        }

        /* Configure DATA_SRC_0 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_DATA_SINK_MSK;
        if (dataSrc) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 1);
            if (result) return result;
        }

        /* Configure FIFO_CADDR_0 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_FIFO_CADDR_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x7F);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x0);
            if (result) return result;
        }

        /* Configure GROUP_ID_0 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_GRP_ID_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, grpId);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3F);
            if (result) return result;
        }

        /* Configure SINK_SEL_1 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_SINK_SEL_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, dataSrc);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3);
            if (result) return result;
        }

        /* Configure DATA_SRC_1 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_DATA_SINK_MSK;
        if (dataSrc) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 1);
            if (result) return result;
        }

        /* Configure FIFO_CADDR_1 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_FIFO_CADDR_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x7F);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x0);
            if (result) return result;
        }

        /* Configure GROUP_ID_1 */
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP1_GRP_ID_MSK;
        if (enable) {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, grpId);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x3F);
            if (result) return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr+4*i, groupParams);
        if (result) return result;
    }

    /* Configure GRP_ENABLE_SLICE */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ENB_1 + 0x4*(otu1Num/2);
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_GRP_ENB_1_GRP_ENABLE_SLICE0_OFF + 16*(otu1Num%2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &enableSlice);
    if (result) return result;
    for (i = offset; i<(offset + 16); i++) {
        mask = 0x1 << i;
        result = hyPhy20gVarFieldWrite(&enableSlice, mask, enable);
        if (result) return result;
   }
    result = sysHyPhy20gWrite(fileHndl, regAddr, enableSlice);
    if (result) return result;

    /* Set RVCPO UPDATE field */
    if (revId == HYPHY20G_REV_ID_B || revId == HYPHY20G_REV_ID_C || revId == HYPHY20G_REV_ID_D) {
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_OFF;
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                            HYPHY20G_DELAY_1250USEC);
        if (result) return result;
    }

    /* Enable/Disable TVCPO slice for this OTU1 */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE_ENABLE_OFF + otu1Num;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, enable);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasOtnOtu1Odu1ModeGrpCfg */

/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnOtu1VcatModeGrpCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures an OTU1 slice within the VCAT/LCAS subsystem
**                  for operation in non-LCAS VCAT mode.  This function can
**                  be called up to 4 times to create an OTU1-4c VCAT group.
**
**                  This function should only be called once the associated
**                  slice within the VCAT/LCAS subsystem has been initialized,
**                  configured for 4xOTU1 operation and configured for OTU1
**                  VCAT mode.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - Selects which data path to configure
**                              - 0 - path 'A' (TVCPO_0, RVCPO_0)
**                              - 1 - path 'B' (TVCPO_1, RVCPO_1)
**                  otu1Num     - Selects which slice within the TVCPO/RVCPO to
**                                configure
**                              - valid values are 0-3
**                  vcatBuf     - Selects which of the 4 available VCAT group
**                                buffers to use for this group.
**                              - valid values are 0-3
**                  grpId       - Indicates the GID group number assigned to the
**                                OTU1.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  seqNo       - Indicates the VCAT sequence number for the
**                                OTU1 being configured.  This value should be
**                                0 for the 1st OTU1 in the group, and should
**                                increment by 1 for each additional member.
**                              - valid values are 0-3
**                  enable      - 0 - disables the OTU1 link
**                              - 1 - enables the OTU1 link
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnOtu1VcatModeGrpCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 vcatBuf, UINT4 grpId,
                                 UINT4 seqNo, UINT4 enable)
{
    UINT4 otu2Value = 0;
    UINT4 otu1ModeValue = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 vcatGid = 0;
    UINT4 groupParams = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 newSpeed = 0;
    UINT4 currentSpeed = 0;
    UINT4 idCode, rev, status;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt;

    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(grpId > 63)||(seqNo > 3)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gCheckRev(fileHndl, &idCode, &rev, &status);
    if (result) return result;

    /* pre-config checking */
    /* check OTU2_OTU1_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check SLICEn_MODE in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    if (mHYPHY20G_GET_FIELD4(otu1ModeValue, mask, offset) != HYPHY20G_VCAT_SLICE_MODE_VCAT_MODE)
        return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check OTU_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                        &otu2Value);
    if (result) return result;
    if (otu2Value == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* pre-config checking */
    /* check SLICEn_MODE in RVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_MD;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeValue);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_MD_SLICE0_MODE_OFF + 2*otu1Num;
    mask = 3 << offset;

    if (mHYPHY20G_GET_FIELD4(otu1ModeValue, mask, offset) != HYPHY20G_VCAT_SLICE_MODE_VCAT_MODE)
        return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    if (slice == 0) {
        pVcatACtxt = pDevCtxt->pVcatACtxt;
        HYPHY20G_ASSERT((pVcatACtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        currentSpeed = pVcatACtxt->vcatGrpSpeedCtxt[grpId];
        if (enable) {
            newSpeed = currentSpeed + 48;
        } else {
              newSpeed = currentSpeed - 48;
        }
        if (newSpeed > 192) return HYPHY20G_ERR_INVALID_PRECONFIG;
        pVcatACtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;
        /* Update group type context */
        if (newSpeed > 0)
        {
            pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU1;
        }
        else
        {
            pVcatACtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_UNUSED;
        }

    } else {
        pVcatBCtxt = pDevCtxt->pVcatBCtxt;
        HYPHY20G_ASSERT((pVcatBCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

        /* Update group speed context */
        currentSpeed = pVcatBCtxt->vcatGrpSpeedCtxt[grpId];
        if (enable) {
            newSpeed = currentSpeed + 48;

        } else {
              newSpeed = currentSpeed - 48;
        }
        if (newSpeed > 192) return HYPHY20G_ERR_INVALID_PRECONFIG;
        pVcatBCtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;
        /* Update group type context */
        if (newSpeed > 0)
        {
            pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_ODU1;
        }
        else
        {
            pVcatBCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_UNUSED;
        }
        
    }

    /* Configure VCAT Group ID in TVCPO */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_GRP_ID;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &vcatGid);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_GRP_ID_VCAT_GROUP0_ID_OFF + 8*vcatBuf;
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&vcatGid, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, vcatGid);
    if (result) return result;

    /* Configure Indirect Link Config Table in TVCPO */
    result = vcatHyPhy20gTVCPOMemIndRead(fileHndl, slice, 0x0,
                                                        otu1Num, &reg0, &reg1);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCPO_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
    if (enable) {
        result = hyPhy20gVarFieldWrite(&reg0, mask, HYPHY20G_VCAT_SLICE_MODE_VCAT_MODE);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&reg0, mask, HYPHY20G_VCAT_SLICE_MODE_UNPROV_MODE);
        if (result) return result;
    }

    mask = HYPHY20G_VCAT_LCAS_TVCPO_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, vcatBuf);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCPO_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_VC_SQ_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, seqNo);
    if (result) return result;

    result = vcatHyPhy20gTVCPOMemIndWrite(fileHndl, slice, 0x0,
                                                        otu1Num, reg0, reg1);
    if (result) return result;

    /* Configure RVCPO Group parameters for the OTU1 */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = regAddr + 0x20*otu1Num;

    groupParams = 0;

    /* Configure SINK_SEL_0 */
    mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_SINK_SEL_MSK;
    result = hyPhy20gVarFieldWrite(&groupParams, mask, 0x1);
    if (result) return result;

    /* Configure DATA_SRC_0 */
    mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_DATA_SINK_MSK;
    result = hyPhy20gVarFieldWrite(&groupParams, mask, 0);
    if (result) return result;

    /* Configure GROUP_ID_0 */
    mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_0_GRP_CFG_SLICE0_GRP0_GRP_ID_MSK;
    result = hyPhy20gVarFieldWrite(&groupParams, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, groupParams);
    if (result) return result;

    /* Set RVCPO UPDATE field */
    if (rev == HYPHY20G_REV_ID_B || rev == HYPHY20G_REV_ID_C || rev == HYPHY20G_REV_ID_D) {
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_CFG;
        regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_OFF;
        mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_UPDATE_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                            HYPHY20G_DELAY_1250USEC);
        if (result) return result;
    }

    /* Configure Indirect Link Config Table in RVCPO */
    result = vcatHyPhy20gRVCPOMemIndRead(fileHndl, slice, 0x0, otu1Num,
                                            &reg0, &reg1, &reg2, &reg3, &reg4);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CFG_TABLE_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, vcatBuf);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CFG_TABLE_IBIT_LNK_CFG_TBL_EXP_SQ_EN_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, 0);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CFG_TABLE_IBIT_LNK_CFG_TBL_MODE_MSK;
    if (enable) {
        result = hyPhy20gVarFieldWrite(&reg0, mask, HYPHY20G_VCAT_SLICE_MODE_VCAT_MODE);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&reg0, mask, HYPHY20G_VCAT_SLICE_MODE_UNPROV_MODE);
        if (result) return result;
    }

    result = vcatHyPhy20gRVCPOMemIndWrite(fileHndl, slice, 0x0, otu1Num,
                                            reg0, reg1, reg2, reg3, reg4);
    if (result) return result;

    /* Configure Indirect Group Configuration Memory in RVCPO */
    result = vcatHyPhy20gRVCPOMemIndRead(fileHndl, slice, 0x1, otu1Num, &reg0, &reg1,
                                    &reg2, &reg3, &reg4);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_GRP_CFG_TABLE_IBIT_GRP_CFG_TBL_MAX_DELAY_MFI_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_GRP_CFG_TABLE_IBIT_GRP_CFG_TBL_MAX_DELAY_OFFSET_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, 0x3FF);
    if (result) return result;

    result = vcatHyPhy20gRVCPOMemIndWrite(fileHndl, slice, 0x1, otu1Num, reg0, reg1,
                                    reg2, reg3, reg4);
    if (result) return result;

    /* Enable/Disable TVCPO slice for this OTU1 */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE_ENABLE_OFF + otu1Num;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, enable);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasOtnOtu1VcatModeGrpCfg */

/*******************************************************************************
**
**  hyPhy20gVcatLcasOtnOtu1VcatSetDefault
**  ___________________________________________________________________________
**
**
**  DESCRIPTION: This function restores the default values for an Odu slice.
**               When changing mode from OPTM to Vcat (for FC400), this function  
**               needs to be called to restore default values to various VCAT 
**               registers.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - Selects which data path to configure
**                           - 0 - path 'A' (TVCPO_0, RVCPO_0)
**                           - 1 - path 'B' (TVCPO_1, RVCPO_1)
**               otu1Num     - Selects which slice within the TVCPO/RVCPO to
**                             configure
**
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasOtnOtu1VcatSetDefault(struct file *fileHndl, 
                                           UINT4 slice, UINT4 otu1Num)
{
    INT4 result;
    UINT4 addr,data;
    UINT4 mask,i;

    if ((slice > 1)||(otu1Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }    
    // TVCPO SLICE MODE CONFIGURATION REGISTER
    addr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG,slice);
    result = sysHyPhy20gRead(fileHndl, addr, &data);
    if (result) return result;
    
    // clear bits to default value 0 (also disable slice)
    mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE_MODE_MSK(otu1Num)
           |(1<<(HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE_ENABLE_OFF+otu1Num));
    data &= (~mask);
    // write values
    // printk("HyPhyMemWrite addr: 0x%08x data: 0x%08x",addr, data);
    result = sysHyPhy20gWrite(fileHndl, addr, data);
    if (result) return result;


    // VCAT GROUP ID REGISTER
    addr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(HYPHY20G_VCAT_LCAS_TVCPO_REG_GRP_ID, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &data);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_GRP_ID_VCAT_GROUP_ID_MSK(otu1Num);
    // clear bits and set to default value
    result= hyPhy20gVarFieldWrite(&data,mask,otu1Num);
    if (result) return result;
    // write values
    // printk("HyPhyMemWrite addr: 0x%08x data: 0x%08x",addr, data);
    result = sysHyPhy20gWrite(fileHndl, addr, data);


    // RVCPO SLICE MODE CONFIGURATION REGISTER
    addr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_MD, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &data);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_BIT_SLC_MD_SLICE_MODE_MSK(otu1Num);
    // clear bits and set to default value
    data &= (~mask);
    // write values
    result = sysHyPhy20gWrite(fileHndl, addr, data);
    if (result) return result;


    // clear slices group configuration TVCPO
    data = 0x0006003F; //default value
    // addr = 0x492042C + slice*0x200 + mapPos*16*4;
    // endAddr = addr + 16*4;
    for (i = 0; i < 16; i++) {
        addr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_GRP_15_CFG_OFFSET(i)+0x40*otu1Num;
        addr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(addr, slice);
        
        result = sysHyPhy20gWrite(fileHndl, addr, data);
        if (result) return result;
    }
    // clear slices group configuration RVCPO
    data = 0xDF80DF80; //default value
    // addr = 0x4920824 + slice*0x200 + mapPos*8*4;
    // endAddr = addr + 8*4;
    for (i = 0; i < 8; i++)
    {
        addr = HYPHY20G_VCAT_LCAS_RVCPO_REG_SLC_0_GRP_CFG + i*0x04;
        addr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(addr, slice);
        addr = addr + 0x20*otu1Num;
        
        result = sysHyPhy20gWrite(fileHndl, addr, data);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;

}



/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetTopCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the top level Sonet/SDH parameters within the
**                  TVCP192_LCAS subsystem and enables the TVCP192_LCAS /
**                  RVCP192_LCAS subsystems.  Optionally initializes the DDR PHY
**                  & DDR Scheduler.  This function should only be called
**                  once the VCAT/LCAS subsystem has been initialized for Sonet
**                  operating mode.
**
**
**  INPUTS:         *fileHndl   - base address
**                  ovrHead     - Selects value to be inserted for SONET/SDH
**                                overhead or fixed stuff byte locations
**                              - valid values are 0x0 - 0xFF
**                  h1ss        - Selects value to be inserted in H1 ss bits
**                              - SDH operation - set to 0x2
**                              - SONET operation - valid values are 0x0 - 0x3
**                  ddrEn       - Selects whether DDR2 SDRAM is present
**                              - 0 - no DDR present - scheduler & PHY will be
**                                    held in reset.
**                              - 1 - DDR present - Scheduler & PHY will be
**                                    enabled using industrial SDRAM Refresh Rate
**                                    with 10k Ohm UDM/LDM termination.
**                              - 2 - DDR present - Scheduler & PHY will be
**                                    enabled using industrial SDRAM Refresh Rate
**                                    with 50 Ohm UDM/LDM termination.
**                                    This configuration provides a more robust
**                                    DDR timing margin compared to ddrEn = 1.
**                              - 3 - DDR present - Scheduler & PHY will be
**                                    enabled using commercial SDRAM Refresh Rate
**                                    with 10k Ohm UDM/LDM termination.
**                              - 4 - DDR present - Scheduler & PHY will be
**                                    enabled using commercial SDRAM Refresh Rate
**                                    with 50 Ohm UDM/LDM termination.
**                                    This configuration provides a more robust
**                                    DDR timing margin compared to ddrEn = 3.
**
**  COMMENTS:       When using ddrEn = 2 or 4, the UDM / LDM pins of the DDR
**                  devices on the board must be pulled to GND using 50 Ohm
**                  resistors.  If UDM / LDM pins are connected to GND using
**                  10k Ohm resistors (as with the original HyPHY20G EVBD),
**                  ddrEn = 1 or 3 must be used.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetTopCfg(struct file *fileHndl, UINT4 ovrHead,
                                 UINT4 h1ss, UINT4 ddrEn)
{
    sHYPHY20G_CTXT *pDevCtxt;

    UINT4 regAddr = 0;
    UINT4 sonetEnValue = 0;
    UINT4 sonetParams = 0;
    UINT4 timingParams1 = 0;
    UINT4 mask = 0;
    UINT4 offset;
    INT4 result;

    /* argument checking */
    if ((ovrHead > 0xFF)||(h1ss > 3)||(ddrEn > 4)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    if (ddrEn) {
        /* Access device level context information */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

        /* Update DDR Timing Parameters for Industrial or Commercial Settings */
        regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_TM_PAR_1;
        result = sysHyPhy20gRead(fileHndl, regAddr, &timingParams1);
        if (result) return result;


        mask = HYPHY20G_VCAT_LCAS_TOP_BIT_TM_PAR_1_REF_NUM_MSK;
        if (ddrEn < 3) {
            result = hyPhy20gVarFieldWrite(&timingParams1, mask, 1);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&timingParams1, mask, 0);
            if (result) return result;
        }

        mask = HYPHY20G_VCAT_LCAS_TOP_BIT_TM_PAR_1_REF_END_DLY_MSK;
        if (ddrEn < 3) {
            result = hyPhy20gVarFieldWrite(&timingParams1, mask, 0x64);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&timingParams1, mask, 0x31);
            if (result) return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr, timingParams1);
        if (result) return result;

        /* Remove Top Level DDR PHY Reset*/
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_3;
        offset = HYPHY20G_TOP_BIT_SYS_SW_RST_3_DDR_PHY_RST_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
        offset = HYPHY20G_TOP_BIT_SYS_SW_RST_3_DDR_PHY_RST_D_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;

        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_3;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_DDR_PHY_LOWPWR_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;

        /* Remove DDR Controller reset */
        result = sysHyPhy20gBitWrite(fileHndl,
                            HYPHY20G_VCAT_LCAS_TOP_REG_RST,
                            HYPHY20G_VCAT_LCAS_TOP_BIT_RST_DSDC_RST_OFF,
                            0);
        if (result) return result;

        /* Initialize DDR SDRAM Controller & PHY */
        result = vcatHyPhy20gVcatDdrPhyInit(fileHndl);
        if (result) return result;

        result = vcatHyPhy20gVcatDdrCtrlrInit(fileHndl, ddrEn%2);
        if (result) return result;

        /* Enable DDR SDRAM Scheduler*/
        result = sysHyPhy20gBitWrite(fileHndl,
                            HYPHY20G_VCAT_LCAS_TOP_REG_CFG,
                            HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SCH_EN_OFF,
                            1);
        if (result) return result;

    }

    /* Enable TVCP192/RVCP192 subsystems */
    result = sysHyPhy20gBitWrite(fileHndl,
                    HYPHY20G_VCAT_LCAS_TVCP_REG_CTL_1,
                    HYPHY20G_VCAT_LCAS_TVCP_BIT_CTL_1_TVCP_ENABLE_OFF,
                    1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl,
                        HYPHY20G_VCAT_LCAS_RVCP_REG_CTL,
                        HYPHY20G_VCAT_LCAS_RVCP_BIT_CTL_RVCP_ENABLE_OFF,
                        1);
    if (result) return result;

    if (ddrEn) {
        /* Initialize TVCP192 and RVCP192 LCAS-related memories */
        result = vcatHyPhy20gVcatIndMemInit(fileHndl);
        if (result) return result;

    }

    /* set PAIS_EN in Top Level to '1' to enable AIS insertion from HOPP */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_PAIS_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* Configure SONET parameters in TVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CTL_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetParams);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_CTL_1_H1_SS_MSK;
    result = hyPhy20gVarFieldWrite(&sonetParams, mask, h1ss);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_CTL_1_OVRHD_PH_MSK;
    result = hyPhy20gVarFieldWrite(&sonetParams, mask, ovrHead);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sonetParams);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetTopCfg */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts192Cfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS / RVCP192_LCAS to output a
**                  single STS-192c link.  This function should only be called
**                  once the VCAT/LCAS subsystem has been initialized for Sonet
**                  operating mode and the top level SONET/SDH parameters have
**                  been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the group number assigned to the
**                                STS-192c.
**                              - valid values are 0x0 - 0x3F (0-63)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts192Cfg(struct file *fileHndl, UINT4 grpId)
{
    UINT4 regAddr = 0;
    UINT4 sonetEnValue = 0;
    UINT4 offset;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;

    /* argument checking */
    if (grpId > 0x3F) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");
    /* Update group speed context */
    pVcatCtxt->vcatGrpSpeedCtxt[grpId] = 192;

    /* Update group speed context */
    pVcatCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_STS192;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* Configure TVCP192 STS192c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, grpId);
    if (result) return result;

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 1);
    if (result) return result;

    /* Configure RVCP192 STS192c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, grpId);
    if (result) return result;

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts192Cfg */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts48Cfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS / RVCP192_LCAS to output an
**                  STS-48c link on one of the four available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-48c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible offsets for
**                                this STS-48c within the 10G (STS-192) output
**                              - valid values are 0-3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts48Cfg(struct file *fileHndl, UINT4 grpId,
                                 UINT4 sts48Num)
{
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 sonetEnValue = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    /* Update group speed context */
    pVcatCtxt->vcatGrpSpeedCtxt[grpId] = 48;

    /* Update group speed context */
    pVcatCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_STS48;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure TVCP192 STS48c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_2;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_2_CHAN_ID_STS48C_0_OFF + 6*sts48Num;
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS48c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    /* Configure RVCP192 STS48c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_2;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_2_CHAN_ID_STS48C_0_OFF + 6*sts48Num;
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS48c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts48Cfg */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts24Cfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS / RVCP192_LCAS to output an
**                  STS-24c link on one of the 8 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-24c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts24Num    - selects one of the 2 possible offsets for
**                                this STS-24c within sts48Num
**                              - valid values are 0x0 - 0x1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts24Cfg(struct file *fileHndl, UINT4 grpId,
                                       UINT4 sts48Num, UINT4 sts24Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts24Off = 2*sts48Num + sts24Num;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    /* Update group speed context */
    pVcatCtxt->vcatGrpSpeedCtxt[grpId] = 24;

    /* Update group speed context */
    pVcatCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_STS24;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure TVCP192 STS24c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_7 + 0x4*(sts48Num/2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_7_CHAN_ID_STS24C_0_OFF + 8*(sts24Off%4);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS24c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;


    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    /* Configure RVCP192 STS24c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_7 + 0x4*(sts48Num/2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_7_CHAN_ID_STS24C_0_OFF + 8*(sts24Off%4);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS24c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;


    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts24Cfg */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts12Cfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS / RVCP192_LCAS to output an
**                  STS-12c link on one of the 16 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-12c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12c within sts48Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts12Cfg(struct file *fileHndl, UINT4 grpId,
                                       UINT4 sts48Num, UINT4 sts12Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts12Off, sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    /* Update group speed context */
    pVcatCtxt->vcatGrpSpeedCtxt[grpId] = 12;

    /* Update group speed context */
    pVcatCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_STS12;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure TVCP192 STS12c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_3 + 4*sts48Num;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_3_CHAN_ID_STS12C_0_OFF + 6*(sts12Num);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS12c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    /* Configure RVCP192 STS12c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_3 + 4*sts48Num;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_3_CHAN_ID_STS12C_0_OFF + 6*(sts12Num);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS12c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts12Cfg */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3Sts1Cfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS / RVCP192_LCAS to output either
**                  an STS-3c or a 3xSTS1 link on one of the 64 available
**                  locations within the 10G (STS-192) output stream.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode and the top
**                  level SONET/SDH parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within the sts12Num
**                              - valid values are 0x0 - 0x3
**                  concat      - 0 - selects a 3xSTS1 output stream
**                              - 1 - selects a 1xSTS3c output stream
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1Cfg(struct file *fileHndl, UINT4 sts48Num,
                                          UINT4 sts12Num, UINT4 sts3Num,
                                          UINT4 concat)
{
    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 sts3Cfg = 0;
    UINT4 sts3Off;
    UINT4 sts12Off;
    UINT4 sts24Off;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 3)||(concat > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure TVCP192 STS192c/48c/12c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    /* Configure TVCP192 STS3 parameters */

    /* Configure STS3_1 value */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_2 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_3 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure RVCP192 STS3 parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    /* Configure STS3_1 value */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_2 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_3 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3Sts1Cfg */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3cTxMemberProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Provisions an STS-3c group member link within the
**                  TVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and an
**                  STS-3c link has been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3c within the sts12Num
**                              - valid values are 0x0 - 0x3
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-3c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  mode        - Operating mode for this member
**                              - 0 - LCAS mapping
**                              - 1 - non-LCAS VCAT mapping
**                              - 2 - transparent mapping
**
**  OUTPUTS:        None
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
INT4 hyPhy20gVcatLcasSonetLinkSts3cTxMemberProv(struct file *fileHndl,
                                                UINT4 sts48Num, UINT4 sts12Num,
                                                UINT4 sts3Num, UINT4 grpId,
                                                UINT4 mode)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    sHYPHY20G_VCAT_STS3c_CTXT *pVcatSts3cCtxt;
    sHYPHY20G_VCAT_MEMBER_CTXT *pVcatSts3cMemberInfo;
    UINT4 vcatGrpTypeCtxt;

    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 sts3Cfg = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 nextSeqNo = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 i = 0;
    UINT4 sts3Off;
    UINT4 sts1Offset;
    UINT4 newSpeed = 0;
    UINT4 currentSpeed = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(grpId > 0x3F)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;

    /* check STS3_1 in TVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    /* retrieve group type context to determine if this is first member added to group 
       if so reset speed context as value may be stale from previous configuration */
    vcatGrpTypeCtxt = (UINT4)pVcatCtxt->vcatGrpTypeCtxt[grpId];
    if(vcatGrpTypeCtxt != HYPHY20G_VCAT_GRP_STS3c){
        pVcatCtxt->vcatGrpSpeedCtxt[grpId] = 0;       
    }

    /* Update group speed context */
    currentSpeed = pVcatCtxt->vcatGrpSpeedCtxt[grpId];
    if (currentSpeed > 189) return HYPHY20G_ERR_INVALID_PRECONFIG;

    newSpeed = currentSpeed + 3;
    pVcatCtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;

    /* Update group speed context */
    pVcatCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_STS3c;

    /* Configure Indirect Link Config Table in TVCP192 */
    sts1Offset = 3*sts3Off;
    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, sts1Offset, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, mode+1);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, grpId);
    if (result) return result;

    /* Set INHIBIT to 0 */
    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_INHIBIT_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, 0);
    if (result) return result;

    if (mode == 2) {
        mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_VC_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
        if (result) return result;
    }

    if (mode == 1) {
        /* Retrieve number of members from VCAT context memory */
        pVcatSts3cCtxt = (sHYPHY20G_VCAT_STS3c_CTXT *)(pVcatCtxt->pVcatSts3cTxCtxt[grpId]);
        for (i=0; i<HYPHY20G_VCAT_NUM_STS3c_MEMBERS; i++) {
            pVcatSts3cMemberInfo = (sHYPHY20G_VCAT_MEMBER_CTXT *)(pVcatSts3cCtxt->pVcatMemberList[i]);
            if (pVcatSts3cMemberInfo->stsNum == HYPHY20G_VCAT_NULL_STSNUM) {
                nextSeqNo = i;
                pVcatSts3cMemberInfo->stsNum = sts1Offset;
                i = HYPHY20G_VCAT_NUM_STS3c_MEMBERS;
            }
        }
        mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_VC_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, nextSeqNo);
        if (result) return result;
    }
    result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Offset, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
    if (result) return result;



    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3cTxMemberProv */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3cRxMemberProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Provisions an STS-3c group member link within the
**                  RVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and an
**                  STS-3c link has been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3c within the sts12Num
**                              - valid values are 0x0 - 0x3
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-3c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  mode        - Operating mode for this member
**                              - 0 - LCAS mapping
**                              - 1 - non-LCAS VCAT mapping
**                              - 2 - transparent mapping
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3cRxMemberProv(struct file *fileHndl,
                                              UINT4 sts48Num, UINT4 sts12Num,
                                              UINT4 sts3Num, UINT4 grpId,
                                              UINT4 mode)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    sHYPHY20G_VCAT_STS3c_CTXT *pVcatSts3cCtxt;
    sHYPHY20G_VCAT_MEMBER_CTXT *pVcatSts3cMemberInfo;

    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 sts3Cfg = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 nextSeqNo = 0;
    UINT4 memberOffset = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 i = 0;
    UINT4 sts3Off;
    UINT4 sts1Offset;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(grpId > 0x3F)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;

    /* check STS3_1 in RVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    sts1Offset = 3*sts3Off;

    /* Configure Indirect Link Config Table in RVCP192 */
    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Offset, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, grpId);
    if (result) return result;

    if (mode == 2) {
        mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
        if (result) return result;

        /* Configure Link Control Packet DCB in RVCP192 to all 0's for this link */
        memberOffset = 64*(48*sts48Num + 12*sts12Num + 3*sts3Num);
        for (i = memberOffset; i<memberOffset + HYPHY20G_VCAT_NUM_GID; i++) {
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x4, i, 0x0, 0x0,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
            if (result) return result;
        }
    }

    if (mode == 1) {
        /* Retrieve number of members from VCAT context memory */
        pVcatSts3cCtxt = (sHYPHY20G_VCAT_STS3c_CTXT *)(pVcatCtxt->pVcatSts3cRxCtxt[grpId]);
        for (i=0; i<HYPHY20G_VCAT_NUM_STS3c_MEMBERS; i++) {
            pVcatSts3cMemberInfo = (sHYPHY20G_VCAT_MEMBER_CTXT *)(pVcatSts3cCtxt->pVcatMemberList[i]);
            if (pVcatSts3cMemberInfo->stsNum == HYPHY20G_VCAT_NULL_STSNUM) {
                nextSeqNo = i;
                pVcatSts3cMemberInfo->stsNum = sts1Offset;
                i = HYPHY20G_VCAT_NUM_STS3c_MEMBERS;
            }
        }
        mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, nextSeqNo);
        if (result) return result;
    }

    /* Set INHIBIT to 0 */
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, 0);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_EN_MSK;
    if (mode >0) {
        result = hyPhy20gVarFieldWrite(&reg0, mask, 1);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&reg0, mask, 0);
        if (result) return result;
    }

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, mode+1);
    if (result) return result;

    result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Offset, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3cRxMemberProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3cTxMemberUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    DeProvisions an STS-3c group member link within the
**                  TVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, an
**                  STS-1 link has been configured, and the sts1 member has
**                  already been provisioned.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3c within the sts12Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3cTxMemberUnProv(struct file *fileHndl,
                                                  UINT4 sts48Num, UINT4 sts12Num,
                                                  UINT4 sts3Num)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;

    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 opMode = 0;
    UINT4 addMode = 0;
    UINT4 groupId = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 sts3Off;
    UINT4 sts1Offset;
    UINT4 newSpeed = 0;
    UINT4 currentSpeed = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;

    /* check STS3_1 in TVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts1Offset = 3*sts3Off;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    /* Determine current operating parameters of the member */
    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, sts1Offset, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_OFF;
    opMode =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_ADD_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_ADD_OFF;
    addMode =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_OFF;
    groupId =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);

    currentSpeed = pVcatCtxt->vcatGrpSpeedCtxt[groupId];
    if (currentSpeed < 3) return HYPHY20G_ERR_INVALID_PRECONFIG;

    switch (opMode) {
        case 0:
            /* Group is not currently provisioned */
            return HYPHY20G_ERR_INVALID_PRECONFIG;
            break;
        case 1:
            /* LCAS Mode Deprovision */
            /* Make sure that 'REMOVE' command has bee initated */
            if (addMode == 1) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            /* Set Prov_MODE to 0 */
            mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;
            result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Offset, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            break;
        case 2:
            /* VCAT Mode Deprovision */
            /* Set Prov_MODE to 0 */
            mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;
            result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Offset, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            result = vcatHyPhy20gVcatAdjustSts3cSequenceInfo(fileHndl, pVcatCtxt,
                                                    1, groupId, sts3Off);
            if (result) return result;
            break;
        case 3:
            /* Transparent Mode Deprovision */
            /* Set Prov_MODE to 0 */
            mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;
            result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Offset, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            break;
    }

    /* Update group speed context */
    newSpeed = currentSpeed - 3;
    pVcatCtxt->vcatGrpSpeedCtxt[groupId] = newSpeed;

    /* Update group speed context */
    if (newSpeed > 0)
    {
        pVcatCtxt->vcatGrpTypeCtxt[groupId] = HYPHY20G_VCAT_GRP_STS3c;
    }
    else
    {
        pVcatCtxt->vcatGrpTypeCtxt[groupId] = HYPHY20G_VCAT_GRP_UNUSED;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3cTxMemberUnProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3cRxMemberUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Deprovisions an STS-3c group member link within the
**                  RVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and an
**                  STS-1 link has been configured, and the sts3c member has
**                  already been provisioned.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3c within sts12Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3cRxMemberUnProv(struct file *fileHndl,
                                                  UINT4 sts48Num, UINT4 sts12Num,
                                                  UINT4 sts3Num)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;

    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 fgActive = 1;
    UINT4 opMode =0;
    UINT4 groupId = 0;
    UINT4 addr = 0;
    UINT4 iwEnb = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 i = 0;
    UINT4 sts3Off;
    UINT4 sts1Off;
    INT4 result;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;

    /* check STS3_1 in RVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    sts1Off = 3*sts3Off;

    /* Determine current operating parameters of the member */
    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_OFF;
    opMode =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_OFF;
    groupId =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);


    switch (opMode) {
        case 0:
            /* Group is not currently provisioned */
            return HYPHY20G_ERR_INVALID_PRECONFIG;
            break;
        case 1:
            /* LCAS Mode Deprovision */

            /* Set FAIL_MST to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_FAIL_MST_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            result = sysHyPhy20gUsDelay(50);
            if (result) return result;

            /* Set INHIBIT to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Wait until FG_ACTIVE = 0 */
            for (i=0; i<10; i++) {
                result = sysHyPhy20gUsDelay(1);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, (2*sts1Off + 1), &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;

                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_MSK;
                offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_OFF;
                fgActive = mHYPHY20G_GET_FIELD4(reg1, mask, offset);
                if (fgActive == 0) {
                    break;
                }
            }
            if (fgActive) {
                return HYPHY20G_ERR_POLL_TIMEOUT;
            }
            /* Set Prov_MODE to 0 */
            result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
            if (result) return result;

            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;

            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* If in interworking mode, reset Link Ctrl Packet DCB
               Memory to 0x00 */
            regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CTL;
            result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCP_BIT_CTL_IW_ENB_OFF,
                        &iwEnb);
            if (result) return result;
            if (iwEnb == 0) {
                for (i=0; i<HYPHY20G_VCAT_NUM_GID; i++) {
                    addr = 64*sts1Off + i;
                    result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x4, addr, 0x0,
                                                        0x0, 0x0, 0x0, 0x0, 0x0,
                                                        0x0, 0x0);
                }
            }
            break;
        case 2:
            /* VCAT Mode Deprovision */
            /* Set INHIBIT to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Wait until FG_ACTIVE = 0 */
            for (i=0; i<10; i++) {
                result = sysHyPhy20gUsDelay(1);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, (2*sts1Off + 1), &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;

                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_MSK;
                offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_OFF;
                fgActive = mHYPHY20G_GET_FIELD4(reg1, mask, offset);
                if (fgActive == 0) {
                    break;
                }
            }
            if (fgActive) {
                return HYPHY20G_ERR_POLL_TIMEOUT;
            }
            /* Set Prov_MODE to 0 */
            result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
            if (result) return result;

            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;

            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Update remaining expected sequence numbers */
            result = vcatHyPhy20gVcatAdjustSts3cSequenceInfo(fileHndl, pVcatCtxt,
                                                    0, groupId, sts3Off);
            if (result) return result;
            break;
        case 3:
            /* Transparent Mode Deprovision */
            /* Set INHIBIT to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Wait until FG_ACTIVE = 0 */
            for (i=0; i<10; i++) {
                result = sysHyPhy20gUsDelay(1);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, (2*sts1Off + 1), &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;

                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_MSK;
                offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_OFF;
                fgActive = mHYPHY20G_GET_FIELD4(reg1, mask, offset);
                if (fgActive == 0) {
                    break;
                }
            }
            if (fgActive) {
                return HYPHY20G_ERR_POLL_TIMEOUT;
            }

            /* Set Prov_MODE to 0 */
            result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
            if (result) return result;

            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;

            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            break;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3cRxMemberUnProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetSts3cLcasMemberAddRemove
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Allows for the provisioning of an LCAS ADD or REMOVE command
**                  for a particular Sts3c Member of a SONET-based group.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and one or
**                  more STS-3c links have been configured for LCAS mode.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3c within sts12Num
**                              - valid values are 0x0 - 0x3
**                  command     - Command to be issued for this member
**                              - 1 - ADD
**                              - 0 - REMOVE
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetSts3cLcasMemberAddRemove (struct file *fileHndl,
                                                    UINT4 sts48Num, UINT4 sts12Num,
                                                    UINT4 sts3Num, UINT4 command)
{


    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 sts3Off;
    UINT4 sts1Offset;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(command > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;

    /* check STS3_1 in TVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_1 in RVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Check that member is configured for LCAS mode */
    sts1Offset = 3*sts3Off;
    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, sts1Offset, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_OFF;
    if (mHYPHY20G_GET_FIELD4(reg0, mask, offset) != 1) {
        /* Member is not in LCAS mode */
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_ADD_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, command);
    if (result) return result;

    result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Offset, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetSts3cLcasMemberAddRemove */




/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts1RxMemberProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Provisions an STS-1 group member link within the
**                  RVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and an
**                  STS-1 link has been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for
**                                this STS-1 within sts3Num
**                              - valid values are 0x0 - 0x2
**                  grpId       - Indicates the GID group number to assign to
**                                the STS-1.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  mode        - Operating mode for this member
**                              - 0 - LCAS mapping
**                              - 1 - non-LCAS VCAT mapping
**                              - 2 - transparent mapping
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts1RxMemberProv(struct file *fileHndl,
                                               UINT4 sts48Num, UINT4 sts12Num,
                                               UINT4 sts3Num, UINT4 sts1Num,
                                               UINT4 grpId,  UINT4 mode)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    sHYPHY20G_VCAT_STS1_CTXT *pVcatSts1Ctxt;
    sHYPHY20G_VCAT_MEMBER_CTXT *pVcatSts1MemberInfo;

    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 nextSeqNo = 0;
    UINT4 memberOffset = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 i = 0;
    UINT4 sts3Off;
    UINT4 sts1Off;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(sts1Num > 0x2)||(grpId > 0x3F)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_1 in RVCP192 */
    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts1Off = 3*sts3Off + sts1Num;

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");


    /* Configure Indirect Link Config Table in RVCP192 */
    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, grpId);
    if (result) return result;

    if (mode == 2) {
        mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
        if (result) return result;

        /* Configure Link Control Packet DCB in RVCP192 to all 0's for this link */
        memberOffset = 64*(48*sts48Num + 12*sts12Num + 3*sts3Num + sts1Num);
        for (i = memberOffset; i<memberOffset + HYPHY20G_VCAT_NUM_GID; i++) {
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x4, i, 0x0, 0x0,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
            if (result) return result;
        }
    }

    if (mode == 1) {
        /* Retrieve number of members from VCAT context memory */
        pVcatSts1Ctxt = (sHYPHY20G_VCAT_STS1_CTXT *)(pVcatCtxt->pVcatSts1RxCtxt[grpId]);
        for (i=0; i<HYPHY20G_VCAT_NUM_STS1_MEMBERS; i++) {
            pVcatSts1MemberInfo = (sHYPHY20G_VCAT_MEMBER_CTXT *)(pVcatSts1Ctxt->pVcatMemberList[i]);
            if (pVcatSts1MemberInfo->stsNum == HYPHY20G_VCAT_NULL_STSNUM) {
                nextSeqNo = i;
                pVcatSts1MemberInfo->stsNum = sts1Off;
                i = HYPHY20G_VCAT_NUM_STS1_MEMBERS;
            }
        }
        mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, nextSeqNo);
        if (result) return result;
    }

    /* Set INHIBIT to 0 */
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, 0);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_EN_MSK;
    if (mode >0) {
        result = hyPhy20gVarFieldWrite(&reg0, mask, 1);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&reg0, mask, 0);
        if (result) return result;
    }

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, mode+1);
    if (result) return result;

    result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts1RxMemberProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts1RxMemberUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Deprovisions an STS-1 group member link within the
**                  RVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and an
**                  STS-1 link has been configured, and the sts1 member has
**                  already been provisioned.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for
**                                this STS-1 within sts3Num
**                              - valid values are 0x0 - 0x2
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts1RxMemberUnProv(struct file *fileHndl,
                                                 UINT4 sts48Num, UINT4 sts12Num,
                                                 UINT4 sts3Num, UINT4 sts1Num)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;

    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 fgActive = 1;
    UINT4 opMode =0;
    UINT4 groupId = 0;
    UINT4 addr = 0;
    UINT4 iwEnb = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 i = 0;
    UINT4 sts3Off;
    UINT4 sts1Off;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(sts1Num > 0x2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_1 in RVCP192 */
    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts1Off = 3*sts3Off + sts1Num;

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");


    /* Determine current operating parameters of the member */
    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_OFF;
    opMode =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_OFF;
    groupId =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);


    switch (opMode) {
        case 0:
            /* Group is not currently provisioned */
            return HYPHY20G_ERR_INVALID_PRECONFIG;
            break;
        case 1:
            /* LCAS Mode Deprovision */

            /* Set FAIL_MST to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_FAIL_MST_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            result = sysHyPhy20gUsDelay(50);
            if (result) return result;

            /* Set INHIBIT to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;

            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Wait until FG_ACTIVE = 0 */
            for (i=0; i<10; i++) {
                result = sysHyPhy20gUsDelay(1);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, (2*sts1Off + 1), &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;

                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_MSK;
                offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_OFF;
                fgActive = mHYPHY20G_GET_FIELD4(reg1, mask, offset);
                if (fgActive == 0) {
                    break;
                }
            }
            if (fgActive) {
                return HYPHY20G_ERR_POLL_TIMEOUT;
            }
            /* Set Prov_MODE to 0 */
            result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
            if (result) return result;

            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;

            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* If in interworking mode, reset Link Ctrl Packet DCB
               Memory to 0x00 */
            regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CTL;
            result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_RVCP_BIT_CTL_IW_ENB_OFF,
                        &iwEnb);
            if (result) return result;
            if (iwEnb == 0) {
                for (i=0; i<HYPHY20G_VCAT_NUM_GID; i++) {
                    addr = 64*sts1Off + i;
                    result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x4, addr, 0x0,
                                                        0x0, 0x0, 0x0, 0x0,
                                                        0x0, 0x0, 0x0);
                }
            }
            break;
        case 2:
            /* VCAT Mode Deprovision */
            /* Set INHIBIT to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Wait until FG_ACTIVE = 0 */
            for (i=0; i<10; i++) {
                result = sysHyPhy20gUsDelay(1);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, (2*sts1Off + 1), &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;

                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_MSK;
                offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_OFF;
                fgActive = mHYPHY20G_GET_FIELD4(reg1, mask, offset);
                if (fgActive == 0) {
                    break;
                }
            }
            if (fgActive) {
                return HYPHY20G_ERR_POLL_TIMEOUT;
            }
            /* Set Prov_MODE to 0 */
            result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
            if (result) return result;

            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;

            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Update remaining expected sequence numbers */
            result = vcatHyPhy20gVcatAdjustSts1SequenceInfo(fileHndl, pVcatCtxt,
                                                    0, groupId, sts1Off);
            if (result) return result;
            break;
        case 3:
            /* Transparent Mode Deprovision */
            /* Set INHIBIT to 1 */
            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_INHIBIT_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x1);
            if (result) return result;
            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;

            /* Wait until FG_ACTIVE = 0 */
            for (i=0; i<10; i++) {
                result = sysHyPhy20gUsDelay(1);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, (2*sts1Off + 1), &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;

                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_MSK;
                offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_OFF;
                fgActive = mHYPHY20G_GET_FIELD4(reg1, mask, offset);
                if (fgActive == 0) {
                    break;
                }
            }
            if (fgActive) {
                return HYPHY20G_ERR_POLL_TIMEOUT;
            }

            /* Set Prov_MODE to 0 */
            result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
            if (result) return result;

            mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;

            result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            break;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts1RxMemberUnProv */



/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts1TxMemberProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Provisions an STS-1 group member link within the
**                  TVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and an
**                  STS-1 link has been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for
**                                this STS-1 within sts3Num
**                              - valid values are 0x0 - 0x2
**                  grpId       - Indicates the GID group number to assign to
**                                the STS-1.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  mode        - Operating mode for this member
**                              - 0 - LCAS mapping
**                              - 1 - non-LCAS VCAT mapping
**                              - 2 - transparent mapping
**
**  OUTPUTS:        None
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
INT4 hyPhy20gVcatLcasSonetLinkSts1TxMemberProv(struct file *fileHndl,
                                               UINT4 sts48Num, UINT4 sts12Num,
                                               UINT4 sts3Num, UINT4 sts1Num,
                                               UINT4 grpId, UINT4 mode)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    sHYPHY20G_VCAT_STS1_CTXT *pVcatSts1Ctxt;
    sHYPHY20G_VCAT_MEMBER_CTXT *pVcatSts1MemberInfo;
    UINT4 vcatGrpTypeCtxt;

    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 nextSeqNo = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 i = 0;
    UINT4 sts3Off;
    UINT4 sts1Off;
    UINT4 newSpeed = 0;
    UINT4 currentSpeed = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(sts1Num > 0x2)||(grpId > 0x3F)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* check STS3_1 in TVCP192 */
    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts1Off = 3*sts3Off + sts1Num;

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    /* retrieve group type context to determine if this is first member added to group 
       if so reset speed context as value may be stale from previous configuration */
    vcatGrpTypeCtxt = (UINT4)pVcatCtxt->vcatGrpTypeCtxt[grpId];
    if(vcatGrpTypeCtxt != HYPHY20G_VCAT_GRP_STS1){
        pVcatCtxt->vcatGrpSpeedCtxt[grpId] = 0;       
    }

    /* Update group speed context */
    currentSpeed = pVcatCtxt->vcatGrpSpeedCtxt[grpId];
    if (currentSpeed > 191) return HYPHY20G_ERR_INVALID_PRECONFIG;

    newSpeed = currentSpeed + 1;
    pVcatCtxt->vcatGrpSpeedCtxt[grpId] = newSpeed;

    /* Update group type context */
    pVcatCtxt->vcatGrpTypeCtxt[grpId] = HYPHY20G_VCAT_GRP_STS1;

    /* Configure Indirect Link Config Table in TVCP192 */
    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, mode+1);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, grpId);
    if (result) return result;

    /* Set INHIBIT to 0 */
    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_INHIBIT_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, 0);
    if (result) return result;

    if (mode == 2) {
        mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_VC_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
        if (result) return result;
    }

    if (mode == 1) {
        /* Retrieve number of members from VCAT context memory */
        pVcatSts1Ctxt = (sHYPHY20G_VCAT_STS1_CTXT *)(pVcatCtxt->pVcatSts1TxCtxt[grpId]);
        for (i=0; i<HYPHY20G_VCAT_NUM_STS1_MEMBERS; i++) {
            pVcatSts1MemberInfo = (sHYPHY20G_VCAT_MEMBER_CTXT *)(pVcatSts1Ctxt->pVcatMemberList[i]);
            if (pVcatSts1MemberInfo->stsNum == HYPHY20G_VCAT_NULL_STSNUM) {
                nextSeqNo = i;
                pVcatSts1MemberInfo->stsNum = sts1Off;
                i = HYPHY20G_VCAT_NUM_STS1_MEMBERS;
            }
        }
        
        mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_VC_SQ_MSK;
        result = hyPhy20gVarFieldWrite(&reg0, mask, nextSeqNo);
        if (result) return result;
    }
    result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts1TxMemberProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts1TxMemberUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    DeProvisions an STS-1 group member link within the
**                  TVCP192_LCAS subsystem.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, an
**                  STS-1 link has been configured, and the sts1 member has
**                  already been provisioned.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for
**                                this STS-1 within sts3Num
**                              - valid values are 0x0 - 0x2
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts1TxMemberUnProv(struct file *fileHndl,
                                                 UINT4 sts48Num, UINT4 sts12Num,
                                                 UINT4 sts3Num, UINT4 sts1Num)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;

    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 opMode = 0;
    UINT4 addMode = 0;
    UINT4 groupId = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 sts3Off;
    UINT4 sts1Off;
    UINT4 newSpeed = 0;
    UINT4 currentSpeed = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(sts1Num > 0x2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_1 in TVCP192 */
    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts1Off = 3*sts3Off + sts1Num;

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    HYPHY20G_ASSERT((pVcatCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pVcatCtxt is NULL.");

    /* Determine current operating parameters of the member */
    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_OFF;
    opMode =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_ADD_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_ADD_OFF;
    addMode =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_OFF;
    groupId =  mHYPHY20G_GET_FIELD4(reg0, mask, offset);
    
    currentSpeed = pVcatCtxt->vcatGrpSpeedCtxt[groupId];
    if (currentSpeed == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


    switch (opMode) {
        case 0:
            /* Group is not currently provisioned */
            return HYPHY20G_ERR_INVALID_PRECONFIG;
            break;
        case 1:
            /* LCAS Mode Deprovision */
            /* Make sure that 'REMOVE' command has been initated */
            if (addMode == 1) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            /* Set Prov_MODE to 0 */
            mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;
            result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            break;
        case 2:
            /* VCAT Mode Deprovision */
            /* Set Prov_MODE to 0 */
            mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;
            result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            result = vcatHyPhy20gVcatAdjustSts1SequenceInfo(fileHndl, pVcatCtxt,
                                                    1, groupId, sts1Off);
            if (result) return result;
            break;
        case 3:
            /* Transparent Mode Deprovision */
            /* Set Prov_MODE to 0 */
            mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
            result = hyPhy20gVarFieldWrite(&reg0, mask, 0x0);
            if (result) return result;
            result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
            if (result) return result;
            break;
    }

    /* Update group speed context */
    newSpeed = currentSpeed - 1;
    pVcatCtxt->vcatGrpSpeedCtxt[groupId] = newSpeed;

    /* Update group speed context */
    if (newSpeed > 0)
    {
    pVcatCtxt->vcatGrpTypeCtxt[groupId] = HYPHY20G_VCAT_GRP_STS1;
    }
    else
    {
        pVcatCtxt->vcatGrpTypeCtxt[groupId] = HYPHY20G_VCAT_GRP_UNUSED;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts1TxMemberUnProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetSts3Sts1GrpDiffDelayCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the maximum allowable differential delay among all
**                  members of a VCAT group.  This function should only be called
**                  once the VCAT/LCAS subsystem  has been initialized for Sonet
**                  operating mode, and the top level SONET/SDH parameters have
**                  been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number being configured.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  grpType     - Indicates the type of group being configured.
**                              - 0 - nxSTS3c group
**                              - 1 - nxSTS1 group
**                              - valid values are 0x0 - 0x3F (0-63)
**                  maxDly      - Maximum allowable differntial delay in mS
**                              - valid values are 0x1 - 0x7F (1-127)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetSts3Sts1GrpDiffDelayCfg(struct file *fileHndl,
                                                  UINT4 grpId, UINT4 grpType,
                                                  UINT4 maxDly)
{

    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 delayMfi2 = 0;
    UINT4 delayOffset = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((maxDly < 1)||(grpId > 0x3F)||(grpType > 1)||(maxDly > 0x7F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure MAX_DELAY parameters in RVCP192 */

    delayMfi2 = maxDly/2;
    if (grpType == 0) {
        delayOffset = (maxDly%2) * 0x249;
    } else {
        delayOffset = (maxDly%2) * 0xBD;
    }

    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x1, grpId, &reg0, &reg1,
                                &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_GRP_CFG_MEM_IBIT_GRP_CFG_MEM_MAX_DELAY_MFI2_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, delayMfi2);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_GRP_CFG_MEM_IBIT_GRP_CFG_MEM_MAX_DELAY_OFFSET_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, delayOffset);
    if (result) return result;

    result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x1, grpId, reg0, reg1,
                                    reg2, reg3, reg4, reg5, reg6, reg7);
    if (result) return result;

    return HYPHY20G_SUCCESS;
}

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetSts1LcasMemberAddRemove
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Allows for the provisioning of an LCAS ADD or REMOVE command
**                  for a particular Sts1 Member of a SONET-based group.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode, the top
**                  level SONET/SDH parameters have been configured, and one or
**                  more STS-1 links have been configured for LCAS mode.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for
**                                this STS-1 within sts3Num
**                              - valid values are 0x0 - 0x2
**                  command     - Command to be issued for this member
**                              - 1 - ADD
**                              - 0 - REMOVE
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetSts1LcasMemberAddRemove (struct file *fileHndl,
                                                   UINT4 sts48Num, UINT4 sts12Num,
                                                   UINT4 sts3Num, UINT4 sts1Num,
                                                   UINT4 command)
{

    UINT4 sonetEnValue = 0;
    UINT4 sts3Cfg = 0;
    UINT4 regAddr = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 sts3Off;
    UINT4 sts1Off;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 0x3)||(sts3Num > 0x3)||(sts1Num > 0x2)||(command > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_1 in TVCP192 */
    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts1Off = 3*sts3Off + sts1Num;

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in TVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_1 in RVCP192 */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_2 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check STS3_3 in RVCP192 */
    regAddr = regAddr + 8;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts3Cfg);
    if (result) return result;
    if (sts3Cfg == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Check that member is configured for LCAS mode */
    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, sts1Off, &reg0, &reg1,
                                    &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_PROV_MODE_OFF;
    if (mHYPHY20G_GET_FIELD4(reg0, mask, offset) != 1) {
        /* Member is not in LCAS mode */
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_ADD_MSK;
    result = hyPhy20gVarFieldWrite(&reg0, mask, command);
    if (result) return result;

    result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, sts1Off, reg0, reg1,
                                        reg2, reg3, reg4, reg5, reg6, reg7);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetSts1LcasMemberAddRemove */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts192TxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to output a
**                  single STS-192c link.  This function should only be called
**                  once the VCAT/LCAS subsystem has been initialized for Sonet
**                  operating mode and the top level SONET/SDH parameters have
**                  been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the group number assigned to the
**                                STS-192c.
**                              - valid values are 0x0 - 0x3F (0-63)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts192TxProv(struct file *fileHndl, UINT4 grpId)
{
    UINT4 regAddr = 0;
    UINT4 sonetEnValue = 0;
    INT4 result;

    /* argument checking */
    if (grpId > 0x3F) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure TVCP192 STS192c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, grpId);
    if (result) return result;

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts192TxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts192RxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to output a
**                  single STS-192c link.  This function should only be called
**                  once the VCAT/LCAS subsystem has been initialized for Sonet
**                  operating mode and the top level SONET/SDH parameters have
**                  been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the group number assigned to the
**                                STS-192c.
**                              - valid values are 0x0 - 0x3F (0-63)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts192RxProv(struct file *fileHndl, UINT4 grpId)
{
    UINT4 regAddr = 0;
    UINT4 sonetEnValue = 0;
    UINT4 offset;
    INT4 result;

    /* argument checking */
    if (grpId > 0x3F) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* Configure RVCP192 STS192c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, grpId);
    if (result) return result;

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts192RxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts192TxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to remove a
**                  single STS-192c link.  This function should only be called
**                  once the VCAT/LCAS subsystem has been initialized for Sonet
**                  operating mode and the top level SONET/SDH parameters have
**                  been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts192TxUnProv(struct file *fileHndl)
{
    UINT4 regAddr = 0;
    UINT4 sonetEnValue = 0;
    INT4 result;

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure TVCP192 STS192c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 0);
    if (result) return result;

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts192TxUnProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts192RxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to remove a
**                  single STS-192c link.  This function should only be called
**                  once the VCAT/LCAS subsystem has been initialized for Sonet
**                  operating mode and the top level SONET/SDH parameters have
**                  been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts192RxUnProv(struct file *fileHndl)
{
    UINT4 regAddr = 0;
    UINT4 sonetEnValue = 0;
    UINT4 offset;
    INT4 result;

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS192c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 0);
    if (result) return result;

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gWrite(fileHndl, regAddr, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts192RxUnProv */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts48TxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to output an
**                  STS-48c link on one of the four available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-48c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible offsets for
**                                this STS-48c within the 10G (STS-192) output
**                              - valid values are 0-3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts48TxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num)
{
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 sonetEnValue = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure TVCP192 STS48c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_2;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_2_CHAN_ID_STS48C_0_OFF + 6*sts48Num;
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS48c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts48TxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts48RxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to output an
**                  STS-48c link on one of the four available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-48c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible offsets for
**                                this STS-48c within the 10G (STS-192) output
**                              - valid values are 0-3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts48RxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num)
{
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 sonetEnValue = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS48c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_2;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_2_CHAN_ID_STS48C_0_OFF + 6*sts48Num;
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS48c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatLcasSonetLinkSts48RxProv */



/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts48TxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to remove an
**                  STS-48c link on one of the four available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible offsets for
**                                this STS-48c within the 10G (STS-192) output
**                              - valid values are 0-3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts48TxUnProv(struct file *fileHndl,
                                            UINT4 sts48Num)
{
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 sonetEnValue = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (sts48Num > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure TVCP192 STS48c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_2;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_2_CHAN_ID_STS48C_0_OFF + 6*sts48Num;
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS48c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts48TxUnProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts48RxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to remove an
**                  STS-48c link on one of the four available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible offsets for
**                                this STS-48c within the 10G (STS-192) output
**                              - valid values are 0-3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts48RxUnProv(struct file *fileHndl, 
                                            UINT4 sts48Num)
{
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 sonetEnValue = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (sts48Num > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS48c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_2;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_2_CHAN_ID_STS48C_0_OFF + 6*sts48Num;
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS48c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatLcasSonetLinkSts48RxUnProv */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts24TxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to output an
**                  STS-24c link on one of the 8 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-24c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts24Num    - selects one of the 2 possible offsets for
**                                this STS-24c within sts48Num
**                              - valid values are 0x0 - 0x1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts24TxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts24Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts24Off = 2*sts48Num + sts24Num;

    /* Configure TVCP192 STS24c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_7 + 0x4*(sts48Num/2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_7_CHAN_ID_STS24C_0_OFF + 8*(sts24Off%4);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS24c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;


    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatLcasSonetLinkSts24TxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts24RxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to output an
**                  STS-24c link on one of the 8 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-24c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts24Num    - selects one of the 2 possible offsets for
**                                this STS-24c within sts48Num
**                              - valid values are 0x0 - 0x1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts24RxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts24Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts24Off = 2*sts48Num + sts24Num;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS24c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_7 + 0x4*(sts48Num/2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_7_CHAN_ID_STS24C_0_OFF + 8*(sts24Off%4);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS24c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;


    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatLcasSonetLinkSts24RxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts24TxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to remove an
**                  STS-24c link on one of the 8 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts24Num    - selects one of the 2 possible offsets for
**                                this STS-24c within sts48Num
**                              - valid values are 0x0 - 0x1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts24TxUnProv(struct file *fileHndl,
                                            UINT4 sts48Num, UINT4 sts24Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts24Off = 2*sts48Num + sts24Num;

    /* Configure TVCP192 STS24c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_7 + 0x4*(sts48Num/2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_7_CHAN_ID_STS24C_0_OFF + 8*(sts24Off%4);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS24c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;


    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts24TxUnProv */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts24RxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to remove an
**                  STS-24c link on one of the 8 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts24Num    - selects one of the 2 possible offsets for
**                                this STS-24c within sts48Num
**                              - valid values are 0x0 - 0x1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts24RxUnProv(struct file *fileHndl,
                                            UINT4 sts48Num, UINT4 sts24Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 3)||(sts24Num > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts24Off = 2*sts48Num + sts24Num;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS24c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_7 + 0x4*(sts48Num/2);
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_7_CHAN_ID_STS24C_0_OFF + 8*(sts24Off%4);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS24c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;


    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts24RxUnProv */



/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts12TxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to output an
**                  STS-12c link on one of the 16 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-12c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12c within sts48Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts12TxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts12Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts12Off, sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure TVCP192 STS12c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_3 + 4*sts48Num;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_3_CHAN_ID_STS12C_0_OFF + 6*(sts12Num);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS12c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatLcasSonetLinkSts12TxProv */


/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts12RxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to output an
**                  STS-12c link on one of the 16 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  grpId       - Indicates the GID group number assigned to the
**                                STS-12c.
**                              - valid values are 0x0 - 0x3F (0-63)
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12c within sts48Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts12RxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts12Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts12Off, sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((grpId > 0x3F)||(sts48Num > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS12c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_3 + 4*sts48Num;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_3_CHAN_ID_STS12C_0_OFF + 6*(sts12Num);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, grpId);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS12c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts12RxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts12TxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to remove an
**                  STS-12c link on one of the 16 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12c within sts48Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts12TxUnProv(struct file *fileHndl, 
                                            UINT4 sts48Num, UINT4 sts12Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts12Off, sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure TVCP192 STS12c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_CHNL_ID_CFG_3 + 4*sts48Num;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_CHNL_ID_CFG_3_CHAN_ID_STS12C_0_OFF + 6*(sts12Num);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure TVCP192 STS12c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatLcasSonetLinkSts12TxUnProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts12RxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to remove an
**                  STS-12c link on one of the 16 available locations within
**                  the 10G (STS-192) output stream.  This function should only
**                  be called once the VCAT/LCAS subsystem has been initialized
**                  for Sonet operating mode and the top level SONET/SDH
**                  parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12c within sts48Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts12RxUnProv(struct file *fileHndl, 
                                            UINT4 sts48Num, UINT4 sts12Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 sts12Off, sts24Off;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 chanId = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 3)||(sts12Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS12c Group ID */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_CHNL_ID_CFG_3 + 4*sts48Num;
    result = sysHyPhy20gRead(fileHndl, regAddr, &chanId);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_CHNL_ID_CFG_3_CHAN_ID_STS12C_0_OFF + 6*(sts12Num);
    mask = 0x3F << offset;
    result = hyPhy20gVarFieldWrite(&chanId, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, chanId);
    if (result) return result;

    /* Configure RVCP192 STS12c parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts12RxUnProv */



/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3Sts1TxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to output either
**                  an STS-3c or a 3xSTS1 link on one of the 64 available
**                  locations within the 10G (STS-192) output stream.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode and the top
**                  level SONET/SDH parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within the sts12Num
**                              - valid values are 0x0 - 0x3
**                  concat      - 0 - selects a 3xSTS1 output stream
**                              - 1 - selects a 1xSTS3c output stream
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1TxProv(struct file *fileHndl, UINT4 sts48Num,
                                             UINT4 sts12Num, UINT4 sts3Num,
                                             UINT4 concat)
{
    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 sts3Cfg = 0;
    UINT4 sts3Off;
    UINT4 sts12Off;
    UINT4 sts24Off;
    INT4 result;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 3)||(concat > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure TVCP192 STS192c/48c/12c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    /* Configure TVCP192 STS3 parameters */

    /* Configure STS3_1 value */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_2 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_3 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3Sts1TxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3Sts1RxProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to output either
**                  an STS-3c or a 3xSTS1 link on one of the 64 available
**                  locations within the 10G (STS-192) output stream.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode and the top
**                  level SONET/SDH parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within the sts12Num
**                              - valid values are 0x0 - 0x3
**                  concat      - 0 - selects a 3xSTS1 output stream
**                              - 1 - selects a 1xSTS3c output stream
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1RxProv(struct file *fileHndl, UINT4 sts48Num,
                                          UINT4 sts12Num, UINT4 sts3Num,
                                          UINT4 concat)
{
    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 sts3Cfg = 0;
    UINT4 sts3Off;
    UINT4 sts12Off;
    UINT4 sts24Off;
    INT4 result;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 3)||(concat > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS3 parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    /* Configure STS3_1 value */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_2 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_3 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    if (concat) {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 0);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3Sts1RxProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3Sts1TxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the TVCP192_LCAS to remove either
**                  an STS-3c or a 3xSTS1 link on one of the 64 available
**                  locations within the 10G (STS-192) output stream.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode and the top
**                  level SONET/SDH parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within the sts12Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1TxUnProv(struct file *fileHndl, UINT4 sts48Num,
                                             UINT4 sts12Num, UINT4 sts3Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 sts3Cfg = 0;
    UINT4 sts3Off;
    UINT4 sts12Off;
    UINT4 sts24Off;
    INT4 result;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure TVCP192 STS192c/48c/12c parameters */

    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    /* Configure TVCP192 STS3 parameters */

    /* Configure STS3_1 value */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_TVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_2 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_3 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3Sts1TxUnProv */

/*******************************************************************************
**
**  hyPhy20gVcatLcasSonetLinkSts3Sts1RxUnProv
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the RVCP192_LCAS to remove either
**                  an STS-3c or a 3xSTS1 link on one of the 64 available
**                  locations within the 10G (STS-192) output stream.  This
**                  function should only be called once the VCAT/LCAS subsystem
**                  has been initialized for Sonet operating mode and the top
**                  level SONET/SDH parameters have been configured.
**
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3
**                  sts12Num    - selects one of the 4 possible offsets for
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3
**                  sts3Num     - selects one of the 4 possible offsets for
**                                this STS-3 within the sts12Num
**                              - valid values are 0x0 - 0x3
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1RxUnProv(struct file *fileHndl, UINT4 sts48Num,
                                          UINT4 sts12Num, UINT4 sts3Num)
{
    UINT4 sonetEnValue = 0;
    UINT4 regAddr = 0;
    UINT4 payloadCfg = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 sts3Cfg = 0;
    UINT4 sts3Off;
    UINT4 sts12Off;
    UINT4 sts24Off;
    INT4 result;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check SONET_EN in VCAT_LCAS top level */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
    if (result) return result;
    if (sonetEnValue == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts12Off = 4*sts48Num + sts12Num;
    sts24Off = 2*sts48Num + sts12Num/2;

    /* Configure Top Level STS192C bit */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure RVCP192 STS3 parameters */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_1;
    result = sysHyPhy20gRead(fileHndl, regAddr, &payloadCfg);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_192C_MSK;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_48C_OFF + sts48Num;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_24C_OFF + sts24Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_1_STS_12C_OFF + sts12Off;
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&payloadCfg, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, payloadCfg);
    if (result) return result;

    /* Configure STS3_1 value */
    regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_PYLD_CFG_2_OFFSET(sts3Off/32);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    offset = HYPHY20G_VCAT_LCAS_RVCP_BIT_PYLD_CFG_2_STS3_1_OFF + (sts3Off%32);
    mask = 1 << offset;
    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_2 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    /* Configure STS3_3 value */
    regAddr = regAddr + 8;
    result = sysHyPhy20gRead(fileHndl, regAddr, &sts3Cfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&sts3Cfg, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sts3Cfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatLcasSonetLinkSts3Sts1RxUnProv */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  vcatHyPhy20gVcatDdrCtrlrInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function initializes all the registers associated
**                  with the DDR2 SDRAM  Controller used for SONET based
**                  VCAT operation
**
**  INPUTS:     *fileHndl   - base address
**              ddrResMode  - 0 = 50 Ohm UDM/LDM termination
**                          - 1 = 10k Ohm UDM/LDM termination
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatDdrCtrlrInit(struct file *fileHndl, UINT4 ddrResMode)
{
    INT4 result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_0, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_1, 0x1000101);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_2, 0x1000000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_3, 0x1000100);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_4, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_5, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_6, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_7, 0x1000000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_8, 0x101);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_9, 0x3000003);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_10, 0x50200);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_11, 0x30000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_12, 0x4030304);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_13, 0x20A0A0A);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_14, 0xB000000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_15, 0x2030704);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_16, 0x6050000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                            HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_17, 0x11000000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_18, 0x1602);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_19, 0x5120000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_20, 0x33);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_23, 0xC2D0000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_25, 0xA00000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_26, 0x200C8);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_27, 0x376D56);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_28, 0xC8);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_29, 0x13880);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_31, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                    HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_35, 0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_57, 0x40000);
    if (result) return result;

    if (ddrResMode == 1) {
        result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_58, 0x5);
        if (result) return result;
    } else {
        result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_58, 0x4405);
        if (result) return result;
    }
    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_59, 0xA520000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_67, 0x2002000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_68, 0x20000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_68, 0x220000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_68, 0x20000);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_7, 0x1010000);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gVcatDdrCtrlrInit */


/*******************************************************************************
**
**  vcatHyPhy20gVcatDdrPhyInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function initializes all the registers associated
**                  with the DDR2 SDRAM PHY used for SONET based
**                  VCAT operation
**
**  INPUTS:     *fileHndl   - base address
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatDdrPhyInit(struct file *fileHndl)
{
    INT4 result;

    result = sysHyPhy20gWrite(fileHndl,
                            HYPHY20G_DDR2_PHY_CORE_REG_PDL_CRS_OFF_1, 0x1489);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                                HYPHY20G_DDR2_PHY_CORE_REG_PDL_CRS_OFF_2, 0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                            HYPHY20G_DDR2_PHY_CORE_REG_PDL_PVT_CMP_TH, 0x7224);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                            HYPHY20G_DDR2_PHY_CORE_REG_PVT_CMP_CFG_2, 0x4447);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                    HYPHY20G_DDR2_PHY_CORE_REG_DQSR_GATE_ADJ_OFF_CFG, 0x1837);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSWC_ADJ_OFF_CFG_OFFSET(0), 0x363B);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSWC_ADJ_OFF_CFG_OFFSET(1), 0x363A);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSWC_ADJ_OFF_CFG_OFFSET(2), 0x373C);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSWC_ADJ_OFF_CFG_OFFSET(3), 0x363B);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSWC_ADJ_OFF_CFG_OFFSET(4), 0x3639);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSRC_ADJ_OFF_CFG_OFFSET(0), 0x393B);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSRC_ADJ_OFF_CFG_OFFSET(1), 0x3839);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSRC_ADJ_OFF_CFG_OFFSET(2), 0x393A);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSRC_ADJ_OFF_CFG_OFFSET(3), 0x393B);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                HYPHY20G_DDR2_PHY_CORE_REG_DQSRC_ADJ_OFF_CFG_OFFSET(4), 0x393B);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                           HYPHY20G_DDR2_PHY_CORE_REG_CLKA_ADJ_OFF_CFG, 0xF02);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                             HYPHY20G_DDR2_PHY_CORE_REG_CA_ADJ_OFF_CFG, 0x0436);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                            HYPHY20G_DDR2_PHY_CORE_REG_CTL_ADJ_OFF_CFG, 0x0436);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                            HYPHY20G_DDR2_PHY_CORE_REG_CFG_1, 0x8003);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl,
                            HYPHY20G_DDR2_PHY_CORE_REG_CFG_2, 0x20F);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gVcatDdrPhyInit */

/*******************************************************************************
**
**  vcatHyPhy20gVcatIndMemInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function initializes all the necessary indirect
**                  memories associated with VCAT based SONET operation, which
**                  are only used when DDR2 SDRAM is present.
**
**  INPUTS:     *fileHndl   - base address
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatIndMemInit(struct file *fileHndl)
{
    INT4 result;
    UINT4 wordOffsetMax = 24;
    UINT4 gidSeed;
    UINT4 i = 0;

    /* Initialize TVCP192 FE_SINK_MST_BUFFER to 0xFF for all members of all
       groups*/
    for (i = 0; i<wordOffsetMax*HYPHY20G_VCAT_NUM_GID; i++) {
        result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x1, i, 0xFF, 0x0,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
        if (result) return result;
    }

    /* Initialize TVCP192 GROUP_CTXT_TABLE page 0 to 0x0 for all groups */
    for (i = 0; i<HYPHY20G_VCAT_NUM_GID; i++) {
        result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x2, 3*i, 0x0, 0x0,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
        if (result) return result;
    }

    /* Initialize TVCP192 GROUP_CTXT_TABLE page 1 (GID_SEED) with a unique value
       for each group */
    for (i = 0; i<HYPHY20G_VCAT_NUM_GID; i++) {
        gidSeed = (i<<8) + i;
        result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x2, (3*i) + 2, gidSeed,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
        if (result) return result;
    }

    /* Initialize RVCP192 NE_GRP_MST to 0xFF for all groups */
    for (i = 0; i<HYPHY20G_VCAT_NUM_GID; i++) {
        gidSeed = (i<<8) + i;
        result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0xB, i, 0xFF,
                                             0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
        if (result) return result;
    }


    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gVcatIndMemInit */

/*******************************************************************************
**
**  vcatHyPhy20gVcatCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates memory for tracking VCAT member context, and 
**                  initialize the VCAT STS1 & STS3c Memory Context.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - 0 = Slice A, 1 = Slice B
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatCreateCtxt(struct file *fileHndl, UINT4 slice)
{
    UINT4 size;
    UINT4 i;
    INT4 result;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt; 

    /* Argument checking */
        if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (slice == 0) {
        /* allocate memory to VCAT Slice A level context information */
        /* if structure already exists destroy previous to prevent memory
           leak */
        if (pDevCtxt->pVcatACtxt != NULL){
            DBG_PRINT("Existing VcatA Context memory detected, destroy it before allocation.\n");
            vcatHyPhy20gVcatDestroyCtxt(fileHndl, slice);
        }

        /* allocate memory for VCAT Top Level Context*/
        size = sizeof(sHYPHY20G_VCATA_CTXT);
        pDevCtxt->pVcatACtxt =(sHYPHY20G_VCATA_CTXT *)sysHyPhy20gMemAlloc(size);

        if(pDevCtxt->pVcatACtxt == NULL) {
            return HYPHY20G_ERR_MEM_ALLOC;
        }
        DBG_PRINT("VcatA Context memory allocated:addr=0x%x size=%ld\n",
                  (unsigned int)pDevCtxt->pVcatACtxt, size); 
        
        pVcatACtxt = pDevCtxt->pVcatACtxt;
        /* allocate memory for each pointer array within the data structure
        ** memory organized:
        **    *pVcatACtxt
        **       sHYPHY20G_VCAT_STS1_CTXT *pVcatSts1TxCtxt[64]
        **       sHYPHY20G_VCAT_STS1_CTXT *pVcatSts1RxCtxt[64]
        **       sHYPHY20G_VCAT_STS3c_CTXT *pVcatSts3cTxCtxt[64]
        **       sHYPHY20G_VCAT_STS3c_CTXT *pVcatSts3cRxCtxt[64]
        */
        size = 128*(sizeof(sHYPHY20G_VCAT_STS1_CTXT) + sizeof(sHYPHY20G_VCAT_STS3c_CTXT));
        
        pVcatACtxt->pVcatSts1TxCtxt[0] = 
                          (sHYPHY20G_VCAT_STS1_CTXT *)sysHyPhy20gMemAlloc(size);
        if (pVcatACtxt->pVcatSts1TxCtxt[0] == NULL) {
            /* deallocate memory allocated up to this point in function */
            DBG_PRINT("file %s, line %d, - HYPHY20G_ERR_MEM_ALLOC, size=%ld\n",
                      __FILE__, __LINE__, (long int)size);
            sysHyPhy20gMemFree(pDevCtxt->pVcatACtxt);
            return HYPHY20G_ERR_MEM_ALLOC;
        }

        DBG_PRINT("memory allocated for pVcatACtxt->pVcatSts1TxCtxt[0] :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts1TxCtxt[0], size);
        /* Assign addr to pointer array pVcatSts1TxCtxt[i] */
        for (i = 1; i<64; i++) {
            pVcatACtxt->pVcatSts1TxCtxt[i] = 
             (sHYPHY20G_VCAT_STS1_CTXT *)(pVcatACtxt->pVcatSts1TxCtxt[i-1] + 1);
        }
        DBG_PRINT("Assigned pVcatACtxt->pVcatSts1TxCtxt[63] to :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts1TxCtxt[63], (long int)sizeof(sHYPHY20G_VCAT_STS1_CTXT));
        
        /* Assign addr to pointer array pVcatSts1RxCtxt[i] */
        pVcatACtxt->pVcatSts1RxCtxt[0] = 
              (sHYPHY20G_VCAT_STS1_CTXT *)(pVcatACtxt->pVcatSts1TxCtxt[63] + 1);
        DBG_PRINT("Assigned pVcatACtxt->pVcatSts1RxCtxt[0] to :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts1RxCtxt[0], (long int)sizeof(sHYPHY20G_VCAT_STS1_CTXT));
        
        for (i = 1; i<64; i++) {
            pVcatACtxt->pVcatSts1RxCtxt[i] = 
            (sHYPHY20G_VCAT_STS1_CTXT *)(pVcatACtxt->pVcatSts1RxCtxt[i-1] + 1);
        }
        DBG_PRINT("Assigned pVcatACtxt->pVcatSts1RxCtxt[63] to :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts1RxCtxt[63], (long int)sizeof(sHYPHY20G_VCAT_STS1_CTXT));
        
        /* Assign addr to pointer array pVcatSts3cTxCtxt[i] */
        pVcatACtxt->pVcatSts3cTxCtxt[0] = 
             (sHYPHY20G_VCAT_STS3c_CTXT *)(pVcatACtxt->pVcatSts1RxCtxt[63] + 1);
        DBG_PRINT("Assigned pVcatACtxt->pVcatSts3cTxCtxt[0] to :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts3cTxCtxt[0], (long int)sizeof(sHYPHY20G_VCAT_STS3c_CTXT));
        for (i = 1; i<64; i++) {
            pVcatACtxt->pVcatSts3cTxCtxt[i] =
           (sHYPHY20G_VCAT_STS3c_CTXT *)(pVcatACtxt->pVcatSts3cTxCtxt[i-1] + 1);
        }
        DBG_PRINT("Assigned pVcatACtxt->pVcatSts3cTxCtxt[63] to :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts3cTxCtxt[63], (long int)sizeof(sHYPHY20G_VCAT_STS3c_CTXT));
        
        /* Assign addr to pointer array pVcatSts3cRxCtxt[i] */
        pVcatACtxt->pVcatSts3cRxCtxt[0] =
            (sHYPHY20G_VCAT_STS3c_CTXT *)(pVcatACtxt->pVcatSts3cTxCtxt[63] + 1);
        DBG_PRINT("Assigned pVcatACtxt->pVcatSts3cRxCtxt[0] to :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts3cRxCtxt[0], (long int)sizeof(sHYPHY20G_VCAT_STS3c_CTXT));
        for (i =1; i<64; i++) {
            pVcatACtxt->pVcatSts3cRxCtxt[i] = 
           (sHYPHY20G_VCAT_STS3c_CTXT *)(pVcatACtxt->pVcatSts3cRxCtxt[i-1] + 1);
        }
        DBG_PRINT("Assigned pVcatACtxt->pVcatSts3cRxCtxt[63] to :addr=0x%x size=%ld\n",
                  (unsigned int)pVcatACtxt->pVcatSts3cRxCtxt[63], (long int)sizeof(sHYPHY20G_VCAT_STS3c_CTXT));

        /* initialize Group Speed Context to 0 */
        for (i=0; i<HYPHY20G_VCAT_NUM_GID; i++) {
            pDevCtxt->pVcatACtxt->vcatGrpSpeedCtxt[i] = 0;
        }
        
        /* initialize Group Type Context to 0 */
        for (i=0; i<HYPHY20G_VCAT_NUM_GID; i++) {
            pDevCtxt->pVcatACtxt->vcatGrpTypeCtxt[i] = HYPHY20G_VCAT_GRP_UNUSED;
        }

        /* Allocate the VCAT Member Context */
        result = vcatHyPhy20gVcatCreateMemberCtxt(fileHndl);
        if (result) {
            sysHyPhy20gMemFree(pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0]);
            sysHyPhy20gMemFree(pDevCtxt->pVcatACtxt);
            return result;
        }        
    } else {
        /* allocate memory to VCAT Slice B level context information */
        /* if structure already exists destroy previous to prevent memory
           leak */
        if (pDevCtxt->pVcatBCtxt != NULL){
            DBG_PRINT("Existing VcatA Context memory detected, destroy it before allocation.\n");
            vcatHyPhy20gVcatDestroyCtxt(fileHndl, slice);
        }

        /* allocate memory for VCAT Top Level Context*/
        size = sizeof(sHYPHY20G_VCATB_CTXT);
        pDevCtxt->pVcatBCtxt =(sHYPHY20G_VCATB_CTXT *)sysHyPhy20gMemAlloc(size);

        if(pDevCtxt->pVcatBCtxt == NULL) {
            return HYPHY20G_ERR_MEM_ALLOC;
        }
        DBG_PRINT("VcatB Context memory allocated:addr=0x%x size=%ld\n",
                  (unsigned int)pDevCtxt->pVcatBCtxt, size);

        
        /* initialize Group Speed Context to 0 */
        for (i=0; i<HYPHY20G_VCAT_NUM_GID; i++) {
            pDevCtxt->pVcatBCtxt->vcatGrpSpeedCtxt[i] = 0;
        }
        
        /* initialize Group Type Context to 0 */
        for (i=0; i<HYPHY20G_VCAT_NUM_GID; i++) {
            pDevCtxt->pVcatBCtxt->vcatGrpTypeCtxt[i] = HYPHY20G_VCAT_GRP_UNUSED;
        }
    } 
    return HYPHY20G_SUCCESS;
    
} /* vcatHyPhy20gVcatCreateCtxt */


/*******************************************************************************
**
**  vcatHyPhy20gVcatCreateMemberCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes the VCAT Member Context for the STS1 and STS3c
**                  memories.
**
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
INT4 vcatHyPhy20gVcatCreateMemberCtxt(struct file *fileHndl)
{
    UINT4 size;
    UINT4 i, j;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatFullCtxt;
    sHYPHY20G_VCAT_MEMBER_CTXT *p;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pVcatFullCtxt = pDevCtxt->pVcatACtxt;
    /* confirm all pointers that reference memory allocated by this function are 
       not NULL */
    for (i=0; i<HYPHY20G_VCAT_NUM_GID; i++) {
        if (pVcatFullCtxt->pVcatSts1TxCtxt[i]  == NULL) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
        if (pVcatFullCtxt->pVcatSts1RxCtxt[i]  == NULL) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
        if (pVcatFullCtxt->pVcatSts3cTxCtxt[i]  == NULL) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
        if (pVcatFullCtxt->pVcatSts3cRxCtxt[i]  == NULL) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }
    
    /* Allocate memory for sHYPHY20G_VCAT_MEMBER_CTXT storage to 
       pVcatSts1TxCtxt[0]->pVcatMemberList[0] */
    size = 64*(192+192+64+64)* sizeof(sHYPHY20G_VCAT_MEMBER_CTXT);
    pVcatFullCtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0]=
                        (sHYPHY20G_VCAT_MEMBER_CTXT *)sysHyPhy20gMemAlloc(size);
                                             
    if (pVcatFullCtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0] == NULL) {
        DBG_PRINT("file %s, line %d, - HYPHY20G_ERR_MEM_ALLOC, size=%ld\n",__FILE__, __LINE__, (long int)size);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    DBG_PRINT("pVcatSts1TxCtxt[0]->pVcatMemberList[0] Context memory allocated:addr=0x%x size=%ld\n",
                  (unsigned int)pVcatFullCtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0], size);

    pVcatFullCtxt->sizeMemberCtxt = size;
                  
    p = pVcatFullCtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0];

    /* Assign other pointers and initialize memory content */
   
    for(i=0; i<64; i++) {    
        for (j = 0 ; j<192; j++) {
            pVcatFullCtxt->pVcatSts1TxCtxt[i]->pVcatMemberList[j] = p;
            pVcatFullCtxt->pVcatSts1TxCtxt[i]->pVcatMemberList[j]->stsNum = HYPHY20G_VCAT_NULL_STSNUM;
            p++;
        }        
    }
    DBG_PRINT("Assigned pVcatSts1TxCtxt[63]->pVcatMemberList[191] to :addr=0x%x size=%ld\n",
              (unsigned int)pVcatFullCtxt->pVcatSts1TxCtxt[63]->pVcatMemberList[191], (long int)sizeof(sHYPHY20G_VCAT_MEMBER_CTXT));  
    
    for(i=0; i<64; i++) {    
        for (j = 0 ; j<192; j++) {
            pVcatFullCtxt->pVcatSts1RxCtxt[i]->pVcatMemberList[j] = p;
            pVcatFullCtxt->pVcatSts1RxCtxt[i]->pVcatMemberList[j]->stsNum = HYPHY20G_VCAT_NULL_STSNUM;
            p++;
        }        
    }
    DBG_PRINT("Assigned pVcatSts1RxCtxt[63]->pVcatMemberList[191] to :addr=0x%x size=%ld\n",
              (unsigned int)pVcatFullCtxt->pVcatSts1RxCtxt[63]->pVcatMemberList[191], (long int)sizeof(sHYPHY20G_VCAT_MEMBER_CTXT));  
   
    for(i=0; i<64; i++) {    
        for (j = 0 ; j<64; j++) {
            pVcatFullCtxt->pVcatSts3cTxCtxt[i]->pVcatMemberList[j] = p;
            pVcatFullCtxt->pVcatSts3cTxCtxt[i]->pVcatMemberList[j]->stsNum = HYPHY20G_VCAT_NULL_STSNUM;
            p++;
        }        
    }
    DBG_PRINT("Assigned pVcatSts3cTxCtxt[63]->pVcatMemberList[63] to :addr=0x%x size=%ld\n",
              (unsigned int)pVcatFullCtxt->pVcatSts3cTxCtxt[63]->pVcatMemberList[63], (long int)sizeof(sHYPHY20G_VCAT_MEMBER_CTXT));
    
    for(i=0; i<64; i++) {    
        for (j = 0 ; j<64; j++) {
            pVcatFullCtxt->pVcatSts3cRxCtxt[i]->pVcatMemberList[j] = p;
            pVcatFullCtxt->pVcatSts3cRxCtxt[i]->pVcatMemberList[j]->stsNum = HYPHY20G_VCAT_NULL_STSNUM;
            p++;
        }        
    }
    DBG_PRINT("Assigned pVcatSts3cRxCtxt[63]->pVcatMemberList[63] to :addr=0x%x size=%ld\n",
              (unsigned int)pVcatFullCtxt->pVcatSts3cRxCtxt[63]->pVcatMemberList[63], (long int)sizeof(sHYPHY20G_VCAT_MEMBER_CTXT));
    return HYPHY20G_SUCCESS;

} /* vcatHyPhy20gVcatCreateMemberCtxt */

/*******************************************************************************
**
**  vcatHyPhy20gVcatDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the VCAT subsystem.  Deallocates memory
**                  used for storing this information.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - 0 = Slice A, 1 = Slice B
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatDestroyCtxt(struct file *fileHndl, UINT4 slice)
{
    sHYPHY20G_VCATA_CTXT *pVcatACtxt = NULL;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt = NULL;
    sHYPHY20G_CTXT *pDevCtxt;

    /* Argument checking */
        if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (slice == 0) {
        pVcatACtxt = pDevCtxt->pVcatACtxt;
        if(pVcatACtxt == NULL){
          /* Nothing to destroy */
        return HYPHY20G_SUCCESS;
        }
    } else {
        pVcatBCtxt = pDevCtxt->pVcatBCtxt;
        if(pVcatBCtxt == NULL){
          /* Nothing to destroy */
        return HYPHY20G_SUCCESS;
        }
    }

    if (slice == 0) {
        vcatHyPhy20gVcatDestroyMemberCtxt(fileHndl);
    }

    /* deallocate memory associated with VCAT structure */
    if (slice == 0) {
        DBG_PRINT("pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0] will be deallocated :addr=0x%x\n", (unsigned int)pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0]);
        sysHyPhy20gMemFree(pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0]);
        DBG_PRINT("pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0] , Deallocated!\n");
        
        DBG_PRINT("pDevCtxt->pVcatACtxt will be deallocated :addr=0x%x\n", (unsigned int)pDevCtxt->pVcatACtxt);
        sysHyPhy20gMemFree(pDevCtxt->pVcatACtxt);
        DBG_PRINT("pDevCtxt->pVcatACtxt , Deallocated!\n");
    } else {
        DBG_PRINT("pDevCtxt->pVcatBCtxt will be deallocated :addr=0x%x\n", (unsigned int)pDevCtxt->pVcatBCtxt);
        sysHyPhy20gMemFree(pDevCtxt->pVcatBCtxt);
        DBG_PRINT("pDevCtxt->pVcatBCtxt , Deallocated!\n");
    }  
    
    return HYPHY20G_SUCCESS;

} /* vcatHyPhy20gVcatDestroyCtxt */


/********************************************************************************
**
**  vcatHyPhy20gVcatDestroyMemberCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset member context of the VCAT subsystem.  Deallocates
**                  memory used for storing this information.
**
**
**  INPUTS:         fileHndl            - device handle
**                  group               - group number
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatDestroyMemberCtxt(struct file *fileHndl)
{
    sHYPHY20G_VCATA_CTXT *pVcatCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    
    pVcatCtxt = pDevCtxt->pVcatACtxt;
    
    DBG_PRINT("pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0] will be deallocated :addr=0x%x\n",(unsigned int) pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0]);
    sysHyPhy20gMemFree (pVcatCtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0]);
    DBG_PRINT("pDevCtxt->pVcatACtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0] , Deallocated!\n");
 
    return HYPHY20G_SUCCESS;
    
} /* vcatHyPhy20gVcatDestroyMemberCtxt */
    
/*******************************************************************************
**  
**  vcatHyPhy20gVcatAdjustSts1SequenceInfo
**  ___________________________________________________________________________
**  
**  
**  DESCRIPTION:    This function re-adjusts the sequence numbers of all
**                  necessary VCAT group members when an STS1 member is removed
**                  from the group.
**  
**  INPUTS:     *fileHndl        - base address
**              *pVcatCtxt       - pointer to pDevCtxt->pVcatCtxt
**              direction        - direction to operate on
**                               - 1 - Tx
**                               - 0 - Rx
**              groupId          - Group Number to operate on
**              sts1Num          - STS1 member to be removed
**  
**  OUTPUTS:    None
**  
**  RETURNS:        HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatAdjustSts1SequenceInfo(struct file *fileHndl,
                                    sHYPHY20G_VCATA_CTXT *pVcatCtxt,
                                    UINT4 direction, UINT4 groupId,
                                    UINT4 sts1Num)
{   
    sHYPHY20G_VCAT_STS1_CTXT *pVcatSts1MemberCtxt;
    
    UINT4 mask = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 members = 0;
    UINT4 memberToDelete = HYPHY20G_VCAT_NULL_STSNUM;
    UINT4 i = 0;

    INT4 result;

    if (direction) {
        pVcatSts1MemberCtxt = pVcatCtxt->pVcatSts1TxCtxt[groupId];
    } else {
        pVcatSts1MemberCtxt = pVcatCtxt->pVcatSts1RxCtxt[groupId];
    }

    for (i = 0; i<HYPHY20G_VCAT_NUM_STS1_MEMBERS; i++) {
        if (pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum == sts1Num) {
            memberToDelete = i;
        }
        if (pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum == HYPHY20G_VCAT_NULL_STSNUM) {
            members = i;
            i = HYPHY20G_VCAT_NUM_STS1_MEMBERS;
        }
        if (i == (HYPHY20G_VCAT_NUM_STS1_MEMBERS - 1)){
            members = i + 1;
        }
    }

    if ((members == 0)||(memberToDelete == HYPHY20G_VCAT_NULL_STSNUM)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG members = %u, memberToDelete = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)members, (unsigned int)memberToDelete);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    for (i = memberToDelete; i<members; i++) {
        pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum = pVcatSts1MemberCtxt->pVcatMemberList[i+1]->stsNum;
        if (i < (members - 1)){
            pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum = pVcatSts1MemberCtxt->pVcatMemberList[i+1]->stsNum;
        } else {
            pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum = HYPHY20G_VCAT_NULL_STSNUM;
        }
        if ((i+1) < members) {
            if (direction) {
                result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum, &reg0, &reg1,
                                        &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;
                mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_VC_SQ_MSK;
                result = hyPhy20gVarFieldWrite(&reg0, mask, i);
                if (result) return result;
                result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum, reg0, reg1,
                                            reg2, reg3, reg4, reg5, reg6, reg7);
                if (result) return result;
            } else {
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum, &reg0, &reg1,
                                        &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;
                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_MSK;
                result = hyPhy20gVarFieldWrite(&reg0, mask, i);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, pVcatSts1MemberCtxt->pVcatMemberList[i]->stsNum, reg0, reg1,
                                            reg2, reg3, reg4, reg5, reg6, reg7);
                if (result) return result;
            }
        }
    }
    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gVcatAdjustSts1SequenceInfo */

/*******************************************************************************
**
**  vcatHyPhy20gVcatAdjustSts3cSequenceInfo
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function re-adjusts the sequence numbers of all
**                  necessary VCAT group members when an STS3c member is removed
**                  from the group.
**
**  INPUTS:     *fileHndl        - base address
**              *pVcatCtxt       - pointer to pDevCtxt->pVcatCtxt
**              direction        - direction to operate on
**                               - 1 - Tx
**                               - 0 - Rx
**              groupId          - Group Number to operate on
**              sts3cNum         - STS3c member to be removed
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatAdjustSts3cSequenceInfo(struct file *fileHndl,
                                    sHYPHY20G_VCATA_CTXT *pVcatCtxt,
                                    UINT4 direction, UINT4 groupId,
                                    UINT4 sts3cNum)
{
    sHYPHY20G_VCAT_STS3c_CTXT *pVcatSts3cMemberCtxt;

    UINT4 mask = 0;
    UINT4 sts1Offset = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 members = 0;
    UINT4 memberToDelete = HYPHY20G_VCAT_NULL_STSNUM;
    UINT4 i = 0;

    INT4 result;

    if (direction) {
        pVcatSts3cMemberCtxt = pVcatCtxt->pVcatSts3cTxCtxt[groupId];
    } else {
        pVcatSts3cMemberCtxt = pVcatCtxt->pVcatSts3cRxCtxt[groupId];
    }

    sts1Offset = 3*sts3cNum;

    for (i = 0; i<HYPHY20G_VCAT_NUM_STS3c_MEMBERS; i++) {
        if (pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum == sts1Offset) {
            memberToDelete = i;
        }
        if (pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum == HYPHY20G_VCAT_NULL_STSNUM) {
            members = i;
            i = HYPHY20G_VCAT_NUM_STS3c_MEMBERS;
        }
        if (i == (HYPHY20G_VCAT_NUM_STS3c_MEMBERS - 1)){
            members = i + 1;
        }
    }

    if ((members == 0)||(memberToDelete == HYPHY20G_VCAT_NULL_STSNUM)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG members = %u, memberToDelete = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)members, (unsigned int)memberToDelete);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    DBG_PRINT("%s, %s, %d, Removing members = %u, memberToDelete = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)members, (unsigned int)memberToDelete);


    for (i = memberToDelete; i<members; i++) {
        if (i < (members - 1)){
            pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum = pVcatSts3cMemberCtxt->pVcatMemberList[i+1]->stsNum;
        } else {
            pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum = HYPHY20G_VCAT_NULL_STSNUM;
        }
        if ((i+1) < members) {
            if (direction) {
                result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum, &reg0, &reg1,
                                        &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;
                mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_VC_SQ_MSK;
                result = hyPhy20gVarFieldWrite(&reg0, mask, i);
                if (result) return result;
                result = vcatHyPhy20gTVCPMemIndWrite(fileHndl, 0x0, pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum, reg0, reg1,
                                            reg2, reg3, reg4, reg5, reg6, reg7);
                if (result) return result;
            } else {
                result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum, &reg0, &reg1,
                                        &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                if (result) return result;
                mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_EXP_SQ_MSK;
                result = hyPhy20gVarFieldWrite(&reg0, mask, i);
                if (result) return result;
                result = vcatHyPhy20gRVCPMemIndWrite(fileHndl, 0x0, pVcatSts3cMemberCtxt->pVcatMemberList[i]->stsNum, reg0, reg1,
                                            reg2, reg3, reg4, reg5, reg6, reg7);
                if (result) return result;
            }
        }
    }
    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gVcatAdjustSts3cSequenceInfo */

/*******************************************************************************
**
**  vcatHyPhy20gVcatGetGrpRate
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function retrieves the interface group usage for a
**                  VCAT group.
**
**  INPUTS:     *fileHndl        - base address
**               slice           - VCAT slice to access:
**                                 0 - VCAT A
**                                 1 - VCAT B
**               grpId           - VCAT Group ID to access:
**                                 0 to 63
**              *memType         - Pointer to storage for the VCAT interface
**                                 type for the group
**                                 Value 0 to 8 as per eHYPHY20G_VCAT_GRP_TYPE
**              *numMem          - Pointer to storage for the number of members
**                                 in the VCAT group of type *memType
**                                 0 to 192 (SONET)
**                                 0 to 64 (OTN)
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 vcatHyPhy20gVcatGetGrpRate(struct file *fileHndl, UINT4 slice, UINT4 grpId,
                                UINT4 *memType, UINT4 *numMem)
{
  UINT4 speed;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt;

    /* argument checking */
    if ((slice > 1)||(grpId > 63)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to VCAT level context information*/
    if (slice == 0) {
        pVcatACtxt = pDevCtxt->pVcatACtxt;
      HYPHY20G_ASSERT((pVcatACtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                       "%s", "pVcatCtxt is NULL.");

    /* Retrieve VCAT Group rate context */
    *memType = pVcatACtxt->vcatGrpTypeCtxt[grpId];
    speed = pVcatACtxt->vcatGrpSpeedCtxt[grpId];
  } else {
    pVcatBCtxt = pDevCtxt->pVcatBCtxt;
    HYPHY20G_ASSERT((pVcatBCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pVcatCtxt is NULL.");

    /* Retrieve VCAT Group rate context */
    *memType = pVcatBCtxt->vcatGrpTypeCtxt[grpId];
    speed = pVcatBCtxt->vcatGrpSpeedCtxt[grpId];
    }

  /* set *numMem as a function of vcatGrpSpeedCtxt */
    switch (*memType){
        case 0: /* HYPHY20G_VCAT_GRP_UNUSED */
            *numMem = 0;
            break;
        case 1: /* HYPHY20G_VCAT_GRP_STS1 */
      *numMem = speed/1;
            break;
        case 2: /* HYPHY20G_VCAT_GRP_STS3c */
      *numMem = speed/3;
            break;
        case 3: /* HYPHY20G_VCAT_GRP_STS12 */
      *numMem = speed/12;
            break;
        case 4: /* HYPHY20G_VCAT_GRP_STS24 */
      *numMem = speed/24;
            break;
        case 5: /* HYPHY20G_VCAT_GRP_STS48 */
      *numMem = speed/48;
            break;
        case 6: /* HYPHY20G_VCAT_GRP_STS192 */
      *numMem = speed/192;
            break;
        case 7: /* HYPHY20G_VCAT_GRP_TS */
      *numMem = speed/3;
            break;
        case 8: /* HYPHY20G_VCAT_GRP_ODU1 */
      *numMem = speed/48;
            break;
        case 9: /* HYPHY20G_VCAT_GRP_ODU2 */
      *numMem = speed/192;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    return HYPHY20G_SUCCESS;
} /* vcatHyPhy20gVcatGetGrpRate */

/*******************************************************************************
**
**  vcatHyPhy20gVcatGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of VcatA and VcatB context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of VcatA and VcatB context
**
*******************************************************************************/
UINT4 vcatHyPhy20gVcatGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt; 
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt; 
    UINT4 size = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    DBG_PRINT("%s, %s, %d, VcatA offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    /* include ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();

    pVcatACtxt = pDevCtxt->pVcatACtxt;
    if (pVcatACtxt != NULL) {
        /* calculate vcatACtxt.vcatGrpSpeedCtxt and  vcatACtxt.vcatGrpTypeCtxt size */
        size += sizeof(pVcatACtxt->vcatGrpSpeedCtxt) + 
                sizeof(pVcatACtxt->vcatGrpTypeCtxt);

        DBG_PRINT("%s, %s, %d, VcatA member ctxt offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, size);
        
        /* include ctxt header size */
        size += utilHyPhy20gGetCtxtHdrSize();
        size += pVcatACtxt->sizeMemberCtxt;
    }

    
    DBG_PRINT("%s, %s, %d, VcatB offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    /* VcatB context size */
    /* include ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();

    pVcatBCtxt = pDevCtxt->pVcatBCtxt;
    if (pVcatBCtxt != NULL) {
        size += sizeof(sHYPHY20G_VCATB_CTXT);
    }
    
    DBG_PRINT("%s, %s, %d, VcatA and VcatB offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* vcatHyPhy20gVcatGetCtxtSize */

/*******************************************************************************
**
**  vcatHyPhy20gVcatExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the VcatA and VcatB context into pre-allocated 
**                  buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for VcatA and VcatB context
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
INT4 vcatHyPhy20gVcatExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt; 
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt; 
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

    DBG_PRINT("%s, %s, %d, VcatA offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract VcatA context */
    pVcatACtxt = pDevCtxt->pVcatACtxt;

    /* insert ctxt buffer header */
    len = (pVcatACtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
            (sizeof(sHYPHY20G_CTXT_HDR) + 
            sizeof(pVcatACtxt->vcatGrpSpeedCtxt) + 
            sizeof(pVcatACtxt->vcatGrpTypeCtxt));
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_VCATA_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    if (pVcatACtxt != NULL) {
        /* extract pVcatACtxt->vcatGrpSpeedCtxt and pVcatACtxt->vcatGrpTypeCtxt */
        len = sizeof(pVcatACtxt->vcatGrpSpeedCtxt) + 
                sizeof(pVcatACtxt->vcatGrpTypeCtxt);
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                        (void *)(pVcatACtxt->vcatGrpSpeedCtxt), len);
        offset += len;

        DBG_PRINT("%s, %s, %d, VcatA member ctxt offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, offset);

        /* do not include the first/second level pointers values */
        /* extract the total sHYPHY20G_VCAT_MEMBER_CTXT */
        /* insert ctxt buffer header */
        len = sizeof(sHYPHY20G_CTXT_HDR) + pVcatACtxt->sizeMemberCtxt;
        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_VCATA_MEMBER_CTXT_ID, HYPHY_NO_INDEX);
        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
            "%s", "pCtxtBuf is NULL.");  
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
            (void *)(pVcatACtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0]), 
            pVcatACtxt->sizeMemberCtxt);
        offset += pVcatACtxt->sizeMemberCtxt;
    }

    DBG_PRINT("%s, %s, %d, VcatB offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract VcatB context */
    pVcatBCtxt = pDevCtxt->pVcatBCtxt;

    /* insert ctxt buffer header */
    len = (pVcatBCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
            (sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_VCATB_CTXT));
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_VCATB_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    if (pVcatBCtxt != NULL) {
        /* extract sHYPHY20G_VCATB_CTXT */
        len = sizeof(sHYPHY20G_VCATB_CTXT);
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                        (void *)(pVcatBCtxt), len);
        offset += len;
    }

    DBG_PRINT("%s, %s, %d, VcatB offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;
    
    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gVcatExtractCtxt */

/*******************************************************************************
**
**  vcatHyPhy20gVcatRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the VcatA and VcatB context into the driver context
**                  memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored VcatA and VcatB context for 
**                  restoration
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
INT4 vcatHyPhy20gVcatRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt; 
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt; 
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0, slice = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the vcatA header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_VCATA_CTXT_ID, HYPHY_NO_INDEX);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_VCATA_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    /* restore each Vcat context */
    for (slice = 0; slice < 2; slice++) {
        DBG_PRINT("%s, %s, %d, Vcat %ld offset 0x%lx 0x%p\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset, pCtxtBuf);

        /* check if the payload is valid */
        if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
            offset += sizeof(sHYPHY20G_CTXT_HDR);
        } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
            /* create Vcatx context */
            result = vcatHyPhy20gVcatCreateCtxt(fileHndl, slice);
            if (result) {
                DBG_PRINT("%s, %s, %d, VcatCreateCtxt failed for slice %ld\n", 
                            __FILE__, __FUNCTION__, __LINE__, slice);
                *pParsedSize = 0;
                return result;
            }
            
            /* retrieve device context */
            pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
            /* confirm device context pointer is valid before processing */
            HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pDevCtxt is NULL.");

            /* offset pCtxtBuf to figure out the payload position */
            offset += sizeof(sHYPHY20G_CTXT_HDR);

            /* restore VcatA context */
            if (slice == 0) {
                pVcatACtxt = pDevCtxt->pVcatACtxt;

                /* restore pVcatACtxt->vcatGrpSpeedCtxt and pVcatACtxt->vcatGrpTypeCtxt */
                len = sizeof(pVcatACtxt->vcatGrpSpeedCtxt) + 
                        sizeof(pVcatACtxt->vcatGrpTypeCtxt);
                sysHyPhy20gMemCpy((void *)(pVcatACtxt->vcatGrpSpeedCtxt), 
                                ((UINT1 *)pCtxtBuf + offset), len);
                offset += len;

                DBG_PRINT("%s, %s, %d, VcatA member ctxt offset 0x%lx\n", 
                            __FILE__, __FUNCTION__, __LINE__, offset);

                /* restore the total sHYPHY20G_VCAT_MEMBER_CTXT */
                /* offset pCtxtBuf to figure out the payload position */
                offset += sizeof(sHYPHY20G_CTXT_HDR);

                sysHyPhy20gMemCpy((void *)(pVcatACtxt->pVcatSts1TxCtxt[0]->pVcatMemberList[0]), 
                                ((UINT1 *)pCtxtBuf + offset), pVcatACtxt->sizeMemberCtxt);
                offset += pVcatACtxt->sizeMemberCtxt;

            } else {
                pVcatBCtxt = pDevCtxt->pVcatBCtxt;
                
                /* restore VcatB context */
                sysHyPhy20gMemCpy((void *)(pVcatBCtxt), 
                            ((UINT1 *)pCtxtBuf + offset), sizeof(sHYPHY20G_VCATB_CTXT));
                offset += sizeof(sHYPHY20G_VCATB_CTXT);
            }

        } else {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
        }
        DBG_PRINT("%s, %s, %d, Vcat %ld offset end 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

        /* move to next ctxt buffer header */
        pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
    }

    *pParsedSize = offset;

    return HYPHY20G_SUCCESS;
}/* vcatHyPhy20gVcatRestoreCtxt */

/********************************************************************************
**
**  vcataHyPhy20gVcataAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function used to audit the VCATA subCTXT.
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
INT4 vcataHyPhy20gVcataAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 regAddr, offset, stsNum, sonetEnValue, sts192cEn;
    UINT4 tvcpoEn, rvcpoEn, tvcpoOtuMode, rvcpoOtuMode;
    sHYPHY20G_VCATA_CTXT *pVcatACtxt;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 reg2 = 0;
    UINT4 reg3 = 0;
    UINT4 reg4 = 0;
    UINT4 reg5 = 0;
    UINT4 reg6 = 0;
    UINT4 reg7 = 0;
    UINT4 grpId,mask, i, j;
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    pVcatACtxt=pDevCtxt->pVcatACtxt;

    if (pVcatACtxt==NULL)
    {
        return HYPHY20G_SUCCESS;
    }
    else
    {
        for(i=0; i<HYPHY20G_VCAT_NUM_GID; i++) 
        {
            if ((pVcatACtxt->pVcatSts1RxCtxt[i]==NULL)||(pVcatACtxt->pVcatSts1TxCtxt[i]==NULL)||
                (pVcatACtxt->pVcatSts3cRxCtxt[i]==NULL)||(pVcatACtxt->pVcatSts3cTxCtxt[i]==NULL)||
                (pVcatACtxt->vcatGrpSpeedCtxt[i]>192)||(pVcatACtxt->vcatGrpTypeCtxt[i]>HYPHY20G_VCAT_GRP_ODU2))
            {
                DBG_PRINT("%s, %s, %d, pVcatACtxt %dth members value wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (int)i);
                return HYPHY20G_FAILURE;
            }
            
            switch (pVcatACtxt->vcatGrpTypeCtxt[i])
            {
                case HYPHY20G_VCAT_GRP_STS1:
                    if (pVcatACtxt->vcatGrpSpeedCtxt[i]==0)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                case HYPHY20G_VCAT_GRP_STS3c:
                    if ((pVcatACtxt->vcatGrpSpeedCtxt[i]%3!=0)||(pVcatACtxt->vcatGrpSpeedCtxt[i]==0))
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;

                case HYPHY20G_VCAT_GRP_STS12:
                    if (pVcatACtxt->vcatGrpSpeedCtxt[i]!=12)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    
                    break;
                
                case HYPHY20G_VCAT_GRP_STS24:
                    if (pVcatACtxt->vcatGrpSpeedCtxt[i]!=24)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                case HYPHY20G_VCAT_GRP_STS48:
                    if (pVcatACtxt->vcatGrpSpeedCtxt[i]!=48)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                case HYPHY20G_VCAT_GRP_STS192:
                    if (pVcatACtxt->vcatGrpSpeedCtxt[i]!=192)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                case HYPHY20G_VCAT_GRP_TS:
                    if ((pVcatACtxt->vcatGrpSpeedCtxt[i]%3!=0)||(pVcatACtxt->vcatGrpSpeedCtxt[i]==0))
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                case HYPHY20G_VCAT_GRP_ODU1:
                    if ((pVcatACtxt->vcatGrpSpeedCtxt[i]%48!=0)||(pVcatACtxt->vcatGrpSpeedCtxt[i]==0))
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                case HYPHY20G_VCAT_GRP_ODU2:
                    if (pVcatACtxt->vcatGrpSpeedCtxt[i]!=192)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                case HYPHY20G_VCAT_GRP_UNUSED:
                    if (pVcatACtxt->vcatGrpSpeedCtxt[i]!=0)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u pVcatACtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                                   __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatACtxt->vcatGrpTypeCtxt[i],
                                   (int)i,(unsigned int)(pVcatACtxt->vcatGrpSpeedCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                    break;
                    
                default:
                    DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%d]=%u (invalid value).\n",
                        __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                    return HYPHY20G_FAILURE;
                    break;

            }
            
            if ((pVcatACtxt->vcatGrpTypeCtxt[i] < HYPHY20G_VCAT_GRP_TS)&&
                (pVcatACtxt->vcatGrpTypeCtxt[i] > HYPHY20G_VCAT_GRP_UNUSED))
            {
                /*check the SONET_EN and STS192C CFG BIT 
                  according to hyPhy20gVcatLcasSonetLinkStsxxxCfg functions*/
                regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
                result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                    HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                                    &sonetEnValue);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "sysHyPhy20gBitRead fail."); 

                if (sonetEnValue == 0) 
                {
                    DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%u]=%u when BIT_CFG_SONET_EN=0.\n",
                        __FILE__, __FUNCTION__, __LINE__,(unsigned int)i,
                        (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                    return HYPHY20G_FAILURE;
                }
                
                regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
                offset = HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_STS192C_OFF;
                result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &sts192cEn);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "sysHyPhy20gBitRead fail."); 

                if (pVcatACtxt->vcatGrpTypeCtxt[i]<HYPHY20G_VCAT_GRP_STS192)
                {
                    if (sts192cEn==1)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%u]=%u when BIT_CFG_STS192C=1.\n",
                            __FILE__, __FUNCTION__, __LINE__,(unsigned int)i,
                            (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                }
                else if (pVcatACtxt->vcatGrpTypeCtxt[i]==HYPHY20G_VCAT_GRP_STS192)
                {
                    if (sts192cEn==0)
                    {
                        DBG_PRINT("%s, %s, %d, pVcatACtxt->vcatGrpTypeCtxt[%u]=%u when BIT_CFG_STS192C=0.\n",
                            __FILE__, __FUNCTION__, __LINE__,(unsigned int)i,
                            (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                        return HYPHY20G_FAILURE;
                    }
                }
            }
            else if((pVcatACtxt->vcatGrpTypeCtxt[i]>=HYPHY20G_VCAT_GRP_TS)
                    &&(pVcatACtxt->vcatGrpTypeCtxt[i]<=HYPHY20G_VCAT_GRP_ODU2))
            {
                /* check the TVCPO_ENABLE and RVCPO_ENABLE according to hyPhy20gVcatLcasBInit*/    
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 0);
                result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                    HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_TVCPO_ENABLE_OFF,
                                    &tvcpoEn);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "sysHyPhy20gBitRead fail."); 
                if (tvcpoEn==0)
                {
                    DBG_PRINT("%s, %s, %d, VCATA TVCPO_ENABLE=0\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
                    

                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 0);
                result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                    HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_RVCPO_ENABLE_OFF,
                                    &rvcpoEn);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "sysHyPhy20gBitRead fail."); 
                if (rvcpoEn==0)
                {
                    DBG_PRINT("%s, %s, %d, VCATA RVCPO_ENABLE=0\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
                    

            /* check TVCPO_BIT_CTL_OTU2_OTU1_MODE and RVCPO_BIT_CFG_OTU_MODE
               according to hyPhy20gVcatLcasOtnOtu2xxxCfg and hyPhy20gVcatLcasOtnOtu1xxx functions */
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
                regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 0);
                result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                    HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                                    &tvcpoOtuMode);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "sysHyPhy20gBitRead fail."); 
                
                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 0);
                result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                    HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                                    &rvcpoOtuMode);
                HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                "%s", "sysHyPhy20gBitRead fail."); 

                if (pVcatACtxt->vcatGrpTypeCtxt[i]<HYPHY20G_VCAT_GRP_ODU2)
                {
                    if ((tvcpoOtuMode == 1)||(rvcpoOtuMode == 0))
                    {
                        DBG_PRINT("%s, %s, %d, VCATA TVCPO|RVCPO OTUMODE wrong.\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_FAILURE;
                    }
                }
                else 
                {
                    if ((tvcpoOtuMode == 0)||(rvcpoOtuMode == 1))
                    {
                        DBG_PRINT("%s, %s, %d, VCATA TVCPO|RVCPO OTUMODE wrong.\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_FAILURE;
                    }
                }
            }
            
                        
            /* check pVcatMemberList for STS1 VCG according to hyPhy20gVcatLcasSonetLinkSts1RxMemberProv
               and hyPhy20gVcatLcasSonetLinkSts1TxMemberProv*/
            for (j=0; j<HYPHY20G_VCAT_NUM_STS1_MEMBERS; j++)
            {
                if ((pVcatACtxt->pVcatSts1RxCtxt[i]->pVcatMemberList[j]==NULL)||
                    (pVcatACtxt->pVcatSts1TxCtxt[i]->pVcatMemberList[j]==NULL))
                {
                    DBG_PRINT("%s, %s, %d, VCATA %uth STS1 VCG RX/TX %uth member is NULL\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j);
                    return HYPHY20G_FAILURE;
                }
                /*check the RX members */
                stsNum=pVcatACtxt->pVcatSts1RxCtxt[i]->pVcatMemberList[j]->stsNum;
                if ((stsNum != HYPHY20G_VCAT_NULL_STSNUM)&&
                    ((stsNum>191)||(pVcatACtxt->vcatGrpTypeCtxt[i]!=HYPHY20G_VCAT_GRP_STS1)))
                {
                    DBG_PRINT("%s, %s, %d, VCATA %uth STS1 VCG %uth RX member stsNum(%u) mismatch vcatGrpTypeCtxt(%u)\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                        (unsigned int)stsNum, (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                    return HYPHY20G_FAILURE;
                }

                if (stsNum != HYPHY20G_VCAT_NULL_STSNUM)
                {
                    /* Read Indirect Link Config Table in RVCP192 */
                    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, stsNum, &reg0, &reg1,
                                            &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "vcatHyPhy20gRVCPMemIndRead fail."); 
                    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_MSK;
                    result = hyPhy20gVarFieldRead(reg0, mask, &grpId);
                    if (grpId != i)
                    {
                        DBG_PRINT("%s, %s, %d, VCATA %uth STS1 VCG %uth RX member(stsNum=%u) GrpId(%u) is wrong.\n",
                            __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                            (unsigned int)stsNum, (unsigned int)(grpId));
                        return HYPHY20G_FAILURE;
                    }
                }
                
                /*check the TX members */
                stsNum=pVcatACtxt->pVcatSts1TxCtxt[i]->pVcatMemberList[j]->stsNum;
                if ((stsNum != HYPHY20G_VCAT_NULL_STSNUM)&&
                    ((stsNum>191)||(pVcatACtxt->vcatGrpTypeCtxt[i]!=HYPHY20G_VCAT_GRP_STS1)))
                {
                    DBG_PRINT("%s, %s, %d, VCATA %uth STS1 VCG %uth TX member stsNum(%u) mismatch vcatGrpTypeCtxt(%u)\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                        (unsigned int)stsNum, (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                    return HYPHY20G_FAILURE;
                }

                if (stsNum != HYPHY20G_VCAT_NULL_STSNUM)
                {
                    /* read Indirect Link Config Table in TVCP192 */
                    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, stsNum, &reg0, &reg1,
                                        &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "vcatHyPhy20gTVCPMemIndRead fail."); 
                    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
                    result = hyPhy20gVarFieldRead(reg0, mask, &grpId);
                    if (grpId != i)
                    {
                        DBG_PRINT("%s, %s, %d, VCATA %uth STS1 VCG %uth TX member(stsNum=%u) GrpId(%u) is wrong.\n",
                            __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                            (unsigned int)stsNum, (unsigned int)(grpId));
                        return HYPHY20G_FAILURE;
                    }
                }
            }

            /* check pVcatMemberList for STS3C VCG according to hyPhy20gVcatLcasSonetLinkSts3cRxMemberProv
               and hyPhy20gVcatLcasSonetLinkSts3cTxMemberProv*/
            for (j=0; j<HYPHY20G_VCAT_NUM_STS3c_MEMBERS; j++)
            {
                if ((pVcatACtxt->pVcatSts3cRxCtxt[i]->pVcatMemberList[j]==NULL)||
                    (pVcatACtxt->pVcatSts3cTxCtxt[i]->pVcatMemberList[j]==NULL))
                {
                    DBG_PRINT("%s, %s, %d, VCATA %uth STS3C VCG RX/TX %uth member is NULL\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j);
                    return HYPHY20G_FAILURE;
                }

                /*check the RX members */
                stsNum=pVcatACtxt->pVcatSts3cRxCtxt[i]->pVcatMemberList[j]->stsNum;

                if ((stsNum != HYPHY20G_VCAT_NULL_STSNUM)&&
                    ((stsNum%3 != 0)||(stsNum > 189)||
                     (pVcatACtxt->vcatGrpTypeCtxt[i] != HYPHY20G_VCAT_GRP_STS3c)))
                {
                    DBG_PRINT("%s, %s, %d, VCATA %uth STS3C VCG %uth RX member stsNum(%u) mismatch vcatGrpTypeCtxt(%u)\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                        (unsigned int)stsNum, (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                    return HYPHY20G_FAILURE;
                }

                if (stsNum != HYPHY20G_VCAT_NULL_STSNUM)
                {
                    
                    /* Read Indirect Link Config Table in RVCP192 */
                    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x0, stsNum, &reg0, &reg1,
                                            &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "vcatHyPhy20gRVCPMemIndRead fail."); 
                    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CFG_MEM_IBIT_LNK_CFG_MEM_GROUP_TAG_MSK;
                    result = hyPhy20gVarFieldRead(reg0, mask, &grpId);

                    if (grpId != i)
                    {
                        DBG_PRINT("%s, %s, %d, VCATA %uth STS3C VCG %uth RX member(stsNum=%u) GrpId(%u) is wrong.\n",
                            __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                            (unsigned int)stsNum, (unsigned int)(grpId));
                        return HYPHY20G_FAILURE;
                    }
                }

                /*check the TX members */
                stsNum=pVcatACtxt->pVcatSts3cTxCtxt[i]->pVcatMemberList[j]->stsNum;

                if ((stsNum != HYPHY20G_VCAT_NULL_STSNUM)&&
                    ((stsNum%3 != 0)||(stsNum > 189)||
                     (pVcatACtxt->vcatGrpTypeCtxt[i] != HYPHY20G_VCAT_GRP_STS3c)))
                {
                    DBG_PRINT("%s, %s, %d, VCATA %uth STS3C VCG %uth TX member stsNum(%u) mismatch vcatGrpTypeCtxt(%u)\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                        (unsigned int)stsNum, (unsigned int)(pVcatACtxt->vcatGrpTypeCtxt[i]));
                    return HYPHY20G_FAILURE;
                }

                if (stsNum != HYPHY20G_VCAT_NULL_STSNUM)
                {
                    /* read Indirect Link Config Table in TVCP192 */
                    result = vcatHyPhy20gTVCPMemIndRead(fileHndl, 0x0, stsNum, &reg0, &reg1,
                                        &reg2, &reg3, &reg4, &reg5, &reg6, &reg7);
                    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                                    "%s", "vcatHyPhy20gTVCPMemIndRead fail."); 
                    mask = HYPHY20G_VCAT_LCAS_TVCP_LNK_CFG_TBL_IBIT_LNK_CFG_TBL_GROUP_TAG_MSK;
                    result = hyPhy20gVarFieldRead(reg0, mask, &grpId);
                    if (grpId != i)
                    {
                        DBG_PRINT("%s, %s, %d, VCATA %uth STS3C VCG %uth TX member(stsNum=%u) GrpId(%u) is wrong.\n",
                            __FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)j, 
                            (unsigned int)stsNum, (unsigned int)(grpId));
                        return HYPHY20G_FAILURE;
                    }
                }
                
            }

        }        
    }
    
    return HYPHY20G_SUCCESS;
    
} /* vcataHyPhy20gVcataAuditCtxt */

/********************************************************************************
**
**  vcatbHyPhy20gVcatbAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function used to audit the VCATB subCTXT.
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
INT4 vcatbHyPhy20gVcatbAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result, i;
    UINT4 regAddr,tvcpoEn,rvcpoEn,tvcpoOtuMode,rvcpoOtuMode ;
    sHYPHY20G_VCATB_CTXT *pVcatBCtxt;
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    pVcatBCtxt=pDevCtxt->pVcatBCtxt;

    if (pVcatBCtxt==NULL)
    {
        return HYPHY20G_SUCCESS;
    }
    
    for(i=0; i<HYPHY20G_VCAT_NUM_GID; i++) 
    {            

        if ((pVcatBCtxt->vcatGrpSpeedCtxt[i]>192)||(pVcatBCtxt->vcatGrpTypeCtxt[i]>HYPHY20G_VCAT_GRP_ODU2))
        {
            DBG_PRINT("%s, %s, %d, pVcatBCtxt->vcatGrpSpeedCtxt[%u]=%u| pVcatBCtxt->vcatGrpTypeCtxt[%u]=%u value error!\n",
                __FILE__, __FUNCTION__, __LINE__,(unsigned int)i,(unsigned int)(pVcatBCtxt->vcatGrpSpeedCtxt[i]),
                (unsigned int)i,(unsigned int)(pVcatBCtxt->vcatGrpTypeCtxt[i]));
            return HYPHY20G_FAILURE;
        }

        switch (pVcatBCtxt->vcatGrpTypeCtxt[i])
        {
            case HYPHY20G_VCAT_GRP_TS:
                if ((pVcatBCtxt->vcatGrpSpeedCtxt[i]%3!=0)||(pVcatBCtxt->vcatGrpSpeedCtxt[i]==0))
                {
                    DBG_PRINT("%s, %s, %d, pVcatBCtxt->vcatGrpTypeCtxt[%d]=%u pVcatBCtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                               __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatBCtxt->vcatGrpTypeCtxt[i],
                               (int)i,(unsigned int)pVcatBCtxt->vcatGrpSpeedCtxt[i]);
                    return HYPHY20G_FAILURE;
                }
                break;
                
            case HYPHY20G_VCAT_GRP_ODU1:
                if ((pVcatBCtxt->vcatGrpSpeedCtxt[i]%48!=0)||(pVcatBCtxt->vcatGrpSpeedCtxt[i]==0))
                {
                    DBG_PRINT("%s, %s, %d, pVcatBCtxt->vcatGrpTypeCtxt[%d]=%u pVcatBCtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                               __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatBCtxt->vcatGrpTypeCtxt[i],
                               (int)i,(unsigned int)pVcatBCtxt->vcatGrpSpeedCtxt[i]);
                    return HYPHY20G_FAILURE;
                }
                break;
                
            case HYPHY20G_VCAT_GRP_ODU2:
                if (pVcatBCtxt->vcatGrpSpeedCtxt[i]!=192)
                {
                    DBG_PRINT("%s, %s, %d, pVcatBCtxt->vcatGrpTypeCtxt[%d]=%u pVcatBCtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                               __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatBCtxt->vcatGrpTypeCtxt[i],
                               (int)i,(unsigned int)pVcatBCtxt->vcatGrpSpeedCtxt[i]);
                    return HYPHY20G_FAILURE;
                }
                break;
                
            case HYPHY20G_VCAT_GRP_UNUSED:
                if (pVcatBCtxt->vcatGrpSpeedCtxt[i]!=0)
                {
                    DBG_PRINT("%s, %s, %d, pVcatBCtxt->vcatGrpTypeCtxt[%d]=%u pVcatBCtxt->vcatGrpSpeedCtxt[%d]=%u HYPHY20G_FAILURE\n",
                               __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)pVcatBCtxt->vcatGrpTypeCtxt[i],
                               (int)i,(unsigned int)pVcatBCtxt->vcatGrpSpeedCtxt[i]);
                    return HYPHY20G_FAILURE;
                }
                break;
                
            default:
                DBG_PRINT("%s, %s, %d, pVcatBCtxt->vcatGrpTypeCtxt[%d]=%u (invalid value).\n",
                    __FILE__, __FUNCTION__, __LINE__, (int)i, (unsigned int)(pVcatBCtxt->vcatGrpTypeCtxt[i]));
                return HYPHY20G_FAILURE;
                break;
        }
        
        if((pVcatBCtxt->vcatGrpTypeCtxt[i]>=HYPHY20G_VCAT_GRP_TS)&&
           (pVcatBCtxt->vcatGrpTypeCtxt[i]<=HYPHY20G_VCAT_GRP_ODU2))
        {

            /* check the TVCPO_ENABLE and RVCPO_ENABLE according to hyPhy20gVcatLcasBInit*/    
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
            result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_TVCPO_ENABLE_OFF,
                                &tvcpoEn);
            HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                            "%s", "sysHyPhy20gBitRead fail."); 
            if (tvcpoEn==0)
            {
                DBG_PRINT("%s, %s, %d, VCATB TVCPO_ENABLE=0\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
                

            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
            result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_RVCPO_ENABLE_OFF,
                                &rvcpoEn);
            HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                            "%s", "sysHyPhy20gBitRead fail."); 
            if (rvcpoEn==0)
            {
                DBG_PRINT("%s, %s, %d, VCATB RVCPO_ENABLE=0\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
                

        /* check TVCPO_BIT_CTL_OTU2_OTU1_MODE and RVCPO_BIT_CFG_OTU_MODE
           according to hyPhy20gVcatLcasOtnOtu2xxxCfg and hyPhy20gVcatLcasOtnOtu1xxx functions */
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_BASE;
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
            result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_VCAT_LCAS_TVCPO_BIT_CTL_OTU2_OTU1_MODE_OFF,
                                &tvcpoOtuMode);
            HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                            "%s", "sysHyPhy20gBitRead fail."); 
            
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_BASE;
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
            result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_VCAT_LCAS_RVCPO_BIT_CFG_OTU_MODE_OFF,
                                &rvcpoOtuMode);
            HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                            "%s", "sysHyPhy20gBitRead fail."); 

            if (pVcatBCtxt->vcatGrpTypeCtxt[i]<HYPHY20G_VCAT_GRP_ODU2)
            {
                if ((tvcpoOtuMode == 1)||(rvcpoOtuMode == 0))
                {
                    DBG_PRINT("%s, %s, %d, VCATB TVCPO|RVCPO value wrong.\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
            }
            else 
            {
                if ((tvcpoOtuMode == 0)||(rvcpoOtuMode == 1))
                {
                    DBG_PRINT("%s, %s, %d, VCATB TVCPO|RVCPO value wrong.\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
            }
        }
    }        
    
    return HYPHY20G_SUCCESS;
    
} /* vcatbHyPhy20gVcatbAuditCtxt */


/********************************************************************************
**
**  vcatHyPhy20gVcatAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function used to audit the VCAT subCTXTs.
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
INT4 vcatHyPhy20gVcatAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 lowPwrBit, rstBit;
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    /* Retrieve VCAT lowPwrBit */
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_VCAT_LOWPWR_OFF,
                             &lowPwrBit);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gBitRead fail."); 


    /* both VCATA and VCATB modules disabled */
    if (lowPwrBit == 1)
    {
        if ((pDevCtxt->pVcatACtxt!=NULL)||(pDevCtxt->pVcatBCtxt!=NULL))
        {
            DBG_PRINT("%s, %s, %d, pDevCtxt->pVcatACtxt|pVcatBCtxt!=NULL when lowPwrBit == 1.\n",
                __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        else
            return HYPHY20G_SUCCESS;
    }
    
    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                             HYPHY20G_TOP_BIT_SYS_SW_RST_3_VCAT_RST_OFF,
                             &rstBit);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gBitRead fail."); 

    if (rstBit == 1)
    {
        if ((pDevCtxt->pVcatACtxt!=NULL)||(pDevCtxt->pVcatBCtxt!=NULL))
        {
            DBG_PRINT("%s, %s, %d, pDevCtxt->pVcatACtxt|pVcatBCtxt!=NULL when VCAT_RST = 1.\n",
                __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        else
            return HYPHY20G_SUCCESS;
    }
    
    /* at least one VCAT module enabled */
    result=vcataHyPhy20gVcataAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, vcataHyPhy20gVcataAuditCtxt(fileHndl) SUCCESS in vcatHyPhy20gVcatAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);
            
    result=vcatbHyPhy20gVcatbAuditCtxt(fileHndl);
    if(result!=HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, vcatbHyPhy20gVcatbAuditCtxt(fileHndl) SUCCESS in vcatHyPhy20gVcatAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    return HYPHY20G_SUCCESS;
    
} /* vcatHyPhy20gVcatAuditCtxt */

/* end of file */
