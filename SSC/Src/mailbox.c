/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup Mailbox Mailbox Functions
@{
*/

/**
\file    mailbox.c
\author EthercatSSC@beckhoff.com
\brief Implementation

<br>Description of the mailbox buffer handling (MAILBOX_QUEUE = 0):<br>
There are two mailbox buffer for sending and receiving mailbox services.<br>
Normal operation:<br>
When starting the mailbox handler psWriteMbx contains mailbox buffer 1,<br>
psReadMbx, psRepeatMbx and psStoreMbx are 0.<br>
In this state a repeat request would be ignored because there was no service sent yet.<br>
When the first mailbox service is sent (in MBX_CopyToSendMailbox), psWriteMbx gets mailbox buffer 2<br>
and psReadMbx gets the sent mailbox buffer 1, psRepeatMbx and psStoreMbx are still 0.<br>
When the first mailbox service was read from the master, the sent mailbox buffer 1 is stored<br>
in psRepeatMbx (in MBX_MailboxReadInd).<br>
After that psReadMbx gets always the actual sent mailbox buffer, psWriteMbx is set to 0 (another<br>
received mailbox service from the master will not be handled until the sent mailbox service was read<br>
and MBX_MailboxReadInd was called).<br>
When the mailbox service is read, psWriteMbx gets the Buffer of psRepeatMbx and psRepeatMbx gets the <br>
buffer of psReadMbx.<br>
Repeat Request from the master:<br>
When a Repeat from the master is requested (MBX_MailboxRepeatReq), there are three different possibilities:<br>
1. no mailbox service was sent since the mailbox handler was started (psRepeatMbx = 0): nothing to do<br>
2. the acknowledge of the last sent mailbox service was received (in MBX_MailboxReadInd) (bSendMbxIsFull = 0):<br>
the last sent mailbox service (psRepeatMbx) will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,<br>
psRepeatMbx will be set to 0<br>
3. the acknowledge of the last sent mailbox service was not received (psReadMbx and psRepeatMbx contain different buffers,<br>
psReadMbx is still in the mailbox (because MBX_MailboxReadInd is not called yet, bSendMbxIsFull = 1): <br>
psReadMbx will be deleted in the mailbox (call of DisableSyncManChannel and EnableSyncManChannel) and<br>
stored in psStoreMbx, psRepeatMbx will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,<br>
psRepeatMbx will be set to 0.<br>
When the repeated mailbox service was sent (call of MBX_MailboxReadInd), psReadMbx will be stored in psRepeatMbx<br>
and psStoreMbx will be sent (in MBX_CopyToSendMailbox) and stored in psReadMbx, psStoreMbx will be set to 0.<br>

<br>Description of the mailbox buffer handling (MAILBOX_QUEUE = 1):<br>
There are two mailbox buffer for sending and receiving mailbox services.<br>
Normal operation (psWriteMbx is only used for local storage):<br>
When starting the mailbox handler psReadMbx, psRepeatMbx and psStoreMbx are 0.<br>
In this state a repeat request would be ignored because there was no service sent yet.<br>
When a mailbox service is received from the master (in MBX_CheckAndCopyMailbox) a mailbox buffer<br>
will be get with APPL_AllocMailboxBuffer and the corresponding protocol service function will<br>
be called (in MBX_WriteMailboxInd). This buffer shall be used for the protocol service response.<br>
When the first mailbox service is sent (in MBX_CopyToSendMailbox), psReadMbx gets the sent mailbox buffer,<br>
psRepeatMbx and psStoreMbx are still 0.<br>
When the first mailbox service was read from the master, the sent mailbox buffer (psReadMbx) is stored<br>
in psRepeatMbx (in MBX_MailboxReadInd).<br>
After that psReadMbx gets always the actual sent mailbox buffer (in MBX_CopyToSendMailbox)<br>
When the mailbox service is read, psRepeatMbx is returned (with APPL_FreeMailboxBuffer) and psRepeatMbx gets the <br>
buffer of psReadMbx.<br>
Repeat Request from the master:<br>
When a Repeat from the master is requested (MBX_MailboxRepeatReq), there are three different possibilities:<br>
1. no mailbox service was sent since the mailbox handler was started (psRepeatMbx = 0): nothing to do<br>
2. the acknowledge of the last sent mailbox service was received (in MBX_MailboxReadInd) (bSendMbxIsFull = 0):<br>
the last sent mailbox service (psRepeatMbx) will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,<br>
psRepeatMbx will be set to 0<br>
3. the acknowledge of the last sent mailbox service was not received (psReadMbx and psRepeatMbx contain different buffers,<br>
psReadMbx is still in the mailbox (because MBX_MailboxReadInd is not called yet, bSendMbxIsFull = 1): <br>
psReadMbx will be deleted in the mailbox (call of DisableSyncManChannel and EnableSyncManChannel) and<br>
stored in psStoreMbx, psRepeatMbx will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,<br>
psRepeatMbx will be set to 0.<br>
When the repeated mailbox service was sent (call of MBX_MailboxReadInd), psReadMbx will be stored in psRepeatMbx<br>
and psStoreMbx will be sent (in MBX_CopyToSendMailbox) and stored in psReadMbx, psStoreMbx will be set to 0.

\version 5.13

<br>Changes to version V5.12:<br>
V5.13 EOE1: <br>
V5.13 MBX2: clear unused bytes before write mailbox response<br>
V5.13 MBX3: fix potential double free memory and lost of last read mailbox data<br>
V5.13 TEST3: 0x2040.4 send ARP on unsupported mailbox request<br>
<br>Changes to version V5.11:<br>
V5.12 EOE5: free pending buffer in EoE_Init, EoE_Init is called on startup and PI transition<br>
V5.12 MBX1: use only 16Bit variables to write the last byte of the mailbox buffer in case of ESC_16BIT_Access,update clear message queue in case of stop mailbox handler<br>
V5.12 MBX2: do not set the pending indication in case of a EoE request, application triggered eoe datagram update<br>
V5.12 MBX3: handle incomplete mailbox communication<br>
V5.12 MBX4: in case of a disable mailbox queue and two consecutive foe uploads the mailbox receive handler is blocked<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 ECAT7: add missing big endian swapping<br>
V5.11 HW1: "move hardware independent functions ""HW_DisableSyncManChannel()"", ""HW_EnableSyncManChannel()"", ""HW_GetSyncMan()"", ""HW_ResetALEventMask()"", ""HW_SetALEventMask()"" to ecatalv.c"<br>
V5.11 MBX2: "check in IP if enough dynamic memory is available to handle mailbox communication, if it is not the case the mbx error ""no memory"" is returned on any mbx request"<br>
V5.11 MBX3: set application triggered emergency and EoE data to pending if no mailbox queue is supported and another mailbox request is currently handled, Handle only one mailbox request at a time (in case that MAILBPX_QUEUE is disabled)<br>
V5.11 TEST4: add new mailbox test behavior (the master mailbox cnt shall be incremented by 1 and the slave mailbox cnt is alternating)<br>
<br>Changes to version V5.01:<br>
V5.10 EOE3: Change local send frame pending indication variable to a global variable (it need to be resetted if the mailbox is stopped and a frame is pending)<br>
V5.10 ESC6: Update SyncManager register size (only for 8Bit ESC access)<br>
V5.10 MBX2: Allocate only memory for current configured mailbox size (max supported size was allocated before)<br>
<br>Changes to version V5.0:<br>
V5.01 ESC2: Add missed value swapping<br>
V5.01 MBX2: Return an error if length specified in mailbnox header is too large.<br>
<br>Changes to version V4.40:<br>
V5.0 ECAT3: Global dummy variables used for dummy ESC operations.<br>
V5.0 ESC1: ESC 32Bit Access added.<br>
V5.0 MBX2: Prevent double swapping (MBX repeat service) of MBX length field.<br>
V5.0 MBX4: Unlock SM buffer based on SM state.<br>
<br>Changes to version V4.11:<br>
V4.40 MBX8: Handle pending mailbox data even if mailbox queue is enabled<br>
V4.40 MBX7: Reset SyncManger 1 Repeat Acknowledge bit if local variable is reset<br>
V4.40 EMCY2: Send queued emergency messages<br>
V4.40 BOOT2: Support only FoE in Bootstrap state<br>
V4.40 MBX5: Prevent multiple free buffer<br>
V4.40 MBX6: change return value if no mailbox buffer is available<br>
<br>Changes to version V4.08:<br>
V4.11 MBX 1: free some dynamic memory to avoid memory leaks in the mailbox queue<br>
<br>Changes to version V4.07:<br>
V4.10 MBX 1: check Mailbox message buffer and print emergency messages<br>
               during state change Init -> PreOP<br>
<br>Changes to version V4.07:<br>
V4.08 MBX 1: The mailbox queue was added (switch MAILBOX_QUEUE), if the<br>
             mailbox should be handled in the ESC-Interrupt Service Routine<br>
             the defines ENTER_MBX_CRITICAL and LEAVE_MBX_CRITICAL could be used<br>
             to disable the interrupts or change the priority<br>
V4.08 AOE 1:    AoE was added<br>
<br>Changes to version V4.06:<br>
V4.07 ECAT 1: The sources for SPI and MCI were merged (in ecat_def.h<br>
                   set the switch MCI_HW to 1 when using the MCI,<br>
                   set the switch SPI_HW to 1 when using the SPI
*/


/*---------------------------------------------------------------------------------------
------
------    Includes
------
---------------------------------------------------------------------------------------*/

#include "ecat_def.h"


#include "ecatslv.h"



#define    _MAILBOX_    1
#include "mailbox.h"
#undef _MAILBOX_
/*remove definition of _MAILBOX_ (#ifdef is used in mailbox.h)*/

#include "ecatcoe.h"
#include "sdoserv.h"


/*--------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
/*variables are declared in ecatslv.c*/
    extern VARVOLATILE UINT16    u16dummy;

    BOOL bNoMbxMemoryAvailable; /**< \brief Indicates if enough dynamic memory is available to handle at least one mailbox datagram */

/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
------
------    functions
------
---------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////
//
//    PutInMbxQueue
//

UINT8 PutInMbxQueue(TMBX MBXMEM * pMbx, TMBXQUEUE MBXMEM * pQueue)
{
    UINT16 lastInQueue;
    ENTER_MBX_CRITICAL;


    lastInQueue = pQueue->lastInQueue+1;
    if (lastInQueue == pQueue->maxQueueSize)
    {
        // Umbruch der Queue
        lastInQueue = 0;
    }

    if (pQueue->firstInQueue == lastInQueue)
    {
        // Ueberlauf der Queue -> letztes Element wieder herausnehmen
        LEAVE_MBX_CRITICAL;
        return MBXERR_NOMOREMEMORY;
    }

    pQueue->queue[pQueue->lastInQueue] = pMbx;
    pQueue->lastInQueue = lastInQueue;

    LEAVE_MBX_CRITICAL;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//    GetOutOfMbxQueue
//

TMBX MBXMEM * GetOutOfMbxQueue(TMBXQUEUE MBXMEM * pQueue)
{
    TMBX MBXMEM * pMbx;
    ENTER_MBX_CRITICAL;

    if (pQueue->firstInQueue != pQueue->lastInQueue)
    {
        // Queue ist nicht leer
        UINT16 firstInQueue = pQueue->firstInQueue;
        pMbx = pQueue->queue[firstInQueue];
        firstInQueue++;
        pQueue->firstInQueue = firstInQueue;
        
        if (pQueue->firstInQueue == pQueue->maxQueueSize)
        {
            // Umbruch der Queue
            pQueue->firstInQueue = 0;
        }
    }
    else
    {
        pMbx = 0;
    }


    LEAVE_MBX_CRITICAL;

    return pMbx;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function intialize the Mailbox Interface.
*////////////////////////////////////////////////////////////////////////////////////////

void MBX_Init(void)
{
    
    u16ReceiveMbxSize = MIN_MBX_SIZE;
    u16SendMbxSize = MAX_MBX_SIZE;
    u16EscAddrReceiveMbx = MIN_MBX_WRITE_ADDRESS;
    u16EscAddrSendMbx = MIN_MBX_READ_ADDRESS;

    sMbxReceiveQueue.firstInQueue    = 0;
    sMbxReceiveQueue.lastInQueue     = 0;
    sMbxReceiveQueue.maxQueueSize = MAX_MBX_QUEUE_SIZE;
    sMbxSendQueue.firstInQueue        = 0;
    sMbxSendQueue.lastInQueue         = 0;
    sMbxSendQueue.maxQueueSize     = MAX_MBX_QUEUE_SIZE;
    psWriteMbx  = NULL;


    psRepeatMbx = NULL;
    psReadMbx    = NULL;
    psStoreMbx    = NULL;

    bMbxRepeatToggle    = FALSE;
    /*Reset Repeat acknowledge bit of SyncManager1 (0x80F bit 2)*/
    {
        UINT16 sm1Activate = 0;
        HW_EscReadWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
        sm1Activate &= SWAPWORD(~0x0200);
        HW_EscWriteWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
    }
    bMbxRunning = FALSE;
    bSendMbxIsFull = FALSE;
    bReceiveMbxIsLocked = FALSE;
    u8MailboxSendReqStored    = 0;
    u8MbxWriteCounter = 0;
    u8MbxReadCounter    = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief     This function includes the state transition from INIT to
 \brief     PRE-OPERATIONAL in the EtherCAT Slave corresponding to
 \brief     local management service Start Mailbox Handler
 \brief     it is checked if the mailbox areas overlaps each other
 \brief     and the Sync Manager channels 0 and 1 are enabled.
 \brief     This function shall only be called if mailbox is supported.
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 MBX_StartMailboxHandler(void)
{
    UINT16 result = 0;
    
    /* get address of the receive mailbox sync manager (SM0) */
    TSYNCMAN ESCMEM * pSyncMan = (TSYNCMAN ESCMEM *)GetSyncMan(MAILBOX_WRITE);

    /* store size of the receive mailbox */
    u16ReceiveMbxSize     = pSyncMan->Length;
    /* store the address of the receive mailbox */
    u16EscAddrReceiveMbx = pSyncMan->PhysicalStartAddress;

    /* get address of the send mailbox sync manager (SM1) */
    pSyncMan =(TSYNCMAN ESCMEM *) GetSyncMan(MAILBOX_READ);

    /* store the size of the send mailbox */
    u16SendMbxSize = pSyncMan->Length;
    /* store the address of the send mailbox */
    u16EscAddrSendMbx = pSyncMan->PhysicalStartAddress;

    // HBu 02.05.06: it should be checked if there are overlaps in the sync manager areas
    if ((u16EscAddrReceiveMbx + u16ReceiveMbxSize) > u16EscAddrSendMbx && (u16EscAddrReceiveMbx < (u16EscAddrSendMbx + u16SendMbxSize)))
    {
        return ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }


    /* enable the receive mailbox sync manager channel */
    EnableSyncManChannel(MAILBOX_WRITE);
    /* enable the send mailbox sync manager channel */
    EnableSyncManChannel(MAILBOX_READ);

        psWriteMbx = (TMBX MBXMEM *) APPL_AllocMailboxBuffer(u16ReceiveMbxSize);
        if(psWriteMbx == NULL)
        {
            bNoMbxMemoryAvailable = TRUE;

            //check if at least enough memory for an mailbox error is available (other wise stop the state transition)
            psWriteMbx = (TMBX MBXMEM *) APPL_AllocMailboxBuffer(10); /* a mailbox error datagram length*/
            if(psWriteMbx == NULL)
            {
                result = ALSTATUSCODE_NOMEMORY;
            }

            APPL_FreeMailboxBuffer(psWriteMbx);
            psWriteMbx = NULL;
        }
        else
        {
            bNoMbxMemoryAvailable = FALSE;
            APPL_FreeMailboxBuffer(psWriteMbx);
            psWriteMbx = NULL;
        }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function includes the state transition from
 \brief    PRE-OPERATIONAL to INIT in the EtherCAT Slave corresponding to
 \brief  local management service Stop Mailbox Handler
 \brief  the Sync Manager channels 0 and 1 are disabled
*////////////////////////////////////////////////////////////////////////////////////////

void MBX_StopMailboxHandler(void)
{
    TMBX MBXMEM * pMbx;

    /* mailbox handler is stopped */
    bMbxRunning = FALSE;
    /* disable the receive mailbox sync manager channel */
    DisableSyncManChannel(MAILBOX_WRITE);
    /* disable the send mailbox sync manager channel */
    DisableSyncManChannel(MAILBOX_READ);
    /* initialize variables again */


    if (psRepeatMbx != NULL)
    {
        APPL_FreeMailboxBuffer(psRepeatMbx);
    }

    if (psStoreMbx != NULL && psStoreMbx != psRepeatMbx)
    {
        APPL_FreeMailboxBuffer(psStoreMbx);
    }

    if (psReadMbx != NULL && psReadMbx != psRepeatMbx && psReadMbx != psStoreMbx)
    {
        APPL_FreeMailboxBuffer(psReadMbx);
    }


    SDOS_ClearPendingResponse();



    psWriteMbx = NULL;
    psRepeatMbx = NULL;
    psReadMbx = NULL;
    psStoreMbx = NULL;

    bMbxRepeatToggle    = FALSE;
    /*Reset Repeat acknowledge bit of SyncManager1 (0x080F bit 2)*/
    {
        UINT16 sm1Activate = 0;
        HW_EscReadWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
        sm1Activate &= SWAPWORD(~0x0200);
        HW_EscWriteWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
    }
    bSendMbxIsFull         = FALSE;
    bReceiveMbxIsLocked = FALSE;
    u8MailboxSendReqStored    = 0;
    u8MbxWriteCounter         = 0;
    u8MbxReadCounter        = 0;

    do
    {
        pMbx = GetOutOfMbxQueue(&sMbxReceiveQueue);
        if (pMbx)
        {
            APPL_FreeMailboxBuffer(pMbx);
        }
    } while (pMbx != NULL);
    
    do
    {
        pMbx = GetOutOfMbxQueue(&sMbxSendQueue);
        if (pMbx)
        {
            APPL_FreeMailboxBuffer(pMbx);
        }
    } while (pMbx != NULL);

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx        Pointer to the received Mailbox command from Master.

 \brief    The function checks the mailbox header for the requested service and calls the
 \brief    corresponding XXXX_ServiceInd-function
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 MailboxServiceInd(TMBX MBXMEM *pMbx)
{
    UINT8 result;


    switch ( (pMbx->MbxHeader.Flags[MBX_OFFS_TYPE] & MBX_MASK_TYPE) >> MBX_SHIFT_TYPE )
    {
    case MBX_TYPE_COE:
        /* CoE datagram received */
        result = COE_ServiceInd((TCOEMBX MBXMEM *) pMbx);
        break;

    default:

        result = MBXERR_UNSUPPORTEDPROTOCOL;
        break;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx        Pointer to the received Mailbox command from Master.

 \brief    This function is called when the Master has written the Receive-Mailbox.
 \brief    It will only be called if the send mailbox is empty, that a response for the
 \brief    mailbox service could be stored.
 \brief    The function checks the mailbox header for the requested service and calls the
 \brief    corresponding XXXX_ServiceInd-function
*////////////////////////////////////////////////////////////////////////////////////////

void MBX_MailboxWriteInd(TMBX MBXMEM *pMbx)
{
    UINT8 result = 0;
    UINT8 mbxCounter = pMbx->MbxHeader.Flags[MBX_OFFS_COUNTER] >> MBX_SHIFT_COUNTER;
    UINT16 MbxLen = SWAPWORD(pMbx->MbxHeader.Length);

    if(MbxLen > MAX_MBX_SIZE)
    {
        /* Mailbox error response: size specified in mailbox header too large*/
        pMbx->MbxHeader.Length     = 4;
        pMbx->MbxHeader.Flags[MBX_OFFS_TYPE]        &= ~MBX_MASK_TYPE;
        pMbx->Data[0]                        = SWAPWORD(MBXSERVICE_MBXERRORCMD);
        pMbx->Data[1]                        = SWAPWORD(MBXERR_INVALIDSIZE);
        MBX_MailboxSendReq(pMbx, 0);
    }
    else
    /* if the mailbox datagram counter (Bit 4-6 of Byte 5 of the mailbox header) is unequal zero,
       the master supports the mailbox data link layer,
        in that case a repeated mailbox write request will be detected, if the counter is unequal zero
        and unchanged */
    if ( mbxCounter == 0 || mbxCounter != u8MbxWriteCounter )
    {
        /* new mailbox service received */
        /* mbxCounter = 0: old EtherCAT master */
        /* new MBX service received, store the new mailbox counter */
        u8MbxWriteCounter = mbxCounter;

        {
            /* check the protocol type and call the XXXX_ServiceInd-function */

            result = PutInMbxQueue(pMbx, &sMbxReceiveQueue);
        }

        if ( result != 0 )
        {
            /* Mailbox error response: type 0 (mailbox service protocol) */
            pMbx->MbxHeader.Length     = 4;
            pMbx->MbxHeader.Flags[MBX_OFFS_TYPE]        &= ~MBX_MASK_TYPE;
            pMbx->Data[0]                        = SWAPWORD(MBXSERVICE_MBXERRORCMD);
            pMbx->Data[1]                        = SWAPWORD(result);
            MBX_MailboxSendReq(pMbx, 0);
        }
    }
    else
    {
        // the mailbox buffer has to be freed here
        APPL_FreeMailboxBuffer(pMbx);
        pMbx = NULL;

    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief This function is called when the Master has read the Send-Mailbox.
*////////////////////////////////////////////////////////////////////////////////////////

void MBX_MailboxReadInd(void)
{
    bSendMbxIsFull = FALSE;

    // HBu 02.05.06: the pointer psRepeatMbx is only free if there is no stored
    //               mailbox service from the last repeat
    if (psRepeatMbx && psStoreMbx == NULL)
    {
        /* the last sent service is not stored for repeat any longer */
/*ECATCHANGE_START(V5.13) MBX3*/
        if (psReadMbx != psRepeatMbx)
        {
            APPL_FreeMailboxBuffer(psRepeatMbx);
            psRepeatMbx = NULL;
        }
/*ECATCHANGE_END(V5.13) MBX3*/

    }

    /* the actual sent service has to be stored for repeat */
    psRepeatMbx = psReadMbx;

      if ( psStoreMbx )
      {
        /* there was a buffer stored */
        MBX_CopyToSendMailbox(psStoreMbx);
        /* no more buffer to be stored any more */
        psStoreMbx = NULL;
      }
      else
    {
        TMBX MBXMEM* pMbx = GetOutOfMbxQueue(&sMbxSendQueue);

        
        if (pMbx)
        {
            MBX_CopyToSendMailbox(pMbx);
        }
    }

      if ( u8MailboxSendReqStored )
    {
        /* there are mailbox services stored to be sent */
        if ( u8MailboxSendReqStored & COE_SERVICE )
        {
           UINT8 result = 0;
            /* reset the flag indicating that CoE service to be sent was stored */
            u8MailboxSendReqStored &= ~COE_SERVICE;

            /* call CoE function that will send the stored CoE service */
            result = COE_ContinueInd(psWriteMbx);

            if (result != 0)
            {
                /*Set the pending CoE indication is an error occurred during the continue indication*/
                u8MailboxSendReqStored |= COE_SERVICE;
            }
        }
        else
/*ECATCHANGE_START(V5.13) EOE1*/
/*pending EoE commands are handled from the MBX_Main function*/
/*ECATCHANGE_END(V5.13) EOE1*/

        {
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief This function is called if the Master has requested a resending of the last
 \brief sent mailbox
*////////////////////////////////////////////////////////////////////////////////////////

void MBX_MailboxRepeatReq(void)
{
    if (psRepeatMbx)
    {
        TMBX MBXMEM *pMbx = psRepeatMbx;
        
        /* send mailbox service stored for repeat */
        /* HBu 13.10.06: if a repeat request is received (again) before the previously repeated mailbox telegram
           was read from the master (psStoreMbx != NULL) the next mailbox telegram to be sent is still in the
            read mailbox so it has not to updated exchanged */
        ENTER_MBX_CRITICAL;

       if (bSendMbxIsFull && psStoreMbx == NULL)
        {
            /* mailbox is full, take the buffer off */
            DisableSyncManChannel(MAILBOX_READ);

            /* store the buffer to be sent next */
            psStoreMbx = psReadMbx;
            /* enable the mailbox again */
            EnableSyncManChannel(MAILBOX_READ);

            /* HBu 15.02.06: flag has to be reset otherwise the mailbox service
                             will not be copied by MBX_CopyToSendMailbox */
            bSendMbxIsFull = FALSE;
        }


        MBX_CopyToSendMailbox(pMbx);
        // HBu 17.06.06: psRepeatMbx has to be set to 0, when it was repeated, otherwise it would be returned twice
        // to the empty queue (MAILBOX_QUEUE=1) or a buffer get lost, if the the next repeat request will happen before
        // the repeated buffer was read
        psRepeatMbx = NULL;
        LEAVE_MBX_CRITICAL;
    }

    // Repeat was finished, toggle the acknowledge bit
    bMbxRepeatToggle = !bMbxRepeatToggle;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pMbx            Pointer to a Mailbox command to be sent (read by the Master)
 \param flags            Bit 0-6:    mailbox protocol type:
                                        0x01 - emergency service
                                        0x02 - CoE service
                                        0x04 - SoE service
                                        0x80 - EoE service
                                        0x10 - AoE service
                                        0x20 - VoE service
                             Bit 7:   0 - no more fragments to be sent for the requested mailbox service
                                       1 - additional fragments to be sent for the mailbox service, the
                                            corresponding XXXX_ContinueInd-function will be called to get
                                            the next fragment

 \return    0: Success - mailbox command could be stored in the send mailbox
            1: Failed - mailbox command could not be stored in the send mailbox, the
                            XXXX_ContinueInd service will be called when the mailbox was
                            read from the master to

 \brief        This function puts a new Mailbox service in the Send Mailbox
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 MBX_MailboxSendReq( TMBX MBXMEM * pMbx, UINT8 flags )
{
    UINT8 result = 0;

    /* HBu 06.02.06: in INIT-state a mailbox send request shall be refused */
    if ( (nAlStatus & STATE_MASK) == STATE_INIT )
    {
        return ERROR_INVALIDSTATE;
    }

    

    ENTER_MBX_CRITICAL;

    /* the counter in the mailbox header has to be incremented with every new mailbox service to be sent
       if the mailbox data link layer is supported (software switch MAILBOX_REPEAT_SUPPORTED set)*/
    pMbx->MbxHeader.Flags[MBX_OFFS_COUNTER] &= ~MBX_MASK_COUNTER;
    /* HBu 13.02.06: Repeat-Counter was incremented too much if the mailbox service could not be sent */
    /* u8MbxCounter holds the actual counter for the mailbox header, only the values
       1-7 are allowed if the mailbox data link layer is supported  */
    if ( (u8MbxReadCounter & 0x07) == 0 )
    {
        u8MbxReadCounter = 1;
    }

    pMbx->MbxHeader.Flags[MBX_OFFS_COUNTER] |= u8MbxReadCounter << MBX_SHIFT_COUNTER;

    /* try to copy the mailbox command in the ESC */
    if ( MBX_CopyToSendMailbox(pMbx) != 0 )
    {
        /* no success, send mailbox was full, set flag  */
        result = PutInMbxQueue(pMbx, &sMbxSendQueue);
        if (result != 0)
        {
            flags |= FRAGMENTS_FOLLOW;
        }
        else
        {
            u8MbxReadCounter++;
        }
    }
    /* HBu 13.02.06: Repeat-Counter was incremented too much if the mailbox service could not be sent */
    else
    {
        u8MbxReadCounter++;
    }

    if ( flags & FRAGMENTS_FOLLOW )
    {
        /* store the mailbox service that the corresponding XXX_ContinueInd function will
           be called when the send mailbox will have been read by the master because there
           are mailbox commands to be sent for this service */
        u8MailboxSendReqStored |= (flags & ((UINT8) ~FRAGMENTS_FOLLOW));
    }

    LEAVE_MBX_CRITICAL;

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function is used to check if the received mailbox command can be processed.

        Also the contents of the Receive Mailbox will be copied in the variable sMbx.
*////////////////////////////////////////////////////////////////////////////////////////

void MBX_CheckAndCopyMailbox( void )
{
    UINT16 mbxLen;

    /* get the size of the received mailbox command and acknowledge the event*/
    HW_EscReadWord(mbxLen,u16EscAddrReceiveMbx);
    
    /* the size has to be swapped here, all other bytes of the mailbox service will be swapped later */
    mbxLen = SWAPWORD(mbxLen);

    if(bNoMbxMemoryAvailable == TRUE)
    {
        /* Return a no memory error in case of any mailbox request*/
        TMBX MBXMEM *pMbx = (TMBX MBXMEM *) APPL_AllocMailboxBuffer(10); /* a mailbox error datagram length*/

        if(pMbx != NULL)
        {
            HMEMSET(pMbx,0x00,10);

            /* Mailbox error response: type 0 (mailbox service protocol) */
            pMbx->MbxHeader.Length     = 4;
            pMbx->MbxHeader.Flags[MBX_OFFS_TYPE]        &= ~(MBX_MASK_TYPE);
            pMbx->Data[0]                        = SWAPWORD(MBXSERVICE_MBXERRORCMD);
            pMbx->Data[1]                        = SWAPWORD(MBXERR_NOMOREMEMORY);
            MBX_MailboxSendReq(pMbx, 0);

            APPL_FreeMailboxBuffer(pMbx);
        }
    }
    /* the length of the mailbox data is in the first two bytes of the mailbox,
       so the length of the mailbox header has to be added */
    mbxLen += MBX_HEADER_SIZE;

    /* in this example there are only two mailbox buffers available in the firmware (one for processing and
       one to stored the last sent response for a possible repeat request), so a
       received mailbox service can only be processed if a free buffer is available */
    if ( ( bSendMbxIsFull )                /* a received mailbox service will not be processed
                                                    as long as the send mailbox is still full
                                                    (waits to be read from the master) */
        ||( u8MailboxSendReqStored )    /* a mailbox service to be sent is still stored
                                                    so the received mailbox service will not be processed
                                                    until all stored mailbox services are sent */
        )
    {
        /* set flag that the processing of the mailbox service will be checked in the
            function MBX_Main (called from ECAT_Main) */
        bReceiveMbxIsLocked = TRUE;
    }
    else
    {
        /* received mailbox command can be processed, reset flag */
        bReceiveMbxIsLocked = FALSE;

        /* if the read mailbox size is too big for the buffer, set the copy size to the maximum buffer size, otherwise
           memory could be overwritten,
           the evaluation of the mailbox size will be done in the mailbox protocols called from MBX_WriteMailboxInd */

        if (mbxLen > u16ReceiveMbxSize)
        {
            mbxLen = u16ReceiveMbxSize;
        }

        psWriteMbx = (TMBX MBXMEM *) APPL_AllocMailboxBuffer(u16ReceiveMbxSize);

        /* if there is no more memory for mailbox buffer, the mailbox should not be read */
        if (psWriteMbx == NULL)
        {
            /* set flag that the processing of the mailbox service will be checked in the
                function MBX_Main (called from ECAT_Main) */
            bReceiveMbxIsLocked = TRUE;
            return;
        }
        /* copy the mailbox header and data*/
        HW_EscReadMbxMem((MEM_ADDR MBXMEM *) psWriteMbx,u16EscAddrReceiveMbx,mbxLen);


        {
        /*Read Control and Status of SyncManager 0 to check if the buffer is unlocked*/
        VARVOLATILE UINT16 smstate = 0x00;
        HW_EscReadWord(smstate,ESC_SYNCMAN_CONTROL_OFFSET);
        smstate = SWAPWORD(smstate);

        if(smstate & SM_STATUS_MBX_BUFFER_FULL)
        {
            /*Unlock the mailbox SyncManger buffer*/
            u16dummy = 0;
            HW_EscReadWord(u16dummy,(u16EscAddrReceiveMbx + u16ReceiveMbxSize - 2));

        }

        }

        /* in MBX_MailboxWriteInd the mailbox protocol will be processed */
        MBX_MailboxWriteInd( psWriteMbx );

    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx        Pointer to the Mailbox command to be send to the master.

 \brief    This function copies data to the Send Mailbox.
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 MBX_CopyToSendMailbox( TMBX MBXMEM *pMbx )
{
    if ( (nAlStatus & STATE_MASK) == STATE_INIT)
    {
        /* the mailbox is disabled if the slave is in the INIT state */
        return( ERROR_INVALIDSTATE );
    }


    if ( !bMbxRunning )
    {
        /* the mailbox is disabled if the slave is in the INIT state */
        return( ERROR_INVALIDSTATE );
    }

    if ( bSendMbxIsFull )
    {
        /* mailbox service cannot be sent because the send mailbox is still full */
        return MBXERR_NOMOREMEMORY;
    }
    else
    {
        /* the variable mbxSize contains the size of the mailbox data to be sent */
        UINT16 mbxSize = pMbx->MbxHeader.Length;
/*ECATCHANGE_START(V5.13) MBX2*/
        /*Reset the not used mailbox memory*/
        {
            UINT16 LastUsedAddr = u16EscAddrSendMbx + mbxSize + MBX_HEADER_SIZE;
            UINT16 LastAddrToReset = (u16EscAddrSendMbx + u16SendMbxSize);
            /*round down to last even 16bit address*/
            LastUsedAddr = LastUsedAddr & 0xFFFE;
            LastAddrToReset = (LastAddrToReset - 2) & 0xFFFE;
            u16dummy = 0;

            /*clear all unused bytes*/
            while (LastUsedAddr < LastAddrToReset) /*reset all bytes until the second last valid address*/
            {
                HW_EscWriteWord(u16dummy, LastUsedAddr);
                LastUsedAddr = LastUsedAddr + 2;
            }
        }
/*ECATCHANGE_END(V5.13) MBX2*/

        HW_EscWriteMbxMem((MEM_ADDR *)pMbx, u16EscAddrSendMbx, (mbxSize + MBX_HEADER_SIZE));


        {
        /*Read Control and Status of SyncManager 1 to check if the buffer is still marked as empty*/
        VARVOLATILE UINT16 smstate = 0x00;
        HW_EscReadWord(smstate,(ESC_SYNCMAN_CONTROL_OFFSET + SIZEOF_SM_REGISTER));
        smstate = SWAPWORD(smstate);

        if(!(smstate & SM_STATUS_MBX_BUFFER_FULL))
        {
            
            UINT16 BytesLeft = u16SendMbxSize - (mbxSize + MBX_HEADER_SIZE);

            /*Write last Byte to trigger mailbox full flag*/
            /*Read last 2 Bytes and write them again (required if low Byte of the WORD were written before)*/
            u16dummy = 0;
            if(BytesLeft < 2)
            {
                /*The last 2Bytes are overlapping the already written buffer*/

                /*Get the valid 16Bit address*/
                UINT32 LastDataAddress = ((mbxSize + MBX_HEADER_SIZE)/2);
            
                /*Copy the buffer to overwrite*/
                MEMCPY((UINT16 *)&u16dummy,(((UINT16 *)pMbx) + LastDataAddress),(2 - BytesLeft));
            }

            HW_EscWriteWord(u16dummy, (u16EscAddrSendMbx + u16SendMbxSize - 2));
        }
        }

        /* store last send mailbox service for a possible repeat
            one buffer includes the last send service (psRepeatMbx),
            the other one the actual service to be sent (psReadMbx),
            there is no buffer available for a mailbox receive service
            until the last sent buffer was read from the master
            the exception is after the INIT2PREOP transition, in that
            case there is no last sent service (psReadMbx = 0) */
        if ( psReadMbx )
        {
            psWriteMbx = NULL;
        }
        psReadMbx = pMbx;

        /* set flag that send mailbox is full now */
        bSendMbxIsFull = TRUE;


        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function is called cyclically to check if a received Mailbox service was
             stored.
*////////////////////////////////////////////////////////////////////////////////////////

void MBX_Main(void)
{
    TMBX MBXMEM *pMbx = NULL;

    do
    {
        UINT8 result = 0;

        pMbx = GetOutOfMbxQueue(&sMbxReceiveQueue);
        if ( pMbx )
        {
            result = MailboxServiceInd(pMbx);
        }

        if ( result != 0 )
        {
            /* Mailbox error response: type 0 (mailbox service protocol) */
            pMbx->MbxHeader.Length     = 4;
            pMbx->MbxHeader.Flags[MBX_OFFS_TYPE]        &= ~(MBX_MASK_TYPE);
            pMbx->Data[0]                        = SWAPWORD(MBXSERVICE_MBXERRORCMD);
            pMbx->Data[1]                        = SWAPWORD(result);
            MBX_MailboxSendReq(pMbx, 0);
        }
    }
    while ( pMbx != NULL );


    if (bReceiveMbxIsLocked)
    {
        /* the work on the receive mailbox is locked, check if it can be unlocked (if all
           mailbox commands has been sent */
        MBX_CheckAndCopyMailbox();
    }

    /*ECATCHANGE_START(V5.13) EOE1*/
    /*Try to send pending Mailbox data,
       could be to pending previously due to local (memory) limitations which are no available*/
    if (u8MailboxSendReqStored)
    {
        /* there are mailbox services stored to be sent */
    }
/*ECATCHANGE_END(V5.13) EOE1*/
}

/** @} */


