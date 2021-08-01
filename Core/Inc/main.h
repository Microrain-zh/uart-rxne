/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define KEY_2_Pin GPIO_PIN_2
#define KEY_2_GPIO_Port GPIOE
#define KEY_1_Pin GPIO_PIN_3
#define KEY_1_GPIO_Port GPIOE
#define KEY_0_Pin GPIO_PIN_4
#define KEY_0_GPIO_Port GPIOE
#define T_CS_Pin GPIO_PIN_13
#define T_CS_GPIO_Port GPIOC
#define ISO_EN1_Pin GPIO_PIN_0
#define ISO_EN1_GPIO_Port GPIOC
#define T_SCL_Pin GPIO_PIN_0
#define T_SCL_GPIO_Port GPIOB
#define T_PEN_Pin GPIO_PIN_1
#define T_PEN_GPIO_Port GPIOB
#define T_SDA_Pin GPIO_PIN_11
#define T_SDA_GPIO_Port GPIOF
#define PK_485_Pin GPIO_PIN_12
#define PK_485_GPIO_Port GPIOB
#define F_CS_Pin GPIO_PIN_14
#define F_CS_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB
#define RS485_CTR_Pin GPIO_PIN_8
#define RS485_CTR_GPIO_Port GPIOG
#define BEEP_Pin GPIO_PIN_11
#define BEEP_GPIO_Port GPIOA
#define DCMI_SCL_Pin GPIO_PIN_6
#define DCMI_SCL_GPIO_Port GPIOD
#define DCMI_SDA_Pin GPIO_PIN_7
#define DCMI_SDA_GPIO_Port GPIOD
#define DCMI_PWDN_Pin GPIO_PIN_9
#define DCMI_PWDN_GPIO_Port GPIOG
#define DCMI_RESET_Pin GPIO_PIN_15
#define DCMI_RESET_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
