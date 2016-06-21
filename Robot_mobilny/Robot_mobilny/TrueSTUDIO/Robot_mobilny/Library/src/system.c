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
#include "uart_raspb.h"
#include "uart_esp.h"
#include "motors.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */

  volatile SYSTEM__sleepFlags_u_t SYSTEM__sleepFlags_u;

  volatile uint8_t system__1msFlag = 0;
  volatile uint8_t system__30sFlag = 0;
  volatile uint32_t system__esp_timeout = 0;
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
     // set core voltage at 1.2V
     PWR->CR = (PWR->CR & 0xFFFFE7FF) | PWR_CR_VOS_1 | PWR_CR_VOS_0;
   }
   else if(MAIN__F_CPU == SYSTEM__16MHZ_CLOCK)
   {
     // set core voltage at 1.5V
     PWR->CR = (PWR->CR & 0xFFFFE7FF) | PWR_CR_VOS_1;
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
	static uint16_t counter = 0;
	static uint8_t rasp_pin_counter_Off = 0;
	static uint32_t rasp_pin_counter_On = 0;


  if(system__1msFlag)
  {
    system__1msFlag = 0;
    OBSTACLE__1msPoll();
    ADC__1msPoll();
    LED__1msPoll();

    //-----------------------------------------------------------------------------------------

    //jezeli na raspberry pojawi sie odpowiedni stan to skroc timeout i wylacz po 15s
    if(((GPIOA->IDR & GPIO_IDR_ID6) == GPIO_IDR_ID6)
    	&& rasp_pin_counter_Off != 0xFF && UART_ESP__GetRaspberry_Run_Flag())
    {
    	rasp_pin_counter_Off++;
    	if(rasp_pin_counter_Off > 10)
    	{
    		SYSTEM__SetEspTimeoutValue(15000);
    		rasp_pin_counter_Off = 0xFF;
    	}
    }
    // w przeciwnym razie, jezeli rasp jest zalaczony i mam stan niski na wejsciu to przedluzaj caly czas
    // dzialanie urzadzenia, chyba ze minie 8 min i urzadzenie nadal bedzie chodzic
    else if(((GPIOA->IDR & GPIO_IDR_ID6) != GPIO_IDR_ID6)
    		&& rasp_pin_counter_Off != 0xFF && UART_ESP__GetRaspberry_Run_Flag()
    		&& MOTORS__GetCurrentDirection() == JAZDA_ZATRZYMANA)
	{
    	rasp_pin_counter_On++;
		if(rasp_pin_counter_On % 1000)
		{
			SYSTEM__SetEspTimeoutValue(60000);
		}
		if(rasp_pin_counter_On > 480000)	//8min
		{
			// natychmiast wylacz wszystko
			SYSTEM__SetEspTimeoutValue(9);
		}
	}

    else if(MOTORS__GetCurrentDirection() != JAZDA_ZATRZYMANA)
    {
    	rasp_pin_counter_On = 0;
    }

    //-----------------------------------------------------------------------------------------
    // jezeli urzadzenie (esp) jest uruchomione, ale jeszcze nie zainicjalizowane to zaczynamy
    // odliczac czas zwloki do inicjalizacji servera. Jest on konieczny, gdyz bez niego ramki dochodzily
    // za wczesnie do esp i byly gubione
    if(UART_ESP__GetIsWakeupFlag() && !UART_ESP__GetStartInit() && !UART_ESP__GetFinishTransmisionFlag())
    {
    	counter++;
    }
    else
    {
    	counter = 0;
    }

    //-----------------------------------------------------------------------------------------

    //wlasciwa funkcja odliczania timeoutu od wylaczenia komunikacji z raspberry i silnikami itp.
    if(UART_ESP__GetTimeout())
	{
		system__esp_timeout--;
		if(system__esp_timeout < 10)
		{
			UART_ESP__SetFinishTransmisionFlag();
			UART_ESP__ClearStartInit();
			UART_ESP__ClearTimeout();
			GPIO__ConfigSharpEspEnable(0);
			GPIO__ConfigMotorsEnable(0);
			GPIO__ConfigRaspbEnable(0);
			UART_ESP__ClearRaspberry_Run_Flag();
			rasp_pin_counter_Off = 0;
			rasp_pin_counter_On = 0;
			GPIOA->BSRR |= (1 << 5); // turn off led
			//dodac odczyt z raspberry ze jezeli chodzi to ustaw pin i raaspberry odczyta, uspi sie i dopiero go wylacz
		}
	}

    //-----------------------------------------------------------------------------------------
  }

  	// jezeli powyzej counter zostanie ustawiony na wartosci wieksza od okreslonej to uruchamiamy inicjalizacje
  	// serwera na esp
	if(counter >=4000)
	{
		counter = 0;
		UART_ESP__SetStartInit();

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
    UART_RASPB__SetVrefToSend();
    //UART__SetIntTempToSend();
    //SPI_Raspb__SetDefaultMessToSend();
    //jezeli esp nie byl wybudzony
    if(!UART_ESP__GetIsWakeupFlag())
	{
    	GPIO__ConfigSharpEspEnable(1);
		UART_ESP__SetIsWakeupFlag();
		UART_ESP__SetTimeout();
		SYSTEM__SetEspTimeoutValue(20000); // 11s
	}
	else if(UART_ESP__GetFinishTransmisionFlag())
	{
		// blokada jednego cyklu aktywacji esp po transmisji, aby nie bylo tak, ze wylacze esp
		// a rtc od razu mi go wybudzi
		UART_ESP__ClearIsWakeupFlag();
		UART_ESP__ClearFinishTransmisionFlag();
		GPIOA->BSRR |= (1 << 5); // turn off led
	}
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

 /******************************* END FUNCTION *********************************/

 void SYSTEM__SetEspTimeoutValue(uint32_t value)
 {
	 system__esp_timeout = value;
 }
#ifdef __cplusplus
  }
#endif
