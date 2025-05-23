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
\file ecatcoe.h
\author EthercatSSC@beckhoff.com

\version 5.11

<br>Changes to version V5.01:<br>
V5.11 COE4: "change prototype of ""COE_ContinueInd()"" return <> 0 if a failure occurred"<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "mailbox.h"

#ifndef _ECATCOE_H_
#define _ECATCOE_H_

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

/*---------------------------------------------
-    Error codes
-----------------------------------------------*/
#define     ERROR_COEINVALIDSERVICE     0x01 /**< \brief Invalid SDO service*/
#define     ERROR_COENOTSUPPORTED       0x02 /**< \brief Not supported service*/


/*---------------------------------------------
-    COE services
-----------------------------------------------*/
#define     COESERVICE_EMERGENCY        0x01 /**< \brief CoE Emergency*/
#define     COESERVICE_SDOREQUEST       0x02 /**< \brief CoE SDO request*/
#define     COESERVICE_SDORESPONSE      0x03 /**< \brief CoE SDO response*/
#define     COESERVICE_TXPDO            0x04 /**< \brief CoE TxPDO*/
#define     COESERVICE_RXPDO            0x05 /**< \brief CoE RxPDO*/
#define     COESERVICE_TXPDOREMREQ      0x06 /**< \brief CoE TxPDO map request*/
#define     COESERVICE_RXPDOREMREQ      0x07 /**< \brief CoE RxPDO map request*/
#define     COESERVICE_SDOINFO          0x08 /**< \brief CoE SDO Info*/



/*---------------------------------------------
-    COE Structures
-----------------------------------------------*/
#define     COEHEADER_COESERVICESHIFT   12 /**< \brief CoE service shift (within CoE header)*/
#define     COEHEADER_COESERVICEMASK    0xF000 /**< \brief CoE service mask (within CoE header)*/

#define     COE_HEADER_SIZE             2 /**< \brief CoE header size*/

typedef UINT16 TCOEHEADER; /**< \brief CoE header*/


/**
 * \brief CoE Mailbox header
 */
typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER        MbxHeader; /**< \brief mailbox header*/
  TCOEHEADER        CoeHeader; /**< \brief CoE header*/
  UINT16            Data[((MAX_MBX_DATA_SIZE)-(COE_HEADER_SIZE)) >> 1]; /**< \brief CoE data*/
}MBX_STRUCT_PACKED_END
TCOEMBX;


#endif //_ECATCOE_H_

#if defined(_ECATCOE_) && (_ECATCOE_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif

/*-----------------------------------------------------------------------------------------
------
------    Global Variables
------
-----------------------------------------------------------------------------------------*/
PROTO    TMBX MBXMEM * VARMEM pCoeSendStored;                /** if the mailbox service could not be sent (or stored),
                                                                the CoE service will be stored in this variable
                                                                and will be sent automatically from the mailbox handler
                                                                (COE_ContinueInd) when the send mailbox will be read
                                                                the next time from the master */

/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/

PROTO   void     COE_Init(void);
PROTO   UINT8    COE_ServiceInd(TCOEMBX MBXMEM *pCoeMbx);
PROTO   UINT8     COE_ContinueInd(TMBX MBXMEM * pMbx);

#undef PROTO
/** @}*/
