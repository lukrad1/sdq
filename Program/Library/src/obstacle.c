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
    uint8_t start_identification : 1;    /*jezeli jest przeszkoda to ta zmienna blokuje ustawianie pwm z tabletu*/
    uint8_t sharp_id;

  } obstacle__data_s = {0};

/* Local exported functions */
/* ... */

/* Interface (exported) functions */

  void OBSTACLE__EnkoderInterrupt(void)
  {


  }

  void OBSTACLE__StartIdentificationTimer(void)
  {
    obstacle__data_s.start_identification = 1;

  }

  void OBSTACLE__StopIdentificationTimer(void)
  {
    obstacle__data_s.start_identification = 0;

  }

  void OBSTACLE__1msPoll(void)
  {
    static int32_t counter = 500;
    static local_state = 10;

    if(obstacle__data_s.start_identification)
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
            obstacle__data_s.start_identification = 0;
            local_state++;
            break;
          }
          case 11:
          {
            local_state = obstacle__data_s.sharp_id;
            // no break
          }
          case SHARP_PRZOD_SRODEK: // tu musi byc sharp
          case SHARP_PRZOD_LEWY:
          case SHARP_PRZOD_PRAWY:
          {
            TIMER__PWM_DC1_2_Change_Duty(40);
            MOTORS__jazda_do_tylu();
            counter = 4000; // local timeout if enkoder will work bad
            local_state = 20;
            break;
          }
          case SHARP_TYL_SRODEK:
          {
            TIMER__PWM_DC1_2_Change_Duty(40);
            MOTORS__jazda_do_przodu();
            counter = 4000; // local timeout if enkoder will work bad
            local_state = 20;
            break;
          }

          case 20:
          {
            //finish obstacle function
            MOTORS__jazda_zatrzymana();
            counter = 500;
            local_state = 10;
          }
          }
        }
      }
    else if(!obstacle__data_s.start_identification && counter != 0)
    {
      counter = 500;
      local_state = 10;
    }
  }

  void OBSTACLE__SetSharpId(uint8_t type_of_sharp)
  {
    obstacle__data_s.sharp_id = type_of_sharp;
  }

#ifdef __cplusplus
  }
#endif
