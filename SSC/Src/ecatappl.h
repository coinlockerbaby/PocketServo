/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
 \addtogroup EcatAppl EtherCAT application
 * @{
 */

/**
\file ecatappl.h
\author EthercatSSC@beckhoff.com

\version 5.12

<br>Changes to version V5.11:<br>
V5.12 EEPROM1: get read size from register 0x502.6<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
<br>Changes to version V5.01:<br>
V5.10 ECAT13: Update Synchronisation handling (FreeRun,SM Sync, Sync0, Sync1)<br>
              Compare DC UINT configuration (by ESC Config data) vs. DC activation register settings<br>
              Update 0x1C3x entries<br>
V5.10 ECAT4: Update alignment macro for 8 to 15 bit alignments (16 and 32 Bit controllers)<br>
             Bugfix calculate LED blink frequency<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */
/*-----------------------------------------------------------------------------------------
------
------    Description
------
------    ecatappl.h
------
------    EtherCAT Slave Application
------                                                                                                                                                                 ------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#include "ecat_def.h"


#ifndef _ECATAPPL_H_
#define _ECATAPPL_H_

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

/*Set to unsigned short to handle bit entries correct*/
#define     BOOLEAN(x)                 UINT16 x:1 /**< \brief Macro to define BOOLEAN object entry*/
#define     BIT1(x)                    UINT16 x:1 /**< \brief Macro to define BIT1 object entry*/
#define     BIT2(x)                    UINT16 x:2 /**< \brief Macro to define BIT2 object entry*/
#define     BIT3(x)                    UINT16 x:3 /**< \brief Macro to define BIT3 object entry*/
#define     BIT4(x)                    UINT16 x:4 /**< \brief Macro to define BIT4 object entry*/
#define     BIT5(x)                    UINT16 x:5 /**< \brief Macro to define BIT5 object entry*/
#define     BIT6(x)                    UINT16 x:6 /**< \brief Macro to define BIT5 object entry*/
#define     BIT7(x)                    UINT16 x:7 /**< \brief Macro to define BIT6 object entry*/
#define     BIT8(x)                    UINT16 x:8 /**< \brief Macro to define BIT7 object entry*/
#define     ALIGN0(x)
#define     ALIGN1(x)                  UINT16 x:1; /**< \brief Macro to define ALIGN1 object entry*/
#define     ALIGN2(x)                  UINT16 x:2; /**< \brief Macro to define ALIGN2 object entry*/
#define     ALIGN3(x)                  UINT16 x:3; /**< \brief Macro to define ALIGN3 object entry*/
#define     ALIGN4(x)                  UINT16 x:4; /**< \brief Macro to define ALIGN4 object entry*/
#define     ALIGN5(x)                  UINT16 x:5; /**< \brief Macro to define ALIGN5 object entry*/
#define     ALIGN6(x)                  UINT16 x:6; /**< \brief Macro to define ALIGN6 object entry*/
#define     ALIGN7(x)                  UINT16 x:7; /**< \brief Macro to define ALIGN7 object entry*/
#define     ALIGN8(x)                  UINT16 x:8; /**< \brief Macro to define ALIGN8 object entry*/
#define     ALIGN9(x)                  UINT16 x:9; /**< \brief Macro to define ALIGN9 object entry*/
#define     ALIGN10(x)                 UINT16 x:10; /**< \brief Macro to define ALIGN10 object entry*/
#define     ALIGN11(x)                 UINT16 x:11; /**< \brief Macro to define ALIGN11 object entry*/
#define     ALIGN12(x)                 UINT16 x:12; /**< \brief Macro to define ALIGN12 object entry*/
#define     ALIGN13(x)                 UINT16 x:13; /**< \brief Macro to define ALIGN13 object entry*/
#define     ALIGN14(x)                 UINT16 x:14; /**< \brief Macro to define ALIGN14 object entry*/
#define     ALIGN15(x)                 UINT16 x:15; /**< \brief Macro to define ALIGN15 object entry*/
#endif //_ECATAPPL_H_

#if defined(_ECATAPPL_) && (_ECATAPPL_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif


/*-----------------------------------------------------------------------------------------
------
------    Global Variables
------
-----------------------------------------------------------------------------------------*/



PROTO BOOL bEcatWaitForInputUpdate; /**< \brief Wait for input update*/
PROTO BOOL bEtherCATRunLed; /**< \brief Current run LED value*/
PROTO BOOL bEtherCATErrorLed; /**< \brief Current error LED value*/
PROTO BOOL bRunApplication; /**< \brief Indicates if the stack shall be running (if false the Hardware will be released)*/



/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/




PROTO    void       ECAT_CheckTimer(void);
PROTO    void       PDI_Isr(void);
PROTO    void       Sync0_Isr(void);
PROTO    void       Sync1_Isr(void);
PROTO    void       ECAT_Application(void);
PROTO    void       PDO_ResetOutputs(void);
PROTO    void       PDO_ReadInputs(void);
PROTO    void       PDO_InputMapping(void);

PROTO    void       ECAT_SetLedIndication(void);
PROTO    void       CalcSMCycleTime(void);
PROTO    UINT16     ESC_EepromAccess(UINT32 wordaddress, UINT16 wordsize, UINT16 MBXMEM *pData, UINT8 access);
PROTO    UINT16     ESC_EepromWriteCRC(void);




#undef PROTO
/** @}*/
