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
#include "spi_raspb.h"
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
static volatile union spi_raspb__status_u
{
  uint8_t status; /* status variable, which is used to clear all uart status flag */
  struct
  {
    uint8_t txBusyFlag: 1;
    uint8_t rxBusyFlag: 1;
    uint8_t receivedData: 1;
    uint8_t flag3: 1;
    uint8_t flag4: 1;
    uint8_t fullTransmision: 1;
    uint8_t flag6: 1;
    uint8_t flag7;
  };
}
spi_raspb__status_u = {0}; /* Uart__status_u union declaraction. The variable name
                         is this same: uart__status_u. */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

volatile uint8_t spi_raspb__stringtosend[20];
volatile uint8_t spi_raspb__stringtoreceive[1];
volatile uint8_t spi_raspb__RxBuffer[10];
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */

void SPI_RASPB__Init(uint32_t baudrate)
{
  GPIO__ConfigSPI_RASPB();
  /* Enable the peripheral clock SPI1 */
   RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

   /* Configure SPI1 in master */
   /* (1) Master selection, BR: Fpclk/256 (due to C13 on the board, SPI_CLK is set to the minimum)
          CPOL and CPHA at zero (rising first edge), 8-bit data frame */
   /* (2) Slave select output enabled, DMA in reception and transmission */
   /* (3) Enable SPI1 */
   SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR; /* (1) */
   SPI1->CR2 = SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN | SPI_CR2_SSOE; /* (2) */
   SPI1->CR1 |= SPI_CR1_SPE; /* (3) */
}


/******************************* END FUNCTION *********************************/

void SPI_RASPB__DeInit(void)
{

}


/******************************* END FUNCTION *********************************/

void SPI_RASPB__DMAConfig(void)
{
  /* Enable the peripheral clock DMA11 */
   RCC->AHBENR |= RCC_AHBENR_DMA1EN;

   /* DMA1 Channel2 SPI1_RX config */
   /* (1)  Map SPI1_RX DMA channel */
   /* (2) Peripheral address */
   /* (3) Memory address */
   /* (4) Data size */
   /* (5) Memory increment */
   /*     Peripheral to memory */
   /*     8-bit transfer */
   /*     Transfer complete IT */
   DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C2S) | (1 << (1 * 4)); /* (1) */
   DMA1_Channel2->CPAR = (uint32_t)&(SPI1->DR); /* (2) */
   spi_raspb__stringtoreceive = malloc(sizeof(spi_raspb__stringtosend));
   DMA1_Channel2->CMAR = (uint32_t)spi_raspb__stringtoreceive; /* (3) */
   DMA1_Channel2->CNDTR = sizeof(spi_raspb__stringtosend); /* (4) */
   DMA1_Channel2->CCR |= DMA_CCR_MINC | DMA_CCR_TCIE  | DMA_CCR_EN; /* (5) */

   /* DMA1 Channel3 SPI1_TX config */
   /* (6)  Map SPI1_TX DMA channel */
   /* (7) Peripheral address */
   /* (8) Memory address */
   /* (9) Memory increment */
   /*     Memory to peripheral*/
   /*     8-bit transfer */
   DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C3S) | (1 << (2 * 4)); /* (6) */
   DMA1_Channel3->CPAR = (uint32_t)&(SPI1->DR); /* (7) */
   DMA1_Channel3->CMAR = (uint32_t)spi_raspb__stringtosend; /* (8) */
   DMA1_Channel3->CCR |= DMA_CCR_MINC | DMA_CCR_DIR; /* (9) */

   /* Configure IT */
   /* (10) Set priority for DMA1_Channel2_3_IRQn */
   /* (11) Enable DMA1_Channel2_3_IRQn */
   NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0); /* (10) */
   NVIC_EnableIRQ(DMA1_Channel2_3_IRQn); /* (11) */

}

void SPI_RASPB__StartDmaTransmision(int8_t* data, int8_t* additional_text,
                               uint8_t length, int8_t* message)
{
  int i,j,h = 0;
  SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_SPI);

  length += 4; // na znak konca lini i bit startu
  if(length > 20)
  {
    length = 20;
  }
  spi_raspb__stringtosend[0] = 0x02; // start byte
  spi_raspb__stringtosend[1] = message[0]; // T
  for(i = 2; i < (length - 2); i++)
  {
    spi_raspb__stringtosend[i] = data[i-2];
    if(data[i-2] == 0)
    {
      for(j = i; j < (length - 2); j++)
      {
        spi_raspb__stringtosend[j] = additional_text[h];
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
  spi_raspb__stringtosend[length-2] = 10;
  spi_raspb__stringtosend[length-1] = 12;
  /* start 8-bit transmission with DMA*/
  /* Prepare Slave */
  DMA1_Channel5->CCR &=~ DMA_CCR_EN;
  DMA1_Channel5->CNDTR = sizeof(spi_raspb__stringtosend); /* Data size */
  DMA1_Channel5->CCR |= DMA_CCR_EN;
  /* Prepare master */
  DMA1_Channel3->CCR &=~ DMA_CCR_EN;
  DMA1_Channel3->CNDTR = sizeof(spi_raspb__stringtosend); /* Data size */
  DMA1_Channel3->CCR |= DMA_CCR_EN;

  spi_raspb__status_u.txBusyFlag = 1;
}

void SPI_RASPB__RxInterrupt(void)
{
  static uint8_t index = 0;

  if(!spi_raspb__status_u.receivedData)
  {
    if(spi_raspb__stringtoreceive[0] == 0x02)// bit startu
    {
      index = 0;
    }
    else if(index < 4)
    {
      spi_raspb__RxBuffer[index] = spi_raspb__stringtoreceive[0];
      spi_raspb__stringtoreceive[0] = 0;

      if(spi_raspb__RxBuffer[index] == 10 && spi_raspb__RxBuffer[index-1] == 13) // cr + lf
      {
        index = 4; // koniec transmisji i blokada, czekanie na bit startu
        SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_UART);
        spi_raspb__status_u.receivedData = 1;
      }

      index++;
    }
  }
}

void SPI_RASPB__TxInterrupt(void)
{
  //kasowanie flagi zajetosci, bo wszystko bylo w dma, i gdy wszedlem do
  // przerwaniato juz mam po transmisji, odebralem wszystkie dane
  spi_raspb__status_u.txBusyFlag = 0;
  SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_SPI);
}

void SPI_RASPB__Poll(void)
{
  int8_t data[4];

  if(spi_raspb__status_u.receivedData)
  {

    if(spi_raspb__RxBuffer[0] == 49 && spi_raspb__RxBuffer[1] == 49)
    {
      MOTORS__jazda_do_tylu();
      MOTORS__SetLastDirection(JAZDA_DO_TYLU);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(spi_raspb__RxBuffer[0] == 49 && spi_raspb__RxBuffer[1] == 48)
    {
      MOTORS__skret_w_lewo();
      MOTORS__SetLastDirection(JAZDA_W_LEWO);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(spi_raspb__RxBuffer[0] == 50 && spi_raspb__RxBuffer[1] == 50)
    {
      MOTORS__jazda_zatrzymana();
      MOTORS__SetLastDirection(JAZDA_ZATRZYMANA);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(spi_raspb__RxBuffer[0] == 50 && spi_raspb__RxBuffer[1] == 48)
    {
      MOTORS__skret_w_prawo();
      MOTORS__SetLastDirection(JAZDA_W_PRAWO);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
    else if(spi_raspb__RxBuffer[0] == 50 && spi_raspb__RxBuffer[1] == 53)
    {
      MOTORS__jazda_do_przodu();
      MOTORS__SetLastDirection(JAZDA_DO_PRZODU);
      OBSTACLE__ClearAvoidObstacleIsrFlag();
    }
   // pamietac o pwm na stale

    spi_raspb__status_u.receivedData = 0;
    // jezeli bedzie cos do wyslania, to ponizej te flage wyzeruje w funkcji start dma
    SYSTEM__SetSleepReadyFlag(SYSTEM__SLEEPREADY_SPI);
  }

  if(!spi_raspb__status_u.txBusyFlag)
  {

  }
  else
  {
    SYSTEM__ClearSleepReadyFlag(SYSTEM__SLEEPREADY_SPI);
  }

}


/******************************* END FUNCTION *********************************/



#ifdef __cplusplus
}
#endif

