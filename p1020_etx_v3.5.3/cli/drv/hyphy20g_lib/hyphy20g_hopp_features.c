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
**  FILE        : hyphy20g_hopp_features.c
**
**  $Date: 2011-11-09 16:36:02 +0800 (Wed, 09 Nov 2011) $
**
**  $Revision: 16914 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/


/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hopp_features.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 utilHyPhy20gHoppPohTtmByteLengthChk(struct file *fileHndl, UINT4 slice,
                                         UINT4 sts3Num, UINT4 sts1Num,
                                         UINT4 ttmLength);
INT4 utilHyPhy20gHoppCrcCalc(struct file *fileHndl, UINT4 msg[15],
                             UINT4 *e164Crc);
INT4 utilHyPhy20gHoppSberPathSdCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts3, UINT4 sts1, UINT4 sdip2,
                                   UINT4 sdip1, UINT4 sdst, UINT4 sdct,
                                   UINT4 bermEnbl);
INT4 utilHyPhy20gHoppSberPathSfCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts3, UINT4 sts1, UINT4 sfip2,
                                   UINT4 sfip1, UINT4 sfst, UINT4 sfct,
                                   UINT4 bermEnbl);
/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gHoppPohTxCfgTtmLength
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the length of trail trace messages
**                  to be inserted in the J1 byte for a HOPP subsystem in the
**                  transmit direction.
**
**
**                  Associated functions:
**                      - hyPhy20gHoppPohRxCfgTtmLength
**
**                      - hyPhy20gHoppPohTxCfg16ByteTtm
**                      - hyPhy20gHoppPohRxCfg16ByteTtm
**                      - hyPhy20gHoppPohRxExt16ByteTtm
**
**                      - hyPhy20gHoppPohTxCfg64ByteTtm
**                      - hyPhy20gHoppPohRxCfg64ByteTtm
**                      - hyPhy20gHoppPohRxExt64ByteTtm
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 3
**                sts12Num     - 0 to 3
**                sts3Num      - 0 to 3
**                sts1Num      - 0 to 2
**                ttmLength    - Trail trace message length
**                                0 - 16 byte
**                                1 - 64 bytes
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
INT4 hyPhy20gHoppPohTxCfgTtmLength(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 ttmLength)
{
    /* variable declaration */
    UINT2 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 hoppTttpRegData;
    INT4 result;

    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
	    (ttmLength > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

    /* Retreive TTTP_PATH Indirect Address 0x4954640 + 0x01000*A(A=0:3) */
    iAddr = 0x0; /* indirect TTTP_PATH Trace Configuration */
    result = hoppHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddr, sts3Off,
                                          sts1Num, &hoppTttpRegData);
    if (result) return result;

     /* Configure trace message length */
    switch(ttmLength)
    {
        /* Configure message length to be 16 bytes */
        case 0:

            /* Set TTTP_PATH BYTEEN to 0 */
            msk =
   HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            result = hyPhy20gVarFieldWrite(&hoppTttpRegData, msk, 0);
            if (result) return result;

            /* Set TTTP_PATH LENGTH16 to 1 */
            msk =
 HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&hoppTttpRegData, msk, 1);
            if (result) return result;

            break;

        /* Configure message length to be 64 bytes */
        case 1:

            /* Set TTTP_PATH BYTEEN to 0 */
            msk =
   HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            result = hyPhy20gVarFieldWrite(&hoppTttpRegData, msk, 0);
            if (result) return result;

            /* Set TTTP_U LENGTH16 to 0 */
            msk =
 HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&hoppTttpRegData, msk, 0);
            if (result) return result;

            break;

        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* Write updated configuration to HOPP:TTTP indirect Trace Configuration
       memory (0x0) */
    result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddr, sts3Off,
                                           sts1Num, hoppTttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohTxCfgTtmLength */

/*******************************************************************************
**
**  hyPhy20gHoppPohTxCfg16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the 16 byte Trail Trace Message
**                  to be transmit on the J1 byte of a specified link within
**                  the Hopp subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gHoppPohTxCfgTtmLength must be called for the link
**                  to configure the link for 1 byte TTM.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 3
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
INT4 hyPhy20gHoppPohTxCfg16ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 msg[15], UINT4 enbl)
{
    /* variable declaration */
    UINT4 iAddrTttpTrace, iAddrTttpCfg;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 hoppTttpRegData;
    UINT4 i;
    UINT4 e164Crc;
    UINT4 zeroEnbl;
    INT4 result;

    /* argument checking */
	if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
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
    result = utilHyPhy20gHoppPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                 sts1Num, 0);
    DBG_PRINT("After length check\n");
    if (result) return result;

    iAddrTttpTrace = 0x40; /* TTTP_PATH indirect Trace */

    /* Configure E.164 CRC for the 15 byte Trace Message */
    result = utilHyPhy20gHoppCrcCalc(fileHndl, msg, &e164Crc);
    DBG_PRINT("After crc calc\n");
    if (result) return result;

    DBG_PRINT("After call to CrcCalc\n");
    result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                          sts3Off, sts1Num, e164Crc);
    if (result) return result;
    DBG_PRINT("After writing e164 CrcCalc\n");

    /* Configure indirect Trace Message bytes 1 to 15 */
    for (i=0; i<15; i++)
    {
        iAddrTttpTrace++;

        /* Read the next byte of TTM */
        result = hoppHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpTrace,
                                             sts3Off, sts1Num,
                                             &hoppTttpRegData);
        if (result) return result;

        msk = HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_TRACE_MSK;
        result = hyPhy20gVarFieldWrite(&hoppTttpRegData, msk, msg[i]);
        if (result) return result;

        /* Write the msg */
        result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                              sts3Off, sts1Num,
                                              hoppTttpRegData);
        if (result) return result;
    }

    /* Configure ZEROEN in HOPP:TTTP 0x4954640 + 0x01000*A(A=0:3) */
    iAddrTttpCfg = 0x0; /* Indirect register Trace Configuration */
    zeroEnbl = 1 - enbl;
    result = hoppHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpCfg,
                                         sts3Off, sts1Num,
                                         &hoppTttpRegData);
    if (result) return result;

    /* Set ZEROEN */
    msk =
   HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_ZEROEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppTttpRegData, msk, zeroEnbl);
    if (result) return result;

    /* Write updated configuration to HOPP:TTTP indirect Trace Configuration
       memory(0x0) */
    result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpCfg,
                                          sts3Off, sts1Num, hoppTttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohTxCfg16ByteTtm */

/*******************************************************************************
**
**  hyPhy20gHoppPohRxCfg16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Receive parameters for 16-byte
**                  trail trace messages to be extracted from the J1 byte within
**                  a HOPP subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gHoppPohRxCfgTtmLength must be called for the link
**                  to configure the link for 1 byte TTM.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 3
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
INT4 hyPhy20gHoppPohRxCfg16ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 msg[15],
                                   UINT4 enblMode)
{
    /* variable declaration */
    UINT2 iAddrRttpEtrace, iAddrRttpCfg;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 hoppRttpRegData;
    UINT4 i;
    UINT4 e164Crc;
    INT4 result;

    /* argument checking */
	if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
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
    result = utilHyPhy20gHoppPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                 sts1Num, 1);
    DBG_PRINT("After length check\n");
    if (result) return result;

    /* Configure 1st byte of expected message from far end and write the trace
       message in HOPP:RTTP 0x4954240 + 0x01000*A(A=0:3) */
    iAddrRttpEtrace = 0xc0; /* indirect Trace Configuration */

    /* Configure E.164 CRC for the 15 byte Trace Message */
    result = utilHyPhy20gHoppCrcCalc(fileHndl, msg, &e164Crc);
    DBG_PRINT("After crc calc\n");
    if (result) return result;

    DBG_PRINT("After call to CrcCalc\n");
    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpEtrace,
                                          sts3Off, sts1Num, e164Crc);
    if (result) return result;
    DBG_PRINT("After writing e164 CrcCalc\n");

    /* Configure indirect Trace Message bytes 1 to 15 */
    for (i=0; i<15; i++)
    {
        iAddrRttpEtrace++;

        /* Read the next byte of TTM */
        result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrRttpEtrace,
                                             sts3Off, sts1Num,
                                             &hoppRttpRegData);
        if (result) return result;

        msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_EXP_TRC_ETRACE_MSK;
        result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, msg[i]);
        if (result) return result;

        /* Write the msg */
        result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpEtrace,
                                              sts3Off, sts1Num,
                                              hoppRttpRegData);
        if (result) return result;
    }

    /* Configure the receive trail trace message processing algorithm */
    iAddrRttpCfg = 0x3; /* HOPP:RTTP indirect Trace Configuration memory */
    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrRttpCfg,
                                         sts3Off, sts1Num, &hoppRttpRegData);
    if (result) return result;

    /* Unconditionally set ALGO[1:0] to 00b */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 0);
    if (result) return result;

    /* Write to HOPP:RTTP Indirect Trace Cfg Mem 0x3 */
    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpCfg,
                                           sts3Off, sts1Num, hoppRttpRegData);
    if (result) return result;

    /* Set ALGO[1:0] as specified by the user */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, enblMode);
    if (result) return result;

    /* Set TTMMOEN bit to 1 for proper SECTION trace monitoring */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 1);
    if (result) return result;

    /* Write to HOPP:RTTP Indirect Trace Cfg Mem 0x3 */
    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrRttpCfg,
                                          sts3Off, sts1Num, hoppRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohRxCfg16ByteTtm */

/*******************************************************************************
**
**  hyPhy20gHoppPohRxCfg64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Receive parameters for 64-byte
**                  trail trace messages to be extracted from the J1 byte within
**                  a HOPP subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gHoppPohRxCfgTtmLength must be called for the link
**                  to configure the link for 1 byte TTM.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 3
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  msg[62]     - 62 byte expected TTM
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
INT4 hyPhy20gHoppPohRxCfg64ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 msg[62],
                                   UINT4 enblMode)
{
    /* variable declaration */
    UINT4 hoppRttpRegData;
    UINT4 iAddrCfg, iAddrEtrace;
    UINT4 i;
    UINT4 sts3Off;
    UINT4 CR = 0x0D;
    UINT4 LF = 0x0A;
    UINT4 msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
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
    DBG_PRINT("After msg[i] checking\n");

    /* Check if device is configured for 64 byte TTM */
    result = utilHyPhy20gHoppPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                 sts1Num, 3);
    if (result) return result;
    DBG_PRINT("After length checking\n");

    /* Write the trace message in HOPP:RTTP_PATH
       0x4954240 + 0x01000*A(A=0:3) */
    iAddrEtrace = 0xc0; /* indirect Expected Trace memory */

    for (i=0; i<62; i++)
    {
        /* Write the trace message in HOPP:RTTP_PATH indirect Expected Trace
           memory (0xc0) */
        result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrEtrace,
                                              sts3Off, sts1Num, msg[i]);
        if (result) return result;

        iAddrEtrace++;
    }
    DBG_PRINT("After writing the message to reg\n");

    /* Write Carriage Return, Line Feed into the last two bytes of the message*/
    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrEtrace,
                                          sts3Off, sts1Num, CR);
    if (result) return result;

    iAddrEtrace++;
    DBG_PRINT("After writing CR to reg\n");

    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrEtrace,
                                          sts3Off, sts1Num, LF);
    if (result) return result;
    DBG_PRINT("After writing LF to reg\n");

    /* Configure the receive trail trace message processing algorithm
       in HOPP:RTTP 0x4954240 + 0x01000*A(A=0:3) indirect address 0x3 */
    iAddrCfg = 0x3; /* indirect Trace Configuration memory */
    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                          sts3Off, sts1Num,
                                          &hoppRttpRegData);
    if (result) return result;

    /* Unconditionally set ALGO[1:0] to 00b */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 0);
    if (result) return result;
    DBG_PRINT("After setting algo=0\n");

    /* Write to HOPP:RTTP Indirect Trace Cfg Mem 0x3 */
    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                          sts3Off, sts1Num, hoppRttpRegData);
    if (result) return result;
    DBG_PRINT("After writing algo=0 to reg\n");

    /* Set ALGO[1:0] as specified by the user */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, enblMode);
    if (result) return result;
    DBG_PRINT("After setting algo= %ld\n", enblMode);

    /* Set TTMMONEN bit to 1 for proper PATH trace monitoring */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 1);
    if (result) return result;
    DBG_PRINT("After setting TTMMOEN=1\n");

    /* Write to HOPP:RTTP Indirect Trace Cfg Mem 0x3 */
    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                          sts3Off, sts1Num, hoppRttpRegData);
    if (result) return result;
    DBG_PRINT("After updating algo=user to reg\n");

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohRxCfg64ByteTtm */

/*******************************************************************************
**
**  hyPhy20gHoppPohRxCfgTtmLength
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the length of trail trace messages
**                  to be inserted in the J1 byte for a HOPP subsystem in the
**                  transmit direction.
**
**
**                  Associated functions:
**                      - hyPhy20gHoppPohTxCfgTtmLength
**
**                      - hyPhy20gHoppPohTxCfg16ByteTtm
**                      - hyPhy20gHoppPohRxCfg16ByteTtm
**                      - hyPhy20gHoppPohRxExt16ByteTtm
**
**                      - hyPhy20gHoppPohTxCfg64ByteTtm
**                      - hyPhy20gHoppPohRxCfg64ByteTtm
**                      - hyPhy20gHoppPohRxExt64ByteTtm
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 3
**                sts12Num     - 0 to 3
**                sts3Num      - 0 to 3
**                sts1Num      - 0 to 2
**                ttmLength    - Trail trace message length
**                                0 - 16 byte
**                                1 - 64 bytes
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
INT4 hyPhy20gHoppPohRxCfgTtmLength(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 ttmLength)
{
    /* variable declaration */
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 hoppRttpRegData;
    INT4 result;

    /* argument checking */
	if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
	    (ttmLength > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

    /* Retreive RTTP_PATH Indirect Address 0x4954240 + 0x01000*A(A=0:3) */
    iAddr = 0x3; /* indirect RTTP_PATH Trace Configuration */
    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddr, sts3Off,
                                         sts1Num, &hoppRttpRegData);
    if (result) return result;

    /* if configuring for 64 bytes */
    if (ttmLength == 1)
    {
        /* Change the SYNC_CRLF bit to 1 */
        result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddr, sts3Off,
                                             sts1Num, &hoppRttpRegData);
        if (result) return result;

        msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_SYNC_CRLF_MSK;
        result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 1);
        if (result) return result;

        result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddr, sts3Off,
                                              sts1Num, hoppRttpRegData);
        if (result) return result;
    }

    /* Configure trace message length */
    switch(ttmLength)
    {
        /* Configure message length to be 16 bytes */
        case 0:

            /* Set RTTP_PATH LENGTH16 to 1 */
            msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 1);
            if (result) return result;

            break;

        /* Configure message length to be 64 bytes */
        case 1:

            /* Set RTTP_PATH LENGTH16 to 0 */
            msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 0);
            if (result) return result;

            break;

        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* Write updated configuration to HOPP:RTTP_PATH indirect Trace
       Configuration memory (0x3) */
    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddr, sts3Off,
                                          sts1Num, hoppRttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohRxCfgTtmLength */

/*******************************************************************************
**
**  hyPhy20gHoppPohSts1SdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring for a STS-1/AU-3 path within the
**               Hopp subsystem
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 3
**               sts12Num    - 0 to 3
**               sts3Num     - 0 to 3
**               sts1Num     - 0 to 2
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
**               berModeSF   - BER Configuration Setting for Signal Failiure
**                             0 - Disabled
**                             1 - 1.E-03
**                             2 - 1.E-04
**                             3 - 1.E-05
**                             4 - 1.E-06
**                             5 - 1.E-07
**                             6 - 1.E-08
**                             7 - 1.E-09
**                             8 - 1.E-10
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohSts1SdSfCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 sts12Num, UINT4 sts3Num,
                                UINT4 sts1Num, UINT4 berModeSD, UINT4 berModeSF)
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
    if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
        (berModeSD > 8) || (berModeSF > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3Num;

    /* To set BERM settings for SD */

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
    result = utilHyPhy20gHoppSberPathSdCfg(fileHndl, slice, sts3Off,
                                           sts1Num, sdip2, sdip1, sdst,
                                           sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gSberPathSfCfg */
    result = utilHyPhy20gHoppSberPathSfCfg(fileHndl, slice, sts3Off,
                                           sts1Num, sfip2, sfip1, sfst,
                                           sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohSts1SdSfCfg */

/*******************************************************************************
**
**  hyPhy20gHoppPohSts3cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring for a STS-3c/AU-4 path within the
**               HOPP subsystem
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 3
**               sts12Num    - 0 to 3
**               sts3c       - 0 to 3
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
**               berModeSF   - BER Configuration Setting for Signal Failure
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
INT4 hyPhy20gHoppPohSts3cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12Num, UINT4 sts3c,
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
    if ((slice > 3) || (sts12Num > 3) || (sts3c > 3) || (berModeSD > 8) ||
        (berModeSF > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4*sts12Num + sts3c;

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

    /* make call to utilHyPhy20gHoppSberPathSdSfCfg */
    result = utilHyPhy20gHoppSberPathSdCfg(fileHndl, slice, sts3Off, 0,
                                           sdip2, sdip1, sdst, sdct,
                                           sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gHoppSberPathSfCfg */
    result = utilHyPhy20gHoppSberPathSfCfg(fileHndl, slice, sts3Off, 0,
                                           sfip2, sfip1, sfst, sfct,
                                           sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohSts3cSdSfCfg */

/*******************************************************************************
**
**  hyPhy20gHoppPohSts12cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring for a STS-12c/AU-4-4c path within the
**               HOPP subsystem
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 3
**               sts12c      - 0 to 3
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
**               berModeSF   - BER Configuration Setting for Signal Failure
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
INT4 hyPhy20gHoppPohSts12cSdSfCfg(struct file *fileHndl, UINT4 slice,
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
    if ((slice > 3) || (sts12c > 3) || (berModeSD > 8) || (berModeSF > 8)) {
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

    /* make call to utilHyPhy20gHoppSberPathSdSfCfg */

    result = utilHyPhy20gHoppSberPathSdCfg(fileHndl, slice, sts3, 0, sdip2,
                                           sdip1, sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gHoppSberPathSfCfg */

    result = utilHyPhy20gHoppSberPathSfCfg(fileHndl, slice, sts3, 0, sfip2,
                                           sfip1, sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohSts12cSdSfCfg */

/*******************************************************************************
**
**  hyPhy20gHoppPohSts48cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring for a STS-48c/AU-4-16c path within the
**               HOPP subsystem
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 3
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
**               berModeSF   - BER Configuration Setting for Signal Failure
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
INT4 hyPhy20gHoppPohSts48cSdSfCfg(struct file *fileHndl, UINT4 slice,
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
    if ((slice > 3) || (berModeSD > 8) || (berModeSF > 8)) {
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

    /* make call to utilHyPhy20gHoppSberPathSdSfCfg */
    result = utilHyPhy20gHoppSberPathSdCfg(fileHndl, slice, 0, 0, sdip2,
                                           sdip1, sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gHoppSberPathSdSfCfg */
    result = utilHyPhy20gHoppSberPathSfCfg(fileHndl, slice, 0, 0, sfip2,
                                           sfip1, sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohSts48cSdSfCfg */

/*******************************************************************************
**
**  hyPhy20gHoppPohSts192cSdSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring for a STS-192c/AU-4-48c path within the
**               HOPP subsystem
**
**  INPUTS:      *fileHndl   - base address
**               berModeSD   - BER Configuration Setting for Signal Degrade
**                             0 - Disabled
**                             1 - 1.E-05
**                             2 - 1.E-06
**                             3 - 1.E-07
**                             4 - 1.E-08
**                             5 - 1.E-09
**                             6 - 1.E-10
**                             7 - 1.E-11
**               berModeSF   - BER Configuration Setting for Signal Failure
**                             0 - Disabled
**                             1 - 1.E-05
**                             2 - 1.E-06
**                             3 - 1.E-07
**                             4 - 1.E-08
**                             5 - 1.E-09
**                             6 - 1.E-10
**                             7 - 1.E-11
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohSts192cSdSfCfg(struct file *fileHndl, UINT4 berModeSD,
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
    INT4 result;

    /* argument checking */
    if ((berModeSD > 7) || (berModeSF > 7)) {
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

    /* make call to utilHyPhy20gHoppSberPathSdCfg */
    result = utilHyPhy20gHoppSberPathSdCfg(fileHndl, 0, 0, 0, sdip2,
                                           sdip1, sdst, sdct, sdEnbl);
    if (result) return result;

    /* make call to utilHyPhy20gHoppSberPathSfCfg */
    result = utilHyPhy20gHoppSberPathSfCfg(fileHndl, 0, 0, 0, sfip2,
                                           sfip1, sfst, sfct, sfEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohSts192cSdSfCfg */


/*******************************************************************************
**
**  hyPhy20gHoppPohTxCfg64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the 64 byte Trail Trace Message
**                  to be transmit on the J1 byte of a specified link within
**                  the HOPP subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gHoppPohTxCfgTtmLength must be called for the link
**                  to configure the link for 64 byte TTM.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 3
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  msg[62]      - 62 byte TTM to be transmitted
**                  enbl         - Enable generation of trail trace messages
**                                0 - disabled (configured ttm not transmitted)
**                                1 - enabled (configured ttm transmitted)
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
INT4 hyPhy20gHoppPohTxCfg64ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 msg[62], UINT4  enbl)
{
     /* variable declaration */
    UINT4 hoppTttpRegData;
    UINT4 iAddrTttpTrace, iAddrTttpCfg;
    UINT4 i;
    UINT4 sts3Off;
    UINT4 zeroen;
    UINT4 CR = 0x0D;
    UINT4 LF = 0x0A;
    UINT4 msk;
    INT4 result;

    /* argument checking */
	if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)
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
    result = utilHyPhy20gHoppPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 2);
    if (result) return result;

    iAddrTttpTrace = 0x40; /* TTTP_PATH indirect Trace */

    /* Configure indirect Trace Message bytes 1 to 62 */
    for (i=0; i<62; i++)
    {
        /* Write the next byte of the message */
        result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                              sts3Off, sts1Num,
                                              msg[i]);
        if (result) return result;

        iAddrTttpTrace++;
    }

    /* Write Carriage Return, Line Feed into the last two bytes of the message*/
    result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                          sts3Off, sts1Num, CR);
    if (result) return result;

    iAddrTttpTrace++;

    result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpTrace,
                                           sts3Off, sts1Num, LF);
    if (result) return result;

    /* Configure ZEROEN in HOPP:TTTP */
    iAddrTttpCfg = 0x0; /* indirect Trace Configuration memory */
    zeroen = 1 - enbl;
    result = hoppHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpCfg,
                                         sts3Off, sts1Num, &hoppTttpRegData);
    if (result) return result;

    /* Set ZEROEN */
    msk =
   HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_ZEROEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppTttpRegData, msk, zeroen);
    if (result) return result;

    /* Write updated configuration to HOPP:TTTP indirect Trace Configuration
       memory (0x0) */
    result = hoppHyPhy20gTTTPPathIndWrite(fileHndl, slice, iAddrTttpCfg,
                                          sts3Off, sts1Num, hoppTttpRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohTxCfg64ByteTtm */

/*******************************************************************************
**
**  hyPhy20gHoppPohRxExt16ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  16 byte Trail Trace Message received in the J1 byte of a
**                  specified path within the HOPP subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gHoppPohRxCfgTtmLength and
**                  hyPhy20gHoppPohRxCfg16ByteTtm must be called for the link to
**                  configure the link for a 16 byte TTM and configure an
**                  expected TTM message.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 3
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  extType     - Type of message to be extracted
**                                0 - Captured
**                                1 - Accepted
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
INT4 hyPhy20gHoppPohRxExt16ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 extType,
                                   UINT4 *extE164Crc, UINT4 *extMsg)
{
     /* variable declaration */
    UINT4 hoppRttpCtraceData, hoppRttpAtraceData;
    UINT4 hoppRttpRegData, hoppRttpStatRegData;
    UINT4 iAddrCfg, iAddrStat;
    UINT4 numCheckTiu = 0;
    UINT4 iAddrCtrace, iAddrAtrace;
    UINT4 i;
    UINT4 msk;
    UINT4 offset;
    UINT4 sts3Off;
    UINT4 crcLocation = 0;
    UINT4 msg[16];
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num  > 2) ||
        (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddrCfg = 0x3;
    iAddrCtrace = 0x40;
    iAddrAtrace = 0x80;
    iAddrStat = 0x0;

    sts3Off = 4*sts12Num + sts3Num;
    DBG_PRINT("sts3Off = %ld\n", sts3Off);
    DBG_PRINT("extType = %ld\n", extType);

    /* Check if device is configured for 16 byte TTM */
    result = utilHyPhy20gHoppPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 1);
    if (result) return result;

    /* Set TTMMONEN to enable proper Path trace monitoring */
    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                         sts3Off, sts1Num,
                                         &hoppRttpRegData);
    if (result) return result;

    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 1);
    if (result) return result;

    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                          sts3Off, sts1Num,
                                          hoppRttpRegData);
    if (result) return result;

    if (extType == 0) {

        /* WAIT for valid captured message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;

        for (i=0; i<16; i++)
        {

            /* Read Captured trace */
            result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCtrace,
                                                 sts3Off, sts1Num,
                                                 &hoppRttpCtraceData);
            if (result) return result;

            /* Place inside message array */
            msg[i] = hoppRttpCtraceData;

            if (hoppRttpCtraceData >= 0x80)
            {
                crcLocation = i;
                *extE164Crc = hoppRttpCtraceData;
            }

            iAddrCtrace++;
        }
    } else if (extType == 1) {
        DBG_PRINT("Accepted selected\n");
        DBG_PRINT("ext1: sts3Off = %ld\n", sts3Off);
        /* check if device is configured for Algorithm 2
           in HOPP:RTTP indirect Trace Configuration memory (0x3) */
        result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                             sts3Off, sts1Num,
                                             &hoppRttpRegData);
        if (result) return result;

        msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
        offset = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_OFF;
        DBG_PRINT("ALGO = 0x%lx\n", ((hoppRttpRegData & msk) >> offset));
        if (((hoppRttpRegData & msk) >> offset) != 0x2)
            return HYPHY20G_ERR_INVALID_PRECONFIG;

        /* WAIT for valid captured message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;

        /* Wait for potential TIU assertion at indirect 0x0 */
        result = sysHyPhy20gMsDelay(100);
        if (result) return result;

        /* Check TIU, if TIU = 0, accepted msgs are valid
           else wait for TIU = 0 before reading */
        result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                             sts3Off, sts1Num,
                                             &hoppRttpStatRegData);
        if (result) return result;

        /* While TIU is asserted and number of read accesses < 10 */
        while((hoppRttpStatRegData & 1) && (numCheckTiu < 10))
        {
            result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                                 sts3Off, sts1Num,
                                                 &hoppRttpStatRegData);
            if (result) return result;

            numCheckTiu++;
        }

        if(numCheckTiu == 10)
            return HYPHY20G_ERR_TIU;

        for (i=0; i<16; i++)
        {
            /* Read Accepted Trace */
            result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrAtrace,
                                                 sts3Off, sts1Num,
                                                 &hoppRttpAtraceData);
            if (result) return result;

            msg[i] = hoppRttpAtraceData;

            if (hoppRttpAtraceData >= 0x80)
            {
                crcLocation = i;
                *extE164Crc = hoppRttpAtraceData;
            }
            iAddrAtrace++;
        }
    }

    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                         sts3Off, sts1Num, &hoppRttpRegData);
    if (result) return result;

    /* Set TTMMONEN to 0 to enable monitoring on all paths */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 0);

    if (result) return result;

    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg, sts3Off,
                                           sts1Num, hoppRttpRegData);
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
} /* hyPhy20gHoppPohRxExt16ByteTtm */

/*******************************************************************************
**
**  hyPhy20gHoppPohRxExt64ByteTtm
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  64 byte Trail Trace Message received in the J1 byte of a
**                  specified path within the HOPP subsystem.
**
**                  Prior to calling this function,
**                  hyPhy20gHoppPohRxCfgTtmLength and
**                  hyPhy20gHoppPohRxCfg64ByteTtm must be called for the link to
**                  configure the link for a 64 byte TTM and configure an
**                  expected TTM message.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 3
**                  sts12Num    - 0 to 3
**                  sts3Num     - 0 to 3
**                  sts1Num     - 0 to 2
**                  extType     - Type of message to be extracted
**                                0 - Captured
**                                1 - Accepted
**                  *extMsg[64] - pointer to 16 byte TTM that has been extracted
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
INT4 hyPhy20gHoppPohRxExt64ByteTtm(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12Num, UINT4 sts3Num,
                                   UINT4 sts1Num, UINT4 extType,
                                   UINT4 *extMsg)
{
     /* variable declaration */
    UINT4 hoppRttpCtraceData, hoppRttpAtraceData;
    UINT4 hoppRttpRegData;
    UINT4 hoppRttpStatRegData;
    UINT4 lastRttpCtraceData = 0;
    UINT4 lastRttpAtraceData = 0;
    UINT4 iAddrCfg, iAddrStat;
    UINT4 iAddrCtrace, iAddrAtrace;
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
    if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
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
    result = utilHyPhy20gHoppPohTtmByteLengthChk(fileHndl, slice, sts3Off,
                                                  sts1Num, 3);
    if (result) return result;

    if (extType == 0) {

        /* Set TTMMONEN to enable proper Path trace monitoring */
        result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                             sts3Off, sts1Num,
                                             &hoppRttpRegData);
        if (result) return result;
        //DBG_PRINT("hoppRTTPRegData1 = 0x%lx\n", hoppRttpRegData);

        msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
        result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 1);
        if (result) return result;
        //DBG_PRINT("hoppRTTPRegData2 = 0x%lx\n", hoppRttpRegData);

        result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                              sts3Off, sts1Num,
                                              hoppRttpRegData);
        if (result) return result;

        /* WAIT for valid captured message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;
        //result = sysHyPhy20gMsDelay(1000);
        //if (result) return result;
        for (i=0; i<64; i++)
        {
            /* Read Captured trace */
            result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCtrace,
                                                 sts3Off, sts1Num,
                                                 &hoppRttpCtraceData);
            if (result) return result;

            /* Place inside message array */
            msg[i] = hoppRttpCtraceData;

            /* Look for CRLF to determine the end of the message */
            if ((hoppRttpCtraceData == LF)&&(lastRttpCtraceData == CR))
                cRLFLocation = i;

            lastRttpCtraceData = hoppRttpCtraceData;

            iAddrCtrace++;
        }
    }
    else if (extType == 1) {

        /* check if device is configured for Algorithm 2
           in HOPP:RTTP indirect Trace Configuration memory (0x3) */
        result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg,
                                             sts3Off, sts1Num,
                                             &hoppRttpRegData);
        if (result) return result;

        /* Set TTMMONEN to enable proper Path trace monitoring */
        msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
        result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 1);
        if (result) return result;

        result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg,
                                              sts3Off, sts1Num,
                                              hoppRttpRegData);
        if (result) return result;

        msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_MSK;
        offset = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_ALGO_OFF;
        if (((hoppRttpRegData & msk) >> offset) != 0x2)
            return HYPHY20G_ERR_INVALID_PRECONFIG;

        /* WAIT for valid accepted message latency */
        result = sysHyPhy20gMsDelay(680);
        if (result) return result;

        /* Wait for potential TIU assertion at indirect 0x0 */
        result = sysHyPhy20gMsDelay(100);
        if (result) return result;

        /* Check TIU, if TIU = 0, accepted msgs are valid
           else wait for TIU = 0 before reading */
        result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                             sts3Off, sts1Num,
                                             &hoppRttpStatRegData);
        if (result) return result;

        /* While TIU is asserted and number of read accesses < 10 */
        while((hoppRttpStatRegData & 1) && (numCheckTiu < 10))
        {
            result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrStat,
                                                 sts3Off, sts1Num,
                                                 &hoppRttpStatRegData);
            if (result) return result;

            numCheckTiu++;
        }

        if(numCheckTiu == 10)
            return HYPHY20G_ERR_TIU;

        for (i=0; i<64; i++)
        {
            /* Read Accepted Trace */
            result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrAtrace,
                                                 sts3Off, sts1Num,
                                                 &hoppRttpAtraceData);
            if (result) return result;

            msg[i] = hoppRttpAtraceData;

            /* Look for CRLF to determine the end of the message */
            if ((hoppRttpAtraceData == LF)&&(lastRttpAtraceData == CR))
                cRLFLocation = i;

            lastRttpAtraceData = hoppRttpAtraceData;

            iAddrAtrace++;
        }
    }

    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrCfg, sts3Off,
                                         sts1Num, &hoppRttpRegData);
    if (result) return result;

    /* Set TTMMOEN to 0 to enable monitoring on all paths */
    msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_TTMMONEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRttpRegData, msk, 0);
    if (result) return result;

    result = hoppHyPhy20gRTTPPathIndWrite(fileHndl, slice, iAddrCfg, sts3Off,
                                          sts1Num, hoppRttpRegData);
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
} /* hyPhy20gHoppPohRxExt64ByteTtm */

/*******************************************************************************
**
**  hyPhy20gHoppPohPslPdiCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function configures the transmit VC-3/VC-4/VC-4-xC PSL
**               and the expected receive PSL and PDI in the C2 byte for any
**               path within the HOPP subsystem
**
**               By default the PSL is configured to transmit and
**               expects a receive Signal Label of 0x00 (Unequipped) and PDI
**               detection is disabled.
**
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 3
**               sts12       - 0 to 3
**               sts3        - 0 to 3
**               sts1        - 0 to 2
**               pslValue    - Path Signal Label to be transmitted and expected
**                             to be received:
**                             Legal range 0x00 to 0xFF
**                             SONET/SDH standards define the use of a subset
**                             of this range.  Refer to the relevant standards
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
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohPslPdiCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 sts12, UINT4 sts3, UINT4 sts1,
                              UINT4 pslValue, UINT4 pdiRangeEn, UINT4 pdiVal)
{
    UINT4 hoppRhapRegData, hoppRhapPdiRegData;
    UINT4 hoppThppRegData;
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 c2Enbl = 1;
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (sts12 > 3) || (sts3 > 3) || (sts1 > 2) ||
        (pslValue > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4 * sts12 + sts3;

    /* Insert the PSL value into the C2 byte in HOPP:THPP block
       Read THPP J1 and C2 register at 0x49545C0 + 0x01000*A(A=0:3) */
    iAddr = 0x3; /* Indirect J1 and C2 */
    result = hoppHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                     &hoppThppRegData);
    if (result) return result;

    /* write C2 byte */
    msk = HYPHY20G_HOPP_THPP_THPP48_IND_IBIT_THPP_J1_C2_C2V_MSK;
    result = hyPhy20gVarFieldWrite(&hoppThppRegData, msk, pslValue);
    if (result) return result;

    /* Write to HOPP:THPP J1 and C2 register indirectly at 0x3 */
    result = hoppHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                      hoppThppRegData);
    if (result) return result;

    /* Enable insertion of the C2 byte
       Read from HOPP:THPP Priority Cfg register indirectly at 0x1 */
    iAddr = 0x1;
    result = hoppHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                     &hoppThppRegData);
    if (result) return result;

    /* Write REGC2EN bit */
    msk =
   HYPHY20G_HOPP_THPP_THPP48_IND_IBIT_THPP_PRIORITY_CFG_REGC2EN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppThppRegData, msk, c2Enbl);
    if (result) return result;

    /* Write to Priority Cfg register indirectly at 0x1 */
    result = hoppHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                      hoppThppRegData);
    if (result) return result;


    /* Write the expected PSL in HOPP:RHAP block 0x4954200 + 0x01000*A(A=0:3)
       Read RHAP Expected PSL and PDI register indirectly at 0x4 */
    iAddr = 0x4;
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                     &hoppRhapRegData);
    if (result) return result;

    /* Write EPSL[7:0] with user supplied pslValue */
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_EPSL_PDI_EPSL_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRhapRegData, msk, pslValue);
    if (result) return result;

    /* Write RHAP Expected PSL and PDI register indirectly at 0x4 */
    result = hoppHyPhy20gRHAPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                      hoppRhapRegData);
    if (result) return result;

    /* Read RHAP Expected PSL and PDI register indirectly at 0x4 */
    iAddr = 0x4;
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                     &hoppRhapPdiRegData);
    if (result) return result;

    /* Modify PDI and PDIRANGE with the user supplied pdiRangeEn and pdiVal */
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_EPSL_PDI_PDIRANGE_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRhapPdiRegData, msk, pdiRangeEn);
    if (result) return result;
    DBG_PRINT("hoppRhapPdiRegData(pdiRangeEn) = 0x%lx\n", hoppRhapPdiRegData);

    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_EPSL_PDI_PDI_MSK;
    result = hyPhy20gVarFieldWrite(&hoppRhapPdiRegData, msk, pdiVal);
    if (result) return result;

    DBG_PRINT("hoppRhapPdiRegData after = 0x%lx\n", hoppRhapPdiRegData);

    /* Write PDI[4:0] and PDIRange in HOPP:RHAP
       Expected PSL and PDI indirect register 0x4 */
    result = hoppHyPhy20gRHAPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                      hoppRhapPdiRegData);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohPslPdiCfg */

/*******************************************************************************
**
**  hyPhy20gHoppPohExtPsl
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the received captured or accepted
**                  Path Signal Label received in the C2 byte of a
**                  specified path within the HOPP subsystem.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 3
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
INT4 hyPhy20gHoppPohExtPsl(struct file *fileHndl, UINT4 slice,
                           UINT4 sts12, UINT4 sts3, UINT4 sts1,
                           UINT4 extType, UINT4 *extMsg)
{
     /* Variable Declaration */
    UINT4 hoppRhapRegData;
    UINT4 msk;
    UINT4 iAddr;
    UINT4 cPsl, aPsl;
    UINT4 sts3Off;
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (sts3 > 3) || (sts1 > 2) || (extType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    iAddr = 0x6;

    sts3Off = 4 * sts12 + sts3;

    if (extType == 0) {
        /* Read from HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3) indirect
           Captured and Accepted PSL register 0x6 */
        result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off,
                                         sts1, &hoppRhapRegData);
        if (result) return result;

        msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_CPSLV_MSK;
        result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &cPsl);
        if (result) return result;

        *extMsg = cPsl;

    } else if (extType == 1) {
         /* Read from HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3) indirect
           Captured and Accepted PSL register 0x6 */
        result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off,
                                         sts1, &hoppRhapRegData);
        if (result) return result;

        msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_APSLV_MSK;
        result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &aPsl);
        if (result) return result;

        *extMsg = aPsl;
   }

   return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohExtPsl */

/*******************************************************************************
**
**  hyPhy20gHoppPohRxCntrs
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function updates the counters for detected Path BIP
**                  error counts (B3 byte) and Path REI counts (G1 byte)for a
**                  specified path within the HOPP subsystem.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 3
**                  sts12Num     - 0 to 3
**                  sts3Num      - 0 to 3
**                  sts1Num      - 0 to 2
**                  accessMode   - 1 - trigger slice level counter accumulation
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
INT4 hyPhy20gHoppPohRxCntrs(struct file *fileHndl, UINT4 slice,
                            UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                            UINT4 accessMode, UINT4 *extBipP, UINT4 *extReiP)
{
     /* variable declaration */
    UINT4 hoppRhapRegData;
    UINT4 regAddr;
    UINT4 bipP, reiP;
    UINT4 iAddr;
    UINT4 msk;
    UINT4 sts3Off;
    INT4 result;

    /* argument checking */
    if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

   sts3Off = 4 * sts12Num + sts3Num;

   /* Register address for RHAP Counter Update 0x495422C + 0x01000*A(A=0:3) */
    regAddr = HYPHY20G_HOPP_RHAP_REG_CNT_UDA;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    /* Trigger access with a write to RHAP counter update if required */
    if (accessMode == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0);
        if (result) return result;
    }

    /* Read from HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3)
       indirect Path BIP Error Counter register 0xA */
    iAddr = 0xA;
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num,
                                     &hoppRhapRegData);
    if (result) return result;

    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_PATH_BIP_ERR_CNT_PBIPE_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &bipP);
    if (result) return result;

    /* Read from HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3)
       indirect Path REI Error Counter register 0xB */
    iAddr = 0xB;
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num,
                                     &hoppRhapRegData);
    if (result) return result;

    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_PATH_REI_ERR_CNT_PREIE_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &reiP);
    if (result) return result;

    *extBipP = bipP;
    *extReiP = reiP;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohRxCntrs */

/*******************************************************************************
**
**  hyPhy20gHoppPohCfgRdiModeP
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures a slice within the HOPP subsystem
**                  to operate in RDI or ERDI mode of operation.
**
**                  This function disables RDI/ERDI generation on all paths.
**
**                  When ERDI mode is configured, SARC_H is configured to
**                  disable ERDI insertion in response to TIM and TIU events.
**
**                  This function disables transfer of RDI/ERDI and REI data
**                  from being reported from receive to transmit paths.
**
**                  The function hyPhy20gPohTxCfgRdiReiP can be called after
**                  this function to configure RDI/ERDI and REI generation
**                  per path in response to receive events.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 3
**                  rdiMode     - RDI vs ERDI configuration
**                                0 - ERDI
**                                1 - RDI
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohCfgRdiModeP(struct file *fileHndl, UINT4 slice,
                                 UINT4 rdiMode)
{
     /* Variable declaration */
    UINT4 iAddr, regAddr;
    UINT4 msk;
    UINT4 hoppSarcRegData, hoppThppRegData;
    UINT4 sts12Num, sts3Num, sts1Num;
    UINT4 sts3Off;
    UINT4 ptenMsk;
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (rdiMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    DBG_PRINT("after arg check\n");
    /* for each path set PTEN = 0 at HOPP:SARC at 0x4954400 + 0x01000*A(A=0:3)
       indirect register 0xE Receive Path ERDI Enable and set G1INSDIS = 1 */
    iAddr = 0xe;
    ptenMsk =
            HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_ERDI_REI_EN_DAT_PTEN_MSK;

    for (sts12Num = 0; sts12Num <= 3; sts12Num ++) {
	    for (sts3Num = 0; sts3Num <= 3; sts3Num++) {
            for (sts1Num = 0; sts1Num <= 2; sts1Num++) {
                sts3Off = 4 * sts12Num + sts3Num;

                result = hoppHyPhy20gSARCIndRead(fileHndl, slice, iAddr,
                                                 sts3Off, sts1Num,
                                                 &hoppSarcRegData);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&hoppSarcRegData, ptenMsk, 0);
                if (result) return result;

                result = hoppHyPhy20gSARCIndWrite(fileHndl, slice, iAddr,
                                                  sts3Off, sts1Num,
                                                  hoppSarcRegData);
                if (result) return result;

                /* Read HOPP:THPP at 0x49545C0 + 0x01000*A(A=0:3) indirect
                   register Priority Configuration 0x1 */
                iAddr = 0x1;
                result = hoppHyPhy20gTHPPIndRead(fileHndl, slice, iAddr,
                                                 sts3Off, sts1Num,
                                                 &hoppThppRegData);
                if (result) return result;

                /* set G1INSDIS to 1 for the path */
                msk =
   HYPHY20G_HOPP_THPP_THPP48_IND_IBIT_THPP_PRIORITY_CFG_G1INSDIS_MSK;
                result = hyPhy20gVarFieldWrite(&hoppThppRegData, msk, 1);
                if (result) return result;

                result = hoppHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr,
                                                  sts3Off, sts1Num,
                                                  hoppThppRegData);
                if (result) return result;
           }
        }
    }

    /* Configure RDIEN bit in HOPP:SARC 0x4954418 + 0x01000*A(A=0:3)
       Receive Path Configuration */
    regAddr = HYPHY20G_HOPP_SARC_REG_RX_PATH_CFG;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    msk = HYPHY20G_HOPP_SARC_BIT_RX_PATH_CFG_RDIEN_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, rdiMode);
    if (result) return result;

    /* Unconditionally set PERDI22 = 1 in HOPP:SARC
       0x4954418 + 0x01000*A(A=0:3) Receive Path Configuration */
    msk = HYPHY20G_HOPP_SARC_BIT_RX_PATH_CFG_PERDI22_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    if (result) return result;

    /* Unconditionally set PERDIPRIO = 1 in HOPP:SARC
       0x4954408 + 0x01000*A(A=0:3) Receive Configuration */
    regAddr = HYPHY20G_HOPP_SARC_REG_RX_CFG;
    regAddr = HYPHY20G_HOPP_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    msk = HYPHY20G_HOPP_SARC_BIT_RX_CFG_PERDIPRIO_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohCfgRdiModeP */

/*******************************************************************************
**
**  hyPhy20gHoppPohTxCfgRdiReiP
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables automatic Path
**                  RDI and REI generation into the transmit data stream on a
**                  per path basis within the HOPP subsystem
**
**                  Prior to calling this function the device must have been
**                  globally configured for RDI vs ERDI mode of operation with
**                  hyPhy20gHoppPohCfgRdiModeP.
**
**                  If configured for ERDI mode of operation, TIM-P and TIU-P
**                  events can be configured to contribute to the ERDI value
**                  generated in the transmit stream.
**
**                  REI may be configured to report a count of BIP-8 errors or
**                  a block indication if any BIP-8 errors occurred in the
**                  multiframe.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 to 3
**                  sts12       - 0 to 3
**                  sts3        - 0 to 3
**                  sts1        - 0 to 2
**                  erdiMode    - Contribution of TIM-P and TIU-P to ERDI
**                                generation
**                                0 - TIM-P and TIU-P do not contribute to ERDI
**                                1 - TIM-P and TIU-P do contribute to ERDI
**                  reiMode     - REI counting vs block mode configuration
**                                0 - REI provides a count of BIP-8 errors from
**                                    0 to 8
**                                1 - REI reports 0 if no BIP-8 errors or 1 if
**                                    one or more BIP-8 errors.
**                  rdiErdiEnbl  - Enables insertion of RDI/ERDI in the G1 byte
**                                 based on receive alarms from the SARC block
**                                 (TPERDIINS)
**                                0 - Disable insertion of RDI/ERDI based on  
**                                    receive path alarms 
**                                1 - Enable insertion of RDI/ERDI based on 
**                                    recieve path alarms
**                                    NOTE: G1 byte is generated as per the 
**                                    "Path overhead byte source priority" table
**                                    in the device hardward specification.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohTxCfgRdiReiP(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12, UINT4 sts3, UINT4 sts1,
                                 UINT4 erdiMode, UINT4 reiMode,
                                 UINT4 rdiErdiEnbl)
{
     /* Variable declaration */
    UINT4 iAddr;
    UINT4 msk;
    UINT4 g1InsDisVal = 0;
    UINT4 hoppSarcRegData, hoppThppRegData0, hoppThppRegData1;
    UINT4 sts3Off;
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (sts12 > 3) || (sts3 > 3) || (sts1 > 2) ||
        (erdiMode > 1) || (reiMode > 1) || (rdiErdiEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts3Off = 4 * sts12 + sts3;

    /* Configure PTEN in HOPP:SARC indirect 0xE Receive Path ERDI Enable */
    iAddr = 0xe;
    result = hoppHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                     &hoppSarcRegData);
    if (result) return result;

    /* Configure PTEN for erdiMode */
    msk = HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_ERDI_REI_EN_DAT_PTEN_MSK;
    result = hyPhy20gVarFieldWrite(&hoppSarcRegData, msk, erdiMode);
    if (result) return result;

    result = hoppHyPhy20gSARCIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                      hoppSarcRegData);
    if (result) return result;

    /* Configure PREIEBLK in HOPP:THPP indirect register 0x0
       Control for counting or blocking mode for BIP-8 reporting */
    iAddr = 0x0;
    result = hoppHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                     &hoppThppRegData0);
    if (result) return result;

    msk = HYPHY20G_HOPP_THPP_THPP48_IND_IBIT_THPP_CTL_PREIEBLK_MSK;
    result = hyPhy20gVarFieldWrite(&hoppThppRegData0, msk, reiMode);
    if (result) return result;

    result = hoppHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                      hoppThppRegData0);
    if (result) return result;

    /* Configure G1INSDIS in HOPP:THPP indirect register 0x1
       Priority Configuration to enable or disable RDI/ERDI and REI insertion */
    iAddr = 0x1;
    result = hoppHyPhy20gTHPPIndRead(fileHndl, slice, iAddr, sts3Off, sts1,
                                     &hoppThppRegData1);
    if (result) return result;

    if (rdiErdiEnbl == 1){
        g1InsDisVal = 0;
    } else if (rdiErdiEnbl == 0) {
        g1InsDisVal = 1;
    }

    msk =
   HYPHY20G_HOPP_THPP_THPP48_IND_IBIT_THPP_PRIORITY_CFG_G1INSDIS_MSK;
    result = hyPhy20gVarFieldWrite(&hoppThppRegData1, msk, g1InsDisVal);
    if (result) return result;

    result = hoppHyPhy20gTHPPIndWrite(fileHndl, slice, iAddr, sts3Off, sts1,
                                      hoppThppRegData1);

    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohTxCfgRdiReiP */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  utilHyPhy20gHoppPohTtmByteLengthChk
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Confirms if current state of the byte length configuration
**                  is set correctly in the HOPP high order path.
**
**                  This function should only be called within hopp_features
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
INT4 utilHyPhy20gHoppPohTtmByteLengthChk(struct file *fileHndl, UINT4 slice,
                                         UINT4 sts3Num, UINT4 sts1Num,
                                         UINT4 ttmLength)
{
    /* variable declaration */
    UINT2 iAddrTttpCfg, iAddrRttpCfg;
    UINT4 hoppRttpRegData, hoppTttpRegData;
    UINT4 msk;
	INT4 result = 0;

    /* argument checking */
	if ((slice > 15) || (sts3Num > 15) || (sts1Num > 2) || (ttmLength > 3)) {
	    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
	    return HYPHY20G_ERR_INVALID_ARG;
	}

    /* Retrieve RTTP_PATH Trace configuration memory for RMW */
    iAddrRttpCfg = 0x3;
    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddrRttpCfg,
                                          sts3Num, sts1Num, &hoppRttpRegData);
    if (result) return result;

    /* Retrieve HOPP:TTTP_PATH 0x4954640 + 0x01000*A(A=0:3) */
    iAddrTttpCfg = 0x0; /* Trace configuration */
    result = hoppHyPhy20gTTTPPathIndRead(fileHndl, slice, iAddrTttpCfg,
                                         sts3Num, sts1Num, &hoppTttpRegData);
    if (result) return result;

    /* Check trace message length */
    switch(ttmLength)
    {
        /* Check for 16 byte cfg in Tx */
        case 0:

            /* Check TTTP_PATH BYTEEN is 0 */
            msk =
   HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            if ((hoppTttpRegData & msk) != 0) {
                DBG_PRINT("Byteen tttp is not set to 0\n");
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            /* Check TTTP_PATH LENGTH16 is 1 */
            msk =
 HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            if ((hoppTttpRegData & msk) == 0) {
                DBG_PRINT("Length16 tttp is not set to 1\n");
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

        /* Check for 16 byte cfg in Rx */
        case 1:

            /* Check RTTP_PATH LENGTH16 is set to 1 */
            msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            if ((hoppRttpRegData & msk) == 0) {
                DBG_PRINT("Length16 rttp is not set to 1\n");
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

        /* Check for 64 byte cfg in Tx */
        case 2:

            /* Check TTTP_PATH BYTEEN is 0 */
            msk =
   HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_BYTEEN_MSK;
            if ((hoppTttpRegData & msk) != 0) {
                DBG_PRINT("Byteen tttp is not set to 0\n");
                 return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            /* Check TTTP_PATH LENGTH16 set to 0 */
            msk =
 HYPHY20G_HOPP_TTTP_PATH_TTTP48_IND_IBIT_TTTP_TRC_CFG_LENGTH16_MSK;
            if ((hoppTttpRegData & msk) != 0) {
                DBG_PRINT("Length16 tttp is not set to 0\n");
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            break;

        /* Check for 64 byte cfg in Rx */
        case 3:

            /* Check RTTP_PATH LENGTH16 is set to 0 */
            msk = HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_CFG_LENGTH16_MSK;
            if ((hoppRttpRegData & msk) != 0) {
                DBG_PRINT("Length16 rttp is not set to 0\n");
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            break;

        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gHoppPohTtmByteLengthChk */

/*******************************************************************************
**
**  utilHyPhy20gCrcCalc
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function calculates the E.164 CRC byte for a 15 byte
**                  message.
**
**                  This function should only be called within hopp_features
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
INT4 utilHyPhy20gHoppCrcCalc(struct file *fileHndl, UINT4 msg[15],
                             UINT4 *e164Crc)
{
    UINT4 syndromeTable[256];
    UINT4 i, j, syndrome;
    UINT4 crcPolynomial = 0x89; /* X^7 + X^3 + 1 */
    UINT4 crcAccum = 0;

    for (i = 0; i < 256; i++) {
        // syndrome = ((i & 0x80) != 0)? (i ^ crcPolynomial) : i;
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
**  utilHyPhy20gHoppSberPathSdCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 3
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
INT4 utilHyPhy20gHoppSberPathSdCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts3, UINT4 sts1, UINT4 sdip2,
                                   UINT4 sdip1, UINT4 sdst, UINT4 sdct,
                                   UINT4 bermEnbl)
{
    UINT4 hoppSberRegData;
    UINT4 iAddrSdStat = 0x0;
    UINT4 iAddrIntStat = 0x2;
    UINT4 iAddrSdSetThres = 0x4;
    UINT4 iAddrSdClrThres = 0x5;
    UINT4 iAddrSdMonEnbl = 0x6;
    UINT4 iAddrSdIntPer2 = 0x7;
    UINT4 iAddrSdIntPer1 = 0x8;
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (sts3 > 15) || (sts1 > 2) || (sdst > 0xFFF) ||
        (sdct > 0xFFF) || (bermEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (bermEnbl == 0)
    {
        sdst = 0xFFF;
        /* Set SD threshold to max value */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdSetThres,
                                              sts3, sts1, sdst);
        if (result) return result;

         /* Clear status bits */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdStat,
                                              sts3, sts1, 0x0);
        if (result) return result;

        /* Disable monitoring for SD */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdMonEnbl,
                                              sts3, sts1, bermEnbl);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = hoppHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat,
                                             sts3, sts1, &hoppSberRegData);
        if (result) return result;
    } else {

        /* Clear status bits */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdStat,
                                              sts3, sts1, 0x0);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = hoppHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat,
                                             sts3, sts1, &hoppSberRegData);
        if (result) return result;

        /* Write SD set threshold */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdSetThres,
                                              sts3, sts1, sdst);
        if (result) return result;

        /* Write SD clear threshold */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdClrThres,
                                              sts3, sts1, sdct);
        if (result) return result;

        /* Write SD high order SDIP value */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdIntPer2,
                                              sts3, sts1, sdip2);
        if (result) return result;

        /* Write SD low order SDIP value */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdIntPer1,
                                              sts3, sts1, sdip1);
        if (result) return result;

        /* Enable monitoring for SD */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdMonEnbl,
                                              sts3, sts1, bermEnbl);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gHoppSberPathSdSfCfg */

/*******************************************************************************
**
**  utilHyPhy20gHoppSberPathSfCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION: This function has the capability of configuring the Path B3
**               Bit Error Rate Monitoring.
**
**  INPUTS:      *fileHndl   - base address
**               slice       - 0 to 3
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
INT4 utilHyPhy20gHoppSberPathSfCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts3, UINT4 sts1, UINT4 sfip2,
                                   UINT4 sfip1, UINT4 sfst, UINT4 sfct,
                                   UINT4 bermEnbl)
{
    UINT4 hoppSberRegData;
    UINT4 iAddrSdStat = 0x0;
    UINT4 iAddrIntStat = 0x2;
    UINT4 iAddrSfSetThres = 0x14;
    UINT4 iAddrSfClrThres = 0x15;
    UINT4 iAddrSfMonEnbl = 0x16;
    UINT4 iAddrSfIntPer2 = 0x17;
    UINT4 iAddrSfIntPer1 = 0x18;
    INT4 result;

    /* Argument checking */
    if ((slice > 3) || (sts3 > 15) || (sts1 > 2) || (sfst > 0xFFF) ||
        (sfct > 0xFFF) || (bermEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (bermEnbl == 0)
    {
        sfst = 0xFFF;

        /* Set SF threshold to max value */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfSetThres,
                                              sts3, sts1, sfst);
        if (result) return result;

        /* Clear status bits */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdStat,
                                              sts3, sts1, 0x0);
        if (result) return result;

        /* Disable monitoring for SF */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfMonEnbl,
                                              sts3, sts1, bermEnbl);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = hoppHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat,
                                             sts3, sts1, &hoppSberRegData);
        if (result) return result;


    } else {

        /* Clear status bits */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSdStat,
                                              sts3, sts1, 0x0);
        if (result) return result;

        /* Read to clear interrupt bits */
        result = hoppHyPhy20gSBERPathIndRead(fileHndl, slice, iAddrIntStat,
                                             sts3, sts1, &hoppSberRegData);
        if (result) return result;

        /* Write SF set threshold */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfSetThres,
                                              sts3, sts1, sfst);
        if (result) return result;

        /* Write SF clear threshold */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfClrThres,
                                              sts3, sts1, sfct);
        if (result) return result;

        /* Write SF high order SFIP value */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfIntPer2,
                                              sts3, sts1, sfip2);
        if (result) return result;

        /* Write SF low order SFIP value */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfIntPer1,
                                              sts3, sts1, sfip1);
        if (result) return result;

        /* Enable monitoring for SF */
        result = hoppHyPhy20gSBERPathIndWrite(fileHndl, slice, iAddrSfMonEnbl,
                                              sts3, sts1, bermEnbl);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gHoppSberPathSfCfg */

/* end of file */
