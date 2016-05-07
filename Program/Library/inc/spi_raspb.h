/** \file spi_raspb.h
* \brief This file contains spi function which transmit date to raspberry pi.
*
* \author Lukasz Radola
* \date 04.05.16
*
* \details In this file are definitions, declarations of spi functions.
*/
#ifndef LRADOLA_SPI_RASPB_H_
#define LRADOLA_SPI_RASPB_H_

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


#define SPI_RASPB__SPI_FCPU       MAIN__F_CPU /* UART CPU Value */
#define SPI_RASPB__BAUDRATE_19200    19200
#define SPI_RASPB__BAUDRATE_9600     9600


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

// extern volatile const uint8_t stringtosend[4];
 extern volatile uint8_t spi_raspb__stringtoreceive[1];

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


void SPI_RASPB__Init(uint32_t baudrate);
void SPI_Raspb__SetDefaultMessToSend(void);
void SPI_RASPB__Poll(void);
void SPI_RASPB__TxInterrupt(void);
void SPI_RASPB__RxInterrupt(void);
void SPI_RASPB__StartDmaTransmision(int8_t* data, int8_t* additional_text,
                               uint8_t length, int8_t* message);


#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_SPI_RASPB_H_ */
