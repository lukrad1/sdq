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



typedef enum system__sleepready_enum
  {
    SYSTEM__SLEEPREADY_MEAS_ADC = 0,
    SYSTEM__SLEEPREADY_UART,
    SYSTEM__SLEEPREADY_SPI,
    SYSTEM__SLEEPREADY_UART_RASPB,
    SYSTEM__SLEEPREADY_BUTTON,
    SYSTEM__SLEEPREADY_LED_BLINK,
    SYSTEM__SLEEPREADY_ROBOT_RUN,

    // clear all autoclear flags
    SYSTEM__SLEEPREADY_ALL_BEFORE_SLEEP = 0x6F, // other means: it is #define ...
    // clear all flags
    SYSTEM__SLEEPREADY_ALL = 0xFF               // other means: it is #define ...
  } SYSTEM__sleep_e;

//this define masks unused sleep flags
#define SYSTEM__SLEEPFLAG_NUMBER 7
// which flag are used
#define SYSTEM__SLEEPFLAG_MASK (uint8_t)0xFF
// which flag can be autoclear after wakeup
#define SYSTEM__SLEEP_AUTOCLEAR_MASK (uint8_t)0x37

  typedef union system__sleepflags_union
  {
    uint8_t flags;
    struct
    {

      uint8_t measure_adc: 1;
      uint8_t uart: 1;
      uint8_t spi: 1;
      uint8_t uart_raspb:1;
      uint8_t button: 1;
      uint8_t led_blink: 1;
      uint8_t robot_run: 1;
      uint8_t flag7 : 1;

    };
  } SYSTEM__sleepFlags_u_t;
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
void SYSTEM__1msPoll(void);
void SYSTEM__30sPoll(void);
void SYSTEM__1msTick(void);
void SYSTEM__30sTick(void);
void SYSTEM__SetSleepReadyFlag(SYSTEM__sleep_e flag);
void SYSTEM__ClearSleepReadyFlag(SYSTEM__sleep_e flag);
uint8_t SYSTEM__GetSleepReadyFlag(SYSTEM__sleep_e flag);
void SYSTEM__SleepPoll(void);
void SYSTEM__SetEspTimeoutValue(uint32_t value);



#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_SYSTEM_SYSTEM_H_ */
