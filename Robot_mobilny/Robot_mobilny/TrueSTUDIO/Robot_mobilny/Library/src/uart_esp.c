/** \file uart_esp.c
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
#include "uart_esp.h"
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
static volatile union uart_esp__status_u
{
  uint16_t status; /* status variable, which is used to clear all uart status flag */
  struct
  {
    uint8_t txBusyFlag: 1;
    uint8_t rxBusyFlag: 1;
    uint8_t receivedData: 1;
    uint8_t rst_esp: 1;
    uint8_t setCwmode: 1;
    uint8_t setCipmode: 1;
    uint8_t setMultiConnection: 1;
    uint8_t startEspServer : 1;

    uint8_t startEspData : 1;
    uint8_t sendEspData : 1;
    uint8_t sendEspDataPayload : 1;
    uint8_t finishEspData : 1;
    uint8_t fullTransmision: 1;
    uint8_t isEspReset: 1;
    uint8_t Pwm_value;
  };
}
uart_esp__status_u = {0}; /* Uart__status_u union declaraction. The variable name
                         is this same: uart__status_u. */

static volatile union uart_esp__init_u
{
  uint8_t status; /* status variable, which is used to clear all uart status flag */
  struct
  {
    uint8_t esp_is_wakeup: 1;
    uint8_t esp_finish_transmision: 1;
    uint8_t esp_start_init: 1;
    uint8_t esp_transmision_timeout: 1;
    uint8_t raspberry_is_run: 1;
    uint8_t flag5: 1;
    uint8_t flag6: 1;
    uint8_t flag7 : 1;

  };
}
uart_esp__init_u = {0}; /* Uart__status_u union declaraction. The variable name
                         is this same: uart__status_u. */
/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

volatile uint8_t stringtosend_esp[90];
volatile uint8_t stringtoreceive_esp[1];
volatile uint8_t RxBuffer_esp[10];
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */

void UART_ESP__Init(uint32_t baudrate)
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

void UART_ESP__DeInit(void)
{
  /* Disable the peripheral clock USART2 */
  RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
  GPIO__ConfigUART(0);
}


/******************************* END FUNCTION *********************************/

void UART_ESP__DMAConfig(void)
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
   DMA1_Channel4->CMAR = (uint32_t)stringtosend_esp; /* (3) */
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
   DMA1_Channel5->CMAR = (uint32_t)stringtoreceive_esp; /* (7) */
   DMA1_Channel5->CNDTR = 1; /* (8) */
   DMA1_Channel5->CCR = DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN; /* (9) */

   /* Configure IT */
   /* (10) Set priority for DMA1_Channel2_3_IRQn */
   /* (11) Enable DMA1_Channel2_3_IRQn */
   NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 1); /* (10) */
   NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn); /* (11) */

}

void UART_ESP__StartDmaTransmision(int8_t* data, int8_t* additional_text,
                               uint8_t length)
{
  int i,j,h = 0;
  SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);

  //length += 4; // na znak konca lini i bit startu

  for(i = 0; i < 90;i++)
  {
    stringtosend_esp[i] = 0;
  }
  i = 0;

  if(length > 90)
  {
    length = 90;
  }
  //stringtosend_esp[0] = 0x02; // start byte
  //stringtosend_esp[1] = message[0]; // T
  for(i = 0; i < length; i++)
  {
    stringtosend_esp[i] = data[i];
    if(data[i] == 0)
    {
      for(j = i; j < length; j++)
      {
        stringtosend_esp[j] = additional_text[h];
        if(additional_text[h] == 0)
        {
          length = j+1;
          j = 91;
        }
        h++;
      }
      // wyjdz z fora i jednoczesnie miej miejsce na lf
      i = 91;
    }
  }
  //stringtosend_esp[length-2] = 10;
  //stringtosend_esp[length-1] = 12; // czy tu nie powinno byc 13????
  /* start 8-bit transmission with DMA */
  DMA1_Channel4->CCR &=~ DMA_CCR_EN;
  DMA1_Channel4->CNDTR = length;/* Data size */
  DMA1_Channel4->CCR |= DMA_CCR_EN;
  uart_esp__status_u.txBusyFlag = 1;
}

void UART_ESP__RxInterrupt(void)
{
  static uint8_t index = 0;
  static uint8_t http_message = 0;
  static uint8_t timeout = 0;

  if(!uart_esp__status_u.receivedData)
  {
    if(stringtoreceive_esp[0] == 0x4F && (!uart_esp__status_u.isEspReset))// duze O
    {
      index = 0;
      RxBuffer_esp[index] = stringtoreceive_esp[0];
      stringtoreceive_esp[0] = 0;
      index++;
    }
    else if(stringtoreceive_esp[0] == 0x4B && RxBuffer_esp[0] == 0x4F) //duze K
    {
      RxBuffer_esp[index] = stringtoreceive_esp[0];
      stringtoreceive_esp[0] = 0;
      index = 0; // koniec transmisji i blokada, czekanie na bit startu
      SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
      uart_esp__status_u.receivedData = 1;
    }
    else if(stringtoreceive_esp[0] == 0x2B && index == 0) // odebrano +
    {
      RxBuffer_esp[index] = stringtoreceive_esp[0];
      stringtoreceive_esp[0] = 0;
      http_message = 1;
      index++;
    }
    else if(http_message)
    {
      if(stringtoreceive_esp[0] == 0x77) // odebrano w
      {
        RxBuffer_esp[index] = stringtoreceive_esp[0];
        stringtoreceive_esp[0] = 0;
        index++;
      }
      else if(stringtoreceive_esp[0] == 0x79 && RxBuffer_esp[1] == 0x77) // odebrano y
      {
        RxBuffer_esp[index] = stringtoreceive_esp[0];
        stringtoreceive_esp[0] = 0;
        index++;
      }
      else if(stringtoreceive_esp[0] == 0x62 && RxBuffer_esp[2] == 0x79) // odebrano b
      {
        RxBuffer_esp[index] = stringtoreceive_esp[0];
        stringtoreceive_esp[0] = 0;
        index++;
      }
      else if(stringtoreceive_esp[0] == 0x75 && RxBuffer_esp[3] == 0x62) // odebrano u
      {
        RxBuffer_esp[index] = stringtoreceive_esp[0];
        stringtoreceive_esp[0] = 0;
        index++;
      }
      else if(stringtoreceive_esp[0] == 0x64 && RxBuffer_esp[4] == 0x75) // odebrano d
      {
        RxBuffer_esp[index] = stringtoreceive_esp[0];
        stringtoreceive_esp[0] = 0;
        index++;
      }
      else if(stringtoreceive_esp[0] == 0x7A && RxBuffer_esp[5] == 0x64) // odebrano z
      {
        RxBuffer_esp[index] = stringtoreceive_esp[0];
        stringtoreceive_esp[0] = 0;
        index = 0; // koniec transmisji i blokada, czekanie na bit startu
        http_message = 0;
        SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
        uart_esp__status_u.receivedData = 1;
        timeout = 0;
      }
      else if(timeout > 20)
      {
        timeout = 0;
        http_message = 0;
        index = 0;
      }
      else
      {
        timeout++;
      }
    }
    else
    {
      index = 0;

    }
  }
}

void UART_ESP__TxInterrupt(void)
{
  //kasowanie flagi zajetosci, bo wszystko bylo w dma, i gdy wszedlem do
  // przerwaniato juz mam po transmisji, odebralem wszystkie dane
  //uart_esp__status_u.txBusyFlag = 0;
  SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
}

void UART_ESP__Poll(void)
{
  static uint32_t timeout = 0;
  static uint32_t run_server_timeout = 0xFFFF;
  static uint8_t all_data_is_send = 0;

  if(UART_ESP__GetStartInit() && !all_data_is_send)
  {
	  all_data_is_send = 1;
	  UART_ESP__SendAllData();
  }
  else if(!UART_ESP__GetStartInit() && all_data_is_send)
  {
	  all_data_is_send = 0;
  }

  if(MOTORS__GetCurrentDirection() != JAZDA_ZATRZYMANA)
  {
	  SYSTEM__SetEspTimeoutValue(80000); // timeout na 80s
  }

  if(uart_esp__status_u.receivedData)
  {

    if(RxBuffer_esp[0] == 0x4F && RxBuffer_esp[1] == 0x4B) // odebrano OK
    {
      uart_esp__status_u.txBusyFlag = 0;
    }
    //+wybudz
    else if(RxBuffer_esp[0] == 0x2B && RxBuffer_esp[1] == 0x77 && RxBuffer_esp[2] == 0x79
            && RxBuffer_esp[3] == 0x62 && RxBuffer_esp[4] == 0x75 && RxBuffer_esp[5] == 0x64
            && RxBuffer_esp[6] == 0x7A)
    {

      if(!UART_ESP__GetRaspberry_Run_Flag())
      {
		  //turn on raspberry and motors
		  GPIO__ConfigRaspbEnable(1);
		  GPIO__ConfigMotorsEnable(1);
		  SYSTEM__SetEspTimeoutValue(120000); // timeout na 120s
		  uart_esp__status_u.sendEspData = 1;
		  uart_esp__init_u.raspberry_is_run = 1;
      }
    }
    
    
    uart_esp__status_u.receivedData = 0;
    // jezeli bedzie cos do wyslania, to ponizej te flage wyzeruje w funkcji start dma
    SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
  }

  if(!uart_esp__status_u.txBusyFlag || timeout >= 30000) //xxxx cykli procesora
  {
    timeout = 0;

    if(uart_esp__status_u.rst_esp)
    {
      uart_esp__status_u.rst_esp = 0;
      run_server_timeout = 0;
      uart_esp__status_u.isEspReset = 1;
      UART_ESP__StartDmaTransmision("AT+", "RST\r\n", 8);
      uart_esp__status_u.setCwmode = 1;
    }
    else if(uart_esp__status_u.setCwmode)
    {
      uart_esp__status_u.setCwmode = 0;
      uart_esp__status_u.isEspReset = 0;
      run_server_timeout = 0;
      UART_ESP__StartDmaTransmision("AT+", "CWMODE=1\r\n", 13);
      uart_esp__status_u.setCipmode = 1;
    }
    else if(uart_esp__status_u.setCipmode)
    {
      uart_esp__status_u.setCipmode = 0;
      run_server_timeout = 0;
      UART_ESP__StartDmaTransmision("AT+", "CIPMODE=0\r\n", 14);
      uart_esp__status_u.setMultiConnection = 1;
    }
    else if(uart_esp__status_u.setMultiConnection)
    {
      uart_esp__status_u.setMultiConnection = 0;
      run_server_timeout = 0;
      UART_ESP__StartDmaTransmision("AT+", "CIPMUX=1\r\n", 13);
      uart_esp__status_u.startEspServer = 1;
    }

    else if(uart_esp__status_u.startEspServer)
    {
      uart_esp__status_u.startEspServer = 0;
      run_server_timeout = 0;
      UART_ESP__StartDmaTransmision("AT+", "CIPSERVER=1,5020\r\n", 21);
      run_server_timeout = 0;
    }
    else if(uart_esp__status_u.startEspData)
    {
      uart_esp__status_u.startEspData = 0;
      run_server_timeout = 0;
      UART_ESP__StartDmaTransmision("AT+", "CIPSTART=\"TCP\",\"www.lukrad1.cba.pl\",80\r\n", 43);
    }
    else if(uart_esp__status_u.sendEspData)
    {
      uart_esp__status_u.sendEspData = 0;
      run_server_timeout = 0;
      uart_esp__status_u.isEspReset = 1;
      timeout = 25000;
      UART_ESP__StartDmaTransmision("AT+", "CIPSEND=0,8\r\n", 16);
      uart_esp__status_u.sendEspDataPayload = 1;
    }
    else if(uart_esp__status_u.sendEspDataPayload)
    {
      uart_esp__status_u.sendEspDataPayload = 0;
      run_server_timeout = 0;
      UART_ESP__StartDmaTransmision("ODEBRAN","O", 8);
      uart_esp__status_u.isEspReset = 0;
      uart_esp__status_u.finishEspData = 1;
    }
    else if(uart_esp__status_u.finishEspData)
    {
      uart_esp__status_u.finishEspData = 0;
      run_server_timeout = 0;
      UART_ESP__StartDmaTransmision("AT+","CIPCLOSE=0\r\n", 15);
    }

  }
  else
  {
    timeout++;
    SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
  }

  if(run_server_timeout < 35000)
  {
    run_server_timeout++;
  }
  else if (run_server_timeout >= 35000 && run_server_timeout != 0xFFFF)
  {
    //uart_esp__status_u.rst_esp = 1;
    //wylacz serwer wifi
    if(!uart_esp__status_u.txBusyFlag) //xxxx cykli procesora
    {
      UART_ESP__StartDmaTransmision("", "\r\n", 2);
    }
    run_server_timeout = 0xFFFF;
  }
}



/******************************* END FUNCTION *********************************/

void UART_ESP__SendAllData(void)
{
  //uart_esp__status_u.rst_esp = 1;
  uart_esp__status_u.setCwmode = 1;
  //uart_esp__status_u.setCipmode = 1;
  //uart_esp__status_u.setMultiConnection = 1;
  //uart_esp__status_u.startEspServer = 1;
  //uart_esp__status_u.sendEspData = 1;
  //uart_esp__status_u.sendEspDataPayload = 1;
  //uart_esp__status_u.finishEspData = 1;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__SetIsWakeupFlag(void)
{
	uart_esp__init_u.esp_is_wakeup = 1;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__ClearIsWakeupFlag(void)
{
	uart_esp__init_u.esp_is_wakeup = 0;
}
/******************************* END FUNCTION *********************************/

uint8_t UART_ESP__GetIsWakeupFlag(void)
{
	return uart_esp__init_u.esp_is_wakeup;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__SetFinishTransmisionFlag(void)
{
	uart_esp__init_u.esp_finish_transmision = 1;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__ClearFinishTransmisionFlag(void)
{
	uart_esp__init_u.esp_finish_transmision = 0;
}

/******************************* END FUNCTION *********************************/

uint8_t UART_ESP__GetFinishTransmisionFlag(void)
{
	return uart_esp__init_u.esp_finish_transmision;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__SetStartInit(void)
{
	uart_esp__init_u.esp_start_init = 1;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__ClearStartInit(void)
{
	uart_esp__init_u.esp_start_init = 0;
}

/******************************* END FUNCTION *********************************/

uint8_t UART_ESP__GetStartInit(void)
{
	return uart_esp__init_u.esp_start_init;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__SetTimeout(void)
{
	uart_esp__init_u.esp_start_init = 1;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__ClearTimeout(void)
{
	uart_esp__init_u.esp_start_init = 0;
}

/******************************* END FUNCTION *********************************/

uint8_t UART_ESP__GetTimeout(void)
{
	return uart_esp__init_u.esp_start_init;
}

/******************************* END FUNCTION *********************************/

void UART_ESP__ClearRaspberry_Run_Flag(void)
{
	uart_esp__init_u.raspberry_is_run = 0;
}

/******************************* END FUNCTION *********************************/

uint8_t UART_ESP__GetRaspberry_Run_Flag(void)
{
	return uart_esp__init_u.raspberry_is_run;
}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
}
#endif

