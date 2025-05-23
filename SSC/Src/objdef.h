/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
 * \addtogroup CoE CAN Application Profile over EtherCAT
 * @{
 */

/**
\file objdef.h
\author EthercatSSC@beckhoff.com
\brief CAN application profile over EtherCAT object dictionary

\version 5.11

<br>Changes to version V5.10:<br>
V5.11 COE2: update synctype supported flag for freerun<br>
V5.11 COE3: change 0x10F3.2 (Sync Error limit) from UINT32 to UINT16 (according to the ETG.1020)<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
<br>Changes to version V5.01:<br>
V5.10 COE1: Define one entry description for all 0x1C3x objects and change data type of SI11,12,13 to UINT16 (according ETG.1020)<br>
V5.10 COE2: Change return value of Get ObjectSize to UINT32<br>
            Change object size to UINT32<br>
V5.10 ECAT13: Update Synchronisation handling (FreeRun,SM Sync, Sync0, Sync1)<br>
              Compare DC UINT configuration (by ESC Config data) vs. DC activation register settings<br>
              Update 0x1C3x entries<br>
V5.10 ECAT8: Update Datatype defines according ETG.1020<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "sdoserv.h"


#ifndef _OBJDEF_H_
#define _OBJDEF_H_


/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

/*---------------------------------------------
-    DataTypes
-----------------------------------------------*/
#define     DEFTYPE_NULL                0x0000 /**< \brief Null*/
#define     DEFTYPE_BOOLEAN             0x0001 /**< \brief BOOLEAN*/
#define     DEFTYPE_INTEGER8            0x0002 /**< \brief INTEGER8*/
#define     DEFTYPE_INTEGER16           0x0003 /**< \brief INTEGER16*/
#define     DEFTYPE_INTEGER24           0x0010 /**< \brief INTEGER24*/
#define     DEFTYPE_INTEGER32           0x0004 /**< \brief INTEGER32*/
#define     DEFTYPE_INTEGER40           0x0012 /**< \brief INTEGER40*/
#define     DEFTYPE_INTEGER48           0x0013 /**< \brief INTEGER48*/
#define     DEFTYPE_INTEGER56           0x0014 /**< \brief INTEGER56*/
#define     DEFTYPE_UNSIGNED8           0x0005 /**< \brief UNSIGNED8*/
#define     DEFTYPE_UNSIGNED16          0x0006 /**< \brief UNSIGNED16*/
#define     DEFTYPE_UNSIGNED24          0x0016 /**< \brief UNSIGNED24*/
#define     DEFTYPE_UNSIGNED32          0x0007 /**< \brief UNSIGNED32*/
#define     DEFTYPE_REAL32              0x0008 /**< \brief REAL32*/
#define     DEFTYPE_VISIBLESTRING       0x0009 /**< \brief VISIBLE_STRING*/
#define     DEFTYPE_OCTETSTRING         0x000A /**< \brief OCTET_STRING*/
#define     DEFTYPE_UNICODE_STRING      0x000B /**< \brief UNICODE_STRING*/
#define     DEFTYPE_TIME_OF_DAY         0x000C /**< \brief TIME_OF_DAY*/
#define     DEFTYPE_TIME_DIFFERENCE     0x000D /**< \brief TIME_DIFFERENCE*/
#define     DEFTYPE_INTEGER24           0x0010 /**< \brief INTEGER24*/
#define     DEFTYPE_REAL64              0x0011 /**< \brief REAL64*/
#define     DEFTYPE_INTEGER40           0x0012 /**< \brief INTEGER40*/
#define     DEFTYPE_INTEGER48           0x0013 /**< \brief INTEGER48*/
#define     DEFTYPE_INTEGER56           0x0014 /**< \brief INTEGER56*/
#define     DEFTYPE_INTEGER64           0x0015 /**< \brief INTEGER64*/
#define     DEFTYPE_UNSIGNED24          0x0016 /**< \brief UNSIGNED24*/
#define     DEFTYPE_UNSIGNED40          0x0018 /**< \brief UNSIGNED40*/
#define     DEFTYPE_UNSIGNED48          0x0019 /**< \brief UNSIGNED48*/
#define     DEFTYPE_UNSIGNED56          0x001A /**< \brief UNSIGNED56*/
#define     DEFTYPE_UNSIGNED64          0x001B /**< \brief UNSIGNED64*/
#define     DEFTYPE_GUID                0x001D /**< \brief DEFTYPE_GUID*/
#define     DEFTYPE_BYTE                0x001E /**< \brief DEFTYPE_BYTE*/
#define     DEFTYPE_WORD                0x001F /**< \brief DEFTYPE_WORD*/
#define     DEFTYPE_DWORD               0x0020 /**< \brief DEFTYPE_DWORD*/
#define     DEFTYPE_PDOMAPPING          0x0021 /**< \brief PDO_MAPPING*/
#define     DEFTYPE_IDENTITY            0x0023 /**< \brief IDENTITY*/
#define     DEFTYPE_COMMAND             0x0025 /**< \brief COMMAND_PAR*/
#define     DEFTYPE_PDOCOMPAR           0x0027 /**< \brief PDO_PARAMETER*/
#define     DEFTYPE_ENUM                0x0028 /**< \brief DEFTYPE_ENUM */
#define     DEFTYPE_SMPAR               0x0029 /**< \brief SM_SYNCHRONISATION*/ 
#define     DEFTYPE_RECORD              0x002A /**< \brief DEFTYPE_RECORD */
#define     DEFTYPE_BACKUP              0x002B /**< \brief BACKUP_PARAMETER*/
#define     DEFTYPE_MDP                 0x002C /**< \brief MODULAR_DEVICE_PROFILE*/
#define     DEFTYPE_BITARR8             0x002D /**< \brief BITARR8*/
#define     DEFTYPE_BITARR16            0x002E /**< \brief BITARR16*/
#define     DEFTYPE_BITARR32            0x002F /**< \brief BITARR32*/
#define     DEFTYPE_BIT1                0x0030 /**< \brief BIT1*/
#define     DEFTYPE_BIT2                0x0031 /**< \brief BIT2*/
#define     DEFTYPE_BIT3                0x0032 /**< \brief BIT3*/
#define     DEFTYPE_BIT4                0x0033 /**< \brief BIT4*/
#define     DEFTYPE_BIT5                0x0034 /**< \brief BIT5*/
#define     DEFTYPE_BIT6                0x0035 /**< \brief BIT6*/
#define     DEFTYPE_BIT7                0x0036 /**< \brief BIT7*/
#define     DEFTYPE_BIT8                0x0037 /**< \brief BIT8*/
#define     DEFTYPE_ARRAY_OF_INT        0x0260 /**< \brief DEFTYPE_ARRAY_OF_INT*/
#define     DEFTYPE_ARRAY_OF_SINT       0x0261 /**< \brief DEFTYPE_ARRAY_OF_SINT*/
#define     DEFTYPE_ARRAY_OF_DINT       0x0262 /**< \brief DEFTYPE_ARRAY_OF_DINT*/
#define     DEFTYPE_ARRAY_OF_UDINT      0x0263 /**< \brief DEFTYPE_ARRAY_OF_UDINT*/
#define     DEFTYPE_ERRORHANDLING       0x0281 /**< \brief DEFTYPE_ERRORHANDLING*/
#define     DEFTYPE_DIAGHISTORY         0x0282 /**< \brief DEFTYPE_DIAGHISTORY*/
#define     DEFTYPE_SYNCSTATUS          0x0283 /**< \brief DEFTYPE_SYNCSTATUS*/
#define     DEFTYPE_SYNCSETTINGS        0x0284 /**< \brief DEFTYPE_SYNCSETTINGS*/
#define     DEFTYPE_FSOEFRAME           0x0285 /**< \brief DEFTYPE_FSOEFRAME*/
#define     DEFTYPE_FSOECOMMPAR         0x0286 /**< \brief DEFTYPE_FSOECOMMPAR*/


/*---------------------------------------------
-    Flags for supported Synctypes (0x1C32.4 / 0x1C33.4)
-----------------------------------------------*/

#define     SYNCTYPE_FREERUNSUPP        0x0001 /**< \brief FreeRun supported*/
#define     SYNCTYPE_SYNCHRONSUPP       0x0002 /**< \brief SyncManager synchron supported*/
#define     SYNCTYPE_DCSYNC0SUPP        0x0004 /**< \brief Sync0 synchron supported*/
#define     SYNCTYPE_DCSYNC1SUPP        0x0008 /**< \brief Sync1 synchron supported*/
#define     SYNCTYPE_SUBCYCLESUPP       0x0010 /**< \brief Subordinated Application with fixed Sync0 supported*/
#define     SYNCTYPE_LOCALSHIFTSUPP     0x0020 /**< \brief Event shift with local timer supported. Output shift : 0x1C32.4 ; Input shift : 0x1C33.4 (not supported by the default SSC)*/
#define     SYNCTYPE_SHIFTBYSYNC1SUPP   0x0040 /**< \brief Event shift with Sync1 event supported. Output shift : 0x1C32.4 ; Input shift : 0x1C33.4*/
#define     SYNCTYPE_MEASURE_DELAYSUPP  0x0200 /**< \brief Delay times should be measured (not supported by the default SSC, only the cycle time can be measured (1ms resolution)*/
#define     SYNCTYPE_FIXED_DELAYSUPP    0x0400 /**< \brief Delay times are fixed*/
#define     SYNCTYPE_TIMESVARIABLE      0x4000 /**< \brief Dynamic Cycle Times supported*/


#define     IS_PDO_ASSIGN(x)            ((x >= 0x1C10) && (x <= 0x1C2F)) /**< \brief Macro to check if object index is SyncManager assign object*/
#define     IS_RX_PDO(x)                (((x) >= 0x1600) && ((x) <= 0x17FF)) /**< \brief Macro to check if object index RxPDO mapping object*/
#define     IS_TX_PDO(x)                (((x) >= 0x1A00) && ((x) <= 0x1BFF)) /**< \brief Macro to check if object index TxPDO mapping object*/


/**
 * \brief Object dictionary entry structure
 */
typedef struct OBJ_ENTRY
{
    struct OBJ_ENTRY                      *pPrev; /**< \brief Previous entry(object) in the object dictionary list*/
    struct OBJ_ENTRY                      *pNext; /**< \brief Next entry(object) in the object dictionary list*/

    UINT16                                Index; /**< \brief Object index*/
    TSDOINFOOBJDESC                       ObjDesc; /**< \brief Object access, type and code*/
    OBJCONST TSDOINFOENTRYDESC OBJMEM     *pEntryDesc; /**< \brief pointer to object entry descriptions*/
    OBJCONST UCHAR OBJMEM                 *pName; /**< \brief Pointer to object and entry names*/
    void MBXMEM                           *pVarPtr; /**< \brief Pointer to object buffer*/
    UINT8 (* Read)( UINT16 Index, UINT8 Subindex, UINT32 Size, UINT16 MBXMEM * pData, UINT8 bCompleteAccess ); /**< \brief Function pointer to read function (if NULL default read function will be used)*/
    UINT8 (* Write)( UINT16 Index, UINT8 Subindex, UINT32 Size, UINT16 MBXMEM * pData, UINT8 bCompleteAccess ); /**< \brief Function pointer to write function (if NULL default write function will be used)*/
    UINT16                                 NonVolatileOffset; /**< \brief Offset within the non volatile memory (need to be defined for backup objects)*/
}
TOBJECT;


/**
 * Object 0x1C3x (SyncManager Parameter) data structure
 */
typedef struct OBJ_STRUCT_PACKED_START
{
    UINT16    subindex0;/**< \brief SubIndex0*/
    UINT16    u16SyncType; /**< \brief SunbIndex 001: Sync type*/
    UINT32    u32CycleTime;/**< \brief SunbIndex 002: Cycle time */
    UINT32    u32Subindex003;/**< \brief SunbIndex 003: Shift time (not supported, variable is only used as a place holder)*/
    UINT16    u16SyncTypesSupported;/**< \brief SunbIndex 004: Supported Sync types*/
    UINT32    u32MinCycleTime;/**< \brief SunbIndex 005: Min cycle time*/
    UINT32    u32CalcAndCopyTime;/**< \brief SunbIndex 006: Calc and Copy time*/
    UINT32    u32Si7Reserved;/**< \brief SunbIndex 007: 32Bit Reserved*/
    UINT16    u16GetCycleTime;/**< \brief SunbIndex 008: Get cycle time*/
    UINT32    u32DelayTime; /**< \brief SunbIndex 009: Delay time*/
    UINT32    u32Sync0CycleTime; /**< \brief SunbIndex 010: Sync0 cycle time*/
    UINT16    u16SmEventMissedCounter; /**< \brief SunbIndex 011: SyncManager event missed counter*/
    UINT16    u16CycleExceededCounter; /**< \brief SunbIndex 012: Cycle exceed counter*/
    UINT16    u16ShiftTooShort; /**< \brief SunbIndex 013: Shift too short*/
    UINT16    u16Si14Reserved; /**< \brief SubIndex14 not supported*/
    UINT32    u32Si15Reserved; /**< \brief SubIndex15 not supported*/
    UINT32    u32Si16Reserved; /**< \brief SubIndex16 not supported*/
    UINT32    u32Si17Reserved; /**< \brief SubIndex17 not supported*/
    UINT32    u32Si18Reserved; /**< \brief SubIndex18 not supported*/
    UINT8    u8SyncError; /**< \brief Sync Error*/
}OBJ_STRUCT_PACKED_END
TSYNCMANPAR;

/**
 * Cycle diagnosis
 */
typedef struct OBJ_STRUCT_PACKED_START
{
    UINT16    syncFailedCounter; /**< \brief Sync Failed Counter*/
}OBJ_STRUCT_PACKED_END
TCYCLEDIAG;


/**
 * Object 0x10F1 (Error Settings Object) data structure
 */
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex0*/
   UINT32   u32LocalErrorReaction; /**< \brief Local error reaction*/
   UINT16   u16SyncErrorCounterLimit; /**< \brief Sync error counter limit*/
} OBJ_STRUCT_PACKED_END
TOBJ10F1;

#endif //_OBJDEF_H_


/*-----------------------------------------------------------------------------------------
------
------    Global variables
------
-----------------------------------------------------------------------------------------*/
#if defined(_OBJDEF_) && (_OBJDEF_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif

/**
 * Indicates if the synchronisation was written by the user on startup (by writing 0x1C3x.1).
 * If it was not written the sync mode will be set based on the DC registers 0x980:0x981
 */
PROTO BOOL bSyncSetByUser;

/**
  * \brief Cycle diagnosis variable
  */
PROTO TCYCLEDIAG sCycleDiag;


/**
  * \brief Object 0x1C32 (SyncManager 2 Parameter) object variable
  */
PROTO TSYNCMANPAR MBXMEM sSyncManOutPar;


/**
  * \brief Object 0x1C33 (SyncManager 3 Parameter) object variable
  */
PROTO TSYNCMANPAR MBXMEM sSyncManInPar;


/**
 * \brief Object 0x10F1 (Error Settings) object variable
 *
 * SubIndex0 : 2
 * Local error reaction : 1
 * Sync error counter limit : MAX_SM_EVENT_MISSED
 */
PROTO TOBJ10F1 sErrorSettings
#if defined(_OBJDEF_) && (_OBJDEF_ == 1)
= {2, 0x01, MAX_SM_EVENT_MISSED}
#endif
;


/**
 * \brief Default entry name "SubIndex 000"
 */
PROTO    CHAR OBJMEM         aSubindexDesc[13]
#if defined(_OBJDEF_) && (_OBJDEF_ == 1)
 = { "SubIndex 000" }
#endif
;


/*-----------------------------------------------------------------------------------------
------
------    Global functions
------
-----------------------------------------------------------------------------------------*/

PROTO    OBJCONST TOBJECT OBJMEM *  OBJ_GetObjectHandle( UINT16 index );
PROTO    UINT32  OBJ_GetObjectLength( UINT16 index, UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 bCompleteAccess);
PROTO    UINT16  OBJ_GetNoOfObjects(UINT8 listType);
PROTO    UINT16  OBJ_GetObjectList(UINT16 listType, UINT16 *pIndex, UINT16 size, UINT16 MBXMEM *pData,UINT8 *pAbort);
PROTO    UINT16  OBJ_GetDesc( UINT16 index, UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData );
PROTO    OBJCONST TSDOINFOENTRYDESC OBJMEM * OBJ_GetEntryDesc(OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 Subindex);
PROTO    OBJCONST TSDOINFOOBJDESC OBJMEM * OBJ_GetObjDesc(OBJCONST TOBJECT OBJMEM * pObjEntry);
PROTO    UINT16  OBJ_GetEntryOffset(UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry);
PROTO    UINT8   CheckSyncTypeValue(UINT16 index, UINT16 NewSyncType);
PROTO    UINT8   OBJ_Read(UINT16 index, UINT8 subindex, UINT32 objSize, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO    UINT8   OBJ_Write(UINT16 index, UINT8 subindex, UINT32 dataSize, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO    void    COE_WriteBackupEntry(UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry);

#undef PROTO
/** @}*/
