/** \file device.h
* \brief This file contains special device function.
*
* \author Lukasz Radola
* \date 28.07.15
*
* \details In this file are definitions, declarations of functions, which can  
* set system clock, enable timer interrupt et cetera...
* 
* 
*/
#ifndef LRADOLA_DEVICE_DEVICE_H_
#define LRADOLA_DEVICE_DEVICE_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*
void DEVICE__MENU(void);
void DEVICE__SleepPoll(void);
void DEVICE__ResetSTM(void);

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


/**
* \brief This function is executive menu function.
*
* It's a polling function, which is activated menu option, when the button is
* held.
*
*/
void DEVICE__MENU(void);
/**
* \brief This function turn on DCDC switching converter.
*
* When the device is power on battery, then the dcdc converter has to be enabled
* during RF transmision, LED ON et cetera.
*
*/
void DEVICE__SleepPoll(void);

/**
* \brief This function resets STM to default parameters.
*
*
*/
void DEVICE__ResetSTM(void);


#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_DEVICE_DEVICE_H_ */
