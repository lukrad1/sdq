/** \file uart_esp.h
* \brief This file contains uart function.
*
* \author Lukasz Radola
* \date 28.08.15
*
* \details In this file are definitions, declarations of uart functions.
*/
#ifndef LRADOLA_UART_ESP_H_
#define LRADOLA_UART_ESP_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*

*/
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* ... */
/* other (non local) library */
/* ... */
/* local library */
#include "main.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition */

/* Typedef definition */

/*========================= UART_TYPE DEFINITION ==================*/


#define UART_ESP__UART_FCPU 			MAIN__F_CPU /* UART CPU Value */ 				
#define UART_ESP__BAUDRATE_19200		19200
#define UART_ESP__BAUDRATE_9600 		9600


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

// extern volatile const uint8_t stringtosend[4];
 extern volatile uint8_t stringtoreceive_esp[1];

/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions (in *.c file) 2. Local exported */ 
/* functions (in this or in second *.h file) 3. Interface (exported) functions) */

/* Local functions in *.c file */

/* Exported functions */

// Funkcje inicjalizacji UART

/**
* @brief  UART init function.
*
* This function sets all uart option for example: baudrate, stop bit et cetera.
* @param baudrate - uart transmision speed for example 9600, 115200 kbps.
*/
void UART_ESP__Init(uint32_t baudrate);	
void UART_ESP__DeInit(void);
void UART_ESP__Start(void);														
void UART_ESP__Stop(void);														
// Funkcje odbioru i wysylania danych
void UART_ESP__Poll(void); 	
void UART_ESP__RxInterrupt();
void UART_ESP__TxInterrupt(void);
// Pozostale funkcje
void UART_ESP__DMAConfig(void);
void UART_ESP__StartDmaTransmision(int8_t* data, int8_t* additional_text,
                               uint8_t length);
void UART_ESP__SetIntTempToSend(void);
void UART_ESP__SendAllData(void);

void UART_ESP__SetIsWakeupFlag(void);
void UART_ESP__ClearIsWakeupFlag(void);
uint8_t UART_ESP__GetIsWakeupFlag(void);
void UART_ESP__SetFinishTransmisionFlag(void);
void UART_ESP__ClearFinishTransmisionFlag(void);
uint8_t UART_ESP__GetFinishTransmisionFlag(void);
void UART_ESP__SetStartInit(void);
void UART_ESP__ClearStartInit(void);
uint8_t UART_ESP__GetStartInit(void);
void UART_ESP__SetTimeout(void);
void UART_ESP__ClearTimeout(void);
uint8_t UART_ESP__GetTimeout(void);
void UART_ESP__ClearRaspberry_Run_Flag(void);
uint8_t UART_ESP__GetRaspberry_Run_Flag(void);
#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_UART_UART_H_ */
