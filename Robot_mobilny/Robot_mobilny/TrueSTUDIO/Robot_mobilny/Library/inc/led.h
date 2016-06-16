/** \file led.h
* \brief This file contains in general gpio configurartion.
*
* \author Lukasz Radola
* \date 25.08.15
*
* \details In this file are definitions, declarations of LED functions.
*/
#ifndef LRADOLA_LED_LED_H_
#define LRADOLA_LED_LED_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*

void LED__Init(void);
void LED__RGB(uint8_t red, uint8_t green, uint8_t blue);
void LED__Poll(void);
uint8_t LED__GetLedRedDuty(void);
uint8_t LED__GetLedGreenDuty(void);
uint8_t LED__GetLedBlueDuty(void);

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

/* This definition is used in led.c file. LED polling function is lunched every
0,0001 sec so this value specify frequency and is maximum duty value = 100% */

#define LED__TIME									100 	

/* LED - specific */ 
#define LED__RED_ON GPIO__LED_R_PORT->ODR &= (uint8_t)~GPIO__LED_R_PIN 
#define LED__RED_OFF GPIO__LED_R_PORT->ODR |= (uint8_t)GPIO__LED_R_PIN

#define LED__GREEN_ON GPIO__LED_G_PORT->ODR &= (uint8_t)~GPIO__LED_G_PIN 
#define LED__GREEN_OFF GPIO__LED_G_PORT->ODR |= (uint8_t)GPIO__LED_G_PIN

#define LED__BLUE_ON GPIO__LED_B_PORT->ODR &= (uint8_t)~GPIO__LED_B_PIN 
#define LED__BLUE_OFF GPIO__LED_B_PORT->ODR |= (uint8_t)GPIO__LED_B_PIN




enum ledColour
{
  OFF = 0,
  ON,
  XOR,
  DEFAULT_LED_DUTY = 90
  
};
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

/**
* \brief This function is lunched when software is started.
*
* This init function turns on DCDC power if is battery power supply and turn on
* one white led blink.
*
*/
void LED__Init(void);

/**
* \brief This function turns on/off LED.
* 
* \param[in] red - this param specify operation on red colour
* \param [in] green - this param specify operation on green colour
* \param [in] blue - this param specify operation on blue colour
*
*/
void LED__RGB(uint8_t red, uint8_t green, uint8_t blue);

/**
* \brief LED polling function.
*
* In this function are checking all timer flags. It is running in while(1) loop.
*
*/
void LED__1msPoll(void);

/**
* \brief Function which gets led red duty value.
*
* \return led__data.led__red_duty value.
*
*/
uint8_t LED__GetLedRedDuty(void);

/**
* \brief Function which gets led green duty value.
*
* \return led__data.led__green_duty value.
*
*/
uint8_t LED__GetLedGreenDuty(void);

/**
* \brief Function which gets led blue duty value.
*
* \return led__data.led__blue_duty value.
*
*/
uint8_t LED__GetLedBlueDuty(void);

#ifdef __cplusplus
  }
#endif

#endif /* LRADOLA_LED_LED_H_ */

