/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include <stdint.h>
#include "SimpleModbusSlave.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
//=======================================RELAY===========================================

#define CLUTCH_PORT 			 GPIOC
#define CLUTCH_PIN 				 GPIO_PIN_2

#define PRE_VALVE_PORT 			 GPIOC
#define PRE_VALVE_PIN  			 GPIO_PIN_3

#define COA_VALVE_PORT 	 		 GPIOC
#define COA_VALVE_PIN  			 GPIO_PIN_0
//======================================BTN================================================
#define BTN_MANUAL_PORT			 GPIOA
#define BTN_MANUAL_PIN			 GPIO_PIN_0

#define BTN_AZIMUTH_UP_PORT		 GPIOA
#define BTN_AZIMUTH_UP_PIN	 	 GPIO_PIN_1

#define BTN_AZIMUTH_DOWN_PORT	 GPIOA
#define BTN_AZIMUTH_DOWN_PIN	 GPIO_PIN_2

#define BTN_ELEVATION_UP_PORT	 GPIOA
#define BTN_ELEVATION_UP_PIN	 GPIO_PIN_3

#define BTN_ELEVATION_DOWN_PORT	 GPIOA
#define BTN_ELEVATION_DOWN_PIN	 GPIO_PIN_4
//=======================================RELAY_OUT==========================================


#define CLUTCH_RELAY_ON      			HAL_GPIO_WritePin(CLUTCH_PORT, CLUTCH_PIN, GPIO_PIN_SET)
#define CLUTCH_RELAY_OFF      			HAL_GPIO_WritePin(CLUTCH_PORT, CLUTCH_PIN, GPIO_PIN_RESET)

#define PRE_VALVE_RELAY_ON      		HAL_GPIO_WritePin(PRE_VALVE_PORT, PRE_VALVE_PIN, GPIO_PIN_SET)
#define PRE_VALVE_RELAY_OFF     		HAL_GPIO_WritePin(PRE_VALVE_PORT, PRE_VALVE_PIN, GPIO_PIN_RESET)

#define COA_VALVE_RELAY_ON      	    HAL_GPIO_WritePin(COA_VALVE_PORT, COA_VALVE_PIN, GPIO_PIN_SET)
#define COA_VALVE_RELAY_OFF     		HAL_GPIO_WritePin(COA_VALVE_PORT, COA_VALVE_PIN, GPIO_PIN_RESET)
//=====================================BTN_IN================================================

#define BTN_MANUAL_READ					!HAL_GPIO_ReadPin(BTN_MANUAL_PORT, BTN_MANUAL_PIN)
#define BTN_ELEVATION_UP_READ			!HAL_GPIO_ReadPin(BTN_ELEVATION_UP_PORT, BTN_ELEVATION_UP_PIN)
#define BTN_ELEVATION_DOWN_READ  		!HAL_GPIO_ReadPin(BTN_ELEVATION_DOWN_PORT, BTN_ELEVATION_DOWN_PIN)
#define BTN_AZIMUTH_UP_READ 			!HAL_GPIO_ReadPin(BTN_AZIMUTH_UP_PORT, BTN_AZIMUTH_UP_PIN)
#define BTN_AZIMUTH_DOWN_READ 			!HAL_GPIO_ReadPin(BTN_AZIMUTH_DOWN_PORT, BTN_AZIMUTH_DOWN_PIN)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
