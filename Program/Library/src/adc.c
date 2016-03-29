/** \file adc.c
* \brief This file contains definitions adc.
*
* \author Lukasz Radola
* \date 30.06.15
* 
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
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "motors.h"
/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */


/*! Local struct which contains adc measure result, adc operation flags etc. */
static volatile struct
{
	uint8_t isInitiated : 1;    /*!< Did ADC initialization make? */
	uint8_t is_Obstacle : 1;    /*jezeli jest przeszkoda to ta zmienna blokuje ustawianie pwm z tabletu*/
	uint8_t end_adc_measure : 1;
	uint16_t vrefcalib_value;  /*!< calib adc vref value*/
	uint16_t vdda_value;    /*!< stm power voltage value in mv */
	uint32_t vref_adc;    /*!< Vref value in ADC */
	uint32_t vref_mv;     /*!< Vref value in [mV] */
	int32_t temp_adc;    /*!< Temperature value in ADC */
	int32_t temp_degree;  /*!< Temperature value in [mV] */
	uint32_t sharp_przod_srodek_mv;
	uint32_t sharp_przod_lewy_mv;
	uint32_t sharp_przod_prawy_mv;
	uint32_t sharp_tyl_srodek_mv;
	uint16_t batt_mv;     /*!< Battery value in [mV] */
  
  uint32_t EE_Vref;
} adc__data_s = {0};


/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

int32_t ADC_array[NUMBER_OF_ADC_CHANNEL]; //Array to store the values coming from the ADC and copied by DMA
uint32_t CurrentChannel; //index on the ADC_array
/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/

/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */


/******************************* END FUNCTION *********************************/

/**
* \brief ADC initialization function.
*
* This init function sets adc register, system clock etc. 
*
*/
static void adc__Init(void)
{

  /* SetClockForADC */
  /* (1) Enable the peripheral clock of the ADC and SYSCFG */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_SYSCFGEN; /* (1) */

    /* ConfigureADC */

       /* (1) Select HSI16 by writing 00 in CKMODE (reset value) */
         /* (2) Select the continuous mode and scanning direction */
         /* (3) Select CHSEL10, CHSEL17 and CHSEL18, Select CHSEL18 for temperature sensor  */
    // resolution by default is 12-bit.
         /* (4) Select a sampling mode of 111 i.e. 239.5 ADC clk to be greater than 5 us */
         /* (5) Enable interrupts on EOC, EOSEQ and overrrun */
         /* (6) Wake-up the VREFINT (only for VLCD, Temp sensor and VRefInt) */
         /* (7) Enable VREFINT buffer  for ADC by setting EN_VREFINT and
                ENBUF_VREFINT_ADC bit in SYSCFG_CFGR3 and Enable Sensor buffer
                for ADC by setting both  ENBUF_SENSOR_ADC bit */
         /* (8) Wait for VREFINT ADC buffer ready */
         //ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE; /* (1) */
         ADC1->CFGR1 |= ADC_CFGR1_WAIT |ADC_CFGR1_CONT | ADC_CFGR1_SCANDIR; /* (2) */
         ADC1->CHSELR = ADC_CHSELR_CHSEL10 | ADC_CHSELR_CHSEL11 | ADC_CHSELR_CHSEL12 \
                      | ADC_CHSELR_CHSEL13 | ADC_CHSELR_CHSEL17 | ADC_CHSELR_CHSEL18; /* (3) */
         ADC1->SMPR |= ADC_SMPR_SMPR_0 | ADC_SMPR_SMPR_1 | ADC_SMPR_SMPR_2; /* (4) */
         ADC1->IER = ADC_IER_EOCIE | ADC_IER_EOSEQIE | ADC_IER_OVRIE; /* (5) */
         ADC->CCR |= ADC_CCR_VREFEN | ADC_CCR_TSEN;  /* (6) */
         SYSCFG->CFGR3 |= SYSCFG_CFGR3_EN_VREFINT
                        | SYSCFG_CFGR3_ENBUF_VREFINT_ADC
                        | SYSCFG_CFGR3_ENBUF_SENSOR_ADC;/* (7) */
         while ((SYSCFG->CFGR3 & SYSCFG_VREFINT_ADC_RDYF &&
                 SYSCFG->CFGR3 & SYSCFG_CFGR3_SENSOR_ADC_RDYF) == 0) /* (8) */

         {
           /* For robust implementation, add here time-out management */
         }

         /* Configure NVIC for ADC */
         /* (1) Enable Interrupt on ADC */
         /* (2) Set priority for ADC */
         NVIC_EnableIRQ(ADC1_COMP_IRQn); /* (1) */
         NVIC_SetPriority(ADC1_COMP_IRQn,0); /* (2) */

       /* Calibrate ADC */
        /* (1) Ensure that ADEN = 0 */
        /* (2) Clear ADEN */
        /* (3) Set ADCAL=1 */
        /* (4) Wait until EOCAL=1 */
        /* (5) Clear EOCAL */
        if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
        {
         ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);  /* (2) */
        }
        ADC1->CR |= ADC_CR_ADCAL; /* (3) */
        while ((ADC1->ISR & ADC_ISR_EOCAL) == 0) /* (4) */
        {
         /* For robust implementation, add here time-out management */
        }
        ADC1->ISR |= ADC_ISR_EOCAL; /* (5) */


       /* (1) Enable the ADC */
       /* (2) Wait until ADC ready if AUTOFF is not set */
       ADC1->CR |= ADC_CR_ADEN; /* (1) */
       if ((ADC1->CFGR1 &  ADC_CFGR1_AUTOFF) == 0)
       {
        while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* (2) */
        {
          /* For robust implementation, add here time-out management */
        }
       }

       adc__data_s.vrefcalib_value = *VREFINT_CAL_ADDR;
       adc__data_s.isInitiated = 1;

}


/******************************* END FUNCTION *********************************/

/**
* \brief Convert function: adc_temperature to temperature in degree.
*
* This function converts temperature value in ADC to temperature in Celsius degree. 
* The temperature value is saving in adc_data.temp_degree.
*
* \param[in] adc_temperature Temperature value in ADC.
*/
/**
  * Brief   This function computes the temperature from the temperature sensor measure
  *         The computation uses the following formula :
  *         Temp = (Vsense - V30)/Avg_Slope + 30
  *         Avg_Slope = (V130 - V30) / (130 - 30)
  *         V30 = Vsense @30°C (calibrated in factory @3V)
  *         V130 = Vsense @110°C (calibrated in factory @3V)
  *         VDD_APPLI/VDD_CALIB coefficient allows to adapt the measured value
  *         according to the application board power supply versus the
  *         factory calibration power supply.
  * Param   measure is the a voltage measured from the temperature sensor (can have been filtered)
  * Retval  returns the computed temperature
  */
static int32_t adc__ConvertTemperature(int32_t measure)
{
  int32_t temperature;

  temperature = ((measure * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR ) ;
  temperature = temperature * (int32_t)(130 - 30);
  temperature = temperature / (int32_t)(*TEMP130_CAL_ADDR - *TEMP30_CAL_ADDR);
  temperature = temperature + 30;
  return(temperature);
}


/******************************* END FUNCTION *********************************/

void ADC__DeInit(void)
{
  /* (1) Ensure that no conversion on going */
  /* (2) Stop any ongoing conversion */
  /* (3) Wait until ADSTP is reset by hardware i.e. conversion is stopped */
  /* (4) Disable the ADC */
  /* (5) Wait until the ADC is fully disabled */
  if ((ADC1->CR & ADC_CR_ADSTART) != 0) /* (1) */
  {
    ADC1->CR |= ADC_CR_ADSTP; /* (2) */
  }
  while ((ADC1->CR & ADC_CR_ADSTP) != 0) /* (3) */
  {
     /* For robust implementation, add here time-out management */
  }
  ADC1->CR |= ADC_CR_ADDIS; /* (4) */
  while ((ADC1->CR & ADC_CR_ADEN) != 0) /* (5) */
  {
    /* For robust implementation, add here time-out management */
  }

  adc__data_s.isInitiated = 0;
}


/******************************* END FUNCTION *********************************/

void ADC__MeasureAllAdc(void)
{
    adc__Init();

    if ((ADC1->CR & ADC_CR_ADSTART) != 0) /* Check if conversion on going */
    {
      ADC1->CR |= ADC_CR_ADSTP; /* Stop the sequence conversion */
    }
    else
    {
      ADC1->CFGR1 ^= ADC_CFGR1_SCANDIR;  /* Toggle SCANDIR */
      CurrentChannel = 0;
      ADC1->CR |= ADC_CR_ADSTART; /* Restart the sequence conversion */
    }



}

/******************************* END FUNCTION *********************************/

void ADC__UpdateAdcStruct(int32_t* data)
{
    static uint8_t vdd_vref_measure = 0;

    static uint8_t number_of_sample = 0;
    static uint32_t sharp_przod_srodek_temp = 0;
    static uint32_t sharp_przod_lewy_temp = 0;
    static uint32_t sharp_przod_prawy_temp = 0;
    static uint32_t sharp_tyl_srodek_temp = 0;


    number_of_sample++;
    if(!vdd_vref_measure)
    {
      vdd_vref_measure = 1; // tylko raz podczas dzialania programu bedzie dokonany pomiar vref
      adc__data_s.vdda_value = (3*1000*adc__data_s.vrefcalib_value/data[VREF]);

      adc__data_s.vref_adc = data[VREF];
      adc__data_s.vref_mv = ((adc__data_s.vdda_value * adc__data_s.vref_adc)/4095);
    }

    adc__data_s.temp_adc = data[TEMPERATURA];
    adc__data_s.temp_degree = adc__ConvertTemperature(adc__data_s.temp_adc);

    sharp_przod_srodek_temp += data[SHARP_PRZOD_SRODEK];
    sharp_przod_lewy_temp += data[SHARP_PRZOD_LEWY];
    sharp_przod_prawy_temp += data[SHARP_PRZOD_PRAWY];
    sharp_tyl_srodek_temp += data[SHARP_TYL_SRODEK];



    if(number_of_sample >= 32)
    {
      number_of_sample = 0;

      adc__data_s.sharp_przod_srodek_mv = ((adc__data_s.vdda_value *
                                          (sharp_przod_srodek_temp>>5)) / 4095);
      adc__data_s.sharp_przod_lewy_mv = ((adc__data_s.vdda_value *
                                          (sharp_przod_lewy_temp>>5)) / 4095);
      adc__data_s.sharp_przod_prawy_mv = ((adc__data_s.vdda_value *
                                          (sharp_przod_prawy_temp>>5)) / 4095);
      adc__data_s.sharp_tyl_srodek_mv = ((adc__data_s.vdda_value *
                                          (sharp_tyl_srodek_temp>>5)) / 4095);

      sharp_przod_srodek_temp = 0;
      sharp_przod_lewy_temp = 0;
      sharp_przod_prawy_temp = 0;
      sharp_tyl_srodek_temp = 0;

    }


    adc__data_s.end_adc_measure = 1;

}


/******************************* END FUNCTION *********************************/

int32_t ADC__GetSharp1MvValue(void)
{

  return adc__data_s.sharp_przod_srodek_mv;
}
/******************************* END FUNCTION *********************************/

int32_t ADC__GetSharpPrzodLewyMvValue(void)
{

  return adc__data_s.sharp_przod_lewy_mv;
}

/******************************* END FUNCTION *********************************/

int32_t ADC__GetSharpPrzodPrawyMvValue(void)
{

  return adc__data_s.sharp_przod_prawy_mv;
}

/******************************* END FUNCTION *********************************/

int32_t ADC__GetSharpTylSrodekMvValue(void)
{

  return adc__data_s.sharp_tyl_srodek_mv;
}
/******************************* END FUNCTION *********************************/

int32_t ADC__GetVrefAdcValue(void)
{

  return adc__data_s.vref_adc;
}

/******************************* END FUNCTION *********************************/

int32_t ADC__GetTempAdcValue(void)
{

  return adc__data_s.temp_adc;
}

/******************************* END FUNCTION *********************************/

int32_t ADC__GetTempDegreeValue(void)
{

  return adc__data_s.temp_degree;
}

/******************************* END FUNCTION *********************************/

void ADC__SetIsObstacleFlag(void)
{

  adc__data_s.is_Obstacle = 1;
}

/******************************* END FUNCTION *********************************/

void ADC__ResetIsObstacleFlag(void)
{

  adc__data_s.is_Obstacle = 0;
}

/******************************* END FUNCTION *********************************/

uint8_t ADC__GetIsObstacleFlag(void)
{

  return adc__data_s.is_Obstacle;
}


void ADC__Poll(void)
{
  uint8_t i = 0;
  uint8_t counter = 0;

  if(timer__data_u.time_adc_2ms_flag)
  {
    timer__data_u.time_adc_2ms_flag = 0;

    // deinicjalizacja jest po pomiarach
    if(!adc__data_s.end_adc_measure && !adc__data_s.isInitiated)
    {
      ADC__MeasureAllAdc();
    }
    else
    {
      adc__data_s.end_adc_measure = 0;
      // vref oraz temp zawsze musza byc na koncu, by ponizsze dzialalo
      for(i = 0; i < NUMBER_OF_SHARP; i++)
      {
        if(ADC_array[i] > 2500 && timer__data_u.time_pwm_is_on)
        {
          MOTORS__jazda_zatrzymana();
          switch(i)
          {
            case SHARP_PRZOD_SRODEK:
            {
              TIMER__PWM_DC1_2_Change_Duty(40);
              MOTORS__jazda_do_tylu();
              UART__SetSharp1ToSend();
              //tutaj beda tylko flagi ustawiane i w innym pollingu bedzie wykonywane cofanie, omijanie
              break;
            }
            case SHARP_PRZOD_LEWY:
            {
              TIMER__PWM_DC1_2_Change_Duty(40);
              MOTORS__jazda_do_tylu();
              UART__SetSharpLewyPrzodToSend();
              break;
            }
            case SHARP_PRZOD_PRAWY:
            {
              TIMER__PWM_DC1_2_Change_Duty(40);
              MOTORS__jazda_do_tylu();
              UART__SetSharpPrawyPrzodToSend();

              break;
            }
            case SHARP_TYL_SRODEK:
            {
              TIMER__PWM_DC1_2_Change_Duty(40);
              MOTORS__jazda_do_przodu();
              UART__SetSharpSrodekTylToSend();
              break;
            }
          }
        }
        else if(ADC_array[i] > 2400)
        {
          if(timer__data_u.time_pwm_last_value > 40)
          {
            TIMER__PWM_DC1_2_Change_Duty(40);
          }

          adc__data_s.is_Obstacle = 1;
        }
        else if(ADC_array[i] > 2100)
        {
          if(timer__data_u.time_pwm_last_value > 50)
          {
            TIMER__PWM_DC1_2_Change_Duty(50);
          }

          adc__data_s.is_Obstacle = 1;
        }

        else if(ADC_array[i] > 1800)
        {
          if(timer__data_u.time_pwm_last_value > 80)
          {
            TIMER__PWM_DC1_2_Change_Duty(80);
          }
          adc__data_s.is_Obstacle = 1;
        }
        else
        {
          counter++;
        }
      }

      if(counter >= NUMBER_OF_SHARP)
      {
        adc__data_s.is_Obstacle = 0;
      }
      //counter kasowany automatyczznie bo zmienna lokalna
    }
  }
}

#ifdef __cplusplus
  }
#endif
