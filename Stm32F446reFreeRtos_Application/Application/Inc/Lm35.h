/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Lm35.h
  * @brief          : Header for Lm35.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Sudharshan Godi.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  * History: v01
  * 	17-07-2025	-	v01	- Initial version
  *		21-07-2025	- 	Over temperature update to >50*C
  *
  *
  *	| Temp (°C) | Voltage (V) | ADC Value (12-bit @ 3.3V) |
  *	| --------- | ----------- | ------------------------- |
  *	| 0         | 0.0 V       | 0                         |
  *	| 10        | 0.1 V       | \~124                     |
  *	| 25        | 0.25 V      | \~310                     |
  *	| 40        | 0.4 V       | \~497                     |
  *	| 50        | 0.5 V       | \~621                     |
  *	| 60        | 0.6 V       | \~745                     |
  *	| 100       | 1.0 V       | \~1240                    |
  *
  *
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef INC_LM35_H_
#define INC_LM35_H_

/******************************************************************************
*							INCLUDES
******************************************************************************/
#include "App.h"
/******************************************************************************
*							MACRO DEFINITION
******************************************************************************/
#define LM35_ADC_TIMEOUT     100
#define LM35_DISCONNECT_ADC  30     // ADC value below this = sensor fault
#define LM35_SAMPLING_DELAY  1000   // 1 second
#define LM35_OVERTEMPERATURE_ADC 625



/******************************************************************************
*							DATA TYPE DECLARATION
******************************************************************************/
typedef struct
{
    uint32_t adc_raw;
    float temperature_c;
    bool adc_timeout_error;
    bool sensor_disconnected;
} LM35_Data_t;

/******************************************************************************
*							API DECLARATIONS
******************************************************************************/

//LM35 handler
void LM35_Handler(void *pvParameters);

// Read-only access for the Sensor Data
const LM35_Data_t* LM35_GetData(void);

/******************************************************************************
*							EOF
******************************************************************************/


#endif /* INC_LM35_H_ */
