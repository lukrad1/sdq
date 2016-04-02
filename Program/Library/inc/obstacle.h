/** \file obstacle.h
* \brief This file contains in general obstacle functions configuration.
*
* \author Lukasz Radola
* \date 02.04.16
*
* \details In this file are definitions, declarations of functions, which can
* detect obstacle et cetera...
*/
#ifndef LRADOLA_OBSTACLE_OBSTACLE_H_
#define LRADOLA_OBSTACLE_OBSTACLE_H_

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



/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition */



/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */
/* for example: extern unsigned int module_variable_1; */

/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions (in *.c file) 2. Local exported */
/* functions (in this or in second *.h file) 3. Interface (exported) functions) */

/* Local functions in *.c file */

/* Exported functions */
  void OBSTACLE__StartIdentificationTimer(void);
  void OBSTACLE__StopIdentificationTimer(void);
  void OBSTACLE__EnkoderInterrupt(void);
  void OBSTACLE__SetSharpId(uint8_t type_of_sharp);
  void OBSTACLE__1msPoll(void);

#ifdef __cplusplus
  }
#endif

#endif /* LRADOLA_OBSTACLE_OBSTACLE_H_ */
