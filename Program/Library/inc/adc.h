/** \file adc.h
* \brief This file contains analog to digital converter functions.
*
* \author Lukasz Radola
* \date 30.06.15
*
* \details In this file are definitions, declarations of functions, which can  
* convert, measure analog value et cetera...
*/
#ifndef LRADOLA_ADC_ADC_H_
#define LRADOLA_ADC_ADC_H_

#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************/
/*                              FUNCTIONS LIST                              */
/****************************************************************************/
/*
void ADC__DeInit(void);
uint16_t ADC__CalcBattery(void);
int16_t ADC__CalcTemperature(void);

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
#include "adc.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition */

/* Temperature sensor calibration value address */
#define TEMP130_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007A))
#define VDD_CALIB ((uint16_t) (300))
#define VDD_APPLI ((uint16_t) (326))
/* Internal voltage reference calibration value address */
#define VREFINT_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF80078))
 /*! Special adress which contain Vref calibrate value */
#define NUMBER_OF_ADC_CHANNEL 6
#define NUMBER_OF_SHARP (NUMBER_OF_ADC_CHANNEL - 2)


#define ERROR_UNEXPECTED_ADC_IT 0x01
/* Typedef definition */

typedef enum adc_array
{
  SHARP_PRZOD_SRODEK = 0,
  SHARP_PRZOD_LEWY,
  SHARP_PRZOD_PRAWY,
  SHARP_TYL_SRODEK,
//  SHARP_TYL_LEWY,
//  SHARP_TYL_PRAWY,
//  SHARP_BOK_LEWY,
//  SHARP_BOK_PRAWY,
  VREF,
  TEMPERATURA

};
/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration - use it isn't recommended*/

extern int32_t ADC_array[]; //Array to store the values coming from the ADC and copied by DMA
extern uint32_t CurrentChannel; //index on the ADC_array
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
* \brief ADC deinitialization function.
*
* This deinit functions resets adc register, system clock etc. to default values.
*
*/
void ADC__DeInit(void);

void ADC__UpdateAdcStruct(int32_t* data);
void ADC__MeasureAllAdc(void);

int32_t ADC__GetSharp1MvValue(void);
int32_t ADC__GetVrefAdcValue(void);
int32_t ADC__GetTempAdcValue(void);
int32_t ADC__GetTempDegreeValue(void);
int32_t ADC__GetSharpPrzodLewyMvValue(void);
int32_t ADC__GetSharpPrzodPrawyMvValue(void);
int32_t ADC__GetSharpTylSrodekMvValue(void);

void ADC__SetIsObstacleFlag(void);
void ADC__ResetIsObstacleFlag(void);
uint8_t ADC__GetIsObstacleFlag(void);

void ADC__1msPoll(void);
#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_ADC_ADC_H_ */
