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
#define READ_WRITE     0x02
#define REGISTER_SEL   0x01

/******************************************************************************
*                            GLOBAL VARIABLES
******************************************************************************/
extern QueueHandle_t xTempQueue;
extern I2C_HandleTypeDef hi2c3;

/******************************************************************************
*                            LOCAL FUNCTION DECLARATIONS
******************************************************************************/
static void LCD_Send_Cmd(uint8_t cmd);
static void LCD_Send_Data(uint8_t data);
static void LCD_Send_4Bits(uint8_t data);
static void LCD_Enable_Pulse(uint8_t data);

static void LCD_Init(void);
static void LCD_Clear(void);
static void LCD_Set_Cursor(uint8_t row, uint8_t col);
static void LCD_Send_String(char *str);

/******************************************************************************
*                            API IMPLEMENTATION
******************************************************************************/
void Lcd16x2_Handler(void *params)
{
    LcdMessage_t lcdMsg;
    float temperature_c;

    TickType_t xLastWakeTime = xTaskGetTickCount();

#ifdef DEBUG_I2C_SCAN
    // Optional I2C scan for debugging only
    HAL_StatusTypeDef res;
    for (uint8_t i = 1; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(&hi2c3, (i << 1), 1, 10);
        if (res == HAL_OK) {
            printf("I2C device found at 0x%X\r\n", i << 1);
        }
    }
#endif

    LCD_Init();
    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_Send_String(" LCD Ready ");

    while (1)
    {
        if (xQueueReceive(xTempQueue, &temperature_c, portMAX_DELAY) == pdPASS)
        {

            // Prepare LCD message
            snprintf(lcdMsg.line1, 16, "Temp: %.1f C", temperature_c);
            snprintf(lcdMsg.line2, 16, "TempQueue: OK");

            LCD_Clear();
            LCD_Set_Cursor(0, 0);
            LCD_Send_String(lcdMsg.line1);
            LCD_Set_Cursor(1, 0);
            LCD_Send_String(lcdMsg.line2);
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
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
    uint8_t high_nibble = (data & 0xF0) | REGISTER_SEL | BACKLIGHT;
    uint8_t low_nibble  = ((data << 4) & 0xF0) | REGISTER_SEL | BACKLIGHT;
    LCD_Send_4Bits(high_nibble | BACKLIGHT);
    LCD_Send_4Bits(low_nibble | BACKLIGHT);
}

static void LCD_Send_4Bits(uint8_t data)
{
    uint8_t data_t[1];

    data_t[0] = data | LCD_ENABLE;
    HAL_I2C_Master_Transmit(&hi2c3, LCD_ADDR, data_t, 1, LCD_I2C_TIMEOUT);
    vTaskDelay(pdMS_TO_TICKS(1));  // Small delay

    LCD_Enable_Pulse(data);
}

static void LCD_Enable_Pulse(uint8_t data)
{
    uint8_t data_t[1];
    data_t[0] = data & ~LCD_ENABLE;
    HAL_I2C_Master_Transmit(&hi2c3, LCD_ADDR, data_t, 1, LCD_I2C_TIMEOUT);
}

/******************************************************************************
*                            EOF
******************************************************************************/
