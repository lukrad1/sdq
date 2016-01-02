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



#if (_MAIN__F_CPU == TIMER__16MHZ_CLK)
{
  /* High speed clock prescaler: 16 */

}
#elif (_MAIN__F_CPU == TIMER__4MHZ_CLK)
{
  /* High speed clock prescaler: 4 */

}
#elif (_MAIN__F_CPU == TIMER__1MHZ_CLK)
{
  /* High speed clock prescaler: 1 */

}
#endif
  
  /* Wait for stabilization of the HSI clock */

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
