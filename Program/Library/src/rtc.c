/** \file rtc.c
* \brief This file contains definitions rtc clock and functions.
*
* \author Lukasz Radola
* \date 02.08.15
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

#include "gpio.h"
#include "stm32l0xx.h"
#include "rtc.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */

#define ERROR_RTC 0x01
#define RTC_OK    0x00
static volatile struct RTC__struct
{
    uint8_t isInitiated : 1;
    uint8_t calibStage : 1;
    uint8_t calibEnd : 1;
		
} RTC__status = {0};

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

uint16_t LSI_measured_frequency;

									//ok. RTC_sleep_time+1 sekund spania  1s->18,2h

/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */


static void rtc__initTime(void)
{
  /* RTC init mode */
  /* Configure RTC */
  /* (1) Write access for RTC registers */
  /* (2) Enable init phase */
  /* (3) Wait until it is allow to modify RTC register values */
  /* (4) set prescaler, 40kHz/64 => 625 Hz, 625Hz/625 => 1Hz */
  /* (5) New time in TR */
  /* (6) Disable init phase */
  /* (7) Disable write access for RTC registers */
  RTC->WPR = 0xCA; /* (1) */
  RTC->WPR = 0x53; /* (1) */
  RTC->ISR |= RTC_ISR_INIT; /* (2) */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* (3) */
  {
   /* add time out here for a robust application */
  }
  RTC->PRER = 0x003F0270; /* (4) */
  RTC->TR = RTC_TR_PM | 1; /* (5) */
  RTC->ISR &=~ RTC_ISR_INIT; /* (6) */
  RTC->WPR = 0xFE; /* (7) */
  RTC->WPR = 0x64; /* (7) */
}

/******************************* END FUNCTION *********************************/
static uint8_t rtc__DeInit(void)
{
  uint32_t timeout = 0;

  /* Configure RTC */
  /* (7) Write access for RTC regsiters */
  /* (8) Disable wake up timerto modify it */
  /* (9) Wait until it is allow to modify wake up reload value */
  /* (10) Modify wake up value reload counter to have a wake up each 1Hz */
  /* (11) Enable wake up counter and wake up interrupt */
  /* (12) Disable write access */
  RTC->WPR = 0xCA; /* (7) */
  RTC->WPR = 0x53; /* (7) */
  RTC->ISR |= RTC_ISR_INIT;

  while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF) /* (9) */
  {
    timeout++;
    if(timeout > 20000)
    { /* Exit Initialization mode */
      RTC->ISR &= ((uint32_t)~RTC_ISR_INIT);
      return ERROR_RTC;
    }
    /* add time out here for a robust application */
  }

  RTC->TR = (uint32_t)0x00000000;
  RTC->DR = ((uint32_t)(RTC_DR_WDU_0 | RTC_DR_MU_0 | RTC_DR_DU_0));
  RTC->CR &= ~RTC_CR_WUCKSEL;

  while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF) /* (9) */
  {
    timeout++;
    if(timeout > 20000)
    {
      break;
      //return ERROR_RTC;
    }
    /* add time out here for a robust application */
  }

  timeout = 0;
  /* Reset all RTC CR register bits */
   RTC->CR &= (uint32_t)0x00000000;
   RTC->WUTR = RTC_WUTR_WUT;
   RTC->PRER = ((uint32_t)(RTC_PRER_PREDIV_A | 0x000000FF));
   RTC->ALRMAR = (uint32_t)0x00000000;
   RTC->ALRMBR = (uint32_t)0x00000000;
   RTC->SHIFTR = (uint32_t)0x00000000;
   RTC->CALR = (uint32_t)0x00000000;
   RTC->ALRMASSR = (uint32_t)0x00000000;
   RTC->ALRMBSSR = (uint32_t)0x00000000;

   /* Reset ISR register and exit initialization mode */
   RTC->ISR = (uint32_t)0x00000000;

   /* Reset Tamper configuration register */
   RTC->TAMPCR = 0x00000000;

   /* Reset Option register */
   RTC->OR = 0x00000000;
   /* Enable the write protection for RTC registers */

   /* Exit Initialization mode */
      RTC->ISR &= ((uint32_t)~RTC_ISR_INIT);
   RTC->WPR=0xFF;

   return RTC_OK;
}
uint8_t RTC__Init(void)
{
  uint32_t timeout = 0;

 /*if(rtc__DeInit() == ERROR_RTC)
  {

    return ERROR_RTC;
  }*/
  /* Enable the peripheral clock RTC */
    /* (1) Enable the LSI */
    /* (2) Wait while it is not ready */
    /* (3) Enable PWR clock */
    /* (4) Enable write in RTC domain control register */
    /* (5) LSI for RTC clock */
    /* (6) Disable PWR clock */
    RCC->CSR |= RCC_CSR_LSION; /* (1) */
    while((RCC->CSR & RCC_CSR_LSIRDY)!=RCC_CSR_LSIRDY) /* (2) */
    {
      /* add time out here for a robust application */
      timeout++;
      if(timeout > 20000)
      {
        /* Enable the write protection for RTC registers */
        return ERROR_RTC;
      }
    }
    timeout = 0;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN; /* (3) */
    PWR->CR |= PWR_CR_DBP; /* (4) */
    RCC->CSR = (RCC->CSR & ~RCC_CSR_RTCSEL) | RCC_CSR_RTCEN | RCC_CSR_RTCSEL_1; /* (5) */
    RCC->APB1ENR &=~ RCC_APB1ENR_PWREN; /* (6) */


/******************* MAIN HAL RTC INIT *****************************/

   RTC->WPR = 0xCA; /* (7) */
   RTC->WPR = 0x53; /* (7) */
   /* Go to Init RTC mode = all counters stopped */
   RTC->ISR |= RTC_ISR_INIT;

   while(RTC->ISR & RTC_ISR_INITF != RTC_ISR_INITF)
   {
     timeout++;
     if(timeout > 20000)
     {
       /* Enable the write protection for RTC registers */
       return ERROR_RTC;
     }

   }
   timeout = 0;
   /* Clear RTC_CR FMT, OSEL and POL Bits */
   RTC->CR &= ((uint32_t)~(RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL));

   /* Configure the RTC PRER */
   RTC->PRER = (uint32_t)(255);
   RTC->PRER |= (uint32_t)(127 << 16);

   /* Exit Initialization mode */
   RTC->ISR &= ((uint32_t)~RTC_ISR_INIT);

   /* Enable the write protection for RTC registers */

  RTC->WPR=0xFF;

  /******************* MAIN HAL RTC INIT *****************************/

    /* Configure RTC */
    /* (7) Write access for RTC regsiters */
    /* (8) Disable wake up timerto modify it */
    /* (9) Wait until it is allow to modify wake up reload value */
    /* (10) Modify wake up value reload counter to have a wake up each 1Hz */
    /* (11) Enable wake up counter and wake up interrupt */
    /* (12) Disable write access */
    RTC->WPR = 0xCA; /* (7) */
    RTC->WPR = 0x53; /* (7) */

    RTC->ISR |= RTC_ISR_INIT;
    while(RTC->ISR & RTC_ISR_INITF != RTC_ISR_INITF)
    {
      timeout++;
      if(timeout > 20000)
      {
        /* Enable the write protection for RTC registers */
        return ERROR_RTC;
      }

    }
    timeout = 0;

    if((RTC->CR & RTC_CR_WUTE) != RESET)
    {
       /* Wait till RTC WUTWF flag is reset and if Time out is reached exit */
       while((RTC->ISR & RTC_ISR_WUTWF) == RTC_ISR_WUTWF)
       {
         timeout++;
         if(timeout > 20000)
         {
           /* Enable the write protection for RTC registers */
              RTC->WPR=0xFF;
           return ERROR_RTC;
         }
       }
    }
    timeout = 0;
    RTC->CR &=~ RTC_CR_WUTE; /* (8) */
    while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF) /* (9) */
    {
      /* add time out here for a robust application */
      timeout++;
      if(timeout > 20000)
      {
        /* Enable the write protection for RTC registers */
           RTC->WPR=0xFF;
        return ERROR_RTC;
      }
    }
    timeout = 0;
    RTC->WUTR = (uint32_t)80; /* (10) */ // ze wzgledu na brak kalibracji 60 sek = 48 realych

      /* Clear the Wakeup Timer clock source bits in CR register */
      RTC->CR &= (uint32_t)~RTC_CR_WUCKSEL;

      /* Configure the clock source */
      RTC->CR |= (uint32_t)RTC_WAKEUPCLOCK_CK_SPRE_16BITS;

      /* RTC WakeUpTimer Interrupt Configuration: EXTI configuration */
      EXTI->IMR |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT;

      EXTI->RTSR |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT;

      /* Configure the Interrupt in the RTC_CR register */
      RTC->CR |=  RTC_CR_WUTIE ; /* (11) */

      /* Enable the Wakeup Timer */
      RTC->CR |=  RTC_CR_WUTE ; /* (11) */

      /* Exit Initialization mode */
         RTC->ISR &= ((uint32_t)~RTC_ISR_INIT);

      /* Enable the write protection for RTC registers */
      RTC->WPR=0xFF;

    //see page 162 pdf in reference manual
    /* Configure exti and nvic for RTC IT */
      /* Note: the define for the EXTI20 are not done in the stm32l053xx.h */
      /* (13) unmask line 20 */
      /* (14) Rising edge for line 20 */
      /* (15) Set priority */
      /* (16) Enable RTC_IRQn */
     // EXTI->IMR |= EXTI_IMR_IM20; /* (13) */
     // EXTI->RTSR |= EXTI_RTSR_TR20; /* (14) */
      NVIC_SetPriority(RTC_IRQn, 0); /* (15) */
      NVIC_EnableIRQ(RTC_IRQn); /* (16) */

      return RTC_OK;
      //rtc__initTime();
}

/******************************* END FUNCTION *********************************/

/**
* @brief Funkcja ustawiajaca kolejny czas wybudzenia uC
*
*
* @param RTC__next_wakeup_time - czas po ktorym zostanie wygenerowane kolejne przerwanie RTC
* @retval void
*/
void RTC__Set_Wakeup(uint32_t *RTC__next_wakeup_time)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Ustawia alarm poprzez czas
*
*
* @param Czas do najblizszego alarmu w sek.
* @retval void
*/
void RTC__Set_Alarm(uint32_t *time)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Zatrzymuje zegar odliczajacy do nastepnego przerwania RTC Wakeup
*
*
* @param void
* @retval void
*/
void RTC__Disable_Wakeup(void)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Procedura przerwania od RTC
*
*
* @param void
* @retval void
*/
void RTC__Interrupt_Routine(void)
{

}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif
