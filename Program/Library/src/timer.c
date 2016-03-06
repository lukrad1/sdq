/** \file timer.c
* \brief This file contains definitions system clock and timers functions.
*
* \author Lukasz Radola
* \date 26.05.15
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
#include "timer.h"
#include "gpio.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

static volatile timer__union_u_t timer__data_u;

/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */


void TIMER__InitClk(void)
{
  /* (1) Enable the peripheral clock of Timer x */
    /* (2) Enable the peripheral clock of GPIOA */
    /* (3) Select alternate function mode on GPIOA pin 0 and 1 */
    /* (4) Select AF2 (see datasheet page 45) on PA0 and PA1 in AFRLow for
     * TIM2_CH1 and CH2*/

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* (1) */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; /* (2) */
    GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1)) \
                   | (GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1); /* (3) */
    GPIOA->AFR[0] |= (0x02 << 0); /* (4) */
    GPIOA->AFR[0] |= (0x02 << (1*4)); /* (4) */

    /* (1) Set prescaler to 40, so APBCLK/4 i.e 10kHz */
    /* (2) Set ARR = 10, as timer clock is 10kHz the period is 100 us */
    /* (3) Set CCRx = 5, , the signal will be high during 50 us */
    /* (4) Select PWM mode 1 on OC1  (OC1M = 110),
           enable preload register on OC1 (OC1PE = 1) */
    /* (5) Select active high polarity on OC1 (CC1P = 0, reset value),
           enable the output on OC1 (CC1E = 1)*/
    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1)
           select edge aligned mode (CMS = 00, reset value)
           select direction as upcounter (DIR = 0, reset value) */
    /* (8) Force update generation (UG = 1) */

    TIMx->PSC = (4-1); /* (1) */
    TIMx->ARR = (100-1); /* (2) */
    TIMx->CCR1 = 50; /* (3) */
    TIMx->CCR2 = 50; /* (3) */
    TIMx->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 |TIM_CCMR1_OC1M_2 |
                   TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE; /* (4) */
    TIMx->CCER |=  TIM_CCER_CC2E | TIM_CCER_CC1E; /* (5) */
    //TIMx->CR1 |= TIM_CR1_CEN; /* (7) */
    TIMx->EGR |= TIM_EGR_UG; /* (8) */

}

/******************************* END FUNCTION *********************************/

void TIMER__PWM_DC1_2_ON(void)
{

  TIMx->CR1 |= TIM_CR1_CEN;
  TIMx->EGR |= TIM_EGR_UG; /* (8) */
}

/******************************* END FUNCTION *********************************/

void TIMER__PWM_DC1_2_OFF(void)
{
  TIMx->CR1 &= ~TIM_CR1_CEN; /* (7) */
}

/******************************* END FUNCTION *********************************/

void TIMER__PWM_DC1_2_Change_Duty(uint8_t duty_in_percent)
{


   /* (3) Set CCRx = 5, , the signal will be high during 50 us */
   /* (4) Select PWM mode 1 on OC1  (OC1M = 110),
          enable preload register on OC1 (OC1PE = 1) */
   /* (5) Select active high polarity on OC1 (CC1P = 0, reset value),
          enable the output on OC1 (CC1E = 1)*/
   /* (6) Enable output (MOE = 1)*/
   /* (7) Enable counter (CEN = 1)
          select edge aligned mode (CMS = 00, reset value)
          select direction as upcounter (DIR = 0, reset value) */
   /* (8) Force update generation (UG = 1) */

  TIMER__PWM_DC1_2_OFF();

   TIMx->CCR1 = duty_in_percent; /* (3) */
   TIMx->CCR2 = duty_in_percent; /* (3) */

   TIMER__PWM_DC1_2_ON();

}

/******************************* END FUNCTION *********************************/

void TIMER__DelayMs(uint16_t ms)
{
  while(ms--)
  {

  }
}

/******************************* END FUNCTION *********************************/

void TIMER__Delay10us(uint16_t us)
{
  while(us--)
  {

  }
}

/******************************* END FUNCTION *********************************/

void TIMER__InitSystemTimer(void)
{
  
#if (_MAIN__F_CPU == TIMER__4MHZ_CLK)
{
  /* for 4 MHz CPU -> prescaler 2^2 = 4->frequency of CK_CNT = 4MHz / 4 = 1 Mhz */

}
#elif (_MAIN__F_CPU == TIMER__1MHZ_CLK)
{
  /* for 1 MHz CPU -> prescaler 2^0 = 1->frequency of CK_CNT = 1 MHz / 1 = 1 Mhz */

}
#endif
  

}

/******************************* END FUNCTION *********************************/

void TIMER__InitInterruptTim4Isr(void)
{
  
}

/******************************* END FUNCTION *********************************/

void TIMER__Poll(void)
{

}

/******************************* END FUNCTION *********************************/

uint8_t TIMER__GetStatus(void)
{
  return timer__data_u.status;
}

/******************************* END FUNCTION *********************************/

void TIMER__SetFlag(timer__union_u_t* flags, uint8_t set_reset_status)
{
  if(set_reset_status == SET)
  {
    timer__data_u.status |= flags->status;
  }
  else
  {
    timer__data_u.status &= (uint8_t)~(flags->status);
  }
}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif
