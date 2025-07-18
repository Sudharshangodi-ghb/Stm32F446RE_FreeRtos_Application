/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Led.h
  * @brief          : Header for led.c file.
  *                   This file contains the common defines of the application.
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
  * 	18-07-2025	-	v01	- Initial version
  *
  *
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef LCD16X2_H_
#define LCD16X2_H_


/******************************************************************************
*							INCLUDES
******************************************************************************/

/******************************************************************************
*							MACRO DEFINITION
******************************************************************************/

/******************************************************************************
*							DATA TYPE DECLARATION
******************************************************************************/
typedef struct {
    char line1[17];
    char line2[17];
} LcdMessage_t;


#define LCD_ADDR       0x4E  // Change to 0x4E or 0x27 based on your module (<<1 from 0x27)
#define BACKLIGHT      0x08
#define LCD_ENABLE     0x04
#define READ_WRITE     0x02
#define REGISTER_SEL   0x01


/******************************************************************************
*							API DECLARATIONS
******************************************************************************/

/* Lcd16x2_Handler */
void Lcd16x2_Handler(void *params);

/******************************************************************************
*							EOF
******************************************************************************/

#endif /* LCD16X2_H_ */
