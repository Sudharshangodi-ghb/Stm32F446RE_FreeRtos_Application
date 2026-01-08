/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Button.c
  * @brief          : User button handler (PC13)
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
  * 	08-01-2026	-	v01	- Initial version
  *
  *
  *
  ******************************************************************************
  */

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "Button.h"
#include "App.h"
#include "stm32f4xx_hal.h"

/******************************************************************************
* GLOBALS
******************************************************************************/
static SemaphoreHandle_t xButtonSemaphore;

/******************************************************************************
* LOCAL VARIABLES
******************************************************************************/
static TickType_t pressStartTick = 0;
static uint8_t pressCount = 0;

/******************************************************************************
* API IMPLEMENTATION
******************************************************************************/
void Button_Init(void)
{
    xButtonSemaphore = xSemaphoreCreateBinary();
}

void Button_ISR_Callback(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Button_Task(void *params)
{
    TickType_t releaseTick;
    TickType_t lastReleaseTick = 0;
    uint32_t pressDurationMs;

    while (1)
    {
        /* Wait for button press signal */
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdPASS)
        {
            pressStartTick = xTaskGetTickCount();

            /* Wait while button is held */
            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            releaseTick = xTaskGetTickCount();
            pressDurationMs =
                (releaseTick - pressStartTick) * portTICK_PERIOD_MS;

            /* LONG PRESS */
            if (pressDurationMs >= BUTTON_LONG_PRESS_MS)
            {
                App_HandleButtonEvent(APP_BUTTON_LONG_PRESS);
                pressCount = 0;
                continue;
            }

            /* Ignore bounce */
            if (pressDurationMs < BUTTON_DEBOUNCE_MS)
            {
                continue;
            }

            /* SHORT / DOUBLE PRESS HANDLING */
            pressCount++;

            if (pressCount == 1)
            {
                lastReleaseTick = releaseTick;
            }
            else if (pressCount == 2)
            {
                if ((releaseTick - lastReleaseTick) *
                    portTICK_PERIOD_MS <= BUTTON_DOUBLE_GAP_MS)
                {
                    App_HandleButtonEvent(APP_BUTTON_DOUBLE_PRESS);
                    pressCount = 0;
                }
                else
                {
                    pressCount = 1;
                    lastReleaseTick = releaseTick;
                }
            }

            /* Wait to see if second press comes */
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DOUBLE_GAP_MS));

            if (pressCount == 1)
            {
                App_HandleButtonEvent(APP_BUTTON_SHORT_PRESS);
                pressCount = 0;
            }
        }
    }
}

/******************************************************************************
*							LOCAL FUNCTION DEFINITIONS
******************************************************************************/


/******************************************************************************
*							EOF
******************************************************************************/

