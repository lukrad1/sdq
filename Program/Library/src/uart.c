/** \file uart.c
 * \brief file contains uart engine and all the most important uart
 * function.
 *
 * \author Lukasz Radola
 * \date 28.08.15
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
/* ... */
/* other (non local) library */
/* ... */
/* local library */
#include "stm32l0xx.h"
#include "gpio.h"
#include "uart.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */

/* UART status union. In this union are uart flags for example transmision flag */

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

volatile const uint8_t stringtosend[4] = "DMA\n";
volatile uint8_t stringtoreceive[2];
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */

void UART__Init(uint32_t baudrate)
{
  /* Enable the peripheral clock USART2 */
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

  /* Configure USART2 */
  /* (1) oversampling by 16, 9600 baud = USARTDIV = clk/9600, BRR = USARTDIV*/
  /* (2) Enable DMA in reception and transmission */
  /* (3) 8 data bit, 1 start bit, 1 stop bit, no parity, reception and transmission enabled */
  USART2->BRR = MAIN__F_CPU / baudrate; /* (1) */
  USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR; /* (2) */
  USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; /* (3) */

  while((USART2->ISR & USART_ISR_TC) != USART_ISR_TC)/* polling idle frame Transmission */
  {
    /* add time out here for a robust application */
  }
  USART2->ICR = USART_ICR_TCCF;/* Clear TC flag */
}


/******************************* END FUNCTION *********************************/

void UART__DMAConfig(void)
{
  /* Enable the peripheral clock DMA1 */
   RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    /* reference manual str. 267 pdf */
   /* DMA1 Channel4 USART2_TX config  */
   /* (1)  Map USART2_TX DMA channel CSELR = 0100 na C4S (12 - 15 bit) */
   /* (2)  Peripheral address */
   /* (3)  Memory address */
   /*Set the memory address in the DMA_CMARx register. The data will be written to or
read from this memory after the peripheral event.*/
   /* (4)  Memory increment */
   /*      Memory to peripheral */
   /*      8-bit transfer */
   /*      Transfer complete IT */
   DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C4S) | (4 << (3 * 4)); /* (1) */
   DMA1_Channel4->CPAR = (uint32_t)&(USART2->TDR); /* (2) */
   DMA1_Channel4->CMAR = (uint32_t)stringtosend; /* (3) */
   DMA1_Channel4->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE; /* (4) */

   /* DMA1 Channel5 USART_RX config */
   /* (5)  Map USART2_RX DMA channel  CSELR = 0100 na C5S (16 - 19 bit)*/
   /* (6)  Peripheral address */
   /* (7)  Memory address */
   /* (8)  Data size */
   /* (9)  Memory increment */
   /*      Peripheral to memory*/
   /*      8-bit transfer */
   /*      Transfer complete IT */
   DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C5S) | (4 << (4 * 4)); /* (5) */
   DMA1_Channel5->CPAR = (uint32_t)&(USART2->RDR); /* (6) */
   DMA1_Channel5->CMAR = (uint32_t)stringtoreceive; /* (7) */
   DMA1_Channel5->CNDTR = sizeof(stringtoreceive); /* (8) */
   DMA1_Channel5->CCR = DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN; /* (9) */

   /* Configure IT */
   /* (10) Set priority for DMA1_Channel2_3_IRQn */
   /* (11) Enable DMA1_Channel2_3_IRQn */
   NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0); /* (10) */
   NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn); /* (11) */

}

void UART__StartDmaTransmision(void)
{
  /* start 8-bit transmission with DMA */
  DMA1_Channel4->CCR &=~ DMA_CCR_EN;
  DMA1_Channel4->CNDTR = sizeof(stringtosend);/* Data size */
  DMA1_Channel4->CCR |= DMA_CCR_EN;

}
#ifdef __cplusplus
}
#endif

