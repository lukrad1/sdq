/** \file rtc.h
* \brief This file contains rtc function.
*
* \author Lukasz Radola
* \date 02.09.15
*
* \details In this file are definitions, declarations of rtc functions.
*/
#ifndef LRADOLA_RTC_RTC_H_
#define LRADOLA_RTC_RTC_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*

*/
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* ... */
/* other (non local) library */
/* ... */
/* local library */
#include "main.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition */


/* Typedef definition */



/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */


extern uint32_t RTC__nextWakeupTime;
extern uint32_t RTC__nextAlarmTime;
extern uint16_t RTC__rtcFrequency;
extern int32_t _param_RTC_sleep_time; //ok. RTC_sleep_time+1 sekund spania  1s->18,2h

/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions (in *.c file) 2. Local exported */ 
/* functions (in this or in second *.h file) 3. Interface (exported) functions) */

/* Local functions in *.c file */

/* Exported functions */

void RTC__Init(void);

void RTC__Set_Wakeup(uint32_t *wakeup_time);
void RTC__Set_Alarm(uint32_t *alarm_time);

void RTC__Interrupt_Routine(void);
void RTC__Disable_Wakeup(void);



#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_RTC_RTC_H_ */

