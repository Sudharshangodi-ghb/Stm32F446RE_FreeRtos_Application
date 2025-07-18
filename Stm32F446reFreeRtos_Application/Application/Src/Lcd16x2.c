/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Lcd16x2.c
  * @brief          : Lcd16x2 Handler
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
  * 	18-07-2025	-	v01	- Initial version
  *
  *
  *
  ******************************************************************************
  */

/******************************************************************************
*							INCLUDES
******************************************************************************/
#include "Lcd16x2.h"
#include "App.h"
/******************************************************************************
*							GLOBAL VARIABLES
******************************************************************************/
extern QueueHandle_t xLcdQueue;
extern I2C_HandleTypeDef hi2c3;

/******************************************************************************
*							LOCAL FUNCTION DECLARATIONS
******************************************************************************/
static void LCD_Send_Cmd(uint8_t cmd);
static void LCD_Send_Data(uint8_t data);
static void LCD_Send_4Bits(uint8_t data);
static void LCD_Enable_Pulse(uint8_t data);
static void LCD_Delay(uint16_t us);

static void LCD_Init(void);
static void LCD_Clear(void);
static void LCD_Set_Cursor(uint8_t row, uint8_t col);
static void LCD_Send_String(char *str);

/******************************************************************************
*							CONST DECLARATIONS
******************************************************************************/


/******************************************************************************
*							API IMPLEMENTATION
******************************************************************************/
void Lcd16x2_Handler(void *params)
{
    LcdMessage_t msg;

    HAL_StatusTypeDef res;
    uint8_t i;

    for (i = 1; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(&hi2c3, (i << 1), 1, 10);
        if (res == HAL_OK) {
            printf("I2C device found at 0x%X\r\n", i << 1);
        }
    }

    LCD_Init();
    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_Send_String(" LCD Ready ");

    while(1)
    {
        if (xQueueReceive(xLcdQueue, &msg, portMAX_DELAY) == pdPASS)
        {
            LCD_Clear();
            LCD_Set_Cursor(0, 0);
            LCD_Send_String(msg.line1);
            LCD_Set_Cursor(1, 0);
            LCD_Send_String(msg.line2);
        }

    }
}


/******************************************************************************
*							LOCAL FUNCTION DEFINITIONS
******************************************************************************/


static void LCD_Init(void)
{
    HAL_Delay(50);  // Wait for LCD power-up

    // Init 4-bit mode
    LCD_Send_4Bits(0x30);
    HAL_Delay(5);
    LCD_Send_4Bits(0x30);
    HAL_Delay(1);
    LCD_Send_4Bits(0x30);
    LCD_Send_4Bits(0x20);  // Set to 4-bit mode

    // Function set: 4-bit, 2 line, 5x8 dots
    LCD_Send_Cmd(0x28);
    // Display ON/OFF
    LCD_Send_Cmd(0x0C);
    // Clear Display
    LCD_Send_Cmd(0x01);
    HAL_Delay(2);
    // Entry mode set
    LCD_Send_Cmd(0x06);
}


static void LCD_Clear(void)
{
    LCD_Send_Cmd(0x01);
    HAL_Delay(2);
}

static void LCD_Set_Cursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Send_Cmd(address);
}

static void LCD_Send_String(char *str)
{
    while (*str)
    {
        LCD_Send_Data((uint8_t)(*str++));
    }
}

static void LCD_Send_Cmd(uint8_t cmd)
{
    uint8_t high_nibble = (cmd & 0xF0);
    uint8_t low_nibble = ((cmd << 4) & 0xF0);
    LCD_Send_4Bits(high_nibble | BACKLIGHT);
    LCD_Send_4Bits(low_nibble | BACKLIGHT);
}

static void LCD_Send_Data(uint8_t data)
{
    uint8_t high_nibble = (data & 0xF0) | REGISTER_SEL;
    uint8_t low_nibble = ((data << 4) & 0xF0) | REGISTER_SEL;
    LCD_Send_4Bits(high_nibble | BACKLIGHT);
    LCD_Send_4Bits(low_nibble | BACKLIGHT);
}

static void LCD_Send_4Bits(uint8_t data)
{
    uint8_t data_t[1];
    data_t[0] = data | LCD_ENABLE;
    HAL_I2C_Master_Transmit(&hi2c3, LCD_ADDR, data_t, 1, HAL_MAX_DELAY);
    LCD_Enable_Pulse(data);
}

static void LCD_Enable_Pulse(uint8_t data)
{
    uint8_t data_t[1];
    data_t[0] = data & ~LCD_ENABLE;
    HAL_I2C_Master_Transmit(&hi2c3, LCD_ADDR, data_t, 1, HAL_MAX_DELAY);
    LCD_Delay(50);
}

static void LCD_Delay(uint16_t us)
{
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - startTick) < delayTicks);
}


/******************************************************************************
*							EOF
******************************************************************************/

