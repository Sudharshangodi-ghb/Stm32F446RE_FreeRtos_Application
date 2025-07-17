/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Led.c
  * @brief          : Led Handler
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
  *History: v01
  * 	17-07-2025	-	v01	- Initial version
  *
  *
  *
  ******************************************************************************
  */

/******************************************************************************
*							INCLUDES
******************************************************************************/
#include "led.h"
#include "App.h"
/******************************************************************************
*							GLOBAL VARIABLES
******************************************************************************/

/* FreeRtos Queue for LED control */
extern QueueHandle_t xLedModeQueue;



/******************************************************************************
*							LOCAL FUNCTION DECLARATIONS
******************************************************************************/

/******************************************************************************
*							CONST DECLARATIONS
******************************************************************************/


/******************************************************************************
*							API IMPLEMENTATION
******************************************************************************/
void LedTask_Handler(void *params)
{
    uint8_t current_mode = 0;
    TickType_t on_time = 300, off_time = 300;

    while (1)
    {
    	/* LED Pattern Handling */
        if (xQueueReceive(xLedModeQueue, &current_mode, pdMS_TO_TICKS(10)) == pdPASS)
        {
            switch (current_mode) {
                case 0: on_time = off_time = 100; break;
                case 1: on_time = off_time = 300; break;
                case 2: on_time = off_time = 600; break;
                default: on_time = off_time = 300; break;
            }

        }
        else
        {
        	/* Do nothing */
        }

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // LED ON
        vTaskDelay(pdMS_TO_TICKS(on_time));

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // LED OFF
        vTaskDelay(pdMS_TO_TICKS(off_time));
    }
}

/******************************************************************************
*							LOCAL FUNCTION DEFINITIONS
******************************************************************************/


/******************************************************************************
*							EOF
******************************************************************************/

