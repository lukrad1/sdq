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
#include "adc.h"
#include "motors.h"
#include "timer.h"
#include "system.h"
#include "obstacle.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */

/* UART status union. In this union are uart flags for example transmision flag */
static volatile union uart__status_u
{
  uint16_t status; /* status variable, which is used to clear all uart status flag */
  struct
  {
    uint8_t txBusyFlag: 1;
    uint8_t rxBusyFlag: 1;
    uint8_t receivedData: 1;
    uint8_t sendInternalTemp: 1;
    uint8_t sendSharp1 : 1;
    uint8_t sendSharpPrzodLewy : 1;
    uint8_t sendSharpPrzodPrawy : 1;
    uint8_t sendSharpTylSrodek : 1;
    uint8_t sendVrefValue:1;
    uint8_t sendPWMValue:1;
    uint8_t flag10: 1;
    uint8_t fullTransmision: 1;
    uint8_t flag12: 1;
    uint8_t Pwm_value;
    uint8_t flag14: 1;
    uint8_t flag15: 1;
  };
}
uart__status_u = {0}; /* Uart__status_u union declaraction. The variable name
                         is this same: uart__status_u. */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

volatile uint8_t stringtosend[20];
volatile uint8_t stringtoreceive[1];
volatile uint8_t RxBuffer[10];
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */

void UART__Init(uint32_t baudrate)
{
  GPIO__ConfigUART(1);

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

void UART__DeInit(void)
{
  /* Disable the peripheral clock USART2 */
  RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
  GPIO__ConfigUART(0);
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
   DMA1_Channel5->CNDTR = 1; /* (8) */
   DMA1_Channel5->CCR = DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN; /* (9) */

   /* Configure IT */
   /* (10) Set priority for DMA1_Channel2_3_IRQn */
   /* (11) Enable DMA1_Channel2_3_IRQn */
   NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 1); /* (10) */
   NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn); /* (11) */

}

void UART__StartDmaTransmision(int8_t* data, int8_t* additional_text,
                               uint8_t length, int8_t* message)
{
  int i,j,h = 0;
  SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);

  length += 4; // na znak konca lini i bit startu
  if(length > 20)
  {
    length = 20;
  }
  stringtosend[0] = 0x02; // start byte
  stringtosend[1] = message[0]; // T
  for(i = 2; i < (length - 2); i++)
  {
    stringtosend[i] = data[i-2];
    if(data[i-2] == 0)
    {
      for(j = i; j < (length - 2); j++)
      {
        stringtosend[j] = additional_text[h];
        if(additional_text[h] == 0)
        {
          length = j+2;
          j = 21;
        }
        h++;
      }
      // wyjdz z fora i jednoczesnie miej miejsce na lf
      i = 21;
    }
  }
  stringtosend[length-2] = 10;
  stringtosend[length-1] = 12; // czy tu nie powinno byc 13????
  /* start 8-bit transmission with DMA */
  DMA1_Channel4->CCR &=~ DMA_CCR_EN;
  DMA1_Channel4->CNDTR = length-1;/* Data size */
  DMA1_Channel4->CCR |= DMA_CCR_EN;
  uart__status_u.txBusyFlag = 1;
}

void UART__RxInterrupt(void)
{
  static uint8_t index = 0;

  if(!uart__status_u.receivedData)
  {
    if(stringtoreceive[0] == 0x02)// bit startu
    {
      index = 0;
    }
    else if(index < 4)
    {
      RxBuffer[index] = stringtoreceive[0];
      stringtoreceive[0] = 0;

      if(RxBuffer[index] == 10 && RxBuffer[index-1] == 13) // cr + lf
      {
        index = 4; // koniec transmisji i blokada, czekanie na bit startu
        SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
        uart__status_u.receivedData = 1;
      }

      index++;
    }
  }
}

void UART__TxInterrupt(void)
{
  //kasowanie flagi zajetosci, bo wszystko bylo w dma, i gdy wszedlem do
  // przerwaniato juz mam po transmisji, odebralem wszystkie dane
  uart__status_u.txBusyFlag = 0;
  SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
}

void UART__Poll(void)
{
  int8_t data[4];

  if(uart__status_u.receivedData)
  {

    if(RxBuffer[0] == 49 && RxBuffer[1] == 49)
    {
      MOTORS__jazda_do_tylu();
      MOTORS__SetLastDirection(JAZDA_DO_TYLU);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(RxBuffer[0] == 49 && RxBuffer[1] == 48)
    {
      MOTORS__skret_w_lewo();
      MOTORS__SetLastDirection(JAZDA_W_LEWO);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(RxBuffer[0] == 50 && RxBuffer[1] == 50)
    {
      MOTORS__jazda_zatrzymana();
      MOTORS__SetLastDirection(JAZDA_ZATRZYMANA);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(RxBuffer[0] == 50 && RxBuffer[1] == 48)
    {
      MOTORS__skret_w_prawo();
      MOTORS__SetLastDirection(JAZDA_W_PRAWO);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(RxBuffer[0] == 50 && RxBuffer[1] == 53)
    {
      MOTORS__jazda_do_przodu();
      MOTORS__SetLastDirection(JAZDA_DO_PRZODU);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(RxBuffer[0] == 37)
    {
      if(!ADC__GetIsObstacleFlag())
      {
        uart__status_u.Pwm_value = RxBuffer[1];
        TIMER__PWM_DC1_2_Change_Duty(uart__status_u.Pwm_value);
        //uart__status_u.sendPWMValue = 1;
      }

    }

    uart__status_u.receivedData = 0;
    // jezeli bedzie cos do wyslania, to ponizej te flage wyzeruje w funkcji start dma
    SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
  }

  if(!uart__status_u.txBusyFlag)
  {
    if(uart__status_u.sendInternalTemp)
    {

      uart__status_u.sendInternalTemp = 0;

      sprintf(data, "%d", (int)ADC__GetTempDegreeValue());
      UART__StartDmaTransmision(data, "", 2,"T");

    }
    else if(uart__status_u.sendSharp1)
    {

      uart__status_u.sendSharp1 = 0;

      sprintf(data, "%d", (int)ADC__GetSharp1MvValue());
      UART__StartDmaTransmision(data,"", 0,"B");

    }
    else if(uart__status_u.sendSharpPrzodLewy)
    {

      uart__status_u.sendSharpPrzodLewy = 0;

      sprintf(data, "%d", (int)ADC__GetSharpPrzodLewyMvValue());
      UART__StartDmaTransmision(data,"", 0,"A");

    }
    else if(uart__status_u.sendSharpPrzodPrawy)
    {

      uart__status_u.sendSharpPrzodPrawy = 0;

      sprintf(data, "%d", (int)ADC__GetSharpPrzodPrawyMvValue());
      UART__StartDmaTransmision(data,"", 0,"C");

    }
    else if(uart__status_u.sendSharpTylSrodek)
    {

      uart__status_u.sendSharpTylSrodek = 0;

      sprintf(data, "%d", (int)ADC__GetSharpTylSrodekMvValue());
      UART__StartDmaTransmision(data,"", 0,"E");

    }
    else if(uart__status_u.sendVrefValue)
    {
      uart__status_u.sendVrefValue = 0;

      sprintf(data, "%d", (int)ADC__GetVrefAdcValue());
      UART__StartDmaTransmision(data," Vref", 9,"V");

    }
    else if(uart__status_u.sendPWMValue)
    {
      uart__status_u.sendPWMValue = 0;
      sprintf(data, "%d", (int)uart__status_u.Pwm_value);
      UART__StartDmaTransmision(data, " PWM value", 12,"P");
    }

   }
  else
  {
    SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
  }

}


/******************************* END FUNCTION *********************************/

void UART__SetIntTempToSend(void)
{
  uart__status_u.sendInternalTemp = 1;
}

/******************************* END FUNCTION *********************************/

void UART__SetSharp1ToSend(void)
{
  uart__status_u.sendSharp1 = 1;
}

/******************************* END FUNCTION *********************************/

void UART__SetSharpLewyPrzodToSend(void)
{
  uart__status_u.sendSharpPrzodLewy = 1;
}

/******************************* END FUNCTION *********************************/

void UART__SetSharpPrawyPrzodToSend(void)
{
  uart__status_u.sendSharpPrzodPrawy = 1;
}

/******************************* END FUNCTION *********************************/

void UART__SetSharpSrodekTylToSend(void)
{
  uart__status_u.sendSharpTylSrodek = 1;
}
/******************************* END FUNCTION *********************************/

void UART__SetVrefToSend(void)
{
  uart__status_u.sendVrefValue = 1;
}

/******************************* END FUNCTION *********************************/




#ifdef __cplusplus
}
#endif

