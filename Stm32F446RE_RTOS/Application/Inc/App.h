/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : App.h
  * @brief          : Header for App.c file.
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

#ifndef SRC_APP_H_
#define SRC_APP_H_


/******************************************************************************
*							INCLUDES
******************************************************************************/
/* Library headers */
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdint.h>

/* Free RTOS - Headers */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Application Headers */
#include "Led.h"
#include "Lm35.h"
/******************************************************************************
*							MACRO DEFINITION
******************************************************************************/

/******************************************************************************
*							DATA TYPE DECLARATION
******************************************************************************/


/******************************************************************************
*							API DECLARATIONS
******************************************************************************/
void App_Run(void);

int __io_putchar(int ch);

/******************************************************************************
*							EOF
******************************************************************************/
#endif /* SRC_APP_H_ */


