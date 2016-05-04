/** \file main.c
 * \brief This file contains main function.
 *
 * \author Lukasz Radola
 * \date 22.05.15
 *
 * \details
 *
 */
#ifdef __cplusplus
extern "C"
{
#endif
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* #include <math.h> */
/* other (non local) library */
/* ... */
/* local library */
#include "string.h"
#include "stm32l0xx.h"
#include "timer.h"
#include "system.h"
#include "gpio.h"
#include "adc.h"
#include "uart.h"
#include "motors.h"
#include "obstacle.h"
#include "button_engine.h"
#include "rtc.h"
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

volatile uint16_t error = 0;  //initialized at 0 and modified by the functions
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
  /* Enable SYSCFG Clock - it's required to adc measure, uart and dma */
  RCC->APB2ENR |= (RCC_APB2ENR_SYSCFGEN);
  GPIO__Init();
  TIMER__InitClk();
  SysTick_Config(4000); /* 1ms config */
 // UART__DMAConfig();
 // UART__Init(UART__BAUDRATE_19200);
 // BUTTON__Init();
//  GPIO__ConfigEnkoders(1);
  ADC__ResetIsObstacleFlag();
  MOTORS__jazda_zatrzymana();
  if(RTC__Init() == 0x01)
  {
    GPIOA->ODR ^= (1 << 5);//toggle green led on PA5
  }
  //Zezwolenie na przerwanie globalne
  //  __enable_irq(); // po resecie przerwania sa zalaczone z automatu
  /* Infinite loop */

  GPIOC->BSRR = (1<<11);
  while(1)
  {

    SYSTEM__1msPoll();
    SYSTEM__30sPoll();

  //  UART__Poll();
    SYSTEM__SleepPoll();

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
  while(1)
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
  SYSTEM__1msTick();
  //GPIOC->ODR ^= (1 << 11);//toggle enkoders pin on PC11
}

/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/

/**
 * Brief   This function handles EXTI 4-15 interrupt request. It's button handler
 * Param   None
 * Retval  None
 */
void EXTI4_15_IRQHandler(void)
{
  static uint8_t data[] = "DMA";
  if((EXTI->PR & EXTI_PR_PR10) == EXTI_PR_PR10)
  {
    /* Clear EXTI 0 flag */
    // zerujemy flage przerwania ale UWAGA!!! Tutaj zerujemy ja JEDYNKA, a nie zerem !!!!
    EXTI->PR |= EXTI_PR_PR10;

    OBSTACLE__EnkoderInterrupt();
  }

  if((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13)
  {
    /* Clear EXTI 0 flag */
    // zerujemy flage przerwania ale UWAGA!!! Tutaj zerujemy ja JEDYNKA, a nie zerem !!!!
    EXTI->PR |= EXTI_PR_PR13;
    BUTTON__SetExtiButtonFlag();
    GPIOA->ODR ^= (1 << 5);//toggle green led on PA5
    UART__StartDmaTransmision(data,"", 3,"");

  }
}

/**
 * Brief   This function handles DMA1 channel 4-7 interrupt request.
 * Param   None
 * Retval  None
 */
void DMA1_Channel4_5_6_7_IRQHandler(void)
{
  // If tx transmission is finished, then clear dma flag and tx busy flag
  if((DMA1->ISR & DMA_ISR_TCIF4) == DMA_ISR_TCIF4)
  {
    DMA1->IFCR = DMA_IFCR_CTCIF4;/* Clear TC flag */
    UART__TxInterrupt();
  }

  // if receive data is finished, then check data, clear flags and start wait for
  // recieve state again
  else if((DMA1->ISR & DMA_ISR_TCIF5) == DMA_ISR_TCIF5)
  {
    DMA1->IFCR = DMA_IFCR_CTCIF5;/* Clear TC flag */
    UART__RxInterrupt();
    DMA1_Channel5->CCR &= ~ DMA_CCR_EN;
    DMA1_Channel5->CNDTR = sizeof(stringtoreceive);/* Data size */
    DMA1_Channel5->CCR |= DMA_CCR_EN;
  }
  else
  {
    NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);/* Disable DMA1_Channel2_3_IRQn */
  }
}



/**
  * Brief   This function handles ADC interrupt request.
  *         It stores the data register while EOC occurs.
  *         It reinitializes the CurrentChannel at the End of Sequence
  *         In case of Overrun, the ADC is stopped but not disabled,
  *         and the AD conversion is resumed until the USER button is pressed
  * Param   None
  * Retval  None
  */
void ADC1_COMP_IRQHandler(void)
{
  if ((ADC1->ISR & (ADC_ISR_EOC | ADC_ISR_EOSEQ | ADC_ISR_OVR)) == 0) /* Check if one the expected flag is set */
  {
    error |= ERROR_UNEXPECTED_ADC_IT; /* Report an error */
  }
  else
  {
    if ((ADC1->ISR & ADC_ISR_OVR) != 0)  /* Check OVR has triggered the IT */
    {
      GPIOA->BSRR = (1<<5); /* Switch off green led to report it is due to overrun  */
      ADC1->ISR |= ADC_ISR_EOC | ADC_ISR_EOSEQ | ADC_ISR_OVR; /* Clear all pending flags */
      ADC1->CR |= ADC_CR_ADSTP; /* Stop the sequence conversion */
      /* the data in the DR is considered as not valid */
    }
    else
    {
      if ((ADC1->ISR & ADC_ISR_EOC) != 0)  /* Check EOC has triggered the IT */
      {
        ADC_array[CurrentChannel] = ADC1->DR; /* Read data and clears EOC flag */
        CurrentChannel++;  /* Increment the index on ADC_array */

      }
      if ((ADC1->ISR & ADC_ISR_EOSEQ) != 0)  /* Check EOSEQ has triggered the IT */
      {
        ADC1->ISR |= ADC_ISR_EOSEQ; /* Clear the pending bit */
        CurrentChannel = 0; /* Reinitialize the CurrentChannel */
        ADC__UpdateAdcStruct(ADC_array);
        ADC__DeInit();
      }
    }
  }
}

/**
* @brief This function handles RTC global interrupt through EXTI lines 17, 19 and 20 and LSE CSS interrupt through EXTI line 19.
*/
/**
  * Brief   This function handles RTC interrupt request.
  * Param   None
  * Retval  None
  */
void RTC_IRQHandler(void)
{
  /* Check WUT flag */
  if((RTC->ISR & (RTC_ISR_WUTF)) == (RTC_ISR_WUTF))
  {
    RTC->ISR &=~ RTC_ISR_WUTF; /* Reset Wake up flag */
    EXTI->PR |= EXTI_PR_PR20; /* clear exti line 20 flag */
    GPIOA->ODR ^= (1 << 5) ; /* Toggle Green LED */

  }
  else
  {
    NVIC_DisableIRQ(RTC_IRQn);/* Disable RTC_IRQn */
  }
}
#ifdef __cplusplus
}
#endif
