/** \file ee.c
* \brief This file contains definitions and eeprom functions.
*
* \author Lukasz Radola.
* \date 28.07.15
*
* 
*/
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* #include <math.h> */
/* other (non local) library */
/* ... */
/* local library */

#include "gpio.h"
#include "ee.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */
/* ... */

/*! Local struct which contains adc measure result, adc operation flags etc. */
static volatile struct
{
	uint32_t adc_EE_Vref;

}ee__data_s = {0};


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

void EE__Init(void)
{
	//ee__data_s.adc_EE_Vref = (uint32_t)(0x0600 +
                      //(uint16_t)EE__ReadByte((uint16_t)ADC__VREFINT_CONV_ADDR));
}

/******************************* END FUNCTION *********************************/

uint32_t EE_GetEeVrefValue(void)
{
  //return ee__data_s.adc_EE_Vref;
}

/******************************* END FUNCTION *********************************/

void EE__Unlock(void)
{

}

/******************************* END FUNCTION *********************************/

void EE__Lock(void)
{

}

/******************************* END FUNCTION *********************************/

uint8_t EE__ReadByte(uint16_t address) 
{
	//return(*(PointerAttr uint8_t *) (uint16_t)address);
}

/******************************* END FUNCTION *********************************/

uint16_t EE__ReadHalfWord(uint16_t address) 
{

}

/******************************* END FUNCTION *********************************/

uint32_t EE__ReadWord(uint16_t address) 
{

}

/******************************* END FUNCTION *********************************/

void EE__WriteByte(uint16_t address, uint8_t data)
{

}

/******************************* END FUNCTION *********************************/

void EE__WriteHalfWord(uint16_t address, uint16_t data) 
{

}

/******************************* END FUNCTION *********************************/

void EE__WriteWord(uint16_t address, uint32_t data) 
{

}

/******************************* END FUNCTION *********************************/


uint8_t ee_EraseEEPROM(uint8_t val)
{
	
}

uint8_t ee_VerifyEEPROM(uint8_t val)
{

}

#ifdef __cplusplus
  }
#endif
