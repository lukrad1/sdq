/** \file uart_raspb.c
 * \brief file contains uart engine and all the most important uart
 * function for raspberry pi comunication.
 *
 * \author Lukasz Radola
 * \date 08.05.16
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
#include "uart_raspb.h"
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
static volatile union uart__status_raspb_u
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
  }
}
uart__status_raspb_u = {0}; /* Uart__status_u union declaraction. The variable name
                         is this same: uart__status_u. */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

volatile uint8_t raspb_stringtosend[20];
volatile uint8_t raspb_stringtoreceive[1];
volatile uint8_t raspb_RxBuffer[10];
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */

void UART_RASPB__Init(uint32_t baudrate)
{
  GPIO__ConfigUART_RASPB(1);

  /* Enable the peripheral clock USART1 */
   RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

   /* Configure USART1 */
   /* (1)  oversampling by 16, 9600 baud = USARTDIV = clk/9600, BRR = USARTDIV*/
   /* (2) Enable DMA in reception and transmission */
   /* (3) 8 data bit, 1 start bit, 1 stop bit, no parity, reception and transmission enabled */
   USART1->BRR = MAIN__F_CPU / baudrate; /* (1) */
   USART1->CR3 = USART_CR3_DMAT | USART_CR3_DMAR; /* (2) */
   USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; /* (3) */

   while((USART1->ISR & USART_ISR_TC) != USART_ISR_TC)/* polling idle frame Transmission */
   {
     /* add time out here for a robust application */
   }
   USART1->ICR = USART_ICR_TCCF;/* Clear TC flag */
}


/******************************* END FUNCTION *********************************/

void UART_RASPB__DeInit(void)
{
  /* Disable the peripheral clock USART2 */
  RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
  GPIO__ConfigUART_RASPB(0);
}


/******************************* END FUNCTION *********************************/

void UART_RASPB__DMAConfig(void)
{
   /* Enable the peripheral clock DMA1 */
     RCC->AHBENR |= RCC_AHBENR_DMA1EN;

     /* DMA1 Channel2 USART1_TX config */
     /* (1)  Map USART1_TX DMA channel */
     /* (2)  Peripheral address */
     /* (3)  Memory address */
     /* (4)  Memory increment */
     /*      Memory to peripheral */
     /*      8-bit transfer */
     /*      Transfer complete IT */
     DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C2S) | (3 << (1 * 4)); /* (1) */
     DMA1_Channel2->CPAR = (uint32_t)&(USART1->TDR); /* (2) */
     DMA1_Channel2->CMAR = (uint32_t)raspb_stringtosend; /* (3) */
     DMA1_Channel2->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE; /* (4) */

     /* DMA1 Channel3 USART_RX config */
     /* (5)  Map USART1_RX DMA channel */
     /* (6)  Peripheral address */
     /* (7)  Memory address */
     /* (8)  Data size */
     /* (9)  Memory increment */
     /*      Peripheral to memory*/
     /*      8-bit transfer */
     /*      Transfer complete IT */
     DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C3S) | (3 << (2 * 4)); /* (5) */
     DMA1_Channel3->CPAR = (uint32_t)&(USART1->RDR); /* (6) */
     DMA1_Channel3->CMAR = (uint32_t)raspb_stringtoreceive; /* (7) */
     DMA1_Channel3->CNDTR = 1; /* (8) */
     DMA1_Channel3->CCR = DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN; /* (9) */

     /* Configure IT */
     /* (10) Set priority for DMA1_Channel2_3_IRQn */
     /* (11) Enable DMA1_Channel2_3_IRQn */
     NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0); /* (10) */
     NVIC_EnableIRQ(DMA1_Channel2_3_IRQn); /* (11) */

}

void UART_RASPB__StartDmaTransmision(int8_t* data, int8_t* additional_text,
                               uint8_t length, int8_t* message)
{
  int i,j,h = 0;
  SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART_RASPB);

  length += 4; // na znak konca lini i bit startu
  if(length > 20)
  {
    length = 20;
  }
  raspb_stringtosend[0] = 0x02; // start byte
  raspb_stringtosend[1] = message[0]; // T
  for(i = 2; i < (length - 2); i++)
  {
    raspb_stringtosend[i] = data[i-2];
    if(data[i-2] == 0)
    {
      for(j = i; j < (length - 2); j++)
      {
        raspb_stringtosend[j] = additional_text[h];
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
  raspb_stringtosend[length-2] = 10;
  raspb_stringtosend[length-1] = 13; // czy tu nie powinno byc 13????
  /* start 8-bit transmission with DMA */
  DMA1_Channel2->CCR &=~ DMA_CCR_EN;
  DMA1_Channel2->CNDTR = length-1;/* Data size */
  DMA1_Channel2->CCR |= DMA_CCR_EN;

  uart__status_raspb_u.txBusyFlag = 1;
}

void UART_RASPB__RxInterrupt(void)
{
  static uint8_t index = 0;

  if(!uart__status_raspb_u.receivedData)
  {
    if(raspb_stringtoreceive[0] == 0x02)// bit startu
    {
      index = 0;
    }
    else if(index < 4)
    {
      raspb_RxBuffer[index] = raspb_stringtoreceive[0];
      raspb_stringtoreceive[0] = 0;

      if(raspb_RxBuffer[index] == 10 && raspb_RxBuffer[index-1] == 13) // cr + lf
      {
        index = 4; // koniec transmisji i blokada, czekanie na bit startu
        SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART_RASPB);
        uart__status_raspb_u.receivedData = 1;
      }

      index++;
    }
  }
}

void UART_RASPB__TxInterrupt(void)
{
  //kasowanie flagi zajetosci, bo wszystko bylo w dma, i gdy wszedlem do
  // przerwaniato juz mam po transmisji, odebralem wszystkie dane
  uart__status_raspb_u.txBusyFlag = 0;
  SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_UART_RASPB);
}

void UART_RASPB__Poll(void)
{
  int8_t data[4];

  if(uart__status_raspb_u.receivedData)
  {
	SYSTEM__SetEspTimeoutValue(60000); // timeout na 60s
    if(!ADC__GetIsObstacleFlag())
    {
      uart__status_raspb_u.Pwm_value = 80;
      TIMER__PWM_DC1_2_Change_Duty(uart__status_raspb_u.Pwm_value);
      //uart__status_u.sendPWMValue = 1;
    }

    if(raspb_RxBuffer[0] == 49 && raspb_RxBuffer[1] == 49)
    {
      MOTORS__jazda_do_tylu();
      MOTORS__SetLastDirection(JAZDA_DO_TYLU);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(raspb_RxBuffer[0] == 49 && raspb_RxBuffer[1] == 48)
    {
      MOTORS__skret_w_lewo();
      MOTORS__SetLastDirection(JAZDA_W_LEWO);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(raspb_RxBuffer[0] == 50 && raspb_RxBuffer[1] == 50)
    {
      MOTORS__jazda_zatrzymana();
      MOTORS__SetLastDirection(JAZDA_ZATRZYMANA);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(raspb_RxBuffer[0] == 50 && raspb_RxBuffer[1] == 48)
    {
      MOTORS__skret_w_prawo();
      MOTORS__SetLastDirection(JAZDA_W_PRAWO);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(raspb_RxBuffer[0] == 50 && raspb_RxBuffer[1] == 53)
    {
      MOTORS__jazda_do_przodu();
      MOTORS__SetLastDirection(JAZDA_DO_PRZODU);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(raspb_RxBuffer[0] == 37)
    {
      if(!ADC__GetIsObstacleFlag())
      {
        uart__status_raspb_u.Pwm_value = raspb_RxBuffer[1];
        TIMER__PWM_DC1_2_Change_Duty(uart__status_raspb_u.Pwm_value);
        //uart__status_u.sendPWMValue = 1;
      }

    }

    uart__status_raspb_u.receivedData = 0;
    // jezeli bedzie cos do wyslania, to ponizej te flage wyzeruje w funkcji start dma
    SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_UART_RASPB);
  }

  if(!uart__status_raspb_u.txBusyFlag)
  {
    if(uart__status_raspb_u.sendInternalTemp)
    {

      uart__status_raspb_u.sendInternalTemp = 0;

      sprintf(data, "%d", (int)ADC__GetTempDegreeValue());
      UART_RASPB__StartDmaTransmision(data, "", 2,"T");

    }
    else if(uart__status_raspb_u.sendSharp1)
    {

      uart__status_raspb_u.sendSharp1 = 0;

      sprintf(data, "%d", (int)ADC__GetSharp1MvValue());
      UART_RASPB__StartDmaTransmision(data,"", 0,"B");

    }
    else if(uart__status_raspb_u.sendSharpPrzodLewy)
    {

      uart__status_raspb_u.sendSharpPrzodLewy = 0;

      sprintf(data, "%d", (int)ADC__GetSharpPrzodLewyMvValue());
      UART_RASPB__StartDmaTransmision(data,"", 0,"A");

    }
    else if(uart__status_raspb_u.sendSharpPrzodPrawy)
    {

      uart__status_raspb_u.sendSharpPrzodPrawy = 0;

      sprintf(data, "%d", (int)ADC__GetSharpPrzodPrawyMvValue());
      UART_RASPB__StartDmaTransmision(data,"", 0,"C");

    }
    else if(uart__status_raspb_u.sendSharpTylSrodek)
    {

      uart__status_raspb_u.sendSharpTylSrodek = 0;

      sprintf(data, "%d", (int)ADC__GetSharpTylSrodekMvValue());
      UART_RASPB__StartDmaTransmision(data,"", 0,"E");

    }
    else if(uart__status_raspb_u.sendVrefValue)
    {
      uart__status_raspb_u.sendVrefValue = 0;

      sprintf(data, "%d", (int)ADC__GetVrefAdcValue());
      UART_RASPB__StartDmaTransmision(data," Vref", 9,"V");

    }
    else if(uart__status_raspb_u.sendPWMValue)
    {
      uart__status_raspb_u.sendPWMValue = 0;
      sprintf(data, "%d", (int)uart__status_raspb_u.Pwm_value);
      UART_RASPB__StartDmaTransmision(data, " PWM value", 12,"P");
    }

   }
  else
  {
    SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
  }

}


/******************************* END FUNCTION *********************************/

void UART_RASPB__SetIntTempToSend(void)
{
  uart__status_raspb_u.sendInternalTemp = 1;
}

/******************************* END FUNCTION *********************************/

void UART_RASPB__SetSharp1ToSend(void)
{
  uart__status_raspb_u.sendSharp1 = 1;
}

/******************************* END FUNCTION *********************************/

void UART_RASPB__SetSharpLewyPrzodToSend(void)
{
  uart__status_raspb_u.sendSharpPrzodLewy = 1;
}

/******************************* END FUNCTION *********************************/

void UART_RASPB__SetSharpPrawyPrzodToSend(void)
{
  uart__status_raspb_u.sendSharpPrzodPrawy = 1;
}

/******************************* END FUNCTION *********************************/

void UART_RASPB__SetSharpSrodekTylToSend(void)
{
  uart__status_raspb_u.sendSharpTylSrodek = 1;
}
/******************************* END FUNCTION *********************************/

void UART_RASPB__SetVrefToSend(void)
{
  uart__status_raspb_u.sendVrefValue = 1;
}

/******************************* END FUNCTION *********************************/


#ifdef __cplusplus
}
#endif

