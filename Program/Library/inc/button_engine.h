/** \file button_engine.h
* \brief This file contains button engine.
*
* \author Lukasz Radola
* \date 27.08.15
*
* \details In this file are definitions, declarations of button engine functions.  
*/
#ifndef LRADOLA_BUTTON_ENGINE_BUTTON_ENGINE_H_
#define LRADOLA_BUTTON_ENGINE_BUTTON_ENGINE_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*

void BUTTON__Init(void);

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
* \brief Initialization buttons engine function.
*
* This init function clear button array value. 
*
*/
void BUTTON__Init(void);



#ifdef __cplusplus
  }
#endif
#endif /* FIBARO_BUTTON_ENGINE_BUTTON_ENGINE_H_ */
