/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup CoE CAN Application Profile over EtherCAT
@{
*/

/**
\file objdef.c
\author EthercatSSC@beckhoff.com
\brief Implementation
This file contains the object dictionary access functions

\version 5.13

<br>Changes to version V5.12:<br>
V5.13 COE1: handling objects with 255 entries<br>
V5.13 COE4: update default entry name handling in case of 16Bit characters, add CoE Read/write indication functions<br>
V5.13 COE9: prevent compiler warning in get entry description if char is 8bit<br>
V5.13 TEST7: add entries with bit9-16 types<br>
<br>Changes to version V5.11:<br>
V5.12 COE10: update object entry string handling<br>
V5.12 COE3: update entry access right handling<br>
V5.12 COE6: handle get object length in case of an out of range subindex<br>
V5.12 COE7: in case of a padding entry SDO upload/download return Unsupported access<br>
V5.12 COE9: SDO download 0xF030.0 shall be 0 before writing entries 1..n<br>
V5.12 ECAT1: update SM Parameter measurement (based on the system time), enhancement for input only devices and no mailbox support, use only 16Bit pointer in process data length caluclation<br>
V5.12 ECAT5: update Sync error counter/flag handling,check enum memory alignment depending on the processor,in case of a polled timer disable ESC interrupts during DC_CheckWatchdog<br>
<br>Changes to version V5.10.1:<br>
V5.11 DIAG4: change parameter handling in DIAG_CreateNewMessage()<br>
V5.11 ECAT: <br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 ESM7: "add Sync define for 0x22 (""SYNCTYPE_SM2_SYNCHRON""), support value 0x22 for 0x1C33.1 (SM2 sync)"<br>
V5.11 OBJ1: OBJ_GetEntryOffset: return offset 0 for si0 (always offset 16 was returned before)<br>
V5.11 SDO1: "Obj_Read/Obj_Write: Check if entries > 8bit has even word offset (otherwise ""unsupported access"" is returned). To support these object a read/write callback function need to be used (see Application Note ET9300)"<br>
V5.11 SDO2: Obj_Read/Obj_Write: Handle arrays (string, array of byte, ...) with an odd word length (buffer shift was wrong). Only an entry <=8Bit is allowed after such an entry.<br>
V5.11 SDO3: handle not word aligned enums<br>
V5.11 SDO5: add missing swapping on SI0 write access<br>
V5.11 SDO6: OBJ_GetDesc ensure that the default string is used if no string was found<br>
V5.11 SDO7: add missing value swapping if memory values are bitwise modified on object read/write access<br>
V5.11 SDO9: "handle SDO access for entries which are not accessible (in case of SDO complete access), SDO access handle ""DEFTYPE_ARRAY_OF_INT"" , ""DEFTYPE_ARRAY_OF_SINT"" , ""DEFTYPE_ARRAY_OF_DINT"" , ""DEFTYPE_ARRAY_OF_UDINT"", SDO access handle : ""BYTE"" ,  ""WORD"" and ""DWORD"""<br>
V5.11 TEST2: add test 0x300C object (entry bitlength > 65535)<br>
V5.11 TEST8: create diag message on SDO write access to 0x3002<br>
<br>Changes to version V5.01:<br>
V5.10 COE2: Change return value of Get ObjectSize to UINT32<br>
            Change object size to UINT32<br>
V5.10 ECAT11: Add define "USE_SINGLE_PDO_MAPPING_ENTRY_DESCR" (Only one entry description is required for all PDO mapping entries)<br>
V5.10 ECAT13: Update Synchronisation handling (FreeRun,SM Sync, Sync0, Sync1)<br>
              Compare DC UINT configuration (by ESC Config data) vs. DC activation register settings<br>
              Update 0x1C3x entries<br>
V5.10 ECAT3: Add new datatype handling (BITARRxx)<br>
             Use read/write macro to access STRING entries<br>
V5.10 SDO1: Handle access to enums on odd memory addresses (only for MBX_16BIT_ACCESS)<br>
V5.10 SDO2: Check for termination char in every substring<br>
V5.10 SDO5: Check also for PDO mapping object if the subindex 0 was set to 0 before the entries will be changed.<br>
V5.10 SDO9: SDO complete write access update buffer offset calculation in case of SI0 is readonly<br>
V5.10 TEST2: Add 0x3004.14 (Access to this entry will always rejected with the code "cannot be acceesd because of local control")<br>
V5.10 TEST4: Add CoE test (0x2020.5) to return always max object data on complete access (not limited to value of SI0)<br>
<br>Changes to version V5.0:<br>
V5.01 COE2: Calculate the bit offset also for SI0 in case if an SDO download access.<br>
V5.01 SDO1: Update alignment entry handling.<br>
V5.01 SDO5: Update offset calculation in case of an not accessible entry<br>
V5.01 SDO7: Update object length calculation in case of an BitArray<br>
V5.01 TEST5: Add slave configuration object handling (0x8000, if test application is active)<br>
V5.01 TEST6: Add test to create dummy object dictionary.<br>
<br>Changes to version V4.40:<br>
V5.0 TEST1: Add test application.<br>
V5.0 COE3: Update OCTED_STRING and VISIBLE_STRING read/write.<br>
V5.0 COE5: Add BigEndian support for enum SDO read, handle complete access also for objects with align or not existing entries.<br>
V5.0 COE7: Support SDO Info entry name handling for 16Bit access<br>
V5.0 DIAG2: SDO Info request for diagnosis messages 0x10F3 SI5 or greater return always description of SI6.<br>
V5.0 COE2: Add plausibility check for PDO assign objects.<br>
V5.0 OBJ2: Support DWORD/WORD aligned object structures.<br>
V5.0 SDO7: Update SDO read/write for object(entries) less or equal 8Bit.<br>
<br>Changes to version V4.30:<br>
V4.40 DIAG1: Add diagnosis access functions<br>
V4.40 OBJ2: change counter variable to prevent deadlock caused by overrun (required if subindex0 is 255)<br>
V4.40 COE2: Prevent accessing empty object dictionary entry<br>
V4.40 COE3: Read whole object dictionary<br>
V4.40 COE1: Prototype of OBJ_GetObjectList() changed, set abort code if object dictionary is empty<br>
V4.30 OBJ 3: Object dictionary list handling<br>
V4.30 OBJ 2: fix SDO complete access bug<br>
V4.11 OBJ 1-3: OBJ_GetDesc() Now, subindex 0 instead of 0xFF is used to return the description of the whole object<br>
Subindex 0xFF could be used like a normal subindex.<br>
<br>Changes to version V4.08:<br>
V4.10 OBJ 1: calculate size of PdoObjectEntry Index 1 and not Index 0<br>
V4.10 OBJ 2: the pointer of the object dictionary will be get by a function (COE_GetObjectDictionary)<br>
<br>Changes to version V4.07:<br>
V4.08 OBJ 1: if subindex 0 is writable, the maximum subindex should be checked in an object specific function,<br>
             because for the PDO mapping and PDO assign the object shall only be written if subindex 0 is 0.<br>
<br>Changes to version V4.04:<br>
V4.04 OBJ 1: The loop in OBJ_GetObjectList was one subindex too low<br>
<br>Changes to version V4.03:<br>
V4.04 SDO 1: The SDO interface was changed in that way that a SDO response<br>
             could be sent by the application to a later time. In that case<br>
                 the functions OBJ_Read and OBJ_Write shall return the value<br>
                 ABORTIDX_WORKING. To send the SDO response the new function SDOS_SdoRes<br>
                 has to be called by the application. While waiting for the call<br>
                 of SDOS_SdoRes the SDO interface will answer to another SDO request<br>
                 with the error MBXERR_SERVICEINWORK in the mailbox protocol<br>
V4.04 OBJ 1: The loop in OBJ_GetNoOfObjects was one subindex too low<br>
<br>Changes to version V4.02:<br>
V4.03 OBJ 1: The object list was not returned correctly for the list types 2-5<br>
<br>Changes to version V3.20:<br>
V4.00 OBJ 1: The OBJ functions are modified for a more flexible definition of<br>
             the object dictionary and to support bit types too<br>
V4.00 SDO 2: The object lists will be generated in the functions OBJ_GetNoOfObjects<br>
             and OBJ_GetObjectList in objdef.c to decouple the SDO services from<br>
             the implementation of the object dictionary<br>
V4.00 COEAPPL 2: The handling of backup parameters was included according to<br>
                 the EtherCAT Guidelines and Protocol Enhancements Specification

*/


#define    OBJDEF       0x4000
#define    OBJDEFMAX    0x0A

/*---------------------------------------------------------------------------------------
------
------    Includes
------
---------------------------------------------------------------------------------------*/

#include "ecat_def.h"


#include "ecatslv.h"
#include "coeappl.h"


#define  _OBJDEF_    1
#include "objdef.h"
#undef    _OBJDEF_
/*remove definition of _OBJDEF_ (#ifdef is used in objdef.h)*/

#include "applInterface.h"






extern OBJCONST TOBJECT OBJMEM *pSdoInfoObjEntry;
extern OBJCONST TSDOINFOENTRYDESC OBJMEM * OBJ_GetEntryDesc(OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 Subindex);

//---------------------------------------------------------------------------------------

/*---------------------------------------------------------------------------------------
------
------    Internal types and defines
------
---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
------
------    module internal function declarations
------
---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
------
------    Module internal variable definitions
------
---------------------------------------------------------------------------------------*/
const UINT16 cBitMask[16] = {0x0000,0x0001,0x0003,0x0007,0x000F,0x001F,0x003F,0x007F,0x00FF,0x01FF,0x03FF,0x07FF,0x0FFF,0x1FFF,0x3FFF,0x7FFF};
/*---------------------------------------------------------------------------------------
------
------    Functions
------
---------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index     Indicates the index of the dictionary object.

 \return    A handle to an object of the requested index or NULL if not found.

 \brief    The function looks in all objects of the dictionary after the indicated index
             and returns a handle if found.

*////////////////////////////////////////////////////////////////////////////////////////

OBJCONST TOBJECT OBJMEM *  OBJ_GetObjectHandle( UINT16 index )
{
    OBJCONST TOBJECT OBJMEM * pObjEntry = (OBJCONST TOBJECT OBJMEM *) COE_GetObjectDictionary();

    while (pObjEntry!= NULL)
    {
        
        if (pObjEntry->Index == index)
        {
            return pObjEntry;
        }
        pObjEntry = (TOBJECT OBJMEM *) pObjEntry->pNext;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index           Index of the requested object.
 \param     subindex        Subindex of the requested object.
 \param     pObjEntry       Handle to the dictionary object returned by
                            OBJ_GetObjectHandle which was called before
 \param     bCompleteAccess Indicates if a complete read of all subindices of the
                            object shall be done or not

 \return    The size of the object entry in bytes (!).

 \brief     This function returns the size of the requested entry. If bCompleteaccess is set the size of the complete object is returned (the returned size is byte packed, gaps based on the used platform/compiler are not taken into account)

*////////////////////////////////////////////////////////////////////////////////////////
UINT32 OBJ_GetObjectLength( UINT16 index, UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 bCompleteAccess)
{
    /* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    UINT8 objCode = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT;
    
    UINT8 maxSubindex = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT;
    UINT32 size = 0;

    if ( bCompleteAccess )
    {
        if ( objCode == OBJCODE_VAR )
        {
            return 0;
        }
        else if ((objCode == OBJCODE_ARR)
            )
        {

            /* we have to get the maxSubindex from the actual value of subindex 0,
                which is stored as UINT16 at the beginning of the object's variable */
            maxSubindex = (UINT8) ((UINT16 MBXMEM *) (pObjEntry->pVarPtr))[0];


            size = pObjEntry->pEntryDesc[1].BitLength *maxSubindex;

/* no padding required: Bit entries within an array object may overlap byte borders*/

            /* we have to convert the size in bytes */
            size = BIT2BYTE(size);

            if ( subindex == 0 )
            {
                /* add size for subindex 0 (is transmitted as UINT16) */
                size += 2;
            }
            return size;
        }
        else
        {
/*ECATCHANGE_START(V5.13) COE1*/
            UINT16 i;
/*ECATCHANGE_END(V5.13) COE1*/

            /* add the sizes of all entries */
            for (i = 1; i <= maxSubindex; i++)
            {
                size += pObjEntry->pEntryDesc[i].BitLength;
            }
            /* we have to convert the size in bytes */
            size = BIT2BYTE(size);

            if ( subindex == 0 )
            {
                /* add size for subindex 0 (is transmitted as UINT16) */
                size += 2;
            }
            return size;
        }
    }
    else
    {
        if ( objCode == OBJCODE_VAR )
        {

            return (BIT2BYTE(pObjEntry->pEntryDesc->BitLength));

        }
        else if ( subindex == 0 )
        {
            /* for single access subindex 0 is transmitted as UINT8 */
            return 1;
        }
        else if((objCode == OBJCODE_ARR)
            )
        {
            return (BIT2BYTE(pObjEntry->pEntryDesc[1].BitLength));
        }
        else
        {
            {
                if (maxSubindex < subindex)
                {
                    return 0;

                }
                else
                {
                    return (BIT2BYTE(pObjEntry->pEntryDesc[subindex].BitLength));
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pStr         Pointer to a string
 \param  Number   Number to be included in the string

 \brief    The function copies the Number in the string pStr,
         which shall be initialized with 000

*////////////////////////////////////////////////////////////////////////////////////////

void OBJ_CopyNumberToString(UCHAR MBXMEM *pStr, UINT8 Number)
{
    UINT8 Modulo;

    pStr[2] = '0';
    pStr[0] += (Number / 100);
    Modulo = Number % 100;
    pStr[1] += (Modulo / 10);
    pStr[2] += (Modulo % 10);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     listType requested listType (0=all objects, 1=RxPDO mappable objects,
                        2=TxPDO mappable objects, 3=backup objects, 4=setting objects)

 \return Number of objects of the requested list type

 \brief    The function counts the number of objects of the requested list type

*////////////////////////////////////////////////////////////////////////////////////////

UINT16    OBJ_GetNoOfObjects(UINT8 listType)
{
    /* the variable listFlags contains the mask used for the ObjAccess in the Entry-Desc
       see the structure TSDOINFOENTRYDESC in sdoserv.h, listType = 0 indicates that
       all objects has to be counted */
    
    UINT16 listFlags = 0x0020 << listType;
    
    /* set pObjEntry to the beginning of the object dictionary */
    OBJCONST TOBJECT OBJMEM * pObjEntry = (OBJCONST TOBJECT OBJMEM *) COE_GetObjectDictionary();
    UINT16 n = 0;


    while (pObjEntry != NULL)
    {
        /* count the objects of the requested list type */
        if ( pObjEntry->Index >= 0x1000 )
        {
            UINT8 t = listType;
            if ( t )
            {
                UINT8 maxSubindex = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT;
                UINT16 i = 0;

                while ( t && i <= maxSubindex )
                {
                    if ( OBJ_GetEntryDesc(pObjEntry,(UINT8) i)->ObjAccess & listFlags )
                    {
                        t = 0;
                    }
                    i++;
                }
            }
            if ( !t )
            {
                /* object from listType found */
                n++;
            }
        }
        /* next object in object dictionary */
        pObjEntry = (TOBJECT OBJMEM *) pObjEntry->pNext;
    }

    return n;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     listType 	Requested listType (0=all objects, 1=RxPDO mappable objects,
                        2=TxPDO mappable objects, 3=backup objects, 4=setting objects)
 \param     pIndex     	Pointer to the next Index of the object list to copied in the mailbox buffer,
                         has to adapted    at the end of the function
 \param     size        Size of the available mailbox buffer
 \param     pData     	Pointer to the mailbox buffer where (the part of) the object list requested listType (0=all objects, 1=RxPDO mappable objects,
 \param     pAbort    	Pointer to abortCode

 \return 	Size of the available mailbox buffer which was not copied to

 \brief    The function copies (the part of) the object list in the mailbox buffer

*////////////////////////////////////////////////////////////////////////////////////////
UINT16    OBJ_GetObjectList(UINT16 listType, UINT16 *pIndex, UINT16 size, UINT16 MBXMEM *pData,UINT8 *pAbort)
{
    /* the variable listFlags contains the mask used for the ObjAccess in the Entry-Desc
       see the structure TSDOINFOENTRYDESC in sdoserv.h, listType = 0 indicates that
       all objects has to be counted */
    UINT16 listFlags = 0x0020 << listType;
    OBJCONST TOBJECT OBJMEM * pObjEntry;


    if ( pIndex[0] == 0x1000 )
    {
        /* beginning of object list, set pObjEntry to the beginning of the object dictionary */
       pObjEntry = (OBJCONST TOBJECT OBJMEM *) COE_GetObjectDictionary();
        // set abort code if no object dictionary is available
        if((pObjEntry == NULL) && (pAbort != NULL))
        {
            *pAbort = ABORTIDX_NO_OBJECT_DICTIONARY_IS_PRESENT;
        }
    }
    else
    {
        /* next fragment, the next object to be handled was stored in pSdoInfoObjEntry */
        pObjEntry = pSdoInfoObjEntry;
    }

    if ( pObjEntry != NULL )
    {
        while (pObjEntry != NULL && size > 1 )
        {
            /* get the next index of the requested object list if there is enough space in the mailbox buffer */
            if ( pObjEntry->Index >= 0x1000 )
            {
                /* UINT8 was changed to UINT16 */
                UINT16 t = listType;
                if ( t )
                {
                    UINT8 maxSubindex = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT;
/*ECATCHANGE_START(V5.13) COE1*/
                    UINT16 i = 0;
/*ECATCHANGE_END(V5.13) COE1*/

                    while ( t && i <= maxSubindex )
                    {
                        if ( OBJ_GetEntryDesc(pObjEntry, (UINT8)i)->ObjAccess & listFlags )
                        {
                            t = 0;
                        }
                        i++;
                    }
                }
                if ( !t )
                {
                    /* store the index in the mailbox buffer */
                    *pData = SWAPWORD(pObjEntry->Index);
                    pData++;
                    size -= 2;
                }
            }
        pObjEntry = (TOBJECT OBJMEM *) pObjEntry->pNext;
        }
    }

        /* return the next Index to be handled */
        if(pObjEntry != NULL)
        {
            pIndex[0] = pObjEntry->Index;
        }
        else
        {
            /*last entry reached*/
            pIndex[0] = 0xFFFF;
        }

    /* store object description pointer and index for next fragment */
    pSdoInfoObjEntry = pObjEntry;

    /* return the size of the available mailbox buffer which was not copied to */
    return size;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index     Indicates the index of the dictionary object.
 \param  	subindex  Indicates the subindex of the dictionary object.
                        Subindex 0xff returns the description of the whole object ( the name of
                        the object ).
                        Subindex 0x00 returns the description of the subindex 0 and so on.
 \param     pObjEntry Is a handle to the dictionary object ( for faster access ) or NULL.						
 \param    pData        Is the memory field for the description string or NULL ( if the size of
                        string is unknown ):

 \return    The size in bytes of the description string (without null termination byte ).
            0 will be returned if a description for the indicated entry was not found.

 \brief    The function returns size and description string of the requested entry.

            Its possible to define all description strings of one entry ( including the name
            of the object ) in one structure:        <br>
            {                                                    <br>
                name_of_object with index,              <br>
                description_of_subindex0,                <br>
                .                                                <br>
                description_of_subindexN,                <br>
                0xFF                                            <br>
            }
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 OBJ_GetDesc( UINT16 index, UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData )
{
    UINT16 strSize = 0;
    OBJCONST UCHAR OBJMEM * pDesc = (OBJCONST UCHAR OBJMEM *) pObjEntry->pName;
/* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    UINT8 objCode = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT;



    if ( (subindex == 0) || (objCode == OBJCODE_VAR) )
    {
        // Get object description length
       strSize = (UINT16) OBJSTRLEN((OBJCONST CHAR OBJMEM *) pDesc);

        // If there is a pointer given, copy data:
        if ( pData )
        {
            OBJTOMBXSTRCPY( pData, pDesc, strSize );
        }
    }
    else
    {
        if ( objCode == OBJCODE_REC )
        {
            {
            // get pointer to description of subindex 1 :
            // 16bit variable to avoid overflow if subindex 0xFF is read
            UINT16 i = 1;
            UINT16 tmpSubindex = subindex;

            {

            OBJCONST UCHAR OBJMEM * pSubDesc = (OBJCONST UCHAR OBJMEM *) OBJGETNEXTSTR( pDesc );
            while (( i <= tmpSubindex )
                &&( pSubDesc[0] != 0xFF && pSubDesc[0] != 0xFE 
/* ECATCHANGE_START(V5.13) COE9*/
/* ECATCHANGE_END(V5.13) COE9*/
                    ))
            {
                if ( i == tmpSubindex )
                {
                   strSize = (UINT16) OBJSTRLEN((OBJCONST CHAR OBJMEM *) pSubDesc);
                    if ( pData && strSize )
                    {
                        OBJTOMBXSTRCPY( pData, pSubDesc, strSize );
                    }
                    break;
                }
                else
                {
                    i++;
                    pSubDesc = (OBJCONST UCHAR OBJMEM *) OBJGETNEXTSTR( pSubDesc );
                }
            }
            }
            }
        }

        if ( strSize == 0 )
        {
            // no string found for subindex x -> name is Subindex x
            strSize =    12;

            if ( pData )
            {
                UCHAR OBJMEM         TmpDescr[13];
                /* ECATCHANGE_START(V5.13) COE4*/
                MEMCPY(TmpDescr, aSubindexDesc, SIZEOF(TmpDescr));
                /* ECATCHANGE_END(V5.13) COE4*/
                
                OBJ_CopyNumberToString( &TmpDescr[9], subindex );
                MBXSTRCPY( pData, TmpDescr, SIZEOF(TmpDescr) );
            }
        }
    }

    return strSize;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pObjEntry            handle to the dictionary object returned by
                                     OBJ_GetObjectHandle which was called before
 \param     Subindex                subindex of the requested object.

 \return    Pointer to the EntryDesc of the Subindex

 \brief    The function returns the Entry-Desc of a subindex to allow the application
         to define the object dictionary independent of the sdoserv-files

*////////////////////////////////////////////////////////////////////////////////////////

OBJCONST TSDOINFOENTRYDESC OBJMEM * OBJ_GetEntryDesc(OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 Subindex)
{
    OBJCONST TSDOINFOENTRYDESC OBJMEM *pEntry;
    UINT8 objCode = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT;

    if ((objCode == OBJCODE_ARR)
        )
    {
        /* object is an array */
        if ( Subindex == 0 )
        {
            /* subindex 0 has a description */
            pEntry = &pObjEntry->pEntryDesc[0];
        }
        else
        {
            /* and all other elements have the same description */
            pEntry = &pObjEntry->pEntryDesc[1];
        }
    }
    else
    {
        {
            /* object is a variable or a record return the corresponding entry */
            pEntry = &pObjEntry->pEntryDesc[Subindex];
        }
    }

    return pEntry;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pObjEntry            handle to the dictionary object returned by
                                     OBJ_GetObjectHandle which was called before

 \return    Pointer to the ObjDesc of the Object

 \brief    The function returns the Obj-Desc of an object to allow the application
         to define the object dictionary independent of the sdoserv-files

*////////////////////////////////////////////////////////////////////////////////////////

OBJCONST TSDOINFOOBJDESC OBJMEM * OBJ_GetObjDesc(OBJCONST TOBJECT OBJMEM * pObjEntry)
{
    return &pObjEntry->ObjDesc;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     subindex                subindex of the entry
 \param     pObjEntry            handle to the dictionary object returned by
                                     OBJ_GetObjectHandle which was called before

 \return    bit offset of the entry in the variable

 \brief    This function calculates the bit offset of the entry in the object's variable
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 OBJ_GetEntryOffset(UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry)
{
    UINT16 i;
    /* bitOffset will be initialized with the bit offset of subindex 1 */
    UINT16 bitOffset = 0;
    UINT8 objCode = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT;
    OBJCONST TSDOINFOENTRYDESC OBJMEM *pEntry;

    
    if(subindex > 0)
    {
        /*subindex 1 has an offset of 16Bit (even if Si0 is only an UINT8) */
        bitOffset +=16;
    }

    if (objCode == OBJCODE_VAR)
    {
        return 0;
    }

    for (i = 1; i <= subindex; i++)
    {
        /* get the entry description */
        if ((objCode == OBJCODE_ARR)
           )
           {
            pEntry = &pObjEntry->pEntryDesc[1];
           }
        else
        {
            pEntry = &pObjEntry->pEntryDesc[i];
        }

        switch (pEntry->DataType)
        {
        case    DEFTYPE_INTEGER16:
        case    DEFTYPE_UNSIGNED16:
        case    DEFTYPE_BITARR16:
        case    DEFTYPE_WORD:
        case    DEFTYPE_UNICODE_STRING:
        case    DEFTYPE_ARRAY_OF_INT :

#if OBJ_WORD_ALIGN    || OBJ_DWORD_ALIGN
            /* the 16-bit variables in the structure are word-aligned,
               align the actual bitOffset to a word */
            bitOffset = (bitOffset+15) & 0xFFF0;
#endif


            if (i < subindex)
            {
                if((pEntry->DataType == DEFTYPE_UNICODE_STRING)
                    ||(pEntry->DataType == DEFTYPE_ARRAY_OF_INT))
                {
                    bitOffset += pEntry->BitLength;
                }
                else
                {
                    bitOffset += 16;
                }
            }
            break;
        case    DEFTYPE_UNSIGNED32:
        case    DEFTYPE_INTEGER32:
        case    DEFTYPE_REAL32:
        case    DEFTYPE_BITARR32:
        case    DEFTYPE_DWORD:
        case    DEFTYPE_ARRAY_OF_DINT :
        case    DEFTYPE_ARRAY_OF_UDINT:
#if OBJ_DWORD_ALIGN
            /* the 32-bit variables in the structure are dword-aligned,
               align the actual bitOffset to a dword */
            bitOffset = (bitOffset+31) & 0xFFE0;
#elif OBJ_WORD_ALIGN
            /* the 32-bit variables in the structure are word-aligned,
               align the actual bitOffset to a word */
            bitOffset = (bitOffset+15) & 0xFFF0;
#endif

            if (i < subindex)
            {
                if((pEntry->DataType == DEFTYPE_ARRAY_OF_DINT)
                    ||(pEntry->DataType == DEFTYPE_ARRAY_OF_UDINT))
                {
                    bitOffset += pEntry->BitLength;
                }
                else
                {
                   bitOffset += 32;
                }
            }
            break;
        
        case DEFTYPE_REAL64:
        case DEFTYPE_INTEGER64:
        case DEFTYPE_UNSIGNED64:
#if OBJ_DWORD_ALIGN
            /* the 64-bit variables in the structure are dword-aligned,
               align the actual bitOffset to a dword */
            bitOffset = (bitOffset + 31) & 0xFFE0;
#elif OBJ_WORD_ALIGN
            /* the 64-bit variables in the structure are word-aligned,
               align the actual bitOffset to a word */
            bitOffset = (bitOffset + 15) & 0xFFF0;
#endif

            if (i < subindex)
            {

                bitOffset += 64;
            }
            break;
        default:
            /* align the actual bitOffset to a byte */
            if (i < subindex)
            {
                bitOffset += pEntry->BitLength;
            }
            break;
        }
    }

    return bitOffset;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index                 index of the SyncManager Parameter object 
 \param     NewSyncType           New value for the Sync Type (SubIndex 1)

 \return    result                Result of the value validation

 \brief    Checks if the new Sync type value is valid
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 CheckSyncTypeValue(UINT16 index, UINT16 NewSyncType)
{
    switch (NewSyncType)
    {
    case SYNCTYPE_FREERUN:
        return 0; //free run sync mode is always accepted

    case SYNCTYPE_SM_SYNCHRON:
        if ((index == 0x1C32) 
            && (nPdOutputSize > 0) 
            && ((sSyncManOutPar.u16SyncTypesSupported & SYNCTYPE_SYNCHRONSUPP) > 0))
        {
            /*SyncManager sync mode is supported and output process data is configured*/
            return 0;
        }
        else
        if ((index == 0x1C33) 
            && ((sSyncManInPar.u16SyncTypesSupported & SYNCTYPE_SYNCHRONSUPP) > 0)
            && (nPdOutputSize == 0) 
            && (nPdInputSize > 0))
            {
                /*SyncManager sync mode is supported and input only shall be configured*/
                return 0;
            }
        break;

    case SYNCTYPE_SM2_SYNCHRON:
        if ((index == 0x1C33) 
            && ((sSyncManInPar.u16SyncTypesSupported & SYNCTYPE_SYNCHRONSUPP) > 0)
            && (nPdOutputSize > 0))
            {
                /*SyncManager sync mode is supported and outputs are defined*/
                return 0;
            }
        break;

    case SYNCTYPE_DCSYNC0:
        if ((index == 0x1C32) && ((sSyncManOutPar.u16SyncTypesSupported & SYNCTYPE_DCSYNC0SUPP) > 0))
        {
            return 0;
        }
        else
        if ((index == 0x1C33) && ((sSyncManInPar.u16SyncTypesSupported & SYNCTYPE_DCSYNC0SUPP) > 0))
        {
            return 0;
        }
        break;

    case SYNCTYPE_DCSYNC1:
        if ((index == 0x1C32) && ((sSyncManOutPar.u16SyncTypesSupported & SYNCTYPE_DCSYNC1SUPP) > 0))
        {
            return 0;
        }
        else
        if ((index == 0x1C33) && ((sSyncManInPar.u16SyncTypesSupported & SYNCTYPE_DCSYNC1SUPP) > 0))
        {
            return 0;
        }
        break;
    } //switch 
    return ABORTIDX_VALUE_EXCEEDED;

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index                 index of the requested object.
 \param     subindex                subindex of the requested object.
 \param    objSize                Size of the object, returned by the function OBJ_GetObjectLength
                                     which was called before
 \param     pObjEntry            handle to the dictionary object returned by
                                     OBJ_GetObjectHandle which was called before
 \param    pData                    Pointer to the buffer where the read data shall be copied to
 \param    bCompleteAccess    Indicates if a complete read of all subindices of the
                                     object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief    This function reads the requested object
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 OBJ_Read( UINT16 index, UINT8 subindex, UINT32 objSize, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData, UINT8 bCompleteAccess )
{
    UINT16 i = subindex;
    /* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    UINT8 objCode = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT;
    UINT16 maxSubindex = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT;
    OBJCONST TSDOINFOENTRYDESC OBJMEM *pEntry;
    /* lastSubindex is used for complete access to make loop over the requested entries
    to be read, we initialize this variable with the requested subindex that only
    one loop will be done for a single access */
    UINT16 lastSubindex = subindex;

    if ( objCode != OBJCODE_VAR && index >= 0x1000 )
    {
        /* if the object is an array or record we have to get the maxSubindex from the
        actual value of subindex 0, which is stored as UINT16 at the beginning of the
        object's variable */
        maxSubindex = (*((UINT16 *) (pObjEntry->pVarPtr))) & 0x00FF;

    }

    if ( bCompleteAccess )
    {
        if ( objCode == OBJCODE_VAR || index < 0x1000 )
        {
            /* complete access is not supported with simple objects or ENUM descriptions */
            return ABORTIDX_UNSUPPORTED_ACCESS;
        }


        /* we read until the maximum subindex */
        lastSubindex = maxSubindex;
    }
    else
        if ( subindex > maxSubindex )
        {
            /* the maximum subindex is reached */
            return ABORTIDX_SUBINDEX_NOT_EXISTING;
        }
        else
        {
            /* get the corresponding entry description */
            pEntry = OBJ_GetEntryDesc(pObjEntry, (UINT8)i);

            /*Check access only for non-align entries*/
            if(pEntry->ObjAccess != 0x0)
            {
                /* check if we have read access (bits 0-2 (PREOP, SAFEOP, OP) of ObjAccess)
                by comparing with the actual state (bits 1-3 (PREOP, SAFEOP, OP) of AL Status) */
                if (0 == (((UINT8) ((pEntry->ObjAccess & ACCESS_READ)<<1)) & (nAlStatus & STATE_MASK)))
                {
                    /* we don't have read access */
                    if ( (pEntry->ObjAccess & ACCESS_READ) == 0 )
                    {
                        /* it is a write only entry */
                        return ABORTIDX_WRITE_ONLY_ENTRY;
                    }
                    else
                    {
                        /* we don't have read access in this state */
                        return ABORTIDX_IN_THIS_STATE_DATA_CANNOT_BE_READ_OR_STORED;
                    }
                }
            }
            else
            {
                return ABORTIDX_UNSUPPORTED_ACCESS;
            }

        }

    /* ECATCHANGE_START(V5.13) COE4*/
        if (pAPPL_CoeReadInd != NULL)
        {
            pAPPL_CoeReadInd(index, subindex, bCompleteAccess);
        }
        /* ECATCHANGE_END(V5.13) COE4*/

        if ( pObjEntry->Read != NULL )
        {
            /* Read function is defined, we call the object specific read function */
            return pObjEntry->Read(index, subindex, objSize, pData, bCompleteAccess);
        }
        else if ( index < 0x1000 && subindex != 0 )
        {
            /* an ENUM description is read */
            UINT16 size;
            UINT16 MBXMEM *pVarPtr = (UINT16 MBXMEM *) pObjEntry->pVarPtr;
            CHAR **p;

            /* we get the corresponding entry description */
            pEntry = OBJ_GetEntryDesc(pObjEntry, subindex);
            size = BIT2BYTE(pEntry->BitLength);

            p = (CHAR **) pVarPtr;
            pVarPtr = (UINT16 MBXMEM *)p[subindex-1];

            /*ECATCHANGE_START(V5.13) */
            if((((MEM_ADDR)pVarPtr) & 0x1) == 0x1)
            {
                /*enum is stored at an odd word address*/
/*ECATCHANGE_END(V5.13) */
                UINT16 cnt = 0;

                //get last even WORD address
                pVarPtr = (UINT16 MBXMEM *)(((MEM_ADDR)pVarPtr)& ~(MEM_ADDR)0x1);

                for(cnt = 0; cnt < (size / 2);cnt++)
                {
                    //clear destination buffer
                    pData[0] = (pVarPtr[0] & 0xFF00) >> 8;
                    pData[0] |= (pVarPtr[1] & 0xFF) << 8;

                    pData++;
                    pVarPtr++;
                }

                if((size % 2) == 1)
                {
                    pData[0] &= 0xFF00; //Clear last Byte
                    pData[0] |= (pVarPtr[0] & 0xFF00) >> 8; //Write last byte
                }
            }
            else
            {
            // Get enum value (first 32Bit)
/*ECATCHANGE_START(V5.13) */

            pData[0] = pVarPtr[0];
            pData[1] = pVarPtr[1];
            pData += 2;
            pVarPtr += 2;
/*ECATCHANGE_END(V5.13) */

            // Get enum description
            OBJTOMBXSTRCPY(pData,pVarPtr,size-4);
            }
        }
        else
        {
            UINT8 bRead = 0x0;
            UINT8 result = 0;


            /* a variable object is read */
            for (i = subindex; i <= lastSubindex; i++)
            {
                /* if only a single entry is requested, this loop will only be done once */
                UINT16 MBXMEM *pVarPtr = (UINT16 MBXMEM *) pObjEntry->pVarPtr;
                UINT16 bitOffset = 0;

                
                if (i == 0)
                {
                    /* subindex 0 is requested, the entry's data is at the beginning of the object's variable */
                }
                else if ( index >= 0x1000 )
                {
                    /* subindex 1-n of an variable object is requested, we get the offset of the variable here */
                    bitOffset = OBJ_GetEntryOffset((UINT8)i, pObjEntry);
                }

                /* we increment the variable pointer to the corresponding word address */
                pVarPtr += (bitOffset >> 4);

                /* get the corresponding entry description */
                pEntry = OBJ_GetEntryDesc(pObjEntry, (UINT8)i);
                if (0 != (((UINT8) ((pEntry->ObjAccess & ACCESS_READ)<<1)) & (nAlStatus & STATE_MASK)) )
                {
                    if ( i == subindex                                     /* requested entry */
                        || (bCompleteAccess && i >= subindex) )       /* complete access and entry should be read */
                    {
                        UINT16 bitMask;

                        /* we have to copy the entry */
                        if ( i == 0 && objCode != OBJCODE_VAR )
                        {
                            /* we read subindex 0 of an array or record */
                            {
                                pData[0] = SWAPWORD((UINT16)maxSubindex);
                            }

                            /* we increment the destination pointer by 2 because the subindex 0 will be
                            transmitted as UINT16 for a complete access */
                            pData++;
                        }
                        else
                        {
                            UINT16 dataType = pEntry->DataType;
                            
                            if (pEntry->DataType >= 0x700)
                            {
                                /* the ENUM data types are defined from index 0x700 in this example
                                convert in standard data type for the read access */
                                if ( pEntry->BitLength <= 8 )
                                {
                                    dataType = DEFTYPE_BIT1-1+pEntry->BitLength;
                                }
                                else if ( pEntry->BitLength == 16 )
                                {
                                    dataType = DEFTYPE_UNSIGNED16;
                                }
                                else if ( pEntry->BitLength == 32 )
                                {
                                    dataType = DEFTYPE_UNSIGNED32;
                                }
                            }

                            switch (dataType)
                            {
                            case DEFTYPE_NULL:
                                if(bCompleteAccess)
                                {
                                    /*Handle alignment entry*/
                                    if (((pEntry->BitLength & 0xF) > 0)
                                        && (((bitOffset + pEntry->BitLength) & 0x0F) == 0 ))
                                    {
                                        /* we have reached the UINT16 border */
                                        pData++;
                                    }

                                    /*increment WORD offset*/
                                    pData += ((pEntry->BitLength & 0xF0) >> 4);
                                }
                                else
                                {
                                    return ABORTIDX_DATA_CANNOT_BE_READ_OR_STORED;
                                }
                                break;
                            case     DEFTYPE_BOOLEAN:
                            case     DEFTYPE_BIT1:
                            case     DEFTYPE_BIT2:
                            case     DEFTYPE_BIT3:
                            case     DEFTYPE_BIT4:
                            case     DEFTYPE_BIT5:
                            case     DEFTYPE_BIT6:
                            case     DEFTYPE_BIT7:
                            case     DEFTYPE_BIT8:
                            case     DEFTYPE_BITARR8:
                                /* in this example the objects are defined in that way,
                                that the bit types are always inside a 16-bit field,
                                and they shall not overlap a byte border*/
                            case    DEFTYPE_INTEGER8:
                            case    DEFTYPE_UNSIGNED8:
                            case    DEFTYPE_BYTE :
                                {
                                    /* depending on the bitOffset we have to copy the Hi or the Lo-Byte */
                                    UINT16 TmpValue = 0x0000;

                                    
                                    bitMask = cBitMask[pEntry->BitLength] << (bitOffset & 0x0F);

                                    /*Swap object data (if required); all masks and offsets are defined for little endian format*/
                                    TmpValue = SWAPWORD(pVarPtr[0]);

                                    /*Clear pData if the first bits within the WORD memory will be copied*/
                                    if ((bitOffset & 0x0F) == 0) 
                                    {
                                        pData[0] = 0;
                                    }

                                    pData[0] = SWAPWORD(pData[0]);

                                    if (bCompleteAccess) 
                                    {
                                        /*shifting is not required for Complete access because the bits are set to the correct offset by the master*/
                                        pData[0] |= TmpValue & bitMask;
                                    }
                                    else
                                    {
                                        /*Shift Bits to the beginning of the mailbox memory*/
                                        pData[0] |= ((TmpValue & bitMask) >> (bitOffset & 0x0F));
                                    }

                                    pData[0] = SWAPWORD(pData[0]);
                                    if ( ((bitOffset + pEntry->BitLength) & 0x0F) == 0 )
                                    {
                                        /* we have reached the UINT16 border */
                                        pData++;
                                    }

                                }
                                break;
                            case    DEFTYPE_INTEGER16:
                            case    DEFTYPE_UNSIGNED16:
                            case    DEFTYPE_BITARR16:
                            case    DEFTYPE_WORD:

                                if(bitOffset & 0xF)
                                {
                                    /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                    return ABORTIDX_UNSUPPORTED_ACCESS;
                                }
                                /* in this example the objects are defined in that way,
                                that the 16 bit type are always starting at an exact WORD offset */
                                pData[0] = SWAPWORD(pVarPtr[0]);
                                pData++;
                                break;
                            case    DEFTYPE_UNSIGNED32:
                            case    DEFTYPE_INTEGER32:
                            case    DEFTYPE_REAL32:
                            case    DEFTYPE_BITARR32:
                            case    DEFTYPE_DWORD:
                                if(bitOffset & 0xF)
                                {
                                    /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                    return ABORTIDX_UNSUPPORTED_ACCESS;
                                }

                                /* in this example the objects are defined in that way,
                                that the 32 bit type are always starting at an exact WORD offset */
                                pData[0] = pVarPtr[0];
                                pData[1] = pVarPtr[1];
                                pData += 2;
                                break;
                            case    DEFTYPE_REAL64:
                            case 	DEFTYPE_INTEGER64:
                            case    DEFTYPE_UNSIGNED64:
                                if(bitOffset & 0xF)
                                {
                                    /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                    return ABORTIDX_UNSUPPORTED_ACCESS;
                                }

                                /* in this example the objects are defined in that way,
                                that the 64 bit type are always starting at an exact WORD offset */
                                pData[0] = pVarPtr[0];
                                pData[1] = pVarPtr[1];
                                pData[2] = pVarPtr[2];
                                pData[3] = pVarPtr[3];
                                pData += 4;
                                break;
                            case    DEFTYPE_OCTETSTRING:
                            case    DEFTYPE_UNICODE_STRING:
                            case DEFTYPE_ARRAY_OF_INT :
                            case DEFTYPE_ARRAY_OF_SINT :
                            case DEFTYPE_ARRAY_OF_DINT :
                            case DEFTYPE_ARRAY_OF_UDINT:

                                if(bitOffset & 0xF)
                                {
                                    /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                    return ABORTIDX_UNSUPPORTED_ACCESS;
                                }

                                OBJTOMBXMEMCPY(pData, pVarPtr, BIT2BYTE(pEntry->BitLength));

                                pData += BIT2WORD((pEntry->BitLength & ~0xF));
                                
                                if((pEntry->BitLength & 0xF) != 0)
                                {
                                    /*current entry has an odd word length => clear last byte of next word*/
                                    *pData &= 0xFF;
                                }

                                break;
                            case    DEFTYPE_VISIBLESTRING:
                                if(bitOffset & 0xF)
                                {
                                    /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                    return ABORTIDX_UNSUPPORTED_ACCESS;
                                }

                                /* in this example the objects are defined in that way,
                                that these types are always starting at an even WORD offset */
                                OBJTOMBXSTRCPY(pData, pVarPtr, BIT2BYTE(pEntry->BitLength));
                                pData += BIT2WORD((pEntry->BitLength & ~0xF));

                                if((pEntry->BitLength & 0xF) != 0)
                                {
                                    /*current entry has an odd word length => clear last byte of next word*/
                                    *pData &= 0xFF;
                                }
                                
                                break;
                            default:
                                /* other data types are not supported from this example */
                                return ABORTIDX_DATA_CANNOT_BE_READ_OR_STORED;
                            } //switch (deftype)

                            bRead = 1;
                        }
                    }
                }
                else
                {
                    /*No access to current object entry => shift pData if required*/
                    UINT8 cnt = 0;

                    /*If this entry is the first in the 16Bit block clear the memory*/
                    if ((bitOffset & 0x0F) == 0) 
                    {
                        *pData = 0;
                    }

                    /*Handle Bit/Byte Offset*/
                    if (((pEntry->BitLength & 0xF) > 0)
                        && (((bitOffset + pEntry->BitLength) & 0x0F) == 0 ))
                    {
                        /* we have reached the UINT16 border */
                        pData++;
                        
                        /*Clear new buffer*/
                        *pData = 0;
                    }

                    /*increment WORD offset*/
                    for(cnt = 0; cnt <((pEntry->BitLength & 0xF0) >> 4); cnt++)
                    {
                        
                        /*current 16Bit are skipped => clear current buffer */
                        pData++;

                        /*Clear new buffer*/
                        *pData = 0;
                    }


                    

                    /* we don't have read access */
                    if ( (pEntry->ObjAccess & ACCESS_READ) == 0 )
                    {
                        /* it is a write only entry */
                        result = ABORTIDX_WRITE_ONLY_ENTRY;
                    }
                    else
                    {
                        /* we don't have read access in this state */
                        result = ABORTIDX_IN_THIS_STATE_DATA_CANNOT_BE_READ_OR_STORED;
                    }
                }
            }

            if(bRead == 0)
            {
                return result;
            }
        }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     index                 index of the requested object.
 \param     subindex                subindex of the requested object.
 \param    dataSize                received data size of the SDO Download
 \param     pObjEntry            handle to the dictionary object returned by
                                     OBJ_GetObjectHandle which was called before
 \param    pData                    Pointer to the buffer where the written data can be copied from
 \param    bCompleteAccess    Indicates if a complete write of all subindices of the
                                     object shall be done or not

 \return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdoserv.h))

 \brief    This function writes the requested object
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 OBJ_Write( UINT16 index, UINT8 subindex, UINT32 dataSize, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData, UINT8 bCompleteAccess )
{
    UINT16 i = subindex;
    /* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    UINT8 objCode = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT;
    UINT16 maxSubindex = 0;
    UINT16 maxConfiguredSubindex = (pObjEntry->ObjDesc.ObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT; //required to check if value for Subindex0 is valid
    OBJCONST TSDOINFOENTRYDESC OBJMEM *pEntry;
    BOOL bClearSubindex0Required = FALSE;
    /* lastSubindex is used for complete access to make loop over the requested entries
       to be read, we initialize this variable with the requested subindex that only
       one loop will be done for a single access */
    UINT16 lastSubindex = subindex;

    /* if subindex 0 is writable, the maximum subindex should be checked in an object specific function,
        because for the PDO mapping and PDO assign the object shall only be written if subindex 0 is 0. */
    if ( objCode != OBJCODE_VAR )
    {
        /* if the object is an array or record we have to get the maxSubindex from the
           actual value of subindex 0, which is stored as UINT16 at the beginning of the
            object's variable */
        maxSubindex = (UINT8) ((UINT16 MBXMEM *) (pObjEntry->pVarPtr))[0];

        /*If the subindex0 of a PDO assign or PDO mapping object is 0 the maximum subindex is specified by the object description*/
        if(maxSubindex == 0 && (IS_PDO_ASSIGN(index) || IS_RX_PDO(index) || IS_TX_PDO(index) || (index == 0xF030)))
        {
            maxSubindex = maxConfiguredSubindex;
        }
    }


    if ( bCompleteAccess )
    {
        if ( objCode == OBJCODE_VAR )
        {
            /* complete access is not supported with simple objects */
            return ABORTIDX_UNSUPPORTED_ACCESS;
        }

        if ((subindex == 0) && (dataSize > 0))
        {
            /* we change the subindex 0 */
            maxSubindex = (UINT8) SWAPWORD(pData[0]);
        }


        /* we write until the maximum subindex */
        lastSubindex = maxSubindex;
    }
    else
    if (subindex > maxSubindex)
    {
        /* the maximum subindex is reached */
        return ABORTIDX_SUBINDEX_NOT_EXISTING;
    }
    else
    {
        /* we check the write access for single accesses here, a complete write access
           is allowed if at least one entry is writable (in this case the values for the
            read only entries shall be ignored) */
        /* we get the corresponding entry description */
        pEntry = OBJ_GetEntryDesc(pObjEntry, subindex);

        /* check if we have write access (bits 3-5 (PREOP, SAFEOP, OP) of ObjAccess)
           by comparing with the actual state (bits 1-3 (PREOP, SAFEOP, OP) of AL Status) */
        if (0 == (((UINT8)((pEntry->ObjAccess & ACCESS_WRITE) >> 2)) & (nAlStatus & STATE_MASK) ))
        {
            /* we don't have write access */
            if ( (pEntry->ObjAccess & ACCESS_WRITE) == 0 )
            {
                if (pEntry->ObjAccess == 0)
                {
                        return ABORTIDX_UNSUPPORTED_ACCESS;
                }
                else
                {
                        /* it is a read only entry */
                        return ABORTIDX_READ_ONLY_ENTRY;
                }
            }
            else
            {
                /* we don't have write access in this state */
                return ABORTIDX_IN_THIS_STATE_DATA_CANNOT_BE_READ_OR_STORED;
            }
        }


    }

    /* Subindex 0 shall be set to zero if a single PDO / PDO assign entry is written
    or a complete access without subindex0 is requested */
    if((subindex > 0) &&
        (IS_PDO_ASSIGN(index) || IS_RX_PDO(index)|| IS_TX_PDO(index) || (index == 0xF030))
        )
    {
        /*Check if Subindex0 was cleared before*/
        UINT16 Subindex0 = (*(UINT16 *)pObjEntry->pVarPtr) & 0x00FF;
        if(Subindex0 != 0x00)
        {
            bClearSubindex0Required = TRUE;
        }
    }

    if ( pObjEntry->Write != NULL )
    {
        /* Write function is defined, we call the object specific write function */
        /* ECATCHANGE_START(V5.13) COE4*/
        UINT8 result = 0;

        result = pObjEntry->Write(index, subindex, dataSize, pData, bCompleteAccess);

        if ((result == 0) && (pAPPL_CoeWriteInd != NULL))
        {
            pAPPL_CoeWriteInd(index, subindex, bCompleteAccess);
        }
        
        return result;
        /* ECATCHANGE_END(V5.13) COE4*/
    }
    else
    {
        UINT8 bWritten = 0;
        UINT8 result = ABORTIDX_READ_ONLY_ENTRY;

        if (dataSize == 0)
        {
           return 0; //no error
        }

        /* we use the standard write function */
        for (i = subindex; i <= lastSubindex; i++)
        {
            /* if only a single entry is requested, this loop will only be done once */
            UINT16 MBXMEM *pVarPtr = (UINT16 MBXMEM *) pObjEntry->pVarPtr;
            UINT16 bitOffset = 0;

            /* we get the corresponding entry description */
            pEntry = OBJ_GetEntryDesc(pObjEntry,(UINT8) i);

            /*Get the bitOffset before check the access rights to calculate pData offset*/
            bitOffset = OBJ_GetEntryOffset((UINT8)i, pObjEntry);

            /* we check if we have write access (bits 3-5 (PREOP, SAFEOP, OP) of ObjAccess)
               by comparing with the actual state (bits 1-3 (PREOP, SAFEOP, OP) of AL Status) */
            if (0 != (((UINT8)((pEntry->ObjAccess & ACCESS_WRITE) >> 2)) & (nAlStatus & STATE_MASK) ))
            {
                /* we have write access for this entry */
                if (i != 0)
                {
                    /* we increment the variable pointer to the corresponding word address */
                    pVarPtr += (bitOffset >> 4);
                }


                if ( i == subindex                                     /* requested entry */
                  || (bCompleteAccess && i >= subindex) )       /* complete access and entry should be read */
                {
                    UINT16 bitMask;

                    /* we have to copy the entry */
                    if (i == 0 && objCode != OBJCODE_VAR)
                    {
                        /*check if the value for subindex0 is valid */
                        UINT8 NewSubindex0 = (UINT8) SWAPWORD(pData[0]);
                        
                        if(maxConfiguredSubindex < NewSubindex0)
                        {
                            return ABORTIDX_VALUE_TOO_GREAT;
                        }

                        /* subindex 0 of an array or record shall be written */
                        pVarPtr[0] = SWAPWORD(pData[0]);
                        /* we increment the destination pointer by 2 because the subindex 0 will be
                           transmitted as UINT16 for a complete access */
                        pData++;
                    }
                    else
                    {
                        UINT16 dataType = pEntry->DataType;
                        if (pEntry->DataType >= 0x700)
                        {
                            /* the ENUM data types are defined from index 0x700 in this example
                               convert in standard data type for the write access */
                            if ( pEntry->BitLength <= 8 )
                            {
                                dataType = DEFTYPE_BIT1-1+pEntry->BitLength;
                            }
                            else if ( pEntry->BitLength == 16 )
                            {
                                dataType = DEFTYPE_UNSIGNED16;
                            }
                            else if ( pEntry->BitLength == 32 )
                            {
                                dataType = DEFTYPE_UNSIGNED32;
                            }
                        }

                        switch (dataType)
                        {
                        case DEFTYPE_NULL:
                            if(bCompleteAccess)
                            {
                                /*Handle alignment entry*/
                                if (((pEntry->BitLength & 0xF) > 0)
                                    && (((bitOffset + pEntry->BitLength) & 0x0F) == 0 ))
                                {
                                    /* we have reached the UINT16 border */
                                    pData++;
                                }

                                /*increment WORD offset*/
                                pData += ((pEntry->BitLength & 0xF0) >> 4);
                            }
                            else
                            {
                                return ABORTIDX_DATA_CANNOT_BE_READ_OR_STORED;
                            }
                            break;
                        case     DEFTYPE_BOOLEAN:
                        case     DEFTYPE_BIT1:
                        case     DEFTYPE_BIT2:
                        case     DEFTYPE_BIT3:
                        case     DEFTYPE_BIT4:
                        case     DEFTYPE_BIT5:
                        case     DEFTYPE_BIT6:
                        case     DEFTYPE_BIT7:
                        case     DEFTYPE_BIT8:
                        case     DEFTYPE_BITARR8:
                            /* in this example the objects are defined in that way,
                               that the bit types are always inside a 16-bit field,
                               and shall not overlap a byte border*/
                        case    DEFTYPE_INTEGER8:
                        case    DEFTYPE_UNSIGNED8:
                        case    DEFTYPE_BYTE :
                        {
                            /* depending on the bitOffset we have to copy the Hi or the Lo-Byte */
                            UINT16 TmpValue = 0x0000;

                            bitMask = cBitMask[pEntry->BitLength] << (bitOffset & 0x0F);

                            /*Swap object data (if required); all masks and offsets are defined for little endian format*/
                            TmpValue = SWAPWORD(pVarPtr[0]);

                            /*Clear corresponding bits*/
                            TmpValue &= ~bitMask;

                            if (bCompleteAccess) 
                            {
                                /*shifting is not required for Complete access because the bits are set to the correct offset by the master*/
                                TmpValue |= (SWAPWORD(pData[0]) & bitMask);
                            }
                            else
                            {
                                if((SWAPWORD(pData[0]) & ~cBitMask[pEntry->BitLength]))
                                {
                                    /*written value exceed entry range*/
                                    return ABORTIDX_VALUE_EXCEEDED;
                                }
                                else
                                {
                                    /*Shift Bits to corresponding offset within the object memory*/
                                    TmpValue |= ((SWAPWORD(pData[0]) & cBitMask[pEntry->BitLength]) << (bitOffset & 0x0F));
                                }
                            }

                            /*Swap written data to big endian format (if required)*/
                            pVarPtr[0] = SWAPWORD(TmpValue);

                            if ( ((bitOffset+pEntry->BitLength) & 0x0F) == 0 )
                            {
                                /* we have reached the UINT16 border */
                                pData++;
                            }
                        }
                            break;
                        case    DEFTYPE_INTEGER16:
                        case    DEFTYPE_UNSIGNED16:
                        case    DEFTYPE_BITARR16:
                        case    DEFTYPE_WORD:
                            if(bitOffset & 0xF)
                            {
                                /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                return ABORTIDX_UNSUPPORTED_ACCESS;
                            }

                            {
                            /* in this example the objects are defined in that way,
                            that the 16 bit type are always starting at an exact WORD offset */
                            UINT16 u16NewData = SWAPWORD(pData[0]);
                            if(bClearSubindex0Required && (pVarPtr[0] != u16NewData))
                            {
                                /* try to write new data when subindex0 shall be reset*/
                                return ABORTIDX_ENTRY_CANT_BE_WRITTEN_SI0_NOT_0;
                            }

                            /*check value if a new PDO assign entry should be written*/
                            if(IS_PDO_ASSIGN(index))  //PDO assign
                            {
                                if (!IS_RX_PDO(u16NewData) && !IS_TX_PDO(u16NewData) && (u16NewData != 0)) //check if the new assign entry value is valid
                                {
                                    return ABORTIDX_VALUE_EXCEEDED;
                                }
                            }

                            if(index == 0x1C32 || index == 0x1C33)
                            {
                               if (i == 1) /* "Synchronisation type" written */
                               {
                                  /* The Synchronisation type (0x1C3x.1) was written by the user => the Sync type will not be calculated based on the register settings (if they don't match an error will be returned P_2_S)*/
                                  if (pVarPtr[0] != u16NewData)
                                  {
                                     result = CheckSyncTypeValue(index, u16NewData);

                                     if (result != 0)
                                     {
                                        return result;
                                     }
                                  }

                                  /* The user may force to current Sync Mode for that reason the flag has also to be set if the same value was written */
                                  bSyncSetByUser = TRUE;
                               }

                               if (i == 8) /* "Get Cycle Time" written*/
                               {

                                   sSyncManOutPar.u32CalcAndCopyTime = (PD_OUTPUT_CALC_AND_COPY_TIME);
                                   sSyncManOutPar.u32MinCycleTime = (MIN_PD_CYCLE_TIME);
                                   sSyncManOutPar.u32CycleTime = 0;

                                   sSyncManInPar.u32CalcAndCopyTime = (PD_INPUT_CALC_AND_COPY_TIME);
                                   sSyncManInPar.u32MinCycleTime = (MIN_PD_CYCLE_TIME);
                                   sSyncManInPar.u32CycleTime = 0;


                                  if ((u16NewData & 0x2) == 0x2)
                                  {
                                     /* reset the error counters*/
                                     sSyncManOutPar.u16CycleExceededCounter = 0;
                                     sSyncManOutPar.u16SmEventMissedCounter = 0;
                                     sSyncManOutPar.u8SyncError = 0;

                                     sSyncManInPar.u16CycleExceededCounter = 0;
                                     sSyncManInPar.u16SmEventMissedCounter = 0;
                                     sSyncManInPar.u8SyncError = 0;
                                  }
                               } /* Subindex 8 written*/
                            }

                            pVarPtr[0] = u16NewData;
                            pData++;
                            }
                            break;
                        case    DEFTYPE_UNSIGNED32:
                        case    DEFTYPE_INTEGER32:
                        case    DEFTYPE_REAL32:
                        case    DEFTYPE_BITARR32:
                        case    DEFTYPE_DWORD:
                            if(bitOffset & 0xF)
                            {
                                /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                return ABORTIDX_UNSUPPORTED_ACCESS;
                            }
                            {

                            /* in this example the objects are defined in that way,
                               that the 32 bit type are always starting at an exact WORD offset */
                            if(bClearSubindex0Required && 
                                ((pVarPtr[0] != pData[0])
                                || (pVarPtr[1] != pData[1])))
                            {
                                /* try to write new data when subindex0 shall be reset*/
                                return ABORTIDX_ENTRY_CANT_BE_WRITTEN_SI0_NOT_0;
                            }

                            pVarPtr[0] = pData[0];
                            pVarPtr[1] = pData[1];
                            pData += 2;
                            }
                            break;
                        case    DEFTYPE_REAL64:
                        case 	DEFTYPE_INTEGER64:
                        case    DEFTYPE_UNSIGNED64:
                            if(bitOffset & 0xF)
                            {
                                /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                return ABORTIDX_UNSUPPORTED_ACCESS;
                            }
                            /* in this example the objects are defined in that way,
                               that the 64 bit type are always starting at an exact WORD offset */
                            pVarPtr[0] = pData[0];
                            pVarPtr[1] = pData[1];
                            pVarPtr[2] = pData[2];
                            pVarPtr[3] = pData[3];
                            pData += 4;
                            break;
                        case    DEFTYPE_VISIBLESTRING:
                            if(bitOffset & 0xF)
                            {
                                /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                return ABORTIDX_UNSUPPORTED_ACCESS;
                            }

                            OBJTOMBXSTRCPY(pVarPtr, pData, BIT2BYTE(pEntry->BitLength));

                            pData += BIT2WORD((pEntry->BitLength)& ~0xF);
                            break;
                        case    DEFTYPE_OCTETSTRING:
                        case    DEFTYPE_UNICODE_STRING:
                            case DEFTYPE_ARRAY_OF_INT :
                            case DEFTYPE_ARRAY_OF_SINT :
                            case DEFTYPE_ARRAY_OF_DINT :
                            case DEFTYPE_ARRAY_OF_UDINT:
                            if(bitOffset & 0xF)
                            {
                                /* return an error in case of an odd word offset (to support 16bit data type on odd word addresses an object specific access function need to be implemented and register in the object dictionary, see SSC Application Note)*/
                                return ABORTIDX_UNSUPPORTED_ACCESS;
                            }

                            /* in this example the objects are defined in that way,
                               that the other types are always starting at an even byte offset */
                            OBJTOMBXMEMCPY(pVarPtr, pData, BIT2BYTE(pEntry->BitLength));
                            pData += BIT2WORD((pEntry->BitLength) & ~0xF);

                            break;
                        default:
                            /* other data types are not supported from this example */
                            return ABORTIDX_DATA_CANNOT_BE_READ_OR_STORED;
                        }
                    }

                    /* set flag */
                    bWritten = 1;

                }
            }
            else
            {
                if(i == 0)
                {
                    /* For SubIndex0 16Bit are reserved even if the BitLength is 8 */
                    pData++;
                }
                else
                {
                    /*Handle Bit/Byte Offset*/
                    if (((pEntry->BitLength & 0xF) > 0)
                        && (((bitOffset + pEntry->BitLength) & 0x0F) == 0 ))
                    {
                        /* we have reached the UINT16 border */
                        pData++;
                    }

                    /*increment WORD offset*/
                    pData += ((pEntry->BitLength & 0xFFF0) >> 4);
                }
                /*If no other entry was written this result will be returned*/
                result = ABORTIDX_DATA_CANNOT_BE_READ_OR_STORED;
            }
        }

        if (bWritten == 0)
        {
            /* we didn't write anything, so we have to return the stored error code */
            return result;
        }
    }

    /* ECATCHANGE_START(V5.13) COE4*/
    if (pAPPL_CoeWriteInd != NULL)
    {
        pAPPL_CoeWriteInd(index, subindex, bCompleteAccess);
    }
    /* ECATCHANGE_END(V5.13) COE4*/

    return 0;
}
/** @} */



