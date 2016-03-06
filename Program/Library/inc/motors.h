/** \file motors.h
* \brief This file contains functions which control motors of my robot.
*
* \author Lukasz Radola
* \date 23.01.16
*
* \details In this file are definitions, declarations of LED functions.
*/
#ifndef LRADOLA_MOTORS_MOTORS_H_
#define LRADOLA_MOTORS_MOTORS_H_

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


#define KOLO_PRZOD_PRAWE_PORT GPIOA
#define KOLO_PRZOD_PRAWE_ZEGAR RCC_IOPENR_GPIOAEN

#define KOLO_PRZOD_PRAWE_AIN1_PIN GPIO_MODER_MODE8
#define KOLO_PRZOD_PRAWE_AIN1_PIN_NUMBER 8
#define KOLO_PRZOD_PRAWE_AIN1_PIN_MODE GPIO_MODER_MODE8_0

#define KOLO_PRZOD_PRAWE_AIN2_PIN_NUMBER 9
#define KOLO_PRZOD_PRAWE_AIN2_PIN GPIO_MODER_MODE9
#define KOLO_PRZOD_PRAWE_AIN2_PIN_MODE GPIO_MODER_MODE9_0

/***************************************************************************/

#define KOLO_PRZOD_LEWE_PORT GPIOA
#define KOLO_PRZOD_LEWE_ZEGAR RCC_IOPENR_GPIOAEN

#define KOLO_PRZOD_LEWE_BIN1_PIN_NUMBER 10
#define KOLO_PRZOD_LEWE_BIN1_PIN_MODE GPIO_MODER_MODE10_0
#define KOLO_PRZOD_LEWE_BIN1_PIN GPIO_MODER_MODE10

#define KOLO_PRZOD_LEWE_BIN2_PIN_NUMBER 15
#define KOLO_PRZOD_LEWE_BIN2_PIN_MODE GPIO_MODER_MODE15_0
#define KOLO_PRZOD_LEWE_BIN2_PIN GPIO_MODER_MODE15

  /***************************************************************************/

#define KOLO_TYL_PRAWE_PORT GPIOB
#define KOLO_TYL_PRAWE_ZEGAR RCC_IOPENR_GPIOBEN

#define KOLO_TYL_PRAWE_AIN1_PIN GPIO_MODER_MODE12
#define KOLO_TYL_PRAWE_AIN1_PIN_NUMBER 12
#define KOLO_TYL_PRAWE_AIN1_PIN_MODE GPIO_MODER_MODE12_0

#define KOLO_TYL_PRAWE_AIN2_PIN_NUMBER 15
#define KOLO_TYL_PRAWE_AIN2_PIN GPIO_MODER_MODE15
#define KOLO_TYL_PRAWE_AIN2_PIN_MODE GPIO_MODER_MODE15_0

  /***************************************************************************/

#define KOLO_TYL_LEWE_PORT GPIOB
#define KOLO_TYL_LEWE_ZEGAR RCC_IOPENR_GPIOBEN

#define KOLO_TYL_LEWE_BIN1_PIN_NUMBER 8
#define KOLO_TYL_LEWE_BIN1_PIN_MODE GPIO_MODER_MODE8_0
#define KOLO_TYL_LEWE_BIN1_PIN GPIO_MODER_MODE8

#define KOLO_TYL_LEWE_BIN2_PIN_NUMBER 9
#define KOLO_TYL_LEWE_BIN2_PIN_MODE GPIO_MODER_MODE9_0
#define KOLO_TYL_LEWE_BIN2_PIN GPIO_MODER_MODE9

  /***************************************************************************/


/* Typedef definition */


/* ... */


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


  void MOTORS__skret_w_lewo(void);
  void MOTORS__skret_w_prawo(void);
  void MOTORS__jazda_do_tylu(void);
  void MOTORS__jazda_do_przodu(void);
  void MOTORS__jazda_zatrzymana(void);

#ifdef __cplusplus
  }
#endif

#endif /* LRADOLA_MOTORS_MOTORS_H_ */

