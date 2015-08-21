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
**  FILE        : hyphy20g_sonet_features.c
**
**  $Date: 2012-08-24 15:10:13 +0800 (Fri, 24 Aug 2012) $
**
**  $Revision: 25820 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/


/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_sonet_features.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 utilHyPhy20gSonetTohTtmByteLengthChk(struct file *fileHndl, UINT4 slice,
                                          UINT4 ttmLength);
INT4 utilHyPhy20gSonetPohTtmByteLengthChk(struct file *fileHndl, UINT4 slice,
                                          UINT4 sts3Num, UINT4 sts1Num,
                                          UINT4 ttmLength);
INT4 utilHyPhy20gCrcCalc(struct file *fileHndl, UINT4 msg[15], UINT4 *e164Crc);
INT4 utilHyPhy20gSonetSberLineSdCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 sdip2, UINT4 sdip1, UINT4 sdst,
                                    UINT4 sdct, UINT4 bermEnbl);
INT4 utilHyPhy20gSonetSberLineSfCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 sfip2, UINT4 sfip1, UINT4 sfst,
                                    UINT4 sfct, UINT4 bermEnbl);
INT4 utilHyPhy20gSonetSberPathSdCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts3, UINT4 sts1, UINT4 sdip2,
                                    UINT4 sdip1, UINT4 sdst, UINT4 sdct,
                                    UINT4 bermEnbl);
INT4 utilHyPhy20gSonetSberPathSfCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts3, UINT4 sts1, UINT4 sfip2,
                                    UINT4 sfip1, UINT4 sfst, UINT4 sfct,
                                    UINT4 bermEnbl);
/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gSonetTohTxCfgTtmLength
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the length of trail trace messages
**                  to be inserted in the J0 byte for a SONET subsystem in the
**                  transmit direction.
**
**                  These blocks  also support 64 byte trail trace messages.
**                  Both 16 and 64 byte messages can be delineated with CRLF or
**                  E.164 CRC. These variations are not demonstrated in this
**                  code.
**
**                  Associated functions:
**                      - hyPhy20gSonetTohRxCfgTtmLength
**
**                      - hyPhy20gSonetTohTxCfg1ByteTtm
**                      - hyPhy20gSonetTohRxCfg1ByteTtm
**                      - hyPhy20gSonetTohRxExt1ByteTtm
**
**                      - hyPhy20gSonetTohTxCfg16ByteTtm
**                      - hyPhy20gSonetTohRxCfg16ByteTtm
**                      - hyPhy20gSonetTohRxExt16ByteTtm
**
**                      - hyPhy20gSonetTohTxCfg64ByteTtm
**                      - hyPhy20gSonetTohRxCfg64ByteTtm
**                      - hyPhy20gSonetTohRxExt64ByteTtm
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15
**                ttmLength    - Trail trace message length
**                                0 - 1 byte
**                                1 - 16 bytes
**                                2 - 64 bytes
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohTxCfgTtmLength(struct file *fileHndl, UINT4 slice,
                                    UINT4 ttmLength)
{
    /* variable declaration */
    UINT2 iAddr;
    UINT4 msk;
    UINT4 sonetTttpRegData;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (ttmLength > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Retreive TTTP Indirect Address 0x4940580 + 0x01000*A(A=0:15) */
    iAddr = 0x0; /* indirect TTTP_SECT Trace Configuration */
    result = sonetHyPhy20gTTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetTttpRegData);
    if (result) return result;

    switch(ttmLength)
    {
        /* Configure message length to be 1 byte */
        case 0:

        /* Set BYTEEN to 1 */
            msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 1);
            if (result) return result;

            break;

        /* Configure message length to be 16 bytes */
        case 1:

            /* Set BYTEEN to 0 */
            msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 0);
            if (result) return result;

            /* Set LENGTH16 to 1 */
            msk =
  HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 1);
            if (result) return result;
            break;

        case 2: /*64 bytes*/
                    /* Set BYTEEN to 0 */
            msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 0);
            if (result) return result;

            /* Set LENGTH16 to 0 */
            msk =
  HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 0);
            if (result) return result;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* Write updated configuration to TTTP indirect Trace Configuration memory
       (0x0) */
    result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetTttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohTxCfgTtmLength */

/*******************************************************************************
**
**  hyPhy20gSonetTohRxCfgTtmLength
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the length of trail trace messages
**                  to be inserted in the J0 byte for a SONET subsystem in the
**                  receive direction.
**
**
**                  Associated functions:
**                      - hyPhy20gSonetTohTxCfgTtmLength
**
**                      - hyPhy20gSonetTohTxCfg1ByteTtm
**                      - hyPhy20gSonetTohRxCfg1ByteTtm
**                      - hyPhy20gSonetTohRxExt1ByteTtm
**
**                      - hyPhy20gSonetTohTxCfg16ByteTtm
**                      - hyPhy20gSonetTohRxCfg16ByteTtm
**                      - hyPhy20gSonetTohRxExt16ByteTtm
**
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15
**                ttmLength    - Trail trace message length
**                                0 - 1 byte
**                                1 - 16 bytes
**                                2 - 64 bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxCfgTtmLength(struct file *fileHndl, UINT4 slice,
                                    UINT4 ttmLength)
{
    /* variable declaration */
    UINT2 iAddr;
    UINT4 msk;
    UINT4 sonetRttpRegData;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (ttmLength > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Retreive RTTP Indirect Address 0x4940180 + 0x01000*A(A=0:15) */
    iAddr = 0x3; /* indirect RTTP_SECT Trace Configuration */
    result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetRttpRegData);
    if (result) return result;

    switch(ttmLength)
    {
        /* Configure message length to be 1 byte */
        case 0:

            /* Set ALGO[1:0] to 3 to use 1 byte length */
            msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
            result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 3);
            if (result) return result;

            break;

        /* Configure message length to be 16 bytes */
        case 1:
            /* Set LENGTH16 to 1 */
            msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
            if (result) return result;
            break;
        case 2:
       
            /* Set LENGTH16 to 0 */
            msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
            if (result) return result;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* Write updated configuration to RTTP indirect Trace Configuration memory
       (0x0) */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxCfgTtmLength */

/*******************************************************************************
**
**  hyPhy20gSonetPohTxCfgTtmLength
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the length of trail trace messages
**                  to be inserted in the J1 byte for a SONET subsystem in the
**                  transmit direction.
**
**                  This function and associated functions demonstrate the use
**                  SONET:TTTP to generate and receive 16 or 64 byte trail trace
**                  messages. The 16 byte message will be in E.164 CRC mode and
**                  the 64 byte message will synchronize to CR/LF.
**
**                  Associated functions:
**                      - hyPhy20gSonetPohRxCfgTtmLength
**
**                      - hyPhy20gSonetPohTxCfg16ByteTtm
**                      - hyPhy20gSonetPohRxCfg16ByteTtm
**                      - hyPhy20gSonetPohRxExt16ByteTtm
**
**                      - hyPhy20gSonetPohTxCfg64ByteTtm
**                      - hyPhy20gSonetPohRxCfg64ByteTtm
**                      - hyPhy20gSonetPohRxExt64ByteTtm
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15
**                sts12Num     - 0 to 3
**                sts3Num      - 0 to 3
**                sts1Num      - 0 to 2
**                ttmLength    - Trail trace message length
**                                0 - 16 bytes
**                                1 - 64 bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohTxCfgTtmLength(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 ttmLength)
{
    /* variable declaration */
    UINT2 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 sonetTttpRegData;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
	    (ttmLength > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

    /* Retreive TTTP_PATH Indirect Address 0x4940640 + 0x01000*A(A=0:15) */
    iAddr = 0x0; /* indirect TTTP_PATH Trace Configuration */
    result = sonetHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddr, sts3Off,
                                          sts1Num, &sonetTttpRegData);
    if (result) return result;

     /* Configure trace message length */
    switch(ttmLength)
    {

        /* Configure message length to be 16 bytes */
        case 0:

            /* Set TTTP_PATH BYTEEN to 0 */
            msk =
            HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 0);
            if (result) return result;

            /* Set TTTP_PATH LENGTH16 to 1 */
            msk =
            HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 1);
            if (result) return result;

            break;

        /* Configure message length to be 64 bytes */
        case 1:

            /* Set TTTP_PATH BYTEEN to 0 */
            msk =
            HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 0);
            if (result) return result;

            /* Set TTTP_U LENGTH16 to 0 */
            msk =
            HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, 0);
            if (result) return result;

            break;

        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;

    }

    /* Write updated configuration to SONET:TTTP indirect Trace Configuration
       memory (0x0) */
    result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddr, sts3Off,
                                           sts1Num, sonetTttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohTxCfgTtmLength */


/*******************************************************************************
**
**  hyPhy20gSonetPohRxCfgTtmLength
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the length of tail trace messages
**                  to be inserted in the J1 byte for a SONET subsystem in the
**                  receive direction.
**
**
**                  Associated functions:
**                      - hyPhy20gSonetPohTxCfgTtmLength
**
**                      - hyPhy20gSonetPohTxCfg16ByteTtm
**                      - hyPhy20gSonetPohRxCfg16ByteTtm
**                      - hyPhy20gSonetPohRxExt16ByteTtm
**
**                      - hyPhy20gSonetPohTxCfg64ByteTtm
**                      - hyPhy20gSonetPohRxCfg64ByteTtm
**                      - hyPhy20gSonetPohRxExt64ByteTtm
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15
**                sts12Num     - 0 to 3
**                sts3Num      - 0 to 3
**                sts1Num      - 0 to 2
**                ttmLength    - Tail trace message length
**                                0 - 16 bytes with E.164 CRC
**                                1 - 64 bytes with CR/LF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxCfgTtmLength(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 ttmLength)
{
    /* variable declaration */
    UINT2 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 sonetRttpRegData;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
	    (ttmLength > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

    /* Retreive RTTP_PATH Indirect Address 0x4940240 + 0x01000*A(A=0:15) */
    iAddr = 0x3; /* indirect RTTP_PATH Trace Configuration */
    result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddr, sts3Off,
                                          sts1Num, &sonetRttpRegData);
    if (result) return result;

    /* Change the SYNC_CRLF bit */
    result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddr, sts3Off,
                                          sts1Num, &sonetRttpRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_SYNC_CRLF_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, ttmLength);
    if (result) return result;

    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddr, sts3Off,
                                           sts1Num, sonetRttpRegData);
    if (result) return result;

    /* Configure trace message length */
    switch(ttmLength)
    {
        /* Configure message length to be 16 bytes */
        case 0:

            /* Set RTTP_PATH LENGTH16 to 1 */
            msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
            if (result) return result;

            break;

        /* Configure message length to be 64 bytes */
        case 1:

            /* Set RTTP_PATH LENGTH16 to 0 */
            msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
            if (result) return result;

            break;

        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* Write updated configuration to SONET:RTTP_PATH indirect Trace
       Configuration memory (0x3) */
    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddr, sts3Off,
                                           sts1Num, sonetRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxCfgTtmLength */


/*******************************************************************************
**
**  hyPhy20gSonetPohRxGetSts1PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function reports the master/slave status of the 48 STS1s
**               within a SONET slice in the Rx Direction.
**
**               This function may be used to extract the payload configuration
**               when automatic payload configuration is enabled.
**
**               STS3_1[15:0] refers to STS1_0's in every STS3 of each STS12.
**               STS3_2[15:0] refers to STS1_1's in every STS3 of each STS12.
**               STS3_3[15:0] refers to STS1_2's in every STS3 of each STS12.
**
**
**  INPUTS:       *fileHndl     - base address
**                slice         - 0 to 15
**                *statusWord1  - Pointer to storage for status word1
**                *statusWord2  - Pointer to storage for status word2
**                *statusWord3  - Pointer to storage for status word3
**                                  Word 1 - STS3_1[15:0]
**                                  Word 2 - STS3_2[15:0]
**                                  Word 3 - STS3_3[15:0]
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxGetSts1PayloadCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 *statusWord1, UINT4 *statusWord2,
                                         UINT4 *statusWord3)
{
    /* Variable declaration */
    UINT4 sonetRshpiRegData;
    UINT4 stat1;
    UINT4 stat2;
    UINT4 stat3;
    UINT4 regAddr;
    UINT4 msk;
    INT4 result;

    /* Argument checking */
    if (slice > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Direct read from SONET_RSHPI 0x49401C8 + 0x01000*A(A=0:15)
       Payload Configuration 1 */
    regAddr = HYPHY20G_SONET_RSHPI_REG_PLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRshpiRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RSHPI_BIT_PLD_CFG_1_STS3_1_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &stat1);
    if (result) return result;

    /* Direct read from SONET_RSHPI 0x49401CC + 0x01000*A(A=0:15)
       Payload Configuration 2 */
    regAddr = HYPHY20G_SONET_RSHPI_REG_PLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRshpiRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RSHPI_BIT_PLD_CFG_2_STS3_2_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &stat2);
    if (result) return result;

    /* Direct read from SONET_LINE_TOP 0x4940014 + 0x01000*A(A=0:15)
       SONET/SDH Processing Slice RX Payload Configuration 3 */
    regAddr = HYPHY20G_SONET_RSHPI_REG_PLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRshpiRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RSHPI_BIT_PLD_CFG_3_STS3_3_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &stat3);
    if (result) return result;

    *statusWord1 = stat1;
    *statusWord2 = stat2;
    *statusWord3 = stat3;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxGetOpMode */

/*******************************************************************************
**
**  hyPhy20gSonetPohTxGetSts1PayloadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function reports the master/slave status of the 48 STS1s
**               within a SONET slice in the Tx Direction.
**
**               This function may be used to extract the payload configuration
**               when automatic payload configuration is enabled.
**
**               STS3_1[15:0] refers to STS1_0's in every STS3 of each STS12.
**               STS3_2[15:0] refers to STS1_1's in every STS3 of each STS12.
**               STS3_3[15:0] refers to STS1_2's in every STS3 of each STS12.
**
**  INPUTS:       *fileHndl     - base address
**                slice         - 0 to 15
**                *statusWord1  - Pointer to storage for status word1
**                *statusWord2  - Pointer to storage for status word2
**                *statusWord3  - Pointer to storage for status word3
**                                  Word 1 - STS3_1[15:0]
**                                  Word 2 - STS3_2[15:0]
**                                  Word 3 - STS3_3[15:0]
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohTxGetSts1PayloadCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 *statusWord1, UINT4 *statusWord2,
                                         UINT4 *statusWord3)
{
    /* Variable declaration */
    UINT4 sonetRshpiRegData;
    UINT4 stat1;
    UINT4 stat2;
    UINT4 stat3;
    UINT4 regAddr;
    UINT4 msk;
    INT4 result;

    /* Argument checking */
    if (slice > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Direct read from SONET_TSHPI 0x4940888 + 0x01000*A(A=0:15)
       Payload Configuration 1 */
    regAddr = HYPHY20G_SONET_TSHPI_REG_PLD_CFG_1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRshpiRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_TSHPI_BIT_PLD_CFG_1_STS3_1_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &stat1);
    if (result) return result;

    /* Direct read from SONET_TSHPI 0x494088C + 0x01000*A(A=0:15)
       Payload Configuration 2 */
    regAddr = HYPHY20G_SONET_TSHPI_REG_PLD_CFG_2;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRshpiRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_TSHPI_BIT_PLD_CFG_2_STS3_2_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &stat2);
    if (result) return result;

    /* Direct read from SONET_TSHPI 0x4940890 + 0x01000*A(A=0:15)
       Payload Configuration 3 */
    regAddr = HYPHY20G_SONET_TSHPI_REG_PLD_CFG_3;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRshpiRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_TSHPI_BIT_PLD_CFG_3_STS3_3_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &stat3);
    if (result) return result;

    *statusWord1 = stat1;
    *statusWord2 = stat2;
    *statusWord3 = stat3;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohTxGetOpMode */

/*******************************************************************************
**
**  hyPhy20gSonetTohRxCfg1ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Receive parameters for 1-byte
**                  trail trace messages to be extracted from the J0 byte within
**                  a SONET subsystem.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  msg         - 1 byte expected TTM (DON`T CARE)
**                  enblMode    - Processing mode for received trail trace
**                                messages
**                                0 - disabled (received messages not processed)
**                                1 - Enabled (received messages processed using
**                                             algorithm 3)
**
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxCfg1ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 msg, UINT4 enblMode)
{
    /* variable declaration */
    UINT4 iAddr = 0x0;
    UINT4 msk;
    UINT4 sonetRttpRegData;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (enblMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check if device is configure for 1 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 1);
    if (result) return result;

    result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetRttpRegData);

    /* Configure 1st byte of expected message from far end */
    /* Write TRACE to SONET:RTTP_SECT at 0x4940180 + 0x01000*A(A=0:15) */
    iAddr = 0xc0; /* RTTP_SECT indirect Expected Trace */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr, msg);
    if (result) return result;
    DBG_PRINT("%s, %s, %d, msg = 0x%x\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)msg);

    /* Disable algorithm, and then re-configure with new algorithm to be used */
    iAddr = 0x3; /* indirect RTTP_SECT Trace Configuration */
    result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetRttpRegData);
    if (result) return result;

    /* Disable ALGO */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
    if (result) return result;

    /* Set TTMMONEN bit to 1 for proper SECTION trace monitoring */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
    if (result) return result;

    /* Write in SONET:RTTP Indirect Trace Cfg Memory 0x3 */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetRttpRegData);
    if (result) return result;

    if (enblMode == 1)
    {
        /* Set ALGO */
        msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0x3);
        if (result) return result;

        /* Write in SONET:RTTP Indirect Trace Cfg Memory 0x3 */
        result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr,
                                               sonetRttpRegData);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxCfg1ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetTohRxCfg16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Receive parameters for 16-byte
**                  trail trace messages to be extracted from the J0 byte within
**                  a SONET subsystem.
**
**                  This function specifically configures a 16 byte TTM with
**                  E.164 CRC.  The function is supplied with the 15 message
**                  bytes (in the range 0 to 127) and the function calculates
**                  and appends the E.164 CRC byte.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  msg[15]     - 15 byte expected TTM
**                  enblMode    - Processing mode for received trail trace
**                                messages
**                                0 - disabled (received messages not processed)
**                                1 - algo 1 (rx ttm processed telcordia
**                                    compliant)
**                                2 - algo 2 (rx ttm processed ITU compliant)
**
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxCfg16ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 msg[15], UINT4 enblMode)
{
    /* variable declaration */
    UINT2 iAddr, iAddrEtrace;
    UINT4 msk;
    UINT4 sonetRttpRegData;
    UINT4 e164Crc;
    INT4 result;
    INT4 i;

    /* argument checking */
	if ((slice > 15) || (enblMode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    for (i=0; i<15; i++)
    {
        if (msg[i] >= 0x80) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Check if device is configured for 16 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 3);
    if (result) return result;

    /* Configure E.164 CRC for the 15 byte Trace Message */
    result = utilHyPhy20gCrcCalc(fileHndl, msg, &e164Crc);
    if (result) return result;

    /* Write calculated E.164 CRC to RTTP indirect register */
    iAddrEtrace = 0xc0; /* SONET_RTTP Indirect Expected Trace */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddrEtrace,
                                           e164Crc);
    if (result) return result;
    
    for (i=0; i<15; i++)
    {
        iAddrEtrace++;

        /* Write the trace message in SONET:RTTP indirect Expected Trace
           memory (0xc0) */
        result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddrEtrace,
                                               msg[i]);
        if (result) return result;
    }

    /* Configure the receive trail trace message processing algorithm */
    iAddr = 0x3; /* SONET_RTTP Indirect Trace configuration */
    result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetRttpRegData);
    if (result) return result;

    /* Unconditionally set ALGO[1:0] to 00b */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetRttpRegData);
    if (result) return result;

    /* Set ALGO[1:0] as specified by the user */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, enblMode);
    if (result) return result;

    /* Set TTMMONEN bit to 1 for proper SECTION trace monitoring */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxCfg16ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetTohRxCfg64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Receive parameters for 16-byte
**                  trail trace messages to be extracted from the J0 byte within
**                  a SONET subsystem.
**
**                  This function specifically configures a 64 byte TTM with
**                  E.164 CRC.  The function is supplied with the 64 message
**                  bytes. 
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  syncCrLf    - select whether the current algorithm (except
**                                for algorithm 3) synchronizes on the CR/LF 
**                                ASCII characters or on the byte with its MSB
**                                set high
**                                0 - synchronizes on the byte with its MSB set
**                                    high
**                                1 - synchronizes on the CR/LF ASCII characters
**
**                  msg[64]     - 15 byte expected TTM
**                  enblMode    - Processing mode for received trail trace
**                                messages
**                                0 - disabled (received messages not processed)
**                                1 - algo 1 (rx ttm processed telcordia
**                                    compliant)
**                                2 - algo 2 (rx ttm processed ITU compliant)
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxCfg64ByteTtm(struct file *fileHndl, UINT4 slice,
                                UINT4 syncCrLf, UINT4 msg[64], UINT4 enblMode)
{
    /* variable declaration */
    UINT2 iAddr, iAddrEtrace;
    UINT4 msk;
    UINT4 sonetRttpRegData;
    INT4 result;
    INT4 i;

    /* argument checking */
    if ((slice > 15) || (enblMode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if device is configured for 64 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 5);
    if (result) return result;

    /* Write calculated E.164 CRC to RTTP indirect register */
    iAddrEtrace = 0xc0; /* SONET_RTTP Indirect Expected Trace */
    
    for (i=0; i<64; i++)
    {
        /* Write the trace message in SONET:RTTP indirect Expected Trace
           memory (0xc0) */
        result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddrEtrace,
                                               msg[i]);
        if (result) return result;
        
        iAddrEtrace++;
    }

    /* Configure the receive trail trace message processing algorithm */
    iAddr = 0x3; /* SONET_RTTP Indirect Trace configuration */
    result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetRttpRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_SYNC_CRLF_MSK;
    if(syncCrLf == 0){
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
    }else {
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
    }
    if (result) return result;


    /* Unconditionally set ALGO[1:0] to 00b */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetRttpRegData);
    if (result) return result;

    /* Set ALGO[1:0] as specified by the user */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, enblMode);
    if (result) return result;

    /* Set TTMMONEN bit to 1 for proper SECTION trace monitoring */
    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxCfg16ByteTtm */

/*******************************************************************************
**
**  hyPhy20gSonetPohRxCfg16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Receive parameters for 16-byte
**                  trail trace messages to be extracted from the J1 byte within
**                  a SONET subsystem.
**
**                  This function specifically configures a 16 byte TTM with
**                  E.164 CRC.  The function is supplied with the 15 message
**                  bytes (in the range 0 to 127) and the function calculates
**                  and appends the E.164 CRC byte.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  msg[15]     - 15 byte expected TTM
**                  enblMode    - Processing mode for received trail trace
**                                messages
**                                0 - disabled (received messages not processed)
**                                1 - algo 1 (rx ttm processed telcordia
**                                    compliant)
**                                2 - algo 2 (rx ttm processed ITU compliant)
**
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxCfg16ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 msg[15],
                                    UINT4 enblMode)
{
    /* variable declaration */
    UINT4 iAddrRttpEtrace;
    UINT4 iAddrRttpCfg;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 sonetRttpRegData;
    UINT4 i;
    UINT4 e164Crc;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
	    (enblMode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

     for (i=0; i<15; i++)
    {
        if (msg[i] > 0x80)
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if device is configured for 16 byte TTM */
    result = utilHyPhy20gSonetPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 1);
    if (result) return result;

    /* Configure 1st byte of expected message from far end and write the trace
       message in SONET:RTTP 0x4940240 + 0x01000*A(A=0:15) */
    iAddrRttpEtrace = 0xc0; /* indirect Trace Configuration */

    /* Configure E.164 CRC for the 15 byte Trace Message */
    result = utilHyPhy20gCrcCalc(fileHndl, msg, &e164Crc);
    if (result) return result;

    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpEtrace,
                                           sts3Off, sts1Num, e164Crc);
    if (result) return result;

    /* Configure indirect Trace Message bytes 1 to 15 */
    for (i=0; i<15; i++)
    {
        iAddrRttpEtrace++;

        /* Read the next byte of TTM */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrRttpEtrace,
                                              sts3Off, sts1Num,
                                              &sonetRttpRegData);
        if (result) return result;
    
        msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_EXP_TRC_ETRACE_MSK;
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, msg[i]);
        if (result) return result;

        /* Write the msg */
        result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpEtrace,
                                               sts3Off, sts1Num,
                                               sonetRttpRegData);
        if (result) return result;
    }

    /* Configure the receive trail trace message processing algorithm */
    iAddrRttpCfg = 0x3; /* SONET:RTTP indirect Trace Configuration memory */
    result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrRttpCfg,
                                          sts3Off, sts1Num, &sonetRttpRegData);
    if (result) return result;

    /* Unconditionally set ALGO[1:0] to 00b */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpCfg,
                                           sts3Off, sts1Num, sonetRttpRegData);
    if (result) return result;

    /* Set ALGO[1:0] as specified by the user */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, enblMode);
    if (result) return result;

    /* Set TTMMOEN bit to 1 for proper SECTION trace monitoring */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpCfg,
                                           sts3Off, sts1Num, sonetRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxCfg16ByteTtm */

/*******************************************************************************
**
**  hyPhy20gSonetPohRxCfg64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Receive parameters for 64-byte
**                  trail trace messages to be extracted from the J1 byte within
**                  a SONET subsystem.
**
**                  This function specifically configures a 64 byte TTM with
**                  CR/LF.  The function is supplied with the 62 message
**                  bytes (in the range 0 to 127) and the function appends
**                  CR/LF characters.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  msg[62]      - 62 byte expected TTM
**                  enblMode     - Processing mode for received trail trace
**                                 messages
**                                 0 - disabled (received messages not processed)
**                                 1 - algo 1 (rx ttm processed telcordia
**                                     compliant)
**                                 2 - algo 2 (rx ttm processed ITU compliant)
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxCfg64ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 msg[62],
                                    UINT4 enblMode)
{
    /* variable declaration */
    UINT4 sonetRttpRegData;
    UINT4 iAddrCfg;
    UINT4 iAddrEtrace;
    UINT4 i;
    UINT4 sts3Off;
    UINT4 CR = 0x0D;
    UINT4 LF = 0x0A;
    UINT4 msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
        (enblMode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

    for (i=0; i<62; i++)
    {
        if (msg[i] > 0x80)
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if device is configured for 64 byte TTM */
    result = utilHyPhy20gSonetPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 3);
    if (result) return result;
   
    /* Write the trace message in SONET:RTTP_PATH
       0x4940240 + 0x01000*A(A=0:15) */
    iAddrEtrace = 0xc0; /* indirect Trace Configuration memory */

    for (i=0; i<62; i++)
    {
        /* Write the trace message in SONET:RTTP_PATH indirect Trace
           Configuration memory (0xc0) */
        result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrEtrace,
                                               sts3Off, sts1Num, msg[i]);
        if (result) return result;

        iAddrEtrace++;
    }

    /* Write Carriage Return, Line Feed into the last two bytes of the message*/
    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrEtrace,
                                           sts3Off, sts1Num, CR);
    if (result) return result;

    iAddrEtrace++;

    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrEtrace,
                                           sts3Off, sts1Num, LF);
    if (result) return result;

    /* Configure the receive trail trace message processing algorithm
       in SONET:RTTP 0x4940240 + 0x01000*A(A=0:15) */
    iAddrCfg = 0x3; /* indirect Trace Configuration memory */
    result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                          sts3Off, sts1Num,
                                          &sonetRttpRegData);
    if (result) return result;

    /* Unconditionally set ALGO[1:0] to 00b */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                           sts3Off, sts1Num, sonetRttpRegData);
    if (result) return result;

    /* Set ALGO[1:0] as specified by the user */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, enblMode);
    if (result) return result;

    /* Set TTMMOEN bit to 1 for proper PATH trace monitoring */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
    if (result) return result;

    /* Write to SONET:RTTP Indirect Trace Cfg Mem 0x3 */
    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                           sts3Off, sts1Num, sonetRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxCfg64ByteTtm */

/*******************************************************************************
**
**  hyPhy20gSonetTohTxCfg1ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the 1 byte Trail Trace Message
**                  to be transmit on the J0 byte of a specified link within
**                  the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetTohTxCfgTtmLength must be called for the link
**                  to configure the link for 1 byte TTM.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  msg          - 1 byte TTM to be transmitted
**                  enbl         - Enable generation of trail trace messages
**                                 0 - disabled (configured ttm not transmitted)
**                                 1 - enabled (configured ttm transmitted)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohTxCfg1ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 msg, UINT4 enbl)
{
    /* variable declaration */
    UINT4 iAddr = 0x0;
    UINT4 msk;
    UINT4 sonetTttpRegData;
    UINT4 zeroEnbl;
	UINT4 straceEn;
	UINT4 addr,offset;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (enbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check if device is configure for 1 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 0);
    if (result) return result;

    result = sonetHyPhy20gTTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetTttpRegData);

     /* Write TRACE to SONET:TTTP_SECT at 0x4940580 + 0x01000*A(A=0:15) */
    iAddr = 0x40; /* TTTP_SECT indirect Trace */
    result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddr, msg);
    if (result) return result;

    /* Read from SONET:TTTP_SECT Indirect Trace Configuration Memory 0x0 */
    iAddr = 0x0; /* TTTP_SECT indirect Trace Configuration */
    result = sonetHyPhy20gTTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetTttpRegData);
    if (result) return result;

    if (enbl == 1){
        zeroEnbl = 0;
	    straceEn = 1;
    } else {
        zeroEnbl = 1;
		straceEn = 0;
    }
    /* Set ZEROEN */
    msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_ZEROEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, zeroEnbl);
    if (result) return result;

    /* Write to SONET:TTTP_SECT Indirect Trace Configuration Memory 0x0 */
    result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetTttpRegData);
    if (result) return result;

    /* Set STRACEEN */
    addr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(HYPHY20G_SONET_TRMP_REG_CFG,slice);
	offset = HYPHY20G_SONET_TRMP_BIT_CFG_STRACEEN_OFF;
	result = sysHyPhy20gBitWrite(fileHndl,addr,offset,straceEn);
	if (result) return result;
	
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohTxCfg1ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetTohTxCfg16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the 16 byte Trail Trace Message
**                  to be transmit on the J0 byte of a specified link within
**                  the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetTohTxCfgTtmLength must be called for the link
**                  to configure the link for 16 byte TTM.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  msg[15]      - 15 byte TTM to be transmitted
**                  enbl         - Enable generation of trail trace messages
**                                 0 - disabled (configured ttm not transmitted)
**                                 1 - enabled (configured ttm transmitted)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohTxCfg16ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 msg[15], UINT4 enbl)
{
    /* variable declaration */
    UINT4 iAddrTttpTrace;
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sonetTttpRegData;
    UINT4 zeroEnbl;
	UINT4 straceEn;
	UINT4 addr,offset;
    UINT4 i;
    UINT4 e164Crc;
    INT4 result;


    /* argument checking */
	if ((slice > 15) || (enbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (i=0; i<15; i++)
    {
        if (msg[i] >= 0x80)
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check if device is configure for 16 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 2);
    if (result) return result;

    iAddrTttpTrace = 0x40;

    /* Configure E.164 CRC for the 15 byte Trace Message */
    result = utilHyPhy20gCrcCalc(fileHndl, msg, &e164Crc);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, e164Crc = 0x%x\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)e164Crc);
    result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddrTttpTrace,
                                           e164Crc);
    if (result) return result;

    /* Configure indirect Trace Message bytes 1 to 15 */
    for (i=0; i<15; i++)
    {
        iAddrTttpTrace++;

        /* Write to SONET:TTTP Indirect trace memory 0x40 */
        result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddrTttpTrace,
                                               msg[i]);
        if (result) return result;
    }

    /* Configure ZEROEN in SONET:TTTP 0x4940580 + 0x01000*A(A=0:15) */
    iAddr = 0x0; /* TTTP indirect Trace Configuration memory */
    result = sonetHyPhy20gTTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetTttpRegData);
    if (result) return result;

    if (enbl == 0){
        zeroEnbl = 1;
        straceEn = 0;
    }else{
        zeroEnbl = 0;
        straceEn = 1;
    }

    /* Set ZEROEN */
    msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_ZEROEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, zeroEnbl);
    if (result) return result;

    /* Write updated configuration to SONET:TTTP indirect Trace Configuration
       memory (0x0) */
    result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetTttpRegData);
    if (result) return result;

    /* Set STRACEEN */
    addr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(HYPHY20G_SONET_TRMP_REG_CFG,slice);
    offset = HYPHY20G_SONET_TRMP_BIT_CFG_STRACEEN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl,addr,offset,straceEn);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohTxCfg16ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetTohTxCfg64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the 64 byte Trail Trace Message
**                  to be transmit on the J0 byte of a specified link within
**                  the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetTohTxCfgTtmLength must be called for the link
**                  to configure the link for 64 byte TTM.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  msg[64]      - 64 byte TTM to be transmitted
**                  enbl         - Enable generation of trail trace messages
**                                 0 - disabled (configured ttm not transmitted)
**                                 1 - enabled (configured ttm transmitted)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohTxCfg64ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 msg[64], UINT4 enbl)
{
    /* variable declaration */
    UINT4 iAddrTttpTrace;
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sonetTttpRegData;
    UINT4 zeroEnbl;
    UINT4 straceEn;
    UINT4 addr,offset;
    UINT4 i;
    INT4 result;


    /* argument checking */
    if ((slice > 15) || (enbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check if device is configure for 16 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 4);
    if (result) return result;

    iAddrTttpTrace = 0x40;

    /* Configure indirect Trace Message bytes 1 to 15 */
    for (i=0; i<64; i++)
    {
        /* Write to SONET:TTTP Indirect trace memory 0x40 */
        result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddrTttpTrace,
                                               msg[i]);
        if (result) return result;
        
        iAddrTttpTrace++;
    }

    /* Configure ZEROEN in SONET:TTTP 0x4940580 + 0x01000*A(A=0:15) */
    iAddr = 0x0; /* TTTP indirect Trace Configuration memory */
    result = sonetHyPhy20gTTTPSectIndRead(fileHndl, slice, iAddr,
                                          &sonetTttpRegData);
    if (result) return result;

    if (enbl == 0){
        zeroEnbl = 1;
        straceEn = 0;
    }else{
        zeroEnbl = 0;
        straceEn = 1;
    }

    /* Set ZEROEN */
    msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_ZEROEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, zeroEnbl);
    if (result) return result;

    /* Write updated configuration to SONET:TTTP indirect Trace Configuration
       memory (0x0) */
    result = sonetHyPhy20gTTTPSectIndWrite(fileHndl, slice, iAddr,
                                           sonetTttpRegData);
    if (result) return result;

    /* Set STRACEEN */
    addr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(HYPHY20G_SONET_TRMP_REG_CFG,slice);
    offset = HYPHY20G_SONET_TRMP_BIT_CFG_STRACEEN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl,addr,offset,straceEn);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohTxCfg16ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetPohTxCfg16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the 16 byte Trail Trace Message
**                  to be transmit on the J1 byte of a specified link within
**                  the SONET subsystem.
**
**                  This function specifically configures a 16 byte TTM with
**                  E.164 CRC.  The function is supplied with the 15 message
**                  bytes (in the range 0 to 127) and the function calculates
**                  and appends the E.164 CRC byte.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetPohTxCfgTtmLength must be called for the link
**                  to configure the link for 16 byte TTM.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  msg[15]      - 15 byte TTM to be transmitted
**                  enbl         - Enable generation of trail trace messages
**                                 0 - disabled (configured ttm not transmitted)
**                                 1 - enabled (configured ttm transmitted)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohTxCfg16ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 msg[15], UINT4 enbl)
{
    /* variable declaration */
    UINT4 iAddrTttpTrace;
    UINT4 iAddrTttpCfg;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 sonetTttpRegData;
    UINT4 i;
    UINT4 e164Crc;
    UINT4 zeroEnbl;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
	    (enbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

     for (i=0; i<15; i++)
    {
        if (msg[i] >= 0x80)
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if device is configured for 16 byte TTM */
    result = utilHyPhy20gSonetPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 0);
    if (result) return result;

    iAddrTttpTrace = 0x40; /* TTTP_PATH indirect Trace */

    /* Configure E.164 CRC for the 15 byte Trace Message */
    result = utilHyPhy20gCrcCalc(fileHndl, msg, &e164Crc);
    if (result) return result;

    result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                           sts3Off, sts1Num, e164Crc);
    if (result) return result;

    /* Configure indirect Trace Message bytes 1 to 15 */
    for (i=0; i<15; i++)
    {
        iAddrTttpTrace++;

        /* Read the next byte of TTM */
        result = sonetHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpTrace,
                                              sts3Off, sts1Num,
                                              &sonetTttpRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_TRACE_MSK;
        result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, msg[i]);
        if (result) return result;

        /* Write the msg */
        result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                               sts3Off, sts1Num,
                                               sonetTttpRegData);
        if (result) return result;
    }

    /* Configure ZEROEN in SONET:TTTP 0x4940640 + 0x01000*A(A=0:15) */
    iAddrTttpCfg = 0x0; /* Indirect Trace Configuration memory */
    zeroEnbl = 1 - enbl;
    result = sonetHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpCfg,
                                          sts3Off, sts1Num, &sonetTttpRegData);
    if (result) return result;

    /* Set ZEROEN */
    msk =
  HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_ZEROEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, zeroEnbl);
    if (result) return result;

    /* Write updated configuration to SONET:TTTP indirect Trace Configuration
       memory(0x0) */
    result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpCfg,
                                           sts3Off, sts1Num, sonetTttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohTxCfg16ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetPohTxCfg64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the 64 byte Trail Trace Message
**                  to be transmit on the J1 byte of a specified link within
**                  the SONET subsystem.
**
**                  This function specifically configures a 64 byte TTM with
**                  CR/LF.  The function is supplied with the 62 message
**                  bytes (in the range 0 to 127) and the function appends
**                  CR/LF characters.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetPohTxCfgTtmLength must be called for the link
**                  to configure the link for 64 byte TTM.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  msg[62]      - 62 byte TTM to be transmitted excluding CR/LF
**                  enbl         - Enable generation of trail trace messages
**                                 0 - disabled (configured ttm not transmitted)
**                                 1 - enabled (configured ttm transmitted)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohTxCfg64ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 msg[62], UINT4  enbl)
{
    /* variable declaration */
    UINT4 sonetTttpRegData;
    UINT4 iAddrTttpTrace;
    UINT4 iAddrTttpCfg;
    UINT4 i;
    UINT4 sts3Off;
    UINT4 zeroen;
    UINT4 CR = 0x0D;
    UINT4 LF = 0x0A;
    UINT4 msk;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)
	    || (enbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

    for (i=0; i<62; i++)
    {
        if (msg[i] >= 0x80)
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if device is configured for 64 byte TTM */
    result = utilHyPhy20gSonetPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 2);
    if (result) return result;

    iAddrTttpTrace = 0x40; /* TTTP_PATH indirect Trace */

    /* Configure indirect Trace Message bytes 1 to 62 */
    for (i=0; i<62; i++)
    {
        /* Write the next byte of the message */
        result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                              sts3Off, sts1Num, msg[i]);
        if (result) return result;

        iAddrTttpTrace++;
    }

    /* Write Carriage Return, Line Feed into the last two bytes of the message*/
    result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                           sts3Off, sts1Num, CR);
    if (result) return result;

    iAddrTttpTrace++;

    result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                           sts3Off, sts1Num, LF);
    if (result) return result;

    /* Configure ZEROEN in SONET:TTTP */
    iAddrTttpCfg = 0x0; /* indirect Trace Configuration memory */
    zeroen = 1 - enbl;
    result = sonetHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpCfg,
                                          sts3Off, sts1Num, &sonetTttpRegData);
    if (result) return result;

    /* Set ZEROEN */
    msk =
  HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_ZEROEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetTttpRegData, msk, zeroen);
    if (result) return result;

    /* Write updated configuration to SONET:TTTP indirect Trace Configuration
       memory (0x0) */
    result = sonetHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpCfg,
                                           sts3Off, sts1Num, sonetTttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohTxCfg64ByteTtm */

/*******************************************************************************
**
**  hyPhy20gSonetTohRxExt1ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  1 byte Trail Trace Message received in the J0 byte of a
**                  specified link within the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetTohRxCfgTtmLength and
**                  hyPhy20gSonetTohRxCfg1ByteTtm must be called for the link to
**                  configure the link for a 1 byte TTM and configure an
**                  expected TTM message.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  extType      - Type of message to be extracted
**                                 0 - Captured
**                                 1 - Accepted
**                  *extMsg      - pointer to 1 byte TTM that has been extracted
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxExt1ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 extType, UINT4 *extMsg)
{
    /* variable declaration */
    UINT4 sonetRttpCtraceData;
    UINT4 sonetRttpAtraceData;
    UINT2 iAddr;
    INT4 result;

   /* argument checking */
	if ((slice > 15) || (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if device is configured for 1 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 1);
    if (result) return result;

    /* For Captured message */
    if (extType == 0)
    {
        /* Read Captured 0x40 + 0x1*N(N=0:63) */
        iAddr = 0x40; /* RTTP indirect Captured Trace */
        result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr,
                                              &sonetRttpCtraceData);
        if (result) return result;

        *extMsg = sonetRttpCtraceData;
    }
    /* For Accepted messge */
    else if (extType == 1)
    {
        /* Read Accepted 0x80 + 0x1*N(N=0:63) */
        iAddr = 0x80; /* RTTP indirect Aceepted Trace */
        result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddr,
                                              &sonetRttpAtraceData);
        if (result) return result;

        *extMsg = sonetRttpAtraceData;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxExt1ByteTtm */

/*******************************************************************************
**
**  hyPhy20gSonetTohRxExt16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  16 byte Trail Trace Message received in the J0 byte of a
**                  specified link within the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetTohTxCfgTtmLength and
**                  hyPhy20gSonetTohRxCfg16ByteTtm must be called for the link
**                  to configure the link for a 16 byte TTM and configure an
**                  expected TTM message.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  extType      - Type of message to be extracted
**                                 0 - Captured
**                                 1 - Accepted
**                  *extE164Crc - pointer to E.164 CRC associated with the
**                                extracted 15 byte message.
**                  *extMsg[15] - pointer to 15 byte TTM that has been extracted
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxExt16ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 extType, UINT4 *extE164Crc,
                                    UINT4 *extMsg)
{
     /* variable declaration */
    UINT4 iAddrCfg;
    UINT4 iAddrCtrace;
    UINT4 iAddrAtrace;
    UINT4 msk;
    UINT4 offset;
    UINT4 sonetRttpRegData;
    UINT4 sonetRttpCtraceData;
    UINT4 sonetRttpAtraceData;
    UINT4 msg[16];
    UINT2 crcLocation = 0;
    UINT2 i;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddrCfg = 0x3; /* Trace Configuration */
    iAddrCtrace = 0x40; /* Captured Trace */
    iAddrAtrace = 0x80; /* Accepted Trace */

    /* Check if device is configured for 16 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 3);
    if (result) return result;

    if (extType == 0)
    {
        for (i=0; i<16; i++)
        {
            /* Read Captured trace */
            result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddrCtrace,
                                                  &sonetRttpCtraceData);
            if (result) return result;

            /* Place inside message array */
            msg[i] = sonetRttpCtraceData;

            if (sonetRttpCtraceData >= 0x80)
            {
                crcLocation = i;
                *extE164Crc = sonetRttpCtraceData;
            }
            iAddrCtrace++;
        }
    }
    else if (extType == 1)
    {
        /* check if device is configured for Algorithm 2
           in SONET:RTTP indirect Trace Configuration memory (0x3) */
        result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddrCfg,
                                              &sonetRttpRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
        offset = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_OFF;
        if (((sonetRttpRegData & msk) >> offset) != 0x2)
            return HYPHY20G_ERR_INVALID_PRECONFIG;

        for (i=0; i<16; i++)
        {
            /* Read Accepted Trace */
            result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddrAtrace,
                                                  &sonetRttpAtraceData);
            if (result) return result;

            msg[i] = sonetRttpAtraceData;

            if (sonetRttpAtraceData >= 0x80)
            {
                crcLocation = i;
                *extE164Crc = sonetRttpAtraceData;
            }
            iAddrAtrace++;
        }
    }

    for (i=crcLocation; i<15; i++)
    {
        extMsg[i-crcLocation] = msg[i+1];
    }

    for (i=0; i<crcLocation; i++)
    {
        /* store the remainder of the message starting from where CRC left off */
        extMsg[i + 15 - crcLocation] = msg[i];
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxExt16ByteTtm */

/*******************************************************************************
**
**  hyPhy20gSonetTohRxExt64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  16 byte Trail Trace Message received in the J0 byte of a
**                  specified link within the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetTohTxCfgTtmLength and
**                  hyPhy20gSonetTohRxCfg16ByteTtm must be called for the link
**                  to configure the link for a 64 byte TTM and configure an
**                  expected TTM message.
**                  If Ttm uses CRLF as a synchronizing sign, the first 
**                  characters  after CRLF will be the start byte of the  
                    returned ttm.
**                  If Ttm uses msb bit (bit8) as a synchronizing sign, the byte 
**                  with MSB bit = 1 will be the start byte. 
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  extType      - Type of message to be extracted
**                                 0 - Captured
**                                 1 - Accepted
**                  *extMsg     - pointer to 64 byte TTM that has been extracted
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxExt64ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 extType,UINT4 *extMsg)
{
     /* variable declaration */
    UINT4 iAddrCfg;
    UINT4 iAddrCtrace;
    UINT4 iAddrAtrace;
    UINT4 msk;
    /* UINT4 offset;*/
    UINT4 sonetRttpRegData;
    UINT4 sonetRttpCtraceData;
    UINT4 sonetRttpAtraceData;
    UINT4 msg[64];
    UINT2 i;
    INT4 result;
    UINT4 startPos = 0;
    /* argument checking */
    if ((slice > 15) || (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddrCfg = 0x3; /* Trace Configuration */
    iAddrCtrace = 0x40; /* Captured Trace */
    iAddrAtrace = 0x80; /* Accepted Trace */

    /* Check if device is configured for 64 byte TTM */
    result = utilHyPhy20gSonetTohTtmByteLengthChk(fileHndl, slice, 5);
    if (result) return result;

    if (extType == 0)
    {
        for (i=0; i<64; i++)
        {
            /* Read Captured trace */
            result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddrCtrace,
                                                  &sonetRttpCtraceData);
            if (result) return result;

            /* Place inside message array */
            msg[i] = sonetRttpCtraceData;

            iAddrCtrace++;
        }
    }
    else if (extType == 1)
    {
        /* check if device is configured for Algorithm 2
           in SONET:RTTP indirect Trace Configuration memory (0x3) */
        result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddrCfg,
                                              &sonetRttpRegData);
        if (result) return result;

        /*
        msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
        offset = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_OFF;
        if (((sonetRttpRegData & msk) >> offset) != 0x2)
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        */
            
        for (i=0; i<64; i++)
        {
            /* Read Accepted Trace */
            result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddrAtrace,
                                                  &sonetRttpAtraceData);
            if (result) return result;

            msg[i] = sonetRttpAtraceData;

            iAddrAtrace++;
        }
    }

     /* iAddr = 0x3; SONET_RTTP Indirect Trace configuration */
    result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, 0x3,
                                          &sonetRttpRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_SYNC_CRLF_MSK;
    startPos = 0;
    for (i=0; i<64; i++) {
       if((sonetRttpRegData & msk) == 0){
           if(msg[i]>0x80) {
             startPos = i;
             break;
           }
       }else{ // start at CRLF
           if(i<63){
               if(msg[i]==0x0D && msg[i+1]==0x0A) {
                    startPos = i+2;
                    break;
               }
           }
       }
    }

    for(i=0; i<64; i++){
      extMsg[i] = msg[(startPos>63)?(startPos-64):startPos];
      startPos++;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxExt64ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetPohRxExt16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  16 byte Trail Trace Message received in the J1 byte of a
**                  specified path within the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetPohRxCfgTtmLength and
**                  hyPhy20gSonetPohRxCfg16ByteTtm must be called for the link
**                  to configure the link for a 16 byte TTM and configure an
**                  expected TTM message.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  extType     - Type of message to be extracted
**                                 0 - Captured
**                                 1 - Accepted
**                  *extE164Crc - pointer to E.164 CRC associated with the
**                                extracted 15 byte message.
**                  *extMsg[16] - pointer to 16 byte TTM that has been extracted
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_TIU
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxExt16ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 extType,
                                    UINT4 *extE164Crc, UINT4 *extMsg)
{
    /* variable declaration */
    UINT4 sonetRttpCtraceData;
    UINT4 sonetRttpAtraceData;
    UINT4 sonetRttpRegData;
    UINT4 sonetRttpStatRegData;
    UINT4 iAddrCfg;
    UINT4 iAddrStat;
    UINT4 numCheckTiu = 0;
    UINT4 iAddrCtrace;
    UINT4 iAddrAtrace;
    UINT4 i;
    UINT4 msk;
    UINT4 offset;
    UINT4 sts3Off;
    UINT4 crcLocation = 0;
    UINT4 msg[16];
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num  > 2) ||
        (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddrCfg = 0x3;
    iAddrCtrace = 0x40;
    iAddrAtrace = 0x80;
    iAddrStat = 0x0;

    sts3Off = 4*sts12Num + sts3Num;

    /* Check if device is configured for 16 byte TTM */
    result = utilHyPhy20gSonetPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 1);
    if (result) return result;


    if (extType == 0) {

        /* Set TTMMONEN to enable proper Path trace monitoring */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                              sts3Off, sts1Num,
                                              &sonetRttpRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
        if (result) return result;

        result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                               sts3Off, sts1Num,
                                               sonetRttpRegData);
        if (result) return result;

        /* WAIT for valid captured message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;

        for (i=0; i<16; i++)
        {
            /* Read Captured trace */
            result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCtrace,
                                                  sts3Off, sts1Num,
                                                  &sonetRttpCtraceData);
            if (result) return result;

            /* Place inside message array */
            msg[i] = sonetRttpCtraceData;

            if (sonetRttpCtraceData >= 0x80)
            {
                crcLocation = i;
                *extE164Crc = sonetRttpCtraceData;
            }

            iAddrCtrace++;
        }
    } else if (extType == 1) {

        /* check if device is configured for Algorithm 2
           in SONET:RTTP indirect Trace Configuration memory (0x3) */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                              sts3Off, sts1Num,
                                              &sonetRttpRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
        offset = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_OFF;

        if (((sonetRttpRegData & msk) >> offset) != 0x2)
            return HYPHY20G_ERR_INVALID_PRECONFIG;

        /* Set TTMMONEN to enable proper Path trace monitoring */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                              sts3Off, sts1Num,
                                              &sonetRttpRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
        if (result) return result;

        result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                               sts3Off, sts1Num,
                                               sonetRttpRegData);
        if (result) return result;

        /* WAIT for valid captured message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;

        /* Wait for potential TIU assertion at indirect 0x0 */
        result = sysHyPhy20gMsDelay(100);
        if (result) return result;

        /* Check TIU, if TIU = 0, accepted msgs are valid
           else wait for TIU = 0 before reading */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                              sts3Off, sts1Num,
                                              &sonetRttpStatRegData);
        if (result) return result;

        /* While TIU is asserted and number of read accesses < 10 */
        while((sonetRttpStatRegData & 1) && (numCheckTiu < 10))
        {
            result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                                  sts3Off, sts1Num,
                                                  &sonetRttpStatRegData);
            if (result) return result;

            numCheckTiu++;
        }

        if(numCheckTiu == 10)
            return HYPHY20G_ERR_TIU;

        for (i=0; i<16; i++)
        {
            /* Read Accepted Trace */
            result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrAtrace,
                                                  sts3Off, sts1Num,
                                                  &sonetRttpAtraceData);
            if (result) return result;

            msg[i] = sonetRttpAtraceData;

            if (sonetRttpAtraceData >= 0x80)
            {
                crcLocation = i;
                *extE164Crc = sonetRttpAtraceData;
            }
            iAddrAtrace++;
        }
    }

    result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                          sts3Off, sts1Num, &sonetRttpRegData);
    if (result) return result;

    /* Set TTMMONEN to 0 to enable monitoring on all paths */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);

    if (result) return result;

    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg, sts3Off,
                                           sts1Num, sonetRttpRegData);
    if (result) return result;

    for (i=crcLocation; i<15; i++)
    {
        /* store where message starts in first element */
        extMsg[i-crcLocation] = msg[i+1];
    }

    for (i=0; i<crcLocation; i++)
    {
        /* store the remainder of the message starting from where CRC left off*/
        extMsg[i + 15 - crcLocation] = msg[i];
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxExt16ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetPohRxExt64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  64 byte Trail Trace Message received in the J1 byte of a
**                  specified path within the SONET subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gSonetPohTxCfgTtmLength and
**                  hyPhy20gSonetPohRxCfg64ByteTtm must be called for the link
**                  to configure the link for a 64 byte TTM and configure an
**                  expected TTM message.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  extType     - Type of message to be extracted
**                                0 - Captured
**                                1 - Accepted
**                  *extMsg[64] - pointer to 64 byte TTM that has been extracted
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_TIU
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxExt64ByteTtm(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts12Num, UINT4 sts3Num,
                                    UINT4 sts1Num, UINT4 extType, UINT4 *extMsg)
{
    /* variable declaration */
    UINT4 sonetRttpCtraceData;
    UINT4 sonetRttpAtraceData;
    UINT4 sonetRttpRegData;
    UINT4 sonetRttpStatRegData;
    UINT4 lastRttpCtraceData = 0;
    UINT4 lastRttpAtraceData = 0;
    UINT4 iAddrCfg;
    UINT4 iAddrStat;
    UINT4 iAddrCtrace;
    UINT4 iAddrAtrace;
    UINT4 numCheckTiu = 0;
    UINT4 i;
    UINT4 sts3Off;
    UINT4 msk;
    UINT4 offset;
    UINT4 cRLFLocation = 100;
    UINT4 msg[64];
    UINT4 CR = 0x0D;
    UINT4 LF = 0x0A;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
        (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddrCfg = 0x3;
    iAddrCtrace = 0x40;
    iAddrAtrace = 0x80;
    iAddrStat = 0x0;

    sts3Off = 4*sts12Num + sts3Num;

    /* Check if device is configured for 64 byte TTM */
    result = utilHyPhy20gSonetPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 3);
    if (result) return result;

    if (extType == 0) {

        /* Set TTMMONEN to enable proper Path trace monitoring */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                              sts3Off, sts1Num,
                                              &sonetRttpRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
        if (result) return result;

        result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                               sts3Off, sts1Num,
                                               sonetRttpRegData);
        if (result) return result;

        /* WAIT for valid captured message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;

        for (i=0; i<64; i++)
        {
            /* Read Captured trace */
            result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCtrace,
                                                  sts3Off, sts1Num,
                                                  &sonetRttpCtraceData);
            if (result) return result;

            /* Place inside message array */
            msg[i] = sonetRttpCtraceData;

            /* Look for CRLF to determine the end of the message */
            if ((sonetRttpCtraceData == LF)&&(lastRttpCtraceData == CR))
                cRLFLocation = i;

            lastRttpCtraceData = sonetRttpCtraceData;

            iAddrCtrace++;
        }
    }
    else if (extType == 1) {

        /* check if device is configured for Algorithm 2
           in SONET:RTTP indirect Trace Configuration memory (0x3) */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                              sts3Off, sts1Num,
                                              &sonetRttpRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
        offset = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_OFF;
        if (((sonetRttpRegData & msk) >> offset) != 0x2)
            return HYPHY20G_ERR_INVALID_PRECONFIG;

        /* Set TTMMONEN to enable proper Path trace monitoring */
        msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
        result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 1);
        if (result) return result;

        result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                               sts3Off, sts1Num,
                                               sonetRttpRegData);
        if (result) return result;

        /* WAIT for valid accepted message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;

        /* Wait for potential TIU assertion at indirect 0x0 */
        result = sysHyPhy20gMsDelay(250);
        if (result) return result;


        /* Check TIU, if TIU = 0, accepted msgs are valid
           else wait for TIU = 0 before reading */
        result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                              sts3Off, sts1Num,
                                              &sonetRttpStatRegData);
        if (result) return result;

        /* While TIU is asserted and number of read accesses < 10 */
        while((sonetRttpStatRegData & 1) && (numCheckTiu < 10))
        {
            result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                                  sts3Off, sts1Num,
                                                  &sonetRttpStatRegData);
            if (result) return result;

            numCheckTiu++;
        }

        if(numCheckTiu == 10)
            return HYPHY20G_ERR_TIU;

        for (i=0; i<64; i++)
        {
            /* Read Accepted Trace */
            result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrAtrace,
                                                  sts3Off, sts1Num,
                                                  &sonetRttpAtraceData);
            if (result) return result;

            msg[i] = sonetRttpAtraceData;

            /* Look for CRLF to determine the end of the message */
            if ((sonetRttpAtraceData == LF)&&(lastRttpAtraceData == CR))
                cRLFLocation = i;

            lastRttpAtraceData = sonetRttpAtraceData;

            iAddrAtrace++;
        }
    }

    result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg, sts3Off,
                                          sts1Num, &sonetRttpRegData);
    if (result) return result;

    /* Set TTMMONEN to 0 to enable monitoring on all paths */
    msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRttpRegData, msk, 0);
    if (result) return result;

    result = sonetHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg, sts3Off,
                                           sts1Num, sonetRttpRegData);
    if (result) return result;

    if (cRLFLocation != 100) {
        for (i=cRLFLocation; i<63; i++)
        {
            /* store where message starts in first element */
            extMsg[i-cRLFLocation] = msg[i+1];
        }

        for (i=0; i<cRLFLocation + 1; i++)
        {
            /* store the remainder of the message starting from
               where CRLF left off*/
            extMsg[i + 63 - cRLFLocation] = msg[i];
        }
    } else {
        for (i=0; i<64; i++)
        {
            extMsg[i] = msg[i];
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxExt64ByteTtm */


/*******************************************************************************
**
**  hyPhy20gSonetTohOc3SdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Line B2
**               Bit Error Rate Monitoring for various bit error rates to be
**               used for the generation of SD and SF states for an OC-3/STM-1
**               rate within the SONET subsystem.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               berModeSD   - BER Configuration Setting for Signal Degrade
**                             0 - Disabled
**                             1 - 1.E-03
**                             2 - 1.E-04
**                             3 - 1.E-05
**                             4 - 1.E-06
**                             5 - 1.E-07
**                             6 - 1.E-08
**                             7 - 1.E-09
**                             8 - 1.E-10
**                             9 - 1.E-11
**               berModeSF   - BER Configuration Setting for Signal Failure
**                             0 - Disabled
**                             1 - 1.E-03
**                             2 - 1.E-04
**                             3 - 1.E-05
**                             4 - 1.E-06
**                             5 - 1.E-07
**                             6 - 1.E-08
**                             7 - 1.E-09
**                             8 - 1.E-10
**                             9 - 1.E-11
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohOc3SdSfCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 berModeSD, UINT4 berModeSF)
{
    /* variable declaration */
    UINT4 sdip2 = 0x0;
    UINT4 sdip1 = 0x0;
    UINT4 sdst = 0x0;
    UINT4 sdct = 0x0;
    UINT4 sfip2 = 0x0;
    UINT4 sfip1 = 0x0;
    UINT4 sfst = 0x0;
    UINT4 sfct = 0x0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15) || (berModeSD > 9) || (berModeSF > 9)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* To set BERM settings for SD */

    switch (berModeSD) {
        case 0:
            sdEnbl = 0x0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x07;
            sdst = 0x2D;
            sdct = 0x26;
            break;            
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x17;
            sdst = 0x18;
            sdct = 0x11;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0xc7;
            sdst = 0x16;
            sdct = 0xf;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x7a8;
            sdst = 0x15;
            sdct = 0xf;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0x4c79;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x2;
            sdip1 = 0xfca4;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x1d;
            sdip1 = 0xde4a;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0x12a;
            sdip1 = 0xaecc;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 9:
            sdip2 = 0xbaa;
            sdip1 = 0xd3d9;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /*  To set BERM settings for SF */
    switch (berModeSF) {
        case 0:
            sfEnbl = 0x0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x07;
            sfst = 0x2D;
            sfct = 0x26;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x17;
            sfst = 0x18;
            sfct = 0x11;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0xc7;
            sfst = 0x16;
            sfct = 0xf;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x7a8;
            sfst = 0x15;
            sfct = 0xf;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0x4c79;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x2;
            sfip1 = 0xfca4;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x1d;
            sfip1 = 0xde4a;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0x12a;
            sfip1 = 0xaecc;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 9:
            sfip2 = 0xbaa;
            sfip1 = 0xd3d9;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberLineSdCfg */
    result = utilHyPhy20gSonetSberLineSdCfg(fileHndl, slice, sdip2, sdip1,
                                            sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberLineSfCfg */
    result = utilHyPhy20gSonetSberLineSfCfg(fileHndl, slice, sfip2, sfip1,
                                            sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohOc3SdSfCfg */


/*******************************************************************************
**
**  hyPhy20gSonetTohOc12SdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Line B2
**               Bit Error Rate Monitoring for various bit error rates to be
**               used for the generation of SD and SF states for an OC-12/STM-4
**               rate within the SONET subsystem.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               berModeSD   - BER Configuration Setting for Signal Degrade
**                             0 - Disabled
**                             1 - 1.E-03
**                             2 - 1.E-04
**                             3 - 1.E-05
**                             4 - 1.E-06
**                             5 - 1.E-07
**                             6 - 1.E-08
**                             7 - 1.E-09
**                             8 - 1.E-10
**                             9 - 1.E-11
**               berModeSF   - BER Configuration Setting for Signal Failure
**                             0 - Disabled
**                             1 - 1.E-03
**                             2 - 1.E-04
**                             3 - 1.E-05
**                             4 - 1.E-06
**                             5 - 1.E-07
**                             6 - 1.E-08
**                             7 - 1.E-09
**                             8 - 1.E-10
**                             9 - 1.E-11
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohOc12SdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 berModeSD, UINT4 berModeSF)
{
    /* variable declaration */
    UINT4 sdip2 = 0;
    UINT4 sdip1 = 0;
    UINT4 sdst = 0;
    UINT4 sdct = 0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15) || (berModeSD > 9) || (berModeSF > 9)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* To set BERM settings for SD */

    switch (berModeSD) {
        case 0:
            sdEnbl = 0x0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x2;
            sdst = 0x36;
            sdct = 0x2B;
            break;            
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x6;
            sdst = 0x19;
            sdct = 0x11;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0x32;
            sdst = 0x16;
            sdct = 0xf;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x1ea;
            sdst = 0x15;
            sdct = 0xf;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0x131f;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x0;
            sdip1 = 0xbf29;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x7;
            sdip1 = 0x7793;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0x4a;
            sdip1 = 0xabb3;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 9:
            sdip2 = 0x2ea;
            sdip1 = 0xb4f7;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /*  To set BERM settings for SF */
    switch (berModeSF) {
        case 0:
            sfEnbl = 0x0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x2;
            sfst = 0x36;
            sfct = 0x2B;
            break;            
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x6;
            sfst = 0x19;
            sfct = 0x11;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0x32;
            sfst = 0x16;
            sfct = 0xf;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x1ea;
            sfst = 0x15;
            sfct = 0xf;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0x131f;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x0;
            sfip1 = 0xbf29;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x7;
            sfip1 = 0x7793;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0x4a;
            sfip1 = 0xabb3;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 9:
            sfip2 = 0x2ea;
            sfip1 = 0xb4f7;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberLineSdCfg */
    result = utilHyPhy20gSonetSberLineSdCfg(fileHndl, slice, sdip2, sdip1,
                                            sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberLineSfCfg */
    result = utilHyPhy20gSonetSberLineSfCfg(fileHndl, slice, sfip2, sfip1,
                                            sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohOc12SdSfCfg */

/*******************************************************************************
**
**  hyPhy20gSonetTohOc48SdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Line B2
**               Bit Error Rate Monitoring for various bit error rates to be
**               used for the generation of SD and SF states for an OC-48/STM-16
**               rate within the SONET subsystem.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               berModeSD   - BER Configuration Setting for SIgnal Degrade
**                             0 - Disabled
**                             1 - 1.E-03
**                             2 - 1.E-04
**                             3 - 1.E-05
**                             4 - 1.E-06
**                             5 - 1.E-07
**                             6 - 1.E-08
**                             7 - 1.E-09
**                             8 - 1.E-10
**                             9 - 1.E-11
**               berModeSF   - BER Configuration Setting for Signal Failure
**                             0 - Disabled
**                             1 - 1.E-03
**                             2 - 1.E-04
**                             3 - 1.E-05
**                             4 - 1.E-06
**                             5 - 1.E-07
**                             6 - 1.E-08
**                             7 - 1.E-09
**                             8 - 1.E-10
**                             9 - 1.E-11
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohOc48SdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 berModeSD, UINT4 berModeSF)
{   /* variable declaration */
    UINT4 sdip2 = 0x0;
    UINT4 sdip1 = 0x0;
    UINT4 sdst = 0x0;
    UINT4 sdct = 0x0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15) || (berModeSD > 9) || (berModeSF > 9)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* To set BERM settings for SD */
    switch (berModeSD) {
        case 0:
            sdEnbl = 0x0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x1;
            sdst = 0x79;
            sdct = 0x4D;
            break;
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x2;
            sdst = 0x25;
            sdct = 0x16;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0xd;
            sdst = 0x17;
            sdct = 0x10;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x7b;
            sdst = 0x15;
            sdct = 0xf;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0x4c8;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x0;
            sdip1 = 0x2fcb;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x1;
            sdip1 = 0xdde5;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0x12;
            sdip1 = 0xaaf0;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 9:
            sdip2 = 0xba;
            sdip1 = 0xad5a;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* To set BERM settings for SF */
    switch (berModeSF) {
        case 0:
            sfEnbl = 0x0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x1;
            sfst = 0x79;
            sfct = 0x4D;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x2;
            sfst = 0x25;
            sfct = 0x16;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0xd;
            sfst = 0x17;
            sfct = 0x10;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x7b;
            sfst = 0x15;
            sfct = 0xf;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0x4c8;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x0;
            sfip1 = 0x2fcb;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x1;
            sfip1 = 0xdde5;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0x12;
            sfip1 = 0xaaf0;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 9:
            sfip2 = 0xba;
            sfip1 = 0xad5a;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberLineSdSfCfg */
    result = utilHyPhy20gSonetSberLineSdCfg(fileHndl, slice, sdip2, sdip1,
                                            sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberLineSfCfg */
    result = utilHyPhy20gSonetSberLineSfCfg(fileHndl, slice, sfip2, sfip1,
                                            sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohOc48SdSfCfg */


/*******************************************************************************
**
**  hyPhy20gSonetTohOc192SdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function has the capability of configuring the Line B2
**                  Bit Error Rate Monitoring for various bit error rates to be
**                  used for the generation of SD and SF states for an
**                  OC-192/STM-48 rate within the SONET subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                   slice        0 - SONET slices 0-3
**                                1 - SONET slices 4-7
**                                2 - SONET slices 8-11
**                                3 - SONET slices 12-15
**                  berModeSD   - BER Configuration Setting for Signal Degrade
**                                0 - Disabled
**                                1 - 1.E-04
**                                2 - 1.E-05
**                                3 - 1.E-06
**                                4 - 1.E-07
**                                5 - 1.E-08
**                                6 - 1.E-09
**                                7 - 1.E-10
**                                8 - 1.E-11
**                  berModeSF   - BER Configuration Setting for Signal Failure
**                                0 - Disabled
**                                1 - 1.E-04
**                                2 - 1.E-05
**                                3 - 1.E-06
**                                4 - 1.E-07
**                                5 - 1.E-08
**                                6 - 1.E-09
**                                7 - 1.E-10
**                                8 - 1.E-11
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohOc192SdSfCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 berModeSD, UINT4 berModeSF)
{
    /* variable declaration */
    UINT4 sdip2 = 0x0;
    UINT4 sdip1 = 0x0;
    UINT4 sdst = 0x0;
    UINT4 sdct = 0x0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    UINT4 i;
    INT4 result;

    /* argument checking */
    if ((slice > 3) || (berModeSD > 8) || (berModeSF > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* To set BERM settings for SD */
    switch (berModeSD) {
        case 0:
            sdEnbl = 0x0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x1;
            sdst = 0x56;
            sdct = 0x26;
            break;
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x4;
            sdst = 0x1e;
            sdct = 0x12;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0x1f;
            sdst = 0x16;
            sdct = 0xf;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x132;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0xbf3;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x0;
            sdip1 = 0x777a;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x4;
            sdip1 = 0xaabc;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0x2e;
            sdip1 = 0xab57;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* To set BERM settings for SF */
    switch (berModeSF) {
        case 0:
            sfEnbl = 0x0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x1;
            sfst = 0x56;
            sfct = 0x26;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x4;
            sfst = 0x1e;
            sfct = 0x12;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0x1f;
            sfst = 0x16;
            sfct = 0xf;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x132;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0xbf3;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x0;
            sfip1 = 0x777a;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x4;
            sfip1 = 0xaabc;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0x2e;
            sfip1 = 0xab57;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberSectSdSfCfg for all 4 slices in OC192 */
    for (i = 4*slice; i < (4*slice) + 4; i++) {
        result = utilHyPhy20gSonetSberLineSdCfg(fileHndl, i, sdip2,
                                                sdip1, sdst, sdct, sdEnbl);
        if (result) return result;
    }

    /* make call to utilHyPhy20gSberLineSdCfg for all 4 slices in OC192 */
    for (i = 4*slice; i < (4*slice) + 4; i++) {
        result = utilHyPhy20gSonetSberLineSfCfg(fileHndl, i, sfip2,
                                                sfip1, sfst, sfct, sfEnbl);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy0gSonetTohOc192SdSfCfg */


/*******************************************************************************
**
**  hyPhy20gSonetPohSts1SdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function has the capability of configuring the Path B3
**                  Bit Error Rate Monitoring for a STS-1/AU-3 path within the
**                  SONET subsystem
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12       - 0 to 3
**                  sts3        - 0 to 3
**                  sts1        - 0 to 2
**                  berModeSD   - BER Configuration Setting for Sigal Degrade
**                              0 - Disabled
**                              1 - 1.E-03
**                              2 - 1.E-04
**                              3 - 1.E-05
**                              4 - 1.E-06
**                              5 - 1.E-07
**                              6 - 1.E-08
**                              7 - 1.E-09
**                              8 - 1.E-10
**                  berModeSF   - BER Configuration Setting for Signal Failure
**                              0 - Disabled
**                              1 - 1.E-03
**                              2 - 1.E-04
**                              3 - 1.E-05
**                              4 - 1.E-06
**                              5 - 1.E-07
**                              6 - 1.E-08
**                              7 - 1.E-09
**                              8 - 1.E-10
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohSts1SdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12, UINT4 sts3, UINT4 sts1,
                                 UINT4 berModeSD, UINT4 berModeSF)
{
    /* variable declaration */
    UINT4 sdip2 = 0;
    UINT4 sdip1 = 0;
    UINT4 sdst = 0;
    UINT4 sdct = 0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    UINT4 sts3Off;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15) || (sts12 > 3) || (sts3 > 3) || (sts1 > 2) ||
        (berModeSD > 8) || (berModeSF > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12 + sts3;

    switch (berModeSD) {
        case 0:
            sdEnbl = 0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x14;
            sdst = 0x2A;
            sdct = 0x24;
            break;
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x44;
            sdst = 0x17;
            sdct = 0x10;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0x261;
            sdst = 0x16;
            sdct = 0xF;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x177F;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0xEAB1;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x9;
            sdip1 = 0x2AA7;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x5B;
            sdip1 = 0xAA40;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0x394;
            sdip1 = 0xA638;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* To set BERM settings for SF */

    switch (berModeSF) {
        case 0:
            sfEnbl = 0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x14;
            sfst = 0x2A;
            sfct = 0x24;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x44;
            sfst = 0x17;
            sfct = 0x10;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0x261;
            sfst = 0x16;
            sfct = 0xF;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x177F;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0xEAB1;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x9;
            sfip1 = 0x2AA7;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x5B;
            sfip1 = 0xAA40;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0x394;
            sfip1 = 0xA638;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberPathSdCfg */
    result = utilHyPhy20gSonetSberPathSdCfg(fileHndl, slice, sts3Off, sts1,
                                            sdip2, sdip1, sdst, sdct,
                                            sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberPathSdCfg */
    result = utilHyPhy20gSonetSberPathSfCfg(fileHndl, slice, sts3Off, sts1,
                                            sfip2, sfip1, sfst, sfct,
                                            sfEnbl);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohSts1SdSfCfg */

/*******************************************************************************
**
**  hyPhy20gSonetPohSts3cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function has the capability of configuring the Path B3
**                  Bit Error Rate Monitoring for a STS-3c/AU-4 path within the
**                  SONET subsystem
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12       - 0 to 3
**                  sts3c       - 0 to 3
**                  berModeSD   - BER Configuration Setting for Signal Degrade
**                              0 - Disabled
**                              1 - 1.E-04
**                              2 - 1.E-05
**                              3 - 1.E-06
**                              4 - 1.E-07
**                              5 - 1.E-08
**                              6 - 1.E-09
**                              7 - 1.E-10
**                              8 - 1.E-11
**                  berModeSF   - BER Configuration Setting for Signal Failure
**                              0 - Disabled
**                              1 - 1.E-04
**                              2 - 1.E-05
**                              3 - 1.E-06
**                              4 - 1.E-07
**                              5 - 1.E-08
**                              6 - 1.E-09
**                              7 - 1.E-10
**                              8 - 1.E-11
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohSts3cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 sts12, UINT4 sts3c, UINT4 berModeSD,
                                  UINT4 berModeSF)
{
    /* variable declaration */
    UINT4 sdip2 = 0;
    UINT4 sdip1 = 0;
    UINT4 sdst = 0;
    UINT4 sdct = 0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    UINT4 sts3Off;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15) || (sts12 > 3) || (sts3c > 3) || (berModeSD > 8) ||
        (berModeSF > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12 + sts3c;

    /* To set BERM settings for SD */
    switch (berModeSD) {
        case 0:
            sdEnbl = 0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x1D;
            sdst = 0x1A;
            sdct = 0x14;
            break;
        case 2:
            sdip2 = 0x0;
            sdip1 = 0xD0;
            sdst = 0x16;
            sdct = 0xF;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0x7DA;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x4E40;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 5:
            sdip2 = 0x3;
            sdip1 = 0x0E3D;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x1E;
            sdip1 = 0x8E1B;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x131;
            sdip1 = 0x8CC3;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0xBEF;
            sdip1 = 0x7F53;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* To set BERM settings for SF */

    switch (berModeSF) {
        case 0:
            sfEnbl = 0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x1D;
            sfst = 0x1A;
            sfct = 0x14;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0xD0;
            sfst = 0x16;
            sfct = 0xF;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0x7DA;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x4E40;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 5:
            sfip2 = 0x3;
            sfip1 = 0x0E3D;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x1E;
            sfip1 = 0x8E1B;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x131;
            sfip1 = 0x8CC3;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0xBEF;
            sfip1 = 0x7F53;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSonetSberPathSdCfg */
    result = utilHyPhy20gSonetSberPathSdCfg(fileHndl, slice, sts3Off, 0,
                                            sdip2, sdip1, sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSonetSberPathSfCfg */
    result = utilHyPhy20gSonetSberPathSfCfg(fileHndl, slice, sts3Off, 0,
                                            sfip2, sfip1, sfst, sfct, sfEnbl);
    if (result) return result;


    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetPohSts3cSdSfCfg */


/*******************************************************************************
**
**  hyPhy20gSonetPohSts12cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function has the capability of configuring the Path B3
**                  Bit Error Rate Monitoring for a STS-12c/AU-4-4c path within
**                  the SONET subsystem
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12c      - 0 to 3
**                  berModeSD   - BER Configuration Setting for Signal Degrade
**                              0 - Disabled
**                              1 - 1.E-04
**                              2 - 1.E-05
**                              3 - 1.E-06
**                              4 - 1.E-07
**                              5 - 1.E-08
**                              6 - 1.E-09
**                              7 - 1.E-10
**                              8 - 1.E-11
**                  berModeSF   - BER Configuration Setting for Signal Failure
**                              0 - Disabled
**                              1 - 1.E-04
**                              2 - 1.E-05
**                              3 - 1.E-06
**                              4 - 1.E-07
**                              5 - 1.E-08
**                              6 - 1.E-09
**                              7 - 1.E-10
**                              8 - 1.E-11
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohSts12cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12c, UINT4 berModeSD,
                                   UINT4 berModeSF)
{
    /* variable declaration */
    UINT4 sdip2 = 0;
    UINT4 sdip1 = 0;
    UINT4 sdst = 0;
    UINT4 sdct = 0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    UINT4 sts3;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15) || (sts12c > 3) || (berModeSD > 8) || (berModeSF > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* STS-3 Number calculation based on STS-12c number */
    sts3 = (sts12c * 4);

    /* To set BERM settings for SD */

    switch (berModeSD) {
        case 0:
            sdEnbl = 0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x14;
            sdst = 0x2e;
            sdct = 0x29;
            break;
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x3b;
            sdst = 0x18;
            sdct = 0x11;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0x1fd;
            sdst = 0x16;
            sdct = 0xF;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x1396;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0xc395;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x7;
            sdip1 = 0xa38d;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x4c;
            sdip1 = 0x6337;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0x2fb;
            sdip1 = 0xdfdb;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* To set BERM settings for SF */
    switch (berModeSF) {
        case 0:
            sfEnbl = 0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x14;
            sfst = 0x2e;
            sfct = 0x29;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x3b;
            sfst = 0x18;
            sfct = 0x11;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0x1fd;
            sfst = 0x16;
            sfct = 0xF;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x1396;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0xc395;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x7;
            sfip1 = 0xa38d;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x4c;
            sfip1 = 0x6337;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0x2fb;
            sfip1 = 0xdfdb;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberPathSdSfCfg */
    result = utilHyPhy20gSonetSberPathSdCfg(fileHndl, slice, sts3, 0, sdip2,
                                            sdip1, sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberPathSdSfCfg */
    result = utilHyPhy20gSonetSberPathSfCfg(fileHndl, slice, sts3, 0, sfip2,
                                            sfip1, sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetPohSts12cSdSfCfg */

/*******************************************************************************
**
**  hyPhy20gSonetPohSts48cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function has the capability of configuring the Path B3
**                  Bit Error Rate Monitoring for a STS-48c/AU-4-16c path within
**                  the SONET subsystem.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               berModeSD   - BER Configuration Setting for Signal Degrade
**                             0 - Disabled
**                             1 - 1.E-04
**                             2 - 1.E-05
**                             3 - 1.E-06
**                             4 - 1.E-07
**                             5 - 1.E-08
**                             6 - 1.E-09
**                             7 - 1.E-10
**                             8 - 1.E-11
**               berModeSF   - BER Configuration Setting for Signal Faliure
**                             0 - Disabled
**                             1 - 1.E-04
**                             2 - 1.E-05
**                             3 - 1.E-06
**                             4 - 1.E-07
**                             5 - 1.E-08
**                             6 - 1.E-09
**                             7 - 1.E-10
**                             8 - 1.E-11
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohSts48cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 berModeSD, UINT4 berModeSF)
{
     /* variable declaration */
    UINT4 sdip2 = 0;
    UINT4 sdip1 = 0;
    UINT4 sdst = 0;
    UINT4 sdct = 0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 15) || (berModeSD > 8) || (berModeSF > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* To set BERM settings for SD */
    switch (berModeSD) {
        case 0:
            sdEnbl = 0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x54;
            sdst = 0x120;
            sdct = 0x194;
            break;
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x17;
            sdst = 0x1e;
            sdct = 0x17;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0x85;
            sdst = 0x16;
            sdct = 0x10;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0x4ec;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0x30eb;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x1;
            sdip1 = 0xe8e9;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x13;
            sdip1 = 0x18d4;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 8:
            sdip2 = 0xbe;
            sdip1 = 0xf7fd;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* To set BERM settings for SF */
    switch (berModeSF) {
        case 0:
            sfEnbl = 0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x54;
            sfst = 0x120;
            sfct = 0x194;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x17;
            sfst = 0x1e;
            sfct = 0x17;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0x85;
            sfst = 0x16;
            sfct = 0x10;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0x4ec;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0x30eb;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x1;
            sfip1 = 0xe8e9;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x13;
            sfip1 = 0x18d4;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 8:
            sfip2 = 0xbe;
            sfip1 = 0xf7fd;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberPathSdSfCfg */
    result = utilHyPhy20gSonetSberPathSdCfg(fileHndl, slice, 0, 0,
                                            sdip2, sdip1, sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberPathSdSfCfg */
    result = utilHyPhy20gSonetSberPathSfCfg(fileHndl, slice, 0, 0,
                                            sfip2, sfip1, sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohSts48cSdSfCfg */


/*******************************************************************************
**
**  hyPhy20gSonetPohSts192cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function has the capability of configuring the Path B3
**                  Bit Error Rate Monitoring for a STS-192c/AU-4-48c path
**                  within the SONET subsystem
**
**  INPUTS:         *fileHndl   - base address
**                  slice         0 - SONET slices 0-3
**                                1 - SONET slices 4-7
**                                2 - SONET slices 8-11
**                                3 - SONET slices 12-15
**                  berModeSD   - BER Configuration Setting for Signal Degrade
**                                0 - Disabled
**                                1 - 1.E-05
**                                2 - 1.E-06
**                                3 - 1.E-07
**                                4 - 1.E-08
**                                5 - 1.E-09
**                                6 - 1.E-10
**                                7 - 1.E-11
**                  berModeSF   - BER Configuration Setting for Signal Failure
**                                0 - Disabled
**                                1 - 1.E-05
**                                2 - 1.E-06
**                                3 - 1.E-07
**                                4 - 1.E-08
**                                5 - 1.E-09
**                                6 - 1.E-10
**                                7 - 1.E-11
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohSts192cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 berModeSD, UINT4 berModeSF)
{
     /* variable declaration */
    UINT4 sdip2 = 0;
    UINT4 sdip1 = 0;
    UINT4 sdst = 0;
    UINT4 sdct = 0;
    UINT4 sfip2 = 0;
    UINT4 sfip1 = 0;
    UINT4 sfst = 0;
    UINT4 sfct = 0;
    UINT4 sdEnbl = 1;
    UINT4 sfEnbl = 1;
    UINT4 sliceNum;
    INT4 result;

    /* argument checking */
    if ((slice > 3) || (berModeSD > 7) || (berModeSF > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Slice mapping for STS-192c */
    sliceNum = slice * 4;

    /* To set BERM settings for SD */
    switch (berModeSD) {
        case 0:
            sdEnbl = 0;
            break;
        case 1:
            sdip2 = 0x0;
            sdip1 = 0x19;
            sdst = 0x45;
            sdct = 0x47;
            break;
        case 2:
            sdip2 = 0x0;
            sdip1 = 0x28;
            sdst = 0x18;
            sdct = 0x12;
            break;
        case 3:
            sdip2 = 0x0;
            sdip1 = 0x141;
            sdst = 0x16;
            sdct = 0xf;
            break;
        case 4:
            sdip2 = 0x0;
            sdip1 = 0xc41;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 5:
            sdip2 = 0x0;
            sdip1 = 0x7a40;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 6:
            sdip2 = 0x4;
            sdip1 = 0xc63b;
            sdst = 0x15;
            sdct = 0xF;
            break;
        case 7:
            sdip2 = 0x2f;
            sdip1 = 0xbe05;
            sdst = 0x15;
            sdct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* To set BERM settings for SF */
    switch (berModeSF) {
        case 0:
            sfEnbl = 0;
            break;
        case 1:
            sfip2 = 0x0;
            sfip1 = 0x19;
            sfst = 0x45;
            sfct = 0x47;
            break;
        case 2:
            sfip2 = 0x0;
            sfip1 = 0x28;
            sfst = 0x18;
            sfct = 0x12;
            break;
        case 3:
            sfip2 = 0x0;
            sfip1 = 0x141;
            sfst = 0x16;
            sfct = 0xf;
            break;
        case 4:
            sfip2 = 0x0;
            sfip1 = 0xc41;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 5:
            sfip2 = 0x0;
            sfip1 = 0x7a40;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 6:
            sfip2 = 0x4;
            sfip1 = 0xc63b;
            sfst = 0x15;
            sfct = 0xF;
            break;
        case 7:
            sfip2 = 0x2f;
            sfip1 = 0xbe05;
            sfst = 0x15;
            sfct = 0xF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* make call to utilHyPhy20gSberPathSdCfg */
    result = utilHyPhy20gSonetSberPathSdCfg(fileHndl, sliceNum, 0, 0, sdip2,
                                            sdip1, sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberPathSdCfg */
    result = utilHyPhy20gSonetSberPathSfCfg(fileHndl, sliceNum, 0, 0, sfip2,
                                            sfip1, sfst, sfct, sfEnbl);
    if (result) return result;


    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetPohSts192cSdSfCfg */

/*******************************************************************************
**
**  hyPhy20gSonetPohPslPdiCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the transmit VC-3/VC-4/VC-4-xC PSL
**                  and the expected receive PSL & PDI in the C2 byte for any
**                  path within the SONET subsystem
**
**                  By default the PSL is configured to transmit and
**                  expects a receive Signal Label of 0x00 (Unequipped) and PDI
**                  detection is disabled.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               sts12       - 0 to 3
**               sts3        - 0 to 3
**               sts1        - 0 to 2
**               pslValue    - Path Signal Label to be transmitted and expected
**                             to be received:
**                             Legal range 0x00 to 0xFF
**                             SONET/SDH standards define the use of a subset
**                             of this range. Refer to the relevant standards
**                             for actual values associated with the C2 Signal
**                             Label.
**               pdiRangeEn  - 1 - Expected PDI is 0xE1 to (0xE0 + pdiVal)
**                           - 0 - Expected PDI is (0xE0 + pdiVal)
**               pdiVal      - Controls expected PDI value as per equations
**                             above.  Valid values 0 - 0x1C.
**                             NOTE: A value of 0x0 for pdiVal will result in
**                             the disabling of PDI detection.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohPslPdiCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 sts12, UINT4 sts3, UINT4 sts1,
                               UINT4 pslValue, UINT4 pdiRangeEn, UINT4 pdiVal)
{
    UINT4 sonetRhapRegData;
    UINT4 sonetRhapPdiRegData;
    UINT4 sonetThppRegData;
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 c2Enbl = 1;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sts12 > 3) || (sts3 > 3) || (sts1 > 2) ||
        (pslValue > 0xFF) || (pdiRangeEn > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4 * sts12 + sts3;

    /* Insert the PSL value into the C2 byte in SONET:THPP block
       Read THPP J1 and C2 register at 0x49405C0 + 0x01000*A(A=0:15) */
    iAddr = 0x3; /* Indirect J1 and C2 */
    result = sonetHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                      &sonetThppRegData);
    if (result) return result;

    /* write C2 byte */
    msk = HYPHY20G_SONET_THPP_THPP48_IND_IBIT_THPP_J1_C2_C2V_MSK;
    result = hyPhy20gVarFieldWrite(&sonetThppRegData, msk, pslValue);
    if (result) return result;

    /* Write to SONET:THPP J1 and C2 register indirectly at 0x3 */
    result = sonetHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                       sonetThppRegData);
    if (result) return result;

    /* Enable insertion of the C2 byte
       Read from SONET:THPP Priority Cfg register indirectly at 0x1 */
    iAddr = 0x1;
    result = sonetHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                      &sonetThppRegData);
    if (result) return result;

    /* Write REGC2EN bit */
    msk =
   HYPHY20G_SONET_THPP_THPP48_IND_IBIT_THPP_PRIORITY_CFG_REGC2EN_MSK;
    result = hyPhy20gVarFieldWrite(&sonetThppRegData, msk, c2Enbl);
    if (result) return result;

    /* Write to Priority Cfg register indirectly at 0x1 */
    result = sonetHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                       sonetThppRegData);
    if (result) return result;


    /* Write the expected PSL in SONET:RHAP block 0x4940200 + 0x01000*A(A=0:15)
       Read RHAP Expected PSL and PDI register indirectly at 0x4 */
    iAddr = 0x4;
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                      &sonetRhapRegData);
    if (result) return result;

    /* Write EPSL[7:0] with user supplied pslValue */
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_EPSL_PDI_EPSL_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRhapRegData, msk, pslValue);
    if (result) return result;

    /* Write RHAP Expected PSL and PDI register indirectly at 0x4 */
    result = sonetHyPhy20gRHAPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                       sonetRhapRegData);
    if (result) return result;

    /* Read RHAP Expected PSL and PDI register indirectly at 0x4 */
    iAddr = 0x4;
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                      &sonetRhapPdiRegData);
    if (result) return result;

    /* Modify PDI and PDIRANGE with the user supplied pdiRangeEn and pdiVal */
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_EPSL_PDI_PDIRANGE_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRhapPdiRegData, msk, pdiRangeEn);
    if (result) return result;

    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_EPSL_PDI_PDI_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRhapPdiRegData, msk, pdiVal);
    if (result) return result;

    /* Write PDI[4:0] and PDIRange in SONET:RHAP
       Expected PSL and PDI indirect register 0x4 */
    result = sonetHyPhy20gRHAPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                       sonetRhapPdiRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohPslPdiCfg */

/*******************************************************************************
**
**  hyPhy20gSonetPohExtPsl
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  Path Signal Label received in the C2 byte of a
**                  specified path within the SONET subsystem.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12        - 0 to 3
**                  sts3         - 0 to 3
**                  sts1         - 0 to 2
**                  extType      - Type of message to be extracted
**                                 0 - Captured
**                                 1 - Accepted
**                  *extMsg      - pointer to 1 byte PSL that has been extracted
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohExtPsl(struct file *fileHndl, UINT4 slice, UINT4 sts12,
                            UINT4 sts3, UINT4 sts1, UINT4 extType,
                            UINT4 *extMsg)
{
    /* Variable Declaration */
    UINT4 sonetRhapRegData;
    UINT4 msk;
    UINT4 iAddr;
    UINT4 cPsl;
    UINT4 aPsl;
    UINT4 sts3Off;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sts3 > 3) || (sts1 > 2) || (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddr = 0x6;

    sts3Off = 4 * sts12 + sts3;

    if (extType == 0) {
        /* Read from SONET:RHAP 0x4940200 + 0x01000*A(A=0:15) indirect
           Captured and Accepted PSL register 0x6 */
        result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off,
                                          sts1, &sonetRhapRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_CPSLV_MSK;
        result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &cPsl);
        if (result) return result;

        *extMsg = cPsl;

    } else if (extType == 1) {
         /* Read from SONET:RHAP 0x4940200 + 0x01000*A(A=0:15) indirect
           Captured and Accepted PSL register 0x6 */
        result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off,
                                          sts1, &sonetRhapRegData);
        if (result) return result;

        msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_APSLV_MSK;
        result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &aPsl);
        if (result) return result;

        *extMsg = aPsl;
   }

   return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetPohExtPsl */

/*******************************************************************************
**
**  hyPhy20gSonetTohRxExtSsm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received synchronization status
**                  message in the S1 byte of a specified link within the
**                  SONET subsystem.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  byteSsm      - Length of SSM to be extracted
**                                 0 - Nibble - bits 5-8 of S1 byte
**                                 1 - Byte - bits 1-8 of S1 byte
**                  filterEn     - 0 - SSM filtering is disabled
**                               - 1 - SSM filtering is enabled
**                  *extMsg      - pointer to 4 or 8 bit extracted SSM
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxExtSsm(struct file *fileHndl, UINT4 slice,
                              UINT4 byteSsm, UINT4 filterEn,
                              UINT4 *extMsg)
{
    /* Variable declaration */
    UINT4 sonetRrmpRegData;
    UINT4 msk;
    UINT4 regAddr;
    UINT4 ssm;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (byteSsm > 1) || (filterEn > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Update byteSsm with user supplied value
       Read from SONET:RRMP 0x4940098 + 0x01000*A(A=0:15) Received SSM */
	regAddr = HYPHY20G_SONET_RRMP_REG_RXD_SSM;
	regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

	/* direct register read from RRMP status register */
	result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRrmpRegData);
	if (result) return result;

    /* Update BYTESSM with the user supplied byteSsm */
	msk =  HYPHY20G_SONET_RRMP_BIT_RXD_SSM_BYTESSM_MSK;
	result = hyPhy20gVarFieldWrite(&sonetRrmpRegData, msk, byteSsm);
	if (result) return result;

	result = sysHyPhy20gWrite(fileHndl, regAddr, sonetRrmpRegData);
	if (result) return result;

    /* Update FLTRSSM with the user supplied filterEn */
    msk = HYPHY20G_SONET_RRMP_BIT_RXD_SSM_FLTRSSM_MSK;
    result = hyPhy20gVarFieldWrite(&sonetRrmpRegData, msk, filterEn);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, sonetRrmpRegData);
    if (result) return result;

    /* Read from SSMV[7:0] for Synchronization Status Message in S1 Byte */
    result = sysHyPhy20gRead(fileHndl, regAddr, &sonetRrmpRegData);
	if (result) return result;

    msk = HYPHY20G_SONET_RRMP_BIT_RXD_SSM_SSMV_MSK;
    result = hyPhy20gVarFieldRead(sonetRrmpRegData, msk, &ssm);
    if (result) return result;

    if (byteSsm == 1)
        *extMsg = ssm;
    else if (byteSsm == 0) {
        /* Read only bits 5-8 */
        ssm &= 0xf;
        *extMsg = ssm;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxExtSsm */

/*******************************************************************************
**
**  hyPhy20gSonetPohRxExtCntrs
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the counters for detected Path BIP
**                  error counts (B3 byte) and Path REI counts (G1 byte)for a
**                  specified path within the SONET subsystem.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the slice level or the
**                  device level.  The accessMode field allows for the
**                  usage of slice level counter accumulation or not.  When
**                  accessMode is set to 1, counter accumulation is applied
**                  to all resources within the slice.
**
**                  When using slice level counter accumulation for a counting
**                  interval set accessMode to 1 for the first resource
**                  accessed and to 0 for all subsequent resources accessed.
**                  Otherwise, counter events will be truncated.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  accessMode   - Refer to DESCRIPTION
**                               - 1 - trigger slice level counter accumulation
**                               - 0 - do not trigger slice level counter
**                                     accumulation
**                  *extBipP     - pointer to 16 bit Path BIP error count
**                  *extReiP     - pointer to 16 bit Path REI error count
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxExtCntrs(struct file *fileHndl, UINT4 slice,
                                UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                                UINT4 accessMode, UINT4 *extBipP,
                                UINT4 *extReiP)
{
    /* variable declaration */
    UINT4 sonetRhapRegData;
    UINT4 bipP;
    UINT4 reiP;
    UINT4 iAddr;
    UINT4 regAddr;
    UINT4 msk;
    UINT4 sts3Off;
    INT4 result;

    /* argument checking */
    if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)
        || (accessMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4 * sts12Num + sts3Num;

    /* Register address for RHAP Counter Update */
    regAddr = HYPHY20G_SONET_RHAP_REG_CNT_UDA;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    /* Trigger access with a write to RHAP counter update if required */
    if (accessMode == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0);
        if (result) return result;
    }

    /* Read from SONET:RHAP 0x4940200 + 0x01000*A(A=0:15)
       indirect Path BIP Error Counter register 0xA */
    iAddr = 0xA;
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num,
                                      &sonetRhapRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_PATH_BIP_ERR_CNT_PBIPE_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &bipP);
    if (result) return result;

    /* Read from SONET:RHAP 0x4940200 + 0x01000*A(A=0:15)
       indirect Path REI Error Counter register 0xB */
    iAddr = 0xB;
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num,
                                      &sonetRhapRegData);
    if (result) return result;

    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_PATH_REI_ERR_CNT_PREIE_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &reiP);
    if (result) return result;

    *extBipP = bipP;
    *extReiP = reiP;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxExtCntrs */

/*******************************************************************************
**
**  hyPhy20gSonetTohRxExtCntrs
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the counters for detected Section BIP
**                  error counts (B1 byte), Line BIP error counts (B2 byte) and
**                  line REI counts (M1 byte)for a specified link within the
**                  SONET subsystem.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the
**                  usage of local counter accumulation or not.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  accessMode   - 1 - locally trigger counter accumulation
**                               - 0 - do not locally trigger counter
**                                     accumulation
**                  *extBipS     - pointer to 16 bit Section BIP error count
**                  *extBipL     - pointer to 26 bit Line BIP error count
**                  *extReiL     - pointer to 26 bit Line REI error count
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRxExtCntrs(struct file *fileHndl, UINT4 slice,
                                UINT4 accessMode, UINT4 *extBipS,
                                UINT4 *extBipL, UINT4 *extReiL)
{
    /* Variable declaration */
    UINT4 sonetRrmpRegData;
    UINT4 sBipRegAddr;
    UINT4 lowBipRegAddr;
    UINT4 uprBipRegAddr;
    UINT4 lowReiRegAddr;
    UINT4 uprReiRegAddr;
    UINT4 sBipMsk;
    UINT4 lowlBipMsk;
    UINT4 uprlBipMsk;
    UINT4 lowlReiMsk;
    UINT4 uprlReiMsk;
    UINT4 bipS;
    UINT4 bipLL;
    UINT4 bipLM;
    UINT4 reiLL;
    UINT4 reiLM;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (accessMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses for Section and Line BIP Error Counter
       and Line REI Error Counter */
    sBipRegAddr = HYPHY20G_SONET_RRMP_REG_SEC_BIP_ERR_CNT;
	sBipRegAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(sBipRegAddr, slice);

	lowBipRegAddr = HYPHY20G_SONET_RRMP_REG_LN_BIP_ERR_CNT_LSB;
    lowBipRegAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(lowBipRegAddr, slice);

	uprBipRegAddr = HYPHY20G_SONET_RRMP_REG_LN_BIP_ERR_CNT_MSB;
    uprBipRegAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(uprBipRegAddr, slice);

    lowReiRegAddr = HYPHY20G_SONET_RRMP_REG_LN_REI_ERR_CNT_LSB;
	lowReiRegAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(lowReiRegAddr, slice);

	uprReiRegAddr = HYPHY20G_SONET_RRMP_REG_LN_REI_ERR_CNT_MSB;
	uprReiRegAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(uprReiRegAddr, slice);

	/* Determine mask values associated with counter */
	sBipMsk = HYPHY20G_SONET_RRMP_BIT_SEC_BIP_ERR_CNT_SBIPE_MSK;
	lowlBipMsk = HYPHY20G_SONET_RRMP_BIT_LN_BIP_ERR_CNT_LSB_LBIPE_MSK;
	uprlBipMsk = HYPHY20G_SONET_RRMP_BIT_LN_BIP_ERR_CNT_MSB_LBIPE_MSK;
	lowlReiMsk = HYPHY20G_SONET_RRMP_BIT_LN_REI_ERR_CNT_LSB_LREIE_MSK;
	uprlReiMsk = HYPHY20G_SONET_RRMP_BIT_LN_REI_ERR_CNT_MSB_LREIE_MSK;

    /* Trigger access with a write to the holding register if required */
    if (accessMode == 1) {
        result = sysHyPhy20gWrite(fileHndl, sBipRegAddr, 0);
        if (result) return result;

        /* LBIPE and LREIE counters will be transferred to the holding 
         registers by a microprocessor write access to the LBIPE or LREIE
         holding registers addresses. Please see the detail description
         for LBIPE and LREIE in the HYPHY register doc.*/
        result = sysHyPhy20gWrite(fileHndl, lowBipRegAddr, 0);
        if (result) return result;

    }

    /* Check the status of the transfer of section BIP, line BIP 
       and line REI error counters to the holding register.
       Make sure the TIP bit is 0. */
    result = sysHyPhy20gPollBit(fileHndl, 
                       HYPHY20G_SONET_RRMP_REG_CFG,
                       HYPHY20G_SONET_RRMP_BIT_CFG_TIP_MSK,
                       0, HYPHY20G_DELAY_20USEC);
    if (result) return result;
        
	/* Read from SONET:RRMP 0x49400A0 + 0x01000*A(A=0:15)
	   Section BIP Error Counter register */
	result = sysHyPhy20gRead(fileHndl, sBipRegAddr, &sonetRrmpRegData);
	if (result) return result;

	result = hyPhy20gVarFieldRead(sonetRrmpRegData, sBipMsk, &bipS);
	if (result) return result;

	/* Read from SONET:RRMP 0x49400A4 + 0x01000*A(A=0:15)
	   Line BIP Error Counter LSB register */
    result = sysHyPhy20gRead(fileHndl, lowBipRegAddr, &sonetRrmpRegData);
    if (result) return result;

    result = hyPhy20gVarFieldRead(sonetRrmpRegData, lowlBipMsk, &bipLL);
    if (result) return result;

    /* Read from SONET:RRMP 0x49400A8 + 0x01000*A(A=0:15)
       Line BIP Error Counter MSB register */
    result = sysHyPhy20gRead(fileHndl, uprBipRegAddr, &sonetRrmpRegData);
    if (result) return result;

    result = hyPhy20gVarFieldRead(sonetRrmpRegData, uprlBipMsk, &bipLM);
    if (result) return result;

	/* Read from SONET:RRMP 0x49400AC + 0x01000*A(A=0:15)
	   Line REI Error Counter LSB register */
	result = sysHyPhy20gRead(fileHndl, lowReiRegAddr, &sonetRrmpRegData);
	if (result) return result;

	result = hyPhy20gVarFieldRead(sonetRrmpRegData, lowlReiMsk, &reiLL);
	if (result) return result;

	/* Read from SONET:RRMP 0x49400B0 + 0x01000*A(A=0:15)
	   Line REI Error Counter MSB register */
	result = sysHyPhy20gRead(fileHndl, uprReiRegAddr, &sonetRrmpRegData);
	if (result) return result;

	result = hyPhy20gVarFieldRead(sonetRrmpRegData, uprlReiMsk, &reiLM);
	if (result) return result;

	/* Write to user */
	*extBipS = bipS;

	*extBipL = (bipLM << 16) + bipLL;
	DBG_PRINT("%s, %s, %d, bipLM = 0x%x, bipLL = 0x%x\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)bipLM, (unsigned int)bipLL);

	*extReiL = (reiLM << 16) + reiLL;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRxExtCntrs */

/*******************************************************************************
**
**  hyPhy20gSonetR1iInsCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the R1i message insertion
**                  parameters for a specified slice within the SONET
**                  subsystem.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  mode         - 0 - 8-byte R1i message
**                               - 1 - 1-byte R1i message
**                  r1Pos        - Controls R1i byte insertion location
**                               - 0 - S(1,7,1) to S(1,9,M), M=1..16
**                               - 1 - S(2,7,1) to S(2,9,M), M=1..16
**                               - 1 - S(9,4,1) to S(9,6,M), M=1..16
**                  enable       - 1 - R1i message insertion enabled
**                               - 0 - R1i message insertion disabled
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSonetR1iInsCfg(struct file *fileHndl, UINT4 slice, UINT4 mode,
                            UINT4 r1Pos, UINT4 enable)
{
    return HYPHY20G_ERR_NO_SW_CONTENT;
} /* hyPhy20gSonetR1iInsCfg */

/*******************************************************************************
**
**  hyPhy20gSonetR1iTxMsgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the R1i message contents that will
**                  be inserted on a particular path within a slice of the SONET
**                  subsystem.
**
**                  Before calling this function, hyPhy20gSonetR1iInsCfg
**                  should be used to configure the message insertion
**                  parameters for the slice.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  txEsm        - In 1-byte mode, txEsm[7:0] contains the
**                                 message to be inserted.
**                               - In 8-byte mode, txEsm[6:0] will be inserted
**                                 in the first byte of the message.
**                               - 0 - R1i message insertion disabled
**                  txCid        - In 1-byte mode, txCid has no effect.
**                               - In 8-byte mode, txCid[20:0] will be inserted
**                                 in the 2nd, 3rd, and 4th bytes of the message.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSonetR1iTxMsgCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                              UINT4 txEsm, UINT4 txCid)
{
    return HYPHY20G_ERR_NO_SW_CONTENT;
} /* hyPhy20gSonetR1iTxMsgCfg */

/*******************************************************************************
**
**  hyPhy20gSonetR1iTxErrIns
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function allows single or continuous errors to be
**                  in the R1i message to be inserted on a particular path
**                  within a slice of the SONET subsystem.
**
**                  Before calling this function, hyPhy20gSonetR1iInsCfg
**                  should be used to configure the message insertion
**                  parameters for the slice, and hyPhy20gSonetR1iTxMsgCfg
**                  should be called to configure the message contents.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  esmSbe       - 1 - Single error inserted in ESM field.
**                               - 0 - No errors inserted in ESM field.
**                  cidSbe       - 1 - Single error inserted in CID field. (Only
**                                     applies to 8-byte mode).
**                               - 0 - No errors inserted in CID field.
**                  crcSbe       - 1 - Single error inserted in CRC field. (Only
**                                     applies to 8-byte mode).
**                               - 0 - No errors inserted in CRC field.
**                  frmErr       - 1 - Continuous error inserted in R1i framing
**                                     pattern. (Only applies to 8-byte mode).
**                               - 0 - No errors inserted in framing field.
**                  frmSbe       - 1 - Single error inserted in in R1i framing
**                                     pattern. (Only applies to 8-byte mode).
**                               - 0 - No errors inserted in framing field.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSonetR1iTxErrIns(struct file *fileHndl, UINT4 slice,
                              UINT4 sts12Num, UINT4 sts3Num,
                              UINT4 sts1Num, UINT4 esmSbe, UINT4 cidSbe,
                              UINT4 crcSbe, UINT4 frmErr, UINT4 frmSbe)
{
    return HYPHY20G_ERR_NO_SW_CONTENT;
} /* hyPhy20gSonetR1iTxErrIns */

/*******************************************************************************
**
**  hyPhy20gSonetR1iDetCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the R1i message detection
**                  parameters for a specified slice within the the SONET
**                  subsystem.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  mode         - 0 - 8-byte R1i message
**                               - 1 - 1-byte R1i message
**                  r1Pos        - Controls R1i byte insertion location
**                               - 0 - S(1,7,1) to S(1,9,M), M=1..16
**                               - 1 - S(2,7,1) to S(2,9,M), M=1..16
**                               - 1 - S(9,4,1) to S(9,6,M), M=1..16
**                  enable       - 1 - R1i message detection enabled
**                               - 0 - R1i message detection disabled
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSonetR1iDetCfg(struct file *fileHndl, UINT4 slice, UINT4 mode,
                            UINT4 r1Pos, UINT4 enable)
{
    return HYPHY20G_ERR_NO_SW_CONTENT;
} /* hyPhy20gSonetR1iDetCfg */

/*******************************************************************************
**
**  hyPhy20gSonetR1iRxExpCidCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the expected CID within the R1i
**                  message received on a particular path within a
**                  slice of the SONET subsystem.
**
**                  Before calling this function, hyPhy20gSonetR1iDetCfg
**                  should be used to configure the message detection
**                  parameters for the slice.  This functin is only valid when
**                  hyPhy20gSonetR1iDetCfg is called with 'mode' = 0.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  expCid       - expCid[20:0] will be compared against the
**                                 received CID to determine CID framing status.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSonetR1iRxExpCidCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12Num, UINT4 sts3Num,
                                 UINT4 sts1Num, UINT4 expCid)
{
    return HYPHY20G_ERR_NO_SW_CONTENT;
} /* hyPhy20gSonetR1iRxExpCidCfg */

/*******************************************************************************
**
**  hyPhy20gSonetR1iRxMsgStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function detects the status of incoming R1i messages
**                  received on a specified slice within the the SONET
**                  subsystem.
**
**                  Before calling this function, hyPhy20gSonetR1iDetCfg
**                  should be used to configure the message detection
**                  parameters for the slice, and if in 8-byte mode,
**                  hyPhy20gSonetR1iRxExpCidCfg should be called to configure
**                  the expected CID.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  *statusBits - Pointer to storage for status bits
**                                  Bit 2 - ESM_V
**                                  Bit 1 - CID_V (only valid in 8-byte mode)
**                                  Bit 0 - FRAMING (only valid in 8-byte mode)
**                  *accESM     - Pointer to accepted ESM
**                  *crcCnt     - Pointer to CRC error count (only valid in
**                                8-byte mode)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSonetR1iRxMsgStatus(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                                 UINT4 *statusBits, UINT4 *accESM,
                                 UINT4 *crcCnt)
{
    return HYPHY20G_ERR_NO_SW_CONTENT;
} /* hyPhy20gSonetR1iRxMsgStatus */

/*******************************************************************************
**
**  hyPhy20gSonetR1iRxMsgData
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function captures the 8-byte R1i message
**                  received on a specified slice within the the SONET
**                  subsystem.
**
**                  Before calling this function, hyPhy20gSonetR1iDetCfg
**                  should be used to configure the message detection
**                  parameters for the slice, and hyPhy20gSonetR1iRxExpCidCfg
**                  should be called to configure the expected CID.
**
**                  This functin is only valid when
**                  hyPhy20gSonetR1iDetCfg is called with 'mode' = 0.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 15
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  *extMsg2    - Pointer to storage for message[63:32]
**                  *extMsg1    - Pointer to storage for message[31:0]
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSonetR1iRxMsgData(struct file *fileHndl, UINT4 slice,
                               UINT4 sts12um, UINT4 sts3Num, UINT4 sts1Num,
                               UINT4 *extMsg2, UINT4 *extMsg1)
{
    return HYPHY20G_ERR_NO_SW_CONTENT;
} /* hyPhy20gSonetR1iRxMsgData */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  utilHyPhy20gSonetTohTtmByteLengthChk
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Confirms if current state of the byte length configuration
**                  is set correctly in the SONET section.
**
**                  This function should only be called within sonet_features
**                  functions.
**
**  INPUTS:         *fileHndl -  base address
**                  slice     -  0 to 15
**                  ttmLength - Trail trace message type
**                                0 - 1 byte in Tx
**                                1 - 1 byte in Rx
**                                2 - 16 byte in Tx
**                                3 - 16 byte in Rx
**                                4 - 64 byte in Tx
**                                5 - 64 byte in Rx
**  NOTE::
**                  The RETURN value HYPHY20G_ERR_INVALID_PRECONFIG is declared
**                  when the trace message configuration length is not in a
**                  valid state to validate these special accesses.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSonetTohTtmByteLengthChk(struct file *fileHndl, UINT4 slice,
                                          UINT4 ttmLength)
{
    /* variable declaration */
    UINT4 iAddrTttpCfg;
    UINT4 iAddrRttpCfg;
    UINT4 sonetRttpRegData;
    UINT4 sonetTttpRegData;
    UINT4 msk;
    UINT4 offset;
    INT4 result = 0;

    /* argument checking */
    if ((slice > 15) || (ttmLength > 5)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Retrieve RTTP Trace configuration memory for RMW */
    iAddrRttpCfg = 0x3;
    result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iAddrRttpCfg,
                                          &sonetRttpRegData);
    if (result) return result;

    /* Retrieve TTTP Trace configuration memory for RMW */
    iAddrTttpCfg = 0x0;
    result = sonetHyPhy20gTTTPSectIndRead(fileHndl, slice, iAddrTttpCfg,
                                          &sonetTttpRegData);
    if (result) return result;

    /* Check trace message length */
    switch(ttmLength)
    {
            /* Check for 1 byte cfg in Tx */
        case 0:

            /* Check TTTP BYTEEN is 1 */
            msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            if ((sonetTttpRegData & msk) == 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

            /* Check for 1 byte cfg in Rx */
        case 1:

            /* Check RTTP ALGO[1:0] set to 3 */
            msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_MSK;
            offset = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_ALGO_OFF;
            if (((sonetRttpRegData & msk) >> offset) != 3) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

            /* Check for 16 byte cfg in Tx */
        case 2:

            /* Check TTTP BYTEEN is 0 */
            msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            if ((sonetTttpRegData & msk) != 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            /* Check TTTP LENGTH16 set to 1 */
            msk =
  HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            if ((sonetTttpRegData & msk) == 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

            /* Check for 16 byte cfg in Rx */
        case 3:

             /* Check RTTP LENGTH16 set to 1 */
             msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_LENGTH16_MSK;
             if ((sonetRttpRegData & msk) == 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;
        case 4:
            /* Check TTTP BYTEEN is 0 */
            msk =
    HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            if ((sonetTttpRegData & msk) != 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            /* Check TTTP LENGTH16 set to 0 */
            msk =
  HYPHY20G_SONET_TTTP_SECT_TTTP_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            if ((sonetTttpRegData & msk) == 1) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;
        case 5:
             /* Check RTTP LENGTH16 set to 0 */
             msk = HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_CFG_LENGTH16_MSK;
             if ((sonetRttpRegData & msk) == 1) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gSonetTohTtmByteLengthChk */

/*******************************************************************************
**
**  utilHyPhy20gSonetPohTtmByteLengthChk
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Confirms if current state of the byte length configuration
**                  is set correctly in the SONET high order path.
**
**                  This function should only be called within sonet_features
**                  functions.
**
**  INPUTS:         *fileHndl -  base address
**                  slice     -  0 to 15
**                  sts3Num   -  0 to 15
**                  sts1Num   -  0 to 2
**                  ttmLength -  Trail trace message type
**                                0 - 16 byte in Tx
**                                1 - 16 byte in Rx
**                                2 - 64 byte in Tx
**                                3 - 64 byte in Rx
**
**  NOTE::
**                  The RETURN value HYPHY20G_ERR_INVALID_PRECONFIG is declared
**                  when the trace message configuration length is not in a
**                  valid state to validate these special accesses.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSonetPohTtmByteLengthChk(struct file *fileHndl, UINT4 slice,
                                          UINT4 sts3Num, UINT4 sts1Num,
                                          UINT4 ttmLength)
{
    /* variable declaration */
    UINT4 iAddrTttpCfg;
    UINT4 iAddrRttpCfg;
    UINT4 sonetRttpRegData;
    UINT4 sonetTttpRegData;
    UINT4 msk;
	INT4 result = 0;

    /* argument checking */
	if ((slice > 15) || (sts3Num > 15) || (sts1Num > 2) || (ttmLength > 3)) {
	    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
	    return HYPHY20G_ERR_INVALID_ARG;
	}

    /* Retrieve RTTP_PATH Trace configuration memory for RMW */
    iAddrRttpCfg = 0x3;
    result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrRttpCfg,
                                          sts3Num, sts1Num, &sonetRttpRegData);
    if (result) return result;

    /* Retrieve TTTP_PATH 0x4940640 + 0x01000*A(A=0:15)  */
    iAddrTttpCfg = 0x0; /* Indirect Trace configuration memory */
    result = sonetHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpCfg,
                                          sts3Num, sts1Num, &sonetTttpRegData);
    if (result) return result;

    /* Check trace message length */
    switch(ttmLength)
    {
        /* Check for 16 byte cfg in Tx */
        case 0:

            /* Check TTTP_PATH BYTEEN is 0 */
            msk =
  HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            if ((sonetTttpRegData & msk) != 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            /* Check TTTP_PATH LENGTH16 is 1 */
            msk =
HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            if ((sonetTttpRegData & msk) == 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

        /* Check for 16 byte cfg in Rx */
        case 1:

            /* Check RTTP_PATH LENGTH16 is set to 1 */
            msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            if ((sonetRttpRegData & msk) == 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

        /* Check for 64 byte cfg in Tx */
        case 2:

            /* Check TTTP_PATH BYTEEN is 0 */
            msk =
  HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            if ((sonetTttpRegData & msk) != 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            /* Check TTTP_PATH LENGTH16 set to 0 */
            msk =
HYPHY20G_SONET_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            if ((sonetTttpRegData & msk) != 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

        /* Check for 64 byte cfg in Rx */
        case 3:

            /* Check RTTP_PATH LENGTH16 is set to 0 */
            msk = HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            if ((sonetRttpRegData & msk) != 0) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            break;

        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gSonetPohTtmByteLengthChk */


/*******************************************************************************
**
**  utilHyPhy20gCrcCalc
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function calculates the E.164 CRC byte for a 15 byte
**                  message.
**
**                  This function should only be called within sonet_features
**                  functions.
**
**
**  INPUTS:         *fileHndl   -  base address
**                  msg[15]     - 15 byte expected TTM
**                                Legal range of each byte 0x00 to 0x7F
**                  *e164Crc    - Pointer to storage for the E.164 CRC
**                                associated with the 15 byte message.
**                                Legal range 0x80 to 0xFF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gCrcCalc(struct file *fileHndl, UINT4 msg[15], UINT4 *e164Crc)
{
    UINT4 syndromeTable[256];
    UINT4 i, j, syndrome;
    UINT4 crcPolynomial = 0x89; /* X^7 + X^3 + 1 */
    UINT4 crcAccum = 0;

    for (i = 0; i < 256; i++) {
        /* syndrome = ((i & 0x80) != 0)? (i ^ crcPolynomial) : i; */
        if ((i & 0x80) != 0) {
            /* XOR i with the CRC Polynomial */
            syndrome = i ^ crcPolynomial;
        }
        else {
            syndrome = i;
        }

        for (j = 0; j < 7; j++) {
            if (((syndrome <<= 1) & 0x80) != 0) {
                syndrome ^= crcPolynomial;
            }
        }

        syndromeTable[i] = syndrome;
    }

    *e164Crc = 0x80;
    crcAccum = syndromeTable[(crcAccum << 1) ^ *e164Crc];

    for (i = 0; i < 15; i++) {
        crcAccum = syndromeTable[(crcAccum << 1) ^ msg[i]];
    }

    *e164Crc ^= crcAccum;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gCrcCalc */

/*******************************************************************************
**
**  utilHyPhy20gSonetSberLineSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Line B2
**               Bit Error Rate Monitoring.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               sdip2[31:16]- Signal Degrade BERM Set Integration Period 2 used
**                             to define the frame count for the SD set test
**                             integration period 2.
**               sdip1[15:0] - Signal Degrade BERM Set Integration Period 1 used
**                             to define the frame count for the SD set test
**                             integration period 1.
**               sdst[11:0]  - Signal Degrade BERM Set Threshold bits define
**                             the error count threshold which is used by the SD
**                             BERM block to conduct status tests that search
**                             for the setting of an SD condition.
**               sdct[11:0]  - Signal Degrade BERM Clear Threshold bits define
**                             the error count threshold which is used by the SD
**                             BERM block to conduct status tests that search
**                             for the clearing of an SD condition.
**               bermEnbl    - Enabling of Line Bit Error Rate Monitoring
**                                  0 - Disable Line BERM
**                                  1 - Enable Line BERM
**
**  OUTPUTS:     None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSonetSberLineSdCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 sdip2, UINT4 sdip1, UINT4 sdst,
                                      UINT4 sdct, UINT4 bermEnbl)
{
    /* variable declaration */
    UINT4 sonetSberRegData;
    UINT4 iAddrSdStat;
    UINT4 iAddrIntStat;
    UINT4 iAddrSdSetThres;
    UINT4 iAddrSdClrThres;
    UINT4 iAddrSdMonEnbl;
    UINT4 iAddrSdIntPer2;
    UINT4 iAddrSdIntPer1;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 15) || (sdst > 0xFFF) || (sdct > 0xFFF) || (bermEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* SONET:SBER_SECT indirect registers 0x4940100 + 0x01000*A(A=0:15) */
    iAddrSdStat = 0x0; /* SD/SF BERM Status */
    iAddrIntStat = 0x2; /* SD/SF BERM Interrupt Status */
    iAddrSdSetThres = 0x4; /* SD BERM Set Threshold */
    iAddrSdClrThres = 0x5; /* SD BERM Clear Threshold */
    iAddrSdMonEnbl = 0x6; /* SD Path Monitoring Enable */
    iAddrSdIntPer2 = 0x7; /* SD BERM Set Integration Period 2 */
    iAddrSdIntPer1 = 0x8; /* SD BERM Set Integration Period 1 */

    if (bermEnbl == 0)
    {
        sdst = 0xFFF;
        /* Set SD threshold to max value */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdSetThres,
                                               sdst);
        if (result) return result;

        /* Clear status bits */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdStat, 0);
        if (result) return result;

        /* Disable SD Berm monitoring */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdMonEnbl,
                                               0);
        if (result) return result;

        /* Read to clear interrupt bits as last step of disabling BERM */
        result = sonetHyPhy20gSBERSectIndRead(fileHndl, slice, iAddrIntStat,
                                              &sonetSberRegData);
        if (result) return result;

    } else if (bermEnbl == 1) {
        
        /* Clear status bits */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdStat,
                                               0);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = sonetHyPhy20gSBERSectIndRead(fileHndl, slice, iAddrIntStat,
                                              &sonetSberRegData);
        if (result) return result;

        /* Write SD set threshold */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdSetThres,
                                               sdst);
        if (result) return result;

        /* Write SD clear threshold */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdClrThres,
                                               sdct);
        if (result) return result;

        /* Write SD high order SDIP value */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdIntPer2,
                                               sdip2);
        if (result) return result;

        /* Write SD low order SDIP value */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdIntPer1,
                                               sdip1);
        if (result) return result;

        /* Enable monitoring for SD */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSdMonEnbl,
                                               bermEnbl);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSonetSberLineSdCfg */

/*******************************************************************************
**
**  utilHyPhy20gSonetSberLineSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Line B2
**               Bit Error Rate Monitoring.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               sfip2[31:16]- Signal Failure BERM Set Integration Period 2 used
**                             to define the frame count for the SF set test
**                             integration period 2.
**               sfip1[15:0] - Signal Failure BERM Set Integration Period 1 used
**                             to define the frame count for the SF set test
**                             integration period 1.
**               sfst[11:0]  - Signal Failure BERM Set Threshold bits define
**                             the error count threshold which is used by the SF
**                             BERM block to conduct status tests that search
**                             for the setting of an SD condition.
**               sfct[11:0]  - Signal Failure BERM Clear Threshold bits define
**                             the error count threshold which is used by the SF
**                             BERM block to conduct status tests that search
**                             for the clearing of an SF condition.
**               bermEnbl    - Enabling of Line Bit Error Rate Monitoring
**                                  0 - Disable Line BERM
**                                  1 - Enable Line BERM
**
**  OUTPUTS:     None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSonetSberLineSfCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 sfip2, UINT4 sfip1, UINT4 sfst,
                                      UINT4 sfct, UINT4 bermEnbl)
{
    /* variable declaration */
    UINT4 sonetSberRegData;
    UINT4 iAddrSfStat;
    UINT4 iAddrIntStat;
    UINT4 iAddrSfSetThres;
    UINT4 iAddrSfClrThres;
    UINT4 iAddrSfMonEnbl;
    UINT4 iAddrSfIntPer2;
    UINT4 iAddrSfIntPer1;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sfst > 0xFFF) || (sfct > 0xFFF) || (bermEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* SONET:SBER_SECT indirect registers 0x4940100 + 0x01000*A(A=0:15) */
    iAddrSfStat = 0x0; /* SD/SF BERM Status */
    iAddrIntStat = 0x2; /* SD/SF BERM Interrupt Status */
    iAddrSfSetThres = 0x14; /* SF BERM Set Threshold */
    iAddrSfClrThres = 0x15; /* SF BERM Clear Threshold */
    iAddrSfMonEnbl = 0x16; /* SF Path Monitoring Enable */
    iAddrSfIntPer2 = 0x17; /* SF BERM Set Integration Period 2 */
    iAddrSfIntPer1 = 0x18; /* SF BERM Set Integration Period 1 */

    if (bermEnbl == 0)
    {
        sfst = 0xFFF;

        /* Set SF threshold to max value */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfSetThres,
                                               sfst);
        if (result) return result;

        /* Clear status bits */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfStat, 0);
        if (result) return result;

        /* Disable SD Berm monitoring */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfMonEnbl,
                                               0);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = sonetHyPhy20gSBERSectIndRead(fileHndl, slice, iAddrIntStat,
                                              &sonetSberRegData);
        if (result) return result;
     } else if (bermEnbl == 1) {

        /* Clear status bits */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfStat, 0);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = sonetHyPhy20gSBERSectIndRead(fileHndl, slice, iAddrIntStat,
                                              &sonetSberRegData);
        if (result) return result;

        /* Write SF set threshold */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfSetThres,
                                               sfst);
        if (result) return result;

        /* Write SF clear threshold */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfClrThres,
                                               sfct);
        if (result) return result;

        /* Write SF high order SFIP value */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfIntPer2,
                                               sfip2);
        if (result) return result;

        /* Write SF low order SFIP value */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfIntPer1,
                                               sfip1);
        if (result) return result;

        /* Enable monitoring for SF */
        result = sonetHyPhy20gSBERSectIndWrite(fileHndl, slice, iAddrSfMonEnbl,
                                               bermEnbl);
        if (result) return result;

    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSonetSberLineSfCfg */


/*******************************************************************************
**
**  utilHyPhy20gSonetSberPathSdCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               sts3        - 0 to 15
**               sts1        - 0 to 2
**               sdip2[31:16]- Signal Degrade BERM Set Integration Period 2 used
**                             to define the frame count for the SD set test
**                             integration period 2.
**               sdip1[15:0] - Signal Degrade BERM Set Integration Period 1 used
**                             to define the frame count for the SD set test
**                             integration period 1.
**               sdst[11:0]  - Signal Degrade BERM Set Threshold bits define
**                             the error count threshold which is used by the SD
**                             BERM block to conduct status tests that search
**                             for the setting of an SD condition.
**               sdct[11:0]  - Signal Degrade BERM Clear Threshold bits define
**                             the error count threshold which is used by the SD
**                             BERM block to conduct status tests that search
**                             for the clearing of an SD condition.
**               bermEnbl    - Enabling of Line Bit Error Rate Monitoring
**                                  0 - Disable Line BERM
**                                  1 - Enable Line BERM
**
**  OUTPUTS:     None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSonetSberPathSdCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 sts3, UINT4 sts1, UINT4 sdip2,
                                      UINT4 sdip1, UINT4 sdst, UINT4 sdct,
                                      UINT4 bermEnbl)
{
    UINT4 sonetSberRegData;
    UINT4 iAddrSdStat = 0x0;
    UINT4 iAddrIntStat = 0x2;
    UINT4 iAddrSdSetThres = 0x4;
    UINT4 iAddrSdClrThres = 0x5;
    UINT4 iAddrSdMonEnbl = 0x6;
    UINT4 iAddrSdIntPer2 = 0x7;
    UINT4 iAddrSdIntPer1 = 0x8;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sts3 > 15) || (sts1 > 2) || (sdst > 0xFFF) ||
        (sdct > 0xFFF) || (bermEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (bermEnbl == 0)
    {
        sdst = 0xFFF;
        /* Set SD threshold to max value */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdSetThres,
                                               sts3, sts1, sdst);
        if (result) return result;

        /* Clear status bits */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdStat,
                                           sts3, sts1, 0);
        if (result) return result;

        /* Disable monitoring for SD */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdMonEnbl,
                                               sts3, sts1, 0);
        if (result) return result;

        /* Read to clear interrupt bits as last step of disabling BERM */
        result = sonetHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat,
                                              sts3, sts1, &sonetSberRegData);
        if (result) return result;
     } else if (bermEnbl == 1) {

        /* Clear status bits */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdStat,
                                           sts3, sts1, 0);
        if (result) return result;

        /* Read to clear interrupt bits as last step of disabling BERM */
        result = sonetHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat,
                                              sts3, sts1, &sonetSberRegData);
        if (result) return result;

        /* Write SD set threshold */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdSetThres,
                                               sts3, sts1, sdst);
        if (result) return result;

        /* Write SD clear threshold */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdClrThres,
                                               sts3, sts1, sdct);
        if (result) return result;

        /* Write SD high order SDIP value */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdIntPer2,
                                               sts3, sts1, sdip2);
        if (result) return result;

        /* Write SD low order SDIP value */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdIntPer1,
                                               sts3, sts1, sdip1);
        if (result) return result;

        /* Enable monitoring for SD */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdMonEnbl,
                                               sts3, sts1, bermEnbl);
        if (result) return result;

    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSonetSberPathSdCfg */

/*******************************************************************************
**
**  utilHyPhy20gSonetSberPathSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 15
**               sts3        - 0 to 15
**               sts1        - 0 to 2
**               sfip2[31:16]- Signal Failure BERM Set Integration Period 2 used
**                             to define the frame count for the SF set test
**                             integration period 2.
**               sfip1[15:0] - Signal Failure BERM Set Integration Period 1 used
**                             to define the frame count for the SF set test
**                             integration period 1.
**               sfst[11:0]  - Signal Failure BERM Set Threshold bits define
**                             the error count threshold which is used by the SF
**                             BERM block to conduct status tests that search
**                             for the setting of an SF condition.
**               sfct[11:0]  - Signal Failure BERM Clear Threshold bits define
**                             the error count threshold which is used by the SF
**                             BERM block to conduct status tests that search
**                             for the clearing of an SF condition.
**               bermEnbl    - Enabling of Line Bit Error Rate Monitoring
**                                  0 - Disable Line BERM
**                                  1 - Enable Line BERM
**
**  OUTPUTS:     None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSonetSberPathSfCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 sts3, UINT4 sts1, UINT4 sfip2,
                                    UINT4 sfip1, UINT4 sfst, UINT4 sfct,
                                    UINT4 bermEnbl)
{
    UINT4 sonetSberRegData;
    UINT4 iAddrSfStat = 0x0;
    UINT4 iAddrIntStat = 0x2;
    UINT4 iAddrSfSetThres = 0x14;
    UINT4 iAddrSfClrThres = 0x15;
    UINT4 iAddrSfMonEnbl = 0x16;
    UINT4 iAddrSfIntPer2 = 0x17;
    UINT4 iAddrSfIntPer1 = 0x18;
    INT4 result;

    /* Argument checking */
    if ((slice > 15) || (sts3 > 15) || (sts1 > 2) || (sfst > 0xFFF) ||
        (sfct > 0xFFF) || (bermEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (bermEnbl == 0)
    {
        sfst = 0xFFF;

        /* Set SF threshold to max value */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfSetThres,
                                               sts3, sts1, sfst);
        if (result) return result;

        /* Clear status bits */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfStat,
                                               sts3, sts1, 0);
        if (result) return result;

        /* Disable monitoring for SF */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfMonEnbl,
                                               sts3, sts1, 0);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = sonetHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat,
                                              sts3, sts1, &sonetSberRegData);
        if (result) return result;

    } else if (bermEnbl == 1) {

        /* Clear status bits */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfStat,
                                               sts3, sts1, 0);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = sonetHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat, sts3,
                                              sts1, &sonetSberRegData);
        if (result) return result;

        /* Write SF set threshold */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfSetThres,
                                               sts3, sts1, sfst);
        if (result) return result;

        /* Write SF clear threshold */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfClrThres,
                                               sts3, sts1, sfct);
        if (result) return result;

        /* Write SF high order SFIP value */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfIntPer2,
                                               sts3, sts1, sfip2);
        if (result) return result;

        /* Write SF low order SFIP value */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfIntPer1,
                                               sts3, sts1, sfip1);
        if (result) return result;

        /* Enable monitoring for SF */
        result = sonetHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfMonEnbl,
                                               sts3, sts1, bermEnbl);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSonetSberPathSfCfg */

/* end of file */
