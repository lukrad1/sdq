/** \file rtc.c
* \brief This file contains definitions rtc clock and functions.
*
* \author Lukasz Radola
* \date 02.08.15
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

#include "gpio.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */

static volatile struct RTC__struct
{
    uint8_t isInitiated : 1;
    uint8_t calibStage : 1;
    uint8_t calibEnd : 1;
		
} RTC__status = {0};

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

uint16_t LSI_measured_frequency;

									//ok. RTC_sleep_time+1 sekund spania  1s->18,2h

/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */


static void lsi__calibration(void)
{

}

/******************************* END FUNCTION *********************************/
    
void RTC__Init(void)
{
	lsi__calibration();
}

/******************************* END FUNCTION *********************************/

/**
* @brief Funkcja ustawiajaca kolejny czas wybudzenia uC
*
*
* @param RTC__next_wakeup_time - czas po ktorym zostanie wygenerowane kolejne przerwanie RTC
* @retval void
*/
void RTC__Set_Wakeup(uint32_t *RTC__next_wakeup_time)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Ustawia alarm poprzez czas
*
*
* @param Czas do najblizszego alarmu w sek.
* @retval void
*/
void RTC__Set_Alarm(uint32_t *time)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Zatrzymuje zegar odliczajacy do nastepnego przerwania RTC Wakeup
*
*
* @param void
* @retval void
*/
void RTC__Disable_Wakeup(void)
{

}

/******************************* END FUNCTION *********************************/

/**
* @brief Procedura przerwania od RTC
*
*
* @param void
* @retval void
*/
void RTC__Interrupt_Routine(void)
{

}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif
