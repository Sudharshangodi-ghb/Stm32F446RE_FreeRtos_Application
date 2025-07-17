/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Led.h
  * @brief          : Header for led.c file.
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
  *
  *
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef INC_LED_H_
#define INC_LED_H_

/******************************************************************************
*							INCLUDES
******************************************************************************/

/******************************************************************************
*							MACRO DEFINITION
******************************************************************************/
typedef enum
{
    LED_MODE_NORMAL = 0,        // Regular 100ms on/off
    LED_MODE_SENSOR_FAIL,       // 300ms on/off
    LED_MODE_ADC_ERROR          // 600ms on/off
} LedMode_t;

/******************************************************************************
*							DATA TYPE DECLARATION
******************************************************************************/


/******************************************************************************
*							API DECLARATIONS
******************************************************************************/

/* LedTask_Handler */
void LedTask_Handler(void *params);

/******************************************************************************
*							EOF
******************************************************************************/


#endif /* INC_LED_H_ */
