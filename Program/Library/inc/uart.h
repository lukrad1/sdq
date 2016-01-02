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
void UART__Init(uint32_t baudrate);	
void UART__Start(void);														
void UART__Stop(void);														
void UART__Poll(void); 	
void UART__RxInterrupt(uint8_t data);							
void UART__TxInterrupt(void);
void UART__Tick(void);
bool UART__CheckReceive(void);
bool UART__CheckSendAck(void);
bool UART__CheckSendData(void);
void UART__SendACK(uint8_t type);
void UART__SendNACK(uint8_t type);
void UART__SendERROR(uint8_t error);
void UART__SendBUSY(void);
void UART__SendIDLE(void);
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
#define UART__ID_GET					            0x00
#define UART__ID_REPORT										0xFF

/*==============================================================*/

#define UART__UART_FCPU 			MAIN__F_CPU /* UART CPU Value */ 				
#define UART__BAUDRATE_115200		115200
#define UART__BAUDRATE_9600 		9600
#define UART__STARTBYTE				0xAA
#define UART__STOPBYTE				0x55
#define UART__TXBUFFER 				270
#define UART__RXBUFFER 				270


#define UART__RETRANMISIONVALUE 50
/* Minimum time value in ms between uart transmision */
#define UART__MINBEETWENTRANMISIONVALUE 30	
/* Maximum recive data time in ms */
#define UART__RXTIMEOUTVALUE 	30

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */


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
void UART__RxInterrupt(uint8_t data);							
void UART__TxInterrupt(void);
// Pozostale funkcje
void UART__Tick(void);																							
uint8_t UART__CheckReceive(void);
uint8_t UART__CheckSendData(void);
void UART__SendACK(uint8_t type);
void UART__SendNACK(uint8_t type);
void UART__SendERROR(uint8_t error);
void UART__SendBUSY(void);
void UART__SendIDLE(void);
#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_UART_UART_H_ */
