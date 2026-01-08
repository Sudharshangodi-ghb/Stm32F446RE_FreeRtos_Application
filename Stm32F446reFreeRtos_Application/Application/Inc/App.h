/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : App.h
  * @brief          : Application layer header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Sudharshan Godi.
  * All rights reserved.
  *
  * History:
  *   17-07-2025  v01  Initial version
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef INC_APP_H_
#define INC_APP_H_

/******************************************************************************
*                               INCLUDES
******************************************************************************/

/* Standard C headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* STM32 HAL */
#include "stm32f4xx_hal.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Application modules */
#include "Led.h"
#include "Lcd16x2.h"
#include "Lm35.h"
#include "button.h"

/******************************************************************************
*                               DATA TYPE DEFINITIONS
******************************************************************************/

typedef enum
{
    APP_BUTTON_SHORT_PRESS = 0,
    APP_BUTTON_DOUBLE_PRESS,
    APP_BUTTON_LONG_PRESS
} AppButtonEvent_t;

/******************************************************************************
*                               API DECLARATIONS
******************************************************************************/

/* Application entry */
void App_Run(void);

/* printf redirection */
int __io_putchar(int ch);


void App_HandleButtonEvent(AppButtonEvent_t event);


#endif /* INC_APP_H_ */
