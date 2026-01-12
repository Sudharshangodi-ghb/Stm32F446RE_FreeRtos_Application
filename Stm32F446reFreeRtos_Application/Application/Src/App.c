/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : App.c
  * @brief          : Application program body
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

/******************************************************************************
*                               INCLUDES
******************************************************************************/
#include "App.h"
#include "semphr.h"


#define BOOT_REQUEST_MAGIC   0xB007B007U
#define BOOT_REQUEST_ADDR    ((uint32_t *)0x40024000)

/******************************************************************************
*                               GLOBAL VARIABLES
******************************************************************************/

/* UART handle for printf */
extern UART_HandleTypeDef huart2;

/* Application Queues */
QueueHandle_t xLedModeQueue = NULL;
QueueHandle_t xTempQueue    = NULL;

/* Application Mutexes */
static SemaphoreHandle_t xUartMutex = NULL;


/******************************************************************************
*                               LOCAL FUNCTION DECLARATIONS
******************************************************************************/
static void App_Init(void);

/******************************************************************************
*                               API IMPLEMENTATION
******************************************************************************/

/**
 * @brief  Application entry point
 */
void App_Run(void)
{
    /* Application initialization */
    App_Init();

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    while (1)
    {
    }
}


void App_HandleButtonEvent(AppButtonEvent_t event)
{
    switch (event)
    {
        case APP_BUTTON_SHORT_PRESS:
            printf("Button: SHORT press\r\n");
            break;

        case APP_BUTTON_DOUBLE_PRESS:
            printf("Button: DOUBLE press\r\n");
            break;

        case APP_BUTTON_LONG_PRESS:
            printf("Button: LONG press → Boot mode\r\n");

            __HAL_RCC_PWR_CLK_ENABLE();
            HAL_PWR_EnableBkUpAccess();
            __HAL_RCC_BKPSRAM_CLK_ENABLE();

            *BOOT_REQUEST_ADDR = BOOT_REQUEST_MAGIC;

            vTaskDelay(pdMS_TO_TICKS(100));
            NVIC_SystemReset();
            break;

        default:
            break;
    }
}


/******************************************************************************
*                               LOCAL FUNCTION DEFINITIONS
******************************************************************************/

/**
 * @brief  Initialize RTOS objects and tasks
 */
static void App_Init(void)
{
    BaseType_t status;

    /* ---------------- Queue Creation ---------------- */

    /* LED mode queue (latest state only) */
    xLedModeQueue = xQueueCreate(1, sizeof(LedMode_t));
    if (xLedModeQueue == NULL)
    {
        printf("ERROR: LED queue creation failed\r\n");
    }

    /* Temperature queue (latest value only) */
    xTempQueue = xQueueCreate(1, sizeof(float));
    if (xTempQueue == NULL)
    {
        printf("ERROR: Temperature queue creation failed\r\n");
    }

    /* ---------------- Task Creation ---------------- */

    /* LED Task – periodic, low priority */
    status = xTaskCreate(
                Led_Handler,
                "LED_Task",
                128,
                NULL,
                1,
                NULL);
    if (status != pdPASS)
    {
        printf("ERROR: LED task creation failed\r\n");
    }

    /* LM35 Task – periodic, highest priority */
    status = xTaskCreate(
                LM35_Handler,
                "LM35_Task",
                256,
                NULL,
                2,
                NULL);
    if (status != pdPASS)
    {
        printf("ERROR: LM35 task creation failed\r\n");
    }

    /* LCD Task – event-driven, low priority */
    status = xTaskCreate(
                Lcd16x2_Handler,
                "LCD_Task",
                512,
                NULL,
                1,
                NULL);
    if (status != pdPASS)
    {
        printf("ERROR: LCD task creation failed\r\n");
    }

    /* Init the button module */
    Button_Init();

    /* Create Button task */
    xTaskCreate(Button_Task,
                "BUTTON",
                256,
                NULL,
                2,
                NULL);


    /* ---------------- Mutex Creation ---------------- */

    /* UART mutex (supports priority inheritance) */
    xUartMutex = xSemaphoreCreateMutex();
    if (xUartMutex == NULL)
    {
        printf("ERROR: UART mutex creation failed\r\n");
    }
}

/******************************************************************************
*                               LOW-LEVEL IO
******************************************************************************/

/**
 * @brief  Redirect printf to UART2
 */
int __io_putchar(int ch)
{
    if (xUartMutex != NULL)
    {
        xSemaphoreTake(xUartMutex, portMAX_DELAY);
    }

    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);

    if (xUartMutex != NULL)
    {
        xSemaphoreGive(xUartMutex);
    }

    return ch;
}


/******************************************************************************
*                               RTOS HOOKS
******************************************************************************/

/**
 * @brief  Stack overflow hook
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char *pcTaskName)
{
    printf("RTOS Stack Overflow: %s\r\n", pcTaskName);

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(250);
    }
}

/******************************************************************************
*                               EOF
******************************************************************************/
