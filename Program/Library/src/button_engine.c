/** \file button_engine.c
* \brief This file contains buttons engine and all the most important buttons 
* function..
*
* \author Lukasz Radola
* \date 26.05.15
* 
*/
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* ... */
/* other (non local) library */
/* ... */
/* local library */

#include "main.h"
#include "gpio.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */


/* Typedef definition (local) */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

volatile uint8_t BUTTON__Exti_flag = 0;

/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */



void BUTTON__Init(void)
{
  GPIO__ConfigButton(1);
}

/******************************* END FUNCTION *********************************/

void BUTTON__SetExtiButtonFlag(void)
{
  BUTTON__Exti_flag = 1;
}

void BUTTON__ClearExtiButtonFlag(void)
{
  BUTTON__Exti_flag = 0;
}
uint8_t BUTTON__GetExtiButtonFlag(void)
{
  return BUTTON__Exti_flag;
}
#ifdef __cplusplus
  }
#endif
