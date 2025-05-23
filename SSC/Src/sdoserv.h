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
\file sdoserv.h
\author EthercatSSC@beckhoff.com
\brief SDO Service definitions

\version 5.12

<br>Changes to version V5.11:<br>
V5.12 ECAT2: big endian changes<br>
V5.12 MBX3: handle incomplete mailbox communication<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 SDO10: add new SDO abort code 0x6010004 (complete access not supported)<br>
<br>Changes to version V5.01:<br>
V5.10 SDO3: Add new SDO Abort Code (0x06090033)<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecatcoe.h"


#ifndef _SDOSERV_H_
#define _SDOSERV_H_

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

/**
 * \addtogroup SdoErrorCodes SDO Error Codes
 * @{
 */
#define     ERROR_SDOINVALIDCOMMAND             0x1101 /**< \brief  Invalid SDO command*/
#define     ERROR_SDOINVALIDHEADER              0x1102 /**< \brief  Invalid SDO header*/
#define     ERROR_SDONOTSUPPORTED               0x1103 /**< \brief  SDO service not supported*/
/** @}*/


/**
 * \addtogroup SdoHeaderData SDO Header and Data
 * @{
 */
#define     SDOHEADER_SIZEINDICATOR             ((UINT8) 0x01) /**< \brief Size Indicator size of data in Data Set Size specified*/
#define     SDOHEADER_TRANSFERTYPE              ((UINT8) 0x02) /**< \brief Transfer Type<br>1: Expedited<br>0: Normal*/
#define     SDOHEADER_DATASETSIZE               ((UINT8) 0x0C) /**< \brief Data Set Size (used for Expedited transfer)*/
#define     SDOHEADER_COMPLETEACCESS            ((UINT8) 0x10) /**< \brief Complete Access indication<br>0: Only specified entry will be transferred<br>1: Whole object (with or without Subindex0) will be transferred)*/ 
#define     SDOHEADER_COMMAND                   ((UINT8) 0xE0) /**< \brief SDO Command*/
#define     SDOHEADERSHIFT_SIZEINDICATOR        ((UINT8) 0) /**< \brief Shift for Size Indicator*/
#define     SDOHEADERSHIFT_TRANSFERTYPE         ((UINT8) 1) /**< \brief Shift for Transfer Type*/
#define     SDOHEADERSHIFT_DATASETSIZE          ((UINT8) 2) /**< \brief Shift for Data Set Size*/
#define     SDOHEADERSHIFT_INDEXACCESS          ((UINT8) 4) /**< \brief Shift for Complete Access*/
#define     SDOHEADERSHIFT_COMMAND              ((UINT8) 5) /**< \brief Shift for SDO Command*/
/** @}*/


/**
 * \addtogroup SdoServices SDO Services
 * @{
 */
#define     SDOSERVICE_INITIATEDOWNLOADREQ      ((UINT8) (0x01 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO download request*/
#define     SDOSERVICE_INITIATEDOWNLOADRES      ((UINT8) (0x03 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO download response*/
#define     SDOSERVICE_DOWNLOADSEGMENTREQ       ((UINT8) (0x00 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO segmented download request*/
#define     SDOSERVICE_DOWNLOADSEGMENTRES       ((UINT8) (0x01 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO segmented download response*/
#define     SDOSERVICE_INITIATEUPLOADREQ        ((UINT8) (0x02 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO upload request*/
#define     SDOSERVICE_INITIATEUPLOADRES        ((UINT8) (0x02 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO upload response*/
#define     SDOSERVICE_UPLOADSEGMENTREQ         ((UINT8) (0x03 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO segmented upload request*/
#define     SDOSERVICE_UPLOADSEGMENTRES         ((UINT8) (0x00 << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO segmented upload response*/
#define     SDOSERVICE_ABORTTRANSFER            ((UINT8) (((UINT8) 0x04) << (SDOHEADERSHIFT_COMMAND))) /**< \brief SDO abort*/
/** @}*/


/**
 * \addtogroup SdoHeaderData SDO Header and Data
 * @{
 */
/**
 * Structure to handle the basic SDO header
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16 Sdo[2]; /**< \brief 32Bit SDO header buffer*/
            #define     SDOHEADER_COMMANDOFFSET             0 /**< \brief Memory offset for the command*/
            #define     SDOHEADER_INDEXLOOFFSET             0 /**< \brief Memory offset for the low Byte of the object index*/
            #define     SDOHEADER_INDEXHIOFFSET             1 /**< \brief Memory offset for the high Byte of the object index*/
            #define     SDOHEADER_SUBINDEXOFFSET            1 /**< \brief Memory offset for subindex*/
            #define     SDOHEADER_COMMANDMASK               0xFF /**< \brief Mask to get the command Byte*/
            #define     SDOHEADER_COMMANDSHIFT              0 /***< \brief Shift to get the command byte*/
            #define     SDOHEADER_INDEXLOSHIFT              8 /**< \brief Shift to get the low Byte of the object index*/
            #define     SDOHEADER_INDEXLOMASK               0x00FF /** < \brief Mask to get the low byte of the object index */
            #define     SDOHEADER_INDEXHIMASK               0xFF /**< \brief Mask to get the high byte of the object index*/
            #define     SDOHEADER_SUBINDEXSHIFT             8 /**< \brief Shift to get the subindex*/
            #define     SDOHEADER_SUBINDEXMASK              0xFF /**< \brief Mask to get the subindex*/
}MBX_STRUCT_PACKED_END
TINITSDOHEADER;


#define INITSDO_HEADER_SIZE     4 /**< \brief Size of the SDO header*/


/**
 *Complete SDO datagram including mailbox header, CoE header and SDO header
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
}MBX_STRUCT_PACKED_END
TINITSDOMBX;


#define     MAX_EXPEDITED_DATA          4 /**< \brief Maximum number of Bytes handled via an expedited transfer*/
#define     MIN_SEGMENTED_DATA          ((UINT16) 7) /**< \brief Minimum number of Bytes required for a segmented transfer*/
#define     EXPEDITED_FRAME_SIZE        ( COE_HEADER_SIZE + INITSDO_HEADER_SIZE + (MAX_EXPEDITED_DATA) ) /**< \brief Size of an expedited mailbox datagram*/
#define     DOWNLOAD_NORM_REQ_SIZE      ( COE_HEADER_SIZE + INITSDO_HEADER_SIZE + 4 ) /**< \brief Size of an normal download request*/
/* HBu 06.02.06: names of defines changed */
/* HBu 21.03.06: the SDO_Download-Response has to have always 8 bytes */
#define     DOWNLOAD_NORM_RES_SIZE      ( COE_HEADER_SIZE + INITSDO_HEADER_SIZE + 4 ) /**< \brief Frame size of an normal download response*/
#define     UPLOAD_NORM_RES_SIZE        ( COE_HEADER_SIZE + INITSDO_HEADER_SIZE + 4 ) /**< \brief Frame size of an normal upload response*/
#define     SEGMENT_NORM_HEADER_SIZE    ( COE_HEADER_SIZE + 1 ) /**< \brief Minimum SDO header size*/
#define     SEGMENT_NORM_RES_SIZE       ( (SEGMENT_NORM_HEADER_SIZE) + (MIN_SEGMENTED_DATA) ) /**< \brief Minimum SDO size*/
#define     SEGMENT_MBX_SIZE            ( MBX_HEADER_SIZE + (SEGMENT_NORM_HEADER_SIZE) )/**< \brief Minimum mailbox datagram*/


/**
 * Segmented SDO header including data
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16      SegHeader; /**< \brief 8Bit header buffer and first 8Bit of the Data*/
#define    SEGHEADER_MASK                    ((UINT16) 0xFF) /**< \brief SDO header mask*/
#define    SEGHEADER_NOMOREFOLLOWS           ((UINT16) 0x01) /**< \brief More segments follow flag*/
#define    SEGHEADER_SEGDATASIZE             ((UINT16) 0x0E) /**< \brief Segmented data size*/
#define    SEGHEADER_TOGGLE                  ((UINT16) 0x10) /**< \brief Toogle flag*/
#define    SEGHEADER_COMMAND                 ((UINT16) 0xE0) /**< \brief Command identifier*/
#define    SEGHEADERSHIFT_SEGDATASIZE        ((UINT16) 1) /**< \brief Shift for Segmented data size*/
#define    SEGHEADERSHIFT_TOGGLE             ((UINT16) 4) /**< \brief Shift for toggle flag*/
#define    SEGHEADERSHIFT_COMMAND            ((UINT16) 5) /**< \brief Shift for command identifier*/
#define    SEGHDATA_MASK                     ((UINT16) 0xFF00) /**< \brief Data mask*/
#define    SEGDATASHIFT                      ((UINT16) 8) /**< \brief Shift for first data Byte*/
    UINT16      Data[(((MAX_MBX_DATA_SIZE)-(SEGMENT_NORM_HEADER_SIZE) - 1 ) >> 1)];  /**< \brief Data buffer (second and following Bytes, the first byte is located in the header variable)*/
}MBX_STRUCT_PACKED_END
TSDOSEGHEADERDATA;


/**
 *Expedited download request
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
  UINT16                Data[(MAX_EXPEDITED_DATA) >> 1]; /**< \brief Data buffer*/
}MBX_STRUCT_PACKED_END
TINITSDODOWNLOADEXPREQMBX;

/**
 *Normal download request
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
  UINT16                CompleteSize[2]; /**< \brief Complete object size*/
  UINT16                Data[(((MAX_MBX_DATA_SIZE)-(DOWNLOAD_NORM_REQ_SIZE)) >> 1)]; /**< \brief Data buffer*/
}MBX_STRUCT_PACKED_END
TINITSDODOWNLOADNORMREQMBX;


/**
 *Expedited and normal download response:
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
}MBX_STRUCT_PACKED_END
TINITSDODOWNLOADRESMBX;


/**
 *Segmented download request
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TSDOSEGHEADERDATA     SdoHeader; /**< \brief SDO header (data is included in header)*/
}MBX_STRUCT_PACKED_END
TDOWNLOADSDOSEGREQMBX;


/**
 *Segmented download response
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  UINT8                 SegHeader; /**< \brief SDO header*/
}MBX_STRUCT_PACKED_END
TDOWNLOADSDOSEGRESMBX;


/**
 *Expedited and normal upload request
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
}MBX_STRUCT_PACKED_END
TINITSDOUPLOADREQMBX;


/**
 *Expedited upload response
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
  UINT16                Data[((MAX_EXPEDITED_DATA) >> 1)]; /**< \brief Data buffer*/
}MBX_STRUCT_PACKED_END
TINITSDOUPLOADEXPRESMBX;


/**
 *Normal upload response
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
  UINT16                CompleteSize[2]; /**< \brief Complete object size*/
  UINT16                Data[(((MAX_MBX_DATA_SIZE)-(UPLOAD_NORM_RES_SIZE)) >> 1)]; /**< \brief Data buffer*/
}MBX_STRUCT_PACKED_END
TINITSDOUPLOADNORMRESMBX;


/**
 *Segmented upload request
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  UINT8                 SegHeader; /**< \brief SDO header*/
}MBX_STRUCT_PACKED_END
TUPLOADSDOSEGREQMBX;

/**
 *Segmented upload response
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TSDOSEGHEADERDATA     SdoHeader; /**< \brief SDO header (data is included in header)*/
}MBX_STRUCT_PACKED_END
TUPLOADSDOSEGRESMBX;


/**
 *Abort request
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER            MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER            CoeHeader; /**< \brief CoE header*/
  TINITSDOHEADER        SdoHeader; /**< \brief SDO header*/
  UINT32             AbortCode; /**< \brief SDO abort code*/
}MBX_STRUCT_PACKED_END
TABORTSDOTRANSFERREQMBX;


#define ABORT_NORM_RES_SIZE    (SIZEOF(TABORTSDOTRANSFERREQMBX) - SIZEOF(TMBXHEADER))/**< \brief Size of the SDO Abort datagram (without the mailbox header)*/
/** @}*/


/**
 * \addtogroup SDOAbort SDO Abort Codes
 * @{
 */
#define     ABORT_NOERROR                                                   0x00000000 /**< \brief No SDO error*/
#define     ABORT_TOGGLE_BIT_NOT_CHANGED                                    0x05030000 /**< \brief Toggle bit not changed*/
#define     ABORT_SDO_PROTOCOL_TIMEOUT                                      0x05040000 /**< \brief SDO timeout*/
#define     ABORT_COMMAND_SPECIFIER_UNKNOWN                                 0x05040001 /**< \brief Command specifier unknown*/
#define     ABORT_OUT_OF_MEMORY                                             0x05040005 /**< \brief Out of memory*/
#define     ABORT_UNSUPPORTED_ACCESS                                        0x06010000 /**< \brief Unsupported Access*/
#define     ABORT_WRITE_ONLY_ENTRY                                          0x06010001 /**< \brief Write only entry*/
#define     ABORT_READ_ONLY_ENTRY                                           0x06010002 /**< \brief Read only entry*/
#define     ABORT_ENTRY_CANT_BE_WRITTEN_SI0_NOT_0                           0x06010003 /**< \brief Entry can not be written because Subindex0 is not 0*/
#define     ABORT_COMPLETE_ACCESS_NOT_SUPPORTED                             0x06010004 /**< \brief The object can not be accessed via complete access*/
#define     ABORT_OBJECT_NOT_EXISTING                                       0x06020000 /**< \brief Object not existing*/
#define     ABORT_OBJECT_CANT_BE_PDOMAPPED                                  0x06040041 /**< \brief Object can not be mapped to PDO*/
#define     ABORT_MAPPED_OBJECTS_EXCEED_PDO                                 0x06040042 /**< \brief Mapped Object exceeds PDO*/
#define     ABORT_PARAM_IS_INCOMPATIBLE                                     0x06040043 /**< \brief Parameter is incompatible*/
#define     ABORT_INTERNAL_DEVICE_INCOMPATIBILITY                           0x06040047 /**< \brief Device incompatibility*/
#define     ABORT_HARDWARE_ERROR                                            0x06060000 /**< \brief Hardware error*/
#define     ABORT_PARAM_LENGTH_ERROR                                        0x06070010 /**< \brief Parameter length error*/
#define     ABORT_PARAM_LENGTH_TOO_LONG                                     0x06070012 /**< \brief Parameter is too long*/
#define     ABORT_PARAM_LENGTH_TOO_SHORT                                    0x06070013 /**< \brief Parameter is too short*/
#define     ABORT_SUBINDEX_NOT_EXISTING                                     0x06090011 /**< \brief Subindex (Entry) not exists*/
#define     ABORT_VALUE_EXCEEDED                                            0x06090030 /**< \brief Value exceeds*/
#define     ABORT_VALUE_TOO_GREAT                                           0x06090031 /**< \brief Value is too great*/
#define     ABORT_VALUE_TOO_SMALL                                           0x06090032 /**< \brief Value is too small*/
#define     ABORT_MODULE_ID_LIST_NOT_MATCH                                  0x06090033 /**< \brief Detected Module Ident List (0xF030) and Configured Module Ident list (0xF050) does not match*/
#define     ABORT_MAX_VALUE_IS_LESS_THAN_MIN_VALUE                          0x06090036 /**< \brief Value is less than minimum value*/
#define     ABORT_GENERAL_ERROR                                             0x08000000 /**< \brief General error*/
#define     ABORT_DATA_CANNOT_BE_READ_OR_STORED                             0x08000020 /**< \brief Data can not be read or written*/
#define     ABORT_DATA_CANNOT_BE_READ_OR_STORED_BECAUSE_OF_LOCAL_CONTROL    0x08000021 /**< \brief Data can not be accessed because of local control*/
#define     ABORT_DATA_CANNOT_BE_READ_OR_STORED_IN_THIS_STATE               0x08000022 /**< \brief Data can not be read or written in the current state*/
#define     ABORT_NO_OBJECT_DICTIONARY_IS_PRESENT                           0x08000023 /**< \brief Object is not in the object dictionary*/


/**
 * \addtogroup SDOAbortIndex SDO Abort Codes Identifier
 * @{
 * Internal the SDO abort codes are handled in an array.
 */
#define     ABORTIDX_TOGGLE_BIT_NOT_CHANGED                                 0x01 /**< \brief Index of "Toggle bit not changed"*/
#define     ABORTIDX_SDO_PROTOCOL_TIMEOUT                                   0x02 /**< \brief Index of "SDO timeout"*/
#define     ABORTIDX_COMMAND_SPECIFIER_UNKNOWN                              0x03 /**< \brief Index of "Command specifier unknown"*/
#define     ABORTIDX_OUT_OF_MEMORY                                          0x04 /**< \brief Index of "Out of memory"*/
#define     ABORTIDX_UNSUPPORTED_ACCESS                                     0x05 /**< \brief Index of "Unsupported Access"*/
#define     ABORTIDX_WRITE_ONLY_ENTRY                                       0x06 /**< \brief Index of "Write only entry"*/
#define     ABORTIDX_READ_ONLY_ENTRY                                        0x07 /**< \brief Index of "Read only entry"*/
#define     ABORTIDX_OBJECT_NOT_EXISTING                                    0x08 /**< \brief Index of "Object not existing"*/
#define     ABORTIDX_OBJECT_CANT_BE_PDOMAPPED                               0x09 /**< \brief Index of "Object can not be mapped to PDO"*/
#define     ABORTIDX_MAPPED_OBJECTS_EXCEED_PDO                              0x0A /**< \brief Index of "Mapped Object exceeds PDO"*/
#define     ABORTIDX_PARAM_IS_INCOMPATIBLE                                  0x0B /**< \brief Index of "Parameter is incompatible"*/
#define     ABORTIDX_INTERNAL_DEVICE_INCOMPATIBILITY                        0x0C /**< \brief Index of "Device incompatibility"*/
#define     ABORTIDX_HARDWARE_ERROR                                         0x0D /**< \brief Index of "Hardware error"*/
#define     ABORTIDX_PARAM_LENGTH_ERROR                                     0x0E /**< \brief Index of "Parameter length error"*/
#define     ABORTIDX_PARAM_LENGTH_TOO_LONG                                  0x0F /**< \brief Index of "Parameter is too long"*/
#define     ABORTIDX_PARAM_LENGTH_TOO_SHORT                                 0x10 /**< \brief Index of "Parameter is too short"*/
#define     ABORTIDX_SUBINDEX_NOT_EXISTING                                  0x11 /**< \brief Index of "Subindex (Entry) not exists"*/
#define     ABORTIDX_VALUE_EXCEEDED                                         0x12 /**< \brief Index of "Value exceeds"*/
#define     ABORTIDX_VALUE_TOO_GREAT                                        0x13 /**< \brief Index of "Value is too great"*/
#define     ABORTIDX_VALUE_TOO_SMALL                                        0x14 /**< \brief Index of "Value is too small"*/
#define     ABORTIDX_MODULE_ID_LIST_NOT_MATCH                               0x15 /**< \brief  Index of "Unequal Module Id list"*/
#define     ABORTIDX_MAX_VALUE_IS_LESS_THAN_MIN_VALUE                       0x16 /**< \brief Index of "Value is less than minimum value"*/
#define     ABORTIDX_GENERAL_ERROR                                          0x17 /**< \brief Index of "General error"*/
#define     ABORTIDX_DATA_CANNOT_BE_READ_OR_STORED                          0x18 /**< \brief Index of "Data can not be read or written"*/
#define     ABORTIDX_DATA_CANNOT_BE_ACCESSED_BECAUSE_OF_LOCAL_CONTROL       0x19 /**< \brief Index of "Data can not be accessed because of local control"*/
#define     ABORTIDX_IN_THIS_STATE_DATA_CANNOT_BE_READ_OR_STORED            0x1A /**< \brief Index of "Data can not be read or written in the current state"*/
#define     ABORTIDX_NO_OBJECT_DICTIONARY_IS_PRESENT                        0x1B /**< \brief Index of "Object is not in the object dictionary"*/
#define     ABORTIDX_ENTRY_CANT_BE_WRITTEN_SI0_NOT_0                        0x1C /**< \brief Index of "Entry can not be written because Subindex0 is not 0"*/
#define     ABORTIDX_COMPLETE_ACCESS_NOT_SUPPORTED                          0x1D /**< \brief The object can not be accessed via complete access*/
#define     ABORTIDX_WORKING                                                0xFF /**< \brief Index of application is handling the SDO request*/
/** @}*/
/** @}*/


/**
 *\addtogroup SdoServices SDO Services
 * @{
 */
#define    SDOINFOSERVICE_OBJDICTIONARYLIST_Q           0x01 /**< \brief SDO Info Get object list request*/
#define    SDOINFOSERVICE_OBJDICTIONARYLIST_S           0x02 /**< \brief SDO Info Get object list response*/
#define    SDOINFOSERVICE_OBJDESCRIPTION_Q              0x03 /**< \brief SDO Info Get object description request*/
#define    SDOINFOSERVICE_OBJDESCRIPTION_S              0x04 /**< \brief SDO Info Get object description response*/
#define    SDOINFOSERVICE_ENTRYDESCRIPTION_Q            0x05 /**< \brief SDO Info Get entry description request*/
#define    SDOINFOSERVICE_ENTRYDESCRIPTION_S            0x06 /**< \brief SDO Info Get entry description request*/
#define    SDOINFOSERVICE_ERROR_Q                       0x07 /**< \brief SDO Info Error*/
#define    SDOINFOSERVICE_INCOMPLETE                    0x80 /**< \brief SDO Info Incomplete flag*/
/** @}*/


/**
 *\addtogroup SdoHeaderData SDO Header and Data
 * @{
 */
/**
 * SDO Info Object list
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16                ListType; /**< \brief List type variable*/
        #define    INFO_LIST_TYPE_LENGTH    0 /**< \brief Length of all list types*/
        #define    INFO_LIST_TYPE_ALL       1 /**< \brief All objects*/
        #define    INFO_LIST_TYPE_RXPDO     2 /**< \brief Only RxPDO mappable objects*/
        #define    INFO_LIST_TYPE_TXPDO     3 /**< \brief Only TxPDO mappable objects*/
        #define    INFO_LIST_TYPE_BACKUP    4 /**< \brief Only backup objects*/
        #define    INFO_LIST_TYPE_SET       5 /**< \brief Only setting objects*/
        #define    INFO_LIST_TYPE_MAX       5 /**< \brief Max list identifier*/
}MBX_STRUCT_PACKED_END
TSDOINFOLIST;


/**
 * Object description
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16            DataType; /**< \brief Object data type*/
    UINT16            ObjFlags; /**< \brief Object flags (including object code and max subindex)*/

    /*Object Code and MaxSubindex will be set manually in the object dictionary. The value is always created in little endian format*/
    #define    OBJFLAGS_MAXSUBINDEXMASK    0x00FF /**< \brief Max subindex mask*/
    #define    OBJFLAGS_MAXSUBINDEXSHIFT   0 /**< \brief Max subindex shift*/
    #define    OBJFLAGS_OBJCODEMASK        0x0F00 /**< \brief Mask Object code mask*/
    #define    OBJFLAGS_OBJCODESHIFT       8 /**< \brief Object code shift*/

    #define    OBJCODE_VAR                 0x07 /**< \brief Object code VARIABLE*/
    #define    OBJCODE_ARR                 0x08 /**< \brief Object code ARRAY*/
    #define    OBJCODE_REC                 0x09 /**< \brief Object code RECORD*/

//        char                Name[];                // rest of mailbox data
}MBX_STRUCT_PACKED_END
TSDOINFOOBJDESC;


#define SDO_INFO_OBJ_DESC_SIZE      SIZEOF(TSDOINFOOBJDESC) /**< \brief Object description size*/


/**
 * SDO Info object description service data
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16            Index; /**< \brief Object index*/
    TSDOINFOOBJDESC   Res; /**< \brief Description data*/
}MBX_STRUCT_PACKED_END
TSDOINFOOBJ;


#define SDO_INFO_OBJ_DESC_RES_SIZE  SIZEOF(TSDOINFOOBJ)/**< \brief Size of the object description service data*/


/**
 * SDO Info entry description
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16                DataType; /**< \brief Entry data type*/
    UINT16                BitLength; /**< \brief Entry bit size*/
    UINT16                ObjAccess; /**< \brief Entry access rights*/
                                     /**<
                                     Bit 0: Read Access in Pre-Op
                                     <br>Bit 1: Read Access in Safe-Op
                                     <br>Bit 2: Read Access in Op
                                     <br>Bit 3: Write Access in Pre-Op
                                     <br>Bit 4: Write Access in Safe-Op
                                     <br>Bit 5: Write Access in Op
                                     <br>Bit 6: mappable in RxPDO
                                     <br>Bit 7: mappable in TxPDO
                                     <br>Bit 8: entry will be included in backup
                                     <br>Bit 9: entry will be included in settings*/

        #define    ACCESS_READWRITE             0x003F /**< \brief Read/write in all states*/
        #define    ACCESS_READ                  0x0007 /**< \brief Read only in all states*/
        #define    ACCESS_READ_PREOP            0x0001 /**< \brief Read only in PreOP*/
        #define    ACCESS_READ_SAFEOP           0x0002 /**< \brief Read only in SafeOP*/
        #define    ACCESS_READ_OP               0x0004 /**< \brief Read only in OP*/
        #define    ACCESS_WRITE                 0x0038 /**< \brief Write only in all states*/
        #define    ACCESS_WRITE_PREOP           0x0008 /**< \brief Write only in PreOP*/
        #define    ACCESS_WRITE_SAFEOP          0x0010 /**< \brief Write only in SafeOP*/
        #define    ACCESS_WRITE_OP              0x0020 /**< \brief Write only in OP*/
        #define    OBJACCESS_NOPDOMAPPING       0x0000 /**< \brief Not PDO mappable*/
        #define    OBJACCESS_RXPDOMAPPING       0x0040 /**< \brief Mappable in RxPDOs*/
        #define    OBJACCESS_TXPDOMAPPING       0x0080 /**< \brief Mappable in TxPDOs*/
        #define    OBJACCESS_BACKUP             0x0100 /**< \brief Backup entry*/
        #define    OBJACCESS_SETTINGS           0x0200 /**< \brief Setting Entry*/
        #define    OBJACCESS_SAFEINPUTS         0x0400 /**< \brief Safe input*/
        #define    OBJACCESS_SAFEOUTPUTS        0x0800 /**< \brief Safe output*/
        #define    OBJACCESS_SAFEPARAMETER      0x1000 /**< \brief Safe parameter*/
}MBX_STRUCT_PACKED_END
TSDOINFOENTRYDESC;


/**
 * SDO Info entry description service data
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16                Index; /**< \brief Index of the parent object*/
    UINT16                 Info; /**< \brief Info data including the subindex and identifier for the payload data*/
        #define     ENTRY_MASK_SUBINDEX     0x00FF /**< \brief Subindex mask*/
        #define     ENTRY_SUBINDEX_SHIFT    0 /**< \brief Subindex shift*/
        #define     ENTRY_MASK_VALUEINFO    0xFF00 /**< \brief Value info mask*/
        #define     ENTRY_VALUEINFO_SHIFT   8 /**< \brief Value info shift*/

    TSDOINFOENTRYDESC    Res; /**< \brief Entry description data*/
}MBX_STRUCT_PACKED_END
TSDOINFOENTRY;


/**
 * SDO info Error
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT32                ErrorCode; /**< \brief SDO Info error code*/
}MBX_STRUCT_PACKED_END
TSDOINFOERROR;


#define SDO_INFO_ERROR_SIZE     SIZEOF(TSDOINFOERROR)/**< \brief SDO Info error size*/


/**
 *SDO Info header
 */
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16 InfoHead; /**< \brief Opcode, Incomplete flag and 8 reserved bits*/
        #define     INFOHEAD_OPCODE_MASK            0x007F /**< \brief Opcode mask*/
        #define     INFOHEAD_OPCODE_SHIFT           0 /**< \brief Opcode shift*/
        #define     INFOHEADER_INCOMPLETE_MASK      0x0080 /**< \brief Incomplete mask*/
        #define     INFOHEADER_INCOMPLETE_SHIFT     0 /**< \brief Incomplete shift, the incomplete value "SDOINFOSERVICE_INCOMPLETE" is defined as a UINT8 => no shift required*/

    UINT16                FragmentsLeft; /**< \brief Number of fragments which will follow*/

    union MBX_STRUCT_PACKED_START
    {
        TSDOINFOLIST    List;/**< \brief List data*/
        TSDOINFOOBJ     Obj;/**< \brief Object Description data*/
        TSDOINFOENTRY   Entry;/**< \brief Entry description data*/
        TSDOINFOERROR   Error;/**< \brief Error data*/
        UINT16          Data[1];/**< \brief Unspecified data*/
    }MBX_STRUCT_PACKED_END
    Data;/**< \brief Service data*/

}MBX_STRUCT_PACKED_END
TSDOINFOHEADER;


/**
 * SDO Info complete datagram
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER        MbxHeader; /**< \brief Mailbox header*/
  TCOEHEADER        CoeHeader; /**< \brief CoE header*/
  TSDOINFOHEADER    SdoHeader; /**< \brief SDO Info header and data*/
}MBX_STRUCT_PACKED_END
TSDOINFORMATION;


#define     SIZEOF_SDOINFOHEAD                  4 /**< \brief SDO Info header size*/
#define     SIZEOF_SDOINFO                      ( (COE_HEADER_SIZE) + (SIZEOF_SDOINFOHEAD )) /**< \brief SDO Info and CoE Size*/
#define     SIZEOF_SDOINFOSTRUCT                ( (MBX_HEADER_SIZE) + (COE_HEADER_SIZE) + (SIZEOF_SDOINFOHEAD) ) /**< \brief Complete SDO Info datagram size*/
#define     SIZEOF_SDOINFOLISTHEAD              2 /**< \brief SDO Info list header size*/
#define     SIZEOF_SDOINFOLISTSTRUCT            (( COE_HEADER_SIZE) + (SIZEOF_SDOINFOHEAD) + (SIZEOF_SDOINFOLISTHEAD) ) /**< \brief Complete SDO Info list size*/
#define     SIZEOF_SDOINFOOBJSTRUCT             ( (COE_HEADER_SIZE) + (SIZEOF_SDOINFOHEAD) + (SDO_INFO_OBJ_DESC_RES_SIZE) ) /**< \brief Complete SDO Info object description response size*/
#define     SIZEOF_SDOINFOENTRYREQHEAD          4 /**< \brief SDO Info entry description request size*/
#define     SIZEOF_SDOINFOENTRYREQSTRUCT        ( (COE_HEADER_SIZE) + (SIZEOF_SDOINFOHEAD) + (SIZEOF_SDOINFOENTRYREQHEAD) ) /**< \brief SDO Info entry description request size*/
#define     SIZEOF_SDOINFOERRORSTRUCT           ( (COE_HEADER_SIZE) + (SIZEOF_SDOINFOHEAD) + (SDO_INFO_ERROR_SIZE) ) /**< \brief SDO Info error size*/


#define SDO_INFO_HEADER_BYTE_SIZE         ((SIZEOF_SDOINFOSTRUCT)+(SIZEOF_SDOINFOLISTHEAD)) /**< \brief SDO Info size*/
/** @}*/


/**
 * \addtogroup SdoPendingHandler SDO Pending Response
 * @{
 * If this feature is enabled (SDO_RES_INTERFACE 1) a SDO request may set to pending and is completed afterwards.
 * This can be used to forward the read/write request an additional thread, uController.
 */
#define     SDO_PENDING_WRITE       0x1 /**< \brief SDO write request is pending*/
#define     SDO_PENDING_SEG_WRITE   0x2 /**< \brief SDO segmented write request is pending*/
#define     SDO_PENDING_READ        0x3 /**< \brief SDO read request is pending*/
#define     SDO_PENDING_SEG_READ    0x4 /**< \brief SDO segmented read request is pending*/
/** @}*/


#endif //_SDOSRV_H_

/*-----------------------------------------------------------------------------------------
------
------    Global Variables
------
-----------------------------------------------------------------------------------------*/
#if defined(_SDOSERV_) && (_SDOSERV_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif

/**
 * \addtogroup SdoPendingHandler SDO Pending Response
 *@{
 */
PROTO UINT8         u8PendingSdo; /**< \brief Identifier for the pending SDO service (see SDO_PENDING_XXX defines)*/
PROTO BOOL          bStoreCompleteAccess; /**< \brief Indicates if the pending SDO service is an complete access*/
PROTO UINT8         u8StoreSubindex; /**< \brief Contains the subindex for the pending SDO request*/
PROTO UINT16        u16StoreIndex; /**< \brief Contains the Index for the pending SDO request*/
PROTO UINT32        u32StoreDataSize; /**< \brief Complete data size to be handled*/
PROTO UINT16 MBXMEM *pStoreData; /**< \brief Pointer to the buffer to be handled*/
PROTO UINT8 (* pSdoPendFunc)( UINT16 Index, UINT8 Subindex, UINT32 Size, UINT16 MBXMEM * pData, UINT8 bCompleteAccess ); /**< \brief Function pointer for the SDO pending request handler*/
/** @}*/


PROTO MEM_ADDR      VARMEM                aSdoInfoHeader[GET_MEM_SIZE(SDO_INFO_HEADER_BYTE_SIZE)]; /**< \brief SDO Info buffer*/


/**
 * \addtogroup SegmentedSdo Segmented SDO
 * @{
 */
PROTO UINT16 VARMEM * VARMEM                pSdoSegData; /**< \brief Pointer to the buffer handling to full object/entry data*/
PROTO UINT16          VARMEM                nSdoInfoFragmentsLeft; /**< \brief Number of fragments which need to be transmitted*/
/** @}*/


/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/
PROTO    UINT8 SDOS_SdoInfoInd(TSDOINFORMATION MBXMEM *pSdoInfoInd);
PROTO    UINT8 SDOS_SdoInd(TINITSDOMBX MBXMEM *pSdoInd);

PROTO    void  SDOS_SdoRes(UINT8 abort, UINT32 objLength, UINT16 MBXMEM *pData);
PROTO    void  SDOS_ClearPendingResponse(void);


#undef PROTO
/** @}*/
