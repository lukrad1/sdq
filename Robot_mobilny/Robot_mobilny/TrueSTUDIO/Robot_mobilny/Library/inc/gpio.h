/** \file gpio.h
* \brief This file contains in general gpio configurartion.
*
* \author Lukasz Radola
* \date 22.05.15
*
* \details In this file are definitions, declarations of functions, which can  
* set gpio as output, input et cetera...
*/
#ifndef LRADOLA_GPIO_GPIO_H_
#define LRADOLA_GPIO_GPIO_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*

void GPIO__Init(void);
void GPIO__ConfigUART(uint8_t state);
void GPIO__ConfigLedR(uint8_t state);
void GPIO__ConfigLedG(uint8_t state);
void GPIO__ConfigLedB(uint8_t state);


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

/*! Pin options (GPIO CR2): NONE, INPUT - EXTERNAL INTERRUPT, OUT - FAST MODE */
#define GPIO__OPT_NONE               0
#define GPIO__OPT_IN_EXT_INT         1
#define GPIO__OPT_OUT_FAST_MODE      1

/*! HELPER MACROS */
#define GPIO__PORT(periph) (GPIO__##periph##_PORT)
#define GPIO__PIN(periph)  (GPIO__##periph##_PIN)

/*! Port configuration - set (_state == 1) or clear (_state == 0) bit */
#define GPIO__PORT_CFG(_port,_bit,_state) \
{ \
  ((_state)?((_port)|=(uint8_t)(_bit)):((_port) &=(uint8_t)(~(uint8_t)(_bit)))); \
}

#define GPIO__SET(periph, _state) \
{ \
  GPIO__PORT_CFG(GPIO__PORT(periph)->ODR, GPIO__PIN(periph), _state); \
}

/*! GPIO configuration as output push pull with or without special function */
#define GPIO__OUTPUT_PP(periph, value, opt) \
{ \
  GPIO__PORT_CFG(GPIO__PORT(periph)->DDR, GPIO__PIN(periph), 1); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR1, GPIO__PIN(periph), 1); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR2, GPIO__PIN(periph), opt); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->ODR, GPIO__PIN(periph), value); \
}

/*! GPIO configuration as output open drain with or without special function */
#define GPIO__OUTPUT_OD(periph, value, opt) \
{ \
  GPIO__PORT_CFG(GPIO__PORT(periph)->DDR, GPIO__PIN(periph), 1); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR1, GPIO__PIN(periph), 0); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR2, GPIO__PIN(periph), opt); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->ODR, GPIO__PIN(periph), value); \
}

/*! GPIO configuration as input pull up with or without special function */
#define GPIO__INPUT_PUP(periph, opt) \
{ \
  GPIO__PORT_CFG(GPIO__PORT(periph)->DDR, GPIO__PIN(periph), 0); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR1, GPIO__PIN(periph), 1); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR2, GPIO__PIN(periph), opt); \
}								 

/*! GPIO configuration as input floating with or without special function */
#define GPIO__INPUT_FLOATING(periph, opt) \
{ \
  GPIO__PORT_CFG(GPIO__PORT(periph)->DDR, GPIO__PIN(periph), 0); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR1, GPIO__PIN(periph), 0); \
  GPIO__PORT_CFG(GPIO__PORT(periph)->CR2, GPIO__PIN(periph), opt); \
}							

/* UART */




#define GPIO__UART_RX_PORT        GPIOC
#define GPIO__UART_RX_PIN         GPIO_Pin_2

#define GPIO__UART_TX_PORT        GPIOC
#define GPIO__UART_TX_PIN         GPIO_Pin_3

/* BUTTON */
#define GPIO__BUTTON_PORT         GPIOC
#define GPIO__BUTTON_PIN          GPIO_PIN_13
#define KEY_BUTTON_EXTI_LINE                   GPIO_PIN_13
#define KEY_BUTTON_EXTI_IRQn                   EXTI4_15_IRQn


/* LED [DIODE_PWM_R, DIODE_PWM_G, DIODE_PWM_B] */

#define GPIO__LED_G_PORT					GPIOA
#define GPIO__LED_G_PIN						GPIO_PIN_5


/* NOT USED (NOT CONNECTED) */
#define GPIO__UNUSED0_PORT				GPIOA
#define GPIO__UNUSED0_PIN					GPIO_Pin_2

#define GPIO__UNUSED1_PORT				GPIOC
#define GPIO__UNUSED1_PIN					GPIO_Pin_0

#define GPIO__UNUSED2_PORT				GPIOC
#define GPIO__UNUSED2_PIN					GPIO_Pin_1

#define GPIO__UNUSED3_PORT				GPIOC
#define GPIO__UNUSED3_PIN					GPIO_Pin_5

/* Typedef definition */
  /*!< Signed integer types  */
  typedef   signed char     int8_t;
  typedef   signed short    int16_t;
  typedef   signed long     int32_t;

  /*!< Unsigned integer types  */
  typedef unsigned char     uint8_t;
  typedef unsigned short    uint16_t;
  typedef unsigned long     uint32_t;

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
/* Functions definitions (1. Static functions (in *.c file) 2. Local exported */ 
/* functions (in this or in second *.h file) 3. Interface (exported) functions) */

/* Local functions in *.c file */

/* Exported functions */

/**
* \brief Initialization all peripherals (GPIO).
*
* This init functions set all peripherals to default value. In this state
* the power consumption is the lowest.
*
*/
void GPIO__Init(void);
/**
* \brief UART pins configuration.
*
* \param[in] state If state is equal to 1, then pins are enable (active), 
* in other hand pins are disable (disactive).
*
*/
void GPIO__ConfigUART(uint8_t state);

/**
* \brief Red LED pin configuration.
*
* \param[in] state If state is equal to 1, then pin is enable (active), 
* in other hand pin is disable (disactive).
*
*/
void GPIO__ConfigLedR(uint8_t state);
/**
* \brief Green LED pin configuration.
*
* \param[in] state If state is equal to 1, then pin is enable (active), 
* in other hand pin is disable (disactive).
*
*/
void GPIO__ConfigLedG(uint8_t state);
/**
* \brief Blue LED pin configuration.
*
* \param[in] state If state is equal to 1, then pin is enable (active), 
* in other hand pin is disable (disactive).
*
*/
void GPIO__ConfigLedB(uint8_t state);

void GPIO__ConfigButton(uint8_t state);

void GPIO__ConfigEnkoders(uint8_t state);

void GPIO__ConfigSPI_RASPB(uint8_t state);

void GPIO__ConfigUART_RASPB(uint8_t state);

void GPIO__ConfigSharpEspEnable(uint8_t state);

void GPIO__ConfigMotorsEnable(uint8_t state);

void GPIO__ConfigRaspbEnable(uint8_t state);
#ifdef __cplusplus
  }
#endif

#endif /* LRADOLA_GPIO_GPIO_H_ */
