/** \file timer.h
* \brief This file contains system clock and timers functions.
*
* \author Lukasz Radola
* \date 22.05.15
*
* \details In this file are definitions, declarations of functions, which can  
* set system clock, enable timer interrupt et cetera...
*/
#ifndef LRADOLA_TIMER_TIMER_H_
#define LRADOLA_TIMER_TIMER_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*
void TIMER__InitClk(void);
void TIMER__DelayMs(uint16_t ms);
void TIMER__Delay10us(uint16_t us);
void TIMER__InitSystemTimer(void);
void TIMER__InitInterruptTim4Isr(void);
void TIMER__Poll(void);
void TIMER__SetFlag(timer__union_u_t* union_address, uint8_t set_reset_status);
uint8_t TIMER__GetStatus(void);

*/
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* ... */
/* other (non local) library */
/* ... */
/* local library */


/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition */

#define TIMER__16MHZ_CLK 16000000
#define TIMER__4MHZ_CLK 4000000
#define TIMER__1MHZ_CLK 1000000


/* Define the Timer to be configured */
#define TIMx_BASE       TIM2_BASE
#define TIMx            ((TIM_TypeDef *) TIMx_BASE)

/* Typedef definition */

typedef union
{
  uint8_t status;
  
  struct
  {
    uint8_t time_1ms_flag : 1;
    uint8_t time_adc_2ms_flag : 1;
    uint8_t time_adc_5ms_flag : 1;
    uint8_t time_10ms_flag : 1;
    uint8_t time_example : 1;
    uint8_t time_pwm_is_on : 1;
    uint8_t time_pwm_last_value;

  };
  
} timer__union_u_t;

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */
extern volatile timer__union_u_t timer__data_u;
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions (in *.c file) 2. Local exported */ 
/* functions (in this or in second *.h file) 3. Interface (exported) functions) */

/* Local functions in *.c file */

/* Exported functions */


/**
* \brief Initialization system clock.
*
* This init function sets system clock frequency, clock source et cetera 
*
*/
void TIMER__InitClk(void);
/**
* \brief Delay function (millisecond).
*
* \param[in] ms Delay value in millisecond.
*
*/
void TIMER__DelayMs(uint16_t ms);
/**
* \brief Delay function (microsecond).
*
* \param[in] us Delay value in microsecond multiply 10.
*
*/
void TIMER__Delay10us(uint16_t us);
/**
* \brief Initialization main timer in our system.
*
* This functions set all required registers of timer xxx. This timer is main
* soft-system timer.
*/
void TIMER__InitSystemTimer(void);
/**
* \brief Initialization interrupt on timer 4 
*
*/
void TIMER__InitInterruptTim4Isr(void);
/**
* \brief Timer polling function.
*
* In this function are checking all timer flags. It is running in while(1) loop.
*
*/
void TIMER__Poll(void);
/**
* \brief This function set all flags in timer__data_u struct.
*
* \param[in] union_address It's address value of temporary union.
* \param[in] set_reset_status If you want to set flags than you should used 
* TIMER__SET definition else TIMER__RESET definition.
*/
void TIMER__SetFlag(timer__union_u_t* union_address, uint8_t set_reset_status);
/**
* \brief This function get all flags with timer__data_u struct.
*
* \return  timer__data_u.status value is returned
*
*/
uint8_t TIMER__GetStatus(void);

void TIMER__PWM_DC1_2_ON(void);
void TIMER__PWM_DC1_2_OFF(void);
void TIMER__PWM_DC1_2_Change_Duty(uint8_t duty_in_percent);

#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_TIMER_TIMER_H_ */
