/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Button.h
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
  *
  * History: v01
  * 	08-01-2026	-	v01	- Initial version
  *
  *
  *
  *
  ******************************************************************************
  */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/******************************************************************************
* MACRO DEFINITIONS
******************************************************************************/
#define BUTTON_DEBOUNCE_MS        50
#define BUTTON_SHORT_PRESS_MS    500
#define BUTTON_LONG_PRESS_MS     2000
#define BUTTON_DOUBLE_GAP_MS     300

/******************************************************************************
* DATA TYPES
******************************************************************************/
typedef enum
{
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_DOUBLE_PRESS,
    BUTTON_EVENT_LONG_PRESS
} ButtonEvent_t;

/******************************************************************************
* API DECLARATIONS
******************************************************************************/
void Button_Init(void);
void Button_Task(void *params);

/* Called from EXTI ISR */
void Button_ISR_Callback(void);

#endif /* INC_BUTTON_H_ */
