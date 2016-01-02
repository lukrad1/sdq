/** \file ee.h
* \brief This file contains eeprom functions.
*
* \author Lukasz Radola
* \date 28.07.15
*
* \details In this file are definitions, declarations of functions, which can  
* lock, unlock eeprom and write or read data from eeprom et cetera...
*/
#ifndef LRADOLA_EE_EE_H_
#define LRADOLA_EE_EE_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*
void EE__Unlock(void);
void EE__Lock(void);
uint32_t EE_GetEeVrefValue(void);
uint8_t EE__ReadByte(uint16_t address);
uint16_t EE__ReadHalfWord(uint16_t address);
uint32_t EE__ReadWord(uint16_t address);
void EE__WriteByte(uint16_t address, uint8_t data);
void EE__WriteHalfWord(uint16_t address, uint16_t data);
void EE__WriteWord(uint16_t address, uint32_t data);

*/
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
/* std C library */
/* ... */
/* other (non local) library */
/* ... */
/* local library */
#include "gpio.h"
#include "ee.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition */

/*!< End of programming (write or erase operation) flag */


/* Typedef definition */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/


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
* \brief This function unlocks stm eeprom memory.
*
* This functions unlocks eeprom and enables write or read data.
*
*/
void EE__Unlock(void);
/**
* \brief This function locks stm eeprom memory.
*
* This functions locks eeprom and disables write or read data.
*
*/
void EE__Lock(void);
/**
* \brief This function return ee__data.adc_EE_Vref value.
*
* \return ee__data.adc_EE_Vref value.
*
*/
uint32_t EE_GetEeVrefValue(void);
/**
* \brief This function reads one byte from stm eeprom memory.
*
* \param[in] address It's start read address value in eeprom space.
* \return Read eeprom value is returned.
*/
uint8_t EE__ReadByte(uint16_t address);
/**
* \brief This function reads two bytes from stm eeprom memory.
*
* \param[in] address It's start read address value in eeprom space.
* \return Read eeprom value is returned.
*/
uint16_t EE__ReadHalfWord(uint16_t address);
/**
* \brief This function reads four bytes from stm eeprom memory.
*
* \param[in] address It's start read address value in eeprom space.
* \return Read eeprom value is returned.
*/
uint32_t EE__ReadWord(uint16_t address);
/**
* \brief This function writes one bytes into stm eeprom memory.
*
* \param[in] address It's start write address value in eeprom space.
* \param[in] data There are data which will save in eeprom space
*/
void EE__WriteByte(uint16_t address, uint8_t data);
/**
* \brief This function writes twi bytes into stm eeprom memory.
* 
* \param[in] address It's start write address value in eeprom space.
* \param[in] data There are data which will save in eeprom space
*/
void EE__WriteHalfWord(uint16_t address, uint16_t data);
/**
* \brief This function writes four bytes into stm eeprom memory.
*
* \param[in] address It's start write address value in eeprom space.
* \param[in] data There are data which will save in eeprom space
*/
void EE__WriteWord(uint16_t address, uint32_t data);


uint8_t ee_EraseEEPROM(uint8_t val);

uint8_t ee_VerifyEEPROM(uint8_t val);

#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_EE_EE_H */
