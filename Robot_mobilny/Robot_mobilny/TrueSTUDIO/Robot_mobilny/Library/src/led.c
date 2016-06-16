/** \file adc.c
* \brief This file contains definitions system clock and timers functions.
*
* \author Lukasz Radola
* \date 30.06.15
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
#include "stm32l0xx.h"
#include "led.h"
#include "gpio.h"
#include "system.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */

typedef struct
{
	uint8_t led__red_duty;			// duty podane w procentach
	uint8_t led__green_duty;
	uint8_t led__blue_duty;
	
	uint8_t led__red_flag:1;						// flaga okreslajaca, czy dany led zostal zalaczony
	uint8_t led__green_flag:1;
	uint8_t led__blue_flag:1;
	
}led__struct_s;

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */
led__struct_s led__data_s;

/*! Global Enum variable which contain names each soft production engine states */
enum ledColour LEDColour = OFF;
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/

/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */


void LED__Init(void)
{

}

/******************************* END FUNCTION *********************************/

void LED__RGB(uint8_t red, uint8_t green, uint8_t blue)
{

	
}

/******************************* END FUNCTION *********************************/

void LED__1msPoll(void)
{
  if((GPIOA->IDR & (1<<5)))
  {
    SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_LED_BLINK);
  }
  else
  {
    SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_LED_BLINK);
  }
}

/******************************* END FUNCTION *********************************/

uint8_t LED__GetLedRedDuty(void)
{
  return led__data_s.led__red_duty;
}

/******************************* END FUNCTION *********************************/

uint8_t LED__GetLedGreenDuty(void)
{
  return led__data_s.led__green_duty;
}

/******************************* END FUNCTION *********************************/

uint8_t LED__GetLedBlueDuty(void)
{
  return led__data_s.led__blue_duty;
}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif

