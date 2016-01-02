/** \file state_machine.h
* \brief This file contains main engine function of production state.
*
* \author Lukasz Radola
* \date 20.08.15
*
* \details In this file are definitions, declarations of engine functions.
*/
#ifndef LRADOLA_STATE_MACHINE_H_
#define LRADOLA_STATE_MACHINE_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*

void STATEMACHINE__PollFunction(void);

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


enum currentState_e
{
  CURRENT_STATE_NOTHING = 0,
  CURRENT_STATE_CALIBRATION_RTC,
  CURRENT_STATE_MEASURE_TEMPERATURE,
  CURRENT_STATE_MEASURE_VBAT,
  CURRENT_STATE_SLEEP,
  CURRENT_STATE_EE_WRITE,
  CURRENT_STATE_ERROR
};


/* Typedef definition */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration - use it isn't recommended*/

/*! Global Enum variable which contain names each soft production engine states */
extern volatile enum currentState_e currentSTATE;

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
* \brief State machine polling function.
*
* This function is engine of production software. Here are setting STM 
* operations for example temperature measure, LED activation etc.
*
*/

void STATEMACHINE__PollFunction(void);

#ifdef __cplusplus
  }
#endif
#endif /* FIBARO_STATE_MACHINE_H_ */
