/** \file system.c
* \brief This file contains definitions system clock and system functions.
*
* \author Lukasz Radola
* \date 02.01.16
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
#include "system.h"
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


/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/

/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */

/**
  * Brief   This function configures the system clock @4MHz and voltage scale 2
  *         assuming the registers have their reset value before the call.
  *         POWER SCALE   = RANGE 2
  *         SYSTEM CLOCK  = HSI/4
  *         FLASH LATENCY = 0
  * Param   None
  * Retval  None
  */
__INLINE void SystemClock_Config(void)
{

  RCC->APB1ENR |= (RCC_APB1ENR_PWREN); /* (1) */
  // wait for VSO stabilization
  while (PWR->CSR & PWR_CSR_VOSF)
    ;

  if(MAIN__F_CPU == SYSTEM__4MHZ_CLOCK)
  {
    // set core voltage at 1.5V
    PWR->CR = (PWR->CR & 0xFFFFE7FF) | PWR_CR_VOS_1;
  }
  else if(MAIN__F_CPU == SYSTEM__16MHZ_CLOCK)
  {
    // set core voltage at 1.8V
    PWR->CR = (PWR->CR & 0xFFFFE7FF) | PWR_CR_VOS_0;
  }

  // wait for VSO stabilization
  while (PWR->CSR & PWR_CSR_VOSF)
    ;

  // HSI CONFIGURATION
  if(MAIN__F_CPU == SYSTEM__4MHZ_CLOCK)
  {
    // div 4 prescaler
     RCC->CR |= RCC_CR_HSIDIVEN;
  }

  // turn on HSI
  RCC->CR |= RCC_CR_HSION;
  // wait until HSI ready
  while ((RCC->CR & RCC_CR_HSIRDY) == 0)
    ;

   // FLASH LATENCY
   FLASH->ACR &= ~FLASH_ACR_LATENCY;
   // Set clock source to PLL
   RCC->CFGR |= RCC_CFGR_SW_HSI;
   // Check clock source
   while ((RCC->CFGR & RCC_CFGR_SWS_HSI) != RCC_CFGR_SWS_HSI)
     ;

   SystemCoreClockUpdate();
}

/**************************** END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif
