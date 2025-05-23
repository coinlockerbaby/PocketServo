/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
 * \addtogroup ESM EtherCAT State Machine
 * @{
 */

/**
\file ecatslv.h
\author EthercatSSC@beckhoff.com

\version 5.13

<br>Changes to version V5.12:<br>
V5.13 EMCY1: update emergency diagcodes for invalid sm configuration<br>
V5.13 ESM1: local error handling update, ECAT_StateChange triggers only transitions from Op->Any or reject/accept a pending transition<br>
<br>Changes to version V5.11:<br>
V5.12 COE4: add timestamp object (0x10F8) and update diagnosis handling<br>
V5.12 ECAT6: remove mailbox SyncManager if no mailbox is supported (SM0 Output, SM1 Input)<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 ECAT4: enhance SM/Sync monitoring for input/output only slaves<br>
V5.11 ECAT9: add new AL Status Codes (ALSTATUSCODE_FW_SII_NOT_MATCH/0x0006, ALSTATUSCODE_FW_UPDATE_FAILED / 0x0007)<br>
V5.11 ESM7: "add Sync define for 0x22 (""SYNCTYPE_SM2_SYNCHRON""), support value 0x22 for 0x1C33.1 (SM2 sync)"<br>
V5.11 HW1: "move hardware independent functions ""HW_DisableSyncManChannel()"", ""HW_EnableSyncManChannel()"", ""HW_GetSyncMan()"", ""HW_ResetALEventMask()"", ""HW_SetALEventMask()"" to ecatalv.c"<br>
<br>Changes to version V5.01:<br>
V5.10 COE1: Define one entry description for all 0x1C3x objects and change data type of SI11,12,13 to UINT16 (according ETG.1020)<br>
V5.10 ECAT12: Add new AL Status Codes (0x2E,0x52,0x70)<br>
V5.10 ECAT13: Update Synchronisation handling (FreeRun,SM Sync, Sync0, Sync1)<br>
              Compare DC UINT configuration (by ESC Config data) vs. DC activation register settings<br>
              Update 0x1C3x entries<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#include "ecat_def.h"

#include  "esc.h"
#include "aw_ecat_ssc.h"

#ifndef _ECATSLV_H_
#define _ECATSLV_H_


/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

#ifndef OBJGETNEXTSTR
    /* the old definition was not working with all compilers */
    /* old:    #define    OBJGETNEXTSTR(p)    ( (OBJCONST CHAR OBJMEM * )( ((UINT32) p) + OBJSTRLEN( (OBJCONST CHAR OBJMEM *) p ) + 1 ) ) */
    #define    OBJGETNEXTSTR(p)    ( (OBJCONST CHAR OBJMEM * )(  &((p)[OBJSTRLEN( (OBJCONST CHAR OBJMEM *) (p) ) + 1]) ) ) /**< \brief Macro to get next name within the objetc name string*/
#endif

#ifndef LO_BYTE
    #define    LO_BYTE                            0 /**< \brief LowByte within an WORD*/
#endif

#ifndef HI_BYTE
    #define    HI_BYTE                            1 /**< \brief HighByte within an WORD*/
#endif

#ifndef LOLO_BYTE
    #define    LOLO_BYTE                          0 /**< \brief LowLowByte within an DWORD*/
#endif

#ifndef LOHI_BYTE
    #define    LOHI_BYTE                          1 /**< \brief LowHighByte within an DWORD*/
#endif

#ifndef HILO_BYTE
    #define    HILO_BYTE                         2 /**< \brief HighLowByte within an DWORD*/
#endif

#ifndef HIHI_BYTE
    #define    HIHI_BYTE                          3 /**< \brief HighHighByte within an DWORD*/
#endif

#ifndef LO_WORD
    #define    LO_WORD                            0 /**< \brief LowWord within an DWORD*/
#endif

#ifndef HI_WORD
    #define    HI_WORD                            1 /**< \brief HighWord within an DWORD*/
#endif

#ifndef SWAPWORD
    #define    SWAPWORD(x)                        (x) /**< \brief Macro to swap a WORD*/
#endif

#ifndef SWAPDWORD
    #define    SWAPDWORD(x)                     (x) /**< \brief Macro to swap a DWORD*/
#endif

#ifndef LOBYTE
    #define    LOBYTE(x)                        ((x)&0xFF) /**< \brief LowByte mask*/
#endif

#ifndef HIBYTE
    #define    HIBYTE(x)                        (((x)&0xFF00)>>8)/**< \brief HighwByte mask*/
#endif

#ifndef LOLOBYTE
    #define    LOLOBYTE(x)                        ((x)&0xFF)/**< \brief LowLowByte mask*/
#endif

#ifndef LOHIBYTE
    #define    LOHIBYTE(x)                        (((x)&0xFF00)>>8)/**< \brief LowHighByte mask*/
#endif

#ifndef HILOBYTE
    #define    HILOBYTE(x)                        (((x)&0xFF0000)>>16)/**< \brief HighLowByte mask*/
#endif

#ifndef HIHIBYTE
    #define    HIHIBYTE(x)                        (((x)&0xFF000000)>>24)/**< \brief HighHighByte mask*/
#endif

#ifndef LOWORD
    #define    LOWORD(x)                        ((x)&0xFFFF)/**< \brief LowWord mask*/
#endif

#ifndef HIWORD
    #define    HIWORD(x)                        (((x)&0xFFFF0000)>>16)/**< \brief HighByte mask*/
#endif

#ifndef BIT2BYTE
    #define    BIT2BYTE(x)        (((x)+7)>>3) /**< \brief Macro to convert Bit size to Byte size (round up)*/
#endif

#ifndef BYTE2BIT
    #define    BYTE2BIT(x)        ((x)<<3) /**< \brief Macro to convert Byte size to Bit size*/
#endif

#ifndef BIT2WORD
    #define    BIT2WORD(x)        (((x)+15)>>4) /**< \brief Macro to convert Bit size to Word size (round up)*/
#endif

#ifndef BYTE2WORD
    #define    BYTE2WORD(x)    (((x)+1)>>1) /**< \brief Macro to convert Bytes size to Word size (round up)*/
#endif

#ifndef ROUNDUPBYTE2WORD
    #define    ROUNDUPBYTE2WORD(x)    ((((x)+1)>>1)<<1) /**< \brief Round up Byte size to even number of Words*/
#endif


/*---------------------------------------------
-    State defines
-----------------------------------------------*/
#define     STATE_INIT                        ((UINT8) 0x01) /**< \brief State INIT*/
#define     STATE_PREOP                       ((UINT8) 0x02) /**< \brief State PreOP*/
#define     STATE_BOOT                        ((UINT8) 0x03) /**< \brief State BOOT*/
#define     STATE_SAFEOP                      ((UINT8) 0x04) /**< \brief State SafeOP*/
#define     STATE_OP                          ((UINT8) 0x08) /**< \brief State OP*/

#define     STATE_MASK                        ((UINT8) 0x0F) /**< \brief State mask*/
#define     STATE_CHANGE                      ((UINT8) 0x10) /**< \brief State change (Error indication/acknowledge) mask and value*/
#define     STATE_DEVID                       ((UINT8) 0x20) /**< \brief Request/response Explicit device ID*/
#define     STATE_VALID(x)                    ((((x) & STATE_MASK) == STATE_INIT) || (((x) & STATE_MASK) == STATE_PREOP) || (((x) & STATE_MASK) == STATE_SAFEOP) || (((x) & STATE_MASK) == STATE_OP)) /**< \brief Check if one of the mandatory supported state values are set*/

#define     BOOT_2_INIT                       ((UINT8)((STATE_BOOT) << 4) | (STATE_INIT)) /**< \brief State transition BOOT to INIT*/
#define     BOOT_2_PREOP                      ((UINT8)((STATE_BOOT) << 4) | (STATE_PREOP)) /**< \brief State transition BOOT to INIT*/
#define     BOOT_2_SAFEOP                     ((UINT8)((STATE_BOOT) << 4) | (STATE_SAFEOP)) /**< \brief State transition BOOT to INIT*/
#define     BOOT_2_OP                         ((UINT8)((STATE_BOOT) << 4) | (STATE_OP)) /**< \brief State transition BOOT to INIT*/

#define     INIT_2_BOOT                       ((UINT8)((STATE_INIT) << 4) | (STATE_BOOT)) /**< \brief State transition INIT to BOOT*/
#define     PREOP_2_BOOT                      ((UINT8)((STATE_PREOP) << 4) | (STATE_BOOT)) /**< \brief State transition PREOP to BOOT*/
#define     SAFEOP_2_BOOT                     ((UINT8)((STATE_SAFEOP) << 4) | (STATE_BOOT)) /**< \brief State transition SAFEOP to BOOT*/
#define     OP_2_BOOT                         ((UINT8)((STATE_OP) << 4) | (STATE_BOOT)) /**< \brief State transition OP to BOOT*/

#define     INIT_2_INIT                       ((UINT8)((STATE_INIT) << 4) | (STATE_INIT)) /**< \brief State transition INIT to INIT*/
#define     INIT_2_PREOP                      ((UINT8)((STATE_INIT) << 4) | (STATE_PREOP)) /**< \brief State transition INIT to PREOP*/
#define     INIT_2_SAFEOP                     ((UINT8)((STATE_INIT) << 4) | (STATE_SAFEOP)) /**< \brief State transition INIT to SAFEOP*/
#define     INIT_2_OP                         ((UINT8)((STATE_INIT) << 4) | (STATE_OP)) /**< \brief State transition INIT to OP*/

#define     PREOP_2_INIT                      ((UINT8)((STATE_PREOP) << 4) | (STATE_INIT)) /**< \brief State transition PREOP to INIT*/
#define     PREOP_2_PREOP                     ((UINT8)((STATE_PREOP) << 4) | (STATE_PREOP)) /**< \brief State transition PREOP to PREOP*/
#define     PREOP_2_SAFEOP                    ((UINT8)((STATE_PREOP) << 4) | (STATE_SAFEOP)) /**< \brief State transition PREOP to SAFEOP*/
#define     PREOP_2_OP                        ((UINT8)((STATE_PREOP) << 4) | (STATE_OP)) /**< \brief State transition PREOP to OP*/

#define     SAFEOP_2_INIT                     ((UINT8)((STATE_SAFEOP) << 4) | (STATE_INIT)) /**< \brief State transition SAFEOP to INIT*/
#define     SAFEOP_2_PREOP                    ((UINT8)((STATE_SAFEOP) << 4) | (STATE_PREOP)) /**< \brief State transition SAFEOP to PREOP*/
#define     SAFEOP_2_SAFEOP                   ((UINT8)((STATE_SAFEOP) << 4) | (STATE_SAFEOP)) /**< \brief State transition SAFEOP to SAFEOP*/
#define     SAFEOP_2_OP                       ((UINT8)((STATE_SAFEOP) << 4) |( STATE_OP)) /**< \brief State transition SAFEOP to OP*/

#define     OP_2_INIT                         ((UINT8)((STATE_OP) << 4) | (STATE_INIT)) /**< \brief State transition OP to INIT*/
#define     OP_2_PREOP                        ((UINT8)((STATE_OP) << 4) | (STATE_PREOP)) /**< \brief State transition OP to PREOP*/
#define     OP_2_SAFEOP                       ((UINT8)((STATE_OP) << 4) | (STATE_SAFEOP)) /**< \brief State transition OP to SAFEOP*/
#define     OP_2_OP                           ((UINT8)((STATE_OP) << 4) | (STATE_OP)) /**< \brief State transition OP to OP*/


/*-------------------------------------------------------------------------------------------------------------------------

 ESM transition error codes

*/
/*ECATCHANGE_START(V5.13) EMCY1*/
#define    SYNCMANCHSIZE                                            0x00 /**< \brief Emergency and Diagnosis code for an invalid  SyncManager size*/
#define    SYNCMANCHADDRESS                                         0x01 /**< \brief Emergency and Diagnosis code for an invalid SyncManager address*/
#define    SYNCMANCHSETTINGS                                        0x02 /**< \brief Emergency and Diagnosis code for an invalid SyncManager settings*/
#define    ERROR_SYNCMANCH(code, channel)                    ((code)+((channel)<<2)) /**< \brief Macro to add SyncManager channel*/

/*ECATCHANGE_END(V5.13) EMCY1*/
#define    ERROR_SYNCMANCHADDRESS(channel)                    ((SYNCMANCHADDRESS)+((channel)<<2)) /**< \brief Macro to add SyncManager channel*/
#define    ERROR_SYNCMANCHSIZE(channel)                          ((SYNCMANCHSIZE)+((channel)<<2)) /**< \brief Macro to add SyncManager channel*/
#define    ERROR_SYNCMANCHSETTINGS(channel)                    ((SYNCMANCHSETTINGS)+((channel)<<2)) /**< \brief Macro to add SyncManager channel*/
#define    ERROR_SYNCTYPES                                        0x80 /**< \brief Emergency and Diagnosis code for an invalid Sync types*/
#define    ERROR_DCSYNCCONTROL                                    0x81 /**< \brief Emergency and Diagnosis code for an invalid DC Sync control*/
#define    ERROR_DCSYNC0CYCLETIME                                0x82 /**< \brief Emergency and Diagnosis code for an invalid Sync0 cycle time*/
#define    ERROR_DCSYNC1CYCLETIME                                0x83 /**< \brief Emergency and Diagnosis code for an invalid Sync1 cycle time*/
#define    ERROR_DCCYCLEPARAMETER                                0x84 /**< \brief Emergency and Diagnosis code for an invalid Cycle parameter*/
#define    ERROR_DCLATCHCONTROL                                    0x85 /**< \brief Emergency and Diagnosis code for an invalid Latch control*/

#define    ERROR_INVALIDSTATE                                    0xF0 /**< \brief Emergency and Diagnosis code for an invalid State*/
#define    ERROR_NOMEMORY                                            0xF1 /**< \brief Emergency and Diagnosis code for no memory*/
#define    ERROR_OBJECTDICTIONARY                                0xF2 /**< \brief Emergency and Diagnosis code for generic object dictionary error*/
#define    ERROR_NOSYNCMANACCESS                                0xF3 /**< \brief Emergency and Diagnosis code for no SyncManager access*/
#define    ERROR_NOOFRXPDOS                                        0xF4 /**< \brief Emergency and Diagnosis code for no RxPDOs*/
#define    ERROR_NOOFTXPDOS                                        0xF5 /**< \brief Emergency and Diagnosis code for no TxPDOs*/
#define    ERROR_STATECHANGE                                        0xF6 /**< \brief Emergency and Diagnosis code state change error*/

#define    NOERROR_NOSTATECHANGE                                0xFE /**< \brief Indicates no state change*/
#define    NOERROR_INWORK                                            0xFF /**< \brief Indication for no error but operation is pending*/

#define    EMCY_SM_ERRORCODE                                        0xA000 /**< \brief Emergency and Diagnosis code for an SyncManager error*/
#define    EMCY_SM_DEVICESPECIFIC                                0xFF00 /**< \brief Emergency and Diagnosis code for a device specific error*/


/*---------------------------------------------
-    AL Status Codes
-----------------------------------------------*/
#define    ALSTATUSCODE_NOERROR                        0x0000 /**< \brief No error*/
#define    ALSTATUSCODE_UNSPECIFIEDERROR               0x0001 /**< \brief Unspecified error*/
#define    ALSTATUSCODE_NOMEMORY                       0x0002 /**< \brief No Memory*/
#define    ALSTATUSCODE_INVALID_REVISION               0x0004 /**< \brief Output/Input mapping is not valid for this hardware or software revision (0x1018:03)*/
#define    ALSTATUSCODE_FW_SII_NOT_MATCH               0x0006 /**< \brief Firmware and EEPROM do not match. Slave needs BOOT-INIT transition*/
#define    ALSTATUSCODE_FW_UPDATE_FAILED               0x0007 /**< \brief Firmware update not successful. Old firmware still running*/
#define    ALSTATUSCODE_INVALIDALCONTROL               0x0011 /**< \brief Invalid requested state change*/
#define    ALSTATUSCODE_UNKNOWNALCONTROL               0x0012 /**< \brief Unknown requested state*/
#define    ALSTATUSCODE_BOOTNOTSUPP                    0x0013 /**< \brief Bootstrap not supported*/
#define    ALSTATUSCODE_NOVALIDFIRMWARE                0x0014 /**< \brief No valid firmware*/
#define    ALSTATUSCODE_INVALIDMBXCFGINBOOT            0x0015 /**< \brief Invalid mailbox configuration (BOOT state)*/
#define    ALSTATUSCODE_INVALIDMBXCFGINPREOP           0x0016 /**< \brief Invalid mailbox configuration (PreOP state)*/
#define    ALSTATUSCODE_INVALIDSMCFG                   0x0017 /**< \brief Invalid sync manager configuration*/
#define    ALSTATUSCODE_NOVALIDINPUTS                  0x0018 /**< \brief No valid inputs available*/
#define    ALSTATUSCODE_NOVALIDOUTPUTS                 0x0019 /**< \brief No valid outputs*/
#define    ALSTATUSCODE_SYNCERROR                      0x001A /**< \brief Synchronization error*/
#define    ALSTATUSCODE_SMWATCHDOG                     0x001B /**< \brief Sync manager watchdog*/
#define    ALSTATUSCODE_SYNCTYPESNOTCOMPATIBLE         0x001C /**< \brief Invalid Sync Manager Types*/
#define    ALSTATUSCODE_INVALIDSMOUTCFG                0x001D /**< \brief Invalid Output Configuration*/
#define    ALSTATUSCODE_INVALIDSMINCFG                 0x001E /**< \brief Invalid Input Configuration*/
#define    ALSTATUSCODE_INVALIDWDCFG                   0x001F /**< \brief Invalid Watchdog Configuration*/
#define    ALSTATUSCODE_WAITFORCOLDSTART               0x0020 /**< \brief Slave needs cold start*/
#define    ALSTATUSCODE_WAITFORINIT                    0x0021 /**< \brief Slave needs INIT*/
#define    ALSTATUSCODE_WAITFORPREOP                   0x0022 /**< \brief Slave needs PREOP*/
#define    ALSTATUSCODE_WAITFORSAFEOP                  0x0023 /**< \brief Slave needs SAFEOP*/
#define    ALSTATUSCODE_INVALIDINPUTMAPPING            0x0024 /**< \brief Invalid Input Mapping*/
#define    ALSTATUSCODE_INVALIDOUTPUTMAPPING           0x0025 /**< \brief Invalid Output Mapping*/
#define    ALSTATUSCODE_INCONSISTENTSETTINGS           0x0026 /**< \brief Inconsistent Settings*/
#define    ALSTATUSCODE_FREERUNNOTSUPPORTED            0x0027 /**< \brief FreeRun not supported*/
#define    ALSTATUSCODE_SYNCHRONNOTSUPPORTED           0x0028 /**< \brief SyncMode not supported*/
#define    ALSTATUSCODE_FREERUNNEEDS3BUFFERMODE        0x0029 /**< \brief FreeRun needs 3Buffer Mode*/
#define    ALSTATUSCODE_BACKGROUNDWATCHDOG             0x002A /**< \brief Background Watchdog*/
#define    ALSTATUSCODE_NOVALIDINPUTSANDOUTPUTS        0x002B /**< \brief No Valid Inputs and Outputs*/
#define    ALSTATUSCODE_FATALSYNCERROR                 0x002C /**< \brief Fatal Sync Error*/
#define    ALSTATUSCODE_NOSYNCERROR                    0x002D /**< \brief No Sync Error*/
#define    ALSTATUSCODE_CYCLETIMETOOSMALL              0x002E /**< \brief EtherCAT cycle time smaller Minimum Cycle Time supported by slave*/
#define    ALSTATUSCODE_DCINVALIDSYNCCFG               0x0030 /**< \brief Invalid DC SYNCH Configuration*/
#define    ALSTATUSCODE_DCINVALIDLATCHCFG              0x0031 /**< \brief Invalid DC Latch Configuration*/
#define    ALSTATUSCODE_DCPLLSYNCERROR                 0x0032 /**< \brief PLL Error*/
#define    ALSTATUSCODE_DCSYNCIOERROR                  0x0033 /**< \brief DC Sync IO Error*/
#define    ALSTATUSCODE_DCSYNCMISSEDERROR              0x0034 /**< \brief DC Sync Timeout Error*/
#define    ALSTATUSCODE_DCINVALIDSYNCCYCLETIME         0x0035 /**< \brief DC Invalid Sync Cycle Time*/
#define    ALSTATUSCODE_DCSYNC0CYCLETIME               0x0036 /**< \brief DC Sync0 Cycle Time*/
#define    ALSTATUSCODE_DCSYNC1CYCLETIME               0x0037 /**< \brief DC Sync1 Cycle Time*/
#define    ALSTATUSCODE_MBX_AOE                        0x0041 /**< \brief MBX_AOE*/
#define    ALSTATUSCODE_MBX_EOE                        0x0042 /**< \brief MBX_EOE*/
#define    ALSTATUSCODE_MBX_COE                        0x0043 /**< \brief MBX_COE*/
#define    ALSTATUSCODE_MBX_FOE                        0x0044 /**< \brief MBX_FOE*/
#define    ALSTATUSCODE_MBX_SOE                        0x0045 /**< \brief MBX_SOE*/
#define    ALSTATUSCODE_MBX_VOE                        0x004F /**< \brief MBX_VOE*/
#define    ALSTATUSCODE_EE_NOACCESS                    0x0050 /**< \brief EEPROM no access*/
#define    ALSTATUSCODE_EE_ERROR                       0x0051 /**< \brief EEPROM Error*/
#define    ALSTATUSCODE_EXT_HARDWARE_NOT_READY         0x0052 /**< \brief External hardware not ready. This AL Status Code should be used if the EtherCAT-Slave refused the state transition due to an external connection to another device or signal is missing*/
#define    ALSTATUSCODE_DEVICE_IDENT_VALUE_UPDATED     0x0061 /**< \brief In legacy identification mode (dip switch mapped to register 0x12) this error is returned if the EEPROM ID value does not match to dipswitch value*/
#define    ALSTATUSCODE_MODULE_ID_LIST_NOT_MATCH       0x0070 /**< \brief Detected Module Ident List (0xF030) and Configured Module Ident List (0xF050) does not match*/
#define    ALSTATUSCODE_SUPPLY_VOLTAGE_TOO_LOW         0x0080 /**< \brief The slave supply voltage is too low*/
#define    ALSTATUSCODE_SUPPLY_VOLTAGE_TOO_HIGH        0x0081 /**< \brief The slave supply voltage is too high*/
#define    ALSTATUSCODE_TEMPERATURE_TOO_LOW            0x0082 /**< \brief The slave temperature is too low*/
#define    ALSTATUSCODE_TEMPERATURE_TOO_HIGH           0x0083 /**< \brief The slave temperature is too high*/
                                                              

                                                              
/*---------------------------------------------
-    Configured Sync Type (0x1C32.1 / 0x1C33.1)
-----------------------------------------------*/
#define     SYNCTYPE_FREERUN            0x0000 /**< \brief Sync type FreeRun*/
#define     SYNCTYPE_SM_SYNCHRON        0x0001 /**< \brief SyncManager synchron (synchron to the corresponding SM, 0x1C32.1 -> SM2 ; 0x1C33.1 -> SM3)  */
#define     SYNCTYPE_SM2_SYNCHRON       0x0022 /**< \brief SyncManager2 synchron (only used for 0x1C33.1)*/
#define     SYNCTYPE_DCSYNC0            0x0002 /**< \brief Sync type Sync0 synchron*/
#define     SYNCTYPE_DCSYNC1            0x0003 /**< \brief Sync type Sync1 synchron*/


/*---------------------------------------------
-    AL Event masks
-----------------------------------------------*/
#define     AL_CONTROL_EVENT                    ((UINT16) 0x01) /**< \brief AL Control event*/
#define     SYNC0_EVENT                         ((UINT16) 0x04) /**< \brief Sync0 event*/
#define     SYNC1_EVENT                         ((UINT16) 0x08) /**< \brief Sync1 event*/
#define     SM_CHANGE_EVENT                     ((UINT16) 0x10) /**< \brief SyncManager changed event*/
#define     EEPROM_CMD_PENDING                  ((UINT16) 0x20) /**< \brief EEPROM command pending*/
#define     MAILBOX_WRITE_EVENT                 ((UINT16) 0x0100) /**< \brief MBoxOut write event*/
#define     MAILBOX_READ_EVENT                  ((UINT16) 0x0200) /**< \brief MBoxIn read event*/
#define     PROCESS_OUTPUT_EVENT                ((UINT16) 0x0400) /**< \brief Output process data write event*/
#define     PROCESS_INPUT_EVENT                 ((UINT16) 0x0800) /**< \brief Input process data read event*/

#define    MAILBOX_WRITE                        0 /**< \brief SyncManager ID for MBoxOut (master to slave)*/
#define    MAILBOX_READ                         1 /**< \brief SyncManager ID for MBoxIn (slave to master)*/
#define    PROCESS_DATA_OUT                     2 /**< \brief SyncManager ID for output process data (master to slave)*/
#define    PROCESS_DATA_IN                      3 /**< \brief SyncManager ID for input process data (slave to master)*/




/*---------------------------------------------
-     Codes for LED
      bit 7: invert flag
      bit 6: toggle    (if toggle == 1 and number of flashes == 0 => infinite toggle)
      bit 5: fast toggle flag (50ms cycle)
      bit 4-0: number of flashes
-----------------------------------------------*/
#define    LED_OFF                      0x00 /**< \brief LED off*/
#define    LED_FLICKERING               0x60 /**< \brief LED flickering*/
#define    LED_BLINKING                 0x40 /**< \brief LED blinking*/
#define    LED_SINGLEFLASH              0x41 /**< \brief LED single flash*/
#define    LED_DOUBLEFLASH              0x42 /**< \brief LED double flash*/
#define    LED_INVERT_DOUBLEFLASH       0xC2 /**< \brief LED invert double flash*/
#define    LED_ON                       0x01 /**< \brief LED on*/



#define    MEMORY_START_ADDRESS            0x1000 /**< \brief ESC DPRAM start address*/


#ifndef    DC_SYNC_ACTIVE
    #define    DC_SYNC_ACTIVE                ESC_DC_SYNC0_ACTIVE_MASK /**< \brief Expected activated Sync signals by the master*/
#endif
#ifndef    DC_EVENT_MASK
    #define    DC_EVENT_MASK                PROCESS_OUTPUT_EVENT /**< \brief AL Event mask (value of register 0x204) in case of DC synchronisation*/
#endif


#endif //_ECATSLV_H_

#if defined(_ECATSLV_) && (_ECATSLV_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif
/*-----------------------------------------------------------------------------------------
------
------    Global Variables
------
-----------------------------------------------------------------------------------------*/
PROTO    BOOL                           bEcatOutputUpdateRunning;  /**< \brief Indicates the OP state, will be set in StartOutputHandler
                                                                                and reset in StopOutputHandler*/

PROTO    BOOL                            bEcatInputUpdateRunning;  /**< \brief Indicates the SAFEOP or OP state, will be set in StartInputHandler 
                                                                                and reset in Stop InputHandler*/

PROTO    BOOL                           bEcatFirstOutputsReceived; /**< \brief Indicates if outputs were received (SM2-event)
                                                                                or inputs were read (SM3-event, if the output size is 0),
                                                                                has to be set by the application and reset in StopOutputHandler*/

PROTO    BOOL                           bWdTrigger; /**< \brief Indicates that the SM2 WD trigger bit (0x814 bit6) is set.*/

PROTO    BOOL                           bDcSyncActive; /**< \brief Indicates that the Distributed Clocks synchronization is active,*/

PROTO    INT16                          EsmTimeoutCounter; /**< \brief Counter used to detect an ESM timeout.
                                                           -1 indicates a deactivated counter and 0 is expired*/

#define MAX_SM_EVENT_MISSED             4 /**< \brief threshold of max missed counter value (0x1C32.11 / 0x1C33.11)*/

PROTO BOOL                              bDcRunning; /**< \brief Indicates if Sync0 events are received*/

PROTO UINT16                            u16SmSync0Counter; /**< /brief Incremented by one on every Sync0 event and reset to 0 on every SM event. It is used to check the SM/Sync0 sequence */
PROTO UINT16                            u16SmSync0Value; /**< /brief Allowed Sync0 events within one SM cycle. If 0 the Sequence check is disabled */

PROTO BOOL                              bSmSyncSequenceValid; /**< \brief Set to true if SM/Sync0 sequence is valid*/

PROTO INT16                             i16WaitForPllRunningTimeout; /**< \brief The time bPllRunnig shall be true while state change from SafeOp to Op*/

PROTO INT16                             i16WaitForPllRunningCnt; /**< \brief Incremented with every valid Sync-SyncManger cycle */

PROTO UINT16                            Sync0WdCounter; /**< \brief Sync0 watchdog counter*/
PROTO UINT16                            Sync0WdValue; /**< \brief Sync0 watchdog value*/

PROTO UINT16                            Sync1WdCounter; /**< \brief Sync1 watchdog counter*/
PROTO UINT16                            Sync1WdValue; /**< \brief Sync1 watchdog value*/

PROTO UINT16                            LatchInputSync0Value; /**< \brief Sync0 event on which the inputs shall be latched and copied to the ESC buffer. If the inputs shall be latched base don Sync1 the value is set to 0.*/
PROTO UINT16                            LatchInputSync0Counter; /**< \brief Sync0 counter used to get the Sync0 event to latch the Inputs. */

PROTO BOOL b32BitDc;



PROTO BOOL                              bEscIntEnabled; /**< \brief Indicates that the ESC interrupt is enabled (SM2/3 or SYNC0/1-event),
                                                                     will be set in StartInputHandler and reset in StopInputHandler*/

PROTO BOOL                              b3BufferMode; /**< \brief Indicates that inputs and outputs are running in 3-Buffer-Mode*/

/*ECATCHANGE_START(V5.13) ESM1*/
PROTO UINT16                            u16LocalErrorCode; /**< \brief Reason for local error*/
PROTO UINT8                             u8LocalErrorState; /**< \brief State of the current local error (lower state could be set by the master)*/
/*ECATCHANGE_END(V5.13) ESM1*/
PROTO BOOL                              bApplEsmPending; /**< \brief Indicates if the local application ESM function need to be called from Al_ConntrolRes (is true if NOERR_INWORK is returned by generic ESM function)*/
PROTO BOOL                              bEcatWaitForAlControlRes; /**< \brief Contains the information that the state machine waits for an acknowledge
                                                                             for the last AL_ControlInd from the application/generic stack*/

PROTO UINT16                            nEcatStateTrans; /**< \brief Current state transition*/

PROTO UINT8                             u8EcatErrorLed; /**< \brief Current value of the error LED*/

PROTO UINT8                             u8EcatRunLed; /**< \brief Current value of the run LED*/

PROTO UINT16                            nPdInputSize; /**< \brief Contains the input size (SM3 size)/SM2 if no outputs are supported, has to be written by the application*/

PROTO UINT16                            nPdOutputSize; /**< \brief Contains the output size (SM2 size), has to be written by the application*/

PROTO UINT8                             nMaxSyncMan; /**< \brief Contains the maximum number of Sync Manager channels, will be initialized in ECAT_Main*/

PROTO UINT16                            nMaxEscAddress; /**< \brief Max supported address by the ESC (register(0x1000) + size of the supported DPRAM in Byte)*/

PROTO UINT8                             nAlStatus; /**< \brief Contains the actual AL Status, will be written in AL_ControlInd*/

PROTO BOOL                              bExplicitDevIdRequested; /**< \brief Indicates if 0x0120.5 is set*/

PROTO UINT16                            EcatWdValue; /**< \brief Contains the value of the watchdog in ms, will be written in StartInputHandler. 
                                                                    In case that the ESC watchdog feature is used this variable just indicates if the watchdog is enabled or disabled*/
PROTO    UINT16                         nEscAddrOutputData; /**< \brief Contains the SM address for the output process data*/
PROTO    UINT16                         nEscAddrInputData; /**< \brief Contains the SM address for the input process data*/


/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/
PROTO void EnableSyncManChannel(UINT8 channel);
PROTO void DisableSyncManChannel(UINT8 channel);
PROTO TSYNCMAN ESCMEM *GetSyncMan(UINT8 channel);
PROTO void SetALStatus(UINT8 alStatus, UINT16 alStatusCode);
PROTO void AL_ControlInd(UINT8 alControl, UINT16 alStatusCode);
PROTO void DC_CheckWatchdog(void);
PROTO    void CheckIfEcatError(void);
PROTO void ECAT_Init(void);

PROTO void ECAT_Main(void);

#undef PROTO
/** @}*/
