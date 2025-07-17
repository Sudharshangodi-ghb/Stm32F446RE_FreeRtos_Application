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

QueueHandle_t xLedModeQueue;
/******************************************************************************
*							LOCAL FUNCTION DECLARATIONS
******************************************************************************/
static void Create_Tasks(void);


/* Task lists */
void Task_ADC_Read(void *params);

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
    Create_Tasks();

    /* Start the FreeRTOS Scheduler */
    vTaskStartScheduler();
}

/******************************************************************************
*							LOCAL FUNCTION DEFINITIONS
******************************************************************************/
static void Create_Tasks(void)
{

	xTaskCreate(LedTask_Handler,     "LED",   128, NULL, 1, NULL);
	xTaskCreate(LM35_Handler,        "LM35",  128, NULL, 1, NULL);
//	xTaskCreate(Task_PWM_Servo,      "Servo", 128, NULL, 1, NULL);
//	xTaskCreate(Task_UART_Comm,      "UART",  128, NULL, 1, NULL);
//	xTaskCreate(Task_CAN_Send,       "CAN",   128, NULL, 1, NULL);
//	xTaskCreate(Task_SPI_MPU6050,    "SPI",   128, NULL, 1, NULL);
//	xTaskCreate(Task_I2C_LCD,        "LCD",   128, NULL, 1, NULL);


	/* Create Queues */
	xLedModeQueue = xQueueCreate(5, sizeof(LedMode_t));
	if (xLedModeQueue == NULL)
	{
	    // Handle queue creation failure
	}


}

int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}



/******************************************************************************
*							EOF
******************************************************************************/

