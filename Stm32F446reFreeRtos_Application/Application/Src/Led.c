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
void Led_Handler(void *pvParameters)
{
    LedMode_t currentMode = LED_MODE_NORMAL;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        if (xQueueReceive(xLedModeQueue, &currentMode, 0) == pdPASS)
        {
            // Mode updated
        }

        switch (currentMode)
        {
            case LED_MODE_SENSOR_FAIL:
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
                break;

            case LED_MODE_ADC_ERROR:
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
                break;

            case LED_MODE_NORMAL:
            default:
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
                break;
        }
    }
}


/******************************************************************************
*							LOCAL FUNCTION DEFINITIONS
******************************************************************************/


/******************************************************************************
*							EOF
******************************************************************************/

