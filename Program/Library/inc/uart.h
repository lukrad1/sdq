/** \file uart.h
* \brief This file contains uart function.
*
* \author Lukasz Radola
* \date 28.08.15
*
* \details In this file are definitions, declarations of uart functions.
*/
#ifndef LRADOLA_UART_UART_H_
#define LRADOLA_UART_UART_H_

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


#define UART__UART_FCPU 			MAIN__F_CPU /* UART CPU Value */ 				
#define UART__BAUDRATE_115200		115200
#define UART__BAUDRATE_9600 		9600


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

// extern volatile const uint8_t stringtosend[4];
 extern volatile uint8_t stringtoreceive[4];

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
void UART__Init(uint32_t baudrate);	

void UART__Start(void);														
void UART__Stop(void);														
// Funkcje odbioru i wysylania danych
void UART__Poll(void); 	
void UART__RxInterrupt();
void UART__TxInterrupt(void);
// Pozostale funkcje
void UART__DMAConfig(void);
void UART__StartDmaTransmision(uint8_t* data, uint8_t length);
void UART__SetIntTempToSend(void);
#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_UART_UART_H_ */
