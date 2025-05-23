/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup EcatAppl EtherCAT application
@{
*/

/**
\file applInterface.h
\author EthercatSSC@beckhoff.com
\brief Definition of the application interface functions

\version 5.13

<br>Changes to version V5.12:<br>
V5.13 COE4: update default entry name handling in case of 16Bit characters, add CoE Read/write indication functions<br>
<br>Changes to version V5.11:<br>
V5.12 APPL1: add optional application function called from the main loop (after mbx and esm are executed)<br>
V5.12 EEPROM3: implement a store EEPROM timeout handler<br>
V5.12 EOE1: move icmp sample to the sampleappl,add EoE application interface functions<br>
V5.12 FOE1: update new interface,move the FoE sample to sampleappl,add FoE application callback functions<br>
<br>Changes to version - :<br>
V5.10.1 : Start file change log
*/

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/


#ifndef _APPL_INTERFACE_H_
#define _APPL_INTERFACE_H_

#include "ecat_def.h"
#include <stdlib.h>
#include <string.h>
#include "esc.h"
#include "ecatslv.h"
#include "objdef.h"
#include "ecatappl.h"





#endif /*#ifndef _APPL_INTERFACE_H_*/

#if defined(_APPL_INTERFACE_) && (_APPL_INTERFACE_ == 1)
   #define PROTO
#else
    #define PROTO extern
#endif

/*-----------------------------------------------------------------------------------------
------
------    Function pointer which are used to call optional functions from the application
------
-----------------------------------------------------------------------------------------*/






/* ECATCHANGE_START(V5.13) COE4*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
\param     Index            Index of the object to be read
\param     Subindex         Subindex of the object entry to be read
\param     CompleteAccess   Indicates if the object is read with complete access

\brief    The function is called before an object/entry is read
*////////////////////////////////////////////////////////////////////////////////////////
PROTO void(*pAPPL_CoeReadInd)(UINT16 Index, UINT8 Subindex, BOOL CompleteAccess);

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param     Index            Index of the object which was written
\param     Subindex         Subindex of the object entry which was written
\param     CompleteAccess   Indicates if the object was written with complete access

\brief    The function is called after an object/entry was successfully written
*////////////////////////////////////////////////////////////////////////////////////////
PROTO void(*pAPPL_CoeWriteInd)(UINT16 Index, UINT8 Subindex, BOOL CompleteAccess);

/* ECATCHANGE_END(V5.13) COE4*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function is called by the SSC from the main loop
\brief	   The function pointer is reset in MainInit() so it shall be set afterwards
*////////////////////////////////////////////////////////////////////////////////////////
PROTO void(*pAPPL_MainLoop)();

/*-----------------------------------------------------------------------------------------
------
------    Functions provided by the SSC to be called from the application
------
-----------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
\return   0 if successful or greater 0 in case of an error

\brief    Initialize the generic slave stack.
\brief    This function should be called after the platform including operating system and
\brief    ESC is ready to use.
*////////////////////////////////////////////////////////////////////////////////////////
PROTO UINT16 MainInit(void);


/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function handles the low priority function like EtherCAT state machine
\brief    handling, mailbox protocols and if no synchronization is enabled also the
\brief    application.
\brief    This function shall be called cyclically from the application.
*////////////////////////////////////////////////////////////////////////////////////////
PROTO void MainLoop(void);


/////////////////////////////////////////////////////////////////////////////////////////
/**
\param    alStatus       requested AL Status 
\param    alStatusCode   requested AL Status Code

\brief    This function shall be called by the application to trigger state transition in case
\brief    of an application error or to complete a pending transition.
\brief    If the function was called due to an error it shall be again if the error is gone.
\brief    NOTE: state requests to a higher state than the current state are not allowed.
*////////////////////////////////////////////////////////////////////////////////////////
PROTO void ECAT_StateChange(UINT8 alStatus, UINT16 alStatusCode);



#undef PROTO
