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

  GPIO__ConfigSharpEspEnable(0);
  GPIO__ConfigMotorsEnable(0);
  GPIO__ConfigRaspbEnable(0);
}

/******************************* END FUNCTION *********************************/

void GPIO__ConfigUART(uint8_t state)
{
	if(state)
	{
	  /* Enable the peripheral clock of GPIOA */
	     RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	     /* GPIO configuration for USART2 signals */
	     /* (1) Select AF mode (10) on PA2 and PA3 */
	     /* (2) AF4 for USART2 signals */
	     GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE2|GPIO_MODER_MODE3))\
	                    | (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1); /* (1) */
	     GPIOA->AFR[0] = (GPIOA->AFR[0] &~ (0x0000FF00))\
	                     | (4 << (2 * 4)) | (4 << (3 * 4)); /* (2) */
	}
	else
	{


	}
}

/******************************* END FUNCTION *********************************/

void GPIO__ConfigUART_RASPB(uint8_t state)
{
  if(state)
  {

       /* Enable the peripheral clock of GPIOA */
       RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

       /* GPIO configuration for USART1 signals */
       /* (1) Select AF mode (10) on PA9 and PA10 */
       /* (2) AF4 for USART1 signals */
       GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE9|GPIO_MODER_MODE10))\
                      | (GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1); /* (1) */
       GPIOA->AFR[1] = (GPIOA->AFR[1] &~ (0x00000FF0))\
                       | (4 << (1 * 4)) | (4 << (2 * 4)); /* (2) */
  }
  else
  {


  }
}

/******************************* END FUNCTION *********************************/

void GPIO__ConfigSPI_RASPB(uint8_t state)
{
  if(state)
  {
    /* Enable the peripheral clock of GPIOA and GPIOB */
     RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
     RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

     /* (1) Select AF mode (10) on PA4, PA6, PA7 */
     /* (2) AF0 for SPI1 signals */
     /* (3) Select AF mode (10) on PB3 */
     /* (4) AF0 for SPI1 signals */
     GPIOA->MODER = (GPIOA->MODER
                     & ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7))\
                     | (GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1); /* (1) */
     GPIOA->AFR[0] = (GPIOA->AFR[0] & \
                      ~((0xF<<(4*6)) | ((uint32_t)0xF<<(4*7)))); /* (2) */
      GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE3)) | GPIO_MODER_MODE3_1; /* (3) */
      GPIOB->AFR[0] = (GPIOB->AFR[0] & ~((0xF<<(4*3)))); /* (4) */
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
/**
  * Brief   This function enables the peripheral clocks on GPIO port A,
  *         configures the EXTI register and NVIC IRQ.
  *         PA0 is set in input,no pull-up, no pull-down
  *         SYSCFG_EXTICR1 is kept at its reset value to select Port A for EXTI0
  * Param   None
  * Retval  None
  */
void GPIO__ConfigButton(uint8_t state)
{
  /* Enable the peripheral clock of GPIOC */

     RCC->IOPENR |= (RCC_IOPENR_GPIOCEN);

     /** @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
       * @note   After reset, the peripheral clock (used for registers read/write access)
       *         is disabled and the application software has to enable this clock before
       *         using it.
       */

      /* Select mode */
      /* Select input mode (00) on PC13 */
      GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODE13));

      /* Configure Syscfg, exti and nvic for pushbutton PA0 */
      /* (1) PC as source input */
      /* (2) Unmask pin 13 */
      /* (3) Rising edge */
      /* (4) Set priority */
      /* (5) Enable EXTI0_1_IRQn */
      SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~SYSCFG_EXTICR4_EXTI13) | SYSCFG_EXTICR4_EXTI13_PC; /* (1) */
      EXTI->IMR |= EXTI_IMR_IM13; /* (2) */
      EXTI->RTSR |= EXTI_RTSR_TR13; /* (3) */
      //EXTI->FTSR |= EXTI_RTSR_TR13;
      NVIC_SetPriority(EXTI4_15_IRQn, 0); /* (4) */
      NVIC_EnableIRQ(EXTI4_15_IRQn); /* (5) */
}

void GPIO__ConfigEnkoders(uint8_t state)
{
  /* Enable the peripheral clock of GPIOC */

     RCC->IOPENR |= (RCC_IOPENR_GPIOCEN);

     /** @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
       * @note   After reset, the peripheral clock (used for registers read/write access)
       *         is disabled and the application software has to enable this clock before
       *         using it.
       */


       /* (2) Select output mode (01) on GPIOC pin 11 */
       GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODE11))
              | (GPIO_MODER_MODE11_0); /* (2) */

      /* Select mode */
      /* Select input mode (00) on PC6 */
      GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODE6));

      /* Configure Syscfg, exti and nvic for pushbutton PA0 */
      /* (1) PC as source input */
      /* (2) Unmask pin 13 */
      /* (3) Rising edge */
      /* (4) Set priority */
      /* (5) Enable EXTI0_1_IRQn */
      SYSCFG->EXTICR[1] = (SYSCFG->EXTICR[1] & ~SYSCFG_EXTICR2_EXTI6) | SYSCFG_EXTICR2_EXTI6_PC; /* (1) */
      EXTI->IMR |= EXTI_IMR_IM6; /* (2) */
      EXTI->RTSR |= EXTI_RTSR_TR6; /* (3) */
      //EXTI->FTSR |= EXTI_RTSR_TR10;
      NVIC_SetPriority(EXTI4_15_IRQn, 0); /* (4) */
      NVIC_EnableIRQ(EXTI4_15_IRQn); /* (5) */
}

void GPIO__ConfigSharpEspEnable(uint8_t state)
{
	/* (1) Enable the peripheral clock of GPIOC */
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN; /* (1) */
	 /* (2) Select output mode (01) on GPIOC pin 9 */
	GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODE9))
				   | (GPIO_MODER_MODE9_0); /* (2) */
	/* (3) Set open drain mode */
			  GPIOC->OTYPER |= GPIO_OTYPER_OT_9; // (3)

	  if(state)
	  {
		  GPIOC->BRR = (1<<9);
	  }
	  else
	  {
		  GPIOC->BSRR = (1<<9);
	  }

}

void GPIO__ConfigMotorsEnable(uint8_t state)
{
	/* (1) Enable the peripheral clock of GPIOC */
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN; /* (1) */
	 /* (2) Select output mode (01) on GPIOC pin 9 */
	GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE1))
				   | (GPIO_MODER_MODE1_0); /* (2) */
	/* (3) Set open drain mode */
			  GPIOB->OTYPER |= GPIO_OTYPER_OT_1; // (3)

	  if(state)
	  {
		  GPIOB->BRR = (1<<1);
	  }
	  else
	  {
		  GPIOB->BSRR = (1<<1);
	  }

}

void GPIO__ConfigRaspbEnable(uint8_t state)
{
	/* (1) Enable the peripheral clock of GPIOC */
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN; /* (1) */
	 /* (2) Select output mode (01) on GPIOC pin 9 */
	GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODE14))
				   | (GPIO_MODER_MODE14_0); /* (2) */
	/* (3) Set open drain mode */
			  GPIOC->OTYPER |= GPIO_OTYPER_OT_14; // (3)

	  if(state)
	  {
		  GPIOC->BRR = (1<<14);
	  }
	  else
	  {
		  GPIOC->BSRR = (1<<14);
	  }

}
#ifdef __cplusplus
  }
#endif
