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

#include "led.h"
#include "gpio.h"
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
  /* "Welcome screen" */
//  GPIO__ConfigLedG(ENABLE);
//  TIMER__DelayMs(1000); /* "Welcome screen" 1 sec LED ON and 1 sec LED off */
//  GPIO__ConfigLedG(DISABLE);
//  TIMER__DelayMs(1000);
//  DEVICE__DCDC_Off();
//  DEVICE__SetDeviceBatteryPowerFlag(DISABLE);
}

/******************************* END FUNCTION *********************************/

void LED__RGB(uint8_t red, uint8_t green, uint8_t blue)
{

	//RED
	if(red == OFF)
	{
		led__data_s.led__red_duty = OFF;
	}
	else if(red == ON)
	{
		led__data_s.led__red_duty = DEFAULT_LED_DUTY;
	}
	else if(red == XOR)
	{
		led__data_s.led__red_duty = 
    (uint8_t)((led__data_s.led__red_duty)? OFF : DEFAULT_LED_DUTY);
	}
	//GREEN
	if(green == OFF)
	{
		led__data_s.led__green_duty = OFF;
	}
	else if(green == ON)
	{
		led__data_s.led__green_duty = DEFAULT_LED_DUTY;
	}
	else if(green == XOR)
	{
		led__data_s.led__green_duty = 
    (uint8_t)((led__data_s.led__green_duty)? OFF : DEFAULT_LED_DUTY);
	}
	//BLUE
	if(blue == OFF)
	{
		led__data_s.led__blue_duty = OFF;
	}
	else if(blue == ON)
	{
		led__data_s.led__blue_duty = DEFAULT_LED_DUTY;
	}
	else if(blue == XOR)
	{
		led__data_s.led__blue_duty = 
    (uint8_t)((led__data_s.led__blue_duty)? OFF : DEFAULT_LED_DUTY);
	}
	
}

/******************************* END FUNCTION *********************************/

void LED__Poll(void)
{
	
//	static int interrupt_counter = 0;
//
//	if(interrupt_counter < led__data_s.led__red_duty)
//	{
//		LED__RED_ON;
//	}
//	else
//	{
//		LED__RED_OFF;
//	}
//
//	/*******************************  *********************************/
//
//	if(interrupt_counter < led__data_s.led__green_duty)
//	{
//		LED__GREEN_ON;
//	}
//	else
//	{
//		LED__GREEN_OFF;
//	}
//
//	/*******************************  *********************************/
//
//
//	if(interrupt_counter < led__data_s.led__blue_duty)
//	{
//
//		LED__BLUE_ON;
//	}
//	else
//	{
//		LED__BLUE_OFF;
//	}
//
//	/*******************************  *********************************/
//
//	interrupt_counter++;
//
//	if(interrupt_counter >= LED__TIME)
//	{
//		interrupt_counter = 0;
//	}
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

