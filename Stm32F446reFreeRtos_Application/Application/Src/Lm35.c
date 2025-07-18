/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Lm35.c
  * @brief          : Lm35 Temperature sensor Handler
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
#include "Lm35.h"
#include "App.h"
/******************************************************************************
*							GLOBAL VARIABLES
******************************************************************************/
/* ADC Handler structure */
extern ADC_HandleTypeDef hadc1;
extern QueueHandle_t xLedModeQueue;
extern QueueHandle_t xLcdQueue;

// Static global structure (private to lm35.c only)
static LM35_Data_t lm35_data = {
    .adc_raw = 0,
    .temperature_c = 0.0f,
    .adc_timeout_error = false,
    .sensor_disconnected = false
};

/******************************************************************************
*							LOCAL FUNCTION DECLARATIONS
******************************************************************************/

/******************************************************************************
*							CONST DECLARATIONS
******************************************************************************/


/******************************************************************************
*							API IMPLEMENTATION
******************************************************************************/
void LM35_Handler(void *pvParameters)
{
	LedMode_t mode;
	LcdMessage_t lcd_msg;
	char TempString[10]={""};

    while(1)
    {
        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, LM35_ADC_TIMEOUT) == HAL_OK)
        {
            lm35_data.adc_raw = HAL_ADC_GetValue(&hadc1);
            lm35_data.adc_timeout_error = false;

            if (lm35_data.adc_raw < LM35_DISCONNECT_ADC)
            {
                lm35_data.sensor_disconnected = true;
                lm35_data.temperature_c = -100.0f;
            }
            else
            {
                lm35_data.sensor_disconnected = false;
                lm35_data.temperature_c = ((float)(lm35_data.adc_raw) * 3.3f * 100.0f) / 4095.0f;
            }
        }
        else
        {
            lm35_data.adc_timeout_error = true;
        }


        if (lm35_data.sensor_disconnected)
        {
            mode = LED_MODE_SENSOR_FAIL;
        }
        else if (lm35_data.adc_timeout_error)
        {
            mode = LED_MODE_ADC_ERROR;
        }
        else
        {
            mode = LED_MODE_NORMAL;
        }

        /* Update the xLedModeQueue with Mode to enable the LED pattern */
        xQueueSend(xLedModeQueue, &mode, 0);
        HAL_ADC_Stop(&hadc1);
        vTaskDelay(LM35_SAMPLING_DELAY);

        strncpy(lcd_msg.line1, "Temp: ", 16);
        sprintf(TempString, "%f", lm35_data.temperature_c);
        strncat(lcd_msg.line1,TempString,strlen(TempString));

        strncpy(lcd_msg.line2, "Sensor OK", 16);
        xQueueSend(xLcdQueue, &lcd_msg, 0);
        vTaskDelay(LM35_SAMPLING_DELAY);
    }
}

//  Read-only pointer to structure
const LM35_Data_t* LM35_GetData(void)
{
    return &lm35_data;
}

/******************************************************************************
*							LOCAL FUNCTION DEFINITIONS
******************************************************************************/


/******************************************************************************
*							EOF
******************************************************************************/

