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
**  FILE        : hyphy20g_gfp_status.c
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
#include "hyphy20g_gfp_status.h"

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
**  hyPhy20gGfpTxPmonGfpFHdlcCntrs
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function retrieves the PMON counters for events within
**                  the TXDP64 block of the GFP subsystem.
**
**                  The TXDP64 PMON counters can be updated by using the 
**                  'trigger' parameter within this function.
**                  
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  grpId               - grpId of the group being queried
**                                      - valid values are 0x0 - 0x3F (0-63)
**                  trigger             - 1 - PMON counters will be updated for 
**                                        all 64 groups
**                                      - 0 - PMON counters will not be updated
**                  *txByteCntMsb       - pointer to bits 39:32 of 40-bit bit Tx Byte Count
**                  *txByteCntLsb       - pointer to bits 31:0 of 40-bit bit Tx Byte Count
**                  *txFrmCnt           - pointer to 32 bit Tx Frame Count
**                  *txErrFrmCnt        - pointer to 32 bit Tx Errord Frame Count
**                  *txCmfFrmCnt        - pointer to 32 bit Tx CMF Frame Count
**                  *txIdleFrmCnt       - pointer to 32 bit Tx Idle Frame Count
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
INT4 hyPhy20gGfpTxPmonGfpFHdlcCntrs(struct file *fileHndl, UINT4 slice, 
                                    UINT4 grpId, UINT4 trigger, 
                                    UINT4 *txByteCntMsb, UINT4 *txByteCntLsb,
                                    UINT4 *txFrmCnt, UINT4 *txErrFrmCnt,
                                    UINT4 *txCmfFrmCnt, UINT4 *txIdleFrmCnt)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 msbs;
    UINT4 lsbs;
    UINT4 data0, data1, data2, data3, data4, data5;
         
   /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(trigger > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Capture PMON data */   
    result = gfpfHyPhy20gTXDPPerfMonIndRead(fileHndl, slice, grpId, trigger,
                                            &data0, &data1, &data2, &data3, 
                                            &data4, &data5);
    if (result) return result;
      
    /* Compute Results */
    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data1, mask, offset);

    *txByteCntMsb = msbs;
    *txByteCntLsb = data0;
    
    mask = 0xFFFFFF00;
    offset = 8; 
    lsbs =  mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data2, mask, offset);

    *txFrmCnt = lsbs + (msbs << 24);
    
    mask = 0xFFFFFF00;
    offset = 8; 
    lsbs =  mHYPHY20G_GET_FIELD4(data2, mask, offset);

    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data3, mask, offset);

    *txErrFrmCnt = lsbs + (msbs << 24);

    mask = 0xFFFFFF00;
    offset = 8; 
    lsbs =  mHYPHY20G_GET_FIELD4(data3, mask, offset);

    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data4, mask, offset);

    *txCmfFrmCnt = lsbs + (msbs << 24);

    mask = 0xFFFFFF00;
    offset = 8; 
    lsbs =  mHYPHY20G_GET_FIELD4(data4, mask, offset);

    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data5, mask, offset);

    *txIdleFrmCnt = lsbs + (msbs << 24);
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTxPmonGfpFHdlcCntrs */   

/*******************************************************************************
**
**  hyPhy20gGfpRxPmonGrpFrmCntrs
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function retrieves the PMON counters for Group-based
**                  frame events within the RXDP64 block of the GFP subsystem.
**                  
**                  A trigger to update the PMON counters for all 64 groups 
**                  can be issued by either using the LCLK signal, or the 
**                  hyPhy20gGfpRxdpPmonTrig function.
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  grpId               - grpId of the group being queried
**                                      - valid values are 0x0 - 0x3F (0-63)
**                  *rxByteCntMsb       - pointer to bits 39:32 of 40-bit bit Rx Byte Count
**                  *rxByteCntLsb       - pointer to bits 31:0 of 40-bit bit Rx Byte Count
**                  *rxCtrlFrmCnt       - pointer to 8 bit Rx Control Frame Count
**                  *rxFrmCnt           - pointer to 32 bit Rx Frame Count
**                  *rxIdleFrmCnt       - pointer to 32 bit Rx Idle Frame Count
**                                
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
INT4 hyPhy20gGfpRxPmonGrpFrmCntrs(struct file *fileHndl, UINT4 slice, 
                                  UINT4 grpId, UINT4 *rxByteCntMsb,
                                  UINT4 *rxByteCntLsb, UINT4 *rxCtrlFrmCnt, 
                                  UINT4 *rxFrmCnt, UINT4 *rxIdleFrmCnt)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 msbs;
    UINT4 data0, data1, data2, data3;
         
   /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Capture PMON data from PMON counter 0 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x7, grpId,
                                            &data0, &data1);
    if (result) return result;
      
    /* Capture PMON data from PMON counter 1 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x8, grpId,
                                            &data2, &data3);
    if (result) return result;
    
    /* Compute Results */
    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data1, mask, offset);

    *rxByteCntMsb = msbs;
    *rxByteCntLsb = data0;
    
    mask = 0xFF00;
    offset = 8; 
    *rxCtrlFrmCnt = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    *rxFrmCnt = data2;
    
    *rxIdleFrmCnt = data3;
          
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxPmonGrpFrmCntrs */   


/*******************************************************************************
**
**  hyPhy20gGfpRxPmonGrpErrCntrs
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function retrieves the PMON counters for Group-based
**                  error events within the RXDP64 block of the GFP subsystem.
**                  
**                  A trigger to update the PMON counters for all 64 groups 
**                  can be issued by either using the LCLK signal, or the 
**                  hyPhy20gGfpRxdpPmonTrig function.
**
**  INPUTS:         *fileHndl       - base address
**                  slice           - 0 - Slice 'A'
**                                  - 1 - Slice 'B'
**                  grpId           - grpId of the group being queried
**                                  - valid values are 0x0 - 0x3F (0-63)
**                  *rxChecAbrt     - GFP Mode : pointer to 16 bit uncorrectable
**                                               cHEC error count.
**                                  - HDLC Mode : pointer to 16 bit abort
**                                                sequence error count.
**                  *rxThecMinfl    - GFP Mode : pointer to 16 bit uncorrectable
**                                               tHEC error count.
**                                  - HDLC Mode : pointer to 16 bit minimum
**                                                frame length error count.
**                  *rxEhecMaxfl    - GFP Mode : pointer to 16 bit uncorrectable
**                                               eHEC error count.
**                                  - HDLC Mode : pointer to 16 bit maximum
**                                                frame length error count.
**                  *rxEcorrCnt     - GFP Mode : pointer to 20 bit Rx Counter for 
**                                               frames received with single bit 
**                                               error in the frame header. 
**                                  - HDLC Mode : not valid
**                  *rxDropFrmCnt   - pointer to 16 bit Rx Counter for frames
**                                    dropped due to Generic Filter 0/1 mismatch.
**                  *rxExiFltDrop   - GFP Mode : pointer to 24 bit EXI Filter
**                                               drop frame count.
**                                  - HDLC Mode : not valid.
**                                
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
INT4 hyPhy20gGfpRxPmonGrpErrCntrs(struct file *fileHndl, UINT4 slice, 
                                  UINT4 grpId, UINT4 *rxChecAbrt,
                                  UINT4 *rxThecMinfl, UINT4 *rxEhecMaxfl,
                                  UINT4 *rxEcorrCnt,  UINT4 *rxDropFrmCnt,
                                  UINT4 *rxExiFltDrop)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 msbs, lsbs;
    UINT4 data0, data1, data2, data3, data4, data5;
         
   /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Capture PMON data from PMON counter 2 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x9, grpId,
                                            &data0, &data1);
    if (result) return result;
      
    /* Capture PMON data from PMON counter 3 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0xA, grpId,
                                            &data2, &data3);
    if (result) return result;
    
    /* Capture PMON data from PMON counter 4 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0xB, grpId,
                                            &data4, &data5);
    if (result) return result;

    /* Compute Results */
    mask = 0xFFFF;
    offset = 0; 
    *rxChecAbrt =   mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = 0xFFFF0000;
    offset = 16;  
    lsbs =  mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = 0xF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data1, mask, offset);

    *rxThecMinfl = lsbs + (msbs << 16);

    mask = 0xFFFF0000;
    offset = 16;  
    *rxEhecMaxfl =  mHYPHY20G_GET_FIELD4(data1, mask, offset);
          
    mask = 0xFFFFF;
    offset = 0; 
    *rxEcorrCnt =   mHYPHY20G_GET_FIELD4(data2, mask, offset);

    mask = 0xFFFF;
    offset = 0; 
    *rxDropFrmCnt =   mHYPHY20G_GET_FIELD4(data3, mask, offset);

    mask = 0xFFFFFF;
    offset = 0; 
    *rxExiFltDrop = mHYPHY20G_GET_FIELD4(data4, mask, offset);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxPmonGrpErrCntrs */ 

/*******************************************************************************
**
**  hyPhy20gGfpRxPmonGfpFCpbidCntrs
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function retrieves the PMON counters for Channel-based
**                  events within the RXDP64 block of the GFP subsystem.
**                  
**                  A trigger to update the PMON counters for all 64 groups 
**                  can be issued by either using the LCLK signal, or the 
**                  hyPhy20gGfpRxdpPmonTrig function.
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  cpbId               - cpbId of the channel being queried
**                                      - valid values are 0x0 - 0x3F (0-63)
**                  *rxFdfOvfCnt        - pointer to 16 bit Rx Counter for frames
**                                        dropped due to FDF overflow
**                  *rxCsfFrmCnt        - pointer to 8 bit Rx CSF Frame Count
**                                        (Applies to GFP only)
**                  *rxErrFcsCnt        - pointer to 20 bit Rx Counter for frames
**                                        received with an erred pFCS.
**                  *rxPauseFrmCnt      - pointer to 8 bit Rx Counter for pause
**                                        frames dropped after extracting the
**                                        pause time. (Applies to GFP only).
**                  *rxPtiUpiDropCnt    - pointer to 24 bit Rx Counter for frames
**                                        dropped by the PTI/UPI filter.  
**                                        (Applies to GFP only).
**                                
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
INT4 hyPhy20gGfpRxPmonGfpFCpbidCntrs(struct file *fileHndl, UINT4 slice, 
                                     UINT4 cpbId, UINT4 *rxFdfOvfCnt,
                                     UINT4 *rxCsfFrmCnt, UINT4 *rxErrFcsCnt,
                                     UINT4 *rxPauseFrmCnt, UINT4 *rxPtiUpiDropCnt)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 data0, data1, data2, data3, data4, data5;
         
   /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Capture PMON data from PMON counter 3 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0xA, cpbId,
                                            &data0, &data1);
    if (result) return result;
      
    /* Capture PMON data from PMON counter 5 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x23, cpbId,
                                            &data2, &data3);
    if (result) return result;
    
    /* Capture PMON data from PMON counter 6 */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x24, cpbId,
                                            &data4, &data5);
    if (result) return result;

    /* Compute Results */
    mask = 0xFFFF0000;
    offset = 16;  
    *rxFdfOvfCnt = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    mask = 0xFF;
    offset = 0; 
    *rxCsfFrmCnt =  mHYPHY20G_GET_FIELD4(data2, mask, offset);

    mask = 0xFFFFF00;
    offset = 8; 
    *rxErrFcsCnt =  mHYPHY20G_GET_FIELD4(data2, mask, offset);
          
    mask = 0xFF;
    offset = 0; 
    *rxPauseFrmCnt =  mHYPHY20G_GET_FIELD4(data4, mask, offset);

    mask = 0xFFFFFF00;
    offset = 8; 
    *rxPtiUpiDropCnt =  mHYPHY20G_GET_FIELD4(data4, mask, offset);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxPmonGfpFCpbidCntrs */   

/*******************************************************************************
**
**  hyPhy20gGfpRxGetPauseTime
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function extracts the Pause time received in the 
**                  pause frame.
**                  
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  cpbId               - cpbId of the channel being queried
**                                      - valid values are 0x0 - 0x3F (0-63)
**                  *rxPauseTime        - pointer to 16 bit Pause Time extracted
**                                        from Rx Pause frame.
**                                
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
INT4 hyPhy20gGfpRxGetPauseTime(struct file *fileHndl, UINT4 slice, 
                               UINT4 cpbId, UINT4 *rxPauseTime)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 data0, data1;
         
   /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Capture RXDP data */   
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x22, cpbId,
                                            &data0, &data1);
    if (result) return result;

    /* Compute Results */
    mask = 0xFFFF;
    offset = 0; 
    *rxPauseTime = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxGetPauseTime */   

/*******************************************************************************
**
**  hyPhy20gGfpRxGrpStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of group based status 
**                  indicators within the RXDP64 block of the GFP subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl     - base address
**                  slice         - 0 - Slice 'A'
**                                - 1 - Slice 'B'
**                  grpId         - GID of the group being queried
**                                - valid values are 0x0 - 0x3F (0-63)
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                  function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                  function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 12 - LOFD_I
**                                      Bit 11 - UCHEC_ABORT_I 
**                                      Bit 10 - CCHEC_LAPSTSPE_I
**                                      Bit 9 - UTHEC_MINFL_I 
**                                      Bit 8 - CTHEC_I
**                                      Bit 7 - UEHEC_MAXFL_I 
**                                      Bit 6 - CEHEC_I
**                                      Bit 5 - EXI_I 
**                                      Bit 4 - DROP_I
**                                      Bit 3 - IDLE_I 
**                                      Bit 2 - CTRLFRM_I
**                                      Bit 1 - IDLE_V
**                                      Bit 0 - LOFD_V
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxGrpStatus(struct file *fileHndl, UINT4 slice, 
                                  UINT4 grpId, UINT4 intClear, 
                                  UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;      
    UINT4 lofdI, uchecAbortI, cchecLapstspeI, uthecMinflI, cthecI;
    UINT4 uehecMaxflI, cehecI, exiI, dropI, idleI, ctrlFrmI, idleV, lofdV;

          
   /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get LOFD_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_LOSS_FRM_DELN_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &lofdI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get UCHEC_ABORT_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_UCHEC_ERR_HDLC_ABORT_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &uchecAbortI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     
    
    /* Get CCHEC_LAPSTSPE_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CCHEC_ERR_LAPS_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &cchecLapstspeI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get UTHEC_MINFL_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_UNCORR_THEC_ERR_HDLC_MINFL_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &uthecMinflI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get CTHEC_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CTHEC_ERR_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &cthecI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get UEHEC_MAXFL_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_UNCORR_EHEC_ERR_HDLC_MAX_FRM_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &uehecMaxflI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get CEHEC_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CEHEC_ERR_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &cehecI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get EXI_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_EXI_MSMTCH_ERR_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &exiI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get DROP_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_HDLC_DRP_FRM_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &dropI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get IDLE_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_IDLE_FRM_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &idleI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get CTRLFRM_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_NON_IDLE_CTL_FRM_INTR_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &ctrlFrmI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get IDLE_V result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_FRM_IDLE_INT_VAL_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &idleV);
    if (result) return result;         
      
    /* Get LOFD_V result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_LOSS_OF_FRM_DELN_INT_VAL_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &lofdV);
    if (result) return result;         

    /* Compute results */
    *statusBits = (lofdI << 12) + (uchecAbortI << 11) + (cchecLapstspeI << 10) +
                  (uthecMinflI << 9) + (cthecI << 8) + (uehecMaxflI << 7) +
                  (cehecI << 6) + (exiI << 5) + (dropI << 4) +
                  (idleI << 3) + (ctrlFrmI << 2) + (idleV << 1) + lofdV;
                  

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxGrpStatus */   

/*******************************************************************************
**
**  hyPhy20gGfpRxCpbidStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of channel based status 
**                  indicators within the RXDP64 block of the GFP subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl     - base address
**                  slice         - 0 - Slice 'A'
**                                - 1 - Slice 'B'
**                  cpbId         - CPBID of the channel being queried
**                                - valid values are 0x0 - 0x3F (0-63)
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                  function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                  function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 9 - DWNSTRM_FRM_I
**                                      Bit 8 - PFCSE_I
**                                      Bit 7 - PTIUPI_MSMCH_I 
**                                      Bit 6 - PTIUPI_DRP_I
**                                      Bit 5 - DATAFRM_I 
**                                      Bit 4 - CSF_I
**                                      Bit 3 - FDF_DATA_AV_I 
**                                      Bit 2 - FDF_OVF_I
**                                      Bit 1 - PAUSE_I
**                                      Bit 0 - CPBID_LUTFAIL_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxCpbidStatus(struct file *fileHndl, UINT4 slice, 
                              UINT4 cpbId, UINT4 intClear, 
                              UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset; 
    UINT4 pfcseI, ptiUpiMsmchI, ptiUpiDrpI, dataFrmI, csfI, fdfDataAvI; 
    UINT4 fdfOvfI, pauseI, cpbidLutFailI, dwnstrmFrm; 
         
   /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get DWNSTRM_FRM_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_DOWNSTREAM_FRM_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &dwnstrmFrm);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get PFCSE_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_PYLD_FCS_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &pfcseI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get PTIUPI_MSMCH_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_PTI_UPI_MSMTCH_ERR_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &ptiUpiMsmchI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     
    
    /* Get PTIUPI_DRP_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_PTI_UPI_MTCH_DROP_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &ptiUpiDrpI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get DATAFRM_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CLNT_DAT_FRM_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &dataFrmI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get CSF_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CSF_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &csfI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get FDF_DATA_AV_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_FRM_DAT_FIFO_AVLBL_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &fdfDataAvI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get FDF_OVF_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_FRM_DAT_FIFO_OFL_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &fdfOvfI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get PAUSE_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_PAUSE_INTR_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = cpbId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &pauseI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get CPBID_LUTFAIL_I result */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_LKUP_FAIL_INTR;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = 0;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &cpbidLutFailI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Compute results */
    *statusBits = (dwnstrmFrm << 9) + (pfcseI << 8) + (ptiUpiMsmchI << 7) + 
                  (ptiUpiDrpI << 6) + (dataFrmI << 5) + (csfI << 4) + 
                  (fdfDataAvI << 3) + (fdfOvfI << 2) + (pauseI << 1) + 
                  cpbidLutFailI;
                  
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxCpbidStatus */   

/*******************************************************************************
**
**  hyPhy20gGfpRxCsfFifoStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of group-independant
**                  CSF status FIFO within the RXDP64 block of the GFP 
**                  subsystem.
**                  
**
**  INPUTS:         *fileHndl     - base address
**                  slice         - 0 - Slice 'A'
**                                - 1 - Slice 'B'
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                  function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                  function will be left untouched.
**                  fifoClear     - 1 - CSF status FIFO will be cleared after 
**                                  reading FIFO status bits
**                                - 0 - CSF status FIFO will not be cleared
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 1 - CSF_ST_AV_I
**                                      Bit 0 - CSF_ST_FIFO_OF
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxCsfFifoStatus(struct file *fileHndl, UINT4 slice,  
                                UINT4 intClear, UINT4 fifoClear,
                                UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 csfFifoOf;
    UINT4 csfStAv;
         
   /* argument checking */
    if ((slice > 1)||(intClear > 1)||(fifoClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CSF_ST_AV_INTR;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);              
    result = sysHyPhy20gBitRead(fileHndl, regAddr, 0, &csfStAv);

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 1);
        if (result) return result;   
    }     

    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CSF_ST_FIFO_OF_INTR;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);              
    result = sysHyPhy20gBitRead(fileHndl, regAddr, 0, &csfFifoOf);

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 1);
        if (result) return result;   
    }     

    /* Clear FIFO if enabled */
    if (fifoClear) {
        regAddr = HYPHY20G_GFP_FRM_RXDP_TSB_BASE;
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);              
        offset = HYPHY20G_GFP_FRM_RXDP_BIT_CFG_CSF_FIFO_CLR_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;   
    }       

    /* Compute results */
    *statusBits = (csfStAv << 1) + csfFifoOf;
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxCsfFifoStatus */   

/*******************************************************************************
**
**  hyPhy20gGfpRxCsfFifoRead
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function provides the UPI and CPBID for a CSF frame 
**                  contained within the CSF status FIFO
**                  
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  *cpbId              - pointer to CPBID on which the CSF was
**                                        received.
**                  *upi                - pointer to UPI value within the CSF.
**                  *valid              - pointer to VALID bit - when this bit
**                                        is '0' it indicates that new data is 
**                                        being provided.  When this bit is '1' it
**                                        indicates data from a previous CSF is
**                                        being provided.  This function should
**                                        be called repeatedly until VALID = 1.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxCsfFifoRead(struct file *fileHndl, UINT4 slice, 
                              UINT4 *cpbId, UINT4 *upi, UINT4 *valid)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 csfSt;

    /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CSF_STAT;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &csfSt);
    if (result) return result;   

    mask = HYPHY20G_GFP_FRM_RXDP_BIT_CSF_STAT_CSF_ST_CHID_MSK;
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_CSF_STAT_CSF_ST_CHID_OFF;
    *cpbId =  mHYPHY20G_GET_FIELD4(csfSt, mask, offset);
    
    mask = HYPHY20G_GFP_FRM_RXDP_BIT_CSF_STAT_CSF_ST_UPI_MSK;
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_CSF_STAT_CSF_ST_UPI_OFF;
    *upi =  mHYPHY20G_GET_FIELD4(csfSt, mask, offset);

    mask = HYPHY20G_GFP_FRM_RXDP_BIT_CSF_STAT_CSF_ST_VALID_MSK;
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_CSF_STAT_CSF_ST_VALID_OFF;
    *valid =  mHYPHY20G_GET_FIELD4(csfSt, mask, offset);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxCsfFifoRead */

/*******************************************************************************
**
**  hyPhy20gGfpTxFifoStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of group-independant
**                  FIFO interrupts within the TXDP64 block of the GFP 
**                  subsystem.
**                  
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  intClear            - 1 - all *_I interrupt bits accessed by this 
**                                        function will be cleared by writing them to '1'
**                                      - 0 - all *_I interrupt bits accessed by this  
**                                        function will be left untouched.
**                  *statusBits         - Pointer to storage for status bits
**                                        Bit 1 - FDI_FIFO_OVR_I
**                                        Bit 0 - FDI_FIFO_UDR_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTxFifoStatus(struct file *fileHndl, UINT4 slice,  
                             UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 fdiFifoI;
    UINT4 resultMask;
         
   /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_GRP_IDEP_INT_STAT_EVNT;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    result = sysHyPhy20gRead(fileHndl, regAddr, &fdiFifoI);
    if (result) return result;         

    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_GFP_FRM_TXDP_BIT_GRP_IDEP_INT_STAT_EVNT_FDI_FIFO_UDR_I_MSK +
                     HYPHY20G_GFP_FRM_TXDP_BIT_GRP_IDEP_INT_STAT_EVNT_FDI_FIFO_OVR_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    }   

    mask = 0x3;
    offset = 0; 
    *statusBits = mHYPHY20G_GET_FIELD4(fdiFifoI, mask, offset);
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTxFifoStatus */   

/*******************************************************************************
**
**  hyPhy20gGfpTxGrpStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of group based status 
**                  indicators within the TXDP64 block of the GFP subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl     - base address
**                  slice         - 0 - Slice 'A'
**                                - 1 - Slice 'B'
**                  grpId         - GID of the group being queried
**                                - valid values are 0x0 - 0x3F (0-63)
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                  function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                  function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 1 - FIFO_ERR_INT_I
**                                      Bit 0 - LEN_ERR_INT_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTxGrpStatus(struct file *fileHndl, UINT4 slice, 
                            UINT4 grpId, UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;  
    UINT4 fifoErrI, lenErrI;  
         
   /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get FIFO_ERR_INT_I result */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_FIFO_ERR_INT_EVNT_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &fifoErrI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     

    /* Get LEN_ERR_INT_I result */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_FRM_LEN_ERR_INT_EVNT_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &lenErrI);
    if (result) return result;         

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    }     
    

    /* Compute results */
    *statusBits = (fifoErrI << 1) + lenErrI;    
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTxGrpStatus */   

/*******************************************************************************
**
**  hyPhy20gGfpTranspTxGrpStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of group based status 
**                  indicators within the OTNMA block of the GFP subsystem.
**                  
**
**  INPUTS:         *fileHndl     - base address
**                  slice         - 0 - Slice 'A'
**                                - 1 - Slice 'B'
**                  grpId         - GID of the group being queried
**                                - valid values are 0x0 - 0x3F (0-63)
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                  function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                  function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 5 - GRP_N_FAIL_NORMAL_MODE
**                                      Bit 4 - PN11_INSERT_I
**                                      Bit 3 - PKT_LEN_COMP_OVRFLOW_I 
**                                      Bit 2 - PKT_LEN_COMP_UNDFLOW_I
**                                      Bit 1 - IFIFO_OVRFLOW_I
**                                      Bit 0 - IFIFO_UNDFLOW_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTranspTxGrpStatus(struct file *fileHndl, UINT4 slice, 
                                  UINT4 grpId, UINT4 intClear,
                                  UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;   
    UINT4 grpIntSt;
    UINT4 resultMask = 0; 
    UINT4 lsbs;
    UINT4 grpMode;
         
   /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get *_I results */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_GRP_SPEC_INT_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    result = sysHyPhy20gRead(fileHndl, regAddr, &grpIntSt);
    if (result) return result;         

    mask = 0x1F;
    offset = 0; 
    lsbs = mHYPHY20G_GET_FIELD4(grpIntSt, mask, offset);

    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_GFP_TRA_OTNMA_BIT_GRP_SPEC_INT_IFIFO_UNDFLOW_I_MSK +
                     HYPHY20G_GFP_TRA_OTNMA_BIT_GRP_SPEC_INT_IFIFO_OVRFLOW_I_MSK +
                     HYPHY20G_GFP_TRA_OTNMA_BIT_GRP_SPEC_INT_PKT_LEN_COMP_UNDFLOW_I_MSK +
                     HYPHY20G_GFP_TRA_OTNMA_BIT_GRP_SPEC_INT_PKT_LEN_COMP_OVRFLOW_I_MSK +
                     HYPHY20G_GFP_TRA_OTNMA_BIT_GRP_SPEC_INT_PN11_INSERT_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    } 
    
    /* Get GRP_N_FAIL_NORMAL_MODE results */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_STAT_1 + 4*(grpId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &grpMode);
    if (result) return result;  
      

    /* Compute results */
    *statusBits = (grpMode << 5) + lsbs;    
   
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTranspTxGrpStatus */   

/*******************************************************************************
**
**  hyPhy20gGfpTranspRxGrpStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of group based status 
**                  indicators within the OTNMD block of the GFP subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl           - base address
**                  slice         - 0 - Slice 'A'
**                                - 1 - Slice 'B'
**                  grpId         - GID of the group being queried
**                                - valid values are 0x0 - 0x3F (0-63)
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                  function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                  function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 1 - IFIFO_OVRFLOW_I
**                                      Bit 0 - IFIFO_UNDFLOW_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTranspRxGrpStatus(struct file *fileHndl, UINT4 slice, 
                                  UINT4 grpId, UINT4 intClear,
                                  UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;    
    UINT4 resultMask = 0;
    UINT4 grpIntSt;
         
   /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get *_I results */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_GRP_SPEC_INT_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    result = sysHyPhy20gRead(fileHndl, regAddr, &grpIntSt);
    if (result) return result;         


    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_GFP_TRA_OTNMD_BIT_GRP_SPEC_INT_IFIFO_UNDFLOW_I_MSK +
                     HYPHY20G_GFP_TRA_OTNMD_BIT_GRP_SPEC_INT_IFIFO_OVRFLOW_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    }   

    /* Compute results */
    mask = 0x3;
    offset = 0; 
    *statusBits = mHYPHY20G_GET_FIELD4(grpIntSt, mask, offset);
   
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTranspRxGrpStatus */   

/*******************************************************************************
**
**  hyPhy20gGfpTranspFifoStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of FIFO interrupts 
**                  within the OTNMA and OTNMD block of the GFP subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**
**  INPUTS:         *fileHndl     - base address
**                  slice         - 0 - Slice 'A'
**                                - 1 - Slice 'B'
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                  function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                  function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 3 - OTNMA_DLY_FIFO_OVRFLOW_I
**                                      Bit 2 - OTNMA_DLY_FIFO_UNDFLOW_I
**                                      Bit 1 - OTNMD_DLY_FIFO_OVRFLOW_I
**                                      Bit 0 - OTNMD_DLY_FIFO_UNDFLOW_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTranspFifoStatus(struct file *fileHndl, UINT4 slice, 
                                    UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;    
    UINT4 otnmaIntSt, otnmdIntSt;
    UINT4 msbs, lsbs;
    UINT4 resultMask = 0;
         
   /* argument checking */
    if ((slice > 1)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get OTNMA _I results */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_DLY_FIFO_INT;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaIntSt);
    if (result) return result;         

    mask = 0x3;
    offset = 0; 
    msbs = mHYPHY20G_GET_FIELD4(otnmaIntSt, mask, offset);

    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_GFP_TRA_OTNMA_BIT_DLY_FIFO_INT_DLY_FIFO_UNDFLOW_I_MSK +
                     HYPHY20G_GFP_TRA_OTNMA_BIT_DLY_FIFO_INT_DLY_FIFO_OVRFLOW_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    } 

    /* Get OTNMD _I results */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_DLY_FIFO_SPEC_INT;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmdIntSt);
    if (result) return result;         

    mask = 0x3;
    offset = 0; 
    lsbs = mHYPHY20G_GET_FIELD4(otnmdIntSt, mask, offset);

    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_GFP_TRA_OTNMD_BIT_DLY_FIFO_SPEC_INT_DLY_FIFO_UNDFLOW_I_MSK +
                     HYPHY20G_GFP_TRA_OTNMD_BIT_DLY_FIFO_SPEC_INT_DLY_FIFO_OVRFLOW_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    } 
      

    /* Compute results */
    *statusBits = (msbs << 2) + lsbs;    
   
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTranspFifoStatus */  

/*******************************************************************************
**
**  hyPhy20gGfpRxPmonGfpTCpbidCntrs
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function retrieves the PMON counters for Channel-based
**                  events within the RXGFPT block of the GFP subsystem.
**                  
**                  Calling this function will automatically trigger an update
**                  of the RXGFPT PMON counters.
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  cpbId               - cpbId of the channel being queried
**                                      - valid values are 0x0 - 0x3F (0-63)
**                  *rxCorCrcCnt        - pointer to 28 bit Rx Counter for 
**                                        correctable CRC-16 errors received
**                                        on the channel.
**                  *rxUncCrcCnt        - pointer to 28 bit Rx Counter for 
**                                        uncorrectable CRC-16 errors received 
**                                        on the channel.
**                  *rxErrFreeSbCnt     - pointer to 28 bit Rx Counter for 
**                                        error-free superblocks received on the
**                                        channel.
**                  *rxProtErrCnt       - pointer to 28 bit Rx Counter for 
**                                        protocol errors received on the channel.
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
INT4 hyPhy20gGfpRxPmonGfpTCpbidCntrs(struct file *fileHndl, UINT4 slice, 
                                     UINT4 cpbId, UINT4 *rxCorCrcCnt,
                                     UINT4 *rxUncCrcCnt, UINT4 *rxErrFreeSbCnt,
                                     UINT4 *rxProtErrCnt)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 data0, data1, data2, data3;
         
   /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Capture PMON data */   
    result = gfptHyPhy20gRXGFPTPerfMonIndRead(fileHndl, slice, cpbId,
                                            &data0, &data1, &data2, &data3);
    if (result) return result;
      
    /* Compute Results */
    mask = 0xFFFFFFF;
    offset = 0; 
    *rxCorCrcCnt =  mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = 0xFFFFFFF;
    offset = 0; 
    *rxUncCrcCnt =  mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = 0xFFFFFFF;
    offset = 0; 
    *rxErrFreeSbCnt =   mHYPHY20G_GET_FIELD4(data2, mask, offset);

    mask = 0xFFFFFFF;
    offset = 0; 
    *rxProtErrCnt =   mHYPHY20G_GET_FIELD4(data3, mask, offset);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRxPmonGfpTCpbidCntrs */   

/*******************************************************************************
**
**  hyPhy20gGfpTxPmonGfpTCntrs
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function updates the PMON counters for events within
**                  the TXGFPT block of the GFP subsystem.
**                  
**                  Calling this function will automatically trigger an update
**                  of the TXGFPT PMON counters.
**
**  INPUTS:         *fileHndl           - base address
**                  slice               - 0 - Slice 'A'
**                                      - 1 - Slice 'B'
**                  cpbId               - cpbId of the channel being queried
**                                      - valid values are 0x0 - 0x3F (0-63)
**                  *txByteCntMsb       - pointer to bits 39:32 of 40 bit Tx Byte Count
**                  *txByteCntLsb       - pointer to bits 31:0 of 40 bit Tx Byte Count
**                  *tx65bPadCntMsb     - pointer to bits 39:32 of 40 bit Tx 65B Pad Character
**                                        Count
**                  *tx65bPadCntLsb     - pointer to bits 31:0 of 40 bit Tx 65B Pad Character
**                                        Count
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
INT4 hyPhy20gGfpTxPmonGfpTCntrs(struct file *fileHndl, UINT4 slice, 
                                UINT4 cpbId, UINT4 *txByteCntMsb,
                                UINT4 *txByteCntLsb, UINT4 *tx65bPadCntMsb,
                                UINT4 *tx65bPadCntLsb)
{
    INT4 result;
    UINT4 mask;
    UINT4 offset;
    UINT4 msbs;
    UINT4 lsbs;
    UINT4 data0, data1, data2, data3;
         
   /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Capture PMON data */   
    result = gfptHyPhy20gTXGFPTPerfMonIndRead(fileHndl, slice, cpbId,
                                            &data0, &data1, &data2, &data3);
    if (result) return result;
      
    /* Compute Results */
    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data1, mask, offset);

    *txByteCntMsb = msbs;
    *txByteCntLsb = data0;

    mask = 0xFFFFFF00;
    offset = 8; 
    lsbs =  mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = 0xFF;
    offset = 0; 
    msbs =  mHYPHY20G_GET_FIELD4(data2, mask, offset);

    *tx65bPadCntLsb = lsbs + (msbs << 24);
     
    mask = 0xFF00;
    offset = 8; 
    *tx65bPadCntMsb =   mHYPHY20G_GET_FIELD4(data2, mask, offset);


    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTxPmonGfpTCntrs */


/*******************************************************************************
**
**  hyPhy20gGfpRxdpPmonTrig
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures a manual update of the PMON counters within
**                  the RXDP64 of the GFP subsystem.  This procedure can be used 
**                  when the LCLK PMON update method is not being used.  Calling 
**                  this command will trigger a transfer of all the PMON counts 
**                  for all groups and channels in the RXDP64 from internal memory 
**                  to holding registers.  
**
**                  The following commands can then be used to query PMON counts for GFP/HDLC traffic
**                  within the RXDP64 block:
**
**                  hyPhy20gGfpRxPmonGfpFCpbidCntrs 
**                  hyPhy20gGfpRxPmonGrpErrCntrs 
**                  hyPhy20gGfpRxPmonGrpFrmCntrs 
**              
**  INPUTS:     *fileHndl       - base address
**              slice           - 0 - Slice 'A'
**                              - 1 - Slice 'B'
**                                
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxdpPmonTrig(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 gfpFrmRst, gfpTraRst;
    UINT4 mask;
    UINT4 offset;

    /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_PMON_LATCH_CTL;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    /* Set PMON_LATCH_TRIG bit */
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_PMON_LATCH_CTL_PMON_LATCH_TRIG_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;         
    
    /* Verify that PMON_LATCH_TRIG bit clears */
    mask = HYPHY20G_GFP_FRM_RXDP_BIT_PMON_LATCH_CTL_PMON_LATCH_TRIG_MSK;
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;

} /* hyPhy20gGfpRxdpPmonTrig */



