/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
* \addtogroup DPort-ECT DPort-ECT
* @{
*/

/**
\file DPort-ECTObjects
\author ET9300Utilities.ApplicationHandler (Version 1.6.4.0) | EthercatSSC@beckhoff.com

\brief DPort-ECT specific objects<br>
\brief NOTE : This file will be overwritten if a new object dictionary is generated!<br>
*/

#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
#define PROTO
#else
#define PROTO extern
#endif
/******************************************************************************
*                    Object 0x1600 : Output mapping 0
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | Output mapping 0
* @{
* \brief Object 0x1600 (Output mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Reference to 0x7000.1<br>
* SubIndex 2 - Reference to 0x7000.2<br>
* SubIndex 3 - Reference to 0x7010.1<br>
* SubIndex 4 - Reference to 0x7020.1<br>
* SubIndex 5 - Reference to 0x7020.2<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - Reference to 0x7000.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - Reference to 0x7000.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - Reference to 0x7010.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 - Reference to 0x7020.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex5 - Reference to 0x7020.2 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600[] = "Output mapping 0\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - Reference to 0x7000.1 */
UINT32 SI2; /* Subindex2 - Reference to 0x7000.2 */
UINT32 SI3; /* Subindex3 - Reference to 0x7010.1 */
UINT32 SI4; /* Subindex4 - Reference to 0x7020.1 */
UINT32 SI5; /* Subindex5 - Reference to 0x7020.2 */
} OBJ_STRUCT_PACKED_END
TOBJ1600;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600 OutputMapping00x1600
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={5,0x70000108,0x70000208,0x70100120,0x70200120,0x70200220}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A00 : Input mapping 0
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | Input mapping 0
* @{
* \brief Object 0x1A00 (Input mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Reference to 0x6000.1<br>
* SubIndex 2 - Reference to 0x6000.2<br>
* SubIndex 3 - Reference to 0x6000.3<br>
* SubIndex 4 - Reference to 0x6010.1<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - Reference to 0x6000.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - Reference to 0x6000.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - Reference to 0x6000.3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex4 - Reference to 0x6010.1 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00[] = "Input mapping 0\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - Reference to 0x6000.1 */
UINT32 SI2; /* Subindex2 - Reference to 0x6000.2 */
UINT32 SI3; /* Subindex3 - Reference to 0x6000.3 */
UINT32 SI4; /* Subindex4 - Reference to 0x6010.1 */
} OBJ_STRUCT_PACKED_END
TOBJ1A00;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00 InputMapping00x1A00
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={4,0x60000120,0x60000220,0x60000320,0x60100120}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C12 : SyncManager 2 assignment
******************************************************************************/
/**
* \addtogroup 0x1C12 0x1C12 | SyncManager 2 assignment
* @{
* \brief Object 0x1C12 (SyncManager 2 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C12[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C12[] = "SyncManager 2 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJ1C12;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C12 sRxPDOassign
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={1,{0x1600}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C13 : SyncManager 3 assignment
******************************************************************************/
/**
* \addtogroup 0x1C13 0x1C13 | SyncManager 3 assignment
* @{
* \brief Object 0x1C13 (SyncManager 3 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C13[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C13[] = "SyncManager 3 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJ1C13;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C13 sTxPDOassign
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={1,{0x1A00}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6000 : INFO_PDIChannel
******************************************************************************/
/**
* \addtogroup 0x6000 0x6000 | INFO_PDIChannel
* @{
* \brief Object 0x6000 (INFO_PDIChannel) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Pos<br>
* SubIndex 2 - Vel<br>
* SubIndex 3 - Iq<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex1 - Pos */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex2 - Vel */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }}; /* Subindex3 - Iq */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6000[] = "INFO_PDIChannel\000"
"Pos\000"
"Vel\000"
"Iq\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 Pos; /* Subindex1 - Pos */
REAL32 Vel; /* Subindex2 - Vel */
REAL32 Iq; /* Subindex3 - Iq */
} OBJ_STRUCT_PACKED_END
TOBJ6000;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6000 INFO_PDIChannel0x6000
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6010 : MODE_PDIChannel
******************************************************************************/
/**
* \addtogroup 0x6010 0x6010 | MODE_PDIChannel
* @{
* \brief Object 0x6010 (MODE_PDIChannel) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Mode<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6010[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ }}; /* Subindex1 - Mode */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6010[] = "MODE_PDIChannel\000"
"Mode\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT32 Mode; /* Subindex1 - Mode */
} OBJ_STRUCT_PACKED_END
TOBJ6010;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6010 MODE_PDIChannel0x6010
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7000 : LED_PDOChannel
******************************************************************************/
/**
* \addtogroup 0x7000 0x7000 | LED_PDOChannel
* @{
* \brief Object 0x7000 (LED_PDOChannel) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - LED1<br>
* SubIndex 2 - LED2<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ }, /* Subindex1 - LED1 */
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ }}; /* Subindex2 - LED2 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7000[] = "LED_PDOChannel\000"
"LED1\000"
"LED2\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 LED1; /* Subindex1 - LED1 */
UINT8 LED2; /* Subindex2 - LED2 */
} OBJ_STRUCT_PACKED_END
TOBJ7000;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7000 LED_PDOChannel0x7000
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={2,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7010 : MODE_PDOChannel
******************************************************************************/
/**
* \addtogroup 0x7010 0x7010 | MODE_PDOChannel
* @{
* \brief Object 0x7010 (MODE_PDOChannel) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Set_Mode<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7010[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ }}; /* Subindex1 - Set_Mode */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7010[] = "MODE_PDOChannel\000"
"Set_Mode\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT32 Set_Mode; /* Subindex1 - Set_Mode */
} OBJ_STRUCT_PACKED_END
TOBJ7010;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7010 MODE_PDOChannel0x7010
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7020 : INFO_PDOChannel
******************************************************************************/
/**
* \addtogroup 0x7020 0x7020 | INFO_PDOChannel
* @{
* \brief Object 0x7020 (INFO_PDOChannel) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Set_Pos<br>
* SubIndex 2 - Set_Vel<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7020[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex1 - Set_Pos */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }}; /* Subindex2 - Set_Vel */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7020[] = "INFO_PDOChannel\000"
"Set_Pos\000"
"Set_Vel\000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 Set_Pos; /* Subindex1 - Set_Pos */
REAL32 Set_Vel; /* Subindex2 - Set_Vel */
} OBJ_STRUCT_PACKED_END
TOBJ7020;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7020 INFO_PDOChannel0x7020
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={2,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF000 : Modular Device Profile
******************************************************************************/
/**
* \addtogroup 0xF000 0xF000 | Modular Device Profile
* @{
* \brief Object 0xF000 (Modular Device Profile) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Index distance <br>
* SubIndex 2 - Maximum number of modules <br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex1 - Index distance  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }}; /* Subindex2 - Maximum number of modules  */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xF000[] = "Modular Device Profile\000"
"Index distance \000"
"Maximum number of modules \000\377";
#endif //#ifdef _OBJD_

#ifndef _DPORT_ECT_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT16 IndexDistance; /* Subindex1 - Index distance  */
UINT16 MaximumNumberOfModules; /* Subindex2 - Maximum number of modules  */
} OBJ_STRUCT_PACKED_END
TOBJF000;
#endif //#ifndef _DPORT_ECT_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF000 ModularDeviceProfile0xF000
#if defined(_DPORT_ECT_) && (_DPORT_ECT_ == 1)
={2,0x0010,0}
#endif
;
/** @}*/







#ifdef _OBJD_
TOBJECT    OBJMEM ApplicationObjDic[] = {
/* Object 0x1600 */
{NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 5 | (OBJCODE_REC << 8)} , asEntryDesc0x1600 , aName0x1600 , &OutputMapping00x1600 , NULL , NULL , 0x0000 },
/* Object 0x1A00 */
{NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 4 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00 , aName0x1A00 , &InputMapping00x1A00 , NULL , NULL , 0x0000 },
/* Object 0x1C12 */
{NULL , NULL ,  0x1C12 , {DEFTYPE_UNSIGNED16 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C12 , aName0x1C12 , &sRxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x1C13 */
{NULL , NULL ,  0x1C13 , {DEFTYPE_UNSIGNED16 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C13 , aName0x1C13 , &sTxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x6000 */
{NULL , NULL ,  0x6000 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x6000 , aName0x6000 , &INFO_PDIChannel0x6000 , NULL , NULL , 0x0000 },
/* Object 0x6010 */
{NULL , NULL ,  0x6010 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6010 , aName0x6010 , &MODE_PDIChannel0x6010 , NULL , NULL , 0x0000 },
/* Object 0x7000 */
{NULL , NULL ,  0x7000 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x7000 , aName0x7000 , &LED_PDOChannel0x7000 , NULL , NULL , 0x0000 },
/* Object 0x7010 */
{NULL , NULL ,  0x7010 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x7010 , aName0x7010 , &MODE_PDOChannel0x7010 , NULL , NULL , 0x0000 },
/* Object 0x7020 */
{NULL , NULL ,  0x7020 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x7020 , aName0x7020 , &INFO_PDOChannel0x7020 , NULL , NULL , 0x0000 },
/* Object 0xF000 */
{NULL , NULL ,  0xF000 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0xF000 , aName0xF000 , &ModularDeviceProfile0xF000 , NULL , NULL , 0x0000 },
{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
#endif    //#ifdef _OBJD_

#undef PROTO

/** @}*/
#define _DPORT_ECT_OBJECTS_H_
