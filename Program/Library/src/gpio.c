/** \file gpio.c
* \brief This file contains definitions of all local and interface functions
* which configure gpio.
* \author Lukasz Radola
* \date 26.05.15
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
#include "gpio.h"

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


/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */
/* ... */

/* Static functions definitions */


/* Local exported functions */
/* ... */

/* Interface (exported) functions */

void GPIO__Init(void)
{
/* (1) Enable the peripheral clock of GPIOA and GPIOB */
  /* (2) Select output mode (01) on GPIOA pin 5 */
  /* (3) Select output mode (01) on GPIOB pin 4 */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN; /* (1) */
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE5))
			   | (GPIO_MODER_MODE5_0); /* (2) */
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE4))
			   | (GPIO_MODER_MODE4_0); /* (3) */
}

/******************************* END FUNCTION *********************************/

void GPIO__ConfigUART(uint8_t state)
{
	if(state)
	{

	}
	else
	{

	}
}

/******************************* END FUNCTION *********************************/

void GPIO__ConfigLedR(uint8_t state)
{ 
	//GPIO__OUTPUT_PP(LED_R, state ? 0 : 1, GPIO__OPT_NONE);
}

/******************************* END FUNCTION *********************************/

void GPIO__ConfigLedG(uint8_t state)
{ 
	//GPIO__OUTPUT_PP(LED_G, state ? 0 : 1, GPIO__OPT_NONE);
}

/******************************* END FUNCTION *********************************/

void GPIO__ConfigLedB(uint8_t state)
{ 
	//GPIO__OUTPUT_PP(LED_B, state ? 0 : 1, GPIO__OPT_NONE);
}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif
