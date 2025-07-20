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


/******************************************************************************
*							API DECLARATIONS
******************************************************************************/

/* Lcd16x2_Handler */
void Lcd16x2_Handler(void *params);

/******************************************************************************
*							EOF
******************************************************************************/

#endif /* LCD16X2_H_ */
