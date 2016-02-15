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
  /* Enable SYSCFG Clock - it's required to adc measure, uart and dma */
  RCC->APB2ENR |= (RCC_APB2ENR_SYSCFGEN);
  GPIO__Init();
  SysTick_Config(4000); /* 1ms config */
  GPIO__ConfigUART(1);
  UART__DMAConfig();
  UART__Init(UART__BAUDRATE_9600);
  GPIO__ConfigButton(1);

  MOTORS__jazda_zatrzymana();
  //Zezwolenie na przerwanie globalne
  //  __enable_irq(); // po resecie przerwania sa zalaczone z automatu
  /* Infinite loop */
  while(1)
  {

    if(flag_1ms)
    {
      flag_1ms = 0;
      counter++;
    }

    if(counter >= 5000)
    {
      ADC__CalcTemperature();
      UART__SetIntTempToSend();
      counter = 0;
    }

    UART__Poll();
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
  flag_1ms = 1;
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

  if((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13)
  {
    /* Clear EXTI 0 flag */
    // zerujemy flage przerwania ale UWAGA!!! Tutaj zerujemy ja JEDYNKA, a nie zerem !!!!
    EXTI->PR |= EXTI_PR_PR13;

    GPIOA->ODR ^= (1 << 5);//toggle green led on PA5
    UART__StartDmaTransmision(data, 3);

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


#ifdef __cplusplus
}
#endif
