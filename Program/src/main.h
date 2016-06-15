/** \file main.h
* \brief This file contains all included library using in project.
*
* \author Lukasz Radola
* \date 22.05.15
*
* \details You should use #include "main.h" in all project files.
*
*/
#ifndef LRADOLA_MAIN_MAIN_H_
#define LRADOLA_MAIN_MAIN_H_

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
//#include "gpio.h"
//#include "timer.h"
//#include "adc.h"
//#include "ee.h"
//#include "device.h"
//#include "button_engine.h"
//#include "led.h"
//#include "uart.h"
//#include "state_machine.h"
//#include "rtc.h"
//#include "stm32l0xx.h"
  
  /****************************************************************************/
  /*                      DECLARATION AND DEFINITIONS                         */
  /****************************************************************************/
  /* Macro and const variable definition */

//#define BLUETOOTH
#define ESP_8266

  /*! System clock frequency */
  #define _MAIN__F_CPU     4000000
  #define MAIN__F_CPU      (uint32_t)_MAIN__F_CPU
  /*! STM software version */
  #define STM_VERSION     0
  #define STM_SUBVERSION  1

  /* Typedef definition */

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




#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_MAIN_MAIN_H_ */
