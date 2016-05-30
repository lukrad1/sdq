/** \file uart_raspb.h
* \brief This file contains uart function for raspberry pi.
*
* \author Lukasz Radola
* \date 08.05.16
*
* \details In this file are definitions, declarations of uart functions.
*/
#ifndef LRADOLA_UART_RASPB_H_
#define LRADOLA_UART_RASPB_H_

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


#define UART_RASPB__UART_FCPU       MAIN__F_CPU /* UART CPU Value */
#define UART_RASPB__BAUDRATE_19200    19200
#define UART_RASPB__BAUDRATE_9600     9600


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

// extern volatile const uint8_t stringtosend[4];
 extern volatile uint8_t raspb_stringtoreceive[1];

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
void UART_RASPB__Init(uint32_t baudrate);
void UART_RASPB__DeInit(void);
void UART_RASPB__Start(void);
void UART_RASPB__Stop(void);
// Funkcje odbioru i wysylania danych
void UART_RASPB__Poll(void);
void UART_RASPB__RxInterrupt();
void UART_RASPB__TxInterrupt(void);
// Pozostale funkcje
void UART_RASPB__DMAConfig(void);
void UART_RASPB__StartDmaTransmision(int8_t* data, int8_t* additional_text,
                               uint8_t length, int8_t* message);
void UART_RASPB__SetIntTempToSend(void);
void UART_RASPB__SetSharp1ToSend(void);
void UART_RASPB__SetVrefToSend(void);
void UART_RASPB__SetSharpLewyPrzodToSend(void);
void UART_RASPB__SetSharpPrawyPrzodToSend(void);
void UART_RASPB__SetSharpSrodekTylToSend(void);

#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_UART_RASPB_H_ */
