
  
 /** \file state_machine.c
* \brief This file contains production engine which runs functions in according to
* current state.
*
* \author Lukasz Radola
* \date 23.08.15
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


#include "state_machine.h"

/****************************************************************************/
/*                      DECLARATION AND DEFINITIONS                         */
/****************************************************************************/
/* Macro and const variable definition (local) */

/* Typedef definition (local) */
/* ... */

/*! Local struct which contains important informations of flood device  etc. */

/****************************************************************************/
/*                         GLOBAL VARIABLE DECLARATION                      */
/****************************************************************************/

/* Global variable declaration */

/*! Global Enum variable which contain names each soft production engine states */
volatile enum currentState_e currentSTATE = CURRENT_STATE_NOTHING;

 /*! Non export global variable. It is 1 ms flag which is set by function in 
timer file */



/****************************************************************************/
/*                  FUNCTIONS DECLARATIONS AND DEFINITIONS                  */
/****************************************************************************/
/* Static function declaration */
/* ... */
/* Functions definitions (1. Static functions 2. Local exported functions */
/* 3. Interface (exported) functions) */
/* ... */


void STATEMACHINE__PollFunction(void)
{
  
  switch(currentSTATE)
  {
/*================== CURRENT_STATE_NOTHING ==================*/		
		case CURRENT_STATE_NOTHING:
    {
			break;
    }
/*================== CURRENT_STATE_ERROR ==================*/		
		case CURRENT_STATE_ERROR:
    {
			break;
    }
/*================== CURRENT_STATE_CALIBRATION_RTC ==================*/				
		case CURRENT_STATE_CALIBRATION_RTC:
    {
			currentSTATE = CURRENT_STATE_NOTHING;
			break;
    }
/*================== CURRENT_STATE_MEASURE_TEMPERATURE ==================*/					
		case CURRENT_STATE_MEASURE_TEMPERATURE:
    {

			currentSTATE = CURRENT_STATE_NOTHING;
			break;
    }

/*================== CURRENT_STATE_MEASURE_VBAT ==================*/			
		case CURRENT_STATE_MEASURE_VBAT:
    {
			currentSTATE = CURRENT_STATE_NOTHING;
			break;
    }
/*================== CURRENT_STATE_SLEEP ==================*/				
		case CURRENT_STATE_SLEEP:
    {

			currentSTATE = CURRENT_STATE_NOTHING;

			break;
    }
    

/*================== CURRENT_STATE_EE_WRITE ==================*/		
    case CURRENT_STATE_EE_WRITE:
    {
      
      currentSTATE = CURRENT_STATE_NOTHING;
      break;
    }
  }
    
}

/******************************* END FUNCTION *********************************/

#ifdef __cplusplus
  }
#endif




