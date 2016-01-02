/** \file system.h
* \brief This file contains system clock and system functions.
*
* \author Lukasz Radola
* \date 22.05.15
*
* \details In this file are definitions, declarations of functions, which can
* set system clock et cetera...
*/
#ifndef LRADOLA_SYSTEM_SYSTEM_H_
#define LRADOLA_SYSTEM_SYSTEM_H_

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
#include "gpio.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition */

#define SYSTEM__4MHZ_CLOCK  (uint32_t) 4000000
#define SYSTEM__16MHZ_CLOCK  (uint32_t) 16000000
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


void SystemClock_Config(void);

#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_SYSTEM_SYSTEM_H_ */
