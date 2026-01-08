/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Lcd16x2.c
  * @brief          : Lcd16x2 Handler
  * @author         : Saturation Godi
  ******************************************************************************
  */

/******************************************************************************
*                            INCLUDES
******************************************************************************/
#include "Lcd16x2.h"
#include "App.h"

/******************************************************************************
*                            DEFINES
******************************************************************************/
#define LCD_I2C_TIMEOUT     20   // I2C timeout for each transfer
#define LCD_ADDR            (0x27 << 1)  // PCF8574 I2C address

#define DEBUG_I2C_SCAN

#define BACKLIGHT      0x08
#define LCD_ENABLE     0x04
#define REGISTER_SEL   0x01

/******************************************************************************
*                            GLOBAL VARIABLES
******************************************************************************/
extern QueueHandle_t xTempQueue;
extern I2C_HandleTypeDef hi2c3;

/******************************************************************************
*                            LOCAL FUNCTION DECLARATIONS
******************************************************************************/
static void LCD_Init(void);
static void LCD_Clear(void);
static void LCD_Set_Cursor(uint8_t row, uint8_t col);
static void LCD_Send_String(char *str);
static void LCD_Send_Cmd(uint8_t cmd);
static void LCD_Send_Data(uint8_t data);
static void LCD_Send_4Bits(uint8_t data);
static void LCD_Enable_Pulse(uint8_t data);

/******************************************************************************
*                            API IMPLEMENTATION
******************************************************************************/
void Lcd16x2_Handler(void *params)
{
    float temperature_c;
    LcdMessage_t lcdMsg;

    LCD_Init();
    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_Send_String(" LCD Ready ");

    while (1)
    {
        if (xQueueReceive(xTempQueue, &temperature_c, portMAX_DELAY) == pdPASS)
        {
            snprintf(lcdMsg.line1, 16, "Temp: %.1f C", temperature_c);
            snprintf(lcdMsg.line2, 16, "Sensor Status");

            LCD_Clear();
            LCD_Set_Cursor(0, 0);
            LCD_Send_String(lcdMsg.line1);
            LCD_Set_Cursor(1, 0);
            LCD_Send_String(lcdMsg.line2);
        }
    }
}

/******************************************************************************
*                            LOCAL FUNCTION DEFINITIONS
******************************************************************************/
static void LCD_Init(void)
{
    vTaskDelay(pdMS_TO_TICKS(50));  // Wait for LCD power-up

    // Force back light ON
    uint8_t backlight = BACKLIGHT;
    HAL_I2C_Master_Transmit(&hi2c3, LCD_ADDR, &backlight, 1, LCD_I2C_TIMEOUT);

    LCD_Send_4Bits(0x30);
    vTaskDelay(pdMS_TO_TICKS(5));
    LCD_Send_4Bits(0x30);
    vTaskDelay(pdMS_TO_TICKS(1));
    LCD_Send_4Bits(0x30);
    LCD_Send_4Bits(0x20);  // Set to 4-bit mode

    LCD_Send_Cmd(0x28);    // 4-bit, 2 lines, 5x8 font
    LCD_Send_Cmd(0x0C);    // Display ON, Cursor OFF
    LCD_Send_Cmd(0x01);    // Clear Display
    vTaskDelay(pdMS_TO_TICKS(2));
    LCD_Send_Cmd(0x06);    // Entry mode
}

static void LCD_Clear(void)
{
    LCD_Send_Cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}

static void LCD_Set_Cursor(uint8_t row, uint8_t col)
{
    LCD_Send_Cmd((row == 0) ? (0x80 + col) : (0xC0 + col));
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
    LCD_Send_4Bits(cmd & 0xF0);
    LCD_Send_4Bits((cmd << 4) & 0xF0);
}

static void LCD_Send_Data(uint8_t data)
{
    LCD_Send_4Bits((data & 0xF0) | REGISTER_SEL);
    LCD_Send_4Bits(((data << 4) & 0xF0) | REGISTER_SEL);
}

static void LCD_Send_4Bits(uint8_t data)
{
    uint8_t buf = data | BACKLIGHT | LCD_ENABLE;
    HAL_I2C_Master_Transmit(&hi2c3, LCD_ADDR, &buf, 1, LCD_I2C_TIMEOUT);
    vTaskDelay(pdMS_TO_TICKS(1));
    LCD_Enable_Pulse(data);
}

static void LCD_Enable_Pulse(uint8_t data)
{
    uint8_t buf = data | BACKLIGHT;
    HAL_I2C_Master_Transmit(&hi2c3, LCD_ADDR, &buf, 1, LCD_I2C_TIMEOUT);
}

/******************************************************************************
*                            EOF
******************************************************************************/
