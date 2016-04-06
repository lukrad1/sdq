
/** \file motors.c
* \brief This file contains functions which configure, control etc motors in my robot
* \author Lukasz Radola
* \date 23.01.16
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
#include "motors.h"
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

  static volatile struct
  {
    uint8_t last_direction;    /*!< Did ADC initialization make? */
    uint8_t current_direction;
  } motors_data_s = {0};

/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */
/* ... */

/* Static functions definitions */


/* Local exported functions */
/* ... */

/* Interface (exported) functions */


static void kolo_przod_prawe_do_przodu(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_PRZOD_PRAWE_ZEGAR; /* (1) */

  KOLO_PRZOD_PRAWE_PORT->MODER =
  (KOLO_PRZOD_PRAWE_PORT->MODER & ~(KOLO_PRZOD_PRAWE_AIN1_PIN))
  | (KOLO_PRZOD_PRAWE_AIN1_PIN_MODE);

  KOLO_PRZOD_PRAWE_PORT->MODER =
  (KOLO_PRZOD_PRAWE_PORT->MODER & ~(KOLO_PRZOD_PRAWE_AIN2_PIN))
  | (KOLO_PRZOD_PRAWE_AIN2_PIN_MODE);

  //set AIN1 as HIGH
  KOLO_PRZOD_PRAWE_PORT->BSRR = (1 << KOLO_PRZOD_PRAWE_AIN1_PIN_NUMBER);
  //reset AIN2 (set as LOW)
  KOLO_PRZOD_PRAWE_PORT->BRR = (1 << KOLO_PRZOD_PRAWE_AIN2_PIN_NUMBER);

}


static void kolo_przod_prawe_do_tylu(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_PRZOD_PRAWE_ZEGAR; /* (1) */

  KOLO_PRZOD_PRAWE_PORT->MODER =
  (KOLO_PRZOD_PRAWE_PORT->MODER & ~(KOLO_PRZOD_PRAWE_AIN1_PIN))
  | (KOLO_PRZOD_PRAWE_AIN1_PIN_MODE);

  KOLO_PRZOD_PRAWE_PORT->MODER =
  (KOLO_PRZOD_PRAWE_PORT->MODER & ~(KOLO_PRZOD_PRAWE_AIN2_PIN))
  | (KOLO_PRZOD_PRAWE_AIN2_PIN_MODE);

  //set AIN2 as HIGH
  KOLO_PRZOD_PRAWE_PORT->BSRR = (1 << KOLO_PRZOD_PRAWE_AIN2_PIN_NUMBER);
  //reset AIN1 (set as LOW)
  KOLO_PRZOD_PRAWE_PORT->BRR = (1 << KOLO_PRZOD_PRAWE_AIN1_PIN_NUMBER);

}

static void kolo_przod_prawe_stop(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_PRZOD_PRAWE_ZEGAR; /* (1) */

  KOLO_PRZOD_PRAWE_PORT->MODER =
  (KOLO_PRZOD_PRAWE_PORT->MODER & ~(KOLO_PRZOD_PRAWE_AIN1_PIN))
  | (KOLO_PRZOD_PRAWE_AIN1_PIN_MODE);

  KOLO_PRZOD_PRAWE_PORT->MODER =
  (KOLO_PRZOD_PRAWE_PORT->MODER & ~(KOLO_PRZOD_PRAWE_AIN2_PIN))
  | (KOLO_PRZOD_PRAWE_AIN2_PIN_MODE);

  //reset AIN1 (set as LOW)
  KOLO_PRZOD_PRAWE_PORT->BRR = (1 << KOLO_PRZOD_PRAWE_AIN1_PIN_NUMBER);
  //reset AIN2 (set as LOW)
  KOLO_PRZOD_PRAWE_PORT->BRR = (1 << KOLO_PRZOD_PRAWE_AIN2_PIN_NUMBER);

}


static void kolo_przod_lewe_do_przodu(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_PRZOD_LEWE_ZEGAR; /* (1) */

  KOLO_PRZOD_LEWE_PORT->MODER = (KOLO_PRZOD_LEWE_PORT->MODER & ~(KOLO_PRZOD_LEWE_BIN1_PIN))
  | (KOLO_PRZOD_LEWE_BIN1_PIN_MODE);

  KOLO_PRZOD_LEWE_PORT->MODER =
  (KOLO_PRZOD_LEWE_PORT->MODER & ~(KOLO_PRZOD_LEWE_BIN2_PIN))
  | (KOLO_PRZOD_LEWE_BIN2_PIN_MODE);

  //set BIN1 as HIGH
  KOLO_PRZOD_LEWE_PORT->BSRR = (1 << KOLO_PRZOD_LEWE_BIN1_PIN_NUMBER);
  //resetBIN2 (set as LOW)
  KOLO_PRZOD_LEWE_PORT->BRR = (1 << KOLO_PRZOD_LEWE_BIN2_PIN_NUMBER);

}
static void kolo_przod_lewe_do_tylu(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_PRZOD_LEWE_ZEGAR; /* (1) */

  KOLO_PRZOD_LEWE_PORT->MODER =
  (KOLO_PRZOD_LEWE_PORT->MODER & ~(KOLO_PRZOD_LEWE_BIN1_PIN))
  | (KOLO_PRZOD_LEWE_BIN1_PIN_MODE);

  KOLO_PRZOD_LEWE_PORT->MODER =
  (KOLO_PRZOD_LEWE_PORT->MODER & ~(KOLO_PRZOD_LEWE_BIN2_PIN))
  | (KOLO_PRZOD_LEWE_BIN2_PIN_MODE);

  //set BIN2 as HIGH
  KOLO_PRZOD_LEWE_PORT->BSRR = (1 << KOLO_PRZOD_LEWE_BIN2_PIN_NUMBER);
  //reset BIN1 (set as LOW)
  KOLO_PRZOD_LEWE_PORT->BRR = (1 << KOLO_PRZOD_LEWE_BIN1_PIN_NUMBER);

}
static void kolo_przod_lewe_stop(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_PRZOD_LEWE_ZEGAR; /* (1) */

  KOLO_PRZOD_LEWE_PORT->MODER =
  (KOLO_PRZOD_LEWE_PORT->MODER & ~(KOLO_PRZOD_LEWE_BIN1_PIN))
  | (KOLO_PRZOD_LEWE_BIN1_PIN_MODE);

  KOLO_PRZOD_LEWE_PORT->MODER =
  (KOLO_PRZOD_LEWE_PORT->MODER & ~(KOLO_PRZOD_LEWE_BIN2_PIN))
  | (KOLO_PRZOD_LEWE_BIN2_PIN_MODE);

  //reset AIN1 (set as LOW)
  KOLO_PRZOD_LEWE_PORT->BRR = (1 << KOLO_PRZOD_LEWE_BIN1_PIN_NUMBER);
  //reset AIN2 (set as LOW)
  KOLO_PRZOD_LEWE_PORT->BRR = (1 << KOLO_PRZOD_LEWE_BIN2_PIN_NUMBER);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void kolo_tyl_prawe_do_przodu(void)
{

  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_TYL_PRAWE_ZEGAR; /* (1) */

  KOLO_TYL_PRAWE_PORT->MODER =
  (KOLO_TYL_PRAWE_PORT->MODER & ~(KOLO_TYL_PRAWE_AIN1_PIN))
  | (KOLO_TYL_PRAWE_AIN1_PIN_MODE);

  KOLO_TYL_PRAWE_PORT->MODER =
  (KOLO_TYL_PRAWE_PORT->MODER & ~(KOLO_TYL_PRAWE_AIN2_PIN))
  | (KOLO_TYL_PRAWE_AIN2_PIN_MODE);

  //set AIN1 as HIGH
  KOLO_TYL_PRAWE_PORT->BSRR = (1 << KOLO_TYL_PRAWE_AIN1_PIN_NUMBER);
  //reset AIN2 (set as LOW)
  KOLO_TYL_PRAWE_PORT->BRR = (1 << KOLO_TYL_PRAWE_AIN2_PIN_NUMBER);

}
static void kolo_tyl_prawe_do_tylu(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_TYL_PRAWE_ZEGAR; /* (1) */

  KOLO_TYL_PRAWE_PORT->MODER =
  (KOLO_TYL_PRAWE_PORT->MODER & ~(KOLO_TYL_PRAWE_AIN1_PIN))
  | (KOLO_TYL_PRAWE_AIN1_PIN_MODE);

  KOLO_TYL_PRAWE_PORT->MODER =
  (KOLO_TYL_PRAWE_PORT->MODER & ~(KOLO_TYL_PRAWE_AIN2_PIN))
  | (KOLO_TYL_PRAWE_AIN2_PIN_MODE);

  //set AIN2 as HIGH
  KOLO_TYL_PRAWE_PORT->BSRR = (1 << KOLO_TYL_PRAWE_AIN2_PIN_NUMBER);
  //reset AIN1 (set as LOW)
  KOLO_TYL_PRAWE_PORT->BRR = (1 << KOLO_TYL_PRAWE_AIN1_PIN_NUMBER);
}
static void kolo_tyl_prawe_stop(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_TYL_PRAWE_ZEGAR; /* (1) */

  KOLO_TYL_PRAWE_PORT->MODER =
  (KOLO_TYL_PRAWE_PORT->MODER & ~(KOLO_TYL_PRAWE_AIN1_PIN))
  | (KOLO_TYL_PRAWE_AIN1_PIN_MODE);

  KOLO_TYL_PRAWE_PORT->MODER =
  (KOLO_TYL_PRAWE_PORT->MODER & ~(KOLO_TYL_PRAWE_AIN2_PIN))
  | (KOLO_TYL_PRAWE_AIN2_PIN_MODE);

  //reset AIN2 (set as LOW)
  KOLO_TYL_PRAWE_PORT->BRR = (1 << KOLO_TYL_PRAWE_AIN1_PIN_NUMBER);
  //reset AIN2 (set as LOW)
  KOLO_TYL_PRAWE_PORT->BRR = (1 << KOLO_TYL_PRAWE_AIN2_PIN_NUMBER);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

static void kolo_tyl_lewe_do_przodu(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_TYL_LEWE_ZEGAR; /* (1) */

  KOLO_TYL_LEWE_PORT->MODER =
  (KOLO_TYL_LEWE_PORT->MODER & ~(KOLO_TYL_LEWE_BIN1_PIN))
  | (KOLO_TYL_LEWE_BIN1_PIN_MODE);

  KOLO_TYL_LEWE_PORT->MODER =
  (KOLO_TYL_LEWE_PORT->MODER & ~(KOLO_TYL_LEWE_BIN2_PIN))
  | (KOLO_TYL_LEWE_BIN2_PIN_MODE);

  //set BIN1 as HIGH
  KOLO_TYL_LEWE_PORT->BSRR = (1 << KOLO_TYL_LEWE_BIN1_PIN_NUMBER);
  //resetBIN2 (set as LOW)
  KOLO_TYL_LEWE_PORT->BRR = (1 << KOLO_TYL_LEWE_BIN2_PIN_NUMBER);
}
static void kolo_tyl_lewe_do_tylu(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_TYL_LEWE_ZEGAR; /* (1) */

  KOLO_TYL_LEWE_PORT->MODER =
  (KOLO_TYL_LEWE_PORT->MODER & ~(KOLO_TYL_LEWE_BIN1_PIN))
  | (KOLO_TYL_LEWE_BIN1_PIN_MODE);

  KOLO_TYL_LEWE_PORT->MODER =
  (KOLO_TYL_LEWE_PORT->MODER & ~(KOLO_TYL_LEWE_BIN2_PIN))
  | (KOLO_TYL_LEWE_BIN2_PIN_MODE);

  //set BIN2 as HIGH
  KOLO_TYL_LEWE_PORT->BSRR = (1 << KOLO_TYL_LEWE_BIN2_PIN_NUMBER);
  //resetBIN1 (set as LOW)
  KOLO_TYL_LEWE_PORT->BRR = (1 << KOLO_TYL_LEWE_BIN1_PIN_NUMBER);
}
static void kolo_tyl_lewe_stop(void)
{
  /* (1) Enable the peripheral clock of GPIOA */
  /* (2) Select output mode (01) on GPIOA pin  */
  RCC->IOPENR |= KOLO_TYL_LEWE_ZEGAR; /* (1) */

  KOLO_TYL_LEWE_PORT->MODER =
  (KOLO_TYL_LEWE_PORT->MODER & ~(KOLO_TYL_LEWE_BIN1_PIN))
  | (KOLO_TYL_LEWE_BIN1_PIN_MODE);

  KOLO_TYL_LEWE_PORT->MODER =
  (KOLO_TYL_LEWE_PORT->MODER & ~(KOLO_TYL_LEWE_BIN2_PIN))
  | (KOLO_TYL_LEWE_BIN2_PIN_MODE);

  //resetBIN2 (set as LOW)
  KOLO_TYL_LEWE_PORT->BRR = (1 << KOLO_TYL_LEWE_BIN1_PIN_NUMBER);
  //resetBIN2 (set as LOW)
  KOLO_TYL_LEWE_PORT->BRR = (1 << KOLO_TYL_LEWE_BIN2_PIN_NUMBER);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MOTORS__skret_w_lewo(void)
{
  kolo_przod_lewe_do_tylu();
  kolo_tyl_lewe_do_tylu();
  kolo_przod_prawe_do_przodu();
  kolo_tyl_prawe_do_przodu();
  TIMER__PWM_DC1_2_ON();
  motors_data_s.current_direction = JAZDA_W_LEWO;
}

void MOTORS__skret_w_prawo(void)
{
  kolo_przod_prawe_do_tylu();
  kolo_tyl_prawe_do_tylu();
  kolo_przod_lewe_do_przodu();
  kolo_tyl_lewe_do_przodu();
  TIMER__PWM_DC1_2_ON();
  motors_data_s.current_direction = JAZDA_W_PRAWO;
}

void MOTORS__jazda_do_tylu(void)
{
  kolo_przod_lewe_do_przodu();
   kolo_przod_prawe_do_przodu();
   kolo_tyl_lewe_do_przodu();
   kolo_tyl_prawe_do_przodu();
   TIMER__PWM_DC1_2_ON();
   motors_data_s.current_direction = JAZDA_DO_TYLU;

}

void MOTORS__jazda_do_przodu(void)
{
  kolo_przod_lewe_do_tylu();
   kolo_przod_prawe_do_tylu();
   kolo_tyl_lewe_do_tylu();
   kolo_tyl_prawe_do_tylu();
   TIMER__PWM_DC1_2_ON();
   motors_data_s.current_direction = JAZDA_DO_PRZODU;
}

void MOTORS__jazda_zatrzymana(void)
{
  kolo_przod_lewe_stop();
  kolo_przod_prawe_stop();
  kolo_tyl_lewe_stop();
  kolo_tyl_prawe_stop();
  TIMER__PWM_DC1_2_OFF();
  motors_data_s.current_direction = JAZDA_ZATRZYMANA;
}

void MOTORS__SetLastDirection(uint8_t direction)
{
  motors_data_s.last_direction = direction;
}

uint8_t  MOTORS__GetCurrentDirection(void)
{
  return motors_data_s.current_direction;
}

void MOTORS__GoInLastDirection(void)
{

  switch(motors_data_s.last_direction)
  {
    case JAZDA_DO_PRZODU:
    {
      MOTORS__jazda_do_przodu();
      break;
    }
    case JAZDA_DO_TYLU:
    {
      MOTORS__jazda_do_tylu();
      break;
    }
    case JAZDA_W_LEWO:
    {
      MOTORS__skret_w_lewo();
      break;
    }
    case JAZDA_W_PRAWO:
    {
      MOTORS__skret_w_prawo();
      break;
    }
    case JAZDA_ZATRZYMANA:
    {
      MOTORS__jazda_zatrzymana();
      break;
    }
  }
}
#ifdef __cplusplus
  }
#endif
