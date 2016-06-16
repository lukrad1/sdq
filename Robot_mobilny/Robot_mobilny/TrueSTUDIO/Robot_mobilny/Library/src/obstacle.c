/** \file obstacle.c
* \brief This file contains definitions of all local and interface functions
* which detect obstacle.
* \author Lukasz Radola
* \date 02.04.16
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
#include "obstacle.h"
#include "motors.h"
#include "adc.h"
#include "timer.h"
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
  static volatile struct
  {
    uint8_t isInitiated : 1;    /*!< Did ADC initialization make? */
    uint8_t start_identification;    /*jezeli jest przeszkoda to ta zmienna blokuje ustawianie pwm z tabletu*/
    uint8_t sharp_id;
    uint8_t obstacle_timeout :1;
    uint8_t avoid_obstacle_isr_steps : 1;
    uint8_t avoid_obstacle_isr_counter;
  } obstacle__data_s = {0};

/* Local exported functions */
/* ... */

/* Interface (exported) functions */

  void OBSTACLE__EnkoderInterrupt(void)
  {

    if(obstacle__data_s.avoid_obstacle_isr_steps)
    {
      obstacle__data_s.avoid_obstacle_isr_counter++;
      GPIOA->ODR ^= (1 << 5);//toggle green led on PA5

      if(obstacle__data_s.avoid_obstacle_isr_counter > 50)
      {
        MOTORS__jazda_zatrzymana();
        obstacle__data_s.avoid_obstacle_isr_steps = 0;
        obstacle__data_s.avoid_obstacle_isr_counter = 0;
      }
    }
    else
    {
      obstacle__data_s.avoid_obstacle_isr_counter = 0;
    }
  }

  void OBSTACLE__StartIdentificationTimer(void)
  {
    if(obstacle__data_s.start_identification != 2)
    {
      obstacle__data_s.start_identification = 2;
    }
  }

  void OBSTACLE__WaitIdentificationTimer(void)
  {
    if(obstacle__data_s.start_identification != 1)
    {
      obstacle__data_s.start_identification = 1;
    }
  }

  uint8_t OBSTACLE__GetIdentificationTimer(void)
  {

     return obstacle__data_s.start_identification;
  }

  uint8_t OBSTACLE__GetAvoidObstacleIsrFlag(void)
  {

     return obstacle__data_s.avoid_obstacle_isr_steps;
  }

  uint8_t OBSTACLE__GetSharpId(void)
  {
     return obstacle__data_s.sharp_id;
  }

  void OBSTACLE__ClearAvoidObstacleIsrFlag(void)
  {

    obstacle__data_s.avoid_obstacle_isr_steps = 0;
    obstacle__data_s.avoid_obstacle_isr_counter = 0;
  }

  void OBSTACLE__StopIdentificationTimer(void)
  {
    if(obstacle__data_s.start_identification != 0)
    {
      MOTORS__GoInLastDirection();
      obstacle__data_s.start_identification = 0;
    }
  }

  void OBSTACLE__1msPoll(void)
  {
    static int32_t counter = 500;
    static uint8_t local_state = 10;

    // only if is set start value
    if(obstacle__data_s.start_identification == 2 &&
       !obstacle__data_s.avoid_obstacle_isr_steps)
    {
      if(counter > 0)
      {
        counter--;
      }
      if(counter <= 0)
      {
        switch(local_state)
        {
          case 10:
          {
            //again check obstacle in adc, so clear start identification
            // no clear counter, becouse
            OBSTACLE__WaitIdentificationTimer();
            local_state++;
            break;
          }
          case 11:
          {
            local_state = obstacle__data_s.sharp_id;
            break;
          }
          case SHARP_PRZOD_SRODEK: // tu musi byc sharp
          case SHARP_PRZOD_LEWY:
          case SHARP_PRZOD_PRAWY:
          {
            TIMER__PWM_DC1_2_Change_Duty(40);
            MOTORS__jazda_do_tylu();
            obstacle__data_s.avoid_obstacle_isr_steps = 1;
            obstacle__data_s.start_identification = 0;
            counter = 500;
            local_state = 10;
            break;
          }
          case SHARP_TYL_SRODEK:
          {
            TIMER__PWM_DC1_2_Change_Duty(40);
            MOTORS__jazda_do_przodu();
            obstacle__data_s.avoid_obstacle_isr_steps = 1;
            obstacle__data_s.start_identification = 0;
            counter = 500;
            local_state = 10;
            break;
          }
        }
      }
    }
    else if(!obstacle__data_s.start_identification && counter != 0 && counter != 500)
    {
      //continue drive or stop
      counter = 500;
      local_state = 10;
      //MOTORS__GoInLastDirection();
    }
  }

  void OBSTACLE__SetSharpId(uint8_t type_of_sharp)
  {
    obstacle__data_s.sharp_id = type_of_sharp;
  }

#ifdef __cplusplus
  }
#endif
