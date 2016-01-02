/** \file main.c
* \brief This file contains main function.
*
* \author Lukasz Radola
* \date 22.05.15
*
* \details First template of project in accordance with new Fibaro coding law.
*
*/
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* #include <math.h> */
/* other (non local) library */
/* ... */
/* local library */

#include "stm32l0xx.h"
#include "timer.h"
#include "system.h"
#include "gpio.h"
//#include "adc.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */



/* Typedef definition (local) */
/* ... */

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */
/* for example: extern unsigned int module_variable_1; */



volatile uint8_t flag_1ms = 0;
uint16_t counter = 0;
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */
/* ... */

/* Static functions definitions */

/****************************************************************************/
/*                              MAIN FUNCTION                               */
/****************************************************************************/


int main(void)
{
	/*System_Init(); This function is enabled in startup stm32 file by default*/
	SystemClock_Config();
	GPIO__Init();
  SysTick_Config(4000); /* 1ms config */
  //GPIOA->BSRR = (1 << 5); //switch on the red LED on PA5

  /* Infinite loop */
  while (1)
  {
    if(flag_1ms)
    {
      flag_1ms = 0;
      counter++;
    }

    if(counter >= 3000)
    {
      //ADC__CalcTemperature();
      counter = 0;
//      if(temperature_C > 25)
//      {
//        GPIOA->BSRR = (1<<5); /* Set red led on PA5 */
//      }
//      else
//      {
//        GPIOA->BRR = (1<<5); /* Switch off red led on PA5 */
//      }
      GPIOA->ODR ^= (1 << 5);//toggle green led on PA5
    }
  }
}




/******************************************************************************/
/*            Cortex-M0 Plus Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * Brief   This function handles NMI exception.
  * Param   None
  * Retval  None
  */
void NMI_Handler(void)
{
}

/**
  * Brief   This function handles Hard Fault exception.
  * Param   None
  * Retval  None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * Brief   This function handles SVCall exception.
  * Param   None
  * Retval  None
  */
void SVC_Handler(void)
{
}

/**
  * Brief   This function handles PendSVC exception.
  * Param   None
  * Retval  None
  */
void PendSV_Handler(void)
{
}

/**
  * Brief   This function handles SysTick Handler.
  *         It toggles the green led if the action has been performed correctly
  *         and toggles the red led coding the error number
  * Param   None
  * Retval  None
  */
void SysTick_Handler(void)
{
  flag_1ms = 1;
}


/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/

/**
  * Brief   This function handles PPP interrupt request.
  * Param   None
  * Retval  None
  */
/*void PPP_IRQHandler(void)
{
}*/


#ifdef __cplusplus
  }
#endif
