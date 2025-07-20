/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : app.c
  * @brief          : application program body
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

/******************************************************************************/
#include "App.h"
/******************************************************************************
*							INCLUDES
******************************************************************************/

/******************************************************************************
*							GLOBAL VARIABLES
******************************************************************************/
extern UART_HandleTypeDef huart2;

QueueHandle_t xLedModeQueue = NULL;
QueueHandle_t xLcdQueue     = NULL;
/******************************************************************************
*							LOCAL FUNCTION DECLARATIONS
******************************************************************************/
static void App_Init(void);


/******************************************************************************
*							CONST DECLARATIONS
******************************************************************************/

/******************************************************************************
*							API IMPLEMENTATION
******************************************************************************/
void App_Run(void)
{
	/* Application specific initializations */


	/* Creating the tasks for the Application */
    App_Init();

    /* Start the FreeRTOS Scheduler */
    vTaskStartScheduler();
}

/******************************************************************************
*							LOCAL FUNCTION DEFINITIONS
******************************************************************************/
// Create tasks
void App_Init(void)
{
    // Create Queues
    xLedModeQueue = xQueueCreate(5, sizeof(LedMode_t));
    if (xLedModeQueue == NULL)
    {
        printf("Failed to create LED queue!\r\n");
    }

    xLcdQueue = xQueueCreate(5, sizeof(LcdMessage_t));
    if (xLcdQueue == NULL)
    {
        printf("Failed to create LCD queue!\r\n");
    }

    // Create Tasks with adjusted priorities and stack
    BaseType_t status;

    status = xTaskCreate(Led_Handler, "LED", 128, NULL, 1, NULL);
    if (status != pdPASS) printf("LED Task creation failed!\r\n");

    status = xTaskCreate(LM35_Handler, "LM35", 128, NULL, 2, NULL);  // Highest priority
    if (status != pdPASS) printf("LM35 Task creation failed!\r\n");

    status = xTaskCreate(Lcd16x2_Handler, "LCD", 512, NULL, 1, NULL);  // Increased stack
    if (status != pdPASS) printf("LCD Task creation failed!\r\n");
}



int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}


/* RTOS - Hooks */

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Log error (e.g., to UART or LED)
    printf("Stack overflow in task: %s\r\n", pcTaskName);

    // Optionally blink an LED or halt system
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);  // Toggle onboard LED
        HAL_Delay(250);  // Use HAL for blinking to avoid blocking FreeRTOS
    }
}




/******************************************************************************
*							EOF
******************************************************************************/

