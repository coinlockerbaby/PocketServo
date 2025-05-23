/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup ESM EtherCAT State Machine
@{
*/

/**
\file ecatslv.c
\author EthercatSSC@beckhoff.com
\brief Implementation
This file contains the EtherCAT State Machine.

\version 5.13

<br>Changes to version V5.12:<br>
V5.13 BOOT1: support Init-to-Init transition in bootloader application<br>
V5.13 CIA402 3: change define "CIA402_DEVICE" to "CiA402_SAMPLE_APPLICATION"<br>
V5.13 CIA402 4: decouple CIA402 state machine and application from ESM (according ETG.6010, clause 4)<br>
V5.13 ECAT1: handle Sync mapped to AL Event<br>
V5.13 ECAT2: explicit device ID handling, the ID value shall only be latched on the rising edge of 0x120.5<br>
V5.13 ECAT3: reset local Error flag in case of two consecutive pending state response and the the first failes<br>
V5.13 ESM1: local error handling update, ECAT_StateChange triggers only transitions from Op->Any or reject/accept a pending transition<br>
V5.13 ESM2: support ErrorSafeOP to OP transition<br>
V5.13 ESM3: Safe-to-OP transition in DC mode, ack OP state if no error was detected<br>
V5.13 ESM4: implement disable sync error reaction 0x10F1.2 is set to 0<br>
V5.13 MBX1: change mbx_read flag handling to SM1 buffer state handling (required in case of a mbx read frame with an invalid CRC, the read flag would be set but the SM bufer is still locked)<br>
<br>Changes to version V5.11:<br>
V5.12 BOOT1: add a bootloader sample application (only the ESM and FoE is supported)<br>
V5.12 ECAT1: update SM Parameter measurement (based on the system time), enhancement for input only devices and no mailbox support, use only 16Bit pointer in process data length caluclation<br>
V5.12 ECAT4: update Sync1 watchdog calculation (in case of subordinated cycles take one addiitonal Sync0 cycle into account )<br>
V5.12 ECAT5: update Sync error counter/flag handling,check enum memory alignment depending on the processor,in case of a polled timer disable ESC interrupts during DC_CheckWatchdog<br>
V5.12 ECAT7: set error single flash also in case of an application error<br>
V5.12 ESM1: overwrite the current error in case of a local error with a lower target state,Do not overwrite the current AL Status in case of an local error<br>
V5.12 ESM2: enable the PD SM in case of a clear error transition<br>
V5.12 ESM3: set internal ESM timeout to -10% of the configured value (to return an errorcode before the master will run into an timeout)<br>
V5.12 ESM4: enable the AL Event mask in case of pending ESM transition<br>
V5.12 TEST2: add pending ESM test,trigger complete ESM transition from ecat main<br>
<br>Changes to version V5.10:<br>
V5.11 COE3: change 0x10F3.2 (Sync Error limit) from UINT32 to UINT16 (according to the ETG.1020)<br>
V5.11 DIAG4: change parameter handling in DIAG_CreateNewMessage()<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 ECAT4: enhance SM/Sync monitoring for input/output only slaves<br>
V5.11 ECAT5: "Add missing ""bEscIntEnabled"" initialization if ""AL_EVENT_ENBALED"" is 0"""<br>
V5.11 ECAT7: add missing big endian swapping<br>
V5.11 ESC1: update max address calculation<br>
V5.11 ESM1: update calculation of subordinated cycles<br>
V5.11 ESM2: DC_SUPPORTED, Sync0 is not supported and Sync0 is generated according register values the state transition to SafeOP shall be rejected<br>
V5.11 ESM3: update checking of the user configured sync type<br>
V5.11 ESM4: prevent to go from ErrSafeOP to OP without re enabling Sync0/1<br>
V5.11 ESM5: DPRAM range was double checked<br>
V5.11 ESM6: in the SO transition wait by default until the master has send process data<br>
V5.11 HW1: "move hardware independent functions ""HW_DisableSyncManChannel()"", ""HW_EnableSyncManChannel()"", ""HW_GetSyncMan()"", ""HW_ResetALEventMask()"", ""HW_SetALEventMask()"" to ecatalv.c"<br>
V5.11 HW2: check during ESM handling if the SM address and length is aligned according the ESC access<br>
V5.11 TEST9: "add behaviour 0x2020.7 (SDO requests on 0x3006.0 are set to pending until an FoE read request on ""UnlockSdoResp"" is received or in case that no mbx queue is supported when a new mbx request was received)"<br>
<br>Changes to version V5.01:<br>
V5.10 COE1: Define one entry description for all 0x1C3x objects and change data type of SI11,12,13 to UINT16 (according ETG.1020)<br>
V5.10 DIAG1: Define diagmessage textIDs<br>
V5.10 ECAT13: Update Synchronisation handling (FreeRun,SM Sync, Sync0, Sync1)<br>
              Compare DC UINT configuration (by ESC Config data) vs. DC activation register settings<br>
              Update 0x1C3x entries<br>
V5.10 ESC2: Check if defined SM settings do not exceed the available DPRAM range (in error case AL Status 0x14 is returned)<br>
V5.10 ESC3: Handle DC cControl register values in case of 32Bit ESC access (a Sync activation mask need to defined/used)<br>
V5.10 ESC4: Mask lower 4 Bit of AL status to get Run led value<br>
            Invalid RunLed code was calculated if ESC set ECAT Run Led<br>
V5.10 ESC5: Add missing swapping<br>
V5.10 ESM2: Update "bApplEsmPending" flag during a transition to a lower state<br>
V5.10 ESM3: Add "volatile" directive for ESM dummy variables<br>
V5.10 HW5: Block ESC interrupts during Timer ISR<br>
V5.10 TEST9: Add option to prevent SM3 unlock during PS<br>
<br>Changes to version V5.0:<br>
V5.01 APPL3: Include library demo application<br>
V5.01 ESC2: Add missed value swapping<br>
V5.01 ESM1: Don't overwrite the error reason in case of an failed PS transition<br>
V5.01 ESM2: Don't check the "appl trigger" flag in case on an regular transition to a lower state (OS, SP, PI).<br>
V5.01 ESM3: Call Error acknowledge indication only if error was acknowledged by the master<br>
V5.01 HW3: Update blink code of an SM watchdog error<br>
<br>Changes to version V4.42:<br>
V5.0 ECAT1: Support Explicit Device ID.<br>
V5.0 ECAT2: Application specific functions are moved to application files.<br>
V5.0 ECAT3: Global dummy variables used for dummy ESC operations.<br>
V5.0 ESC1: ESC 32Bit Access added.<br>
V5.0 ESC2: Support ESC EtherCAT LED Indication.<br>
V5.0 ESC3: Support EEPROM Emulation.<br>
V5.0 ESM1: Update "LocalErrorFlag" handling.<br>
V5.0 ESM2: Update Error Acknowledge by ALControl INIT (without error acknowledge)<br>
V5.0 ESM3: Handle pending ESM transition<br>
V5.0 ESM4: ECAT_StateChange() will only be called form application. In case of an communication error AL_ControlInd is called.<br>
V5.0 MBX1: Support configuration without mailbox protocol support.<br>
V5.0 TEST1: Add test application. See Application Note ET9300 for more details.<br>
<br>Changes to version V4.40:<br>
V4.42 ESM1: Reset local error flag if master set the acknowledge bit (0x120.4)<br>
<br>Changes to version V4.30:<br>
V4.40 ESM5: Enable output SyncManager if local error acknowledged<br>
V4.40 HW0: Use common hardware access functions<br>
V4.40 PDO3: Add support if only input process data is used<br>
V4.40 ECAT4: Add read SM activation register to acknowledge SM Change event<br>
V4.40 PDO2: Check if max process data size was exceed<br>
V4.40 DIAG1: add diagnosis message support<br>
V4.40 ESM4: Change Check WD setup; add define OP_PD_REQUIRED (defines if process data required in state change to OP)<br>
V4.40 WD1: change WD behaviour depending if process data required in OP state<br>
V4.40 MBX4: Change processing order of mailbox SyncManager flags<br>
V4.40 ECAT1: Merge content of HW_Main (spihw.c /mcihw.c) to ECAT_Main<br>
V4.40 ECAT2: Added CheckIfLocalError() to check local flags and set ALStatus /Al Status code if required. This function is called cyclic from MainLoop.<br>
V4.40 ESM2: Add AL_ControlRes() to complete pending state requests. Change SafeOP to OP state response<br>
V4.40 ESM1: Prevent double call of StopOutputHandler()<br>
V4.40 BOOT1: Enable Mailbox SyncManger on state change to BOOT state (to enable FoE)<br>
V4.40 ESM3: Change State machine behaviour according to ETG.1000 V1.0.2 (state change #26)<br>
V4.40 LED1: Set error blink code<br>
V4.40 TIMER1: Added DC_CheckWatchdog() triggered from ECAT_CheckTimer(). Change local Sync0 watchdog variables. Change bus cycle calculation<br>
V4.40 WD1: Change check process data watchdog settings<br>
<br>Changes to version V4.20:<br>
V4.30 OBJ 3: initialize the object dictionary in state change INIT->PREOP; clear object dictionary in state change PREOP->INIT<br>
V4.30 SYNC: add 0x1C32:10; 0x1C33:10 (Sync0 cycle), change synchronisation control functionality<br>
V4.30 CiA402: add CiA402_Init() call in state change from PREOP to SAFEOP if DC synchronisation is enabled,<br>
                   else the Init function is called when bus cycle time is calculated [CalcSMCycleTime() ].<br>
                   trigger error handling if the EtherCAT state machine gets a transition from OP to an "lower" state<br>
V4.20 ECAT 1: add LEGACY_MODE behaviour in ECAT_CheckWatchdog()<br>
V4.20 DC 1: Add DC pending state machine handling and Dc watchdog functionality<br>
V4.20 ESM 2: Add State transition from BOOT to INIT<br>
V4.20 ESM 1: Non LEGACY_MODE State change handling<br>
V4.11 Renamed the function parameter "code" of Function "SendSmFailedEmergency() to avoid<br>
problems with some compilers"<br>
V4.11 ECAT 1: Fixed a possible problem with state change Init -> SafeOP. The output syncmanager<br>
was enabled by the state change-flag and not by the actual state<br>
V4.11 LED 1: Clear the error LED during error acknowledgement<br>
V4.11 ESC 1: fixed size of MBXHEADER in the TFOEMBX struct <br>
<br>Changes to version V4.08:<br>
V4.10 ECAT 1: clear bEcatOutputsReceived in startMailboxhandler()<br>
V4.10 ECAT 2: clear bEcatOutputsReceived in stopMailboxhandler()<br>
V4.10 ECAT 3: when switching from INIT to BOOT the SM settings shall be checked<br>
V4.10 ECAT 4: APPL_StartInputHandler shall always be called and bEcatInputUpdateRunning shall always be set<br>
              in StartInputHandler independent of the input size<br>
V4.10 ECAT 5: AL_ControlInd: the error acknowledge behaviour was changed<br>
              according to the protocol enhancements and the conformance test<br>
V4.10 ECAT 6: AL_ControlInd: if a state transitions failed the corresponding stop function is<br>
              called to get a consistent set of variables<br>
V4.10 ECAT 7: the local application requested to leave the state OP so we have to disable the SM2<br>
                   and make the state change from OP to SAFEOP by calling StopOutputHandler<br>
V4.10 ECAT 8: the AL Status Code has to be reset if the error was acknowledged by the master<br>
V4.10 ECAT 9: ECAT_StateChange: when waiting for a State Change response from the application the<br>
              AL Status shall only be written if the final state was reached<br>
<br>Changes to version V4.07:<br>
V4.08 ECAT 1: The watchdog value was not rounded up<br>
V4.08 ECAT 2: The value of u16WdValue was not set 0 if the register 0x420 is 0<br>
V4.08 ECAT 3: The AlStatusCode is changed as parameter of the function AL_ControlInd<br>
V4.08 ECAT 4: In a state transition OP2PREOP, SAFEOP2INIT or OP2INIT is requested,<br>
              this was not working correctly if one of the application functions<br>
              APPL_StopInputHandler or APPL_StopOutputHandler were returning NOERROR_INWORK<br>
              (because only the first state transition was made in that case)<br>
V4.08 AOE 1:    AoE was added<br>
<br>Changes to version V4.06:<br>
V4.07 ECAT 1: The sources for SPI and MCI were merged (in ecat_def.h<br>
                   set the switch MCI_HW to 1 when using the MCI,<br>
                   set the switch SPI_HW to 1 when using the SPI<br>
<br>Changes to version V4.00:<br>
V4.01 ECAT 1: The Output sync Manager was not disabled when the state OP was left<br>
              by a local request (watchdog or io error)<br>
V4.01 ECAT 2: APPL_StopOutputHandler returns an UINT16<br>
V4.01 ECAT 3: TwinCAT compatibility mode: The state transition to OP is allowed when the<br>
                    WD-Trigger-Bit of the SM2-Control-Byte (0x814.6) is FALSE, in that case the<br>
                    watchdog will not be started before the outputs were received the first time<br>
V4.01 ECAT 4: "else" was too much<br>
<br>Changes to version V3.20:<br>
V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to<br>
              the EtherCAT Guidelines and Protocol Enhancements Specification<br>
V4.00 ECAT 2: The output sync manager is initialized during the state transition<br>
              from PREOP to SAFEOP that the master can check if the slave could update<br>
              inputs and outputs before switching the slave to OP<br>
              behaviour according to the EtherCAT Guidelines and Protocol Enhancements Specification<br>
V4.00 ECAT 3: The watchdog will be enabled in SAFE-OP that it can be checked if the last SM event<br>
              was received during the watchdog time before switching to OP<br>
V4.00 ECAT 4: The function CheckSmChannelParameters is included in the function<br>
              CheckSmSettings to get a better overview<br>
V4.00 ECAT 5: In synchronous mode the slave should support 1- and 3-buffer mode, 3-buffer mode<br>
              should be the standard setting, because the controlling if the process data was updated<br>
              should be done with the TxPDO Toggle, but the 1-buffer mode should be setable too,<br>
              that the master could easily check if all slaves are synchronous by checking the<br>
              the working counter (if the outputs were not read or the inputs were not written<br>
              the ESC of the slave would not increment the working counter with expected value<br>
              if the 1-buffer mode is running)<br>
V4.00 ECAT 6: The function ECAT_StateChange was added, which the application should call if a local error<br>
                   is detected (with the parameters alStatus = STATE_SAFEOP, alStatusCode = error code (> 0x1000))<br>
                   or gone (with the parameters alStatus = STATE_OP, alStatusCode = 0)<br>
                   or if one of the functions APPL_StartMailboxHandler, APPL_StopMailboxHandler, APPL_StartInputHandler,<br>
                   APPL_StopInputHandler, APPL_StartOutputHandler, APPL_StopOutputHandler has returned NOERROR_INWORK<br>
                   to acknowledge the last state transition (with the parameters alStatus = new AL-Status, alStatusCode =<br>
                   new AL-Status-Code)<br>
V4.00 ECAT 7: The return values for the AL-StatusCode were changed to UINT16
*/

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#define    _ECATSLV_    1
#include "ecatslv.h"
#undef    _ECATSLV_
/*remove definition of _ECATSLV_ (#ifdef is used in ecatslv.h)*/

#include "ecatappl.h"





#include "mailbox.h"

#include "ecatcoe.h"
#include "objdef.h"



/*ECATCHANGE_START(V5.13) CIA402 3*/
#include "DPort-ECT.h"

/*--------------------------------------------------------------------------------------
------
------    local Types and Defines
------
--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
UINT16    u16ALEventMask;                      // Value which will be written to the 0x204 register (AL event mask) during the state transition PreOP to SafeOP
/*ECATCHANGE_START(V5.13) ECAT2*/
UINT16	  u16IdValue;						   /**< \brief Explicit Device ID value of the latest ID Request by the master*/
/*ECATCHANGE_END(V5.13) ECAT2*/

/*Dummy variable to trigger read or writes events in the ESC*/
    VARVOLATILE UINT16    u16dummy;


        VARVOLATILE UINT16 SMActivate = 0;

TSYNCMAN		SyncManInfo;

//indicates if the EEPORM was loaded correct
BOOL EepromLoaded = FALSE;


/*-----------------------------------------------------------------------------------------
------
------    local functions
------
-----------------------------------------------------------------------------------------*/
/*ECATCHANGE_START(V5.13) ECAT1*/
/*ECATCHANGE_END(V5.13) ECAT1*/
void ResetALEventMask(UINT16 intMask);

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    intMask        interrupt mask (disabled interrupt shall be zero)

 \brief    This function makes an logical and with the AL Event Mask register (0x204)
*////////////////////////////////////////////////////////////////////////////////////////
void ResetALEventMask(UINT16 intMask)
{
    UINT16 mask;
    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);
    
    mask &= intMask;


    DISABLE_ESC_INT();


    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    ENABLE_ESC_INT();
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    intMask        interrupt mask (enabled interrupt shall be one)

  \brief    This function makes an logical or with the AL Event Mask register (0x204)
*////////////////////////////////////////////////////////////////////////////////////////
void SetALEventMask(UINT16 intMask)
{
    UINT16 mask;
    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);
    

    mask |= intMask;

    DISABLE_ESC_INT();


    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    ENABLE_ESC_INT();
}


/////////////////////////////////////////////////////////////////////////////////////////
/**

\brief    This function reads the EEPROM loaded state
*////////////////////////////////////////////////////////////////////////////////////////
void UpdateEEPROMLoadedState(void)
{
    UINT16 TmpVar = 0;
    //read EEPROM loaded information
    HW_EscReadWord(TmpVar, ESC_EEPROM_CONTROL_OFFSET);
    TmpVar = SWAPWORD(TmpVar);


    if (((TmpVar & ESC_EEPROM_ERROR_CRC) > 0)
        || ((TmpVar & ESC_EEPROM_ERROR_LOAD) > 0))
    {
        EepromLoaded = FALSE;
    }
    else
    {
        EepromLoaded = TRUE;
    }
}


/*-----------------------------------------------------------------------------------------
------
------    functions
------
-----------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \return     pPdSyncMan        Pointer to the settings of requested SYNC Manager channel

 \brief    This function is called to read the SYNC Manager channel descriptions of the
             process data SYNC Managers.
*////////////////////////////////////////////////////////////////////////////////////////

TSYNCMAN ESCMEM * GetSyncMan( UINT8 channel )
{
    HW_EscRead((MEM_ADDR *)&SyncManInfo, ESC_SYNCMAN_REG_OFFSET + (channel * SIZEOF_SM_REGISTER), SIZEOF_SM_REGISTER );



    return &SyncManInfo;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \brief    This function disables a Sync Manager channel
*////////////////////////////////////////////////////////////////////////////////////////
void DisableSyncManChannel(UINT8 channel)
{
    UINT16 Offset;
    //The register 0x806 is only readable from PDI => writing 0 is valid
    VARVOLATILE UINT16 smStatus = SM_SETTING_PDI_DISABLE;
    Offset = (ESC_SYNCMAN_ACTIVE_OFFSET + (SIZEOF_SM_REGISTER*channel));


    HW_EscWriteWord(smStatus,Offset);
    
    /*wait until SyncManager is disabled*/
    do
    {
        HW_EscReadWord(smStatus, Offset);
    }while(!(smStatus & SM_SETTING_PDI_DISABLE));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \brief    This function enables a Sync Manager channel
*////////////////////////////////////////////////////////////////////////////////////////
void EnableSyncManChannel(UINT8 channel)
{
    UINT16 Offset;
    //The register 0x806 is only readable from PDI => writing 0 is valid
    VARVOLATILE UINT16 smStatus = 0x0000;
    Offset = (ESC_SYNCMAN_ACTIVE_OFFSET + (SIZEOF_SM_REGISTER*channel));


    HW_EscWriteWord(smStatus,Offset);
    
    /*wait until SyncManager is enabled*/
    do
    {
        HW_EscReadWord(smStatus,Offset);

    }while((smStatus & SM_SETTING_PDI_DISABLE));
}



/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param  maxChannel    last SM channel which should be checked

 \return                 0: okay else AL Status Code

 \brief    This function checks all SM channels

*////////////////////////////////////////////////////////////////////////////////////////

UINT8    CheckSmSettings(UINT8 maxChannel)
{
    UINT8 i;
    UINT8 result = 0;
    TSYNCMAN ESCMEM *pSyncMan;
    UINT16 SMLength = 0;
    UINT16 SMAddress = 0;


        //Check if max address defines are within the available ESC address range
        if ((nMaxEscAddress < MAX_PD_WRITE_ADDRESS)
            || (nMaxEscAddress < MAX_PD_READ_ADDRESS)
            || (nMaxEscAddress < MAX_MBX_WRITE_ADDRESS)
            || (nMaxEscAddress < MAX_MBX_READ_ADDRESS))
        {
            /*The defines for maximum SM addresses are invalid for the used ESC (change the defines in the file ecat_def.h or the SSC Tool)
            It may be also required to adapt the SM settings in the ESI file*/


                return ALSTATUSCODE_NOVALIDFIRMWARE;
        }

    /* check the Sync Manager Parameter for the Receive Mailbox (Sync Manager Channel 0) */
    pSyncMan = GetSyncMan(MAILBOX_WRITE);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;



    if (!(pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE))
    {
        /* receive mailbox is not enabled */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if ((pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) != SM_SETTING_DIRECTION_WRITE_VALUE)
    {
        /* receive mailbox is not writable by the master*/
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if ((pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) != SM_SETTING_MODE_ONE_BUFFER_VALUE)
    {
        /* receive mailbox is not in one buffer mode */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMLength < MIN_MBX_SIZE)
    {
        /* receive mailbox size is too small */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMLength > MAX_MBX_SIZE)
    {
        /* receive mailbox size is too great */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMAddress < MIN_MBX_WRITE_ADDRESS)
    {
        /* receive mailbox address is too small */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMAddress > MAX_MBX_WRITE_ADDRESS)
    {
        /* receive mailbox address is too great */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }


    if ( result == 0 )
    {
        /* check the Sync Manager Parameter for the Send Mailbox (Sync Manager Channel 1) */
        pSyncMan = GetSyncMan(MAILBOX_READ);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;


    if (!(pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE))
    {
        /* send mailbox is not enabled */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if ((pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) != SM_SETTING_DIRECTION_READ_VALUE)
    {
        /* receive mailbox is not readable by the master*/
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if ((pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) != SM_SETTING_MODE_ONE_BUFFER_VALUE)
    {
        /* receive mailbox is not in one buffer mode */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMLength < MIN_MBX_SIZE)
    {
        /* send mailbox size is too small */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMLength > MAX_MBX_SIZE)
    {
        /* send mailbox size is too great */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMAddress < MIN_MBX_READ_ADDRESS)
    {
        /* send mailbox address is too small */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    else if (SMAddress > MAX_MBX_READ_ADDRESS)
    {
        /* send mailbox address is too great */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }
    }

    if ( result == 0 && maxChannel > PROCESS_DATA_IN )
    {
        /* b3BufferMode is only set, if inputs and outputs are running in 3-Buffer-Mode when leaving this function */
        b3BufferMode = TRUE;
        /* check the Sync Manager Parameter for the Inputs (Sync Manager Channel 2 (0 in case if no mailbox is supported)) */
        pSyncMan = GetSyncMan(PROCESS_DATA_IN);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;



    if ((pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE) != 0 && SMLength == 0)
    {
        /* the SM3 size is 0 and the SM3 is active */
        result = SYNCMANCHSETTINGS + 1;
    }
        else if (pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE)
        {
            /* Sync Manager Channel 3 is active, input size has to greater 0 */
                if (SMLength != nPdInputSize || nPdInputSize == 0 || SMLength > MAX_PD_INPUT_SIZE)
                {
                    /* sizes don't match */
                    result = SYNCMANCHSIZE + 1;
                }
                else
                {
                    /* sizes matches */
                    if ((pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) == SM_SETTING_DIRECTION_READ_VALUE)
                    {
                        /* settings match */
                        if (((nAlStatus == STATE_PREOP) && (SMAddress >= MIN_PD_READ_ADDRESS) && (SMAddress <= MAX_PD_READ_ADDRESS))
                            || ((nAlStatus != STATE_PREOP) && (SMAddress == nEscAddrInputData))
                            )
                        {
                            /* addresses match */

                                if ((pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) == SM_SETTING_MODE_ONE_BUFFER_VALUE)
                                {
                                    /* inputs are running in 1-Buffer-Mode, reset flag b3BufferMode */
                                    b3BufferMode = FALSE;
                                }
                        }
                        else
                        {
                            /* input address is out of the allowed area or has changed in SAFEOP or OP */
                            result = SYNCMANCHADDRESS + 1;
                        }
                    }
                    else
                    {
                        /* input settings do not match */
                        result = SYNCMANCHSETTINGS + 1;
                    }
                }
        }
        else if (SMLength != 0 || nPdInputSize != 0)
        {
            /* input size is not zero although the SM3 channel is not enabled */
            result = SYNCMANCHSIZE + 1;
        }



        if ( result != 0 )
        {
            result = ALSTATUSCODE_INVALIDSMINCFG;
        }
    }


//    else
    if (result == 0 && maxChannel > PROCESS_DATA_OUT)
    {
        /* check the Sync Manager Parameter for the Outputs (Sync Manager Channel 2) */
        pSyncMan = GetSyncMan(PROCESS_DATA_OUT);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;



    if ((pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE) != 0 && SMLength == 0)
    {
        /* the SM2 size is 0 and the SM2 is active */
        result = SYNCMANCHSETTINGS + 1;
    }
        else if (pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE)
        {
            /* Sync Manager Channel 2 is active, output size has to greater 0 */
            if ( SMLength == nPdOutputSize && nPdOutputSize != 0 && SMLength <= ((UINT16)MAX_PD_OUTPUT_SIZE))
            {
                /* sizes match */
                if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) == SM_SETTING_DIRECTION_WRITE_VALUE )
                {
                    /* settings match */
                    if ( ( ( nAlStatus == STATE_PREOP )&&( SMAddress >= MIN_PD_WRITE_ADDRESS )&&( SMAddress <= MAX_PD_WRITE_ADDRESS ) )
                       ||( ( nAlStatus != STATE_PREOP )&&( SMAddress == nEscAddrOutputData ) )
                        )
                    {
                        /* addresses match */
                        {
                            /* check, if watchdog trigger is enabled */
                            if (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_WATCHDOG_VALUE)
                            {
                                bWdTrigger = TRUE;
                            }
                            else
                            {
                                bWdTrigger = FALSE;
                            }

                            if ((pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) == SM_SETTING_MODE_ONE_BUFFER_VALUE)
                            {
                                /* outputs are running in 1-Buffer-Mode, reset flag b3BufferMode */
                                b3BufferMode = FALSE;
                                }
                        }
                    }
                    else
                    {
                        /* output address is out of the allowed area or has changed in SAFEOP or OP */
                        result = SYNCMANCHADDRESS + 1;
                    }
                }
                else
                {
                    /* output settings do not match */
                    result = SYNCMANCHSETTINGS + 1;
                }
            }
            else
            {
                /* output sizes don't match */
                result = SYNCMANCHSIZE + 1;
            }
        }
        else if (SMLength != 0 || nPdOutputSize != 0)
        {
            /* output size is not zero although the SM2 channel is not enabled */
            result = SYNCMANCHSIZE + 1;
        }

        if ( result != 0 )
        {
            result = ALSTATUSCODE_INVALIDSMOUTCFG;
        }
    }


    if ( result == 0 )
    {
        /* the Enable-Byte of the rest of the SM channels has to be read to acknowledge the SM-Change-Interrupt */
        for (i = maxChannel; i < nMaxSyncMan; i++)
        {
            pSyncMan = GetSyncMan(i);
            SMActivate = pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET];
        }
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    This function is called in case of the state transition from PREOP to SAFEOP.
 |brief  the areas of the Sync Managers will be checked for overlapping,
 \brief  the synchronization mode (Free Run, Synchron, Distributed Clocks) is selected,
 \brief  the requested cycle time will be checked, the watchdog is started
 \brief  and the AL Event Mask register will be set

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 StartInputHandler(void)
{
    TSYNCMAN ESCMEM * pSyncMan;

    UINT16        dcControl;

    UINT16     wdiv = 0;
    UINT16     wd = 0;
    UINT32     cycleTimeSync0 = 0; /* Sync0 cycle time */
    UINT32     shiftTimeSync1 = 0; /* Delay between the Sync0 and Sycn1 signal. A new Sync1 cycle starts on the next Sync0 signal after Sync1 signal.*/
    BOOL bSubordinatedCycles = FALSE;

    UINT16    nPdInputBuffer = 3;

    UINT16    nPdOutputBuffer = 3;

    UINT16 SyncType0x1C32 = 0; /* Helper variable for sync type for SM2 (required if no CoE is supported or no output process data available)*/
    UINT16 SyncType0x1C33 = 0; /* Helper variable for sync type for SM3 (required if no CoE is supported or no input process data available)*/

    UINT16 u16MinSuppSyncType = 0xFFFF;  /* Minimum supported Sync Types */

    u16MinSuppSyncType &= sSyncManOutPar.u16SyncTypesSupported;
    u16MinSuppSyncType &= sSyncManInPar.u16SyncTypesSupported;

    u16ALEventMask = 0;


    /* 
        --- Check if SyncManager areas overlapping --- 
    */
    bEcatFirstOutputsReceived = FALSE;

    /* get a pointer to the Sync Manager Channel 2 (Outputs) */
    pSyncMan = GetSyncMan(PROCESS_DATA_OUT);
    /* store the address of the Sync Manager Channel 2 (Outputs) */
    nEscAddrOutputData = pSyncMan->PhysicalStartAddress;
    /* get the number of output buffer used for calculating the address areas */
    if (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_ONE_BUFFER_VALUE)
    {
       nPdOutputBuffer = 1;
    }


    /* get a pointer to the Sync Manager Channel 3 (Inputs) */
    pSyncMan = GetSyncMan(PROCESS_DATA_IN);
    /* store the address of the Sync Manager Channel 3 (Inputs)*/
    nEscAddrInputData = pSyncMan->PhysicalStartAddress;


    /* get the number of input buffer used for calculating the address areas */
    if (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_ONE_BUFFER_VALUE)
    {
        nPdInputBuffer = 1;
    }
    /* it has be checked if the Sync Manager memory areas for Inputs and Outputs will not overlap
       the Sync Manager memory areas for the Mailbox */

    if (((nEscAddrInputData + nPdInputSize * nPdInputBuffer) > u16EscAddrSendMbx && (nEscAddrInputData < (u16EscAddrSendMbx + u16SendMbxSize)))
       || ((nEscAddrInputData + nPdInputSize * nPdInputBuffer) > u16EscAddrReceiveMbx && (nEscAddrInputData < (u16EscAddrReceiveMbx + u16ReceiveMbxSize)))
        )
    {
        return ALSTATUSCODE_INVALIDSMINCFG;
    }

    if (
        ((nEscAddrOutputData + nPdOutputSize * nPdOutputBuffer) > u16EscAddrSendMbx && (nEscAddrOutputData < (u16EscAddrSendMbx + u16SendMbxSize)))
        ||((nEscAddrOutputData + nPdOutputSize * nPdOutputBuffer) > u16EscAddrReceiveMbx && (nEscAddrOutputData < (u16EscAddrReceiveMbx + u16ReceiveMbxSize)))
        ||
        ((nEscAddrOutputData + nPdOutputSize * nPdOutputBuffer) > nEscAddrInputData && (nEscAddrOutputData < (nEscAddrInputData + nPdInputSize)))
        )
    {

        /* Sync Manager Channel 2 memory area (Outputs) overlaps the Sync Manager memory areas for the Mailbox
           or the Sync Manager Channel 3 memory area (Inputs) */
        return ALSTATUSCODE_INVALIDSMOUTCFG;
    }

    /* 
        --- Check configured synchronization ---
    */

    /* Get the DC Control/Activation register value*/
    /*Read registers 0x980:0x981 (corresponding masks are adapted)*/
    HW_EscReadWord(dcControl, ESC_DC_UNIT_CONTROL_OFFSET);
    dcControl = SWAPWORD(dcControl);

    // Cycle time for Sync0
        HW_EscReadDWord(cycleTimeSync0, ESC_DC_SYNC0_CYCLETIME_OFFSET);
        cycleTimeSync0 = SWAPDWORD(cycleTimeSync0);

    // Cycle time for Sync1
        HW_EscReadDWord(shiftTimeSync1, ESC_DC_SYNC1_CYCLETIME_OFFSET);
        shiftTimeSync1 = SWAPDWORD(shiftTimeSync1);


    SyncType0x1C32 = sSyncManOutPar.u16SyncType;
    SyncType0x1C33 = sSyncManInPar.u16SyncType;



    /* check general DC register plausibility and if configuration is supported
       - 0x981 DC Active
       - 0x9A0:0x9A3 Sync0 Cycle
       - 0x9A4:0x9A7 Sync1 Cycle
    */
    if((dcControl & (ESC_DC_SYNC_UNIT_ACTIVE_MASK | ESC_DC_SYNC_UNIT_AUTO_ACTIVE_MASK)) != 0)
    {
        /* DC unit is active at least one Sync signal shall be generated */
        if((dcControl & (ESC_DC_SYNC0_ACTIVE_MASK | ESC_DC_SYNC1_ACTIVE_MASK)) == 0)
        {
            return ALSTATUSCODE_DCINVALIDSYNCCFG;
        }

        /* If Sync1 shall only be active if also Sync0 will be generated*/
        if(((dcControl & ESC_DC_SYNC0_ACTIVE_MASK) == 0)
            && ((dcControl & ESC_DC_SYNC1_ACTIVE_MASK) != 0))
        {
            return ALSTATUSCODE_DCINVALIDSYNCCFG;
        }

        if(u16MinSuppSyncType != 0)
        {
            if((((u16MinSuppSyncType & SYNCTYPE_DCSYNC0SUPP) == 0) && ((dcControl & ESC_DC_SYNC0_ACTIVE_MASK) != 0))
                ||(((u16MinSuppSyncType & SYNCTYPE_DCSYNC1SUPP) == 0) && ((dcControl & ESC_DC_SYNC1_ACTIVE_MASK) != 0)))
            {
                /* Sync0 is not supported but will be generated*/
                return ALSTATUSCODE_DCINVALIDSYNCCFG;                   
    }
        }

        {
            UINT32 curMinCycleTime = MIN_PD_CYCLE_TIME;
            curMinCycleTime = sSyncManOutPar.u32MinCycleTime;

            /*Check if Sync0 cycle time is supported*/
            if (cycleTimeSync0 != 0 && (cycleTimeSync0 < curMinCycleTime || cycleTimeSync0 > MAX_PD_CYCLE_TIME))
            {
                    return ALSTATUSCODE_DCSYNC0CYCLETIME;
            }
        }


        /* Check if Subordinated cycles are configured */
        if(((dcControl & ESC_DC_SYNC0_ACTIVE_MASK) != 0) && ((dcControl & ESC_DC_SYNC1_ACTIVE_MASK) != 0))
        {
            /* For Subordinated cycles both Sync signals shall be active and Sync0 is not configured in single shot (cycle time == 0)*/
            if((shiftTimeSync1 > 0) && (shiftTimeSync1 >= cycleTimeSync0))
            {
                bSubordinatedCycles = TRUE;
            }
        }

        /* Dump an error if subordinated cycles are configured but not supported */
        if(bSubordinatedCycles && ((u16MinSuppSyncType & SYNCTYPE_SUBCYCLESUPP) == 0))
        {
             return ALSTATUSCODE_DCINVALIDSYNCCFG;
        }
    }


    /*
        Check if the user configured Sync Type matches the DC register values (if the Sync Type is supported was already checked in the object write function)
    */
    if(bSyncSetByUser)
    {
        if((dcControl & (ESC_DC_SYNC_UNIT_ACTIVE_MASK | ESC_DC_SYNC_UNIT_AUTO_ACTIVE_MASK)) == 0)
        {
            /* DC out unit not enabled => no DC mode shall be set */
            if((SyncType0x1C32 == SYNCTYPE_DCSYNC0) || (SyncType0x1C32 == SYNCTYPE_DCSYNC1)
                ||(SyncType0x1C33 == SYNCTYPE_DCSYNC0) || (SyncType0x1C33 == SYNCTYPE_DCSYNC1))
            {
                return ALSTATUSCODE_DCINVALIDSYNCCFG;
            }
        } //if((dcControl & (ESC_DC_SYNC_UNIT_ACTIVE_MASK | ESC_DC_SYNC_UNIT_AUTO_ACTIVE_MASK)) == 0)
    else
    {
            if((dcControl & ESC_DC_SYNC1_ACTIVE_MASK) == 0)
            {
                /* No Sync 1 is generated => No Sync1 Sync Type shall configured*/
                if((SyncType0x1C32 == (UINT16)SYNCTYPE_DCSYNC1)
                    ||(SyncType0x1C33 == (UINT16)SYNCTYPE_DCSYNC1))
                {
                    return ALSTATUSCODE_DCINVALIDSYNCCFG;
                }
            } //if((dcControl & ESC_DC_SYNC1_ACTIVE_MASK) == 0)

            if((dcControl & ESC_DC_SYNC0_ACTIVE_MASK) == 0)
            {
                /* No Sync 0 is generated => No Sync0 Sync Type shall configured*/
                if((SyncType0x1C32 == (UINT16)SYNCTYPE_DCSYNC0)
                    ||(SyncType0x1C33 == (UINT16)SYNCTYPE_DCSYNC0))
                {
                    return ALSTATUSCODE_DCINVALIDSYNCCFG;
                }
            } //if((dcControl & ESC_DC_SYNC0_ACTIVE_MASK) == 0)

        }
    } //if(bSyncSetByUser)
    else
    {
        /* No Sync Type selected by user => Configure Sync Type based on DC register values*/
        if((dcControl & (ESC_DC_SYNC_UNIT_ACTIVE_MASK | ESC_DC_SYNC_UNIT_AUTO_ACTIVE_MASK)) == 0)
        {
            /* Activation or auto activation of the Sync Out Unit is disabled => Free Run or SM Sync is configured*/

            /* AL Event enabled => Configure SM Sync*/
            if (nPdOutputSize > 0)
            {
                SyncType0x1C32 = SYNCTYPE_SM_SYNCHRON;
                
                if (nPdInputSize > 0)
                {
                    SyncType0x1C33 = SYNCTYPE_SM2_SYNCHRON;
                }
                else
                {
                    SyncType0x1C33 = SYNCTYPE_FREERUN;
                }
            }
            else if (nPdInputSize > 0)
            {
                SyncType0x1C32 = SYNCTYPE_FREERUN;
                SyncType0x1C33 = SYNCTYPE_SM_SYNCHRON;
            }
            else
            {
                SyncType0x1C32 = SYNCTYPE_FREERUN;
                SyncType0x1C33 = SYNCTYPE_FREERUN;
            }

        }
        else
        {
            if (nPdOutputSize > 0)
            {
                /* Sync Signal generation is active*/
                if (bSubordinatedCycles)
                {
                    SyncType0x1C32 = SYNCTYPE_DCSYNC1;
                }
                else
                {
                    SyncType0x1C32 = SYNCTYPE_DCSYNC0;
                }
            }
            else
            {
                SyncType0x1C32 = SYNCTYPE_FREERUN;
            }


            if (nPdInputSize > 0)
            {
                if ((dcControl & ESC_DC_SYNC1_ACTIVE_MASK) != 0)
                {
                    /* If Sync1 is available the inputs will always be mapped with Sync1 */
                    SyncType0x1C33 = SYNCTYPE_DCSYNC1;
                }
                else
                {
                    /* Map Inputs based on Sync0*/
                    SyncType0x1C33 = SYNCTYPE_DCSYNC0;
                }
            }
            else
            {
                SyncType0x1C33 = SYNCTYPE_FREERUN;
            }
        }
    }

    /* Update Cycle time entries if DC Sync Mode enabled */
    if(SyncType0x1C32 == SYNCTYPE_DCSYNC1)
    {
        sSyncManOutPar.u32Sync0CycleTime = (UINT32)cycleTimeSync0;
        sSyncManOutPar.u32CycleTime = (UINT32)cycleTimeSync0;

        sSyncManInPar.u32Sync0CycleTime = (UINT32)cycleTimeSync0;
        sSyncManInPar.u32CycleTime = (UINT32)cycleTimeSync0;
    }
    else if(SyncType0x1C32 == SYNCTYPE_DCSYNC0)
    {
        sSyncManOutPar.u32Sync0CycleTime = (UINT32)cycleTimeSync0;
        sSyncManOutPar.u32CycleTime = (UINT32)cycleTimeSync0;

        sSyncManInPar.u32Sync0CycleTime = (UINT32)cycleTimeSync0;
        sSyncManInPar.u32CycleTime = (UINT32)cycleTimeSync0;
    }

    /* Set global flags based on Sync Type */
    if ( !b3BufferMode )
    {
        /* 1-Buffer-Mode configured => For free run it shall be 3Buffer mode*/
        if (( SyncType0x1C32 == SYNCTYPE_FREERUN ) || ( SyncType0x1C33 == SYNCTYPE_FREERUN ))
        {
                return ALSTATUSCODE_FREERUNNEEDS3BUFFERMODE;
        }
    }

    /* If no free run is supported the EscInt is always enabled*/
        if (( SyncType0x1C32 != SYNCTYPE_FREERUN ) || ( SyncType0x1C33 != SYNCTYPE_FREERUN ))
        {
        /* ECAT Synchron Mode, the ESC interrupt is enabled */
        bEscIntEnabled = TRUE;
    }

        /* Update value for AL Event Mask register (0x204) */
        if(bEscIntEnabled)
        {
            if(nPdOutputSize > 0)
            {
                u16ALEventMask = PROCESS_OUTPUT_EVENT;
            }
            else if(nPdInputSize > 0)
            {
                u16ALEventMask = PROCESS_INPUT_EVENT;
            }

        }

        if ((SyncType0x1C32 == SYNCTYPE_DCSYNC0) || (SyncType0x1C32 == SYNCTYPE_DCSYNC1)
            || (SyncType0x1C33 == SYNCTYPE_DCSYNC0) || (SyncType0x1C33 == SYNCTYPE_DCSYNC1))/* Sync to Sync0 or Sync1 is enabled*/
        {
            /* slave is running in DC-mode */
            bDcSyncActive = TRUE;

            /*In case of an Input only application with DC no PDI Isr handling is required*/
            if (nPdOutputSize == 0)
            {
               u16ALEventMask = 0;
            }
        }



    sSyncManOutPar.u16SyncType = SyncType0x1C32;
    sSyncManInPar.u16SyncType = SyncType0x1C33;

    /* Calculate number of Sync0 events within one SM cycle and the Sync0 events on which the inputs has to be latched*/
    LatchInputSync0Value = 0;
    LatchInputSync0Counter = 0;
    u16SmSync0Value = 0;
    u16SmSync0Counter = 0;


    if(bSubordinatedCycles == TRUE)
    {
        
        UINT32 cycleTimeSync1 = (shiftTimeSync1 + cycleTimeSync0);


        /* get the number of Sync0 event within on SM cycle */
        if(shiftTimeSync1 >= cycleTimeSync0)
        {

            u16SmSync0Value = (UINT16)(cycleTimeSync1 / cycleTimeSync0);
            
            if((cycleTimeSync1 % cycleTimeSync0) == 0)
            {
                /* if the Sync1cycletime/Sync0cycletime ratio is even one additional tick */
                u16SmSync0Value ++;
            }
        }
        else
        {
            u16SmSync0Value = 1;
        }

        /* Calculate the Sync0 tick on which the inputs shall be latched (last Sync0 before the next Sync1 event)*/
        LatchInputSync0Value = (UINT16) (cycleTimeSync1 / cycleTimeSync0);

        if ((cycleTimeSync1 % cycleTimeSync0) > 0)
        {
            LatchInputSync0Value++;
        }

    }
    else 
    {
        if(SyncType0x1C32 == SYNCTYPE_DCSYNC0)
        {
            /* if SyncType of 0x1C32 is 2 the Sync0 event is trigger once during a SM cycle */
            u16SmSync0Value = 1;
        }   

        if(SyncType0x1C33 != SYNCTYPE_DCSYNC1)
        {
            LatchInputSync0Value = 1;
        }
    }



    /* reset the error counter indicating synchronization problems */
    sCycleDiag.syncFailedCounter = 0;


    /*
        --- Check watchdog settings ---
    */

    /*get the watchdog time (register 0x420). if value is > 0 watchdog is active*/
    HW_EscReadWord(wd, ESC_PD_WD_TIME);
    wd = SWAPWORD(wd);

    if (nPdOutputSize > 0 &&  wd != 0 )
    {
    /*get watchdog divider (register 0x400)*/
    HW_EscReadWord(wdiv, ESC_WD_DIVIDER_OFFSET);
    wdiv = SWAPWORD(wdiv);
        if ( wdiv != 0 )
        {
            /* the ESC subtracts 2 in register 0x400 so it has to be added here */
            UINT32 d = wdiv+2;


            d *= wd;
            /* store watchdog in ms in variable EcatWdValue */
            /* watchdog value has to be rounded up */
            d = (INT32)(d + 24999);
            d /= 25000;
            EcatWdValue = (UINT16) d;
        }
        else
        {
            wd = 0;
            /* wd value has to be set to zero, if the wd is 0 */
            EcatWdValue = 0;
        }
    }
    else
    {
        /* the watchdog is deactivated or slave has no output process data*/
        wdiv = 0;
        EcatWdValue = 0;
    }

    if((EcatWdValue == 0 && bWdTrigger) || (EcatWdValue != 0 && !bWdTrigger))
    {
        /* if the WD-Trigger in the Sync Manager Channel 2 Control-Byte is set (Bit 6 of Register 0x814)
            an error has to be returned */
        return ALSTATUSCODE_INVALIDWDCFG;
    }

    if ( bEscIntEnabled && nPdOutputSize != 0 )
    {
        /* ECAT synchron Mode is active, the Sync Manager Channel 2 event
           has to activated in the AL-Event mask register */
        u16ALEventMask |= PROCESS_OUTPUT_EVENT;
    }
/*The application ESM function is separated from this function to handle pending transitions*/

    Sync0WdValue = 0;
    Sync0WdCounter = 0;
    Sync1WdCounter = 0;
    Sync1WdValue = 0;
    bDcRunning = FALSE;
    bSmSyncSequenceValid = FALSE;
    i16WaitForPllRunningTimeout = 0;

/*ECATCHANGE_START(V5.13) ECAT1*/
    /*Get Sync mapped to AL Event indication*/
    {
        UINT32 u32TmpVar = 0;
        HW_EscReadDWord(u32TmpVar, ESC_PDI_CONFIGURATION);
        if ((u32TmpVar & ESC_SYNC0_MAPPED_TO_ALEVENT) > 0)
        {
            u16ALEventMask |= SYNC0_EVENT;
        }

        if ((u32TmpVar & ESC_SYNC1_MAPPED_TO_ALEVENT) > 0)
        {
            u16ALEventMask |= SYNC1_EVENT;
        }
    }
/*ECATCHANGE_END(V5.13) ECAT1*/
    sSyncManInPar.u16SmEventMissedCounter = 0;
    sSyncManInPar.u16CycleExceededCounter = 0;
    sSyncManInPar.u8SyncError = 0;


    sSyncManOutPar.u16SmEventMissedCounter = 0;
    sSyncManOutPar.u16CycleExceededCounter = 0;
    sSyncManOutPar.u8SyncError = 0;

    /* calculate the Sync0/Sync1 watchdog timeouts */
    if ( (dcControl & ESC_DC_SYNC0_ACTIVE_MASK) != 0 )
    {
        /*calculate the Sync0 Watchdog counter value the minimum value is 1 ms
            if the sync0 cycle is greater 500us the Sync0 Wd value is 2*Sycn0 cycle */
        if(cycleTimeSync0 == 0)
        {
            Sync0WdValue = 0;
        }
        else
        {
            UINT32 Sync0Cycle = cycleTimeSync0/100000;

            if(Sync0Cycle < 5)
            {
                /*Sync0 cycle less than 500us*/
                Sync0WdValue = 1;
            }
            else
            {
                Sync0WdValue = (UINT16)(Sync0Cycle*2)/10;
            }
        }

        /* Calculate also the watchdog time for Sync1*/
        if ( (dcControl & ESC_DC_SYNC1_ACTIVE_MASK) != 0 )
        {
            if(shiftTimeSync1 < cycleTimeSync0)
        {
                /* Sync 1 has the same cycle time than Sync0 (maybe with a shift (shiftTimeSync1 > 0))*/
                Sync1WdValue = Sync0WdValue;
        }
        else
        {
                /* Sync1 cycle is larger than Sync0 (e.g. subordinated Sync0 cycles) */
                UINT32 Sync1Cycle = (shiftTimeSync1  + cycleTimeSync0 )/100000;
                if(Sync1Cycle < 5)
                {
                    /*Sync0 cycle less than 500us*/
                    Sync1WdValue = 1;
                }
                else
                {
                    Sync1WdValue = (UINT16)((Sync1Cycle*2)/10);
                }

                /* add one Sync0 cycle because the Sync1 cycle starts on the next Sync0 after the Sync1 signal */
                Sync1WdValue += Sync0WdValue/2;
            }
    }
    }

    if(nPdOutputSize > 0)
    {
        EnableSyncManChannel(PROCESS_DATA_OUT);
    }

    if(nPdInputSize > 0)
    {
        EnableSyncManChannel(PROCESS_DATA_IN);
    }

    /*write initial input data*/
    PDO_InputMapping();

    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    This function is called in case of the state transition from SAFEOP to OP.
 \brief  It will be checked if outputs had to be received before switching to OP
 \brief  and the state transition would be refused if outputs are missing

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 StartOutputHandler(void)
{
    /* by default the SO transition should be completed in AlControlRes().
       required to support also masters which starts to send process data after the SO transition was triggered
       (if the master don't send process data within "SAFEOP2OPTIMEOUT" the transition is rejected)*/
    UINT16 result = NOERROR_INWORK;
    /*ECATCHANGE_START(V5.13) ESM1*/
    if(STATE_VALID(u8LocalErrorState))
/*ECATCHANGE_END(V5.13) ESM1*/
    {
        /*Local error still exists => skip state request to OP and response with "u16LocalErrorCode"*/
        return u16LocalErrorCode;
    }
/*The application ESM function is separated from this function to handle pending transitions*/


    /*DC synchronisation is active wait until pll is valid*/
    if(bDcSyncActive)
    {
        i16WaitForPllRunningTimeout = 200;

        i16WaitForPllRunningCnt = 0;
    }



    sSyncManOutPar.u16SmEventMissedCounter = 0;
    sSyncManOutPar.u8SyncError = 0;


    sSyncManInPar.u16SmEventMissedCounter = 0;
    sSyncManInPar.u8SyncError = 0;


    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function is called in case of the state transition from OP to SAFEOP
 \brief  the outputs can be set to an application specific safe state,
 \brief  the state transition can be delayed by returning NOERROR_INWORK

*////////////////////////////////////////////////////////////////////////////////////////

void StopOutputHandler(void)
{
    /* reset the flags that outputs were received and that the slave is in OP */
    bEcatFirstOutputsReceived = FALSE;
    bEcatOutputUpdateRunning = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
  \brief    This function is called in case of the state transition from SAFEOP to PREOP

*////////////////////////////////////////////////////////////////////////////////////////

void StopInputHandler(void)
{
    if(nPdOutputSize > 0)
    {
        /* disable the Sync Manager Channel 2 (outputs) */
        DisableSyncManChannel(PROCESS_DATA_OUT);
    }

    if(nPdInputSize > 0)
    {
        /*disable Sync Manager 3 (inputs) if no outputs available*/
        DisableSyncManChannel(PROCESS_DATA_IN);
    }

    /* reset the events in the AL Event mask register (0x204) */
/*ECATCHANGE_START(V5.13) ECAT1*/
/*ECATCHANGE_END(V5.13) ECAT1*/
    {
        UINT16 ResetMask = SYNC0_EVENT | SYNC1_EVENT;
        ResetMask |= PROCESS_OUTPUT_EVENT;
        ResetMask |= PROCESS_INPUT_EVENT;

    ResetALEventMask( ~(ResetMask) );
    }
    /* reset the flags */
    bEcatFirstOutputsReceived = FALSE;
    bEscIntEnabled = FALSE;
/*The application ESM function is separated from this function to handle pending transitions*/

    bDcSyncActive = FALSE;
    bDcRunning = FALSE;
    bSmSyncSequenceValid = FALSE;
    u16SmSync0Value = 0;
    u16SmSync0Counter = 0;

    Sync0WdValue = 0;
    Sync0WdCounter = 0;
    Sync1WdCounter = 0;
    Sync1WdValue = 0;
    LatchInputSync0Value = 0;
    LatchInputSync0Counter = 0;


    sSyncManOutPar.u16SmEventMissedCounter = 0;
    sSyncManOutPar.u16CycleExceededCounter = 0;
    sSyncManOutPar.u8SyncError = 0;


    sSyncManInPar.u16SmEventMissedCounter = 0;
    sSyncManInPar.u16CycleExceededCounter = 0;
    sSyncManInPar.u8SyncError = 0;

    i16WaitForPllRunningTimeout = 0;

    bWdTrigger = FALSE;
    bEcatInputUpdateRunning = FALSE;

    /*Indicate no user specified Sync mode*/
    bSyncSetByUser = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function is called when a X to Init transition is completed

*////////////////////////////////////////////////////////////////////////////////////////

void BackToInitTransition(void)
{
    /* Reset indication that the user has written a sync mode*/
    bSyncSetByUser = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param alStatus        New AL Status (written to register 0x130)
 \param alStatusCode    New AL Status Code (written to register 0x134)

  \brief  The function changes the state of the EtherCAT ASIC to the requested.
*////////////////////////////////////////////////////////////////////////////////////////
void SetALStatus(UINT8 alStatus, UINT16 alStatusCode)
{
    UINT16 Value = alStatusCode;

    /*update global status variable if required*/
    if(nAlStatus != alStatus)
    {
        nAlStatus = alStatus;
    }

    /*Handle Explicit Device ID is requested*/
    if(bExplicitDevIdRequested && !(nAlStatus & STATE_CHANGE) && alStatusCode == 0 && ((nAlStatus & STATE_MASK) != STATE_BOOT))
    {
/*ECATCHANGE_START(V5.13) ECAT2*/
        Value = u16IdValue;
/*ECATCHANGE_END(V5.13) ECAT2*/
        nAlStatus |= STATE_DEVID;
    }
    else
    {
        nAlStatus &= ~STATE_DEVID;
    }

    if (alStatusCode != 0xFFFF)
    {
        Value = SWAPWORD(Value);

        HW_EscWriteWord(Value,ESC_AL_STATUS_CODE_OFFSET);
    }

    Value = nAlStatus;
    Value = SWAPWORD(Value);
    HW_EscWriteWord(Value,ESC_AL_STATUS_OFFSET);

    /*The Run LED state is set in Set LED Indication, only the Error LED blink code is set here*/

    /*set Error blink code*/
    if(alStatusCode == 0x00 || !(alStatus & STATE_CHANGE))
    {
        u8EcatErrorLed = LED_OFF;
    }
    else if((alStatusCode == ALSTATUSCODE_NOSYNCERROR) ||
        (alStatusCode == ALSTATUSCODE_SYNCERROR) ||
        (alStatusCode == ALSTATUSCODE_DCPLLSYNCERROR)
/*ECATCHANGE_START(V5.13) ESM1*/
        || (u8LocalErrorState > 0))
/*ECATCHANGE_END(V5.13) ESM1*/
    {
        u8EcatErrorLed = LED_SINGLEFLASH;
    }
    else if((alStatusCode == ALSTATUSCODE_SMWATCHDOG))
    {
        u8EcatErrorLed = LED_DOUBLEFLASH;
    }
    else
    {
        u8EcatErrorLed = LED_BLINKING;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    alControl        requested new state
 \param    alStatusCode     requested status code

 \brief    This function handles the EtherCAT State Machine. It is called
            * in case of an AL Control event (Bit 0 of AL-Event (Reg 0x220),
               when the Master has written the AL Control Register (from ECAT_Main),
              alControl contains the content of the AL Control (Reg 0x120)
            * in case of a SM-Change event (Bit 4 of AL-Event (Reg 0x220)),
              when an Activate SYNCM y register is written by the master (from ECAT_Main),
              alControl contains the actual state (Bit 0-3 of AL Status (Reg 0x130))
            * in case of a locally expired watchdog (from ECAT_Main),
              alControl contains the requested new state (SAFE_OP)
            * in case of an application specific event to change the EtherCAT state (from application),
              alControl contains the requested new state (INIT, PRE_OP or SAFE_OP)

*////////////////////////////////////////////////////////////////////////////////////////

void AL_ControlInd(UINT8 alControl, UINT16 alStatusCode)
{
    UINT16        result = 0;
    UINT8            bErrAck = 0;
    UINT8         stateTrans;
    /*deactivate ESM timeout counter*/
    EsmTimeoutCounter = -1;
    bApplEsmPending = TRUE;

    /* reset the Error Flag in case of acknowledge by the Master */
    if ( alControl & STATE_CHANGE )
    {
        bErrAck = 1;
        nAlStatus &= ~STATE_CHANGE;
        /*enable SM2 is moved to state transition block. First check SM Settings.*/
    }
    else if ((nAlStatus & STATE_CHANGE)
        // HBu 17.04.08: the error has to be acknowledged before when sending the same (or a higher) state
        //               (the error was acknowledged with the same state before independent of the acknowledge flag)
        /*Error Acknowledge with 0xX1 is allowed*/
        && (alControl & STATE_MASK) != STATE_INIT)
    {
        /* the error flag (Bit 4) is set in the AL-Status and the ErrAck bit (Bit 4)
           is not set in the AL-Control, so the state cannot be set to a higher state
           and the new state request will be ignored */
        return;
    }
    else
    {
        nAlStatus &= STATE_MASK;
    }

    

    /* generate a variable for the state transition
      (Bit 0-3: new state (AL Control), Bit 4-7: old state (AL Status) */
    alControl &= STATE_MASK;
    stateTrans = nAlStatus;
    stateTrans <<= 4;
    stateTrans += alControl;

    /* check the SYNCM settings depending on the state transition */
    switch ( stateTrans )
    {
    case INIT_2_PREOP:
    case OP_2_PREOP:
    case SAFEOP_2_PREOP:
    case PREOP_2_PREOP:
        /* in PREOP only the SYNCM settings for SYNCM0 and SYNCM1 (mailbox)
           are checked, if result is unequal 0, the slave will stay in or
           switch to INIT and set the ErrorInd Bit (bit 4) of the AL-Status */
        result = CheckSmSettings(MAILBOX_READ+1);
        break;
    case PREOP_2_SAFEOP:
        {
        /* before checking the SYNCM settings for SYNCM2 and SYNCM3 (process data)
           the expected length of input data (nPdInputSize) and output data (nPdOutputSize)
            could be adapted (changed by PDO-Assign and/or PDO-Mapping)
            if result is unequal 0, the slave will stay in PREOP and set
            the ErrorInd Bit (bit 4) of the AL-Status */
        result = APPL_GenerateMapping(&nPdInputSize,&nPdOutputSize);

            if (result != 0)
            {
                break;
            }
        }
    case SAFEOP_2_OP:
    case OP_2_SAFEOP:
    case SAFEOP_2_SAFEOP:
    case OP_2_OP:
        /* in SAFEOP or OP the SYNCM settings for all SYNCM are checked
           if result is unequal 0, the slave will stay in or
           switch to PREOP and set the ErrorInd Bit (bit 4) of the AL-Status */
        result = CheckSmSettings(nMaxSyncMan);
        break;

    }

    if ( result == 0 )
    {
        /* execute the corresponding local management service(s) depending on the state transition */
        nEcatStateTrans = 0;
        switch ( stateTrans )
        {
        case INIT_2_BOOT    :
            result = ALSTATUSCODE_BOOTNOTSUPP;



            break;

        case BOOT_2_INIT    :
            result = ALSTATUSCODE_BOOTNOTSUPP;

            BackToInitTransition();



            break;
        case INIT_2_PREOP :

           UpdateEEPROMLoadedState();

            if (EepromLoaded == FALSE)
            {
                //return an error if the EEPROM was not loaded correct  (device restart is required after the new EEPORM update)
                result = ALSTATUSCODE_EE_ERROR;
            }
            if (result == 0)
            {
            /* MBX_StartMailboxHandler (in mailbox.c) checks if the areas of the mailbox
               sync managers SYNCM0 and SYNCM1 overlap each other
              if result is unequal 0, the slave will stay in INIT
              and sets the ErrorInd Bit (bit 4) of the AL-Status */
            result = MBX_StartMailboxHandler();
            if (result == 0)
            {
                bApplEsmPending = FALSE;
                /* additionally there could be an application specific check (in ecatappl.c)
                   if the state transition from INIT to PREOP should be done
                 if result is unequal 0, the slave will stay in INIT
                 and sets the ErrorInd Bit (bit 4) of the AL-Status */
                result = APPL_StartMailboxHandler();
                if ( result == 0 )
                {
                    bMbxRunning = TRUE;
                }
            }

            if(result != 0 && result != NOERROR_INWORK)
            {
                /*Stop APPL Mbx handler if APPL Start Mbx handler was called before*/
                    if (!bApplEsmPending)
                    {
                        APPL_StopMailboxHandler();
                    }

                 MBX_StopMailboxHandler();
            }

            }
            break;

        case PREOP_2_SAFEOP:
            /* start the input handler (function is defined above) */
            result = StartInputHandler();
            if ( result == 0 )
            {
                bApplEsmPending = FALSE;
                result = APPL_StartInputHandler(&u16ALEventMask);

                if(result == 0)
                {
/*ECATCHANGE_START(V5.13) ECAT1*/
/*ECATCHANGE_END(V5.13) ECAT1*/
                    /* initialize the AL Event Mask register (0x204) */
                    SetALEventMask( u16ALEventMask );

                    bEcatInputUpdateRunning = TRUE;
                }
            }

            /*if one start input handler returned an error stop the input handler*/
            if(result != 0 && result != NOERROR_INWORK)
            {
                if(!bApplEsmPending)
                {
                    /*Call only the APPL stop handler if the APPL start handler was called before*/
                    /*The application can react to the state transition in the function APPL_StopInputHandler */
                    APPL_StopInputHandler();
                }

                StopInputHandler();
            }
            break;

        case SAFEOP_2_OP:
/*ECATCHANGE_START(V5.13) ESM2*/
            /*enable SM if error was acknowledged*/
            if (bErrAck)
            {
                if (nPdOutputSize > 0)
                {
                    EnableSyncManChannel(PROCESS_DATA_OUT);
                }
                else
                    if (nPdInputSize > 0)
                    {
                        EnableSyncManChannel(PROCESS_DATA_IN);
                    }
            }
            /*ECATCHANGE_END(V5.13) ESM2*/

            /* start the output handler (function is defined above) */
            result = StartOutputHandler();
            if(result == 0)
            {
                bApplEsmPending = FALSE;
                result = APPL_StartOutputHandler();

                if(result == 0)
                {
                    /*Device is in OPERATINAL*/
                    bEcatOutputUpdateRunning = TRUE;
                }

            }

            if ( result != 0 && result != NOERROR_INWORK)
            {
                    if (!bApplEsmPending)
                    {
                        APPL_StopOutputHandler();
                    }

                StopOutputHandler();
            }

            break;

        case OP_2_SAFEOP:
            /* stop the output handler (function is defined above) */
            APPL_StopOutputHandler();

            StopOutputHandler();

            bApplEsmPending = FALSE;

            break;

        case OP_2_PREOP:
            /* stop the output handler (function is defined above) */
            result = APPL_StopOutputHandler();

            StopOutputHandler();

            bApplEsmPending = FALSE;

            if (result != 0)
            {
                break;
            }

            stateTrans = SAFEOP_2_PREOP;

        case SAFEOP_2_PREOP:
            /* stop the input handler (function is defined above) */
            APPL_StopInputHandler();
           
            StopInputHandler();

            bApplEsmPending = FALSE;

            break;

        case OP_2_INIT:
            /* stop the output handler (function is defined above) */
            result = APPL_StopOutputHandler();

            StopOutputHandler();

            bApplEsmPending = FALSE;

            if (result != 0)
            {
                break;
            }
            
            stateTrans = SAFEOP_2_INIT;

        case SAFEOP_2_INIT:
            /* stop the input handler (function is defined above) */
            result = APPL_StopInputHandler();
            
            StopInputHandler();

            bApplEsmPending = FALSE;

            if (result != 0)
            {
                break;
            }
            stateTrans = PREOP_2_INIT;

        case PREOP_2_INIT:
            MBX_StopMailboxHandler();
            result = APPL_StopMailboxHandler();

            BackToInitTransition();
            break;
        case INIT_2_INIT:
            BackToInitTransition();
        case PREOP_2_PREOP:
        case SAFEOP_2_SAFEOP:
        case OP_2_OP:
            if(bErrAck)
            {
                APPL_AckErrorInd(stateTrans);
            }


                /*no local error flag is currently active, enable SM*/
                if ( nAlStatus & (STATE_SAFEOP | STATE_OP))
                {
                    if(nPdOutputSize > 0)
                    {
                        EnableSyncManChannel(PROCESS_DATA_OUT);
                    }
                    else 
                    if(nPdInputSize > 0)
                    {
                        EnableSyncManChannel(PROCESS_DATA_IN);
                    }
                }
            
            result = NOERROR_NOSTATECHANGE;
            break;

        case INIT_2_SAFEOP:
        case INIT_2_OP:
        case PREOP_2_OP:
        case PREOP_2_BOOT:
        case SAFEOP_2_BOOT:
        case OP_2_BOOT:
        case BOOT_2_PREOP:
        case BOOT_2_SAFEOP:
        case BOOT_2_OP:
            result = ALSTATUSCODE_INVALIDALCONTROL;
            break;

        default:
            result = ALSTATUSCODE_UNKNOWNALCONTROL;
            break;
        }
    }
    else
    {
        /* the checking of the sync manager settings was not successful
            switch back the state to PREOP or INIT */
        switch (nAlStatus)
        {
        case STATE_OP:
            /* stop the output handler (function is defined above) */
            APPL_StopOutputHandler();
            StopOutputHandler();
        case STATE_SAFEOP:
            /* stop the input handler (function is defined above) */
            APPL_StopInputHandler();

            StopInputHandler();
        case STATE_PREOP:
            if ( result == ALSTATUSCODE_INVALIDMBXCFGINPREOP )
            {
                /* the mailbox sync manager settings were wrong, switch back to INIT */
                MBX_StopMailboxHandler();
                APPL_StopMailboxHandler();

                /*Disable SM0 (MBX Out)*/
                DisableSyncManChannel(MAILBOX_WRITE);

                /*Disable SM1 (MBX In)*/
                DisableSyncManChannel(MAILBOX_READ);

                nAlStatus = STATE_INIT;
            }
            else
            {
                nAlStatus = STATE_PREOP;
            }
        }
    }

    if ( result == NOERROR_INWORK )
    {
        /* state transition is still in work
            ECAT_StateChange must be called from the application */
        bEcatWaitForAlControlRes = TRUE;
        /* state transition has to be stored */
        nEcatStateTrans = stateTrans;

        /*Init ESM timeout counter (will be decremented with the local 1ms timer)*/
        switch(nEcatStateTrans)
        {
            case INIT_2_PREOP:
            case INIT_2_BOOT:
                EsmTimeoutCounter = PREOPTIMEOUT;
            break;
            case PREOP_2_SAFEOP:
            case SAFEOP_2_OP:
                EsmTimeoutCounter = SAFEOP2OPTIMEOUT;
                break;
           default:
                EsmTimeoutCounter = 200; //Set default timeout value to 200ms
                break;
        }
        EsmTimeoutCounter -= (INT16) (EsmTimeoutCounter / 10); //subtract 10% from the timeout to react before the master runs into a timeout.

    }
    else if ( alControl != (nAlStatus & STATE_MASK) )
    {
        /* The slave state has changed */

        if ( (result != 0 || alStatusCode != 0) && ((alControl | nAlStatus) & STATE_OP) )
        {
            /* the local application requested to leave the state OP so we have to disable the SM2
               and make the state change from OP to SAFEOP by calling StopOutputHandler */

            //only execute StopOutputHandler() if Output update is still running
            if(bEcatOutputUpdateRunning)
            {
                APPL_StopOutputHandler();

                StopOutputHandler();
            }

            if(nPdOutputSize > 0)
            {
                /* disable the Sync Manager Channel 2 (outputs) */
                DisableSyncManChannel(PROCESS_DATA_OUT);
            }
            else
                if(nPdInputSize > 0)
            {
                /*disable Sync Manager 3 (inputs) if no outputs available*/
                DisableSyncManChannel(PROCESS_DATA_IN);
            }

        }
        if ( result != 0 )
        {
                if (nAlStatus == STATE_OP)
                {
                    nAlStatus = STATE_SAFEOP;
                }
            /* save the failed status to be able to decide, if the AL Status Code shall be
               reset in case of a coming successful state transition */
            nAlStatus |= STATE_CHANGE;
        }
        else
        {
            /* state transition was successful */
            if ( alStatusCode != 0 )
            {
                /* state change request from the user */
                result = alStatusCode;
                alControl |= STATE_CHANGE;
            }
            /* acknowledge the new state */
            nAlStatus = alControl;
        }

        bEcatWaitForAlControlRes = FALSE;

        /* write the AL Status register */
        SetALStatus(nAlStatus, result);
    }
    else
    {
        /* Error acknowledgement without a state transition */

         bEcatWaitForAlControlRes = FALSE;

        /* AL-Status has to be updated and AL-Status-Code has to be reset
           if the the error bit was acknowledged */
        SetALStatus(nAlStatus, 0);
    }
    /*ECATCHANGE_START(V5.13) CIA402 4*/
    /*decouple CIA402 state machine from ESM*/
    /*ECATCHANGE_END(V5.13) CIA402 4*/

}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function is called cyclic if a state transition is pending (bEcatWaitForAlControlRes == TRUE)
 \brief    If the ESM timeout is expired the state transition will be rejected. Otherwise the application specific state transition function is called.
 \brief    If the pending state transition is triggered by the application the transition need to be completed by the application (ECAT_StateChange())
  *////////////////////////////////////////////////////////////////////////////////////////
void AL_ControlRes(void)
{
    if(bEcatWaitForAlControlRes)
    {
        UINT16 result = 0;
        UINT8 Status = 0;
        UINT16 StatusCode = 0;

        if(EsmTimeoutCounter == 0)
        {
            Status =  (UINT8)(nEcatStateTrans >> 4);

            /* ESM timeout expired*/
            switch(nEcatStateTrans)
            {
                case INIT_2_PREOP:
                case INIT_2_BOOT:

                        if (!bApplEsmPending)
                        {
                            APPL_StopMailboxHandler();
                        }

                    MBX_StopMailboxHandler();
                    /*ECATCHANGE_START(V5.13) ESM1*/
                    if((u8LocalErrorState & STATE_MASK) == STATE_INIT)
                        /*ECATCHANGE_END(V5.13) ESM1*/
                    {
                        /*Set application specified error*/
                        StatusCode = u16LocalErrorCode;
                    }
                    else
                    {
                        /*Set unspecified error*/
                        StatusCode = ALSTATUSCODE_UNSPECIFIEDERROR;
                    }
                break;
                case PREOP_2_SAFEOP:

                        if (!bApplEsmPending)
                        {
                            APPL_StopInputHandler();
                        }

                    StopInputHandler();
                    
                    /*ECATCHANGE_START(V5.13) ESM1*/
                    if ((u8LocalErrorState & STATE_MASK) == STATE_PREOP)
                        /*ECATCHANGE_END(V5.13) ESM1*/
                    {
                        /*Set application specified error*/
                        StatusCode = u16LocalErrorCode;
                    }
                    else
                    {
                        /*Set unspecified error*/
                        StatusCode = ALSTATUSCODE_UNSPECIFIEDERROR;
                    }
                break;
                case SAFEOP_2_OP:
                    if(bDcSyncActive)
                    {
                        /*SafeOP to OP timeout expired check which AL status code need to be written*/
                        if(!bDcRunning)
                        {
                            /*no Sync0 signal received*/
                            StatusCode = ALSTATUSCODE_NOSYNCERROR;
                        }
                        else if(!bEcatFirstOutputsReceived && (nPdOutputSize > 0))
                        {
                            /*no process data received*/
                            StatusCode = ALSTATUSCODE_SMWATCHDOG;
                        }
/*ECATCHANGE_START(V5.13) ESM3*/
                        else if (!bSmSyncSequenceValid)
                        {
                            /*SM/Sync Sequence is not valid*/
                            StatusCode = ALSTATUSCODE_SYNCERROR;
                        }
                        else
                        {
                            /*Set valid state transition even if timeout expired*/
                            Status = STATE_OP;
                            StatusCode = 0;
                            /* Slave is OPERATIONAL */
                            bEcatOutputUpdateRunning = TRUE;
                        }
/*ECATCHANGE_END(V5.13) ESM3*/
                    }
                    else
                    {
                        if (nPdOutputSize > 0)
                        {
                            StatusCode = ALSTATUSCODE_SMWATCHDOG;
                        }
                        else
                        {
                            /*ECATCHANGE_START(V5.13) ESM1*/
                            if ((u8LocalErrorState & STATE_MASK) == STATE_SAFEOP)
                            {
                                /*Set application specified error*/
                                StatusCode = u16LocalErrorCode;
                            }
                            else
                                /*ECATCHANGE_END(V5.13) ESM1*/
                            {
                                /*Set valid state transition even if timeout expired*/
                                Status = STATE_OP;
                                StatusCode = 0;
                                /* Slave is OPERATIONAL */
                                bEcatOutputUpdateRunning = TRUE;
                            }
                        }
                    }

                    /*Stop handler on failed transition*/
                    if(StatusCode != 0)
                    {
                            if (!bApplEsmPending)
                            {
                                APPL_StopOutputHandler();
                            }

                        StopOutputHandler();
                    }
                break;
            }
        } //ESM timeout
        else
        {
            /*Call application specific transition function and complete transition it the function returns 0*/
            switch(nEcatStateTrans)
            {
                case INIT_2_PREOP:
                case INIT_2_BOOT:
                    if(bApplEsmPending)
                    {
                        bApplEsmPending = FALSE;
                        /*APPL_StartMailboxHandler() need to be called*/
                        result = APPL_StartMailboxHandler();

                        if(result == 0)
                        {
                            /*The application specific transition was successful => set active mailbox handler indication*/
                            bMbxRunning = TRUE;
                            Status =  (UINT8)(nEcatStateTrans & STATE_MASK);
                        }
                        else
                        {
                            /*The application specific transition failed.
                            (In pending case the application need to complete the transition)*/

                            if(result != NOERROR_INWORK)
                            {
                                APPL_StopMailboxHandler();
                                MBX_StopMailboxHandler();
                            }
                        }
                    }
                break;
                case PREOP_2_SAFEOP:
                    if(bApplEsmPending)
                    {
                        bApplEsmPending = FALSE;
                        result = APPL_StartInputHandler(&u16ALEventMask);

                        if(result == 0)
                        {
                            bEcatInputUpdateRunning = TRUE;
                            Status = STATE_SAFEOP;
                        }
                        else
                        {
                            /*The application specific transition failed.
                            (In pending case the application need to complete the transition)*/

                            if(result != NOERROR_INWORK)
                            {
                                APPL_StopInputHandler();
                                StopInputHandler();
                            }
                        }
                    }
                break;
                case SAFEOP_2_OP:
                   if(bApplEsmPending)
                    {
                        if(bDcSyncActive)
                        {
                            if(i16WaitForPllRunningTimeout > 0 && i16WaitForPllRunningTimeout <= i16WaitForPllRunningCnt)
                            {
                                /*Pll sequence valid for 200ms (set in APPL_StartOutputHandler() )
                                acknowledge state transition to OP */

                                i16WaitForPllRunningTimeout = 0;
                                i16WaitForPllRunningCnt = 0;

                                bApplEsmPending = FALSE;
                                result = APPL_StartOutputHandler();

                                if(result == 0)
                                {
                                    /* Slave is OPERATIONAL */
                                    bEcatOutputUpdateRunning = TRUE;
                                    Status = STATE_OP;
                                }
                                else
                                {
                                    if(result != NOERROR_INWORK)
                                    {
                                        APPL_StopOutputHandler();
                                        StopOutputHandler();
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(nPdOutputSize == 0 || bEcatFirstOutputsReceived)
                            {
                                bApplEsmPending = FALSE;  
                                result = APPL_StartOutputHandler();

                                if(result == 0)
                                {
                                    /* Slave is OPERATIONAL */
                                    bEcatOutputUpdateRunning = TRUE;
                                    Status = STATE_OP;
                                }
                                else
                                {
                                    if(result != NOERROR_INWORK)
                                    {
                                        APPL_StopOutputHandler();
                                        StopOutputHandler();
                                    }
                                }
                            }
                        }       
                    }             
                break;
            }//Switch - transition
        }

        if(Status != 0)
        {
            /*Pending state transition finished => write AL Status and AL Status Code*/
            bEcatWaitForAlControlRes = FALSE;

            if (StatusCode != 0)
            {
                Status |= STATE_CHANGE;
            }

            SetALStatus(Status,StatusCode);
        }
    }// Pending state transition (bEcatWaitForAlControlRes == true)
}


/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function checks the current Sync state and set the local flags
 The analyse of the local flags is handled in "CheckIfEcatError"

*////////////////////////////////////////////////////////////////////////////////////////
void DC_CheckWatchdog(void)
{

    if(bDcSyncActive && bEcatInputUpdateRunning)
    {
        /*If Sync0 watchdog is enabled and expired*/
        if((Sync0WdValue > 0) && (Sync0WdCounter >= Sync0WdValue))
        {
                /*Sync0 watchdog expired*/
                bDcRunning = FALSE;        
        }
        else
        {
            if(Sync0WdCounter < Sync0WdValue)
            {
                Sync0WdCounter ++;
            }

            bDcRunning = TRUE;
        }

        if(bDcRunning)
        {
            /*Check the Sync1 cycle if Sync1 Wd is enabled*/
            if(Sync1WdValue > 0)
            {
                if(Sync1WdCounter < Sync1WdValue)
                {
                    Sync1WdCounter ++;
                }
                else
                {
                    /*Sync1 watchdog expired*/
                    bDcRunning = FALSE;
                }
            }
        }
        if(bDcRunning)
        {
/*ECATCHANGE_START(V5.13) ESM4*/
           if((sErrorSettings.u16SyncErrorCounterLimit == 0) || (sSyncManOutPar.u16SmEventMissedCounter < sErrorSettings.u16SyncErrorCounterLimit))
/*ECATCHANGE_END(V5.13) ESM4*/
            {
                bSmSyncSequenceValid = TRUE;

                /*Wait for PLL is active increment the Pll valid counter*/
                if (i16WaitForPllRunningTimeout > 0)
                {
                    i16WaitForPllRunningCnt++;
                }
            }
            else if (bSmSyncSequenceValid)
            {
                    bSmSyncSequenceValid = FALSE;

                /*Wait for PLL is active reset the Pll valid counter*/
                if (i16WaitForPllRunningTimeout > 0)
                {
                    i16WaitForPllRunningCnt = 0;
                }
            }
        }
        else if(bSmSyncSequenceValid)
        {
           bSmSyncSequenceValid = FALSE;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    Checks communication and synchronisation variables and update AL status / AL status code if an error has occurred

*////////////////////////////////////////////////////////////////////////////////////////
void CheckIfEcatError(void)
{
   /*if the watchdog is enabled check the process data watchdog in the ESC
   and set the AL status code 0x1B if the watchdog expired*/
   if (EcatWdValue != 0)
   {
      /*watchdog time is set => watchdog is active*/
      UINT16 WdStatusOK = 0;

      HW_EscReadWord(WdStatusOK, ESC_PD_WD_STATE);
      WdStatusOK = SWAPWORD(WdStatusOK);

      if (!(WdStatusOK & ESC_PD_WD_TRIGGER_MASK) && (nPdOutputSize > 0))
      {
         /*The device is in OP state*/

         if (bEcatOutputUpdateRunning
            )
         {
            AL_ControlInd(STATE_SAFEOP, ALSTATUSCODE_SMWATCHDOG);
            return;
         }

         else
         {
            bEcatFirstOutputsReceived = FALSE;
         }
      }
   }

   if(bDcSyncActive)
   {
       if(bEcatOutputUpdateRunning)
       {
           /*Slave is in OP state*/
           if(!bDcRunning)
           {
               AL_ControlInd(STATE_SAFEOP, ALSTATUSCODE_FATALSYNCERROR);
               return;
           }
           else if(!bSmSyncSequenceValid)
           {
               AL_ControlInd(STATE_SAFEOP, ALSTATUSCODE_SYNCERROR);
               return;
           }
        
       }
   }
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    alStatus: requested state (ignored if the "alStatusCode" is 0)
 \param    alStatusCode: value for the AL-Status register
 
 \brief    This function changes the state of the EtherCAT slave if the requested state
             is lower than the actual state, otherwise the error condition will be reset.
*////////////////////////////////////////////////////////////////////////////////////////

void ECAT_StateChange(UINT8 alStatus, UINT16 alStatusCode)
{
    UINT8 Status = alStatus;

    /*ECATCHANGE_START(V5.13) ESM1*/
    /*return in case of invalid parameters*/
    if (alStatusCode != 0 && !STATE_VALID(alStatus))
    {
        return;
    }

    /* call the application requested state transition only once*/
    if (bEcatWaitForAlControlRes == FALSE && u8LocalErrorState == alStatus && u16LocalErrorCode == alStatusCode)
    {
        return;
    }
    /*ECATCHANGE_END(V5.13) ESM1*/


    if(bEcatWaitForAlControlRes)
    {
        /*State transition is pending*/

        if(bApplEsmPending)
        {
            /*The generic stack has currently control of the state transition.
            In case on an local error force ESM timeout*/
            if(alStatusCode != 0)
            {
                /*ECATCHANGE_START(V5.13) ESM1*/
                u8LocalErrorState = (alStatus & STATE_MASK);
                /*ECATCHANGE_END(V5.13) ESM1*/
                u16LocalErrorCode = alStatusCode;
                EsmTimeoutCounter = 0;
            }
            else
            { 
                u8LocalErrorState = 0;
                u16LocalErrorCode = alStatusCode;
            }
        }
        else
        {
            /*complete the state transition*/

            if(alStatusCode != 0)
            {
                /*ECATCHANGE_START(V5.13) ESM1*/
                u8LocalErrorState = (alStatus & STATE_MASK);
                /*ECATCHANGE_END(V5.13) ESM1*/
                u16LocalErrorCode = alStatusCode;

                /*State transition failed due to local application reasons*/
                switch(nEcatStateTrans)
                {
                    case INIT_2_PREOP:
                    case INIT_2_BOOT:
                     
                          APPL_StopMailboxHandler();
                          MBX_StopMailboxHandler();
                    break;
                    case PREOP_2_SAFEOP:
                          APPL_StopInputHandler();
                          StopInputHandler();
                    break;
                    case SAFEOP_2_OP:
                          APPL_StopOutputHandler();
                          StopOutputHandler();
                    break;
                }

                /*In case of a failed state transition the */
                Status =  (UINT8)(nEcatStateTrans >> 4);
            }
            else
            {
                /*State transition succeed*/
                 
                switch(nEcatStateTrans)
                {
                    case INIT_2_PREOP:
                    case INIT_2_BOOT:
                        bMbxRunning = TRUE;
                    break;
                    case PREOP_2_SAFEOP:
/*ECATCHANGE_START(V5.13) ECAT1*/
/*ECATCHANGE_END(V5.13) ECAT1*/
                        /* initialize the AL Event Mask register (0x204) */
                        SetALEventMask(u16ALEventMask);
                        bEcatInputUpdateRunning = TRUE;
                    break;
                    case SAFEOP_2_OP:
                          bEcatOutputUpdateRunning = TRUE;
                    break;
                }



                Status =  (UINT8)(nEcatStateTrans & STATE_MASK);
            }
                /*Pending state transition finished => write AL Status and AL Status Code*/
                bEcatWaitForAlControlRes = FALSE;

                if (alStatusCode != 0)
                {
                    Status |= STATE_CHANGE;
                }
/*ECATCHANGE_START(V5.13) ECAT3*/
                else if (u8LocalErrorState != 0)
                {
                    /*a local error is cleared*/
                    /*ECATCHANGE_START(V5.13) ESM1*/
                    u8LocalErrorState = 0;
                    /*ECATCHANGE_END(V5.13) ESM1*/
                    u16LocalErrorCode = 0x00;
                }
/*ECATCHANGE_END(V5.13) ECAT3*/

                SetALStatus(Status,alStatusCode);

        }/*state transition need to be completed by the local application*/
    }/*State transition pending*/
    else
    {
        /*ECATCHANGE_START(V5.13) ESM1*/
        if ( alStatusCode != 0 && ((alStatus & STATE_MASK) != STATE_OP) && STATE_VALID(alStatus))
        {
            u8LocalErrorState = (alStatus & STATE_MASK);
            /*ECATCHANGE_END(V5.13) ESM1*/
            u16LocalErrorCode = alStatusCode;

            /*trigger state transition only state transition from OP to lower state (for all other transitions the corresponding state transition functions shall be used)*/
            if ((nAlStatus & STATE_MASK) == STATE_OP)
            {
               /* no error pending and the target state is lower than the current one*/
                AL_ControlInd(alStatus, alStatusCode);
            }
        }
        /*ECATCHANGE_START(V5.13) ESM1*/
        else if (u8LocalErrorState != 0)
        {
            /*a local error is gone*/
            u8LocalErrorState = 0;
            u16LocalErrorCode = 0x00;
        }
        /*ECATCHANGE_END(V5.13) ESM1*/
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function initialize the EtherCAT Slave Interface.
*////////////////////////////////////////////////////////////////////////////////////////

void ECAT_Init(void)
{
    UINT8 i;
    /*Get Maximum Number of SyncManagers and supported DPRAM size*/
    {
    UINT16 TmpVar = 0;

    HW_EscReadWord(TmpVar, ESC_COMM_INFO_OFFSET);

    TmpVar = SWAPWORD(TmpVar);
    nMaxSyncMan = (UINT8) ((TmpVar & ESC_SM_CHANNELS_MASK)>> ESC_SM_CHANNELS_SHIFT);

    HW_EscReadWord(TmpVar, ESC_DPRAM_SIZE_OFFSET);
    TmpVar = SWAPWORD(TmpVar);

    //get max address (register + DPRAM size in Byte (in the register it is stored in KB))
    nMaxEscAddress = (UINT16) ((TmpVar & ESC_DPRAM_SIZE_MASK) << 10) + 0xFFF;
    }

/*ECATCHANGE_START(V5.13) ECAT2*/
    u16IdValue = 0;
/*ECATCHANGE_END(V5.13) ECAT2*/

    /* Get EEPROM loaded information */
    UpdateEEPROMLoadedState();

    /* disable all Sync Manager channels */
    for (i = 0; i < nMaxSyncMan; i++)
    {
        DisableSyncManChannel(i);
    }

    /* initialize the mailbox handler */
    MBX_Init();

    /* initialize variables */
    bApplEsmPending = FALSE;
    bEcatWaitForAlControlRes = FALSE;
    bEcatFirstOutputsReceived = FALSE;
     bEcatOutputUpdateRunning = FALSE;
     bEcatInputUpdateRunning = FALSE;
     bExplicitDevIdRequested = FALSE;
    bWdTrigger = FALSE;
    EcatWdValue = 0;
    Sync0WdCounter = 0;
    Sync0WdValue = 0;
    Sync1WdCounter = 0;
    Sync1WdValue = 0;
    bDcSyncActive = FALSE;

    /*ECATCHANGE_START(V5.13) ESM1*/
    u8LocalErrorState = 0;
    /*ECATCHANGE_END(V5.13) ESM1*/
    u16LocalErrorCode = 0x00;

    u16ALEventMask = 0;
    nPdOutputSize = 0;
    nPdInputSize = 0;

    /* initialize the AL Status register */
    nAlStatus    = STATE_INIT;
    SetALStatus(nAlStatus, 0);
    nEcatStateTrans = 0;
    u8EcatErrorLed = LED_OFF;

    bEscIntEnabled = FALSE;

    /* initialize the COE part */
    COE_Init();

/*ECATCHANGE_START(V5.13) ECAT1*/
/*ECATCHANGE_END(V5.13) ECAT1*/
    /*reset AL event mask*/
    ResetALEventMask(0);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief        This function has to be called cyclically.
*////////////////////////////////////////////////////////////////////////////////////////

void ECAT_Main(void)
{
    UINT16 ALEventReg;
    UINT16 EscAlControl = 0x0000;
/*ECATCHANGE_START(V5.13) MBX1*/
    UINT16 sm1Activate = SM_SETTING_ENABLE_VALUE;
    UINT16 sm1Status = 0; /*SM1 status need to be read (not MBX_READ_EVENT) to handle readframes with invalid CRCs*/
/*ECATCHANGE_END(V5.13) MBX1*/


    /* check if services are stored in the mailbox */
    MBX_Main();


    if ( bMbxRunning )
    {
        /* Slave is at least in PREOP, Mailbox is running */

/*ECATCHANGE_START(V5.13) MBX1*/
        /* get the Activate-Byte of SM 1 (Register 0x80E) to check if a mailbox repeat request was received */
        HW_EscReadWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
        sm1Activate = SWAPWORD(sm1Activate);

        HW_EscReadWord(sm1Status, (ESC_SYNCMAN_CONTROL_OFFSET + SIZEOF_SM_REGISTER));
        sm1Status = SWAPWORD(sm1Status);
/*ECATCHANGE_END(V5.13) MBX1*/
    }

    /* Read AL Event-Register from ESC */
    ALEventReg = HW_GetALEventRegister();
    ALEventReg = SWAPWORD(ALEventReg);


    if ((ALEventReg & AL_CONTROL_EVENT) && !bEcatWaitForAlControlRes)
    {
        /* AL Control event is set, get the AL Control register sent by the Master to acknowledge the event
          (that the corresponding bit in the AL Event register will be reset) */

        HW_EscReadWord( EscAlControl, ESC_AL_CONTROL_OFFSET);
        EscAlControl = SWAPWORD(EscAlControl);


            /*ECATCHANGE_START(V5.13) ECAT2*/
                /*Evaluate if register 0x120 Bit5 (Request Explicit DeviceID) is set*/
            if ((EscAlControl & (UINT16)STATE_DEVID) == STATE_DEVID)
            {
                if (bExplicitDevIdRequested == FALSE)
                {
                    u16IdValue = APPL_GetDeviceID();
                }

                bExplicitDevIdRequested = TRUE;
            }
            else
            {
                bExplicitDevIdRequested = FALSE;
            }
            /*ECATCHANGE_END(V5.13) ECAT2*/

        /* reset AL Control event and the SM Change event (because the Sync Manager settings will be checked
           in AL_ControlInd, too)*/
            ALEventReg &= ~((AL_CONTROL_EVENT) | (SM_CHANGE_EVENT));

            AL_ControlInd((UINT8)EscAlControl, 0); /* in AL_ControlInd the state transition will be checked and done */

            /* SM-Change-Event was handled too */

    }

    if ( (ALEventReg & SM_CHANGE_EVENT) && !bEcatWaitForAlControlRes && (nAlStatus & STATE_CHANGE) == 0 && (nAlStatus & ~STATE_CHANGE) != STATE_INIT )
    {
        /* the SM Change event is set (Bit 4 of Register 0x220), when the Byte 6 (Enable, Lo-Byte of Register 0x806, 0x80E, 0x816,...)
           of a Sync Manager channel was written */
        ALEventReg &= ~(SM_CHANGE_EVENT);

        /* AL_ControlInd is called with the actual state, so that the correct SM settings will be checked */
        AL_ControlInd(nAlStatus & STATE_MASK, 0);
    }

    if(bEcatWaitForAlControlRes)
    {
        AL_ControlRes();
    }
    /*The order of mailbox event processing was changed to prevent race condition errors.
        The SM1 activate Byte (Register 0x80E) was read before reading AL Event register.
        1. Handle Mailbox Read event
        2. Handle repeat toggle request
        3. Handle Mailbox write event
    */
    if ( bMbxRunning )
    {
        /*SnycManger change event (0x220:4) could be acknowledged by reading the SM1 control register without notification to the local application
        => check if the SyncManger 1 is still enabled*/
            if (!(sm1Activate & SM_SETTING_ENABLE_VALUE))
            {
                AL_ControlInd(nAlStatus & STATE_MASK, 0);
            }

/*ECATCHANGE_START(V5.13) MBX1*/
        if (((sm1Status & SM_STATUS_MBX_BUFFER_FULL) == 0)
            && bSendMbxIsFull) 
/*ECATCHANGE_END(V5.13) MBX1*/
        {
            /* SM 1 (Mailbox Read) event is set, when the mailbox was read from the master,
               to acknowledge the event the first byte of the mailbox has to be written,
               by writing the first byte the mailbox is locked, too */
            u16dummy = 0;
            HW_EscWriteWord(u16dummy,u16EscAddrSendMbx);

            /* the Mailbox Read event in the variable ALEventReg shall be reset before calling
               MBX_MailboxReadInd, where a new mailbox datagram (if available) could be stored in the send mailbox */
            ALEventReg &= ~(MAILBOX_READ_EVENT);
            MBX_MailboxReadInd();
        }

            /* bMbxRepeatToggle holds the last state of the Repeat Bit (Bit 1) */

            if (((sm1Activate & SM_SETTING_REPAET_REQ_MASK) && !bMbxRepeatToggle)
                || (!(sm1Activate & SM_SETTING_REPAET_REQ_MASK) && bMbxRepeatToggle))
            {
                /* Repeat Bit (Bit 1) has toggled, there is a repeat request, in MBX_MailboxRepeatReq the correct
                   response will put in the send mailbox again */
                MBX_MailboxRepeatReq();
                /* acknowledge the repeat request after the send mailbox was updated by writing the Repeat Bit
                   in the Repeat Ack Bit (Bit 1) of the PDI Ctrl-Byte of SM 1 (Register 0x80F) */
                if (bMbxRepeatToggle)
                {
                    sm1Activate |= SM_SETTING_REPEAT_ACK; //set repeat acknowledge bit (bit 9)
                }
                else
                {
                    sm1Activate &= ~SM_SETTING_REPEAT_ACK; //clear repeat acknowledge bit (bit 9)
                }

                sm1Activate = SWAPWORD(sm1Activate);
                HW_EscWriteWord(sm1Activate, (ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
            }


        /* Reload the AlEvent because it may be changed due to a SM disable, enable in case of an repeat request */
        ALEventReg = HW_GetALEventRegister();
        ALEventReg = SWAPWORD(ALEventReg);

        if ( ALEventReg & (MAILBOX_WRITE_EVENT) )
        {
            /* SM 0 (Mailbox Write) event is set, when the mailbox was written from the master,
               to acknowledge the event the first byte of the mailbox has to be read,
               which will be done in MBX_CheckAndCopyMailbox */
            /* the Mailbox Write event in the variable ALEventReg shall be reset before calling
               MBX_CheckAndCopyMailbox, where the received mailbox datagram will be processed */
            ALEventReg &= ~(MAILBOX_WRITE_EVENT);
            MBX_CheckAndCopyMailbox();

        }
    }
}


/** @} */

