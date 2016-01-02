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
#define VDD_APPLI ((uint16_t) (323))

 /*! Special adress which contain Vref calibrate value */


/* Typedef definition */


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration - use it isn't recommended*/

extern int32_t temperature_C; //contains the computed temperature
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

/**
* \brief Global function, which measures and returns baterry voltage.
*
* This function launches adc__MeasureBatt() function.
*
* \return  adc__data.batt_mv value is returned.
*/
uint16_t ADC__CalcBattery(void);

/**
* \brief Global function, which measures and returns temperature value.
*
* This function launches adc__MeasureTemp() function and converts adc temperature
* value into temperature in Celsius degree.
*
* \return  adc__data_s.temp_degree value is returned.
*/
int32_t ADC__CalcTemperature(void);


#ifdef __cplusplus
  }
#endif
#endif /* LRADOLA_ADC_ADC_H_ */
