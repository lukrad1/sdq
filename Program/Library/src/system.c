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
#include "obstacle.h"
#include "uart.h"
#include "adc.h"
#include "button_engine.h"
#include "led.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */

  volatile SYSTEM__sleepFlags_u_t SYSTEM__sleepFlags_u;

  volatile uint8_t system__1msFlag = 0;
  volatile uint8_t system__30sFlag = 0;
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

static void system__CheckInterruptSleepFlag(void)
{
  if(BUTTON__GetExtiButtonFlag())
  {
    BUTTON__ClearExtiButtonFlag();
    SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_BUTTON);
  }
  else
  {
    SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_BUTTON);
  }
}

/**************************** END FUNCTION *********************************/

/**
* \brief This function executes every 2ms when uC dosen't sleep.
*
*/
void SYSTEM__1msPoll(void)
{
  if(system__1msFlag)
  {
    system__1msFlag = 0;
    OBSTACLE__1msPoll();
    ADC__1msPoll();
    LED__1msPoll();
  }
}

/******************************* END FUNCTION *********************************/

/**
* \brief This function executes once per 30s (once per wakeup).
*
*
*/
void SYSTEM__30sPoll(void)
{
  if(system__30sFlag)
  {
    system__30sFlag = 0;

    UART__SetIntTempToSend();
  }
}

/******************************* END FUNCTION *********************************/

/**
* \brief This functions provides a 2ms time base for the purposes of system.
* It should be placed in 2ms interrupt.
*
*
*/
void SYSTEM__1msTick(void)
{
  system__1msFlag = 1;

}

/******************************* END FUNCTION *********************************/

/**
* \brief This functions provides a 30s time base for the purposes of system.
* It should be placed in 30s interrupt.
*
*/
void SYSTEM__30sTick(void)
{
  system__30sFlag = 1;
  SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_ALL_BEFORE_SLEEP);

}

/******************************* END FUNCTION *********************************/

/**
* \brief This function sets flags which indicates that coresponding module is
* ready to go to sleep.
*
*/
 void SYSTEM__SetSleepReadyFlag(SYSTEM__sleep_e flag)
 {

   if(flag == SYSTEM__SLEEPREADY_ALL)
   {
     SYSTEM__sleepFlags_u.flags = SYSTEM__SLEEPFLAG_MASK;
   }
   else if(flag == SYSTEM__SLEEPREADY_ALL_BEFORE_SLEEP)
   {
     SYSTEM__sleepFlags_u.flags |= SYSTEM__SLEEP_AUTOCLEAR_MASK;
   }
   else
   {
     SYSTEM__sleepFlags_u.flags |= (uint8_t)((uint8_t)1 << (uint8_t)flag);
   }

 }

 /******************************* END FUNCTION *********************************/

 /**
 * \brief This function clear flags which indicates that modules are
 * ready to go to sleep.
 *
 */
 void SYSTEM__ClearSleepReadyFlag(SYSTEM__sleep_e flag)
 {
   if(flag == SYSTEM__SLEEPREADY_ALL)
   {
     SYSTEM__sleepFlags_u.flags = 0x00;
   }
   else if(flag == SYSTEM__SLEEPREADY_ALL_BEFORE_SLEEP)
   {
     SYSTEM__sleepFlags_u.flags &= ~SYSTEM__SLEEP_AUTOCLEAR_MASK;
   }
   else
   {
     SYSTEM__sleepFlags_u.flags &= (uint8_t)~((uint8_t)1 << (uint8_t)flag);
   }
 }
 /******************************* END FUNCTION *********************************/

 /**
 * \brief This function returns flags which indicates that modules are
 * ready to go to sleep.
 *
 */
 uint8_t SYSTEM__GetSleepReadyFlag(SYSTEM__sleep_e flag)
 {
   if(flag == 0xFF)
   {
     return (uint8_t)SYSTEM__sleepFlags_u.flags;
   }

   return (uint8_t)(SYSTEM__sleepFlags_u.flags & ((uint8_t)1<<(uint8_t)flag));
 }
 /******************************* END FUNCTION *********************************/

 /**
 * \brief This function periodicaly checks sleep conditions and goes to sleep.
 *
 *
 */
 void SYSTEM__SleepPoll(void)
 {
   system__CheckInterruptSleepFlag();

   if((SYSTEM__sleepFlags_u.flags & SYSTEM__SLEEPFLAG_MASK) == SYSTEM__SLEEPFLAG_MASK)
   {
     ADC__DeInit();
     UART__DeInit();
     GPIOB->BSRR = (1<<5); //switch off led
     //be sure that an adc is turned off
     //ADC__DeInitOptimal();
     //turn off step-up converter
     //PSU__SetupBatteryConverter(PSU__OFF);
     //go to sleep
    //halt();
     //after wakeup flags of the sleep are cleared in SYSTEM__30sTick()
     //automatically
   }
 }
#ifdef __cplusplus
  }
#endif
